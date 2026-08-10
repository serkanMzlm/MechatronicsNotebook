# Algoritmalar

!!! note "Genel Bakış"
    Bu sayfa algoritmaların **nasıl çalıştığını** ve **neden tercih edildiğini** açıklar. Kod değil mantık odaklıdır: her algoritmanın arka planını, zaman/mekan karmaşıklığını ve hangi problem tipine uygun olduğunu anlayabilmek için kaleme alınmıştır.

!!! info "Optical Flow ve VIO"
    Optical Flow, VIO (Visual Inertial Odometry), OpenVINS ve Kalibr konuları ayrı bir sayfaya taşındı: **[VIO ve Kalibrasyon](../robotics/vio.md)**

---

## Graf Algoritmaları

### BFS - Genişlik Öncelikli Arama

**Mantık:** Başlangıç düğümünden katman katman dışa doğru genişler. Önce tüm komşular ziyaret edilir, sonra komşuların komşuları. **Kuyruk (FIFO)** veri yapısıyla çalışır.

```mermaid
graph TD
    A((A)) --> B((B))
    A --> C((C))
    B --> D((D))
    B --> E((E))
    C --> F((F))

    style A fill:#FFD700
    style B fill:#90EE90
    style C fill:#90EE90
    style D fill:#87CEEB
    style E fill:#87CEEB
    style F fill:#87CEEB
```

Ziyaret sırası: A → B → C → D → E → F (seviye seviye)

| Özellik                  | Değer                      |
| ------------------------ | -------------------------- |
| Zaman Karmaşıklığı       | O(V + E)                   |
| Alan Karmaşıklığı        | O(V) - kuyruktaki düğümler |
| En kısa yol (ağırlıksız) | ✓ Garanti                  |
| Döngü tespiti            | ✓                          |

**Ne zaman kullanılır?**
- Ağırlıksız grafta iki düğüm arasındaki en kısa yol
- Katman/seviye bazlı analiz (sosyal ağda 2. derece arkadaşlar)
- Web crawler'lar (bağlantı seviyesine göre tara)

---

### DFS - Derinlik Öncelikli Arama

**Mantık:** Bir yolu sonuna kadar takip eder, çıkmaz sokağa gelince geri döner ve başka bir yol dener. **Yığın (LIFO)** veya özyineleme ile çalışır.

```mermaid
graph TD
    A((A)) --> B((B))
    A --> C((C))
    B --> D((D))
    B --> E((E))
    C --> F((F))
```

Ziyaret sırası: A → B → D → E → C → F (dala gir, bitince geri dön)

| Özellik            | Değer                    |
| ------------------ | ------------------------ |
| Zaman Karmaşıklığı | O(V + E)                 |
| Alan Karmaşıklığı  | O(V) - özyineleme yığını |
| En kısa yol        | ✗ Garanti etmez          |
| Döngü tespiti      | ✓ (geri kenar tespiti)   |

**Ne zaman kullanılır?**
- Topolojik sıralama (bağımlılık çözümü - paket yöneticileri)
- Bağlantılı bileşen bulma
- Labirent çözme
- Döngü tespiti

!!! note "BFS vs DFS Seçim Kuralı"
    Hedefe **en kısa yol** arıyorsan → **BFS**.  
    Tüm olasılıkları **derinlemesine keşfet** veya **topolojik sıralama** gerekiyorsa → **DFS**.

---

### Dijkstra

**Mantık:** Negatif olmayan ağırlıklı grafta tek kaynaktan en kısa yolları bulur. Her adımda henüz işlenmemiş düğümler arasından **en düşük maliyetli** olanı seçer (greedy). Öncelik kuyruğu (min-heap) kullanır.

```mermaid
graph LR
    A((A)) -->|4| B((B))
    A -->|1| C((C))
    C -->|2| B
    B -->|5| D((D))
    C -->|8| D
```

A'dan D'ye en kısa yol: A→C→B→D (1+2+5=8), A→C→D değil (1+8=9)

**Çalışma prensibi:**
1. Başlangıç düğümü mesafe=0, diğerleri sonsuz
2. Min-heap'ten en küçük mesafeli düğümü çıkar
3. Komşuların mesafelerini güncelle (relaxation)
4. Heap'e tekrar ekle
5. Tüm düğümler işlenene kadar devam et

| Özellik            | Değer                           |
| ------------------ | ------------------------------- |
| Zaman Karmaşıklığı | O((V + E) log V) - min-heap ile |
| Negatif kenar      | ✗ Desteklemez                   |
| Negatif döngü      | ✗ Tespit edemez                 |

**Ne zaman kullanılır?**
- GPS yol bulma (haritalar)
- Ağ yönlendirme (OSPF protokolü)
- Robotik yol planlaması (maliyet haritaları)

---

### Bellman-Ford

**Mantık:** Dijkstra'nın yapamadığı şeyi yapar: **negatif ağırlıklı kenarları** işler ve **negatif döngü** tespit eder. Her kenarı V-1 kez gevşetir (relaxation). Dinamik programlama yaklaşımı.

**Çalışma prensibi:**
1. Başlangıç mesafe=0, diğerleri sonsuz
2. Tüm kenarlar üzerinden V-1 kez geçerek mesafeleri güncelle
3. V. geçişte hâlâ güncelleme olursa → negatif döngü var

| Özellik                | Değer             |
| ---------------------- | ----------------- |
| Zaman Karmaşıklığı     | O(V · E)          |
| Negatif kenar          | ✓                 |
| Negatif döngü tespiti  | ✓                 |
| Dijkstra'dan yavaş mı? | Evet (genellikle) |

**Ne zaman kullanılır?**
- Negatif ağırlıklar içeren graflarda
- Finans: arbitraj fırsatı tespiti (negatif döngü = para kazanma döngüsü)
- Ağ protokollerinde (RIP - Routing Information Protocol)

---

### Floyd-Warshall

**Mantık:** Grafttaki **tüm çiftler arasındaki** en kısa yolları tek seferde hesaplar. Her düğümü potansiyel ara nokta olarak dener. Dinamik programlama.

**Temel fikir:** `dist[i][j]` = i'den j'ye en kısa yol. k. düğümü ara nokta olarak ekleyerek güncelle:

```
dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```

| Özellik               | Değer                   |
| --------------------- | ----------------------- |
| Zaman Karmaşıklığı    | O(V³)                   |
| Alan Karmaşıklığı     | O(V²) - matris          |
| Negatif kenar         | ✓                       |
| Negatif döngü tespiti | ✓ (köşegen eksi olursa) |
| Tek kaynak mı?        | ✗ Tüm çiftler           |

**Ne zaman kullanılır?**
- Graf küçük ve tüm çiftler arası mesafe gerekiyorsa
- Ağ gecikmesi matrisi hesaplama
- Erişilebilirlik analizi (hangi düğümler birbirine ulaşabilir?)

---

### A* (A-Star)

**Mantık:** Dijkstra + heuristik (tahmini mesafe). Her düğüm için gerçek maliyet `g(n)` ile hedefe tahmini mesafe `h(n)` toplanarak `f(n) = g(n) + h(n)` hesaplanır. Heuristik akıllıca yönlendirme sağlar.

```mermaid
graph LR
    subgraph DIJKSTRA["Dijkstra - Her yöne eşit arar"]
        D1((Start)) --> D2((...))
        D2 --> D3((...))
        D2 --> D4((...))
        D3 --> D5((Goal))
        D4 --> D6((...))
    end
    subgraph ASTAR["A* - Hedefe doğru yönelir"]
        A1((Start)) --> A2((...))
        A2 --> A3((Goal))
    end
```

**Heuristik Kriterleri:**
- **Kabul edilebilir (admissible):** Gerçek maliyeti hiçbir zaman aşmaz → A* optimal çözüm garantisi verir
- Yaygın heuristikler: Manhattan mesafesi (ızgara), Öklid mesafesi (açık alan)

| Özellik   |  Dijkstra  |             A*             |
| --------- | :--------: | :------------------------: |
| Optimal   |     ✓      | ✓ (kabul edilebilir h ile) |
| Hız       |    Orta    |  Daha hızlı (hedef yönlü)  |
| Heuristik |     ✗      |             ✓              |
| Kullanım  | Genel graf |     Uzamsal yol bulma      |

**Ne zaman kullanılır?**
- Robot navigasyonu (ROS Nav2 / costmap)
- Oyun yapay zekası (karakter yol bulma)
- GPS navigasyonu (coğrafi heuristik ile)

!!! tip "Kısa Karşılaştırma"
    | Algoritma      | Negatif Kenar  | Tüm Çiftler | Heuristik | Karmaşıklık  |
    | -------------- | :------------: | :---------: | :-------: | :----------: |
    | BFS            | ✗ (ağırlıksız) |      ✗      |     ✗     |    O(V+E)    |
    | Dijkstra       |       ✗        |      ✗      |     ✗     | O((V+E)logV) |
    | Bellman-Ford   |       ✓        |      ✗      |     ✗     |    O(VE)     |
    | Floyd-Warshall |       ✓        |      ✓      |     ✗     |    O(V³)     |
    | A*             |       ✗        |      ✗      |     ✓     |  O(E log V)  |

---

## Sıralama Algoritmaları

### Bubble Sort - Kabarcık Sıralaması

**Mantık:** Komşu iki elemanı karşılaştırır, yanlış sıradaysa yer değiştirir. Her geçişte en büyük eleman sona "kabarcık gibi" yükselir. Saf kuvvet yöntemi.

```
[5, 3, 8, 1] → [3, 5, 1, 8] → [3, 1, 5, 8] → [1, 3, 5, 8]
```

|                    | Değer                                    |
| ------------------ | ---------------------------------------- |
| En iyi             | O(n) - zaten sıralıysa (erken çıkış ile) |
| Ortalama / En kötü | O(n²)                                    |
| Bellek             | O(1) - yerinde                           |
| Kararlı (Stable)   | ✓                                        |

**Kullanım:** Eğitim amaçlı. Gerçek sistemlerde kullanılmaz.

---

### Selection Sort - Seçme Sıralaması

**Mantık:** Her geçişte dizinin sıralanmamış kısmından **en küçük elemanı** bulur ve başa taşır. Bubble'dan farkı: swap sayısı azdır (en fazla n-1 swap).

```
[5, 3, 8, 1] → min=1 → [1, 3, 8, 5] → min=3 → [1, 3, 8, 5] → ...
```

|             | Değer                   |
| ----------- | ----------------------- |
| Her durumda | O(n²) - erken çıkış yok |
| Bellek      | O(1)                    |
| Kararlı     | ✗                       |

**Kullanım:** Bellek yazma işlemi pahalıysa (flash bellek gibi) - swap sayısı minimumdur.

---

### Insertion Sort - Ekleme Sıralaması

**Mantık:** Kart oyunundaki gibi - sıralanmış kısma yeni elemanı doğru yerine ekler. Sıralı kısım solda büyür.

```
[5, 3, 8, 1]
→ [3, 5, 8, 1]   (3'ü doğru yere koy)
→ [3, 5, 8, 1]   (8 zaten doğru yerde)
→ [1, 3, 5, 8]   (1'i en başa ekle)
```

|         | Değer                         |
| ------- | ----------------------------- |
| En iyi  | O(n) - neredeyse sıralıysa    |
| En kötü | O(n²)                         |
| Bellek  | O(1)                          |
| Kararlı | ✓                             |
| Online  | ✓ Veri geldikçe sıralayabilir |

**Kullanım:** Küçük veri (n < 20) veya neredeyse sıralı veri için pratik. Birçok hibrit algoritma (TimSort) küçük parçalarda insertion sort kullanır.

---

### Merge Sort - Birleştirme Sıralaması

**Mantık:** Böl-ve-yönet. Diziyi ikiye böler, her yarıyı özyinelemeli olarak sıralar, sonra iki sıralı yarıyı birleştirir.

```mermaid
graph TD
    A["[5, 3, 8, 1]"] --> B["[5, 3]"]
    A --> C["[8, 1]"]
    B --> D["[5]"]
    B --> E["[3]"]
    C --> F["[8]"]
    C --> G["[1]"]
    D & E --> H["[3, 5]"]
    F & G --> I["[1, 8]"]
    H & I --> J["[1, 3, 5, 8]"]
```

|             | Değer                     |
| ----------- | ------------------------- |
| Her durumda | O(n log n)                |
| Bellek      | O(n) - ek dizi gerektirir |
| Kararlı     | ✓                         |

**Kullanım:** Bağlı listeler için ideal (eleman kopyalamadan birleştirilir). Büyük veri kümelerinde garantili O(n log n). Dış sıralama (disk üzerindeki veriler).

---

### Quick Sort - Hızlı Sıralama

**Mantık:** Bir **pivot** seçer, pivot'tan küçükleri sola, büyükleri sağa taşır (partition). Sonra her iki tarafı özyinelemeli sıralar.

```
[5, 3, 8, 1]  → pivot=5
→ [3, 1 | 5 | 8]   (soldakiler < 5, sağdakiler > 5)
→ [1, 3 | 5 | 8]   (sol kısmı sırala)
→ [1, 3, 5, 8]
```

|          | Değer                                         |
| -------- | --------------------------------------------- |
| Ortalama | O(n log n)                                    |
| En kötü  | O(n²) - pivot her seferinde min/max seçilirse |
| Bellek   | O(log n) - özyineleme yığını                  |
| Kararlı  | ✗                                             |

**Neden hızlı?** Bellek erişim örüntüsü cache'e dosttur (in-place, yerel erişim). Merge sort'un aksine ek bellek gerektirmez.

**Pivot seçimi kritik:** Rastgele pivot veya "median of three" yöntemi en kötü durum olasılığını azaltır.

**Kullanım:** Genel amaçlı sıralama için pratikte en hızlı. C++'ın `std::sort`, Java'nın `Arrays.sort(int[])` temelde introsort kullanır - quicksort bazlı.

---

### Heap Sort - Yığın Sıralaması

**Mantık:** Önce diziyi **max-heap** (her ebeveyn çocuklarından büyük) yapısına dönüştürür. Ardından heap'in kökünü (maksimum) sondan başa yerleştirir ve heap'i yeniden düzenler.

```
Heap oluştur: [5, 3, 8, 1] → [8, 3, 5, 1]
Max çıkar: [8] → [1, 3, 5] → heapify → [5, 3, 1]
Max çıkar: [8, 5] → [1, 3] → [3, 1]
...
Sonuç: [1, 3, 5, 8]
```

|             | Değer          |
| ----------- | -------------- |
| Her durumda | O(n log n)     |
| Bellek      | O(1) - yerinde |
| Kararlı     | ✗              |

**Quicksort ile farkı:** En kötü durumda bile O(n log n) garantisi verir, yerinde çalışır. Ancak cache performansı quicksort'tan zayıf (heap erişimi sıçramalı).

**Kullanım:** Bellek kısıtlı ve worst-case garantisi gereken durumlar.

---

### Karşılaştırma Tablosu

| Algoritma      |   En İyi   |    Ortalama    |  En Kötü   |  Bellek  | Kararlı |
| -------------- | :--------: | :------------: | :--------: | :------: | :-----: |
| Bubble Sort    |    O(n)    |     O(n²)      |   O(n²)    |   O(1)   |    ✓    |
| Selection Sort |   O(n²)    |     O(n²)      |   O(n²)    |   O(1)   |    ✗    |
| Insertion Sort |  **O(n)**  |     O(n²)      |   O(n²)    |   O(1)   |    ✓    |
| Merge Sort     | O(n log n) |   O(n log n)   | O(n log n) |   O(n)   |    ✓    |
| Quick Sort     | O(n log n) | **O(n log n)** |   O(n²)    | O(log n) |    ✗    |
| Heap Sort      | O(n log n) |   O(n log n)   | O(n log n) | **O(1)** |    ✗    |

!!! tip "Pratikte Ne Kullanılır?"
    - Küçük veri (n < 20) → **Insertion Sort**
    - Genel amaç → **Quick Sort** (stdlib default)
    - Kararlılık şart → **Merge Sort** veya **TimSort** (Python default)
    - Bellek kısıtlı + worst-case garanti → **Heap Sort**
    - Neredeyse sıralı → **Insertion Sort**

---

## Arama Algoritmaları

### Linear Search - Doğrusal Arama

**Mantık:** Dizinin başından sonuna kadar teker teker her elemana bakılır. Bulunan ilk eşleşmede durur.

| Özellik                | Değer                    |
| ---------------------- | ------------------------ |
| Zaman Karmaşıklığı     | O(n)                     |
| Sıralanmış gerekir mi? | ✗                        |
| Kullanım               | Küçük / sırasız listeler |

**Ne zaman?** Veri küçükse, sıralanmamışsa veya yalnızca bir kez aranacaksa. Sıralama maliyeti aramanın faydası aştığında tercih edilir.

---

### Binary Search - İkili Arama

**Mantık:** **Sıralanmış** dizide orta elemana bakılır. Aranan değer ortadan küçükse sol yarıya, büyükse sağ yarıya odaklanılır. Her adımda arama alanı **ikiye bölünür**.

```
Dizi: [1, 3, 5, 8, 12, 17, 23]  Aranan: 12

Adım 1: mid=8  → 12 > 8, sağa bak  →  [12, 17, 23]
Adım 2: mid=17 → 12 < 17, sola bak →  [12]
Adım 3: mid=12 → Bulundu!
```

| Özellik                | Değer                |
| ---------------------- | -------------------- |
| Zaman Karmaşıklığı     | O(log n)             |
| Sıralanmış gerekir mi? | ✓ Şart               |
| Kullanım               | Büyük sıralı diziler |

**Kritik nokta:** Sıralama maliyeti O(n log n), arama O(log n). Eğer veri nadiren değişiyor ve sık aranıyorsa sıralama maliyeti amortize edilir.

**Ne zaman?** Veritabanı indeksleri, kelime sözlükleri, sıralı sensor değerleri içinde eşik bulma.
