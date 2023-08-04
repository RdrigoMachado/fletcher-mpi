#!/bin/bash

# Fun    o para substituir o ponto "." por v  rgula ","
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}

# Arquivo para salvar as linhas que come  am com "Execution time (s) is"
output_file="draco23_hw.csv"

# Limpa o conte  do do arquivo antes de executar o programa
echo -n "" > "$output_file"
backend=OpenMP
linha=""

echo "tamanho;nocontrol-packing;nocontrol-nopacking;control-packing;control-nopacking" > "$output_file"
echo "Draco2 e Draco3 - HW THREAD" > "$output_file"
#Gerar dados rodando em uma maquina - Todas as threads
export OMP_NUM_THREADS=32
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
            versao="$comunicao-$empacotamento"
            tempo=$(mpirun -host draco2,draco3 -np 2 --use-hwthread-cpus --rankfile rankfile_2_maquinas_hwthread ./$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
            tempo_formatado=$(replace_dot_with_comma)
            linha="$linha;$tempo_formatado"
        done
        done
        echo "$linha" >> "$output_file"
        linha=""
    done
done