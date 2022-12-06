#include <gtest/gtest.h>
#include <monogon/Matrix.h>
#include <monogon/Vector.h>

TEST(matrix, initializer_list_constructor)
{
    Matrix M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    std::valarray result = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    for (std::size_t i = 0; i<result.size(); i++)
    {
        GTEST_ASSERT_TRUE(result[i]==M.get_data()[i]);
    }

    GTEST_ASSERT_EQ(M.get_rows(), 2);
    GTEST_ASSERT_EQ(M.get_columns(), 3);
}


TEST(matrix, container_constructor)
{
    std::vector<std::vector<double>> vec = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    Matrix M(vec);
    std::valarray result = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    for (std::size_t i = 0; i<result.size(); i++)
    {
        GTEST_ASSERT_TRUE(result[i]==M.get_data()[i]);
    }

    GTEST_ASSERT_EQ(M.get_rows(), 2);
    GTEST_ASSERT_EQ(M.get_columns(), 3);
}

TEST(matrix, subscription)
{
    Matrix M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

    GTEST_ASSERT_EQ(M(0, 0), 1.0);
    GTEST_ASSERT_EQ(M(0, 1), 2.0);
    GTEST_ASSERT_EQ(M(0, 2), 3.0);
    GTEST_ASSERT_EQ(M(1, 0), 4.0);
    GTEST_ASSERT_EQ(M(1, 1), 5.0);
    GTEST_ASSERT_EQ(M(1, 2), 6.0);
}

TEST(matrix, const_subscription)
{
    const Matrix M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

    GTEST_ASSERT_EQ(M(0, 0), 1.0);
    GTEST_ASSERT_EQ(M(0, 1), 2.0);
    GTEST_ASSERT_EQ(M(0, 2), 3.0);
    GTEST_ASSERT_EQ(M(1, 0), 4.0);
    GTEST_ASSERT_EQ(M(1, 1), 5.0);
    GTEST_ASSERT_EQ(M(1, 2), 6.0);
}

TEST(matrix, unary_minus)
{
    Matrix A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    Matrix B = {{-1.0, -2.0, -3.0}, {-4.0, -5.0, -6.0}};
    GTEST_ASSERT_EQ(-A==B, true);
}

TEST(matrix, comparison_equal)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A==B, true);
}

TEST(matrix, comparison_not_equal)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 13.0}
    };

    GTEST_ASSERT_EQ(A==B, false);
}

TEST(matrix, addition)
{
    Matrix<double> A = {
            {12.0, 11.0, 10.0, 9.0},
            {8.0, 7.0, 6.0, 5.0},
            {4.0, 3.0, 2.0, 1.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0}
    };

    GTEST_ASSERT_EQ(A+B==C, true);
}

TEST(matrix, substraction)
{
    Matrix<double> A = {
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {12.0, 11.0, 10.0, 9.0},
            {8.0, 7.0, 6.0, 5.0},
            {4.0, 3.0, 2.0, 1.0}
    };

    GTEST_ASSERT_EQ(A-B==C, true);
}

TEST(matrix, multiplication)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {1.0, 4.0, 9.0, 16.0},
            {25.0, 36.0, 49.0, 64.0},
            {81.0, 100.0, 121.0, 144.0}
    };

    GTEST_ASSERT_EQ(A*B==C, true);
}

TEST(matrix, division)
{
    Matrix<double> A = {
            {1.0, 4.0, 9.0, 16.0},
            {25.0, 36.0, 49.0, 64.0},
            {81.0, 100.0, 121.0, 144.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A/B==C, true);
}

TEST(matrix, vector_addition)
{
    Matrix A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Matrix C = {{5.0, 6.0},
                {4.0, 5.0},
                {3.0, 4.0},
                {2.0, 3.0}};

    GTEST_ASSERT_EQ(A+v2==C, true);
}

TEST(matrix, vector_substraction)
{
    Matrix A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Matrix C = {{-3.0, -2.0},
                {-2.0, -1.0},
                {-1.0, 0.0},
                {0.0, 1.0}};

    GTEST_ASSERT_EQ(A-v2==C, true);
}

TEST(matrix, vector_multiplication)
{
    Matrix A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Matrix C = {{4.0, 8.0},
                {3.0, 6.0},
                {2.0, 4.0},
                {1.0, 2.0}};

    GTEST_ASSERT_EQ(A*v2==C, true);
}

TEST(matrix, vector_division)
{
    Matrix A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {8.0, 4.0, 2.0, 1.0};

    Matrix C = {{0.125, 0.25},
                {0.25, 0.5},
                {0.5, 1.0},
                {1.0, 2.0}};

    GTEST_ASSERT_EQ(A/v2==C, true);
}

TEST(matrix, scalar_addition)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    double b = 2.0;

    Matrix<double> C = {
            {3.0, 4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0}
    };

    GTEST_ASSERT_EQ(A+b==C, true);
}


TEST(matrix, scalar_substraction)
{
    Matrix<double> A = {
            {3.0, 4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0}
    };

    double b = 2.0;

    Matrix<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A-b==C, true);
}



TEST(matrix, scalar_multiplication)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    double b = 2.0;

    Matrix<double> C = {
            {2.0, 4.0, 6.0, 8.0},
            {10.0, 12.0, 14.0, 16.0},
            {18.0, 20.0, 22.0, 24.0}
    };

    GTEST_ASSERT_EQ(A*b==C, true);
}


TEST(matrix, scalar_division)
{
    Matrix<double> A = {
            {2.0, 4.0, 6.0, 8.0},
            {10.0, 12.0, 14.0, 16.0},
            {18.0, 20.0, 22.0, 24.0}
    };

    double b = 2.0;

    Matrix<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A/b==C, true);
}

TEST(matrix, transposition)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {1.0, 5.0, 9.0},
            {2.0, 6.0, 10.0},
            {3.0, 7.0, 11.0},
            {4.0, 8.0, 12.0}
    };

    GTEST_ASSERT_EQ(A.transpose() == C, true);
}

TEST(matrix, dot_matrix)
{
    Matrix<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Matrix<double> B = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0},
            {10.0, 11.0, 12.0}
    };

    Matrix<double> C = {
            {70.0, 80.0, 90.0},
            {158.0, 184.0, 210.0},
            {246.0, 288.0, 330.0}
    };

    GTEST_ASSERT_EQ(C==A.dot(B), true);
}

TEST(matrix, dot_vector)
{
    Matrix A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    Vector v2 = {1.0, 2.0, 3.0};
    Vector v3 = {14.0, 32.0, 50.0, 68.0};

    GTEST_ASSERT_EQ(v3==A.dot(v2), true);
}

TEST(matrix, max)
{
    Matrix A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    Matrix expected_result = {
        {5.0, 5.0, 5.0},
        {5.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };
    GTEST_ASSERT_EQ(A.max(5.0) == expected_result, true);
}

TEST(matrix, avg)
{
    Matrix A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A.avg() == 6.5, true);
}
