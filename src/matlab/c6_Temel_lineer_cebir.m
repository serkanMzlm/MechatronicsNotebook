clc;
clear all;
close all;

a = magic(3);
b = randi(5,2,3);
c = b' %transpozesi 
det(a) %determinat hesaplar
trace(a) %ksegenini toplar
diag(a) %kosegeni bir vektor yapar
sum(b) %sütunları toplar
triu(b)%ust ücgen matrısını oluşturu
tril(b)%alt ucgen matrisini olusturur
sortrows(a,1)%bir matrisin belirtilen sütünuna göre sıralar



