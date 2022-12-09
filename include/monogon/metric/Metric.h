//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_METRIC_H
#define MONOGON_METRIC_H

#include <cstddef>
#include "../Matrix.h"

template <typename T> class Metric
{
public:
    virtual T operator()(const Matrix<T>& y_pred, const Matrix<T>& y) const = 0;
};



#endif //MONOGON_METRIC_H
