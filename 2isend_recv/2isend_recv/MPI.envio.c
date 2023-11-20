#include "../MPI.envio.h"

int sequencia_envio;
float *onda_par, *onda_impar;
MPI_Request requests[2];


void inicializar_envio(int sx, int sy, int sz)
{
  int tamanho = sx * sy * sz;
  onda_impar = malloc(sizeof(float) * tamanho);
  onda_par   = malloc(sizeof(float) * tamanho);
  requests[0] = MPI_REQUEST_NULL;
  requests[1] = MPI_REQUEST_NULL;
  sequencia_envio = 1;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  int tamanho = sx * sy * sz;
  if(sequencia_envio % 2 == 0)
  {
    memcpy(onda_par, ondaPtr, tamanho * sizeof(float));
    MPI_Wait(&(requests[0]), MPI_STATUS_IGNORE);
    MPI_Isend((void *) onda_par, tamanho , MPI_FLOAT, 2, sequencia_envio, MPI_COMM_WORLD, &(requests[0]));
  }
  else
  {
    memcpy(onda_impar, ondaPtr, tamanho * sizeof(float));
    MPI_Wait(&(requests[1]), MPI_STATUS_IGNORE);
    MPI_Isend((void *) onda_impar, tamanho , MPI_FLOAT, 1, sequencia_envio, MPI_COMM_WORLD, &(requests[1]));
  }
  sequencia_envio++;
  p->itCnt++;
}

void  finalizar_envio()
{
  MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  
  free(onda_par);
  free(onda_impar);
}
