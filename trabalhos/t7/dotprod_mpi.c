#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

typedef struct{
   double *a;
   double *b;
   double c;
   int wsize;
   int repeat;
} dotdata_t;

dotdata_t dotdata;

double dotprod_worker(double* vetX, double* vetY, int n, int loop){
	int i, j;
	double dotprod = 0.0;
  
  	for(i=0 ; i<loop ; i++){
		dotprod = 0.0;
		for(j=0 ; j<n ; j++){
    		dotprod += vetX[j]*vetY[j];
		}
	}
	
	return dotprod;
}

void fill(double* a, int size, double value){
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

long wtime(){
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

int main(int argc, char **argv)
{
   int num;
   int myrank, p;
   long start_time, end_time;
   double prod, main_prod;

   if ((argc != 3)) {
      printf("Uso: %s <n_elements> <repetitions>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   num = atoi(argv[1]);
   dotdata.repeat = atoi(argv[2]);
   
   start_time = wtime();
   
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   
   dotdata.a = (double *) malloc(num*p*sizeof(double));
   fill(dotdata.a, p*num, 0.01);
   dotdata.b = (double *) malloc(num*p*sizeof(double));
   fill(dotdata.b, p*num, 1.0);

   main_prod = dotprod_worker(dotdata.a, dotdata.b, num, dotdata.repeat);
   
   MPI_Reduce(&main_prod, &prod, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   end_time = wtime();
	
   MPI_Finalize();

   if(myrank == 0){
      printf("dotProduct = %.2f\n", prod);
      printf("%ld usec\n", (long) (end_time - start_time));
   }
   free(dotdata.a);
   free(dotdata.b);

   return 0;
}
