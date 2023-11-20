#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP

cd ../original/
version=original
echo "-----------------------------------------------------"
echo "   $version"
echo "-----------------------------------------------------"
make clean
make backend=$backend
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x

cd ../pthread/
version=pthread
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean

cd ../isend_recv/
version=isend_recv
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean

cd ../send_recv/
version=send_recv
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean

cd ../2isend_recv/
version=2isend_recv
echo "-----------------------------------------------------"
echo "  $version"
echo "-----------------------------------------------------"
make clean
make version=$version
mv ModelagemFletcher.exe ../exp/bin/$version.$backend.x
make clean