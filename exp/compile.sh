#!/usr/bin/env bash

#set -o errexit -o nounset -o pipefail -o posix

mkdir -p bin/
cd ../

cd original
for backend in OpenMP; do
	echo "-----------------------------------------------------"
	echo "  Original - $backend"
	echo "-----------------------------------------------------"
	make clean
	make backend=$backend
	mv ModelagemFletcher.exe ../exp/bin/original.$backend.`hostname`.x
done
cd ..