def  fonksiyon():
    print("fonksiyon calisti...")

class sinif(): # yada class sinif  '()' koymasakta olur
    print("sinif calisti")
    sayi = 10

"""
sinifla fonksiyon arasinda bulunan farklardan biri fonksiyonlar
cagrilana kadar calismazlar ama siniflar ise calisir
"""

fonksiyon() #fonksiyon cagrilir
print(sinif.sayi) # sinifin niteliklerine erisme
ornek = sinif() #sinifi orneklemis olduk
print(ornek.sayi)
ornek.sayi = 20
print("*"*20)
class sinif2():
    sayi = 20 #sinif niteligi tanımlamıs olduk
    def __init__(self): # init foksiyonu sayesinde  class ilk basta calışması engellenir
        self.sayi = 15 #ornek niteligi tanimlamis olduk
print(sinif2().sayi) #orneklem yapılmadan direk class uzerinden  erismek istedigimiz  zaman () konulur
ornek2 = sinif2()
print(ornek2.sayi)

"""
class icinde iki tane sayi adında kabiliyet vardir 
biz sayiyi cagirinca bize  self eki olani getirir
eger diger sayi degerine erismek istersek mecburen sinifin 
adi ile erismemeiz lazim
"""

"""
self kelimesi yerine baska bir sey yazilabilir
yazilan kelime kabiliyetlerin on eki olmak zorundadır 
sadece 
"""

print(sinif2.sayi)
print("-"*20,"ornek : ",sep="\n")
# __init__ kullanılma nedeni ornekle anlatımı
class a():
    def __init__(self):
        self.aa = []
class b():
    bb = []

deneme = a()
deneme2 = a()
deneme.aa.append(10)
print("__init__ kullanildi ==> ","deneme = ",deneme.aa,"\n","deneme2 = ",deneme2.aa)
deneme3 = b()
deneme4 = b()
deneme3.bb.append(30)
print("__init__ kullanilmadigi ==> ","deneme3 = ",deneme3.bb,"\n","deneme4 = ",deneme4.bb)

#init kulanilmadigi sinifta  butun nesneler etkilenir
