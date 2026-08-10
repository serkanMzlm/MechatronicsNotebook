import cv2,time
import mediapipe as mp

kamera = cv2.VideoCapture(0) #görüntü alacağımız yerin ID numarası (webcam = 0)

mpEl = mp.solutions.hands
el = mpEl.Hands(max_num_hands=1) #En fazla kac el algılayacagı ve
mpCiz = mp.solutions.drawing_utils #elimizde bulunan 21 noktayı gösterir

cTime  = 0
pTime = 0

while True:
    suc, img = kamera.read() #gelen verileri okur suc kontrol değişkenidir yane gelen veri var mı
    img = cv2.flip(img,1) #görüntü aynalar
    imgRGB = cv2.cvtColor(img,cv2.COLOR_BGR2RGB) #RGB formatına çevirir
    sonuc = el.process(imgRGB) #eli algılamaya yarar.

    if sonuc.multi_hand_landmarks: #elin olup olmadığını kontrol eder
        for elLms in sonuc.multi_hand_landmarks: #her bir noktanın konum bilgilerini verir
            for id,lm in enumerate(elLms.landmark): #her noktaya ID atayıp bunların konumlarını yazdırır
                #lm degeri konum bunu pixel cevirmek için yukseklık ve genişlikle carparız
                h ,w,_ = img.shape #resmin h w ve kanal  degerlerini alır
                cx , cy = int(lm.x*w),int(lm.y*h)
                print(f"id:{id}",f"x:{cx}",f"y:{cy}","-"*15,sep="\n")

                if id == 4:
                    cv2.circle(img,(cx,cy),20,(200,0,255),4) #belirlediğimiz konumu cember içine alır kalınlık "cv2.FILLED" yapılırsa daire olur
                    
            mpCiz.draw_landmarks(img,elLms,mpEl.HAND_CONNECTIONS) #bu konumlara nokta koyar son parametre sayesinde noktalar birleştirilir

#fps hesaplama
    cTime = time.time()
    fps = 1 / (cTime-pTime)
    pTime = cTime

    cv2.putText(img,str(int(fps)),(20,50),cv2.FONT_HERSHEY_PLAIN,3,(123,123,0),3) #ekrana yazı ekleme

    cv2.imshow("El Takip", img)  # Cerceve adı ve sergilenecek resim

    if cv2.waitKey(1) == ord("q"): #waitKey sayesinde klavye bağlama sağlıyoruz
        break
kamera.release() #serbest bırakır
cv2.destroyAllWindows()
