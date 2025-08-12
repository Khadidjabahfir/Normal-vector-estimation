#ifndef KNN_ALGORITHM_H
#define KNN_ALGORITHM_H
#include <vector>
class KnnAlgorithm
{
public:
    int k; // Number of neighbors
    std::vector<std::vector<int>> neighbor_indices;
    std::vector<std::vector<float>> neighbor_distances;
    KnnAlgorithm(int k = 3);
    ~KnnAlgorithm();
    void fit(const std::vector<std::vector<float>> &X_train, const std::vector<float> &y_train);
    std::vector<float> predict(const std::vector<std::vector<float>> &X_test);
};
#endif