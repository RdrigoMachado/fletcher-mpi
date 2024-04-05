#Variveis
SIZE=56
STEP=0.001
THRESHOLD=0.5

#Execucao
for VERSION in spawn_all_at_once spawn_one_at_time; do
        for PROCESSOS in 2 4 8; do
                output_file="$VERSION.$PROCESSOS.csv"
                saida="Computacao;Escrita;Total"
                echo "$saida" >> "$output_file"
                for i in 1 2 3 4 5 6 7 8 9 10; do
                        saida=""
                        saida=$(mpirun -np 1 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD $PROCESSOS)
                        rm TTI*
                        echo "$saida" >> "$output_file"
                done
        done
done
