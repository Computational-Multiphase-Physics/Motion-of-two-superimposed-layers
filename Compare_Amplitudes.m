% This script plots the amplitude data from the simulation and from the Numerical inverse Laplace Transform
% It requires the amplitude from the numerical inverse LT, so it needs to be run after Numerical_inverseLT_Prosperetti.m

% Load simulation data 
data = dlmread('amplitude_Ga1e8.dat', ' ', 1, 0);

% Extract columns into variables
t_data = data(:, 1); %time 
amp_data = data(:, 2); %Amplitude

% Convert sym array to double for plotting
amp_P = abs(double(amp_NI));

% Plot data
plot(t_data, amp_data, ...
    t, amp_P,'.');
xlabel('$\tilde{t} = t \sqrt{g/\lambda}$','Interpreter','latex', 'FontSize',16);
ylabel('$\tilde{A} = A/\lambda$','Interpreter','latex','FontSize',16);
legend('Basilisk', 'Prosperetti');

%Turn the figure into .png
saveas(gcf, 'Ga1e8.png');
