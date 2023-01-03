//
// Created by Mateusz Paszkowski on 09.12.22.
//

#ifndef MONOGON_SLICE_H
#define MONOGON_SLICE_H

#include "../Array.h"
#include <cstddef>

struct Slice
{
    template <typename T>
    Array<T> operator()(const Array<T> &matrix, std::size_t from, std::size_t to)
    {
        Array<T> result(to - from, matrix.get_columns(), 0.0);
        for (size_t i = 0; i < to - from; i++)
        {
            for (size_t j = 0; j < matrix.get_columns(); j++)
            {
                result(i, j) = matrix(from + i, j);
            }
        }
        return result;
    }
};

#endif //MONOGON_SLICE_H
