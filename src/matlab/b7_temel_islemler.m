clc
clear
close all %acılmış diger pencereleri kapatır
%% yeni section oluştumak için kullanılır % den sonra bir boşluk bırakılır

factorial(10) %faktöriyel almak 
sqrt(100) %karekök almak
exp(3) %e^3 demek
sin(pi/6) %radyan cinsinden yazılmalıdır
sind(30) %derece cinsinden yazılabilir

%% section 2
a = [1 2 3 4];
b = [5 6 7 8];
c = [a;b];
d = c' %transpozunu aldık yane satırlar ile sütünlar yer değiştirir
e = transpose(d) % ' ile ayni
f = magic(3) %3x3 matris olusturur
numel(f)%(number of selement) sicinde kac tane eleman oldugunu verir
length(f) %bir matrisin uzun kenarının sayisini verir
size(f) %satir ve sütün sayisini verir
f(end,2) %matris in son satir 2 sütün cagırır.


%% section 3
%0-270 arasını 10 eşit parcaya böler
a1 = linspace(0,270,10) % linspace(baslangi,bitis,bolunecek_sayi)
b1 = 0:30:270 %baslangic:artis:bitis
c1 = rand(4) %rasgele 4x4 matris olusturur
c2 = rand(2,4) %2 sira 4 sutun olusturur
c3 = randi(100,4,4) %rasgele tam sayilar üretir 1-100 arasında 4x4

%% section 4
%radyandan dereceye  dereceden radyana cevırme

radyan = deg2rad(30) %radyana cevriri
derece = rad2deg(radyan) %deeceye cevriri

%% section 5
 log(10) % ln10 anlamına gelir
 log10(10) %log 10 tabanında 10 analmına gelir





