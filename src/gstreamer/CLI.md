# Örnek Gstreamer Terminal Kodları
### Kamera -> UDP (jpeg & h265)
```bash 
gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw' ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host= port=3000
```
```bash
gst-launch-1.0 -v v4l2src device=/dev/video0 ! 'video/x-raw,width=640, height=480,framerate=30/1' ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=  port=3000
```
```bash
gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw' ! decodebin ! videoconvert ! x264enc tune=zerolatency ! rtph264pay ! udpsink host= port=5000
```
```bash
gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw' ! videoconvert ! nvvidconv ! 'video/x-raw(memory:NVMM), format=NV12' ! nvv4l2h265enc ! h265parse ! rtph265pay config-interval=1 pt=96 ! udpsink host=  port=3000
```
```bash
gst-launch-1.0 -v v4l2src device=/dev/video0 ! image/jpeg, width=640, height=480, framerate=30/1 !  queue ! jpegdec ! nvvidconv ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h265enc bitrate=4000000 ! h265parse ! rtph265pay config-interval=1 pt=96 ! udpsink host=10.223.6.6  port=3000
```

### UPD -> Ekran (jpeg & h265)
```bash
gst-launch-1.0 udpsrc port=3000 ! application/x-rtp, encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink
```
```bash
gst-launch-1.0 -v udpsrc port=3000 ! application/x-rtp,encoding-name=H265,payload=96 ! rtph265depay ! h265parse ! decodebin ! autovideosink
```
```bash
gst-launch-1.0 udpsrc port=5000 ! application/x-rtp,media=video,clock-rate=90000,encoding-name=H264,payload=96 ! rtph264depay ! avdec_h264 ! autovideosink
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264 latency=100 ! decodebin ! autovideosink
```
```bash
gst-launch-1.0 -v rtspsrc location=rtsp://192.168.144.25:8554/main.264 latency=100 ! decodebin ! autovideosink # -v çıktının daha detaylı olmasını dağlar.
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264 ! rtph265depay ! h265parse ! avdec_h265  ! autovideosink
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264 latency=100 ! decodebin ! videoconvert ! video/x-raw,format=I420 ! udpsink host= port=5000
```
- `dec_h265` yerine `avdec_h265` yazılabilir.
    
```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264 latency=100  ! rtph265depay ! h265parse ! dec_h265 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host= port=3000
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264 latency=100 ! decodebin ! videoconvert ! video/x-raw,format=I420 ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host= port=3000
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264  latency=10 ! rtph265depay ! h265parse ! avdec_h265 ! videoconvert ! nvvidconv ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h265enc ! h265parse ! rtph265pay config-interval=1 pt=96 ! udpsink host= port=3000
```
- `Bitrate` ayrası sayesinde veri boyutu azaltılır.
```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264  latency=10 ! rtph265depay ! h265parse ! avdec_h265 ! videoconvert ! queue  ! nvvidconv ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h265enc bitrate=600000 ! h265parse ! rtph265pay config-interval=1 pt=96 ! udpsink host= port=3000
```

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264  latency=1 ! rtph265depay ! h265parse ! avdec_h265 !  nvvidconv ! 'video/x-raw(memory:NVMM),width=1280,height=720' ! nvv4l2h264enc  bitrate=600000 insert-sps-pps=1 idrinterval=15 ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host= port=3000 sync=false async=false
```

- `videoflip` video üzerinde işlemler yapmamızı sağlar. 
    - `none` (0) bir değişiklik yapılmaması
    - `clockwise` (1) saat yönünde 90 derece
    - `rotate-180` (2) 180 derece döndürür
    - `counterclockwise` (3) saat yönünün tersine 90 derece
    - `horizontal-flip` (4) yatay çevirme
    - `vertical-flip` (5) dikey çevirme
    - `upper-left-diagonal` (6) 
    - `upper-right-diagonal` (7)
    - `automatic` (8)

```bash
gst-launch-1.0 rtspsrc location=rtsp://192.168.144.25:8554/main.264  latency=10 ! rtph265depay ! h265parse ! avdec_h265 ! videoflip method=rotate-180 ! nvvidconv ! 'video/x-raw(memory:NVMM),format=NV12' ! nvv4l2h264enc bitrate=600000 ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host= port=3000
```
