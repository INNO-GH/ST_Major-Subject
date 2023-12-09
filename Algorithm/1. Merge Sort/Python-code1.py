import sys

# 2. MergeSort and Merge
def MergeSort(array, p, r):
    if(p<r):
        q=int((p+r)/2)
        MergeSort(array, p, q)
        MergeSort(array, q+1, r)
        Merge(array, p, q, r)

def Merge(array, p, q, r):
    n1 = q-p+1
    n2 = r-q
    L=[]
    R=[]
    for i in range(n1):
        L.append(array[p+i])
    for j in range(n2):
        R.append(array[q+j+1])
    L.append(10**10)
    R.append(10**10)
    i=0
    j=0
    for k in range(p, r+1):
        if(L[i]<=R[j]):
            array[k]=L[i]
            i=i+1
        else:
            array[k]=R[j]
            j=j+1

# 1. Get Input
Read = open("read.txt", "r")   ## File Read -> 1. A = open("~", "r") 2. A.readline() 3. A.close()
Write = open("write.txt", "w") ## File Write -> 1. A = open("~", "w") 2. A.write(~) 3. A.close()
N = int(Read.readline())
array = []
for i in range(N):
    array.append(int(Read.readline()))

# 3. Run
MergeSort(array, 0, N-1)

# 4. Print Output
for i in range(N):
    Write.write(str(array[i])+'\n')
Read.close()
Write.close()

