/*
C++ Component Base Class
Created on Sept. 20, 2023

@author: EthanMayer
*/

#include "comp.hpp"

/*
Base class for RIAPS Cython application components;
*/

CppComponent::CppComponent(std::any object) {
    /*
    Constructor
    */
    // //std::any class_ = getattr('__class__');
    // this->className = "";// = getattr(class_, '__name__');
    // //this->owner = class_.OWNER; // This is set in the parent part (temporarily)
    // this->loggerName = this->owner.getActorName() + '.' + this->owner.getName();
    // this->logger = spdlog_setup.get_logger(loggerName);
    // if (this->logger == NULL) {
    //     this->logger = spdlog.ConsoleLogger(loggerName, true, true, false);
    //     this->logger.set_pattern(spdlog_setup.global_pattern);
    // }
    // // print  ( "Component() : '%s'" % self );
    // this->coord = NULL;//Coordinator(self);
}

std::string CppComponent::getName() {
    /*;
    Return the name of the component (as in model);
    */
    // return this->owner.getName();
    return "EXAMPLE_NAME";
}

std::string CppComponent::getTypeName() {
    /*;
    Return the name of the type of the component (as in model) ;
    */
    // return this->owner.getTypeName();
    return "EXAMPLE_TYPE";
}

std::uint64_t CppComponent::getLocalID() {
    /*;
    Return a locally unique ID (int) of the component. The ID is unique within the actor.;
    */
    // return id(self);
    return 0;
}

std::string CppComponent::getActorName() {
    /*;
    Return the name of the parent actor (as in model);
    */
    // return this->owner.getActorName();
    return "EXAMPLE_ACTOR_NAME";
}

std::string CppComponent::getAppName() {
    /*;
    Return the name of the parent application (as in model);
    */
    // return this->owner.getAppName();
    return "EXAMPLE_APP_NAME";
}

std::uint64_t CppComponent::getActorID() {
    /*;
    Return a globally unique ID (8 bytes) for the parent actor. ;
    */
    // return this->owner.getActorID();
    return 1;
}

std::uint64_t CppComponent::getUUID() {
    /*;
    Return the network unique ID for the parent actor;
    */
    // return this->owner.getUUID();
    return 2;
}

std::any CppComponent::joinGroup(std::any groupName, std::any instName, std::any groupPriority) {
    // if (!groupPriority.has_value()) {
    //     groupPriority = this->GROUP_PRIORITY_MIN;
    // }
    // if (!this->thread.has_value()) {
    //     this->thread = this->owner.thread;
    // }
    // std::any group = this->coord.getGroup(groupName, instName);
    // if (!group.has_value()) {
    //     group = this->coord.joinGroup(this->thread, groupName, instName, this->getLocalID());
    //     this->thread.addGroupSocket(group, groupPriority);
    // }
    // return group;
    return NULL;
}

bool CppComponent::leaveGroup(std::any group) {
    //assert group.type().name() == Group;
    // if (!this->thread.has_value()) {
    //     this->thread = this->owner.thread;
    // }
    // this->thread.delGroupSocket(group);
    // this->coord.leaveGroup(group);
    return true;
}