clear;
close all;
clc;

t = -10:0.1:10;
y=heaviside(t);
plot(y);
axis([-10 0 0 2]);
