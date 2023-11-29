#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=CUDA

cd ../mpi_async_multi_many_receivers/
version=multiple_files
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version backend=$backend
mv ModelagemFletcher.exe ../exp/bin/many.$backend.x
make clean
