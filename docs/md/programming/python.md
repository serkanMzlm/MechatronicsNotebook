# Python

Yüksek seviyeli, dinamik tipli ve çok paradigmalı bir programlama dilidir. **"Batteries included"** felsefesiyle kapsamlı standart kütüphane, okunabilir sözdizimi ve hızlı prototipleme kapasitesiyle ön plana çıkar. Veri bilimi, sistem otomasyonu, web geliştirme ve gömülü sistemlerde (MicroPython) yaygın olarak kullanılır.

- **Immutable (Değiştirilemez):** `int`, `float`, `complex` , `str`, `bytes`, `bool`, `tuple`, `frozenset`
- **Mutable (Değiştirilebilir)** `list`, `dict`, `set`, `bytearray`

- `break`     Döngüyü sonlandırır                          
- `continue`  Bir sonraki iterasyona geçer                 
- `pass`      İşlem yapmaz; sözdizimi için yer tutar       
- `return`    Fonksiyondan çıkar; `return None` varsayılan 

| Operatör | Açıklama                        | Örnek                    |
| -------- | ------------------------------- | ------------------------ |
| `**`     | Üst alma                        | `2 ** 10 = 1024`         |
| `//`     | Tam bölme (floor division)      | `7 // 2 = 3`             |
| `%`      | Mod                             | `7 % 2 = 1`              |
| `@`      | Matris çarpımı (PEP 465)        | `A @ B`                  |
| `:=`     | Walrus (atama + kullanma, 3.8+) | `if (n := len(a)) > 10:` |
| `is`     | Kimlik (aynı nesne mi?)         | `a is None`              |
| `in`     | Üyelik                          | `'a' in 'python'`        |


| Fonksiyon                             | Açıklama                                          |
| ------------------------------------- | ------------------------------------------------- |
| `len(x)`                              | Uzunluk                                           |
| `range(start, stop, step)`            | Sayı dizisi üreteci                               |
| `enumerate(it, start=0)`              | `(index, value)` çiftleri                         |
| `zip(*iters)`                         | Paralel iterasyon                                 |
| `map(f, it)`                          | Her elemana `f` uygular; lazy                     |
| `filter(f, it)`                       | `f(x)` True olanları geçirir; lazy                |
| `sorted(it, key, reverse)`            | Yeni sıralı liste                                 |
| `reversed(seq)`                       | Ters iterator                                     |
| `all(it)` / `any(it)`                 | Tümü / en az biri True                            |
| `sum(it, start=0)`                    | Toplam                                            |
| `max()` / `min()`                     | En büyük / küçük; `key` parametresi alır          |
| `abs(x)`                              | Mutlak değer                                      |
| `round(x, n)`                         | n ondalığa yuvarlama                              |
| `divmod(a, b)`                        | `(a//b, a%b)` çifti                               |
| `isinstance(obj, type)`               | Tür denetimi; tuple ile birden fazla tip          |
| `issubclass(cls, base)`               | Alt sınıf sorgusu                                 |
| `type(obj)`                           | Nesne türü                                        |
| `id(obj)`                             | Bellek kimliği (CPython'da adres)                 |
| `hash(obj)`                           | Hash değeri; immutable için                       |
| `dir(obj)`                            | Öznitelik ve metod listesi                        |
| `vars(obj)`                           | `__dict__` döner                                  |
| `callable(obj)`                       | Çağrılabilir mi?                                  |
| `repr(obj)`                           | Resmi string gösterimi                            |
| `eval(expr)`                          | String'i değerlendirir (güvenli ortamda kullanın) |
| `exec(code)`                          | String'i çalıştırır (güvenli ortamda kullanın)    |
| `ord(c)` / `chr(n)`                   | Karakter ↔ Unicode kod noktası                    |
| `bin(x)` / `oct(x)` / `hex(x)`        | Taban dönüşümleri                                 |
| `bytes(x)` / `bytearray(x)`           | Byte nesneleri                                    |
| `open(path, mode, encoding)`          | Dosya açma                                        |
| `print(*objs, sep, end, file, flush)` | Çıktı                                             |
| `input(prompt)`                       | Kullanıcı girişi - her zaman `str` döner          |


```python
# Sayı Gösterimleri
n = 100_000_000  # _ rakam ayırıcı (okunabilirlik için)
n = 0b1010       # İkili
n = 0o12         # Sekizlik
n = 0x1F         # Onaltılık

# Ternary expression
sonuc = "çift" if n % 2 == 0 else "tek"

# Walrus - atama ve kontrol tek satırda
if (n := len(input("deger: "))) < 10:
    print(f"Girdi {n} karakterden kısa")

# for-else: break ile çıkılmadıysa else çalışır
for harf in metin:
    if harf == 'a':
        print("bulundu")
        break
else:
    print("bulunamadı")  # sadece break tetiklenmediyse
```


## Veri Türleri

- **String:** Tek (`'…'`), çift (`"…"`) veya üçlü tırnak ile tanımlanır. **Immutable** olduğu için  bir karakter değiştirmek için yeni string üretmek gerekir.

    | Yöntem             | Sözdizimi              | Not                   |
    | ------------------ | ---------------------- | --------------------- |
    | `%` (printf tarzı) | `"%s %d" % (isim, n)`  | Eski; hâlâ geçerli    |
    | `str.format()`     | `"{} {}".format(a, b)` | Esnek; Python 2.6+    |
    | **f-string**       | `f"{isim!r:>10}"`      | En hızlı; Python 3.6+ |

    ```python
    kelime = "python"
    kelime[0]       # 'p'
    kelime[-1]      # 'n'
    kelime[::-1]    # 'nohtyp'  (ters çevirme)
    kelime[::2]     # 'pto'
    kelime * 3      # 'pythonpythonpython'

    isim, yas = "Serkan", 30
    print(f"{isim:<10} yaşında {yas:03d}")  # 'Serkan     yaşında 030'
    print(f"{3.14159:.2f}")                  # '3.14'
    print(f"{1_000_000:,}")                  # '1,000,000'
    print("{0:^9} {0:^9x} {0:^9b}".format(15))  # onluk/hex/binary

    # String <-> Bytes dönüşümü
    s = "İstanbul"
    b = s.encode("utf-8")         # b'\xc4\xb0stanbul'
    print(b.decode("utf-8"))      # 'İstanbul'

    print(ord('A'))                # 65
    print(chr(65))                 # 'A'

    ba = bytearray(b"mutable")    # Değiştirilebilir byte dizisi
    ba[0] = 77                    # 'm' → 'M'

    bytes.fromhex("deadbeef")     # b'\xde\xad\xbe\xef'
    ```

- **List:** Bellekte sıralı, dinamik boyutlu **mutable** dizi.

    ```python
    a = [3, 1, 4, 2] # Liste

    # List Comprehension
    kareler  = [x**2 for x in range(10)]
    ciftle   = [x for x in range(100) if x % 2 == 0]
    matris   = [[1,2],[3,4]]
    duz      = [x for satir in matris for x in satir]  # [1,2,3,4]
    ```

- **Tuple:** `()` içinde virgülle oluşturulur. **Immutable** değiştirilemez.

    ```python
    t = (1, 2, 3)
    t2 = 4, 5, 6       # Parantez zorunlu değil
    tek = (42,)        # Tek elemanlı - sondaki virgül zorunlu

    # Unpacking
    a, b, c = t
    baş, *orta, son = [1, 2, 3, 4, 5]  # baş=1, orta=[2,3,4], son=5
    ```

    !!! tip "Named Tuple"
        ```python
        from collections import namedtuple
        Nokta = namedtuple('Nokta', ['x', 'y'])
        p = Nokta(3, 4)
        print(p.x, p.y)       # 3  4
        print(p._asdict())    # {'x': 3, 'y': 4}
        ```

- **Dict:** Anahtar-değer çifti. Python 3.7+'da **ekleme sırası korunur**. Key'ler immutable ve hashlanabilir olmalı.

    ```python
    sozluk = {"isim": "Ali", "yas": 30}

    sozluk["sehir"] = "Ankara"           # Ekleme / güncelleme
    sozluk.get("tel", "Yok")             # Güvenli okuma
    sozluk.pop("sehir")                  # Siler, değeri döner
    sozluk.setdefault("tel", "000")      # Yoksa ekler, varsa dokunmaz
    sozluk.update({"yas": 31, "x": 1})  # Toplu güncelleme
    {**sozluk, "yeni": True}             # Dict merge (3.9+'da | operatörü)

    for k, v in sozluk.items():
        print(f"{k}: {v}")
    ```

    !!! tip "Dict Comprehension ve zip"
        ```python
        kareler = {x: x**2 for x in range(5)}

        keys, values = ['a', 'b', 'c'], [1, 2, 3]
        d = dict(zip(keys, values))  # {'a':1, 'b':2, 'c':3}
        ```

- **Set:** Sırasız, benzersiz elemanlar. Üyelik kontrolü ortalama O(1).

    ```python
    a = {1, 2, 3, 4}
    b = {3, 4, 5, 6}

    a | b    # Birleşim   {1,2,3,4,5,6}
    a & b    # Kesişim    {3,4}
    a - b    # Fark       {1,2}
    a ^ b    # Simetrik   {1,2,5,6}

    a.issubset(b)    # a ⊆ b mı?
    a.isdisjoint(b)  # Ortak eleman yok mu?
    ```

    !!! tip "Frozenset"
        `frozenset` immutable set; dict key veya başka bir set'in elemanı olabilir.


## Fonksiyonlar

| Parametre Türü      | Sözdizimi  | Açıklama                                          |
| ------------------- | ---------- | ------------------------------------------------- |
| Pozisyonel zorunlu  | `a`        | Sırayla verilmesi şart                            |
| Varsayılan          | `a=10`     | Verilmezse varsayılan kullanılır                  |
| Variadic positional | `*args`    | Ek pozisyonel argümanları tuple yapar             |
| Keyword-only        | `*, k`     | `*` sonrasındaki; yalnızca isimle verilir         |
| Variadic keyword    | `**kwargs` | Ek keyword argümanlarını dict yapar               |
| Positional-only     | `/` öncesi | `/` öncesi yalnızca pozisyonel geçilebilir (3.8+) |

```python
def f(a, b=10, *args, anahtar_only=False, **kwargs):
    print(a, b, args, anahtar_only, kwargs)

f(1, 2, 3, 4, anahtar_only=True, x=99)
# 1  2  (3,4)  True  {'x': 99}
```

!!! note "Argument Unpacking"
    ```python
    def topla(a, b, c): return a + b + c

    topla(*[1, 2, 3])           # Pozisyonel açma
    topla(**{'a': 1, 'b': 2, 'c': 3})  # Keyword açma
    ```

### Lambda ve Closures

```python
kare  = lambda x: x * x
siralama = sorted(isimler, key=lambda x: x.lower())

# Closure: iç fonksiyon dış değişkeni "kapar"
def carp_ile(carpan):
    def ic(x):
        return x * carpan   # carpan closure üzerinden yaşar
    return ic

ikiyle_carp = carp_ile(2)
print(ikiyle_carp(5))  # 10
```

### Decorators

Fonksiyonu sarmalayarak davranışını değiştiren veya genişleten fonksiyon.

=== "Temel Decorator"
    ```python
    import functools

    def zamanlayici(func):
        @functools.wraps(func)   # __name__, __doc__ korunur
        def wrapper(*args, **kwargs):
            import time
            t0 = time.perf_counter()
            sonuc = func(*args, **kwargs)
            print(f"{func.__name__}: {time.perf_counter()-t0:.4f}s")
            return sonuc
        return wrapper

    @zamanlayici
    def agir_islem():
        sum(range(10_000_000))
    ```

=== "Parametre Alan Decorator (Factory)"
    ```python
    def tekrarla(n):
        def decorator(func):
            @functools.wraps(func)
            def wrapper(*args, **kwargs):
                for _ in range(n):
                    func(*args, **kwargs)
            return wrapper
        return decorator

    @tekrarla(3)
    def merhaba():
        print("Merhaba")

    merhaba()  # 3 kez yazdırır
    ```

=== "Stacked Decorators"
    ```python
    @A
    @B
    def f(): pass
    # Eşdeğeri: f = A(B(f))
    # Uygulama sırası: içten dışa (önce B, sonra A)
    ```

!!! danger "functools.wraps Neden Zorunlu?"
    `@wraps(func)` olmadan wrapper, `__name__`, `__doc__`, `__module__` gibi nitelikleri kaybeder. Bu durum loglama, debugging ve bazı framework'lerin (Flask gibi) iç mekanizmasını bozabilir.

### Generators ve yield

Büyük veri setlerini lazy değerlendirme ile bellekte saklamadan üretir.

```python
def fibonacci():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b

gen = fibonacci()
[next(gen) for _ in range(8)]  # [0, 1, 1, 2, 3, 5, 8, 13]
```

|                   |      Liste       |      Generator      |
| ----------------- | :--------------: | :-----------------: |
| Bellek            |     Tüm veri     | Sadece anlık eleman |
| Sözdizimi         | `[expr for ...]` |   `(expr for ...)`  |
| Tekrar edilebilir |       Evet       |   Hayır (tükenir)   |
| `len()`           |        ✓         |          ✗          |

!!! note "yield from"
    ```python
    def zincir(a, b):
        yield from a    # a'nın tüm değerlerini delegation ile üretir
        yield from b
    ```

!!! tip "send() ile İki Yönlü İletişim"
    ```python
    def akumulator():
        toplam = 0
        while True:
            n = yield toplam
            toplam += (n or 0)

    g = akumulator()
    next(g)        # Başlat (ilk yield'e kadar ilerle)
    g.send(10)     # 10
    g.send(5)      # 15
    ```

### Type Hints

Çalışma zamanında zorunlu değil; `mypy`, IDE ve linter için statik analiz sağlar.

```python
from typing import Optional, TypeVar, Generic, Protocol

def selamla(isim: str) -> str:
    return f"Merhaba {isim}"

def ilk_eleman(lst: list[int]) -> Optional[int]:  # 3.9+'da list[int] yeterli
    return lst[0] if lst else None

T = TypeVar('T')

class Yigin(Generic[T]):
    def __init__(self) -> None:
        self._veri: list[T] = []
    def push(self, item: T) -> None:
        self._veri.append(item)
    def pop(self) -> T:
        return self._veri.pop()
```

!!! note "Protocol - Yapısal Subtipleme"
    ```python
    class Yazdirabilir(Protocol):
        def yazdir(self) -> None: ...

    def cikti_al(obj: Yazdirabilir) -> None:
        obj.yazdir()
    # Herhangi bir sınıf `yazdir` metoduna sahipse Protocol'u karşılar.
    # Explicit miras almak gerekmez - duck typing'in tip güvenli hali.
    ```

### Fonksiyon İmzası ve Anotasyonlar

```python
from typing import Callable

def uygula(f: Callable[[int], int], x: int) -> int:
    return f(x)

uygula(lambda x: x * 2, 5)  # 10
```


## OOP

| Nitelik Türü | Erişim                         | Açıklama                                   |
| ------------ | ------------------------------ | ------------------------------------------ |
| `isim`       | Her yerden                     | Public                                     |
| `_isim`      | Kural gereği yalnızca içeriden | Protected (gelenek)                        |
| `__isim`     | Yalnızca içeriden              | Private - name mangling: `_SinifAdi__isim` |

| Dunder                   | Tetikleyen           | Açıklama                         |
| ------------------------ | -------------------- | -------------------------------- |
| `__init__`               | `Sinif()`            | Constructor                      |
| `__del__`                | GC / `del obj`       | Destructor                       |
| `__repr__`               | `repr(obj)` / REPL   | Resmi, parse edilebilir gösterim |
| `__str__`                | `str(obj)` / `print` | Kullanıcı dostu gösterim         |
| `__len__`                | `len(obj)`           | Uzunluk                          |
| `__getitem__`            | `obj[key]`           | İndeksleme                       |
| `__setitem__`            | `obj[key] = v`       | Atama                            |
| `__delitem__`            | `del obj[key]`       | Silme                            |
| `__contains__`           | `x in obj`           | Üyelik kontrolü                  |
| `__iter__`               | `iter(obj)`          | Iterator başlatma                |
| `__next__`               | `next(obj)`          | Sonraki eleman                   |
| `__add__`                | `a + b`              | Toplama                          |
| `__eq__`                 | `a == b`             | Eşitlik                          |
| `__lt__`                 | `a < b`              | Küçüklük                         |
| `__hash__`               | `hash(obj)`          | Dict key / set eleman için       |
| `__call__`               | `obj()`              | Nesneyi çağrılabilir yapar       |
| `__enter__` / `__exit__` | `with obj:`          | Context manager protokolü        |

```python
class Sekil:
    sinif_sayisi: int = 0     # Sınıf niteliği - tüm örnekler paylaşır

    def __init__(self, renk: str) -> None:
        self.renk = renk          # Örnek niteliği
        Sekil.sinif_sayisi += 1

    @classmethod
    def kac_adet(cls) -> int:    # cls = Sekil veya alt sınıf
        return cls.sinif_sayisi

    @staticmethod
    def tanim() -> str:          # Ne self ne cls - bağımsız
        return "Geometrik şekil"

    @property
    def bilgi(self) -> str:      # Getter - parametre almaz
        return f"Renk: {self.renk}"

    @bilgi.setter
    def bilgi(self, yeni: str) -> None:
        self.renk = yeni
    
class Nokta:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __add__(self, other):
        return Nokta(self.x + other.x, self.y + other.y)

    def __repr__(self):
        return f"Nokta({self.x}, {self.y})"

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y


class Sayac:
    toplam = 0
    def __init__(self):
        Sayac.toplam += 1
        self.id = Sayac.toplam
        self.__gizli = 42


p1, p2 = Nokta(1, 2), Nokta(3, 4)
print(p1 + p2)  # Nokta(4, 6)


a, b = Sayac(), Sayac()
print(Sayac.toplam)          # 2
# print(a.__gizli)           # AttributeError
print(a._Sayac__gizli)       # 42 (name mangling ile erişim)
```


### Kalıtım ve MRO

```mermaid
graph LR
    A[Sekil] --> B[Dortgen]
    A --> C[Daire]
    B --> D[Kare]
    style A fill:#2196F3,color:#fff
    style B fill:#4CAF50,color:#fff
    style C fill:#4CAF50,color:#fff
    style D fill:#FF9800,color:#fff
```

```python
class Sekil:
    def alan(self) -> float:
        raise NotImplementedError

class Dortgen(Sekil):
    def __init__(self, a: float, b: float) -> None:
        self.a, self.b = a, b

    def alan(self) -> float:
        return self.a * self.b

class Kare(Dortgen):
    def __init__(self, kenar: float) -> None:
        super().__init__(kenar, kenar)  # Üst sınıf __init__

print(Kare.__mro__)
# (<class 'Kare'>, <class 'Dortgen'>, <class 'Sekil'>, <class 'object'>)
```

!!! note "MRO (Method Resolution Order)"
    Python çoklu kalıtımda hangi metodun çağrılacağını **C3 Linearization** algoritmasıyla belirler. `super()` her zaman MRO'daki sıradaki sınıfı çağırır.

### Abstract Base Classes

```python
from abc import ABC, abstractmethod

class Sekil(ABC):
    @abstractmethod
    def alan(self) -> float: ...

    @abstractmethod
    def cevre(self) -> float: ...

class Daire(Sekil):
    def __init__(self, r: float) -> None:
        self.r = r
    def alan(self) -> float:
        return 3.14159 * self.r ** 2
    def cevre(self) -> float:
        return 2 * 3.14159 * self.r

# s = Sekil()   # TypeError - abstract sınıf örneklenemez
```

### Dataclasses (Python 3.7+)

`__init__`, `__repr__`, `__eq__` gibi boilerplate'i otomatik üretir.

```python
from dataclasses import dataclass, field

@dataclass(order=True, frozen=False)
class Nokta:
    x: float
    y: float
    z: float = 0.0
    etiket: list = field(default_factory=list)  # Mutable default - GÜVENLİ

    def uzaklik(self) -> float:
        return (self.x**2 + self.y**2 + self.z**2) ** 0.5

p1 = Nokta(1.0, 2.0)
p2 = Nokta(1.0, 2.0)
print(p1 == p2)   # True  (otomatik __eq__)
```

!!! danger "field(default_factory=...) Neden Şart?"
    `etiket: list = []` yazılırsa tüm dataclass örnekleri **aynı listeyi** paylaşır. `field(default_factory=list)` her nesne için bağımsız liste üretir.

### Context Managers

`with` bloğu ile kaynak yönetimini otomatize eder; exception'da dahi `__exit__` çalışır.

=== "Sınıf Tabanlı"
    ```python
    class ZamanOlcer:
        def __enter__(self):
            import time
            self.t = time.perf_counter()
            return self

        def __exit__(self, exc_type, exc_val, exc_tb):
            import time
            self.sure = time.perf_counter() - self.t
            return False   # False → Exception'ı bastırma, yukarı ilet

    with ZamanOlcer() as z:
        sum(range(10_000_000))
    print(f"{z.sure:.3f}s")
    ```

=== "@contextmanager"
    ```python
    from contextlib import contextmanager

    @contextmanager
    def zaman_olcer():
        import time
        t = time.perf_counter()
        try:
            yield              # With bloğu burada çalışır
        finally:
            print(f"{time.perf_counter()-t:.3f}s")

    with zaman_olcer():
        sum(range(10_000_000))
    ```

### \_\_slots\_\_

Her örnek için `__dict__` yerine sabit alan ayırır; bellek kullanımını önemli ölçüde azaltır.

```python
class Nokta:
    __slots__ = ('x', 'y')   # Yalnızca bu nitelikler var olabilir

    def __init__(self, x, y):
        self.x, self.y = x, y

# p.z = 1  → AttributeError
# Milyonlarca örnek oluşturulacaksa __slots__ belleği %40–70 azaltabilir
```

### Descriptors

`@property`'nin arkasındaki mekanizma. `__get__` / `__set__` / `__delete__` tanımlanarak nitelik erişimi sınıf düzeyinde kontrol edilir.

```python
class Pozitif:
    def __set_name__(self, owner, name):
        self._name = name

    def __get__(self, obj, objtype=None):
        return obj.__dict__.get(self._name, 0)

    def __set__(self, obj, value):
        if value < 0:
            raise ValueError(f"{self._name} negatif olamaz")
        obj.__dict__[self._name] = value

class Dikdortgen:
    genislik  = Pozitif()
    yukseklik = Pozitif()
    # Her niteliğe atama Pozitif.__set__ üzerinden geçer
```

### Metaclass ve type

Her Python sınıfı aslında `type`'ın bir örneğidir. `type(isim, temeller, sozluk)` ile çalışma zamanında sınıf üretilebilir.

```python
# Dinamik sınıf oluşturma
Hayvan = type("Hayvan", (), {"ses": lambda self: "..."})

# Metaclass: tüm alt sınıflara otomatik davranış ekle
class AutoRepr(type):
    def __new__(mcs, name, bases, ns):
        ns.setdefault('__repr__', lambda self: f"{type(self).__name__}({vars(self)})")
        return super().__new__(mcs, name, bases, ns)

class Nokta(metaclass=AutoRepr):
    def __init__(self, x, y):
        self.x, self.y = x, y

print(Nokta(1, 2))  # Nokta({'x': 1, 'y': 2})
```

!!! tip "\_\_init_subclass\_\_ - Metaclass Alternatifi"
    ```python
    class Plugin:
        _registry: dict = {}

        def __init_subclass__(cls, tip: str, **kwargs):
            super().__init_subclass__(**kwargs)
            Plugin._registry[tip] = cls

    class JsonPlugin(Plugin, tip="json"): ...
    class CsvPlugin(Plugin,  tip="csv"):  ...

    # Plugin._registry → {'json': JsonPlugin, 'csv': CsvPlugin}
    # Eklenti kaydı için metaclass'a gerek kalmadan çalışır
    ```


## Hata Yönetimi

```python
class UygulamaHatasi(Exception):
    """Özel uygulama hatası."""
    def __init__(self, mesaj: str, kod: int = 0) -> None:
        super().__init__(mesaj)
        self.kod = kod

try:
    deger = int(input("Sayı: "))
    if deger < 0:
        raise UygulamaHatasi("Negatif değer yasak!", kod=400)
except ValueError:
    print("Geçersiz sayı.")
except UygulamaHatasi as e:
    print(f"Hata ({e.kod}): {e}")
except Exception as e:
    print(f"Beklenmedik hata: {e}")
    raise          # Yeniden fırlat
else:
    print("Başarılı")    # Exception fırlatılmadıysa çalışır
finally:
    print("Her zaman çalışır")
```

| İstisna             | Nedeni                       |
| ------------------- | ---------------------------- |
| `ValueError`        | Tip doğru ama değer geçersiz |
| `TypeError`         | Yanlış tip                   |
| `KeyError`          | Dict'te olmayan anahtar      |
| `IndexError`        | Liste sınırı dışı            |
| `AttributeError`    | Nesne özniteliği yok         |
| `FileNotFoundError` | Dosya bulunamadı             |
| `ZeroDivisionError` | Sıfıra bölme                 |
| `StopIteration`     | Generator tükendi            |
| `RecursionError`    | Özyineleme limiti aşıldı     |

!!! tip "Exception Chaining"
    ```python
    try:
        ...
    except ValueError as e:
        raise RuntimeError("Dönüşüm başarısız") from e
    # Traceback her iki hatayı da gösterir - kayıp bağlam olmaz
    ```


## Dosya İşlemleri

| Mod   | Açıklama                            |
| ----- | ----------------------------------- |
| `'r'` | Okuma (varsayılan)                  |
| `'w'` | Yazma (varsa üzerine yazar)         |
| `'a'` | Sonuna ekler                        |
| `'x'` | Oluşturma; varsa `FileExistsError`  |
| `'b'` | Binary mod ekler: `'rb'`, `'wb'`    |
| `'+'` | Hem okuma hem yazma: `'r+'`, `'w+'` |

```python
# Text modu - with bloğu kapanışı garanti eder
with open("veri.txt", "r", encoding="utf-8") as f:
    icerik = f.read()          # Tüm içerik
    # ya da satır satır (büyük dosyalarda bellek verimli):
    for satir in f:
        print(satir.rstrip())

# Binary modu - magic number ile dosya türü tespiti
with open("resim.png", "rb") as f:
    header = f.read(8)
    if header[:8] == b"\x89PNG\r\n\x1a\n":
        print("PNG dosyası")
```


```python title="os.path yerine modern, nesne yönelimli yol API"
from pathlib import Path

p = Path("/home/user/docs/rapor.txt")

p.exists()     # Var mı?
p.is_file()    # Dosya mı?
p.suffix       # '.txt'
p.stem         # 'rapor'
p.parent       # /home/user/docs

# / operatörü ile yol birleştirme
log = Path("/var/log") / "uygulama" / "app.log"

# Tüm .py dosyalarını recursive bul
for dosya in Path(".").rglob("*.py"):
    print(dosya)

# open() yerine doğrudan
log.write_text("başladı\n", encoding="utf-8")
icerik = log.read_text(encoding="utf-8")
```

!!! note "Dosya Magic Number ve Metadata"

    | Dosya Türü  | Magic Bytes                              | Hex                             |
    | ----------- | ---------------------------------------- | ------------------------------- |
    | PNG         | `\x89PNG\r\n\x1a\n`                      | `89 50 4E 47 0D 0A 1A 0A`       |
    | JPEG (JFIF) | İlk 2 byte `\xFF\xD8`, offset 6-9 `JFIF` | `FF D8 FF E0 .. .. 4A 46 49 46` |
    | GIF         | `GIF`                                    | `47 49 46`                      |
    | BMP         | `BM`                                     | `42 4D`                         |
    | TIFF LE     | `II`                                     | `49 49`                         |

    ```python
    PDF_ETIKETI = {
        b"/Creator": "Oluşturan uygulama",
        b"/Producer": "PDF kütüphanesi",
        b"/Author": "Yazar",
        b"/Title": "Başlık",
        b"/CreationDate": "Oluşturma tarihi",
    }

    with open("belge.pdf", "rb") as f:
        data = f.read()
        for etiket, aciklama in PDF_ETIKETI.items():
            idx = data.find(etiket)
            if idx != -1:
                print(f"{aciklama}: {data[idx:idx+80].split(b'\n', 1)[0]}")
    ```


```python
import json

veri = {"isim": "Ali", "yas": 30, "sehirler": ["Ankara", "İstanbul"]}

metin = json.dumps(veri, ensure_ascii=False, indent=2)  # Python → JSON string
nesne = json.loads(metin)                                # JSON string → Python

with open("veri.json", "w", encoding="utf-8") as f:
    json.dump(veri, f, ensure_ascii=False, indent=2)

with open("veri.json", encoding="utf-8") as f:
    nesne = json.load(f)
```

## Modüller ve Paketler

```python
import os
import urllib.request                # Alt modül tam yolu
from os import path as p             # Alias
from os import getcwd, listdir       # Belirli isimler
from sys import *                    # Önerilmez

# Aynı paket içi göreli import
from . import helper                 # mypkg/helper.py
from .sub import utils               # mypkg/sub/utils.py
from ..core import config            # Bir üst pakete çıkış
```

!!! note "Paket Yapısı"
    ```
    mypackage/
    ├── __init__.py       # Paketi tanımlar (3.3+'da zorunlu değil, iyi pratik)
    ├── core.py
    └── utils/
        ├── __init__.py
        └── helpers.py
    ```

!!! tip "__all__ ile Public API Kontrolü"
    ```python
    # mypackage/__init__.py
    __all__ = ["core", "yardimci"]   # from mypackage import * için filtre
    ```

!!! note "sys.path ile Dinamik Import"
    ```python
    import sys
    sys.path.append("/ozel/dizin")   # Bu dizindeki .py dosyaları import edilebilir
    ```

!!! example "re (Regular Expressions)"

    | Fonksiyon           | Açıklama                          |
    | ------------------- | --------------------------------- |
    | `re.match(p, s)`    | Yalnızca string başından eşleşir  |
    | `re.search(p, s)`   | İlk eşleşmeyi bulur               |
    | `re.findall(p, s)`  | Tüm eşleşmeleri liste döner       |
    | `re.finditer(p, s)` | Tüm eşleşmeleri iterator döner    |
    | `re.sub(p, r, s)`   | Eşleşmeleri değiştirir            |
    | `re.split(p, s)`    | Desene göre böler                 |
    | `re.compile(p)`     | Tekrar kullanım için desen derler |

    | Özel Karakter   | Anlam                         |
    | --------------- | ----------------------------- |
    | `.`             | Yeni satır hariç her karakter |
    | `^` / `$`       | Satır başı / sonu             |
    | `\d` / `\D`     | Rakam / rakam değil           |
    | `\w` / `\W`     | `[A-Za-z0-9_]` / değil        |
    | `\s` / `\S`     | Boşluk / boşluk değil         |
    | `*` / `+` / `?` | 0+, 1+, 0-1 tekrar            |
    | `{m,n}`         | m-n arası tekrar              |
    | `(...)`         | Yakalama grubu                |
    | `(?:...)`       | Yakalamayan grup              |
    | `(?P<isim>...)` | İsimli yakalama grubu         |

    ```python
    import re

    metin = "2024-06-19 tarihinde 3 olay meydana geldi."
    m = re.search(r"(\d{4})-(\d{2})-(\d{2})", metin)
    if m:
        yil, ay, gun = m.groups()  # ('2024', '06', '19')

    # Compile ile tekrar kullanım - performans
    email_re = re.compile(r"[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}")
    mailler = email_re.findall("info@ornek.com ve admin@test.org")

    # re.VERBOSE ile okunabilir desen
    tarih = re.compile(r"""
        ^(\d{4})    # Yıl
        -(\d{2})    # Ay
        -(\d{2})$   # Gün
    """, re.VERBOSE)
    ```


## Eşzamanlılık

- **GIL (Global Interpreter Lock):** CPython'da aynı anda yalnızca bir thread Python bytecode çalıştırabilir.

    | Senaryo               | Çözüm                      | Açıklama                                     |
    | --------------------- | -------------------------- | -------------------------------------------- |
    | I/O-bound (ağ, disk)  | `threading` veya `asyncio` | GIL, I/O bekleme sırasında serbest bırakılır |
    | CPU-bound (hesaplama) | `multiprocessing`          | Her process kendi GIL'ine sahip              |

- **threading** (Deadlock: İki thread birbirinin kilidini beklerse program sonsuza kadar bloke olur. Kilitleri her zaman **aynı sırayla** alın)

- **multiprocessing**

- **asyncio:** Cooperative multitasking - `await` noktasında event loop başka coroutine'lere geçer.

!!! note "async/await Kuralları"
    - `async def` ile tanımlanan fonksiyon **coroutine** döner; çağrılırken `await` şart.
    - `await` yalnızca `async def` içinde kullanılabilir.
    - `asyncio.run()` event loop'u başlatır ve coroutine tamamlanana kadar bekler.

|            | `threading` | `multiprocessing` | `asyncio`  |
| ---------- | :---------: | :---------------: | :--------: |
| GIL kısıtı |     Evet    |       Hayır       |    N/A     |
| I/O-bound  |     İyi     | Gereksiz overhead |   En iyi   |
| CPU-bound  |   Yetersiz  |       En iyi      |  Yetersiz  |
| Bellek     |  Paylaşılan |       İzole       | Paylaşılan |


## Virtual Environment

- Her proje bağımsız paket sürümlerine sahip olur; sistem Python'u kirletilmez. Takım çalışmasında `requirements.txt` aynı ortamı yeniden oluşturur.

```bash
python3 -m venv .venv            # Sanal ortam oluştur
source .venv/bin/activate        # Linux/macOS etkinleştir
.venv\Scripts\activate.bat       # Windows etkinleştir
deactivate                       # Devre dışı bırak
```
