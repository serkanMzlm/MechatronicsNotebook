%% 
clear;
close all;
clc;

a1 = magic(3) %rasgele 3x3 matris olusturur
a2 = [1:3;
4 5 6;
linspace(1,9,3)]
c1 = a1+a2 %matris toplama
c2 = a1*a2 %normal matris carpim
c3 = a1.*a2 %eleman eleman carpım
c4 = sqrt(a1) %SQuaRe Root karekök alma 
a3 = randi(100,3,4)
sum(a3)%icindeki sayilari toplar sütün sütün toplar
sum(a3,2) %2 boyututta toplar yane satır satır toplar
sum(a3,'all') % tüm hepsini toplar yada sum(sum(a3)) seklinde yazilabilir
diag(a2) % kosegenleri gosterir
trace(a2) %kosegenleri toplar

