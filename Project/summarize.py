from __future__ import print_function
import os

NUM_ITER = 50
os.system('./a.out > out.txt')
for i in range(NUM_ITER - 1):
	os.system('./a.out >> out.txt')

f = open('out.txt', 'r')
a = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

pos = 0
for line in f:
	line = line[:-1]
	if(line == ''):
		pos = 0
	else:
		a[pos] += float(line)
		pos += 1

for i in range(len(a)):
	a[i] = a[i]/NUM_ITER

print('# Jobs Completed :', a[0])
print('# Jobs Dropped :', a[1])
print('# Jobs Entered Server :', a[2])
print('# Jobs Created :', a[3])
print('# Deadline Slack :', a[4])
print('# CPU Cycles Wasted :', a[5])
print('# CPU Cycles Idle :', a[6])
print('# Total Time elapsed :', a[7])
print('Avg Response Time :', a[8])
print('Avg Waiting Time :', a[9])
print()
for x in a:
	print(x, end="\t")
