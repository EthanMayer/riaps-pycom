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

# Import Riaps Libraries
from riaps.utils import spdlog_setup
import spdlog
from riaps.run.dc import Coordinator,Group

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

class CythonComponentThread(threading.Thread):

    def __init__(self, parent):
        threading.Thread.__init__(self,daemon=False)
        # self.name = parent.name
        # self.parent = parent
        # self.context = parent.context
        # self.instance = parent.instance
        # self.schedulerType = parent.scheduler
        # self.control = None

    #def setupControl():
        # Setup socket to connect to testPart.py

    #def setupSockets():

    def launchThread(self):
        cdef pthread_t t1    # Thread 1's ID
        portDict = {}   # Dictionary for ports

        # Create main thread pair socket and bind to IP
        cdef void* ctx = z.zmq_ctx_new()
        cdef void* sckt = z.zmq_socket(ctx, z.ZMQ_PAIR)

        if (z.zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0):
            error("Could not bind comp.pyx PAIR socket address")

        # Store socket in dictionary
        store_sckt("Thread1", sckt, portDict)

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
        cdef char buf[6]
        if (z.zmq_recvbuf(get_sckt("Thread1", portDict), buf, sizeof(buf), 0) == -1):
            error("Could not receive on comp.pyx PAIR socket")
        print("Cython: received " + str(buf))

        # Send build message to thread
        cdef char* message = 'Build'
        if (z.zmq_sendbuf(get_sckt("Thread1", portDict), message, sizeof(message), 0) != sizeof(message)):
            error("Comp.pyx PAIR send build message length incorrect")
        print("Cython: Sent message: " + str(message))

        # Clean up socket
        if (z.zmq_close(get_sckt("Thread1", portDict)) == -1):
            error("Could not close comp.pyx PAIR socket")
        if (z.zmq_ctx_destroy(ctx) == -1):
            error("Could not destroy comp.pyx ZMQ context")

        for i in range(5):
            print("Cython Main Thread: Test")
            time.sleep(1)

        # Join thread
        print("Cython: Joining thread")
        if (pthread_join(t1, NULL) == -1):
            error("Could not join thread1 in comp.pyx")

        for i in range(5):
            print("Cython Main Thread: Test2")
            time.sleep(1)

    def run(self):
        # self.setupControl()
        # self.setupSockets()
        # self.setupPoller()
        # self.setupScheduler()
        self.launchThread()

class CythonComponent(object):
    '''
    Base class for RIAPS application components
    '''
    GROUP_PRIORITY_MAX = 0  # Priority 0 means highest priority
    GROUP_PRIORITY_MIN = 256  # Priority 256 means lowest priority (>= 256 port indices are unexpected)
    
    def __init__(self):
        '''
        Constructor
       '''
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
        loggerName = self.owner.getActorName() + '.' + self.owner.getName()
        self.logger = spdlog_setup.get_logger(loggerName)
        if self.logger == None:
            self.logger = spdlog.ConsoleLogger(loggerName, True, True, False)
            self.logger.set_pattern(spdlog_setup.global_pattern)
        # print  ( "Component() : '%s'" % self )
        self.coord = Coordinator(self)
        self.thread = None
 
    def getName(self):
        '''
        Return the name of the component (as in model)
        '''
        return self.owner.getName()
    
    def getTypeName(self):
        '''
        Return the name of the type of the component (as in model) 
        '''
        return self.owner.getTypeName()
    
    def getLocalID(self):
        '''
        Return a locally unique ID (int) of the component. The ID is unique within the actor.
        '''
        return id(self)

    def getActorName(self):
        '''
        Return the name of the parent actor (as in model)
        '''
        return self.owner.getActorName()
    
    def getAppName(self):
        '''
        Return the name of the parent application (as in model)
        '''
        return self.owner.getAppName()
    
    def getActorID(self):
        '''
        Return a globally unique ID (8 bytes) for the parent actor. 
        '''
        return self.owner.getActorID()
    
    def getUUID(self):
        '''
        Return the network unique ID for the parent actor
        '''
        return self.owner.getUUID()
    
    def handleActivate(self):
        '''
        Default activation handler
        '''
        pass
    
    def handleDeactivate(self):
        '''
        Default deactivation handler
        '''
        pass
    
    def handlePassivate(self):
        '''
        Default activation handler
        '''
        pass
    
    def handleCPULimit(self):
        ''' 
        Default handler for CPU limit exceed
        '''
        pass
    
    def handleMemLimit(self):
        ''' 
        Default handler for memory limit exceed
        '''
        pass
    
    def handleSpcLimit(self):
        ''' 
        Default handler for space limit exceed
        '''
        pass
        
    def handleNetLimit(self):
        ''' 
        Default handler for space limit exceed
        '''
        pass
    
    def handleNICStateChange(self, state):
        ''' 
        Default handler for NIC state change
        '''
        pass
    
    def handlePeerStateChange(self, state, uuid):
        ''' 
        Default handler for peer state change
        '''
        pass
    
    def handleDeadline(self, _funcName):
        '''
        Default handler for deadline violation
        '''
        pass
    
    def handleGroupMessage(self, _group):
        '''
        Default handler for group messages.
        Implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    def handleVoteRequest(self, group, rfvId):
        '''
        Default handler for vote requests (in member)
        Implementation must recv/recv_pyobj to obtain the topic. 
        '''
        pass
    
    def handleVoteResult(self, group, rfvId, vote):
        '''
        Default handler for the result of a vote (in member)
        '''
        pass
    
    def handleActionVoteRequest(self, group, rfvId, when):
        '''
        Default handler for request to vote an action in the future (in member)
        Implementation must recv/recv_pyobj to obtain the action topic. 
        '''
        pass
        
    def handleMessageToLeader(self, group):
        '''
        Default handler for messages sent to the leader (in leader)
        Leader implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    def handleMessageFromLeader(self, group):
        '''
        Default handler for messages received from the leader (in member) 
        Member implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        '''
        pass
    
    def handleMemberJoined(self, group, memberId):
        '''
        Default handler for 'member join' events
        '''  
        pass
    
    def handleMemberLeft(self, group, memberId):
        '''
        Default handler for 'member leave' events
        '''          
        pass
    
    def handleLeaderElected(self, group, leaderId):
        '''
        Default handler for 'leader elected' events
        '''  
        pass
    
    def handleLeaderExited(self, group, leaderId):
        '''
        Default handler for 'leader exited' events
        '''  
        pass
    
    def joinGroup(self, groupName, instName, groupPriority=GROUP_PRIORITY_MIN):
        if self.thread == None:
            self.thread = self.owner.thread
        group = self.coord.getGroup(groupName, instName)
        if group == None:
            group = self.coord.joinGroup(self.thread, groupName, instName, self.getLocalID())
            self.thread.addGroupSocket(group, groupPriority)
        return group
            
    def leaveGroup(self,group):
        assert type(group) == Group
        if self.thread == None:
            self.thread = self.owner.thread
        self.thread.delGroupSocket(group)
        self.coord.leaveGroup(group)
        return True