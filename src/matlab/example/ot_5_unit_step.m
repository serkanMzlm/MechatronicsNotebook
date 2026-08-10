clear;
close all;
clc;
t = -10:0.001:10;
y=heaviside(t);
plot(t,y);
axis([-10 10 0 2]);