# TEST Python file

import zmq
import timeit
import jsonplus as json
import threading
from collections import namedtuple

def threadBody(context, addr, runs):
    control = context.socket(zmq.PAIR)
    control.connect(addr)

    i = 0
    while (i < runs):
        # print("Python_Thread: ready to send")
        send = json.dumps(("Hello", "There"))
        control.send(bytes(send, encoding='utf8'))

        # print("Python_Thread: ready to receive")
        message = control.recv()
        message = json.loads(message)
        # print("Python_Thread: received: " + str(message) + " of type: " + str(type(message)))
        # i = message.y
        i = i + 1

def main(runs):
    start_time = timeit.default_timer()

    context1 = zmq.Context()
    control1 = context1.socket(zmq.PAIR)
    # control.bind('inproc://part_TEST_control')
    # control.bind('tcp://0.0.0.0:5555')
    control1.bind('inproc://PYTHON_TEST1_control')

    context2 = zmq.Context()
    control2 = context2.socket(zmq.PAIR)
    control2.bind('inproc://PYTHON_TEST2_control')

    t = threading.Thread(target=threadBody, args=(context1, 'inproc://PYTHON_TEST1_control', runs, ))
    t.start()

    t2 = threading.Thread(target=threadBody, args=(context2, 'inproc://PYTHON_TEST2_control', runs, ))
    t2.start()

    i = 0
    while (i < runs):
        # print("Python_Main: ready to receive")
        message = control1.recv()
        message = json.loads(message)
        # print("Python_Main: received: " + str(message) + " of type: " + str(type(message)))

        Point = namedtuple("Point", ["x", "y"])
        send = json.dumps(Point(i, i+1))
        control1.send_json(send)

        message = control2.recv()
        message = json.loads(message)
        Point = namedtuple("Point", ["x", "y"])
        send = json.dumps(Point(i, i+1))
        control2.send(bytes(send, encoding='utf8'))

        i = i + 1

    end_time = timeit.default_timer()
    total_time = end_time - start_time
    # print("Total runtime for " + str(i) + " messages: " + str(total_time) + " seconds")
    return total_time