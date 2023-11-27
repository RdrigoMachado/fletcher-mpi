#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=CUDA

cd ../original/
version=original
echo "-----------------------------------------------------"
echo "   $version"
echo "-----------------------------------------------------"
make clean
make backend=$backend
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x

cd ../2isend_recv/
version=2isend_recv
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version backend=$backend
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean
