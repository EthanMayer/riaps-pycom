from PYTHON_testMain import main
from statistics import median, mean, stdev
import numpy as np

times = []

for i in range(10):
    times.append(main())


myList = list(np.around(np.array(times), 3))

print("Median time: " + str(median(times)))
print("Mean time: " + str(mean(times)))
print("Standard Deviation: " + str(stdev(times)))
print("All times: " + str(times))