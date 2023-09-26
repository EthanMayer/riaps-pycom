/*
C++ Thread Launch File
Created on Sept. 21, 2023

@author: EthanMayer
*/

#include <pthread.h>
#include <iostream>
#include <string>
#include <unistd.h>

// Error handling
void error(std::string msg) {
    std::cout << msg << ": " << std::strerror(errno) << std::endl;
    exit(-1);
}

void* thread1(void*) {
    pthread_t tid = pthread_self();
    // long pid = getpid();
    std::cout << "Thread: Process ID: " << getpid() << " Thread ID: " << tid << std::endl;
    pthread_exit(NULL);
}

int main() {
    pthread_t t1;

    std::cout << "Main: Process ID: " << getpid() << std::endl;

    if (pthread_create(&t1, NULL, thread1, NULL) == -1) {
        error("Could not create thread in launch.cpp");
    }

    pthread_exit(NULL);
}