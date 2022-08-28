clc
clear
n=100;
len = 1000;
x = zeros(1,len);
for i = 1 : len
   x(i) = (sum(unifrnd (-1,1,1,n)))/sqrt(n/12)/2;
end
b=x(randperm(100));%随机抽样
jz=mean(b);
fc=moment(b,2);
[muhat,sigmahat,muci,sigmaci]=normfit(b,0.05);
b = b';
alpha = 0.05;
[mu, sigma] = normfit(b);
F = normcdf(b, mu, sigma);
[h,s] = kstest(b, [b,F], alpha);
if h == 0 %h==0表示接受
disp('该数据源服从正态分布。')
else
disp('该数据源不服从正态分布。')
end