#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################
int num_processo = 0;

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  
  num_processo++;
  char offset[6];
  sprintf(offset, "%d", num_processo);
  char size[100];
  sprintf(size, "%d", tamanho);



  char** spawn_argv = (char**)malloc((argc + 2) * sizeof(char*));
  spawn_argv[0] = "./spawn.x";
  spawn_argv[1] = strdup(offset);
  spawn_argv[2] = strdup(size);


  MPI_Comm childcomm;
  MPI_Comm_spawn("spawn.x", spawn_argv, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);
  MPI_Send(ondaPtr, tamanho, MPI_FLOAT, 0, 101, childcomm);
}


//##########################    MPI  RECEIVE   ##################################

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho); 

  char nome_arquivo_aux[128];
  strcpy(nome_arquivo_aux, FNAMEBINARYPATH);
  strcat(nome_arquivo_aux, nome_arquivo);
  strcat(nome_arquivo_aux, ".rsf@");

  FILE *arquivo = fopen(nome_arquivo, "w+");


  int ixStart=0;
  int ixEnd=sx-1;
  int iyStart=0;
  int iyEnd=sy-1;
  int izStart=0;
  int izEnd=sz-1;

  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;

 
  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);

  for (int it=1; it<=st; it++) {

    tSim=it*dt;
    if (tSim >= tOut) {
      MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      fwrite((void *) onda, sizeof(float), tamanho, arquivo);
      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }
  salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  fclose(arquivo);
  MPI_Finalize();
  exit(0);
}

