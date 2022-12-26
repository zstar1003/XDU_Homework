
clear all;
close all;
clc;

% img_origin=imread('D:\Desktop\计算机视觉\平时作业\图像处理与机器视觉课内实验补充资料\lena.jpg');


%巴特沃斯滤波器
I=imread('D:\Desktop\计算机视觉\平时作业\图像处理与机器视觉课内实验补充资料\lena.jpg');
% I_gray=im2gray(I);
I2=im2double(I);
%计算滤波器的行数
M=2*size(I,1);
N=2*size(I,2);
u=-M/2:(M/2-1);
v=-N/2:(N/2-1);
[U,V]=meshgrid(u,v);
D=sqrt(U.^2+V.^2);
D0=15;
n=6;
H=1./(1+(D./D0).^(2*n));
J=fftshift(fft2(I2,size(H,1),size(H,2)));
K=J.*H;
L=ifft2(ifftshift(K));
L=L(1:size(I2,1),1:size(I,2));
figure;
plot(I2);
figure;
imshow(I);
figure;
imshow(L);