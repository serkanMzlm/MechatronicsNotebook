clc;
clear;
clear all;

syms x l  %symbol oluşturuldu
diff(sin(x)); %türek alma komutu
a = int(1-x/l+x^2/l^2) %integral alma komutu
diff(sin(x)^4,2); %, sonraki kısım kacıncı türevi alındıgı belirlenir