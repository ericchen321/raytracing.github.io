#!/bin/bash

rm -rf build/
mkdir build/
cd build/
cmake --target inOneWeekend -B ../
cd ../
cmake --build build