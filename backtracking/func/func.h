#include <stdlib.h>
#include <stdio.h>
#define linha 10
#define coluna 10

int resolverLabirinto(int labirinto[linha][coluna], int x, int y){
    if(x < 0 || x >= linha || y < 0 || y >= coluna || labirinto[x][y] == 1 || labirinto[x][y] == 2){
        return 0; // Fora dos limites, parede ou já visitado
    }

    if(x == linha - 1 && y == coluna - 1){
        labirinto[x][y] = 2; // Marca o destino
        printf("Caminho encontrado!\n");
        return 1;
    }

    labirinto[x][y] = 2; // Marca como parte do caminho

    if(resolverLabirinto(labirinto, x + 1, y) || // baixo
       resolverLabirinto(labirinto, x - 1, y) || // cima
       resolverLabirinto(labirinto, x, y + 1) || // direita
       resolverLabirinto(labirinto, x, y - 1)) { // esquerda
        return 1;
    }

    labirinto[x][y] = 0; // Backtrack
    return 0;
}