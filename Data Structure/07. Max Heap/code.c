#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{ // Type: Heap(Capacity/Size/Elements)
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap; // Create Heap
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n': // Confirm Heap's Size and Create Heap
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i': // Insert
				fscanf(fin, "%d", &key);
				Insert(maxHeap, key);
				break;
			case 'd': // Delete
				max_element = DeleteMax(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p': // PrintHeap
				PrintHeap(maxHeap);
				break;
			case 'f': // Find
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}
	free(maxHeap->Elements);
    free(maxHeap);
    return 0;
}

// Create Heap
Heap* CreateHeap(int heapSize){
    Heap* newheap;
    newheap=(Heap*)malloc(sizeof(Heap));
    newheap->Capacity=heapSize;
    newheap->Size=0;
    newheap->Elements=(int *)malloc(sizeof(int)*(heapSize+1)); // Make RootIndex = 1
    return newheap;
}
// Insert
void Insert(Heap *heap, int value){
    if(IsFull(heap)){ // Check Heap is Full
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    if(Find(heap, value)){ // Check Heap already has value
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }
    heap->Elements[++heap->Size]=value; // Insert Value
    fprintf(fout, "insert %d\n", value);
    int i=heap->Size; // Percolate UP
    int temp;
    while((i/2)!=0){
        if(heap->Elements[i]>heap->Elements[i/2]){
          temp=heap->Elements[i/2];
          heap->Elements[i/2]=heap->Elements[i];
          heap->Elements[i]=temp;
          i=i/2;
        }
        else{
            return;
        }
    }
    return;
}
// Find
int Find(Heap* heap, int value){
    int i;
    for(i=1;i<=heap->Size;i++){
        if(heap->Elements[i]==value){ // True
            return 1;
        }
    }
    return 0; // False
}
// DeleteMax
int DeleteMax(Heap* heap){
	if(heap->Size==0){ // Check Heap is Empty
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    int deletedmax=heap->Elements[1]; // Bring Last Node to Root
    heap->Elements[1]=heap->Elements[heap->Size--];
    int i=1; // Percolate DOWN
    int temp;
    while((2*i)<=heap->Size){
        if((2*i)==heap->Size){ // Only One Child
            if(heap->Elements[i]<heap->Elements[2*i]){
                temp=heap->Elements[2*i]; // Child is bigger than parent
                heap->Elements[2*i]=heap->Elements[i];
                heap->Elements[i]=temp;
                i=2*i;
            }
            else{ // Child is smaller than parent
                break;
            }
        }
        else{ // Two Child
            if((heap->Elements[2*i]<heap->Elements[i])&&(heap->Elements[2*i+1]<heap->Elements[i])){
                break; // Both Childs are smaller than parent
            }
            else if(heap->Elements[2*i]>heap->Elements[2*i+1]){ // Left is biggest
                temp=heap->Elements[2*i];
                heap->Elements[2*i]=heap->Elements[i];
                heap->Elements[i]=temp;
                i=2*i;
            }
            else{ // Right is biggest
                temp=heap->Elements[2*i+1];
                heap->Elements[2*i+1]=heap->Elements[i];
                heap->Elements[i]=temp;
                i=2*i+1;
            }
        }
    }
    return deletedmax;
}
// PrintHeap
void PrintHeap(Heap* heap){
    if(heap->Size==0){ // Check Heap is empty
        fprintf(fout, "print error : heap is empty");
    }
    else{ // Print
        int i;
        for(i=1;i<=heap->Size;i++){
            fprintf(fout, "%d ", heap->Elements[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}
// Checking Full
int IsFull(Heap* heap){ 
    return heap->Size==heap->Capacity;
}
