#!/bin/bash

clang-format -i src/*.cpp 2>/dev/null
clang-format -i src/*.hpp 2>/dev/null
clang-format -i src/*.inl 2>/dev/null
mkdir -p build
cd build
cmake ..
if cmake --build . ; then
	ctest --verbose
fi

