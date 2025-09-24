#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_NAME 100
#define N_NOTAS 3

typedef struct {
    char nome[MAX_NAME];
    float *notas;
    int num_notas;
    float media;
} Aluno;

// Conta o número de alunos (linhas do arquivo)
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

// Lê os dados dos alunos
void ler_alunos(const char *filename, Aluno *alunos, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[MAX_LINE];
    int i = 0;
    while (fgets(line, sizeof(line), fp) && i < n) {
        char *token = strtok(line, ",");

        // Copia o nome
        if (token) {
            strncpy(alunos[i].nome, token, MAX_NAME);
            alunos[i].nome[MAX_NAME - 1] = '\0';
        }

        // Aloca memória para as notas
        alunos[i].notas = malloc(N_NOTAS * sizeof(float));
        if (!alunos[i].notas) {
            printf("Erro de memória ao alocar notas.\n");
            exit(1);
        }

        // Lê as notas
        for (int j = 0; j < N_NOTAS; j++) {
            token = strtok(NULL, ",");
            if (token)
                alunos[i].notas[j] = atof(token);
            else
                alunos[i].notas[j] = 0.0f;
        }

        // Calcula a média
        float soma = 0.0f;
        for (int j = 0; j < N_NOTAS; j++)
            soma += alunos[i].notas[j];
        alunos[i].media = soma / N_NOTAS;

        i++;
    }

    fclose(fp);
}

// Função de ordenação BubbleSort (ordena por média decrescente)
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

// Imprime o relatório
void imprimir_relatorio(Aluno *alunos, int n) {
    float soma_medias = 0.0f;

    // Ordena os alunos por média (decrescente)
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

int main() {
    const char *filename = "notas.csv";
    int n = contar_alunos(filename);
    if (n <= 0) {
        printf("Erro ao abrir arquivo ou arquivo vazio.\n");
        return 1;
    }

    // Aloca memória para os alunos
    Aluno *alunos = malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("Erro de memória ao alocar alunos.\n");
        return 1;
    }

    // Lê os dados dos alunos
    ler_alunos(filename, alunos, n);

    // Imprime o relatório
    imprimir_relatorio(alunos, n);

    // Libera a memória
    for (int i = 0; i < n; i++) {
        free(alunos[i].notas);
    }
    free(alunos);

    return 0;
}
