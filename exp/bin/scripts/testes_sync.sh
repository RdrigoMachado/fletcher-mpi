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

output_file="draco2_32_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 - 32 THREADS" >> "$output_file"
echo "tamanho;nocontrol-packing;nocontrol-nopacking;control-packing;control-nopacking" >> "$output_file"

export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
            versao="$comunicao-$empacotamento"
            tempo=$(mpirun -np 2 --use-hwthread-cpus --rankfile rankfile_1_maquina_hwtrhread ../$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
            tempo_formatado=$(replace_dot_with_comma)
            linha="$linha;$tempo_formatado"
        done
        done
        echo "$linha" >> "$output_file"
        linha=""
    done
done


#########################################################################################
#DRACO2 - (16 threads)
#########################################################################################

output_file="draco2_16_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 - 16 THREADS" >> "$output_file"
echo "tamanho;nocontrol-packing;nocontrol-nopacking;control-packing;control-nopacking" >> "$output_file"

export OMP_NUM_THREADS=16
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
            versao="$comunicao-$empacotamento"
            tempo=$(mpirun -np 2 --rankfile rankfile_1_maquina ../$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
            tempo_formatado=$(replace_dot_with_comma)
            linha="$linha;$tempo_formatado"
        done
        done
        echo "$linha" >> "$output_file"
        linha=""
    done
done


#########################################################################################
#DRACO2 e DRACO3 - HW (32 threads)
#########################################################################################

output_file="draco2_draco3_32_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 e Draco3 - 32 THREADS" >> "$output_file"
echo "tamanho;nocontrol-packing;nocontrol-nopacking;control-packing;control-nopacking" >> "$output_file"

export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
          versao="$comunicao-$empacotamento"
          tempo=$(mpirun -host draco2,draco3 -np 2 --use-hwthread-cpus --rankfile rankfile_2_maquinas_hwthread ../$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
          tempo_formatado=$(replace_dot_with_comma)
          linha="$linha;$tempo_formatado"
        done
        done
        echo "$linha" >> "$output_file"
        linha=""
done


#########################################################################################
#DRACO2 e DRACO3 - HW (16 threads)
#########################################################################################

output_file="draco2_draco3_16_threads.csv"
echo -n "" > "$output_file"

echo "Draco2 e Draco3 - 16 THREADS" >> "$output_file"
echo "tamanho;nocontrol-packing;nocontrol-nopacking;control-packing;control-nopacking" >> "$output_file"
#Gerar dados rodando em uma maquina - Todas as threads
export OMP_NUM_THREADS=16
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
          versao="$comunicao-$empacotamento"
          tempo=$(mpirun -host draco2,draco3 -np 2 --rankfile rankfile_2_maquinas ../$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
          tempo_formatado=$(replace_dot_with_comma)
          linha="$linha;$tempo_formatado"
        done
        done
        echo "$linha" >> "$output_file"
        linha=""
done
