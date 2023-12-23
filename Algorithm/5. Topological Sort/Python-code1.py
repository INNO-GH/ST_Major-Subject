import sys

# 1. Get Input
Read=open("read.txt", "r")
Write=open("write.txt", "w")
NM = Read.readline() 
N,M = map(int, NM.split())
Topology=[] # save edge relationship
for i in range(M):
    SD = Read.readline()
    SD = list(map(int, SD.split()))
    Topology.append(SD)

# 2. Make Graph
Graph = []
for i in range(N+1): # use Adjacency List
    Graph.append(['~'])
for SD in Topology: # update edge information
    Sour = SD[0]
    Dest = SD[1]
    Graph[Sour].append(Dest)

# 3. Make Indegree Array
Array = [] 
for i in range(N+1): # Indegree Array by edge information
    Array.append(0)
for SD in Topology:
    Sour = SD[0]
    Dest = SD[1]
    Array[Dest]+=1

# 4. Topological Sort
for i in range(N):
    for j in range(1, N+1):
        if(Array[j]==0): # First Indegree==0 Check
            Array[j]=-1  # Update Indegree with -1
            for k in Graph[j]:
                if(k!='~'): 
                    Array[k]-=1 # Update Indegree of Expanding by Graph information
            Write.write(str(j)+' ')
            break
Read.close()
Write.close()
