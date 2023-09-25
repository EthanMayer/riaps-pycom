/*
C++ Component Base Class
Created on Sept. 24, 2023

@author: EthanMayer
*/



cdef class CythonComponent(void* object)
    /*
    Base class for RIAPS Cython application components
    */
    # All cython class attributes must be declared at compile time
    cdef int GROUP_PRIORITY_MAX
    cdef int GROUP_PRIORITY_MIN
    cdef object owner
    cdef object logger
    cdef object coord
    cdef dict __dict__ # catch-all for dynamic attributes assigned at runtime
    cdef object thread

    def __init__(self):
        /*
        Constructor
        */
        self.GROUP_PRIORITY_MAX = 0 # Priority 0 means highest priority
        self.GROUP_PRIORITY_MIN = 256 # Priority 256 means lowest priority (>= 256 port indices are unexpected)
        cdef type class_ = getattr(self, '__class__')
        cdef str className = getattr(class_, '__name__')
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
        cdef str loggerName = self.owner.getActorName() + '.' + self.owner.getName()
        self.logger = spdlog_setup.get_logger(loggerName)
        if self.logger == None:
            self.logger = spdlog.ConsoleLogger(loggerName, True, True, False)
            self.logger.set_pattern(spdlog_setup.global_pattern)
        # print  ( "Component() : '%s'" % self )
        self.coord = Coordinator(self)
        self.thread = None
 
    cpdef getName(self)
        /*
        Return the name of the component (as in model)
        */
        return self.owner.getName()
    
    cpdef getTypeName(self)
        /*
        Return the name of the type of the component (as in model) 
        */
        return self.owner.getTypeName()
    
    cpdef getLocalID(self)
        /*
        Return a locally unique ID (int) of the component. The ID is unique within the actor.
        */
        return id(self)

    cpdef getActorName(self)
        /*
        Return the name of the parent actor (as in model)
        */
        return self.owner.getActorName()
    
    cpdef getAppName(self)
        /*
        Return the name of the parent application (as in model)
        */
        return self.owner.getAppName()
    
    cpdef getActorID(self)
        /*
        Return a globally unique ID (8 bytes) for the parent actor. 
        */
        return self.owner.getActorID()
    
    cpdef getUUID(self)
        /*
        Return the network unique ID for the parent actor
        */
        return self.owner.getUUID()
    
    cpdef handleActivate(self)
        /*
        Default activation handler
        */
        pass
    
    cpdef handleDeactivate(self)
        /*
        Default deactivation handler
        */
        pass
    
    cpdef handlePassivate(self)
        /*
        Default activation handler
        */
        pass
    
    cpdef handleCPULimit(self)
        /* 
        Default handler for CPU limit exceed
        */
        pass
    
    cpdef handleMemLimit(self)
        /* 
        Default handler for memory limit exceed
        */
        pass
    
    cpdef handleSpcLimit(self)
        /* 
        Default handler for space limit exceed
        */
        pass
        
    cpdef handleNetLimit(self)
        /* 
        Default handler for space limit exceed
        */
        pass
    
    cpdef handleNICStateChange(self, state)
        /* 
        Default handler for NIC state change
        */
        pass
    
    cpdef handlePeerStateChange(self, state, uuid)
        /* 
        Default handler for peer state change
        */
        pass
    
    cpdef handleDeadline(self, _funcName)
        /*
        Default handler for deadline violation
        */
        pass
    
    cpdef handleGroupMessage(self, _group)
        /*
        Default handler for group messages.
        Implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        pass
    
    cpdef handleVoteRequest(self, group, rfvId)
        /*
        Default handler for vote requests (in member)
        Implementation must recv/recv_pyobj to obtain the topic. 
        */
        pass
    
    cpdef handleVoteResult(self, group, rfvId, vote)
        /*
        Default handler for the result of a vote (in member)
        */
        pass
    
    cpdef handleActionVoteRequest(self, group, rfvId, when)
        /*
        Default handler for request to vote an action in the future (in member)
        Implementation must recv/recv_pyobj to obtain the action topic. 
        */
        pass
        
    cpdef handleMessageToLeader(self, group)
        /*
        Default handler for messages sent to the leader (in leader)
        Leader implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        pass
    
    cpdef handleMessageFromLeader(self, group)
        /*
        Default handler for messages received from the leader (in member) 
        Member implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        pass
    
    cpdef handleMemberJoined(self, group, memberId)
        /*
        Default handler for 'member join' events
        */  
        pass
    
    cpdef handleMemberLeft(self, group, memberId)
        /*
        Default handler for 'member leave' events
        */          
        pass
    
    cpdef handleLeaderElected(self, group, leaderId)
        /*
        Default handler for 'leader elected' events
        */  
        pass
    
    cpdef handleLeaderExited(self, group, leaderId)
        /*
        Default handler for 'leader exited' events
        */  
        pass
    
    cpdef object joinGroup(self, groupName, instName, groupPriority):
        if groupPriority == None:
            groupPriority = self.GROUP_PRIORITY_MIN
        if self.thread == None:
            self.thread = self.owner.thread
        cdef object group = self.coord.getGroup(groupName, instName)
        if group == None:
            group = self.coord.joinGroup(self.thread, groupName, instName, self.getLocalID())
            self.thread.addGroupSocket(group, groupPriority)
        return group
            
    cpdef object leaveGroup(self,group):
        assert type(group) == Group
        if self.thread == None:
            self.thread = self.owner.thread
        self.thread.delGroupSocket(group)
        self.coord.leaveGroup(group)
        return True