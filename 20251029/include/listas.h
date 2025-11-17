#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char nome[30];
    char autor[30];
    int ano;
} Filme;

typedef struct No {
    Filme dado;
    struct No *prox;
} No;

// 2. Definição da Estrutura da Lista Ligada
typedef struct {
    No *inicio; // Ponteiro para o primeiro nó da lista (HEAD)
    int tamanho; // Tamanho da lista (opcional)
} Lista;

void cria_lista(Lista *lista);

int lista_vazia(Lista lista);

int inserir_no_inicio(Lista *lista, Filme f);

int remove_no_inicio(Lista *lista, Filme *f);

int inserir_no_indice(Lista *lista, Filme f, int i);

int inserir_ordenado_ano(Lista *lista, Filme f);

void cria_lista(Lista *lista){
    lista->inicio = NULL;
    lista->tamanho = 0;
}

int lista_vazia(Lista lista){
    if(lista.inicio == NULL)
        return 1;
    return 0;
}

int inserir_no_inicio(Lista *lista, Filme f){
    No *novo = (No*) malloc(sizeof(No));
    if(novo == NULL)
        return 0; // Falha na alocação de memória

    novo->dado = f; // Atribui o filme ao novo nó
    novo->prox = lista->inicio; // Novo nó aponta para o antigo primeiro nó
    lista->inicio = novo; // Atualiza o início da lista para o novo nó
    lista->tamanho++; // Incrementa o tamanho da lista
    return 1; // Sucesso
}

int remove_no_inicio(Lista *lista, Filme *f){
    if(lista_vazia(*lista))
        return 0; // Lista vazia, nada a remover

    No *temp = lista->inicio; // Armazena o nó a ser removido
    *f = temp->dado; // Copia o dado do nó removido para o filme fornecido
    lista->inicio = lista->inicio->prox; // Atualiza o início da lista
    free(temp); // Libera a memória do nó removido
    lista->tamanho--; // Decrementa o tamanho da lista
    return 1; // Sucesso
}

int inserir_no_indice(Lista *lista, Filme f, int i){
    //IMPLEMENTAR
    No *novo = (No*) malloc(sizeof(No));
    if(novo == NULL)
        return 0;
    
    novo->dado = f;
    novo->prox = NULL;
    
    if(i==0){
        novo->prox = lista->inicio;
        lista->inicio = novo;
        return 1;
    }
    No *aux = lista->inicio;
    int pos = 0;
    while (aux->prox != NULL && pos < i - 1){
        aux = aux->prox;
        pos++;
    }
    novo->prox = aux-> prox;
    aux->prox = novo;
    return 1;
}

int inserir_ordenado_ano(Lista *lista, Filme f){
    //IMPLEMENTAR
    No *novo = (No*) malloc(sizeof(No));
    if(novo == NULL)
        return 0;

    novo->dado = f;
    novo->prox = NULL;

    if(lista->inicio == NULL || f.ano < lista->inicio->dado.ano){
        novo->prox = lista->inicio;
        lista->inicio=novo;
        return 1;
    }

    No *aux = lista->inicio;
    while (aux->prox != NULL && aux->prox->dado.ano < f.ano){
        aux = aux->prox;
    }
    novo->prox = aux-> prox;
    aux->prox = novo;
    return 1;
    
    
}