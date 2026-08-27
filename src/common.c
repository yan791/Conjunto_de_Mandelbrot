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
