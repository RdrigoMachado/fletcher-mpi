#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP


cd ../mpi_async_multi/

echo "-----------------------------------------------------"
echo "MPI_SYNC"
echo "-----------------------------------------------------"
#for comunicao in i_nocontrol i_control; do
for comunicacao in i_nocontrol; do
	cd $comunicacao
	pwd
	for send in isend_irecv; do
		versao="${send}_packing"
		echo "-----------------------------------------------------"
		echo "  OpenMP - $versao"
		echo "-----------------------------------------------------"
		make clean
		make versao=$versao
		mv ModelagemFletcher.exe ../../exp/bin/$versao.$backend.x
		make clean
	done
	cd ..
done
cd ..
