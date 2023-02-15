'''
Cython Component class
Created on Feb 15, 2023

@author: EthanMayer
'''

class CythonComponentThread:

    def __init__(self, parent):
        self.name = parent.name
        self.parent = parent
        self.context = parent.context
        self.instance = parent.instance
        self.schedulerType = parent.scheduler
        self.control = None
