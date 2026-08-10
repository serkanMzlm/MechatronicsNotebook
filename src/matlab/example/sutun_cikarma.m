% fonk.m dosyasi ile aynı klasörde olmalıdır.
clear;
close all;
clc;

A = randi([0 100],4,5);
fprintf("matris:\n")
A
cikar = input("cıkarmak istediğiniz sütün:");
A = fonk(A,cikar)
