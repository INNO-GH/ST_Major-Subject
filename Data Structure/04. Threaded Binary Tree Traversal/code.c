#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr; // Pointer to Node
typedef int ElementType;

// Node
struct ThreadedTree {
	int left_thread; // flag if ptr is thread
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread
}ThreadedTree;

// Root node = flagleft/address/data/address/flagright
ThreadedPtr CreateTree(){
	ThreadedPtr tree = NULL;
	tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(tree==NULL){
		fprintf(fout, "Out of Space!\n");
		return NULL;
	}
	tree->data = -1;
	tree->left_thread = 1;
	tree->left_child = tree;
	tree->right_thread = 1;
	tree->right_child = tree;
	return tree;
}

// Insert Function
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
    ThreadedPtr node = NULL; // 1. Make node
    node=(ThreadedPtr)malloc(sizeof(ThreadedTree));
    if(node==NULL){
        fprintf(fout, "Out of Space!\n");
        return 0;
    }
    node->data=data;
    node->left_thread=FROMTHREAD;
    node->right_thread=FROMTHREAD;
    int a[7]; // 2. a array is traverse information(reverse). 0 means left and 1 means right
    int idx_a=idx;
    int i=0;
    while(idx_a > 1){
        a[i]=idx_a%2;
        idx_a=idx_a/2;
        i++;
    }
    a[i]=1;
    ThreadedPtr pre, cur, node_right, node_left; // 3. Find Position by traverse information
    pre = root; // pre is parent node of new node
    cur = root->right_child; // cur is leading node to find position
    node_left = root; // node_left records last left connection (successor)
    node_right = root; // node_right records last right connection (predecessor)
    while(i>=1){
        if(a[i-1]==0){
            pre=cur;
            cur=cur->left_child;
            node_left=pre;
        }
        else{
            pre=cur;
            cur=cur->right_child;
            node_right=pre;
        }
        --i;
    }
    if(a[i]==0){ // 4. Connectiong pre and node and Complete node
        pre->left_child=node;
        pre->left_thread=FROMPARENT;
        node->left_child=node_right;
        node->right_child=node_left;
    }
    else{
        pre->right_child=node;
        pre->right_thread=FROMPARENT;
        node->left_child=node_right;
        node->right_child=node_left;
    }
    return 1;
}
// Print Function
void printInorder(ThreadedPtr root){
    fprintf(fout, "inorder traversal :");
    ThreadedPtr tmp=root; // Make tmp node
    int i=0; // i is flag that we should skip first tmp=root
    while((tmp!=root)||(i==0)){
        i++;
        if(tmp->right_thread==FROMPARENT){ // Go to leftmost of subtree
            tmp=tmp->right_child;
            while(tmp->left_thread==FROMPARENT){
                tmp=tmp->left_child;
            }
            fprintf(fout, " %d", tmp->data);
        }
        else{ // Go to successor
            tmp=tmp->right_child;
            if(tmp->data!=-1){
                fprintf(fout, " %d", tmp->data);
            }
        }
    }
}
// Delte Function
void DeleteTree(ThreadedPtr root){
    ThreadedPtr pre=root; // It has same logic with Print, But substitute 'print' to 'free'
    ThreadedPtr lead=root;
    int i=0;
    while((lead!=root)||(i==0)){
        i++;
        if(lead->right_thread==FROMPARENT){
            pre=lead;
            lead=lead->right_child;
            while(lead->left_thread==FROMPARENT){
                lead=lead->left_child;
            }
            if(pre!=root){
                free(pre);
            }
        }
        else{
            pre=lead;
            lead=lead->right_child;
            if(pre!=root){
                free(pre);
            }
        }
    }
    free(root);
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree(); // Make Root

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES); // Get Number of Node

	int root_idx=0, idx=0;

	// Make Tree by repeating Insert
    while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			return 0; // This is for Error
		}
	}

	printInorder(root); // Print Inoreder Traversal
	DeleteTree(root); // Free all memory
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
