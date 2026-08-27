#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "mandelbrot.h"

int mandelbrot_ponto(double cr, double ci, int max_iter) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    while (iter < max_iter && (zr * zr + zi * zi) <= 4.0) {
        double zr_novo = zr * zr - zi * zi + cr;
        double zi_novo = 2.0 * zr * zi + ci;

        zr = zr_novo;
        zi = zi_novo;
        iter++;
    }

    return iter;
}

double col_to_real(int col, int width) {
    if (width <= 1) {
        return REAL_MIN;
    }

    return REAL_MIN + (double) col * (REAL_MAX - REAL_MIN) / (double) (width - 1);
}

double row_to_imag(int row, int height) {
    if (height <= 1) {
        return IMAG_MIN;
    }

    return IMAG_MIN + (double) row * (IMAG_MAX - IMAG_MIN) / (double) (height - 1);
}

unsigned char iterations_to_intensity(int iterations, int max_iter) {
    if (max_iter <= 0) {
        return 0;
    }

    double proporcao = (double) iterations / (double) max_iter;
    int valor = (int) (proporcao * 255.0 + 0.5);

    if (valor < 0) {
        valor = 0;
    }

    if (valor > 255) {
        valor = 255;
    }

    return (unsigned char) valor;
}

unsigned char *alloca_imagem(int width, int height) {
    if (width <= 0 || height <= 0) {
        return NULL;
    }

    size_t w = (size_t) width;
    size_t h = (size_t) height;

    if (w != 0 && h > (SIZE_MAX / w)) {
        return NULL;
    }

    size_t total = w * h;

    unsigned char *imagem = (unsigned char *) malloc(total * sizeof(unsigned char));

    return imagem;
}