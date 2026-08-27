#include <stdio.h>
#include <stdlib.h>
#include "mandelbrot.h"

int main(void) {
    config config;

    config.largura = 16;
    config.altura = 12;
    config.max_iter = 100;
    config.threads = 1;

    unsigned char *image = alloca_imagem(
        config.largura,
        config.altura
    );

    if (image == NULL) {
        fprintf(stderr, "Erro: nao foi possivel alocar a imagem.\n");
        return 1;
    }

    compute_serial(image, &config);

    if (pgm_salva(
            "mandelbrot_serial.pgm",
            image,
            config.largura,
            config.altura) != 0) {

        fprintf(stderr, "Erro: nao foi possivel salvar a imagem.\n");
        free(image);
        return 1;
    }

    free(image);

    return 0;
}