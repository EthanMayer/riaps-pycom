from PYTHON_testMain import main
from TEST import main as main_c
from statistics import median, mean, stdev
import numpy as np

times = []

for i in range(10):
    times.append(main(100000))

times = list(np.around(np.array(times), 3))

median1 = round(median(times), 3)
mean1 = round(mean(times), 3)
std1 = round(stdev(times), 3)

print("Median time (Python): " + str(median1))
print("Mean time (Python): " + str(mean1))
print("Standard Deviation (Python): " + str(std1))
print("All times (Python): " + str(times))

times = []

for i in range(10):
    times.append(main_c(100000))

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

# time = main_c()
# print("Time: " + str(main_c()))