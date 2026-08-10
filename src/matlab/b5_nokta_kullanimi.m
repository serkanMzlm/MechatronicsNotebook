clc;
clear;
clear all;

x = [1 2 3]
y = 3*x;
%x^2 , x/x veya x*x durumalarında  aynı diziler birbiri ile carpıldıgı için
%"." operatörü kullanılır.
y2 = x.^2  %x^2 yazılırsa hata alınır . kullanılmasının dedeni birebir bir
%carpım olsun diye
b= @(a) exp((-a.^2)/2);
integral(b,-Inf,Inf) %Inf burda sonsuz anlamı taşır