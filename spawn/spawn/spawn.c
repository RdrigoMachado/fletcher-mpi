#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 

#define TERMINAR    -1
#define MSG_CONTROLE 101
#define MSG_ONDA     102

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, num_escrita, tamanho, sucesso;
    float *onda;
    MPI_File arquivo;
    MPI_Comm interCommParent;
    MPI_Offset deslocamento;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_get_parent(&interCommParent);
    MPI_Recv(&tamanho, 1, MPI_INT, 0, MSG_CONTROLE, interCommParent, MPI_STATUS_IGNORE);
    
    deslocamento = 70 * (MPI_Offset) tamanho;
    onda = malloc(sizeof(float) * tamanho);
    if(onda == NULL)
    {
        printf("#%d - Erro ao alocar memoria. Terminando\n", rank);
        MPI_Finalize();
        return 0;
    }
    else
        printf("#%d - Memoria alocada\n", rank);

    
    sucesso = MPI_File_open(MPI_COMM_WORLD, "TTI.rsf",
                MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &arquivo);
    if(sucesso != MPI_SUCCESS)
        printf("#%d - ERRO ao abrir arquivo\n", rank);
    else
        printf("#%d - Arquivo aberto\n", rank);

    
    sucesso = MPI_Recv(&num_escrita, 1, MPI_INT, 0, MSG_CONTROLE, interCommParent, MPI_STATUS_IGNORE);
    if(sucesso != MPI_SUCCESS)
            printf("#%d - ERRO ao receber msg controle\n", rank);
    else
            printf("#%d - Recebida msg controle\n", rank);

    while(num_escrita != TERMINAR)
    {   
        MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, interCommParent, MPI_STATUS_IGNORE);
        deslocamento = (num_escrita) * (MPI_Offset) tamanho;

        sucesso = MPI_File_set_view(arquivo, deslocamento * sizeof(float),
                        MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);
        if(sucesso != MPI_SUCCESS)
            printf("#%d - ERRO ao definir view\n", rank);
        else
            printf("#%d - view definida\n", rank);

        sucesso = MPI_File_write(arquivo, onda, tamanho, MPI_FLOAT, MPI_STATUS_IGNORE);
        if(sucesso != MPI_SUCCESS)
            printf("#%d - ERRO ao escrever no arquivo\n", rank);
        else
            printf("#%d - escrita arquivo\n", rank);

        sucesso = MPI_Recv(&num_escrita, 1, MPI_INT, 0, MSG_CONTROLE, interCommParent, MPI_STATUS_IGNORE);
        if(sucesso != MPI_SUCCESS)
            printf("#%d - ERRO ao receber msg controle\n", rank);
        else
            printf("#%d - Recebida msg controle %d\n", rank, num_escrita);

    }
    
    free(onda);
    sucesso = MPI_File_close(&arquivo);
    if(sucesso != MPI_SUCCESS)
        printf("#%d - ERRO ao fechar arquivo\n", rank);
    else
        printf("#%d - Arquivo fechado\n", rank);
    MPI_Send(&num_escrita, 1, MPI_INT, 0, 101, interCommParent);
    MPI_Finalize();
    return 0;
}
