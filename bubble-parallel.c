#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int * merge(int *v1, int size1, int *v2, int size2);
void swap(int*, int, int); 
void bubblesort(int *v, int n);

double startT,stopT;

int * merge(int *v1, int size1, int *v2, int size2){
	int i,j,k;
	int * result;

	result = (int *)malloc((size1+size2)*sizeof(int));

	i=0; j=0; k=0;
	while(i < size1 && j < size2){
		if(v1[i] < v2[j]){
			result[k] = v1[i];
			i++;
		}
		else{
			result[k] = v2[j];
			j++;
		} 
        k++;
    }
	if(i == size1){
		while(j < size2){
			result[k] = v2[j];
			j++; 
            k++;
		}
    }
	else{
		while(i < size1){
			result[k] = v1[i];
			i++;
            k++;
		}
    }

	return result;
}

void swap(int* v, int a, int b){
    unsigned temp;
    temp=v[a];
    v[a]=v[b];
    v[b]=temp;
}

// void bubbleSort(int* a, int left, int right){
//    int i, j;
//    for (i = left; i < right; i++)      
//        for (j = left; j < right; j++){
//            if (a[j] > a[j+1]){
// 				swap(&a[j], &a[j+1]);
// 			}
// 	   }
// }

void printzada(int vetSize, int * data){
	for(int i=0;i<vetSize;i++) 
		printf(" %d;",data[i]);
	printf("\n");
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


    // for (i = 1; i <= numProcs - 2; i++) { // All but the last subarray
	// 	int middle = num * chunkSize;
	// 	merge(a, b, 0, middle, middle + chunkSize - 1);
	// }
	// merge(a, b, 0, (numProcs - 1) * chunkSize, vet - 1); // Last subarray

	// step = 1;
	// while(step < numProcs){
	// 	id = n
	// 	if(id % (2 * step) != 0){ // id 0 nunca entra nesse if, apenas ids ímpares no primeiro step e ids pares nos steps > 1
	// 		int destinationId = id-step;
	// 		MPI_Send(&chunkSize,1,MPI_INT,destinationId,0,MPI_COMM_WORLD); // envia o tamanho do pedaço do array ordenado para o processo destinatário
	// 		MPI_Send(chunk,chunkSize,MPI_INT,destinationId,0,MPI_COMM_WORLD); // envia os valores do pedaço do array ordenado para o processo destinatário
	// 		break;
	// 	}

	//     if(id + step < numProcs){ // id 0 sempre entra nesse if e ids pares esperam receber dos ids impares no primeiro step
	//         MPI_Recv(&chunkAuxSize,1,MPI_INT,id+step,0,MPI_COMM_WORLD,&status); // espera receber o tamanho do pedaço do array ordenado do processo de origem
	//         chunkAux = (int *)malloc(chunkAuxSize*sizeof(int));

	//         MPI_Recv(chunkAux,chunkAuxSize,MPI_INT,id+step,0,MPI_COMM_WORLD,&status); // espera receber os valores do pedaço do array ordenado do processo de origem
	//         chunk = merge(chunk,chunkSize,chunkAux,chunkAuxSize); // junta os pedaços do array ordenado no array principal
	//         chunkSize = chunkSize+chunkAuxSize;
	//     } 

	//     step = step*2;
	// }

	stopT = clock();

	printzada(vetSize, data);

	printf("\n");

	printf("array size: %d; %d processors; %f secs\n",vetSize,numProcs,(stopT-startT)/CLOCKS_PER_SEC);
}