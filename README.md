# Connect4

Gra Connect 4 napisana w języku C z wykorzystaniem biblioteki Allegro 5.

## Opis

Connect 4 to klasyczna gra logiczna dla dwóch graczy. Celem gry jest ułożenie czterech swoich pionków w linii (pionowo, poziomo lub na skos) na planszy o wymiarach 6x7.

## Wymagania

- Kompilator C (np. gcc)
- Biblioteka [Allegro 5](https://liballeg.org/)
- Plik czcionki `arial.ttf` w katalogu z plikiem wykonywalnym

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

## Pliki

- `src/` – kod źródłowy gry
- `assets/fonts/` – czcionki używane w grze
- `scores.txt` – plik z zapisanymi wynikami

## Autorzy

- Jakub Stasierski
- Nikodem Witkowski
