/*

 * PROJETO: Sistema de Navegação GPS com Dijkstra e Heap

 * DISCIPLINA: Algoritmos e Estruturas de Dados II

 * ALUNO: _________________________________________________

 */



#include <stdio.h>

#include <stdlib.h>

#include <limits.h>

#include <time.h>



// --- CONSTANTES E DEFINIÇÕES ---

#define MAX_VERTICES 100 // Tamanho da nossa cidade (Grid 10x10)

#define INFINITO INT_MAX



// Estrutura para um nó na lista de adjacência (Aresta)

typedef struct Node {

    int destino;

    int peso;

    struct Node* prox;

} Node;



// Estrutura para o Grafo

typedef struct Graph {

    int numVertices;

    Node* listasAdj[MAX_VERTICES];

} Graph;



// Estrutura para elementos da Min-Heap

typedef struct HeapNode {

    int v;      // Índice do vértice

    int dist;   // Distância acumulada (prioridade)

} HeapNode;



// Estrutura da Min-Heap

typedef struct MinHeap {

    int tamanho;

    int capacidade;

    int *pos;     // Mapa para rastrear posição do vértice no array da heap (Opcional, mas recomendado para decreaseKey)

    HeapNode **array;

} MinHeap;



// --- PROTÓTIPOS (NÃO ALTERAR A MAIN) ---

Graph* criarGrafo(int V);

void adicionarAresta(Graph* graph, int src, int dest, int peso);

void imprimirCaminho(int parent[], int j);

void dijkstra(Graph* graph, int src, int dest);



// Protótipos das funções que VOCÊS devem implementar:

MinHeap* criarMinHeap(int capacidade);  //feito

void minHeapify(MinHeap* minHeap, int idx); // feito

HeapNode* extractMin(MinHeap* minHeap); //feito

void decreaseKey(MinHeap* minHeap, int v, int dist); //feito

int isInMinHeap(MinHeap* minHeap, int v); //se eu nao me engano feito



// ---------------------------------------------------------------------------

// ESPAÇO PARA IMPLEMENTAÇÃO DOS ALUNOS

// ---------------------------------------------------------------------------



// 1. Cria e inicializa a Heap vazia

MinHeap* criarMinHeap(int capacidade) {

    // TODO: Alocar memória para a struct MinHeap, para o array de nós e para o vetor de posições
    MinHeap *heap = malloc (sizeof(MinHeap)); // Aloca memória para a estrutura principal do MinHeap
     if(heap == NULL ) //verifica se a heap foi alocada corretamente
        return NULL; // retorna null caso tenha sido alocada incorretamente
    heap->array = malloc(capacidade * sizeof(HeapNode*)); // Aloca memória para o array de nós do heap com a capacidade especificada
    if(heap->array == NULL){ //verifica se a alocação foi bem sucedida ou nao
        free(heap); // libera a memoria alocada anteriormente pois ja foi alocada
        return NULL; // retorna null caso o array de nos da heap tenha diso alocada incorretamente
    }
    heap->pos = malloc(capacidade * sizeof(int)); // Aloca memória para o vetor de posições (mapeia índices para posições no heap)
    if(heap->pos == NULL){ // Verifica se a alocação do vetor de posições foi bem-sucedida
        free(heap->array); // libera a memoria alocada anteriormente do array de nos da heap
        free(heap); // libera a memoria alocada na estrutura principal
        return NULL; // caso o vetor de posições tenha sido alocado incorretamente retora null
    }

    heap->tamanho = 0; // inicia o tamanho atual da heap em 0
    heap->capacidade = capacidade; // armazena a capacidade maxima da heap  

    // Inicialização do vetor de posições (índices) para controle do heap
    for(int i = 0; i < capacidade; i++){
         heap->pos[i] = -1;  // -1 indica que o elemento correspondente não está atualmente no heap
    }

    // Retornar ponteiro

    return heap; 

}



// 2. Garante a propriedade de Heap Mínimo no índice idx (Desce o elemento)

void minHeapify(MinHeap* minHeap, int idx) {
    // TODO: Implementar lógica de heapify (comparar pai com filhos esquerdo e direito)
    int menor = idx; // Rastrear qual índice tem o menor valor (pai, filho esquerdo ou direito)
    int esq = 2*idx +1; // Calcula o índice do filho esquerdo na representação de array
    int dir = 2*idx +2; // Calcula o índice do filho direito

    // Comparar com filho esquerdo
    if(esq < minHeap->tamanho && minHeap->array[esq]->dist < minHeap->array[menor]->dist){
        menor = esq;
    }
    // Comparar com filho direito
    if(dir < minHeap->tamanho && minHeap->array[dir]->dist < minHeap->array[menor]->dist){
        menor = dir;
    }
    // Se o menor não é o pai, trocar
    if(menor != idx){
        // Salvar temporariamente o nó atual
        HeapNode* temp = minHeap->array[idx];                    

         // Troca o nó atual (pai) com o nó menor encontrado (filho)
        minHeap->array[idx] = minHeap->array[menor]; // Filho sobe para a posição do pai        
        minHeap->array[menor] = temp;  // Pai desce para a posição do filho   

        // Atualiza o vetor de posições após a troca:
        minHeap->pos[minHeap->array[idx]->v] = idx;  // Atualiza a posição do vértice que foi para idx
        minHeap->pos[minHeap->array[menor]->v] = menor;  // Atualiza a posição do vértice que foi para menor

        minHeapify(minHeap, menor); // Chamar recursivamente na nova posição
    }

}



// 3. Extrai o nó com menor distância da Heap

HeapNode* extractMin(MinHeap* minHeap) {
    // TODO: Salvar o nó raiz, substituir pelo último, reduzir tamanho e chamar heapify.
    // Verifica se o heap é válido e não está vazio
    if(minHeap == NULL || minHeap->tamanho == 0){
        return NULL;
    }
    // Salva a raiz (elemento mínimo)
    HeapNode* raiz = minHeap->array[0];
    // Substitui a raiz pelo último elemento do heap
    minHeap->array[0] = minHeap->array[minHeap->tamanho -1];
    // Atualiza a posição do elemento movido para a raiz
    minHeap->pos[minHeap->array[0]->v] = 0;
    // Marca o vértice extraído como ausente do heap
    minHeap->pos[raiz->v] = -1;
    // Reduz o tamanho do heap
    minHeap->tamanho = minHeap->tamanho - 1;
    // Se ainda houver elementos, reorganiza o heap a partir da raiz
    if(minHeap->tamanho > 0 ){
        minHeapify(minHeap, 0);
    }
    // Retorna o nó extraído (mínimo)
    return raiz;
}



// 4. Atualiza a distância de um vértice existente (Sobe o elemento)

void decreaseKey(MinHeap* minHeap, int v, int dist) {
    // Verifica se o ponteiro do heap é válido
    if(minHeap == NULL){
        return;  // Se for NULL, encerra a função sem fazer nada
    }
    
    // Obtém a posição do vértice v no heap usando o vetor de posições
    int i = minHeap->pos[v];

    // Verifica se o vértice v está presente no heap (posição -1 indica ausência)
    if( i  == -1){
        return;  // Se não está, encerra a função
    }

    // Verifica se a nova distância fornecida é realmente menor que a distância atual
    if(dist >= minHeap->array[i]->dist){
        return;  // Se não for menor, não há necessidade de atualizar, encerra
    }
    
    // Atualiza a distância do vértice v no heap com o novo valor menor
    minHeap->array[i]->dist = dist;

    // Enquanto o vértice não for a raiz e sua distância for menor que a do seu pai
    while (i>0 && minHeap->array[i]->dist < minHeap->array[(i-1)/2]->dist){
        // Calcula o índice do pai do nó atual
        int pai = (i-1)/2;

        // Troca o nó atual com seu pai usando um ponteiro temporário
        HeapNode* temp = minHeap->array[i];
        minHeap->array[i] = minHeap->array[pai];
        minHeap->array[pai] = temp;

        // Atualiza o vetor de posições para refletir as novas posições dos vértices trocados
        minHeap->pos[minHeap->array[i]->v] = i;
        minHeap->pos[minHeap->array[pai]->v] = pai;

        // Move o índice para a posição do pai, para continuar verificando se precisa subir mais
        i = pai;
    }

     // Dica: Use o vetor 'pos' para achar onde 'v' está no array da heap

}



// 5. Verifica se o vértice v está na heap

int isInMinHeap(MinHeap* minHeap, int v) {
    // TODO: Retornar 1 se v estiver na heap, 0 caso contrário      
    
    // Verifica se o heap é válido e v está dentro dos limites
    if (minHeap == NULL || v < 0 || v >= minHeap->capacidade) {
        return 0;
    }
    
    // Verifica se o vértice está no heap
    if (minHeap->pos[v] != -1) {
        return 1;  // Está no heap
    } else {
        return 0;  // Não está no heap
    }
}


// 6. O Algoritmo Principal

void dijkstra(Graph* graph, int src, int dest) {

    printf("\n--- Calculando rota de %d para %d ---\n", src, dest);
    int V = graph->numVertices;
    int dist[V];      // Array para armazenar a menor distância de src a i
    int parent[V];    // Array para rastrear o caminho mais curto

    
    for (int i = 0; i < V; ++i) {
        dist[i] = INFINITO;
        parent[i] = -1; // -1 indica que não há predecessor
    }

    // Heap
    MinHeap* minHeap = criarMinHeap(V);
    dist[src] = 0;

    for (int v = 0; v < V; ++v) {
        // Criamos um HeapNode para cada vértice
        HeapNode* newNode = (HeapNode*)malloc(sizeof(HeapNode));
        newNode->v = v;
        // Se for a origem, a distância é 0, caso contrário, INFINITO
        newNode->dist = (v == src) ? 0 : INFINITO; 
        
        minHeap->array[v] = newNode;
        minHeap->pos[v] = v; // Inicialmente, a posição é igual ao índice do vértice
    }
    minHeap->tamanho = V; 

    //criar a heap
    for (int i = (minHeap->tamanho - 1) / 2; i >= 0; --i) {
        minHeapify(minHeap, i);
    }
    
    
    while (minHeap->tamanho > 0) {
        HeapNode* heapNode = extractMin(minHeap);
        int u = heapNode->v;

        // Se o destino for encontrado, podemos parar (otimização)
        if (u == dest) {
            free(heapNode);
            break; 
        }

        // b) Para cada vizinho v de u: (Iterar pela lista de adjacência)
        Node* crawl = graph->listasAdj[u];
        while (crawl != NULL) {
            int v = crawl->destino;
            int peso = crawl->peso;

            // Se v ainda está na heap e um caminho mais curto é encontrado
            if (isInMinHeap(minHeap, v) && dist[u] != INFINITO && dist[u] + peso < dist[v]) {
                
                // Relaxamento:
                // Atualize dist[v]
                dist[v] = dist[u] + peso;
                
                // Atualize vetor de pais
                parent[v] = u;
                
                // decreaseKey(heap, v, nova_dist)
                decreaseKey(minHeap, v, dist[v]);
            }
            crawl = crawl->prox;
        }
        free(heapNode); // Libera o nó extraído do heap
    }
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);


    
    if (dist[dest] == INFINITO) {
        printf("\n=> Nao ha caminho possivel de %d para %d.\n", src, dest);
    } else {
        printf("\n=> Caminho mais curto encontrado:\n");
        printf("Custo Total (Tempo): %d\n", dist[dest]);
        printf("Rota: %d", src);
        imprimirCaminho(parent, dest);
        printf("\n");
    }

}
    



// ---------------------------------------------------------------------------

// FUNÇÕES AUXILIARES JÁ IMPLEMENTADAS (NÃO PRECISA ALTERAR)

// ---------------------------------------------------------------------------



Node* novoNode(int dest, int peso) {

    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->destino = dest;

    newNode->peso = peso;

    newNode->prox = NULL;

    return newNode;

}



Graph* criarGrafo(int V) {

    Graph* graph = (Graph*)malloc(sizeof(Graph));

    graph->numVertices = V;

    for (int i = 0; i < V; ++i)

        graph->listasAdj[i] = NULL;

    return graph;

}



void adicionarAresta(Graph* graph, int src, int dest, int peso) {
// Lembrete: A função auxiliar 'novoNode' já está definida no seu código base.
    // Cria um novo nó de aresta
    Node* newNodeSrc = novoNode(dest, peso); 
    
    // Adiciona o novo nó no início da lista de adjacência de src
    newNodeSrc->prox = graph->listasAdj[src];
    graph->listasAdj[src] = newNodeSrc;

    // Cria outro novo nó de aresta com o mesmo peso
    Node* newNodeDest = novoNode(src, peso);
    
    // Adiciona o novo nó no início da lista de adjacência de dest
    newNodeDest->prox = graph->listasAdj[dest];
    graph->listasAdj[dest] = newNodeDest;

}



void imprimirCaminho(int parent[], int j) {

    if (parent[j] == -1)

        return;

    imprimirCaminho(parent, parent[j]);

    printf(" -> %d", j);

}



// ---------------------------------------------------------------------------

// MAIN (CENÁRIO DE TESTE - NÃO ALTERAR A ESTRUTURA GERAL)

// ---------------------------------------------------------------------------

int main() {

    srand(time(NULL));

    int V = 100;

    Graph* cidade = criarGrafo(V);

    printf("=== SISTEMA DE GPS DE GRAPH CITY ===\n");

    printf("Mapa carregado: 100 cruzamentos (Grade 10x10)\n");

    for (int i = 0; i < 10; i++) { // i: Linha (0 a 9)
        for (int j = 0; j < 10; j++) { // j: Coluna (0 a 9)
            
            int u = i * 10 + j; // Vértice atual (Origem)
            int peso_temp;

            // CONEXÃO COM O VIZINHO DA DIREITA (Movimento Horizontal)
            if (j < 9) {
                int v_direita = i * 10 + (j + 1);

                // Regra de peso para ruas horizontais
                if (i == 2 || i == 7) {
                    peso_temp = 1; // Avenida Rápida (Linha 2 ou 7)
                } else {
                    // Peso Padrão: 1 a 10
                    peso_temp = (rand() % 10) + 1; 
                }
                // Adiciona aresta não-direcionada u <-> v_direita
                adicionarAresta(cidade, u, v_direita, peso_temp);
            }

            // CONEXÃO COM O VIZINHO DE BAIXO (Movimento Vertical)
            if (i < 9) {
                int v_baixo = (i + 1) * 10 + j;

                // Regra de peso para ruas verticais
                if (j == 4 || j == 5) {
                    peso_temp = 20; // Zona de Congestionamento (Coluna 4 ou 5)
                } else {
                    // Peso Padrão: 1 a 10
                    peso_temp = (rand() % 10) + 1;
                }
                // Adiciona aresta não-direcionada u <-> v_baixo
                adicionarAresta(cidade, u, v_baixo, peso_temp);
            }
        }
    }

    dijkstra(cidade, 0, 99); 



    // Teste 2: Rota curta

    dijkstra(cidade, 20, 72);



    return 0;

} 