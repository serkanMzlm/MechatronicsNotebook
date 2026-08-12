# Terminal Komutları

## Dosya ve Dizin İşlemleri

| Komut      | Açıklama                                           |
| ---------- | -------------------------------------------------- |
| `pwd`      | Mevcut dizin yolunu gösterir                       |
| `cd`       | Dizin değiştirir                                   |
| `ls`       | Dizin içeriğini listeler                           |
| `mkdir`    | Dizin oluşturur                                    |
| `rmdir`    | Boş dizin siler                                    |
| `touch`    | Boş dosya oluşturur veya tarih günceller           |
| `cp`       | Dosya/dizin kopyalar                               |
| `mv`       | Dosya taşır veya yeniden adlandırır                |
| `rm`       | Dosya/dizin siler                                  |
| `ln`       | Link oluşturur (-s: sembolik, -i: hard)            |
| `file`     | Dosyanın gerçek türünü içerik analizi ile belirler |
| `stat`     | Ayrıntılı dosya meta bilgileri                     |
| `tree`     | Dizin yapısını ağaç şeklinde gösterir              |
| `readlink` | Sembolik linkin gerçek hedefini gösterir           |
| `install`  | Kopyalama + izin + sahip atama birleşimi           |

```bash
cd ~  # "~" -> Home, "-" -> Önceki, ".." -> Bir üst dizini

ls -l -a -h -t  # Detaylı listele, gizli dahil, anlaşılabilir birim, zamana göre sırala
ls -l -S        # Boyuta göre sırala (direkt -lS diye yazılabilir)
ls -R           # Alt dizinleri de listele

mkdir -p a/b/c    # İç içe dizin oluşturur

rm -rf dizin/     # Zorla sil
rm -i dosya.txt   # Silmeden önce onay sor
rm -rf !(a.txt)   # a.txt dışında her şeyi sil

cp -r src/ dst/   # Dizini özyinelemeli kopyala
cp -p dosya dst   # İzin ve tarihleri koru
cp -l dosya link  # Hard link olarak kopyala

ln -s /hedef /link_adı    # Sembolik link
ln kaynak link             # Hard link

tree -d            # Sadece dizinleri göster
tree -h            # Boyutları göster
tree -I ".git"     # Belirli kalıbı hariç tut

install -m 755 app /usr/local/bin/    # İzinli kopyala
install -d /etc/myapp                  # Dizin oluştur
```

## Dosya Görüntüleme

| Komut  | Açıklama                                        |
| ------ | ----------------------------------------------- |
| `cat`  | Dosya içeriğini ekrana basar                    |
| `less` | Sayfalı, aranabilir görüntüleme                 |
| `more` | `less`'in eski ve sınırlı hali                  |
| `head` | Dosyanın başını gösterir (varsayılan: 10 satır) |
| `tail` | Dosyanın sonunu gösterir                        |
| `tac`  | Satırları ters sırada gösterir                  |
| `wc`   | Satır, kelime, byte sayar                       |

```bash
cat -n dosya.txt           # Satır numarası ile
cat -A dosya.txt           # Görünmez karakterleri göster

# less içinde
# g → Başa git | G → Sona git | / → Ara | n → Sonraki eşleşme | q → Çık

head -n 20 dosya.txt       # İlk 20 satır
head -c 100 dosya.txt      # İlk 100 byte
tail -n 20 dosya.txt       # Son 20 satır
tail -f /var/log/syslog    # Canlı log takibi
tail -f -n 100 uygulama.log # Son 100 satır + canlı

wc    dosya.txt            # Satır kelime byte
wc -l dosya.txt            # Satır sayısı
wc -w dosya.txt            # Kelime sayısı
wc -c dosya.txt            # Byte sayısı
```


## Arama Komutları

| Komut     | Açıklama                                          |
| --------- | ------------------------------------------------- |
| `find`    | Gerçek zamanlı dosya arama                        |
| `locate`  | İndeks tabanlı hızlı arama (gerçek zamanlı değil) |
| `which`   | PATH üzerindeki executable konumu                 |
| `whereis` | Binary, source, manual konumları                  |
| `grep`    | Metin içinde desen arama                          |

```bash
# find
find . -name "*.py"                    # İsme göre ara
find . -type f -size +10M             # 10 MB'dan büyük dosyalar
find . -type f -perm 0777             # Belirli izinli dosyalar
find . -mtime -3                       # Son 3 günde değişenler
find . -user serkan                    # Belirli sahibin dosyaları
find . -type d -name "node_modules" -exec rm -rf {} + # Sil

# locate
locate -i network                      # Büyük/küçük harf duyarsız
sudo updatedb                          # İndeksi güncelle

# which / whereis
which python3
which -a gcc                           # Tüm eşleşmeler
whereis ls                             # Binary + manual
whereis -b gcc                         # Sadece binary

# grep
grep "hata" dosya.log
grep -i "error" dosya.log             # Büyük/küçük harf duyarsız
grep -n "failed" dosya.log            # Satır numarasıyla
grep -r "TODO" ./src                  # Özyinelemeli
grep -E "ERROR|WARN|FATAL" app.log    # ERE
grep -v "^#" /etc/ssh/sshd_config    # Yorum satırlarını atla
grep -o "[0-9]\+\.[0-9]\+\.[0-9]\+\.[0-9]\+" dosya  # Sadece eşleşen kısmı
```


## Metin İşleme

| Komut  | Açıklama                                          |
| ------ | ------------------------------------------------- |
| `sed`  | Akış düzenleyici; satır içi değiştirme            |
| `awk`  | Alan bazlı metin işleme programlama dili          |
| `cut`  | Belirli sütunları veya karakterleri ayıklar       |
| `sort` | Satırları sıralar                                 |
| `uniq` | Birbirini izleyen tekrar eden satırları filtreler |
| `tr`   | Karakter dönüşümleri                              |
| `diff` | Dosya farklılıklarını satır bazında gösterir      |
| `cmp`  | Dosyaları byte bazında karşılaştırır              |
| `tee`  | Çıktıyı hem terminale hem dosyaya yazar           |

```bash
# sed
sed 's/eski/yeni/g' dosya.txt             # Değiştir (stdout)
sed -i 's/eski/yeni/g' dosya.txt          # In-place
sed -n '/ERROR/p' dosya.txt               # Sadece eşleşen satırlar
sed '/^$/d' dosya.txt                      # Boş satırları sil
sed -n '10,20p' dosya.txt                  # 10-20. satırlar

# awk
awk '{print $1}' dosya.txt                # İlk alan
awk '{print $1, $3}' dosya.txt            # 1. ve 3. alan
awk '{print $NF}' dosya.txt               # Son alan
awk -F':' '{print $1}' /etc/passwd        # Ayırıcı değiştir
awk '/ERROR/ {print NR": "$0}' dosya.txt  # Satır no ile yazdır
awk '{sum+=$2} END{print "Toplam:", sum}' # Toplama
awk 'NR==5' dosya.txt                     # 5. satır

# cut
cut -d',' -f2 data.csv                    # CSV 2. sütun
cut -c1-20 dosya.txt                      # İlk 20 karakter

# sort
sort dosya.txt                             # Alfabetik
sort -r dosya.txt                          # Ters
sort -n sayilar.txt                        # Sayısal
sort -k2 -n dosya.txt                     # 2. alana göre sayısal
sort -rh                                   # İnsan okunabilir boyut (du çıktısı)

# uniq
sort dosya.txt | uniq                     # Tekrarları kaldır
sort dosya.txt | uniq -c                  # Kaç kez tekrar ettiği
sort dosya.txt | uniq -d                  # Sadece tekraları göster
sort dosya.txt | uniq -u                  # Sadece benzersizleri

# tr
cat dosya.txt | tr 'a-z' 'A-Z'           # Büyük harfe çevir
cat dosya.txt | tr -d '\r'               # Windows satır sonu sil
cat dosya.txt | tr -s ' '               # Çoklu boşluğu tekleştir
```


## Arşiv ve Sıkıştırma

| Komut             | Açıklama                                       |
| ----------------- | ---------------------------------------------- |
| `tar`             | Arşivleme aracı (`.tar`, `.tar.gz`, `.tar.xz`) |
| `gzip`/`gunzip`   | `.gz` sıkıştırma                               |
| `bzip2`/`bunzip2` | `.bz2` sıkıştırma                              |
| `xz`/`unxz`       | `.xz` yüksek sıkıştırma                        |
| `zip`/`unzip`     | `.zip` arşivi                                  |
| `zcat`            | `.gz` dosyasını açmadan görüntüle              |

```bash
# tar
tar -cvf arsiv.tar dizin/          # Arşiv oluştur
tar -czvf arsiv.tar.gz dizin/      # gzip ile sıkıştır
tar -cJvf arsiv.tar.xz dizin/      # xz ile sıkıştır
tar -xvf arsiv.tar                  # Arşiv aç
tar -xzvf arsiv.tar.gz             # gzip'li arşiv aç
tar -xvf arsiv.tar -C /hedef/      # Belirli dizine aç
tar -tvf arsiv.tar                  # İçeriği listele
tar --exclude=".git" -czf proje.tar.gz proje/  # Dışla

# Hızlı kopya (sıkıştırmadan)
tar -cf - src/ | tar -xf - -C dst/

# gzip
gzip -k dosya.txt          # Orijinali koru
gzip -9 büyük.log          # Maksimum sıkıştırma
gzip -l arsiv.gz           # Bilgi göster
zcat arsiv.log.gz | grep error  # Açmadan içinde ara
```


## Process ve Sistem Yönetimi

| Komut     | Açıklama                                |
| --------- | --------------------------------------- |
| `ps`      | Çalışan process listesi                 |
| `top`     | Canlı sistem kaynak monitörü            |
| `htop`    | Gelişmiş interaktif process monitörü    |
| `kill`    | Process'e sinyal gönderir               |
| `pkill`   | İsimle process öldürür                  |
| `killall` | İsimle tüm eşleşen process'leri öldürür |
| `nice`    | Düşük öncelikle başlatır                |
| `renice`  | Çalışan process önceliğini değiştirir   |
| `nohup`   | Terminale bağımsız çalıştırır           |
| `bg`      | Arka planda devam ettirir               |
| `fg`      | Ön plana alır                           |
| `jobs`    | Arka plan işlerini listeler             |
| `timeout` | Komut süresini sınırlar                 |

```bash
# ps
ps aux                              # Tüm process'ler (a=tüm kullanıcı, u=detay, x=tty'siz)
ps -ef                              # Full format
ps -u serkan                        # Belirli kullanıcı
ps --sort=-%cpu | head              # CPU kullanımına göre sırala
pstree                              # Ağaç görünümü

# STAT sütunu anlamları:
# R=Çalışıyor, S=Uyuyor, D=I/O bekliyor, Z=Zombi, T=Durmuş

# kill sinyalleri
kill -l                             # Sinyal listesi
kill -9  <PID>                     # SIGKILL (zorla)
kill -15 <PID>                     # SIGTERM (nazikçe)
kill -1  <PID>                     # SIGHUP (yeniden yükle)
pkill -f "uygulama_adi"            # Kalıba göre öldür

# top etkileşim
# k → kill | r → renice | M → Belleğe göre sırala | P → CPU'ya göre

# nohup
nohup ./betik.sh &
nohup ./betik.sh > cikti.log 2>&1 &

# nice (-20 en yüksek, +19 en düşük öncelik)
nice -n 10 python3 agir_is.py
renice +5 -p <PID>

# Zamanlanmış sonlandırma
timeout 30s ping google.com
timeout 5m ./uzun_betik.sh || echo "Zaman aşıldı!"
```


## Disk ve Dosya Sistemi

| Komut              | Açıklama                                  |
| ------------------ | ----------------------------------------- |
| `df`               | Bağlı dosya sistemleri disk kullanımı     |
| `du`               | Dizin/dosya disk kullanımı                |
| `lsblk`            | Blok cihazların ağaç görünümü             |
| `blkid`            | Blok cihaz UUID ve tip                    |
| `mount` / `umount` | Dosya sistemi bağla / ayır                |
| `findmnt`          | Mount noktalarını ağaç yapısında gösterir |
| `fdisk`            | Disk bölüm yönetimi                       |
| `mkfs`             | Dosya sistemi oluşturur                   |
| `fsck`             | Dosya sistemi kontrolü ve onarımı         |
| `dd`               | Düşük seviye blok kopyalama               |

```bash
# df
df -h                         # İnsan okunabilir
df -hT                        # Dosya sistemi tipini de göster
df -h /                       # Sadece root

# du
du -sh /var/log               # Tek dizin özeti
du -sh /var/* | sort -rh | head -10  # En büyük alt dizinler
du -h --max-depth=1 /home     # Sadece 1 seviye derine git

# lsblk
lsblk                         # Disk yapısı
lsblk -f                      # Dosya sistemi tipleri ve UUID

# mount
sudo mount /dev/sdb1 /mnt/disk
sudo mount -t ext4 /dev/sdb1 /mnt/disk
sudo mount -o ro /dev/sdb1 /mnt/disk  # Salt okunur
sudo umount /mnt/disk

# dd - Disk imajı
sudo dd if=/dev/sda of=/mnt/backup.img bs=64K conv=noerror,sync status=progress
sudo dd if=/mnt/backup.img of=/dev/sdb bs=64K status=progress
# Doğrula:
sha256sum /dev/sda /mnt/backup.img
```

!!! danger "dd Komutu"
    `dd` kaynak (`if=`) ve hedefi (`of=`) doğrudan silmeden yazar. Hedefi yanlış belirlemek veri kaybına neden olur. Çalıştırmadan önce `if=` ve `of=` değerlerini **iki kez kontrol edin**.


## Kullanıcı ve Yetki Yönetimi

```bash
# Kullanıcı yönetimi
sudo useradd -m -s /bin/bash -G sudo serkan   # Kullanıcı oluştur
sudo passwd serkan                              # Şifre ata
sudo usermod -aG docker,gpio serkan           # Gruplara ekle
sudo usermod -L serkan                         # Kilitle
sudo usermod -U serkan                         # Kilidi aç
sudo userdel -r serkan                         # Home ile birlikte sil

# Grup yönetimi
sudo groupadd arge
sudo groupdel arge
groups                          # Mevcut kullanıcının grupları
id serkan                       # UID, GID ve gruplar

# Giriş bilgisi
who                             # Sisteme giriş yapmış kullanıcılar
w                               # Detaylı
last                            # Tüm girişler
lastb                           # Başarısız girişler

# Kullanıcı geçişi
su serkan                       # Kullanıcıya geç
sudo su                         # Root'a geç
sudo -i                         # Root shell
sudo -s                         # Mevcut shell'de root

# ACL
getfacl dosya.txt
setfacl -m u:serkan:rwx dosya.txt
setfacl -m g:arge:rx dosya.txt
setfacl -x u:serkan dosya.txt   # Kaldır
setfacl -b dosya.txt            # Tümünü kaldır
```


## Sistem Bilgisi

```bash
# Donanım bilgisi
uname -a               # Kernel, hostname, mimari
uname -r               # Kernel sürümü
uname -m               # Mimari (x86_64, aarch64)
lscpu                  # İşlemci detayı
free -h                # RAM kullanımı
lspci                  # PCIe cihazlar
lspci -k               # Hangi sürücüyü kullandığı
lsusb                  # USB cihazlar
lsusb -v               # Detaylı
lsusb -t               # Hiyerarşi
lsmod                  # Yüklü kernel modülleri
modinfo <modül>        # Modül bilgisi

# Sistem bilgisi
cat /proc/cpuinfo      # CPU detayı
cat /proc/meminfo      # Bellek detayı
cat /etc/os-release    # Dağıtım bilgisi
lsb_release -a         # Dağıtım sürümü
hostname               # Sistem adı
hostname -I            # IP adresleri
uptime                 # Çalışma süresi ve yük

# Tarih ve saat
date
timedatectl            # Zaman dilimi ve NTP durumu
timedatectl set-timezone Europe/Istanbul
```


## Zaman ve Otomasyon

```bash
# date formatları
date +"%Y-%m-%d %H:%M:%S"   # 2024-01-15 14:30:00
date +%s                     # Unix timestamp (epoch)
date +%Y%m%d                 # 20240115

# Sistem zamanı ayarla
sudo timedatectl set-time '2024-01-15 14:30:00'
sudo timedatectl set-ntp true   # NTP senkronizasyonu

# watch - periyodik komut tekrar
watch -n 2 df -h         # 2 saniyede bir
watch -n 1 'ps aux --sort=-%cpu | head'

# sleep
sleep 5                  # 5 saniye bekle
sleep 1m                 # 1 dakika
sleep 1h30m              # 1 saat 30 dakika

# at - tek seferlik zamanlama
at now + 30 minutes << 'EOF'
/usr/local/bin/yedekle.sh
EOF
atq                      # Kuyruğu gör
atrm <iş_no>            # İptal et

# Zaman ölçümü
time komut               # Gerçek, user, sys süresi
```


## Ortam Değişkenleri

```bash
env                      # Tüm ortam değişkenleri
printenv PATH            # Belirli değişken
echo $HOME $USER $SHELL  # Sistem değişkenleri

# Geçici tanımlama
export MY_VAR="değer"
MY_VAR="değer" komut     # Sadece o komut için

# Kalıcı (.bashrc veya .profile)
echo 'export MY_VAR="değer"' >> ~/.bashrc
source ~/.bashrc

# Alias
alias ll='ls -alh'
alias gs='git status'
alias update='sudo apt update && sudo apt upgrade -y'
unalias ll               # Kaldır
alias                    # Tüm alias'ları listele

# history
history
history | grep apt       # Filtrelenmiş geçmiş
!!                       # Önceki komut
!124                     # Geçmişte 124. komut
!apt                     # apt ile başlayan son komut
Ctrl+R                   # Ters arama
```


## Mesajlaşma ve Bildirim

```bash
wall "Sistem 10 dakika sonra bakıma alınacak"    # Tüm kullanıcılara
write serkan pts/1                                # Belirli terminale

# notify-send (masaüstü bildirimi)
notify-send "Yedekleme" "Tamamlandı!" --icon=dialog-information
```


## Sistem Debug Araçları

### Sistem Çağrısı İzleme `strace`

Bir process'in kernel'e yaptığı tüm sistem çağrılarını (open, read, write, ioctl, mmap...) gösterir. Gömülü sistemlerde sürücü debug'lamanın temel aracıdır.

```bash
# Temel kullanım
strace ls /tmp                          # Komutun syscall'larını göster
strace -p 1234                          # Çalışan process'e bağlan

# Sadece belirli syscall'lar
strace -e trace=open,read,write ls      # Dosya işlemleri
strace -e trace=network curl http://x  # Ağ çağrıları
strace -e trace=ioctl ./myapp          # ioctl çağrıları (cihaz debug)
strace -e trace=mmap,brk ./myapp       # Bellek tahsisi

# Detay ve zaman
strace -t ./app                         # Zaman damgası
strace -T ./app                         # Her syscall süresi
strace -c ./app                         # Özet istatistik (hangi çağrı kaç kez)

# Dosyaya yaz
strace -o trace.txt ./app
strace -ff -o trace ./app              # Fork'ları ayrı dosyaya

# Pratik: hangi dosyalara erişiyor?
strace -e trace=openat ./app 2>&1 | grep -v ENOENT

# Pratik: cihaz dosyası kullanımını izle
strace -e trace=open,ioctl,read,write ./sensor_app 2>&1 | grep /dev/

# Pratik: başarısız sistem çağrıları
strace -e trace=all ./app 2>&1 | grep " = -1"
```

### Kütüphane Çağrısı İzleme `ltrace`

```bash
ltrace ./app              # Dinamik kütüphane çağrıları
ltrace -S ./app           # Syscall + kütüphane çağrıları
ltrace -p 1234            # Çalışan process
ltrace -c ./app           # Özet istatistik
```

### Binary Analiz Araçları

```bash
# ldd - dinamik bağımlılıklar
ldd /usr/bin/python3               # Hangi .so dosyalarını kullanıyor
ldd -v /bin/ls                     # Detaylı versiyon bilgisi
LD_TRACE_LOADED_OBJECTS=1 ./app   # ldd'nin yaptığı şey

# readelf - ELF dosyası analizi
readelf -h /bin/ls                 # ELF başlığı (mimari, tip)
readelf -S /bin/ls                 # Section listesi
readelf -d ./app                   # Dinamik bölüm (NEEDED = bağımlılıklar)
readelf -l /bin/ls                 # Program header (segment'ler)
readelf --syms ./app               # Sembol tablosu

# nm - sembol tablosu
nm ./app                           # Tüm semboller
nm -D ./app                        # Dinamik semboller
nm -u ./app                        # Tanımsız (dış bağımlılık) semboller
nm --defined-only ./libmylib.so    # Sadece tanımlı semboller

# objdump - disassemble
objdump -d ./app                   # Text section'ı decompile et
objdump -S ./app                   # Kaynak+assembly (debug bilgisi varsa)
objdump -j .data -s ./app         # .data section içeriği

# file - dosya türü
file /bin/ls                       # ELF 64-bit LSB executable, ARM aarch64...
file ./firmware.bin                # Firmware analizi

# strings - ikili içindeki ASCII dizeler
strings ./app | grep -i "error\|config\|version"
strings -n 8 firmware.bin         # En az 8 karakterlik dizeler
```

### Performans Analizi `perf`

```bash
# Temel profil
sudo perf stat ./app                    # CPU istatistikleri (cycles, instructions vb.)
sudo perf stat -e cache-misses ./app   # Cache miss sayısı
sudo perf top                           # Gerçek zamanlı CPU profili (top gibi)

# Örnekleme profili
sudo perf record ./app                  # Örnekleme yap (perf.data dosyası oluşur)
sudo perf record -g ./app              # Stack trace ile
sudo perf report                        # perf.data'yı analiz et
sudo perf report --stdio               # Terminal çıktısı

# Callgraph
sudo perf record -g --call-graph dwarf ./app
sudo perf report --call-graph

# Sistem geneli izleme
sudo perf top -e cycles -p 1234       # Belirli process

# Flamegraph (görsel profil - FlameGraph aracı gerekli)
sudo perf record -g ./app
sudo perf script | ./stackcollapse-perf.pl | ./flamegraph.pl > flame.svg
```

!!! tip "Gömülü Sistemlerde Debug Öncelikleri"
    1. **Çalışmıyor mu?** → `strace -e trace=openat,ioctl` ile hangi dosyaya/cihaza erişemeduğunu bul
    2. **Yavaş mı?** → `strace -c` ile en çok zaman harcayan syscall'ı bul, sonra `perf` ile darboğazı bul
    3. **Crash oluyor mu?** → `gdb ./app core` ile core dump analiz et
    4. **Kütüphane hatası mı?** → `ldd` + `ltrace` kombinasyonu