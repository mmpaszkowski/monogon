//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_INPUT_H
#define MONOGON_INPUT_H

#include "Layer.h"
#include <vector>

template <typename T = double>
class InputNode : public LayerNode<T>
{
public:
    InputNode(std::vector<size_t> shape); //#TODO multidimensional __shape

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
InputNode<T>::InputNode(std::vector<size_t> s)
{
    this->shape.insert(this->shape.end(), std::make_move_iterator(s.begin()), std::make_move_iterator(s.end()));
}

template <typename T>
void InputNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T>
std::vector<size_t> InputNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T>
Variable<Array<T>> InputNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X);
    }
    return X;
}

template <typename T>
void InputNode<T>::update_weights([[maybe_unused]] const Optimizer<T> &optimizer)
{
}
template <typename T>
void InputNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

template <typename T = double>
class Input : public Layer<T>
{
public:
    Input(std::vector<size_t> shape);
    Input(const Input &input) = default;
    Input(Input &&input) noexcept = default;
    ~Input() = default;

    Input &operator=(const Input &) = default;
    Input &operator=(Input &&) noexcept = default;

    std::shared_ptr<LayerNode<T>> get_node() const override;

public:
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::shared_ptr<InputNode<T>> node;
};

template <typename T>
Input<T>::Input(std::vector<size_t> shape) : node(std::make_shared<InputNode<T>>(shape))
{
}

template <typename T>
Variable<Array<T>> Input<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T>
std::shared_ptr<LayerNode<T>> Input<T>::get_node() const
{
    return this->node;
}

template <typename T>
void Input<T>::update_weights(const Optimizer<T> &optimizer)
{
    node->update_weights(optimizer);
}

template <typename T>
void Input<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    node->update_weights_chain(optimizer);
}

#endif //MONOGON_INPUT_H
