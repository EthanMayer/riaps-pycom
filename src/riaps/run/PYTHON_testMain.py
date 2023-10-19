# TEST Python file

import zmq
import timeit
import jsonplus as json
import threading
from collections import namedtuple
from math import sqrt

def fib(n):
    if (n == 0): return 0
    if (n == 1): return 1

    prevNum = 0
    curNum = 1

    for i in range(2,n+1):
        prevPrevNum = prevNum
        prevNum = curNum
        curNum = prevNum + prevPrevNum

    return curNum

def threadBody(context, addr, runs, math = 0, debug = 0):
    control = context.socket(zmq.PAIR)
    control.connect(addr)

    i = 0
    while (i < runs):
        if debug: print("Python_Thread: ready to send")

        if math: 
            x = fib(int(sqrt(i)))
            send_str = "Fibonacci number of sqrt(" + str(i) + ")"
            send = json.dumps((send_str, x))
        else:
            send = json.dumps(("Contents", "Ready"))
        control.send(bytes(send, encoding='utf8'))

        if debug: print("Python_Thread: ready to receive")
        message = control.recv()
        message = json.loads(message)
        if debug: print("Python_Thread: received: " + str(message) + " of type: " + str(type(message)))
        # i = message.y
        i = i + 1

def main(runs, math = 0, debug = 0):
    start_time = timeit.default_timer()

    context1 = zmq.Context()
    control1 = context1.socket(zmq.PAIR)
    # control.bind('inproc://part_TEST_control')
    # control.bind('tcp://0.0.0.0:5555')
    control1.bind('inproc://PYTHON_TEST1_control')

    context2 = zmq.Context()
    control2 = context2.socket(zmq.PAIR)
    control2.bind('inproc://PYTHON_TEST2_control')

    t = threading.Thread(target=threadBody, args=(context1, 'inproc://PYTHON_TEST1_control', runs, math, debug, ))
    t.start()

    t2 = threading.Thread(target=threadBody, args=(context2, 'inproc://PYTHON_TEST2_control', runs, math, debug, ))
    t2.start()

    i = 0
    while (i < runs):
        if debug: print("Python_Main: ready to receive")
        message = control1.recv()
        message = json.loads(message)
        if debug: print("Python_Main: received: " + str(message) + " of type: " + str(type(message)))

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