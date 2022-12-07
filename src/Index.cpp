//
// Created by noname on 12.11.22.
//

#include <monogon/tool/Index.h>
#include <monogon/tool/Shape.h>
#include <cassert>

Index::Index(std::vector<value_type> data) : __data(data)
{
}

Index::Index(Shape shape) : __data(shape.size()), __shape(shape)
{
}

Index::Index(const Index& index) : __data(index.__data), __shape(index.__shape)
{
}

Index::value_type &Index::operator[](value_type i)
{
    return __data[i];
}

Index::const_reference Index::operator[](value_type i) const
{
    return __data[i];
}

Index::value_type &Index::operator()(long long i)
{
    if (i < 0)
        return __data[__data.size() + i];
    else
        return __data[i];
}

Index::const_reference Index::operator()(long long i) const
{
    if (i < 0)
        return __data[__data.size() + i];
    else
        return __data[i];
}

Index &Index::operator++()
{
    assert(__shape);

    auto shape_it = __shape->rbegin();
    for (auto index_it = __data.rbegin(); index_it != __data.rend(); ++index_it, ++shape_it)
    {
        (*index_it)++;

        if (*index_it >= *shape_it)
        {
            if (shape_it + 1 == __shape->rend())
                break;
            (*index_it) = 0;
        }
        else
            break;
    }
    return *this;
}

bool Index::operator<(const Index &index)
{
    return __data < index.__data;
}

Index::value_type Index::size() const
{
    return __data.size();
}

 Index::iterator Index::begin() noexcept
{
    return __data.begin();
}
 Index::const_iterator Index::begin() const noexcept
{
    return __data.begin();
}
 Index::iterator Index::end() noexcept
{
    return __data.end();
}

 Index::const_iterator Index::end() const noexcept
{
    return __data.end();
}

 Index::reverse_iterator Index::rbegin() noexcept
{
    return __data.rbegin();
}
 Index::const_reverse_iterator Index::rbegin() const noexcept
{
    return __data.rbegin();
}
 Index::reverse_iterator Index::rend() noexcept
{
    return __data.rend();
}
 Index::const_reverse_iterator Index::rend() const noexcept
{
    return __data.rend();
}

std::ostream &operator<<(std::ostream &os, const Index &index)
{
    os << "{";
    for (Index::size_type i = 0; i < index.size() - 1; i++)
    {
        os << index[i] << ", ";
    }
    os << index.__data.back() << "}";

    return os;
}
Index& Index::increment(long long index)
{
    assert(__shape);

    size_t skip;
    if (index < 0)
        skip = this->size() + index + 1;
    else
        skip = index + 1;

    auto shape_it = __shape->rbegin();
    for (auto index_it = __data.rbegin(); index_it != __data.rend(); ++index_it, ++shape_it)
    {
        if (skip > 0)
        {
            skip--;
            continue;
        }
        (*index_it)++;

        if (*index_it >= *shape_it)
        {
            if (shape_it + 1 == __shape->rend())
                break;
            (*index_it) = 0;
        }
        else
            break;
    }
    return *this;
}