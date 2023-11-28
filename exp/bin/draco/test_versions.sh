#!/bin/bash
time=0
replace_dot_with_comma () {
  echo "$time" | tr . ,
}


omp_threads=16

#create or make file empty
for size in 504; do
for version in original send_recv isend_recv pthread 2isend_recv; do
    echo -n "" > "${size}_${version}_${omp_threads}.csv"
done
done


#run tests
export OMP_NUM_THREADS=16
for size in 504; do
for i in 1 2 3; do
for version in original send_recv isend_recv pthread 2isend_recv; do
    if test "$version" = "original" 
    then
        time=$(./original.OpenMP.x TTI $size $size $size 16 12.5 12.5 12.5 0.001 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    elif test "$version" = "2isend_recv"
    then
        time=$(mpirun --use-hwthread-cpus -np 3 ./$version.OpenMP.x TTI $size $size $size 16 12.5 12.5 12.5 0.001 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    else
        time=$(mpirun --use-hwthread-cpus -np 2 ./$version.OpenMP.x TTI $size $size $size 16 12.5 12.5 12.5 0.001 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    fi
done
done
done
