/*
C++ Thread Launch File
Created on Sept. 21, 2023

@author: EthanMayer
*/

#include <pthread.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dlfcn.h>

#include "comp.hpp"

// Error handling
void error(std::string msg) {
    std::cout << msg << ": " << std::strerror(errno) << std::endl;
    exit(-1);
}

int main(int runs = 0) {
    pthread_t t1;
    pthread_t t2;

    // std::cout << "Main: Process ID: " << getpid() << std::endl;

    // std::cout << "Int: " << test << std::endl;

    char* libpath = "threadBody.so";
    void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL) {
        error("Main: Could not open shared library in launch.cpp");
    }

    typedef void* (*fptr)();

    void* thread1 = dlsym(libhandle, "thread1");

    std::string addr1 = "ipc://part_TEST1_control";

    if (pthread_create(&t1, NULL, (void * _Nullable (* _Nonnull)(void * _Nullable))thread1, &addr1) == -1) {
        error("Main: Could not create thread in launch.cpp");
    }

    std::string addr2 = "ipc://part_TEST2_control";

    if (pthread_create(&t2, NULL, (void * _Nullable (* _Nonnull)(void * _Nullable))thread1, &addr2) == -1) {
        error("Main: Could not create thread in launch.cpp");
    }

    // std::cout << "Joining Thread" << std::endl;

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;

    // pthread_exit(NULL);
}