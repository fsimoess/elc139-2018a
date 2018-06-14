#include <stdio.h>
#include <mpi.h>
#define SIZE 8

MPI_Status status;

double a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

void fill_matrix(double m[SIZE][SIZE])
{
  static int n=0;
  int i, j;
  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      m[i][j] = n++;
}


int main(int argc, char **argv){
  	int nprocs, myrank, numprocs, source, dest, colunas, x, i, j, k, num = 0;

	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  	numprocs = nprocs-1;

  	if(myrank == 0){
    	fill_matrix(a);
    	fill_matrix(b);

	    colunas = SIZE/numprocs;
    	x = 0;

	    for(dest = 1; dest <= numprocs; dest++){
    	  MPI_Send(&x, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
	      MPI_Send(&colunas, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
	      MPI_Send(&a[x][0], colunas*SIZE, MPI_DOUBLE,dest, 1, MPI_COMM_WORLD);
	      MPI_Send(&b, SIZE*SIZE, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
    	  x = x + colunas;
	    }

	    for(i = 1; i <= numprocs; i++){
	      source = i;
	      MPI_Recv(&x, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
	      MPI_Recv(&colunas, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
	      MPI_Recv(&c[x][0], colunas*SIZE, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    	}

		printf("Matriz A:\n");
	    for(i = 0; i < SIZE; i++){
	      for(j = 0; j < SIZE; j++)
	        printf("%6.2f   ", a[i][j]);
		  printf("\n");
    	}

		printf("Matriz B:\n");
	    for(i = 0; i < SIZE; i++){
	      for(j = 0; j < SIZE; j++)
	        printf("%6.2f   ", b[i][j]);
		  printf("\n");
    	}

    	printf("Matriz C:\n");
	    for(i = 0; i < SIZE; i++){
	      for(j = 0; j < SIZE; j++)
	        printf("%6.2f   ", c[i][j]);
		  printf("\n");
}
	    
  	}

  	if(myrank != 0){
    	source = 0;
	    MPI_Recv(&x, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
	    MPI_Recv(&colunas, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
	    MPI_Recv(&a, colunas*SIZE, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
	    MPI_Recv(&b, SIZE*SIZE, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    	/* Matrix multiplication */
	    for(k = 0; k < SIZE; k++)
	      for(i = 0; i < colunas; i++){
    	    c[i][k] = 0.0;
    	    for(j = 0; j < SIZE; j++)
    	      c[i][k] += a[i][j] * b[j][k];
    	  }

	    MPI_Send(&x, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    	MPI_Send(&colunas, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	    MPI_Send(&c, colunas*SIZE, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	
	return 0;
}
