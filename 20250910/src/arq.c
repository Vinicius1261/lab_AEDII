#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/func.h"


int main() {
    const char *filename = "../csv/notas.csv";
    int n = contar_alunos(filename);
    if (n <= 0) {
        printf("Erro ao abrir arquivo ou arquivo vazio.%d\n", n);
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
