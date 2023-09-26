/*
C++ Component Base Class
Created on Sept. 20, 2023

@author: EthanMayer
*/

#ifndef COMP_HPP
#define COMP_HPP

#include <string>
#include <cinttypes>
#include <any>

class CppComponent {
    // Class parameters
    const std::uint64_t GROUP_PRIORITY_MAX = 0; // Priority 0 means highest priority
    const std::uint64_t GROUP_PRIORITY_MIN = 256; // Priority 256 means lowest priority (>= 256 port indices are unexpected)
    
    std::string className = "";
    std::string loggerName = "";
    
    std::any owner = NULL;
    std::any logger = NULL;
    std::any coord = NULL;
    std::any thread = NULL;

    public:

        // Base Class Functions

        // Constructor
        CppComponent(std::any object);

        /*
        Return the name of the component (as in model);
        */
        std::string getName();
    
        /*
        Return the name of the type of the component (as in model) ;
        */
        std::string getTypeName();
        
        /*
        Return a locally unique ID (int) of the component. The ID is unique within the actor.;
        */
        std::uint64_t getLocalID();

        /*
        Return the name of the parent actor (as in model);
        */
        std::string getActorName();
        
        /*
        Return the name of the parent application (as in model);
        */
        std::string getAppName();
        
        /*
        Return a globally unique ID (8 bytes) for the parent actor. ;
        */
        std::uint64_t getActorID();
        
        /*
        Return the network unique ID for the parent actor;
        */
        std::uint64_t getUUID();

        std::any joinGroup(std::any groupName, std::any instName, std::any groupPriority);

        bool leaveGroup(std::any group);

        // Pure Virtual Functions
        
        /*
        Default activation handler;
        */
        virtual void handleActivate() = 0;
        
        /*
        Default deactivation handler;
        */
        virtual void handleDeactivate() = 0;
        
        /*
        Default activation handler;
        */
        virtual void handlePassivate() = 0;
        
        /*
        Default handler for CPU limit exceed
        */
        virtual void handleCPULimit() = 0;
        
        /* 
        Default handler for memory limit exceed
        */
        virtual void handleMemLimit() = 0;
        
        /* 
        Default handler for space limit exceed
        */
        virtual void handleSpcLimit() = 0;
            
        /* 
        Default handler for space limit exceed
        */
        virtual void handleNetLimit() = 0;
        
        /* 
        Default handler for NIC state change
        */
        virtual void handleNICStateChange(std::any state) = 0;
        
        /* 
        Default handler for peer state change
        */
        virtual void handlePeerStateChange(std::any state, std::uint64_t uuid) = 0;
        
        /*
        Default handler for deadline violation
        */
        virtual void handleDeadline(std::any _funcName) = 0;
        
        /*
        Default handler for group messages.
        Implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        virtual void handleGroupMessage(std::any _group) = 0;
        
        /*
        Default handler for vote requests (in member)
        Implementation must recv/recv_pyobj to obtain the topic. 
        */
        virtual void handleVoteRequest(std::any group, std::uint64_t rfvId) = 0;
        
        /*
        Default handler for the result of a vote (in member)
        */
        virtual void handleVoteResult(std::any group, std::uint64_t rfvId, std::any vote) = 0;
        
        /*
        Default handler for the result of a vote (in member)
        */
        virtual void handleActionVoteRequest(std::any group, std::uint64_t rfvId, std::any when) = 0;
            
        /*
        Default handler for request to vote an action in the future (in member)
        Implementation must recv/recv_pyobj to obtain the action topic. 
        */
        virtual void handleMessageToLeader(std::any group) = 0;
        
        /*
        Default handler for messages received from the leader (in member) 
        Member implementation must immediately call recv/recv_pyobj on the group to obtain message. 
        */
        virtual void handleMessageFromLeader(std::any group) = 0;
        
        /*
        Default handler for 'member join' events
        */  
        virtual void handleMemberJoined(std::any group, std::uint64_t memberId) = 0;
        
        /*
        Default handler for 'member leave' events
        */    
        virtual void handleMemberLeft(std::any group, std::uint64_t memberId) = 0;
        
        /*
        Default handler for 'leader elected' events
        */  
        virtual void handleLeaderElected(std::any group, std::uint64_t leaderId) = 0;
        
        /*
        Default handler for 'leader exited' events
        */ 
        virtual void handleLeaderExited(std::any group, std::uint64_t leaderId) = 0;
};

#endif