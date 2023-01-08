//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_DENSE_H
#define MONOGON_DENSE_H

#include "../Array.h"
#include "../initializer/Initializer.h"
#include "Layer.h"

#include <utility>
#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class DenseNode : public LayerNode<T>
{
public:
    DenseNode(std::vector<size_t> s,
              std::shared_ptr<LayerNode<T>> l,
              std::shared_ptr<Initializer<T>> init);
    DenseNode(const DenseNode<T> &layer) = delete;
    DenseNode(DenseNode<T> &&layer) noexcept = delete;
    virtual ~DenseNode() = default;

    DenseNode &operator=(const DenseNode &) = delete;
    DenseNode &operator=(DenseNode &&) noexcept = delete;

public:
    void add_next_layer(std::shared_ptr<LayerNode<T>> layer) override;
    std::vector<size_t> get_shape() const override;
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::vector<std::shared_ptr<LayerNode<T>>> next_layers;
    std::vector<size_t> shape;
    std::shared_ptr<Initializer<T>> initializer;
    Variable<Array<T>> weights;
    Variable<Array<T>> bias;
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
DenseNode<T>::DenseNode(std::vector<size_t> s,
                        std::shared_ptr<LayerNode<T>> l,
                        std::shared_ptr<Initializer<T>> init)
    : shape(s), initializer(init), weights(init->operator()(l->get_shape()[0], s[0])),
      bias(Array<T>(Shape({1, this->shape[0]}), 0.0))
{
    assert(this->shape.size() == 1 && l->get_shape().size() == 1);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
void DenseNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T>
std::vector<size_t> DenseNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T>
Variable<Array<T>> DenseNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X.dot(this->weights) + this->bias);
    }
    return X.dot(this->weights) + this->bias;
}

template <typename T>
void DenseNode<T>::update_weights(const Optimizer<T> &optimizer)
{
    optimizer.minimize(weights);
    optimizer.minimize(bias);
}

template <typename T>
void DenseNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    update_weights(optimizer);
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class Dense : public Layer<T>
{
public:
    Dense(std::vector<size_t> shape, Layer<T> &layer);
    Dense(const Dense &dense) = default;
    Dense(Dense &&dense) noexcept = default;
    ~Dense() = default;

    Dense &operator=(const Dense &) = default;
    Dense &operator=(Dense &&) noexcept = default;

public:
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    std::shared_ptr<LayerNode<T>> get_node() const override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::shared_ptr<DenseNode<T>> node;
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
Dense<T>::Dense(std::vector<size_t> shape, Layer<T> &layer)
    : node(std::make_shared<DenseNode<T>>(shape, layer.get_node(), std::make_shared<GlorotUniform<T>>()))
{
    layer.get_node()->add_next_layer(this->node);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
Variable<Array<T>> Dense<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T>
std::shared_ptr<LayerNode<T>> Dense<T>::get_node() const
{
    return this->node;
}

template <typename T>
void Dense<T>::update_weights(const Optimizer<T> &optimizer)
{
    this->node->update_weights(optimizer);
}
template <typename T>
void Dense<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    this->node->update_weights_chain(optimizer);
}

#endif //MONOGON_DENSE_H
