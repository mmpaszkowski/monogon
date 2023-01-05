//
// Created by Mateusz Paszkowski on 04.01.23.
//

#include <gtest/gtest.h>
#include <monogon/activation/ActivationFunction.h>

TEST(activation_function, relu_construction)
{
    ReLu reLu;
    ReLu reLu_copy = reLu;
    ReLu reLu_moved = std::move(reLu_copy);
    reLu_copy = reLu;
    reLu_moved = std::move(reLu_copy);
}

TEST(activation_function, relu_call_operator)
{
    Variable A = Variable<Array<double>>({{-3.0, -2.0, -1.0}, {1.0, 2.0, 3.0}});
    Array expected_A = {{0.0, 0.0, 0.0}, {1.0, 2.0, 3.0}};
    ReLu reLu;

    GTEST_ASSERT_TRUE(reLu(A).get_value() == expected_A);
}

TEST(activation_function, sigmoid_construction)
{
    Sigmoid sigmoid;
    Sigmoid sigmoid_copy = sigmoid;
    Sigmoid sigmoid_moved = std::move(sigmoid_copy);
    sigmoid_copy = sigmoid;
    sigmoid_moved = std::move(sigmoid_copy);
}

TEST(activation_function, sigmoid_call_operator)
{
    Variable A = Variable<Array<double>>({{-3.0, -2.0, -1.0}, {1.0, 2.0, 3.0}});
    Array expected_A = {{0.047426, 0.119203, 0.268941}, {0.731059, 0.880797, 0.952574}};
    Sigmoid sigmoid;
    Array result = sigmoid(A).get_value();

    for(size_t i = 0; i < 2; i++)
        for(size_t j = 0; j < 3; j++)
            EXPECT_NEAR(result(i, j), expected_A(i, j), 0.000001);
}
