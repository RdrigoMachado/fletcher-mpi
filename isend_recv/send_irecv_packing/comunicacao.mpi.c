#include "../comunicacao.mpi.h"

MPI_Request request = MPI_REQUEST_NULL;
MPI_Status  status;

//#############################    MPI  SEND   ##################################
void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr) 
{
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho);
  memcpy(onda, ondaPtr, tamanho * sizeof(float));
  
  MPI_Send((void *) onda, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  
}

void  finalizar_comunicacao()
{
  MPI_Finalize();
}

//##########################    MPI  RECEIVE   ##################################

void MPI_escrita_disco(int sx, int sy, int sz, const int st, const float dtOutput, const float dt)
{
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho); 
  FILE *arquivo = fopen(NOME_ARQUIVO, "w+");
 
  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;

  MPI_Irecv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, &request);
  MPI_Wait(&request, &status);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);

  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {

      MPI_Irecv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, &status);
      fwrite((void *) onda, sizeof(float), tamanho, arquivo);

      tOut=(++nOut)*dtOutput;
    }
  }
  fclose(arquivo);
  MPI_Finalize();
  exit(0);
}