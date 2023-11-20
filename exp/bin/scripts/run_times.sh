#!/bin/bash
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}


linha=""
backend=OpenMP


output_file="times.txt"
echo -n "" > "$output_file"


export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        for versao in original multiple_files; do
            if test "$versao" = "original"
            then
                saida="original"
                saida+=$(./original.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.1)
                echo "$saida" >> "$output_file"
            else
                saida="multipl_files"
                saida+=$(mpirun --hostfile hostfile  --rankfile 2_maquinas_receivers  --use-hwthread-cpus -np 3 ./multiple_files.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.1)
                echo "$saida" >> "$output_file"
            fi
            echo "" >> "$output_file"
        done
    done
done