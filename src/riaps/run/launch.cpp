/*
C++ Thread Launch File
Created on Sept. 21, 2023

@author: EthanMayer
*/

#include <pthread.h>
#include <iostream>
#include <string>

// Error handling
void error(std::string msg) {
    std::cout << msg << ": " << std::strerror(errno) << std::endl;
    exit(-1);
}

void* thread1(void* threadid) {
    long tid = (long)threadid;
    std::cout << "Hello World! Thread ID: " << tid << std::endl;
    pthread_exit(NULL);
}

int main() {
    pthread_t t1;

    if (pthread_create(&t1, NULL, thread1, (void*)t1) == -1) {
        error("Could not create thread in launch.cpp");
    }

    pthread_exit(NULL);
}