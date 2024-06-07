#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array {
    int size;
    int* values;
} Array;

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    
    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        perror("Error opening input file");
        return 1;
    }
    
    fout = fopen(argv[2], "w");
    if (fout == NULL) {
        perror("Error opening output file");
        fclose(fin);
        return 1;
    }

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);

    fclose(fin);
    fclose(fout);

    return 0;
}

Array* CreateArray(int size){
    Array* array = (Array*)malloc(sizeof(Array));
    array->size = size;
    array->values = (int*)malloc(size * sizeof(int));
    return array;
}

void QuickSort(Array* array, int left, int right, int depth){
    if (left < right) {
        int pivotIndex = Partition(array, left, right, depth);
        fprintf(fout, "Depth %d: ", depth);
        PrintArray(array, left, right);
        QuickSort(array, left, pivotIndex - 1, depth + 1);
        QuickSort(array, pivotIndex + 1, right, depth + 1);
    }
}

int Partition(Array* array, int left, int right, int depth){
    int pivot = array->values[right];  // 피봇을 가장 오른쪽 원소로 설정
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (array->values[j] <= pivot) {
            i++;
            swap(&array->values[i], &array->values[j]);
        }
    }
    swap(&array->values[i + 1], &array->values[right]);
    return i + 1;
}

void PrintArray(Array* array, int left, int right){
    for(int i = 0; i < array->size; i++){
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
    int temp = *a;
    *a = *b;
    *b = temp;
}
