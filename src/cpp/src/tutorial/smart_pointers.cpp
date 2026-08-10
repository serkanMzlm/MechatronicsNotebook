// ============================================================
//  C++ SMART POINTERS - Öğretici Örnek
//  Konular: unique_ptr | shared_ptr | weak_ptr
//  Not: #include <memory> başlığı gereklidir.
// ============================================================

#include <iostream>
#include <memory>    // unique_ptr, shared_ptr, weak_ptr
#include <string>

// ── Örnek sınıf ──────────────────────────────────────────────
class Araba {
public:
    std::string marka;

    explicit Araba(const std::string& m) : marka(m) {
        std::cout << "[+] Araba oluşturuldu: " << marka << "\n";
    }
    ~Araba() {
        // Yıkıcı çağrıldığında bellek serbest bırakılıyor
        std::cout << "[-] Araba yıkıldı: " << marka << "\n";
    }
    void sur() const {
        std::cout << marka << " sürülüyor!\n";
    }
};


// ════════════════════════════════════════════════════════════
//  1) unique_ptr
//  • Belleğin TEK sahibidir; kopyalanamaz, yalnızca taşınabilir.
//  • Scope (kapsam) dışına çıkınca nesneyi otomatik siler.
//  • Ham pointer'a (raw pointer) tercih edilmesi gereken durum:
//    tek sahiplik yeterliyse.
// ════════════════════════════════════════════════════════════
void uniquePtrOrnegi() {
    std::cout << "\n=== unique_ptr ===\n";

    // make_unique ile oluşturma (C++14, önerilen yol)
    std::unique_ptr<Araba> araba1 = std::make_unique<Araba>("Toyota");

    araba1->sur();                   // ok: normal kullanım
    std::cout << "Sahip? " << (araba1 ? "Evet" : "Hayır") << "\n";

    // ① Sahipliği TAŞIMA (move): artık araba2 sahibi, araba1 nullptr
    std::unique_ptr<Araba> araba2 = std::move(araba1);
    std::cout << "Taşıma sonrası araba1 sahip? " << (araba1 ? "Evet" : "Hayır") << "\n";
    araba2->sur();

    // ② Erken serbest bırakma
    araba2.reset();                  // Araba yıkıcısı burada çağrılır
    std::cout << "reset() sonrası araba2 sahip? " << (araba2 ? "Evet" : "Hayır") << "\n";

    // ③ Scope bitince kalan unique_ptr'ler otomatik temizlenir
    {
        auto geciciAraba = std::make_unique<Araba>("Honda");
        geciciAraba->sur();
    } // ← Scope dışı: Honda burada silinir
    std::cout << "Scope bitti, Honda silindi\n";
}


// ════════════════════════════════════════════════════════════
//  2) shared_ptr
//  • Belleği birden fazla pointer paylaşabilir.
//  • İçeride bir REFERANS SAYACI tutar; sayaç 0 olunca nesne silinir.
//  • Kopyalama yapıldığında sayaç artar; pointer yok olunca azalır.
// ════════════════════════════════════════════════════════════
void sharedPtrOrnegi() {
    std::cout << "\n=== shared_ptr ===\n";

    // make_shared ile oluşturma (tek bellek tahsisi, daha verimli)
    std::shared_ptr<Araba> paylasan1 = std::make_shared<Araba>("BMW");
    std::cout << "Referans sayısı: " << paylasan1.use_count() << "\n";  // 1

    {
        // Kopyalama → sahiplik paylaşılır, sayaç artar
        std::shared_ptr<Araba> paylasan2 = paylasan1;
        std::cout << "paylasan2 oluşturuldu, sayaç: " << paylasan1.use_count() << "\n"; // 2

        std::shared_ptr<Araba> paylasan3 = paylasan1;
        std::cout << "paylasan3 oluşturuldu, sayaç: " << paylasan1.use_count() << "\n"; // 3

        paylasan2->sur();
        paylasan3->sur();
    } // ← paylasan2 ve paylasan3 yok oldu → sayaç 1'e düştü

    std::cout << "Scope sonrası sayaç: " << paylasan1.use_count() << "\n";  // 1
    // paylasan1 da yok olunca sayaç 0 → BMW silinecek
}


// ════════════════════════════════════════════════════════════
//  3) weak_ptr
//  • shared_ptr'e ZAYIF referanstır; referans sayacını artırmaz.
//  • Nesnenin hâlâ var olup olmadığını kontrol edebiliriz.
//  • Döngüsel referansları (circular reference) kırmak için kullanılır.
//  • Kullanmadan önce lock() ile geçici shared_ptr alınmalıdır.
// ════════════════════════════════════════════════════════════
void weakPtrOrnegi() {
    std::cout << "\n=== weak_ptr ===\n";

    std::weak_ptr<Araba> zayifRef;   // henüz hiçbir şeye bağlı değil

    {
        auto guclu = std::make_shared<Araba>("Mercedes");
        zayifRef = guclu;            // zayif_ref bağlandı ama sayacı artırmadı

        std::cout << "Scope içi – sayaç: " << guclu.use_count() << "\n";     // 1 (weak saymaz)
        std::cout << "Nesne var mı? " << (zayifRef.expired() ? "Hayır" : "Evet") << "\n";

        // Nesneye erişmek için lock() ile geçici shared_ptr al
        if (auto gecici = zayifRef.lock()) {
            gecici->sur();           // güvenli erişim
        }
    } // ← guclu yok oldu, sayaç 0 → Mercedes silindi

    // Scope dışında nesne artık yok
    std::cout << "Scope sonrası – nesne var mı? "
              << (zayifRef.expired() ? "Hayır (silindi)" : "Evet") << "\n";

    // lock() artık nullptr döner; dereference etmek tanımsız davranış olurdu
    if (auto gecici = zayifRef.lock()) {
        gecici->sur();               // bu satıra girilmez
    } else {
        std::cout << "lock() başarısız: nesne zaten silinmiş\n";
    }
}


// ════════════════════════════════════════════════════════════
//  4) Döngüsel Referans Problemi ve weak_ptr Çözümü
//  Problem: A → B ve B → A shared_ptr tutarsa hiçbiri silinmez!
// ════════════════════════════════════════════════════════════
struct Dugum {
    std::string ad;
    std::shared_ptr<Dugum> sonraki;   // ← bunu weak_ptr yaparsak sorun çözülür
    // std::weak_ptr<Dugum> sonraki;  // doğru çözüm

    explicit Dugum(const std::string& a) : ad(a) {
        std::cout << "[+] Düğüm oluşturuldu: " << ad << "\n";
    }
    ~Dugum() {
        std::cout << "[-] Düğüm yıkıldı: " << ad << "\n";
    }
};

void dongüselReferansProblemi() {
    std::cout << "\n=== Döngüsel Referans (shared_ptr SORUNU) ===\n";

    auto a = std::make_shared<Dugum>("A");
    auto b = std::make_shared<Dugum>("B");

    a->sonraki = b;   // A → B
    b->sonraki = a;   // B → A  ← döngü oluştu!

    // Scope bitse de A ve B hiç silinmeyecek → bellek sızıntısı!
    // Çözüm: Dugum::sonraki'yi weak_ptr<Dugum> yapmak.
    std::cout << "DİKKAT: A ve B yıkılmayacak (bellek sızıntısı)!\n";
    std::cout << "a sayaç: " << a.use_count()
              << "  b sayaç: " << b.use_count() << "\n";
}


// ════════════════════════════════════════════════════════════
//  ÖZET TABLOSU
// ════════════════════════════════════════════════════════════
//  Pointer        Sahiplik      Kopya   Sayaç   Tipik Kullanım
//  ──────────────────────────────────────────────────────────
//  unique_ptr     Tek           Hayır   Yok     Tek sahiplik, fabrika
//  shared_ptr     Çoklu         Evet    Var     Paylaşımlı kaynak
//  weak_ptr       Yok (zayıf)   Evet    Yok     Döngü kırma, gözlemleme

int main() {
    uniquePtrOrnegi();
    sharedPtrOrnegi();
    weakPtrOrnegi();
    dongüselReferansProblemi();

    std::cout << "\nTüm örnekler tamamlandı.\n";
    return 0;
}