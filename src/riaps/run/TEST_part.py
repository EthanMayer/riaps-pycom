#   TEST_part.py
#    
#   Author - Ethan Mayer
#   Spring 2023
#
#   This the top-level file of the project, somewhat emulating part.py in RIAPS. 
#   This file calls comp.pyx to spawn a thread.

# Import everything from the project's built cython module comp
from comp import *

class Part(object):
    
    def __init__(self):
        self.thread = CythonComponentThread(self)     # Create component thread just like in part.py
        self.thread.start()