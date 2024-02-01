#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################
int num_processo = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  num_processo++;

  char** spawn_argv = (char**)malloc((argc) * sizeof(char*));

  // Copy the original command-line arguments
  for (int i = 0; i < argc; ++i) {
      spawn_argv[i] = strdup(argv[i]);
  }

  printf("depois das copias strdup\n");

  MPI_Comm childcomm;
  MPI_Comm_spawn("spawn.x", NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);

  printf("depois spawn\n");

  MPI_Send(&num_processo, 1, MPI_INT, 0, 100, childcomm);
  MPI_Send(&tamanho, 1, MPI_INT, 0, 101, childcomm);
  MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 102, childcomm);
}
