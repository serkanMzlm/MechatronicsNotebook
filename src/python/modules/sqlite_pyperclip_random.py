import random
import sqlite3 as sql
import pyperclip as pyp

vt = sql.connect("deneme1.sqlite") # bir ad koyarak veri tabanı olusturuldu
#vt = sql.connect(":memory:") # sabit disk yerine RAM de saklanır
im = vt.cursor()
im.execute("CREATE TABLE IF NOT EXISTS kisi(isim,soyisim)") #CREATE TABLE tablo oluşturma kommutu 
#adres_defteri adında iki tablo oluşturuldu
im.execute("INSERT  INTO kisi VALUES('ali','koc')") #işlenecek verileri hazırlar
vt.commit() #verilerin işlenmesini sağlar
im.execute("SELECT * FROM kisi") #kisi adlı tabloda bulunan bütün veriler anlamına gelir
veri = im.fetchall() # secilen verilerin alınmasını saglar
#veri = im.fetchone her cagrıldıgında sadece bir ver alıp bir ileri gider
#veri = im.fetchmany(5) belirtilen kadar veriyi ceker
#im.execute("SELECT name  FROM kisi") sayesinde kisi tablosunun baslıklarını ogreniriz
#im.execute("SELECT * FROM kisi WHERE ad = 'ali'") seklinde istenilen veriye gider
print(veri)
vt.close() #veritabanını kapatılması


pyp.copy("merhaba") # panoya kopyalanır
pyp.paste() #yapıştırır
pyp.waitForPaste() #panoda veri varmı ona bakar yoksa bekler gelince yapıstırı

random.random() # 0-1.0 arasında sayı  rasgele sayi üretir
random.uniform(10,2000) #belirlenen aralıkta rasgele sayi üretir
random.randint(45,200) #belirlediğimiz aralıkta tam sayı dondürür
liste = ['ali', 'veli', 'ahmet', 'mehmet', 'celal', 'selin', 'nihat']
random.choice(liste) #listeden rasgele birini cekme
random.shuffle(liste) #listelerin icini karıştırır
random.randrange(10,100,5) #randint aynı  sayılır farkı 1 2 ve 3 parametre ile calışabılıyor olması ve sınır degerleri alamamsı
liste2 = range(100)
random.sample(liste2,10) # diziden kac tane numune alınacagını belırler