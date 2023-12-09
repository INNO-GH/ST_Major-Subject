#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

/*Make New Type: struct node and struct node **/
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
struct Node {
    ElementType element;
    Position next;
};

/*Declare Function*/
List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void DeleteList(List L);

/*Define Finction*/
/*Make Header with element=-1 and next=NULL*/
List MakeEmpty(List L){
    L=(List)malloc(sizeof(struct Node));
    L->element = -1;
    L->next = NULL;
    return L;
}
/*Determine whether the List->next is NULL or not (To judge empty list)*/
int IsEmpty(List L){
    return L->next == NULL;
}
/*Determine whether the Position->next is NULL or not (To judge last node)*/
int IsLast(Position P, List L){
    return P->next == NULL;
}
/*Insert X after P*/
void Insert(ElementType X, List L, Position P){
    Position addnode; /*Make additional node to insert*/
    addnode=(Position)malloc(sizeof(struct Node));
    addnode->element=X;
    addnode->next=P->next; /*addnode->next should get address of next node first*/
    P->next=addnode; /*And P->next get address of addnode*/
}
/*Print List*/
void PrintList(List L){
    if(IsEmpty(L)){ /*This means that List have only header and empty*/
        fprintf(fout, "empty list!");
    }
    else {
         Position P; /*P is temporary node which take another node of list*/
         P=L->next;
         while(P!=NULL){ /*Print P->element until P=NULL*/
             fprintf(fout, "key:%d ", P->element);
             P=P->next;
         }
    }
    fprintf(fout, "\n"); /*Last Enter*/
}
/*Delete Node which have X*/
void Delete(ElementType X, List L){
    Position P, P_a; /*P and P_a is temporary node which take another node of the list*/
    P=FindPrevious(X, L);
    if(IsLast(P, L)){ /*This means that P is a last node and FindPrevious cannot find X*/
        fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", X, X);
    }
    else { /*Delete P->next which have X*/
        P_a=P->next; /*P_a is needed for temporary save of P->next*/
        P->next=P_a->next;
        free(P_a);
    }
}
/*Find Postion of X*/
Position Find(ElementType X, List L){
    Position P; /*P is temporary node which take another node of the list*/
    P=L;
    while(P!=NULL && P->element!=X){
        P=P->next;
    } 
    return P; /*So, if there's no X, return NULL or if P->element=X, return P*/ 
}
/*Find Previous Position of X*/
Position FindPrevious(ElementType X, List L){
    Position P; /*P is temporary node which take another node of the list*/
    P=L;
    while(P->next!=NULL && P->next->element!=X){
        P=P->next;
    }
    return P; /*So, if there's no X, return P = Last Node, or if there's X, return P = Previous Node*/
}
/*Delete header and all node of list*/
void DeleteList(List L){
    Position P, P_A; /*P and P_A is temporary node which take another node of the list*/
    P=L->next;
    free(L); /*First, P saved header->next and free L*/
    while(P!=NULL){
        P_A=P->next;
        free(P);
        P=P_A; /*Second, P_A saved P->next and free P*/
    }
}

/*Main Function*/
int main(int argc, char **argv){
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");
    char x;

    Position header=NULL, tmp=NULL; /*Make header node pointer and tmp node pointer*/
    header = MakeEmpty(header); /*Make header node with basic data*/
    while(fscanf(fin, "%c", &x)!=EOF){
            if(x=='i'){ /*Insert a after b*/
                int a,b; fscanf(fin, "%d%d", &a, &b);
                tmp = Find(a, header); /*First, Check an existion of the a*/
                if(tmp!=NULL){
                    fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
                    continue;
                }
                tmp = Find(b, header); /*Second, Check an existion of the b*/
                if(tmp==NULL){
                    fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
                    continue;
                }
                Insert(a, header, tmp); /*Third, Insert a after b*/
            }
            else if(x=='d'){ /*Delete a*/
                int a;
                fscanf(fin, "%d", &a);
                Delete(a, header);
            }
            else if(x=='f'){ /*FindPrevious a*/
                int a;
                fscanf(fin, "%d", &a);
                tmp=FindPrevious(a, header); /*tmp is a node that point previous of a's node*/
                if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n", a, a); /*tmp cannot findprevious a*/
                else{ /*tmp findprevious a*/
                    if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
                    else fprintf(fout, "key of the previous node of %d is head\n", a);
                }
            }
            else if(x=='p') PrintList(header); /*Print List*/
    }
    DeleteList(header);
    fclose(fin);
    fclose(fout);
    return 0;
}

