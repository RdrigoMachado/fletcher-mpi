#include "../MPI.sender.h"

//Vars
int tamanho_buffer_envio;
int ordem;
int tamanho;

//Arrays
float **buffers;
MPI_Request *requests;


void inicializar_sender(int sx, int sy, int sz, int tamanho_buffer)
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

void  finalizar_sender()
{
  MPI_Waitall(tamanho_buffer_envio, requests, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  
  for(int i =0; i < tamanho_buffer_envio; i++)
  {
    free(buffers[i]);  
  }
  
  free(buffers);
  free(requests);
  exit(0);
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

void MPI_sender(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt, int tamanho_buffer)
{
  inicializar_sender(sx, sy, sz, tamanho_buffer);
  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;


  int posicao = posicao_conexao();
  free(buffers[posicao]);
  buffers[posicao] = malloc(sizeof(float) * tamanho);
  MPI_Recv((void *) buffers[posicao], tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  
  MPI_Isend((void *) buffers[posicao], tamanho , MPI_FLOAT, 2, ordem, MPI_COMM_WORLD, &(requests[posicao]));
  ordem++;


  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {
        posicao = posicao_conexao();
        free(buffers[posicao]);
        buffers[posicao] = malloc(sizeof(float) * tamanho);
        MPI_Recv((void *) buffers[posicao], tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Isend((void *) buffers[posicao], tamanho , MPI_FLOAT, 2, ordem, MPI_COMM_WORLD, &(requests[posicao]));
        ordem++;

        tOut=(++nOut)*dtOutput;
        itCnt++;
    }
  }
  finalizar_sender();
}

