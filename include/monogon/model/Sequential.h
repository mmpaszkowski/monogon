//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_SEQUENTIAL_H
#define MONOGON_SEQUENTIAL_H

#include "../layer/Input.h"
#include "../layer/Layer.h"
#include "Model.h"
#include <memory>
#include <vector>
template <typename T>
class Sequential : public Model<T>
{
public:
    void compile(Loss<T> loss, Optimizer<T> optimizer) override;

    void fit(const Array<T> &x, const Array<T> &y, std::size_t epochs = 10, std::size_t batch_size = 32) override;

    void predict(Array<T> training_data) override;

    void add(Input<T> layer);
    void add(LayerNode<T> layer);

public:
    //    Loss<T> loss;
    //    Optimizer<T> optimizer;

    std::vector<std::shared_ptr<LayerNode<T>>> layers;
};

template <typename T>
void Sequential<T>::compile(Loss<T> loss, Optimizer<T> optimizer)
{
    this->loss = loss;
    this->optimizer = optimizer;
}

template <typename T>
void Sequential<T>::fit(const Array<T> &x, const Array<T> &y, std::size_t epochs, std::size_t batch_size)
{
}


template <typename T>
void Sequential<T>::predict(Array<T> training_data)
{
}

template <typename T>
void Sequential<T>::add(Input<T> input)
{
    assert(this->layers.size() == 0);
    this->layers.template emplace_back(input);
}

template <typename T>
void Sequential<T>::add(LayerNode<T> layer)
{
    assert(this->layers.size() > 0);
    std::shared_ptr<LayerNode<T>> next_layer = std::make_shared<LayerNode<T>>(layer);

    this->layers.back()->add(next_layer);
    this->layers.push_back(next_layer);
}

#endif //MONOGON_SEQUENTIAL_H
