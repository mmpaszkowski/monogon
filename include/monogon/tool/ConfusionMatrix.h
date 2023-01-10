//
// Created by Mateusz Paszkowski on 08.01.23.
//

#ifndef MONOGON_CONFUSION_MATRIX_H
#define MONOGON_CONFUSION_MATRIX_H

#include "../Array.h"

struct ConfusionMatrix
{
    template <typename T>
    Array<T> operator()(const Array<T> &v1, const Array<T> &v2)
    {
        size_t v1_max = v1(0);
        size_t v2_max = v2(0);

        for(size_t i = 1; i < v1.get_shape()(0); i++)
        {
            if(v1_max < v1(i))
                v1_max = v1(i);
            if(v2_max < v2(i))
                v2_max = v2(i);
        }

        Array<size_t> result(Shape({v1_max + 1, v2_max + 1}), 0);

        for (size_t i = 0; i < v1.get_shape()(0); i++)
        {
            result(v1(i), v2(i)) += 1;
        }
        return result;
    }
};

#endif //MONOGON_CONFUSION_MATRIX_H
