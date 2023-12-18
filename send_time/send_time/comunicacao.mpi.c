#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  double write_time=0.0;
  const double t1=wtime();
  MPI_Send((void *) ondaPtr, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  
  write_time+=wtime()-t1;
      
  printf("send %lf\n", write_time);
}


//##########################    MPI  RECEIVE   ##################################

void salvarInformacoesExecucao(int ixStart, int ixEnd, int iyStart, int iyEnd, int izStart, int izEnd, 
		       float dx, float dy, float dz, float dt, int itCnt, char* nome){
  
   char nome_arquivo[128];

  strcpy(nome_arquivo, FNAMEBINARYPATH);
  strcat(nome_arquivo, nome);
  strcat(nome_arquivo, ".rsf");

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

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho); 

  char nome_arquivo_aux[128];
  strcpy(nome_arquivo_aux, FNAMEBINARYPATH);
  strcat(nome_arquivo_aux, nome_arquivo);
  strcat(nome_arquivo_aux, ".rsf@");

  FILE *arquivo = fopen(nome_arquivo, "w+");


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

 
  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);

  int contador = 0;
  for (int it=1; it<=st; it++) {
    contador++;
    tSim=it*dt;
    if (tSim >= tOut) {
  
      MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      double write_time=0.0;
      const double t1=wtime();
  
      fwrite((void *) onda, sizeof(float), tamanho, arquivo);
      write_time+=wtime()-t1;
      
      printf("%d execucoes\n", contador);
      printf("%d - %lf\n", itCnt, write_time);
      contador = 0;
      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }
    printf("transmissao ok\n");
    
  salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  

  double close_time=0.0;
  const double close=wtime();
  fclose(arquivo);
  close_time+=wtime()-close;
  printf("close %d - %lf\n",close_time);

  double finalize_time=0.0;
  const double finalize=wtime();
  MPI_Finalize();
  finalize_time+=wtime()-finalize;
  printf("close %d - %lf\n",finalize_time);

  
  exit(0);
}

