#!/bin/bash
tempo=0
replace_dot_with_comma () {
  echo "$tempo" | tr . ,
}


linha=""
backend=OpenMP
output_file="original.csv"
echo -n "" > "$output_file"

echo "Draco2 e Draco3 - 32 THREADS" >> "$output_file"
echo "tamanho;2-2;2-4;4-2;4-4" >> "$output_file"

export OMP_NUM_THREADS=32

for i in 1 2 3; do
    linha="original-344"
    versao="${send}_packing"
    tempo=$(./original.OpenMP.x TTI 344 344 344 16 12.5 12.5 12.5 0.000685 0.5)
    tempo_formatado=$(replace_dot_with_comma)
    linha="$linha;$tempo_formatado"
    echo "$linha" >> "$output_file"
    linha=""
done