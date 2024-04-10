# Prosperetti
contains a matlab script (Numerical_inverseLT_Prosperetti.m) that numerically inverts the dimensionless amplitude in Laplace space for the interface between two superimposed fluids
and a matlab script (Compare_amplitudes.m) that compares the amplitude from the numerical inverse LT to the amplitude data from a Basilisk simulation. 
In order to run Numerical_inverseLT_Prosperetti.m, you need to download the Numerical Inverse Laplace Transform package from Tucker McClure (2013). 

I've included some data from my Basilisk simulation, labelled "amplitude_Ga(...).dat", and the corresponding figures where this data is compared to the numerically inverted LT data, labelled "Ga(...).png". 

Brief explanation of what we're doing here: For a system of two superimposed viscous fluids, such as air and water in the context of a surface gravity wave, and where the oscillation of the interface between the fluid is small, an expression for the amplitude of the interface can be derived in Laplace space, as demonstrated by Prosperetti in 1981. However, in real space a general expression doesn't exist, except in cases where the dynamic viscosity of both fluids is identical or when one of the fluids is devoid of density and viscosity. By numerically inverting the amplitude expression of the interface from the Laplace space (Eq 22 and 23 Prosperetti 1981) to the real space, one can use this numerical data to compare it with the simulation data of the amplitude of the interface in order to check the validity of the simulation.  
