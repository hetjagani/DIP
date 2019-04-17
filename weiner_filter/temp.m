clc;clear;close all
N = 512;
fin=fopen('boat_512_512.raw','r');
%fout=fopen('boat_512_512_dis.raw','w');

I=fread(fin,N*N,'uint8=>uint8'); 
ft_Z=reshape(I,N,N);
ft_Z=ft_Z';
figure;
imshow(ft_Z);

LEN = 5;
avgH = (1/LEN^2) .* ones(LEN,LEN);
J = conv2(ft_Z, avgH,'same');
figure;
imshow(uint8(J));
%fwrite(fout, uint8(J'),'uint8'); 

im_fft = fft2(J);


psf_padded = zeros(N, N);
psf_padded(1:LEN,1:LEN) = avgH;
psf_ft = fft2(psf_padded);

for i=1:N;
  for j=1:N;
    rec_ft(i,j) = (conj(psf_ft(i,j)) / (( conj(psf_ft(i,j)) * psf_ft(i,j) ) + 0.02) ) * im_fft(i,j);
  end
end

rec_img = ifft2(rec_ft);
figure;
imshow(uint8(real(rec_img)));