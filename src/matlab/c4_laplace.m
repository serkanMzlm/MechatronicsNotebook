clear;
close all;
clc;

syms L TL f t;
f = exp(-2*t)*(cos(4*t));
L = laplace(f) %laplace alır 
TL = ilaplace(L) %ters laplace alır