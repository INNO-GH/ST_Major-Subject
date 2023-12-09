#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;
typedef struct _Queue { // Queue(max_queue_size, qsize, rear, first, key)
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;
typedef struct _Graph { // Graph(size, node, matrix(Adjacency))
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150]; // numbers = vertice's room / inputs = edge's room
	fgets(numbers, 100, fin); // 1. Read first line about vertex
	int n = 0;
	countInput(&n, numbers); // n = number of vertice
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n); // Put vertice into array(nodes)

	Graph* G = CreateGraph(nodes, n); // Create Graph by nodes and n

	fgets(inputs, 100, fin); // 2. Read second line about edge
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	} // insert Edge in graph one by one

	PrintGraph(G); // Print Graph by Adjacent Matrix
	Topsort(G); // Print Graph Traversal by Topsorting
	DeleteGraph(G); // Delete Graph

	fclose(fin);
	fclose(fout);

	return 0;
}

// Basic Function to get information from file
void countInput(int* n, char* str) { // Counting one digit number in first line(vertice)
    int len = strlen(str), i;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}
void parseInput(int* arr, char* str, int n) { // Put vertice into array(nodes)
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; i++)
        if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

// Create Graph(by Adjacent Matrix Representation)
Graph* CreateGraph(int* nodes, int n){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = n;
    sort_node(nodes, n);
    graph->node = nodes;
    graph->matrix = (int**)malloc(sizeof(int*)*(n+1)); // Row
    int i, j;
    for(i=0;i<(n+1);i++){ // Column
        graph->matrix[i] = (int*)malloc(sizeof(int)*(n+1));
    }
    graph->matrix[0][0] = -1; // This means No data
    for(i=0;i<n;i++){
        graph->matrix[i+1][0] = graph->node[i]; // Source
        graph->matrix[0][i+1] = graph->node[i]; // Target
        for(j=0;j<n;j++){
            graph->matrix[j+1][i+1] = 0; // edge(start=0)
        }
    }
    return graph;
}
void sort_node(int* arr, int n){
    int i, j, temp; // Bubble Sort
    for(i=0;i<n-1;i++){
        for(j=0;j<n-1-i;j++){
            if(arr[j]>arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
void InsertEdge(Graph* G, int a, int b){
    int i, a_index, b_index;
    int n=G->size;
    for(i=0;i<n;i++){
        if(G->node[i]==a) a_index=i;
        if(G->node[i]==b) b_index=i;
    }
    G->matrix[a_index+1][b_index+1]=1;
}

// Print Graph
void PrintGraph(Graph* G){
    int i, j;
    int n=G->size;
    for(i=0;i<=n;i++){
        for(j=0;j<=n;j++){
            if(G->matrix[i][j]==-1) fprintf(fout, "   ");
            else fprintf(fout, "%d  ", G->matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

// Topological Sorting (dequeue -> erase outdegree -> enqueue indegree==0) // Cycle == 1. Queue's empty 2. Not have enqueue possibilty
void Topsort(Graph* G){    
    int n=G->size, i;
    int* save = (int*)malloc(sizeof(int)*n); // For saving dequeued key
    int countsave = 0;
    int* idg = (int*)malloc(sizeof(int)*n); // For check indegree
    for(i=0;i<n;i++) idg[i]=1; 
    Queue* Q = MakeNewQueue(n);
    while(1){
        checkIndegree(G, idg); // If indgree==0 -> enqueue
        for(i=0;i<n;i++){
            if(idg[i]==0) Enqueue(Q, G->matrix[0][i+1]);
        }
        if(IsEmpty(Q)) break;
        save[countsave++]=Dequeue(Q); // Dequeue
        int index; // erase outdegree
        for(i=0;i<n;i++){
            if(G->matrix[i+1][0]==save[countsave-1]) index=i+1;
        }
        for(i=0;i<n;i++){
            G->matrix[index][i+1]=0;
        }
    }
    if(countsave==n){
        for(i=0;i<n;i++) fprintf(fout, "%d ", save[i]);
    }
    else fprintf(fout, "sorting error : cycle!");
    fprintf(fout, "\n");
    DeleteQueue(Q);
    free(save);
    free(idg);
}
Queue* MakeNewQueue(int X){ 
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->max_queue_size = X; // number of vertices
    queue->qsize = 0;
    queue->rear = 0;
    queue->first = 1;
    queue->key = (int*)malloc(sizeof(int)*(X+1)); // +1 for not back to the 0 index
    return queue;
}
int IsEmpty(Queue* Q){
    return Q->qsize==0;
}
int Dequeue(Queue* Q){
    Q->qsize--;
    Q->first = Q->first+1;
    return Q->key[Q->first-1];
}
void Enqueue(Queue* Q, int X){
    Q->qsize++;
    Q->rear = Q->rear+1;
    Q->key[Q->rear] = X;
}
void DeleteQueue(Queue* Q){
    free(Q->key);
    free(Q);
}
void checkIndegree(Graph* G, int* idg){
    int i, j;
    int n=G->size;
    for(i=0;i<n;i++){
        if((idg[i]==0)||(idg[i]==-1)) idg[i]=-1; // -1 means that already enqueued
        else { // Not enqueued yet
            int sum=0;
            for(j=0;j<n;j++){
                sum=sum+G->matrix[j+1][i+1];
            }
            idg[i]=sum;
        }
    }
}

// Delete Graph
void DeleteGraph(Graph* G){
    int i;
    for(i=0;i<=G->size;i++){
        free(G->matrix[i]);
    }
    free(G->matrix);
    free(G->node);
    free(G);
}
