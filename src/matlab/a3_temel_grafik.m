clc;
clear;
clear all;
x = 0:1:10;
y = x;
stem(x,y) %x ve y eksenleri verilen grafik şeklinde 
% gösterir plottan farkı noktaları birleştirmez
grid %grid ekran seklinde gösterir parametre  olarak "on" ve "off" kullanılır
xlabel("x ekseni") %x eksenine isim veriririz
ylabel("y ekseni") %y eksenine isim veririz
title("Başlık") %grafiğe başlık vermek 
