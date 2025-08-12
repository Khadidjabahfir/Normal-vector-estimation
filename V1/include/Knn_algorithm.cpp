#include "knn_algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>
KnnAlgorithm::KnnAlgorithm(int k) : k(k)
{
    std::cout << "KNN Algorithm Constructor Is Called with k = " << k << std::endl;
}
KnnAlgorithm::~KnnAlgorithm()
{
    std::cout << "KNN Algorithm Destructor Is Called" << std::endl;
}
float euclidean_distance(const std::vector<float> &a, const std::vector<float> &b) {
    float sum = 0.0f;
    for (size_t i = 0; i < a.size(); i++) {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

void KnnAlgorithm::fit(const std::vector<std::vector<float>> &X_train,
                       const std::vector<float> &y_train)
{
    std::cout << "Fitting KNN Algorithm Model with k = " << k << std::endl;

    int n = X_train.size();   

   
    neighbor_indices.assign(n, std::vector<int>(k + 1));
    neighbor_distances.assign(n, std::vector<float>(k + 1));

    for (int i = 0; i < n; i++) {
        neighbor_indices[i][0] = i;
        neighbor_distances[i][0] = 0.0f;

        std::vector<std::pair<float, int>> dist_idx;
        dist_idx.reserve(n - 1);

        
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            float d = euclidean_distance(X_train[i], X_train[j]);
            dist_idx.push_back({d, j});
        }

        std::sort(dist_idx.begin(), dist_idx.end(),
                  [](const auto &a, const auto &b) {
                      return a.first < b.first;
                  });

        
        for (int neighbor_id = 0; neighbor_id < k; neighbor_id++) {
            neighbor_indices[i][neighbor_id + 1] = dist_idx[neighbor_id].second;
            neighbor_distances[i][neighbor_id + 1] = dist_idx[neighbor_id].first;
        }
    }
        std::cout << "KNN Algorithm Model Fitted Successfully" << std::endl;
        this->indices = neighbor_indices;
        this->distances = neighbor_distances;
}



std::vector<int> SortByDensity(const std::vector<std::vector<float>>& distances, 
                               int k)
{
    // we have used a different density function that also relies on the distance 
    int n = distances.size();
    std::vector<std::pair<float, int>> density_index(n);

    for (int i = 0; i < n; i++) {
        float sum_dist = 0.0f;
        for (int neighbor = 1; neighbor <= k; neighbor++) { 
            sum_dist += distances[i][neighbor];
        }
        float density = k / sum_dist;
        density_index[i] = {density, i};
    }

    std::sort(density_index.begin(), density_index.end(),
              [](const std::pair<float,int>& a, const std::pair<float,int>& b) {
                  return a.first > b.first; 
              });

    std::vector<int> sorted_indices;
    sorted_indices.reserve(n);
    for (const auto& p : density_index) {
        sorted_indices.push_back(p.second);
    }

    return sorted_indices; 
}

