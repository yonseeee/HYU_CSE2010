#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key;
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);


void Insert(LE* header, int inserted_key, int target_key){
    LE *target=Find(header,target_key);// Find로 target_key 노드 찾기
    
    if((target==NULL)&&(target_key!=-1)) //현재 linkedlist에 target_key가 없는지 확인, target_key가 -1일 경우 header 뒤에 insert해야하므로 따로 처리
    {   fprintf(fout, "insertion %d failed: can not find location\n", inserted_key);
        return;
    }
    if(Find(header, inserted_key)!=NULL){//현재 linkedlist에 inserted_key가 이미 있는지 확인
        fprintf(fout, "insertion %d failed: the key already exists\n", inserted_key);
        return;
    }
    //위 조건문 2개로 예외 상황 처리 완료

    LE *inserted=(LE*)malloc(sizeof(LE));// inserted_key를 malloc을 이용해 생성
    inserted->key=inserted_key;//inserted 노드의 key 값을 inserted_key로 설정
    inserted->next_pointer=NULL;//inserted 노드의 next_pointer를 NULL로 설정
    if(target_key==-1) { //target_key가 -1일 경우 header 뒤에 삽입
        inserted->next_pointer=header->next_pointer;//inserted_key 노드의 next_pointer를 header의 next_pointer로 저장
        header->next_pointer=inserted;//header의 next_pointer를 inserted_key의 메모리 주소로 저장
    }
    else{//target_key가 -1이 아닐 경우 
        inserted->next_pointer=target->next_pointer;// inserted_key 노드의 next_pointer 를 target_key의 next_pointer로 저장
        target->next_pointer=inserted;// target_key 노드의 next_pointer 를 inserted_key의 메모리 주소로 저장
    }
}

void Delete(LE* header, int target_key){
    if(Find(header, target_key)==NULL){//target_key가 linkedlist에 없을 때
        fprintf(fout, "deletion %d failed: node is not in the list\n", target_key);
        return;
    }
    LE *previous=FindPrevious(header,target_key);// FindPrevious를 통해 target_key의 직전 노드를 찾기
    LE *target=previous->next_pointer;// target_key의 노드도 next_pointer로 찾기
    previous->next_pointer=target->next_pointer;// FindPrevious의 next_pointer를 target_key의 next_pointer로 변경
    free(target);// target_key를 메모리 해제
}

LE* Find(LE* header, int target_key){
    LE *current=header;    // 포인터 current를 header로 설정
    while(current->next_pointer!=NULL)    // current 의 next_pointer가 NULL인지 체크
    {
        current=current->next_pointer;//current를 current의 next_pointer로 설정
        if(current->key==target_key) {//current의 key와 target_key가 같으면
            return current;// current를 반환
            }
    }
    return NULL;//target_key를 못 찾았을 경우 NULL을 반환
}

LE* FindPrevious(LE* header, int target_key){
    LE *current=header;// 포인터 current를 header로 설정

    while(current->next_pointer!=NULL){//current의 next_pointer가 NULL인지 확인
        if(current->next_pointer->key==target_key)//current의 next_pointer의 key값이 target_key와 같으면
        {
            return current; // target의 직전 노드 반환
        }
        current=current->next_pointer;//current를 current의 next_pointer로 설정
    }
    return NULL;//없으면 NULL 반환
    
}

void PrintPrevious(LE* header, int target_key){
    LE *previous=FindPrevious(header,target_key);//FindPrevious를 사용하여 target의 직전 노드 찾기
    if(previous==NULL)//target의 직전 노드 못 찾은 경우
    fprintf(fout, "finding %d failed : node is not in the list\n", target_key);
    else{//직전 노드 찾은 경우
        if(previous==header)//그 직전 노드가 header인 경우
        fprintf(fout, "previous node of %d is head\n", target_key);
        else fprintf(fout, "previous node of %d is %d\n", target_key, previous->key);//그 직전 노드가 header가 아닌 경우
     }
}

void PrintList(LE* header){
    LE *current=header;//포인터 current를 header로 설정
    if(header->next_pointer==NULL) fprintf(fout, "empty list\n");//linkedlist가 비어있는 경우
    else{//linkedlist가 비어있지 않은 경우
        while(current->next_pointer!=NULL){//current의 next_pointer가 NULL이 아닌지 확인
            current=current->next_pointer;//current를 current의 next_pointer로 설정
         fprintf(fout, "%d ", current->key);//current의 key 출력
        }
    fprintf(fout, "\n");//개행
    }
}

LE* CreateList(){
    LE *header=(LE*)malloc(sizeof(LE));//malloc을 이용하여 header 생성
    header->key=0;//header의 key를 0으로 초기화
    header->next_pointer=NULL;//header의 next_pointer를 NULL로 초기화
    return header;//header 반환
    
}

void DeleteList(LE* header){
    LE *current=header;//포인터 current를 header로 설정
    while(current!=NULL){//current가 NULL이 아닌지 확인
        LE *next=current->next_pointer;//current의 next_pointer를 next에 저장
        free(current);//current의 메모리 해제
        current=next;//current의 메모리 주소를 다시 next의 메모리주소로 설정
    }
    //free를 이용하여 header부터 끝까지 전부 해제
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

    LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return 0;
}