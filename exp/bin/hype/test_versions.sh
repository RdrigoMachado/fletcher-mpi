#!/bin/bash
time=0
replace_dot_with_comma () {
  echo "$time" | tr . ,
}


omp_threads=20

#create or make file empty
for size in 504; do
for version in multiple_files; do
    echo -n "" > "${size}_${version}_${omp_threads}.csv"
done
done


#run tests
export OMP_NUM_THREADS=20
for size in 504; do
for i in 1 2 3; do
for version in multiple_files; do
       time=$(mpirun --hostfile hostfile --rankfile rankfile --use-hwthread-cpus -np 3 ./$version.OpenMP.x TTI $size $size $size 16 12.5 12.5 12.5 0.001 0.5 10 5)
       echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
done
done
done
