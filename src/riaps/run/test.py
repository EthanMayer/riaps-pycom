import time
from cython_test import Calc

start = time.time()
class NewCalc(Calc):
    def __init__(self):
        super().__init__()
        
    def find_num(self):
        self.find_primes()
        self.num = self.numPrimes
    
newCalc = NewCalc()
newCalc.find_num()
print(newCalc.num)
end = time.time()
print("Total time elapsed: " + str(end - start))