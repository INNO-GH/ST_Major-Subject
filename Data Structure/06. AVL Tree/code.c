#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode; 
typedef struct AVLNode *Position; // AVLNode Pointer = Position = AVLTree
typedef struct AVLNode *AVLTree;
typedef int ElementType;
typedef struct AVLNode{ // AVLNode(element, left, right, height)
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
int Height(AVLTree T);
void HeightAll(AVLTree T);
AVLTree Rearrange(AVLTree T, int X);

int main(int argc, char *argv[]){
    fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL; // Make Tree(Root)
	char cv; // First Char
	int key; // Sceond Num

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i': // Insert
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd': // Delete
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree); // Print
        fprintf(fout, "\n");
	}

	DeleteTree(Tree); // Delete
	fclose(fin);
	fclose(fout);
    
	return 0;
}

// Insert
AVLTree Insert(ElementType X, AVLTree T){
    AVLTree node; // Make node to be inserted
    node=(AVLTree)malloc(sizeof(AVLNode));
    node->element=X;
    node->left=NULL;
    node->right=NULL;
    node->height=0;
    Position pre, lead; // Make pre and lead node to find postion
    lead = T;
    if(lead==NULL){ // Case when tree is empty
        return node;
    }    
    while(lead!=NULL){ // Find Position to be inserted
        pre=lead;
        if(X < lead->element){
            lead=lead->left;
        }
        else if(X > lead->element){
            lead=lead->right;
        }
        else if(X == lead->element){
            fprintf(fout, "insertion error : %d is already in the tree!\n", X);
            free(node);
            return T;
        }
    }
    if(X < pre->element){ // Insert
        pre->left=node;
    }
    else if(X > pre->element){
        pre->right=node;
    }
    T=Rearrange(T, X); // Rearrange Tree
    HeightAll(T); // Recalculate Height of All node
    return T;
}
// Delete
AVLTree Delete(ElementType X, AVLTree T){
    Position pre=T; // Make pre/temp Node // (Almost Same with lab05)
    Position temp=T;
    while(1){ // Find Node which has X
        if(temp == NULL){
            fprintf(fout, "deletion error : %d is not in the tree!\n", X);
            return T;
        }
        else if(X == temp->element){
            break;
        }
        else if(X < temp->element){
            pre=temp;
            temp=temp->left;
        }
        else if(X > temp->element){
            pre=temp;
            temp=temp->right;
        }
    }
    int a; // (a is element to rearrange the tree)
    if((temp->right==NULL)&&(temp->left==NULL)){ // 1. There's no child
        if(temp==T){ // (when deleting root)
            free(T);
            return NULL;
        }
        else{ // (when deleting endnode)
            if(pre->element < temp->element){
                pre->right=NULL;
                free(temp);
            }
            else{
                pre->left=NULL;
                free(temp);
            }
            a=pre->element;
        }
    }
    else if((temp->right==NULL)){ // 2. There's left but not right tree
        if(temp==T){ // (when deleting root)
            Position retemp=temp->left;
            free(T);
            return retemp;
        }
        else{ // (when not deleting root)
            if(pre->element < temp->element){
                pre->right=temp->left;
                free(temp);
            }
            else{
                pre->left=temp->left;
                free(temp);
            }
            a=pre->element;
        }
    }
    else{ // 3. There's right tree
        Position prebring=temp; // bring smallest in right subtree
        Position bring=temp->right;
        while(bring->left!=NULL){
            prebring=bring;
            bring=bring->left;
        }
        if(prebring->element < bring->element){ // Fill empty space
            temp->element=bring->element;
            prebring->right=bring->right;
            free(bring);
        }
        else{
            temp->element=bring->element;
            prebring->left=bring->right;
            free(bring);
        }
        a=prebring->element;
    }
    T=Rearrange(T, a); // Rearrange Tree
    HeightAll(T); // Recalculate Height of All node
    return T;
}
// LL
Position SingleRotateWithLeft(Position node){
    Position L=node->left;
    node->left=L->right;
    L->right=node;
    return L;
}
// RR
Position SingleRotateWithRight(Position node){
    Position R=node->right;
    node->right=R->left;
    R->left=node;
    return R;
}
// LR
Position DoubleRotateWithLeft(Position node){
    node->left=SingleRotateWithRight(node->left);
    return SingleRotateWithLeft(node);
}
// RL
Position DoubleRotateWithRight(Position node){
    node->right=SingleRotateWithLeft(node->right);
    return SingleRotateWithRight(node);
}
// Print
void PrintInorder(AVLTree T){
    if(T){ // Print using inorder traverse (recursive)
        PrintInorder(T->left);
        fprintf(fout, "%d(%d) ", T->element, T->height);
        PrintInorder(T->right);
    }
}
// DeleteAll
void DeleteTree(AVLTree T){
    if(T){ // DeleteAll using postorder traverse (recursive)
        DeleteTree(T->left);
        DeleteTree(T->right);
        free(T);
    }
}
// Height
int Height(AVLTree T){
    if(T==NULL){ // NULL's Height = -1
        return -1;
    }
    int lh, rh; // lh = height which consider only with left subtree / rh = vice versa 
    lh=1+Height(T->left);
    rh=1+Height(T->right);
    if(lh >= rh){ // return larger one ld vs rd
        return lh; // (Assume that leaf's height = 0)
    }
    else if(lh < rh){
        return rh; // (Assume that leaf's height = 0)
    }
}
// HeightAll
void HeightAll(AVLTree T){
    if(T){ // Calculate Height of all node using postorder traverse (recursive)
        HeightAll(T->left);
        HeightAll(T->right);
        T->height=Height(T);
    }
}
// Rearrange = Find final problem node + rotate + recursive
AVLTree Rearrange(AVLTree T, int X){
    Position preFP, FP, pre, lead; // Find final problem node
    preFP = NULL;
    FP = NULL;
    pre = NULL;
    lead = T;
    while(1){
        if(abs(Height(lead->left)-Height(lead->right)) > 1){
            preFP=pre;
            FP=lead;
        }
        if(lead->element==X){
            break;
        }
        pre=lead;
        if(X < lead->element){
            lead=lead->left;
        }
        else if(X > lead->element){
            lead=lead->right;
        }
    }
    if(FP==NULL){ // There's no problem node
        return T;
    }
    if(Height(FP->left) > Height(FP->right)){
        if(Height(FP->left->left) >= Height(FP->left->right)){ // LL
            if(preFP==NULL) T=SingleRotateWithLeft(FP);
            else if(preFP->element > FP->element) preFP->left=SingleRotateWithLeft(FP); 
            else preFP->right=SingleRotateWithLeft(FP);     
        }
        else{ // LR
            if(preFP==NULL) T=DoubleRotateWithLeft(FP);
            else if(preFP->element > FP->element) preFP->left=DoubleRotateWithLeft(FP);
            else preFP->right=DoubleRotateWithLeft(FP);             
        }
    }
    else{
        if(Height(FP->right->right) >= Height(FP->right->left)){ // RR
            if(preFP==NULL) T=SingleRotateWithRight(FP);
            else if(preFP->element > FP->element) preFP->left=SingleRotateWithRight(FP);
            else preFP->right=SingleRotateWithRight(FP);
        }
        else{ // RL
            if(preFP==NULL) T=DoubleRotateWithRight(FP);
            else if(preFP->element > FP->element) preFP->left=DoubleRotateWithRight(FP);
            else preFP->right=DoubleRotateWithRight(FP);
        }
    }
    return T=Rearrange(T, X); // Recursive
}
