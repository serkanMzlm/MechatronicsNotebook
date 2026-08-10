function  y=fonk(A,B)
x = size(A,2);
y = [A(:,1:(B-1)) A(:,(B+1):x)];

end