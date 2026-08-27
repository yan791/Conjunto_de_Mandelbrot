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

#endif