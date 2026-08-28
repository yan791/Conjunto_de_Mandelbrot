#ifndef MANDELBROT_H
#define MANDELBROT_H
#include <stddef.h>

#define REAL_MIN (-2.0)
#define REAL_MAX (1.0)
#define IMAG_MIN (-1.5)
#define IMAG_MAX (1.5)

int mandelbrot_ponto(double cr, double ci, int max_iter);
double col_to_real(int col, int width);
double row_to_imag(int row, int height);
unsigned char iterations_to_intensity(int iterations, int max_iter);

typedef struct {
    int largura;
    int altura;
    int max_iter;
    int threads;
} config;

unsigned char *alloca_imagem(int width, int height);
void compute_serial(unsigned char *image, const config *config);
int pgm_salva(const char *filename, const unsigned char *image, int width, int height);
double retorna_segundos(void);
#endif