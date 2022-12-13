//
// Created by Mateusz Paszkowski on 11.05.22.
//

#ifndef MONOGON_OPERATION_H
#define MONOGON_OPERATION_H

#include <any>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <tuple>

template <typename T>
class Vector;

template <typename T>
class Matrix;

template <typename T>
class Tensor;

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
struct Init<Vector<U>>
{
    template <typename V>
    Vector<U> initialize(const Vector<U> &val_type, V value) const
    {
        return Vector<U>(val_type.size(), value);
    }
};

template <typename U>
struct Init<Matrix<U>>
{
    template <typename V>
    Matrix<U> initialize(const Matrix<U> &val_type, V value) const
    {
        return Matrix<U>(val_type.get_rows(), val_type.get_columns(), value);
    }
};

template <typename U>
struct Init<Tensor<U>>
{
    template <typename V>
    Tensor<U> initialize(const Tensor<U> &val_type, V value) const
    {
        return Tensor<U>(val_type.shape(), value);
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
struct AddOperation<T, Matrix<U>, Vector<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;
    using rhs_type = Vector<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           [[maybe_unused]] const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        Vector<V> r_result = Vector<V>(rhs_val.size(), 0.0);
        for (size_t i = 0; i < grad_val.get_rows(); i++)
        {
            for (size_t j = 0; j < grad_val.get_columns(); j++)
            {
                r_result(i) = r_result(i) + grad_val(i, j);
            }
        }
        return std::tuple(grad, r_result);
    }
};

template <typename T, typename U, typename V>
struct AddOperation<T, Matrix<U>, Matrix<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;
    using rhs_type = Matrix<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        Matrix<U> l_result = Matrix<U>(lhs_val.get_rows(), lhs_val.get_columns(), 0.0);
        Matrix<V> r_result = Matrix<V>(rhs_val.get_rows(), rhs_val.get_columns(), 0.0);

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

//template <typename T, typename U, typename V>
//struct AddOperation<T, Tensor<U>, Tensor<V>> : public Operation
//{
//    using grad_type = T;
//    using lhs_type = Matrix<U>;
//    using rhs_type = Matrix<V>;
//
//    std::tuple<std::any, std::any> perform(const std::any &grad,
//                                           const std::any &lhs,
//                                           const std::any &rhs) const override
//    {
//        grad_type grad_val = std::any_cast<grad_type>(grad);
//        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
//        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);
//
//        Matrix<U> l_result = Matrix<U>(lhs_val.get_rows(), lhs_val.get_columns(), 0.0);
//        Matrix<V> r_result = Matrix<V>(rhs_val.get_rows(), rhs_val.get_columns(), 0.0);
//
//        for (size_t i = 0; i < grad_val.get_rows(); i++)
//        {
//            for (size_t j = 0; j < grad_val.get_columns(); j++)
//            {
//                r_result(i % r_result.get_rows(), j % r_result.get_columns()) =
//                    r_result(i % r_result.get_rows(), j % r_result.get_columns()) + grad_val(i, j);
//                l_result(i % l_result.get_rows(), j % l_result.get_columns()) =
//                    l_result(i % l_result.get_rows(), j % l_result.get_columns()) + grad_val(i, j);
//            }
//        }
//        return std::tuple(l_result, r_result);
//    }
//};

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
struct DotOperation<T, Vector<U>, Vector<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Vector<U>;
    using rhs_type = Vector<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        Vector<U> l_result = rhs_val;
        Vector<V> r_result = lhs_val;
        return std::tuple(l_result * grad_val, r_result * grad_val);
    }
};

template <typename T, typename U, typename V>
struct DotOperation<T, Matrix<U>, Vector<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;
    using rhs_type = Vector<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        grad_type grad_val = std::any_cast<grad_type>(grad);
        lhs_type lhs_val = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_val = std::any_cast<rhs_type>(rhs);

        Vector<V> r_result = std::any_cast<lhs_type>(lhs).transpose().dot(std::any_cast<grad_type>(grad));
        Matrix<V> l_result = std::any_cast<grad_type>(grad).dot(std::any_cast<rhs_type>(rhs).transpose());
        return std::tuple(l_result, r_result);
    }
};

template <typename T, typename U, typename V>
struct DotOperation<T, Matrix<U>, Matrix<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;
    using rhs_type = Matrix<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        rhs_type r_result = std::any_cast<lhs_type>(lhs).transpose().dot(std::any_cast<grad_type>(grad));
        lhs_type l_result = std::any_cast<grad_type>(grad).dot(std::any_cast<rhs_type>(rhs).transpose());
        return std::tuple(l_result, r_result);
    }
};

template <typename T, typename U, typename V>
struct DotOperation<T, Tensor<U>, Tensor<V>> : public Operation
{
    using grad_type = T;
    using lhs_type = Tensor<U>;
    using rhs_type = Tensor<V>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           const std::any &rhs) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        rhs_type rhs_value = std::any_cast<rhs_type>(rhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        std::vector<size_t> v;
        for(size_t i = 0; i < lhs_value.shape().size(); i++)
        {
            v.push_back(i);
        }
        std::swap(v.back(), v[v.size()-2]);


        rhs_type r_result = lhs_value.transpose(v).dot(grad_value);
        lhs_type l_result = grad_value.dot(rhs_value.transpose(v));
        return std::tuple(l_result, r_result);
    }
};

template <typename T, typename U, typename V>
struct MaxOperation;

template <typename T, typename U, typename V>
struct MaxOperation<T, Matrix<U>, V> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;
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
struct AvgOperation<T, Vector<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Vector<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad,
                                           const std::any &lhs,
                                           [[maybe_unused]] const std::any &rhs) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Vector<U> l_result = Vector<U>(lhs_value.size(), 1.0 / lhs_value.size()) * grad_value;
        return std::tuple(l_result, std::any());
    }
};

template <typename T, typename U>
struct AvgOperation<T, Matrix<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &lhs, const std::any &) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        //        rhs_type rhs_value = std::any_cast<rhs_type>(rhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Matrix<U> l_result = Matrix<U>(lhs_value.get_rows(),
                                       lhs_value.get_columns(),
                                       1.0 / (lhs_value.get_rows() * lhs_value.get_columns())) *
                             grad_value;
        return std::tuple(l_result, std::any());
    }
};

template <typename T, typename U>
struct AvgOperation<T, Tensor<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Tensor<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &lhs, const std::any &) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        //        rhs_type rhs_value = std::any_cast<rhs_type>(rhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Tensor<U> l_result = lhs_type(lhs_value.shape(),
                                       1.0 / std::accumulate(lhs_value.shape().begin(), lhs_value.shape().end(), static_cast<size_t>(1), std::multiplies())) *
                             grad_value;
        return std::tuple(l_result, std::any());
    }
};

template <typename T, typename U>
struct ExpOperation;

template <typename T, typename U>
struct ExpOperation<T, Matrix<U>> : public Operation
{
    using grad_type = T;
    using lhs_type = Matrix<U>;

    std::tuple<std::any, std::any> perform(const std::any &grad, const std::any &lhs, const std::any &) const override
    {
        lhs_type lhs_value = std::any_cast<lhs_type>(lhs);
        grad_type grad_value = std::any_cast<grad_type>(grad);

        Matrix<U> l_result = lhs_value.exp() * grad_value;
        return std::tuple(l_result, std::any());
    }
};


#endif //MONOGON_OPERATION_H

