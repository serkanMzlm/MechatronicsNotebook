clear;
clc;
close all;

a =input("bir deger girin: "); %kullanıcıdan veri almak "float ve int"
b = input("adinizi girin: ","s"); %string girilmesini sağlar

disp("adiniz:")
disp(b) %ekranda gösterme
fprintf("sayiniz %d\n",a) %ekranda görterme disp farkı icine atama yapıla
%bilir yer tutm yapılamaz bunda yapilir.

switch x
    case 1
        fprintf("ilk secenek")
    case 2 
        fprintf("iki secenek")
    case 3
        fprintf("uc secenek")
    case {5,6,7,8,9} %birden fazla sayi girmek istersek bu sekilde
        fprintf("bu sayılar yok")
    case "ankara"
        fprintf("başkent")
    otherwise
        fprintf("anlayamadim")
end
a= rand(2);
b= rand(3);
isequal(a,b) %iki dizinin birbirine eşit olup olmadıgını kontrol eder

       
    
