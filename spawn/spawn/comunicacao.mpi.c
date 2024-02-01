#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################
int num_processo = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  num_processo++;

  printf("antes do sprintf\n");

  char offset[6];
  if (sprintf(offset, "%d", num_processo) < 0) {
      // Handle error
      fprintf(stderr, "Error: Failed to allocate memory for offset\n");
      exit(EXIT_FAILURE);
  }

  char size[100];
  if (sprintf(size, "%d", tamanho) < 0) {
      // Handle error
      fprintf(stderr, "Error: Failed to allocate memory for size\n");
      exit(EXIT_FAILURE);
  }
  
  
  printf("depois do sprintf\n");



  char** spawn_argv = (char**)malloc((3) * sizeof(char*));
  spawn_argv[0] = "./spawn.x";
  spawn_argv[1] = strdup(offset);
  spawn_argv[2] = strdup(size);

  printf("depois das copias strdup\n");

  MPI_Comm childcomm;
  MPI_Comm_spawn("spawn.x", spawn_argv, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);

  printf("depois spawn\n");

  MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 101, childcomm);
}
