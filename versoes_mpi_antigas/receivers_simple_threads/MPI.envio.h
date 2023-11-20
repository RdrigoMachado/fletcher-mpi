#ifndef _ENVIO_MPI_
#define _ENVIO_MPI_

#include "MPI.comunicacao.h"
#include <pthread.h>
#define NUM_THREADS 2
#define EXIT 1

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p);
void inicializar_envio(int sx, int sy, int sz);
void finalizar_envio();
#endif