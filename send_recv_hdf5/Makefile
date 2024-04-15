# Environment and compilation flags at Exaflop
#   module load gompi

# Default
backend=CUDA
arch=$(backend)
version=send_recv_hdf5

include config.mk
include $(arch)/flags.mk

OBJ1=\
	source.o \
	utils.o \
	comunicacao.mpi.o \
	boundary.o \
	walltime.o \
	model.o \
	map.o

ifdef PAPI
	LIBS += $(PAPI_LIBS)
	OBJ1 += ModPAPI.o
	CFLAGS += -DPAPI
endif

OBJ=main.o $(OBJ1)

TARGET = ModelagemFletcher.exe

all: 	$(OBJ)
	cd $(arch) && make
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(arch)/*.o $(LIBS) -L./hdf5/lib -lhdf5 -I./hdf5/include

main.o:	main.c $(OBJ1)
	$(CC) -c $(CFLAGS) main.c -L./hdf5/lib -lhdf5 -I./hdf5/include

boundary.o:	boundary.c boundary.h  map.o
	$(CC) -c $(CFLAGS) boundary.c

source.o:	source.c source.h
	$(CC) -c $(CFLAGS) source.c

comunicacao.mpi.o:	$(version)/comunicacao.mpi.c comunicacao.mpi.h
	$(CC) -c $(CFLAGS) $(version)/comunicacao.mpi.c -L./hdf5/lib -lhdf5 -I./hdf5/include
	
utils.o:	utils.c utils.h map.o source.o
	$(CC) -c $(CFLAGS) utils.c

map.o:	map.c map.h
	$(CC) -c $(CFLAGS) map.c

model.o:	model.c model.h
	$(CC) -c $(CFLAGS) $(COMMON_FLAGS) model.c  -L./hdf5/lib -lhdf5 -I./hdf5/include

walltime.o:	walltime.c walltime.h
	$(CC) -c $(CFLAGS) walltime.c

ModPAPI.o:	ModPAPI.c ModPAPI.h
	$(CC) -c $(CFLAGS) ModPAPI.c


compare.exe:	compare.c
	gcc compare.c -o compare.exe

.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	cd $(arch) && make clean
	rm -f *.o $(TARGET)

clean-all:
	rm -f */*.o *.o $(TARGET)
