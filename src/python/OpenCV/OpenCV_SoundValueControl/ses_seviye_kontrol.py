
"""
Parmak arasında bulunan mesafeye göre
Ses degerini değiştirme yapar
"""

import  cv2
from time import time
import numpy as np
import handsTrackingModule as htm
from math import hypot
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume




#ses ayarları
devices = AudioUtilities.GetSpeakers()
interface = devices.Activate(
    IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
volume = cast(interface, POINTER(IAudioEndpointVolume))

#min ve max degerler için 
volRange = volume.GetVolumeRange()
minVol = volRange[0]
maxVol = volRange[1]

cap = cv2.VideoCapture(0)

ctime = ptime = 0

#ekran boyutlarını ayarlama
cap.set(3,1280)
cap.set(4,680)

handsTrack = htm.handsDetector(detectionCon=0.65,maxHands=1) #el hassasiyeti ve en fazla algılayacağı el sayısı


while True:
    _,img= cap.read()
    img = cv2.flip(img,1) #resmi aynalama
    img = handsTrack.findHands(img)
    lmList = handsTrack.findPosition(img,False)

    if len(lmList) != 0:
        x1,y1 = lmList[4][1],lmList[4][2]
        x2,y2= lmList[8][1],lmList[8][2]
        cx,cy = (x1+x2)//2,(y1+y2)//2
        cv2.circle(img,(x1,y1),20,(0,0,255),5)
        cv2.circle(img,(x2, y2), 20, (0, 0, 255),5)
        cv2.circle(img, (cx,cy), 20, (0, 255, 0), cv2.FILLED)
        cv2.line(img,(x1,y1),(x2,y2),(255,0,0),5)
        length = hypot(x1-x2,y1-y2)
        vol = np.interp(length,[50,300],[minVol,maxVol]) #değerleri istenilen degerler arasına cevirme
        volume.SetMasterVolumeLevel(vol, None) #bilgisayarın ses seviyesini ayarlama

    ctime = time()
    fps = 1/(ctime-ptime)
    ptime = ctime
    cv2.putText(img,f"FPS: {int(fps)}",(40,70),cv2.FONT_HERSHEY_COMPLEX,1,(255,0,0),2)

    cv2.imshow("Ses Ayarlama",img)


    if cv2.waitKey(1) == ord("q"):
        break
cv2.destroyAllWindows()

