//
// Created by Mateusz Paszkowski on 05.01.23.
//

#ifndef MONOGON_CONV2D_H
#define MONOGON_CONV2D_H


#include "Layer.h"
#include <vector>

template <typename T = double>
class Conv2DNode : public LayerNode<T>
{
public:
    Conv2DNode(std::vector<size_t> shape); //#TODO multidimensional __shape
    Conv2DNode(const Conv2DNode&) = delete;
    Conv2DNode(Conv2DNode&&) noexcept = delete;
    Conv2DNode& operator=(const Conv2DNode&) = delete;
    Conv2DNode& operator=(Conv2DNode&&) noexcept = delete;
    virtual ~Conv2DNode() = default;
public:
    void add_next_layer(std::shared_ptr<LayerNode<T>> layer) override;
    std::vector<size_t> get_shape() const override;
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::vector<std::shared_ptr<LayerNode<T>>> next_layers;
    std::vector<size_t> shape;
};

template <typename T>
Conv2DNode<T>::Conv2DNode(std::vector<size_t> s)
{
    this->shape.insert(this->shape.end(), std::make_move_iterator(s.begin()), std::make_move_iterator(s.end()));
}

template <typename T>
void Conv2DNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T>
std::vector<size_t> Conv2DNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T>
Variable<Array<T>> Conv2DNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X);
    }
    return X;
}

template <typename T>
void Conv2DNode<T>::update_weights([[maybe_unused]] const Optimizer<T> &optimizer)
{
}
template <typename T>
void Conv2DNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

template <typename T = double>
class Conv2D : public Layer<T>
{
public:
    Conv2D(std::vector<size_t> shape);
    Conv2D(const Conv2D &conv2d) = default;
    Conv2D(Conv2D &&conv2d) noexcept = default;
    ~Conv2D() = default;

    Conv2D &operator=(const Conv2D &) = default;
    Conv2D &operator=(Conv2D &&) noexcept = default;

    std::shared_ptr<LayerNode<T>> get_node() const override;

public:
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::shared_ptr<Conv2D<T>> node;
};

template <typename T>
Conv2D<T>::Conv2D(std::vector<size_t> shape) : node(std::make_shared<Conv2DNode<T>>(shape))
{
}

template <typename T>
Variable<Array<T>> Conv2D<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T>
std::shared_ptr<LayerNode<T>> Conv2D<T>::get_node() const
{
    return this->node;
}

template <typename T>
void Conv2D<T>::update_weights(const Optimizer<T> &optimizer)
{
    node->update_weights(optimizer);
}

template <typename T>
void Conv2D<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    node->update_weights_chain(optimizer);
}

#endif //MONOGON_CONV2D_H
