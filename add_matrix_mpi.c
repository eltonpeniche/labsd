#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define LIMITE 500

long double init_matrix(long double m[][LIMITE], int valor){
	int i, j;
	for (i = 0; i < LIMITE; i = i + 1){
		for (j = 0; j < LIMITE; j = j + 1){
			m[i][j] = valor;	
		} 
			
 	}
}

long double mostrar_matriz(long double m[][LIMITE]){
	int i, j;
	for (i = 0; i < LIMITE; i = i + 1){
		printf("\n");
		for (j = 0; j < LIMITE; j = j + 1) {
			printf("%LG ", m[i][j]);	
		}
	}
}

long double somar_matriz(long double A[][LIMITE], long double B[][LIMITE], long double C[][LIMITE], int ii, int il, int ji, int jl){
	int i, j;
	for (i = ii; i < il; i++){
		for (j = ji; j < jl; j++){
			C[i][j] = A[i][j] + B[i][j];	
		} 
			
 	}	
}


long double merge_matrix(long double A[][LIMITE], long double B[][LIMITE], int ii, int il, int ji, int jl){
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
	if (world_size != 4 ) {
		fprintf(stderr, "World size must be equal 4 for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	long double A[LIMITE][LIMITE];
	init_matrix(A,1);
	
	long double B[LIMITE][LIMITE];
	init_matrix(B,2);
	
	long double C[LIMITE][LIMITE];
	init_matrix(C,0);

	int number;
	
	if (world_rank == 0) {
		// If we are rank 0, set the number to -1 and send it to process 1
		somar_matriz(A,B,C,0,LIMITE/2,0,LIMITE/2);
		//mostrar_matriz(C);

		long double C2[LIMITE][LIMITE];

		MPI_Recv(C2, LIMITE*LIMITE, MPI_LONG_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		merge_matrix(C, C2, 0,LIMITE/2,LIMITE/2,LIMITE);

		MPI_Recv(C2, LIMITE*LIMITE, MPI_LONG_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		merge_matrix(C, C2, LIMITE/2,LIMITE,0,LIMITE/2);

		MPI_Recv(C2, LIMITE*LIMITE, MPI_LONG_DOUBLE, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		merge_matrix(C, C2, LIMITE/2,LIMITE,LIMITE/2,LIMITE);

		mostrar_matriz(C);
		
	} else if (world_rank == 1) {
		
		somar_matriz(A,B,C,0,LIMITE/2,LIMITE/2,LIMITE);
		
		MPI_Send(C, LIMITE*LIMITE, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	}else if (world_rank == 2) {
		
		somar_matriz(A,B,C,LIMITE/2,LIMITE,0,LIMITE/2);
		
		MPI_Send(C, LIMITE*LIMITE, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	}else if (world_rank == 3) {
		
		somar_matriz(A,B,C,LIMITE/2,LIMITE,LIMITE/2,LIMITE);
		
		MPI_Send(C, LIMITE*LIMITE, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	}
MPI_Finalize();

}
