// basics.hpp
#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// =============================================================================
//  NAMESPACE
//  İsim çakışmalarını önlemek için kullanılan kapsam mekanizması.
//  Not: Header'da "using namespace X;" kullanma — dahil eden her dosyaya sızar.
//
//  inline namespace → versiyon kontrolü için kullanılır.
//    API::connect()     → v2 çağrılır (varsayılan)
//    API::v1::connect() → açıkça eski versiyon
// =============================================================================

namespace MathUtils {
    int add(int a, int b);
    int multiply(int a, int b);

    namespace Advanced {           // C++17 öncesi iç içe yöntem
        double power(double base, int exp);
    }
}

namespace Graphics::D2::Shapes {  // C++17 kısa sözdizimi
    void draw();
}

namespace API {
    namespace v1 {
        void connect();
    }
    inline namespace v2 {          // varsayılan; API::connect() buraya gelir
        void connect();
    }
}


// =============================================================================
//  COMMON TYPES
// =============================================================================

// typedef  → Eski C stili; şablonlarla ÇALIŞMAZ.
// using    → Modern C++11; şablon takma adı için ZORUNLUDUR.
typedef unsigned long long ULL;
using uint64 = unsigned long long;

template<typename T>
using Vec = std::vector<T>;

// enum       → Değerler global scope'a sızar, tip güvenli DEĞİLDİR.
// enum class → Kapsüllü, tip güvenli. Mülakatta her zaman tercih et.
enum Color { RED, GREEN, BLUE };
enum class Direction { NORTH, SOUTH, EAST, WEST };

// union: Tüm alanlar AYNI bellek adresini paylaşır.
// Aynı anda yalnızca bir alan geçerlidir.
union DataHolder {
    int   intVal;
    float floatVal;
    char  charVal;
};

// struct: Üyeleri varsayılan olarak public'tir. Saf veri için tercih edilir.
struct Point {
    double x;
    double y;
};

// const < constexpr < consteval  (derleme zamanı garanti seviyesi artar)
inline constexpr int MAX_SIZE    = 100;
inline constexpr int BUFFER_SIZE = 256;

constexpr int square(int x) { return x * x; }      // derleme veya çalışma zamanında
consteval int cube(int x)   { return x * x * x; }  // YALNIZCA derleme zamanında


// =============================================================================
//  CLASS
//  struct ile tek farkı: üyeleri varsayılan olarak private'dır.
// =============================================================================

class Basics {
public:
    // explicit: tek parametreli ctor'larda örtülü dönüşümü engeller.
    // Basics b = "hello";  → HATA
    // Basics b("hello");   → OK
    explicit Basics(std::string_view name) : name_(name) {
        ++instance_count_;
    }

    // = default: derleyici copy ctor'ı kendisi üretsin
    Basics(const Basics& other) = default;

    // = delete: copy assignment tamamen yasaklandı
    // b = other;  → derleme hatası
    Basics& operator=(const Basics& other) = delete;

    // = default: move ctor derleyici üretsin
    Basics(Basics&& other) noexcept = default;

    // = delete: move assignment yasaklandı
    // b = std::move(other);  → derleme hatası
    Basics& operator=(Basics&& other) noexcept = delete;

    ~Basics() = default;

    // const üye fonksiyon: nesneyi değiştirmez garantisi verir
    void print() const {
        std::cout << "Basics { name=" << name_ << " }\n";
    }

    // static: nesneye değil, SINIFA aittir. Tüm nesneler paylaşır.
    static int getInstanceCount() { return instance_count_; }

    void runVariables() const;
    void runNamespaces() const;
    void runCollections() const;
    void runIO() const;

private:
    std::string name_;
    static int  instance_count_;
};