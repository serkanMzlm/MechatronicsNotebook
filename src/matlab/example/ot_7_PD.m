clear;
close all;
clc;
kp=500
kd=10
C=pid(kp,0,kd)
G=tf([1],[1 10 20])
Y=feedback(C*G,1);