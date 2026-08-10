import cv2
import socket
import struct
import pickle
import numpy as np

# Sunucu ayarları
host = '0.0.0.0'  # Tüm arayüzleri dinle
port = 5000

# İstenen görüntü boyutları (göndericiyle aynı olmalı)
display_width = 1280
display_height = 720

# Soket oluştur
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((host, port))
server_socket.listen(5)
print(f"Yayın dinleniyor: {host}:{port}")

conn, addr = server_socket.accept()
print(f"Bağlantı kabul edildi: {addr}")
data = b""
payload_size = struct.calcsize(">L")

# Pencereyi sadece 1 kere oluştur
cv2.namedWindow('Raspberry Pi Yayını', cv2.WINDOW_AUTOSIZE)
# cv2.resizeWindow('Raspberry Pi Yayını', display_width, display_height)

try:
    while True:
        # Frame boyut bilgisini al
        while len(data) < payload_size:
            packet = conn.recv(4096)
            if not packet: 
                break
            data += packet
        if len(data) < payload_size:
            break
            
        packed_msg_size = data[:payload_size]
        data = data[payload_size:]
        msg_size = struct.unpack(">L", packed_msg_size)[0]
        
        # Frame verisini al
        while len(data) < msg_size:
            packet = conn.recv(4096)
            if not packet:
                break
            data += packet
        if len(data) < msg_size:
            break
            
        frame_data = data[:msg_size]
        data = data[msg_size:]
        
        # Görüntüyü aç
        try:
            frame = pickle.loads(frame_data, fix_imports=True, encoding="bytes")
            frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
            
            if frame is not None:
                # Görüntüyü istenen boyuta yeniden ölçeklendir
                frame = cv2.resize(frame, (display_width, display_height))
                cv2.imshow('Raspberry Pi Yayını', frame)
            else:
                print("Hata: Geçersiz frame alındı")
                
        except Exception as e:
            print(f"Frame işleme hatası: {e}")
            continue
            
        if cv2.waitKey(1) == ord('q'):
            break

except KeyboardInterrupt:
    print("Kullanıcı tarafından durduruldu")
except Exception as e:
    print(f"Hata oluştu: {e}")
finally:
    cv2.destroyAllWindows()
    conn.close()
    server_socket.close()
    print("Bağlantılar kapatıldı")
