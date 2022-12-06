//
// Created by Mateusz Paszkowski on 22.07.22.
//

#ifndef MONOGON_VECTOR_H
#define MONOGON_VECTOR_H

#include <valarray>
#include <cassert>
#include <ostream>
//---------------------------------------------------- Class body ------------------------------------------------------
template <typename T> class Matrix;

template <typename T> class Vector
{
  public:
    using value_type = T;
    using size_type = std::size_t;

    Vector(size_t size);
    Vector(size_t size, const T &value);
    Vector(std::initializer_list<value_type> list);

    template <template <typename...> class Container> Vector(Container<value_type> container);

  public:
    value_type &operator()(std::size_t index);

    const value_type &operator()(std::size_t index) const;

    auto operator-() const;

    template <typename U> auto operator+(const Vector<U> &rhs) const;
    template <typename U> auto operator-(const Vector<U> &rhs) const;
    template <typename U> auto operator*(const Vector<U> &rhs) const;
    template <typename U> auto operator/(const Vector<U> &rhs) const;

    template <typename U> auto operator+(const Matrix<U> &rhs) const;
    template <typename U> auto operator-(const Matrix<U> &rhs) const;
    template <typename U> auto operator*(const Matrix<U> &rhs) const;
    template <typename U> auto operator/(const Matrix<U> &rhs) const;

    template <typename U> auto operator+(const U &val) const;
    template <typename U> auto operator-(const U &val) const;
    template <typename U> auto operator*(const U &val) const;
    template <typename U> auto operator/(const U &val) const;

    template <typename U> bool operator==(const Vector<U> &rhs) const;

    Matrix<T> transpose() const;

    template <typename U> auto dot(const Vector<U> &rhs) const;
    template <typename U> auto dot(const Matrix<U> &rhs) const;

    auto avg() const;

  public:
    const std::valarray<value_type> &get_data() const;

    size_type size() const;

  private:
    std::valarray<value_type> data;
  public:
    template <typename U> friend std::ostream& operator<<(std::ostream& os, const Vector<U>& variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------
template <typename T> Vector<T>::Vector(size_t size) : data(size)
{
}

template <typename T> Vector<T>::Vector(size_t size, const T &value) : data(size)
{
    for (auto &&item : data)
        item = value;
}

template <typename T> Vector<T>::Vector(std::initializer_list<T> list)
{
    this->data.resize(list.size());
    size_t i = 0;
    for (auto &&item : list)
        data[i++] = item;
}

template <typename T> template <template <typename...> class Container> Vector<T>::Vector(Container<T> container)
{
    this->data.resize(container.size());
    size_t i = 0;
    for (auto &&item : container)
        data[i++] = item;
}

//----------------------------------------------------- Operators ------------------------------------------------------

template <typename T> T &Vector<T>::operator()(std::size_t index)
{
    return data[index];
}

template <typename T> const T &Vector<T>::operator()(std::size_t index) const
{
    return data[index];
}

template <typename T> auto Vector<T>::operator-() const
{
    Vector<T> result(this->size());
    result.data = -this->data;
    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator+(const Vector<U> &rhs) const
{
    assert(this->size() == rhs.size());
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());

    Vector<result_val_type> result(this->size());
    result.data = this->data + rhs.data;

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator-(const Vector<U> &rhs) const
{
    assert(this->size() == rhs.size());
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Vector<result_val_type> result(this->size());
    result.data = this->data - rhs.data;

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator*(const Vector<U> &rhs) const
{
    assert(this->size() == rhs.size());
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Vector<result_val_type> result(this->size());
    result.data = this->data * rhs.data;

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator/(const Vector<U> &rhs) const
{
    assert(this->size() == rhs.size());
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Vector<result_val_type> result(this->size());
    result.data = this->data / rhs.data;

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator+(const Matrix<U> &rhs) const
{
    assert(this->size() == rhs.get_rows());
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());

    Matrix<result_val_type> result(rhs.get_rows(), rhs.get_columns());
    for (size_t i = 0; i < rhs.get_rows(); i++)
        for (size_t j = 0; j < rhs.get_columns(); j++)
            result(i, j) = data[i] + rhs(i, j);

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator-(const Matrix<U> &rhs) const
{
    assert(this->size() == rhs.get_rows());
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());

    Matrix<result_val_type> result(rhs.get_rows(), rhs.get_columns());
    for (size_t i = 0; i < rhs.get_rows(); i++)
        for (size_t j = 0; j < rhs.get_columns(); j++)
            result(i, j) = data[i] - rhs(i, j);

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator*(const Matrix<U> &rhs) const
{
    assert(this->size() == rhs.get_rows());
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());

    Matrix<result_val_type> result(rhs.get_rows(), rhs.get_columns());
    for (size_t i = 0; i < rhs.get_rows(); i++)
        for (size_t j = 0; j < rhs.get_columns(); j++)
            result(i, j) = data[i] * rhs(i, j);

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator/(const Matrix<U> &rhs) const
{
    assert(this->size() == rhs.get_rows());
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());

    Matrix<result_val_type> result(rhs.get_rows(), rhs.get_columns());
    for (size_t i = 0; i < rhs.get_rows(); i++)
        for (size_t j = 0; j < rhs.get_columns(); j++)
            result(i, j) = data[i] / rhs(i, j);

    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator+(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Vector<result_val_type> result(this->size());
    result.data = data + val;
    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator-(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() - std::declval<U>());
    Vector<result_val_type> result(this->size());
    result.data = data - val;
    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator*(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Vector<result_val_type> result(this->size());
    result.data = data * val;
    return result;
}

template <typename T> template <typename U> auto Vector<T>::operator/(const U &val) const
{
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());
    Vector<result_val_type> result(this->size());
    result.data = data / val;
    return result;
}

template <typename T> template <typename U> bool Vector<T>::operator==(const Vector<U> &rhs) const
{
    for (size_t i = 0; i < this->size(); ++i)
        if (this->operator()(i) != rhs(i))
            return false;
    return true;
}

//------------------------------------------------------ Methods -------------------------------------------------------

template <typename T> Matrix<T> Vector<T>::transpose() const
{
    Matrix<T> result(1, this->size());

    for (size_t i = 0; i < this->size(); i++)
        result(0, i) = this->data[i];
    return result;
}

template <typename T> template <typename U> auto Vector<T>::dot(const Vector<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<T>() * std::declval<U>());
    result_val_type result{};

    for (size_t i = 0; i < this->size(); ++i)
        result += data[i] * rhs(i);
    return result;
}

template <typename T> template <typename U> auto Vector<T>::dot(const Matrix<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Matrix<result_val_type> result(this->size(), rhs.get_columns());

    for (size_t i = 0; i < this->size(); ++i)
        for (size_t j = 0; j < rhs.get_columns(); ++j)
            result(i, j) = data[i] * rhs(0, j);

    return result;
}

template <typename T> auto Vector<T>::avg() const
{
    T sum {};
    for(const auto& item : data)
        sum += item;

    return sum/static_cast<T>(data.size());
}

//----------------------------------------------------- Accessors ------------------------------------------------------

template <typename T> const std::valarray<T> &Vector<T>::get_data() const
{
    return this->data;
}

template <typename T> std::size_t Vector<T>::size() const
{
    return this->data.size();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vector)
{
    os << "[";
    for(size_t i = 0; i < vector.size()-1; i++)
    {
        os << vector(i) << ",";
    }
    os << vector(vector.size()-1);
    os << "]" << std::endl;
    return os;
}


#endif //MONOGON_VECTOR_H
