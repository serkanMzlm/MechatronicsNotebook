from kivy.app import  App
from kivy.uix.label import  Label
from kivy.uix.boxlayout import  BoxLayout
from kivy.uix.button import Button
from kivy.uix.textinput import  TextInput


class pg(App):
    def build(self):
        duzen=BoxLayout(orientation = "vertical")
        ilks = BoxLayout()
        ikis = BoxLayout()
        kullanici = Label(text = "Kullanici Adi")
        kkutu = TextInput()
        sifre = Label(text = "Sifre")
        skutu = TextInput()
        buton = Button(text="Devam Et")
        ilks.add_widget(kullanici)
        ilks.add_widget(kkutu)
        ikis.add_widget(sifre)
        ikis.add_widget(skutu)
        duzen.add_widget(ilks)
        duzen.add_widget(ikis)
        duzen.add_widget(buton)
        return duzen
pg().run()
 
