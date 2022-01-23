#include <stdio.h>
#include <stdlib.h>
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

void bubblesort(int array[], int size)
{
    for (int step = 0; step < size - 1; ++step)
        for (int i = 0; i < size - step - 1; ++i)
            if (array[i] > array[i+1])
                swap(array, i, i+1);
}

int main()
{
    int vetSize;

    printf("size of the array: ");
    scanf("%i", &vetSize);

	int* data = (int*)malloc(vetSize * sizeof(int));

	initWorstCase(data, vetSize);

    double startT = clock();
	bubblesort(data, vetSize);
    double stopT = clock();

	// 	printArray(vetSize, data);

	double time = (stopT - startT)/CLOCKS_PER_SEC;

    printf("\n");
    printf("array size: %i; %f secs \n", vetSize, time);
}