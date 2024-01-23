#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*

cd ../original_times/

backend=OpenMP
version=original
echo "-----------------------------------------------------"
echo "   $version - $backend"
echo "-----------------------------------------------------"
make clean
make backend=$backend
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x