#ifndef _RECEBIMENTO_MPI_
#define _RECEBIMENTO_MPI_

#include "MPI.comunicacao.h"

void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo, const int st,  const float dtOutput,
                        const float dt, float dx, float dy, float dz);
void finalizar_recebimento();
#endif