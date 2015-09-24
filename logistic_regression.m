%%Logistic function
function x = logistic(z)
x = 1./(1+exp(-z));
%%logistic regression
function [T] = logress(x,y)

% newton raphson: theta = theta - inv(H)* grad;
% with H = hessian, grad = gradient

[m, n] = size(x);
I = m ;
O = n; 
T = zeros(O,1); %parameter vector the length of columns
for i=1:500
    grad = zeros(n,1); %slope
    H = zeros(n,n); %the hessian
    for j=1:m   
        hyp = logistic(T*x(j,:)); %The hypothesis
        grad = grad + x(j,:).'*(y(j)- hyp);
        H = H - hyp*(1-hyp)*x(j,:)'*x(j,:);
    end
T = T - inv(H)*grad; %implement newton's method
end

%%Main
load('q1x.dat');
load('q1y.dat');
m=size(q1x,1);
q1x = [ones(m,1), q1x]; %padding with 1s and append 
for i=1:m
    if(q1y(i)==1)   
        plot(q1x(i,2),q1x(i,3),'+g'); %set a graph of our classified points
    else
        plot(q1x(i,2),q1x(i,3),'sr');
    end
    hold on;
end
[T] = logress(q1x,q1y);
x = min(q1x(:,2)):max(q1x(:,2)); 
y = -T(1)-(T(2))*x;  		   %equation of a line
plot(x,y,'k');                 %visualize the separation boundary
title('Logistic Regression')
xlabel('x1');
ylabel('x2');
T  % [-2.6205 .7604 1.1719].
