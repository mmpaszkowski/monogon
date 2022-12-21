//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_METRIC_H
#define MONOGON_METRIC_H

#include "../Array.h"
#include <cstddef>

template <typename T> class Metric
{
public:
    virtual T operator()(const Array<T>& y_pred, const Array<T>& y) const = 0;
};



#endif //MONOGON_METRIC_H
