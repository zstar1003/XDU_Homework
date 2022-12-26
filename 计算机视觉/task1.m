% 生成第一问图像
f1=zeros(128,128);
for i=38:1:90
    for j=58:1:70
        f1(i,j)=255;
    end
end
% 第一小问
figure(1)
subplot(1,2,1);
imshow (f1);
subplot(1,2,2);
imshow (fft2(f1));
% 第二小问
for i=1:1:128
    for j=1:1:128
        f2(i,j)=(-1)^(i+j)*f1(i,j);
    end
end
figure(2);
subplot(1,2,1);
imshow (f2);
subplot(1,2,2);
imshow (fft2(f2));
% 第三小问
figure(3);
f3=imrotate(f2,-45,'bilinear');%将f2顺时针旋转45度
subplot(1,2,1);
imshow(fft2(f2));%显示f2的频谱
title('FFT(f2)的幅度谱');
subplot(1,2,2);
imshow(fft2(f3));%显示f3的频谱
title('FFT(f3)的幅度谱');
