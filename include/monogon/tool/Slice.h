//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_SLICE_H
#define MONOGON_SLICE_H

#include <cstddef>
#include "../Matrix.h"

struct Slice
{
    template<typename T> Matrix<T> operator()(const Matrix<T>& matrix, std::size_t from, std::size_t to) {
        Matrix<T> result(to - from, matrix.get_columns(), 0.0);
        for(size_t i = 0; i < to - from; i++)
        {
            for(size_t j = 0; j < matrix.get_columns(); j++)
            {
                result(i,j) = matrix(to+i, j);
            }
        }
        return result;
    }
};


#endif //MONOGON_SLICE_H
