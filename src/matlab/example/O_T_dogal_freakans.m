clear;
close all;
clc;

zeta =0.3;
wn=1;
y = tf([wn],[1 2*zeta*wn wn]);
step(y);