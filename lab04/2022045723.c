#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);


void DeleteTree(TREE* current){
    if(current!=NULL){//current가 존재하면
    DeleteTree(current->left_pointer);//왼쪽 트리 삭제
    DeleteTree(current->right_pointer);//오른쪽 트리 삭제
    free(current);}//현재 트리 노드 삭제
}
void printPreorder(TREE* current){
    if(current!=NULL){//current가 존재하면
    fprintf(fout, "%d ", current->data);//현재 트리 노드 출력
    printPreorder(current->left_pointer);//왼쪽 트리 출력
    printPreorder(current->right_pointer);//오른쪽 트리 출력
    }
}
void printInorder(TREE* current){
    if(current!=NULL){//current가 존재하면
    printInorder(current->left_pointer);//왼쪽 트리 출력
    fprintf(fout, "%d ", current->data);//현재 트리 노드 출력
    printInorder(current->right_pointer);//오른쪽 트리 출력
    }
}
void printPostorder(TREE* current){
    if(current!=NULL){//current가 존재하면
    printPostorder(current->left_pointer);//왼쪽 트리 출력
    printPostorder(current->right_pointer);//오른쪽 트리 출력
    fprintf(fout, "%d ", current->data);//현재 트리 노드 출력
    }
}
void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){
    current->data=inserted_data_array[current_index];
    current->left_pointer=NULL;
    current->right_pointer=NULL;//현재 트리 노드 데이터 설정
    //
    if(current_index*2<length){//if 왼쪽 노드 가능할 경우
    TREE* left=(TREE*)malloc(sizeof(TREE));
    current->left_pointer=left;//왼쪽 노드 메모리 할당
    CreateFromArray(left,current_index*2,inserted_data_array,length);//왼쪽 노드에 대한 재귀
    }//if-end
    //
    if(current_index*2+1<length){//if 오른쪽 노드 가능할 경우
    TREE*right=(TREE*)malloc(sizeof(TREE));
    current->right_pointer=right;//오른쪽 노드 메모리 할당
    CreateFromArray(right,current_index*2+1,inserted_data_array,length);//오른쪽 노드에 대한 재귀
    }//if-end
    return;
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
	    fscanf(fin, "%d", &input_data);
	    array[i + 1] = input_data;
	}
	TREE* header = malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1);
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
	DeleteTree(header);
	fclose(fin);
	fclose(fout);
	return 0;
}