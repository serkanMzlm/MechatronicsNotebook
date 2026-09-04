# Shell Scripting

```bash
cat /etc/shells    # Sistemde yüklü shell'leri listeler
echo $SHELL        # Aktif shell
chsh -s /bin/zsh   # Varsayılan shell değiştir (oturum yenilenmeli)
```

!!! tip "Shebang"
    Shebang (`#!/bin/bash`) kernel'e hangi yorumlayıcının kullanılacağını söyler. `#!/usr/bin/env bash` kullanımı daha taşınabilirdir - `bash`'ı PATH üzerinden bulur.


```bash
#!/bin/bash
set -euo pipefail                      # Güvenli mod: hata=dur, tanımsız değişken=dur, pipe hatası=dur
trap 'rm -f /tmp/geçici_$$.txt' EXIT   # Script çıkışında temizlik çalışır

degiskenler() {
    local isim="Serkan"
    local tarih; tarih=$(date +%Y-%m-%d)          # Komut çıktısını değişkene ata
    echo "İsim: $isim, Tarih: ${tarih}"           # {} değişken sınırını belirtir

    local metin="Hello, Linux World!"
    echo "${#metin} | ${metin:7:5} | ${metin//o/0} | ${metin,,}"  # uzunluk | substring | değiştir | küçük harf
    echo "${AYAR:-varsayılan}"                    # AYAR tanımsızsa varsayılan değeri döner
}

kontrol_akisi() {
    local puan=85
    if [ "$puan" -ge 90 ]; then echo "AA"
    elif [ "$puan" -ge 70 ]; then echo "BA/BB"
    else echo "Başarısız"; fi

    [[ "$1" == ser* ]] && echo "ser ile başlıyor"   # [[ ]] glob destekler, string karşılaştırma

    for i in {1..10}; do
        [ "$i" -eq 5 ] && break               # 5'te dur
        [ "$((i % 2))" -eq 0 ] && continue    # Çift sayıları atla
        echo -n "$i "
    done; echo

    while IFS= read -r satir; do echo "→ $satir"; done < /etc/hosts   # Dosyayı satır satır oku
}

dosya_var_mi() { [ -f "$1" ]; }        # Son komutun çıkış kodunu (0/1) döner
ikiye_bol()    { echo $(( $1 / 2 )); } # Değeri stdout üzerinden aktar

diziler() {
    local meyveler=("elma" "armut" "kiraz")
    meyveler+=("mango")                        # Sona ekleme
    for i in "${!meyveler[@]}"; do echo "[$i] = ${meyveler[$i]}"; done  # İndeks + değer

    local -A kisi                              # İlişkisel dizi (Bash 4+)
    kisi["isim"]="Serkan"; kisi["yas"]=30
    echo "${kisi["isim"]} - ${kisi["yas"]}"
}

argumanlari_isle() {
    local dosya=""
    while getopts ":f:" opt; do
        case "$opt" in
            f) dosya="$OPTARG" ;;
            \?) echo "Bilinmeyen seçenek: -$OPTARG" ;;
        esac
    done
    shift $((OPTIND - 1))                      # İşlenen seçenekleri kaydır; $@ = kalan argümanlar
    echo "Dosya: $dosya, Kalan argümanlar: $*"
}

metin_ve_heredoc() {
    grep -c "ERROR" uygulama.log 2>/dev/null || true                       # Hata içeren satır sayısı
    cut -d',' -f2 data.csv 2>/dev/null | sort | uniq -c | sort -rn         # Sütun al, sırala, tekrar say
    sed -i 's/HOST=localhost/HOST=192.168.1.10/g' config.env 2>/dev/null || true  # Yerinde değiştir

    cat << EOF          # Tırnaksız sınırlayıcı: değişkenler/komutlar genişler
Host: $(hostname), Tarih: $(date)
EOF
    grep "pattern" <<< "bu bir test dizesidir"  # Here String: tek satırlık stdin
}

degiskenler
kontrol_akisi "serkan"
dosya_var_mi "/etc/hosts" && echo "/etc/hosts mevcut"
bolum=$(ikiye_bol 20); echo "20 / 2 = $bolum"
diziler
metin_ve_heredoc
```

| Operatör   | Sayısal |          String         |                   Dosya                    |
| ---------- | :-----: | :---------------------: | :----------------------------------------: |
| Eşit       |  `-eq`  |      `=` veya `==`      |                     -                      |
| Eşit değil |  `-ne`  |           `!=`          |                     -                      |
| Küçük      |  `-lt`  |            -            |                     -                      |
| Büyük      |  `-gt`  |            -            |                     -                      |
| Küçük eşit |  `-le`  |            -            |                     -                      |
| Büyük eşit |  `-ge`  |            -            |                     -                      |
| Var mı     |    -    | `-z` (boş), `-n` (dolu) | `-e` (dosya/dir), `-f` (dosya), `-d` (dir) |

!!! danger "local Kullanmak Zorunlu Değil Ama Kritik"
    Fonksiyon içinde `local` kullanılmazsa değişken **global** olur. İki fonksiyonun aynı değişkeni paylaşması beklenmedik hatalara yol açar.

---

## Pratik Script Örnekleri

=== "Yedekleme Scripti"

    ```bash
    #!/bin/bash
    set -euo pipefail

    KAYNAK="/home/serkan/projeler"
    HEDEF="/backup"
    ARSIV="$HEDEF/yedek_$(date +%Y%m%d_%H%M%S).tar.gz"

    mkdir -p "$HEDEF"
    tar -czf "$ARSIV" -C "$(dirname "$KAYNAK")" "$(basename "$KAYNAK")"
    echo "Başarılı: $(du -sh "$ARSIV" | cut -f1)"

    find "$HEDEF" -name "yedek_*.tar.gz" -mtime +30 -delete   # 30 günden eski yedekleri sil
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
            echo "✗ $servis DURDU - yeniden başlatılıyor..."
            systemctl restart "$servis" || { echo "HATA: $servis başlatılamadı"; HATA=1; }
        fi
    done

    exit $HATA
    ```

---

## Crontab

```
# Crontab formatı:
# dakika saat gün_ay ay gün_haftası komut
#   0     2   *     *      *       /backup.sh

0 2 * * *     /usr/local/bin/backup.sh     # Her gün 02:00
*/5 * * * *   /opt/kontrol.sh               # Her 5 dakikada bir
0 8-18 * * 1-5 /mesai_bildiri.sh            # Hafta içi 08-18 arası
```

```bash
crontab -e    # Mevcut kullanıcının crontab'ını düzenle
crontab -l    # Listeye bak
crontab -u serkan -l  # Başka kullanıcının crontab'ı (root)
```

!!! warning "Crontab Ortam Değişkenleri"
    Cron, interaktif shell'den farklı bir ortamda çalışır; `$PATH` çok kısıtlıdır. Script'lerde komutların **tam yolunu** kullanın: `/usr/bin/python3` yerine `python3` kullanmak çalışmayabilir. Script başına `source /etc/profile` veya `export PATH=/usr/local/bin:$PATH` ekleyin.

---

## İleri Bash Teknikleri

```bash
diff <(ls /dir1) <(ls /dir2)          # Process substitution: çıktıları geçici dosya oluşturmadan karşılaştır

mkfifo /tmp/veri_akisi                # Named pipe: iki process arasında veri akışı
./veri_üreten.sh > /tmp/veri_akisi &
./veri_işleyen.sh < /tmp/veri_akisi
rm /tmp/veri_akisi

printf "%-15s %-10s %s\n" "HOST" "STATUS" "UPTIME"    # printf: echo'dan daha güvenilir biçimlendirme
printf "%-15s %-10s %s\n" "192.168.1.1" "UP" "5 gün"

mapfile -t satirlar < /etc/hosts       # Dosyayı while yerine tek seferde diziye oku
echo "${satirlar[0]} (${#satirlar[@]} satır)"

PS3="Seçiminiz: "                      # select: numaralı, etkileşimli menü
secenekler=("Disk Bilgisi" "RAM Bilgisi" "Çıkış")
select secim in "${secenekler[@]}"; do
    case "$REPLY" in
        1) df -h ;;
        2) free -h ;;
        3) break ;;
        *) echo "Geçersiz seçim: $REPLY" ;;
    esac
done

for host in 192.168.1.{1..10}; do      # Paralel çalıştırma: arka plana alıp topluca bekle
    ping -c 1 -W 1 "$host" &>/dev/null && echo "$host UP" &
done
wait

gecici=$(mktemp)                       # Çakışmayan, güvenli geçici dosya
trap 'rm -f "$gecici"' EXIT            # Script çıkışında otomatik temizlik
```
