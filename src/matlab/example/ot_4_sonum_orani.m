clear;
close all;
clc;
% dogal frekans 10-90
wn=1;
zeta=0.3
y=tf([wn],[1 2*zeta*wn wn])
step(y)