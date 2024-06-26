#Variveis
SIZE=504
STEP=0.001
THRESHOLD=1.5

#Execucao
for VERSION in spawn_all_at_once spawn_one_at_time; do
    for PROCESSOS in 2 4 8 16 32; do
        nsys profile -t cuda,mpi -o $VERSION.$PROCESSOS.$SIZE.$THRESHOLD.SSD mpirun -np 1 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD $PROCESSOS
    done
done