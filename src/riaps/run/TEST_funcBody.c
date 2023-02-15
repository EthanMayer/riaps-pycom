//  TEST_funcBody.c
//    
//  Author - Ethan Mayer
//  Spring 2023
//
//  This is the low-level file for the project, somewhat emulating the dynamically loaded functions that comp.py offloads to threads in RIAPS.
//  This file contains c functions to be called and spawned in POSIX threads by comp.pyx. They receive information to operate on and return to comp.pyx via ZMQ PAIR sockets.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <zmq.h>

// Error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// Function to be run on a thread to send back data
void* test() {
    // Debug identify self
    long thread = pthread_self();
    printf("Thread %ld: Started send_back function\n", thread);
    fflush(stdout);

    // Create thread1 receiver pair socket and connect to main's sender pair
    void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);
    if (zmq_connect(sckt, "tcp://127.0.0.1:5556") != 0) {
        error("Could not connect to main socket\n");
    }
    
    // Send the Ready message to main thread
    char* buf = "Ready";
    printf("Thread %ld: Sending 'Ready' signal\n", thread);
    fflush(stdout);
    if (zmq_send(sckt, buf, sizeof(buf), 0) != sizeof(buf)) {
        error("Pair send buffer length incorrect\n");
    }

    printf("Thread %ld: Done!")

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);

    // Exit
    printf("Thread %ld: Exiting\n", thread);
    fflush(stdout);
    return NULL;
}