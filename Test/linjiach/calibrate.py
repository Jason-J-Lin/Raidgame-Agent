#!/usr/bin/env python
# coding=utf-8

import os
import time
import sys

ret = ""
mode = "generator.py"
if len(sys.argv)>2:
    if sys.argv[2]=="rd":
	mode = "rdgenerator.py"	
for i in xrange(4,27):
    os.system('python {1} {0} 200 50'.format(i,mode))
    print("-->On size {0}x{0}<--".format(i))
    for j in xrange(1,20):
        print("-->On depth {0}<--".format(j))
        start_time = time.time()
        os.system('timeout 6s ./Jason_acc {0} > /dev/null'.format(j))
        print("Runing time: {0}ms".format(int((time.time() - start_time) * 1000)))
        ret += str(int((time.time() - start_time) * 1000))+" "
        if int((time.time() - start_time)*1000)>5800: break
        # if int((time.time() - start_time)*1000)>1000 and i > 9: break
        # if int((time.time() - start_time)*1000)>800 and i > 12: break
        # if int((time.time() - start_time)*1000)>600 and i > 19: break
    ret+="\n"
f_out = open(sys.argv[1], 'w')
f_out.write(ret)
f_out.close()
os.system('rm -f input.txt output.txt')
