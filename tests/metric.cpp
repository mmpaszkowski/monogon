//
// Created by Mateusz Paszkowski on 21.08.22.
//

#include <gtest/gtest.h>
#include <monogon/metric/CategoricalAccuracy.h>

TEST(metric, CategoricalAccuracy)
{
    Array A = {{0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}};
    Array B = {{0.1, 0.9, 0.8}, {0.05, 0.95, 0.0}, {0.05, 0.95, 0.0}, {0.05, 0.95, 0.0}};
    CategoricalAccuracy categorical_accuracy;
    CategoricalAccuracy categorical_accuracy_copy = categorical_accuracy;
    CategoricalAccuracy categorical_accuracy_moved = std::move(categorical_accuracy_copy);
    categorical_accuracy_copy = categorical_accuracy;
    categorical_accuracy_moved = std::move(categorical_accuracy_copy);

    GTEST_ASSERT_EQ(categorical_accuracy_moved.operator()(A, B), 0.75);
}
