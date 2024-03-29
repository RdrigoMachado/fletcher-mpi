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
    int num_escrita, tamanho;
    MPI_Offset deslocamento;
    float *onda;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_File_open(MPI_COMM_WORLD, "TTI.rsf",
                MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &thefile);
    
    MPI_Recv(&tamanho, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
    onda = malloc(sizeof(float) * tamanho); 

    MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);

    deslocamento = 70 * (MPI_Offset) tamanho;
printf("rank %d escrevendo %d na posicao %lld size %d\n", rank, num_escrita, deslocamento, sizeof(MPI_Offset));
    while(num_escrita != TERMINAR)
    {   
printf("rank %d comecou laco\n", rank);

        MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, 102, parentcomm, MPI_STATUS_IGNORE);
        deslocamento = (num_escrita) * (MPI_Offset) tamanho;
printf("rank %d escrevendo %d na posicao %lld\n", rank, num_escrita, deslocamento);
        MPI_File_set_view(thefile, deslocamento * sizeof(float),
                        MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);

printf("rank %d escrevendo\n", rank);
        MPI_File_write(thefile, onda, tamanho, MPI_FLOAT, MPI_STATUS_IGNORE);
printf("rank %d recebendo num escrita\n", rank);
        MPI_Recv(&num_escrita, 1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
printf("rank %d recebeu instrucoes\n", rank);

    }
    

printf("Barreira child %d waiting\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);
printf("Barreira child %d ended\n", rank);
    

printf("RANK %d exiting\n", rank);
    free(onda);
    MPI_File_close(&thefile);
    MPI_Finalize();
printf("RANK %d returning\n", rank);
    return 0;
}
