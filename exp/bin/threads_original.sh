#!/bin/bash
saida=""
replace_dot_with_comma () {
  echo "$saida" | tr . ,
}


linha=""
backend=OpenMP


#########################################################################################
#Hype 1 e 2  - HW (32 threads)
#########################################################################################

output_file="threads_32.csv"
echo -n "" > "$output_file"

echo "Hype 1 e 2 - 32 THREADS - 180" >> "$output_file"
echo "" >> "$output_file"

export OMP_NUM_THREADS=32
for tamanho in 180; do
for versao  in original.OpenMP.x multiple_files.OpenMP.x; do
    for i in 1 2 3; do
        if test "$versao" = "multiple_files.OpenMP.x"
        then 
            echo "MPI" >> "$output_file"
            saida=$(mpirun --hostfile hostfile --rankfile 2_maquinas_receivers  --use-hwthread-cpus -np 3 ./multiple_files.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.5)
            saida_formatada=$(replace_dot_with_comma)
        else
            echo "Naive" >> "$output_file"
            saida=$(./original.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.5)
            saida_formatada=$(replace_dot_with_comma)
        fi
        echo "$saida_formatada" >> "$output_file"
        echo "" >> "$output_file"
    done
done
done

#########################################################################################
#Hype 1 e 2  - HW (16 threads)
#########################################################################################

output_file="threads_16.csv"
echo -n "" > "$output_file"

echo "Hype 1 e 2 - 16 THREADS - 180" >> "$output_file"
echo "" >> "$output_file"

export OMP_NUM_THREADS=16
for tamanho in 180; do
for versao  in original.OpenMP.x multiple_files.OpenMP.x; do
    for i in 1 2 3; do
        if test "$versao" = "multiple_files.OpenMP.x"
        then 
            echo "MPI" >> "$output_file"
            saida=$(mpirun --hostfile hostfile --rankfile 2_maquinas_receivers  --use-hwthread-cpus -np 3 ./multiple_files.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.5)
            saida_formatada=$(replace_dot_with_comma)
        else
            echo "Naive" >> "$output_file"
            saida=$(./original.OpenMP.x TTI $tamanho $tamanho $tamanho 16 12.5 12.5 12.5 0.000685 0.5)
            saida_formatada=$(replace_dot_with_comma)
        fi
        echo "$saida_formatada" >> "$output_file"
        echo "" >> "$output_file"
    done
done
done
