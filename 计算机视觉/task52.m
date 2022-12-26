clear;
close all;
Img=imread('D:\Desktop\计算机视觉\平时作业\img\iris-Na.tif');
% Img=imread('D:\Desktop\计算机视觉\平时作业\img\bridge-RS.jpg'); 
% Img=imread('D:\Desktop\计算机视觉\平时作业\img\road-SAR.png'); 
I = rgb2gray(Img);
figure(1)
%Roberts梯度法锐化
subplot(2,3,1);imshow(Img);title('原始图像');
S=edge(I,'Roberts');
subplot(2,3,2);imshow(S);title('Roberts梯度法锐化图像');
%Prewitt算子锐化
S=edge(I,'Prewitt');
subplot(2,3,3);imshow(S);title('Prewitt算子锐化图像');
%Sobel算子锐化
S=edge(I,'Sobel');
subplot(2,3,4);imshow(S);title('Sobel算子锐化图像');
%Laplacian算子锐化
S=edge(I,'log');
subplot(2,3,5);imshow(S);title('Laplacian算子锐化图像');
%Canny算子锐化
Cimg = edge(I,'canny');
subplot(2,3,6);imshow(Cimg);title('Canny算子锐化图像');