//
//  Este programa MPI tem um erro!
//  Identifique este erro e corrija-o para que 2 processos
//  possam trocar mensagens.
//  
//  Uso: mpirun -np 2 <prog>
//  
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char *argv[]){
   int numtasks, rank, dest, tag, source, rc;
   MPI_Status stat;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   printf("Processo %d iniciando...\n",rank);
   
   srand(time(NULL));
   int n = rand()%200;

   if (rank == 0) {
      dest = 1;
      source = dest;
      tag = 0;
      printf("Valor a ser enviado: %d\n", n);
      n+=1;
      rc = MPI_Send(&n, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      printf("Enviei mensagem para processo %d...\n", dest);
      n+=1;
   }
   else if (rank == numtasks - 1) {
      source = rank - 1;
      tag = 0;
      rc = MPI_Recv(&n, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &stat);
      printf("Recebi mensagem do processo %d...\n", source);
      printf("Fim de execucao. Valor final = %d\n", n);
   } else {
   	  dest = rank+1;
   	  source = rank-1;
   	  tag = 0;
   	  rc = MPI_Recv(&n, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &stat);
   	  printf("Recebi mensagem do processo %d... Numero recebido: %d\n", source, n);
   	  n+=1;
   	  rc = MPI_Send(&n, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
   	  printf("Enviei mensagem pro processo %d... Numero enviado: %d\n", dest, n);
   }

   MPI_Finalize();
}
