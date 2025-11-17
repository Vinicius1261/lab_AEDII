#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/func.h"


int main() {
    const char *filename = "../csv/notas.csv";
    int n = contar_alunos(filename);
    if (n <= 0) {
        printf("Erro ao abrir arquivo ou arquivo vazio.%d\n", n);
        return 1;
    }

    Aluno *alunos = malloc(n * sizeof(Aluno));
    Aluno *alunosquick = malloc(n * sizeof(Aluno));
    if (!alunos) {
        printf("Erro de memória ao alocar alunos.\n");
        return 1;
    }

    
    ler_alunos(filename, alunos, n);
    ler_alunos(filename, alunosquick, n);
    bubbleSort(alunos, n);
    quicksort(alunosquick, 0, n-1);
    imprimir_relatorio(alunos, n);
    imprimir_relatorio(alunosquick, n);

    
    for (int i = 0; i < n; i++) {
        free(alunos[i].notas);
    }
    free(alunos);
    free(alunosquick);


    return 0;
}
