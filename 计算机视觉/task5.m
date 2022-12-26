I=imread('D:\Desktop\计算机视觉\平时作业\img\iris-Na.tif');
% I=imread('D:\Desktop\计算机视觉\平时作业\img\bridge-RS.jpg'); 
% I=imread('D:\Desktop\计算机视觉\平时作业\img\road-SAR.png'); 
figure(1)
%Roberts梯度法锐化
subplot(2,3,1);imshow(I);title('原始图像');
% J=double(I);
% [IX,IY]=gradient(J); 
% A=sqrt(IX.*IX+IY.*IY);
% subplot(2,3,2);imshow(A,[]);title('Roberts梯度法锐化图像');
m = 600;
n = 600;
imgr = zeros(m,n);
T = 20;
for i=2:m-1
    for j=2:n-1
        imgr(i,j)= abs(I(i,j)-I(i+1,j+1)) + abs(I(i+1,j)-I(i,j+1));
        if imgr(i,j)<T
            imgr(i,j) = 0;
        else
            imgr(i,j) = 255;
        end
    end
end
subplot(2,3,2);imshow(imgr);title('Roberts梯度法锐化图像');
%Prewitt算子锐化
% S=imfilter(I,fspecial('Prewitt'));
imgl = zeros(m,n);
for i=2:m-1
    for j=2:n-1
        imgl(i,j)= abs(I(i+1,j)+I(i-1,j)+I(i,j+1)+I(i,j-1)-4*I(i,j));
        if imgl(i,j)<T
            imgl(i,j) = 0;
        else
            imgl(i,j) = 255;
        end
    end
end
subplot(2,3,3);imshow(imgl);title('Prewitt算子锐化图像');
%Sobel算子锐化
% S=imfilter(I,fspecial('sobel'));
imgs = zeros(m,n);
for i=2:m-1
    for j=2:n-1
        imgs(i,j)= abs(I(i-1,j+1)+2*I(i,j+1)+I(i+1,j+1)-I(i-1,j-1)-2*I(i,j-1)-I(i+1,j-1)) + abs(I(i+1,j-1)+2*I(i+1,j)+I(i+1,j+1)-I(i-1,j-1)-2*I(i-1,j)-I(i-1,j+1));
        if imgs(i,j)<T
            imgs(i,j) = 0;
        else
            imgs(i,j) = 255;
        end
    end
end

subplot(2,3,4);imshow(imgs);title('Sobel算子锐化图像');
%Laplacian算子锐化
% L2=[0 -2 0;-2 9 -2;0 -2 0];
% S=imfilter(I,L2,'replicate');% α=2时的拉普拉斯算子
% S=imfilter(I,fspecial('Laplacian'));
% S=imfilter(I,fspecial('sobel'));
imgl = zeros(m,n);
for i=2:m-1
    for j=2:n-1
        imgl(i,j)= abs(I(i+1,j)+I(i-1,j)+I(i,j+1)+I(i,j-1)-4*I(i,j));
        if imgl(i,j)<T
            imgl(i,j) = 0;
        else
            imgl(i,j) = 255;
        end
    end
end

subplot(2,3,5);imshow(imgl);title('Laplacian算子锐化图像');


Cimg = edge(I,'canny');
subplot(2,3,6);imshow(Cimg);title('Canny算子锐化图像');