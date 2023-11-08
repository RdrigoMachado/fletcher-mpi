#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP


cd ../receivers_simple_times/
pwd

echo "-----------------------------------------------------"
echo "MPI MULTIPLE CONNECTIONS"
echo "-----------------------------------------------------"
#for comunicao in i_nocontrol i_control; do
for versao in multiple_files; do
		echo "-----------------------------------------------------"
		echo "  OpenMP - $versao"
		echo "-----------------------------------------------------"
		make clean
		make versao=$versao
		mv ModelagemFletcher.exe ../exp/bin/$versao.$backend.x
		make clean
done
cd ..
