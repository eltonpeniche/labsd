#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define LIMITE 10

int init_matrix(int m[][LIMITE], int valor){
	int i, j;
	for (i = 0; i < LIMITE; i = i + 1){
		for (j = 0; j < LIMITE; j = j + 1){
			m[i][j] = valor;	
		} 
			
 	}
}

int mostrar_matriz(int m[][LIMITE]){
	int i, j;
	for (i = 0; i < LIMITE; i = i + 1){
		printf("\n");
		for (j = 0; j < LIMITE; j = j + 1) {
			printf("%d ", m[i][j]);	
		}
	}
}

int somar_matriz(int A[][LIMITE], int B[][LIMITE], int C[][LIMITE], int ii, int il, int ji, int jl){
	int i, j;
	for (i = ii; i < il; i++){
		for (j = ji; j < jl; j++){
			C[i][j] = A[i][j] + B[i][j];	
		} 
			
 	}	
}


int merge_matrix(int A[][LIMITE], int B[][LIMITE], int ii, int il, int ji, int jl){
	int i, j;
	for (i = ii; i < il; i++){
		for (j = ji; j < jl; j++){
			A[i][j] = B[i][j];	
		} 
			
 	}	
}

int main(int argc, char** argv) {
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	
	// Find out rank, size
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// We are assuming at least 2 processes for this task
	if (world_size != 3 ) {
		fprintf(stderr, "World size must be equal 4 for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	int A[LIMITE][LIMITE];
	init_matrix(A,1);
	
	int B[LIMITE][LIMITE];
	init_matrix(B,2);
	
	int C[LIMITE][LIMITE];
	init_matrix(C,0);

	int number;
	if (world_rank == 0) {
		// If we are rank 0, set the number to -1 and send it to process 1
		somar_matriz(A,B,C,0,5,0,5);
		//mostrar_matriz(C);
		
		MPI_Send(C, 100, MPI_INT, 2, 0, MPI_COMM_WORLD);
		
	} else if (world_rank == 1) {
		somar_matriz(A,B,C,5,10,5,10);
		MPI_Send(C, 100, MPI_INT, 2, 0, MPI_COMM_WORLD);
		
	}else if (world_rank == 2) {
		
		int C2[LIMITE][LIMITE];

		MPI_Recv(C, 100, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(C2, 100, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//printf("\n");
		merge_matrix(C, C2, 5, 10, 5, 10);
		mostrar_matriz(C);

	}
MPI_Finalize();

}
