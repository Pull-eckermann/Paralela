#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int num, i;
    MPI_Init(&argc, &argv);
    
    srand(time(0));
    
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0) {
        for(i=1; i<num_proc; ++i) {
            num = rand() % 100;
            MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("%d, enviando numero %d para rank %d\n",rank, num, i);
        }
    }
    else {
        MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%d, numero recebido: %d\n",rank, num);
    }
    
    MPI_Finalize();
}