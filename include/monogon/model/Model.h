//
// Created by Mateusz Paszkowski on 21.10.22.
//

#ifndef MONOGON_MODEL_H
#define MONOGON_MODEL_H

#include "../activation/ActivationFunction.h"
#include "../layer/Layer.h"
#include "../loss/Loss.h"
#include "../metric/CategoricalAccuracy.h"
#include "../optimizer/Optimizer.h"
#include "../tool/Slice.h"
#include "../view/ModelRenderer.h"
#include <indicators/progress_bar.hpp>
#include <iostream>
using namespace indicators;

template <typename T = double>
class Model
{
public:
    Model(T alpha = 0.0, T threshold = 0.0, std::optional<T> max_value = std::optional<T>());
    Model(const Model<T> &model) = default;
    Model(Model<T> &&model) noexcept = default;

    Model &operator=(const Model<T> &model) = default;
    Model &operator=(Model<T> &&model) noexcept = default;

    template <template <typename> typename Layer1, template <typename> typename Layer2>
    Model(const Layer1<T> &begin, const Layer2<T> &end);

    virtual ~Model() = default;


    template <template <typename> typename O, template <typename> typename L>
    void compile(const O<T> &opt, const L<T> &l);
    virtual void fit(const Array<T> &x, const Array<T> &y, std::size_t epochs = 10, std::size_t batch_size = 32);
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
Model<T>::Model(const L1<T> &b, const L2<T> &e)
    : begin(std::make_shared<L1<T>>(b)), end(std::make_shared<L2<T>>(e))
{
}

template <typename T>
template <template <typename> typename O, template <typename> typename L>
void Model<T>::compile(const O<T> &opt, const L<T> &l)
{
    this->optimizer = std::make_shared<O<T>>(opt);
    this->loss = std::make_shared<L<T>>(l);
}

template <typename T>
void Model<T>::fit(const Array<T> &x, const Array<T> &y, size_t epochs, std::size_t batch_size)
{
    Slice slicer;

    for (size_t i = 0; i < epochs; i++)
    {
        T total_loss = 0.0;
        T total_accuracy = 0.0;
        size_t total_batches = 0;
        ModelRenderer modelRenderer;
        CategoricalAccuracy<T> categoricalAccuracy;
        modelRenderer.render_epoch(i + 1, epochs);
        for (size_t j = 0; j < x.get_shape()(-2); j += batch_size)
        {
            Array sub_x = slicer(x, j, j + batch_size);
            Array sub_y = slicer(y, j, j + batch_size);
            Variable y_pred = begin->feed_forward(Variable(std::move(sub_x)));
            Variable loss_val = (*this->loss)(y_pred, Variable(sub_y));
            loss_val.back_propagation();
            total_loss += loss_val.get_value();
            total_accuracy += categoricalAccuracy(y_pred.get_value(), sub_y);
            total_batches++;
            begin->update_weights_chain(*optimizer);
            loss_val.zero_grad();
            modelRenderer.render_progress_bar(j / batch_size + 1,
                                              x.get_shape()(-2) / batch_size,
                                              total_loss / static_cast<T>(total_batches),
                                              total_accuracy / static_cast<T>(total_batches));
        }
        modelRenderer.finish();
    }
}

template <typename T>
Array<T> Model<T>::predict(Array<T> x)
{
    return begin->feed_forward(std::move(Variable(x))).get_value();
}

#endif //MONOGON_MODEL_H
