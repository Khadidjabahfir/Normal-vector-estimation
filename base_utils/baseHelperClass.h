#ifndef BASE_HELPER_CLASS_H
#define BASE_HELPER_CLASS_H
#include <vector>
using namespace std;
template <typename T>
class baseHelperClass
{
public:
    baseHelperClass()
    {
        // empty constructor
    }
    virtual ~baseHelperClass() = default; // deconstructor
    virtual void fit(const vector<vector<int>> &X_train, const vector<T> &y_train) = 0;
    virtual vector<T> predict(const vector<vector<int>> &X_test) = 0;
};
#endif