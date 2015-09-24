%Affine Policy
%find optimal values of x0 and K by solving a standard explicit
%convex optimization problem (i.e., one that does not involve an expectation or an
%infinite number of constraints)
cvx_begin
	variables x0(n) K(n,p)
	minimize (c'*x0)
	subject to
	A*x0 + norms(A*K-B,1,2) <= b0
cvx_end
% Against samples
obj = []; 
obj1 = [];
for i=1:100
	s = 2*rand(p,1)-1;
	cvx_begin 
		variable x_opt(n)
		minimize (c'*x_opt)
		subject to
		A*x_opt <= b0+B*s
	cvx_end
obj(i) = c'*(x0+K*s);
obj1(i) = cvx_optval;
end

plot(obj1,obj,'o'); 
hold on;
plot(xlim, xlim);
