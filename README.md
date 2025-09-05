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



```bash
cd OpenChromDist/
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B ./cmake-build-debug
cmake --build ./cmake-build-debug
