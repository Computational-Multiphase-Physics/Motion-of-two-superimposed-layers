#include "navier-stokes/centered.h"
//#define FILTERED
#include "two-phase.h"
#include "reduced.h"

// density and viscosity ratios
#define Rhor_AirOcean (1e-1) // this is the density ratio of air to that of the ocean...
#define Mur_AirOcean (1e0) //OV:1e-2; this is the viscosity ratio of air to that of the ocean...


// viscosities
#define Ga (1e4) // Galileo number: gL^3/(\mu/\rho)^2 -- this is based on the viscosity of ocean
#define A0 0.01 // initial amplitude of the ocean wave

#define tmax (10.)
#define tsnap (tmax/100.)


#define fErr (1e-3)
#define VelErr (1e-2)
#define OmegaErr (1e-3)
#define KAPPAErr (1e-4)

char nameOut[80], amplitudeFile[80], name[80];
static FILE * fp1 = NULL;
static FILE * fp2 = NULL;

// grid resolution 
#define MAXlevel 7// maximum level of refinement (the original was 7)
#define MINlevel 0 // minimum level of refinement

int LEVEL;

uf.n[left]   = 0.;
uf.n[right]  = 0.;
uf.n[top]    = 0.;
uf.n[bottom] = 0.;

/**
The initial condition is a small amplitude plane wave of wavelength ($\lambda$)
unity. This wavelength is the relevant length scale for this problem. Note that $k = 2\pi$ */
event init (t = 0) {
  fraction (f, -y + A0*cos (2.*pi*x));
}

int main() {

  L0 = 2.0;
  Y0 = -L0/2.;
  // f.sigma = 0.0;
  TOLERANCE = 1e-6;
  DT = 1e-3; 

  LEVEL = MAXlevel;
  init_grid(1 << LEVEL);
  rho1 = 1.0; rho2 = 0.;
  mu1 = 1./sqrt(Ga); mu2 = 0.;
  G.y = -1.; //-1. acceleration due to gravity

  char comm[80];
  sprintf (comm, "mkdir -p intermediate");
  system(comm);

  fp1 = fopen ("logfile.dat", "w");
  fp2 = fopen ("amplitude.dat", "w");

  run();

  fclose (fp1);
  fclose (fp2);
}

scalar omega[], KAPPA[];
event adapt (i++) {
  curvature(f, KAPPA);
  vorticity (u, omega);
  adapt_wavelet ((scalar *){f, u.x, u.y, omega, KAPPA},
     (double[]){fErr, VelErr, VelErr, OmegaErr, KAPPAErr},
      maxlevel = MAXlevel, minlevel = MINlevel);
}

/**
## Writing Output files
*/

event writingFiles (t = 0; t += tsnap; t <= tmax) {
  dump (file = "dump");
  sprintf (nameOut, "intermediate/snapshot-%6.5f", t);
  dump(file=nameOut);
}

/**
The calculation of amplitude of the surface wave is same as done [(here)](http://basilisk.fr/src/test/capwave.c).<br/>
By default tracers are defined at $t-\Delta t/2$. We use the *first*
keyword to move VOF advection before the *amplitude* output i.e. at
$t+\Delta/2$. This improves the results. */

event vof (i++, first);

/**
We output the amplitude of the standing surface wave.
*/

event amplitude (i++) {

  /**
  To get an accurate amplitude, we reconstruct interface position
  (using height functions) and take the corresponding maximum. */

  scalar pos[];
  position (f, pos, {0,1});
  double max = statsf(pos).max;

  /**
  We output the corresponding evolution in a file indexed with the
  case number. */
  if (i == 0) {
    fprintf (fp2, "t AmpOcean\n");
  }
  fprintf (fp2, "%g %g\n", t, max);
  fflush (fp2);

}

event logfile (i++) {
  double ke = 0.;
  foreach (reduction(+:ke)){
    ke += sq(Delta)*(sq(u.x[]) + sq(u.y[]))*f[];
  }
  fprintf (fp1, "%g %g %d\n", t, ke, mgp.i);
  fprintf (ferr, "%d %g %g %d\n", i, t, ke, mgp.i);
  fflush (fp1);
}