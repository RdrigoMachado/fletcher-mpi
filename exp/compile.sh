#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

clear
mkdir -p bin/
rm bin/*.x
rm bin/*.rsf*
backend=OpenMP


cd ../mpi_sync/

echo "-----------------------------------------------------"
echo "MPI_SYNC"
echo "-----------------------------------------------------"

for comunicao in nocontrol control; do
	cd $comunicao
	pwd
	for empacotamento in packing nopacking; do
		versao="$comunicao-$empacotamento"
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

cd ./mpi_async/

echo "-----------------------------------------------------"
echo "MPI_ASYNC"
echo "-----------------------------------------------------"

for comunicao in i_nocontrol i_control; do
	cd $comunicao
	pwd
	for empacotamento in packing; do
		versao="$comunicao-$empacotamento"
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