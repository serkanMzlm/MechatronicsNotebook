#!/bin/bash
# Crontab formatı ve mevcut kullanıcının crontab'ına script eklemeyi/kaldırmayı
# idempotent (tekrar çalıştırılsa da aynı satırı ikinci kez eklemeyen) şekilde yapan yardımcı fonksiyonlar.
#
# Crontab formatı:
#   dakika  saat  gün_ay  ay  gün_haftası  komut
#     0      2      *     *       *        /backup.sh        -> her gün 02:00
#    */5     *      *     *       *        /opt/kontrol.sh   -> her 5 dakikada bir
#     0     8-18    *     *      1-5       /mesai_bildiri.sh -> hafta içi 08-18 arası
set -euo pipefail

# ÖNEMLİ: cron, login shell'den farklı ve çok kısıtlı bir $PATH ile çalışır.
# Cron ile çalıştırılan script'lerde komutların TAM YOLUNU kullanın
# (örn. python3 yerine /usr/bin/python3), aksi halde "command not found" hatası alınır.

cron_job_ekle() {
    local zamanlama="$1"          # örn: "0 2 * * *"
    local komut="$2"              # örn: "/usr/local/bin/backup.sh"
    local satir="$zamanlama $komut"

    if crontab -l 2>/dev/null | grep -qF "$komut"; then
        echo "Zaten mevcut, tekrar eklenmedi: $komut"
        return 0
    fi

    # Mevcut crontab'ı al (yoksa boş kabul et), yeni satırı ekle, geri yükle
    { crontab -l 2>/dev/null || true; echo "$satir"; } | crontab -
    echo "Eklendi: $satir"
}

cron_job_kaldir() {
    local komut="$1"
    crontab -l 2>/dev/null | grep -vF "$komut" | crontab -
    echo "Kaldırıldı (varsa): $komut"
}

cron_job_listele() {
    crontab -l 2>/dev/null || echo "Bu kullanıcı için tanımlı crontab yok."
}

# Örnek kullanım:
# cron_job_ekle "0 2 * * *" "/usr/local/bin/backup.sh"
# cron_job_listele
# cron_job_kaldir "/usr/local/bin/backup.sh"
