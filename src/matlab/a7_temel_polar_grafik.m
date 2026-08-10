clc;
clear;
clear all;
x = 0:pi/100:2*pi;
y = sin(x);
z = cos(x);
polar(x,y,"--r") %polar grafikte gösterir
figure
polar(x,z) 
hold on
polar(x,y)
