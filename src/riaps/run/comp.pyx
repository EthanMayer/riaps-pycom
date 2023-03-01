'''
Cython Component class
Created on Feb 15, 2023

@author: EthanMayer
'''

# Import C libraries
from libc.stdlib cimport malloc, free, atoi
from posix.dlfcn cimport dlopen, dlsym, RTLD_LAZY
from libc.string cimport strerror
from libc.errno cimport errno
from libc.stdio cimport sprintf
from posix.unistd cimport fork, pid_t
from cpython.pycapsule cimport PyCapsule_New, PyCapsule_IsValid, PyCapsule_GetPointer, PyCapsule_GetName
from zmq.backend.cython cimport libzmq as z
# Import project's pthread .pxd header file
from pthread cimport pthread_create, pthread_join, pthread_t

# Import Python libraries
import threading
from time import sleep

# Error handling
cpdef error(msg):
    print(msg + ": " + str(strerror(errno)))
    exit(1)

# Destructor for pointers stored in PyCapsule (only needed for dynamically allocated pointers stored as PyCapsules)
cdef void free_ptr(object cap):
    free(PyCapsule_GetPointer(cap,PyCapsule_GetName(cap)))

# Wrap a void* socket pointer in a PyCapsule and store in a Python dictionary
cdef void store_sckt(const char* name, void* sckt, portDict):
    portDict[name] = PyCapsule_New(sckt, name, NULL)

# Unrwap a void* socket pointer stored in a PyCapsule that is stored in a Python dictionary
cdef void* get_sckt(const char* name, portDict):
    # First check to ensure capsule is valid
    if (PyCapsule_IsValid(portDict[name], name)):
        return PyCapsule_GetPointer(portDict[name], name)
    else:
        error("Could not retrieve socket pointer from dictionary in comp.pyx")

class CythonComponentThread(threading.Thread):

    def __init__(self, parent):
        threading.Thread.__init__(self,daemon=False)
        # self.name = parent.name
        # self.parent = parent
        # self.context = parent.context
        # self.instance = parent.instance
        # self.schedulerType = parent.scheduler
        # self.control = None

    #def setupControl():
        # Setup socket to connect to testPart.py

    #def setupSockets():

    def launchThread(self):
        #cdef pthread_t t1    # Thread 1's ID
        portDict = {}   # Dictionary for ports

        # Create main thread pair socket and bind to IP (happens later in fork if statements)
        cdef void* ctx
        cdef void* sckt

        # Open .so shared library and grab function from it
        cdef char* libpath = "TEST_funcBody.so";
        cdef void* libhandle = dlopen(libpath, RTLD_LAZY);

        if (libhandle == NULL):
            error("Could not open shared library in comp.pyx")

        # Create thread with the .so function as body
        cdef void *thread1 = dlsym(libhandle, "test")

        # Declare message variables early (outside of fork if statements)
        cdef char buf[6]
        cdef char* message = 'Build'

        # Fork to create a new process
        cdef pid_t pid = fork()
        
        # Error occured
        if (pid < 0):
            error("Fork failed in comp.pyx")

        # Code to run in new process
        elif (pid == 0):
            # Call the function pointer as function, not in a pthread
            (<void (*)()>thread1)()

            ####### OLD PTHREAD CODE ########
            #if (pthread_create(&t1, NULL, thread1, NULL) == -1):
            #    error("Could not create thread in comp.pyx")
            
            # Join thread
            #print("Cython: Joining thread")
            #if (pthread_join(t1, NULL) == -1):
            #    error("Could not join thread1 in comp.pyx")

            print("Cython Fork: Done!")

            # Wait to verify two processes are running
            sleep(5)
        
        # Code to run in current process
        else:
            # Create socket
            ctx = z.zmq_ctx_new()
            sckt = z.zmq_socket(ctx, z.ZMQ_PAIR)

            # Bind socket
            if (z.zmq_bind(sckt, "tcp://127.0.0.1:5556") != 0):
                error("Could not bind comp.pyx PAIR socket address")

            # Store socket in dictionary
            store_sckt("Thread1", sckt, portDict)

            # Receive "Ready" message to know the thread is ready (to buffer declared earlier)
            if (z.zmq_recvbuf(get_sckt("Thread1", portDict), buf, sizeof(buf), 0) == -1):
                error("Could not receive on comp.pyx PAIR socket")
            print("Cython: received " + str(buf))

            sleep(1)

            # Send build message (declared earlier) to thread
            if (z.zmq_sendbuf(get_sckt("Thread1", portDict), message, sizeof(message), 0) != sizeof(message)):
                error("Comp.pyx PAIR send build message length incorrect")
            print("Cython: Sent message: " + str(message))

            sleep(1)

            print("Cython: Done!")

            # Clean up socket
            if (z.zmq_close(get_sckt("Thread1", portDict)) == -1):
                error("Could not close comp.pyx PAIR socket")
            if (z.zmq_ctx_destroy(ctx) == -1):
                error("Could not destroy comp.pyx ZMQ context")

            print("Cython: Exiting")

            # Wait to verify two processes are running
            sleep(5)

        

    def run(self):
        # self.setupControl()
        # self.setupSockets()
        # self.setupPoller()
        # self.setupScheduler()
        self.launchThread()


