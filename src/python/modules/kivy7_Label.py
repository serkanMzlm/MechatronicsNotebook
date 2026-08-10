from kivy.app import  App
from kivy.uix.label import Label

class pg (App):
    def build(self): 
        yazi = Label(text= "Yazı Boyutu 30 pixel",
                     font_size="30sp", #yazı boyutunu ayarlar pixel
                     halign="center",#yazıyı saga sola veya ortaya hizlar
                     color=[0,1,.9,1],#renk ayarlama RGBA [1,1,1,1]
                     italic=True, #italik
                     line_height=4,#satır arası genişlik
                     bold=True) #kalın
                     
        return yazi
pg().run()
