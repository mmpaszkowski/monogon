//
// Created by noname on 12.11.22.
//

#ifndef MATH_INDEX_H
#define MATH_INDEX_H

//------------------------------------------------- Class Definition ---------------------------------------------------
#include <optional>
#include "Shape.h"

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
    Index(std::vector<value_type> data);
    Index(Shape data);
    Index(const Index& data);

  public:
    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    reference operator()(long long i);
    const_reference operator()(long long i) const;

    Index &operator++();

    bool operator<(const Index &index);

  public:
    Index & increment(long long axis);

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


#endif //MATH_INDEX_H
