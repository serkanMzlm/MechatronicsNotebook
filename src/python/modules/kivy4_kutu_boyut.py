from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout #pencere boyutunu ayarlamaya yarar


class pg(App):
    def build(self):

        self.duzen = FloatLayout() 

        self.buton = Button(text = "buton",
                                size_hint = (.3,.3), pos = (280,10)) #size_hint genel arana oranı  pos konumu

        self.duzen.add_widget(self.buton) 

        return self.duzen


pg().run()
