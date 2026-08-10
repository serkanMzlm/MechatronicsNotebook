clear;
clc;
close all;

num=[1];
den=[1 3 1];
g = tf(num,den);
h = 1;  
kp=1;
ki=0;
kd=0;

c = pid(kp,ki,kd);
t = feedback(c*g,h);
step(t)
hold on;
kp=1;
ki=5;
kd=0;

c = pid(kp,ki,kd);
t = feedback(c*g,h);
step(t)
legend("1","2")

