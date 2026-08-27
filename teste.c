#include <stdio.h>
#include "mandelbrot.h"

int main(void) {

    printf("=== TESTE 1: salvar imagem normalmente ===\n");

    int width = 2;
    int height = 3;

    unsigned char imagem_teste[] = {
        10, 20,
        30, 40,
        50, 60
    };

    int resultado = pgm_salva(
        "teste.pgm",
        imagem_teste,
        width,
        height
    );

    if (resultado == 0) {
        printf("Arquivo salvo com sucesso.\n");
    } else {
        printf("ERRO: nao foi possivel salvar o arquivo.\n");
    }


    printf("\n=== TESTE 2: verificar arquivo gerado ===\n");

    FILE *fp = fopen("teste.pgm", "r");

    if (fp == NULL) {
        printf("ERRO: o arquivo teste.pgm nao existe.\n");
    } else {
        char linha[100];

        while (fgets(linha, sizeof(linha), fp) != NULL) {
            printf("%s", linha);
        }

        fclose(fp);
    }


    printf("\n=== TESTE 3: filename NULL ===\n");

    resultado = pgm_salva(
        NULL,
        imagem_teste,
        width,
        height
    );

    printf("Resultado: %d\n", resultado);


    printf("\n=== TESTE 4: image NULL ===\n");

    resultado = pgm_salva(
        "erro.pgm",
        NULL,
        width,
        height
    );

    printf("Resultado: %d\n", resultado);


    printf("\n=== TESTE 5: width zero ===\n");

    resultado = pgm_salva(
        "erro.pgm",
        imagem_teste,
        0,
        height
    );

    printf("Resultado: %d\n", resultado);


    printf("\n=== TESTE 6: height zero ===\n");

    resultado = pgm_salva(
        "erro.pgm",
        imagem_teste,
        width,
        0
    );

    printf("Resultado: %d\n", resultado);


    printf("\n=== TESTE 7: width negativo ===\n");

    resultado = pgm_salva(
        "erro.pgm",
        imagem_teste,
        -2,
        height
    );

    printf("Resultado: %d\n", resultado);


    printf("\n=== TESTE 8: height negativo ===\n");

    resultado = pgm_salva(
        "erro.pgm",
        imagem_teste,
        width,
        -3
    );

    printf("Resultado: %d\n", resultado);


    return 0;
}

