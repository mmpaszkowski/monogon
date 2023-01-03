//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_ACTIVATION_H
#define MONOGON_ACTIVATION_H

#include "../Array.h"
#include "../activation/ActivationFunction.h"
#include "Layer.h"

#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T>
class ActivationNode : public LayerNode<T>
{
public:
    ActivationNode(std::shared_ptr<ActivationFunction<T>> activation_function, std::shared_ptr<LayerNode<T>> layer);
    ActivationNode(const ActivationNode<T> &activationNode) = default;
    ActivationNode(ActivationNode<T> &&activationNode) noexcept = default;

    ActivationNode &operator=(const ActivationNode<T> &activationNode) = default;
    ActivationNode &operator=(ActivationNode<T> &&activationNode) noexcept = default;

    ~ActivationNode() = default;

public:
    void add_next_layer(std::shared_ptr<LayerNode<T>> layer) override;
    std::vector<size_t> get_shape() const override;
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::vector<std::shared_ptr<LayerNode<T>>> next_layers;
    std::vector<size_t> shape;
    std::shared_ptr<ActivationFunction<T>> activation_function;
};

//--------------------------------------------------- Constructors -----------------------------------------------------


template <typename T>
ActivationNode<T>::ActivationNode(std::shared_ptr<ActivationFunction<T>> activation_function,
                                  std::shared_ptr<LayerNode<T>> layer)
    : shape(layer->get_shape()), activation_function(activation_function)
{
    assert(this->shape.size() == 1 && layer->get_shape().size() == 1);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
void ActivationNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T>
std::vector<size_t> ActivationNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T>
Variable<Array<T>> ActivationNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(this->activation_function->operator()(X));
    }
    return this->activation_function->operator()(X);
}

template <typename T>
void ActivationNode<T>::update_weights([[maybe_unused]] const Optimizer<T> &optimizer)
{
}

template <typename T>
void ActivationNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double>
class Activation : public Layer<T>
{
public:
    template <template <typename> typename A>
    Activation(const A<T> &activation_function, Layer<T> &layer);

    Activation(const Activation<T> &activation) = default;
    Activation(Activation<T> &&activation) noexcept = default;

    Activation &operator=(const Activation<T> &activation) = default;
    Activation &operator=(Activation<T> &&activation) noexcept = default;

    ~Activation() = default;

public:
    Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) override;
    std::shared_ptr<LayerNode<T>> get_node() const override;
    void update_weights(const Optimizer<T> &optimizer) override;
    void update_weights_chain(const Optimizer<T> &optimizer) override;

private:
    std::shared_ptr<ActivationNode<T>> node;
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
template <template <typename> typename A>
Activation<T>::Activation(const A<T> &activation_function, Layer<T> &layer)
    : node(std::make_shared<ActivationNode<T>>(std::make_shared<A<T>>(activation_function), layer.get_node()))
{
    layer.get_node()->add_next_layer(this->node);
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T>
Variable<Array<T>> Activation<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T>
std::shared_ptr<LayerNode<T>> Activation<T>::get_node() const
{
    return this->node;
}

template <typename T>
void Activation<T>::update_weights(const Optimizer<T> &optimizer)
{
    this->node->update_weights(optimizer);
}

template <typename T>
void Activation<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    this->node->update_weights_chain(optimizer);
}

#endif //MONOGON_ACTIVATION_H
