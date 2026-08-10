clear;
close all;
clc;

G=tf([1],[1 2])
H=tf([1],[1,3])
Y=feedback(G,H)

%hızlı bir şekilde transfer fonksiynu oluşturmuş olduk