clc
clear
m=100; %仿真次数
N=30;%学生人数
for j = 1:m
    B = zeros(365);
    for i=1:N
        A(i)=unidrnd(365);%生日的365天
        B(A(i))= B(A(i))+1;
        if (B(i)>=1)
            plot(j,A(i),'*','color',[1 0 0]);
            hold on;
        else 
            plot(j,A(i),'*','color',[0.6 0.6 0.6]);
            hold on;
        end
end
end
xlabel("times");
ylabel("date");
title("Birthday paradox scatter diagram(30 students)");
axis([0,100,0,365]);
