#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../include/func.h"

int contar_alunos(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) > 1) count++;
    }
    fclose(fp);
    return count;
}

void ler_alunos(const char *filename, Aluno *alunos, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    char line[256];
    for (int i = 0; i < n; i++) {
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        char *token = strtok(line, ",");
        strcpy(alunos[i].nome, token);
        alunos[i].notas = NULL;
        int notas_count = 0;
        float soma = 0;
        while ((token = strtok(NULL, ",")) != NULL) {
            alunos[i].notas = realloc(alunos[i].notas, (notas_count + 1) * sizeof(float));
            alunos[i].notas[notas_count] = atof(token);
            soma += alunos[i].notas[notas_count];
            notas_count++;
        }
        alunos[i].num_notas = notas_count;
        alunos[i].media = (notas_count > 0) ? soma / notas_count : 0;
    }
    fclose(fp);
}

void bubbleSort(Aluno *alunos, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (alunos[j].media < alunos[j + 1].media) {
                Aluno temp = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = temp;
                Sleep(400);
            }
        }
    }
}

int particiona(Aluno *alunos, int low, int high) {
    float pivo = alunos[high].media;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (alunos[j].media > pivo) {
            i++;
            Aluno temp = alunos[i];
            alunos[i] = alunos[j];
            alunos[j] = temp;
            Sleep(400);
        }
    }
    Aluno temp = alunos[i + 1];
    alunos[i + 1] = alunos[high];
    alunos[high] = temp;
    return i + 1;
}

void quickSort_alunos(Aluno *alunos, int low, int high) {
    if (low < high) {
        int pi = particiona(alunos, low, high);
        quickSort_alunos(alunos, low, pi - 1);
        quickSort_alunos(alunos, pi + 1, high);
    }
}

void merge(Aluno *alunos, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Aluno *L = malloc(n1 * sizeof(Aluno));
    Aluno *R = malloc(n2 * sizeof(Aluno));
    for (int i = 0; i < n1; i++) L[i] = alunos[l + i];
    for (int j = 0; j < n2; j++) R[j] = alunos[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].media >= R[j].media)
            alunos[k++] = L[i++];
        else
            alunos[k++] = R[j++];
        Sleep(400);
    }
    while (i < n1) alunos[k++] = L[i++];
    while (j < n2) alunos[k++] = R[j++];
    free(L);
    free(R);
}

void mergeSort_alunos(Aluno *alunos, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort_alunos(alunos, l, m);
        mergeSort_alunos(alunos, m + 1, r);
        merge(alunos, l, m, r);
    }
}

void imprimir_relatorio(Aluno *alunos, int n) {
    printf("\n%20s Média\n", "Nome");
    for (int i = 0; i < n; i++) {
        printf("%20s %.2f\n", alunos[i].nome, alunos[i].media);
    }
}

void liberar_alunos(Aluno *alunos, int n) {
    if (!alunos) return;
    for (int i = 0; i < n; i++) free(alunos[i].notas);
}
