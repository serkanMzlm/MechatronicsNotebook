clear;
clc;
close all;
%%
hold on
kp =1;
num=[kp]; %denklemin pay
den=[1 2+kp]; %denklem payda
y = tf(num,den);
step(y)
kp=2;
den=[1 2+kp];
y = tf(num,den);
num=[kp];
step(y,"--")

%%
G = tf([1],[1 10 20])
Y = feedback(G,1) %geri besleme degeri girilir
step(y)

%% Kontroler eklenirse PID
kp=50
kd=10
C = pid(kp,0,kd)
G = tf([1],[1 10 20])
Y = feedback(C*G,1) %geri besleme degeri girilir
step(Y,"r")

legend("1","2","3")

