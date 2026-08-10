# Shell Scripting

!!! note "Genel Bakış"
    Shell script, Linux komut satırı araçlarını bir dosyada birleştirerek otomasyon, yedekleme, yapılandırma ve dağıtım görevlerini gerçekleştirmenin temel yoludur. Bu bölüm Bash (Bourne Again Shell) temel alınarak yazılmıştır.

```bash
cat /etc/shells    # Sistemde yüklü shell'leri listeler
echo $SHELL        # Aktif shell
chsh -s /bin/zsh   # Varsayılan shell değiştir (oturum yenilenmeli)
```

---

## Shebang ve Script Yapısı

```bash
#!/bin/bash
# Bu dosya bir bash scriptidir.
# Çalıştırmak için: bash script.sh  veya  chmod +x script.sh && ./script.sh

set -e   # Hata olursa hemen dur
set -u   # Tanımsız değişken kullanılırsa hata ver
set -o pipefail  # Pipe içindeki hata propagate etsin

echo "Script başlıyor..."
```

!!! tip "Shebang Neden Önemli?"
    Shebang (`#!/bin/bash`) kernel'e hangi yorumlayıcının kullanılacağını söyler. `#!/usr/bin/env bash` kullanımı daha taşınabilirdir — `bash`'ı PATH üzerinden bulur.

---

## Değişkenler

```bash
#!/bin/bash

# Değişken atama — eşittir işaretinin yanında boşluk OLMAZ
isim="Serkan"
sayi=42
tarih=$(date +%Y-%m-%d)  # Komut çıktısı atama
hostname_val=`hostname`   # Eski yöntem (backtick); $() tercih edilmeli

# Kullanım
echo "İsim: $isim"
echo "Tarih: ${tarih}"    # Süslü parantez: değişken sınırını belirler

# Özel değişkenler
echo "$0"   # Script adı
echo "$1"   # 1. argüman
echo "$#"   # Argüman sayısı
echo "$@"   # Tüm argümanlar (ayrı ayrı)
echo "$*"   # Tüm argümanlar (tek string)
echo "$$"   # Mevcut process'in PID'i
echo "$?"   # Son komutun çıkış kodu (0=başarı)
echo "$!"   # Son arka plan process PID'i
```

### Salt Okunur ve Dışa Aktarma

```bash
readonly PI=3.14159     # Değiştirilemez
export PATH="$PATH:/opt/myapp/bin"  # Alt process'lere aktar

# Değişken silme
unset isim
```

### String İşlemleri

```bash
metin="Hello, Linux World!"

echo ${#metin}               # Uzunluk: 20
echo ${metin:7:5}            # Substring: Linux (offset:uzunluk)
echo ${metin/Linux/Bash}     # İlk eşleşmeyi değiştir
echo ${metin//o/0}           # Tüm eşleşmeleri değiştir
echo ${metin,,}              # Tümü küçük harf
echo ${metin^^}              # Tümü büyük harf
echo ${metin#Hello, }        # Baştan sil (kısa eşleşme)
echo ${metin%World!}         # Sondan sil (kısa eşleşme)

# Varsayılan değer
echo ${AYAR:-"varsayılan"}   # AYAR yoksa "varsayılan" döner
echo ${AYAR:="varsayılan"}   # Yoksa ata ve döner
```

---

## Kullanıcıdan Veri Alma

```bash
echo "Adınızı girin:"
read -r ad
read -r -p "Soyadınızı girin: " soyad
read -r -s -p "Şifre: " sifre    # -s = gizli giriş (echo yok)
echo ""

echo "Merhaba, $ad $soyad"

# Zaman aşımlı okuma
if read -r -t 10 -p "Devam et? (E/h): " cevap; then
    echo "Cevap: $cevap"
else
    echo "Zaman aşımı!"
fi
```

---

## Kontrol Akışı

### if — elif — else

```bash
#!/bin/bash
puan=85

if [ "$puan" -ge 90 ]; then
    echo "AA"
elif [ "$puan" -ge 80 ]; then
    echo "BA"
elif [ "$puan" -ge 70 ]; then
    echo "BB"
else
    echo "Başarısız"
fi

# Dosya/dizin kontrolü
if [ -f "/etc/hosts" ]; then
    echo "Dosya var"
fi

if [ -d "/tmp/work" ]; then
    echo "Dizin mevcut"
else
    mkdir -p /tmp/work
fi

# String karşılaştırma
ad="serkan"
if [ "$ad" = "serkan" ]; then echo "Eşit"; fi
if [[ "$ad" == ser* ]]; then echo "ser ile başlıyor"; fi  # [[ ]] glob destekler
```

### Karşılaştırma Operatörleri

| Operatör   | Sayısal |          String         |                   Dosya                    |
| ---------- | :-----: | :---------------------: | :----------------------------------------: |
| Eşit       |  `-eq`  |      `=` veya `==`      |                     —                      |
| Eşit değil |  `-ne`  |           `!=`          |                     —                      |
| Küçük      |  `-lt`  |            —            |                     —                      |
| Büyük      |  `-gt`  |            —            |                     —                      |
| Küçük eşit |  `-le`  |            —            |                     —                      |
| Büyük eşit |  `-ge`  |            —            |                     —                      |
| Var mı     |    —    | `-z` (boş), `-n` (dolu) | `-e` (dosya/dir), `-f` (dosya), `-d` (dir) |

### case

```bash
#!/bin/bash
read -r -p "Seçim (a/b/c): " secim

case "$secim" in
    a|A)
        ls -la
        ;;
    b|B)
        df -h
        ;;
    c|C)
        echo "Hello, World!"
        ;;
    [0-9])
        echo "Rakam girdiniz: $secim"
        ;;
    *)
        echo "Geçersiz seçim"
        exit 1
        ;;
esac
```

---

## Döngüler

### for

```bash
# Liste üzerinde
for isim in Ali Mert Serkan; do
    echo "Merhaba, $isim"
done

# Sayı aralığı
for i in {1..10}; do
    echo "$i"
done

# C stili (bash)
for ((i=0; i<5; i++)); do
    echo "i=$i"
done

# Glob ile dosyalar
for dosya in /var/log/*.log; do
    echo "Log: $dosya"
done

# Komut çıktısı üzerinde
for pid in $(pgrep nginx); do
    echo "Nginx PID: $pid"
done
```

### while

```bash
#!/bin/bash
sayac=0

while [ "$sayac" -lt 5 ]; do
    echo "Sayaç: $sayac"
    ((sayac++))
    sleep 1
done

# Dosya satır satır okuma
while IFS= read -r satir; do
    echo "→ $satir"
done < /etc/hosts

# Sonsuz döngü
while true; do
    echo "Çalışıyor..."
    sleep 5
done
```

### until

```bash
# Koşul sağlanana kadar döner (while'ın tersi)
sayac=10
until [ "$sayac" -le 0 ]; do
    echo "Geri sayım: $sayac"
    ((sayac--))
    sleep 1
done
echo "Başlat!"
```

### break ve continue

```bash
for i in {1..10}; do
    [ "$i" -eq 5 ] && break     # 5'te dur
    [ "$((i % 2))" -eq 0 ] && continue  # Çift sayıları atla
    echo "$i"
done
```

---

## Fonksiyonlar

```bash
#!/bin/bash

# Fonksiyon tanımlama
selamla() {
    local isim="$1"   # local: fonksiyon dışında görünmez
    local zaman=$(date +%H)

    if [ "$zaman" -lt 12 ]; then
        echo "Günaydın, $isim!"
    else
        echo "İyi günler, $isim!"
    fi
    return 0  # Çıkış kodu (0=başarı)
}

# Çıkış kodu dönen fonksiyon
dosya_var_mi() {
    [ -f "$1" ]  # Son komutun çıkış kodunu döner
}

# Çoklu değer döndürme (global veya echo ile)
ikiye_bol() {
    local sonuc=$(( $1 / 2 ))
    echo "$sonuc"   # stdout üzerinden değer aktar
}

# Kullanım
selamla "Serkan"
selamla "Mert"

if dosya_var_mi "/etc/hosts"; then
    echo "/etc/hosts mevcut"
fi

bolum=$(ikiye_bol 20)
echo "20 / 2 = $bolum"
```

!!! danger "local Kullanmak Zorunlu Değil Ama Kritik"
    Fonksiyon içinde `local` kullanılmazsa değişken **global** olur. İki fonksiyonun aynı değişkeni paylaşması beklenmedik hatalara yol açar.

---

## Argüman İşleme

```bash
#!/bin/bash

# Kullanım kılavuzu
kullanim() {
    echo "Kullanım: $0 [-v] [-f DOSYA] [-n SAYI] [ARG...]"
    echo "  -v       Ayrıntılı mod"
    echo "  -f FILE  Girdi dosyası"
    echo "  -n NUM   Sayı parametresi"
    exit 1
}

verbose=0
dosya=""
sayi=10

# getopts ile argüman ayrıştırma
while getopts ":vf:n:" opt; do
    case "$opt" in
        v) verbose=1 ;;
        f) dosya="$OPTARG" ;;
        n) sayi="$OPTARG" ;;
        :) echo "Seçenek -$OPTARG argüman gerektirir."; kullanim ;;
        \?) echo "Bilinmeyen seçenek: -$OPTARG"; kullanim ;;
    esac
done
shift $((OPTIND - 1))  # İşlenen seçenekleri kaydır; $@ = kalan argümanlar

[ $verbose -eq 1 ] && echo "Verbose mod açık"
[ -n "$dosya" ] && echo "Dosya: $dosya"
echo "Kalan argümanlar: $@"
```

---

## Diziler (Arrays)

```bash
#!/bin/bash

# Tanımlama
meyveler=("elma" "armut" "kiraz")
sayilar=(1 2 3 4 5)

# Erişim
echo "${meyveler[0]}"       # elma
echo "${meyveler[@]}"       # Tümü
echo "${#meyveler[@]}"      # Eleman sayısı (3)
echo "${meyveler[@]:1:2}"   # Dilim (1'den 2 eleman)

# Ekleme
meyveler+=("mango")
meyveler[10]="üzüm"         # Sparse array mümkün

# Silme
unset meyveler[1]

# Döngü
for meyve in "${meyveler[@]}"; do
    echo "- $meyve"
done

# İndeks + değer
for i in "${!meyveler[@]}"; do
    echo "[$i] = ${meyveler[$i]}"
done

# İlişkisel dizi (Bash 4+)
declare -A kisi
kisi["isim"]="Serkan"
kisi["yas"]=30
echo "${kisi["isim"]} — ${kisi["yas"]}"
```

---

## Hata Yönetimi

```bash
#!/bin/bash
set -euo pipefail   # Güvenli mod: hata=dur, tanımsız=dur, pipe hata=dur

# Trap ile temizlik
temizle() {
    echo "Çıkılıyor, temizlik yapılıyor..."
    rm -f /tmp/geçici_$$.txt
}
trap temizle EXIT     # Script çıkışında çalışır
trap 'echo "Sinyal alındı!"' INT TERM  # Ctrl+C veya kill

# Hata kontrolü
komut_calis() {
    if ! komut_adı 2>/dev/null; then
        echo "HATA: komut başarısız" >&2   # stderr'e yaz
        return 1
    fi
}

# Çıkış kodu kontrolü
ls /var/log/syslog
if [ "$?" -ne 0 ]; then
    echo "Dosya okunamadı"
fi

# Kısa versiyon
ls /yanlış/yol || { echo "Dizin yok!"; exit 1; }
```

---

## Metin İşleme — Araç Kombinasyonları

```bash
# Hata içeren log satırlarını say
grep -c "ERROR" uygulama.log

# İlk 5 büyük dosyayı bul
du -sh /var/log/* | sort -rh | head -5

# Belirli bir kullanıcının çalışan process sayısı
ps aux | awk '{print $1}' | grep -c "serkan"

# CSV'den 2. sütunu al ve sırala
cut -d',' -f2 data.csv | sort | uniq -c | sort -rn

# Log satırlarından IP adreslerini çıkart
grep -oE '([0-9]{1,3}\.){3}[0-9]{1,3}' access.log | sort | uniq -c | sort -rn

# Dosyada kelime değiştir (in-place)
sed -i 's/HOST=localhost/HOST=192.168.1.10/g' config.env

# AWK: ms>500 olan log satırlarını bul
awk -F' ' '{for(i=1;i<=NF;i++) if($i~/^ms=/ && substr($i,4)+0>500) print $0}' app.log
```

---

## Here Document ve Here String

```bash
# Here Document — birden fazla satır
cat > /tmp/mesaj.txt << 'EOF'
Merhaba!
Bu birden fazla satır içerir.
Değişkenler burada genişlemez: $HOME
EOF

# Değişken genişletme aktifken (tek tırnak yok)
cat << EOF
Host: $(hostname)
Tarih: $(date)
Kullanıcı: $USER
EOF

# Here String — tek satır stdin
grep "pattern" <<< "bu bir test dizesidir"
```

---

## Pratik Script Örnekleri

=== "Yedekleme Scripti"

    ```bash
    #!/bin/bash
    set -euo pipefail

    KAYNAK="/home/serkan/projeler"
    HEDEF="/backup"
    TARIH=$(date +%Y%m%d_%H%M%S)
    ARSIV="$HEDEF/yedek_$TARIH.tar.gz"

    mkdir -p "$HEDEF"

    echo "Yedekleme başlıyor: $KAYNAK → $ARSIV"
    tar -czf "$ARSIV" -C "$(dirname "$KAYNAK")" "$(basename "$KAYNAK")"
    echo "Başarılı: $(du -sh "$ARSIV" | cut -f1)"

    # 30 günden eski yedekleri sil
    find "$HEDEF" -name "yedek_*.tar.gz" -mtime +30 -delete
    echo "Eski yedekler temizlendi."
    ```

=== "Servis Sağlık Kontrol"

    ```bash
    #!/bin/bash
    SERVISLER=("nginx" "postgresql" "redis")
    HATA=0

    for servis in "${SERVISLER[@]}"; do
        if systemctl is-active --quiet "$servis"; then
            echo "✓ $servis çalışıyor"
        else
            echo "✗ $servis DURDU — yeniden başlatılıyor..."
            systemctl restart "$servis" || { echo "HATA: $servis başlatılamadı"; HATA=1; }
        fi
    done

    exit $HATA
    ```

=== "Log Rotasyon"

    ```bash
    #!/bin/bash
    LOG_DIR="/var/log/myapp"
    MAX_SIZE_MB=100
    MAX_DOSYA=5

    for log in "$LOG_DIR"/*.log; do
        boyut=$(du -m "$log" | cut -f1)
        if [ "$boyut" -gt "$MAX_SIZE_MB" ]; then
            # Rotate
            mv "$log" "${log}.$(date +%Y%m%d)"
            gzip "${log}.$(date +%Y%m%d)"
            touch "$log"
            echo "Rotate: $log ($boyut MB)"
        fi
    done

    # En eski log dosyasını sil
    dosya_sayisi=$(ls "$LOG_DIR"/*.log.gz 2>/dev/null | wc -l)
    if [ "$dosya_sayisi" -gt "$MAX_DOSYA" ]; then
        ls -t "$LOG_DIR"/*.log.gz | tail -n +$((MAX_DOSYA + 1)) | xargs rm -f
    fi
    ```

=== "Sistem Bilgi Raporu"

    ```bash
    #!/bin/bash
    echo "=== Sistem Raporu $(date) ==="
    echo ""
    echo "--- Hostname ---"
    hostname -f

    echo ""
    echo "--- CPU ---"
    grep "model name" /proc/cpuinfo | head -1 | cut -d: -f2 | xargs

    echo ""
    echo "--- RAM ---"
    free -h | awk '/^Mem:/{printf "Toplam: %s | Kullanılan: %s | Boş: %s\n", $2, $3, $4}'

    echo ""
    echo "--- Disk ---"
    df -h / | awk 'NR==2 {printf "Toplam: %s | Kullanılan: %s (%s)\n", $2, $3, $5}'

    echo ""
    echo "--- Son 5 Login ---"
    last -5

    echo ""
    echo "--- Yüksek CPU Process'ler ---"
    ps aux --sort=-%cpu | head -6
    ```

---

## Crontab

```mermaid
graph LR
    CRON[Cron Daemon] --> TAB[/var/spool/cron/\ncrontab dosyası]
    CRON --> ETC[/etc/cron.d/\n/etc/cron.daily/\n/etc/cron.weekly/]
    CRON --> RUN[Script Çalıştır]
```

```
# Crontab formatı:
# dakika saat gün_ay ay gün_haftası komut
#   0     2   *     *      *       /backup.sh

# Özel değerler
@reboot      # Sistem başlangıcında bir kez
@daily       # Her gün 00:00
@weekly      # Her hafta
@monthly     # Her ay

# Örnekler
0 2 * * *     /usr/local/bin/backup.sh        # Her gün 02:00
*/5 * * * *   /opt/kontrol.sh                  # Her 5 dakikada bir
0 9 1 * *     /raporla.sh                      # Her ayın 1'i saat 09:00
0 8-18 * * 1-5 /mesai_bildiri.sh               # Hafta içi 08-18 arası
```

```bash
crontab -e    # Mevcut kullanıcının crontab'ını düzenle
crontab -l    # Listeye bak
crontab -r    # Sil (dikkat!)
crontab -u serkan -l  # Başka kullanıcının crontab'ı (root)
```

!!! warning "Crontab Ortam Değişkenleri"
    Cron, interaktif shell'den farklı bir ortamda çalışır; `$PATH` çok kısıtlıdır. Script'lerde komutların **tam yolunu** kullanın: `/usr/bin/python3` yerine `python3` kullanmak çalışmayabilir. Script başına `source /etc/profile` veya `export PATH=/usr/local/bin:$PATH` ekleyin.

---

## İleri Bash Teknikleri

### Process Substitution

Bir komutun çıktısını sanki dosyaymış gibi başka bir komuta argüman olarak aktarır.

```bash
# diff: iki komut çıktısını karşılaştır (geçici dosya gerekmez)
diff <(ls /dir1) <(ls /dir2)

# sort + comm: iki dosyanın sıralı farkları
comm -23 <(sort dosya1.txt) <(sort dosya2.txt)

# while: iki komutu aynı anda oku
paste <(cut -d: -f1 /etc/passwd) <(cut -d: -f6 /etc/passwd)

# tee ile birden fazla işlem yürütme
cat büyük_dosya.txt | tee >(gzip > arsiv.gz) >(wc -l) > /dev/null
```

### Named Pipe (FIFO)

```bash
# FIFO oluştur
mkfifo /tmp/myfifo

# Terminal 1: yazar
echo "Merhaba FIFO" > /tmp/myfifo

# Terminal 2: okuyucu (okuyucu olmadan yazar bloklanır)
cat /tmp/myfifo

# Pratik: iki script arasında veri akışı
mkfifo /tmp/veri_akisi
./veri_üreten.sh > /tmp/veri_akisi &
./veri_işleyen.sh < /tmp/veri_akisi
rm /tmp/veri_akisi
```

### printf — Biçimlendirilmiş Çıktı

```bash
# printf, echo'dan daha güvenilir ve taşınabilir
printf "Merhaba, %s!\n" "Dünya"
printf "Sayı: %d, Ondalık: %.2f\n" 42 3.14159
printf "Hex: %x, Oct: %o\n" 255 255     # ff  377
printf "%-20s %5d\n" "Öğe"  42          # Sola hizalı, sağa hizalı
printf "%0*d\n" 5 7                      # 00007 — genişlik değişkende

# Renkli çıktı (ANSI escape)
printf "\033[1;32m%-10s\033[0m %s\n" "OK" "Servis çalışıyor"
printf "\033[1;31m%-10s\033[0m %s\n" "HATA" "Bağlantı kesildi"

# Pratik: tablo çıktısı
printf "%-15s %-10s %s\n" "HOST" "STATUS" "UPTIME"
printf "%-15s %-10s %s\n" "192.168.1.1" "UP" "5 gün"
printf "%-15s %-10s %s\n" "192.168.1.2" "DOWN" "-"
```

### mapfile / readarray

```bash
# Dosyayı diziye oku (while okuma yerine)
mapfile -t satirlar < /etc/hosts
echo "${satirlar[0]}"          # İlk satır
echo "${#satirlar[@]}"         # Satır sayısı

# Komut çıktısını diziye al
mapfile -t process_list < <(ps -eo pid,comm --no-header)

# Döngü
for satir in "${satirlar[@]}"; do
    [[ "$satir" == \#* ]] && continue    # Yorum satırlarını atla
    echo "→ $satir"
done
```

### select — Etkileşimli Menü

```bash
#!/bin/bash

PS3="Seçiminiz: "    # select isteği
secenekler=("Disk Bilgisi" "RAM Bilgisi" "CPU Bilgisi" "Çıkış")

select secim in "${secenekler[@]}"; do
    case "$REPLY" in
        1) df -h ;;
        2) free -h ;;
        3) lscpu | grep "Model name" ;;
        4) echo "Çıkılıyor..."; break ;;
        *) echo "Geçersiz seçim: $REPLY" ;;
    esac
done
```

### Paralel Komut Çalıştırma

```bash
# Arka plana alıp bekle
for host in 192.168.1.{1..10}; do
    ping -c 1 -W 1 "$host" &>/dev/null && echo "$host UP" &
done
wait   # Tüm arka plan işleri tamamlanana kadar bekle

# xargs ile paralel
cat host_listesi.txt | xargs -P 4 -I{} ping -c 1 {} 2>/dev/null

# GNU parallel (kurulumsa)
parallel -j 4 ping -c 1 {} ::: 192.168.1.{1..20}

# Semafor ile paralel (maksimum N eş zamanlı iş)
maks=4
sayac=0
for dosya in /data/*.csv; do
    ./isle.sh "$dosya" &
    ((++sayac >= maks)) && { wait -n; ((sayac--)); }
done
wait
```

### Güvenli Geçici Dosya

```bash
# mktemp — güvenli geçici dosya/dizin
gecici=$(mktemp)                       # /tmp/tmp.XXXXXX
gecici_dir=$(mktemp -d)               # /tmp/tmp.XXXXXX/

# Temizlik garantisi
trap 'rm -f "$gecici"; rm -rf "$gecici_dir"' EXIT

# İsimli şablon
log=$(mktemp /tmp/uygulama_XXXXXX.log)
echo "Log: $log"
```