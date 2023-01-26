#include <stdio.h>
#include <mpi.h>

int main () {
    static long num_steps = 100000;
    double step;
    //Inicia paralelização
    MPI_Init(&argc, &argv);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int i; double x, pi, sum = 0.0;
    step = 1.0/(double) num_steps;
    
    for (i=0;i< num_steps; i++){
        x = (i + 0.5) * step; // Largura do retângulo
        sum = sum + 4.0 / (1.0 + x*x); // Sum += Área do retângulo
    }

    pi = step * sum;
    printf("PI: %f\n",pi);
}