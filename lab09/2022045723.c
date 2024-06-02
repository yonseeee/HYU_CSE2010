#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct Node {
	int index; // index of this node
	int dist;  // distance for heap
	int prev;
} Node;

typedef struct Graph {
	int size;
	int** distMatrix;
	Node** nodes;
} Graph;

typedef struct Heap {
	int capacity;
	int size;
	Node** element;
} Heap;

// Function declarations
Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
Node* Pop(Heap* heap);

//구현하면 좋은 함수
bool isEmpty(Heap* heap);
void swap(Node** node1, Node** node2);


void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = CreateGraph(size);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->distMatrix[node1 - 1][node2 - 1] = weight;
		tmp = fgetc(fin);
	}

	PrintShortestPath(g);

	DeleteGraph(g);
}

Graph* CreateGraph(int size) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));//Graph할당
	graph->size = size;
	graph->distMatrix = (int**)malloc(size * sizeof(int*));//인접행렬
	graph->nodes = (Node**)malloc(size * sizeof(Node*));//노드배열
	for (int i = 0; i < size; i++) {
		graph->distMatrix[i] = (int*)malloc(size * sizeof(int));
		for (int j = 0; j < size; j++) {//인접행렬 초기화
			if(i==j) graph->distMatrix[i][j] = 0;//같은 노드면 0으로
			else graph->distMatrix[i][j] = INF;//아니면 무한대
		}
		graph->nodes[i] = (Node*)malloc(sizeof(Node));
		graph->nodes[i]->index = i;
		graph->nodes[i]->dist = INF;
		graph->nodes[i]->prev = -1;
	}
	return graph;
}

void DeleteGraph(Graph* graph) {
	for (int i = 0; i < graph->size; i++) {
		free(graph->distMatrix[i]);//인접행렬의 각 행 해제
		free(graph->nodes[i]);//노드 배열의 각 노드 해제
	}
	free(graph->distMatrix);//인접행렬 배열 해제
	free(graph->nodes);//노드 배열 해제
	free(graph);//그래프 해제
}

Heap* CreateHeap(int heapSize) {
	Heap* heap = (Heap*)malloc(sizeof(Heap));//힙 할당
	heap->capacity = heapSize;
	heap->size = 0;//비어있음
	heap->element = (Node**)malloc(heapSize * sizeof(Node*));
	return heap;
}

void DeleteHeap(Heap* heap) {//힙 삭제
	free(heap->element);
	free(heap);
}

bool isEmpty(Heap* heap) {
	if(heap->size==0)return true;//heap->size가 0이면 비어있음, true반환
	else return false;//아니면 false 반환
}

void swap(Node** node1, Node** node2) {//swap
	Node* tmp = *node1;
	*node1 = *node2;
	*node2 = tmp;
}

void Push(Heap* heap, Node* node) {
	if (heap->size == heap->capacity) {//힙이 꽉 찬 경우 예외처리
		return;
	}
	heap->element[heap->size] = node;//힙의 맨 끝에 삽입
	int index = heap->size;//방금 삽입된 노드의 인덱스로 index
	while (index > 0 && heap->element[index]->dist<heap->element[(index - 1) / 2]->dist) {//부모 거리보다 작은 동안
		swap(&heap->element[index], &heap->element[(index-1)/2]);//swap
		index = (index - 1) / 2;//percolate up
	}
	heap->size++;//heap 크기 ++
}

Node* Pop(Heap* heap) {
	if (isEmpty(heap)) {//heap 비었을 때 예외처리
		return NULL;
	}
	Node* result = heap->element[0];//루트 노드에 있는 것 제거할 것이다
	heap->element[0] = heap->element[--heap->size];//마지막 원소를 루트로 이동, 힙 크기는 -1
	int index = 0;//루트 노드를 인덱스로
	while (2 * index + 1 < heap->size) {
		int left = 2 * index + 1;//왼쪽자식
		int right = 2 * index + 2;//오른쪽자식
		int x = left;//자식 중 작은값
		if (right < heap->size && heap->element[right]->dist < heap->element[left]->dist) {//오른쪽이 왼쪽보다 작으면
			x = right;//x를 right로 바꿈
		}
		if (heap->element[index]->dist < heap->element[x]->dist) {//힙 Property
			break;
		}
		swap(&heap->element[index], &heap->element[x]);//swap
		index = x;
	}
	return result;
}

void PrintShortestPath(Graph* graph) {
	int start=1;//시작을 1번으로
	graph->nodes[start-1]->dist = 0;//시작노드부터의 거리가 0
	Heap* heap = CreateHeap(graph->size);//heap만들고
	Push(heap, graph->nodes[start-1]);//시작 노드를 push

	while (!isEmpty(heap)) {//heap이 안 비어있을 동안
		Node* u = Pop(heap);//u를 pop
		for (int v = 0; v < graph->size; v++) {
			if (u->index != v && graph->distMatrix[u->index][v] != INF) {//u에서v로 가는간선 유
				int x = u->dist + graph->distMatrix[u->index][v];//u->v거리
				if (x < graph->nodes[v]->dist) {//현재 v까지의 거리보다 작으면
					graph->nodes[v]->dist = x;//dist를 x로 업데이트
					graph->nodes[v]->prev = u->index;//prevfmf u의 인덱스로 업데이트
					Push(heap, graph->nodes[v]);//v를 push
				}
			}
		}
	}

	for (int i = 1; i < graph->size; i++) {
		//fprintf(fout, "1 to %d: ", i);
		if (graph->nodes[i]->dist == INF) {//거리가 무한대면 경로 없음
			fprintf(fout, "can not reach to node %d", i+1);
		} else {//경로 있을 때
			//fprintf(fout, "distance = %d, path = ", graph->nodes[i]->dist);
			int route[graph->size];//경로 저장할 배열
			int cnt = 0;
			for (int j = i; j != -1; j = graph->nodes[j]->prev) {//prev를 거슬러 올라가며
				route[cnt++] = j;//route에 인덱스를 저장
			}
			for (int j = cnt - 1; j >= 0; j--) {//경로랑 거꾸로
				fprintf(fout, "%d", route[j]+1);
				if (j > 0) {
					fprintf(fout, "->");
				}
			}
			fprintf(fout, " (cost : %d)\n", graph->nodes[i]->dist);
		}
	}

	DeleteHeap(heap);//힙 삭제
}
