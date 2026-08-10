class sinif:
    veri = []
    def __init__(self,isim):
        self.veri.append(isim)
    def ornek_metot(self):
        print("Örnek metot calişti...")
        print("-"*30)
    @classmethod
    def sinif_metot(cls):
        print("Sınıf metot calışti...")
        print("-"*30)
    @staticmethod #sinif metotları ile ayni tek fark cls gibi ilk parametre kullanilmasina gerek yok
    def statik_metot():
        print("Statik metot calişti...")
        print("-" * 30)
print("-"*30)
sinif.sinif_metot()
o1 = sinif("ali")
o1.ornek_metot()
o1.sinif_metot()
o1.statik_metot()
sinif.statik_metot()
