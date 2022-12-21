#include <gtest/gtest.h>
#include <monogon/model/Model.h>
#include <monogon/layer/Dense.h>
#include <monogon/layer/Input.h>
#include <monogon/layer/Activation.h>
#include <monogon/loss/MSE.h>
#include <monogon/optimizer/SGD.h>

TEST(model, model)
{
    Array x = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    Array y = {{0.0, 0.0, 0.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 0.0}};

    ReLu relu;
    Sigmoid sigmoid;

    Input input({2});
    Dense dense1({120}, input);
    Activation activation1(relu, dense1);
    Dense dense2({120}, activation1);
    Activation activation2(relu, dense2);
    Dense dense3({3}, activation2);
    Activation activation3(sigmoid, dense3);

    Model model(dense1, activation3);
    model.compile(SGD(1.0), MSE());
    model.fit(x, y, 10, 2);
    std::cout << model.predict(x);
}