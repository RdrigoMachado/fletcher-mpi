#include "../MPI.recebimento.h"

int recv_byte_size;
int recv_numero_escrita;
int recv_pos_atual;
float *recv_buffer[TAMANHO_BUFFER];
MPI_Request recv_requests[TAMANHO_BUFFER];


char recv_nome[128];


void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo, const int st,  const float dtOutput,
                        const float dt, float dx, float dy, float dz);
{
  strcpy(recv_nome, nome_arquivo);
  recv_byte_size = (sx * sy * sz) * sizeof(float);
  recv_numero_escrita = 1;
  for(int i = 0; i < TAMANHO_BUFFER; i++)
	{
    recv_buffer[i]  = malloc(byte_size);
    recv_requests[i] = MPI_REQUEST_NULL;
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

void escreve_em_disco()
{  
  char nome_arquivo[128];
  strcpy(nome_arquivo, recv_nome);
  
  char parte[3];
  sprintf(parte, "%d", recebimento_posicao[recv_numero_escrita]);
  
  strcat(nome_arquivo,".part");
  strcat(nome_arquivo, parte);
  FILE *arquivo = abrirArquivo(nome_arquivo); 
  fwrite((void *) recv_buffes[recv_numero_escrita], sizeof(float), recv_byte_size, arquivo);
  fclose(arquivo);
  recv_numero_escrita++;
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
                const int st,  const float dtOutput, const float dt, float dx, float dy, float dz, int tamanho_buffer, int par_impar)
{
  inicializar_recebimento(sx, sy, sz, tamanho_buffer, par_impar);
  float *onda = malloc(sizeof(float) * tamanho_onda); 
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

  MPI_Irecv((void *) recv_buffers[0], recv_byte_size, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &(recv_requests[0]));

  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {
      
        if(it >= TAMANHO_BUFFER)
        {
          recv_pos_atual = it % TAMANHO_BUFFER;
          MPI_Wait(&(recv_requests[recv_pos_atual]), MPI_STATUS_IGNORE);
          escreve_em_disco();
        }
        MPI_Irecv((void *) recv_buffers[recv_pos_atual], recv_byte_size, MPI_FLOAT, 0, it+1, MPI_COMM_WORLD, &(recv_requests[recv_pos_atual]));

      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }
  escreve_tudo();
  salvarInformacoesExecucao(ixStart, ixEnd, iyStart, iyEnd, izStart, izEnd, dx, dy, dz, dt, itCnt, nome_arquivo);
  MPI_Finalize();
  exit(0);
}