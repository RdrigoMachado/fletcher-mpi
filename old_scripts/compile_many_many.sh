#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP

cd ../mpi_async_multi_many_receivers/
version=multiple_files
echo "-----------------------------------------------------"
echo "   $version"
echo "-----------------------------------------------------"
make clean
make backend=$backend CC=mpicc version=$version
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean
