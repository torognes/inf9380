import math

class Vector(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def norm(self):
        return math.sqrt(pow(self.x, 2) + pow(self.y, 2))

    def __add__(self, vect):
        '''Operators (e.g. "+") can also be overloaded'''
        return Vector(self.x + vect.x, self.y + vect.y)


