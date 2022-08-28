clc
clear
n=10000;%实验次数
car=0;
%假设1，2为羊，3为车
%如果每次都更改选择
for i = 1:n
    choose = unidrnd(3);
    if(choose == 1 || choose == 2)
        car = car + 1;
    end
end
fprintf("更改选择选中汽车的概率为：%f\n",car/n)
%如果每次都不改选择
car=0;
for i = 1:n
    choose = unidrnd(3);
    if(choose == 3)
        car = car + 1;
    end
end
fprintf("不更改选择选中汽车的概率为：%f\n",car/n)

'''
a=0;
b=0;
result=zeros(10000);
for i = 1:10000
    for j = 1:18
        t=unidrnd(2);
        if t==1
            a=a+1;
        else 
            b=b+1;
        end
        if(a==8 || b==11)
            if (a==8)
                result(i)=0;
            else 
                result(i)=1;
            end
            a=0;
            b=0;
            
            break;
        end
    end
end

        
    
