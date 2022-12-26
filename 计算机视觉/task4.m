I=imread('D:\Desktop\计算机视觉\平时作业\img\lena.jpg');
figure(1);
subplot(1,3,1);imshow(I);title('原始图像');
L=fspecial('laplacian');
L1=[0 -1 0;-1 5 -1;0 -1 0];
L2=[0 -2 0;-2 9 -2;0 -2 0];
LP1=imfilter(I,L1,'replicate');% α=1时的拉普拉斯算子
LP2=imfilter(I,L2,'replicate');% α=2时的拉普拉斯算子
subplot(1,3,2);imshow(LP1);title('Laplacian算子α=1锐化图像');
subplot(1,3,3);imshow(LP2);title('Laplacian算子α=2锐化图像');
