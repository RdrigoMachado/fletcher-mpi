#include "../MPI.recebimento.h"

int myRank;
int tamanho_grupo;
int tamanho_onda;
int tamanho_onda_bytes;
float *onda;
char recv_nome[128];


void inicializar_recebimento(int sx, int sy, int sz, int rank, int groupSize)
{
  myRank = rank;
  tamanho_grupo = groupSize;
  tamanho_onda = sx * sy * sz;
  tamanho_onda_bytes = tamanho_onda * sizeof(float);
  onda = malloc(tamanho_onda_bytes);
}



//##########################    MPI  RECEIVE   ##################################

void escreve_em_disco(int id_escrita)
{
  char nome_arquivo[128];
  strcpy(nome_arquivo, recv_nome);
  
  char parte[3];
  sprintf(parte, "%d", id_escrita);
  
  strcat(nome_arquivo,".part");
  strcat(nome_arquivo, parte);
  FILE *arquivo = fopen(nome_arquivo, "w+"); 
  fwrite((void *) onda, sizeof(float), tamanho_onda, arquivo);
  fclose(arquivo);
}

void MPI_recebimento(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt, int rank, int groupSize)
{
  inicializar_recebimento(sx, sy, sz, rank, groupSize);
  strcpy(recv_nome, NOME_ARQUIVO);
  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;
  

  int id = itCnt % (tamanho_grupo - 1);
  if(id == 0)
    id = 1;
  if(id == myRank)
  {
    MPI_Recv((void *) onda, tamanho_onda, MPI_FLOAT, 0, itCnt, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
    escreve_em_disco(itCnt);
  }

  for (int it=1; it<=st; it++) {
    tSim=it*dt;
   
    if (tSim >= tOut) {
      tOut=(++nOut)*dtOutput;
      itCnt++;
    
      id = itCnt % (tamanho_grupo - 1);
      if(id == 0)
        id = tamanho_grupo - 1;
      
      if(id == myRank)
      {
        MPI_Recv((void *) onda, tamanho_onda, MPI_FLOAT, 0, itCnt, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
        escreve_em_disco(itCnt);
      }
  
      
    }
  }
  free(onda);
  MPI_Finalize();
  exit(0);
}