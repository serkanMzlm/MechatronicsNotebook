clear;
close all;
clc;
kp=1;
num=[kp];
den=[1 2+kp];
y = tf(num,den)
step(y); 