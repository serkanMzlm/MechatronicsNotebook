clear;
close all;
clc;
Kp=0;
Ki=1;
Kd=0;
C=pid(Kp,Ki,Kd);
den=[1];
num=[1 2 3];
G=tf(num,den);
H=1;
T=feedback(G*C,H);
step(T)