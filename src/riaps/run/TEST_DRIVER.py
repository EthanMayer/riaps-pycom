from PYTHON_testMain import main
from TEST import main as main_c
from statistics import median, mean, stdev
import numpy as np
import sys

if (len(sys.argv) > 1):
    runs = int(sys.argv[1])
    math = int(sys.argv[2])
    debug = int(sys.argv[3])
else:
    runs = 100000
    math = 0
    debug = 0

print("===================================")
print("SPEED TESTS")
print("Parameters:")
print("Tests Per Category: 10")
print("Messages Per Test: " + str(runs))
print("Work Between Messages: " + str(bool(math)))
print("Debug Print: " + str(bool(debug)))
print("===================================")

print("===================================")
print("Running PURE PYTHON Tests...")
times = []

for i in range(10):
    times.append(main(runs, math, debug))

times = list(np.around(np.array(times), 3))

median1 = round(median(times), 3)
mean1 = round(mean(times), 3)
std1 = round(stdev(times), 3)

print("Median time (Python): " + str(median1))
print("Mean time (Python): " + str(mean1))
print("Standard Deviation (Python): " + str(std1))
print("All times (Python): " + str(times))

print("===================================")
print("Running PYTHON + C++ Tests...")
times = []

for i in range(10):
    times.append(main_c(runs, math, debug))

times = list(np.around(np.array(times), 3))

median2 = round(median(times), 3)
mean2 = round(mean(times), 3)
std2 = round(stdev(times), 3)

print("Median time (Python + C++): " + str(median2))
print("Mean time (Python + C++): " + str(mean2))
print("Standard Deviation (Python + C++): " + str(std2))
print("All times (Python + C++): " + str(times))

speedup = round(((mean2 - mean1) / mean1 * 100), 3)
print("Average runtime difference: " + str(speedup) + "%")