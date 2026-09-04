#!/bin/bash
# İleri seviye bash teknikleri: process substitution, named pipe, printf,
# mapfile, select menüsü, arka planda paralel çalıştırma ve mktemp/trap.
set -uo pipefail

process_substitution_demo() {
    echo "=== Process Substitution Demo ==="
    # diff <(...) <(...) : iki komutun çıktısını geçici dosya oluşturmadan karşılaştırır.
    diff <(ls /etc | sort) <(ls /var | sort) | head -5 || true
}

named_pipe_demo() {
    echo "=== Named Pipe (FIFO) Demo ==="
    local pipe="/tmp/veri_akisi_$$"     # $$ : bu script'in PID'i, çakışmayı önler
    mkfifo "$pipe"

    # Okuyucuyu arka planda başlat, yoksa yazan taraf pipe dolana kadar bloklanır
    (cat "$pipe") &
    local okuyucu_pid=$!

    echo "merhaba named pipe" > "$pipe"
    wait "$okuyucu_pid"
    rm -f "$pipe"
}

printf_demo() {
    echo "=== printf Demo ==="
    # printf, echo'nun aksine biçimlendirmede taşınabilir ve öngörülebilirdir (echo -e her shell'de aynı davranmaz)
    printf "%-15s %-10s %s\n" "HOST" "STATUS" "UPTIME"
    printf "%-15s %-10s %s\n" "192.168.1.1" "UP" "5 gün"
}

mapfile_demo() {
    echo "=== mapfile Demo ==="
    local satirlar
    mapfile -t satirlar < /etc/hosts     # Dosyayı while-read yerine tek seferde diziye oku (-t: satır sonu \n'i at)
    echo "İlk satır: ${satirlar[0]}"
    echo "Toplam satır: ${#satirlar[@]}"
}

select_menu_demo() {
    echo "=== select Menü Demo ==="
    local secenekler=("Disk Bilgisi" "RAM Bilgisi" "Çıkış")
    local PS3="Seçiminiz: "               # select'in prompt değişkeni

    select secim in "${secenekler[@]}"; do
        case "$REPLY" in
            1) df -h; break ;;
            2) free -h; break ;;
            3) break ;;
            *) echo "Geçersiz seçim: $REPLY" ;;
        esac
    done
}

parallel_ping_demo() {
    echo "=== Arka Planda Paralel Çalıştırma Demo ==="
    for host in 192.168.1.{1..3}; do
        # Her ping'i arka plana at, hepsi bittiğinde 'wait' ile senkronize ol.
        # Çok sayıda host için xargs -P veya GNU parallel daha ölçeklenebilirdir.
        (ping -c 1 -W 1 "$host" &>/dev/null && echo "$host UP" || echo "$host DOWN") &
    done
    wait
}

temp_file_with_trap_demo() {
    echo "=== mktemp + trap Demo ==="
    local gecici
    gecici=$(mktemp)                      # Çakışmayan, güvenli geçici dosya adı üretir
    trap 'rm -f "$gecici"' EXIT           # Script/fonksiyon çıkışında (hata dahil) otomatik temizlik

    echo "geçici veri" > "$gecici"
    cat "$gecici"
}

process_substitution_demo; echo
named_pipe_demo; echo
printf_demo; echo
mapfile_demo; echo
parallel_ping_demo; echo
temp_file_with_trap_demo
# select_menu_demo   # İnteraktif olduğu için otomatik çalıştırmada yorum satırı bırakıldı
