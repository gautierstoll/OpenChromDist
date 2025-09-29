# OpenChromDist

C++ API for estimating gaussian-based open chromatin distribution 
from ATAC seq data

Current version number in CMakeLists.txt

Doxygen generated [doc](https://gautierstoll.github.io/OpenChromDist/index.html)

## 🛠 Features

- API for distances between single cell ATAC peaks, based on cumulative probability estimation of open chromatin.


## 📦 Build and Run

### Requirements

- CMake ≥ 3.28
- ninja ≥ 1.11.1
- C++ compiler (e.g., GCC, Clang)

### Build


Fast compilattion
```bash
cd OpenChromDist/
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B ./cmake-build-debug
cmake --build ./cmake-build-debug
```

Optimzed speed
```bash
cd OpenChromDist/
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B ./cmake-build-release
cmake --build ./cmake-build-release
```

### Usage
#### For creating distribution:
```bash
OpenChromDist chromFile barCodeFile fragFile windEval outBindFile
```
chromFile contains

CHROMOSOME=

CHRLENGTH=

BPSTEP=

WINDSIZE=

fragFile is a list a ATAC hits.

barCodeFile contains a list of barcodes separated by newline.

windEval is the window size (in bp) on which probability distribution is evaluated given a peak.
It is an estimation parameters that should be mixed with WINDSIZE which defines the sd of the distribution.

With no output binary file, distance matrix is directly computed.

#### For computing distance matrix from binary file of distribution:
```bash
OpenChromDist -l binFile
```
 
#### For help:
```bash 
OpenChromDist -h
```