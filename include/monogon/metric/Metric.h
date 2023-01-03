//
// Created by Mateusz Paszkowski on 09.12.22.
//

#ifndef MONOGON_METRIC_H
#define MONOGON_METRIC_H

#include "../Array.h"
#include <cstddef>

template <typename T>
class Metric
{
public:
    Metric() = default;
    Metric(const Metric<T> &metric) = default;
    Metric(Metric<T> &&metric) noexcept = default;

    Metric &operator=(const Metric<T> &metric) = default;
    Metric &operator=(Metric<T> &&metric) noexcept = default;

    virtual ~Metric() = default;
public:
    virtual T operator()(const Array<T> &y_pred, const Array<T> &y) const = 0;
};


#endif //MONOGON_METRIC_H
