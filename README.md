# Connect4

Gra Connect 4 napisana w języku C z wykorzystaniem biblioteki Allegro 5.

## Opis

Connect 4 to klasyczna gra logiczna dla dwóch graczy. Celem gry jest ułożenie czterech swoich pionków w linii (pionowo, poziomo lub na skos) na planszy o wymiarach 6x7.
Statystyki graczy zapisywane są do pliku scores.txt, można je zresetować w menu klikając R.

## Wymagania

- Kompilator C (np. gcc)
- Biblioteka [Allegro 5](https://liballeg.org/)
- Plik czcionki `arial.ttf` w katalogu z plikiem wykonywalnym

W folderze Windows znajduje się gotowy skompilowany plik connect4.exe na systemy Windows z biblioteką Allegro5. 

## Budowanie

Aby zbudować projekt, przejdź do katalogu `connect4_modular` i uruchom:

```sh
make
```

Plik wykonywalny zostanie utworzony jako `connect4`.

## Uruchamianie

Po zbudowaniu uruchom grę poleceniem:

```sh
./connect4
```

## Sterowanie

- **ENTER** – rozpocznij grę
- **ESC** – wróć do menu lub wyjdź z gry
- **R** – restart planszy (w trakcie gry) lub zresetuj statystyki (w menu)

### Sterowanie myszką / klawiaturą
- **Ruch myszy / Strzałki lewo prawo** - wybierz kolumnę
- **Kliknięcie myszy / ENTER** - wrzuć pionek do kolumny 

## Autorzy

- Jakub Stasierski
- Nikodem Witkowski
