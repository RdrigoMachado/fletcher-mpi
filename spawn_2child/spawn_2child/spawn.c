#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define TERMINAR -1

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);


    MPI_Comm parentcomm;
    MPI_Comm_get_parent(&parentcomm);
    MPI_File thefile;
    int rank;
    int num_escrita, tamanho;
    MPI_Offset deslocamento;
    float *onda;
    char nome_base[128];
    char nome_arquivo[128];
    char parte[5];
    FILE *arquivo;
    strcpy(nome_base, "TTI.rsf");


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Recv(&tamanho, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
    onda = malloc(sizeof(float) * tamanho); 

    MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);

    while(num_escrita != TERMINAR)
    {   
        strcpy(nome_arquivo, nome_base);
        sprintf(parte, "%d", num_escrita);
        strcat(nome_arquivo,".part");
        strcat(nome_arquivo, parte);
        arquivo = fopen(nome_arquivo, "w+"); 

        MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, 102, parentcomm, MPI_STATUS_IGNORE);

        fwrite(onda, sizeof(float), tamanho, arquivo);
        fclose(arquivo);
        
        MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
    }

    free(onda);
    MPI_Finalize();
    return 0;
}
