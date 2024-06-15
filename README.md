# Speed-up of Data Analysis with Kernel Trick in Encrypted Domain

## Overview

This repository contains the source code for the research paper "Speed-up of Data Analysis with Kernel Trick in Encrypted Domain". The code is divided into two main parts: CKKS and TFHE, each addressing ML/STAT algorithms in the encrypted domain.

## Structure of the Repository

The repository is structured as follows:

### CKKS

The `ckks` directory contains the implementation of several data analysis algorithms using the CKKS scheme. These algorithms include:

- **Support Vector Machine (SVM)**
- **Principal Component Analysis (PCA)**
- **Total Variance**
- **Distance Calculations**
- **Norm Calculations**
- **Similarity Measures**

The source files are organized to facilitate the evaluation of these algorithms in an encrypted domain using the CKKS homomorphic encryption scheme.

### TFHE

The `tfhe` directory focuses on the implementation of the k-means and k-nearest neighbors (k-NN) algorithms using the TFHE scheme. The directory is further divided into the following subdirectories:

- **kmeans**: Contains the source code and scripts related to the k-means clustering algorithm.
- **knn**: Contains the source code and scripts related to the k-NN classification algorithm.

Each subdirectory contains the necessary source code, test scripts, and data files required to run and evaluate the algorithms.
