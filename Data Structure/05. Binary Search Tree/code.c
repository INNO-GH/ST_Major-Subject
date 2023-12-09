#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree; // Tree=Pointer
typedef struct BST{ // BST(value/left/right)
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv; // cv gets input(char)
	int key; // key gets input(number)

	Tree root = NULL; // Make Root(empty)

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i': // insertnode
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f': // findnode
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd': // deletenode
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p': // printinorder
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}

// Insert
Tree insertNode(Tree root, int key){
    if(findNode(root, key)){ // Print Error if key is already in the tree
        fprintf(fout, "insertion error: %d is already in the tree\n", key);
        return root;
    }
    Tree node, pre, lead; // Make Node and pre/lead
    node=(Tree)malloc(sizeof(BST));
    node->value=key;
    node->left=NULL;
    node->right=NULL;
    pre=root;
    lead=root;
    if(lead==NULL){ // Case when tree is empty
        fprintf(fout, "insert %d\n", key);
        return node;
    }
    while(lead!=NULL){ // Find Postion to be inserted
        if(key < lead->value){
            pre=lead;
            lead=lead->left;
        }
        else if(key > lead->value){
            pre=lead;
            lead=lead->right;
        }
    }
    if(key < pre->value){ // insert
        pre->left=node;
    }
    else if(key > pre->value){
        pre->right=node;
    }
    fprintf(fout, "insert %d\n", key);
    return root;
}
// Delete
Tree deleteNode(Tree root, int key){
    Tree pre=root; // Make pre/temp Node
    Tree temp=root;
    while(1){ // Find Node which has key
        if(key == temp->value){
            break;
        }
        else if(key < temp->value){
            pre=temp;
            temp=temp->left;
        }
        else if(key > temp->value){
            pre=temp;
            temp=temp->right;
        }
    }
    if((temp->right==NULL)&&(temp->left==NULL)){ // 1. There's no child
        if(temp==root){ // (when deleting root)
            free(root);
            return NULL;
        }
        else{ // (when deleting endnode)
            if(pre->value < temp->value){
                pre->right=NULL;
                free(temp);
            }
            else{
                pre->left=NULL;
                free(temp);
            }
        }
    }
    else if((temp->right==NULL)){ // 2. There's left but not right tree
        if(temp==root){ // (when deleting root)
            Tree retemp=temp->left;
            free(root);
            return retemp;
        }
        else{ // (when not deleting root)
            if(pre->value < temp->value){
                pre->right=temp->left;
                free(temp);
            }
            else{
                pre->left=temp->left;
                free(temp);
            }
        }
    }
    else{ // 3. There's right tree
        Tree prebring=temp; // bring smallest in right subtree
        Tree bring=temp->right;
        while(bring->left!=NULL){
            prebring=bring;
            bring=bring->left;
        }
        if(prebring->value < bring->value){ // Fill empty space
            temp->value=bring->value;
            prebring->right=bring->right;
            free(bring);
        }
        else{
            temp->value=bring->value;
            prebring->left=bring->right;
            free(bring);
        }
    }
    return root;
}
// Find
int findNode(Tree root, int key){
    Tree temp=root; // Make Temp Node
    while(temp!=NULL){ // Check Node which has key
        if(key == temp->value){
            return 1;
        }
        else if(key < temp->value){
            temp=temp->left;
        }
        else if(key > temp->value){
            temp=temp->right;
        }
    }
    return 0;
}
// Print
void printInorder(Tree root){
    if(root){ // Print using Inorder traverse (recursive)
        printInorder(root->left);
        fprintf(fout, "%d ", root->value);
        printInorder(root->right);
    }
}
// Delete All
void deleteTree(Tree root){
    if(root){ // Delete using postorder traverse (recursive)
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}
