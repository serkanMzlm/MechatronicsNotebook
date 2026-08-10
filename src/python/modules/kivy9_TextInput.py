from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.textinput import TextInput

class Program(App):
    def build(self):

        self.anaDuzen = BoxLayout(orientation = "vertical")

        self.ilkSatir = BoxLayout()
        self.ikinciSatir = BoxLayout()

        self.nick = Label(text = "Kullanıcı Adı")
        self.nickKutu = TextInput()

        self.sifre = Label(text = "Şifre")
        self.sifreKutu = TextInput(multiline=False, #enter basılınca alt satıra gecmıyıcek
                                   password=True, #yazılanlar mask bulunan karakter gözükücek
                                   password_mask="*") # default olarak * dır 

        self.buton = Button(text = "Giriş Yap",
                            background_color=[0.1,1,.5,1])


        self.ilkSatir.add_widget(self.nick)
        self.ilkSatir.add_widget(self.nickKutu)

        self.ikinciSatir.add_widget(self.sifre)
        self.ikinciSatir.add_widget(self.sifreKutu)

        # Şimdi hepsini ana düzene yerleştiriyoruz

        self.anaDuzen.add_widget(self.ilkSatir)
        self.anaDuzen.add_widget(self.ikinciSatir)
        self.anaDuzen.add_widget(self.buton)

        return self.anaDuzen

Program().run()
