clc;
clear;
clear all;
x = 0:1:10;
y1=x;
y2=2*x;
y3=3*x;
subplot(2,2,1)% tek sayfada kümeler oluşturu
plot(x,y1,'-r') 
grid
subplot(2,2,2)%ilk iki yer satır ve sütün
plot(x,y2,'--b')
grid
subplot(2,2,3) %son yer bu kutucuklarda yerleştirilecek konum
plot(x,y3,':k',"LineWidth",2)  
grid