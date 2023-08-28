#include "../comunicacao.mpi.h"

MPI_Request *request;
MPI_Status  *status;

//#############################    MPI  SEND   ##################################
float* empacotar(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{

  int tamanho = sx * sy * sz * sizeof(float);
  float *onda = malloc(sizeof(float) * tamanho);

  memcpy(onda, ondaPtr, tamanho);
  return onda;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p) 
{

  int tamanho = sx * sy * sz;
  float *onda = empacotar(sx, sy, sz, ondaPtr, p);
  int livre;
  
  if(request!=NULL)
  {
      MPI_Wait(request, status);
  } 
  else 
  {
    request = (MPI_Request*) malloc(sizeof(MPI_Request));
    status = (MPI_Status*) malloc(sizeof(MPI_Request));
  }
  int envio_onda = FLAG_ONDA;
  MPI_Send(&envio_onda, 1, MPI_INT, 1, MSG_CONTROLE, MPI_COMM_WORLD);
 
  MPI_Isend((void *) onda, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD, request);  
  p->itCnt++;
}

void  finalizar_comunicacao()
{
  MPI_Wait(request, status);
  MPI_Finalize();
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

FILE* abrirArquivo(char* nome)
{
  char nome_arquivo[128];

  strcpy(nome_arquivo, FNAMEBINARYPATH);
  strcat(nome_arquivo,nome);
  strcat(nome_arquivo,".rsf@");

  return fopen(nome_arquivo, "w+");
}

void finalizar(FILE *arquivo)
{
  fclose(arquivo);
  MPI_Finalize();
  exit(0);
}

void receberOnda(float *onda, int tamanho, FILE *arquivo) 
{
  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);
}

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
    int flag = 0;
    int tamanho = sx * sy * sz;
    float *onda = malloc(sizeof(float) * tamanho); 
    FILE *arquivo = abrirArquivo(nome_arquivo);
    int ixStart=0;
    int ixEnd=sx-1;
    int iyStart=0;
    int iyEnd=sy-1;
    int izStart=0;
    int izEnd=sz-1;
    int itCnt = 0;

    while(1) {
      MPI_Recv(&flag, 1, MPI_INT, 0, MSG_CONTROLE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      if(flag == FLAG_FINALIZAR)
      {
        salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
        finalizar(arquivo);
      }
      
      receberOnda(onda, tamanho, arquivo);
      itCnt++;
    }

}
