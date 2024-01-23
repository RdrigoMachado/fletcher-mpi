#include "../MPI.recebimento.h"

//Vars
int sequencia;
int tamanho;
char nome_arquivo[128];
char parte[3];

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


void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz, int par_impar)
{

  int parOuImpar = par_impar;
  tamanho = sx * sy * sz;
  sequencia = 1;
  
  float *onda = malloc(sizeof(float) * tamanho); 
  int ixStart=0;
  int ixEnd=sx-1;
  int iyStart=0;
  int iyEnd=sy-1;
  int izStart=0;
  int izEnd=sz-1;

  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;

  
  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, sequencia, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // strcpy(nome_arquivo, "TTI.rsf");
  // strcat(nome_arquivo,".part");
  // sprintf(parte, "%d", sequencia);
  // strcat(nome_arquivo, parte);
  
  FILE *arquivo = fopen(nome_arquivo, "w+");
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);  
  fclose(arquivo);
  sequencia++;

  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {
      MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, sequencia, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // strcpy(nome_arquivo, "TTI.rsf");
      // strcat(nome_arquivo,".part");
      // sprintf(parte, "%d", sequencia);
      // strcat(nome_arquivo, parte);
      
      FILE *arquivo = fopen(nome_arquivo, "a");
      fwrite((void *) onda, sizeof(float), tamanho, arquivo);  
      fclose(arquivo);
      sequencia++;

      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }
  if(parOuImpar == 0)
    salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  MPI_Finalize();
  exit(0);
}