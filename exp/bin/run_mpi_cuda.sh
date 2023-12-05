#Variveis
VERSION=pthread
SIZE=440
STEP=0.001
THRESHOLD=1

#Execucao
mpirun --rankfile rankfile -np 2 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD
