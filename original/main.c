#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "boundary.h"
#include "source.h"
#include "utils.h"
#include "map.h"
#include "driver.h"
#include "fletcher.h"
#include "model.h"
#include "comunicacao.mpi.h"

#include <unistd.h>
#include <sys/time.h>


enum Form {ISO, VTI, TTI};


int main(int argc, char** argv) {
  //Inicia MPI
  MPI_Init(&argc, &argv);
  int rank, cluser_size;  
  //quantidade de processos do cluster
  MPI_Comm_size(MPI_COMM_WORLD, &cluser_size);
  //rank do procsso atual
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  //Pra confirmar que esta rodando em mais de uma maquina
  char hostbuffer[256];
  struct hostent *host_entry;
  int hostname;
  hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  
  if (hostname != -1) {
    printf("***RANK1 Hostname: %s\n", hostbuffer);
  }
  //----------------------------------------------------

  enum Form prob;        // problem formulation
  int nx;                // grid points in x
  int ny;                // grid points in y
  int nz;                // grid points in z
  int bord=4;            // border size to apply the stencil at grid extremes
  int absorb;            // absortion zone size
  int sx;                // grid dimension in x (grid points + 2*border + 2*absortion)
  int sy;                // grid dimension in y (grid points + 2*border + 2*absortion)
  int sz;                // grid dimension in z (grid points + 2*border + 2*absortion)
  int st;                // number of time steps
  float dx;              // grid step in x
  float dy;              // grid step in y
  float dz;              // grid step in z
  float dt;              // time advance at each time step
  float tmax;            // desired simulation final time
  int ixSource;          // source x index
  int iySource;          // source y index
  int izSource;          // source z index
  int iSource;           // source index (ix,iy,iz) maped into 1D array
//PPL  int i, ix, iy, iz, it; // for indices
  int i, it;             // for indices
//PPL  char fNameAbs[128];    // prefix of absortion file
  char fNameSec[128];    // prefix of sections files

  const float dtOutput=0.01;

  it = 0; //PPL
    
  // input problem definition
  
  if (argc<ARGS) {
    printf("program requires %d input arguments; execution halted\n",ARGS-1);
    exit(-1);
  } 
  strcpy(fNameSec,argv[1]);
  nx=atoi(argv[2]);
  ny=atoi(argv[3]);
  nz=atoi(argv[4]);
  absorb=atoi(argv[5]);
  dx=atof(argv[6]);
  dy=atof(argv[7]);
  dz=atof(argv[8]);
  dt=atof(argv[9]);
  tmax=atof(argv[10]);



  // verify problem formulation

  if (strcmp(fNameSec,"ISO")==0) {
    prob=ISO;
  }
  else if (strcmp(fNameSec,"VTI")==0) {
    prob=VTI;
  }
  else if (strcmp(fNameSec,"TTI")==0) {
    prob=TTI;
  }
  else {
    printf("Input problem formulation (%s) is unknown\n", fNameSec);
    exit(-1);
  }


  // grid dimensions from problem size

  sx=nx+2*bord+2*absorb;
  sy=ny+2*bord+2*absorb;
  sz=nz+2*bord+2*absorb;

  


  // number of time iterations

  st=ceil(tmax/dt);
  // source position

//MPI ESCRITA
  if(rank == 1){
    MPI_escrita_disco(sx, sy, sz, fNameSec, st, dtOutput, dx, dy, dz, dt);
  }



  ixSource=sx/2;
  iySource=sy/2;
  izSource=sz/2;
  iSource=ind(ixSource,iySource,izSource);


  // allocate input anisotropy arrays
  
  float *vpz=NULL;      // p wave speed normal to the simetry plane
  vpz = (float *) malloc(sx*sy*sz*sizeof(float));

  float *vsv=NULL;      // sv wave speed normal to the simetry plane
  vsv = (float *) malloc(sx*sy*sz*sizeof(float));
  
  float *epsilon=NULL;  // Thomsen isotropic parameter
  epsilon = (float *) malloc(sx*sy*sz*sizeof(float));
  
  float *delta=NULL;    // Thomsen isotropic parameter
  delta = (float *) malloc(sx*sy*sz*sizeof(float));
  
  float *phi=NULL;     // isotropy simetry azimuth angle
  phi = (float *) malloc(sx*sy*sz*sizeof(float));
  
  float *theta=NULL;  // isotropy simetry deep angle
  theta = (float *) malloc(sx*sy*sz*sizeof(float));

  // input anisotropy arrays for selected problem formulation

  switch(prob) {

  case ISO:

    for (i=0; i<sx*sy*sz; i++) {
      vpz[i]=3000.0;
      epsilon[i]=0.0;
      delta[i]=0.0;
      phi[i]=0.0;
      theta[i]=0.0;
      vsv[i]=0.0;
    }
    break;

  case VTI:

    if (SIGMA > MAX_SIGMA) {
      printf("Since sigma (%f) is greater that threshold (%f), sigma is considered infinity and vsv is set to zero\n", 
          SIGMA, MAX_SIGMA);
    }
    for (i=0; i<sx*sy*sz; i++) {
      vpz[i]=3000.0;
      epsilon[i]=0.24;
      delta[i]=0.1;
      phi[i]=0.0;
      theta[i]=0.0;
      if (SIGMA > MAX_SIGMA) {
  vsv[i]=0.0;
      } else {
  vsv[i]=vpz[i]*sqrtf(fabsf(epsilon[i]-delta[i])/SIGMA);
      }
    }
    break;

  case TTI:

    if (SIGMA > MAX_SIGMA) {
      printf("Since sigma (%f) is greater that threshold (%f), sigma is considered infinity and vsv is set to zero\n", 
          SIGMA, MAX_SIGMA);
    }
    for (i=0; i<sx*sy*sz; i++) {
      vpz[i]=3000.0;
      epsilon[i]=0.24;
      delta[i]=0.1;
      //      phi[i]=0.0;
      phi[i]=1.0; // evitando coeficientes nulos
      theta[i]=atanf(1.0);
      if (SIGMA > MAX_SIGMA) {
  vsv[i]=0.0;
      } else {
  vsv[i]=vpz[i]*sqrtf(fabsf(epsilon[i]-delta[i])/SIGMA);
      }
    }
  } // end switch

  // stability condition
  
  float maxvel;
  maxvel=vpz[0]*sqrt(1.0+2*epsilon[0]);
  for (i=1; i<sx*sy*sz; i++) {
    maxvel=fmaxf(maxvel,vpz[i]*sqrt(1.0+2*epsilon[i]));
  }
  float mindelta=dx;
  if (dy<mindelta)
    mindelta=dy;
  if (dz<mindelta)
    mindelta=dz;
  float recdt;
  recdt=(MI*mindelta)/maxvel;


  // random boundary speed

  RandomVelocityBoundary(sx, sy, sz,
      nx, ny, nz,
      bord, absorb,
      vpz, vsv);
  // pressure fields at previous, current and future time steps
  
  float *pp=NULL;
  pp = (float *) malloc(sx*sy*sz*sizeof(float)); 
  float *pc=NULL;
  pc = (float *) malloc(sx*sy*sz*sizeof(float)); 
  float *qp=NULL;
  qp = (float *) malloc(sx*sy*sz*sizeof(float)); 
  float *qc=NULL;
  qc = (float *) malloc(sx*sy*sz*sizeof(float)); 
  for (i=0; i<sx*sy*sz; i++) {
    pp[i]=0.0f; pc[i]=0.0f; 
    qp[i]=0.0f; qc[i]=0.0f;
  }

  // slices

  //PPL  char fName[10];
  int ixStart=0;
  int ixEnd=sx-1;
  int iyStart=0;
  int iyEnd=sy-1;
  int izStart=0;
  int izEnd=sz-1;

  SlicePtr sPtr;
  sPtr=OpenSliceFile(ixStart, ixEnd,
        iyStart, iyEnd,
        izStart, izEnd,
        dx, dy, dz, dt,
        fNameSec);

  struct timeval start, end;
  gettimeofday(&start, NULL);
  //###### ENVIAR ONDA MPI
  MPI_enviar_onda(sx,sy,sz,pc);
      
  // Model do:
  // - Initialize
  // - time loop
  // - calls Propagate
  // - calls TimeForward
  // - calls InsertSource
  // - do AbsorbingBoundary and DumpSliceFile, if needed
  // - Finalize
  Model(st,     iSource, dtOutput, sPtr,
        sx,     sy,      sz,       bord,
        dx,     dy,      dz,       dt,   it, 
        pp,     pc,      qp,       qc,
  vpz,    vsv,     epsilon,  delta,
  phi,    theta);

  CloseSliceFile(sPtr);

  MPI_Finalize();
  gettimeofday(&end, NULL);
  long seconds = (end.tv_sec - start.tv_sec);
  printf("Tempo = %ld\n", seconds);
  
}