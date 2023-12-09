import sys

# printing function
def printing(index):
    if(index==0):
        return
    printing(XS[index][1])
    Write.write(str(Array[index])+' ')

# 1. Get Input
Read = open("read.txt", "r")
Write = open("write.txt", "w")
N = int(Read.readline())
Array = Read.readline()
Array = list(map(int, Array.split()))
Array.insert(0, 0);

# 2. Main Dynamic Programming
# Make XS-table
XS=[]
for i in range(N+1):
    XS.append([0, 0]) # x = length / s = fromindex
# Build XS-table
for i in range(1, N+1):
    for j in range(0, i):
        if(Array[i] > Array[j] and XS[i][0] < XS[j][0]+1): # from small problem j -> can be max expanded by i
            XS[i][0] = XS[j][0]+1
            XS[i][1] = j

# 3. Print Value and Path
value = XS[N][0]
Write.write(str(value)+'\n')
index = N
printing(index)
Read.close()
Write.close()

    
