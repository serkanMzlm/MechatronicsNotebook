from colorama import Fore, Back, Style, init
from datetime import datetime 

#konsol ekranında renklendirme yapılması için gerekli ayarlamalar yapıldı
init(autoreset=True) 

# Fore sayesinde yazının rengini ayarlayabiliriz. 
# Back arkaplan renginin ayarlanmasını sağlar.
# Style.RESET_ALL konsol ayarlarını sıfırlanmasını sağlar.
print(Fore.BLUE + "Mavi", Fore.RED + "Kirmizi", Fore.GREEN + "Yesil") 
print(Back.CYAN + "Cyan", Back.YELLOW + "Yellow", Back.MAGENTA + "Magenta")
print(Style.DIM + 'Python')
print(Style.RESET_ALL) 


now = datetime.now() # ya da datetime.todey()

# Verilerin daha okunaklı olması için kullanılır
date = datetime.ctime(now) # ya da datetime.strftime(now,"%c")
print("now: ", now, "\ndate: ",date)

# %x: Tarih bilgisi, %X: Saat bilgisi
# %c: Tam tarih, saat ve zaman bilgisi
# %d, gün bilgisi | %m, ay bilgisi
# %m: Sayı değerli bir karakter dizisi olarak ay
# %a, gün - %b, ay - %y, yıl kısa yazımları | %A, %B, %Y tam yazımları 
# %j, yıllın kaçıncı günü (1 - 366) - %U yıllın kaçıncı haftası (0 - 53)

# now.year - now.month - now.day - now.hour
# now.minute - now.second - now.microsecond