#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

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

int FindIndex(int* arr, int size, int value){
    for (int i = 0; i <size; i++){
        if (arr[i] == value){
            return i;
        }
    }
    return -1;
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


Graph* CreateGraph(int* nodes, int n){   // 새로운 graph 생성하는 함수
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (g == NULL) return NULL;          // 동적할당실패 예외 처리
    g->size = n;
    g->node = nodes; 
    g->matrix = (bool**)malloc(n*sizeof(bool*));
    for (int i = 0; i < n; i++){
        g->matrix[i] = (bool*)malloc(n*sizeof(bool));
        for (int j = 0; j < n; j++){
            g->matrix[i][j] = false;
        } 
    }
    //graph malloc
    //matrix의 경우는 bool 2차원 array
    // -> malloc으로 bool* 우선 할당 후 for문으로 내부 bool 크기만큼 할당
    return g;
}

void InsertEdge(Graph* G, int a, int b){
    int ia = FindIndex(G->node, G->size, a);
    int ib = FindIndex(G->node, G->size, b);

    if (ia != -1 && ib != -1){
        G->matrix[ia][ib] = true;
    }
    // a, b값과 index값은 다르므로 G->node에서 찾기
    // 해당 값을 a', b'라 하면 G->matrix[a'][b'] = true
    // G->matrix[b'][a'] 안하는 이유는 '방향 그래프'라서서
}

void PrintGraph(Graph* G){  // 그래프의 matrix 출력 함수
    if (G == NULL) return;  // 예외처리

    fprintf(fout, "0 ");
    for (int i = 0; i < G->size; i++){
        fprintf(fout, "%d ", G->node[i]);
    }
    fprintf(fout, "\n");

    for (int i = 0; i < G->size; i++){
        fprintf(fout, "%d ", G->node[i]);
        for (int j = 0; j < G->size; j++){
            fprintf(fout, "%d ", G->matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    // 출력(띄어쓰기에 주의)
    // 0 2 4 5 6
    // 2 0 0 0 1
    // 4 0 0 0 0
    // 5 1 0 1 0
    // 6 0 0 1 1
    // 위와 같은 형태로 출력이 목표(bool 은 %d로 0 혹은 1로 출력 가능)
}

void DeleteGraph(Graph* G){
    for (int i = 0; i < G->size; i++){
        free(G->matrix[i]);
    }
    free(G->matrix);
    free(G->node);
    free(G);
    //G 내부의 malloc 포인터들부터 free 후 G free
    //matrix의 경우 for문으로 matrix[i]를 free 후 matrix를 free
}

void TopologicalSort(Graph* G){
    int* indegree = (int*)malloc((G->size)*sizeof(int));
    SetIndegree(G, indegree);
    Queue* q = CreateQueue();
    int* printOrder = (int*)malloc((G->size)*sizeof(int));
    int printOrderSize = 0;
    
    for (int i = 0; i < G->size; i++){
        if (indegree[i] == 0){
            Enqueue(q, i);
        }
    }

    while (!IsEmpty(q)){
        int u = Dequeue(q);
        printOrder[printOrderSize++] = G->node[u];

        for (int v = 0; v < G->size; v++){
            if (G->matrix[u][v]){
                indegree[v]--;
                if (indegree[v] == 0){
                    Enqueue(q,v);
                }
            }
        }
    }

    if (printOrderSize != G->size){
        fprintf(fout, "sorting error : cycle\n");
    }
    else {
        for (int i = 0; i < printOrderSize; i++){
            fprintf(fout, "%d ", printOrder[i]);
        }
        fprintf(fout, "\n");
    }

    free(indegree);
    free(printOrder);
    DeleteQueue(q);
    
    //Queue, printorder, printorder_size 초기화 -> printorder, printorder_size의 경우 오류 발생시 오류 메시지만을 출력하기 위함함
    //indegree의 경우 setindegree 사용해서 초기화
    
    //indegree 가 0인 노드 인덱스를 queue에 삽입
    //queue가 비어있을 때까지
    //     Dequeue 후 해당 노드를 선행으로 하는 노드를 찾기(matrix[i][j] 이용) -> 그 노드의 indegree를 1개 줄이기
    //     만약 그 노드의 indegree가 0이면 queue에 삽입
    
    //사이클 판단(size, G->size와 비교)
    //에러메시지나 정상적인 값 출력
    
    //indegree, printorder, queue 전부 free(queue는 deletequeue 사용)
}

Queue* CreateQueue(){      // Queue를 생성하는 함수
    Queue* Q = (Queue*)malloc(sizeof(Queue));
    Q->front = Q->rear = NULL;
    Q->size = 0;
    return Q;
    //queue malloc
    //여기서 queue는 linked list로 구현
}

bool IsEmpty(Queue* Q){
    //size 0인지 체크
    return Q->size == 0;
}

int Dequeue(Queue* Q){
    if (IsEmpty(Q)) return -1;
    QueueNode* temp = Q->front;
    int data = temp->data;
    Q->front = Q->front->next;
    if (Q->front == NULL) Q->rear = NULL;
    free(temp);
    Q->size--;
    return data;
    //queue 맨 앞쪽의 노드를 꺼내기
    //size --
    //linked list때와 동일하게 Q->front 포인터를 갱신(빼면 비게될 경우 front, rear 둘 다 NULL로)
}

void Enqueue(Queue* Q, int X){
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->data = X;
    temp->next = NULL;
    if (IsEmpty(Q)){
        Q->front = Q->rear = temp; 
    }
    else {
        Q->rear->next = temp;
        Q->rear = temp;
    }
    Q->size++;
    //queue 맨 뒤에 노드를 삽입
    //size ++
    //linked list때와 동일하게 Q->rear->next와 Q->rear 포인터를 갱신(빈 queue에 추가할 경우 front, rear 둘 다 새로 만든 node로)
}

void DeleteQueue(Queue* Q){
    if (Q == NULL) return; 
    while (!IsEmpty(Q)){
        Dequeue(Q);
    }
    free(Q);
    //IsEmpty일 때까지 Dequeue 후 free
}

void SetIndegree(Graph* G, int* idg){
    for (int i = 0; i < G->size; i++){
        idg[i] = 0;
    } 
    for (int i = 0; i < G->size; i++){
        for (int j = 0; j < G->size; j++){
            if (G->matrix[i][j]){
                idg[j]++;
            }
        }
    }
    //Indegree array를 초기화
    //2중 for문
    //G->matrix[j][i]가 true면 idg[i]++
}

void SortNode(int* arr, int n){             // n개의 숫자로 이루어진 배열을 오름차순으로 정렬하는 함수
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n- i - 1; j++){
            if (arr[j] > arr[j + 1]){
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    //입력값의 순서가 정해저 있지 않음. 따라서 정렬이 필요요
    //간단한 버블 소트도 n이 작으므로 충분히 가능
}