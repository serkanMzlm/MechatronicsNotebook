from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
class pg(App):
    def build(self): 
        self.govde = BoxLayout(orientation = "vertical")
        self.yazi = Label(text = "Ekran",font_size="30sp")
        self.buton = Button(text = "Bas",size_hint_y=.2,
                            #background_normal="resim.png "resim koyma
                            #background_down ="resim.png" basılı oldugunda arka plan resmı
                            #buton = Button(text = "Buton",disabled = True) disable komutu sayesinde
                            background_color=[0,0,.8,1],#renk arkaplanrgba formatı
                          
                             )
        """
        background_disabled_normal normal arkaplan
        background_disabled_down   basıldıgında arkaplan
        """

        self.buton.bind(on_release=self.cek)
        self.buton.bind(on_press=self.bas)
        self.govde.add_widget(self.yazi)
        self.govde.add_widget(self.buton)
        return self.govde

    def bas(self,nesne):
        self.yazi.text = "basıldı"
    def cek (self,nesne):
        self.yazi.text = "bırakıldı"

pg().run()


