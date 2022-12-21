//
// Created by noname on 21.12.22.
//

#include "monogon/utils/low_level.h"
#include <cblas.h>

void mat_mul(size_t m, size_t n, size_t p, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, static_cast<int>(m), static_cast<int>(n), static_cast<int>(p), 1.0, A, static_cast<int>(p), B, static_cast<int>(n), 0.0, C, static_cast<int>(n));
}