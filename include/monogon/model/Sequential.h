//
// Created by noname on 21.10.22.
//

#ifndef MATH_SEQUENTIAL_H
#define MATH_SEQUENTIAL_H

#include "Model.h"
#include "math/layer/Input.h"
#include "math/layer/Layer.h"
#include <memory>
#include <vector>
template <typename T> class Sequential : public Model<T>
{
  public:
    void compile(Loss<T> loss, Optimizer<T> optimizer) override;
    void fit(Matrix<T> training_data, Matrix<T> target_data, size_t epochs = 1) override;
    void predict(Matrix<T> training_data) override;

    void add(Input<T> layer);
    void add(LayerNode<T> layer);

  public:
    //    Loss<T> loss;
    //    Optimizer<T> optimizer;

    std::vector<std::shared_ptr<LayerNode<T>>> layers;
};

template <typename T> void Sequential<T>::compile(Loss<T> loss, Optimizer<T> optimizer)
{
    this->loss = loss;
    this->optimizer = optimizer;
}

template <typename T> void Sequential<T>::fit(Matrix<T> training_data, Matrix<T> target_data, size_t epochs)
{
}

template <typename T> void Sequential<T>::predict(Matrix<T> training_data)
{
}

template <typename T> void Sequential<T>::add(Input<T> input)
{
    assert(this->layers.size() == 0);
    this->layers.template emplace_back(input);
}

template <typename T> void Sequential<T>::add(LayerNode<T> layer)
{
    assert(this->layers.size() > 0);
    std::shared_ptr<LayerNode<T>> next_layer = std::make_shared<LayerNode<T>>(layer);

    this->layers.back()->add(next_layer);
    this->layers.push_back(next_layer);
}

#endif //MATH_SEQUENTIAL_H
