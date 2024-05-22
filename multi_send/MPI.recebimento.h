#ifndef _RECEBIMENTO_MPI_
#define _RECEBIMENTO_MPI_

#include "MPI.comunicacao.h"
void MPI_recebimento(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt, int rank, int groupSize);
void finalizar_recebimento();
#endif