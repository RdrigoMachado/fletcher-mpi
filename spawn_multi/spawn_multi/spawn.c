#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 
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
  estatus_conexao[id_request] = LIVRE;
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm parentcomm;
    MPI_Comm_get_parent(&parentcomm);

    int numero_processo, tamanho;
    MPI_Recv(&numero_processo, 1, MPI_INT, 0, 100, parentcomm, MPI_STATUS_IGNORE);
    MPI_Recv(&tamanho,      1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
       

    float *onda = malloc(sizeof(float) * tamanho); 
    MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, 102, parentcomm, MPI_STATUS_IGNORE);


    char nome_arquivo[128];
    strcpy(nome_arquivo, "TTI.rsf");
    
    char parte[3];
    sprintf(parte, "%d", numero_processo);
    
    strcat(nome_arquivo,".part");
    strcat(nome_arquivo, parte);
    FILE *arquivo = abrirArquivo(nome_arquivo); 
    fwrite((void *) recebimento_buffers[id_request], sizeof(float), tamanho_onda, arquivo);
    fclose(arquivo);
    MPI_Finalize();
    return 0;
}
