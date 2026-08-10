% "..." ve "%" isareti yorum satırı yapar
clc; % ekranı temizler
clear; % değişkenleri siler
close all; % değişkenler baglantılı olan her şeyi siler

%{
 doc bilgi istenilen komut bu sayede istenılen konudun dökümanina ulaşılır
format compact cıktıların bosluklarını kapatır
formt loose cıktıların bosluklarını acar
who degişkenlerimizi gösterir
whos degişkenlerimiz hakkında bılgı verır
%}
a = 10; % ";" sayesinde ekrana basılması engelenir
b = 1:9; % 1-9 kadar  bir artarak sayıları kaydeder
c = 1:3:9; % 1-9 kadar  üc artarak sayıları kaydeder
z = [1 2 3; 4 5 6 ; 7 8 9]; % bir matris yapı ; satır olusturur
z(1,:) %birinci satırı cağırır 
disp("merhaba") 
%ekrana yazı yazar
length(z) % dizilerin uzunlugunu verir
