#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

mkdir -p bin/
rm -r bin/*
backend=OpenMP

cd ../original/
for comunicao in nocontrol control i_nocontrol; do
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
