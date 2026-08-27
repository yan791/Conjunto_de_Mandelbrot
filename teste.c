#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

int main(void) {
    int max_iter = 100;

    printf("=== Teste mandelbrot_ponto ===\n");

    printf("c = (0, 0): %d\n",
           mandelbrot_ponto(0.0, 0.0, max_iter));

    printf("c = (-1, 0): %d\n",
           mandelbrot_ponto(-1.0, 0.0, max_iter));

    printf("c = (1, 0): %d\n",
           mandelbrot_ponto(1.0, 0.0, max_iter));

    printf("c = (2, 0): %d\n",
           mandelbrot_ponto(2.0, 0.0, max_iter));

    printf("\n=== Teste col_to_real ===\n");

    printf("col 0, width 4: %.2f\n",
           col_to_real(0, 4));

    printf("col 1, width 4: %.2f\n",
           col_to_real(1, 4));

    printf("col 2, width 4: %.2f\n",
           col_to_real(2, 4));

    printf("col 3, width 4: %.2f\n",
           col_to_real(3, 4));

    printf("col 0, width 1: %.2f\n",
           col_to_real(0, 1));

    printf("\n=== Teste row_to_imag ===\n");

    printf("row 0, height 4: %.2f\n",
           row_to_imag(0, 4));

    printf("row 1, height 4: %.2f\n",
           row_to_imag(1, 4));

    printf("row 2, height 4: %.2f\n",
           row_to_imag(2, 4));

    printf("row 3, height 4: %.2f\n",
           row_to_imag(3, 4));

    printf("row 0, height 1: %.2f\n",
           row_to_imag(0, 1));

    printf("\n=== Teste iterations_to_intensity ===\n");

    printf("0 / 100: %d\n",
           iterations_to_intensity(0, 100));

    printf("25 / 100: %d\n",
           iterations_to_intensity(25, 100));

    printf("50 / 100: %d\n",
           iterations_to_intensity(50, 100));

    printf("75 / 100: %d\n",
           iterations_to_intensity(75, 100));

    printf("100 / 100: %d\n",
           iterations_to_intensity(100, 100));

    printf("max_iter = 0: %d\n",
           iterations_to_intensity(50, 0));

    printf("150 / 100: %d\n",
           iterations_to_intensity(150, 100));

    printf("-10 / 100: %d\n",
           iterations_to_intensity(-10, 100));


    printf("\n=== Teste compute_serial ===\n");

    config cfg;

    cfg.largura = 4;
    cfg.altura = 4;
    cfg.max_iter = 100;
    cfg.threads = 1;

    size_t total_pixels = (size_t) cfg.largura * cfg.altura;

    unsigned char *image = malloc(total_pixels * sizeof(unsigned char));

    if (image == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    compute_serial(image, &cfg);

    printf("Imagem 4x4:\n");

    for (int row = 0; row < cfg.altura; row++) {
        for (int col = 0; col < cfg.largura; col++) {
            printf("%d ", image[(size_t) row * cfg.largura + col]);
        }

        printf("\n");
    }

    free(image);

    return 0;
}