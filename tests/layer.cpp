//
// Created by noname on 21.10.22.
//
#include <gtest/gtest.h>

#include <monogon/Array.h>
#include <monogon/Variable.h>
#include <monogon/layer/Dense.h>
#include <monogon/layer/Input.h>
#include <monogon/layer/Activation.h>
#include <monogon/optimizer/SGD.h>

TEST(layer, input)
{
    SGD sgd;
    GlorotUniform glorotUniform;

    Input input({32ull});
    Input input_copy = input;
    Input input_moved = std::move(input_copy);
    input_copy = input;
    input_moved = std::move(input_copy);

    Variable X = glorotUniform(32, 32);
    Variable Y = input_moved.feed_forward(X);
    Y.back_propagation();
    Array x = X.get_value();
    Array y = Y.get_value();
    EXPECT_TRUE(x == y);
    input_moved.update_weights_chain(sgd);
    input_moved.update_weights(sgd);
}

TEST(layer, input_feed_forward)
{
    SGD sgd;
    Zeros zeros;
    Input input({4ull});

    Dense dense({4ull}, input);
    Dense dense_copy = dense;
    Dense dense_moved = std::move(dense_copy);
    dense_copy = dense;
    dense_moved = std::move(dense_copy);

    Variable X = zeros(2, 4);
    Variable Y = dense_moved.feed_forward(X);
    Y.back_propagation();
    Array x = X.get_value();
    Array y = Y.get_value();
    EXPECT_TRUE(x == zeros(2, 4).get_value());
    dense_moved.update_weights_chain(sgd);
    dense_moved.update_weights(sgd);
}

TEST(layer, activation_feed_forward)
{
    SGD sgd;
    Zeros zeros;
    Input input({4ull});
    ReLu reLu;

    Activation activation(reLu, input);
    Activation activation_copy = activation;
    Activation activation_moved = std::move(activation_copy);
    activation_copy = activation;
    activation_moved = std::move(activation_copy);

    Variable X = Variable(Array<double>({{-4.0, -3.0, -2.0, -1.0}, {0.0, 0.0, 0.0, 0.0}, {1.0, 2.0, 3.0, 4.0}}));
    Variable Y = activation_moved.feed_forward(X);
    Y.back_propagation();
    Array x = X.get_value();
    Array y = Y.get_value();
    Array expected_y = Array<double>({{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {1.0, 2.0, 3.0, 4.0}});
    EXPECT_TRUE(y == expected_y);
    activation_moved.update_weights_chain(sgd);
    activation_moved.update_weights(sgd);
}
