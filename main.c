#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank;  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Info info;
    MPI_Comm childcomm;

    char** spawn_argv = (char**)malloc((argc + 2) * sizeof(char*));

    // Copy the original command-line arguments
    for (int i = 0; i < argc; ++i) {
        spawn_argv[i] = strdup(argv[i]);
    }
    spawn_argv[argc] = "1";

    // Add additional arguments for spawned processes
    spawn_argv[argc+1] = NULL;  // Null-terminate the array
    float onda[20];
    for (int i = 0; i < 20; i++)
    {
        onda[i] = i;
    }

    MPI_Comm_spawn("spawn.x", spawn_argv, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);
    MPI_Send(onda, 20, MPI_FLOAT, 0, 101, childcomm);


    float onda[20];
    for (int i = 0; i < 20; i++)
    {
        onda[i] = 20 + i;
    }
    MPI_Comm_spawn("spawn.x", spawn_argv, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &childcomm, MPI_ERRCODES_IGNORE);
    MPI_Send(onda, 20, MPI_FLOAT, 0, 101, childcomm);









    MPI_Finalize();

    FILE* file = fopen("testfile", "rb");
    
    // Determine the file size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

      // Allocate memory to store the data
    float* data = (float*)malloc(filesize);

    // Read the data from the file
    fread(data, sizeof(float), filesize / sizeof(float), file);

    // Close the file
    fclose(file);

    // Print the read data
    printf("Data read from file:\n");
    for (long i = 0; i < filesize / sizeof(float); ++i) {
        printf("%f ", data[i]);
    }
    printf("\n");

    // Free allocated memory
    free(data);

    return 0;
}
