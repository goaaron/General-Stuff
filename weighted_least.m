%%Weighted Least Squares

load('q2x.dat')
load('q2y.dat')
[m,n] = size(q2x)
x=[ones(m,1) q2x]; %pre-pad with ones and append
T = inv(x'*x)*x'*q2y; %analytic solution to the regression

plot(x(:,2),q2y,'k*');
x_1=min(x(:,2)):max(x(:,2));
y = T(1) + T(2)*x_1;
hold on;

plot(x_1,y,'r');
xlabel('x')
ylabel('y')
title('Unweighted Linear Regression');

%%Implementing Weighted Least Squares
tau = .8;
for i = 1:length(x_1)
    W=zeros(m,m) %initialize weight matrix
    for j=1:m
        W(j,j) = exp(-((x_1(i)-x(j,2)).^2)/(2*tau^2)); % diagonal
    end
    a=x'*W*x
    a_inv=inv(a)
    T_w =a_inv*x'*W*q2y;
    y_w(i) = T_w(1) + T_w(2)*x_1(i);
end
plot(x_1,y_w, 'b');

hold on
tau = .1
for i = 1:length(x_1)
    W=zeros(m,m) %initialize weight matrix
    for j=1:m
        W(j,j) = exp(-((x_1(i)-x(j,2)).^2)/(2*tau^2)); % diagonal
    end
    a=x'*W*x
    a_inv=inv(a)
    T_w =a_inv*x'*W*q2y;
    y_w(i) = T_w(1) + T_w(2)*x_1(i);
end
plot(x_1,y_w, 'y');

tau = .3
for i = 1:length(x_1)
    W=zeros(m,m) %initialize weight matrix
    for j=1:m
        W(j,j) = exp(-((x_1(i)-x(j,2)).^2)/(2*tau^2)); % diagonal
    end
    a=x'*W*x
    a_inv=inv(a)
    T_w =a_inv*x'*W*q2y;
    y_w(i) = T_w(1) + T_w(2)*x_1(i);
end
plot(x_1,y_w, 'g');

tau = 2
for i = 1:length(x_1)
    W=zeros(m,m) %initialize weight matrix
    for j=1:m
        W(j,j) = exp(-((x_1(i)-x(j,2)).^2)/(2*tau^2)); % diagonal
    end
    a=x'*W*x
    a_inv=inv(a)
    T_w =a_inv*x'*W*q2y;
    y_w(i) = T_w(1) + T_w(2)*x_1(i);
end
plot(x_1,y_w, 'c');

tau = 10
for i = 1:length(x_1)
    W=zeros(m,m) %initialize weight matrix
    for j=1:m
        W(j,j) = exp(-((x_1(i)-x(j,2)).^2)/(2*tau^2)); % diagonal
    end
    a=x'*W*x
    a_inv=inv(a)
    T_w =a_inv*x'*W*q2y;
    y_w(i) = T_w(1) + T_w(2)*x_1(i);
end
plot(x_1,y_w, 'm');