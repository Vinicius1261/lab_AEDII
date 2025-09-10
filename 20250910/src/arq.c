#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_NAME 100
#define N_NOTAS 3  // ajuste conforme o número de notas no CSV

typedef struct {
    char nome[MAX_NAME];
    float notas[N_NOTAS];
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
            alunos[i].nome[MAX_NAME-1] = '\0';
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

void imprimir_relatorio(Aluno *alunos, int n) {
    float soma_medias = 0.0f;
    printf("Relatorio de Notas:\n");
    printf("%-20s", "Nome");
    for (int j = 0; j < N_NOTAS; j++)
        printf("Nota%d ", j+1);
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

    Aluno *alunos = malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("Erro de memoria.\n");
        return 1;
    }

    ler_alunos(filename, alunos, n);
    imprimir_relatorio(alunos, n);

    free(alunos);
    return 0;
}