//
// Created by Mateusz Paszkowski on 08.12.22.
//

#ifndef MONOGON_ONE_HOT_H
#define MONOGON_ONE_HOT_H

#include "../Array.h"
#include <cstddef>

struct OneHot
{
    template <typename T, typename U>
    Array<U> operator()(Array<T> indices, std::size_t depth, U on_value, U off_value)
    {
        Array result(Shape({indices.get_shape()(-1), depth}), off_value);

        for (size_t i = 0; i < indices.get_shape()(-1); i++)
        {
            result(i, static_cast<size_t>(indices(i))) = on_value;
        }
        return result;
    }
};

#endif //MONOGON_ONE_HOT_H
