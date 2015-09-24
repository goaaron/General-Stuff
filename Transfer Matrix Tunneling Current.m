%Transfer Matrix Method for Estimating Tunneling Current
%Aaron Goebel

%SiO2 constants
h=6.63e-34
hbar=h/(2*pi)
e=1.6e-19
m0=9.1e-31
kb=1.383e-23
T=300
c_band = 3.17% for Si 
tox = 1
numStep = 50
numGate = 50
numEn = 50;
vGate = linspace(0,2,numStep); %gate voltages


%Transfer Matrix 
MF = zeros(1, numStep+2);           %Effective masses in each layer use a 2 to get the final boundary
MF(1)=.26

VM = zeros(1,numStep+2);      %Potential in each layer
DM = zeros(1, numStep+2);     %Thickness of each layer
DM(1)=0
							  %assuming we aren't heading to any new boundaries
for i = 2:(numStep+1)
	MF(i) = .39;
	DM(i) = tox/numStep;
end
MF(numStep+2)= .26
DM(numStep+2)= 0;
prob_vec = zeros(1, numGate); %start by creating a tunneling probability vector for each gate bias

VM(1) = 0;

for j =1:length(vGate) 
    VM(1) = 0;
    energy_values = linspace(0,c_band, numEn);
	for k = 2:numStep+1
		VM(k) = c_band - (k-1)*vGate(j)/numStep;
	end 
	VM(numStep+2) = -vGate(j);
    trans_prob = zeros(1, length(energy_values)); %calculating transmission probability
 	
	for l = 1:length(energy_values)
		k_1 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(1)*(energy_values(l)-VM(1)));
		k_2 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(2)*(energy_values(l)-VM(2)));
		d = (k_2/k_1)*(MF(1)/MF(2));
		transmit = .5*[1+d,1-d;1-d, 1+d];
		
		for m = 2:length(MF)-1 	%all the rest of the interior layers
			k_m = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(m)*(energy_values(l) - VM(m)));
			k_m_2 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(m+1)*(energy_values(l)-VM(m+1)));
			d_m = (k_m_2/k_m)*(MF(m)/MF(m+1));
			Prop_mat = [exp(-1i*k_m*DM(m)),0;0,exp(1i*k_m*DM(m))]*.5*[1+d_m,1-d_m;1-d_m, 1+d_m];
			transmit = transmit*Prop_mat;
		end
	
		trans_prob(l) = 1 - abs(transmit(2,1))^2/abs(transmit(1,1))^2;
		
		if l ==1
			trans_prob(l) = 0 ;		%fix singularities
		end
		
		if isnan(trans_prob(l))
			trans_prob(l) = trans_prob(l-1);
		end
	end 
	
	
	%integrating for the tunneling current 
	
	J_den(j) = (e*MF(1)*m0*kb*T/(2*pi^2*hbar^3))*trapz(energy_values*e, (trans_prob.*exp(-energy_values*e/(kb*T))));
	
		
end


%Repeat with high-K dielectric
%Hf02 constants
c_band = 2.2% for Si 
tox = 6.41
numStep = 50
numGate = 50
numEn = 50;
vGate = linspace(0,2,numStep); %gate voltages
%subst = @(B,m,n) B(m,n);


MF = zeros(1, numStep+2);           %Effective masses in each layer use a 2 to get the final boundary
MF(1)=.26
%*m0  %inital and final effective masses
VM = zeros(1,numStep+2);      %Potential in each layer
DM = zeros(1, numStep+2);     %Thickness of each layer
DM(1)=0
   %assuming we aren't heading to any new boundaries
for i = 2:(numStep+1)
	MF(i) = .1%*m0;
	DM(i) = tox/numStep;
end
MF(numStep+2)=.26
DM(numStep+2)=0;
prob_vec = zeros(1, numGate); %start by creating a tunneling probability vector for each gate bias

VM(1) = 0;

for j =1:length(vGate) 
    VM(1) = 0;
    energy_values = linspace(0,c_band, numEn);
	for k = 2:numStep+1
		VM(k) = c_band - (k-1)*vGate(j)/numStep;
	end 
	VM(numStep+2) = -vGate(j);
    trans_prob = zeros(1, length(energy_values)); %calculating transmission probability
 	for l = 1:length(energy_values);
	
		k_1 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(1)*(energy_values(l)-VM(1)));
		k_2 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(2)*(energy_values(l)-VM(2)));
		d = (k_2/k_1)*(MF(1)/MF(2));
		transmit = .5*[1+d,1-d;1-d, 1+d];
		
		for m = 2:length(MF)-1 %all rest of the interior layers
			k_m = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(m)*(energy_values(l) - VM(m)));
			k_m_2 = sqrt(((2*e*m0*10^-18)/(hbar^2))*MF(m+1)*(energy_values(l)-VM(m+1)));
			d_m = (k_m_2/k_m)*(MF(m)/MF(m+1));
			Prop_mat = [exp(-1i*k_m*DM(m)),0;0,exp(1i*k_m*DM(m))]*.5*[1+d_m,1-d_m;1-d_m, 1+d_m];
			transmit = transmit*Prop_mat;
		end
	
		trans_prob(l) = 1 - abs(transmit(2,1))^2/abs(transmit(1,1))^2;
		
		if l ==1
			trans_prob(l) = 0 ;
		end
		
		if isnan(trans_prob(l))
			trans_prob(l) = trans_prob(l-1);
		end
	end 
	
	

	
	J_den2(j) = (e*MF(1)*m0*kb*T/(2*pi^2*hbar^3))*trapz(energy_values*e, (trans_prob.*exp(-energy_values*e/(kb*T))));
	
		
end

semilogy(vGate, J_den/10000,'r'); %convert to cm
hold on;
semilogy(vGate, J_den2/10000,'b')
title('J vs. V_G')
ylabel('A/cm^2')
xlabel('eV')
legend('SiO_2','HfO_2')
