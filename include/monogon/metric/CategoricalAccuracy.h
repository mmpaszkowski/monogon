//
// Created by noname on 09.12.22.
//

#ifndef MONOGON_CATEGORICALACCURACY_H
#define MONOGON_CATEGORICALACCURACY_H

#include "Metric.h"

template <typename T> class CategoricalAccuracy : public Metric<T>
{
public:
    virtual T operator()(const Array<T>& y_pred, const Array<T>& y) const override{
        size_t result = 0;
        for(std::size_t i = 0; i < y.get_rows(); i++)
        {
            size_t max1 = 0;
            size_t max2 = 0;

            for(size_t j = 0; j < y.get_columns(); j++)
            {
                if(y_pred(i,j) > y_pred(i, max1)) max1 = j;
                if(y(i,j) > y(i, max2)) max2 = j;
            }
            if(max1 == max2) result++;
        }
        return static_cast<double>(result)/static_cast<double>(y.get_rows());
    }
};

#endif //MONOGON_CATEGORICALACCURACY_H
