#include "../MPI.envio.h"


void inicializar_envio(int sx, int sy, int sz, int tamanho_buffer){}


void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  int tamanho = sx * sy * sz;

  MPI_Send((void *) ondaPtr, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  
  p->itCnt++;
}

void finalizar_envio()
{
    MPI_Finalize();
}
