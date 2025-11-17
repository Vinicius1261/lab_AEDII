#include "raylib.h"
#include <stdlib.h> // Para malloc, free, rand, srand
#include <time.h>   // Para time (seed do rand)

// --- Constantes do Jogo ---
#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define TAMANHO_BLOCO 20 // Tamanho de cada bloco (em pixels)

// --- Estrutura da Lista Encadeada ---

/**
 * @brief Define a estrutura de um "No" (segmento) da cobra.
 * Cada nó armazena sua posição (x, y) e um ponteiro para o próximo nó.
 */
typedef struct No {
    Vector2 posicao;
    struct No *proximo;
} No;

/**
 * @brief Define a estrutura da "Lista" (a cobra inteira).
 * Encapsula os ponteiros para o início (head) e o fim (tail) da lista.
 */
typedef struct Lista {
    No *inicio;
    No *fim;
} Lista;


// --- Variáveis Globais do Jogo ---
Lista cobra; // A cobra é a nossa lista encadeada
Vector2 posicao_comida;
int pontuacao = 0;
bool fim_de_jogo = false;
Vector2 direcao = {1, 0}; // A direção atual do movimento da cobra
int quadros_crescimento_restantes = 0; // Frames de crescimento restantes

// --- Funções da Lista Encadeada ---

/**
 * @brief Adiciona um novo segmento na cabeça da cobra (início da lista).
 * @param lista Um ponteiro para a Lista que será modificada.
 * @param pos A posição (Vector2) onde o novo nó será criado.
 */
void insert_head(Lista *lista, Vector2 pos) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        fim_de_jogo = true;
        return;
    }

    novo->posicao = pos;
    novo->proximo = lista->inicio;
    lista->inicio = novo;

    if (lista->fim == NULL) {
        lista->fim = novo;
    }
}

/**
 * @brief Remove o último segmento da cobra (o fim da lista).
 * @param lista Um ponteiro para a Lista que será modificada.
 */
void remove_tail(Lista *lista) {
    // TODO: IMPLEMENTE ESTA FUNÇÃO
    //
    // 1. Verificar se a lista está vazia (lista->inicio == NULL). Se estiver, apenas retorne.
    //
    // 2. Verificar o caso especial: Se houver APENAS UM elemento na lista
    //    (lista->inicio == lista->fim).
    //    - Liberar a memória desse único nó (use free).
    //    - Definir lista->inicio e lista->fim como NULL.
    //    - Retornar.
    //
    // 3. Se houver mais de um elemento (caso geral):
    //    - Criar um ponteiro 'no_atual' e inicializá-lo com 'lista->inicio'.
    //    - Percorrer a lista com 'no_atual' ATÉ encontrar o PENÚLTIMO nó.
    //      (Dica: O penúltimo nó é aquele onde 'no_atual->proximo == lista->fim').
    //
    // 4. Quando encontrar o penúltimo nó ('no_atual'):
    //    - Liberar a memória do último nó (o 'fim' atual da lista).
    //    - Atualizar o 'fim' da lista para que ele aponte para 'no_atual' (o penúltimo).
    //    - Definir o ponteiro 'proximo' do NOVO fim como NULL.
}

/**
 * @brief Posiciona a comida em um local aleatório da tela.
 * Garante que a comida não apareça em cima da cobra.
 */
void place_food() {
    while(1) {
        posicao_comida.x = (rand() % (LARGURA_TELA / TAMANHO_BLOCO));
        posicao_comida.y = (rand() % (ALTURA_TELA / TAMANHO_BLOCO));

        bool sobre_a_cobra = false;
        No *no_atual = cobra.inicio; // Percorre a lista 'cobra'
        while (no_atual != NULL) {
            if (CheckCollisionRecs(
                (Rectangle){posicao_comida.x * TAMANHO_BLOCO, posicao_comida.y * TAMANHO_BLOCO, TAMANHO_BLOCO, TAMANHO_BLOCO},
                (Rectangle){no_atual->posicao.x * TAMANHO_BLOCO, no_atual->posicao.y * TAMANHO_BLOCO, TAMANHO_BLOCO, TAMANHO_BLOCO}
            )) {
                sobre_a_cobra = true;
                break;
            }
            no_atual = no_atual->proximo;
        }

        if (!sobre_a_cobra) {
            break;
        }
    }
}

// --- Funções Principais do Jogo ---

/**
 * @brief Inicializa as variáveis do jogo, a cobra inicial e a primeira comida.
 */
void InitGame(void) {
    srand(time(NULL));

    // Inicializa a lista como vazia
    cobra.inicio = NULL;
    cobra.fim = NULL;

    int x_inicial = (LARGURA_TELA / TAMANHO_BLOCO) / 2;
    int y_inicial = (ALTURA_TELA / TAMANHO_BLOCO) / 2;
    
    // Passa o endereço da 'cobra' para as funções da lista
    insert_head(&cobra, (Vector2){x_inicial - 2, y_inicial});
    insert_head(&cobra, (Vector2){x_inicial - 1, y_inicial});
    insert_head(&cobra, (Vector2){x_inicial, y_inicial});

    place_food();

    SetTargetFPS(10); 
}

/**
 * @brief Processa a entrada do jogador (teclas direcionais).
 * Impede que a cobra inverta sua direção (ex: ir de cima para baixo).
 */
void HandleInput(void) {
    if (IsKeyPressed(KEY_UP) && direcao.y == 0) {
        direcao = (Vector2){0, -1};
    }
    if (IsKeyPressed(KEY_DOWN) && direcao.y == 0) {
        direcao = (Vector2){0, 1};
    }
    if (IsKeyPressed(KEY_LEFT) && direcao.x == 0) {
        direcao = (Vector2){-1, 0};
    }
    if (IsKeyPressed(KEY_RIGHT) && direcao.x == 0) {
        direcao = (Vector2){1, 0};
    }
}

/**
 * @brief Atualiza a lógica do jogo a cada frame (movimento, colisão, comida).
 * Esta é a função principal que gerencia o estado do jogo.
 */
void UpdateGame(void) {
    if (fim_de_jogo) return;

    Vector2 nova_posicao_cabeca = {
        cobra.inicio->posicao.x + direcao.x, // Usa a 'direcao' principal
        cobra.inicio->posicao.y + direcao.y  // Usa a 'direcao' principal
    };

    if (nova_posicao_cabeca.x < 0 || nova_posicao_cabeca.x >= (LARGURA_TELA / TAMANHO_BLOCO) ||
        nova_posicao_cabeca.y < 0 || nova_posicao_cabeca.y >= (ALTURA_TELA / TAMANHO_BLOCO)) {
        fim_de_jogo = true;
        return;
    }

    No *no_atual = cobra.inicio->proximo; // Começa do SEGUNDO nó (depois da cabeça)
    //Loop para atualizar o corpo
    while (no_atual != NULL) {
        // Verifica se a nova posição da cabeça colide com algum segmento do corpo
        if (nova_posicao_cabeca.x == no_atual->posicao.x &&
            nova_posicao_cabeca.y == no_atual->posicao.y) {
            fim_de_jogo = true;
            return;
        }

        no_atual = no_atual->proximo;
    }

    insert_head(&cobra, nova_posicao_cabeca);

    if (nova_posicao_cabeca.x == posicao_comida.x && nova_posicao_cabeca.y == posicao_comida.y) {
        pontuacao++;
        quadros_crescimento_restantes = 2; 
        place_food();
    }

    if (quadros_crescimento_restantes > 0) {
        quadros_crescimento_restantes--;
    } else {
        remove_tail(&cobra);
    }
}

/**
 * @brief Desenha todos os elementos do jogo na tela.
 * Chamada uma vez por frame, dentro do loop principal.
 */
void DrawGame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE); 

    if (fim_de_jogo) {
        DrawText("FIM DE JOGO", 
                 LARGURA_TELA / 2 - MeasureText("FIM DE JOGO", 40) / 2, 
                 ALTURA_TELA / 2 - 20, 40, RED);
    } else {
        No *no_atual = cobra.inicio;
        while (no_atual != NULL) {
            Color cor = (no_atual == cobra.inicio) ? DARKGREEN : GREEN;
            DrawRectangle(no_atual->posicao.x * TAMANHO_BLOCO, 
                          no_atual->posicao.y * TAMANHO_BLOCO, 
                          TAMANHO_BLOCO, TAMANHO_BLOCO, cor);
            no_atual = no_atual->proximo;
        }

        DrawRectangle(posicao_comida.x * TAMANHO_BLOCO, posicao_comida.y * TAMANHO_BLOCO, 
                      TAMANHO_BLOCO, TAMANHO_BLOCO, RED);
    }

    DrawText(TextFormat("Pontos: %d", pontuacao), 10, 10, 20, DARKGRAY);

    EndDrawing();
}

/**
 * @brief Função principal - Ponto de entrada do jogo.
 * Configura a janela e executa o loop principal do jogo.
 */
int main(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Jogo Snake com raylib e Listas Encadeadas");
    InitGame();

    while (!WindowShouldClose()) {
        HandleInput();
        UpdateGame();
        DrawGame();
    }

    // Libera toda a memória da lista encadeada
    No *no_atual = cobra.inicio;
    while (no_atual != NULL) {
        // TODO: IMPLEMENTE A LIMPEZA DA MEMÓRIA
    }

    CloseWindow();
    return 0;
}

