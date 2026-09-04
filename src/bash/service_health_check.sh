#!/bin/bash
# systemd servislerinin durumunu kontrol eder, çalışmayanları yeniden başlatmayı dener.
# Kullanım: sudo ./service_health_check.sh nginx postgresql redis
set -uo pipefail   # NOT: -e kullanılmadı; bir servis hatası döngünün geri kalanını durdurmamalı.

SERVISLER=("$@")
if [[ ${#SERVISLER[@]} -eq 0 ]]; then
    SERVISLER=("nginx" "postgresql" "redis")   # Argüman verilmezse varsayılan liste
fi

HATA=0

for servis in "${SERVISLER[@]}"; do
    if systemctl is-active --quiet "$servis"; then
        echo "✓ $servis çalışıyor"
        continue
    fi

    echo "✗ $servis DURDU - yeniden başlatılıyor..."
    if systemctl restart "$servis"; then
        echo "  -> $servis yeniden başlatıldı"
    else
        echo "  -> HATA: $servis başlatılamadı, loglara bakın: journalctl -u $servis -n 50" >&2
        HATA=1
    fi
done

exit $HATA
