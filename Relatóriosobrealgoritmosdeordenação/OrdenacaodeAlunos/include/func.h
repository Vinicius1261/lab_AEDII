#ifndef FUNC_H
#define FUNC_H

typedef struct {
    char nome[50];
    float *notas;
    int num_notas;
    float media;
} Aluno;

int contar_alunos(const char *filename);
void ler_alunos(const char *filename, Aluno *alunos, int n);
void bubbleSort(Aluno *alunos, int n);
void quickSort_alunos(Aluno *alunos, int low, int high);
void mergeSort_alunos(Aluno *alunos, int l, int r);
void imprimir_relatorio(Aluno *alunos, int n);
void liberar_alunos(Aluno *alunos, int n);

#endif
