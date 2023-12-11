#include "../MPI.recebimento.h"

int recv_byte_size;
int recv_size;
int recv_numero_escrita;
int recv_pos_atual;
float *recv_buffer[TAMANHO_BUFFER];
MPI_Request recv_requests[TAMANHO_BUFFER];


char recv_nome[128];


void inicializar_recebimento(int sx, int sy, int sz)
{
  recv_size = sx * sy * sz;
  recv_byte_size = recv_size * sizeof(float);
  recv_numero_escrita = 1;
  for(int i = 0; i < TAMANHO_BUFFER; i++)
	{
    recv_buffer[i]  = malloc(recv_byte_size);
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
  sprintf(parte, "%d", recv_numero_escrita);
  
  strcat(nome_arquivo,".part");
  strcat(nome_arquivo, parte);
  FILE *arquivo = fopen(nome_arquivo, "w+"); 
  int posicao = (recv_numero_escrita - 1) % TAMANHO_BUFFER;
  fwrite((void *) recv_buffer[posicao], sizeof(float), recv_size, arquivo);
  fclose(arquivo);
  recv_numero_escrita++;
}

void MPI_recebimento(int sx, int sy, int sz, char* nome_arquivo, const int st,  const float dtOutput, const float dt, float dx, float dy, float dz)
{
  inicializar_recebimento(sx, sy, sz);
  strcpy(recv_nome, nome_arquivo);
  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;
  recv_pos_atual = 0;

  printf("recv recebendo %d\n", recv_pos_atual);
  MPI_Irecv((void *) recv_buffer[recv_pos_atual], recv_size, MPI_FLOAT, 0, itCnt, MPI_COMM_WORLD, &(recv_requests[0]));
  
  
  for (int it=1; it<=st; it++) {
    tSim=it*dt;
    if (tSim >= tOut) {

        if(itCnt >= TAMANHO_BUFFER)
        {
          recv_pos_atual = (recv_numero_escrita - 1) % TAMANHO_BUFFER;
          printf("recv esperando %d\n", recv_pos_atual);
          MPI_Wait(&(recv_requests[recv_pos_atual]), MPI_STATUS_IGNORE);
          escreve_em_disco();
        } else {
          recv_pos_atual++;
        }
        printf("recv recebendo %d\n", recv_pos_atual);
        MPI_Irecv((void *) recv_buffer[recv_pos_atual], recv_size, MPI_FLOAT, 0, itCnt+1, MPI_COMM_WORLD, &(recv_requests[recv_pos_atual]));

        tOut=(++nOut)*dtOutput;
        itCnt++;
    }
  }

  while (recv_numero_escrita <= itCnt)
  {
    recv_pos_atual = (recv_numero_escrita - 1) % TAMANHO_BUFFER;
    MPI_Wait(&(recv_requests[recv_pos_atual]), MPI_STATUS_IGNORE);
    escreve_em_disco();
  }
  salvarInformacoesExecucao(0, sx-1, 0, sy-1, 0, sz-1, dx, dy, dz, dt, itCnt, nome_arquivo);
  MPI_Finalize();
  exit(0);
}