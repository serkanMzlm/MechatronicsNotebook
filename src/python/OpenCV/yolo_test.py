import cv2
import time
from ultralytics import YOLO

# export QT_QPA_PLATFORM=wayland

model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture('/dev/video8')

if not cap.isOpened():
    print("Kamera açılamadı!")
    exit()

frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

out = cv2.VideoWriter("output.mp4",
                      cv2.VideoWriter_fourcc(*"mp4v"),
                      30,  # fixed FPS for output
                      (frame_width, frame_height))

cv2.namedWindow("YOLO Detection", cv2.WINDOW_NORMAL)
cv2.resizeWindow("YOLO Detection", 1280, 720)

start_time = time.time()

while True:
    ret, frame = cap.read()
    if not ret:
        print("Video ended or frame read failed.")
        break

    current_time = time.time()
    elapsed_time = current_time - start_time

    # if elapsed_time > 60:
    #    print("10 seconds reached.")
    #    break

    results = model(frame)
    annotated_frame = results[0].plot()

    cv2.imshow("YOLO Detection", annotated_frame)
    # out.write(annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
out.release()
cv2.destroyAllWindows()


