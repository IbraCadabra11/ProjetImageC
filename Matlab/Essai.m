clc;
clear all;
close all;
Imin = imread('017.pgm');
ImC = imcomplement(Imin);
Lev = graythresh(ImC);
str = strel('disk',4);
BW = im2bw(ImC,Lev);
BW1 = imclose(BW,str);
Im = imfill(BW1,'holes');
[Lab nbr]= bwlabel(BW,8);
props = regionprops(Lab,'Centroid','Perimeter','Area','ConvexImage');
centroids = cat(1, props.Centroid);
BW6 = bwmorph(BW1,'skel',Inf);
BW7 = bwmorph(BW6,'endpoints');
perimeter = cat(1, props.Perimeter);
area = cat(1, props.Area);
compacity = (perimeter.*perimeter)./(4*pi*area);
subplot(2,2,1);
imshow(BW7);
hold on
plot(centroids(:,1),centroids(:,2), 'b*')
hold off