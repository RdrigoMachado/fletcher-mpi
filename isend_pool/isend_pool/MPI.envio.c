#include "../MPI.envio.h"

int byte_size;
int size;
int numero_escrita;
int pos_atual;
float *buffers[TAMANHO_BUFFER];
MPI_Request requests[TAMANHO_BUFFER];


void inicializar_envio(int sx, int sy, int sz)
{
  size = sx * sy * sz;
  byte_size = size * sizeof(float);
  numero_escrita = 1;
  pos_atual = -1;
  for(int i = 0; i < TAMANHO_BUFFER; i++)
	{
    buffers[i]  = malloc(byte_size);
    requests[i] = MPI_REQUEST_NULL;
	}
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  if(numero_escrita >= TAMANHO_BUFFER)
  {

    pos_atual = numero_escrita % TAMANHO_BUFFER;
    //printf("snd esperando %d num %d\n", pos_atual, numero_escrita);
    MPI_Wait(&(requests[pos_atual]), MPI_STATUS_IGNORE);
  } else {
    pos_atual++;
  }
  
  //printf("snd enviando %d num %d\n", pos_atual, numero_escrita);

  memcpy(buffers[pos_atual], ondaPtr, byte_size);
  MPI_Isend((void *) buffers[pos_atual], size , MPI_FLOAT, 1, numero_escrita, MPI_COMM_WORLD, &(requests[pos_atual]));
  
  numero_escrita++;
  p->itCnt++;
}

void  finalizar_envio()
{
  MPI_Waitall(TAMANHO_BUFFER, requests, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  
  for(int i = 0; i < TAMANHO_BUFFER; i++)
  {
    free(buffers[i]);  
  }
}