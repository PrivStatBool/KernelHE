# CKKS Implementation for Machine Learning Algorithms

## Dependencies

To use the CKKS algorithms, you need to install the `OpenFHE` library. You can find the library and installation instructions here: [OpenFHE library](https://github.com/openfheorg/openfhe-development).

If you prefer an installation guide for Ubuntu 20.04, refer to the end of this README file.

## After Installation

### Running Algorithms

To run all algorithms in the cipher and plain domains, navigate to the `ckks` directory and execute the following commands:

1. **Create build directory**:
 ```
 mkdir build
 cd build
 ```

2. **Run CMake**:
```sh
cmake ..
```

3. **Build the project**:
```sh
make
```

This will build the executables for both the kernel approach and the general approach in both the cipher and plain domains.

### Separate Execution
- If you want to run the kernel and general approaches separately in either the cipher or plain domains, navigate to the specific directory and execute the appropriate test script. For example:
```sh
cd build/plain/distance
./distance
```

### Dataset
All generated datasets are located in their respective sections of the source codes (cipher and plain).

## Installation Guide (Detailed Step for OpenFHE)
### Prerequisites
Before installing OpenFHE, ensure your system meets the following requirements:
- C++ Compiler: A compiler supporting C++11 standards, such as g++ (version 5.2 or higher) or clang (version 3.8 or higher). This is necessary for compiling the core OpenFHE library.
OpenFHE Installation

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
