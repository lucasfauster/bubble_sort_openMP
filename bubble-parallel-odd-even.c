#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void printArray(int vetSize, int *data)
{
	for (int i = 0; i < vetSize; i++)
		printf(" %d;", data[i]);
	printf("\n\n");
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

void swap(int* array, int i, int j){
    int temp = array[i];
    array[i] = array[i + 1];
    array[i + 1] = temp;
}

void bubblesort(int *data, int vetSize, int numProcs)
{
	omp_set_num_threads(numProcs); // seta o número de processos

	for (int step = 0; step < vetSize - 1; ++step)
		#pragma omp parallel
		{
			#pragma omp for
			for (int i = 0; i < vetSize - 1; i += 2)
				if (data[i] > data[i + 1])
					swap(data, i, i+1);

			#pragma omp for
			for (int i = 1; i < vetSize - 1; i += 2)
				if (data[i] > data[i + 1])
					swap(data, i, i+1);
		}	
}

void main(int argc, char **argv)
{
	int vetSize, numProcs;

	printf("size of the array: ");
	fflush(stdout);
	scanf("%i", &vetSize);

	printf("num of processes: ");
	fflush(stdout);
	scanf("%i", &numProcs);
	
	int* data = (int*)malloc(vetSize * sizeof(int));

	initWorstCase(data, vetSize);

	double startT = omp_get_wtime();
	bubblesort(data, vetSize, numProcs);
	double stopT = omp_get_wtime();

	printArray(vetSize, data);

	double time = (stopT - startT);

	printf("%d elements; %d processors; %f secs\n", vetSize, numProcs, time);
}