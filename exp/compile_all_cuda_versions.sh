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
make backend=$backend CC=gcc
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x

for version in pthread isend_recv send_recv 2isend_recv; do

    cd ../$version/
    echo "-----------------------------------------------------"
    echo "  $version"
    echo "-----------------------------------------------------"
    make clean
    make version=$version backend=$backend
    mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
    make clean

done