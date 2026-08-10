ardu = arduino('com5','Uno')
methods(ardu)
for i=1:1000
    y(i) = readVoltage(ardu,"A0");
    y(i) = (y(i)/5)*180;
    plot(y,'b',"LineWidth",2)
    xlabel("Gelen veri");
    ylabel("Aci");
    title("X Ekseni Konum");
    set(gca,'color',[0 0 0])
    drawnow 
    
end
