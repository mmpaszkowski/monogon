//
// Created by Mateusz Paszkowski on 09.12.22.
//

#ifndef MONOGON_SLICE_H
#define MONOGON_SLICE_H

#include <cstddef>
#include "../Array.h"

struct Slice
{
    template <typename T>
    Array<T> operator()(const Array<T> &array, std::size_t from, std::size_t to)
    {
        Array<T> result(Shape({to - from, array.get_shape()(-1)}), 0.0);
        for (size_t i = 0; i < to - from; i++)
        {
            for (size_t j = 0; j < array.get_shape()(-1); j++)
            {
                result(i, j) = array(from + i, j);
            }
        }
        return result;
    }
};

#endif //MONOGON_SLICE_H
