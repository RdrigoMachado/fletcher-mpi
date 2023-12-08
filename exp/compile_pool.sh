#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP
for version in isend_pool; do

    cd ../$version/
    echo "-----------------------------------------------------"
    echo "  $version"
    echo "-----------------------------------------------------"
    make clean-all
    make backend=$backend version=$version 
    mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
    make clean-all

done
