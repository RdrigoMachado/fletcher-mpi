#!/bin/bash

# Fun    o para substituir o ponto "." por v  rgula ","
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}

cd bin
# Arquivo para salvar as linhas que come  am com "Execution time (s) is"
output_file="output.csv"

# Limpa o conte  do do arquivo antes de executar o programa
echo -n "" > "$output_file"
#cabecalho
echo "num_threads;tamanho;tempo" > "$output_file"

# Loop para executar o programa com diferentes par  metros
for num_threads in 16 32; do
    export OMP_NUM_THREADS=$num_threads
    for tamanho in 56 180 344; do

        for comunicao in nocontrol control; do
        for empacotamento in packing nopacking; do
            versao="$comunicao-$empacotamento"

            for i in 1 2 3; do
                tempo=$($versao.$backend.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.07)
                tempo_formatado=$(replace_dot_with_comma)

                printf "%s;%s;%s\n" \
                            "$num_threads" "$tamanho" "$tempo_formatado" >> "$output_file"

            done
        
        done
        done
   done
done