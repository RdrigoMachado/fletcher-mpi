#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
backend=CUDA
version=spawn

cd ../$version/$version
mpicc -o spawn.x spawn.c
mv spawn.x ../../exp/bin/spawn.x
cd ..
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean-all
make backend=$backend version=$version 
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean-all

