#!/bin/bash
time=0
replace_dot_with_comma () {
  echo "$time" | tr . ,
}


omp_threads=20

#create or make file empty
for size in 504; do
for version in original send_recv isend_recv pthread 2isend_recv many; do
    echo -n "" > "${size}_${version}_${omp_threads}.csv"
done
done


#run tests
export OMP_NUM_THREADS=20
for size in 504; do
for i in 1; do
for version in original send_recv isend_recv pthread 2isend_recv many; do
    if test "$version" = "original"
    then
        time=$(./original.CUDA.x TTI $size $size $size 16 12.5 12.5 12.5 0.000685 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    elif test "$version" = "2isend_recv"
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 3 ./$version.CUDA.x TTI $size $size $size 16 12.5 12.5 12.5 0.000685 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    elif test "$version" = "many"
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 3 ./$version.CUDA.x TTI $size $size $size 16 12.5 12.5 12.5 0.000685 0.5 20 10)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    else
        time=$(mpirun --hostfile hostfile  --rankfile rankfile  --use-hwthread-cpus -np 2 ./$version.CUDA.x TTI $size $size $size 16 12.5 12.5 12.5 0.000685 0.5)
        echo $(replace_dot_with_comma) >> "${size}_${version}_${omp_threads}.csv"
    fi
done
done
done
