import sys
from random import randint

# input format: arg1 = N, arg2 = CPUTIME

n = int(sys.argv[1])
max_v = int(sys.argv[3])
ret = str(n) + '\nCOMPETITION\n'
side = randint(0,1)
ret += 'X\n'
# if side == 0: ret += 'X\n'
# else: ret += 'O\n'
ret += sys.argv[2] + '\n'

for i in range(0,n):
    for j in range(0,n-1):
        ret += str(randint(0,max_v)) + ' '
    ret += str(randint(0,max_v)) + '\n'

for i in range(0,n):
    for j in range(0,n):
	k = randint(0,3)
	if k < 2: ret += '.'
	if k == 2: ret += 'X'
	if k == 3: ret += 'O'
    ret += '\n'
    
f_out = open('input.txt', 'w')
f_out.write(ret)
f_out.close()

time = ''
time += sys.argv[2] + '\n'
time += sys.argv[2] + '\n'
t_out = open('time.txt','w')
t_out.write(time)
t_out.close()