#include <mpi.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

#define MSG_ONDA      101
#define MSG_CONTROLE  100
#define FLAG_FINALIZAR -1
#define FLAG_ONDA 1

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p);
void MPI_terminar();
