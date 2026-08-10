clear;
close all;
clc;

%% G = ss()
A=[0 -1;-2 2]
B=[0;1]
C=[1 0]
D=[0]

G=ss(A,B,C,D)

%% veya [] = ssdata(G)
[A,B,C,D] = ssdata(G) %ikiside ayni anlama gelir

%% Durum uzayini -> transfer fonskiyonu
[n,d]=ss2tf(A,B,C,D) 
Y= tf(n,d)

%% transfer fonksiyonu -> Durum uzayi
[a,b,c,d]=tf2ss(n,d)
