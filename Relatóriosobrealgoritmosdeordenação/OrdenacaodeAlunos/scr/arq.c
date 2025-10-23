#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <windows.h>
#include <direct.h>
#include <errno.h>
#include "../include/func.h"

// Copia alunos com notas
void copiar_alunos(Aluno *dest, Aluno *orig, int n) {
    for (int i = 0; i < n; i++) {
        strcpy(dest[i].nome, orig[i].nome);
        dest[i].num_notas = orig[i].num_notas;
        dest[i].media = orig[i].media;

        if (orig[i].num_notas > 0) {
            dest[i].notas = malloc(orig[i].num_notas * sizeof(float));
            for (int j = 0; j < orig[i].num_notas; j++)
                dest[i].notas[j] = orig[i].notas[j];
        } else {
            dest[i].notas = NULL;
        }
    }
}

// Gera CSV com notas e média dos alunos
void gerar_csv_medias(const char *pasta, const char *filename, Aluno *alunos, int n) {
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s\\%s", pasta, filename);
    FILE *fp = fopen(caminho, "w");
    if (!fp) { perror("Erro ao abrir CSV"); return; }

    fprintf(fp, "Nome,Notas,Média\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,", alunos[i].nome);
        for (int j = 0; j < alunos[i].num_notas; j++) {
            fprintf(fp, "%.2f", alunos[i].notas[j]);
            if (j < alunos[i].num_notas - 1) fprintf(fp, ",");
        }
        fprintf(fp, ",%.2f\n", alunos[i].media);
    }

    fclose(fp);
}

// Plota gráfico usando Gnuplot
void plotar_grafico(const char *arquivo_csv) {
    FILE *gp = _popen("gnuplot -persistent", "w");
    if (!gp) { printf("Erro: instale o Gnuplot.\n"); return; }

    fprintf(gp, "set title 'Comparação de Algoritmos de Ordenação'\n");
    fprintf(gp, "set xlabel 'Instância (arquivo)'\n");
    fprintf(gp, "set ylabel 'Tempo (s)'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set style data linespoints\n");
    fprintf(gp, "set xtics rotate by -45\n");
    fprintf(gp, "set datafile separator ','\n");

    fprintf(gp,
        "plot '%s' using 2:xtic(1) title 'Bubble', "
        "'%s' using 3:xtic(1) title 'Quick', "
        "'%s' using 4:xtic(1) title 'Merge'\n",
        arquivo_csv, arquivo_csv, arquivo_csv);

    fflush(gp);
    _pclose(gp);
}

int main() {
    const char *pasta_csv = "csv";          // pasta de entrada dentro de scr
    const char *pasta_saida = "..\\saida";  // pasta de saída uma acima de scr

    // Cria a pasta de saída, se não existir
    if (_mkdir(pasta_saida) != 0 && errno != EEXIST) {
        perror("Erro ao criar pasta de saída");
        return 1;
    }

    // Caminho completo do CSV final
    char caminho_final[512];
    snprintf(caminho_final, sizeof(caminho_final), "%s\\log_medias.csv", pasta_saida);

    FILE *log_final = fopen(caminho_final, "w");
    if (!log_final) {
        perror("Erro ao criar CSV final");
        return 1;
    }
    fprintf(log_final, "Arquivo,Bubble,Quick,Merge\n");

    DIR *dir = opendir(pasta_csv);
    if (!dir) { perror("Erro ao abrir pasta de entrada"); fclose(log_final); return 1; }

    struct dirent *entry;
    int rep_count = 5;

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        int len = (int)strlen(entry->d_name);
        if (len < 4 || strcmp(entry->d_name + len - 4, ".csv") != 0) continue;

        char caminho_csv[512];
        snprintf(caminho_csv, sizeof(caminho_csv), "%s\\%s", pasta_csv, entry->d_name);

        int n = contar_alunos(caminho_csv);
        if (n <= 0) continue;

        Aluno *orig = malloc(n * sizeof(Aluno));
        Aluno *a1 = malloc(n * sizeof(Aluno));
        Aluno *a2 = malloc(n * sizeof(Aluno));
        Aluno *a3 = malloc(n * sizeof(Aluno));
        if (!orig || !a1 || !a2 || !a3) { free(orig); free(a1); free(a2); free(a3); continue; }

        ler_alunos(caminho_csv, orig, n);

        double soma_bubble = 0.0, soma_quick = 0.0, soma_merge = 0.0;

        for (int rep = 1; rep <= rep_count; rep++) {
            copiar_alunos(a1, orig, n);
            copiar_alunos(a2, orig, n);
            copiar_alunos(a3, orig, n);

            clock_t ini;

            // Bubble Sort
            ini = clock();
            bubbleSort(a1, n);
            double tempo_bubble = (double)(clock() - ini) / CLOCKS_PER_SEC;
            soma_bubble += tempo_bubble;
            char csv_bubble[512];
            snprintf(csv_bubble, sizeof(csv_bubble), "bubble_%s_exec%d.csv", entry->d_name, rep);
            gerar_csv_medias(pasta_saida, csv_bubble, a1, n);

            // Quick Sort
            ini = clock();
            quickSort_alunos(a2, 0, n - 1);
            double tempo_quick = (double)(clock() - ini) / CLOCKS_PER_SEC;
            soma_quick += tempo_quick;
            char csv_quick[512];
            snprintf(csv_quick, sizeof(csv_quick), "quick_%s_exec%d.csv", entry->d_name, rep);
            gerar_csv_medias(pasta_saida, csv_quick, a2, n);

            // Merge Sort
            ini = clock();
            mergeSort_alunos(a3, 0, n - 1);
            double tempo_merge = (double)(clock() - ini) / CLOCKS_PER_SEC;
            soma_merge += tempo_merge;
            char csv_merge[512];
            snprintf(csv_merge, sizeof(csv_merge), "merge_%s_exec%d.csv", entry->d_name, rep);
            gerar_csv_medias(pasta_saida, csv_merge, a3, n);

            liberar_alunos(a1, n);
            liberar_alunos(a2, n);
            liberar_alunos(a3, n);
        }

        double media_bubble = soma_bubble / rep_count;
        double media_quick = soma_quick / rep_count;
        double media_merge = soma_merge / rep_count;

        fprintf(log_final, "%s,%.6f,%.6f,%.6f\n", entry->d_name, media_bubble, media_quick, media_merge);

        liberar_alunos(orig, n);
        free(orig); free(a1); free(a2); free(a3);
    }

    closedir(dir);
    fclose(log_final);

    printf("\nProcesso concluído! CSV final em '%s\\log_medias.csv'\n", pasta_saida);

    // Converte para caminho absoluto para Gnuplot
    char caminho_abs[512];
    GetFullPathNameA(caminho_final, 512, caminho_abs, NULL);
    plotar_grafico(caminho_abs);

    return 0;
}
