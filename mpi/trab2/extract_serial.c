#include <stdio.h>
#include <stdlib.h>
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

    int tam_serie = atoi(argv[2]);
    int tam_janela = atoi(argv[3]);
    
    //Inicio da região paralelizável
    double *serie = (double *) malloc(sizeof(double)*tam_serie);
    read_serie(argv[1], serie, tam_serie);
    printf("tamanho da serie: %d, tamanho da janela: %d\n",tam_serie, tam_janela);
    double max, min, media;
    max_min_avg(serie,tam_serie, &max, &min, &media);
    printf("serie total - max: %lf, min: %lf, media: %lf\n", max, min, media);
    for(int i = 0; i <= tam_serie - tam_janela; i++){
        max_min_avg(&serie[i],tam_janela, &max, &min, &media);
        printf("janela %d - max: %lf, min: %lf, media: %lf\n", i, max, min, media);
    }
    //Fim da região paralelizável

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time#%f#\n",cpu_time_used);

    return 0;
}