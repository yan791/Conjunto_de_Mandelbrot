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

int faz_pthreads2(unsigned char *image, const config *config) {
    int n = config->threads;

    if (n > config->altura) {
        n = config->altura;
    }

    pthread_t *threads =
        malloc((size_t) n * sizeof(pthread_t));

    filatrab fila;

    fila.imagem = image;
    fila.config = config;
    fila.proxima_linha = 0;
    fila.erro = 0;

    int criadas = 0;

    for (int i = 0; i < n; i++) {
        int rc = pthread_create(
            &threads[i],
            NULL,
            pega_linha,
            &fila
        );

        if (rc != 0) {
            fprintf(stderr,
                    "Erro: falha ao criar thread %d.\n",
                    i);

            for (int j = 0; j < criadas; j++) {
                pthread_join(threads[j], NULL);
            }

            free(threads);
            return -1;
        }

        criadas++;
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    return 0;
}   