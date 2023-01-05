//
// Created by noname on 21.08.22.
//

#include <gtest/gtest.h>
#include <monogon/Array.h>
#include <monogon/Variable.h>
#include <monogon/loss/MSE.h>

TEST(loss, no_arg_constructor)
{
    MSE mse;
    GTEST_ASSERT_TRUE(&mse);
}

TEST(loss, copy_constructor)
{
    MSE mse;
    MSE mse_copy = mse;
    GTEST_ASSERT_TRUE(&mse_copy);
}

TEST(loss, move_constructor)
{
    MSE mse;
    MSE mse_move = std::move(mse);
    GTEST_ASSERT_TRUE(&mse_move);
}

TEST(loss, copy_assignment)
{
    MSE mse;
    MSE mse_copy;
    mse_copy = mse;
    GTEST_ASSERT_TRUE(&mse_copy);
}

TEST(loss, move_assignment)
{
    MSE mse;
    MSE mse_move;
    mse_move = std::move(mse);
    GTEST_ASSERT_TRUE(&mse_move);
}

TEST(loss, mse)
{
    MSE mse;
    MSE mse_copy = mse;
    MSE mse_moved = std::move(mse_copy);
    mse_copy = mse;
    mse_moved = std::move(mse_copy);

    Variable y = Variable(Array<double>({{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {1.0, 1.0}}));
    Variable y_pred = Variable(Array<double>({{1.0, 1.0}, {1.0, 1.0}, {0.0, 0.0}, {0.0, 0.0}}));

    Variable loss = mse_moved(y_pred, y);

    GTEST_ASSERT_EQ(loss.get_value(), 0.75);
}
