#ifndef _COMMUNICACAO_MPI_
#define _COMMUNICACAO_MPI_

#include <mpi.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

#define MSG_ONDA      101
#define MSG_CONTROLE  100
#define FLAG_FINALIZAR -1
#define FLAG_ONDA 1

extern MPI_Request *request;
extern MPI_Status  *status;

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo, const int st,  const float dtOutput, const float dt, float dx, float dy, float dz);

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p);

void finalizar_comunicacao();
#endif