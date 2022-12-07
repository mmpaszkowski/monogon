//
// Created by noname on 12.11.22.
//

#ifndef MATH_SHAPE_H
#define MATH_SHAPE_H

//------------------------------------------------- Class Definition ---------------------------------------------------

#include <cstddef>
#include <ostream>
#include <vector>

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
    Shape(std::vector<value_type> data);
    Shape(const Shape & shape);

  public:
    reference operator[](value_type i);
    const_reference operator[](value_type i) const;
    reference operator()(long long i);
    const_reference operator()(long long i) const;

    bool operator==(const Shape &rhs) const;

  public:
    value_type size() const;

     iterator begin() noexcept {    return data.begin();    }
     const_iterator begin() const noexcept {    return data.begin();    }
     iterator end() noexcept {    return data.end();    }
     const_iterator end() const noexcept {    return data.end();    }

     reverse_iterator rbegin() noexcept {    return data.rbegin();    }
     const_reverse_iterator rbegin() const noexcept {    return data.rbegin();    }
     reverse_iterator rend() noexcept {    return data.rend();    }
     const_reverse_iterator rend() const noexcept{    return data.rend();    }

  private:
    std::vector<value_type> data;

  public:
    friend std::ostream &operator<<(std::ostream &os, const Shape &variable);
};

#endif //MATH_SHAPE_H
