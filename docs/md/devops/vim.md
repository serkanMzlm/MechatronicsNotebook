# Vim / VI

## Mod Sistemi

```mermaid
stateDiagram-v2
    [*] --> Normal: vim dosya.txt
    Normal --> Insert: i / a / o / O / I / A
    Normal --> Visual: v / V / Ctrl+v
    Normal --> Command: :
    Insert --> Normal: Esc
    Visual --> Normal: Esc
    Command --> Normal: Enter / Esc
```

| Mod         | Giriş              | Açıklama                                   |
| ----------- | ------------------ | ------------------------------------------ |
| **Normal**  | `Esc`              | Komut modu - gezinme, silme, kopyalama     |
| **Insert**  | `i`, `a`, `o`      | Metin yazma                                |
| **Visual**  | `v`, `V`, `Ctrl+v` | Blok seçme                                 |
| **Command** | `:`                | Dosya işlemleri, ayarlar, arama-değiştirme |

---

## .vimrc Yapılandırması

```vim title="~/.vimrc"
" ===== Görünüm =====
set number              " Satır numaraları
set relativenumber      " Göreceli satır numarası
syntax on               " Syntax highlight
set termguicolors       " True color
colorscheme desert

" ===== Girinti =====
set tabstop=4
set shiftwidth=4
set softtabstop=4
set noexpandtab         " Gerçek TAB (space değil)

" ===== Kullanılabilirlik =====
set ruler
set showcmd
set cursorline
set wildmenu
set mouse=a

" ===== Dosya / Güvenlik =====
set nobackup
set nowritebackup
set noswapfile

" ===== Arama =====
set incsearch           " Yazarken ara
set hlsearch            " Sonuçları vurgula
set ignorecase
set smartcase           " Büyük harf varsa case-sensitive
```

---

## Dosya Açma / Kaydetme / Çıkış

```vim
:w              " Kaydet
:wq             " Kaydet + çık
:q!             " Kaydetmeden zorla çık
:w yeni.txt     " Farklı kaydet
:e dosya.txt    " Başka dosya aç
:r dosya.txt    " Dosya içeriğini ekle
:r !ls          " Komut çıktısını ekle
```

---

## Gezinme

```vim
" Temel hareket
h / j / k / l       " sol / aşağı / yukarı / sağ
0 / ^ / $           " satır başı / ilk karakter / satır sonu
w / b / e           " kelime ileri / geri / kelime sonu
gg / G              " dosya başı / dosya sonu
10G                 " 10. satıra git
Ctrl+u / Ctrl+d     " yarım sayfa yukarı / aşağı
Ctrl+b / Ctrl+f     " tam sayfa geri / ileri
H / M / L           " ekran üst / orta / alt

" Karakter arama (satır içi)
f;                  " ; karakterine git
t;                  " ; öncesine git
; / ,               " son f/t tekrar et (aynı/ters yön)
%                   " eşleşen paranteze zıpla
```

---

## Insert Mod Girişi

```vim
i       " imleç soluna ekle
I       " satır başına ekle
a       " imleç sağına ekle
A       " satır sonuna ekle
o       " alt satır aç
O       " üst satır aç
r X     " tek karakter değiştir
R       " replace modu (üzerine yaz)
```

---

## Silme / Kopyalama / Yapıştırma

```vim
" Silme (delete = cut, register'a gider)
x / X           " altındaki / solundaki karakteri sil
dw / d2w        " 1 / 2 kelime sil
d$ / d0         " satır sonu / başına kadar sil
dd / 5dd        " 1 / 5 satır sil
D               " imlecten satır sonuna sil

" Değiştirme (sil + insert moda gir)
cw / c2w        " 1 / 2 kelime değiştir
cc / C          " satır / satır sonu değiştir

" Kopyalama (yank)
yy / 5yy        " 1 / 5 satır kopyala
y$ / y0         " satır sonu / başına kopyala
yw              " kelime kopyala

" Yapıştırma
p / P           " imleçten sonra / önce yapıştır

" Indent
>> / <<         " satırı sağ / sol kaydır
>G / <G         " imlecten dosya sonuna kadar

" Tekrar
.               " son değişikliği tekrar et (çok güçlü)
u / Ctrl+r      " geri al / yeniden yap
```

---

## Arama ve Değiştirme

```vim
/pattern        " ileri ara
?pattern        " geri ara
n / N           " sonraki / önceki eşleşme
*               " imlecin altındaki kelimeyi ara

" Değiştirme (:s)
:s/eski/yeni/           " mevcut satırda ilk
:s/eski/yeni/g          " mevcut satırda tümü
:%s/eski/yeni/g         " tüm dosyada
:%s/eski/yeni/gc        " her birinde onay iste
:10,20s/foo/bar/g       " 10-20 satır arası
:%s/\<int\>/int32_t/g   " tam kelime değiştir
:%s/[ \t]\+$//          " satır sonu boşlukları sil
:nohlsearch             " arama vurgusunu kapat
```

---

## Global Komut (:g)

```vim
:g/TODO/p           " TODO içerenleri listele
:g/TODO/d           " TODO içerenleri sil
:g/^#/d             " # ile başlayanları sil
:g/^$/d             " boş satırları sil
:v/DEBUG/d          " DEBUG içermeyenleri sil
:g/^/s/$/;/         " her satır sonuna ; ekle
```

---

## Marklar ve Atlamalar

```vim
ma                  " 'a' markı koy
'a                  " 'a' markının satırına git
`a                  " 'a' markının tam pozisyonuna
''                  " önceki konuma dön (satır bazında)
```

---

## Register (Buffer) Kullanımı

```vim
"ayy            " satırı 'a' register'ına kopyala
"ap             " 'a' register'ını yapıştır
"0p             " son yank (delete'ten etkilenmez)
"+y             " sistem clipboard'a kopyala
"+p             " sistem clipboard'dan yapıştır
:reg            " tüm register'ları listele
```

---

## Makro

```vim
qa              " 'a' register'ına kayıt başlat
" ... komutları çalıştır ...
q               " kaydı bitir
@a              " 'a' makrosunu çalıştır
10@a            " 10 kez çalıştır
@@              " son makroyu tekrar et
```

---

## Pencere ve Split Yönetimi

```vim
:split          " Yatay böl
:vsplit         " Dikey böl
:split b.txt    " Altında b.txt aç
:vsplit c.txt   " Sağında c.txt aç

Ctrl+w h/j/k/l  " Pencereler arası geçiş
Ctrl+w w        " Sırayla dolaş
Ctrl+w =        " Boyutları eşitle
Ctrl+w +/-      " Yükseklik artır/azalt
:q              " Aktif pencereyi kapat
:only           " Diğer pencereleri kapat
```

---

## Buffer Yönetimi

```vim
:ls             " Buffer listesi
:badd dosya.txt " Buffer'a ekle (açmadan)
:buffer 2       " 2 numaralı buffer'a geç
:bnext / :bprev " Sonraki / önceki buffer
:bd             " Aktif buffer'ı kapat
```

---

## Tab Yönetimi

```vim
:tabnew             " Yeni sekme
:tabnew b.txt       " b.txt'yi yeni sekmede aç
:tabnext / :tabprev " Sekmeler arası
:tabclose           " Aktif sekmeyi kapat
gt / gT             " Normal modda sekme değiştir
```

---

## Dış Komut Entegrasyonu

```vim
:!make          " Shell'de make çalıştır
:!git status    " Git durumu
:r !dmesg       " Komut çıktısını dosyaya ekle
:w !sudo tee %  " Root ile kaydet
```

---

## Hızlı Başvuru Tablosu

| Eylem                  | Komut                |
| ---------------------- | -------------------- |
| Kaydet                 | `:w`                 |
| Çık                    | `:q` / `:wq` / `:q!` |
| Tüm dosyada değiştir   | `:%s/eski/yeni/g`    |
| Dosya başı / sonu      | `gg` / `G`           |
| Kelime sil             | `dw`                 |
| Satır sil              | `dd`                 |
| Geri al                | `u`                  |
| Yeniden yap            | `Ctrl+r`             |
| Ara                    | `/pattern`           |
| Son komutu tekrar et   | `.`                  |
| Makro kayıt            | `q{harf}`            |
| Makro çalıştır         | `@{harf}`            |
| Clipboard'dan yapıştır | `"+p`                |
| Clipboard'a kopyala    | `"+y`                |
