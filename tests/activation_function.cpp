//
// Created by Mateusz Paszkowski on 04.01.23.
//

#include <gtest/gtest.h>
#include <monogon/activation/ActivationFunction.h>

TEST(relu, no_arg_constructor)
{
    ReLu reLu;
    GTEST_ASSERT_TRUE(&reLu);
}

TEST(relu, copy_constructor)
{
    ReLu reLu;
    ReLu reLu_copy = reLu;
    GTEST_ASSERT_TRUE(&reLu_copy);
}

TEST(relu, move_constructor)
{
    ReLu reLu;
    ReLu reLu_move = std::move(reLu);
    GTEST_ASSERT_TRUE(&reLu_move);
}

TEST(relu, copy_assignment)
{
    ReLu reLu;
    ReLu reLu_copy;
    reLu_copy = reLu;
    GTEST_ASSERT_TRUE(&reLu_copy);
}

TEST(relu, move_assignment)
{
    ReLu reLu;
    ReLu reLu_move;
    reLu_move = std::move(reLu);
    GTEST_ASSERT_TRUE(&reLu_move);
}

TEST(relu, call_operator)
{
    Variable A = Variable(Array<double>({{-3.0, -2.0, -1.0}, {1.0, 2.0, 3.0}}));
    Array expected_A = {{0.0, 0.0, 0.0}, {1.0, 2.0, 3.0}};
    ReLu reLu;

    GTEST_ASSERT_TRUE(reLu(A).get_value() == expected_A);
}

TEST(sigmoid, no_arg_constructor)
{
    Sigmoid sigmoid;
    GTEST_ASSERT_TRUE(&sigmoid);
}

TEST(sigmoid, copy_constructor)
{
    Sigmoid sigmoid;
    Sigmoid sigmoid_copy = sigmoid;
    GTEST_ASSERT_TRUE(&sigmoid_copy);
}

TEST(sigmoid, move_constructor)
{
    Sigmoid sigmoid;
    Sigmoid sigmoid_move = std::move(sigmoid);
    GTEST_ASSERT_TRUE(&sigmoid_move);
}

TEST(sigmoid, copy_assignment)
{
    Sigmoid sigmoid;
    Sigmoid sigmoid_copy;
    sigmoid_copy = sigmoid;
    GTEST_ASSERT_TRUE(&sigmoid_copy);
}

TEST(sigmoid, move_assignment)
{
    Sigmoid sigmoid;
    Sigmoid sigmoid_move;
    sigmoid_move = std::move(sigmoid);
    GTEST_ASSERT_TRUE(&sigmoid_move);
}

TEST(sigmoid, call_operator)
{
    Variable A(Array<double>{{-3.0, -2.0, -1.0}, {1.0, 2.0, 3.0}});
    Array expected_A = {{0.047426, 0.119203, 0.268941}, {0.731059, 0.880797, 0.952574}};
    Sigmoid sigmoid;
    Array result = sigmoid(A).get_value();

    for(size_t i = 0; i < 2; i++)
        for(size_t j = 0; j < 3; j++)
            EXPECT_NEAR(result(i, j), expected_A(i, j), 0.000001);
}
