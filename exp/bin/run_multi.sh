#Variveis
VERSION=multi_send
SIZE=504
STEP=0.001
THRESHOLD=1.5

#Execucao
nsys profile -t cuda,mpi -o $VERSION.$SIZE.$THRESHOLD.HD mpirun -np 2 ./$VERSION.CUDA.x TTI $SIZE $SIZE $SIZE 16 12.5 12.5 12.5 $STEP $THRESHOLD