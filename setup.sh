rm -rf build
mkdir build
cd build

CXX=clang++ CC=clang cmake \
  -DCMAKE_BUILD_TYPE=Release \
  ..

cd ..
