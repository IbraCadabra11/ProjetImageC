clc;
clear all;
close all;
ImIn = imread('001.pgm');
ImC = imcomplement(ImIn);
str = strel('disk',5,0);
Lev = graythresh(ImC);
BW = im2bw(ImC,Lev);
BW2 = imclose(BW,str);
BW3 = imfill(BW2,'holes');
Lab = bwlabel(BW3,8);
Props = regionprops(Lab,'Centroid','MajorAxisLength','MinorAxisLength');
centers = Props.Centroid;
diameters = mean([Props.MajorAxisLength Props.MinorAxisLength],2);
R = diameters/2;

surface = length(find(BW3)); % surface
% centre gravité
[r c] = find(BW3);
CGi = sum(r)/surface;
CGj = sum(c)/surface;

% rayon "équivalent" S=PIxR2 pour disques
[Nblig Nbcol] = size(ImIn); 
CERCLE = logical(zeros(Nblig,Nbcol));
% création image binaire cercle équivalent 
for VThetaDeg =0:0.1:360
    VTheta = VThetaDeg *pi / 180;

    XCercle = round(CGi + R * cos(VTheta));
    YCercle = round(CGj + R * sin(VTheta));
    CERCLE(XCercle,YCercle) = 1;
end
Label = bwlabel(CERCLE,8);
CERCLE = label2rgb(Label,'jet',[0 0 0]);
figure; hold on;
imshow(BW3);
imshow(CERCLE);
hold off;
