#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // Accessing command line arguments
    printf("Number of arguments: %d\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    MPI_Finalize();
    return 0;
}
