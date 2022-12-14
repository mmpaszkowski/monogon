//
// Created by Mateusz Paszkowski on 21.12.22.
//

#include "monogon/utils/low_level.h"
#include <cblas.h>

void mat_mat_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor,
                CblasNoTrans,
                CblasNoTrans,
                static_cast<int>(m),
                static_cast<int>(k),
                static_cast<int>(n),
                1.0,
                A,
                static_cast<int>(n),
                B,
                static_cast<int>(k),
                0.0,
                C,
                static_cast<int>(k));
}

void mat_mat_T_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor,
                CblasNoTrans,
                CblasTrans,
                static_cast<int>(m),
                static_cast<int>(k),
                static_cast<int>(n),
                1.0,
                A,
                static_cast<int>(n),
                B,
                static_cast<int>(n),
                0.0,
                C,
                static_cast<int>(k));
}

void mat_T_mat_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor,
                CblasTrans,
                CblasNoTrans,
                static_cast<int>(m),
                static_cast<int>(k),
                static_cast<int>(n),
                1.0,
                A,
                static_cast<int>(m),
                B,
                static_cast<int>(k),
                0.0,
                C,
                static_cast<int>(k));
}
