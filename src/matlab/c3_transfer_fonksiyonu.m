clear;
close all;
clc;

num = [1 , 1]; %pay kısmı  kat sayıları
den = [1 2 3 4 5]; %payda kısmının katsayıları
y = tf (num,den) %transfer fonksiyonu olusturduk
a= zero(y) %sıfırlarını bul
b= pole(y) %kutupları bul
