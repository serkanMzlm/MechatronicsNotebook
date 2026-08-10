clear;
close all;
clc;

%% problem 1 sin ve cos grafigi ciz
t = 0:pi/100:2*pi;
y = sin(t);
plot(t,y,"--")
xlabel('Zaman');
ylabel("Genlik");
title("Sin-Cos");
hold on
y = cos(t);
plot(t,y,"*");
legend("sin","cos");
hold off

%% problem 2 bar grafigi icine bir vektor koy
A=[2 1 3 -2 4];
bar(A); %bar grafigi olusturur
xlabel("Bar");
title("Bar Grafigi");

%% problem 2.1 bar grafigi 
stairs(A); %cizgi seklinde grafik olusur
xlabel("Bar");
title("Bar Grafigi");

%% problem 3 3 boyut sin grafik oluşturun
x = -2:0.1:2;
y = x;
[X,Y]=meshgrid(x,y);
Z =X.*exp(-X.^2-Y.^2);
mesh(Z);

%% problem 3.1 3 boyut cos grafigi olusturun (surf)
t = 0:pi/10:2*pi;
[X,Y,Z] = cylinder(2+cos(t));
surf(X,Y,Z)

%% problem 4 vektor uzunlugu hesaplama
x=[15 20 30 25 10];
y=[5 0 1 0 0]; %dilim butunlugu 1 yazdıgımız bir adım onde olur
pie3(x,y); %y dilim butunlugu belirtir
title("Yüzdelik Dilim");
legend("15","20","30","25","10");
figure(2);
bar(x);
uzunluk = length(x) %x kac elemanlı oldugu
toplam = sum(x) %x elemanları toplamı
en_buku=max(x) %en buyuk eleman 
en_kucuk=min(x) %en kucuk eleman
ortalam = mean(x) %ortalama hesaplar
carpim = prod(x) %vektorun icinde bulunan elemanları carpar
sirala = sort(x) %kucukten buyuge siralar
