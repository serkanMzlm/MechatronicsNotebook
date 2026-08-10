import cv2
import numpy as np
import face_recognition as fr
from matplotlib import pyplot as plt

xImg = fr.load_image_file("x.jfif") #resmin klasör yeri
xImg = cv2.cvtColor(xImg,cv2.COLOR_BGR2RGB) #RGB formatına cevirme

yImg = fr.load_image_file("y.jfif") #resmin klasör yeri
yImg = cv2.cvtColor(yImg,cv2.COLOR_BGR2RGB) #RGB formatına cevirme

xFaceLoc = fr.face_locations(xImg) #parametre bize yuzun kordinatlarını verir (x1,y1,x2,y2)
xEnco = fr.face_encodings(xImg)[0]

yFaceLoc = fr.face_locations(yImg)
yEnco = fr.face_encodings(yImg)[0]

#Yüz kordinatlarını alıp kare cizme
cv2.rectangle(xImg,(xFaceLoc[0][3],xFaceLoc[0][0]),(xFaceLoc[0][1],xFaceLoc[0][2]),(255,0,0),2)
cv2.rectangle(yImg,(yFaceLoc[0][3],yFaceLoc[0][0]),(yFaceLoc[0][1],yFaceLoc[0][2]),(255,0,0),2)

res = fr.compare_faces([xEnco],yEnco) #iki resmi kıyaslayıp benzer olup olmadığına bakıyor (False true)
Dis = fr.face_distance([xEnco],yEnco)

#resimlerin üstüne yazı yazma
cv2.putText(xImg,f"{res} discord: {round(Dis[0],2)}",(10,20),cv2.FONT_HERSHEY_COMPLEX,0.4,(0,0,255),1)
cv2.putText(yImg,f"{res} discord: {round(Dis[0],2)}",(10,20),cv2.FONT_HERSHEY_COMPLEX,0.4,(0,0,255),1)


#tek bir ekranda resimleri gösterme
plt.figure(figsize=(12,6)) #ekran ölcüleri
plt.subplot(1,2,1) #resim konumu
plt.imshow(cv2.cvtColor(xImg,cv2.COLOR_RGB2BGR)) #resim
plt.title("x image") #baslık
plt.xticks([]) #x ekseni
plt.yticks([]) #y ekseni
plt.subplot(1,2,2)
plt.imshow(cv2.cvtColor(yImg,cv2.COLOR_RGB2BGR))
plt.title("y image")
plt.xticks([])
plt.yticks([])
plt.show() #ekranı calıştıma
