'''
Cython Component class
Created on Feb 15, 2023

@author: EthanMayer
'''

# Error handling
cpdef error(msg):
    print(msg + ": " + strerror(errno))
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

class CythonComponentThread:

    def __init__(self, parent):
        self.name = parent.name
        self.parent = parent
        self.context = parent.context
        self.instance = parent.instance
        self.schedulerType = parent.scheduler
        self.control = None

    def setupControl():
        # Setup socket to connect to testPart.py

    def setupSockets():

    def launchThread():
        cdef pthread_t t1    # Thread 1's ID

        # Open .so shared library and grab function from it
        cdef char* libpath = "funcBody.so";
        cdef void* libhandle = dlopen(libpath, RTLD_LAZY);

        if (libhandle == NULL):
            error("Could not open shared library in comp.pyx")

        # Create thread with the .so function as body
        cdef void *thread1 = dlsym(libhandle, func)

        if (pthread_create(&t1, NULL, thread1, NULL) == -1):
            error("Could not create thread in comp.pyx")

        # Receive "Ready" message to know the thread is ready
        cdef char buf[6]
        if (z.zmq_recvbuf(get_sckt("Thread1", portDict), buf, sizeof(buf), 0) == -1):
            error("Could not receive on comp.pyx PAIR socket")
        print("Cython: received " + str(buf))

    def start():
        # self.setupControl()
        self.setupSockets()
        # self.setupPoller()
        # self.setupScheduler()
        self.launchThread()
