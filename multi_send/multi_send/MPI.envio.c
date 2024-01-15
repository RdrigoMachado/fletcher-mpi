#include "../MPI.envio.h"

int ordem;
int tamanho;
int tamanho_grupo_escrita;

void inicializar_envio(int sx, int sy, int sz, int groupSize)
{
  tamanho_grupo_escrita = groupSize;
  tamanho = sx * sy * sz;
  ordem = 1;
}

//#############################    MPI  SEND   ##################################

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{
  int destino = (ordem % tamanho_grupo_escrita);
  if(destino == 0)
    destino = tamanho_grupo_escrita;

  MPI_Send((void *) ondaPtr, tamanho , MPI_FLOAT, destino, ordem, MPI_COMM_WORLD);
  ordem++;
  p->itCnt++;
}

