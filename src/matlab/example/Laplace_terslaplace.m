clear;
close all;
clc;
%%  sin(t)*(e^-2t)

syms L fonk zaman;
fonk = sin(zaman)*exp(-2*zaman);
L = laplace(fonk) %laplace
ilaplace(L) % ters laplace
