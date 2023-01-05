//
// Created by Mateusz Paszkowski on 04.01.23.
//

#include <gtest/gtest.h>
#include <monogon/initializer/Initializer.h>

TEST(initializer, glorot_uniform)
{
    GlorotUniform glorot_uniform;
    GlorotUniform glorot_uniform_copy = glorot_uniform;
    GlorotUniform glorot_uniform_moved = std::move(glorot_uniform_copy);
    glorot_uniform_copy = glorot_uniform;
    glorot_uniform_moved = std::move(glorot_uniform_copy);
}

TEST(initializer, glorot_uniform_call)
{
    GlorotUniform glorot_uniform;
    Array X = glorot_uniform(2, 3).get_value();
    double v = sqrt(6.0 / (2.0 + 3.0));
    for(size_t i = 0; i < 2; i++)
        for(size_t j = 0; j < 3; j++)
            EXPECT_NEAR(X(i, j), 0, v);
}

TEST(initializer, ones)
{
    Ones ones;
    Ones ones_copy = ones;
    Ones ones_moved = std::move(ones_copy);
    ones_copy = ones;
    ones_moved = std::move(ones_copy);
}

TEST(initializer, ones_call)
{
    Ones ones;
    Array X = ones(2, 3).get_value();
    Array expected_X = {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}};
    EXPECT_TRUE(X == expected_X);
}

TEST(initializer, zeros)
{
    Zeros zeros;
    Zeros zeros_copy = zeros;
    Zeros zeros_moved = std::move(zeros_copy);
    zeros_copy = zeros;
    zeros_moved = std::move(zeros_copy);
}

TEST(initializer, zeros_call)
{
    Zeros zeros;
    Array X = zeros(2, 3).get_value();
    Array expected_X = {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
    EXPECT_TRUE(X == expected_X);
}
