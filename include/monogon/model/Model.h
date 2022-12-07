//
// Created by noname on 21.10.22.
//

#ifndef MATH_MODEL_H
#define MATH_MODEL_H

#include "../loss/Loss.h"
#include "../optimizer/Optimizer.h"
#include "../layer/Layer.h"
#include "../activation/ActivationFunction.h"
#include <chrono>
#include <iostream>

template <typename T = double> class Model
{
  public:
    template <template <typename> typename Layer1, template <typename> typename Layer2>
    Model(const Layer1<T> &begin, const Layer2<T> &end);

    template <template <typename> typename O, template <typename> typename L>
    void compile(const O<T> &optimizer, const L<T> &loss);
    virtual void fit(Matrix<T> x, Matrix<T> y, std::size_t epochs = 10);
    virtual Matrix<T> predict(Matrix<T> x);

  private:
    std::shared_ptr<Layer<T>> begin;
    std::shared_ptr<Layer<T>> end;

    std::shared_ptr<Optimizer<T>> optimizer;
    std::shared_ptr<Loss<T>> loss;
    std::shared_ptr<ActivationFunction<T>> activation_function;
};

template <typename T>
template <template <typename> typename L1, template <typename> typename L2>
Model<T>::Model(const L1<T> &begin, const L2<T> &end)
    : begin(std::make_shared<L1<T>>(begin)), end(std::make_shared<L2<T>>(end))
{
}

template <typename T>
template <template <typename> typename O, template <typename> typename L>
void Model<T>::compile(const O<T> &optimizer, const L<T> &loss)
{
    this->optimizer = std::make_shared<O<T>>(optimizer);
    this->loss = std::make_shared<L<T>>(loss);
}

template <typename T> void Model<T>::fit(Matrix<T> x, Matrix<T> y, size_t epochs)
{
    for (size_t i = 0; i < epochs; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        Variable loss = (*this->loss)(begin->feed_forward(x), y);
        loss.back_propagation();
        begin->update_weights_chain(*optimizer);
        loss.zero_grad();
        auto finish = std::chrono::high_resolution_clock::now();

        std::cout << "Epoch " << i + 1 << "/" << epochs << " - "
                  << std::chrono::duration_cast<std::chrono::seconds>((finish - start)) % 1000 << " "
                  << std::chrono::duration_cast<std::chrono::milliseconds>((finish - start)) % 1000 << " "
                  << std::chrono::duration_cast<std::chrono::microseconds>((finish - start)) % 1000
                  << " / step - loss: " << loss.get_value() << std::endl;
    }
}

template <typename T> Matrix<T> Model<T>::predict(Matrix<T> x)
{
    return begin->feed_forward(x).get_value();
}

#endif //MATH_MODEL_H
