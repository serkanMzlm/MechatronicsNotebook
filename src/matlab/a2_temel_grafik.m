clc;      % Konsol Ekranını Temizler
clear;    % Workspace Kısmında Bulunan Verilerin Silinmesini Sağlar
 
x_label =    "Time [s]";
y_label =    "Position [m]";
y_label_2 =    "Position 2 [m]";
main_title = "Position / Time Graph";

t = 0:0.1:2*pi;
a = sin(t);
b = 125*cos(t);

% 'hold on' Grafiklerde bulunan değerlerin saklanmasını sağlar. Bu sayede
% bir grafiğin üstünde farklı veriler gösterilebiliriz. plot komutu x, y
% eksenine kullanılıcak veriler ve oluşucak çizginin rengi ve kalınlığının
% ayarlanmasını sağlar.
hold on
yyaxis left; % Değerlerin hangi tarafta olucağını ayarlar.
plot(t, a, 'r', LineWidth=2);
xlabel(x_label, FontSize=10); % x ekseni başlığı ve font boyutu
ylabel(y_label, FontSize=10); % y ekseni başlığı ve font boyutu
title(main_title, FontSize=10); % Ana başlık ve font boyutu
text(1, 0, "Theta is 90") % grafiğin belirli noktasında yazı yazmayı sağlar
axis([0 6 -1 1])          % grafiğin gösteriliceği aralık

yyaxis right;
plot(t, b, 'b', LineWidth=2);
ylabel(y_label_2, FontSize=10); % y ekseni başlığı ve font boyutu
axis([0 6 -150 150])          % grafiğin gösteriliceği aralık

grid on; % grid aktif veya pasif olmasını sağlar.

% Grafikte  cizgilerin isimlendirmemizi saglar lambda , pi ,epsilon gibi 
 % simgeleri kullanmak için \ kullanılır.
legend("sin", "cos")     