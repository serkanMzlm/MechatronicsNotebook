# Raspberry Pi

## Uzaktan Bağlantı ve Yönetim

```bash
# SSH
ssh pi@raspberrypi.local      # Varsayılan kullanıcı: pi (eski) / username (yeni)
ssh pi@192.168.1.100

# VNC (masaüstü paylaşımı)
# raspi-config → Interface → VNC → Enable
vncviewer raspberrypi.local:5900

# Dosya kopyalama
scp dosya.py pi@raspberrypi.local:~/
rsync -avz ./proje/ pi@raspberrypi.local:~/proje/

# mDNS (LAN'da IP olmadan bul)
ping raspberrypi.local
avahi-browse -at   # Ağdaki tüm mDNS servislerini gör
```

## config.txt - Donanım Yapılandırması

- **Boot yapılandırma dosyası:** `/boot/firmware/config.txt` (eski sürümlerde `/boot/config.txt`)

```ini
# GPU bellek ayırma (MB)
gpu_mem=128

# I2C arayüzü aktif
dtparam=i2c_arm=on

# SPI arayüzü aktif
dtparam=spi=on

# UART aktif
enable_uart=1

# Kamera modülü
dtoverlay=imx219       # IMX219 (Pi Kamera V2)
dtoverlay=ov5647       # OV5647 (Pi Kamera V1)

# Fan kontrolü (PWM)
dtoverlay=gpio-fan,gpiopin=18,temp=60000  # 60°C'de devreye girer

# HDMI zorla (monitör bağlı olmasa da)
hdmi_force_hotplug=1

# Overclock (Pi 4 - soğutucu gerekli)
arm_freq=1900
gpu_freq=600
over_voltage=2
```

## GPIO

```bash
# GPIO numaralarını göster
sudo cat /sys/kernel/debug/gpio

# GPIO'ya userspace erişim izni udev dosyası oluştur.
# SUBSYSTEM=="gpio*", PROGRAM="/bin/sh -c 'chown -R root:gpio /sys/class/gpio && chmod -R 770 /sys/class/gpio'"
sudo nano /etc/udev/rules.d/99-gpio-permissions.rules

sudo udevadm control --reload-rules
sudo usermod -aG gpio $USER   # Kullanıcıyı gpio grubuna ekle
```

## Kamera

```bash title="Kamera Tespiti"
vcgencmd get_camera           # Eski API (Pi 4 / legacy)
# supported=1 detected=1  → Bağlı ve tespit edildi
# supported=1 detected=0  → Bağlı ama tespit edilemedi
# supported=0 detected=0  → Bağlı değil veya desteklenmiyor

dmesg | grep -i camera        # Kernel mesajlarında kamera bilgisi
dmesg | grep -i imx219

# v4l2 (modern libcamera)
v4l2-ctl --list-devices
v4l2-ctl -d /dev/video0 --all
v4l2-ctl -d /dev/video0 --list-formats-ext

libcamera-hello                          # Kamera önizleme
libcamera-still -o foto.jpg              # Fotoğraf çek
libcamera-vid -t 10000 -o video.h264    # 10 saniye video
libcamera-jpeg -o hızlı.jpg             # Hızlı JPEG

# Çözünürlük ve format
libcamera-still --width 1920 --height 1080 -o foto_fhd.jpg

gst-launch-1.0 libcamerasrc ! video/x-raw,width=1280,height=720 ! \
    videoconvert ! autovideosink

# MJPEG stream
v4l2-ctl --stream-mmap --stream-count=100 -d /dev/video0
v4l2-ctl --set-fmt-video=width=1920,height=1080,pixelformat=MJPG --stream-mmap --stream-count=10 -d /dev/video0
```

## Genel Komutlar

```bash title="I2C & SPI"
sudo apt install i2c-tools spi-tools

i2cdetect -y 1                     # I2C-1 bus'taki cihazları tara
i2cget -y 1 0x68 0x3B              # 0x68 adresli cihazdan 0x3B register oku
i2cset -y 1 0x68 0x6B 0x00         # Register yaz
spi-config -d /dev/spidev0.0 -q    # SPI yapılandırmasını göster
```

```bash title="Swap Alanı"
# Swap dosyası oluştur (SD kart ömrü için /var/swap tercih et)
sudo fallocate -l 2G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile

# Kalıcı yapmak için /etc/fstab
echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab

# Swap kullanım önceliği (0=mümkün olduğunca az)
echo 'vm.swappiness=10' | sudo tee -a /etc/sysctl.conf
sudo sysctl -p
```

```bash title="SD Kart Ömrünü Uzatma"
# /tmp'yi RAM'e al (zaten tmpfs olabilir)
cat /proc/mounts | grep tmpfs

# log2ram: logları RAM'de tut, ara ara SD'ye yaz
sudo apt install log2ram
# /etc/log2ram.conf düzenle

# Gereksiz servis durdurma
sudo systemctl disable bluetooth avahi-daemon cups
```

```bash title="Güç Tüketimi Takibi"
# CPU sıcaklığı
vcgencmd measure_temp
cat /sys/class/thermal/thermal_zone0/temp   # 1000 ile böl = °C

# CPU frekansı
vcgencmd measure_clock arm
vcgencmd measure_clock core

# Voltaj
vcgencmd measure_volts core
vcgencmd measure_volts sdram_c

# Throttling durumu
vcgencmd get_throttled
# 0x0      = Normal
# 0x50005  = Under-voltage + Throttled
```

| Throttle Bit | Anlam                                     |
| :-: | ----------------------------------------- |
|  0  | Şu an düşük voltaj                        |
|  1  | Şu an ARM frekansı sınırlandı             |
|  2  | Şu an throttled                           |
|  16 | Başlangıçtan bu yana düşük voltaj yaşandı |
|  18 | Başlangıçtan bu yana throttling yaşandı   |

!!! danger "Güç Kaynağı Seçimi"
    Pi 4: 5V / 3A (15W). Pi 5: 5V / 5A (27W). Yetersiz güç kaynağı throttling ve veri bozulmasına yol açar. Resmi Raspberry Pi güç adaptörü kullanın.



```bash title="Yardımcı Komutlar"
# Sistem bilgisi
uname -a
cat /proc/cpuinfo | grep "Hardware"    # Pi modeli
cat /proc/cpuinfo | grep "Revision"    # Revizyon kodu
vcgencmd version                        # Firmware versiyonu

# Disk imaj alma (başka Linux'ta)
sudo dd if=/dev/sdb bs=4M status=progress | gzip -9 > rpi_backup.img.gz

# İmajı geri yükleme
gzip -dc rpi_backup.img.gz | sudo dd of=/dev/sdb bs=4M status=progress

# SD kart boyutuna göre imaj küçültme (pi-shrink aracı)
# github.com/Drewsif/PiShrink
sudo pishrink.sh rpi_backup.img
```

---

## udev Kuralları

```bash
# Kural dosyası
sudo nano /etc/udev/rules.d/99-mydevice.rules

# Örnek: USB-Serial cihaza sabit isim ver
SUBSYSTEM=="tty", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", \
    SYMLINK+="ttyArduino"

# Örnek: USB cihaz bağlandığında script çalıştır
SUBSYSTEM=="usb", ACTION=="add", \
    ATTRS{idVendor}=="0403", RUN+="/usr/local/bin/usb_connected.sh"

# Kuralları yenile
sudo udevadm control --reload-rules
sudo udevadm trigger

# Cihaz bilgisi
udevadm info --query=all --name=/dev/ttyUSB0
```

---

## Systemd Servis - Uygulama Otomasyonu

Pi'de çalışan uygulamayı önyüklemede otomatik başlatmak ve izlemek için systemd kullanılır.

```ini title="/etc/systemd/system/myapp.service"
[Unit]
Description=My Embedded Application
After=network.target
Wants=network.target

[Service]
Type=simple
User=pi
WorkingDirectory=/home/pi/myapp
ExecStart=/home/pi/myapp/myapp --config /etc/myapp.conf
Restart=on-failure
RestartSec=5
StandardOutput=journal
StandardError=journal

# Kaynak sınırları (isteğe bağlı)
MemoryMax=128M
CPUQuota=50%

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable myapp.service    # Önyüklemede başlat
sudo systemctl start  myapp.service    # Hemen başlat
sudo systemctl status myapp.service    # Durum kontrol
journalctl -u myapp.service -f         # Canlı log
```

---

## Cross-Compilation (Çapraz Derleme)

Geliştirme bilgisayarında (x86_64) Pi için ARM kodu derlemek, Pi üzerinde native derlemeye kıyasla çok daha hızlıdır.

```bash
# ARM64 cross-compiler kurulumu (Ubuntu/Debian)
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# Basit C programı cross-compile
aarch64-linux-gnu-gcc -o hello_arm hello.c
file hello_arm   # ELF 64-bit LSB executable, ARM aarch64

# CMake ile cross-compile
cat > toolchain-aarch64.cmake << 'EOF'
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF

mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-aarch64.cmake
make -j$(nproc)

# Pi'ye kopyala ve çalıştır
scp hello_arm pi@raspberrypi.local:~/
ssh pi@raspberrypi.local ./hello_arm
```

### QEMU ile Pi Öykünme

```bash
# QEMU ARM64 kurulumu
sudo apt install qemu-user-static

# Derlenmiş binary'yi host'ta çalıştır (QEMU sayesinde)
qemu-aarch64-static ./hello_arm

# Debian ARM64 chroot ortamı
sudo debootstrap --arch=arm64 bookworm /opt/arm64-sysroot
sudo chroot /opt/arm64-sysroot /bin/bash
```
