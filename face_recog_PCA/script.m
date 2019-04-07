clc;close all, clear;

sample_img = imresize(imread('yalefaces/subject1.gif'), 0.5);
nrows = size(sample_img, 1);
ncols = size(sample_img, 2);

for k = 1 : 15
    filename = strcat(strcat('yalefaces/subject', num2str(k)), '.gif');
    img = imread(filename);
    img = imresize(img, 0.5); 
    A(:, k) = img(:);
end

A = double(A);
rank_A = rank(A);
[u,s,v] = svd(A);
co_ords = transpose(A)*u(:, 1:rank_A);
imp_vars = [nrows, ncols, rank_A];


x = input('Enter image name (Enter 0 to exit) : ', 's');
testname = strcat('yalefaces/', x);
test_img = imread(testname);
test_img = imresize(test_img, 0.5); 
test_img = test_img(:);
test_img = double(test_img);
test_coords = transpose(test_img)*u(:,1:rank_A);

for j = 1:size(co_ords, 1)
    sim(j) = norm(co_ords(j, :) - test_coords);
end
[min_val, ind] = min(sim);
min_val

figure;
subplot(1,2,1)
subimage(uint8(reshape(test_img, [nrows,ncols])))
title('Entered Image');
subplot(1,2,2)
subimage(uint8(reshape(A(:, ind), [nrows,ncols])))
title('Matched Image');
