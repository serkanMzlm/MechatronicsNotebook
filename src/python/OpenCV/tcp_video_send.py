import cv2
import numpy as np
import socket
import struct
import pickle
import threading

# Ağ ayarları
host = '192.168.xx.xxx'
port = 5000

# Kare boyutları (gerektiğinde ayarlayın)
frame_width = 640
frame_height = 480

# Video yakalama ayarla
cap = cv2.VideoCapture('/dev/video8')

# Kamera ayarlarını kontrol et
if not cap.isOpened():
    print("Kamera açılamadı!")
    exit()

# Soket oluştur
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host, port))
connection = client_socket.makefile('wb')

def send_frame():
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Görüntü alınamıyor")
            break
        
        # Görüntünün ortasına kare çiz
        height, width = frame.shape[:2]
        center_x, center_y = width // 2, height // 2
        square_size = min(width, height) // 4  # Kare boyutu
        
        # Kare çiz
        top_left = (center_x - square_size//2, center_y - square_size//2)
        bottom_right = (center_x + square_size//2, center_y + square_size//2)
        cv2.rectangle(frame, top_left, bottom_right, (0, 255, 0), 2)
        
        # Görüntüyü sıkıştır ve gönder
        result, encoded_frame = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        data = pickle.dumps(encoded_frame, 0)
        size = len(data)
        
        try:
            client_socket.sendall(struct.pack(">L", size) + data)
        except:
            print("Bağlantı hatası")
            break

    cap.release()
    client_socket.close()

# Yayın thread'ini başlat
thread = threading.Thread(target=send_frame)