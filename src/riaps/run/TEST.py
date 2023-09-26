# TEST Python file

import zmq
from ctypes import CDLL

context = zmq.Context()
control = context.socket(zmq.PAIR)
control.bind('inproc://part_TEST_control')

libc = CDLL("launch.so")
libc.main()


