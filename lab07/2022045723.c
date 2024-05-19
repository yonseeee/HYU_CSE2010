#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

//구현하면 좋은 함수
//getHeight, max, updateHeight, rebalance
int getHeight(AVLNode *node){//node의 height를 반환하는 함수
    if(node==NULL) return 0; //트리가 비어있으면 높이는 0에서 시작
    return node->height;//height 반환
}

int max(int a, int b){//두 정수 중 큰 값 구하는 함수
    if(a>b)return a;
    else return b;
}

void updateHeight(AVLNode *node){//height를 update하는 함수
    //printf("[%d %d]", getHeight(node->left), getHeight(node->right));
    int higher=max(getHeight(node->left),getHeight(node->right));
    node->height=higher+1;//왼쪽 자식과 오른쪽 자식 중 더 큰 높이보다 1만큼 큰 값이 height가 된다
}

AVLNode* RotateRight(AVLNode* node){//우회전
    AVLNode *L=node->left;//node->left를 L이라 할 때
    node->left=L->right;//node->left를 L->right로
    L->right=node;//L->right를 node로
    updateHeight(node);//node의 height update
    updateHeight(L);//L의 height update
    return L;//return L
}

AVLNode* RotateLeft(AVLNode* node){//좌회전
    AVLNode *R=node->right;//node->right를 R이라 할 때
    node->right=R->left;//node->right를 R->left로
    R->left=node;//R->left를 node로
    updateHeight(node);//node의 height update
    updateHeight(R);//p와 R의 height update
    return R;//return R;
}

AVLNode* rebalance(AVLNode* T){
    if(getHeight(T->left)>getHeight(T->right)+1){// if 왼쪽 높이 > 오른쪽 높이 + 1 일 경우
        if(getHeight(T->left->left)<getHeight(T->left->right))T->left=RotateLeft(T->left);//     만약 LR 케이스면 좌회전
        T=RotateRight(T);//     우회전
    }
    else if(getHeight(T->left)+1 < getHeight(T->right)){// elif 왼쪽 높이 + 1 < 오른쪽 높이 일 경우 
        if(getHeight(T->right->left)>getHeight(T->right->right)) T->right=RotateRight(T->right);//     만약 RL 케이스면 우회전
        T=RotateLeft(T);//     좌회전
    }
    updateHeight(T);// 높이 갱신
    return T;
}

AVLNode* Insert(Element X, AVLNode* T){
    if(T==NULL){//NULL이면 AVLNode 생성 후 반환
        AVLNode*node=(AVLNode*)malloc(sizeof(AVLNode));
        node->element=X;
        node->left=NULL;
        node->right=NULL;
        node->height=1;
        return node;
    }
    if(X<T->element)T->left=Insert(X,T->left);//T->element보다 작으면 recursive하게 찾고,T->left = Insert(X, T->left) 형태로 포인터 갱신
    else if(X>T->element)T->right=Insert(X,T->right);//T->element보다 크면 recursive하게 찾음
    else{//T->element == X면 삽입 오류
        fprintf(fout,"insertion error : %d is already in the tree\n", X);
        }
    updateHeight(T);//T의 height update
    return rebalance(T);//rebalance 해줘야 함(삽입 시 균형 망가질 수 있음)
}

AVLNode* Delete(Element X, AVLNode* T){
    if(T==NULL){//NULL이면 오류
        fprintf(fout, "deletion error : %d is not in the tree\n",X);
        return T;
    }
    if(X<T->element)T->left=Delete(X,T->left);//Insert와 동일하게 recursive
    else if(X>T->element)T->right=Delete(X,T->right);
    else{//T->element == X면 BST 삭제 과정
    	if(T->left!=NULL&&T->right!=NULL){//자식이 둘일 경우
			AVLNode*p=T->right;//우측 자식 따로 저장
			while(p->left!=NULL)p=p->left;//우측 자식의 가장 좌측 자식 찾기
			T->element=p->element;//그 노드의 값으로 T->element 설정
			T->right=Delete(p->element,T->right);//우측 자식의 가장 좌측 자식 삭제 recursive
		}
		else if(T->left!=NULL){// 자식이 왼쪽 자식 하나일 경우 -> free T -> return 자식
			AVLNode*p=T->left;//왼쪽 자식 저장해놓고
			free(T);//free T
			return p;//return 자식
		}
		else if(T->right!=NULL){//자식이 오른쪽 자식 하나일 경우
			AVLNode*p=T->right;//오른쪽 자식 저장해놓고
			free(T);//free T
			return p;//자식 반환
		}
		else{    //  자식이 없을 경우 
			free(T); // free T
			return NULL; // return NULL
		}
    }
    //마지막으로 해당 노드를 서브트리의 루트로 rebalance
    if (T) {
        updateHeight(T);
        return rebalance(T);
    } 
    else return NULL;
}

void PrintInorder(AVLNode* T){//Inorder로 출력
    if (T) {
        PrintInorder(T->left);
        fprintf(fout, "%d(%d) ", T->element, T->height);
        PrintInorder(T->right);
    }
}

void DeleteTree(AVLNode* T){//postorder로 free
    if (T) {
        DeleteTree(T->left);
        DeleteTree(T->right);
        free(T);
    }
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}
