#ifndef LINEAR_REGRESSION_H
#define LINEAR_REGRESSION_H
#include "../../base_utils/baseHelperClass.h"
#include <vector>
using namespace std;

class LinearRegression : public baseHelperClass<float>
{
private:
    float learning_rate_alpha = 0.01;
    float number_epochs = 100;
    float regularization_rate = 0;
    vector<float> compute_predictions(const vector<vector<int>> &X, int n, int m);
    void compute_gradients(vector<float> &gradient_weights,
                           float &gradient_bias,
                           int n,
                           int m,
                           const vector<vector<int>> &X_train,
                           const vector<float> &y_train,
                           const vector<float> &y_pred);

public:
    vector<float> weights;
    float bias;
    LinearRegression(
        float learning_rate_alpha = 0.01,
        float number_epochs = 100,
        float regularization_rate = 0);
    ~LinearRegression();
    void fit(const vector<vector<int>> &X_train, const vector<float> &y_train) override;
    vector<float> predict(const vector<vector<int>> &X_test) override;
};
#endif