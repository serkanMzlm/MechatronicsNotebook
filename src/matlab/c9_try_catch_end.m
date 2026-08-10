clear;
close all;
clc;

try % hata olsada programı kırmadan devam etmek için kullanılır
   a = alfa();  
catch
    warning("hata oldu")
    a = 0;
    disp(a)
end