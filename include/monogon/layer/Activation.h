//
// Created by noname on 21.10.22.
//

#ifndef MATH_ACTIVATION_H
#define MATH_ACTIVATION_H

#include "../Array.h"
#include "../activation/ActivationFunction.h"
#include "Layer.h"

#include <vector>

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T> class ActivationNode : public LayerNode<T>
{
  public:
    ActivationNode(std::vector<size_t> shape, std::shared_ptr<LayerNode<T>> layer);
    ActivationNode(std::shared_ptr<ActivationFunction<T>> activation_function, std::shared_ptr<LayerNode<T>> layer);
    ActivationNode(const ActivationNode<T> &layer);

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
ActivationNode<T>::ActivationNode(std::vector<size_t> shape, std::shared_ptr<LayerNode<T>> layer)
    : shape(layer->get_shape())
{
    assert(this->shape.size() == 1 && layer->get_shape().size() == 1);
}

template <typename T>
ActivationNode<T>::ActivationNode(std::shared_ptr<ActivationFunction<T>> activation_function,
                                  std::shared_ptr<LayerNode<T>> layer)
    : shape(layer->get_shape()), activation_function(activation_function)
{
    assert(this->shape.size() == 1 && layer->get_shape().size() == 1);
}

template <typename T>
ActivationNode<T>::ActivationNode(const ActivationNode<T> &layer) : next_layers(layer.next_layers), shape(layer.shape)
{
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T> void ActivationNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T> std::vector<size_t> ActivationNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T> Variable<Array<T>> ActivationNode<T>::feed_forward(const Variable<Array<T>> &X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(this->activation_function->operator()(X));
    }
    return this->activation_function->operator()(X);
}

template <typename T> void ActivationNode<T>::update_weights(const Optimizer<T> &optimizer)
{
}

template <typename T> void ActivationNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    for (auto &&next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T = double> class Activation : public Layer<T>
{
  public:
    template <template <typename> typename A> Activation(const A<T> &activation_function, Layer<T> &layer);

    Activation(std::vector<size_t> shape, Layer<T> &layer);
    Activation(const Activation &dense);

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

template <typename T>
Activation<T>::Activation(std::vector<size_t> shape, Layer<T> &layer)
    : node(std::make_shared<ActivationNode<T>>(std::make_shared<ReLu<T>>(), layer.get_node()))
{
    layer.get_node()->add_next_layer(this->node);
}

template <typename T> Activation<T>::Activation(const Activation<T> &activation) : node(activation.node)
{
}

//---------------------------------------------------- Interface -------------------------------------------------------

template <typename T> Variable<Array<T>> Activation<T>::feed_forward(const Variable<Array<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T> std::shared_ptr<LayerNode<T>> Activation<T>::get_node() const
{
    return this->node;
}

template <typename T> void Activation<T>::update_weights(const Optimizer<T> &optimizer)
{
    this->node->update_weights(optimizer);
}

template <typename T> void Activation<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    this->node->update_weights_chain(optimizer);
}

#endif //MATH_ACTIVATION_H
