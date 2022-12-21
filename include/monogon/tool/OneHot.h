//
// Created by noname on 08.12.22.
//

#ifndef MONOGON_ONEHOT_H
#define MONOGON_ONEHOT_H

#include "../Array.h"
#include "../Vector.h"
#include <cstddef>

struct OneHot
{
    template<typename T, typename U>
    Array<U> operator()(Vector<T> indices, std::size_t depth, U on_value, U off_value) {
        Array result(indices.size(), depth, off_value);
        for(size_t i = 0; i < indices.size(); i++)
        {
            result(i, indices(i)) = on_value;
        }
        return result;
    }
};

#endif //MONOGON_ONEHOT_H
