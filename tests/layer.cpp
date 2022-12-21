//
// Created by noname on 21.10.22.
//
#include <gtest/gtest.h>

#include <monogon/Array.h>
#include <monogon/Variable.h>
#include <monogon/Vector.h>
#include <monogon/layer/Dense.h>
#include <monogon/layer/Input.h>
#include <monogon/loss/MSE.h>
#include <monogon/optimizer/SGD.h>

TEST(layer, layer)
{
    Variable<Array<double>> x = Array<double>{{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    Variable<Array<double>> y = Array<double> {{0.0}, {0.0}, {0.0}, {1.0}};

    MSE mse;
    SGD sgd;

    Input input({2});
    Dense dense1({3}, input);
    Dense dense2({1}, dense1);

    Variable loss = mse(input.feed_forward(x), y);
    loss.back_propagation();

    input.update_weights_chain(sgd);

    Variable y_pred = input.feed_forward(x).get_value();
//    Array expected_y_pred = {{2.6944750000000002}, {4.6402750000000008}, {4.6402750000000008}, {6.586075000000001}};

//    GTEST_ASSERT_EQ(y_pred.get_value() == expected_y_pred, true);
}

