#include "../MPI.envio.h"

int sequencia_envio;
int tamanho_envio;
float *onda_par, *onda_impar;
MPI_Request requests[2];


void inicializar_envio(int sx, int sy, int sz)
{
  tamanho_envio = sx * sy * sz;
  sequencia_envio = 1;

  onda_par = NULL;
  onda_impar = NULL;

  requests[0] = MPI_REQUEST_NULL;
  requests[1] = MPI_REQUEST_NULL;
}

void  finalizar_envio()
{
  MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  
  free(onda_par);
  free(onda_impar);
}

//#############################    MPI  SEND   ##################################

float* empacotar(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  float *onda = malloc(sizeof(float) * tamanho_envio);
  memcpy(onda, ondaPtr, tamanho_envio * sizeof(float));
  return onda;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  if(sequencia_envio % 2 == 0)
  {
    onda_par = empacotar(sx, sy, sz, ondaPtr, p);
    MPI_Wait(&(requests[0]), MPI_STATUS_IGNORE);
    MPI_Isend((void *) onda_par, tamanho_envio , MPI_FLOAT, 2, sequencia_envio, MPI_COMM_WORLD, &(requests[0]));
  }
  else
  {
    onda_impar = empacotar(sx, sy, sz, ondaPtr, p);
    MPI_Wait(&(requests[1]), MPI_STATUS_IGNORE);
    MPI_Isend((void *) onda_impar, tamanho_envio , MPI_FLOAT, 1, sequencia_envio, MPI_COMM_WORLD, &(requests[1]));
  }
  sequencia_envio++;
  p->itCnt++;
}

