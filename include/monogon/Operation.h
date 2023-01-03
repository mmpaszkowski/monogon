//
// Created by Mateusz Paszkowski on 11.05.22.
//

#ifndef MONOGON_OPERATION_H
#define MONOGON_OPERATION_H

#include "utils/low_level.h"
#include <any>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

template <typename T>
class Array;

template <typename T>
struct Init
{
    template <typename U>
    T initialize(const T &val_type, U value) const;
};

template <typename T>
template <typename U>
T Init<T>::initialize([[maybe_unused]] const T &val_type, U value) const
{
    return value;
}

template <typename U>
struct Init<Array<U>>
{
    template <typename V>
    Array<U> initialize(const Array<U> &val_type, V value) const
    {
        return Array<U>(val_type.get_rows(), val_type.get_columns(), value);
    }
};

struct Operation
{
    Operation() = default;
    Operation(const Operation &) = default;
    Operation(Operation &&) noexcept = default;
    virtual ~Operation() = default;
    Operation &operator=(const Operation &) = default;
    Operation &operator=(Operation &&) noexcept = default;

    virtual std::tuple<std::any, std::any> perform(const std::any &, const std::any &, const std::any &) const = 0;
};

template <typename T = void, typename U = void, typename V = void>
struct AddOperation : public Operation
{
    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &, const std::any &) const override
    {
        return {grad, grad};
    }
};

template <typename T, typename U, typename V>
struct AddOperation<T, Array<U>, Array<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Array<U>;
    using rhs_type = Array<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        Array<U> l_result = Array<U>(lhs_val.get_rows(), lhs_val.get_columns(), 0.0);
        Array<V> r_result = Array<V>(rhs_val.get_rows(), rhs_val.get_columns(), 0.0);

        for (size_t i = 0; i < grad_val.get_rows(); i++)
        {
            for (size_t j = 0; j < grad_val.get_columns(); j++)
            {
                r_result(i % r_result.get_rows(), j % r_result.get_columns()) =
                    r_result(i % r_result.get_rows(), j % r_result.get_columns()) + grad_val(i, j);
                l_result(i % l_result.get_rows(), j % l_result.get_columns()) =
                    l_result(i % l_result.get_rows(), j % l_result.get_columns()) + grad_val(i, j);
            }
        }
        return std::tuple(l_result, r_result);
    }
};

template <typename T>
struct SubOperation : public Operation
{
    using grad_type = T;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &, const std::any &) const override
    {
        return std::tuple(grad, -(std::any_cast<grad_type>(grad)));
    }
};

template <typename T, typename U, typename V>
struct MulOperation : public Operation
{
    using grad_type = T;
    using lhs_type = U;
    using rhs_type = V;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        return std::tuple(grad_val * rhs_val, grad_val * lhs_val);
    }
};

template <typename T, typename U, typename V>
struct DivOperation : public Operation
{
    using grad_type = T;
    using lhs_type = U;
    using rhs_type = V;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        Init<lhs_type> lhs_initializer;

        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        return std::tuple(grad_val * (lhs_initializer.initialize(lhs_val, 1.0) / rhs_val),
                          grad_val * (-(lhs_val / (rhs_val * rhs_val))));
    }
};

template <typename T = void, typename U = void, typename V = void>
struct EqualOperation : public Operation
{
    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &, const std::any &) const override
    {
        return {grad, std::any()};
    }
};

template <typename T, typename U, typename V>
struct DotOperation;

template <typename T, typename U, typename V>
struct DotOperation<T, Array<U>, Array<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Array<U>;
    using rhs_type = Array<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        lhs_type l_result(grad_val.get_rows(), rhs_val.get_rows());
        rhs_type r_result(lhs_val.get_columns(), grad_val.get_columns());

        mat_T_mat_mul(lhs_val.get_columns(),
                      lhs_val.get_rows(),
                      grad_val.get_columns(),
                      &lhs_val(0, 0),
                      &grad_val(0, 0),
                      &r_result(0, 0));
        mat_mat_T_mul(grad_val.get_rows(),
                      grad_val.get_columns(),
                      rhs_val.get_rows(),
                      &grad_val(0, 0),
                      &rhs_val(0, 0),
                      &l_result(0, 0));

        return std::tuple(l_result, r_result);
    }
};

template <typename T, typename U, typename V>
struct MaxOperation;

template <typename T, typename U, typename V>
struct MaxOperation<T, Array<U>, V> : public Operation
{
    using grad_type = T;
    using lhs_type = Array<U>;
    using rhs_type = V;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_value = std::any_cast<rhs_type>(rhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);
        lhs_type lhs_result(lhs_value.get_rows(), lhs_value.get_columns());

        for (size_t i = 0; i < lhs_value.get_rows(); i++)
            for (size_t j = 0; j < lhs_value.get_columns(); j++)
                lhs_result(i, j) = lhs_value(i, j) < rhs_value ? U(0.0) : U(1.0);

        return std::tuple(grad_value * lhs_result, rhs_type(0.0));
    }
};

template <typename T, typename U>
struct AvgOperation;

template <typename T, typename U>
struct AvgOperation<T, Array<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Array<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &lhs, const std::any &) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Array<U> l_result = Array<U>(lhs_value.get_rows(),
                                     lhs_value.get_columns(),
                                     1.0 / (lhs_value.get_rows() * lhs_value.get_columns())) *
                            grad_value;
        return std::tuple(l_result, std::any());
    }
};

template <typename T, typename U>
struct ExpOperation;

template <typename T, typename U>
struct ExpOperation<T, Array<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Array<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &lhs, const std::any &) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Array<U> l_result = lhs_value.exp() * grad_value;
        return std::tuple(l_result, std::any());
    }
};

#endif //MONOGON_OPERATION_H
