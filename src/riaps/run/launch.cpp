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

int main(int test = 0) {
    pthread_t t1;

    std::cout << "Main: Process ID: " << getpid() << std::endl;

    std::cout << "Int: " << test << std::endl;

    char* libpath = "threadBody.so";
    void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL) {
        error("Main: Could not open shared library in launch.cpp");
    }

    typedef void* (*fptr)();

    void* thread1 = dlsym(libhandle, "thread1");

    if (pthread_create(&t1, NULL, (void * _Nullable (* _Nonnull)(void * _Nullable))thread1, NULL) == -1) {
        error("Main: Could not create thread in launch.cpp");
    }

    pthread_exit(NULL);
}