#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

#define TAM 2

// Funcao para imprimir toda a mensagem em apenas um unico printf
// Garantindo que a mensagem nao serah cortada por uma mensagem
// de outro processo
void imprime_vetor(int rank, int *vetor, int tamanho, char *msg_inicial){
    int i;
    char *str, strT[20];
    
    str = (char*)malloc(sizeof(char)*tamanho*20);
    str[0]='\0' ;
    
    // Salva o vetor em uma unica string
    for(i=0; i<tamanho; ++i){
        sprintf(strT, "%d ",vetor[i]);
        strcat(str,strT);
    }
    
    printf("%s processo %d, vetor: %s\n",msg_inicial,rank,str);
    free(str);
}
int main(int argc, char** argv) {
    int i;
    int *vetor_reduce, *vetor_parcial;
    
    MPI_Init(&argc, &argv);
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(time(NULL)*rank);
    // Inicializa o vetor apenas no processo 0
    vetor_parcial = (int*) malloc(sizeof(int)*TAM);
    
    for(i=0;i<TAM;++i)
        vetor_parcial[i] = rand()%10;
    imprime_vetor(rank, vetor_parcial, TAM, "Vetor parcial,");
    
    fflush(stdout);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(rank == 0)
        vetor_reduce = (int*) malloc(sizeof(int)*TAM);
    // Copia o vetor do processo 0 para todos os processos no comunicador MPI_COMM_WORLD
    MPI_Reduce(vetor_parcial, vetor_reduce, TAM, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0){
        imprime_vetor(rank,vetor_reduce,TAM, "Vetor reduce,");
    }
    free(vetor_parcial);
    
    if(rank == 0)
        free(vetor_reduce);
    
    MPI_Finalize();
}