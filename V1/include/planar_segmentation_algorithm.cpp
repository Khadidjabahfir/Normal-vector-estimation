#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <Eigen/Dense>
#include <cmath>
#include "Knn_algorithm.h"
using namespace std;

struct PlanarPlan
{
    std::vector<std::vector<float>> points; // renamed, matches your struct
    Eigen::Vector3f eigen_vector;
    float planarity_score;
};

std::vector<int> SortByDensity(const std::vector<std::vector<float>> &distances,
                               int k)
{
    int n = distances.size();
    std::vector<std::pair<float, int>> density_index(n);

    for (int i = 0; i < n; i++)
    {
        float sum_dist = 0.0f;
        // assume distances[i][0] is self, neighbors start at 1
        for (int neighbor = 1; neighbor <= k && neighbor < (int)distances[i].size(); neighbor++)
        {
            sum_dist += distances[i][neighbor];
        }
        float density = (sum_dist == 0.0f) ? std::numeric_limits<float>::infinity() : (float)k / sum_dist;
        density_index[i] = {density, i};
    }

    std::sort(density_index.begin(), density_index.end(),
              [](const std::pair<float, int> &a, const std::pair<float, int> &b)
              {
                  return a.first > b.first;
              });

    std::vector<int> sorted_indices;
    sorted_indices.reserve(n);
    for (const auto &p : density_index)
        sorted_indices.push_back(p.second);

    return sorted_indices;
}

Eigen::Matrix3f compute_covariance(const std::vector<std::vector<float>> &points)
{
    Eigen::MatrixXf mat(points.size(), 3);
    for (size_t i = 0; i < points.size(); ++i)
    {
        mat(i, 0) = points[i][0];
        mat(i, 1) = points[i][1];
        mat(i, 2) = points[i][2];
    }
    Eigen::Vector3f mean = mat.colwise().mean();
    Eigen::MatrixXf centered = mat.rowwise() - mean.transpose();
    Eigen::Matrix3f cov = (centered.adjoint() * centered) / float(mat.rows());
    return cov;
}

float angle_between_normals(const Eigen::Vector3f &n1, const Eigen::Vector3f &n2)
{
    float dot_product = n1.dot(n2);
    float norms = n1.norm() * n2.norm();
    float v = dot_product / (norms + 1e-12f);
    if (v > 1.0f)
        v = 1.0f;
    if (v < -1.0f)
        v = -1.0f;
    return std::acos(v); // radians
}

// unchanged: planar_segmentation_version_4 signature expects neighbor_indices
std::map<int, PlanarPlan> planar_segmentation_version_4(
    const std::vector<std::vector<float>> &X,
    const std::vector<std::vector<int>> &neighbor_indices,
    int n_neighbors,
    float threshold_eigen_value)
{
    std::map<int, PlanarPlan> planar_plans;
    std::unordered_set<int> explored;
    int plan_id = 0;
    int n = X.size();

    for (int seed_idx = 0; seed_idx < n; ++seed_idx)
    {
        if (explored.find(seed_idx) != explored.end())
            continue;

        std::vector<int> neighbors = neighbor_indices[seed_idx];
        if (!neighbors.empty())
            neighbors.erase(neighbors.begin());

        std::vector<std::vector<float>> planar_plan_points;
        planar_plan_points.push_back(X[seed_idx]);
        for (auto ni : neighbors)
            planar_plan_points.push_back(X[ni]);

        Eigen::Matrix3f cov = compute_covariance(planar_plan_points);
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eig_solver(cov);
        Eigen::Vector3f eig_val = eig_solver.eigenvalues();

        float initial_smallest_eig_value = eig_val(0);

        std::vector<int> to_explore = neighbors;
        explored.insert(seed_idx);
        explored.insert(neighbors.begin(), neighbors.end());

        while (!to_explore.empty())
        {
            int idx = to_explore.back();
            to_explore.pop_back();

            const auto &local_neighbors = neighbor_indices[idx];
            for (int local_idx : local_neighbors)
            {
                if (explored.find(local_idx) != explored.end())
                    continue;

                auto temp_points = planar_plan_points;
                temp_points.push_back(X[local_idx]);

                Eigen::Matrix3f local_cov = compute_covariance(temp_points);
                Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> local_solver(local_cov);
                Eigen::Vector3f local_eig_val = local_solver.eigenvalues();

                float ratio = local_eig_val(0) / (initial_smallest_eig_value + 1e-12f);
                if (ratio <= threshold_eigen_value)
                {
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
        float planarity_score = (la_2 - la_1) / (la_3 + 1e-12f);

        PlanarPlan plan;
        plan.points = planar_plan_points;
        plan.eigen_vector = final_eig_vec.col(0);
        plan.planarity_score = planarity_score;

        planar_plans[plan_id++] = plan;
    }

    return planar_plans;
}

// -------------- FIXED final_version_normal_vector_estimation --------------
std::vector<PlanarPlan> final_version_normal_vector_estimation(
    std::vector<std::vector<float>> &X,
    const std::vector<std::vector<int>> &neighbor_indices, // pass neighbors
    int n_neighbors,
    float eigen_value_threshold,
    float angle_threshold_deg)
{
    // Call planar_segmentation_version_4 with correct args
    std::map<int, PlanarPlan> planar_map = planar_segmentation_version_4(X, neighbor_indices, n_neighbors, eigen_value_threshold);

    // Convert map -> vector for indexed access
    std::vector<PlanarPlan> planar_plans;
    planar_plans.reserve(planar_map.size());
    for (const auto &kv : planar_map)
        planar_plans.push_back(kv.second);

    size_t size = planar_plans.size();
    std::vector<PlanarPlan> merged_plans;
    std::set<size_t> used_indices;

    std::cout << "num plans: " << size << std::endl;

    for (size_t i = 0; i < size; ++i)
    {
        if (used_indices.count(i))
            continue;

        // keep points as vector<vector<float>>
        std::vector<std::vector<float>> current_points = planar_plans[i].points;
        Eigen::Vector3f current_normal = planar_plans[i].eigen_vector;
        float current_planarity_score = planar_plans[i].planarity_score;

        std::cout << "planarity: " << current_planarity_score << std::endl;

        for (size_t j = i + 1; j < size; ++j)
        {
            if (used_indices.count(j))
                continue;

            std::vector<std::vector<float>> next_points = planar_plans[j].points;
            Eigen::Vector3f next_normal = planar_plans[j].eigen_vector;

            float angle_deg = angle_between_normals(current_normal, next_normal) * 180.0f / M_PI;
            if (angle_deg < angle_threshold_deg)
            {
                std::cout << "Merging plan " << i << " with plan " << j
                          << " due to angle " << angle_deg << "°" << std::endl;

                // append next_points to current_points
                current_points.insert(current_points.end(), next_points.begin(), next_points.end());

                // update normal (simple average then normalize)
                current_normal = (current_normal + next_normal).normalized();

                // optionally we could update planarity_score (here we keep original)
                used_indices.insert(j);
            }
        }

        PlanarPlan merged_plan;
        merged_plan.points = std::move(current_points);
        merged_plan.eigen_vector = current_normal;
        merged_plan.planarity_score = current_planarity_score; // or recompute if you want

        merged_plans.push_back(std::move(merged_plan));
    }

    return merged_plans;
}
vector<vector<float>> ReadFromWebInput()
{
    string input;
    std::getline(std::cin, input);

    vector<vector<float>> data;
    vector<float> row;
    string num;
    bool insideNumber = false;

    for (char c : input)
    {
        if ((c >= '0' && c <= '9') || c == '.' || c == '-' || c == 'e')
        {
            num += c;
            insideNumber = true;
        }
        else
        {
            if (insideNumber)
            {
                row.push_back(std::stof(num));
                num.clear();
                insideNumber = false;
            }
            if (c == '}' || c == ']')
            {
                if (!row.empty())
                {
                    data.push_back(row);
                    row.clear();
                }
            }
        }
    }

    return data;
}

int main()
{
    vector<vector<float>> data = ReadFromWebInput();
    if (data.empty())
    {
        std::cerr << "No data provided." << std::endl;
        return 1;
    }
    std::vector<std::vector<float>> X = {
        {0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {0, 1, 0}, {1, 1, 0}, {2, 1, 0}, {0, 0, 1}, {1, 0, 1}, {2, 0, 1}, {0, 1, 1}, {1, 1, 1}, {2, 1, 1}};

    std::vector<float> y = {0.0f, 1.0f, 2.0f};

    KnnAlgorithm nneighbors = KnnAlgorithm(3);
    nneighbors.fit(data, y);

    vector<int> results = SortByDensity(nneighbors.neighbor_distances, 3);

    std::map<int, PlanarPlan> planar_map = planar_segmentation_version_4(data, nneighbors.neighbor_indices, 3, 2.0f);

    for (const auto &plan : planar_map)
    {
        std::cout << "Plan ID: " << plan.first << "\n";
        std::cout << "Points:\n";
        for (const auto &point : plan.second.points)
        {
            std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")\n";
        }
        std::cout << "Eigen Vector: (" << plan.second.eigen_vector[0] << ", "
                  << plan.second.eigen_vector[1] << ", "
                  << plan.second.eigen_vector[2] << ")\n";
        std::cout << "Planarity Score: " << plan.second.planarity_score << "\n\n";
    }

    // vector<PlanarPlan> merged = final_version_normal_vector_estimation(data, nneighbors.neighbor_indices, 3, 2.0f, 5.0f);

    // for (const auto &plan : merged)
    // {
    //     cout << plan.eigen_vector.transpose() << endl;
    //     for (const auto &point : plan.points)
    //     {
    //         std::cout << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")\n";
    //     }
    //     cout << "----\n";
    // }
}
