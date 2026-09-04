#!/bin/bash
# Basit dizin yedekleme scripti: kaynağı tar.gz olarak arşivler, eski yedekleri temizler.
# Kullanım: ./backup_script.sh [KAYNAK_DIZIN] [HEDEF_DIZIN]
set -euo pipefail

KAYNAK="${1:-/home/serkan/projeler}"
HEDEF="${2:-/backup}"
ARSIV="$HEDEF/yedek_$(date +%Y%m%d_%H%M%S).tar.gz"
SAKLAMA_GUNU=30

if [[ ! -d "$KAYNAK" ]]; then
    echo "HATA: Kaynak dizin bulunamadı: $KAYNAK" >&2
    exit 1
fi

mkdir -p "$HEDEF"

# -C ile hedef dizine geçip sadece son klasör adını arşivlemek, arşiv içinde
# "/home/serkan/projeler/..." gibi tam yol yerine "projeler/..." göreli yolunu üretir.
tar -czf "$ARSIV" -C "$(dirname "$KAYNAK")" "$(basename "$KAYNAK")"

echo "Başarılı: $ARSIV ($(du -sh "$ARSIV" | cut -f1))"

# $SAKLAMA_GUNU günden eski yedekleri sil (disk şişmesini önlemek için)
find "$HEDEF" -maxdepth 1 -name "yedek_*.tar.gz" -mtime "+$SAKLAMA_GUNU" -print -delete
