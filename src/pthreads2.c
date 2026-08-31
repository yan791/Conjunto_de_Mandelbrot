#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mandelbrot.h"
static pthread_mutex_t fila_mutex;

static void *pega_linha(void *arg) {
    filatrab *fila = (filatrab *) arg;
    const config *config = fila->config;

    while (1) {
        int row;

        pthread_mutex_lock(&fila_mutex);

        row = fila->proxima_linha;

        if (row < config->altura) {
            fila->proxima_linha = row + 1;
        }

        pthread_mutex_unlock(&fila_mutex);

        if (row >= config->altura) {
            break;
        }

        double ci = row_to_imag(row, config->altura);

        for (int col = 0; col < config->largura; col++) {
            double cr = col_to_real(col, config->largura);
            int iterations = mandelbrot_ponto(cr, ci, config->max_iter);
            fila->imagem[(size_t) row * config->largura + col] = iterations_to_intensity(iterations, config->max_iter);
        }
    }

    return NULL;
}