#include "../comunicacao.mpi.h"


//#############################    MPI  SEND   ##################################

void MPI_enviar_onda(int sx, int sy, int sz, float *ondaPtr,  SlicePtr p) {
  int tamanho = sx * sy * sz;
  MPI_Send((void *) ondaPtr, tamanho , MPI_FLOAT, 1, MSG_ONDA, MPI_COMM_WORLD);  
}


//##########################    MPI  RECEIVE   ##################################

void MPI_escrita_disco(int sx, int sy, int sz, const int st,  const float dtOutput, const float dt)
{ 
  int ordem = 0;
  int tamanho = sx * sy * sz;
  float *onda = malloc(sizeof(float) * tamanho); 
  char nome_arquivo[128];
  char parte[5];

  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;
  int itCnt = 1;

  hid_t file  = H5I_INVALID_HID;
  hid_t space = H5I_INVALID_HID;
  hid_t dset  = H5I_INVALID_HID;
  hid_t dcpl  = H5I_INVALID_HID;

  hsize_t dims[1] = {sx * sy * sz};

  if((file = H5Fcreate(FILE_H5, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) == H5I_INVALID_HID){
    printf("Erro ao criar arquivo hd5\n");
    exit(-1);
  }
  if((space = H5Screate_simple(1, dims, NULL)) == H5I_INVALID_HID){
    printf("Erro ao criar dataspace hd5\n");
    exit(-1);
  }
  
  dcpl = H5Pcreate(H5P_DATASET_CREATE);
  H5Pset_chunk(dcpl, 1, dims);
  H5Pset_deflate(dcpl, 3);

  dcpl = H5P_DEFAULT;
  strcpy(nome_arquivo, DATASET_BASE);
  sprintf(parte, "%d", ordem);
  strcat(nome_arquivo, parte);

  MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  dset   = H5Dcreate(file, nome_arquivo, H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
  H5Dwrite(dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, onda);
  H5Dclose(dset);

  ordem++;
  for (int it=1; it<=st; it++) {

    tSim=it*dt;
    if (tSim >= tOut) {
      strcpy(nome_arquivo, DATASET_BASE);
      sprintf(parte, "%d", ordem);
      strcat(nome_arquivo, parte);

      MPI_Recv((void *) onda, tamanho, MPI_FLOAT, 0, MSG_ONDA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      dset   = H5Dcreate(file, nome_arquivo, H5T_NATIVE_FLOAT, space, H5P_DEFAULT, dcpl, H5P_DEFAULT);
      H5Dwrite(dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, onda);
      H5Dclose(dset);
      ordem++;
      tOut=(++nOut)*dtOutput;
      itCnt++;
    }
  }

  H5Sclose(space);
  H5Pclose(dcpl);
  H5Fclose(file);
  MPI_Finalize();
  exit(0);
}

