clear;
close all;
clc;

xlabel("zaman(t)")
ylabel("y(t)")
hold on
t= 0:0.01:10;
a=1;
y=exp(-a*t);
plot(t,y)
a=2;
y=exp(-a*t);
plot(t,y)
a=3;
y=exp(-a*t);
plot(t,y)
legend("a=1","a=2","a=3");
