#!/bin/bash
time=0
replace_dot_with_comma () {
  echo "$time" | tr . ,
}


omp_threads=20
tamanho=440

export OMP_NUM_THREADS=$omp_threads

#create or make file empty
for version in original send_recv isend_recv pthread 2isend_recv many; do
    echo -n "" > "${tamanho}_${version}_${omp_threads}.csv"
done


#run tests

for version in original send_recv isend_recv pthread 2isend_recv many; do
    if test "$version" = "original"
    then
         time=$(./original.CUDA.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.001 1)
    elif test "$version" = "2isend_recv"
    then
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 3 ./$version.CUDA.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.001 1)
    elif test "$version" = "many"
    then
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 3 ./$version.CUDA.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.001 1 20 10)
    else
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 2 ./$version.CUDA.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.001 1)
    fi
    echo $(replace_dot_with_comma) >> "${tamanho}_${version}_${omp_threads}.csv"
done
