#include "../MPI.envio.h"

//Vars
int tamanho_buffer_envio;
int ordem;
int tamanho;

//Arrays
float **buffers;
MPI_Request *requests;


void inicializar_envio(int sx, int sy, int sz, int tamanho_buffer)
{
  tamanho = sx * sy * sz;
  ordem = 1;
  tamanho_buffer_envio = tamanho_buffer;

  buffers           = (float**) malloc(tamanho_buffer * sizeof(float*));
  requests          = (MPI_Request*) malloc(tamanho_buffer * sizeof(MPI_Request));

  for(int i = 0; i < tamanho_buffer_envio; i++)
	{
    buffers[i]  = NULL;
    requests[i] = MPI_REQUEST_NULL;
	}

}

void  finalizar_envio()
{
  MPI_Waitall(tamanho_buffer_envio, requests, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  
  for(int i =0; i < tamanho_buffer_envio; i++)
  {
    free(buffers[i]);  
  }
  
  free(buffers);
  free(requests);
}

//#############################    MPI  SEND   ##################################

float* empacotar(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  float *onda = malloc(sizeof(float) * tamanho);
  memcpy(onda, ondaPtr, tamanho * sizeof(float));
  return onda;
}

int posicao_conexao()
{
  for(int i = 0; i < tamanho_buffer_envio; i++)
  {
    if(requests[i] == MPI_REQUEST_NULL)
      return i;
  }

  int posicao;
  MPI_Waitany(tamanho_buffer_envio, requests, &posicao, MPI_STATUS_IGNORE);
  return posicao;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  int posicao = posicao_conexao();
  free(buffers[posicao]);
  buffers[posicao] = empacotar(sx, sy, sz, ondaPtr, p);

  MPI_Isend((void *) buffers[posicao], tamanho , MPI_FLOAT, 1, ordem, MPI_COMM_WORLD, &(requests[posicao]));
  ordem++;
  p->itCnt++;

}

