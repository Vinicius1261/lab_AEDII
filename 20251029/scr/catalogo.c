#include "listas.h"

int main(){
    Lista l;

    cria_lista(&l);
    Filme filmes[5] = {{"Inception", "Christopher Nolan", 2010},
    {"The Matrix", "The Wachowskis", 1999},
    {"Interstellar", "Christopher Nolan", 2014},
    {"The Godfather", "Francis Ford Coppola", 1972},
    {"Pulp Fiction", "Quentin Tarantino", 1994}};

    for(int i = 0; i < 5; i++){
        inserir_no_comeco(&l, filmes[i]);
    }

    printf("\n---Livros cadastrados de maneira desordenada!!---\n");

    for(int i = 0; i < 5; i++){
        Filme f;
        remove_no_comeco(&l, &f);
        printf("Ano: %d, Nome: %s, Autor: %s\n", f.ano, f.nome, f.autor);
    }

    for(int i = 0; i < 5; i++){
        inserir_ordenado_ano(&l, filmes[i]);
    }

    printf("\n---Livros cadastrados ordenados por ano!!---\n");

    for(int i = 0; i < 5; i++){
        Filme f;
        remove_no_comeco(&l, &f);
        printf("Ano: %d, Nome: %s, Autor: %s\n", f.ano, f.nome, f.autor);
    }

    //IMPLEMENTAR CÓDIGO PARA INSERIR ORDENADO PELA PREFERENCIA DO PROGRAMADOR
    printf("1. %s | Diretor: %s | Ano: %d", filmes[2].nome, filmes[2].autor, filmes[2].ano);
    printf("2. %s | Diretor: %s | Ano: %d", filmes[3].nome, filmes[3].autor, filmes[3].ano);
    printf("3. %s | Diretor: %s | Ano: %d", filmes[4].nome, filmes[4].autor, filmes[4].ano);
    printf("4. %s | Diretor: %s | Ano: %d", filmes[1].nome, filmes[1].autor, filmes[1].ano);
    printf("5. %s | Diretor: %s | Ano: %d", filmes[0].nome, filmes[0].autor, filmes[0].ano);

    //obs: Eu só assisti interstellar

    printf("\n---Livros cadastrados ordenados por preferencia!!---\n");

    for(int i = 0; i < 5; i++){
        Filme f;
        remove_no_comeco(&l, &f);
        printf("Ano: %d, Nome: %s, Autor: %s\n", f.ano, f.nome, f.autor);
    }

    return 0;


}