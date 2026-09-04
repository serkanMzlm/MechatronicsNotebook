#!/bin/bash
# Diziler (indexed/associative array) ve komut satırı argüman işleme (getopts) örnekleri.
set -euo pipefail

indexed_array_demo() {
    echo "=== Indexed Array Demo ==="
    local meyveler=("elma" "armut" "kiraz")
    meyveler+=("mango")                                    # Sona eleman ekleme

    for i in "${!meyveler[@]}"; do                         # "${!dizi[@]}" -> indeksler
        echo "[$i] = ${meyveler[$i]}"
    done

    echo "Toplam eleman: ${#meyveler[@]}"                  # Dizi eleman sayısı
    echo "Tüm elemanlar: ${meyveler[*]}"                   # Tek string olarak (IFS ile birleşir)
    echo "Dizi (quoted): ${meyveler[@]@Q}"                  # Her elemanı ayrı token olarak göster

    unset 'meyveler[1]'                                    # İndeks 1'i (armut) sil; boşluk indekste kalır
    echo "Silme sonrası indeksler: ${!meyveler[@]}"
}

associative_array_demo() {
    echo "=== Associative Array Demo (Bash 4+) ==="
    local -A kisi                                          # -A: key/value dizi, key sırası garanti değildir
    kisi["isim"]="Serkan"
    kisi["yas"]=30
    kisi["sehir"]="İstanbul"

    echo "${kisi["isim"]} - ${kisi["yas"]} - ${kisi["sehir"]}"

    for anahtar in "${!kisi[@]}"; do                       # Anahtarlar üzerinde gez
        echo "  $anahtar => ${kisi[$anahtar]}"
    done

    if [[ -v kisi["meslek"] ]]; then                        # -v: anahtar tanımlı mı (boş string de "tanımlı" sayılır)
        echo "meslek tanımlı"
    else
        echo "meslek tanımlı değil"
    fi
}

# getopts: kısa (-f dosya) opsiyonları işlemek için POSIX uyumlu, harici bağımlılık gerektirmeyen yöntem.
# Uzun opsiyon (--file gibi) gerekiyorsa getopt(1) ya da manuel while/case ile $@ taraması kullanılır.
parse_arguments_demo() {
    echo "=== Argument Parsing Demo (getopts) ==="
    local dosya="" seviye="bilgi" ayrintili=0

    # ":f:s:v" -> f ve s bir değer bekler (':' sonrası), v bayrak (flag), baştaki ':' hataları sessize alır
    local OPTIND=1                                         # Fonksiyon içinde tekrar çağrılabilmesi için sıfırla
    while getopts ":f:s:v" opt "$@"; do
        case "$opt" in
            f) dosya="$OPTARG" ;;
            s) seviye="$OPTARG" ;;
            v) ayrintili=1 ;;
            \?) echo "Bilinmeyen seçenek: -$OPTARG" >&2; return 1 ;;
            :)  echo "-$OPTARG bir değer bekliyor" >&2; return 1 ;;
        esac
    done
    shift $((OPTIND - 1))                                  # İşlenen seçenekleri at; geri kalan pozisyonel argümanlar $@

    echo "Dosya: ${dosya:-<belirtilmedi>}"
    echo "Seviye: $seviye"
    echo "Ayrıntılı: $ayrintili"
    echo "Kalan pozisyonel argümanlar: $*"
}

indexed_array_demo
echo
associative_array_demo
echo
parse_arguments_demo -f "config.yaml" -s "debug" -v ekstra_arg1 ekstra_arg2
