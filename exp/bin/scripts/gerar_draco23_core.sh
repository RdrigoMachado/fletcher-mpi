#!/bin/bash

# Fun    o para substituir o ponto "." por v  rgula ","
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}

# Arquivo para salvar as linhas que come  am com "Execution time (s) is"
output_file="i_draco23_core.csv"

# Limpa o conte  do do arquivo antes de executar o programa
echo -n "" > "$output_file"
backend=OpenMP
linha=""

#cabecalho
echo "Draco2 e Draco3 - SOMENTE CORE FISICO" > "$output_file"
#Gerar dados rodando em uma maquina - Todas as threads
export OMP_NUM_THREADS=16
for tamanho in 56 180 344; do
    for i in 1 2 3; do
        linha="$tamanho"
        versao="i_nocontrol-packing"
        tempo=$(mpirun -host draco2,draco3 -np 2 --rankfile rankfile_2_maquinas ./$versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
        tempo_formatado=$(replace_dot_with_comma)
        linha="$linha;$tempo_formatado"
        echo "$linha" >> "$output_file"
        linha=""
    done
done
