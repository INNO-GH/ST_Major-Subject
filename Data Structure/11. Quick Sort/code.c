#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{ // Array(size, values)
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size); // Make array to be sorted
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0); // Print Process

    fprintf(fout, "\nsorting result :\n"); // Print Result
    PrintArray(array, 0, size - 1);

    DeleteArray(array);

	return 0;
}

Array* CreateArray(int size){ // Make array to be sorted
    Array* array;
    array=(Array*)malloc(sizeof(Array));
    array->size=size;
    array->values=(int*)malloc(sizeof(int)*size);
    return array;
}
void QuickSort(Array* array, int left, int right, int depth){
    if(left>=right) return; // Case that already sorted
    int pivot_index=Partition(array, left, right, depth);
    QuickSort(array, left, pivot_index-1, depth+1);
    QuickSort(array, pivot_index+1, right, depth+1);
}
int Partition(Array* array, int left, int right, int depth){ // Process of Quick
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, array->values[right]); // Print before Situation
    PrintArray(array, left, right);
    int i=left; // 1. i start with value left
    int j=right-1; // 2. j start with value right-1
    int pivot=right; // 3. pivot start with value right(end) 
    while(i<=j){
        if(array->values[j] <= array->values[pivot]){ // 4. Finding j's value smaller or equal compared pivot
            while(1){
                if(i>j){
                    break;
                }   
                if(array->values[i] > array->values[pivot]){ // 5. Then Finding i's value larger than pivot and Swaping
                    swap(&array->values[i], &array->values[j]);
                    PrintArray(array, left, right);
                    break;
                }   
                i=i+1;
            }   
        }   
        j=j-1;
    }   
    if(i!=pivot){ // 6. Finally, Swaping pivot and i
        swap(&array->values[i], &array->values[pivot]);
        PrintArray(array, left, right);
    }
    return i;
}
void PrintArray(Array* array, int left, int right){ // Print Array
    int k;
    for(k=0;k<array->size;k++){
        if((left<=k)&&(k<=right)) fprintf(fout, "%-3d", array->values[k]);
        else fprintf(fout, "%-3s", "-");
    }
    fprintf(fout, "\n");
}
void DeleteArray(Array* array){ // DeleteArray
    free(array->values);
    free(array);
}
void swap(int* a, int* b){ // Swap(We should get pointer value to )
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

