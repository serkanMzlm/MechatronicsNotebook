clear;
close all;
clc;

A = randi([0 100],4,8)
cikar = input("Cıkarmak istediğiniz sütün:");
A = fonk(A,cikar)
