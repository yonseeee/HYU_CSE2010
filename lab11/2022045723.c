#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_cell;
	int* ptr_cell;
	int size_wall;
	bool* ptr_wall;
}DisjointSets;

DisjointSets* init(int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets);

void swap(int *x, int *y) {
	int tmp=*x;
	*x=*y;
	*y=tmp;
}

int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	fscanf(fin, "%d", &num);

	DisjointSets* sets = init(num);
	createMaze(sets, num);
	printMaze(sets, num);

	freeMaze(sets);

	fclose(fin);
	fclose(fout);

	return 0;
}

DisjointSets* init(int num) {
	DisjointSets*sets=(DisjointSets*)malloc(sizeof(DisjointSets));
	sets->size_cell=num*num;   // num * num 으로 size_cell, ptr_cell 초기화
	sets->ptr_cell=(int*)malloc(sizeof(int)*sets->size_cell);
	for(int i=1;i<sets->size_cell;i++)sets->ptr_cell[i]=i;
	sets->size_wall=num*num*2;    // num * num * 2 로 size_wall, ptr_wall 초기화(i 번째 셀의 우측은 i*2, 하단은 i*2+1로 정의)
	sets->ptr_wall=(bool*)malloc(sizeof(bool)*sets->size_wall);
	for(int i=0;i<sets->size_wall;i++)sets->ptr_wall[i]=true;//true면 벽이 있는거

	sets->ptr_wall[sets->size_wall-2]=false;    // sets->size_wall-2 = 0 (최우측 하단은 출구)
 
	return sets;
}

void Union(DisjointSets *sets, int i, int j) {
	int pi=find(sets,i);    //pi = find i
	int pj=find(sets,j);    //pj = find j
	sets->ptr_cell[pi]=pj;    //ptr_cell[pi] = pj
}

int find(DisjointSets *sets, int i) {
	if(sets->ptr_cell[i]==i)return i;    //ptr_cell[i] 가 자기 자신이면 자기 자신을 반환
	sets->ptr_cell[i]=find(sets,sets->ptr_cell[i]);    //ptr_cell[i] 을 find(ptr_cell[i])로 갱신
	
	return sets->ptr_cell[i];    //ptr_cell[i] 반환
}

void shuffle(int* array, int num){
	for(int i=num-1;i>=0;i--){//뒤에서부터 순회
		int r = rand()%(i+1);//해당 인덱스까지의 나머지만 나옴
		swap(&array[i], &array[r]);//그 인덱스의 원소랑 swap
	}
}

void createMaze(DisjointSets *sets, int num) {

	int size=num*num;	//i번째 배열이 i로 차있는 num*num 만큼의 배열 선언
	int*array=(int*)malloc(sizeof(int)*size);
	for(int i=0;i<size;i++)array[i]=i;

	shuffle(array,size);    //위의 배열 shuffle 

	//for(int i=0;i<size;i++)
	//printf("%d ", array[i]);

	for(int i=0;i<size;i++){
		int index=array[i];    //shuffle된 배열에서 처음부터 끝까지 뽑기
		int x=index%num;//셀을 (x,y)로 보고 위치를 각각 x,y에 저장
		int y=index/num;
		if(x+1<num&&find(sets,index)!=find(sets,index+1)){	//  우측 벽 없앨 수 있을 때(maze 밖이 아니고, x + 1이 num을 넘지 않고, 같은 group이 아닐 때)
			Union(sets,index,index+1);	//    union
			sets->ptr_wall[index*2]=false;//그 사이 벽 제거
		}
		if(y+1<num&&find(sets,index)!=find(sets,index+num)){	//  하단 벽 없앨 수 있을 때(maze 밖이 아니고, y + 1이이 num을 넘지 않고, 같은 group이 아닐 때)
			Union(sets, index, index+num);//    union
			sets->ptr_wall[2*index+1]=false;// 그 사이 벽 제거
		}
	}
}

void printMaze(DisjointSets *sets, int num) {
    for (int i = 0; i < num; i++) fprintf(fout, "+---");//맨 윗줄 출력
	fprintf(fout, "+\n");

	for(int j=0;j<num;j++){    //loop:
		if(j!=0)fprintf(fout, "|");
		else fprintf(fout, " ");//맨 위 맨 왼쪽 칸 왼쪽 벽은 공백으로
		for(int i=0;i<num;i++){
			int index=j*num+i;
			if(sets->ptr_wall[index*2]==true)fprintf(fout, "   |");    //  | 혹은 공백 출력
			else{
				fprintf(fout, "    ");
			}
		}
		fprintf(fout, "\n");
		for(int i=0;i<num;i++){
			int index=j*num+i;
			if(sets->ptr_wall[2*index+1]==true){    //  +--- 혹은 +   출력
				fprintf(fout, "+---");
			}else{
				fprintf(fout, "+   ");
			}
		}
		fprintf(fout, "+\n");    //  + 출력
	}
}

void freeMaze(DisjointSets *sets) {
    free(sets->ptr_cell);//free ptr_cell
	free(sets->ptr_wall);//free ptr_wall
	free(sets);//free sets
}
