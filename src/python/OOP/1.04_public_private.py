class sinif:
    __deger1 = [] #private baş kısmında en az iki tane _ kulanılması ile oluşur
    _deger3 = [] #yari gizli tek alt cizgi
    def __init__(self,deger2):
        self.deger2 = deger2 #public
        self.__deger1.append(deger2)
o1 = sinif("ali")
print(o1.deger2) #__deger1 private oldugu icin erişimimiz yok deger2 public oldugu için her yerden erişebiliriz
print(o1._sinif__deger1) #private dısardan erişmek için bir yöntem nesne ismi _ sinif ismi ve gizli metot
