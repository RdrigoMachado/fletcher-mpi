#include "../comunicacao.mpi.h"
#define MAX_CONEXOES    10
#define LIVRE            0
#define TRANSFERINDO     1

#define CHEIO           -1

int estatus_conexao[MAX_CONEXOES];
int posicao[MAX_CONEXOES];
float* buffers[MAX_CONEXOES];
MPI_Request requests[MAX_CONEXOES];
MPI_Status  status[MAX_CONEXOES];
int ordem;
int tamanho;

void init(int sx, int sy, int sz)
{
  tamanho = sx * sy * sz * sizeof(float);

  ordem = 1;

  for(int i = 0; i < MAX_CONEXOES; i++)
	{
    estatus_conexao[i] = LIVRE;
    posicao[i]  = 0;
    buffers[i]  = NULL;
    requests[i] = MPI_REQUEST_NULL;
	}
}


//#############################    MPI  SEND   ##################################
float* empacotar(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{

  int tamanho = sx * sy * sz * sizeof(float);
  float *onda = malloc(sizeof(float) * tamanho);

  memcpy(onda, ondaPtr, tamanho);
  buffers[ordem-1] = onda;
  return onda;
}


void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p)
{

  int tamanho = sx * sy * sz;
  float *onda = empacotar(sx, sy, sz, ondaPtr, p);
  int livre;
  
  MPI_Isend((void *) onda, tamanho , MPI_FLOAT, 1, ordem, MPI_COMM_WORLD, &(requests[ordem-1]));
  ordem++;
  p->itCnt++;
}

void  finalizar_comunicacao()
{
  MPI_Waitall(MAX_CONEXOES, requests, status);
  MPI_Finalize();
  for(int i =0; i<ordem; i++)
  {
    free(buffers[i]);  
  }
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

void escreve_em_disco(int id_request)
{  
  char nome_arquivo[128];
  strcpy(nome_arquivo, FNAMEBINARYPATH);
  
  char parte[3];
  sprintf(parte, "%d", posicao[id_request]);
  
  strcat(nome_arquivo,".part");
  strcat(nome_arquivo, parte);
  
  FILE *arquivo = abrirArquivo(nome_arquivo); 
  fwrite((void *) buffers[id_request], sizeof(float), tamanho, arquivo);
  fclose(arquivo);
  free(buffers[id_request]);
}

void atualiza_status()
{
    for(int i = 0; i < MAX_CONEXOES; i++)
    {
		if(requests[i] == MPI_REQUEST_NULL && estatus_conexao[i] == TRANSFERINDO)
        {
            escreve_em_disco(i);
            estatus_conexao[i] = LIVRE;           
        }
    }
}

int get_indice_nova_comunicacao()
{
    atualiza_status();

	for(int i = 0; i < MAX_CONEXOES; i++)
		if(requests[i] == MPI_REQUEST_NULL && estatus_conexao[i] == LIVRE)
			return i;
    
    int proximo_indice;
    MPI_Status *status_temp;
    MPI_Waitany(MAX_CONEXOES, requests, &proximo_indice, status_temp);
	
    return proximo_indice;
}

void nova_transferencia(int id_request, int ordem)
{
    estatus_conexao[id_request]   = TRANSFERINDO;    
    posicao[id_request]  = ordem;
    buffers[id_request] = malloc(sizeof(float) * tamanho); 
    MPI_Irecv((void *) buffers[id_request], tamanho, MPI_FLOAT, ordem, ordem, MPI_COMM_WORLD, &(requests[id_request]));
}

void finalizar(FILE *arquivo)
{
  MPI_Waitall(MAX_CONEXOES, requests, status);
  atualiza_status();
  MPI_Finalize();
  exit(0);
}


void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo,
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
  init(sx, sy, sz);
  float *onda = malloc(sizeof(float) * tamanho); 
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

  int id_request_atual = get_indice_nova_comunicacao();
  nova_transferencia(id_request_atual, ordem);
  ordem++;

  int temp = 1;
  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {

      id_request_atual = get_indice_nova_comunicacao();
      nova_transferencia(id_request_atual, ordem);
      ordem++;

      tOut=(++nOut)*dtOutput;
      itCnt++;
      temp++;
    }
  }
  salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  finalizar(arquivo);
}