import cv2
import mediapipe as mp
from time import time

class handsDetector():

    def __init__(self,mode=False,maxHands=2,detectionCon=0.5,trackCon=0.5):
        self.mode = mode
        self.maxHands = maxHands
        self.detectionCon = detectionCon
        self.trackCon = trackCon

        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(self.mode,self.maxHands,self.detectionCon,
                                        self.trackCon)
        self.mpDraw = mp.solutions.drawing_utils

    def findHands(self,img,draw=True): #parmaklarda noktalar ve cizgi cizme
        imgRGB = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
        self.results = self.hands.process(imgRGB)
        if self.results.multi_hand_landmarks:
            for handLms in self.results.multi_hand_landmarks:
                if draw:
                    self.mpDraw.draw_landmarks(img,handLms,self.mpHands.HAND_CONNECTIONS)
        return img


    def findPosition(self,img,draw = True,radius=10,color=(0,0,255),thickness=3,point=0,handNo=0): #belirtilen noktanın cember içine alma
        self.lmList= []
        xList = []
        yList = []
        bbox = []
        if self.results.multi_hand_landmarks:
            myHand = self.results.multi_hand_landmarks[handNo]
            for id, lm in enumerate(myHand.landmark):
                h,w,_ = img.shape
                cx,cy = int (lm.x*w),int(lm.y*h)
                xList.append(cx)
                yList.append(cy)
                self.lmList.append([id,cx,cy])
                if draw:
                    if id == point:
                        cv2.circle(img,(cx,cy),radius,color,thickness)
            xmax,xmin,ymax,ymin = max(xList),min(xList),max(yList),min(yList)
            bbox = xmax,xmin,ymax,ymin

        return self.lmList





def main():
    cap = cv2.VideoCapture(0)
    cTime = pTime = 0
    detector = handsDetector()
    while True:
        _,img = cap.read()
        img = detector.findHands(img)
        lmList = detector.findPosition(img,True,15,(255,200,0),5,8)

        img = cv2.flip(img,1)

        cTime = time()
        fps = 1/(cTime-pTime)
        pTime=cTime
        cv2.putText(img,str(int(fps)),(30,50),cv2.FONT_HERSHEY_PLAIN,3,(220,100,0),4)
        cv2.imshow("El Takip",img)
        if cv2.waitKey(1)==ord("q"):
            break




if __name__ == "__main__":
    main()
