#ifndef KNN_ALGORITHM_H
#define KNN_ALGORITHM_H
#include <vector>
#include "../../base_utils/baseHelperClass.h" 
class KnnAlgorithm : public baseHelperClass<float>
{
    public : 
        int k; // Number of neighbors
        std::vector<std::vector<int>> indices ; 
        std::vector<std::vector<float>> distances; 
        KnnAlgorithm(int k = 3);
        ~KnnAlgorithm();
        void fit(const std::vector<std::vector<float>> &X_train, const std::vector<float> &y_train) override;
        std::vector<float> predict(const std::vector<std::vector<float>> &X_test) override;
}; 
#endif