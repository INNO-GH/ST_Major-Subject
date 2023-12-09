import sys
import heapq

# 1. Get Input
Read=open("read.txt", "r")
Write=open("write.txt", "w")
K=int(Read.readline())
Array=Read.readline()
Array=list(map(int, Array.split()))

# 2. Code
heapq.heapify(Array) # BuildMinheap
count=0
while(True):
    if(len(Array) < 2):
        Write.write(str(-1))
        break
    x=heapq.heappop(Array) # Twice ExtractMin
    y=heapq.heappop(Array)
    if(x >= K):
        Write.write(str(count))
        break
    z=x+2*y
    heapq.heappush(Array, z) # Insert Sum
    count=count+1
Read.close()
Write.close()
