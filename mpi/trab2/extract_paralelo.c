#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void read_serie(char * arquivo, double *vetor, int tamanho){
    FILE *fp = fopen(arquivo, "r");
    if(fp==NULL){
        fprintf(stderr, "não foi possivel abrir o arquivo %s\n", arquivo);
        exit(1);
    }
    for(int i=0; i<tamanho; ++i){
        fscanf(fp, "%lf", &vetor[i]);
    }
}

void max_min_avg(double *vetor, int tamanho, double *max, double *min, double *media){
    double soma = 0;
    *max = *min = vetor[0];
    for(int i=0; i< tamanho; i++){
        soma += vetor[i];
        if (vetor[i] > *max)
            *max = vetor[i];
        if (vetor[i] < *min)
            *min = vetor[i];
    }
    *media = soma / tamanho;
}

int main(int argc, char **argv){
    if (argc != 4){
        fprintf(stderr, "necessário 2 argumentos: %s <arquivo time series> <tamanho time series> <tamanho janela>\n", argv[0]);
        return 1;
    }
    clock_t start, end;     //Variáveis para contar o tempo de execução
    double cpu_time_used;
    start = clock();

    //Começo Região paralelizável - Kernel
    MPI_Init(&argc, &argv);

    int tam_serie = atoi(argv[2]);
    int tam_janela = atoi(argv[3]);

    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *serie = (double *) malloc(sizeof(double)*tam_serie);
    read_serie(argv[1], serie, tam_serie);

    double max, min, media;
    if(rank == 0){
        //Instancia o vetor principal e printa sua parte
        fprintf(stdout,"tamanho da serie: %d, tamanho da janela: %d\n",tam_serie, tam_janela);
        max_min_avg(serie,tam_serie, &max, &min, &media);
        fprintf(stdout,"serie total - max: %lf, min: %lf, media: %lf\n", max, min, media);
        for(int i = 0; i <= tam_serie - tam_janela; i++){
            int rdest = 0;
            MPI_Recv(&rdest, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(rdest > 0)
                MPI_Send(&i, 1, MPI_INT, rdest, 0, MPI_COMM_WORLD);
        }
        int final = -1;
        for(int i = 1; i < num_proc; i++)
            MPI_Send(&final, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        free(serie);
    }else{
        while(1){
            MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            int i = 0;
            MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(i == -1){
                free(serie);
                break;
            }
            max_min_avg(&serie[i],tam_janela, &max, &min, &media);
            fprintf(stdout,"janela %d - max: %lf, min: %lf, media: %lf\n", i, max, min, media);
        }
    }
    //Fim da região paralelizável

    MPI_Barrier(MPI_COMM_WORLD); //Espera todos os processos encerrarem
    //Printa o tempo de execução
    if(rank == 0){
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        fprintf(stdout,"Time: %f \n",cpu_time_used);
    }
    MPI_Finalize();

    return 0;
}
