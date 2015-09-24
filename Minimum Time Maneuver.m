%Minimum Time Maneuver
theta = 15*pi/180;
T_feas = 0;
p_feas = 0;
T_max = 2; 
m = 0.1; 
g = [0;-9.8]; 
p_init = [0;0]; 
p_des = [10;2]; 
h = 0.1;
A = [-sin(theta), sin(theta); cos(theta), cos(theta)];



low = 1; 
up = 50; 
while low + 1 ~= up
	k= floor((low+up)/2);
	cvx_begin 
		variables T(2,k-1) v(2,k) p(2,k)
		F = A*T + m*repmat(g,1,k-1);
		minimize 1
		subject to
		p(:,1) == p_init; 
		p(:,end) == p_des;
		v(:,1) == 0; 
		v(:,end) == 0;
		0 <= T <= T_max;
		v(:,2:end) == v(:,1:end-1) + h/m*F;
		p(:,2:end) == p(:,1:end-1) + h*v(:,1:end-1);
	cvx_end
	if cvx_optval == 0
		up = k;
		T_feas = T;
		p_feas = p;
	else
		low = k;
	end
end
k = up
