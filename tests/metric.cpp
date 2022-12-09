//
// Created by noname on 21.08.22.
//

#include <gtest/gtest.h>
#include <monogon/metric/CategoricalAccuracy.h>

TEST(metric, CategoricalAccuracy)
{
    Matrix A = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}};
    Matrix B = {{0.1, 0.9, 0.8}, {0.05, 0.95, 0.0}, {0.05, 0.95, 0.0}, {0.05, 0.95, 0.0}};
    CategoricalAccuracy<double> categoricalAccuracy;

    GTEST_ASSERT_EQ(categoricalAccuracy.operator()(A, B), 0.75);
}
