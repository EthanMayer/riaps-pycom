class Calc(object):
    testNum1 = 0
    testNum2 = 256

    def __init__(self):
        class_ = getattr(self, '__class__')
        className = getattr(class_, '__name__')
        testStr = "Test Name"
        self.name = testStr

    def find_primes(self):
        self.numPrimes = 5
        str = "Test String"
        self.numPrimes = 10

    def getName(self):
        return self.name

    # cpdef find_primes():
    #     cdef int numPrimes = 0
    #     cdef int i
    #     cdef int factor
    #     for i in range(200000):
    #         if (i > 1):
    #             for factor in range(2, i):
    #                 if i % factor == 0:
    #                     break
    #             else:
    #                 numPrimes += 1
    #     return numPrimes