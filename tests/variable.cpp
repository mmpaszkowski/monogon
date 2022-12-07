//
// Created by noname on 21.08.22.
//

#include <gtest/gtest.h>
#include <monogon/Matrix.h>
#include <monogon/Variable.h>
#include <monogon/Vector.h>
#include <monogon/Tensor.h>

TEST(variable, addition)
{
    Variable a(5.0);
    Variable b(5.0);

    GTEST_ASSERT_EQ((a + b).get_value(), 10.0);
    GTEST_ASSERT_EQ((a + Variable(5.0)).get_value(), 10.0);
    GTEST_ASSERT_EQ((Variable(5.0) + b).get_value(), 10.0);
    GTEST_ASSERT_EQ((Variable(5.0) + Variable(5.0)).get_value(), 10.0);

    Variable c = (a + b);
    Variable d = (a + b);

    GTEST_ASSERT_EQ((c + d).get_value(), 20.0);
    GTEST_ASSERT_EQ((c + (a + b)).get_value(), 20.0);
    GTEST_ASSERT_EQ(((a + b) + d).get_value(), 20.0);
    GTEST_ASSERT_EQ(((a + b) + (a + b)).get_value(), 20.0);
}

TEST(variable, substraction)
{
    Variable a(5.0);
    Variable b(5.0);

    GTEST_ASSERT_EQ((a - b).get_value(), 0.0);
    GTEST_ASSERT_EQ((a - Variable(5.0)).get_value(), 0.0);
    GTEST_ASSERT_EQ((Variable(5.0) - b).get_value(), 0.0);
    GTEST_ASSERT_EQ((Variable(5.0) - Variable(5.0)).get_value(), 0.0);

    Variable c = (a - b);
    Variable d = (a - b);

    GTEST_ASSERT_EQ((c - d).get_value(), 0.0);
    GTEST_ASSERT_EQ((c - (a - b)).get_value(), 0.0);
    GTEST_ASSERT_EQ(((a - b) - d).get_value(), 0.0);
    GTEST_ASSERT_EQ(((a - b) - (a - b)).get_value(), 0.0);
}

TEST(variable, multiplication)
{
    Variable a(5.0);
    Variable b(5.0);

    GTEST_ASSERT_EQ((a * b).get_value(), 25.0);
    GTEST_ASSERT_EQ((a * Variable(5.0)).get_value(), 25.0);
    GTEST_ASSERT_EQ((Variable(5.0) * b).get_value(), 25.0);
    GTEST_ASSERT_EQ((Variable(5.0) * Variable(5.0)).get_value(), 25.0);

    Variable c = (a * b);
    Variable d = (a * b);

    GTEST_ASSERT_EQ((c * d).get_value(), 625.0);
    GTEST_ASSERT_EQ((c * (a * b)).get_value(), 625.0);
    GTEST_ASSERT_EQ(((a * b) * d).get_value(), 625.0);
    GTEST_ASSERT_EQ(((a * b) * (a * b)).get_value(), 625.0);
}

TEST(variable, division)
{
    Variable a(5.0);
    Variable b(5.0);

    GTEST_ASSERT_EQ((a / b).get_value(), 1.0);
    GTEST_ASSERT_EQ((a / Variable(5.0)).get_value(), 1.0);
    GTEST_ASSERT_EQ((Variable(5.0) / b).get_value(), 1.0);
    GTEST_ASSERT_EQ((Variable(5.0) / Variable(5.0)).get_value(), 1.0);

    Variable c = (a / b);
    Variable d = (a / b);

    GTEST_ASSERT_EQ((c / d).get_value(), 1.0);
    GTEST_ASSERT_EQ((c / (a / b)).get_value(), 1.0);
    GTEST_ASSERT_EQ(((a / b) / d).get_value(), 1.0);
    GTEST_ASSERT_EQ(((a / b) / (a / b)).get_value(), 1.0);
}

TEST(variable, copy_and_assignment)
{
    Variable a(5.0);
    Variable b = a;

    b.back_propagation();
    a.get_grad();

    GTEST_ASSERT_EQ(a.get_grad(), 1.0);
}

TEST(variable, addition_derivative)
{
    Variable a(6.0);
    Variable b(5.0);
    auto c = a + b + a;
    c.back_propagation();
    GTEST_ASSERT_EQ(a.get_grad(), 2.0);
}

TEST(variable, substaction_derivative)
{
    Variable a(6.0);
    Variable b(5.0);
    auto c = a - b - a - a - a;
    c.back_propagation();
    GTEST_ASSERT_EQ(a.get_grad(), -2.0);
}

TEST(variable, multiplication_derivative)
{
    Variable a(5.0);
    Variable b(5.0);
    auto c = a * b * b;
    c.back_propagation();
    GTEST_ASSERT_EQ(a.get_grad(), 25);
}

TEST(variable, division_derivative)
{
    Variable a(25.0);
    Variable b(5.0);
    Variable c(5.0);
    auto d = ((a / b) * (a / c) * (b / c)) / ((a / b) * (a / c) * (b / c));
    d.back_propagation();
    GTEST_ASSERT_EQ(a.get_grad(), 0);
    GTEST_ASSERT_EQ(b.get_grad(), 0);
    GTEST_ASSERT_EQ(c.get_grad(), 0);
}

TEST(variable, matrix_dot_vector)
{
    Matrix mat_W = {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 10.0, 11.0, 12.0}};

    Vector vec_x = {{1.0, 2.0, 3.0, 4.0}};

    Variable W = mat_W;
    Variable x = vec_x;

    Variable y = W.dot(x);
    Vector expected_y{30.0, 70.0, 110.0};
    GTEST_ASSERT_EQ(expected_y == y.get_value(), true);

    y.back_propagation();

    Matrix expected_dW = {{1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}};
    GTEST_ASSERT_EQ(W.get_grad() == expected_dW, true);

    Vector expected_dx = {15.0, 18.0, 21.0, 24.0};
    GTEST_ASSERT_EQ(x.get_grad() == expected_dx, true);
}

TEST(variable, matrix_dot_matrix)
{
    Matrix mat_W = {{1.0, 2.0, 3.0, 4.0}, {1.0, 2.0, 3.0, 4.0}};

    Matrix mat_X = {{1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}};

    Variable W = mat_W;
    Variable X = mat_X;

    Variable D = W.dot(X);
    Matrix expected_D = {{10.0, 20.0, 30.0}, {10.0, 20.0, 30.0}};
    GTEST_ASSERT_EQ(expected_D == D.get_value(), true);

    D.back_propagation();

    Matrix expected_dW = {{6.0, 6.0, 6.0, 6.0}, {6.0, 6.0, 6.0, 6.0}};
    GTEST_ASSERT_EQ(W.get_grad() == expected_dW, true);

    Matrix expected_dX = {{2.0, 2.0, 2.0}, {4.0, 4.0, 4.0}, {6.0, 6.0, 6.0}, {8.0, 8.0, 8.0}};
    GTEST_ASSERT_EQ(X.get_grad() == expected_dX, true);
}

TEST(variable, matrix_plus_vector_broadcast)
{
    Matrix mat_W = {{1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}};

    Vector vec_x = {1.0, 2.0, 3.0};

    Variable W = mat_W;
    Variable x = vec_x;

    Variable y = W + x;
    Matrix expected_y = {{2.0, 3.0, 4.0}, {3.0, 4.0, 5.0}, {4.0, 5.0, 6.0}};
    GTEST_ASSERT_EQ(expected_y == y.get_value(), true);

    y.back_propagation();

    Matrix expected_dW = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    GTEST_ASSERT_EQ(W.get_grad() == expected_dW, true);

    Vector expected_dx = {3.0, 3.0, 3.0};
    GTEST_ASSERT_EQ(x.get_grad() == expected_dx, true);
}

TEST(variable, matrix_plus_matrix_broadcast)
{
    Matrix mat_W = {{1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}};

    Matrix mat_X = {{1.0, 2.0, 3.0}};

    Variable W = mat_W;
    Variable X = mat_X;

    Variable y = W + X;
    Matrix expected_y = {{2.0, 4.0, 6.0}, {2.0, 4.0, 6.0}, {2.0, 4.0, 6.0}};
    GTEST_ASSERT_EQ(expected_y == y.get_value(), true);

    y.back_propagation();

    Matrix expected_dW = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    GTEST_ASSERT_EQ(W.get_grad() == expected_dW, true);

    Matrix expected_dX = {{3.0, 3.0, 3.0}};
    GTEST_ASSERT_EQ(X.get_grad() == expected_dX, true);
}

TEST(variable, matrix_max)
{
    Variable<Matrix<double>> A = Matrix{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}};

    Matrix expected_result = {{5.0, 5.0, 5.0}, {5.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}};
    Variable result = A.max(Variable<double>(5.0));
    GTEST_ASSERT_EQ(result.get_value() == expected_result, true);

    result.back_propagation();
    Matrix expected_grad = {{0.0, 0.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    GTEST_ASSERT_EQ(A.get_grad() == expected_grad, true);
}

TEST(variable, vector_avg)
{
    Vector v = {1.0, 2.0, 3.0, 4.0};
    Variable<Vector<double>> A = v;

    Variable avg = A.avg();
    avg.back_propagation();

    Vector expected_A = {0.25, 0.25, 0.25, 0.25};
    GTEST_ASSERT_EQ(A.get_grad() == expected_A, true);
}

TEST(variable, matrix_avg)
{
    Matrix a = {{1.0, 2.0}, {4.0, 5.0}, {7.0, 8.0}, {10.0, 11.0}};
    Variable<Matrix<double>> A = a;

    Variable avg = A.avg();
    avg.back_propagation();

    Matrix expected_A = {{0.125, 0.125}, {0.125, 0.125}, {0.125, 0.125}, {0.125, 0.125}};
    GTEST_ASSERT_EQ(A.get_grad() == expected_A, true);
}

TEST(variable, matrix_exp)
{
    Variable<Matrix<double>> A = Matrix{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}, {10.0, 11.0, 12.0}};

    Variable result = A.exp();
    result.back_propagation();

    GTEST_ASSERT_EQ(A.get_grad() == result.get_value(), true);
}

TEST(variable, tensor)
{
    Variable<Tensor<double>> A(
            {{{{1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}}, {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}},
             {{{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}, {{1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}}}});

    Variable<Tensor<double>> B(
            {{{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}, {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}},
             {{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}, {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}}});

    Tensor<double> expected_result(
            {{{{2.0, 2.0, 2.0, 2.0}, {2.0, 2.0, 2.0, 2.0}, {2.0, 2.0, 2.0, 2.0}, {2.0, 2.0, 2.0, 2.0}},
                     {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}},
             {{{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}},
                     {{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}}}});

    Variable result = A.dot(B);
    result.back_propagation();

    GTEST_ASSERT_EQ(result.get_value() == expected_result, true);

    Tensor<double> expected_A_grad_result(
            {{{{4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}}, {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}},
             {{{4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}}, {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}}});

    GTEST_ASSERT_EQ(A.get_grad() == expected_A_grad_result, true);

    Tensor<double> expected_B_grad_result(
            {{{{4.0, 4.0, 4.0, 4.0}, {4.0, 4.0, 4.0, 4.0}}, {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}},
             {{{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}}, {{4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}, {4.0, 4.0}}}});

    GTEST_ASSERT_EQ(B.get_grad() == expected_B_grad_result, true);


    //    Variable result = A.exp();
    //
    //    GTEST_ASSERT_EQ(A.get_grad() == result.get_value(), true);
}

TEST(variable, complex)
{
    const size_t in_features = 2;
    const size_t layer1 = 3;
    const size_t classes = 1;
    double lr = 0.01;

    Variable<Matrix<double>> x = Matrix<double>{{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};

    Variable<Matrix<double>> y = Matrix<double>{{0.0}, {0.0}, {0.0}, {1.0}};
    Variable<Matrix<double>> weights1 = Matrix<double>(in_features, layer1, 1.0);
    Variable<Matrix<double>> weights2 = Matrix<double>(layer1, classes, 1.0);
    Variable<Matrix<double>> bias1 = Matrix<double>(1, layer1, 1.0);
    Variable<Matrix<double>> bias2 = Matrix<double>(1, classes, 1.0);

    Variable y_pred1 = x.dot(weights1) + bias1;
    Variable y_pred = y_pred1.dot(weights2) + bias2;

    Matrix<double> expected_y_pred = {{4.0}, {7.0}, {7.0}, {10.0}};
    GTEST_ASSERT_EQ(expected_y_pred == y_pred.get_value(), true);

    Variable loss = ((y - y_pred) * (y - y_pred)).avg();
    GTEST_ASSERT_EQ(loss.get_value() == 48.75, true);
    loss.back_propagation();

    weights1.set_value(weights1.get_value() - weights1.get_grad() * lr);
    bias1.set_value(bias1.get_value() - bias1.get_grad() * lr);

    weights2.set_value(weights2.get_value() - weights2.get_grad() * lr);
    bias2.set_value(bias2.get_value() - bias2.get_grad() * lr);

    Matrix expected_weights1 = {{0.92, 0.92, 0.92}, {0.92, 0.92, 0.92}};
    GTEST_ASSERT_EQ(weights1.get_value() == expected_weights1, true);

    Matrix expected_bias = {{0.865}, {0.865}, {0.865}};
    GTEST_ASSERT_EQ(bias1.get_value() == expected_bias, true);
}