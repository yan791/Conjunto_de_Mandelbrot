#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mandelbrot.h"

static void *faz_bloco(void *arg) {
    dadosthread *dados = (dadosthread *) arg;
    const config *config = dados->config;

    for (int row = dados->linha_inicio; row < dados->linha_fim; row++) {
        double ci = row_to_imag(row, config->altura);

        for (int col = 0; col < config->largura; col++) {
            double cr = col_to_real(col, config->largura);
            int iterations = mandelbrot_ponto(cr, ci, config->max_iter);
            dados->imagem[(size_t) row * config->largura + col] = iterations_to_intensity(iterations, config->max_iter);
        }
    }

    return NULL;
}