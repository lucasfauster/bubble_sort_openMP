#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void swap(int*, int, int); 
double startT,stopT;

void swap(int* v, int a, int b){
    unsigned temp;
    temp=v[a];
    v[a]=v[b];
    v[b]=temp;
}

void printArray(int vetSize, int * data){
	for(int i=0;i<vetSize;i++) 
		printf(" %d;",data[i]);
	printf("\n\n");
}

void main(int argc, char **argv){
	int * data;
	int * chunk;
	int * chunkAux;
	int chunkAuxSize,vetSize;
	int id,numProcs;
	int chunkSize;
	int i;  
	int step;
    int rest;


	printf("size of the array: ");
	fflush(stdout);
	scanf("%i", &vetSize);

	printf("num of processes: ");
	fflush(stdout);
	scanf("%i", &numProcs);

	chunkSize = vetSize/numProcs;

	data = (int *)malloc(vetSize*sizeof(int));

	int j = 0;
	for(i = vetSize; i > 0; i--){
		data[j] = i;
		j++;
	}

	startT = clock();
	
	omp_set_num_threads(numProcs); // seta o número de processos

    for(i=0;i<vetSize;i++){
        int first=i%2;
        #pragma omp parallel for default(none),shared(data, first, vetSize)
        for(j=first;j<vetSize-1;j+=2){
            if (data[j] > data[j+1]){
                swap(data,j,j+1);
            }
        }
    }

	stopT = clock();

	printArray(vetSize, data);

	printf("%d elements; %d processors; %f secs\n",vetSize,numProcs,(stopT-startT)/CLOCKS_PER_SEC);
}