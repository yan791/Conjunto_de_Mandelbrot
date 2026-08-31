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

int faz_pthreads1(unsigned char *image, const config *config) {
    int n = config->threads;

    if (n > config->altura) {
        n = config->altura;
    }

    if (n < 1) {
        n = 1;
    }

    pthread_t *threads = (pthread_t *) malloc((size_t) n * sizeof(pthread_t));
    dadosthread *dados = (dadosthread *) malloc((size_t) n * sizeof(dadosthread));

    if (threads == NULL || dados == NULL) {
        fprintf(stderr,"Erro: falha ao alocar memoria para threads (pthreads1).\n");
        free(threads);
        free(dados);
        return -1;
    }

    int base = config->altura / n;
    int resto = config->altura % n;
    int proxima_linha = 0;
    int criadas = 0;

    for (int i = 0; i < n; i++) {
        int linhas = base;

        if (i < resto) {
            linhas++;
        }

        dados[i].imagem = image;
        dados[i].config = config;
        dados[i].linha_inicio = proxima_linha;
        dados[i].linha_fim = proxima_linha + linhas;
        dados[i].id = i;
        dados[i].erro = 0;

        proxima_linha += linhas;

        int rc = pthread_create(&threads[i], NULL, faz_bloco, &dados[i]);

        if (rc != 0) {
            fprintf(stderr,"Erro: falha ao criar thread %d (pthreads1): codigo %d.\n",i, rc);

            for (int j = 0; j < criadas; j++) {
                pthread_join(threads[j], NULL);
            }

            free(threads);
            free(dados);
            return -1;
        }

        criadas++;
    }

    for (int i = 0; i < criadas; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(dados);

    return 0;
}