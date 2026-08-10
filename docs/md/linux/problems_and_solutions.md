# Sorunlar ve Çözümler

!!! note "Genel Bakış"
    Gerçek projelerden ve günlük Linux kullanımından derlenen yaygın sorunların çözümleri. Her sorun kısa bir bağlam açıklaması ve doğrudan çözüm adımlarıyla sunulmuştur.

---

## Donanım ve Aygıt Sorunları

### Arduino / USB-Serial Port Görünmüyor

**Belirti:** Arduino IDE'de veya `ls /dev/tty*` ile port görünmüyor.

```bash
# Kullanıcıyı dialout ve tty grubuna ekle
sudo usermod -a -G dialout $USER
sudo usermod -a -G tty $USER
# Çıkış yapıp tekrar giriş gerekli

# Ubuntu 22.04+: brltty çakışması
# brltty, CH340/PL2303 chip'i braille cihaz olarak algılıyor
sudo systemctl stop brltty
sudo systemctl disable brltty

# Alternatif: udev kuralında ilgili satırı devre dışı bırak
sudo nano /usr/lib/udev/rules.d/85-brltty.rules
# Aşağıdaki satırı yorum yap (# ekle):
# ENV{PRODUCT}=="1a86/7523/*", ENV{BRLTTY_BRAILLE_DRIVER}="bm", GOTO="brltty_usb_run"

sudo udevadm control --reload-rules
```

### USB Sürücü / NTFS Hata Veriyor

**Belirti:** NTFS formatlı USB dosya aktarırken "Input/output error" veya "Transport endpoint is not connected".

```bash
# USB'yi güvenli çıkart ve yeniden tak
sudo umount /dev/sdb1
sudo ntfsfix /dev/sdb1       # NTFS metadata onar
sudo mount /dev/sdb1 /mnt/usb

# Tamamen bozuksa Windows'ta chkdsk koşturmak daha etkili
# Veya
sudo fsck.ntfs /dev/sdb1
```

### GPIO / I2C / SPI Aygıtı Görünmüyor

```bash
# Kernel modülünün yüklü olduğunu doğrula
lsmod | grep i2c
sudo modprobe i2c-dev

# /dev/i2c-* yoksa
ls /dev/i2c*
sudo dtparam=i2c_arm=on   # Raspberry Pi'de

# I2C cihaz tarama
sudo i2cdetect -y 1

# Yetki sorunu
sudo usermod -aG i2c $USER
```

---

## Ağ Sorunları

### Wi-Fi Bağlantısı Kurulamıyor

```bash
# Wi-Fi adaptörünü kontrol et
ip link show
nmcli radio wifi        # Wi-Fi hardware durumu
nmcli radio wifi on     # Kapalıysa aç

# NetworkManager logları
journalctl -u NetworkManager -f

# RF kill kontrolü
rfkill list             # Blocked: yes ise
rfkill unblock wifi

# Sürücü yeniden yükleme
sudo modprobe -r ath9k && sudo modprobe ath9k
```

### Sabit IP Sonrası İnternet Yok

```bash
# DNS sunucusu eksik olabilir
cat /etc/resolv.conf
# nameserver yoksa nmcli ile ekle
nmcli con mod "bağlantı_adı" ipv4.dns "8.8.8.8 1.1.1.1"
nmcli con up "bağlantı_adı"

# Gateway eksik olabilir
ip route              # default gw yoksa
ip route add default via 192.168.1.1
```

### SSH Bağlantısı Reddedildi

```bash
# Servis çalışıyor mu?
systemctl status sshd

# Port dinleniyor mu?
ss -tlnp | grep 22

# Güvenlik duvarı?
sudo ufw status
sudo ufw allow 22/tcp

# Çok fazla başarısız giriş (fail2ban)?
sudo fail2ban-client status sshd
sudo fail2ban-client set sshd unbanip 192.168.1.50

# sshd yapılandırma hatası?
sudo sshd -t              # Sözdizimi kontrolü
```

### SSH Bağlantısı Çok Yavaş

```bash
# DNS çözümleme yavaşlatıyor
# /etc/ssh/sshd_config içinde:
UseDNS no

# GSSAPI gereksizse
GSSAPIAuthentication no

sudo systemctl restart sshd
```

---

## Sistem ve Yazılım Sorunları

### Sudo Şifre İstemeyi Kaldır

```bash
sudo visudo     # Sudoers dosyasını güvenli açar

# Belirli kullanıcı için şifresiz sudo
%sudo   ALL=(ALL:ALL) NOPASSWD:ALL     # Tüm sudo grubu
serkan  ALL=(ALL:ALL) NOPASSWD:ALL     # Sadece serkan

# Belirli komutlar için
serkan  ALL=(ALL) NOPASSWD: /usr/bin/apt, /sbin/reboot
```

!!! danger "NOPASSWD Riski"
    `NOPASSWD:ALL` yalnızca güvenilir, izole geliştirme ortamlarında kullanın. Üretim sistemlerinde güvenlik açığıdır.

### Wi-Fi Şifrelerini Görüntüle

```bash
# NetworkManager şifreleri
sudo grep -r psk= /etc/NetworkManager/system-connections/

# Belirli bağlantı
sudo cat /etc/NetworkManager/system-connections/"WIFI_ADI"
```

### Terminalden Uygulama Çalıştırma (PATH Sorunu)

```bash
# Uygulama terminalde bulunmuyorsa symlink kur
sudo ln -s $(readlink -f ./qtcreator) /usr/local/bin/qtcreator
sudo ln -s /opt/myapp/bin/myapp /usr/local/bin/myapp

# Veya PATH'e ekle (.bashrc / .profile)
echo 'export PATH="$PATH:/opt/myapp/bin"' >> ~/.bashrc
source ~/.bashrc
```

### Paket Kurulumu Yarım Kaldı

```bash
# Bozuk bağımlılıkları düzelt
sudo dpkg --configure -a
sudo apt install -f
sudo apt clean && sudo apt update

# "dpkg: error: another process has the lock file"
# Sahte kilit dosyasını sil
sudo rm /var/lib/dpkg/lock
sudo rm /var/lib/dpkg/lock-frontend
sudo rm /var/cache/apt/archives/lock
sudo dpkg --configure -a
```

---

## Disk ve Dosya Sistemi Sorunları

### Disk Alanı Doldu

```bash
# Nerede yer kaplanıyor?
df -h                              # Genel disk kullanımı
du -sh /var/* | sort -rh | head   # En büyük dizinler
du -sh /var/log/* | sort -rh | head -10

# Log temizleme
sudo journalctl --vacuum-size=200M
sudo apt clean
sudo apt autoremove --purge

# Docker temizleme
docker system prune -a --volumes

# Büyük dosyalar bul
find / -size +500M -type f 2>/dev/null | sort -k5 -rn
```

### Dosya Sistemi Salt Okunur Hale Geldi

```bash
# Genellikle disk hatası veya güç kesilmesi sonrası oluşur
# /proc/mounts içinde "ro" ile gösterilir

# Dosya sistemi kontrolü (önce unmount et)
sudo umount /dev/sda1
sudo fsck -y /dev/sda1

# Raspberry Pi: tmpfs logları ve read-only rootfs
# /etc/fstab'da ro olan bölümü rw olarak yeniden mount et
sudo mount -o remount,rw /
```

### Silinmiş Dosyayı Geri Getirme

```bash
# extundelete (ext3/ext4)
sudo apt install extundelete
sudo extundelete /dev/sda1 --restore-all

# testdisk — partition ve dosya kurtarma
sudo apt install testdisk
sudo testdisk /dev/sda

# photorec — medya dosyası kurtarma
sudo photorec /dev/sda
```

---

## Process ve Performans Sorunları

### Zombi Process Temizleme

```bash
# Zombi'leri listele
ps aux | awk '$8 == "Z"'

# Ebeveyn process'i bul ve öldür
pstree -p | grep zombie_PID
kill -9 <EBEVEYN_PID>

# Zombiler genellikle ebeveyn öldürülünce init (PID 1) tarafından temizlenir
```

### Yüksek CPU Kullanan Process Bul

```bash
# Anlık görünüm
ps aux --sort=-%cpu | head -10
top -b -n 1 -o %CPU | head -20

# Süreçleri izle
htop

# 30 saniye CPU profili
pidstat -u 1 30

# Belirli process'in system call'larını takip et
strace -p <PID> -c      # Özet
strace -p <PID>         # Detaylı
```

### "Too Many Open Files" Hatası

```bash
# Mevcut limit
ulimit -n

# Geçici artırma
ulimit -n 65536

# Kalıcı — /etc/security/limits.conf
echo "* soft nofile 65536" | sudo tee -a /etc/security/limits.conf
echo "* hard nofile 65536" | sudo tee -a /etc/security/limits.conf

# systemd servisi için
# [Service] bölümüne:
# LimitNOFILE=65536
sudo systemctl daemon-reload
```

---

## Kullanıcı ve Yetki Sorunları

### "Permission Denied" Hataları

```bash
# Dosya sahibi ve izinlerini kontrol et
ls -la dosya.txt
stat dosya.txt

# Execution izni eksikse
chmod +x script.sh

# Sahiplik düzelt
sudo chown $USER:$USER dosya.txt

# /dev dosyası için grup üyeliği
ls -la /dev/ttyUSB0      # crw-rw---- 1 root dialout
groups                    # dialout grubunda mısın?
sudo usermod -aG dialout $USER

# SELinux/AppArmor (Ubuntu 22.04+)
sudo aa-status
sudo aa-complain /etc/apparmor.d/profil_adi  # İzin ver
```

### Kullanıcı Kilitlendi

```bash
# Kilitli hesapları listele
sudo passwd -S serkan
# Durum: L = Locked, P = Password set, NP = No password

# Kilidi aç
sudo passwd -u serkan

# Sıfırla
sudo passwd serkan

# Giriş denemesi sayacını sıfırla (pam_tally2)
sudo pam_tally2 --user=serkan --reset
# veya Ubuntu 20.04+
sudo faillock --user serkan --reset
```

---

## Geliştirme Araçları Sorunları

### Git SSL Sertifika Hatası

```bash
git clone https://... → SSL certificate problem

# Geçici devre dışı bırak (güvenli değil, sadece test)
git -c http.sslVerify=false clone https://...

# Kalıcı devre dışı bırakma (UYARI)
git config --global http.sslVerify false

# Doğru çözüm: sertifika güncelle
sudo apt install ca-certificates
sudo update-ca-certificates
```

### Python Modülü Bulunamıyor

```bash
# Hangi Python kullanılıyor?
which python3
python3 --version

# Pip hangi Python'a yüklüyor?
pip3 show numpy | grep Location

# Sanal ortam kontrolü
echo $VIRTUAL_ENV    # Aktif bir venv var mı?

# Eğer conda kullanıyorsa
conda activate myenv
conda list | grep numpy
```

### Docker İzin Hatası

```bash
# "permission denied while trying to connect to the Docker daemon socket"
sudo usermod -aG docker $USER
newgrp docker           # Oturumu yenilemeden grup aktivasyonu
# veya
docker run hello-world  # sudo ile test
```

---

## Hızlı Başvuru Tablosu

| Sorun             | İlk Bakılacak Yer                                    |
| ----------------- | ---------------------------------------------------- |
| Servis başlamıyor | `journalctl -u servis_adı -n 50`                     |
| Port açılmıyor    | `ss -tlnp \| grep PORT` ve `ufw status`              |
| Disk doldu        | `df -h` ve `du -sh /*`                               |
| SSH bağlanamıyor  | `systemctl status sshd` ve firewall                  |
| Yüksek CPU        | `ps aux --sort=-%cpu \| head`                        |
| Yavaş sistem      | `top`, `iotop`, `vmstat 1`                           |
| Paket kurulamıyor | `apt install -f` ve `dpkg --configure -a`            |
| Cihaz görünmüyor  | `dmesg \| tail -20` ve `lsusb / lspci`               |
| İzin hatası       | `ls -la` ve `groups`                                 |
| DNS çalışmıyor    | `dig @8.8.8.8 example.com` ve `cat /etc/resolv.conf` |