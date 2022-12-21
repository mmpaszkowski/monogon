#include <gtest/gtest.h>
#include <monogon/tool/OneHot.h>

TEST(one_hot, functor) {
    OneHot oneHot;
    Array v{0, 1, 2, 3, 3};
    Array result = {{1.0, 0.0, 0.0, 0.0},
                     {0.0, 1.0, 0.0, 0.0},
                     {0.0, 0.0, 1.0, 0.0},
                     {0.0, 0.0, 0.0, 1.0},
                     {0.0, 0.0, 0.0, 1.0}};

    GTEST_ASSERT_EQ(oneHot(v, 4, 1.0, 0.0) == result, true);
}