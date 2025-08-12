# Normal Vector Estimation

This repository presents an approach for **normal vector estimation** in 3D point cloud data.  
Our goal is to provide both a **research-oriented** and **production-ready** implementation, starting from early prototypes in notebooks to optimized C++ code with an interface for practical use.

---

## 📂 Repository Structure

### 1. **base_utils/**
Contains the **base model implementation** that serves as the foundation for all methods explored in this project.  
All normal estimation algorithms implemented here **inherit** from this base model, ensuring consistency and reusability across different approaches.

---

### 2. **interface/**
Includes the **interface layer** for estimating the normal vector.  
This allows developers and researchers to integrate the normal estimation functionality into their own workflows without diving into the low-level implementation details.

---

### 3. **notebooks/**
Contains **Jupyter notebooks** used for:
- Algorithm prototyping
- Testing against various **edge cases**
- Visualizing and analyzing results

These notebooks document the exploratory phase of development and provide insights into our design decisions.

---

### 4. **paper/**
Houses the **research papers** and related documentation associated with this project.  
These papers detail:
- The mathematical background
- Methodology
- Experimental results
- Comparisons with other approaches

---

### 5. **V1/**
Contains the **C++ implementation** of the algorithm for efficient computation.  
This version has been tested extensively and is intended for **production-level deployment**.  
It can be compiled into an executable and interfaced with other programming languages.

---

## 🚀 Getting Started

### Prerequisites
- **C++17** or later
- [Eigen3](https://eigen.tuxfamily.org/) (for linear algebra operations)

### Building the C++ Version
run the following commands : 
```bash
make 
./main 
```
### Running The Interface : 
&nbsp;&nbsp;&nbsp; In this section we will talk about how to run the interface. 

```bash 
pip install flask 
pip install flask-cors 
python ./interface/server.py
```
after running this you can access the website using the interface.html 

