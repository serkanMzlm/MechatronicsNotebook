
from kivy.app import App
from kivy.clock import Clock
from kivy.uix.label import Label
import datetime

class Program(App):
    def build(self):
        self.yazi = Label(text = "Merhaba",markup = True)

        Clock.schedule_interval(self.degistir,1) # 1 saniye sonra, self.degistir adlı fonksiyonu çalıştır

        return self.yazi

    def degistir(self,event):
        zaman = datetime.datetime.now()
        self.saat = "[size=50sp]{}:{}:{}[/size]".format(zaman.hour,zaman.minute,zaman.second)
        self.yazi.text = self.saat




Program().run()
