clear all;
N = 64;
fin=fopen('cam1_64_64.raw','r');
I=fread(fin,N*N,'uint8=>uint8'); 
ft_Z=reshape(I,N,N);
ft_Z=ft_Z';
im = ft_Z;
%subplot(1,2,1);
imshow(ft_Z);
for i=1:N;
  for j=1:N;
    ft_Z(i,j) = (-1)^(i-1+j-1) * ft_Z(i,j);
  end
  row_ft(i,:) = fft(ft_Z(i,:));
end
row_ft = transpose(row_ft);
for i=1:N;
  col_ft(i,:) = fft(row_ft(i,:));
end
fft_ans = transpose(col_ft);
fft_ans = abs(fft_ans);
hmm = uint8(255*mat2gray(log(1+fft_ans)));
figure;
imshow(hmm);
%fin=fopen('cam1_64_64.raw','r');
%I=fread(fin,64*64,'uint8=>uint8'); 
%Z=reshape(I,64,64);
%Z=Z';
%subplot(1,2,1);

%hmm = uint8(255*mat2gray(log(1+fftshift(abs(fft2(double(Z)))))));

%subplot(1,2,2);
%imshow(hmm);

%{
img = imread('cameraman.tif');
img = img(1:2:end, 1:2:end);
imshow(img)
imwrite(img, 'cam2.png');
%}

%{
a=[1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 15 16];
uint8(abs(fftshift(fft2(a))))
%}