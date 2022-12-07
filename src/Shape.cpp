//
// Created by noname on 12.11.22.
//

#include <monogon/tool/Shape.h>

Shape::Shape() : data()
{
}

Shape::Shape(std::vector<value_type> data) : data(data)
{
}

Shape::Shape(const Shape & shape): data(shape.data)
{

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
        return data[data.size() + i];
    else
        return data[i];
}

Shape::const_reference Shape::operator()(long long i) const
{
    if (i < 0)
        return data[data.size() + i];
    else
        return data[i];
}

bool Shape::operator==(const Shape &rhs) const
{
    return this->data == rhs.data;
}

Shape::value_type Shape::size() const
{
    return data.size();
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