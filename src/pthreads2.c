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

    if (n < 1) {
        n = 1;
    }

    pthread_t *threads =
        (pthread_t *) malloc((size_t) n * sizeof(pthread_t));

    if (threads == NULL) {
        fprintf(stderr,"Erro: falha ao alocar memoria para threads (pthreads2).\n");
        return -1;
    }

    filatrab fila;
    fila.imagem = image;
    fila.config = config;
    fila.proxima_linha = 0;
    fila.erro = 0;

    if (pthread_mutex_init(&fila_mutex, NULL) != 0) {
        fprintf(stderr, "Erro: falha ao inicializar o mutex (pthreads2).\n");
        free(threads);
        return -1;
    }

    int criadas = 0;

    for (int i = 0; i < n; i++) {
        int rc = pthread_create(&threads[i], NULL, pega_linha, &fila);

        if (rc != 0) {
            fprintf(stderr,"Erro: falha ao criar thread %d (pthreads2): codigo %d.\n",i, rc);

            for (int j = 0; j < criadas; j++) {
                pthread_join(threads[j], NULL);
            }

            free(threads);
            pthread_mutex_destroy(&fila_mutex);
            return -1;
        }

        criadas++;
    }

    for (int i = 0; i < criadas; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    pthread_mutex_destroy(&fila_mutex);

    return 0;
}
