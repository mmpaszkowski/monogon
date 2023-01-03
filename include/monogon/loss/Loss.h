//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_LOSS_H
#define MONOGON_LOSS_H

#include "../Array.h"
#include "../Variable.h"

template <typename T>
class Loss
{
public:
    Loss() = default;
    Loss(const Loss<T> &loss) = default;
    Loss(Loss<T> &&loss) noexcept = default;

    Loss &operator=(const Loss<T> &loss) = default;
    Loss &operator=(Loss<T> &&loss) noexcept = default;

    virtual ~Loss() = default;

public:
    virtual Variable<T> operator()(Variable<Array<T>> y_pred, Variable<Array<T>> y) const = 0;
};

#endif //MONOGON_LOSS_H
