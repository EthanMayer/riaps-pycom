# TEST Python file

import zmq
from ctypes import CDLL, c_int
import jsonplus as json
import threading
from collections import namedtuple
import timeit

def main(runs):
    start_time = timeit.default_timer()

    context1 = zmq.Context()
    control1 = context1.socket(zmq.PAIR)
    # control.bind('inproc://part_TEST_control')
    control1.bind('ipc://part_TEST1_control')

    context2 = zmq.Context()
    control2 = context2.socket(zmq.PAIR)
    control2.bind('ipc://part_TEST2_control')

    # print("Python: launching c++ .so")
    libc = CDLL("launch.so")
    t = threading.Thread(target=libc.main, args=(c_int(runs), ))
    t.start()

    i = 0
    while (i < runs):
        # print("Python: ready to receive")
        message = control1.recv()
        message = json.loads(message)
        # print("Python: received: " + str(message) + " of type: " + str(type(message)))

        Point = namedtuple("Point", ["x", "y"])
        send = json.dumps(Point(i, i+1))
        # send = json.pretty({"dot": Point(1, 2)})
        control1.send(bytes(send, encoding='utf8'))

        message = control2.recv()
        message = json.loads(message)

        Point = namedtuple("Point", ["x", "y"])
        send = json.dumps(Point(i, i+1))
        control2.send(bytes(send, encoding='utf8'))

        i = i + 1

    end_time = timeit.default_timer()
    total_time = end_time - start_time
    return total_time