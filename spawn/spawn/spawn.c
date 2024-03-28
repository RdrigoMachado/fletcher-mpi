#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 

#define TERMINAR -1

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);


    MPI_Comm parentcomm;
    MPI_Comm_get_parent(&parentcomm);
    MPI_File thefile;
    int rank;
    int num_escrita, tamanho, deslocamento;
    float *onda = malloc(sizeof(float) * tamanho); 

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_File_open(MPI_COMM_WORLD, "TTI.rsf",
                MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &thefile);
    
printf("My rank %d\n", rank);

    MPI_Recv(&tamanho, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
printf("tamanho %d\n", tamanho);

    MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
printf("num escrita %d\n", num_escrita);

    while(num_escrita != TERMINAR)
    {   
        printf("entrei");
        MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, 102, parentcomm, MPI_STATUS_IGNORE);
        deslocamento = (num_escrita) * tamanho;
printf("rank %d escrevendo %d na posicao %d\n", rank, num_escrita, deslocamento);
        MPI_File_set_view(thefile, deslocamento * sizeof(float),
                        MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);
printf("%d set view\n", rank);
        MPI_File_write(thefile, onda, tamanho, MPI_FLOAT, MPI_STATUS_IGNORE);
printf("%d file write\n", rank);
        MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
printf("%d num escrita\n", rank);
    }

    free(onda);
    MPI_File_close(&thefile);
    MPI_Finalize();
    return 0;
}
