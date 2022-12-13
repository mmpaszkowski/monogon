//
// Created by Mateusz Paszkowski on 12.11.22.
//

#include <monogon/tool/Shape.h>
#include <monogon/tool/Index.h>
//--------------------------------------------------- Constructors -----------------------------------------------------

Shape::Shape() : __data()
{
}

Shape::Shape(const std::vector<value_type> &data) : __data(data)
{
}

Shape::Shape(std::vector<value_type> &&data) noexcept : __data(std::move(data))
{
}

Shape::Shape(const Shape &shape) = default;

Shape::Shape(Shape &&shape) noexcept : __data(std::move(shape.__data))
{
}

Shape::~Shape() = default;

//---------------------------------------------------- Operators -------------------------------------------------------


Shape &Shape::operator=(const Shape &rhs) = default;

Shape &Shape::operator=(Shape &&rhs) noexcept
{
    this->__data = std::move(rhs.__data);
    return *this;
}

Shape::value_type &Shape::operator[](value_type i)
{
    return __data[i];
}

Shape::const_reference Shape::operator[](value_type i) const
{
    return __data[i];
}

Shape::value_type &Shape::operator()(long long i)
{
    if (i < 0)
        return __data[__data.size() + static_cast<size_type>(i)];
    else
        return __data[static_cast<size_type>(i)];
}

Shape::const_reference Shape::operator()(long long i) const
{
    if (i < 0)
        return __data[__data.size() + static_cast<size_type>(i)];
    else
        return __data[static_cast<size_type>(i)];
}

bool Shape::operator==(const Shape &rhs) const
{
    return this->__data == rhs.__data;
}

//----------------------------------------------------- Methods --------------------------------------------------------

Shape::value_type Shape::size() const
{
    return __data.size();
}

Index Shape::last_index() const
{
// #Todo Index
    return Index({1, 2, 3});
}


Shape::iterator Shape::begin() noexcept
{
    return __data.begin();
}

Shape::const_iterator Shape::begin() const noexcept
{
    return __data.begin();
}

Shape::iterator Shape::end() noexcept
{
    return __data.end();
}

Shape::const_iterator Shape::end() const noexcept
{
    return __data.end();
}

Shape::reverse_iterator Shape::rbegin() noexcept
{
    return __data.rbegin();
}

Shape::const_reverse_iterator Shape::rbegin() const noexcept
{
    return __data.rbegin();
}

Shape::reverse_iterator Shape::rend() noexcept
{
    return __data.rend();
}

Shape::const_reverse_iterator Shape::rend() const noexcept
{
    return __data.rend();
}

std::ostream &operator<<(std::ostream &os, const Shape &shape)
{
    os << "{";
    for (Shape::size_type i = 0; i < shape.size() - 1; i++)
    {
        os << shape[i] << ", ";
    }
    os << shape.__data.back() << "}";

    return os;
}
