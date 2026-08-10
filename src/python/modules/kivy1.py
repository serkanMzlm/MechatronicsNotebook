from kivy.app import App # App sınıfını dahil edilir
from kivy.uix.label import  Label #ekranda yazı göstermemizi saglayan sınıf

ad =  input("adiniz:")
soyad = input("Soyadiniz")
class Program(App):
    def on_start(self):
        #uyguluma baslatıldıgında
        self.title = "ad soyad"
    def on_pause(self):
        #uyguluma arkaplana alındıgında
        return True
    def on_stop(self):
        #uygulama durduruldugunda
        pass
    def on_resume(self):
        self.yazi.text = soyad
        #tekrar giriş yapıldıgında
    def build(self):
        self.yazi=Label(text=ad)
        return self.yazi
    
Program().run() #progarmın calıştırtıma
