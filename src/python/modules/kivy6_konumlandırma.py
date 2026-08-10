from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.uix.button import Button


class pg(App):
    def build(self):
        self.govde = FloatLayout()

        konum = AnchorLayout(anchor_x = "right",anchor_y = "center")
        """
        x ekseni için : left right center
        y ekseni için : top bottom  center
        """
        buton = Button(text = "sag Orta",size_hint = (.1,.1))
        konum.add_widget(buton)

        self.govde.add_widget(konum)

        return self.govde

pg().run()
