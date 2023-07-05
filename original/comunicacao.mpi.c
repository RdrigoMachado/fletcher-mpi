#include "comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p) {

  int tamanho = sx * sy * sz;

  MPI_Ssend((void *) ondaPtr, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  

  p->itCnt++;
}


//##########################    MPI  RECEIVE   ##################################

FILE* abrirArquivo(char* nome){
  char nome_arquivo[128];

  strcpy(nome_arquivo, FNAMEBINARYPATH);
  strcat(nome_arquivo,nome);
  strcat(nome_arquivo,".rsf@");

  return fopen(nome_arquivo, "w+");
}

void finalizar(FILE *arquivo){
  fclose(arquivo);
  MPI_Finalize();
  exit(0);
}

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt)
{
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho); 
  FILE *arquivo = abrirArquivo(nome_arquivo);


  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;


  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);


  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {

      MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      fwrite((void *) onda, sizeof(float), tamanho, arquivo);

      tOut=(++nOut)*dtOutput;
    }
  }

  finalizar(arquivo);
}

