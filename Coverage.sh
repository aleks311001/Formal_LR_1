#!/bin/bash

mkdir build
cd build
cmake ..
make Tests_LR_1
./Tests_LR_1
cd ..

lcov --directory ./build/CMakeFiles/Tests_LR_1.dir/ --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1
genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage/

rm code_coverage.info
rm -r build
