clear;
close all;
clc;

num= [1 2];
den= [1 -2 0 3];
y = tf(num,den)
step(y)
