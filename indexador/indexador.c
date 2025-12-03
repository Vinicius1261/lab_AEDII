#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>  // Para suporte ao português

#ifdef _WIN32
#include <windows.h>  // Para configurações do console no Windows
#endif

#define MAX_WORD 100
#define MAX_FILES 2   // Agora são sempre 2 arquivos
#define MAX_FILENAME 256

// Struct para nós da árvore de arquivos (agora com contagem de frequência)
typedef struct arv_arquivo {
    int arquivo_id;  // Índice do arquivo
    int frequencia;  // Quantas vezes a palavra aparece neste arquivo
    struct arv_arquivo *esq, *dir;
} ArvArquivo;

// Struct para nós da árvore de palavras
typedef struct arv_palavra {
    char palavra[MAX_WORD];
    ArvArquivo *arquivos;  // Árvore de arquivos que contêm esta palavra
    struct arv_palavra *esq, *dir;
} ArvPalavra;

// Struct para armazenar informações dos arquivos
typedef struct {
    char nome[MAX_FILENAME];
    FILE *ponteiro;
} ArquivoInfo;

// Funções para a árvore de arquivos
ArvArquivo* criar_no_arquivo(int id) {
    ArvArquivo* novo = (ArvArquivo*)malloc(sizeof(ArvArquivo));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar no de arquivo.\n");
        exit(EXIT_FAILURE);
    }
    novo->arquivo_id = id;
    novo->frequencia = 1;  // Inicializa com 1 ocorrência
    novo->esq = novo->dir = NULL;
    return novo;
}

ArvArquivo* inserir_arquivo(ArvArquivo* raiz, int id) {
    if (raiz == NULL) return criar_no_arquivo(id);

    if (id < raiz->arquivo_id)
        raiz->esq = inserir_arquivo(raiz->esq, id);
    else if (id > raiz->arquivo_id)
        raiz->dir = inserir_arquivo(raiz->dir, id);
    else
        raiz->frequencia++;  // Se já existe, incrementa a frequência

    return raiz;
}

// Nova função: contar ocorrências da palavra em um arquivo específico
int contar_ocorrencias_arquivo(ArvArquivo* raiz, int arquivo_id) {
    if (raiz == NULL) return 0;
    
    if (arquivo_id < raiz->arquivo_id)
        return contar_ocorrencias_arquivo(raiz->esq, arquivo_id);
    else if (arquivo_id > raiz->arquivo_id)
        return contar_ocorrencias_arquivo(raiz->dir, arquivo_id);
    else
        return raiz->frequencia;  // Retorna a frequência do arquivo
}

// Funções para a árvore de palavras
ArvPalavra* criar_no_palavra(char* palavra, int arquivo_id) {
    ArvPalavra* novo = (ArvPalavra*)malloc(sizeof(ArvPalavra));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar no de palavra.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->palavra, palavra, MAX_WORD - 1);
    novo->palavra[MAX_WORD - 1] = '\0';
    novo->arquivos = criar_no_arquivo(arquivo_id);
    novo->esq = novo->dir = NULL;
    return novo;
}

ArvPalavra* inserir_palavra(ArvPalavra* raiz, char* palavra, int arquivo_id) {
    if (raiz == NULL) return criar_no_palavra(palavra, arquivo_id);

    int cmp = strcmp(palavra, raiz->palavra);

    if (cmp < 0)
        raiz->esq = inserir_palavra(raiz->esq, palavra, arquivo_id);
    else if (cmp > 0)
        raiz->dir = inserir_palavra(raiz->dir, palavra, arquivo_id);
    else
        raiz->arquivos = inserir_arquivo(raiz->arquivos, arquivo_id);

    return raiz;
}

ArvPalavra* buscar_palavra(ArvPalavra* raiz, char* palavra) {
    if (raiz == NULL || strcmp(raiz->palavra, palavra) == 0)
        return raiz;

    if (strcmp(palavra, raiz->palavra) < 0)
        return buscar_palavra(raiz->esq, palavra);
    else
        return buscar_palavra(raiz->dir, palavra);
}

// Função para normalizar palavra (minúsculas, sem pontuação)
void normalizar_palavra(char* palavra) {
    int i = 0, j = 0;
    while (palavra[i]) {
        if (isalpha(palavra[i])) {
            palavra[j++] = tolower(palavra[i]);
        }
        i++;
    }
    palavra[j] = '\0';
}

// Função para processar um arquivo
void processar_arquivo(ArvPalavra** raiz, ArquivoInfo arquivo, int arquivo_id) {
    char palavra[MAX_WORD];

    rewind(arquivo.ponteiro);  // Garantir que estamos no início
    
    while (fscanf(arquivo.ponteiro, "%99s", palavra) == 1) {
        normalizar_palavra(palavra);
        if (strlen(palavra) > 0) {
            *raiz = inserir_palavra(*raiz, palavra, arquivo_id);
        }
    }
    
    if (ferror(arquivo.ponteiro)) {
        fprintf(stderr, "Erro ao ler arquivo: %s\n", arquivo.nome);
    }
}

// Nova função para contar palavras totais em um arquivo
int contar_palavras_arquivo(FILE* arquivo) {
    char palavra[MAX_WORD];
    int contador = 0;
    
    rewind(arquivo);
    while (fscanf(arquivo, "%99s", palavra) == 1) {
        contador++;
    }
    rewind(arquivo);  // Voltar ao início para futuras leituras
    
    return contador;
}

// Função para liberar memória
void liberar_arv_arquivo(ArvArquivo* raiz) {
    if (raiz != NULL) {
        liberar_arv_arquivo(raiz->esq);
        liberar_arv_arquivo(raiz->dir);
        free(raiz);
    }
}

void liberar_arv_palavra(ArvPalavra* raiz) {
    if (raiz != NULL) {
        liberar_arv_palavra(raiz->esq);
        liberar_arv_palavra(raiz->dir);
        liberar_arv_arquivo(raiz->arquivos);
        free(raiz);
    }
}

int main() {
    // Configurar codificação no Windows
    #ifdef _WIN32
    // Configurar o console para UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    
    // Configurar locale para português (sem acentos para evitar problemas)
    setlocale(LC_ALL, "C");
    
    ArvPalavra* raiz_palavras = NULL;
    ArquivoInfo arquivos[MAX_FILES];
    int num_arquivos = 0;
    char palavra_busca[MAX_WORD];
    int i;
    
    printf("=================================================\n");
    printf("      SISTEMA DE BUSCA DE PALAVRAS EM ARQUIVOS\n");
    printf("=================================================\n\n");
    
    printf("Arquivos fixos utilizados:\n");
    printf("1. arquivo1.txt\n");
    printf("2. arquivo2.txt\n\n");
    
    // PASSO 1: Abrir os arquivos fixos
    printf("--- FASE 1: ABERTURA DOS ARQUIVOS ---\n");
    
    // Definir os nomes dos arquivos fixos
    const char* nomes_arquivos[MAX_FILES] = {"arquivo1.txt", "arquivo2.txt"};
    
    for (i = 0; i < MAX_FILES; i++) {
        strcpy(arquivos[i].nome, nomes_arquivos[i]);
        arquivos[i].ponteiro = fopen(nomes_arquivos[i], "r");
        
        if (arquivos[i].ponteiro == NULL) {
            printf("ERRO: Nao foi possivel abrir o arquivo '%s'\n", nomes_arquivos[i]);
            printf("   Certifique-se de que o arquivo esta na mesma pasta do programa.\n");
            
            // Tentar criar o arquivo se não existir
            FILE* temp = fopen(nomes_arquivos[i], "w");
            if (temp != NULL) {
                // Conteúdo de exemplo com palavras repetidas para testar contagem
                if (i == 0) {  // arquivo1.txt
                    fprintf(temp, "gato cachorro peixe gato\n");
                    fprintf(temp, "gato cachorro cachorro\n");
                    fprintf(temp, "computador programa algoritmo\n");
                } else {  // arquivo2.txt
                    fprintf(temp, "peixe aquario agua peixe peixe\n");
                    fprintf(temp, "gato felino domestico gato\n");
                    fprintf(temp, "programa computador software\n");
                }
                fclose(temp);
                
                // Agora tenta abrir novamente
                arquivos[i].ponteiro = fopen(nomes_arquivos[i], "r");
                if (arquivos[i].ponteiro != NULL) {
                    printf("Arquivo '%s' criado automaticamente\n", nomes_arquivos[i]);
                    num_arquivos++;
                }
            }
        } else {
            printf("Arquivo '%s' aberto com sucesso\n", nomes_arquivos[i]);
            num_arquivos++;
        }
    }
    
    if (num_arquivos == 0) {
        printf("\nNenhum dos arquivos foi aberto. Encerrando programa.\n");
        return 1;
    }
    
    printf("\nTotal de arquivos abertos: %d\n", num_arquivos);
    
    // PASSO 2: Processar cada arquivo e construir a arvore de palavras
    printf("\n--- FASE 2: PROCESSAMENTO DOS ARQUIVOS ---\n");
    for (i = 0; i < num_arquivos; i++) {
        printf("Processando: %s\n", arquivos[i].nome);
        processar_arquivo(&raiz_palavras, arquivos[i], i);
    }
    printf("Processamento concluido!\n");
    
    // Mostrar estatisticas dos arquivos
    printf("\n--- ESTATISTICAS DOS ARQUIVOS ---\n");
    for (i = 0; i < num_arquivos; i++) {
        int total_palavras = contar_palavras_arquivo(arquivos[i].ponteiro);
        printf("%s: %d palavra%s no total\n", 
               arquivos[i].nome, 
               total_palavras,
               total_palavras == 1 ? "" : "s");
    }
    
    // PASSO 3: Pedir ao usuario para digitar palavras para buscar
    printf("\n--- FASE 3: BUSCA DE PALAVRAS ---\n");
    printf("Digite palavras para buscar (ou 'sair' para encerrar)\n");
    printf("=====================================================\n");
    
    // Limpar buffer antes de começar a ler palavras
   // int c;
    //while ((c = getchar()) != '\n' && c != EOF);
    
    while (1) {
        printf("\nDigite uma palavra para buscar: ");
        
        if (fgets(palavra_busca, sizeof(palavra_busca), stdin) == NULL) {
            break;
        }
        
        // Remover o newline
        palavra_busca[strcspn(palavra_busca, "\n")] = 0;
        
        // Verificar se o usuario quer sair
        if (strcmp(palavra_busca, "sair") == 0 || strcmp(palavra_busca, "SAIR") == 0) {
            printf("Encerrando busca...\n");
            break;
        }
        
        // Normalizar a palavra
        normalizar_palavra(palavra_busca);
        
        // Verificar se a palavra não ficou vazia
        if (strlen(palavra_busca) == 0) {
            printf("Palavra invalida. Digite apenas letras.\n");
            continue;
        }
        
        // Buscar a palavra na arvore
        ArvPalavra* resultado = buscar_palavra(raiz_palavras, palavra_busca);
        
        // Mostrar resultados
        if (resultado != NULL) {
            printf("\nA palavra '%s' foi encontrada:\n", palavra_busca);
            
            // Mostrar ocorrencias especificas para cada arquivo
            for (i = 0; i < num_arquivos; i++) {
                int ocorrencias = contar_ocorrencias_arquivo(resultado->arquivos, i);
                if (ocorrencias > 0) {
                    printf("  - Em %s: %d vez%s\n", 
                           arquivos[i].nome, 
                           ocorrencias,
                           ocorrencias == 1 ? "" : "es");
                } else {
                    printf("  - Em %s: NAO encontrada\n", arquivos[i].nome);
                }
            }
        } else {
            printf("\nA palavra '%s' NAO foi encontrada em nenhum arquivo.\n", palavra_busca);
        }
    }
    
    // PASSO 4: Liberar memoria e fechar arquivos
    printf("\n--- FASE 4: LIMPEZA E ENCERRAMENTO ---\n");
    for (i = 0; i < num_arquivos; i++) {
        if (arquivos[i].ponteiro != NULL) {
            fclose(arquivos[i].ponteiro);
            printf("Arquivo fechado: %s\n", arquivos[i].nome);
        }
    }
    
    liberar_arv_palavra(raiz_palavras);
    
    printf("\n=================================================\n");
    printf("Programa encerrado com sucesso!\n");
    printf("=================================================\n");
    
    // Manter a janela aberta no Windows
    #ifdef _WIN32
    printf("\nPressione Enter para sair...");
    getchar();
    #endif
    
    return 0;
}