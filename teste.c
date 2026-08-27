#include <stdio.h>
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

    return 0;
}