%This script performs a numerical inverse Laplace Transform on the dimensionless amplitude in Laplace space of the interface between two superimposed fluid
%Reference: Equation for Amplitude in LP space corresponds to Eq 22. and 23. in Prosperetti 1981

%Initialize variables (indice u and l describe the upper and lower fluid)
t = 0:0.05:10; 
Ga_l = 1e8; %Galileo number
Ga_u = Ga_l*(1e-2); 
mu_ml = 1e-2; %dynamic viscosity ratio
pu_pl = 1e-3; %density ratio 
a0 = 0.01;
u0 = -1e-5;

%Calculating the amplitude in Laplace space (dimensionless version of Eq.22 and .23)
k_ll = @(s) 1-sqrt(1+s*sqrt(Ga_l)/(4*pi^2)); 
k_lu = @(s) 1-sqrt(1+s*sqrt(Ga_u)/(4*pi^2));
den = (1-pu_pl)/(1+pu_pl);

lambda = @(s) (-4*s*pu_pl + 16*(pi^2/sqrt(Ga_l))*(mu_ml-1) ...
*(pu_pl*k_ll(s)-k_lu(s)) + 64*(pi^4/Ga_l)*(1/s)*(1-mu_ml)^2*k_ll(s)*k_lu(s)) ...
/((1+pu_pl)*(k_lu(s)+pu_pl*(k_ll(s))));

amp_LP = @(s) a0/s + (s*u0 - 2*pi*den*a0)/(s^3+lambda(s)*s^2+2*pi*den*s);

%Numerically inverting the amplitude in Laplace space
amp_NI = talbot_inversion_sym(amp_LP, t, 128);

plot(t, amp_NI,'.');
xlabel('$\tilde{t} = t \sqrt{g/\lambda}$','Interpreter','latex', 'FontSize',16);
ylabel('$\tilde{A} = A/\lambda$','Interpreter','latex','FontSize',16);
title('Numerical Laplace Inversion');


