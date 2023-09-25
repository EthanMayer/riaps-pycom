/*
C++ Component Base Class
Created on Sept. 24, 2023

@author: EthanMayer
*/

#include "comp.hpp"

class CppComponent {
    /*
    Base class for RIAPS Cython application components;
    */

    CppComponent(void* object) {
        /*
        Constructor
        */
        this->GROUP_PRIORITY_MAX = 0; // Priority 0 means highest priority
        this->GROUP_PRIORITY_MIN = 256; // Priority 256 means lowest priority (>= 256 port indices are unexpected)
        void* class_;// = getattr('__class__');
        std::string className;// = getattr(class_, '__name__');
        this->owner;// = class_.OWNER; // This is set in the parent part (temporarily)
        //
        // Logger attributes
        // logger: logger for this class
        // loghandler: handler for the logger (defaults to a StreamHandler)
        // logformatter: formatter assigned to the handler (default: Level:Time:Process:Class:Message)
        // this->logger = logging.getLogger(className);
        // this->logger.setLevel(logging.INFO);
        // this->logger.propagate=false;
        // this->loghandler = logging.StreamHandler();
        // this->loghandler.setLevel(logging.INFO);
        // this->logformatter = logging.Formatter('%(levelname)s:%(asctime)s:[%(process)d]:%(name)s:%(message)s');
        // this->loghandler.setFormatter(this->logformatter);
        // this->logger.addHandler(this->loghandler);
        //
        std::string loggerName = this->owner.getActorName() + '.' + this->owner.getName();
        this->logger = spdlog_setup.get_logger(loggerName);
        if (this->logger == NULL) {
            this->logger = spdlog.ConsoleLogger(loggerName, true, true, false);
            this->logger.set_pattern(spdlog_setup.global_pattern);
        }
        // print  ( "Component() : '%s'" % self );
        this->coord = NULL;//Coordinator(self);
        this->thread = NULL;
    }
 
    void getName() {
        /*;
        Return the name of the component (as in model);
        */
        return this->owner.getName();
    }
    
    void getTypeName() {
        /*;
        Return the name of the type of the component (as in model) ;
        */
        return this->owner.getTypeName();
    }
    
    void getLocalID() {
        /*;
        Return a locally unique ID (int) of the component. The ID is unique within the actor.;
        */
        return id(self);
    }

    void getActorName() {
        /*;
        Return the name of the parent actor (as in model);
        */
        return this->owner.getActorName();
    }
    
    void getAppName() {
        /*;
        Return the name of the parent application (as in model);
        */
        return this->owner.getAppName();
    }
    
    void getActorID() {
        /*;
        Return a globally unique ID (8 bytes) for the parent actor. ;
        */
        return this->owner.getActorID();
    }
    
    void getUUID() {
        /*;
        Return the network unique ID for the parent actor;
        */
        return this->owner.getUUID();
    }
    
    void handleActivate() {
        /*;
        Default activation handler;
        */
        ;
    }
    
    void handleDeactivate() {
        /*;
        Default deactivation handler;
        */
        ;
    }
    
    void handlePassivate() {
        /*;
        Default activation handler;
        */
        ;
    }
    
    void handleCPULimit() {
        /* ;
        Default handler for CPU limit exceed
        */
        ;
    }
    
    void handleMemLimit() {
        /* 
        Default handler for memory limit exceed
        */
        ;
    }
    
    void handleSpcLimit() {
        /* 
        Default handler for space limit exceed
        */
        ;
    }
        
    void handleNetLimit() {
        /* 
        Default handler for space limit exceed
        */
        ;
    }
    
    void handleNICStateChange(state) {
        /* 
        Default handler for NIC state change
        */
        ;
    }
    
    void handlePeerStateChange(state, uuid) {
        /* 
        Default handler for peer state change
        */
        ;
    }
    
    void handleDeadline(_funcName) {
        /*
        Default handler for deadline violation
        */
        ;
    }
    
    void handleGroupMessage(_group) {
        /*
        Default handler for group messages.
        Implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        ;
    }
    
    void handleVoteRequest(group, rfvId) {
        /*
        Default handler for vote requests (in member)
        Implementation must recv/recv_pyobj to obtain the topic. 
        */
        ;
    }
    
    void handleVoteResult(group, rfvId, vote) {
        /*
        Default handler for the result of a vote (in member)
        */
        ;
    }
    
    void handleActionVoteRequest(group, rfvId, when) {
        /*
        Default handler for request to vote an action in the future (in member)
        Implementation must recv/recv_pyobj to obtain the action topic. 
        */
        ;
    }
        
    void handleMessageToLeader(group) {
        /*
        Default handler for messages sent to the leader (in leader)
        Leader implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        ;
    }
    
    void handleMessageFromLeader(group) {
        /*
        Default handler for messages received from the leader (in member) 
        Member implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        ;
    }
    
    void handleMemberJoined(group, memberId) {
        /*
        Default handler for 'member join' events
        */  
        ;
    }
    
    void handleMemberLeft(group, memberId) {
        /*
        Default handler for 'member leave' events
        */          
        ;
    }
    
    void handleLeaderElected(group, leaderId) {
        /*
        Default handler for 'leader elected' events
        */  
        ;
    }
    
    void handleLeaderExited(void* group, std::uint64_t leaderId) {
        /*
        Default handler for 'leader exited' events
        */ 
        ;
    }
    
    void* joinGroup(void* groupName, void* instName, void* groupPriority) {
        if (groupPriority == NULL) {
            groupPriority = this->GROUP_PRIORITY_MIN;
        }
        if (this->thread == NULL) {
            this->thread = this->owner.thread;
        }
        object group = this->coord.getGroup(groupName, instName);
        if (group == NULL) {
            group = this->coord.joinGroup(this->thread, groupName, instName, this->getLocalID());
            this->thread.addGroupSocket(group, groupPriority);
        }
        return group;
    }

    bool leaveGroup(void* group) {
        assert type(group) == Group;
        if (this->thread == NULL) {
            this->thread = this->owner.thread;
        }
        this->thread.delGroupSocket(group);
        this->coord.leaveGroup(group);
        return true
    }
}