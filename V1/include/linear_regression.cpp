#include "linear_regression.h"
#include <iostream>
using namespace std;
LinearRegression::LinearRegression(
    float learning_rate_alpha,
    float number_epochs,
    float regularization_rate)
    : learning_rate_alpha(learning_rate_alpha),
      number_epochs(number_epochs),
      regularization_rate(regularization_rate)
{
    cout << "Linear Regression Constructor Is Called " << endl;
}
LinearRegression::~LinearRegression()
{
    cout << "Linear Regression Destructor Is Called " << endl;
}
vector<float> LinearRegression::compute_predictions(const vector<vector<int>> &X, int n, int m)
{
    vector<float> y_pred(n, 0.0f);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            y_pred[i] += weights[j] * X[i][j];
        }
        y_pred[i] += bias;
    }
    return y_pred;
}
void LinearRegression::compute_gradients(vector<float> &gradient_weights,
                                         float &gradient_bias,
                                         int n,
                                         int m,
                                         const vector<vector<int>> &X_train,
                                         const vector<float> &y_train,
                                         const vector<float> &y_pred)
{
    for (int i = 0; i < n; i++)
    {
        gradient_bias += (y_pred[i] - y_train[i]);
    }
    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            gradient_weights[j] += (y_pred[i] - y_train[i]) * X_train[i][j];
        }
    }
}
// we are going to implement batch linear regression
void LinearRegression::fit(const vector<vector<int>> &X_train, const vector<float> &y_train)
{
    cout << "Fitting Linear Regression Model" << endl;
    for (int epoch = 0; epoch < number_epochs; ++epoch)
    {
        int m = X_train[0].size(); // number of features
        int n = X_train.size();    // number of samples
        if (weights.size() == 0)
        {
            weights.resize(m, 0.0f); // initialize weights to zero
        }
        vector<float> y_pred = compute_predictions(X_train, n, m);

        float gradient_bias = 0.0f;
        vector<float> gradient_weights(m, 0.0f);
        compute_gradients(gradient_weights, gradient_bias, n, m, X_train, y_train, y_pred);
        // update the weights now
        bias = bias - learning_rate_alpha * (gradient_bias / n);
        for (int j = 0; j < m; j++)
        {
            weights[j] = weights[j] - (learning_rate_alpha * (gradient_weights[j] / n)) - (regularization_rate * weights[j]);
        }
        // print the epoch number
        cout << "Epoch: " << epoch + 1 << " / " << number_epochs << endl;
    }
}
vector<float> LinearRegression::predict(const vector<vector<int>> &X_test)
{
    cout << "Predicting using Linear Regression Model" << endl;
    int n = X_test.size();
    int m = X_test[0].size();
    vector<float> y_pred = compute_predictions(X_test, n, m);
    return y_pred;
}
