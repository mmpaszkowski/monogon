//
// Created by noname on 21.10.22.
//

#ifndef MATH_DENSE_H
#define MATH_DENSE_H

#include "../Array.h"
#include "../initializer/Initializer.h"
#include "Layer.h"

#include <utility>
#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class DenseNode : public LayerNode<T>
{
  public:
    DenseNode(std::vector<size_t> shape,
              std::shared_ptr<LayerNode<T>> layer,
              std::shared_ptr<Initializer<T>> initializer);
    DenseNode(const DenseNode<T> &layer);

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
DenseNode<T>::DenseNode(std::vector<size_t> shape,
                        std::shared_ptr<LayerNode<T>> layer,
                        std::shared_ptr<Initializer<T>> initializer)
    : shape(shape), initializer(initializer), weights(initializer->operator()(layer->get_shape()[0], shape[0])),
      bias(Array<T>(1, this->shape[0], 0.0))
{
    assert(this->shape.size() == 1 && layer->get_shape().size() == 1);
}

template <typename T>
DenseNode<T>::DenseNode(const DenseNode<T> &layer)
    : next_layers(layer.next_layers), shape(layer.shape), weights(layer.weights), bias(layer.bias)
{
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T> void DenseNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T> std::vector<size_t> DenseNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T> Variable<Array<T>> DenseNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X.dot(this->weights) + this->bias);
    }
    return X.dot(this->weights) + this->bias;
}

template <typename T> void DenseNode<T>::update_weights(const Optimizer<T> &optimizer)
{
    optimizer.minimize(weights);
    optimizer.minimize(bias);
}

template <typename T> void DenseNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    update_weights(optimizer);
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class Dense : public Layer<T>
{
  public:
    Dense(std::vector<size_t> shape, Layer<T> &layer);
    Dense(const Dense &dense);

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

template <typename T> Dense<T>::Dense(const Dense<T> &dense) : node(dense.node)
{
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T> Variable<Array<T>> Dense<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T> std::shared_ptr<LayerNode<T>> Dense<T>::get_node() const
{
    return this->node;
}

template <typename T> void Dense<T>::update_weights(const Optimizer<T> &optimizer)
{
    this->node->update_weights(optimizer);
}
template <typename T> void Dense<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    this->node->update_weights_chain(optimizer);
}

#endif //MATH_DENSE_H
