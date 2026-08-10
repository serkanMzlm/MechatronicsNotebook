clc;
clear;
clear all;

syms x a b ;
denklem = sin(x)==0;
cozum = solve(denklem,x) %denklemi cözer yane sinx hangi degerinde 1 oldugunu  
%gösterir  ilk deger parametre ikincisi ise degisken 
denk1 = a-b ==10;
denk2 = a+b == 20;
cozum2 = solve([denk1 denk2],[a b]);
cozum2.a  %burda a ve b nin degerleerini ögreniyoruz5
cozum2.b