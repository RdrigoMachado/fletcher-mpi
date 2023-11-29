#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP

cd ../original-mpi/
version=original
echo "-----------------------------------------------------"
echo "   $version"
echo "-----------------------------------------------------"
make clean-all
make backend=$backend CC=mpicc
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean-all
