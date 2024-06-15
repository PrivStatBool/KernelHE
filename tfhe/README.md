# TFHE Implementation for k-Means and k-NN Algorithms

## Dependencies

To use the TFHE algorithms, you need to install the TFHE library. You can find the library and installation instructions here: [TFHE library](https://tfhe.github.io/tfhe/).

## After Installation

### Running k-Means Algorithms

To run all k-means algorithms in the cipher and plain domains, navigate to the `tfhe` directory and execute:

```sh
sh run_test_tfhe_kmeans.sh

This script runs both the kernel approach and the general approach in both the cipher and plain domains.
