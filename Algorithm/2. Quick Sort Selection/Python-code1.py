import sys

# Input Function
def inputset_1():
    read = Read.readline()
    read = read[1:len(read)-2] # cannot delete but extract
    read = list(map(int, read.split(', ')))
    return read

def inputset_2():
    read = Read.readline()
    read = read[2:len(read)-3] # ignore end char
    read = list(read.split('], ['))
    read_list = []
    for i in read:
        read_list.append(list(map(int, i.split(', '))))
    return read_list

# Quick Sort Selection Function
def Partition(A, p, r):
    x=A[r]
    i=p-1
    for j in range(p, r):
        if(A[j]<=x):
            i=i+1
            A[i], A[j] = A[j], A[i]
    A[i+1], A[r] = A[r], A[i+1]
    return i+1

def QuickSortSelection(A, p, r, k):
    if(p<r):
        q=Partition(A, p, r)
        if(k==q): # q is unvariable
            return A[k]
        elif(k<q): # See left if k is smaller
            return QuickSortSelection(A, p, q-1, k)
        else: # See right if k is bigger
            return QuickSortSelection(A, q+1, r, k)
    else:
        return A[k]

# 1. Get Input
Read = open("read.txt", "r")
Write = open("write.txt", "w")
Read_1 = inputset_1()
Read_2 = inputset_2()

# 2. Quick Sort Selection
Write.write('[')
i=0
for command in Read_2:
    targetlist = Read_1[command[0]-1:command[1]]
    Write.write(str(QuickSortSelection(targetlist, 0, command[1]-command[0], command[2]-1)))
    i=i+1
    if(len(Read_2)!=i):
        Write.write(', ')
Write.write(']')
Read.close()
Write.close()
