#include <mpi.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "hdf5.h"
#define MSG_ONDA      101
#define MSG_CONTROLE  100
#define FLAG_FINALIZAR -1
#define FLAG_ONDA 1

#define FILE_H5         "TTI.h5"
#define DATASET_BASE    "TTI.part"


void MPI_escrita_disco(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt);
void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p);
