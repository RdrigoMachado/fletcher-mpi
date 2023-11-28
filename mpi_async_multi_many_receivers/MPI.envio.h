#ifndef _ENVIO_MPI_
#define _ENVIO_MPI_

#include "MPI.comunicacao.h"

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p);

void finalizar_envio();
void inicializar_envio(int sx, int sy, int sz, int tamanho_buffer);
#endif