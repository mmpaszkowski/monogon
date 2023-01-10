#include "monogon/tool/ArgMax.h"
#include "monogon/tool/ConfusionMatrix.h"
#include <gtest/gtest.h>
#include <monogon/tool/OneHot.h>

TEST(one_hot, functor)
{
    OneHot oneHot;
    Array v{0, 1, 2, 3, 3};
    Array result = {{1.0, 0.0, 0.0, 0.0},
                    {0.0, 1.0, 0.0, 0.0},
                    {0.0, 0.0, 1.0, 0.0},
                    {0.0, 0.0, 0.0, 1.0},
                    {0.0, 0.0, 0.0, 1.0}};

    GTEST_ASSERT_EQ(oneHot(v, 4, 1.0, 0.0) == result, true);
}

TEST(arg_max, functor)
{
    ArgMax arg_max;
    Array M = {{1.0, 2.0, 3.0, 4.0},
               {5.0, 4.0, 2.0, 1.0},
               {2.0, 2.0, 4.0, 4.0},
               {3.0, 2.0, 5.0, 1.0},
               {0.0, 0.0, 0.0, 1.0}};
    Array<size_t> result{3, 0, 2, 2, 3};
    GTEST_ASSERT_EQ(arg_max(M) == result, true);
}

TEST(confusion_matrix, functor)
{
    ConfusionMatrix confusion_matrix;
    Array<size_t> V1 = {0, 1, 2, 3, 4, 0};
    Array<size_t> V2 = {0, 1, 2, 3, 4, 0};


    Array<size_t> M =
          {{2, 0, 0, 0, 0},
           {0, 1, 0, 0, 0},
           {0, 0, 1, 0, 0},
           {0, 0, 0, 1, 0},
           {0, 0, 0, 0, 1}};

    std::cout << confusion_matrix(V1, V2);

    GTEST_ASSERT_EQ(confusion_matrix(V1, V2) == M, true);
}