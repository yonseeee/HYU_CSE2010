#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

/*
구현해야 하는 함수들
*/
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void TopologicalSort(Graph* G);
Queue* CreateQueue();
bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
/*
구현하면 좋은 함수들
*/
// Indegree 배열 설정
void SetIndegree(Graph* G, int* idg);
// Bubble sort도 무방
void SortNode(int* arr, int n);



void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){
    Graph* g = (Graph*)malloc(sizeof(Graph));//graph malloc
    if(g==NULL) return NULL;
    g->node=nodes;
    g->size=n;
    g->matrix=(bool**)malloc(sizeof(bool*)*n);//matrix의 경우는 bool 2차원 array

    for(int i=0;i<n;i++){
        g->matrix[i]=(bool*)malloc(sizeof(bool)*n);
        for(int j=0;j<n;j++)g->matrix[i][j]=false;
    }
    // -> malloc으로 bool* 우선 할당 후 for문으로 내부 bool 크기만큼 할당
    return g;
}

void InsertEdge(Graph* G, int a, int b){
    // a, b값과 index값은 다르므로 G->node에서 찾기
    int a2 = -1, b2 = -1;
    for(int i=0;i<G->size;i++){
        if(G->node[i]==a) a2 = i;
        if(G->node[i]==b)b2 = i;
    }
    if(a2!=-1 && b2!=-1)
    G->matrix[a2][b2]=true;
    // 해당 값을 a', b'라 하면 G->matrix[a'][b'] = true
    // G->matrix[b'][a'] 안하는 이유는 '방향 그래프'라서서
}

void PrintGraph(Graph* G){
    if(G==NULL)return;//G가 비었을 때 처리
    // 출력(띄어쓰기에 주의)
    // 0 2 4 5 6
    // 2 0 0 0 1
    // 4 0 0 0 0
    // 5 1 0 1 0
    // 6 0 0 1 1

    // 위와 같은 형태로 출력이 목표(bool 은 %d로 0 혹은 1로 출력 가능)
    fprintf(fout, "0 ");
    for(int i=0;i<G->size;i++)fprintf(fout, "%d ", G->node[i]);
    fprintf(fout,"\n");

    for(int i=0;i<G->size;i++){
        fprintf(fout, "%d ", G->node[i]);
        for(int j=0;j<G->size;j++)
            fprintf(fout, "%d ", G->matrix[i][j]);
        fprintf(fout, "\n");
    }
}

void DeleteGraph(Graph* G){
    //G 내부의 malloc 포인터들부터 free 후 G free
    for(int i=0;i<G->size;i++)free(G->matrix[i]); //matrix의 경우 for문으로 matrix[i]를 free 후 matrix를 free
   free(G->matrix);
   free(G->node);
   free(G);
}

void TopologicalSort(Graph* G){
    //Queue, printorder, printorder_size 초기화 -> printorder, printorder_size의 경우 오류 발생시 오류 메시지만을 출력하기 위함함
    Queue* q=CreateQueue();
    int* printorder=(int*)malloc(sizeof(int)*(G->size));
    int printorder_size=0;
    int* indegree=(int*)malloc(sizeof(int)*(G->size));
    SetIndegree(G,indegree);
    //indegree의 경우 setindegree 사용해서 초기화
    
    for(int i=0;i<G->size;i++)
        if(indegree[i]==0)
            Enqueue(q,i);//indegree 가 0인 노드 인덱스를 queue에 삽입
    while(!IsEmpty(q)){//queue가 비어있을 때까지
        int x;
        x=Dequeue(q);
        printorder[printorder_size++]=G->node[x];

        for(int i=0;i<G->size;i++) 
            if(G->matrix[x][i]==1){
                indegree[i]--;//     Dequeue 후 해당 노드를 선행으로 하는 노드를 찾기(matrix[i][j] 이용) -> 그 노드의 indegree를 1개 줄이기
                if(indegree[i]==0)Enqueue(q,i);//     만약 그 노드의 indegree가 0이면 queue에 삽입
            }
    }
    if((G->size)!=printorder_size)fprintf(fout, "sorting error : cycle\n");    //사이클 판단(size, G->size와 비교)
    else {
        for(int i=0;i<printorder_size;i++)fprintf(fout, "%d ", printorder[i]);
        fprintf(fout, "\n");   //에러메시지나 정상적인 값 출력
    }
    
    free(indegree);
    free(printorder);
    DeleteQueue(q);
    //indegree, printorder, queue 전부 free(queue는 deletequeue 사용)
}

Queue* CreateQueue(){
    Queue* q = (Queue*)malloc(sizeof(Queue));//queue malloc
    q->front=NULL;//여기서 queue는 linked list로 구현
    q->rear=NULL;
    q->size=0;
    return q;
}

bool IsEmpty(Queue* Q){
    if(Q->size==0) return true;
    else return false;//size 0인지 체크
}

int Dequeue(Queue* Q){
    if(IsEmpty(Q)) return -1;
    QueueNode* p = Q->front;//queue 맨 앞쪽의 노드를 꺼내기
    Q->size--;//size --
    int x = p->data;

    Q->front=Q->front->next;
    if(Q->front==NULL)Q->rear=NULL; //linked list때와 동일하게 Q->front 포인터를 갱신(빼면 비게될 경우 front, rear 둘 다 NULL로)
    
    free(p);
    
    return x;
}

void Enqueue(Queue* Q, int X){
    QueueNode* p = (QueueNode*)malloc(sizeof(QueueNode));
    p->data=X;
    p->next=NULL;

    if(IsEmpty(Q)){//Q가 empty할때
        Q->front=p;
        Q->rear=p;
    }
    else{//queue 맨 뒤에 노드를 삽입
        Q->rear->next=p;
        Q->rear=p;    //linked list때와 동일하게 Q->rear->next와 Q->rear 포인터를 갱신(빈 queue에 추가할 경우 front, rear 둘 다 새로 만든 node로)
    }
    Q->size++;    //size ++


}

void DeleteQueue(Queue* Q){
    if(Q==NULL)return;//G가 비어있을 때 처리
    while(!IsEmpty(Q)) Dequeue(Q);//IsEmpty일 때까지 Dequeue 후 free
    free(Q);
}

void SetIndegree(Graph* G, int* idg){
    for(int i=0;i<G->size;i++)idg[i]=0;//Indegree array를 초기화

    for(int i=0;i<G->size;i++)  //2중 for문
        for(int j=0;j<G->size;j++)
            if(G->matrix[j][i]==true)idg[i]++;//G->matrix[j][i]가 true면 idg[i]++
  
 
}

void SortNode(int* arr, int n){
    //입력값의 순서가 정해저 있지 않음. 따라서 정렬이 필요요
    //간단한 버블 소트도 n이 작으므로 충분히 가능

    int tmp;//버블
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-1-i;j++)
                if(arr[j]>arr[j+1]){
                    tmp=arr[j];
                    arr[j]=arr[j+1];
                    arr[j+1]=tmp;
                }
}

