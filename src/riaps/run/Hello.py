# import riaps
from comp import CythonComponent
import logging

class Hello(CythonComponent):
    def __init__(self):
        super(Hello, self).__init__()
        
    def on_clock(self):
        now = self.clock.recv_pyobj()   # Receive time.time() as float
        self.logger.info('on_clock(): %s' % str(now))
    
