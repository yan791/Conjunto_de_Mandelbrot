#include <omp.h>
#include "mandelbrot.h"

void faz_openmp(unsigned char *image, const config *config) {
    omp_set_num_threads(config->threads);

    #pragma omp parallel for schedule(dynamic, 1)
    for (int row = 0; row < config->altura; row++) {
        double ci = row_to_imag(row, config->altura);

        for (int col = 0; col < config->largura; col++) {
            double cr = col_to_real(col, config->largura);
            int iterations = mandelbrot_ponto(cr, ci, config->max_iter);
            image[(size_t) row * config->largura + col] = iterations_to_intensity(iterations, config->max_iter);
        }
    }
}
