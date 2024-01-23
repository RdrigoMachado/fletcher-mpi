#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    MPI_Info info;
    MPI_Comm parentcomm;

    char** spawn_argv = (char**)malloc((argc + 2) * sizeof(char*));

    // Copy the original command-line arguments
    for (int i = 0; i < argc; ++i) {
        spawn_argv[i] = strdup(argv[i]);
    }
    spawn_argv[argc] = "1";

    // Add additional arguments for spawned processes
    spawn_argv[argc+1] = NULL;  // Null-terminate the array

    
    
    MPI_Comm_spawn("spawn.x", spawn_argv, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &parentcomm, MPI_ERRCODES_IGNORE);
    
    MPI_Finalize();
    return 0;
}
