#!/bin/bash
# grep/cut/sed ile metin işleme, heredoc ve here-string kullanımı.
set -uo pipefail   # NOT: -e kullanılmadı; grep eşleşme bulamayınca 1 döner, bu script'te bu normal bir durumdur.

log_analysis_demo() {
    echo "=== Log/Text Analysis Demo ==="
    local log_dosyasi="/tmp/ornek_uygulama.log"

    # Örnek log dosyası oluştur (gerçek projede bu satır olmaz, mevcut log kullanılır)
    cat > "$log_dosyasi" << 'EOF'
2026-01-01 10:00:01 INFO  Servis başladı
2026-01-01 10:00:05 ERROR Bağlantı zaman aşımına uğradı
2026-01-01 10:00:07 ERROR Veritabanına erişilemedi
2026-01-01 10:00:10 INFO  Yeniden deneme başarılı
EOF

    grep -c "ERROR" "$log_dosyasi" || true                          # Hata satırı sayısı; hiç yoksa grep 1 döner
    grep -E "ERROR|WARN" "$log_dosyasi"                              # Birden fazla kalıp (ERE)
    cut -d' ' -f4- "$log_dosyasi" | sort | uniq -c | sort -rn         # Mesajları grupla ve say

    rm -f "$log_dosyasi"                                              # Geçici dosyayı temizle
}

find_and_replace_demo() {
    echo "=== sed ile Yerinde Değiştirme Demo ==="
    local config_dosyasi="/tmp/ornek_config.env"
    echo "HOST=localhost" > "$config_dosyasi"

    sed -i 's/HOST=localhost/HOST=192.168.1.10/g' "$config_dosyasi"  # -i: dosyayı yerinde değiştirir (yedeksiz)
    sed -i.bak 's/^/# /' "$config_dosyasi"                            # .bak uzantısıyla yedek alarak değiştir

    cat "$config_dosyasi"
    rm -f "$config_dosyasi" "$config_dosyasi.bak"
}

heredoc_and_herestring_demo() {
    echo "=== Heredoc ve Here-String Demo ==="

    # Tırnaksız sınırlayıcı (EOF): içindeki $degisken ve $(komut) genişler
    cat << EOF
Sunucu: $(hostname)
Tarih : $(date +%F)
EOF

    # Tırnaklı sınırlayıcı ('EOF'): hiçbir genişleme yapılmaz, ham metin olarak yazılır
    cat << 'EOF'
Bu satırdaki $degisken ve $(komut) OLDUĞU GİBİ yazılır, genişletilmez.
EOF

    grep "test" <<< "bu bir test dizesidir"                          # Here-string: tek satırlık stdin verme
}

log_analysis_demo
echo
find_and_replace_demo
echo
heredoc_and_herestring_demo
