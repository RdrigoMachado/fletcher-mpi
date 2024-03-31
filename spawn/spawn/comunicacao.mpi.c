#include "../comunicacao.mpi.h"

#define TERMINAR -1
//#############################    MPI  SEND   ##################################
int rank_destino = 0;
int primeiraExecucao = 1;
MPI_Comm childcomm;
int num_escrita = 0;
int tamanho;
void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  tamanho = sx * sy * sz;

  if(primeiraExecucao)
  {
    MPI_Comm_spawn("./spawn.x", MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);
    MPI_Ssend(&tamanho, 1, MPI_INT, 0, 101, childcomm);
    MPI_Ssend(&tamanho, 1, MPI_INT, 1, 101, childcomm);
    MPI_Ssend(&tamanho, 1, MPI_INT, 2, 101, childcomm);
    primeiraExecucao = 0;
  }
  

  MPI_Ssend(&num_escrita, 1, MPI_INT, rank_destino, 101, childcomm);
  MPI_Ssend(ondaPtr, tamanho, MPI_FLOAT, rank_destino, 102, childcomm);
  printf("Parent - Enviada onda para #%d\n", rank_destino);

  if(rank_destino == 0)
    rank_destino = 1;
  else if (rank_destino == 1)
    rank_destino = 2;
  else
    rank_destino = 0;

  num_escrita++;
}

void MPI_terminar()
{
  int msg = TERMINAR;

  MPI_Ssend(&msg, 1, MPI_INT, 0, 101, childcomm);
  printf("Parent - Enviada msg termino #0\n");
  MPI_Ssend(&msg, 1, MPI_INT, 1, 101, childcomm);
  printf("Parent - Enviada msg termino #1\n");
  MPI_Ssend(&msg, 1, MPI_INT, 2, 101, childcomm);
  printf("Parent - Enviada msg termino #2\n");
}
