//
// Created by noname on 03.11.22.
//

#include <gtest/gtest.h>
#include <monogon/Tensor.h>

TEST(tensor, operator_equal)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T2({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T3({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}, {{7.0, 8.0, 9.0}, {10.0, 11.0, 13.0}}});

    GTEST_ASSERT_EQ(T1 == T2, true);
    GTEST_ASSERT_EQ(T1 == T3, false);
}

TEST(tensor, operator_sum)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T2({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T3({{{2.0, 4.0, 6.0}, {8.0, 10.0, 12.0}},
               {{14.0, 16.0, 18.0}, {20.0, 22.0, 24.0}},
               {{26.0, 28.0, 30.0}, {32.0, 34.0, 36.0}},
               {{38.0, 40.0, 42.0}, {44.0, 46.0, 48.0}}});

    GTEST_ASSERT_EQ(T1 + T2 == T3, true);
    GTEST_ASSERT_EQ(T1 + T1 == T3, true);
}

TEST(tensor, operator_diff)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T2({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T3({{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
               {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
               {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
               {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}});

    GTEST_ASSERT_EQ(T1 - T2 == T3, true);
    GTEST_ASSERT_EQ(T1 - T1 == T3, true);
}

TEST(tensor, operator_mul)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T2({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T3({{{1.0, 4.0, 9.0}, {16.0, 25.0, 36.0}},
               {{49.0, 64.0, 81.0}, {100.0, 121.0, 144.0}},
               {{169.0, 196.0, 225.0}, {256.0, 289.0, 324.0}},
               {{361.0, 400.0, 441.0}, {484.0, 529.0, 576.0}}});

    GTEST_ASSERT_EQ(T1 * T2 == T3, true);
    GTEST_ASSERT_EQ(T1 * T1 == T3, true);
}

TEST(tensor, operator_div)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T2({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});
    Tensor T3({
        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
    });
    GTEST_ASSERT_EQ(T1 / T2 == T3, true);
    GTEST_ASSERT_EQ(T1 / T1 == T3, true);
}

TEST(tensor, transpose)
{
    Tensor T1({{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}},
               {{7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}},
               {{13.0, 14.0, 15.0}, {16.0, 17.0, 18.0}},
               {{19.0, 20.0, 21.0}, {22.0, 23.0, 24.0}}});

    Tensor T2({{{1.0, 2.0, 3.0}, {7.0, 8.0, 9.0}, {13.0, 14.0, 15.0}, {19.0, 20.0, 21.0}},
               {{4.0, 5.0, 6.0}, {10.0, 11.0, 12.0}, {16.0, 17.0, 18.0}, {22.0, 23.0, 24.0}}});

    Tensor T3({{{1.0, 4.0}, {7.0, 10.0}, {13.0, 16.0}, {19.0, 22.0}},
               {{2.0, 5.0}, {8.0, 11.0}, {14.0, 17.0}, {20.0, 23.0}},
               {{3.0, 6.0}, {9.0, 12.0}, {15.0, 18.0}, {21.0, 24.0}}});

    GTEST_ASSERT_EQ(T1.transpose({1, 0, 2}) == T2, true);
    GTEST_ASSERT_EQ(T1.transpose({2, 0, 1}) == T3, true);
}

TEST(tensor, dot)
{
    Tensor T1({{{{1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}}, {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}},
               {{{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}, {{1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}}}});

    Tensor T2({{{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}, {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}},
               {{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}, {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}}});
}

TEST(tensor, complex)
{
    const size_t in_features = 2;
    const size_t layer1 = 3;
    const size_t classes = 1;
    double lr = 0.01;

    Tensor x{{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    Tensor y{std::initializer_list<double>{0.0}, std::initializer_list<double>{0.0}, std::initializer_list<double>{0.0}, std::initializer_list<double>{1.0}};
    Tensor weights1{{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    Tensor weights2{std::initializer_list<double>{1.0}, std::initializer_list<double>{1.0}, std::initializer_list<double>{1.0}};
    Tensor bias1{std::initializer_list<double>{1.0}, std::initializer_list<double>{1.0}, std::initializer_list<double>{1.0}};
    Tensor bias2{std::initializer_list<double>{1.0}};

    Tensor y_pred1 = x.dot(weights1) + bias1;
    Tensor y_pred = y_pred1.dot(weights2) + bias2;

    Tensor expected_y_pred = {std::initializer_list<double>{4.0},
                              std::initializer_list<double>{7.0},
                              std::initializer_list<double>{7.0},
                              std::initializer_list<double>{10.0}};

    GTEST_ASSERT_EQ(expected_y_pred == y_pred, true);

    double loss = ((y - y_pred) * (y - y_pred)).avg();
//
//    std::cout << loss;
//    GTEST_ASSERT_EQ(loss == 48.75, true);
//    loss.back_propagation();
//
//    weights1.set_value(weights1.get_value() - weights1.get_grad() * lr);
//    bias1.set_value(bias1.get_value() - bias1.get_grad() * lr);
//
//    weights2.set_value(weights2.get_value() - weights2.get_grad() * lr);
//    bias2.set_value(bias2.get_value() - bias2.get_grad() * lr);
//
//    Array expected_weights1 = {{0.92, 0.92, 0.92}, {0.92, 0.92, 0.92}};
//    GTEST_ASSERT_EQ(weights1.get_value() == expected_weights1, true);
//
//    Array expected_bias = {{0.865}, {0.865}, {0.865}};
//    GTEST_ASSERT_EQ(bias1.get_value() == expected_bias, true);
}