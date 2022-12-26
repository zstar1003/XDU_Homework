figure(1);
data=imread('D:\Desktop\计算机视觉\平时作业\lena\lena.bmp'); 
subplot(2,2,1)
imshow(data);
title('原图');
subplot(2,2,2);
imshow(fft2(data));
title('幅度谱图');
%低通滤波
s=fftshift(fft2(data));
[M,N]=size(s);                    
n=2;   
%GLPF滤波d0=30 
d0=30;                             
n1=floor(M/2);                          %对M/2进行取整
n2=floor(N/2);                           %对N/2进行取整
for i=1:M 
    for j=1:N
        d=sqrt((i-n1)^2+(j-n2)^2);         %点（i,j）到傅立叶变换中心的距离
               h=1*exp(-1/2*(d^2/d0^2));  %GLPF滤波函数
        s(i,j)=h*s(i,j);                   %GLPF滤波后的频域表示
    end
end
s=ifftshift(s);                           %对s进行反FFT移动
s=uint8(real(ifft2(s)));                                     
subplot(2,2,3);      
imshow(s);     
title('低通滤波');
%高通滤波
p=fftshift(fft2(data));
[M,N]=size(p);                     %分别返回p的行数到M中，列数到N中
n=2;                                  %对n赋初值
%GHPF滤波d1=30
d1=30;                               
n3=floor(M/2);                        
n4=floor(N/2);                         
for i=1:M 
    for j=1:N
        dd=sqrt((i-n3)^2+(j-n4)^2);         %点（i,j）到傅立叶变换中心的距离
               h1=1-exp(-1/2*(dd^2/d1^2));  %GHPF滤波函数
        p(i,j)=h1*p(i,j);                   %GHPF滤波后的频域表示
    end
end
p=ifftshift(p);                           %对p进行反FFT移动
p=uint8(real(ifft2(p)));                                     
subplot(2,2,4);     
imshow(p);
title('高通滤波');
