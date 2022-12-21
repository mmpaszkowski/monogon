//
// Created by noname on 21.12.22.
//

#include "monogon/utils/low_level.h"
#include <cblas.h>

void mat_mat_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, static_cast<int>(m), static_cast<int>(k), static_cast<int>(n), 1.0, A, static_cast<int>(n), B, static_cast<int>(k), 0.0, C, static_cast<int>(k));
}

//                 ###        #####
//#####            ###        #####
//##### 3n    X    ### 4p   = ##### 4p
//#####            ###        #####
//  5m              3n          5m
void mat_mat_T_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, static_cast<int>(m), static_cast<int>(k), static_cast<int>(n), 1.0, A, static_cast<int>(n), B, static_cast<int>(n), 0.0, C, static_cast<int>(k));

}

void mat_T_mat_mul(size_t m, size_t n, size_t k, const double *A, const double *B, double *C)
{
    cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, static_cast<int>(m), static_cast<int>(k), static_cast<int>(n), 1.0, A, static_cast<int>(m), B, static_cast<int>(k), 0.0, C, static_cast<int>(k));
}

//void main()
//{
//    int i=0;
//    double A[6] = {1.0,2.0,1.0,-3.0,4.0,-1.0};
//    double B[6] = {1.0,2.0,1.0,-3.0,4.0,-1.0};
//    double C[9] = {.5,.5,.5,.5,.5,.5,.5,.5,.5};
//    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,3,3,2,1,A, 3, B, 3,2,C,3);
//
//    for(i=0; i<9; i++)
//        printf("%lf ", C[i]);
//    printf("\n");
//}