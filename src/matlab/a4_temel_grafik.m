clc;
clear;
clear all;
x = 0:1:10;
y1=x;
y2=2*x;
y3=3*x;
hold on  %grafikte degerleri saklayip üstüne diger degerleri cizer
plot(x,y1,'-r') %grafikte cizgi red olur
plot(x,y2,'--b') % ii tane -- koymak kesikli cizgi demek blue
grid
figure %yeni bir grafik açar
plot(x,y3,':k',"LineWidth",2) %black : noktalı gösterir  
grid

