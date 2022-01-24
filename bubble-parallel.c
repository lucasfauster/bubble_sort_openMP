#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int *data, int size, int left, int mid, int right);
void swap(int* array, int i, int j);
void bubbleSort(int *data, int left, int right);
void initWorstCase(int *data, int vetSize);
void printArray(int vetSize, int *data);

void swap(int* array, int i, int j){
    int temp = array[i];
    array[i] = array[i + 1];
    array[i + 1] = temp;
}

void bubbleSort(int *data, int left, int right){
    int i, j;
    for (i = left; i < right; i++)
        for (j = left; j < right; j++)
            if (data[j] > data[j + 1])
                swap(data, j, j+1);
}

// Mergeia dois subarrays de dentro do data
void merge(int *data, int vetSize, int left, int mid, int right){
    int *dataTemp = malloc(vetSize*sizeof(int));

    int left_end = mid - 1;
    int tmp_pos = left;
    int count = right - left + 1;
    

    //enquanto houver elemento na esquerda ou na direita ainda não visitado
    while ((left <= left_end) && (mid <= right)){
        if (data[left] <= data[mid]){
            dataTemp[tmp_pos] = data[left];
            tmp_pos++;
            left++;
        }
        else{
            dataTemp[tmp_pos] = data[mid];
            tmp_pos++;
            mid++;
        }
    }

    // Se sobrou algo na esquerda
    while (left <= left_end){
        dataTemp[tmp_pos] = data[left];
        left++;
        tmp_pos++;
    }

    // Se sobrou algo na direita
    while (mid <= right){
        dataTemp[tmp_pos] = data[mid];
        mid++;
        tmp_pos++;
    }

    // copiando a array temporaria pra data
    for (int i = 0; i < count; i++){
        data[right] = dataTemp[right];
        right--;
    }

    free(dataTemp);
}

void initWorstCase(int *data, int vetSize){
    int j = 0;
    for (int i = vetSize; i > 0; i--){
        data[j] = i;
        j++;
    }
}

void printArray(int vetSize, int *data){
    printf("Array: \n");
	for (int i = 0; i < vetSize; i++)
		printf(" %d;", data[i]);
	printf("\n\n");
}

void main(int argc, char *argv[]){
    int vetSize = atoi(argv[1]); // tamanho do vetor
    int numProcs = atoi(argv[2]); // número de threads

    int *data = malloc(vetSize*sizeof(int));
    
    initWorstCase(data, vetSize);

    int chunkSize = vetSize/numProcs;

    omp_set_num_threads(numProcs);

    double startT = omp_get_wtime();
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int left = tid * chunkSize;
        int right = left + chunkSize - 1;
        if (tid < numProcs - 1)
            bubbleSort(data, left, right);
        else
            bubbleSort(data, left, vetSize - 1);
    }

	
    for(int i = 2; i <= numProcs; i *= 2){
        chunkSize = chunkSize*2;
        omp_set_num_threads(numProcs/i);

        #pragma omp parallel
        {
            int tid = omp_get_thread_num(); // id dessa thread
            int left = tid * chunkSize; // posição inicial da esquerda da fatia
            int right = left + chunkSize - 1; // posição final da direita da fatia
            int mid = (left + right)/2 + 1; // posição inicial da direita da fatia

            if(tid != (numProcs/i - 1) || right == vetSize-1) 
                merge(data, vetSize, left, mid, right); // se não for o último processo ou se for mas ele chega até o final do vetor            
            
            else{ // se for o que está com a última fatia
                merge(data, vetSize, left, mid, right); // primeiro ordena as fatias que tem
                merge(data, vetSize, left, right+1, vetSize-1); // ordena a última fatia na junção das fatias
            }
        }
    }

    double stopT = omp_get_wtime();

    printArray(vetSize, data);
    
    double time = (stopT - startT);
    printf("%f\n", time);
}