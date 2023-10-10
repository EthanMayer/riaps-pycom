# TEST Python file

import zmq
from ctypes import CDLL
import jsonplus as json
import threading
from collections import namedtuple

context = zmq.Context()
control = context.socket(zmq.PAIR)
# control = context.socket(zmq.REP)
# control.bind('inproc://part_TEST_control')
control.bind('tcp://0.0.0.0:5555')

print("Python: launching c++ .so")
libc = CDLL("launch.so")
t = threading.Thread(target=libc.main)
t.start()
# libc.main()

while True:
    print("Python: ready to receive")
    message = control.recv_json()
    print("Python: received: ", message)
    # ret = {"Contents": "Return"}
    # print("Python: sending: ", ret)
    Point = namedtuple("Point", ["x", "y"])
    send = json.pretty({"dot": Point(1, 2)})
    send = json.loads(send)
    control.send_json(send)
# print(control.recv_json())
