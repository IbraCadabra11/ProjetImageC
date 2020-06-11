clc;
clear all;
close all;
Imin = imread('033.pgm');
Imin2 = imread('019.pgm');
Imcomp = imcomplement(Imin);
Imcomp2 = imcomplement(Imin2);
lev = graythresh(Imcomp);
lev2 = graythresh(Imcomp2);
BW = im2bw(Imcomp,lev);
Lab = bwlabel(BW,8);
props = regionprops(Lab,'Solidity');

bwconncomp