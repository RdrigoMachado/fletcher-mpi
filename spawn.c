#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("rank %d\n", rank);

    float onda[20];

    // Accessing command line arguments
    printf("Number of arguments: %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    MPI_Comm parentcomm;
    MPI_Comm_get_parent(&parentcomm);

    MPI_Recv(onda, 20, MPI_FLOAT, 0, 101, parentcomm, MPI_STATUS_IGNORE);


    printf("onda\n");
    for(int i = 0; i < 20; i++)
    {
        printf("%f ", onda[i]);
    }
    printf("\n");


    MPI_Finalize();
    return 0;
}
