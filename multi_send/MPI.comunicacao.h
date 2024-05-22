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
#define NOME_ARQUIVO "TTI.rsf@"

#define LIVRE               0
#define TRANSFERINDO        1
#define AGUARDANDO_ESCRITA  2
#define NENHUM_DISPONIVEL   -1
#endif