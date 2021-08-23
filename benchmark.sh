#!/bin/bash

cp src/$1.cpp  test/reasoner.cpp
cp src/$1.hpp  test/reasoner.hpp

if [ -f benchmark ]; then
  make clean-benchmark
fi

if [ "d$2" = "dmonotonic" ]; then
  make benchmark-monotonic
else
  make benchmark
fi

echo "finished compiling";
