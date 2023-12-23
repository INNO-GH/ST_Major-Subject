from heapq import heapify, heappush, heappop

# 1. Get Input
Read=open("read.txt", "r")
Write=open("write.txt", "w")
N = int(Read.readline())
E = int(Read.readline())
Edge = []
for i in range(E):
    SDC = Read.readline()
    SDC = list(map(int, SDC.split()))
    Edge.append(SDC)
K = int(Read.readline())

# 2. Make Graph
Graph=[]
Graph.append([[True,True]])
for i in range(N):
    Graph.append([])
for edge in Edge: # adjacency list of weighted undirected graph
    v1=edge[0]
    v2=edge[1]
    w=edge[2]
    Graph[v1].append([v2, w])
    Graph[v2].append([v1, w])

# 3. Make Dis Array
Dis=[]
Dis.append(True)
Dis.append(0)
for i in range(N-1):
    Dis.append(1000000) # sentinel value

# 4. Dijstra
# 파이썬에서 다익스트라를 구현할때는, BuildMinheap -> ExtractMin 이 아닌, InsertIfRelaxed -> ExtractMin 사용
minheap = []           
minheap.append([0,1])     # minheap(first-thing) library 
while(len(minheap)>0):    # from heapq import heapify, heappush, heappop
    ex = heappop(minheap) # heapify(heap), heappush(heap, element), heappop(heap) 
    exnode, exdis = ex[1], ex[0]
    for nextedge in Graph[exnode]:
        nextnode, nextdis = nextedge[0], nextedge[1]
        if(Dis[nextnode]>(exdis+nextdis)):
            Dis[nextnode]=(exdis+nextdis)
            heappush(minheap, [Dis[nextnode], nextnode])
            
# 5. Get Answer
Answer = 0
for distance in Dis[1:N+1]:
    if(distance<=K):
        Answer+=1   
Write.write(str(Answer))
Read.close()
Write.close()
