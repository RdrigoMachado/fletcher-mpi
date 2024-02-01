#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################
int num_processo = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  num_processo++;

  MPI_Comm childcomm;
  MPI_Comm_spawn("spawn.x", NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);

  MPI_Send(&num_processo, 1, MPI_INT, 0, 100, childcomm);
  MPI_Send(&tamanho, 1, MPI_INT, 0, 101, childcomm);
  MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 102, childcomm);
}
