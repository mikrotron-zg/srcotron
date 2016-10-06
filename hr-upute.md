# Srcotron - kratke upute za korištenje

## Sastavljanje

1. Zavijte Arduino/Iteaduino UNO pločicu na svoje mjesto u kutiji
2. Ubodite Olimex ECG-EMG shield u pločicu pazeći na pinove
3. Zatvorite kutiju i spojite potrebne kablove
4. Gotovo!

## Osnovni uvjeti za rad

* Sastavljen Srcotron, USB kabel i elektrode
* [Python 2.66](https://www.python.org/download/releases/2.6.6/) ili kasnija 2.-- verzija + [pygame](http://www.pygame.org/download.shtml)
* Arduino IDE
* Arduino UNO ili kompatibilna pločica koju možete programirati putem USB-a

## Postavljanje koda na srcotron

Svodi se na učitavanje koda u pločicu putem Arduino IDE-a. Skinite datoteku ekg_py.ino iz mape python/ekg_py i uploadajte je na srcotron.
Program u toj datoteci slat će očitanja putem USB-a koja ćete moći vidjeti kao EKG signal u python skripti test.py koja se također nalazi u
mapi python.

## Postavljanje python skripte

Jedina bitna postavka u python skripti jest lokacija COM porta na kojem računalo komunicira sa srcotronom. Taj port možete vidjeti
u Arduino IDE sučelju pri uploadu koda. Skriptu možete otvoriti jednostavnim uređivačem teksta kao što je notepad. Pod sekcijom koda
"serial init" valja pronaći postavku s.port i u navodne znakove upisati ime COM porta na kojem vaše računalo vidi srcotron.

## Korištenje srcotrona za mjerenje

Kad ste postavili programski dio srcotrona, dovoljno je pokrenuti skriptu i pravilno postaviti elektrode! Signal, kao i mjerenje
RR intervala (pulsa) događaju se u realnom vremenu. Elektrode valja postaviti na obje ruke i lijevu nogu prema [Einthovenovom trokutu](http://www.medicine.mcgill.ca/physio/vlab/cardio/images/einthoven.jpg).
Ovisno o elektrodama koje imate, boje i oznake mogu se razlikovati, pa se poslužite jednostavnim algoritmom koji će u najviše dva
koraka pravilno postaviti elektrode:

0. Ako su rezultati očekivani, super!
1. Ako je signal invertiran (okrenut naopako), zamijenite elektrode lijeve i desne ruke i -> 1.
2. Ako je signal neočekivanog oblika, zamijenite mjesta elektrode na lijevoj nozi i bilo koje od elektroda na rukama i -> 2.

Detaljnije informacije o radu uređaja možete pronaći u komentarima u kodu
