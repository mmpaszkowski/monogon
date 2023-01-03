//
// Created by Mateusz Paszkowski on 12.11.22.
//

#include <monogon/tool/Index.h>
//--------------------------------------------------- Constructors -----------------------------------------------------

Shape::Shape() : data()
{
}

Shape::Shape(const std::vector<value_type> &d) : data(d)
{
}

Shape::Shape(std::vector<value_type> &&d) noexcept : data(std::move(d))
{
}

Shape::Shape(const Shape &shape) = default;

Shape::Shape(Shape &&shape) noexcept : data(std::move(shape.data))
{
}

Shape::~Shape() = default;

//---------------------------------------------------- Operators -------------------------------------------------------


Shape &Shape::operator=(const Shape &rhs) = default;

Shape &Shape::operator=(Shape &&rhs) noexcept
{
    this->data = std::move(rhs.data);
    return *this;
}

Shape::value_type &Shape::operator[](value_type i)
{
    return data[i];
}

Shape::const_reference Shape::operator[](value_type i) const
{
    return data[i];
}

Shape::value_type &Shape::operator()(long long i)
{
    if (i < 0)
        return data[data.size() + static_cast<size_type>(i)];
    else
        return data[static_cast<size_type>(i)];
}

Shape::const_reference Shape::operator()(long long i) const
{
    if (i < 0)
        return data[data.size() + static_cast<size_type>(i)];
    else
        return data[static_cast<size_type>(i)];
}

bool Shape::operator==(const Shape &rhs) const
{
    return this->data == rhs.data;
}

//----------------------------------------------------- Methods --------------------------------------------------------

Shape::value_type Shape::size() const
{
    return data.size();
}

Index Shape::last_index() const
{
    // #Todo Index
    return Index({1, 2, 3});
}


Shape::iterator Shape::begin() noexcept
{
    return data.begin();
}

Shape::const_iterator Shape::begin() const noexcept
{
    return data.begin();
}

Shape::iterator Shape::end() noexcept
{
    return data.end();
}

Shape::const_iterator Shape::end() const noexcept
{
    return data.end();
}

Shape::reverse_iterator Shape::rbegin() noexcept
{
    return data.rbegin();
}

Shape::const_reverse_iterator Shape::rbegin() const noexcept
{
    return data.rbegin();
}

Shape::reverse_iterator Shape::rend() noexcept
{
    return data.rend();
}

Shape::const_reverse_iterator Shape::rend() const noexcept
{
    return data.rend();
}

std::ostream &operator<<(std::ostream &os, const Shape &shape)
{
    os << "{";
    for (Shape::size_type i = 0; i < shape.size() - 1; i++)
    {
        os << shape[i] << ", ";
    }
    os << shape.data.back() << "}";

    return os;
}
