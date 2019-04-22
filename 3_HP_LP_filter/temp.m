clear all;
N = 128;
fin=fopen('cam2_128_128.raw','r');
I=fread(fin,N*N,'uint8=>uint8'); 
ft_Z=reshape(I,N,N);
ft_Z=ft_Z';
im = ft_Z;
imshow(ft_Z);

fft2(im)
