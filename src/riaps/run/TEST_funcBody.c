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
#include <unistd.h>
#include "Python.h"

// Error handling
void error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
    fflush(stdout);
    exit(1);
}

// Function to be run on a thread to send back data
void* test(void* context) {
    Py_Initialize();
    PyGILState_STATE gstate;
    // Py_BEGIN_ALLOW_THREADS
    // gstate = PyGILState_Ensure();
    // PyRun_SimpleString("print('Thread: importing ZMQ')");
    // PyObject* zmqP = PyImport_ImportModule("zmq");
    // PyRun_SimpleString("print('Thread: performing zmq.Context()')");
    // PyObject* ctxP = PyObject_CallMethod(zmqP, "Context", NULL);
    // PyRun_SimpleString("print('Thread: performing ctx.socket(zmq.PAIR)')")
    // PyObject* scktP = PyObject_CallMethod(ctxP, "socket", "zmq.PAIR");
    // PyGILState_Release(gstate);
    // Py_END_ALLOW_THREADS

    // Debug identify self
    long thread = pthread_self();
    printf("Thread %ld: Started test function\n", thread);
    fflush(stdout);

    // Create thread1 receiver pair socket and connect to main's sender pair
    //void *context = zmq_ctx_new();
    void *sckt = zmq_socket(context, ZMQ_PAIR);
    if (zmq_connect(sckt, "inproc://part_Test_Part_control") != 0) {
        error("Could not connect to main socket\n");
    }
    sleep(1);
    // Send the Ready message to main thread
    char* buf = "build";
    // fflush(stdout);
    // Py_BEGIN_ALLOW_THREADS
    // gstate = PyGILState_Ensure();
    // PyObject* obj = Py_BuildValue("s", buf); 
    // PyObject* objRepr = PyObject_Repr(obj);
    // char* objStr = PyUnicode_AsUTF8(objRepr);
    // printf("Thread %ld: Object - %s\n", thread, objStr);
    printf("Thread %ld: Sending %s signal\n", thread, buf);
    fflush(stdout);
    if (zmq_send(sckt, buf, sizeof(buf), 0) != sizeof(buf)) {
        error("Pair send buffer length incorrect\n");
    }
    // Py_DECREF(objRepr);
    // Py_DECREF(obj);
    // PyGILState_Release(gstate);
    // Py_END_ALLOW_THREADS
    sleep(1);

    // Receive build message from main thread
    char buf2[6];
    printf("Thread %ld: Ready to receive build message\n", thread);
    fflush(stdout);
    if (zmq_recv(sckt, buf2, sizeof(buf2), 0) == -1) {
        error("Could not receive on thread1 receive socket\n");
    }
    printf("Thread %ld: Received message: %s\n", thread, buf2);
    fflush(stdout);

    sleep(1);

    printf("State of GIL pre-lock: %d\n", PyGILState_Check());
    fflush(stdout);

    //Py_BEGIN_ALLOW_THREADS
    gstate = PyGILState_Ensure();

    printf("State of GIL post-lock: %d\n", PyGILState_Check());
    fflush(stdout);

    /* Perform Python actions here. */
    PyObject *pObj = PyBytes_FromString("Thread: Print\n"); /* Object creation, ref count = 1. */
    for (int i = 0; i < 5; i++) {
        PyObject_Print(pObj, stdout, 0);
        fflush(stdout);
        sleep(1);
    } 
    Py_DECREF(pObj);    /* ref count becomes 0, object deallocated.*/
    /* evaluate result or handle exception */

    /* Release the thread. No Python API allowed beyond this point. */
    PyGILState_Release(gstate);
    //Py_END_ALLOW_THREADS

    printf("Thread %ld: Done!\n");

    // Clean up socket
    zmq_close(sckt);
    zmq_ctx_destroy(context);

    // Exit
    printf("Thread %ld: Exiting\n", thread);
    fflush(stdout);
    return NULL;
}