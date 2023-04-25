#   TEST_part.py
#    
#   Author - Ethan Mayer
#   Spring 2023
#
#   This the top-level file of the project, somewhat emulating part.py in RIAPS. 
#   This file calls comp.pyx to spawn a thread.

# Import everything from the project's built cython module comp
from comp import *
import logging
import zmq

class Part(object):
    
    def __init__(self):
        self.logger = logging.getLogger(__name__)
        self.thread = None
        self.name = "t"
        self.parent = None
        self.context = zmq.Context()
        self.instance = None
        self.schedulerType = None
        self.scheduler = None
        
        self.control = self.context.socket(zmq.PAIR)
        self.control.bind('inproc://part_' + self.name + '_control')

    def sendControl(self, cmd, timeOut):
        '''
        Send a control message to component thread
        '''
        if self.control != None:
            self.control.setsockopt(zmq.SNDTIMEO, timeOut) 
            self.control.send_pyobj(cmd)

    def setup(self):
        self.thread = CythonComponentThread(self)     # Create component thread just like in part.py
        self.thread.start()
        time.sleep(0.01)
        self.sendControl("build", -1)