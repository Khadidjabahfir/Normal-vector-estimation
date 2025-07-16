#include "../include/linear_regression.h"
#include <iostream>
#include <vector>

int main()
{

    std::vector<std::vector<int>> X_train = {
        {1},
        {2},
        {3},
        {4},
        {5}};
    std::vector<float> y_train = {5.0f, 7.0f, 9.0f, 11.0f, 13.0f};

    LinearRegression model(0.01f, 100, 0.0f);
    model.fit(X_train, y_train);

    std::vector<std::vector<int>> X_test = {{6}, {7}, {8}};
    std::vector<float> y_pred = model.predict(X_test);

    std::cout << "\nPredictions:\n";
    for (float pred : y_pred)
    {
        std::cout << pred << std::endl;
    }

    return 0;
}
