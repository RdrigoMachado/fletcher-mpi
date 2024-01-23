#Variveis
VERSION=send_time
SIZE=504
STEP=0.001
THRESHOLD=1.5

#Execucao

saida=""
output_file="send_times.txt"


for i in 1 2 3 4 5; do
	saida=$(mpirun --rankfile rankfile --use-hwthread-cpus -np 2 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD)
	rm TTI*
        echo "$saida" >> "$output_file"
	echo "" >> "$output_file"
done


saida=""
output_file="original_times.txt"


for i in 1 2 3 4 5; do
        saida=$(./original.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD)
        rm TTI*
        echo "$saida" >> "$output_file"
        echo "" >> "$output_file"
done

