#  Internship Report: Normal Vector Estimation  

![School Logo](ensia_logo.png)  
  

![CDTA Logo](logo-cdta.png)  


##  Internship Details  

- **Student Name**: Bahfir Khadidja  
- **Supervisor Name**: Tchantchane Zahida  
- **Duration**: June – July 2025  
- **Institution**: [Your School Name] in collaboration with CDTA  

---

##  Introduction  

Estimating normal vectors from 3D point clouds is a fundamental task in computer vision, computer graphics, and 3D data processing. Despite its importance, the problem remains challenging due to the presence of noise, outliers, non-uniform sampling densities, and sharp geometric features.  

Traditional approaches, such as Principal Component Analysis (PCA)-based methods, often struggle to deliver accurate results under these conditions. To overcome these limitations, recent research has introduced a variety of improved techniques. Some approaches extend classical geometry-based methods by integrating robust statistical measures, while others adopt learning-based strategies, including neural networks and graph neural networks, to adaptively estimate normals.  

For example, **DeepFit** combines neural networks with weighted least squares to improve robustness, while **HSurf-Net** employs implicit hypersurface representations to predict normals directly. Collectively, these works reflect the field’s transition from purely geometric formulations toward more flexible, data-driven solutions.  

This report presents the work carried out during the internship, which included literature review, implementation, testing, and deployment. The adopted approach was based on **eigenvalue-driven region growing combined with angle-based merging of regions**, offering a balance between geometric interpretability and computational efficiency.  

---

##  Literature Review  

- **Traditional PCA methods**: Efficient but sensitive to noise, outliers, and sharp features.  
- **Learning-based methods**:  
  - *DeepFit [2]* → Neural networks + weighted least squares for robust normals.  
  - *Deep Iterative Surface Normal Estimation [3]* → Graph neural networks with adaptive anisotropic kernels.  
  - *HSurf-Net [5]* → Implicit hypersurface learning with MLPs for direct normal prediction.  
- **Robust Geometry-based methods**:  
  - *Robust and Edge-Aware Normal Estimation [4]* → Robust PCA with feature-awareness for handling outliers.  

 *Conclusion*: The field is shifting toward **data-driven solutions**, but classical methods remain valuable due to their efficiency and simplicity.  

---

## Methodology  

The implemented method is based on **region-growing segmentation with eigenvalue analysis**:  

1. **Seeding**  
   - Highest Local Planarity (eigenvalue-based)  
   - Density-Based Seeding  
   - k-NN Seeding  

2. **Planar Segmentation (Region Growing)**  
   - Incrementally adds points if eigenvalue ratio conditions are satisfied.  
   - Planarity Score is computed as:  

   \[
   \text{Planarity Score} = \frac{\lambda_2 - \lambda_1}{\lambda_3}, \quad \lambda_1 \leq \lambda_2 \leq \lambda_3
   \]  

3. **Merging Step**  
   - Regions with similar normals are merged if the angle between them is below a threshold.  

4. **Final Output**  
   - A set of merged planar regions with **estimated normal vectors** and **planarity scores**.  

---

##  Deployment & Usage  

A **web application** was deployed to make the algorithm accessible and interactive:  

🔗 [Normal Vector Estimation Platform](https://normal-vector-estimation-1.onrender.com/)  

- **Input format**: JSON-like 3D points (using curly braces)  

Example input:  
```json
{0, 0, 0}, {1, 0, 0}, {2, 0, 0},  
{0, 1, 0}, {1, 1, 0}, {2, 1, 0},  
{0, 0, 1}, {1, 0, 1}, {2, 0, 1},  
{0, 1, 1}, {1, 1, 1}, {2, 1, 1}
