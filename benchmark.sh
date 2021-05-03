#!/bin/bash

cp src/$1.cpp  test/reasoner.cpp
cp src/$1.hpp  test/reasoner.hpp

make
echo "finished compiling";
mkdir -p logs

./benchmark 1000 > logs/$1.log