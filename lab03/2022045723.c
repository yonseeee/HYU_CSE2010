#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			Push(stack,input_str[i]-'0');
		}
		else{
			int b = Pop(stack);
			int a = Pop(stack);
			switch (input_str[i]){
				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;
				case '/' : 
					if(b==0) {fprintf(fout,"\nerror : invalid postfix expression, divide by zero");//나누는 수가 0일 때 에러 메시지 출력 후
					exit(1);}//프로그램 강제종료
					Push(stack, a/b); 
					break;
				case '%' : 
					if(b==0) {fprintf(fout, "\nerror : invalid postfix expression, divide by zero");//나누는 수가 0일 때 에러 메시지 출력 후
					exit(1);}//프로그램 강제종료
					Push(stack, a%b); break;
				default : break;
			}
		}
		fprintf(fout, "%d ", Top(stack));
		i++;
	}
	if(stack->top>=0){//Stack 내부 원소가 1개보다 많은 상태에서 Expression이 종료되는 경우
		fprintf(fout, "\nerror : invalid postfix expression, %d elements are left", stack->top+1);
		exit(1);//에러 메시지 출력 후 강제종료
	}

	fprintf(fout, "\nevaluation result : %d\n", Top(stack));//정상적으로 작동했을 때 메시지, top 출력
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
	return 0;
}

Stack* CreateStack(int max){
    Stack *S=(Stack*)malloc(sizeof(Stack));// malloc으로 Stack 포인터 S 할당
    S->key=(int*)malloc(sizeof(int)*max);// S->key 를 max 크기의 int array pointer 할당 (malloc) ??
    S->max_stack_size=max;// S->max_stack_size 설정
    S->top=-1;// S->top 을 -1로 설정
    return S;// S 반환
}

void DeleteStack(Stack* S){
    free(S->key);// key를 free
    free(S);// S를 free
	return;
}

void Push(Stack* S, int X){
	if(IsFull(S)==true){//IsFull이 참일 때
		fprintf(fout,"\nerror : invalid postfix expression, stack is full");//에러 메시지 출력 후 
		exit(1);//프로그램 강제종료
	}

    S->top++;// S->top += 1
    S->key[S->top]=X;// S->key의 top 에 X 대입
	return;
}

int Pop(Stack* S){
	if(IsEmpty(S)==true){//IsEmpty가 참일 때
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty");//에러 메시지 출력 후 
		exit(1);//프로그램 강제종료
	}
	int result;
    result=S->key[S->top]; // S->key의 top
    S->top--;// S->top -= 1
	return result;//top 반환
}

int Top(Stack* S){
    return S->key[S->top];// S->key의 top 반환
}

bool IsFull(Stack* S){
	if(S->top>=S->max_stack_size)return true;//S->top이 S의 최대 크기를 넘어가면 true 반환
	else return false;//아니면 false 반환
}

bool IsEmpty(Stack* S){
	if(S->top==-1) return true;//S->top이 -1인 경우 스택이 비어있다는 걸 의미하므로 true 반환
	else return false;//아니면 false 반환
}
