//
// Created by Mateusz Paszkowski on 12.11.22.
//

#ifndef MONOGON_SHAPE_H
#define MONOGON_SHAPE_H


#include <cstddef>
#include <ostream>
#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

class Index;

class Shape
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
    Shape();
    Shape(const std::vector<value_type> &data);
    Shape(std::vector<value_type> &&data) noexcept;
    Shape(const Shape &shape);
    Shape(Shape &&shape) noexcept;
    ~Shape();
    Shape &operator=(const Shape &rhs);
    Shape &operator=(Shape &&rhs) noexcept;

public:
    reference operator[](value_type i);
    const_reference operator[](value_type i) const;
    reference operator()(long long i);
    const_reference operator()(long long i) const;

    bool operator==(const Shape &rhs) const;

public:
    value_type size() const;
    Index last_index() const;
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

public:
    friend std::ostream &operator<<(std::ostream &os, const Shape &variable);
};

#endif //MONOGON_SHAPE_H
