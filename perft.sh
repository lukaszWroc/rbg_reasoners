#!/bin/bash

cp src/$1.cpp  test/reasoner.cpp
cp src/$1.hpp  test/reasoner.hpp

if [ -f perft ]; then
  make clean-perft
fi

make perft

echo "finished compiling";

