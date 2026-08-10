// basics.cpp
#include "basics.hpp"

// static üye tanımı sınıf dışında yapılır
int Basics::instance_count_ = 0;

namespace MathUtils {
    int add(int a, int b)      { return a + b; }
    int multiply(int a, int b) { return a * b; }

    namespace Advanced {
        double power(double base, int exp) {
            double result = 1.0;
            for (int i = 0; i < exp; ++i) result *= base;
            return result;
        }
    }
}

namespace Graphics::D2::Shapes {
    void draw() { std::cout << "Drawing 2D Shape\n"; }
}

namespace API {
    namespace v1 { void connect() { std::cout << "API v1 connect\n"; } }
    namespace v2 { void connect() { std::cout << "API v2 connect (default)\n"; } }
}

void Basics::runVariables() const {
    std::cout << "--- Variables ---\n";

    int    a = 10;    // copy initialization
    int    b(20);     // direct initialization
    int    c{30};     // direct list initialization — narrowing engeller

    char   ch   = 'A';
    bool   flag  = true;
    double pi    = 3.14159;
    [[maybe_unused]] float f = 2.71f;

    std::cout << "int: " << a << ", " << b << ", " << c << "\n";
    std::cout << "char: " << ch << ", bool: " << flag << ", double: " << pi << "\n";

    // std::string  → heap'te kopya, değiştirilebilir
    // std::string_view → kopyasız pencere, kaynak yaşadığı sürece geçerli
    std::string      str  = "Hello, World!";
    std::string_view view = str;
    std::cout << "string: " << str << ", view: " << view << "\n";

    // union: aynı anda yalnızca bir alan geçerli
    DataHolder dh;
    dh.intVal = 42;
    std::cout << "union int: " << dh.intVal << "\n";
    dh.floatVal = 3.14f;
    std::cout << "union float: " << dh.floatVal << "\n";

    // enum class: :: zorunlu, tip güvenli
    Color     c1 = RED;
    Direction d1 = Direction::NORTH;
    std::cout << "Color: " << c1 << ", Direction: " << static_cast<int>(d1) << "\n";

    // constexpr → derleme zamanında, consteval → derleme zamanında ZORUNLU
    constexpr int sq = square(5);
    constexpr int cu = cube(3);
    std::cout << "square(5)=" << sq << ", cube(3)=" << cu << "\n";

    // typedef / using
    ULL    big1 = 9999999999ULL;
    uint64 big2 = 8888888888ULL;
    std::cout << "ULL: " << big1 << ", uint64: " << big2 << "\n";
}

void Basics::runNamespaces() const {
    std::cout << "\n--- Namespaces ---\n";
    std::cout << "add(3,4):      " << MathUtils::add(3, 4)             << "\n";
    std::cout << "multiply(3,4): " << MathUtils::multiply(3, 4)        << "\n";
    std::cout << "power(2,8):    " << MathUtils::Advanced::power(2, 8) << "\n";
    Graphics::D2::Shapes::draw();
    API::connect();        // inline namespace → v2
    API::v1::connect();    // açıkça v1
}

void Basics::runCollections() const {
    std::cout << "\n--- Collections ---\n";

    Vec<int> nums = {10, 20, 30, 40, 50};

    std::cout << "Classic for:     ";
    for (int i = 0; i < static_cast<int>(nums.size()); ++i)
        std::cout << nums[i] << " ";
    std::cout << "\n";

    // range-based for: const& → kopyalama yok, değiştirme yok
    std::cout << "Range-based for: ";
    for (const auto& n : nums)
        std::cout << n << " ";
    std::cout << "\n";
}

void Basics::runIO() const {
    std::cout << "\n--- IO ---\n";
    // std::cout → stdout, tamponlanmış
    // std::cerr → stderr, tamponlanmamış, hata çıktısı
    // "\n" tercih edilir — std::endl flush yaptığı için yavaştır
    std::cout << "stdout via cout\n";
    std::cerr << "stderr via cerr\n";
}

int main() {
    std::cout << "===== C++ Notes Demo =====\n\n";

    Basics b("demo");
    b.print();
    std::cout << "Instance count: " << Basics::getInstanceCount() << "\n\n";

    b.runVariables();
    b.runNamespaces();
    b.runCollections();
    b.runIO();

    // copy ctor çalışır
    Basics b2 = b;
    b2.print();

    // Basics b3("x");
    // b3 = b;              // HATA: copy assignment = delete
    // b3 = std::move(b);   // HATA: move assignment = delete

    return 0;
}