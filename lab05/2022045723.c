#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
    if(current==NULL) {//if current == NULL일 경우 할당
		current=(BSTNode*)malloc(sizeof(BSTNode));
		current->value=key;
		current->left=NULL;
		current->right=NULL;
	}
    else if(key<current->value){//elif key < current->value 일 경우 recursive
		current->left=insertNode(current->left,key);
	}
    else if(key>current->value){//elif key > current->value 일 경우 recursive
		current->right=insertNode(current->right,key);
	}
    else //else 에러
	{
		fprintf(fout, "insertion error %d\n", key);
	}
    return current;  //return current
}
//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){
    if(current==NULL) return current; //current == NULL일경우 return current
    if(key < current->value){//if   key < current->value 일 경우 recursive
		current->left=deleteNode(current->left,key);
	}
    else if(key > current->value){//elif key > current->value 일 경우 recursive
		current->right=deleteNode(current->right,key);
	}
    else{//else 의 경우 4가지 (삭제할 노드를 찾음)
		if(current->left!=NULL&&current->right!=NULL){//자식이 둘일 경우
			BSTNode*p=current->right;//우측 자식 따로 저장
			while(p->left!=NULL)p=p->left;//우측 자식의 가장 좌측 자식 찾기
			current->value=p->value;//그 노드의 값으로 current->value 설정
			current->right=deleteNode(current->right,p->value);//우측 자식의 가장 좌측 자식 삭제 recursive
		}
		else if(current->left!=NULL){// 자식이 왼쪽 자식 하나일 경우 -> free current -> return 자식
			BSTNode*p=current->left;//왼쪽 자식 저장해놓고
			free(current);//free current
			return p;//return 자식
		}
		else if(current->right!=NULL){//자식이 오른쪽 자식 하나일 경우
			BSTNode*p=current->right;//오른쪽 자식 저장해놓고
			free(current);//free current
			return p;//자식 반환
		}
		else{    //  자식이 없을 경우 
			free(current); // free current
			return NULL; // return NULL
		}
	}
    return current;  //return current
}
BSTNode* findNode(BSTNode* current, int key){
    if(current==NULL) return current;//current == NULL일경우 return current

    if(key<current->value){//if   key < current->value 일 경우 current->left에 대해 recursive
		return findNode(current->left, key); 
		}
    else if (key>current->value){//elif key > current->value 일 경우 current->right에 대해 recursive
		return findNode(current->right, key);
		}
    else return current;//else return current
}
void printInorder(BSTNode* current){
	if(current!=NULL){//current가 존재하면
    printInorder(current->left);//왼쪽 트리 출력
    fprintf(fout, "%d ", current->value);//현재 트리 노드 출력
    printInorder(current->right);//오른쪽 트리 출력
    }
}
void deleteTree(BSTNode* current){
	if(current!=NULL){//current가 존재하면
		deleteTree(current->left);//왼쪽 트리 삭제
		deleteTree(current->right);//오른쪽 트리 삭제
		free(current);//free current
	}
}
void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}