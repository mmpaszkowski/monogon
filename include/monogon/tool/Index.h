//
// Created by Mateusz Paszkowski on 12.11.22.
//

#ifndef MONOGON_INDEX_H
#define MONOGON_INDEX_H

#include "Shape.h"
#include <optional>

//------------------------------------------------- Class Definition ---------------------------------------------------

class Index
{
public:
    using value_type = size_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using iterator = std::vector<value_type>::iterator;
    using const_iterator = std::vector<value_type>::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    Index(const std::vector<value_type> &data);
    Index(std::vector<value_type> &&data) noexcept;
    Index(Shape shape);
    Index(const Index &index);
    Index(Index &&index) noexcept;
    ~Index();

    Index &operator=(const Index &rhs);
    Index &operator=(Index &&rhs) noexcept;

public:
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    reference operator()(long long i);
    const_reference operator()(long long i) const;

    Index &operator++();

    bool operator<(const Index &index);

    Index operator%(const Shape &shape);

public:
    Index &increment(long long axis);

public:
    value_type size() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;

private:
    std::vector<value_type> __data;
    std::optional<Shape> __shape;

public:
    friend std::ostream &operator<<(std::ostream &os, const Index &variable);
};

#endif //MONOGON_INDEX_H
