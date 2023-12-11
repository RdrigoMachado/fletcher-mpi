#Variveis
VERSION=pthread
SIZE=504
STEP=0.001
THRESHOLD=1.5

#Execucao
mpirun --rankfile rankfile --use-hwthread-cpus -np 2 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD
