#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <Eigen/Dense>
#include <cmath>
#include "knn_algorithm.h"
using namespace std; 

struct PlanarPlan {
    std::vector<std::vector<float>> points;
    Eigen::Vector3f eigen_vector;
    float planarity_score;
};


Eigen::Matrix3f compute_covariance(const std::vector<std::vector<float>>& points) {
    Eigen::MatrixXf mat(points.size(), 3);
    for (size_t i = 0; i < points.size(); ++i) {
        mat(i, 0) = points[i][0];
        mat(i, 1) = points[i][1];
        mat(i, 2) = points[i][2];
    }
    Eigen::Vector3f mean = mat.colwise().mean();
    Eigen::MatrixXf centered = mat.rowwise() - mean.transpose();
    Eigen::Matrix3f cov = (centered.adjoint() * centered) / float(mat.rows());
    return cov;
}


std::map<int, PlanarPlan> planar_segmentation_version_4(
    const std::vector<std::vector<float>>& X,
    const std::vector<std::vector<int>>& neighbor_indices,
    int n_neighbors,
    float threshold_eigen_value)
{
    std::map<int, PlanarPlan> planar_plans;
    std::unordered_set<int> explored;
    int plan_id = 0;
    int n = X.size();

    for (int seed_idx = 0; seed_idx < n; ++seed_idx) {
        if (explored.find(seed_idx) != explored.end())
            continue;

        std::vector<int> neighbors = neighbor_indices[seed_idx];
        if (!neighbors.empty()) neighbors.erase(neighbors.begin());

        std::vector<std::vector<float>> planar_plan_points;
        planar_plan_points.push_back(X[seed_idx]);
        for (auto ni : neighbors) planar_plan_points.push_back(X[ni]);

        Eigen::Matrix3f cov = compute_covariance(planar_plan_points);
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eig_solver(cov);
        Eigen::Vector3f eig_val = eig_solver.eigenvalues();

        float initial_smallest_eig_value = eig_val(0);

        std::vector<int> to_explore = neighbors;
        explored.insert(seed_idx);
        explored.insert(neighbors.begin(), neighbors.end());

        while (!to_explore.empty()) {
            int idx = to_explore.back();
            to_explore.pop_back();

            const auto& local_neighbors = neighbor_indices[idx];
            for (int local_idx : local_neighbors) {
                if (explored.find(local_idx) != explored.end())
                    continue;

                auto temp_points = planar_plan_points;
                temp_points.push_back(X[local_idx]);

                Eigen::Matrix3f local_cov = compute_covariance(temp_points);
                Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> local_solver(local_cov);
                Eigen::Vector3f local_eig_val = local_solver.eigenvalues();

                float ratio = local_eig_val(0) / initial_smallest_eig_value;
                if (ratio <= threshold_eigen_value) {
                    planar_plan_points.push_back(X[local_idx]);
                    to_explore.push_back(local_idx);
                    explored.insert(local_idx);
                }
            }
        }

        Eigen::Matrix3f final_cov = compute_covariance(planar_plan_points);
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> final_solver(final_cov);
        Eigen::Vector3f final_eig_val = final_solver.eigenvalues();
        Eigen::Matrix3f final_eig_vec = final_solver.eigenvectors();

        float la_1 = final_eig_val(0);
        float la_2 = final_eig_val(1);
        float la_3 = final_eig_val(2);
        float planarity_score = (la_2 - la_1) / la_3;

        PlanarPlan plan;
        plan.points = planar_plan_points;
        plan.eigen_vector = final_eig_vec.col(0);
        plan.planarity_score = planarity_score;

        planar_plans[plan_id++] = plan;
    }

    return planar_plans;
}

// -------- Main test --------
int main() {
    
    std::vector<std::vector<float>> X = {
        {0, 0, 0}, {1, 0, 0}, {2, 0, 0},   
        {0, 1, 0}, {1, 1, 0}, {2, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {2, 0, 1},   
        {0, 1, 1}, {1, 1, 1}, {2, 1, 1}
    };
    std::vector<int> y = {0,1,2}
    nneighbors = KnnAlgorithm()
    nneighbors.fit(X, y )
    vector<int> results = SortByDensity(nneighbors.distances, 3);
    for (int i = 0 ; i<results.size() ; i++) {
        cout<< results[i] << endl ; 
    }

    return 0;
}
