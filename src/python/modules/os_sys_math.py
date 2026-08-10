import os
import math
import sys

file = "/home/serkan/source_code/tutorial/images/simple_plot.png"

os_flag = False
math_flag = False
sys_flag = False

if os_flag:
    os.name # OS türünü verir.(nt -> windows, posix -> linux & MAC)
    os.sep  # OS hakkında ayırıcı bilgisini verir ('\' veya '/')
    os.getcwd()   # Kodun çalıştırıldığı dizin (linux -> pwd)
    os.stat(file) # Dosya hakkında bilgi verir.
    os.mkdir("new_directory") # Yeni dizin oluşturur.
    os.makedirs(".\\a\\b\\new_directory") # Belirtilen yollar yoksa oluşturur.
    os.rename("a","b") # İsim değiştirme
    os.remove("a.txt") # Dosya silme
    os.rmdir("new_directory") # Boş dizileri siler
    # os.removedirs(C:\\...) # İçi boş dizi yollarını siler
    os.chdir('C:\\Users\\') # Dizinler arasında geçiş sağlar.
    os.listdir("C:\\Users\\") # Dizinin içini listeler
    os.system("notepad.exe") # Programların başlatılmasını saglar
    os.urandom() # Rasgele 12 bytan oluşan diziler oluşturmayı sağlar
    os.path.abspath("a.txt") # Tam yolunu gösterir

if math_flag:
    math.ceil(19.23)  # Üste yuvarlar
    math.floor(19.23) # Alta yuvarlar
    math.copysign(15,-1) # İkinci sayinin işaretini birincisine verir
    math.fabs(-15)    # Mutlak deger 
    math.factorial(5) # Faktöriyel
    math.fmod(15,2)   # Kalan verir
    math.gcd(12,22)   # EBOB
    math.e      # Euler (2.7)
    math.pi     # Pi  (3.14)
    math.tau    # Tau (pi*2 = tau)
    math.exp(2) # e**2
    math.log(25,5) # log
    math.log1p(0)  # e tabanına göre log 
    math.pow(10,2) # Kuvvet alma
    math.sqrt(10)  # Kök alma
    math.degrees(1.5) # Radyana --> derece
    math.radians(90)  # Derece --> radyan

if sys_flag:
    print(sys.argv)  # Programa adı ve girilen parametreleri gösterir (komut satırından)
    sys.executable   # Python calışabilir dosyasının adını gösteriri
    sys.getwindowsversion() # Kullanılan windows hakkında bilgi verir
    sys.platform # Kodların calıştıgı işletim sistemi hakkında bilgi verir
    sys.version  # Python sürümünün versiyonunu verir
    sys.exit()   # Programı sonlanrırır zorla kapatır
