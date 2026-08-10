clear all;
close all;
clc;
%% Mükemmel sayi kendisi hariç pozitif tam bölenlerin toplamına eşit olan sayilardir
sayi = input("sayi girin:");
bolen = [];
for i=1:sayi-1
    if mod(sayi,i)==0
        bolen = [bolen i];
    end
end
if sayi==sum(bolen)
    fprintf("%d sayisi mukemmel sayidir.\n",sayi)
else
    disp("Girdiğiniz sayi mükemmel sayi değildir.")
end

        
