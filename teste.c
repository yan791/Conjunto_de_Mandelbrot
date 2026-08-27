#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

int main(void) {

    printf("=== TESTE 1: imagem 4x4 ===\n");

    int width = 4;
    int height = 4;

    unsigned char *imagem = alloca_imagem(width, height);

    if (imagem == NULL) {
        printf("ERRO: memoria nao foi alocada.\n");
        return 1;
    }

    printf("Memoria alocada com sucesso.\n");

    for (int i = 0; i < width * height; i++) {
        imagem[i] = (unsigned char) i;
    }

    printf("Valores armazenados:\n");

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            printf("%d ", imagem[row * width + col]);
        }

        printf("\n");
    }

    free(imagem);


    printf("\n=== TESTE 2: largura zero ===\n");

    imagem = alloca_imagem(0, 10);

    if (imagem == NULL) {
        printf("OK: largura zero foi rejeitada.\n");
    } else {
        printf("ERRO: largura zero foi aceita.\n");
        free(imagem);
    }


    printf("\n=== TESTE 3: altura zero ===\n");

    imagem = alloca_imagem(10, 0);

    if (imagem == NULL) {
        printf("OK: altura zero foi rejeitada.\n");
    } else {
        printf("ERRO: altura zero foi aceita.\n");
        free(imagem);
    }


    printf("\n=== TESTE 4: largura negativa ===\n");

    imagem = alloca_imagem(-5, 10);

    if (imagem == NULL) {
        printf("OK: largura negativa foi rejeitada.\n");
    } else {
        printf("ERRO: largura negativa foi aceita.\n");
        free(imagem);
    }


    printf("\n=== TESTE 5: altura negativa ===\n");

    imagem = alloca_imagem(10, -5);

    if (imagem == NULL) {
        printf("OK: altura negativa foi rejeitada.\n");
    } else {
        printf("ERRO: altura negativa foi aceita.\n");
        free(imagem);
    }


    printf("\n=== TESTE 6: tamanho da imagem ===\n");

    width = 10;
    height = 20;

    imagem = alloca_imagem(width, height);

    if (imagem == NULL) {
        printf("ERRO: nao conseguiu alocar imagem 10x20.\n");
        return 1;
    }

    size_t total_esperado = (size_t) width * height;

    printf("Pixels esperados: %zu\n", total_esperado);

    for (size_t i = 0; i < total_esperado; i++) {
        imagem[i] = 255;
    }

    printf("Todos os %zu pixels foram escritos.\n", total_esperado);

    free(imagem);


    return 0;
}