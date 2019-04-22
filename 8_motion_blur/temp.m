clc;clear;close all
fin=fopen('blurry.raw','r');
I=fread(fin,256*256,'uint8=>uint8'); 
ft_Z=reshape(I,256,256);
ft_Z=ft_Z';
im_ft = (fft2(double(ft_Z)));
figure;
imshow(ft_Z);

LEN = 21;
THETA = 0;
PSF = fspecial('motion', LEN, THETA);

psf_padded = zeros(256, 256);
psf_padded(1,1:21) = PSF(11,:);
psf_ft = fft2(psf_padded);
rec_ft = im_ft./psf_ft;
rec_img = ifft2(rec_ft);
figure;
imshow(uint8(real(rec_img)));