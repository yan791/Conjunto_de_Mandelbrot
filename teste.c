#include <stdio.h>
#include "mandelbrot.h"

int main(void) {
    int max_iter = 100;

    printf("c = (0, 0): %d\n",
           mandelbrot_ponto(0.0, 0.0, max_iter));

    printf("c = (-1, 0): %d\n",
           mandelbrot_ponto(-1.0, 0.0, max_iter));

    printf("c = (1, 0): %d\n",
           mandelbrot_ponto(1.0, 0.0, max_iter));

    printf("c = (2, 0): %d\n",
           mandelbrot_ponto(2.0, 0.0, max_iter));

    return 0;
}