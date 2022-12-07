#include <gtest/gtest.h>
#include <monogon/Vector.h>
#include <monogon/Matrix.h>
#include <sstream>

template
class Vector<double>;

TEST(vector, initializer_list_constructor) {
    Vector v1 = {1.0, 2.0, 3.0};
    std::valarray result = {1.0, 2.0, 3.0};

    for (std::size_t i = 0; i < result.size(); i++) {
        GTEST_ASSERT_TRUE(result[i] == v1.get_data()[i]);
    }
    GTEST_ASSERT_EQ(v1.size(), 3);
}

TEST(vector, size_value_constructor) {
    Vector v1(3, 1.0);
    std::valarray result = {1.0, 1.0, 1.0};

    for (std::size_t i = 0; i < result.size(); i++) {
        GTEST_ASSERT_TRUE(result[i] == v1.get_data()[i]);
    }
    GTEST_ASSERT_EQ(v1.size(), 3);
}

TEST(vector, container_constructor) {
    std::vector<double> vec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Vector v1(vec);
    std::valarray result = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    for (std::size_t i = 0; i < result.size(); i++) {
        GTEST_ASSERT_TRUE(result[i] == v1.get_data()[i]);
    }
    GTEST_ASSERT_EQ(v1.size(), 6);
}

TEST(vector, copy_constructor) {
    Vector v1(2, 0.0);
    Vector v2(v1);
    Vector result = {0.0, 0.0};

    GTEST_ASSERT_EQ(v2 == v1, true);
    GTEST_ASSERT_EQ(v2 == result, true);
}

TEST(vector, move_constructor) {
    Vector v1(2, 0.0);
    Vector v2(std::move(v1));
    Vector result = {0.0, 0.0};

    GTEST_ASSERT_EQ(v1.size(), 0);
    GTEST_ASSERT_EQ(v2 == result, true);
}

TEST(vector, copy_assignment_operator) {
    Vector v1(2, 0.0);
    Vector v2(1.0, 2.0);
    v2 = v1;
    Vector result = {0.0, 0.0};

    GTEST_ASSERT_EQ(v2 == v1, true);
    GTEST_ASSERT_EQ(v2 == result, true);
}

TEST(vector, move_assignment_operator) {
    Vector v1(2, 0.0);
    Vector v2(1.0, 2.0);
    v2 = std::move(v1);
    Vector result = {0.0, 0.0};

    GTEST_ASSERT_EQ(v1.size(), 0);
    GTEST_ASSERT_EQ(v2 == result, true);
}

TEST(vector, subscription) {
    Vector v = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    GTEST_ASSERT_EQ(v(0), 1.0);
    GTEST_ASSERT_EQ(v(1), 2.0);
    GTEST_ASSERT_EQ(v(2), 3.0);
    GTEST_ASSERT_EQ(v(3), 4.0);
    GTEST_ASSERT_EQ(v(4), 5.0);
    GTEST_ASSERT_EQ(v(5), 6.0);
}

TEST(vector, const_subscription) {
    const Vector v = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    GTEST_ASSERT_EQ(v(0), 1.0);
    GTEST_ASSERT_EQ(v(1), 2.0);
    GTEST_ASSERT_EQ(v(2), 3.0);
    GTEST_ASSERT_EQ(v(3), 4.0);
    GTEST_ASSERT_EQ(v(4), 5.0);
    GTEST_ASSERT_EQ(v(5), 6.0);
}

TEST(vector, unary_minus) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Vector v2 = {-1.0, -2.0, -3.0, -4.0, -5.0, -6.0};
    GTEST_ASSERT_EQ(-v1 == v2, true);
}

TEST(vector, comparison_equal) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    GTEST_ASSERT_EQ(v1 == v2, true);
}

TEST(vector, comparison_not_equal) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Vector v2 = {1.0, 2.0, 3.0, 5.0};

    GTEST_ASSERT_EQ(v1 == v2, false);
}

TEST(vector, addition) {
    Vector v1 = {4.0, 3.0, 2.0, 1.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    Vector v3 = {5.0, 5.0, 5.0, 5.0};

    GTEST_ASSERT_EQ(v1 + v2 == v3, true);
}

TEST(vector, substraction) {
    Vector v1 = {5.0, 5.0, 5.0, 5.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    Vector v3 = {4.0, 3.0, 2.0, 1.0};

    GTEST_ASSERT_EQ(v1 - v2 == v3, true);
}

TEST(vector, multiplication) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    Vector v3 = {1.0, 4.0, 9.0, 16.0};

    GTEST_ASSERT_EQ(v1 * v2 == v3, true);
}

TEST(vector, division) {
    Vector v1 = {1.0, 4.0, 9.0, 16.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    Vector v3 = {1.0, 2.0, 3.0, 4.0};

    GTEST_ASSERT_EQ(v1 / v2 == v3, true);
}

TEST(vector, matrix_addition) {
    Vector v1 = {4.0, 3.0, 2.0, 1.0};

    Matrix B = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Matrix C = {{5.0, 6.0},
                {4.0, 5.0},
                {3.0, 4.0},
                {2.0, 3.0}};

    GTEST_ASSERT_EQ(v1 + B == C, true);
}

TEST(vector, matrix_substraction) {
    Vector v1 = {4.0, 3.0, 2.0, 1.0};

    Matrix B = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Matrix C = {{3.0, 2.0},
                {2.0, 1.0},
                {1.0, 0.0},
                {0.0, -1.0}};

    GTEST_ASSERT_EQ(v1 - B == C, true);
}

TEST(vector, matrix_multiplication) {
    Vector v1 = {4.0, 3.0, 2.0, 1.0};

    Matrix B = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Matrix C = {{4.0, 8.0},
                {3.0, 6.0},
                {2.0, 4.0},
                {1.0, 2.0}};

    GTEST_ASSERT_EQ(v1 * B == C, true);
}

TEST(vector, matrix_division) {
    Vector v1 = {4.0, 3.0, 2.0, 1.0};

    Matrix B = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Matrix C = {{4.0, 2.0},
                {3.0, 1.5},
                {2.0, 1.0},
                {1.0, 0.5}};

    GTEST_ASSERT_EQ(v1 / B == C, true);
}

TEST(vector, scalar_addition) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    double b = 2.0;

    Vector v3 = {3.0, 4.0, 5.0, 6.0};

    GTEST_ASSERT_EQ(v1 + b == v3, true);
}

TEST(vector, scalar_substraction) {
    Vector v1 = {3.0, 4.0, 5.0, 6.0};

    double b = 2.0;

    Vector v3 = {1.0, 2.0, 3.0, 4.0};

    GTEST_ASSERT_EQ(v1 - b == v3, true);
}

TEST(vector, scalar_multiplication) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    double b = 2.0;

    Vector v3 = {2.0, 4.0, 6.0, 8.0};

    GTEST_ASSERT_EQ(v1 * b == v3, true);
}

TEST(vector, scalar_division) {
    Vector v1 = {2.0, 4.0, 6.0, 8.0};

    double b = 2.0;

    Vector v3 = {1.0, 2.0, 3.0, 4.0};

    GTEST_ASSERT_EQ(v1 / b == v3, true);
}

TEST(vector, transposition) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Matrix C = {{1.0, 2.0, 3.0, 4.0}};

    GTEST_ASSERT_EQ(v1.transpose() == C, true);
}

TEST(vector, dot_vector) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Vector v2 = {1.0, 2.0, 3.0, 4.0};

    double c = 30.0;

    GTEST_ASSERT_EQ(v1.dot(v2) == c, true);
}

TEST(vector, dot_matrix) {
    Vector v1 = {1.0, 2.0, 3.0, 4.0};

    Matrix v2 = {{1.0, 2.0, 3.0, 4.0}};

    Matrix c = {{1.0, 2.0, 3.0,  4.0},
                {2.0, 4.0, 6.0,  8.0},
                {3.0, 6.0, 9.0,  12.0},
                {4.0, 8.0, 12.0, 16.0}};

    GTEST_ASSERT_EQ(v1.dot(v2) == c, true);
}

TEST(vector, avg) {
    Vector A = {
            {1.0, 2.0, 3.0, 4.0}
    };

    GTEST_ASSERT_EQ(A.avg() == 2.5, true);
}

TEST(vector, printout) {
    std::ostringstream out;
    out << Vector{1.0, 2.0, 3.0, 4.0};
    GTEST_ASSERT_EQ(out.str(), "[1, 2, 3, 4]\n");
}
