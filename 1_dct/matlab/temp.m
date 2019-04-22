clear all;
N = 64;
fin=fopen('cam1_64_64.raw','r');
I=fread(fin,N*N,'uint8=>uint8'); 
ft_Z=reshape(I,N,N);
ft_Z=ft_Z';
im = ft_Z;
%subplot(1,2,1);
imshow(ft_Z);

for i = 1:64;
  f = fft(im(i,:));
  
  for k = 1:64;
    f(k) = f(k) * exp((-j*pi*(k-1))/128);
    if k == 1;
      f(k) *= 0.5*((1/64)^0.5);
    else
      f(k) *= 0.5*((2/64)^0.5);
    end
  end
  dc(i,:) = f;
end
dc = transpose(dc);

for i = 1:64;
  f = fft(dc(i,:));
  
  for k = 1:64;
    f(k) = f(k) * exp((-j*pi*(k-1))/128);
    if k == 1;
      f(k) *= 0.5*((1/64)^0.5);
    else
      f(k) *= 0.5*((2/64)^0.5);
    end
  end
  ans(i,:) = f;
end
transpose(ans)(1,:)