//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_LAYER_H
#define MONOGON_LAYER_H

#include <vector>
#include "../Array.h"
#include "../Variable.h"

template <typename T>
class Optimizer;

template <typename T = double>
class LayerNode
{
public:
    LayerNode() = default;
    LayerNode(const LayerNode<T> &layer_node) = delete;
    LayerNode(LayerNode<T> &&layer_node) noexcept = delete;

    LayerNode &operator=(const LayerNode<T> &layer_node) = delete;
    LayerNode &operator=(LayerNode<T> &&layer_node) noexcept = delete;
    virtual ~LayerNode() = default;
public:
    virtual void add_next_layer(std::shared_ptr<LayerNode<T>> layer) = 0;
    virtual Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) = 0;
    virtual std::vector<size_t> get_shape() const = 0;
    virtual void update_weights(const Optimizer<T> &optimizer) = 0;
    virtual void update_weights_chain(const Optimizer<T> &optimizer) = 0;
};

template <typename T = double>
class Layer
{
public:
    Layer() = default;
    Layer(const Layer<T> &layer) = default;
    Layer(Layer<T> &&layer) noexcept = default;

    Layer &operator=(const Layer<T> &layer) = default;
    Layer &operator=(Layer<T> &&layer) noexcept = default;
    virtual ~Layer() = default;
public:
    virtual Variable<Array<T>> feed_forward(const Variable<Array<T>> &X) = 0;
    virtual std::shared_ptr<LayerNode<T>> get_node() const = 0;
    virtual void update_weights(const Optimizer<T> &optimizer) = 0;
    virtual void update_weights_chain(const Optimizer<T> &optimizer) = 0;
};


#endif //MONOGON_LAYER_H
