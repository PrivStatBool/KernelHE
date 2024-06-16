# CKKS Implementation for Machine Learning Algorithms

## Dependencies

To use the CKKS algorithms, you need to install the `OpenFHE` library **v.1.0.2**. You can find the library and installation instructions here: [OpenFHE library](https://github.com/openfheorg/openfhe-development). If you prefer an installation guide for Ubuntu 20.04, refer to the end of this README file.

### After Installation

To run the source codes of ML/STAT algorithms using the OpenFHE library, follow these steps:

1. **Navigate to the OpenFHE Development Folder:**
   Ensure you are in the OpenFHE development directory where the library has been installed.

2. **Replicate `CMakeLists.User.txt`:**
   You need to replicate `CMakeLists.User.txt` into the target (ML/STAT) folder for execution. This file contains user-specific configurations required by CMake to build the project correctly.

3. **Run the Algorithms:**
   Execute the ML/STAT algorithms by navigating to the respective directories and using CMake to build and run the code. 

For detailed instructions on how to run the openfhe, refer to the [OpenFHE library documentation](https://github.com/openfheorg/openfhe-development).


### Dataset
All generated datasets are located in their respective sections of the source codes (cipher and plain).

## Installation Guide (Detailed Step for OpenFHE)
### Prerequisites
Before installing OpenFHE, ensure your system meets the following requirements:
- C++ Compiler: A compiler supporting C++11 standards, such as g++ (version 5.2 or higher) or clang (version 3.8 or higher). This is necessary for compiling the core OpenFHE library.

#### OpenFHE Installation
1. Install Compilers:
```sh
sudo apt-get update
sudo apt-get install build-essential cmake
```

2. (Optional) Install clang:

```sh
sudo apt-get install clang-11
sudo apt-get install libomp5 libomp-dev
```

3. Configure clang/clang++ as the default compiler for C and C++:
```sh
export CC=/usr/bin/clang-11
export CXX=/usr/bin/clang++-11
```

4. Clone OpenFHE:
```sh
git clone https://github.com/openfheorg/openfhe-development.git
```

5. Create build directory and run CMake:
```sh
cd openfhe-development
mkdir build
cd build
cmake ..
```

- Note: cmake will check for any system dependencies that are needed for the build process.

6. Build OpenFHE:
```sh
make
```

7. Set Environment Variables:
- Add the following to ~/.bashrc (adjust OpenFHE_PREFIX if necessary):

```sh
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/include
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

- Reload ~/.bashrc:
```sh
source ~/.bashrc
```
