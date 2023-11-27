#include "utils.h"
#include "source.h"
#include "driver.h"
#include "fletcher.h"
#include "walltime.h"
#include "model.h"
#include "MPI.envio.h"

#ifdef PAPI
#include "ModPAPI.h"
#endif

#define MODEL_GLOBALVARS
#include "precomp.h"
#undef MODEL_GLOBALVARS


void ReportProblemSizeCSV(const int sx, const int sy, const int sz,
			  const int bord, const int st, 
			  FILE *f){
  fprintf(f,
	  "sx; %d; sy; %d; sz; %d; bord; %d;  st; %d; \n",
	  sx, sy, sz, bord, st);
}

void ReportMetricsCSV(double walltime, double MSamples,
		      long HWM, char *HWMUnit, FILE *f){
  fprintf(f,
	  "walltime; %lf; MSamples; %lf; HWM;  %ld; HWMUnit;  %s;\n",
	  walltime, MSamples, HWM, HWMUnit);
}


void Model(const int st, const int iSource, const float dtOutput, SlicePtr sPtr, 
           const int sx, const int sy, const int sz, const int bord,
           const float dx, const float dy, const float dz, const float dt, const int it, 
	   float * restrict pp, float * restrict pc, float * restrict qp, float * restrict qc,
	   float * restrict vpz, float * restrict vsv, float * restrict epsilon, float * restrict delta,
	   float * restrict phi, float * restrict theta)
{


  float tSim=0.0;
  int nOut=1;
  float tOut=nOut*dtOutput;

  const long samplesPropagate=(long)(sx-2*bord)*(long)(sy-2*bord)*(long)(sz-2*bord);
  const long totalSamples=samplesPropagate*(long)st;

#define MODEL_INITIALIZE
#include "precomp.h"
#undef MODEL_INITIALIZE

  // DRIVER_Initialize initialize target, allocate data etc
  DRIVER_Initialize(sx,   sy,   sz,   bord,
		      dx,  dy,  dz,  dt,
		      vpz,    vsv,    epsilon,    delta,
		      phi,    theta,
		      pp,    pc,    qp,    qc);

  double computacao=0.0;
  double tempo_transmissao=0.0;

  for (int it=1; it<=st; it++) {

    // Calculate / obtain source value on i timestep
    float src = Source(dt, it-1);
    
    DRIVER_InsertSource(dt,it-1,iSource,pc,qc,src);

    const double t0=wtime();
    DRIVER_Propagate(  sx,   sy,   sz,   bord,
		       dx,   dy,   dz,   dt,   it,
		       pp,    pc,    qp,    qc);

    SwapArrays(&pp, &pc, &qp, &qc);
    computacao+=wtime()-t0;

    tSim=it*dt;
    if (tSim >= tOut) {
      DRIVER_Update_pointers(sx,sy,sz,pc);
            const double tDump=wtime();

      const double tSend=wtime();
      MPI_enviar_onda(sx,sy,sz,pc,sPtr);
      tempo_transmissao+= wtime()-tSend;

      tOut=(++nOut)*dtOutput;
    }
  }

  fflush(stdout);

  // DRIVER_Finalize deallocate data, clean-up things etc 
  DRIVER_Finalize();
  printf("computacao;dump;total;\n");
  printf("%lf;%lf;", computacao, tempo_transmissao);

}

