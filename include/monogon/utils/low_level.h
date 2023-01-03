//
// Created by Mateusz Paszkowski on 21.12.22.
//

#ifndef MONOGON_LOW_LEVEL_H
#define MONOGON_LOW_LEVEL_H

#include <cstddef>

void mat_mat_mul(size_t m, size_t n, size_t p, const double *A, const double *B, double *C);
void mat_mat_T_mul(size_t m, size_t n, size_t p, const double *A, const double *B, double *C);
void mat_T_mat_mul(size_t m, size_t n, size_t p, const double *A, const double *B, double *C);


#endif //MONOGON_LOW_LEVEL_H
