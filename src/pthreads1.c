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

int faz_pthreads1(unsigned char *imagem, const config *config) {
    int n = config->threads;

    if (n > config->altura) {
        n = config->altura;
    }

    pthread_t *threads = malloc((size_t) n * sizeof(pthread_t));
    dadosthread *dados = malloc((size_t) n * sizeof(dadosthread));

    if (threads == NULL || dados == NULL) {
        free(threads);
        free(dados);
        return -1;
    }

    int base = config->altura / n;
    int resto = config->altura % n;

    int linha_atual = 0;

    for (int i = 0; i < n; i++) {
        int quantidade = base;

        if (i < resto) {
            quantidade++;
        }

        dados[i].imagem = imagem;
        dados[i].config = config;
        dados[i].linha_inicio = linha_atual;
        dados[i].linha_fim = linha_atual + quantidade;
        dados[i].id = i;
        dados[i].erro = 0;

        printf("Thread %d: linhas %d ate %d\n",
               dados[i].id,
               dados[i].linha_inicio,
               dados[i].linha_fim - 1);

        linha_atual += quantidade;
    }

    free(threads);
    free(dados);

    return 0;
}