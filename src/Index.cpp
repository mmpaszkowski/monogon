//
// Created by Mateusz Paszkowski on 12.11.22.
//

#include <cassert>
#include <monogon/tool/Index.h>

//--------------------------------------------------- Constructors -----------------------------------------------------

Index::Index(const std::vector<value_type> &d) : data(d), shape()
{
}

Index::Index(std::vector<value_type> &&d) noexcept : data(std::move(d)), shape()
{
}

Index::Index(Shape s) : data(s.size()), shape(s)
{
}

Index::Index(Shape s, const std::vector<value_type> &d): data(d), shape(s)
{
}

Index::Index(const Index &index) = default;

Index::Index(Index &&index) noexcept : data(std::move(index.data)), shape(std::move(index.shape))
{
}

Index::~Index() = default;

//---------------------------------------------------- Operators -------------------------------------------------------

Index &Index::operator=(const Index &rhs) = default;

Index &Index::operator=(Index &&rhs) noexcept
{
    this->data = std::move(rhs.data);
    this->shape = std::move(rhs.shape);
    return *this;
}

Index::value_type &Index::operator[](value_type i)
{
    return data[i];
}

Index::const_reference Index::operator[](value_type i) const
{
    return data[i];
}

Index::value_type &Index::operator()(long long i)
{
    if (i < 0)
        return data[data.size() + static_cast<size_type>(i)];
    else
        return data[static_cast<size_type>(i)];
}

Index::const_reference Index::operator()(long long i) const
{
    if (i < 0)
        return data[data.size() + static_cast<size_type>(i)];
    else
        return data[static_cast<size_type>(i)];
}

Index &Index::operator++()
{
    assert(shape);

    auto shape_it = shape->rbegin();
    for (auto index_it = data.rbegin(); index_it != data.rend(); ++index_it, ++shape_it)
    {
        (*index_it)++;

        if (*index_it >= *shape_it)
        {
            if (shape_it + 1 == shape->rend())
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
    return data < index.data;
}


Index Index::operator%(const Shape &s)
{
    std::vector<size_t> result;

    for (size_t i = 0; i < data.size(); i++)
    {
        result.push_back(data[i] % s[i]);
    }

    return result;
}

//----------------------------------------------------- Methods --------------------------------------------------------

Index::value_type Index::size() const
{
    return data.size();
}

Index::iterator Index::begin() noexcept
{
    return data.begin();
}

Index::const_iterator Index::begin() const noexcept
{
    return data.begin();
}

Index::iterator Index::end() noexcept
{
    return data.end();
}

Index::const_iterator Index::end() const noexcept
{
    return data.end();
}

Index::reverse_iterator Index::rbegin() noexcept
{
    return data.rbegin();
}

Index::const_reverse_iterator Index::rbegin() const noexcept
{
    return data.rbegin();
}

Index::reverse_iterator Index::rend() noexcept
{
    return data.rend();
}

Index::const_reverse_iterator Index::rend() const noexcept
{
    return data.rend();
}

std::ostream &operator<<(std::ostream &os, const Index &index)
{
    os << "{";
    for (Index::size_type i = 0; i < index.size() - 1; i++)
    {
        os << index[i] << ", ";
    }
    os << index.data.back() << "}";

    return os;
}

Index &Index::increment(long long index) // #Todo operator++
{
    assert(shape);
    size_type skip = (index < 0 ? this->size() + static_cast<size_type>(index) : static_cast<size_type>(index)) + 1;

    auto &&shape_it = shape->rbegin();
    for (auto &&index_it = data.rbegin(); index_it != data.rend(); ++index_it, ++shape_it)
    {
        if (skip > 0)
        {
            skip--;
            continue;
        }
        (*index_it)++;

        if (*index_it >= *shape_it)
        {
            if (shape_it + 1 == shape->rend())
                break;
            (*index_it) = 0;
        }
        else
            break;
    }
    return *this;
}
