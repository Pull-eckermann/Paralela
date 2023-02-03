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

    int tam_serie = atoi(argv[2]);
    int tam_janela = atoi(argv[3]);

    //Começo Região paralelizável - Kernel
    MPI_Init(&argc, &argv);
    int num_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double max, min, media;
    if(rank == 0){
        double *serie = (double *) malloc(sizeof(double)*tam_serie);
        read_serie(argv[1], serie, tam_serie);
        printf("tamanho da serie: %d, tamanho da janela: %d\n",tam_serie, tam_janela);
        max_min_avg(serie,tam_serie, &max, &min, &media);
        printf("serie total - max: %lf, min: %lf, media: %lf\n", max, min, media);
        int dest = 0;
        for(int i = 0; i <= tam_serie - tam_janela; i++){
            double *janela = (double *) malloc(sizeof(double)*tam_janela);
            int aux = i;
            for(int j = 0; j < tam_janela; j++){
                janela[j] = serie[aux];
                aux++;
            }
            dest++;
            if(dest >= num_proc)
                dest = 1;
            MPI_Send(janela, tam_janela, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        }
    }else{
        while(1){
            double *janela = (double *) malloc(sizeof(double)*tam_janela);
            MPI_Recv(janela, tam_janela, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            max_min_avg(janela,tam_janela, &max, &min, &media);
            printf("janela %d - max: %lf, min: %lf, media: %lf\n", rank, max, min, media);
        }
    }
    //Fim da região paralelizável
}