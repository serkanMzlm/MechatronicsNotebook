// ============================================================
//  C++ — Inheritance | Polymorphism | Operator Overloading | friend
//  Senaryo: Bir banka sistemi
//    • Hesap (taban)
//        ├── VadesizHesap (türetilmiş)
//        └── VadeliHesap  (türetilmiş)
//  Her kavram kendi bölümünde açıklanmıştır.
// ============================================================

#include <iostream>
#include <string>
#include <stdexcept>   // invalid_argument, runtime_error

// ╔══════════════════════════════════════════════════════════╗
//  BASE CLASS — Hesap
//  • Tüm hesap türlerinin ortak özelliklerini barındırır.
//  • virtual fonksiyonlar → polimorfizm için kapı açılır.
// ╚══════════════════════════════════════════════════════════╝
class Hesap {
protected:
    // protected: türetilmiş sınıflar doğrudan erişebilir,
    //            dış dünya erişemez.
    std::string sahibi;
    double      bakiye;
    int         hesapNo;

    inline static int sonHesapNo = 1000; // Her nesne için artan numara

public:
    // ── Kurucu ───────────────────────────────────────────────
    Hesap(const std::string& isim, double ilkBakiye)
        : sahibi(isim), bakiye(ilkBakiye), hesapNo(++sonHesapNo)
    {
        if (ilkBakiye < 0)
            throw std::invalid_argument("Başlangıç bakiyesi negatif olamaz!");
        std::cout << "[+] Hesap açıldı  #" << hesapNo
                  << " → " << sahibi << "\n";
    }

    // ── Sanal Yıkıcı (polimorfik silme için zorunlu) ─────────
    virtual ~Hesap() {
        std::cout << "[-] Hesap kapatıldı #" << hesapNo
                  << " → " << sahibi << "\n";
    }

    // ── Saf Sanal: Her alt sınıf kendi faiz hesabını yazar ───
    // pure virtual → Hesap sınıfı doğrudan örneklenemez (abstract)
    virtual double faizHesapla() const = 0;

    // ── Sanal: Alt sınıflar override edebilir ────────────────
    virtual void bilgiGoster() const {
        std::cout << "  Hesap #"  << hesapNo
                  << " | Sahibi: " << sahibi
                  << " | Bakiye: " << bakiye << " TL\n";
    }

    // ── Normal üye fonksiyonlar (override edilmez) ────────────
    void paraYatir(double miktar) {
        if (miktar <= 0) throw std::invalid_argument("Miktar pozitif olmalı!");
        bakiye += miktar;
        std::cout << "  [Yatırıldı] +" << miktar
                  << " TL  →  Yeni bakiye: " << bakiye << " TL\n";
    }

    virtual void paraCek(double miktar) {
        if (miktar <= 0) throw std::invalid_argument("Miktar pozitif olmalı!");
        if (miktar > bakiye) throw std::runtime_error("Yetersiz bakiye!");
        bakiye -= miktar;
        std::cout << "  [Çekildi]   -" << miktar
                  << " TL  →  Yeni bakiye: " << bakiye << " TL\n";
    }

    double getBakiye()  const { return bakiye; }
    int    getHesapNo() const { return hesapNo; }
    std::string getSahibi() const { return sahibi; }

    // ── friend bildirimi ──────────────────────────────────────
    // operator<< bir üye değil; ama private/protected alanlara
    // erişebilmesi için friend olarak tanımlanır.
    friend std::ostream& operator<<(std::ostream& os, const Hesap& h);

    // Hesapları birleştiren serbest fonksiyon da friend olabilir
    friend void hesaplaribirlestir(Hesap& hedef, Hesap& kaynak);
};


// ════════════════════════════════════════════════════════════
//  OPERATOR OVERLOADING — operator<<
//  friend serbest fonksiyon olduğu için sınıfın korumalı
//  alanlarına (bakiye, sahibi, hesapNo) doğrudan erişebilir.
// ════════════════════════════════════════════════════════════
std::ostream& operator<<(std::ostream& os, const Hesap& h) {
    os << "Hesap[#" << h.hesapNo    // protected alana erişim
       << " | "     << h.sahibi     // protected alana erişim
       << " | "     << h.bakiye << " TL]";
    return os;   // zincirleme: cout << h1 << h2 mümkün olur
}


// ════════════════════════════════════════════════════════════
//  friend FONKSİYON (serbest)
//  İki hesap arasında para transferi yapar.
//  Her iki sınıfın protected üyelerine doğrudan erişir.
// ════════════════════════════════════════════════════════════
void hesaplaribirlestir(Hesap& hedef, Hesap& kaynak) {
    std::cout << "\n[Transfer] #" << kaynak.hesapNo   // protected erişim
              << " → #" << hedef.hesapNo
              << "  Miktar: " << kaynak.bakiye << " TL\n";
    hedef.bakiye  += kaynak.bakiye;   // protected erişim
    kaynak.bakiye  = 0.0;
}


// ╔══════════════════════════════════════════════════════════╗
//  DERIVED CLASS 1 — VadesizHesap
//  • public inheritance: "VadesizHesap bir Hesap'tır" ilişkisi
//  • faizHesapla() override edildi (saf sanal zorunlu)
//  • paraCek()     override edildi (ek kural: limit kontrolü)
// ╚══════════════════════════════════════════════════════════╝
class VadesizHesap : public Hesap {
private:
    double cekimLimiti;   // günlük çekim tavanı

public:
    VadesizHesap(const std::string& isim, double bakiye,
                 double limit = 5000.0)
        : Hesap(isim, bakiye), cekimLimiti(limit)
    {}

    // Saf sanal override — yıllık %1 faiz
    double faizHesapla() const override {
        return bakiye * 0.01;
    }

    // paraCek override — limit kontrolü eklendi
    void paraCek(double miktar) override {
        if (miktar > cekimLimiti)
            throw std::runtime_error("Günlük çekim limitini aştınız!");
        Hesap::paraCek(miktar);   // taban sınıfın kontrollerini çalıştır
    }

    // bilgiGoster override — ek bilgi
    void bilgiGoster() const override {
        Hesap::bilgiGoster();     // önce taban davranışı
        std::cout << "  [Vadesiz] Çekim Limiti: "
                  << cekimLimiti << " TL/gün"
                  << " | Yıllık Faiz: " << faizHesapla() << " TL\n";
    }

    // ── Operator Overloading: += (aynı türden iki hesabı birleştir)
    VadesizHesap& operator+=(const VadesizHesap& diger) {
        bakiye += diger.bakiye;   // aynı sınıf → protected erişim ok
        return *this;
    }
};


// ╔══════════════════════════════════════════════════════════╗
//  DERIVED CLASS 2 — VadeliHesap
//  • Ek alan: faizOrani ve vadeAy
//  • faizHesapla() farklı formülle override edildi
// ╚══════════════════════════════════════════════════════════╝
class VadeliHesap : public Hesap {
private:
    double faizOrani;   // yıllık oran, örn. 0.30 = %30
    int    vadeAy;

public:
    VadeliHesap(const std::string& isim, double bakiye,
                double oran, int ay)
        : Hesap(isim, bakiye), faizOrani(oran), vadeAy(ay)
    {}

    // Saf sanal override — vadeye göre faiz
    double faizHesapla() const override {
        return bakiye * faizOrani * (vadeAy / 12.0);
    }

    // Vadeli hesaptan çekim yapılamaz (kural)
    void paraCek(double) override {
        throw std::runtime_error("Vadeli hesaptan erken çekim yapılamaz!");
    }

    void bilgiGoster() const override {
        Hesap::bilgiGoster();
        std::cout << "  [Vadeli]  Oran: " << faizOrani * 100 << "%"
                  << "  Vade: " << vadeAy << " ay"
                  << "  Kazanılan Faiz: " << faizHesapla() << " TL\n";
    }

    // ── Operator Overloading: < ve > (faiz karşılaştırması)
    bool operator<(const VadeliHesap& diger) const {
        return faizHesapla() < diger.faizHesapla();
    }
    bool operator>(const VadeliHesap& diger) const {
        return diger < *this;
    }
};


// ════════════════════════════════════════════════════════════
//  POLİMORFİZM gösterimi
//  Taban sınıf pointer dizisi → farklı nesneler, farklı davranış
// ════════════════════════════════════════════════════════════
void tumHesaplariGoster(Hesap* hesaplar[], int adet) {
    std::cout << "\n── Tüm Hesaplar (Polimorfik) ──────────────────────\n";
    for (int i = 0; i < adet; ++i) {
        hesaplar[i]->bilgiGoster();   // hangi override çağrılacak?
        // → Çalışma zamanında (run-time) vtable üzerinden karar verilir
    }
}


// ════════════════════════════════════════════════════════════
//  MAIN
// ════════════════════════════════════════════════════════════
int main() {
    std::cout << "════════════════════════════════════\n";
    std::cout << " BANKA SİSTEMİ — C++ OOP Örneği\n";
    std::cout << "════════════════════════════════════\n\n";

    // ── Nesneler oluştur ─────────────────────────────────────
    VadesizHesap v1("Ahmet Yılmaz", 10000.0, 3000.0);
    VadesizHesap v2("Ayşe Kaya",    5000.0);
    VadeliHesap  vd1("Mehmet Demir", 20000.0, 0.30, 6);   // %30, 6 ay
    VadeliHesap  vd2("Zeynep Arslan",15000.0, 0.35, 12);  // %35, 12 ay

    // ── Para işlemleri ───────────────────────────────────────
    std::cout << "\n── İşlemler ────────────────────────────────────────\n";
    v1.paraYatir(2000);
    v1.paraCek(1500);

    try {
        v2.paraCek(9000);   // limit aşımı → exception
    } catch (const std::exception& e) {
        std::cout << "  [HATA] " << e.what() << "\n";
    }

    try {
        vd1.paraCek(500);   // vadeli çekim → exception
    } catch (const std::exception& e) {
        std::cout << "  [HATA] " << e.what() << "\n";
    }

    // ── Operator Overloading: += ─────────────────────────────
    std::cout << "\n── operator+= (v1 += v2) ───────────────────────────\n";
    std::cout << "  Önce  v1: " << v1 << "\n";
    std::cout << "  Önce  v2: " << v2 << "\n";
    v1 += v2;   // VadesizHesap::operator+= çağrılır
    std::cout << "  Sonra v1: " << v1 << "\n";
    std::cout << "  Sonra v2: " << v2 << "\n";

    // ── Operator Overloading: < / > ──────────────────────────
    std::cout << "\n── operator< / operator> (Vadeli Hesap Karşılaştırma) ─\n";
    std::cout << "  vd1 faizi: " << vd1.faizHesapla() << " TL\n";
    std::cout << "  vd2 faizi: " << vd2.faizHesapla() << " TL\n";
    if (vd1 < vd2)
        std::cout << "  → vd2 daha çok faiz kazanıyor\n";
    else
        std::cout << "  → vd1 daha çok faiz kazanıyor\n";

    // ── operator<< (friend) ──────────────────────────────────
    std::cout << "\n── operator<< (friend) ─────────────────────────────\n";
    std::cout << "  " << v1  << "\n";
    std::cout << "  " << vd1 << "\n";

    // ── friend fonksiyon: hesaplaribirlestir ─────────────────
    std::cout << "\n── friend fonksiyon: hesaplaribirlestir ─────────────\n";
    std::cout << "  Önce  vd1: " << vd1 << "\n";
    std::cout << "  Önce  vd2: " << vd2 << "\n";
    hesaplaribirlestir(vd1, vd2);   // friend → protected alanlara erişir
    std::cout << "  Sonra vd1: " << vd1 << "\n";
    std::cout << "  Sonra vd2: " << vd2 << "\n";

    // ── Polimorfizm ──────────────────────────────────────────
    Hesap* hesaplar[] = { &v1, &v2, &vd1, &vd2 };
    tumHesaplariGoster(hesaplar, 4);
    // Derleme zamanında hangi bilgiGoster()'in çağrılacağı belli değil;
    // vtable aracılığıyla çalışma zamanında karar verilir.

    std::cout << "\n── Program sonu (yıkıcılar otomatik çalışır) ────────\n";
    return 0;
    // ← Stack'teki nesneler ters sırayla yıkılır: vd2, vd1, v2, v1
}