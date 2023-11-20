#include "../MPI.recebimento.h"

//Vars
int recebimento_ordem;
int tamanho_onda;
int tamanho_buffer_recebimento;

//Arrays
int *estatus_conexao;
int *recebimento_posicao;
float** recebimento_buffers;
MPI_Request *recebimento_requests;



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

void escreve_em_disco(int id_request)
{  

  char nome_arquivo[128];
  strcpy(nome_arquivo, "TTI.rsf");
  
  char parte[3];
  sprintf(parte, "%d", recebimento_posicao[id_request]);
  
  strcat(nome_arquivo,".part");
  strcat(nome_arquivo, parte);
  FILE *arquivo = abrirArquivo(nome_arquivo); 
  fwrite((void *) recebimento_buffers[id_request], sizeof(float), tamanho_onda, arquivo);
  fclose(arquivo);
  free(recebimento_buffers[id_request]);
  estatus_conexao[id_request] = LIVRE;
}

void nova_transferencia(int id_request, int ordem)
{
    estatus_conexao[id_request]   = TRANSFERINDO;    
    recebimento_posicao[id_request] = ordem;
    recebimento_buffers[id_request] = malloc(sizeof(float) * tamanho_onda); 
    MPI_Irecv((void *) recebimento_buffers[id_request], tamanho_onda, MPI_FLOAT, 0, ordem, MPI_COMM_WORLD, &(recebimento_requests[id_request]));
}

void atualiza_estatus()
{  
  int pronto;
  for(int i = 0; i < tamanho_buffer_recebimento; i++)
  {
    if(estatus_conexao[i] == TRANSFERINDO)
    {
      MPI_Test(&(recebimento_requests[i]) ,&pronto, MPI_STATUS_IGNORE);
      if(pronto)
        escreve_em_disco(i);
    }
  }
}

int posicao_conexao_recebimento()
{
  atualiza_estatus();

	for(int i = 0; i < tamanho_buffer_recebimento; i++)
  {
		if(estatus_conexao[i] == LIVRE)
			return i;
  }

  int proximo_indice;
  MPI_Waitany(tamanho_buffer_recebimento, recebimento_requests, &proximo_indice, MPI_STATUS_IGNORE);
  escreve_em_disco(proximo_indice);
  return proximo_indice;
}



void inicializar_recebimento(int sx, int sy, int sz, int tamanho_buffer)
{
  tamanho_onda = sx * sy * sz;
  recebimento_ordem = 1;
  tamanho_buffer_recebimento = tamanho_buffer;

  recebimento_buffers   = malloc(tamanho_buffer * sizeof(float*));
  recebimento_requests  = malloc(tamanho_buffer * sizeof(MPI_Request));
  recebimento_posicao   = malloc(tamanho_buffer * sizeof(int));
  estatus_conexao       = malloc(tamanho_buffer * sizeof(int));

  for(int i = 0; i < tamanho_buffer; i++)
	{
    estatus_conexao[i] = LIVRE;
    recebimento_posicao[i]  = 0;
    recebimento_buffers[i]  = NULL;
    recebimento_requests[i] = MPI_REQUEST_NULL;
	}
}

void escreve_tudo()
{
  int continua = 1;
  int pronto;
  while(continua)
  {
    continua = 0;
    for(int i = 0; i < tamanho_buffer_recebimento; i++)
    {
      if(estatus_conexao[i] == TRANSFERINDO)
      {
        MPI_Test(&(recebimento_requests[i]) ,&pronto, MPI_STATUS_IGNORE);
        if(pronto)
          escreve_em_disco(i);
        else
          continua = 1;
      }
    }
  }
}

void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz, int tamanho_buffer)
{
  inicializar_recebimento(sx, sy, sz, tamanho_buffer);
  float *onda = malloc(sizeof(float) * tamanho_onda); 
  FILE *arquivo = abrirArquivo(nome_arquivo);
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

  int id_request_atual = posicao_conexao_recebimento();
  
  nova_transferencia(id_request_atual, recebimento_ordem);
  recebimento_ordem++;

  int temp = 1;
  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {
  
      id_request_atual = posicao_conexao_recebimento();
      nova_transferencia(id_request_atual, recebimento_ordem);
      recebimento_ordem++;

      tOut=(++nOut)*dtOutput;
      itCnt++;
      temp++;
    }
  }
  escreve_tudo();
  salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  MPI_Finalize();
  exit(0);
}