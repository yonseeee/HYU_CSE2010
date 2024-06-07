#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array *array=(Array*)malloc(sizeof(Array));//array할당
    array->size=size;
    array->values=(int*)malloc(sizeof(int)*size);
    return array;
}

void QuickSort(Array* array, int left, int right){
    if(left<right){
    int pivot=Partition(array,left,right);//pivot 찾기
    QuickSort(array,left,pivot-1);//pivot 기준 왼쪽 Quicksort
    QuickSort(array,pivot+1,right);//pivot 기준 오른쪽 Quicksort
    }
}

int Partition(Array* array, int left, int right){
    int pivot=array->values[right];//항상 마지막 인덱스에 위치한 원소가 pivot
    fprintf(fout,"left : %d, right : %d, pivot : %d\n", left,right,pivot);
    PrintArray(array,left,right);

    int low=left;//제일 왼쪽
    int high=right-1;//pivot 바로 앞

    while(low<=high){    //반복 (왼쪽 >= 오른쪽)
        while(array->values[low]<pivot)low++;//왼쪽에서부터 pivot보다 큰 값 찾기
        while(array->values[high]>pivot)high--;       //오른쪽에서부터 pivot보다 작은 값 찾기 

        if(low<=high){
            swap(&array->values[low],&array->values[high]);     //위 두 값 교환
            PrintArray(array,left,right);
        }
    }


    swap(&array->values[right], &array->values[low]);
    if(right!=low)//swap 하기 전 후 같지 않을 때만 출력하기 위해 if문 걸어줌
     PrintArray(array,left,right);
    
    return low;    //return pivot

}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

void swap(int* a, int* b) {
    int tmp;
    tmp=*a;
    *a=*b;
    *b=tmp;
}
