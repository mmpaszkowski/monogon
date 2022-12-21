#include <gtest/gtest.h>
#include <monogon/Array.h>
#include <monogon/Vector.h>

TEST(array, initializer_list_constructor)
{
    Array M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    std::valarray result = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    for (std::size_t i = 0; i<result.size(); i++)
    {
        GTEST_ASSERT_TRUE(result[i]==M.get_data()[i]);
    }

    GTEST_ASSERT_EQ(M.get_rows(), 2);
    GTEST_ASSERT_EQ(M.get_columns(), 3);
}


TEST(array, container_constructor)
{
    std::vector<std::vector<double>> vec = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    Array M(vec);
    std::valarray result = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    for (std::size_t i = 0; i<result.size(); i++)
    {
        GTEST_ASSERT_TRUE(result[i]==M.get_data()[i]);
    }

    GTEST_ASSERT_EQ(M.get_rows(), 2);
    GTEST_ASSERT_EQ(M.get_columns(), 3);
}

TEST(array, subscription)
{
    Array M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

    GTEST_ASSERT_EQ(M(0, 0), 1.0);
    GTEST_ASSERT_EQ(M(0, 1), 2.0);
    GTEST_ASSERT_EQ(M(0, 2), 3.0);
    GTEST_ASSERT_EQ(M(1, 0), 4.0);
    GTEST_ASSERT_EQ(M(1, 1), 5.0);
    GTEST_ASSERT_EQ(M(1, 2), 6.0);
}

TEST(array, const_subscription)
{
    const Array M = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};

    GTEST_ASSERT_EQ(M(0, 0), 1.0);
    GTEST_ASSERT_EQ(M(0, 1), 2.0);
    GTEST_ASSERT_EQ(M(0, 2), 3.0);
    GTEST_ASSERT_EQ(M(1, 0), 4.0);
    GTEST_ASSERT_EQ(M(1, 1), 5.0);
    GTEST_ASSERT_EQ(M(1, 2), 6.0);
}

TEST(array, unary_minus)
{
    Array A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    Array B = {{-1.0, -2.0, -3.0}, {-4.0, -5.0, -6.0}};
    GTEST_ASSERT_EQ(-A==B, true);
}

TEST(array, comparison_equal)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A==B, true);
}

TEST(array, comparison_not_equal)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 13.0}
    };

    GTEST_ASSERT_EQ(A==B, false);
}

TEST(array, addition)
{
    Array<double> A = {
            {12.0, 11.0, 10.0, 9.0},
            {8.0, 7.0, 6.0, 5.0},
            {4.0, 3.0, 2.0, 1.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0}
    };

    GTEST_ASSERT_EQ(A+B==C, true);
}

TEST(array, substraction)
{
    Array<double> A = {
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0},
            {13.0, 13.0, 13.0, 13.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {12.0, 11.0, 10.0, 9.0},
            {8.0, 7.0, 6.0, 5.0},
            {4.0, 3.0, 2.0, 1.0}
    };

    GTEST_ASSERT_EQ(A-B==C, true);
}

TEST(array, multiplication)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {1.0, 4.0, 9.0, 16.0},
            {25.0, 36.0, 49.0, 64.0},
            {81.0, 100.0, 121.0, 144.0}
    };

    GTEST_ASSERT_EQ(A*B==C, true);
}

TEST(array, division)
{
    Array<double> A = {
            {1.0, 4.0, 9.0, 16.0},
            {25.0, 36.0, 49.0, 64.0},
            {81.0, 100.0, 121.0, 144.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A/B==C, true);
}

TEST(array, vector_addition)
{
    Array A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Array C = {{5.0, 6.0},
                {4.0, 5.0},
                {3.0, 4.0},
                {2.0, 3.0}};

    GTEST_ASSERT_EQ(A+v2==C, true);
}

TEST(array, vector_substraction)
{
    Array A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Array C = {{-3.0, -2.0},
                {-2.0, -1.0},
                {-1.0, 0.0},
                {0.0, 1.0}};

    GTEST_ASSERT_EQ(A-v2==C, true);
}

TEST(array, vector_multiplication)
{
    Array A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {4.0, 3.0, 2.0, 1.0};

    Array C = {{4.0, 8.0},
                {3.0, 6.0},
                {2.0, 4.0},
                {1.0, 2.0}};

    GTEST_ASSERT_EQ(A*v2==C, true);
}

TEST(array, vector_division)
{
    Array A = {{1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0},
                {1.0, 2.0}};

    Vector v2 = {8.0, 4.0, 2.0, 1.0};

    Array C = {{0.125, 0.25},
                {0.25, 0.5},
                {0.5, 1.0},
                {1.0, 2.0}};

    GTEST_ASSERT_EQ(A/v2==C, true);
}

TEST(array, scalar_addition)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    double b = 2.0;

    Array<double> C = {
            {3.0, 4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0}
    };

    GTEST_ASSERT_EQ(A+b==C, true);
}


TEST(array, scalar_substraction)
{
    Array<double> A = {
            {3.0, 4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0, 10.0},
            {11.0, 12.0, 13.0, 14.0}
    };

    double b = 2.0;

    Array<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A-b==C, true);
}



TEST(array, scalar_multiplication)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    double b = 2.0;

    Array<double> C = {
            {2.0, 4.0, 6.0, 8.0},
            {10.0, 12.0, 14.0, 16.0},
            {18.0, 20.0, 22.0, 24.0}
    };

    GTEST_ASSERT_EQ(A*b==C, true);
}


TEST(array, scalar_division)
{
    Array<double> A = {
            {2.0, 4.0, 6.0, 8.0},
            {10.0, 12.0, 14.0, 16.0},
            {18.0, 20.0, 22.0, 24.0}
    };

    double b = 2.0;

    Array<double> C = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A/b==C, true);
}

TEST(array, transposition)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {1.0, 5.0, 9.0},
            {2.0, 6.0, 10.0},
            {3.0, 7.0, 11.0},
            {4.0, 8.0, 12.0}
    };

    GTEST_ASSERT_EQ(A.transpose() == C, true);
}

TEST(array, dot_matrix)
{
    Array<double> A = {
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0}
    };

    Array<double> B = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0},
            {10.0, 11.0, 12.0}
    };

    Array<double> C = {
            {70.0, 80.0, 90.0},
            {158.0, 184.0, 210.0},
            {246.0, 288.0, 330.0}
    };

    GTEST_ASSERT_EQ(C==A.dot(B), true);
}

TEST(array, dot_vector)
{
    Array A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    Vector v2 = {1.0, 2.0, 3.0};
    Vector v3 = {14.0, 32.0, 50.0, 68.0};

    GTEST_ASSERT_EQ(v3==A.dot(v2), true);
}

TEST(array, max)
{
    Array A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    Array expected_result = {
        {5.0, 5.0, 5.0},
        {5.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };
    GTEST_ASSERT_EQ(A.max(5.0) == expected_result, true);
}

TEST(array, avg)
{
    Array A = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0},
        {10.0, 11.0, 12.0}
    };

    GTEST_ASSERT_EQ(A.avg() == 6.5, true);
}
