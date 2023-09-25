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

    CppComponent(std::any object) {
        /*
        Constructor
        */
        //std::any class_ = getattr('__class__');
        this->className = "";// = getattr(class_, '__name__');
        //this->owner = class_.OWNER; // This is set in the parent part (temporarily)
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
        this->loggerName = this->owner.getActorName() + '.' + this->owner.getName();
        this->logger = spdlog_setup.get_logger(loggerName);
        if (this->logger == NULL) {
            this->logger = spdlog.ConsoleLogger(loggerName, true, true, false);
            this->logger.set_pattern(spdlog_setup.global_pattern);
        }
        // print  ( "Component() : '%s'" % self );
        this->coord = NULL;//Coordinator(self);
    }
 
    std::string getName() {
        /*;
        Return the name of the component (as in model);
        */
        return this->owner.getName();
    }
    
    std::string getTypeName() {
        /*;
        Return the name of the type of the component (as in model) ;
        */
        return this->owner.getTypeName();
    }
    
    std::uint64_t getLocalID() {
        /*;
        Return a locally unique ID (int) of the component. The ID is unique within the actor.;
        */
        return id(self);
    }

    std::string getActorName() {
        /*;
        Return the name of the parent actor (as in model);
        */
        return this->owner.getActorName();
    }
    
    std::string getAppName() {
        /*;
        Return the name of the parent application (as in model);
        */
        return this->owner.getAppName();
    }
    
    std::uint64_t getActorID() {
        /*;
        Return a globally unique ID (8 bytes) for the parent actor. ;
        */
        return this->owner.getActorID();
    }
    
    std::uint64_t getUUID() {
        /*;
        Return the network unique ID for the parent actor;
        */
        return this->owner.getUUID();
    }
    
    std::any joinGroup(std::any groupName, std::any instName, std::any groupPriority) {
        if (!groupPriority.has_value()) {
            groupPriority = this->GROUP_PRIORITY_MIN;
        }
        if (!this->thread.has_value()) {
            this->thread = this->owner.thread;
        }
        std::any group = this->coord.getGroup(groupName, instName);
        if (!group.has_value()) {
            group = this->coord.joinGroup(this->thread, groupName, instName, this->getLocalID());
            this->thread.addGroupSocket(group, groupPriority);
        }
        return group;
    }

    bool leaveGroup(std::any group) {
        //assert group.type().name() == Group;
        if (!this->thread.has_value()) {
            this->thread = this->owner.thread;
        }
        this->thread.delGroupSocket(group);
        this->coord.leaveGroup(group);
        return true;
    }
};