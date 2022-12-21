//
// Created by noname on 21.10.22.
//

#ifndef MATH_MODEL_H
#define MATH_MODEL_H

#include "../loss/Loss.h"
#include "../optimizer/Optimizer.h"
#include "../layer/Layer.h"
#include "../activation/ActivationFunction.h"
#include "../tool/Slice.h"
#include "../view/ModelRenderer.h"
#include "../metric/CategoricalAccuracy.h"
#include <indicators/progress_bar.hpp>
#include <iostream>
using namespace indicators;

template <typename T = double> class Model
{
  public:
    template <template <typename> typename Layer1, template <typename> typename Layer2>
    Model(const Layer1<T> &begin, const Layer2<T> &end);

    template <template <typename> typename O, template <typename> typename L>
    void compile(const O<T> &optimizer, const L<T> &loss);
    virtual void fit(const Array<T>& x, const Array<T>& y, std::size_t epochs = 10, std::size_t batch_size=32);
    virtual Array<T> predict(Array<T> x);

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

template <typename T> void Model<T>::fit(const Array<T>& x, const Array<T>& y, size_t epochs, std::size_t batch_size)
{
    Slice slicer;

    for (size_t i = 0; i < epochs; i++)
    {
        T total_loss = 0.0;
        T total_accuracy = 0.0;
        size_t total_batches = 0;
        ModelRenderer modelRenderer;
        CategoricalAccuracy<T> categoricalAccuracy;
        modelRenderer.render_epoch(i+1, epochs);
        for(size_t j = 0; j < x.get_rows(); j+=batch_size)
        {
            Array sub_x = slicer(x, j, j+batch_size);
            Array sub_y = slicer(y, j, j+batch_size);
            Variable y_pred = begin->feed_forward(sub_x);
            Variable loss = (*this->loss)(y_pred, sub_y);
            loss.back_propagation();
            total_loss += loss.get_value();
            total_accuracy += categoricalAccuracy(y_pred.get_value(), sub_y);
            total_batches++;
            begin->update_weights_chain(*optimizer);
            loss.zero_grad();
            auto finish = std::chrono::high_resolution_clock::now();
            modelRenderer.render_progress_bar(j / batch_size + 1, x.get_rows()/batch_size, total_loss/static_cast<T>(total_batches), total_accuracy/static_cast<T>(total_batches));
        }
//        modelRenderer.finish();
    }
}

template <typename T>
Array<T> Model<T>::predict(Array<T> x)
{
    return begin->feed_forward(x).get_value();
}

#endif //MATH_MODEL_H
