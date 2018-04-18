import matplotlib.pyplot as plt
c1=open("cpu1.txt","r")
c2=open("cpu2.txt","r")
c3=open("cpu3.txt","r")
c4=open("cpu4.txt","r")
x1=[]
y1=[]
x2=[]
y2=[]
x3=[]
y3=[]
x4=[]
y4=[]

for lin in c1:
	num_str=lin.split()
	x1.append(int(num_str[0]))
	y1.append(float(num_str[1]))
for lin in c2:
	num_str=lin.split()
	x2.append(int(num_str[0]))
	y2.append(float(num_str[1]))
for lin in c3:
	num_str=lin.split()
	x3.append(int(num_str[0]))
	y3.append(float(num_str[1]))
for lin in c4:
	num_str=lin.split()
	x4.append(int(num_str[0]))
	y4.append(float(num_str[1]))
fig = plt.figure()
plt.plot(x1,y1)
plt.plot(x2,y2)
plt.plot(x3,y3)
plt.plot(x4,y4)
plt.legend(['CPU 1', 'CPU 2', 'CPU 3','CPU 4'], loc='upper left')
fig.suptitle('CPUs', fontsize=20)
plt.xlabel('Thread number', fontsize=14)
plt.ylabel('Latency of thread', fontsize=14)
fig.savefig('latency.jpg')
