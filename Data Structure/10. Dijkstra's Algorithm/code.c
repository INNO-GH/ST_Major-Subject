#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;
typedef struct Node { // Node=Vertice(vertex/dist/prev)
	int vertex;
	int dist;	//distance
	int prev;
}Node;
typedef struct Graph { // Graph(size/vertices/nodes)
	int size;
	int** vertices;
	Node* nodes;
}Graph;
typedef struct Heap { // Heap(Capacity/Size/Element)
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);
int printcheck(int vertice, Heap* mh);
void printsub(int vertice, Heap* mh);
int cost(int vertice, Heap* mh);
void percolateDown(int i, Heap* minHeap);

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size); // Get Size
	g = createGraph(size + 1); // Create Graph (Matrix)

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight; // Source-Target-Weight
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight; // This code make complete matrix at beginning
		tmp = fgetc(fin);
	}

	printShortestPath(g);

	deleteGraph(g);
}

Graph* createGraph(int size){ 
    Graph* g;
    g=(Graph*)malloc(sizeof(Graph));
    g->size=size;
    g->vertices=(int**)malloc(sizeof(int*)*size); // Make matrix (0~n) * (0~n) = about Edge
    int i, j;
    for(i=0;i<size;i++){
        g->vertices[i]=(int*)malloc(sizeof(int)*size);
    }
    g->vertices[0][0]=-1;
    for(i=1;i<size;i++){
        g->vertices[0][i]=i;
        g->vertices[i][0]=i;
        for(j=1;j<size;j++){
            g->vertices[i][j]=0;
        }
    }
    return g;
}
void deleteGraph(Graph* g){
    int i;
    for(i=0;i<g->size;i++){
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g);
}
void printShortestPath(Graph* g){
    Heap* mh = createMinHeap(g->size);
    while(mh->Size > 1){
        deleteMin(mh); // 1. DeleteMin
        int i; // 2. Find Outdegree&dis>predis+weight
        for(i=1;i<g->size;i++){
            int source = mh->Element[mh->Size].vertex; // (prenode)
            int target = mh->Element[i].vertex;
            if( (g->vertices[source][target] > 0) && (mh->Element[i].dist > mh->Element[mh->Size].dist + g->vertices[source][target]) ){
                mh->Element[i].prev=source; // 3. pred = prenode
                mh->Element[i].dist = mh->Element[mh->Size].dist + g->vertices[source][target]; // 4. dis=predis+weight
                insertToMinHeap(mh, 0, 0); // 5. Rebuild MinHeap
            }
        }
    }
    int i; // Print
    for(i=2;i<g->size;i++){
        if(printcheck(i, mh)==0){
            fprintf(fout, "can not reach to node %d\n", i);
        }
        else{
            printsub(i, mh);
            fprintf(fout, " (cost : %d)\n", cost(i, mh));
        }
    }
    deleteMinHeap(mh);
}
int printcheck(int vertice, Heap* mh){ // Check that there's path about source to vertice
    int index=1; // Find vertice's index in list
    while(1){
        if(vertice==mh->Element[index].vertex){
            break;
        }
        index++;
    }
    if(mh->Element[index].prev==-2){ // Check
        return 0;
    }
    else if(mh->Element[index].prev==-1){
        return 1;
    }
    else{
        return printcheck(mh->Element[index].prev, mh);
    }
}
void printsub(int vertice, Heap* mh){ // This is for print path by recuresive function
    int index=1; // Find vertice's index in list
    while(1){
        if(vertice==mh->Element[index].vertex){
            break;
        }
        index++;
    }
    if(mh->Element[index].prev==-1){ // Print source
        fprintf(fout, "%d", mh->Element[index].vertex);
        return;
    }
    else{
        printsub(mh->Element[index].prev, mh); // Print path
        fprintf(fout, "->%d", mh->Element[index].vertex);
    }
}
int cost(int vertice, Heap* mh){
    int index=1;
    while(1){
        if(vertice==mh->Element[index].vertex){
            break;
        }
        index++;
    }
    return mh->Element[index].dist;
}
Heap* createMinHeap(int heapSize){
    Heap* mh;
    mh=(Heap*)malloc(sizeof(Heap));
    mh->Capacity=heapSize;
    mh->Size=heapSize;
    mh->Element=(Node*)malloc(sizeof(Node)*heapSize); // Make array about Vertice
    int i;
    for(i=1;i<heapSize;i++){
        mh->Element[i].vertex=i;
        mh->Element[i].dist=INF;
        mh->Element[i].prev=-2;
    }
    mh->Element[1].dist=0;
    mh->Element[1].prev=-1;
    return mh;
}
void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);
}
void insertToMinHeap(Heap* minHeap, int vertex, int distance){ // I'll use this by Rebuild MinHeap
    int i;
    for(i=minHeap->Size-1;i>0;i--){
        percolateDown(i, minHeap);
    }
}
Node deleteMin(Heap* minHeap){
    Node temp;
    temp = minHeap->Element[minHeap->Size-1]; // Change root and end
    minHeap->Element[minHeap->Size-1]=minHeap->Element[1];
    minHeap->Element[1]=temp;
    --minHeap->Size;
    // percolate down
    percolateDown(1, minHeap);
    return temp;
}
void percolateDown(int i, Heap* minHeap){ // percolate down
    Node temp;
    int heapSize=minHeap->Size;
    while(i*2<=heapSize-1){
        if((i*2==heapSize-1)&&(minHeap->Element[i].dist > minHeap->Element[2*i].dist)){ // case 1. There's only one leaf and that is smallest
            temp = minHeap->Element[i];
            minHeap->Element[i] = minHeap->Element[2*i];
            minHeap->Element[2*i] = temp;
            i=2*i;
        }
        else if((i*2<heapSize-1)&&(minHeap->Element[2*i].dist <= minHeap->Element[2*i+1].dist)&&(minHeap->Element[2*i].dist < minHeap->Element[i].dist)){ // case 2. Left Child is smallest
            temp = minHeap->Element[i];
            minHeap->Element[i] = minHeap->Element[2*i];
            minHeap->Element[2*i] = temp;
            i=2*i;
        }
        else if((i*2<heapSize-1)&&(minHeap->Element[2*i].dist >= minHeap->Element[2*i+1].dist)&&(minHeap->Element[2*i+1].dist < minHeap->Element[i].dist)){ // case 3. Right Child is smallest
            temp = minHeap->Element[i];
            minHeap->Element[i] = minHeap->Element[2*i+1];
            minHeap->Element[2*i+1] = temp;
            i=2*i+1;
        }
        else{
            break;
        }
    }
}
