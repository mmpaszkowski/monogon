//
// Created by Mateusz Paszkowski on 08.01.23.
//

#ifndef MONOGON_ARGMAX_H
#define MONOGON_ARGMAX_H

#include "../Array.h"

struct ArgMax
{
    template <typename T>
    Array<size_t> operator()(const Array<T> &array)
    {
        Array<size_t> result(Shape({array.get_shape()(0)}));

        for (size_t i = 0; i < array.get_shape()(0); i++)
        {
            T max = array(i, 0);
            result(i) = 0;
            for (size_t j = 1; j < array.get_shape()(-1); j++)
            {
                if(max < array(i, j))
                {
                    result(i) = j;
                    max = array(i, j);
                }
            }
        }
        return result;
    }
};

#endif //MONOGON_ARGMAX_H
