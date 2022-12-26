figure(1);
I=imread('D:\Desktop\计算机视觉\平时作业\img\pollen.bmp');
I1=imnoise(I,'gaussian'); %加高斯噪声
H1=[0 1 0;1 0 1;0 1 0]/4; %4×4领域模板
J=imfilter(I,H1); %领域平均
subplot(2,4,1),imshow(I); %显示图像I
title('原图像');
subplot(2,4,2),imshow(I1);
title('加噪声后图像');
subplot(2,4,3),imshow(J);       %不加门限平滑
title('不加门限平滑后图像');
%加门限后滤波
T= 2*sum(I1 (:))/128^2;
im_T = zeros(128,128);
for i = 1:128
    for j = 1:128
        if abs(I1(i,j) - J(i,j))>T
            im_T(i,j) = J(i,j);
        else 
            im_T(i,j) = I1(i,j);
        end
    end
end
subplot(2,4,4);
imshow(im_T);
title('加门限后');
