#Optimal Spacecraft Landing
h=1
g=.1
m=10
Fmax=10
p0=np.matrix('50;50;100')
v0=np.matrix('-10;0;-10')
alpha=.5
gamma=1
K=35

#Minimum Fuel
f_list = [cvx.varaible(3) for _ in xrange(K)]
cst_f = [cvx.norm(f) <= Fmax for f in f_list]
v_list = [v0]
for i in xrange(K-1_:
	v_list.append(v_list[-1] + h/m *f_list[i]-h*g)
	
p_list = [p0]
for i in xrange(K-1):
	p_list.append(p_list[-1] + .5*h*(v_list[i] + v_list[i+1]))
cst_[ = [p[2] >= alpha*cvx.norm(p[:2]) for p in p_list]

constraints = [p_list[K-1] == 0, v_list[K-1] ==0] + cst_f + cst_p

target_f = gamma * sum([cvx.norm(f) for f in f_list])
target = cvx.Minimize(target_f)
prob = cvx.Problem(target, constraints) 



#Minimum Time 
K_lower = 1
K_upper = K 
while K_lower != K_upper
	mid = K_lower +(K_upper-K_lower)/2
	constraints = [p_list[mid-1]==0, v_list[mid-1] ==0] +cst_f[:mid] + cst_p[:mid]
	prob = cvx.Problem(cvx.Minimize(1), constraints)
	val = prob.solve()
	if np.isinf(val):
		K_lower = mid+1
	else:
		K_upper = mid
print "Min time:", K_upper*h

constraints = [p_list[K_upper-1]==0, v_list[K_upper-1]==0] + cst_f[:K_upper] + cst_p[:K_upper]
prob = cvx.Problem(cvx.Minimize(1), constraints)
val = prob.solve()
