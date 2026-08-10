class araba(): #Taban Sınıf
    def __init__(self,marka,model,guc,motor):
        self.marka = marka
        self.model = model
        self.guc = guc
        self.motor = motor
        self.km = 0

    def yazdir(self):
        print("-"*30)
        print(f"Marka : {self.marka}\nModel : {self.model}\nGuc : {self.guc} Hp\nMotor : {self.motor}")
        print("-" * 30)


class araba1(araba): #Alt sınıf miras aldıgını belirtmek için () icine miras alınıcak  sınıf yazılır
    def __init__(self,marka,model,guc,motor):
        super().__init__(marka,model,guc,motor) #belirtiğimiz özelliler dışında olanların kaybolmasını engelleriz
        self.marka = marka
        self.model = model
        self.guc = guc
        self.motor = motor

class araba2(araba): #Türetilen sınıf (alt sınıf)
    pass

bmw = araba1("BMW","M6 Cabrio","560","4.4")
bmw.yazdir()

audi = araba2("Audi","Q2","116","1.1")
audi.yazdir()

mercedes = araba("Mercedes-Benz","C-klasse W205","136","1.6")
mercedes.yazdir()

"""
araba1 ve araba2 araba  sınıfından türetildigi için 
bu sınıfın  metoto ve niteliklerine erişebilirler
"""


