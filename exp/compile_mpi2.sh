#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
backend=CUDA

for version in spawn_all_at_once spawn_one_at_time spawn_one_for_each_write; do
    cd ../$version/$version
    mpicc -o $version.x spawn.c
    mv $version.x ../../exp/bin/$version.x
    cd ..
    echo "-----------------------------------------------------"
    echo "  $version"
    echo "-----------------------------------------------------"
    make clean-all
    make backend=$backend version=$version 
    mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
    make clean-all
done
