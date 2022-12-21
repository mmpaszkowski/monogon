//
// Created by noname on 21.10.22.
//

#ifndef MATH_LAYER_H
#define MATH_LAYER_H

#include <vector>

template <typename T = double> class LayerNode
{
  public:
    virtual void add_next_layer(std::shared_ptr<LayerNode<T>> layer) = 0;
    virtual Variable<Array<T>> feed_forward(const Variable<Array<T>>& X) = 0;
    virtual std::vector<size_t> get_shape() const = 0;
    virtual void update_weights(const Optimizer<T>& optimizer) = 0;
    virtual void update_weights_chain(const Optimizer<T>& optimizer) = 0;
};

template <typename T = double> class Layer
{
  public:
    virtual Variable<Array<T>> feed_forward(const Variable<Array<T>>& X) = 0;
    virtual std::shared_ptr<LayerNode<T>> get_node() const = 0;
    virtual void update_weights(const Optimizer<T>& optimizer) = 0;
    virtual void update_weights_chain(const Optimizer<T>& optimizer) = 0;
};


#endif //MATH_LAYER_H
