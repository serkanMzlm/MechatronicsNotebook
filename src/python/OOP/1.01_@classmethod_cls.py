class Kisi():
    liste = []
    def __init__(self,isim,yetenek):
        self.isim = isim
        self.yetenek = []
        self.ekle()
    def ekle(self):
        self.liste.append(self.isim)
        self.yetenek.append(self.yetenek)
        print(f"{self.isim} kisisi basarili bir sekilde eklendi...")
    def goster(self):
        print("Kisiler : ")
        a = 1
        for k in self.liste:
            print(a,".", k)
            a += 1
    def sayi1(self):
        print("örnek fonksiyon calisti...")
        print(len(self.liste))
    @classmethod #hemen altinda bulunan fonksiyonu sınıfın fonksiyonu yapar
    def sayi(cls):
        print("sinifin fonksiyonu calisti...")
        print("kisi sayisi = ",len(cls.liste))

"""
Burda sinifin fonksiyonu  bir örnek oluşturulmadan 
cagrilabilir fakat örnek fonksiyonu cagrilabilmesi 
icin  ornekleme yapilmalidir.
"""
Kisi.sayi()
c1 = Kisi("Ahmet","kosma")
c2 = Kisi("Mert","yuzme")
c3 = Kisi("Burak","atlama")
c4 = Kisi("Hasan","konusma")
c1.goster()
c2.sayi1()
c2.sayi() #sinif fonksiyonu oldugu için hem örnekte hem de ornekleme yapilmadan calişabilir
Kisi.sayi()
