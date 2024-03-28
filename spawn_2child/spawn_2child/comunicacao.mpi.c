#include "../comunicacao.mpi.h"
#define TERMINAR -1

//#############################    MPI  SEND   ##################################
int rank_destino = 0;
int execucao = 1;
MPI_Comm childcomm1;
MPI_Comm childcomm2;
int num_escrita = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;

  if(execucao == 1)
  {
    MPI_Comm_spawn("./spawn.x", MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm1, MPI_ERRCODES_IGNORE);
    MPI_Send(&tamanho, 1, MPI_INT, 0, 101, childcomm1);
    execucao++;
  } else if(execucao == 2)
  {
    MPI_Comm_spawn("./spawn.x", MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm2, MPI_ERRCODES_IGNORE);
    MPI_Send(&tamanho, 1, MPI_INT, 0, 101, childcomm2);
    execucao++;
  }
  
  if(rank_destino == 0)
  {
    MPI_Send(&num_escrita, 1, MPI_INT, 0, 101, childcomm1);
    MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 102, childcomm1);
  }
  else {
    MPI_Send(&num_escrita, 1, MPI_INT, 0, 101, childcomm2);
    MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 102, childcomm2);
  }
  

  rank_destino = (rank_destino == 0) ? 1 : 0;
  num_escrita++;
}

void MPI_terminar()
{
  int msg = TERMINAR;
  MPI_Send(&msg, 1, MPI_INT, 0, 101, childcomm1);
  MPI_Send(&msg, 1, MPI_INT, 0, 101, childcomm2);
}
