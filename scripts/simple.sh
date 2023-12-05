#!/bin/bash
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}


linha=""
backend=OpenMP


#########################################################################################
#DRACO2 - HW (32 threads)
#########################################################################################

output_file="many_draco2_32_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 - 32 THREADS" >> "$output_file"
echo "tamanho;2-2;2-4;4-2;4-4" >> "$output_file"

export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        tempo=$(mpirun --rankfile 1_maquina_receivers  --use-hwthread-cpus -np 3 ./multiple_files.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
        tempo_formatado=$(replace_dot_with_comma)
        linha="$linha;$tempo_formatado"
        echo "$linha" >> "$output_file"
        linha=""
    done
done

#########################################################################################
#DRACO2 e DRACO3 - HW (32 threads)
#########################################################################################

output_file="many_draco2_draco3_32_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 e Draco3 - 32 THREADS" >> "$output_file"
echo "tamanho;2-2;2-4;4-2;4-4" >> "$output_file"

export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        versao="${send}_packing"
        tempo=$(mpirun --hostfile hostfile --rankfile 2_maquinas_receivers  --use-hwthread-cpus -np 3 ./multiple_files.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
        tempo_formatado=$(replace_dot_with_comma)
        linha="$linha;$tempo_formatado"
        echo "$linha" >> "$output_file"
        linha=""
    done
done