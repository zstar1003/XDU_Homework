clc
a=0;
b=0;
awin=0;
bwin=0;
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
                awin=awin+1;
            else 
                bwin=bwin+1;
            end
            a=0;
            b=0;
            break;
        end
    end
end
fprintf("A赢的概率为：%f\n",awin/10000)
fprintf("B赢的概率为：%f\n",bwin/10000)


        