#include "../MPI.envio.h"

int ordem = 1;

float* empacotar(int sx, int sy, int sz,
		   float *ondaPtr, SlicePtr p){

  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho);

  memcpy(onda, ondaPtr, tamanho * sizeof(float));
  return onda;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p) {

  int tamanho = sx * sy * sz;
  float *onda = empacotar(sx, sy, sz, ondaPtr, p);

  MPI_Send((void *) onda, tamanho , MPI_FLOAT, 1, ordem, MPI_COMM_WORLD);  
  ordem++;

  p->itCnt++;
}
