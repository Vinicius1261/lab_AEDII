#include <stdio.h>
#include <stdlib.h>
#include "../func/func.h"


int main(){
    int x = 0, y = 0;
    int labirinto1[10][10] = {
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}
};

    if (labirinto1[x][y] == 1) {
        printf("Ponto de partida bloqueado.\n");
        return 0;
    }

    //Tentando resolver o labirinto
    if (resolverLabirinto(labirinto1, x, y)) {
        printf("Labirinto resolvido:\n");
    } else {
        printf("Não há caminho até a saída.\n");
    }

    //imprime o labirinto no terminal
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            if (labirinto1[i][j] == 0) {
                printf(". ");
            } else if (labirinto1[i][j] == 1) {
                printf("# ");
            } else if (labirinto1[i][j] == 2) {
                printf("* ");
            }
        }
        printf("\n");
    }

    return 0;
}
