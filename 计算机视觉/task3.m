% 第一小问
I=imread('D:\Desktop\计算机视觉\平时作业\img\pollen.tif');
figure(1);
subplot(1,2,1);
imshow(I);
title('原图');
% L=8灰度级
[wid, hei] = size(I);
img8 = zeros(wid, hei);
for i=1:wid
      for j=1:hei
          img8(i,j)=floor(I(i,j)/32);
      end
end
subplot(1,2,2);
imshow(uint8(img8),[0,7]);
title('灰度级L=8');
figure(2);
subplot(3,2,[1,2]);
imhist(I,8);
title('直方图');
J=histeq(I);
subplot(3,2,[3,4]);
imhist(J,8);
title('直方图均衡化');
subplot(3,2,[5,6]);
hgram=[0,0,0,0.15,0.20,0.30,0.20,0.15];
H=histeq(I,hgram);
imhist(H,8);
title('规定化后直方图');
% 第二小问
figure(3);
I1=imnoise(I,'gaussian'); %加高斯噪声
H1=[0 1 0;1 0 1;0 1 0]/4; %4×4领域模板
J1=imfilter(J,H1); %领域平均
subplot(1,4,1);
imshow(I); %显示图像I
title('原图像');
subplot(1,4,2);
imshow(I1);
title('加噪声后图像');
subplot(1,4,3);
imshow(J1);   
title('不加门限平滑后图像');
%加门限后滤波
T= 2*sum(I1(:))/500^2;
% T= 0;
im_T = zeros(500,500);
for i = 1:500
    for j = 1:500
        if abs(I1(i,j) - J(i,j))>T
            im_T(i,j) = J(i,j);
        else 
            im_T(i,j) = I1(i,j);
        end
    end
end
subplot(1,4,4);
imshow(im_T);
title('加门限后');