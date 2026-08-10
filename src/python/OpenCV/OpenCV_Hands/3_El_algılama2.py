
# Eli modül yardımı ile algılma
# cvzone modülü 1.5 sürümü ve üstünde findPosition kaldırıldı

import cv2
from cvzone.HandTrackingModule import HandDetector

cap = cv2.VideoCapture(0)  #kamera ID numarası
detector = HandDetector(detectionCon=0.5,maxHands=2) #max el  sayısı ve el alglama hassasiyeti
I = [] #parmakların konumunu tutmak için

while True:
    _,img = cap.read() #kamera okuma
    img = cv2.flip(img,1) #resmi aynalama
    img = cv2.resize(img,(1280,640)) #ekran boyutlarını ayarlama
    img = detector.findHands(img) #elleri algılama
    I,box=detector.findPosition(img) #parmakları algılama (20 nokta algılama)

    if I:
        #x ve y  konumlarını alma ilk parametre nokta ikincisi x veya y demek
        f = detector.fingersUp()
        x1 = I[4][0]
        y1 = I[4][1]
        x2 = I[8][0]
        y2 = I[8][1]

        #belirtilen konumları cembere alma ve cizgi cekme
        cv2.circle(img,(x1,y1),7,(0,255,255),2)
        cv2.circle(img,(x2, y2), 7, (0, 255, 255), 2)
        cv2.line(img, (x1, y1), (x2, y2), (255, 0, 255), 2)
    #görüntüleme işlemi
    cv2.imshow("Img", img)

    if cv2.waitKey(1) & 0xFF == ord("q"): #'q' basılınca cıkış yapma
        break
cap.release() #serbest bırakır
cv2.destroyAllWindows() #pencereleri kapatmak
