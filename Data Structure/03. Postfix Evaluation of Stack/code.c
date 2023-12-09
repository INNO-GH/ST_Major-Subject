#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// This is for error_flag
#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;
// Make struct Stack type = Stack
typedef struct Stack{
	int *key; // key is array for stack
	int top; // top is top index
	int max_stack_size; // max_stack_size is capacity
}Stack;

// Part for declaring function
Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

// Main
void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack; // stack(struct Stack*)
	char input_str[101]; // input_str[0]~str[100], This is for get text contents
	int max=20, i=0,a,b,result, error_flag=0; // size=20, a and b is two top, result is Final answer, error_flag

	fgets(input_str,101,fin);
	stack = CreateStack(max); // First, make stack with max size
	
	fprintf(fout, "top numbers : ");
	while(input_str[i]!='#'){ // # means "This is Last!"
        // write code
        // It's for Number input (Push)
        if(!(input_str[i]=='+'||input_str[i]=='-'||input_str[i]=='*'||input_str[i]=='/'||input_str[i]=='%')){
            if(IsFull(stack)){
                error_flag = ISFULL; // check stack is Full -> Error    
            }
            else {
                Push(stack, input_str[i]-'0'); // -'0' is for the function that we should transform char type number to int type
            }
        }
        // It's for Operator input (Pop)
        else {
            if(IsEmpty(stack)||stack->top==0){ //I will check empty or just one element stack 
                error_flag = ISEMPTY;
            }
            else {
                a=stack->key[stack->top-1]; // Pick 2 Number on top
                b=stack->key[stack->top];
                if((input_str[i]=='/' || input_str[i]=='%') && b==0){ // check divided 0
                    error_flag = DIVIDEZERO;
                }
                else{ // Operator
                    Pop(stack); // Pop Two Number
                    Pop(stack);
                    if(input_str[i]=='+'){ // Push Operative Result
                        Push(stack, a+b);
                    }
                    else if(input_str[i]=='-'){
                        Push(stack, a-b);
                    }
                    else if(input_str[i]=='*'){
                        Push(stack, a*b);
                    }
                    else if(input_str[i]=='/'){
                        Push(stack, a/b);
                    }
                    else if(input_str[i]=='%'){
                        Push(stack, a%b);
                    }
                }
            }
        }
        // write code

		if(error_flag) break; // If there's error -> break

		int t = Top(stack); // get Top
		fprintf(fout, "%d ",t); // Print Top
		result=t; // result means 'Final result'
		i++;
	}

	if(error_flag == ISFULL){ // 1. error with Full Stack (Push)
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if(error_flag == ISEMPTY){ // 2. error with Empty or one element Stack (Pop)
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n"); // In fact, we should print 'stack is empty or just has one element!', But I wiil not touch skeleton!
	}
	else if(error_flag == DIVIDEZERO){ // 3. error with dividied by 0 (Pop with operator)
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}

	else{
		if(stack->top+1 > 1){ // 4. error with more than 1 elements when finished
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
		}
		else{ // 5. Non-error. Print Result
			fprintf(fout, "\nevaluation result : %d\n",result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack); // Delete Stack
}

Stack* CreateStack(int max){ // Create a new stack with max
    Stack *S=(Stack *)malloc(sizeof(Stack));
    S->key=(int *)malloc(sizeof(int)*max);
    S->max_stack_size=max;
    S->top=-1;
    return S;
}

void DeleteStack(Stack* S){ // free all memeory of stack
    free(S->key);
    free(S);
}

void Push(Stack* S, int X){ // If there's number, Push! (But if stack is full, error)
    S->key[++S->top]=X; // add 1 to top index, and put value in top
}

int Pop(Stack* S){ // If there's operator, Pop! (But, if stack doesn't have enough element or Divider = 0, error)
    S->key[S->top--]=0;
    return 0;
}

int Top(Stack* S){ // Just Give Top
    return S->key[S->top];
}

int IsFull(Stack* S){ // Check Stack's Full
    return S->top==S->max_stack_size-1;
}

int IsEmpty(Stack* S){ // Check Stack's empty
    return S->top==-1;
}
