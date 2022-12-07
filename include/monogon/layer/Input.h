//
// Created by noname on 21.10.22.
//

#ifndef MATH_INPUT_H
#define MATH_INPUT_H

#include "Layer.h"
#include <vector>

template <typename T = double> class InputNode : public LayerNode<T>
{
  public:
    InputNode(std::vector<size_t> shape); //#TODO multidimensional __shape

  public:
    void add_next_layer(std::shared_ptr<LayerNode<T>> layer) override;

    std::vector<size_t> get_shape() const override;

    Variable<Matrix<T>> feed_forward(const Variable<Matrix<T>>& X) override;

    void update_weights(const Optimizer<T>& optimizer) override;

    void update_weights_chain(const Optimizer<T>& optimizer) override;

  private:
    std::vector<std::shared_ptr<LayerNode<T>>> next_layers;
    std::vector<size_t> shape;

};

template <typename T> InputNode<T>::InputNode(std::vector<size_t> shape)
{
    this->shape.insert(this->shape.end(), std::make_move_iterator(shape.begin()), std::make_move_iterator(shape.end()));
}

template <typename T> void InputNode<T>::add_next_layer(std::shared_ptr<LayerNode<T>> layer)
{
    next_layers.push_back(layer);
}

template <typename T> std::vector<size_t> InputNode<T>::get_shape() const
{
    return this->shape;
}

template <typename T> Variable<Matrix<T>> InputNode<T>::feed_forward(const Variable<Matrix<T>>& X)
{
    for (auto &&next_layer : next_layers)
    {
        return next_layer.get()->feed_forward(X);
    }
    return X;
}

template <typename T> void InputNode<T>::update_weights(const Optimizer<T> &optimizer)
{

}
template <typename T> void InputNode<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    for(auto&& next_layer : next_layers)
        next_layer->update_weights_chain(optimizer);
}

template <typename T = double> class Input : public Layer<T>
{
  public:
    Input(std::vector<size_t> shape);
    Input(const Input& input): node(input.node) {}
    std::shared_ptr<LayerNode<T>> get_node() const override;

  public:
    Variable<Matrix<T>> feed_forward(const Variable<Matrix<T>>& X) override;
    void update_weights(const Optimizer<T>& optimizer) override;
    void update_weights_chain(const Optimizer<T>& optimizer) override;

  private:
    std::shared_ptr<InputNode<T>> node;
};

template <typename T> Input<T>::Input(std::vector<size_t> shape):
    node(std::make_shared<InputNode<T>>(shape))
{

}

template <typename T> Variable<Matrix<T>> Input<T>::feed_forward(const Variable<Matrix<T>> &X)
{
    return node->feed_forward(X);
}

template <typename T> std::shared_ptr<LayerNode<T>> Input<T>::get_node() const
{
    return this->node;
}

template <typename T> void Input<T>::update_weights(const Optimizer<T> &optimizer)
{
    node->update_weights(optimizer);
}

template <typename T> void Input<T>::update_weights_chain(const Optimizer<T> &optimizer)
{
    node->update_weights_chain(optimizer);
}

#endif //MATH_INPUT_H
