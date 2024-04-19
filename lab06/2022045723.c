#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
   Heap *H=(Heap*)malloc(sizeof(Heap));//힙 할당
   H->Capacity=heapSize+1;//0번 인덱스는 비워놓기 위해 capacity를 heapSize+1만큼으로 설정
   H->Size=0;
   H->Elements=(int*)malloc(sizeof(int)*(heapSize+1));//힙에서 사용할 배열 할당
   return H;
}
void DeleteHeap(Heap* heap){
	free(heap->Elements);//배열 메모리 해제
    free(heap);//힙 메모리 해제
}
void Push(Heap *heap, int value){
	if(heap->Capacity==heap->Size+1) {//heap이 꽉 차 있으면
		fprintf(fout, "push error : heap is full\n");//에러 메시지 출력
		return;//return
	}
	if(Find(heap, value)==true){//이미 value가 heap에 있으면
		fprintf(fout, "push error : %d is already in the heap\n", value);//에러 메시지 출력
	    return;//return
	}
	heap->Size++;//heap size 1만큼 증가
    
	int i=heap->Size;//heap의 마지막 원소 인덱스 
	int tmp;//swap시 필요한 변수
	// 반복:
	while((i!=1)&&(heap->Elements[i/2]<value)){//부모value보다 현재 value가 더 크면
		heap->Elements[i]=heap->Elements[i/2];//swap
		i=i/2;//percolateup
	}
	heap->Elements[i]=value;//while문에서 찾은 자리에 현재 value넣기
}
bool Find(Heap *heap, int value){
	for(int i=1;i<=heap->Size;i++){
		if(heap->Elements[i]==value){// heap->Elements 에서 값 찾기
			return true;//찾으먄 return true
		}
	}
    return false;//못찾았으면 return false
}
int Pop(Heap* heap){
	if(heap->Size==0){
		fprintf(fout,"pop error : heap is empty\n");
		return -INF;
	}
	int deletedKey=heap->Elements[1];
	//fprintf(fout, "[%d]", heap->Elements[1]);
	heap->Elements[1]=heap->Elements[heap->Size];
	heap->Size--;
    // heap->Elements[1]을 heap->Elements의 마지막 key와 변경
    // Size 변경
    // 반복:
	int i=1;
	while(1){
		
		int left=-INF; //     자식_왼쪽_value = -INF
		int right=-INF;   //     자식_오른쪽_value = -INF
		if(i*2<=heap->Size)   //     if 자식_왼쪽_있음
			left=heap->Elements[2*i];//         자식_왼쪽_value 설정
		if(i*2+1<=heap->Size)//     if 자식_오른쪽_있음
			right=heap->Elements[2*i+1];//         자식_오른쪽_value 설정

		if(left<right&&heap->Elements[i]<right){//if 자식_왼쪽_value < 자식_오른쪽_value and 현재_value < 자식_오른쪽_value
			int tmp=heap->Elements[i];	//swap
			heap->Elements[i]=heap->Elements[2*i+1];
			heap->Elements[2*i+1]=tmp;
			i=2*i+1;	//percolatedown
		}
		else if(left>right&&heap->Elements[i]<left){// elif 자식_오른쪽_value < 자식_왼쪽_value and 현재_value < 자식_왼쪽_value
			int tmp=heap->Elements[i];	//swap
			heap->Elements[i]=heap->Elements[2*i];
			heap->Elements[2*i]=tmp;
			i=2*i;	//percolatedown
		}
		else break;
	}


    return deletedKey;//삭제된 키 반환
	
}
void PrintHeap(Heap* heap){
	if(heap->Size==0){//heap이 비어있으면
		fprintf(fout, "print error : heap is empty\n");//에러 메시지 출력
		return;//return 
	}
	for(int i=1;i<=heap->Size;i++)
	fprintf(fout, "%d ", heap->Elements[i]);// heap->Elements 순서대로 print
	fprintf(fout,"\n");    
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Push(maxHeap, key);
				break;
			case 'd':
				max_element = Pop(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "%d is not in the heap\n", key);
				break;
		}
	}
	DeleteHeap(maxHeap);

	return 0;
}