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
#include <charconv>

#include "comp.hpp"

typedef struct {
    std::string addr;
    int runs;
    int math;
    int print;
    int root;
} arg_array;

// Error handling
void error(std::string msg) {
    std::cout << msg << ": " << std::strerror(errno) << std::endl;
    exit(-1);
}

extern "C" int launch(int runs, int math = 0, int print = 0, int root = 0) {
    pthread_t t1;
    pthread_t t2;
    arg_array arg1, arg2;

    // int print = std::atoi(argv[3]);

    if (print) { std::cout << "Main: Process ID: " << getpid() << std::endl; }

    if (print) { std::cout << "Runs: " << runs << " Math: " << math << " Print: " << print << std::endl; }

    char* libpath = "threadBody.so";
    void* libhandle = dlopen(libpath, RTLD_LAZY);

    if (libhandle == NULL) {
        error("Main: Could not open shared library in launch.cpp");
    }

    typedef void* (*fptr)();

    void* thread1 = dlsym(libhandle, "thread1");

    arg1.addr = "ipc://part_TEST1_control";
    arg1.runs = runs; //std::atoi(argv[1]);
    arg1.math = math; //std::atoi(argv[2]);
    arg1.print = print; //std::atoi(argv[3]);
    arg1.root = root;

    if (pthread_create(&t1, NULL, (void * _Nullable (* _Nonnull)(void * _Nullable))thread1, &arg1) == -1) {
        error("Main: Could not create thread in launch.cpp");
    }

    arg2.addr = "ipc://part_TEST2_control";
    arg2.runs = runs; //std::atoi(argv[1]);
    arg2.math = math; //std::atoi(argv[2]);
    arg2.print = print; //std::atoi(argv[3]);
    arg2.root = root;

    if (pthread_create(&t2, NULL, (void * _Nullable (* _Nonnull)(void * _Nullable))thread1, &arg2) == -1) {
        error("Main: Could not create thread in launch.cpp");
    }

    if (print) { std::cout << "Joining Thread" << std::endl; }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;

    // pthread_exit(NULL);
}