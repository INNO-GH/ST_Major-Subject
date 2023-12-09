#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr; // BNodePtr
struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode; // BNode(order(MaxChild),size(ChildNum),child(Pointerarray),key(array),is_leaf))

BNodePtr CreateTree(int order);
void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);
int Find(BNodePtr root, int key);
void PrintTree(BNodePtr root);
void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order); // Get order
    BNodePtr root = CreateTree(order); // Create root node

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i': // insert
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f': // find
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p': // print
                if (root->size == 1)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr root;
    root=(BNodePtr)malloc(sizeof(BNode));
    root->order=order;
    root->size=1;
    root->child=(BNodePtr *)malloc(sizeof(BNodePtr)*(order+1)); // MaxNUM=order but 1 for Full
    root->is_leaf=1;
    root->key=(int *)malloc(sizeof(int)*(order)); // MaxNUM=order-1 but 1 for Full
    return root;
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언 // root = &root // So *root=root
    *root = Insert_sub(*root, *root, key, 0); // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){ // p_pos = parent's child array index
    BNodePtr root=parent;
    while(!(node->is_leaf)){ // First, Find the leaf node to be inserted
        p_pos=0;
        if(key < node->key[p_pos]){ // 1. Check leftest range in node
            parent=node;
            node=node->child[p_pos];
        }
        else if(key > node->key[node->size-2]){ // 2. Check rightest range in node
            p_pos=node->size-1;
            parent=node;
            node=node->child[p_pos];
        }
        else{
            while(p_pos+1<(node->size-1)){
                if(node->key[p_pos] < key < node->key[p_pos+1]){ // 3. Check middle range in node
                    p_pos=p_pos+1;
                    parent=node;
                    node=node->child[p_pos];
                    break;
                }
                p_pos=p_pos+1;
            }            
        }
    }
    int i=0, k=0; // Second. Put value in the leaf (by sorting)
    while(i<=node->size-2){
        if(key < node->key[i]){
            int j;
            for(j=node->size-2;j>=i;--j){
                node->key[j+1]=node->key[j];
            }
            node->key[i]=key;
            k=1; // k is for checking whether already inserted or not
            break;
        }
        i++;
    }
    if(k==0){
        node->key[node->size-1]=key;
    }
    node->size++;
    return split_node(root, root, key); // Third. Check Full and Spliting
}
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){ // (root, root, key) 
    int key=pos;
    pos=0;
    BNodePtr root=parent;
    while(!(node->is_leaf)){ // 1. Finding parent and pos
        int i, k=0;
        for(i=0;i<=node->size-2;i++){
            if(key < node->key[i]){
                parent=node;
                pos=i;
                node=node->child[pos];
                break;
            }
            else if(key == node->key[i]){
                k=1;
                break;
            }
            else if(key > node->key[node->size-2]){
                parent=node;
                pos=node->size-1;
                node=node->child[pos];
                break;
            }
        }
        if(k){
            break;
        }
    }
    if(node->size-1!=node->order){ // 2. Checking Full
        return root;
    }
    int changeroot=0; // 3. Checking root==Full
    if(node==root){
        parent=Createnode(node->order);
        parent->is_leaf=0;
        parent->size=1;
        changeroot=1;
    }
    BNodePtr splitnode; // 4. Spliting Full Node
    splitnode=Createnode(node->order);
    splitnode->size=(node->order/2)+1;
    int i;
    for(i=0;i<=splitnode->size-2;i++){
        splitnode->key[i]=node->key[((node->order-1)/2)+(i+1)];
    }
    for(i=0;i<=splitnode->size-1;i++){
        splitnode->child[i]=node->child[((node->order-1)/2)+(i+1)];
    }
    node->size=(node->order+1)/2;
    splitnode->is_leaf=node->is_leaf;
    int j; // 5. Put Middle Value on the high level node
    for(j=parent->size-2;j>=pos;--j){
        parent->key[j+1]=parent->key[j];
    }
    for(j=parent->size-1;j>pos;--j){
        parent->child[j+1]=parent->child[j];
    }
    parent->key[pos]=node->key[node->size-1];
    parent->child[pos]=node;
    parent->child[pos+1]=splitnode;
    parent->size++;
    if(changeroot){
        return parent;
    }
    return split_node(root, root, parent->key[pos]);
}
/* 노드 하나를 동적할당하는 함수, size, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){ 
    BNodePtr newnode;
    newnode=(BNodePtr)malloc(sizeof(BNode));
    newnode->order=order;
    newnode->child=(BNodePtr *)malloc(sizeof(BNodePtr)*(order+1)); // MaxNUM=order but 1 for Full
    newnode->key=(int *)malloc(sizeof(int)*(order)); // MaxNUM=order-1 but 1 for Full
    return newnode;
}

/* find node that has key */
int Find(BNodePtr root, int key){
    int i;
    for(i=0;i<(root->size-1);++i){
        if(root->key[i] > key){ // 1. key is smaller than key[i]
            if(root->is_leaf){ // There's no key
                return 0;
            }
            else{ // Go to next node
                return Find(root->child[i], key);
            }
        } 
        else if(root->key[i] == key){ // 2. key is same with key[i]
            return 1; // There's key
        }
        else if(root->key[i] < key){ // 3. key is larger than key[i]
            if(((i+1)==(root->size-1))&&(root->is_leaf)){ // Checking last of leaf
                return 0;
            }
            else if((i+1)==(root->size-1)){ // Just last > Go to next key
                return Find(root->child[i+1], key);
            }
            // Go to next key 
        }
    }
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root){
    int i;
    for(i=0;i<(root->size-1);++i){
        if(root->is_leaf){ // Print Node of leaf
            fprintf(fout, "%d ", root->key[i]);
        }
        else{ // Print Node(leaf X) -> Inoreder Traversal
            PrintTree(root->child[i]);
            fprintf(fout, "%d ", root->key[i]);
        }
    }
    if(!(root->is_leaf)){ // -> This is for Last Child Pointer
        PrintTree(root->child[i]);
    }
}

/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    int i; // Same Logic with PrintTree (like Inorder Traversal)
    for(i=0;i<(root->size-1);++i){
        if(root->is_leaf){
            free_node(root);
            return;
        }
        else{
            DeleteTree(root->child[i]);
        }
    }
    if(!(root->is_leaf)){
        DeleteTree(root->child[i]);
        free_node(root);
        return;
    }
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    free(node->child);
    free(node->key);
    free(node);
}
