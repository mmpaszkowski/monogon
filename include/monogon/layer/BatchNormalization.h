//
// Created by Mateusz Paszkowski on 16.01.23.
//

#ifndef MONOGON_BATCH_NORMALIZATION_H
#define MONOGON_BATCH_NORMALIZATION_H

#include "../Array.h"
#include "../initializer/Initializer.h"
#include "Layer.h"

#include <utility>
#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class BatchNormalizationNode : public LayerNode<T>
{
public:
    BatchNormalizationNode(std::vector<size_t> s,
              std::shared_ptr<LayerNode<T>> l,
              std::shared_ptr<Initializer<T>> init);
    BatchNormalizationNode(const BatchNormalizationNode<T> &layer) = delete;
    BatchNormalizationNode(BatchNormalizationNode<T> &&layer) noexcept = delete;
    virtual ~BatchNormalizationNode() = default;

    BatchNormalizationNode &operator=(const BatchNormalizationNode &) = delete;
    BatchNormalizationNode &operator=(BatchNormalizationNode &&) noexcept = delete;

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
BatchNormalizationNode<T>::BatchNormalizationNode(std::vector<size_t> s,
                        std::shared_ptr<LayerNode<T>> l,
                        std::shared_ptr<Initializer<T>> init)
    : shape(s), initializer(init), weights(init->operator()(l->get_shape()[0], s[0])),
      bias(Array<T>(Shape({1, this->shape[0]}), 0.0))
{
    assert(this->shape.size() == 1 && l->get_shape().size() == 1);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
void BatchNormalizationNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T>
std::vector<size_t> BatchNormalizationNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T>
Variable<Array<T>> BatchNormalizationNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X.dot(this->weights) + this->bias);
    }
    return X.dot(this->weights) + this->bias;
}

template <typename T>
void BatchNormalizationNode<T>::update_weights(const Optimizer<T> &optimizer)
{
    optimizer.minimize(weights);
    optimizer.minimize(bias);
}

template <typename T>
void BatchNormalizationNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    update_weights(optimizer);
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class BatchNormalization : public Layer<T>
{
public:
    BatchNormalization(std::vector<size_t> shape, Layer<T> &layer);
    BatchNormalization(const BatchNormalization &batch_normalization) = default;
    BatchNormalization(BatchNormalization &&batch_normalization) noexcept = default;
    ~BatchNormalization() = default;

    BatchNormalization &operator=(const BatchNormalization &) = default;
    BatchNormalization &operator=(BatchNormalization &&) noexcept = default;

public:
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    std::shared_ptr<LayerNode<T>> get_node() const override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::shared_ptr<BatchNormalizationNode<T>> node;
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
BatchNormalization<T>::BatchNormalization(std::vector<size_t> shape, Layer<T> &layer)
    : node(std::make_shared<BatchNormalizationNode<T>>(shape, layer.get_node(), std::make_shared<GlorotUniform<T>>()))
{
    layer.get_node()->add_next_layer(this->node);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
Variable<Array<T>> BatchNormalization<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T>
std::shared_ptr<LayerNode<T>> BatchNormalization<T>::get_node() const
{
    return this->node;
}

template <typename T>
void BatchNormalization<T>::update_weights(const Optimizer<T> &optimizer)
{
    this->node->update_weights(optimizer);
}
template <typename T>
void BatchNormalization<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    this->node->update_weights_chain(optimizer);
}

#endif //MONOGON_BATCH_NORMALIZATION_H
