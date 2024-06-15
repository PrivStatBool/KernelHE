# TFHE Implementation for k-Means and k-NN Algorithms

## Dependencies

To use the TFHE algorithms, you need to install the `tfhe` library. You can find the library and installation instructions here: [TFHE library](https://tfhe.github.io/tfhe/).

If you prefer an installation guide for ubuntu 20.04, refer to the end of this README file.

## After Installation

### Running k-Means Algorithms

To run all k-means algorithms in the cipher and plain domains, navigate to the `tfhe` directory and execute:

```sh
sh run_test_tfhe_kmeans.sh
```

This script runs both the kernel approach and the general approach in both the cipher and plain domains.

### Running k-NN Algorithms

To run all k-NN algorithms in the cipher and plain domains, navigate to the tfhe directory and execute:

```sh
sh run_test_tfhe_knn.sh
```

This script also runs both the kernel approach and the general approach in both the cipher and plain domains.

### Separate Execution

If you want to run the kernel and general approaches separately in either the cipher or plain domains, navigate to the specific directory and execute the appropriate test script:

```
sh (test_file_execution_script)
```

### Dataset
All generated datasets are located in their respective sections of the folders (cipher and plain).

### Installation Guide (Detailed Step for TFHE)

## Prerequisites

Before installing `tfhe`, ensure your system meets the following requirements:

- **C++ Compiler:** A compiler supporting C++11 standards, such as g++ (version 5.2 or higher) or clang (version 3.8 or higher). This is necessary for compiling the core `tfhe` library.

### TFHE Installation

1. **Install Compilers:**
   ```shell
   sudo apt-get update
   sudo apt-get install g++ clang

2. **Clone TFHE:**
   ```shell
   git clone https://github.com/tfhe/tfhe.git

3. **Build TFHE:**
- Navigate to the 'tfhe' directory and build:
   ```shell
  cd tfhe
  mkdir build && cd build
  ccmake ../src

4. **Set Environment Variables:**
- Add the following to ~/.bashrc (adjust TFHE_PREFIX if necessary):
   ```shell
  export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/include
  export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/include
  export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/lib
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

- Reload ~/.bashrc:
   ```shell
   source ~/.bashrc
