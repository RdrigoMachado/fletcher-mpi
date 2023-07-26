#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################

float* empacotar(int sx, int sy, int sz,
		   float *ondaPtr, SlicePtr p, int tamanho){

  float *onda = malloc(sizeof(float) * tamanho);

  for (int iz=p->izStart; iz<=p->izEnd; iz++)
    for (int iy=p->iyStart; iy<=p->iyEnd; iy++)
        for (int ix=p->ixStart; ix<=p->ixEnd; ix++)
          onda[ind(p->ixStart,iy,iz) + ix] = *(ondaPtr+ind(p->ixStart,iy,iz) + ix);

  return onda;
}

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr, SlicePtr p) {

  int tamanho = sx * sy * sz;
  float *onda = empacotar(sx, sy, sz, ondaPtr, p, tamanho);

  int envio_onda = FLAG_ONDA;
  MPI_Ssend(&envio_onda, 1, MPI_INT, 1, MSG_CONTROLE, MPI_COMM_WORLD);
  MPI_Ssend((void *) onda, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  

  p->itCnt++;
}

//##########################    MPI  RECEIVE   ##################################

FILE* abrirArquivo(char* nome){
  char nome_arquivo[128];

  strcpy(nome_arquivo, FNAMEBINARYPATH);
  strcat(nome_arquivo,nome);
  strcat(nome_arquivo,".rsf@");

  return fopen(nome_arquivo, "w+");
}

void receberOnda(float *onda, int tamanho, FILE *arquivo) {

  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  fwrite((void *) onda, sizeof(float), tamanho, arquivo);
}

void finalizar(FILE *arquivo){
  fclose(arquivo);
  MPI_Finalize();
  exit(0);
}

void MPI_escrita_disco(int sx, int sy, int sz, char* nome_arquivo)
{
    int flag = 0;
    int tamanho = sx * sy * sz;
    float *onda = malloc(sizeof(float) * tamanho); 
    FILE *arquivo = abrirArquivo(nome_arquivo);

    while(1) {
      MPI_Recv(&flag, 1, MPI_INT, 0, MSG_CONTROLE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      if(flag == FLAG_FINALIZAR)
        finalizar(arquivo);

      receberOnda(onda, tamanho, arquivo);
    }
}
