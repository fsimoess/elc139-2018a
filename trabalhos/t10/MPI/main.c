#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include "nqueens.h"

#define N 5

long wtime();

int main(int argc, char** argv) {
	int size = N, nthreads, myrank, p;
	long start_time, end_time;
	
	if(argc == 2) size = atoi(argv[1]);
	
	MPI_Init(&argc, &argv);
	// Descobre o "rank" do processo
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	// Descobre o número de processos
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	
	start_time = wtime();
	int solutions = find_queens(size);
	
	//MPI_Reduce(&solutions, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	end_time = wtime();
	
	MPI_Finalize();
	
	if(myrank==0)
		printf("Nqueens; size: %d; time: %ld; solutions: %d\n", 
			 size, (long) (end_time - start_time), solutions);
}

/* wtime */
long wtime(){
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}
