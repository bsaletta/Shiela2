import random
f=open('sampleFile.txt','w');
print (f)
x=0
y=0
z=0
ax=0
ay=0
az=0

for i in range(0,120):
    random.seed(i);
    x+=random.uniform(1,3);
    y+=random.uniform(1,3);
    z+=random.uniform(1,3);
    ax+=random.uniform(1,15);
    ay+=random.uniform(1,3);
    az+=random.uniform(1,3);
    print ("%d,%d,%d,%d,%d,%d\r" %(x,y,z,ax,ay,az))
    f.write("%d,%d,%d,%d,%d,%d\n" %(x,y,z,ax,ay,az))
