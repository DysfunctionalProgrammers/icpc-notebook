#!/usr/bin/env python3

from random import randint
from subprocess import Popen, PIPE

while True:
    N = 10
    mountains = [randint(0, 10) for i in range(N)]
    instring = ' '.join(str(x) for x in [N] + mountains).encode('utf-8')

    p1 = Popen('./mountains', stdin=PIPE, stdout=PIPE)
    output1 = p1.communicate(instring)
    p2 = Popen('./mountains.3', stdin=PIPE, stdout=PIPE)
    output2 = p2.communicate(instring)
    if output1 != output2:
        print(instring)
        break
    else:
        print('YES')
