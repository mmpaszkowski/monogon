//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_CATEGORICAL_ACCURACY_H
#define MONOGON_CATEGORICAL_ACCURACY_H

#include "Metric.h"

template <typename T = double>
class CategoricalAccuracy : public Metric<T>
{
public:
    CategoricalAccuracy() = default;//#TODO multidimensional __shape
    CategoricalAccuracy(const CategoricalAccuracy&) = default;
    CategoricalAccuracy(CategoricalAccuracy&&) noexcept = default;
    CategoricalAccuracy& operator=(const CategoricalAccuracy&) = default;
    CategoricalAccuracy& operator=(CategoricalAccuracy&&) noexcept = default;
    virtual ~CategoricalAccuracy() = default;
public:
    T operator()(const Array<T> &y_pred, const Array<T> &y) const override;
};

template <typename T>
T CategoricalAccuracy<T>::operator()(const Array<T> &y_pred, const Array<T> &y) const
{
    size_t result = 0;
    for (std::size_t i = 0; i < y.get_rows(); i++)
    {
        size_t max1 = 0;
        size_t max2 = 0;

        for (size_t j = 0; j < y.get_columns(); j++)
        {
            if (y_pred(i, j) > y_pred(i, max1))
                max1 = j;
            if (y(i, j) > y(i, max2))
                max2 = j;
        }
        if (max1 == max2)
            result++;
    }
    return static_cast<double>(result) / static_cast<double>(y.get_rows());
}

#endif //MONOGON_CATEGORICAL_ACCURACY_H
