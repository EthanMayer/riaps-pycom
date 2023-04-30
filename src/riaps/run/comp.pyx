'''
Cython Component class
Created on Feb 15, 2023

@author: EthanMayer
'''

# Import C libraries
from libc.stdlib cimport malloc, free, atoi
from posix.dlfcn cimport dlopen, dlsym, RTLD_LAZY
from libc.string cimport strerror
from libc.errno cimport errno
from libc.stdio cimport sprintf
from cpython.pycapsule cimport PyCapsule_New, PyCapsule_IsValid, PyCapsule_GetPointer, PyCapsule_GetName
from zmq.backend.cython cimport libzmq as z
# Import project's pthread .pxd header file
from pthread cimport pthread_create, pthread_join, pthread_t

# Import Python libraries
import threading
import time
import zmq
import logging
import traceback
import heapq
import itertools
from collections import deque

# Import Riaps Libraries
from riaps.utils import spdlog_setup
import spdlog
from riaps.run.dc import Coordinator,Group
from riaps.run.exc import BuildError

# Error handling
cpdef error(msg):
    print(msg + ": " + strerror(errno))
    exit(1)

# Destructor for pointers stored in PyCapsule (only needed for dynamically allocated pointers stored as PyCapsules)
cdef void free_ptr(object cap):
    free(PyCapsule_GetPointer(cap,PyCapsule_GetName(cap)))

# Wrap a void* socket pointer in a PyCapsule and store in a Python dictionary
cdef void store_sckt(const char* name, void* sckt, portDict):
    portDict[name] = PyCapsule_New(sckt, name, NULL)

# Unrwap a void* socket pointer stored in a PyCapsule that is stored in a Python dictionary
cdef void* get_sckt(const char* name, portDict):
    # First check to ensure capsule is valid
    if (PyCapsule_IsValid(portDict[name], name)):
        return PyCapsule_GetPointer(portDict[name], name)
    else:
        error("Could not retrieve socket pointer from dictionary in comp.pyx")

cdef class CythonComponentThread():
    '''
    Cython Component execution thread. Runs the component's code, and communicates with the parent actor.
    '''

    # All cython class attributes must be declared at compile time
    cdef object logger
    cdef str name
    cdef object parent
    cdef object context
    cdef object instance
    cdef object schedulerType
    cdef object control

    def __init__(self, parent):
        # threading.Thread.__init__(self,daemon=False)
        self.logger = logging.getLogger(__name__)
        self.name = parent.name
        self.parent = parent
        self.context = parent.context
        self.instance = parent.instance
        self.schedulerType = parent.scheduler
        self.control = None

    cpdef setupControl(self):
        '''
        Create the control socket and connect it to the socket in the parent part
        '''
        self.control = self.context.socket(zmq.PAIR)
        #self.control.connect('inproc://part_' + self.name + '_control')
        pass

    cpdef sendControl(self, msg):
        assert self.control != None
        self.control.send_pyobj(msg)

    cpdef setupSockets(self):
        print("Cython: Awaiting build")
        # Create main thread pair socket and bind to IP
        cdef void* ctx = z.zmq_ctx_new()
        cdef void* sckt = z.zmq_socket(ctx, z.ZMQ_PAIR)

        if (z.zmq_bind(sckt, "tcp://part_Test_Part_control:11111") != 0):
            pass #error("Could not bind comp.pyx PAIR socket address")

        cdef char buf[6]
        z.zmq_recvbuf(sckt, buf, sizeof(buf), 0)
            #pass #error("Could not receive on comp.pyx PAIR socket")
        #print("Cython: received " + str(buf))

        
        #msg = self.control.recv()
        print("Received build")
        msg = "Not build"
        if msg != "build":
            raise BuildError('setupSockets: invalid msg: %s' % str(msg)) 
        for portName in self.parent.ports:
            res = self.parent.ports[portName].setupSocket(self)
            portKind = res.portKind
            if portKind in {'tim','ins'}:
                continue
            elif portKind in {'pub', 'sub', \
                              'clt', 'srv', \
                              'req', 'rep', \
                              'qry', 'ans'}:
                self.control.send_pyobj(res)
            else:
                raise BuildError('setupSockets: invalid portKind: %s' % str(portKind))
        self.control.send_pyobj("done")

    cpdef setupPoller(self):
        self.poller = zmq.Poller()
        self.sock2NameMap = {}
        self.sock2PortMap = {}
        self.sock2GroupMap = {}
        self.portName2GroupMap = {}
        self.poller.register(self.control, zmq.POLLIN)
        self.sock2NameMap[self.control] = ""
        self.sock2PrioMap = {}
        for portName in self.parent.ports:
            portObj = self.parent.ports[portName]
            portSocket = portObj.getSocket()
            portIsInput = portObj.inSocket()
            if portSocket != None:
                self.sock2PortMap[portSocket] = portObj
                if portIsInput:
                    self.poller.register(portSocket, zmq.POLLIN)
                    self.sock2NameMap[portSocket] = portName
                    self.sock2PrioMap[portSocket] = portObj.getIndex()
    
    cpdef replaceSocket(self, portObj, newSocket):
        portName = portObj.name
        oldSocket = portObj.getSocket() 
        del self.sock2PortMap[oldSocket]
        if portObj.inSocket():
            self.poller.register(oldSocket, 0)
            del self.sock2NameMap[oldSocket]
            del self.sock2PrioMap[oldSocket]
        oldSocket.close()
        self.sock2PortMap[newSocket] = portObj
        if portObj.inSocket():
            self.poller.register(newSocket, zmq.POLLIN)
            self.sock2NameMap[newSocket] = portName
            self.sock2PrioMap[newSocket] = portObj.getIndex()

    cpdef addGroupSocket(self, group, groupPriority):
        groupSocket = group.getSocket()
        groupId = group.getGroupName()
        self.poller.register(groupSocket, zmq.POLLIN)
        self.sock2GroupMap[groupSocket] = group
        self.portName2GroupMap[groupId] = group
        self.sock2PrioMap[groupSocket] = groupPriority

    cpdef delGroupSocket(self, group):
        groupSocket = group.getSocket()
        groupId = group.getGroupName()
        self.poller.unregister(groupSocket)
        del self.sock2GroupMap[groupSocket]
        del self.portName2GroupMap[groupId]
        del self.sock2PrioMap[groupSocket]

    cpdef runCommand(self):
        res = False
        msg = self.control.recv_pyobj()
        if msg == "kill":
            self.logger.info("kill")
            res = True
        elif msg == "activate":
            self.logger.info("activate")
            for portName in self.parent.ports:
                self.parent.ports[portName].activate()
            self.instance.handleActivate()  
        elif msg == "deactivate":
            self.logger.info("deactivate")
            self.instance.handleDeactivate()   
            for portName in self.parent.ports:
                self.parent.ports[portName].deactivate()
        elif msg == "passivate":
            self.logger.info("passivate")
            self.instance.handlePassivate()                
        else: 
            cmd = msg[0]
            if cmd == "portUpdate":
                self.logger.info("portUpdate: %s" % str(msg))
                (_ignore, portName, host, port) = msg
                ports = self.parent.ports
                groups = self.portName2GroupMap
                if portName in ports:
                    portObj = ports[portName]
                    res = portObj.update(host, port)
                elif portName in groups:
                    groupObj = groups[portName]
                    res = groupObj.update(host, port)
                else:
                    pass
                # self.control.send_pyobj("ok")
            elif cmd == "groupUpdate":
                # handle it in coordinator
                pass
            elif cmd == "limitCPU":
                self.logger.info("limitCPU")
                self.instance.handleCPULimit()
                # self.control.send_pyobj("ok")
            elif cmd == "limitMem":
                self.logger.info("limitMem")
                self.instance.handleMemLimit()
                # self.control.send_pyobj("ok")
            elif cmd == "limitSpc":
                self.logger.info("limitSpc")
                self.instance.handleSpcLimit()
                # self.control.send_pyobj("ok")
            elif cmd == "limitNet":
                self.logger.info("limitNet")
                self.instance.handleNetLimit()
                # self.control.send_pyobj("ok")
            elif cmd == "nicState":
                state = msg[1]
                self.logger.info("nicState %s" % state)
                self.instance.handleNICStateChange(state)
                # self.control.send_pyobj("ok")
            elif cmd == "peerState":
                state, uuid = msg[1], msg[2]
                self.logger.info("peerState %s at %s" % (state, uuid))
                self.instance.handlePeerStateChange(state, uuid)
                # self.control.send_pyobj("ok")
            else:
                self.logger.info("unknown command %s" % cmd)
                pass  # Should report an error
        return res

    cpdef getInfo(self):
        info = []
        for (_portName, portObj) in self.parent.ports:
            res = portObj.getInfo()
            info.append(res)
        return info

    cpdef logEvent(self, msg):
        self.control.send_pyobj(msg)
    
    cpdef executeHandlerFor(self, socket):
        '''
        Execute the handler for the socket
        
        The handler is always allowed to run to completion, the operation is never preempted. 
        '''
        if socket in self.sock2PortMap:
            portName = self.sock2NameMap[socket]
            portObj = self.sock2PortMap[socket]
            deadline = portObj.getDeadline()
            try:
                funcName = 'on_' + portName
                func_ = getattr(self.instance, funcName)
                if deadline != 0:
                    start = time.perf_counter()
                func_()
                if deadline != 0:
                    finish = time.perf_counter()
                    spent = finish - start
                    if spent > deadline:
                        self.logger.error('Deadline violation in %s.%s()' 
                                          % (self.name, funcName))
                        msg = ('deadline',)
                        self.control.send_pyobj(msg)
                        self.instance.handleDeadline(funcName)
            except:
                traceback.print_exc()
                msg = ('exception', traceback.format_exc())
                self.control.send_pyobj(msg)
        elif socket in self.sock2GroupMap:
            group = self.sock2GroupMap[socket]
            try:
                group.handleMessage()
            except:
                traceback.print_exc()
                msg = ('exception', traceback.format_exc())
                self.control.send_pyobj(msg)
        else:
            self.logger.error('Unbound port')

    cpdef batchScheduler(self, sockets):
        '''
        Batch scheduler for the component message processing.
        
        The dictionary containing the active sockets is scanned and the associated handler is invoked. 
        
        '''
        for socket in sockets:
            self.executeHandlerFor(socket)

    cpdef rrScheduler(self, sockets):
        '''
        Round-robin scheduler for the component message processing. 
        
        The round-robin order is determined by the order of component ports. The dictionary of active sockets is scanned, and the \
        associated handlers are invoked in round-robin order. After each invocation, the inputs are polled (in a no-wait operation) \
        and the round-robin queue is updated. 
        '''
        while True:
            jobs = []
            for socket in sockets:
                if socket in self.sock2PortMap:
                    tag = self.sock2PrioMap[socket]
                elif socket in self.sock2GroupMap:
                    tag = self.sock2PrioMap[socket]  # TODO: better solution for group message priority  
                jobs += [(tag, socket)]
            jobs = sorted(jobs)  # Sort jobs by tag
            if len(jobs) != 1:  # More than one job
                if len(self.dq):  # If deque is not empty
                    # Find jobs whose tag is larger than the last executed tag 
                    larger = [ i for i, job in enumerate(jobs) if job[0] > self.last]
                    if len(larger):  # There is at least one such job
                        first = larger[0]  # Shuffle job list to keep rr- order
                        jobs = jobs[-first:] + jobs[0:first]
                else:
                    self.last = jobs[-1][0]  # Tag of last job to be added to deque
            self.dq.extendleft(jobs)  # Add jobs to deque   
            sockets = {}
            while True:
                try:
                    tag, socket = self.dq.pop()
                    self.last = self.dq[0][0] if len(self.dq) > 0 else tag
                    self.executeHandlerFor(socket)
                    if len(self.dq) == 0: return  # Empty queue, return               
                    sockets = dict(self.poller.poll(None))  # Check if something came in
                    if sockets:
                        if self.control in sockets:  # Handle control message
                            self.toStop = self.runCommand()
                            del sockets[self.control]
                            if self.toStop: return  # Return if we must stop
                            if len(sockets):  # More sockets to handle
                                break  #  break from inner loop to schedule tasks
                    else:  # Nothing came in
                        continue  #  keep running inner loop
                except IndexError:  # Queue empty, return
                    print('indexError')
                    return

    cpdef priorityScheduler(self, sockets):
        '''
        priority scheduler for the component message processing. 
        
        The priority order is determined by the order of component ports. The dictionary of active sockets is scanned, and the \
        they are inserted into a priority queue (according to their priority value). The queue is processed (in order of \
        priority). After each invocation, the inputs are polled (in a no-wait operation) and the priority queue is updated. 
        '''
        while True:
            for socket in sockets:
                if socket in self.sock2PortMap:
                    pri = self.sock2PrioMap[socket]
                elif socket in self.sock2GroupMap:
                    pri = self.sock2PrioMap[socket]  # TODO: better solution for group message priority  
                cnt = next(self.tc)
                entry = (pri, cnt, socket)
                heapq.heappush(self.pq, entry)
            sockets = {}
            while True:
                try:
                    pri, cnt, socket = heapq.heappop(self.pq)  # Execute one task
                    self.executeHandlerFor(socket)
                    if len(self.pq) == 0:  # Empty queue, return
                        return
                    sockets = dict(self.poller.poll(None))  # Poll to check if something came in
                    if sockets:
                        if self.control in sockets:  # Handle control message
                            self.toStop = self.runCommand()
                            del sockets[self.control]
                            if self.toStop: return  # Return if we must stop
                        if len(sockets):  # More sockets to handle,
                            break  #  break from inner loop to schedule tasks
                    else:  # Nothing came in
                        continue  #  keep running inner loop
                except IndexError:  # Queue empty, return
                    return

    cpdef setupScheduler(self):
        '''
        Select the message scheduler algorithm based on the model. 
        '''
        if self.schedulerType == 'default':
            self.scheduler = self.batchScheduler
        elif self.schedulerType == 'priority':
            self.scheduler = self.priorityScheduler
            self.pq = []
            self.tc = itertools.count()
        elif self.schedulerType == 'rr':
            self.scheduler = self.rrScheduler
            self.dq = deque()
            self.last = -1
        else:
            self.logger.error('Unknown scheduler type: %r' % self.schedulerType)
            self.scheduler = None

    # Renamed from run -> start since this class is no longer a Thread object
    cpdef start(self):
        self.launchThread()
        self.setupControl()
        self.setupSockets()
        self.setupPoller()
        self.setupScheduler()
        if self.scheduler:
            self.toStop = False
            
            #while True:
            #    sockets = dict(self.poller.poll())
            #    if self.control in sockets:
            #        self.toStop = self.runCommand()
            #        del sockets[self.control]
            #    if self.toStop: break
            #    if len(sockets) > 0: self.scheduler(sockets)
            #    if self.toStop: break
        self.logger.info("stopping")
        if hasattr(self.instance, '__destroy__'):
            destroy_ = getattr(self.instance, '__destroy__')
            destroy_()
        self.logger.info("stopped")
        
        #self.launchThread() # custom

    cpdef launchThread(self):
        cdef pthread_t t1    # Thread 1's ID
        #portDict = {}   # Dictionary for ports

        # Create main thread pair socket and bind to IP
        #cdef void* ctx = z.zmq_ctx_new()
        #cdef void* sckt = z.zmq_socket(ctx, z.ZMQ_PAIR)

        #if (z.zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0):
        #    error("Could not bind comp.pyx PAIR socket address")

        # Store socket in dictionary
        #store_sckt("Thread1", sckt, portDict)

        # Open .so shared library and grab function from it
        cdef char* libpath = "TEST_funcBody.so";
        cdef void* libhandle = dlopen(libpath, RTLD_LAZY);

        if (libhandle == NULL):
            error("Could not open shared library in comp.pyx")

        # Create thread with the .so function as body
        cdef void *thread1 = dlsym(libhandle, "test")

        if (pthread_create(&t1, NULL, thread1, NULL) == -1):
            error("Could not create thread in comp.pyx")

        # Receive "Ready" message to know the thread is ready
        #cdef char buf[6]
        #if (z.zmq_recvbuf(get_sckt("Thread1", portDict), buf, sizeof(buf), 0) == -1):
        #    error("Could not receive on comp.pyx PAIR socket")
        #print("Cython: received " + str(buf))

        # Send build message to thread
        #cdef char* message = 'Build'
        #if (z.zmq_sendbuf(get_sckt("Thread1", portDict), message, sizeof(message), 0) != sizeof(message)):
        #    error("Comp.pyx PAIR send build message length incorrect")
        #print("Cython: Sent message: " + str(message))

        # Clean up socket
        #if (z.zmq_close(get_sckt("Thread1", portDict)) == -1):
        #    error("Could not close comp.pyx PAIR socket")
        #if (z.zmq_ctx_destroy(ctx) == -1):
        #    error("Could not destroy comp.pyx ZMQ context")

        #for i in range(5):
        #    print("Cython Main Thread: Test")
        #    time.sleep(1)

        # Join thread
        #print("Cython: Joining thread")
        #if (pthread_join(t1, NULL) == -1):
        #    error("Could not join thread1 in comp.pyx")

        #for i in range(5):
        #    print("Cython Main Thread: Test2")
        #    time.sleep(1)

cdef class CythonComponent(object):
    '''
    Base class for RIAPS Cython application components
    '''
    # All cython class attributes must be declared at compile time
    cdef int GROUP_PRIORITY_MAX
    cdef int GROUP_PRIORITY_MIN
    cdef object owner
    cdef object logger
    cdef object coord
    #cdef object thread

    def __init__(self):
        '''
        Constructor
       '''
        self.GROUP_PRIORITY_MAX = 0 # Priority 0 means highest priority
        self.GROUP_PRIORITY_MIN = 256 # Priority 256 means lowest priority (>= 256 port indices are unexpected)
        class_ = getattr(self, '__class__')
        className = getattr(class_, '__name__')
        self.owner = class_.OWNER  # This is set in the parent part (temporarily)
        #
        # Logger attributes
        # logger: logger for this class
        # loghandler: handler for the logger (defaults to a StreamHandler)
        # logformatter: formatter assigned to the handler (default: Level:Time:Process:Class:Message)
        # self.logger = logging.getLogger(className)
        # self.logger.setLevel(logging.INFO)
        # self.logger.propagate=False
        # self.loghandler = logging.StreamHandler()
        # self.loghandler.setLevel(logging.INFO)
        # self.logformatter = logging.Formatter('%(levelname)s:%(asctime)s:[%(process)d]:%(name)s:%(message)s')
        # self.loghandler.setFormatter(self.logformatter)
        # self.logger.addHandler(self.loghandler)
        #
        loggerName = "TestLogger" #self.owner.getActorName() + '.' + self.owner.getName()
        self.logger = spdlog_setup.get_logger(loggerName)
        if self.logger == None:
            self.logger = spdlog.ConsoleLogger(loggerName, True, True, False)
            self.logger.set_pattern(spdlog_setup.global_pattern)
        # print  ( "Component() : '%s'" % self )
        self.coord = Coordinator(self)
        #self.thread = None
 
    cpdef getName(self):
        '''
        Return the name of the component (as in model)
        '''
        return self.owner.getName()
    
    cpdef getTypeName(self):
        '''
        Return the name of the type of the component (as in model) 
        '''
        return self.owner.getTypeName()
    
    cpdef getLocalID(self):
        '''
        Return a locally unique ID (int) of the component. The ID is unique within the actor.
        '''
        return id(self)

    cpdef getActorName(self):
        '''
        Return the name of the parent actor (as in model)
        '''
        return self.owner.getActorName()
    
    cpdef getAppName(self):
        '''
        Return the name of the parent application (as in model)
        '''
        return self.owner.getAppName()
    
    cpdef getActorID(self):
        '''
        Return a globally unique ID (8 bytes) for the parent actor. 
        '''
        return self.owner.getActorID()
    
    cpdef getUUID(self):
        '''
        Return the network unique ID for the parent actor
        '''
        return self.owner.getUUID()
    
    cpdef handleActivate(self):
        '''
        Default activation handler
        '''
        pass
    
    cpdef handleDeactivate(self):
        '''
        Default deactivation handler
        '''
        pass
    
    cpdef handlePassivate(self):
        '''
        Default activation handler
        '''
        pass
    
    cpdef handleCPULimit(self):
        ''' 
        Default handler for CPU limit exceed
        '''
        pass
    
    cpdef handleMemLimit(self):
        ''' 
        Default handler for memory limit exceed
        '''
        pass
    
    cpdef handleSpcLimit(self):
        ''' 
        Default handler for space limit exceed
        '''
        pass
        
    cpdef handleNetLimit(self):
        ''' 
        Default handler for space limit exceed
        '''
        pass
    
    cpdef handleNICStateChange(self, state):
        ''' 
        Default handler for NIC state change
        '''
        pass
    
    cpdef handlePeerStateChange(self, state, uuid):
        ''' 
        Default handler for peer state change
        '''
        pass
    
    cpdef handleDeadline(self, _funcName):
        '''
        Default handler for deadline violation
        '''
        pass
    
    cpdef handleGroupMessage(self, _group):
        '''
        Default handler for group messages.
        Implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    cpdef handleVoteRequest(self, group, rfvId):
        '''
        Default handler for vote requests (in member)
        Implementation must recv/recv_pyobj to obtain the topic. 
        '''
        pass
    
    cpdef handleVoteResult(self, group, rfvId, vote):
        '''
        Default handler for the result of a vote (in member)
        '''
        pass
    
    cpdef handleActionVoteRequest(self, group, rfvId, when):
        '''
        Default handler for request to vote an action in the future (in member)
        Implementation must recv/recv_pyobj to obtain the action topic. 
        '''
        pass
        
    cpdef handleMessageToLeader(self, group):
        '''
        Default handler for messages sent to the leader (in leader)
        Leader implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    cpdef handleMessageFromLeader(self, group):
        '''
        Default handler for messages received from the leader (in member) 
        Member implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    cpdef handleMemberJoined(self, group, memberId):
        '''
        Default handler for 'member join' events
        '''  
        pass
    
    cpdef handleMemberLeft(self, group, memberId):
        '''
        Default handler for 'member leave' events
        '''          
        pass
    
    cpdef handleLeaderElected(self, group, leaderId):
        '''
        Default handler for 'leader elected' events
        '''  
        pass
    
    cpdef handleLeaderExited(self, group, leaderId):
        '''
        Default handler for 'leader exited' events
        '''  
        pass
    
    cpdef joinGroup(self, groupName, instName, groupPriority):
        if groupPriority == None: # TODO: check if this is a valid replacement for default value from parameter list
            groupPriority = self.GROUP_PRIORITY_MIN
        if self.thread == None:
            self.thread = self.owner.thread
        group = self.coord.getGroup(groupName, instName)
        if group == None:
            group = self.coord.joinGroup(self.thread, groupName, instName, self.getLocalID())
            self.thread.addGroupSocket(group, groupPriority)
        return group
            
    cpdef leaveGroup(self,group):
        assert type(group) == Group
        if self.thread == None:
            self.thread = self.owner.thread
        self.thread.delGroupSocket(group)
        self.coord.leaveGroup(group)
        return True