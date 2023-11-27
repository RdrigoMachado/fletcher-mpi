#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP

cd ../times/original_times/
version=original
echo "-----------------------------------------------------"
echo "   $version"
echo "-----------------------------------------------------"
make backend=$backend
mv ModelagemFletcher.exe ../../exp/bin/$version.$backend.x
make clean

cd ../2isend_recv_times/
version=2isend_recv
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make version=$version backend=$backend
mv ModelagemFletcher.exe ../../exp/bin/$version.$backend.x
make clean
