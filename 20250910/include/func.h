#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256
#define MAX_NAME 100
#define N_NOTAS 3

typedef struct {
    char nome[MAX_NAME];
    float *notas;
    int num_notas;
    float media;
} Aluno;


int contar_alunos(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;
    int count = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        count++;
    }
    fclose(fp);
    return count;
}


void ler_alunos(const char *filename, Aluno *alunos, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[MAX_LINE];
    int i = 0;
    while (fgets(line, sizeof(line), fp) && i < n) {
        char *token = strtok(line, ",");

      
        if (token) {
            strncpy(alunos[i].nome, token, MAX_NAME);
            alunos[i].nome[MAX_NAME - 1] = '\0';
        }

        
        alunos[i].notas = malloc(N_NOTAS * sizeof(float));
        if (!alunos[i].notas) {
            printf("Erro de memória ao alocar notas.\n");
            exit(1);
        }

        
        for (int j = 0; j < N_NOTAS; j++) {
            token = strtok(NULL, ",");
            if (token)
                alunos[i].notas[j] = atof(token);
            else
                alunos[i].notas[j] = 0.0f;
        }

        
        float soma = 0.0f;
        for (int j = 0; j < N_NOTAS; j++)
            soma += alunos[i].notas[j];
        alunos[i].media = soma / N_NOTAS;

        i++;
    }

    fclose(fp);
}

void quicksort(Aluno *alunos, int inicio, int fim){
    if(inicio < fim){
        float pivo = alunos[fim].media;
        int i = inicio - 1;
        for (int j = inicio; j< fim; j++){
            if(alunos[j].media > pivo){
                i++;
                Aluno temp = alunos[i];
                alunos[i] = alunos [j];
                alunos[j] = temp;
            }
        }
        i++;
        Aluno temp = alunos[i];
        alunos[i] = alunos[fim];
        alunos[fim] = temp;
        quicksort(alunos, inicio, i-1);
        quicksort(alunos, i+1, fim);
    }

}

void mergesort(Aluno *alunos, int n){

}


void bubbleSort(Aluno *alunos, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (alunos[j].media < alunos[j + 1].media) {
                Aluno temp = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = temp;
            }
        }
    }
}


void imprimir_relatorio(Aluno *alunos, int n) {
    float soma_medias = 0.0f;


    bubbleSort(alunos, n);

    printf("Relatorio de Notas:\n");
    printf("%-20s", "Nome");
    for (int j = 0; j < N_NOTAS; j++)
        printf("Nota%d  ", j + 1);
    printf("Media\n");

    for (int i = 0; i < n; i++) {
        printf("%-20s", alunos[i].nome);
        for (int j = 0; j < N_NOTAS; j++)
            printf("%6.2f ", alunos[i].notas[j]);
        printf("%6.2f\n", alunos[i].media);
        soma_medias += alunos[i].media;
    }

    printf("\nMedia global da turma: %.2f\n", soma_medias / n);
}
