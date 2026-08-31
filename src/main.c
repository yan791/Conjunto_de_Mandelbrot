#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "mandelbrot.h"
#include "login.h"
#define TIMES_FILE "times.txt"

#define EXIT_ARGS_ERROR      1
#define EXIT_VALUE_ERROR     2
#define EXIT_MEMORY_ERROR    3
#define EXIT_FILE_ERROR      4
#define EXIT_THREAD_ERROR    5

static int parse_positive_int(const char *text, long *out) {
    if (text == NULL || text[0] == '\0') {
        return 0;
    }

    errno = 0;
    char *endptr = NULL;
    long value = strtol(text, &endptr, 10);

    if (errno == ERANGE) {
        return 0;
    }
    if (endptr == text || *endptr != '\0') {
        return 0; 
    }
    if (value <= 0) {
        return 0; 
    }

    *out = value;
    return 1;
}

static void print_usage(const char *prog_name) {
    fprintf(stderr,
            "Uso: %s [largura] [altura] [max_iteracoes] [num_threads]\n"
            "  largura: inteiro positivo (largura da imagem em pixels)\n"
            "  altura: inteiro positivo (altura da imagem em pixels)\n"
            "  max_iteracoes: inteiro positivo (limite de iteracoes por ponto)\n"
            "  num_threads: inteiro positivo (threads usadas nas versoes paralelas)\n",prog_name);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Erro: numero incorreto de argumentos (esperado 4, recebido %d).\n",argc - 1);
        print_usage(argv[0]);
        return EXIT_ARGS_ERROR;
    }

    long width_l, height_l, max_iter_l, num_threads_l;

    if (!parse_positive_int(argv[1], &width_l)) {
        fprintf(stderr, "Erro: 'largura' invalida (%s). Deve ser um inteiro positivo.\n", argv[1]);
        return EXIT_VALUE_ERROR;
    }
    if (!parse_positive_int(argv[2], &height_l)) {
        fprintf(stderr, "Erro: 'altura' invalida (%s). Deve ser um inteiro positivo.\n", argv[2]);
        return EXIT_VALUE_ERROR;
    }
    if (!parse_positive_int(argv[3], &max_iter_l)) {
        fprintf(stderr, "Erro: 'max_iteracoes' invalido (%s). Deve ser um inteiro positivo.\n", argv[3]);
        return EXIT_VALUE_ERROR;
    }
    if (!parse_positive_int(argv[4], &num_threads_l)) {
        fprintf(stderr, "Erro: 'num_threads' invalido (%s). Deve ser um inteiro positivo.\n", argv[4]);
        return EXIT_VALUE_ERROR;
    }

    if (width_l > INT_MAX || height_l > INT_MAX ||
        max_iter_l > INT_MAX || num_threads_l > INT_MAX) {
        fprintf(stderr, "Erro: um dos parametros excede o limite permitido.\n");
        return EXIT_VALUE_ERROR;
    }

    config config;
    config.largura = (int) width_l;
    config.altura = (int) height_l;
    config.max_iter = (int) max_iter_l;
    config.threads = (int) num_threads_l;

    unsigned char *image = alloca_imagem(config.largura, config.altura);
    if (image == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para a imagem (%dx%d).\n",config.largura, config.altura);
        return EXIT_MEMORY_ERROR;
    }

    char filename[512];
    double t0, t1;
    int reset_times_file = 1; 
    t0 = retorna_segundos();
    compute_serial(image, &config);
    t1 = retorna_segundos();

    snprintf(filename, sizeof(filename), "mandelbrot_%s_serial.pgm", LOGIN);
    if (pgm_salva(filename, image, config.largura, config.altura) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }
    if (salva_time_log(TIMES_FILE, "serial", t1 - t0, reset_times_file) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }
    reset_times_file = 0;

    t0 = retorna_segundos();
    faz_openmp(image, &config);
    t1 = retorna_segundos();

    snprintf(filename, sizeof(filename), "mandelbrot_%s_openmp.pgm", LOGIN);
    if (pgm_salva(filename, image, config.largura, config.altura) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }
    if (salva_time_log(TIMES_FILE, "openmp", t1 - t0, reset_times_file) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }

    t0 = retorna_segundos();
    if (faz_pthreads1(image, &config) != 0) {
        fprintf(stderr, "Erro: falha na execucao da versao pthreads1.\n");
        free(image);
        return EXIT_THREAD_ERROR;
    }
    t1 = retorna_segundos();

    snprintf(filename, sizeof(filename), "mandelbrot_%s_pthreads1.pgm", LOGIN);
    if (pgm_salva(filename, image, config.largura, config.altura) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }
    if (salva_time_log(TIMES_FILE, "pthreads1", t1 - t0, reset_times_file) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }

    t0 = retorna_segundos();
    if (faz_pthreads2(image, &config) != 0) {
        fprintf(stderr, "Erro: falha na execucao da versao pthreads2.\n");
        free(image);
        return EXIT_THREAD_ERROR;
    }
    t1 = retorna_segundos();

    snprintf(filename, sizeof(filename), "mandelbrot_%s_pthreads2.pgm", LOGIN);
    if (pgm_salva(filename, image, config.largura, config.altura) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }
    if (salva_time_log(TIMES_FILE, "pthreads2", t1 - t0, reset_times_file) != 0) {
        free(image);
        return EXIT_FILE_ERROR;
    }

    free(image);
    return 0; 
}