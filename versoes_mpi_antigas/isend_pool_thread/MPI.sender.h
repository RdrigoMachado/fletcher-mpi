#ifndef _SENDER_MPI_
#define _SENDER_MPI_

#include "MPI.comunicacao.h"

void MPI_sender(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt, int tamanho_buffer);
#endif