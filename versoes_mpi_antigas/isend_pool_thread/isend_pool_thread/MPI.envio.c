#include "../MPI.envio.h"

int byte_size;
int size;
int numero_escrita;
int pos_atual;
float *buffers[TAMANHO_BUFFER];
MPI_Request requests[TAMANHO_BUFFER];

pthread_t thread[TAMANHO_BUFFER];

// Producer thread function
void* sender() {
    printf("snd enviando %d num %d\n", pos_atual, numero_escrita);
    MPI_Send(buffers[pos_atual], size, MPI_FLOAT, 1, numero_escrita, MPI_COMM_WORLD);
}


void inicializar_envio(int sx, int sy, int sz)
{
  size = sx * sy * sz;
  byte_size = size * sizeof(float);
  numero_escrita = 0;
  pos_atual = -1;
  for(int i = 0; i < TAMANHO_BUFFER; i++)
	{
    buffers[i]  = malloc(byte_size);
    requests[i] = MPI_REQUEST_NULL;
	}
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  numero_escrita++;

  if(numero_escrita > TAMANHO_BUFFER)
  {
    pos_atual = (numero_escrita - 1) % TAMANHO_BUFFER;
    printf("snd esperando %d num %d\n", pos_atual, numero_escrita);
    pthread_join(thread[pos_atual], NULL);
  } else {
    pos_atual++;
  }
  memcpy(buffers[pos_atual], ondaPtr, byte_size);
  pthread_create(&(thread[pos_atual]), NULL, &sender, NULL);  
  p->itCnt++;
}

void  finalizar_envio()
{
  for(int i = 0; i < TAMANHO_BUFFER; i++)
  {
    pthread_join(thread[i], NULL);
    free(buffers[i]);  
  }
  MPI_Finalize();
}