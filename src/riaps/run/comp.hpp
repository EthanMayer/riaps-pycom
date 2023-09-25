/*
C++ Component Base Class
Created on Sept. 24, 2023

@author: EthanMayer
*/

#ifndef COMP_H
#define COMP_H

#include <string>
#include <cinttypes>

class CppComponent {
    std::uint64_t GROUP_PRIORITY_MAX;
    std::uint64_t GROUP_PRIORITY_MIN;
    void* owner;
    void* logger;
    void* coord;
    //dict __dict__ // catch-all for dynamic attributes assigned at runtime
    void* thread;

    public:
        std::string getName();
    
        std::string getTypeName();
        
        std::uint64_t getLocalID();

        std::string getActorName();
        
        std::string getAppName();
        
        std::uint64_t getActorID();
        
        std::uint64_t getUUID();
        
        void handleActivate();
        
        void handleDeactivate();
        
        void handlePassivate();
        
        void handleCPULimit();
        
        void handleMemLimit();
        
        void handleSpcLimit();
            
        void handleNetLimit();
        
        void handleNICStateChange(void* state);
        
        void handlePeerStateChange(void* state, std::uint64_t uuid);
        
        void handleDeadline(void* _funcName);
        
        void handleGroupMessage(void* _group);
        
        void handleVoteRequest(void* group, std::uint64_t rfvId);
        
        void handleVoteResult(void* group, std::uint64_t rfvId, void* vote);
        
        void handleActionVoteRequest(void* group, std::uint64_t rfvId, void* when);
            
        void handleMessageToLeader(void* group);
        
        void handleMessageFromLeader(void* group);
        
        void handleMemberJoined(void* group, std::uint64_t memberId);
        
        void handleMemberLeft(void* group, std::uint64_t memberId);
        
        void handleLeaderElected(void* group, std::uint64_t leaderId);
        
        void handleLeaderExited(void* group, std::uint64_t leaderId);
        
        void* joinGroup(void* groupName, void* instName, void* groupPriority);

        bool leaveGroup(void* group);
};

#endif