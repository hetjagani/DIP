clear all;
N = 64;
fin=fopen('cam1_64_64.raw','r');
I=fread(fin,N*N,'uint8=>uint8'); 
ft_Z=reshape(I,N,N);
ft_Z=ft_Z';
im = ft_Z;
imshow(ft_Z);

ft = fft2(im);

ft = [zeros(64,32) ft zeros(64,32)];
ft = [zeros(32,128); ft; zeros(32,128)];

zm = abs(ifft2(ft))
maxi = max(max(zm));
mini = min(min(zm));

step = (maxi - mini) / 255;

for i=1:128;
  for j=1:128;
    zm(i,j) = uint8(zm(i,j)/step);
  end
end

imshow(zm);
max(max(zm));
min(min(zm));