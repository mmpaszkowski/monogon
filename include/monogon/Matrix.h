//
// Created by Mateusz Paszkowski on 22.07.22.
//

#ifndef MONOGON_MATRIX_H
#define MONOGON_MATRIX_H

#include <cassert>
#include <ostream>
#include <valarray>

template <typename T> class Vector;

//---------------------------------------------------- Class body ------------------------------------------------------

template <typename T> class Matrix
{
  public:
    using value_type = T;
    using size_type = std::size_t;
    Matrix();
    Matrix(size_t row, size_t column);
    Matrix(size_t row, size_t column, const T &value);

    template <template <typename...> typename Container, template <typename...> typename NestedContainer>
    Matrix(const Container<NestedContainer<value_type>> &container);
    Matrix(std::initializer_list<std::initializer_list<value_type>> nasted_list);

    Matrix(const Matrix<T> &);
    Matrix(Matrix<T> &&);

    Matrix<T> &operator=(const Matrix<T> &);
    Matrix<T> &operator=(const Matrix<T> &&);

  public:
    value_type &operator()(std::size_t row, std::size_t column);
    const value_type &operator()(std::size_t row, std::size_t column) const;

    auto operator-() const;
    template <typename U> auto operator+(const Matrix<U> &rhs) const;
    template <typename U> auto operator-(const Matrix<U> &rhs) const;
    template <typename U> auto operator*(const Matrix<U> &rhs) const;
    template <typename U> auto operator/(const Matrix<U> &rhs) const;

    template <typename U> auto operator+(const Vector<U> &rhs) const;
    template <typename U> auto operator-(const Vector<U> &rhs) const;
    template <typename U> auto operator*(const Vector<U> &rhs) const;
    template <typename U> auto operator/(const Vector<U> &rhs) const;

    template <typename U> auto operator+(const U &val) const;
    template <typename U> auto operator-(const U &val) const;
    template <typename U> auto operator*(const U &val) const;
    template <typename U> auto operator/(const U &val) const;

    template <typename U> bool operator==(const Matrix<U> &rhs) const;

    Matrix<T> transpose() const;
    template <typename U> auto dot(const Matrix<U> &rhs) const;
    template <typename U> auto dot(const Vector<U> &rhs) const;

    auto exp() const;
    auto max(T value) const;
    auto avg() const;

  public:
    const std::valarray<value_type> &get_data() const;
    size_type get_rows() const;
    size_type get_columns() const;

  private:
    std::valarray<value_type> data;
    size_t row_size;
    size_t column_size;

  public:
    template <typename U> friend std::ostream &operator<<(std::ostream &os, const Matrix<U> &variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T> Matrix<T>::Matrix() : data(), row_size(0), column_size(0)
{
}

template <typename T>
Matrix<T>::Matrix(size_t row, size_t column) : data(row * column), row_size(row), column_size(column)
{
}

template <typename T>
Matrix<T>::Matrix(size_t row, size_t column, const T &value) : data(row * column), row_size(row), column_size(column)
{
    for (auto &&item : data)
        item = value;
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> nasted_list)
    : row_size(nasted_list.size()), column_size(nasted_list.begin()->size())
{
    this->data.resize(this->row_size * this->column_size);
    size_t i = 0;
    for (const auto &list : nasted_list)
        for (const auto &item : list)
            data[i++] = item;
}

template <typename T>
template <template <typename...> typename Container, template <typename...> typename NestedContainer>
Matrix<T>::Matrix(const Container<NestedContainer<T>> &container)
    : row_size(container.size()), column_size(container.begin()->size())
{
    this->data.resize(this->row_size * this->column_size);

    for (size_t i = 0, k = 0; i < container.size(); ++i)
        for (size_t j = 0; j < container[i].size(); ++j)
            data[k++] = container[i][j];
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &rhs) : data(rhs.data), row_size(rhs.row_size), column_size(rhs.column_size)
{
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> &&rhs)
    : data(std::move(rhs.data)), row_size(std::move(rhs.row_size)), column_size(std::move(rhs.column_size))
{
}

template <typename T> Matrix<T> &Matrix<T>::operator=(const Matrix<T> &rhs)
{
    this->data = rhs.data;
    this->column_size = rhs.column_size;
    this->row_size = rhs.row_size;
    return *this;
}

template <typename T> Matrix<T> &Matrix<T>::operator=(const Matrix<T> &&rhs)
{
    this->data = std::move(rhs.data);
    this->column_size = std::move(rhs.column_size);
    this->row_size = std::move(rhs.row_size);
    return *this;
}

//----------------------------------------------------- Operators ------------------------------------------------------

template <typename T> T &Matrix<T>::operator()(std::size_t row, std::size_t column)
{
    return data[row * column_size + column];
}

template <typename T> const T &Matrix<T>::operator()(std::size_t row, std::size_t column) const
{
    return data[row * column_size + column];
}

template <typename T> auto Matrix<T>::operator-() const
{
    Matrix<T> result(this->row_size, this->column_size);
    result.data = -this->data;

    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator+(const Matrix<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Matrix<result_val_type> result(std::max(this->row_size, rhs.row_size),
                                   std::max(this->column_size, rhs.get_columns()));

    for (size_t i = 0; i < result.get_rows(); i++)
    {
        for (size_t j = 0; j < result.get_columns(); j++)
        {
            result(i, j) = this->operator()(i % this->get_rows(), j % this->get_columns())
                           + rhs(i % rhs.get_rows(), j % rhs.get_columns());
        }
    }
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator-(const Matrix<U> &rhs) const
{
    assert(this->row_size == rhs.row_size && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Matrix<result_val_type> result(this->row_size, this->column_size);
    result.data = this->data - rhs.data;

    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator*(const Matrix<U> &rhs) const
{
    assert(this->row_size == rhs.row_size && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Matrix<result_val_type> result(this->row_size, this->column_size);
    result.data = this->data * rhs.data;

    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator/(const Matrix<U> &rhs) const
{
    assert(this->row_size == rhs.row_size && this->column_size == rhs.column_size);
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Matrix<result_val_type> result(this->row_size, this->column_size);
    result.data = this->data / rhs.data;

    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator+(const Vector<U> &rhs) const
{
    assert(rhs.size() == this->get_rows());
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());

    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    for (size_t i = 0; i < this->get_rows(); i++)
        for (size_t j = 0; j < this->get_columns(); j++)
            result(i, j) = this->operator()(i, j) + rhs(i);
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator-(const Vector<U> &rhs) const
{
    assert(rhs.size() == this->get_rows());
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    for (size_t i = 0; i < this->get_rows(); i++)
        for (size_t j = 0; j < this->get_columns(); j++)
            result(i, j) = this->operator()(i, j) - rhs(i);
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator*(const Vector<U> &rhs) const
{
    assert(rhs.size() == this->get_rows());
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    for (size_t i = 0; i < this->get_rows(); i++)
        for (size_t j = 0; j < this->get_columns(); j++)
            result(i, j) = this->operator()(i, j) * rhs(i);
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator/(const Vector<U> &rhs) const
{
    assert(rhs.size() == this->get_rows());
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    for (size_t i = 0; i < this->get_rows(); i++)
        for (size_t j = 0; j < this->get_columns(); j++)
            result(i, j) = this->operator()(i, j) / rhs(i);
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator+(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    result.data = data + val;
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator-(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());
    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    result.data = data - val;
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator*(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    result.data = data * val;
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::operator/(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());
    Matrix<result_val_type> result(this->get_rows(), this->get_columns());
    result.data = data / val;
    return result;
}

template <typename T> template <typename U> bool Matrix<T>::operator==(const Matrix<U> &rhs) const
{
    for (size_t i = 0; i < this->row_size; i++)
        for (size_t j = 0; j < this->column_size; j++)
            if (this->operator()(i, j) != rhs(i, j))
                return false;
    return true;
}

//------------------------------------------------------ Methods -------------------------------------------------------

template <typename T> Matrix<T> Matrix<T>::transpose() const
{
    Matrix<T> result(this->get_columns(), this->get_rows());

    for (size_t i = 0; i < this->row_size; i++)
        for (size_t j = 0; j < this->column_size; j++)
            result(j, i) = this->data[i * this->column_size + j];
    return result;
}

template <typename T> template <typename U> auto Matrix<T>::dot(const Matrix<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<T>() * std::declval<U>());
    Matrix<result_val_type> result(this->row_size, rhs.column_size);

    for (size_t i = 0; i < this->row_size; ++i)
        for (size_t j = 0; j < rhs.column_size; ++j)
            for (size_t k = 0; k < this->column_size; ++k)
                result(i, j) += data[i * this->column_size + k] * rhs(k, j);

    return result;
}

template <typename T> template <typename U> auto Matrix<T>::dot(const Vector<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<T>() * std::declval<U>());
    Vector<result_val_type> result(this->row_size);

    for (size_t i = 0; i < this->get_rows(); ++i)
        for (size_t j = 0; j < this->get_columns(); ++j)
            result(i) += this->operator()(i, j) * rhs(j);
    return result;
}

template <typename T> auto Matrix<T>::exp() const
{
    using result_val_type = T;
    Matrix<result_val_type> result(this->row_size, this->column_size);

    for (size_t i = 0; i < this->get_rows(); ++i)
        for (size_t j = 0; j < this->get_columns(); ++j)
            result(i, j) = std::exp(this->operator()(i, j));
    return result;
}

template <typename T> auto Matrix<T>::max(T value) const
{
    using result_val_type = T;
    Matrix<result_val_type> result(this->row_size, this->column_size);

    for (size_t i = 0; i < this->get_rows(); ++i)
        for (size_t j = 0; j < this->get_columns(); ++j)
            result(i, j) = std::max(value, this->operator()(i, j));
    return result;
}

template <typename T> auto Matrix<T>::avg() const
{
    T sum{};
    for (const auto &item : data)
        sum += item;

    return sum / data.size();
}

//----------------------------------------------------- Accessors ------------------------------------------------------

template <typename T> const std::valarray<T> &Matrix<T>::get_data() const
{
    return this->data;
}

template <typename T> size_t Matrix<T>::get_rows() const
{
    return this->row_size;
}

template <typename T> size_t Matrix<T>::get_columns() const
{
    return this->column_size;
}

template <typename T> std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
{
    os << "[";
    for (size_t i = 0; i < matrix.get_rows() - 1; i++)
    {
        if (i == 0)
            os << "[";
        else
            os << " [";
        for (size_t j = 0; j < matrix.get_columns() - 1; j++)
        {
            os << matrix(i, j) << ",";
        }
        os << matrix(i, matrix.get_columns() - 1);
        os << "]," << std::endl;
    }
    if (matrix.get_rows() == 1)
        os << "[";
    else
        os << " [";
    for (size_t j = 0; j < matrix.get_columns() - 1; j++)
    {
        os << matrix(matrix.get_rows() - 1, j) << ",";
    }
    os << matrix(matrix.get_rows() - 1, matrix.get_columns() - 1);
    os << "]";
    os << "]" << std::endl;
    return os;
}

#endif //MONOGON_MATRIX_H
