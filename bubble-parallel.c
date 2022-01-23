#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

void merge(int *data, int size, int left, int mid, int right);
void swap(int* array, int i, int j);
void bubbleSort(int *data, int left, int right);
void initWorstCase(int *data, int vetSize);
void printArray(int vetSize, int *data);

void main(int argc, char *argv[])
{
    int vetSize = atoi(argv[1]); // Array size
    int numProcs = atoi(argv[2]); // Number of threads

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

    for (int i = 1; i <= numProcs - 2; i++)
    {
        int middle = i * chunkSize;
        merge(data, vetSize, 0, middle, middle + chunkSize - 1);
    }
    merge(data, vetSize, 0, (numProcs-1)*chunkSize, vetSize - 1);

    double stopT = omp_get_wtime();

    // printArray(vetSize, data);
    
    double time = (stopT - startT);
    printf("%f\n", time);
}

void swap(int* array, int i, int j){
    int temp = array[i];
    array[i] = array[i + 1];
    array[i + 1] = temp;
}

void bubbleSort(int *data, int left, int right)
{
    int i, j;
    for (i = left; i < right; i++)
        for (j = left; j < right; j++)
            if (data[j] > data[j + 1])
                swap(data, j, j+1);
}

// Mergeia dois subarrays de dentro do data
void merge(int *data, int vetSize, int left, int mid, int right)
{
    int *dataTemp = malloc(vetSize*sizeof(int));

    int left_end = mid - 1;
    int tmp_pos = left;
    int count = right - left + 1;
    

    //enquanto houver elemento na esquerda ou na direita ainda não visitado
    while ((left <= left_end) && (mid <= right))
    {
        if (data[left] <= data[mid])
        {
            dataTemp[tmp_pos] = data[left];
            tmp_pos++;
            left++;
        }
        else
        {
            dataTemp[tmp_pos] = data[mid];
            tmp_pos++;
            mid++;
        }
    }

    // Se sobrou algo na esquerda
    while (left <= left_end)
    {
        dataTemp[tmp_pos] = data[left];
        left++;
        tmp_pos++;
    }

    // Se sobrou algo na direita
    while (mid <= right)
    {
        dataTemp[tmp_pos] = data[mid];
        mid++;
        tmp_pos++;
    }

    // copiando a array temporaria pra data
    for (int i = 0; i < count; i++)
    {
        data[right] = dataTemp[right];
        right--;
    }

    free(dataTemp);
}

void initWorstCase(int *data, int vetSize)
{
    int j = 0;
    for (int i = vetSize; i > 0; i--)
    {
        data[j] = i;
        j++;
    }
}

void printArray(int vetSize, int *data)
{
	for (int i = 0; i < vetSize; i++)
		printf(" %d;", data[i]);
	printf("\n\n");
}
