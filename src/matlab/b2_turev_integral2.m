clc;
clear;
clear all;

denklem = @(x) cos(x); %x baglı bir cos(x) denklemi belirtik
integral(denklem,0,pi/2) %denklem min ve max degerleri

x2 = 0:pi/100:pi/2;
trapz(x2,cos(x2)) %belirtigimiz aralıkta kücük paraclara ayırarak hesabı
% bulur burda pi/1000 yapılırsa 1 sonucuda daha fazla yaklaşır.
denklem2=[3 -2 -4] %3x^2 -2x -4 denklemini yazmış olduk
polyint(denklem2) % denklemin integralini aldık
polyint(denklem2,10) % denklem sabitini(c) biz belirlemiş olduk "10"