clear ;
close all;
clc;

sayilar = [];
while true
sayi = input("bir sayi girin:");
sayilar = [sayilar sayi];
sorgu = input("Çıkış(0):");
if sorgu == 0
    break
end
end
fprintf("En büyük sayi:%d\n",max(sayilar))
fprintf("En küçük sayi:%d\n",min(sayilar))
fprintf("ortalama :%.2f\n",sum(sayilar)/length(sayilar))
