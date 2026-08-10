clc;
clear;
clear all;
i = 1;
while i<10
    x(i) = i;
    i= i+1;
end
x
for i=1:1:10 %while farkı bır baslangıcı içinde belirlememiz
    x = x+1;
end
x