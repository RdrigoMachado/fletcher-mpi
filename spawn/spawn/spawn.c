#include <mpi.h>
#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm parentcomm;
    MPI_Comm_get_parent(&parentcomm);

    int numero_processo, tamanho;
    MPI_Recv(&numero_processo, 1, MPI_INT, 0, 100, parentcomm, MPI_STATUS_IGNORE);
    MPI_Recv(&tamanho,      1, MPI_INT, 0, 101, parentcomm, MPI_STATUS_IGNORE);
    

    MPI_File thefile;
    MPI_File_open(MPI_COMM_WORLD, "TTI",
                MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &thefile);
    MPI_File_set_view(thefile, numero_processo * tamanho * sizeof(float),
                                MPI_FLOAT, MPI_FLOAT, "native", MPI_INFO_NULL);
   

    float *onda = malloc(sizeof(float) * tamanho); 
    MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, 102, parentcomm, MPI_STATUS_IGNORE);

    MPI_File_write(thefile, onda, tamanho, MPI_FLOAT, MPI_STATUS_IGNORE);
    MPI_File_close(&thefile);
    MPI_Finalize();
    return 0;
}
