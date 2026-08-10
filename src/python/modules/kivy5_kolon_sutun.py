from kivy.app import  App
from kivy.uix.button import  Button
from kivy.uix.gridlayout import  GridLayout #kutulara bölme

class pg(App):
    def build(self):
        
        govde = GridLayout(cols = 2) #en fazla iki sutun
#       govde = GridLayout(rows = 3) en fazla uc satır
       
        for i in range(29):
            govde.add_widget(Button(text = f"{i+1}"))

        return govde

pg().run()
