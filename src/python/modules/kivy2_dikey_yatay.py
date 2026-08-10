from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout




class prog(App):
    def build(self):
        duzen = BoxLayout(orientation="vertical")  # dikey 
        duzen2 = BoxLayout() #yatay
        yazi1 = Label(text="serkan")
        yazi2 = Label(text="mazlum")
        duzen2.add_widget(yazi1)
        duzen2.add_widget(yazi2)
        duzen.add_widget(duzen2)
        duzen.add_widget(Label(text="1234"))
       
        return duzen


prog().run()
