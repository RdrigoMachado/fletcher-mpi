#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=CUDA

for version in spawn; do

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

done
