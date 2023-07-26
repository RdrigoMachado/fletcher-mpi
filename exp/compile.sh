#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

mkdir -p bin/
rm -r bin/*
cd ../

cd original
for comunicao in nocontrol control; do
	for empacotamento in packing nopacking; do
		versao="$comunicao.$empacotamento"
		echo "-----------------------------------------------------"
		echo "  OpenMP - $versao"
		echo "-----------------------------------------------------"
		make clean
		make versao=$versao
		mv ModelagemFletcher.exe ../exp/bin/$versao.$backend.x
		make clean
	done
done
cd ..
