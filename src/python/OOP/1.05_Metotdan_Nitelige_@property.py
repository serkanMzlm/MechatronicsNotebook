class Program():
    def __init__(self):
        self._sayı = 0

    def degis(self,deger):
        self._sayı = deger
        return self._sayı

    @property #metot cagrılırken artık nitelik gibi kullanılabilir
    def sayı(self):
        return self._sayı

    @sayı.setter
    def sayı(self, yeni_değer):
        self._sayı = yeni_değer
        return self._sayı


o= Program()
o.sayı = 20 #sayi metodu nitelik gibi kullana bilir cagırabilir hatta deger atayabiliriz
print(o.sayı)
o.degis(30)
print(o.degis())

