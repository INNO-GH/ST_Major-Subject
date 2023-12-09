#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet { // DisjointSet(size_maze, ptr_arr)
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);
void swap(int *x, int *y) { // Swap Function
    int t = *x;
    *x = *y;
    *y = t;
}

int main(int argc, char* agrv[]) {
    srand((unsigned int)time(NULL)); // Make Random Number

    int num, i;
    fin = fopen(agrv[1], "r");
    fout = fopen(agrv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets)); // Make Sets(Room)
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets)); // Make Sets for printing(Wall)

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
	int i;

	sets->size_maze = num * num; // Number of Room
	sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
	for (i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = 0; // I'll Check Depth in Root

	maze_print->size_maze = num * num * 2; // Number of Wall
	maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
	for (i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;
	//maze_print->ptr_arr[(x-1)*2+1 ~ x*2] : two directions(right, down) walls of the number x

	//start and end position have not to have the wall // Delete End Right Wall
	maze_print->ptr_arr[maze_print->size_maze - 1] = 0;
}
void Union(DisjointSets *sets, int i, int j) {
    int iSet = find(sets, i);
    int jSet = find(sets, j);
    if(iSet == jSet) return; // Case 1. Same Set
    if(sets->ptr_arr[iSet] < sets->ptr_arr[jSet]){ // Case 2. iSet is deeper than jSet
        sets->ptr_arr[jSet] = iSet;
    }
    else if(sets->ptr_arr[iSet] > sets->ptr_arr[jSet]){ // Case 3. jSet is deeper than iSet
        sets->ptr_arr[iSet] = jSet; 
    }
    else{ // Case 4. iSet has same depth with jSet
        sets->ptr_arr[jSet] = iSet;
        sets->ptr_arr[iSet]--;
    }
}
int find(DisjointSets *sets, int i) {
    while(1){
        if(sets->ptr_arr[i]<=0) return i; // Root
        i=sets->ptr_arr[i]; // No Root
    }
}
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num){
    while(find(sets, 1) != find(sets, num*num)){ // We should repeat until connecting begin and end
        int irand = rand()%(num*num) + 1;
        int jrand = rand()%(num*num) + 1; // (1~num*num)
        if( find(sets, irand) == find(sets, jrand) ) continue; // Same Set
        if(irand > jrand) swap(&irand, &jrand); // This is for smaller irand
        // When we delete right wall
        if(irand+1 == jrand) {
            if(irand%num == 0) continue; // (when the line jumps between two)
            Union(sets, irand, jrand);
            maze_print->ptr_arr[irand*2-1]=0;
        }
        // When we delete bottom wall
        else if(irand+num == jrand) {
            Union(sets, irand, jrand);
            maze_print->ptr_arr[irand*2]=0;
        }
    }
}
void printMaze(DisjointSets *sets, int num){
    int i;
    // First Horizontal Line
    for(i=0;i<num;i++){
        fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");
    // Complete
    int ver=1; // ver is for checking vertical information in wall's information
    int hor=2; // hor is for checking horizontal information in wall's information
    for(i=1;i<=num*2;i++){ // i means i'st line except first line
        int j;
        if(i%2 == 1) { // 1. When vertical
            if(i==1) fprintf(fout, " "); // We should blank first line's first block
            else fprintf(fout, "|");
            // Judge the wall's existance
            for(j=0;j<num;j++){
                if(sets->ptr_arr[ver] == 1) fprintf(fout, "   |");
                else fprintf(fout, "    ");
                ver = ver + 2;
            }
            fprintf(fout, "\n");
        }
        else { // 2. When horizontal
            for(j=0;j<num;j++){
                if(sets->ptr_arr[hor] == 1) fprintf(fout, "+---");
                else fprintf(fout, "+   ");
                hor = hor + 2;
            }
            fprintf(fout, "+\n");
        }
    }
}
void freeMaze(DisjointSets *sets, DisjointSets *maze_print){
    free(sets->ptr_arr);
    free(maze_print->ptr_arr);
    free(sets);
    free(maze_print);
}

















