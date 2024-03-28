#include "../comunicacao.mpi.h"

#define TERMINAR -1
//#############################    MPI  SEND   ##################################
int rank_destino = 0;
int primeiraExecucao = 1;
MPI_Comm childcomm;
int num_escrita = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;

  if(primeiraExecucao)
  {
    MPI_Comm_spawn("./spawn.x", MPI_ARGV_NULL, 2, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);
    MPI_Send(&tamanho, 1, MPI_INT, 0, 101, childcomm);
    MPI_Send(&tamanho, 1, MPI_INT, 1, 101, childcomm);
    primeiraExecucao = 0;
  }
  
printf("Enviando para rank %d num escrita %d\n", rank_destino, num_escrita);

  MPI_Send(&num_escrita, 1, MPI_INT, rank_destino, 101, childcomm);
printf("Enviado rank %d num escrita\n", rank_destino);

  MPI_Send(ondaPtr, tamanho, MPI_FLOAT, rank_destino, 102, childcomm);
printf("Enviado rank %d onda\n", rank_destino);

  rank_destino = (rank_destino == 0) ? 1 : 0;
  printf("enviado\n\n");
  num_escrita++;
}

void MPI_terminar()
{
  int msg = TERMINAR;
  MPI_Send(&msg, 1, MPI_INT, rank_destino, 101, childcomm);
  rank_destino = (rank_destino == 0) ? 1 : 0;

  MPI_Send(&msg, 1, MPI_INT, rank_destino, 101, childcomm);
}
