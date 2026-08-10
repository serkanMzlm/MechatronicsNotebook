// ============================================================
//  C++ TYPE CASTING - Öğretici Örnek
//  Konular: static_cast | dynamic_cast | const_cast | reinterpret_cast
// ============================================================

#include <iostream>
#include <cstdint>   // uintptr_t için

// ── Polimorfizm için örnek sınıf hiyerarşisi ─────────────────
class Hayvan {
public:
    virtual void sesCikar() const {
        std::cout << "[Hayvan] Ses çıkarıyor...\n";
    }
    virtual ~Hayvan() = default; // dynamic_cast için sanal yıkıcı ZORUNLU
};

class Kopek : public Hayvan {
public:
    void sesCikar() const override {
        std::cout << "[Köpek] Hav hav!\n";
    }
    void getir() const {
        std::cout << "[Köpek] Top getiriyor!\n";
    }
};

class Kedi : public Hayvan {
public:
    void sesCikar() const override {
        std::cout << "[Kedi] Miyav!\n";
    }
};


// ════════════════════════════════════════════════════════════
//  1) static_cast
//  • Derleme zamanında kontrol edilir (compile-time).
//  • İlişkili türler arasında güvenli dönüşüm yapar.
//  • Hatalı kullanımı tanımsız davranışa yol açabilir.
// ════════════════════════════════════════════════════════════
void staticCastOrnegi() {
    std::cout << "\n=== static_cast ===\n";

    // ① Sayısal tür dönüşümü (double → int, kesir kaybolur)
    double pi = 3.14159;
    int tamSayi = static_cast<int>(pi);   // 3
    std::cout << "double->int : " << pi << " → " << tamSayi << "\n";

    // ② Tür genişletme (int → double, güvenli)
    int a = 7, b = 2;
    double bolum = static_cast<double>(a) / b;  // 3.5 (int bölmesi değil!)
    std::cout << "int->double : 7/2 = " << bolum << "\n";

    // ③ Türetilmiş → Taban sınıf (Upcast, her zaman güvenli)
    Kopek kopek;
    Hayvan* hayvanPtr = static_cast<Hayvan*>(&kopek);
    hayvanPtr->sesCikar();   // Kopek::sesCikar çağrılır (virtual)

    // ④ Taban → Türetilmiş (Downcast, DİKKAT: güvensiz olabilir)
    Kopek* kopekPtr = static_cast<Kopek*>(hayvanPtr);
    kopekPtr->getir();       // Tamam; hayvanPtr zaten bir Kopek gösteriyordu
}


// ════════════════════════════════════════════════════════════
//  2) dynamic_cast
//  • Çalışma zamanında kontrol edilir (run-time / RTTI).
//  • Polimorfik sınıflarda GÜVENLİ downcast için kullanılır.
//  • Pointer dönüşümünde başarısız olursa nullptr döner.
//  • Reference dönüşümünde başarısız olursa std::bad_cast fırlatır.
// ════════════════════════════════════════════════════════════
void dynamicCastOrnegi() {
    std::cout << "\n=== dynamic_cast ===\n";

    Hayvan* h1 = new Kopek();
    Hayvan* h2 = new Kedi();

    // ① Başarılı downcast: h1 gerçekte bir Kopek
    Kopek* k = dynamic_cast<Kopek*>(h1);
    if (k) {
        std::cout << "Başarılı: Köpek bulundu → ";
        k->getir();
    }

    // ② Başarısız downcast: h2 bir Kedi, Kopek'e cast edilemez → nullptr
    Kopek* k2 = dynamic_cast<Kopek*>(h2);
    if (!k2) {
        std::cout << "Başarısız: h2 Köpek değil, nullptr döndü\n";
    }

    delete h1;
    delete h2;
}


// ════════════════════════════════════════════════════════════
//  3) const_cast
//  • Bir nesnenin const/volatile niteleyicisini kaldırır.
//  • Yalnızca orijinal nesne const DEĞİLSE değişiklik güvenlidir.
//  • Gerçekten const olan bir nesneyi değiştirmek tanımsız davranıştır!
// ════════════════════════════════════════════════════════════
void constCastOrnegi() {
    std::cout << "\n=== const_cast ===\n";

    // ① Kullanım senaryosu: const pointer alan eski bir API'ye
    //   non-const veri göndermek
    int deger = 42;                         // orijinal: const DEĞİL
    const int* cPtr = &deger;               // const pointer aldık

    // cPtr üzerinden yazamayız, önce const'u kaldırıyoruz:
    int* yazilabilirPtr = const_cast<int*>(cPtr);
    *yazilabilirPtr = 100;                  // GÜVENLİ: deger zaten non-const

    std::cout << "deger (const_cast sonrası): " << deger << "\n";  // 100

    // ② YANLIŞ kullanım (gösterim amaçlı, çalıştırmayın!):
    // const int sabit = 5;
    // int* hata = const_cast<int*>(&sabit);
    // *hata = 10;  // TANIMSIZ DAVRANIŞ - sabit gerçekten const!
    std::cout << "Uyarı: Gerçek const nesneyi değiştirmek tanımsız davranıştır!\n";
}


// ════════════════════════════════════════════════════════════
//  4) reinterpret_cast
//  • Bit örüntüsünü yeniden yorumlar; tür sistemi dışına çıkar.
//  • En tehlikeli cast; taşınabilirlik garantisi vermez.
//  • Genellikle düşük seviyeli / sistem programlamada kullanılır.
// ════════════════════════════════════════════════════════════
void reinterpretCastOrnegi() {
    std::cout << "\n=== reinterpret_cast ===\n";

    // ① Pointer → tam sayı (adres değerini görmek için)
    int x = 255;
    uintptr_t adres = reinterpret_cast<uintptr_t>(&x);
    std::cout << "x'in bellek adresi (int): " << adres << "\n";

    // ② Tam sayı → pointer (ham bellekle çalışmak için)
    //   (Gerçek sistemlerde belirli donanım adresleri için yapılır)
    int* geriPtr = reinterpret_cast<int*>(adres);
    std::cout << "geriPtr'den okunan değer: " << *geriPtr << "\n";  // 255

    // ③ İlişkisiz pointer türleri arasında (düşük seviye senaryo)
    int sayi = 0x41424344;   // 'DCBA' karakterleri
    char* cPtr = reinterpret_cast<char*>(&sayi);
    // Little-endian sistemde ilk byte 0x44 = 'D'
    std::cout << "İlk byte char olarak: " << *cPtr << "\n";
}


// ════════════════════════════════════════════════════════════
//  ÖZET TABLOSU
// ════════════════════════════════════════════════════════════
//  Cast              Kontrol Zamanı   Güvenlik   Tipik Kullanım
//  ─────────────────────────────────────────────────────────────
//  static_cast       Derleme          Orta       Sayısal dönüşüm, upcast
//  dynamic_cast      Çalışma (RTTI)   Yüksek     Polimorfik downcast
//  const_cast        Derleme          Düşük      const kaldırma
//  reinterpret_cast  Yok              Çok düşük  Bellek/donanım işlemleri

int main() {
    staticCastOrnegi();
    dynamicCastOrnegi();
    constCastOrnegi();
    reinterpretCastOrnegi();

    std::cout << "\nTüm örnekler tamamlandı.\n";
    return 0;
}