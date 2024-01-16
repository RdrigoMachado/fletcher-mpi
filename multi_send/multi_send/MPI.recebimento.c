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

void salvarInformacoesExecucao(int ixStart, int ixEnd, int iyStart, int iyEnd, int izStart, int izEnd, 
		       float dx, float dy, float dz, float dt, int itCnt, char* nome)
{
  
   char nome_arquivo[128];

  strcpy(nome_arquivo, FNAMEBINARYPATH);
  strcat(nome_arquivo,nome);
  strcat(nome_arquivo,".rsf");

  FILE* arquivo = fopen(nome_arquivo, "w+");

  enum sliceDirection direcao;

  if (ixStart==ixEnd)
    direcao=XSLICE;
  else if (iyStart==iyEnd)
    direcao=YSLICE;
  else if (izStart==izEnd)
    direcao=ZSLICE;
  else {
    direcao=FULL;
  }
  strcat(nome_arquivo,"@");
  fprintf(arquivo,"in=\"%s\"\n", nome_arquivo);
  fprintf(arquivo,"data_format=\"native_float\"\n");
  fprintf(arquivo,"esize=%lu\n", sizeof(float)); 
  switch(direcao) {
  case XSLICE:
    fprintf(arquivo,"n1=%d\n",iyEnd-iyStart+1);
    fprintf(arquivo,"n2=%d\n",izEnd-izStart+1);
    fprintf(arquivo,"n3=%d\n",itCnt);
    fprintf(arquivo,"d1=%f\n",dy);
    fprintf(arquivo,"d2=%f\n",dz);
    fprintf(arquivo,"d3=%f\n",dt);
    break;
  case YSLICE:
    fprintf(arquivo,"n1=%d\n",ixEnd-ixStart+1);
    fprintf(arquivo,"n2=%d\n",izEnd-izStart+1);
    fprintf(arquivo,"n3=%d\n",itCnt);
    fprintf(arquivo,"d1=%f\n",dx);
    fprintf(arquivo,"d2=%f\n",dz);
    fprintf(arquivo,"d3=%f\n",dt);
    break;
  case ZSLICE:
    fprintf(arquivo,"n1=%d\n",ixEnd-ixStart+1);
    fprintf(arquivo,"n2=%d\n",iyEnd-iyStart+1);
    fprintf(arquivo,"n3=%d\n",itCnt);
    fprintf(arquivo,"d1=%f\n",dx);
    fprintf(arquivo,"d2=%f\n",dy);
    fprintf(arquivo,"d3=%f\n",dt);
    break;
  case FULL:
    fprintf(arquivo,"n1=%d\n",ixEnd-ixStart+1);
    fprintf(arquivo,"n2=%d\n",iyEnd-iyStart+1);
    fprintf(arquivo,"n3=%d\n",izEnd-izStart+1);
    fprintf(arquivo,"n4=%d\n",itCnt);
    fprintf(arquivo,"d1=%f\n",dx);
    fprintf(arquivo,"d2=%f\n",dy);
    fprintf(arquivo,"d3=%f\n",dz);
    fprintf(arquivo,"d4=%f\n",dt);
    break;
  }
  fclose(arquivo);

}

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

void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo, const int st,  const float dtOutput, 
                    const float dt, float dx, float dy, float dz, int rank, int groupSize)
{
  inicializar_recebimento(sx, sy, sz, rank, groupSize);
  strcpy(recv_nome, nome_arquivo);
  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;
  

  int id = itCnt % (tamanho_grupo - 1);
  if(id == 0)
    id = 1;
  if(id == myRank)
  {
    printf("rank %d recebendo %d\n", myRank, itCnt);
    MPI_Recv((void *) onda, tamanho_onda, MPI_FLOAT, 0, itCnt, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
    escreve_em_disco(itCnt);
  }

  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {

      id = itCnt % (tamanho_grupo - 1);
      if(id == 0)
        id = 1;
      
      if(id == myRank)
      {
        printf("rank %d recebendo %d\n", myRank, itCnt);
        MPI_Recv((void *) onda, tamanho_onda, MPI_FLOAT, 0, itCnt, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
        escreve_em_disco(itCnt);
      }
  
      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }

  salvarInformacoesExecucao(0, sx-1, 0, sy-1, 0, sz-1, dx, dy, dz, dt, itCnt, nome_arquivo);
  free(onda);
  MPI_Finalize();
  exit(0);
}