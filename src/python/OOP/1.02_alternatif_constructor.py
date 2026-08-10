#alternatif inşa durumunda farklı parametrelerde farklı durumların oluşmasıdır
class sorgu:
    a = 0
    def __init__(self):
        self.liste = [("ali","kan",90,1),
            ("mert","can",20,2),
            ("burak","şeker",50,3),
            ("Tarkan","tehinoğlu",95,4)]
    def ara(self,deger,indis):
        return [l for l in self.liste if deger == l[indis]]
    def sor(self,ara = None, deger = None):
        self.a += 1
        durum = {
            "ad" : self.ara(deger,0),
            "soyad": self.ara(deger,1),
            "not" : self.ara(deger,2),
            "numara": self.ara(deger,3)
        }
        print(self.a,". sorgu :")
        for o in durum.get(ara, self.liste):
            print(*o, sep = ', ')
        print("-"*30)
o1 = sorgu()
o1.sor("ad","ali")
o1.sor() # Alternatif constructor  oldugu icin bu kisimda tüm listeyi gösterir
o1.sor("ad") # indis verilmedigi icin sonuc cıkmaz
