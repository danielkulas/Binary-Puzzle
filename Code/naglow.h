#ifndef NAGLOW_H
#endif
#pragma once

#define ROZMIARPODSTAWOWY 12
#define LEGENDA_PO_PRAWEJ 0 // "1" skutkuje wyswietleniem legendy po prawej stronie
#define PRAWDA 1
#define FALSZ 0
#define SPELNIONA 1
#define NIESPELNIONA 0
#define KOLORJEDNOZNACZNEZERO 1 // Kolor dla pola jednoznacznego o wartosci jeden
#define KOLORJEDNOZNACZNEJEDEN 2 // Kolor dla pola jednoznacznego o wartosci jeden
#define PROGMINIMALNY 40 // Minimalny prog wypelnienia planszy w '%'!
#define PROGMAKSYMALNY 50 // Maksymalny prog wypelnienia planszy w '%'!
#define MAXLICZBAPROB 100 // Wyrazona w '%' z rozmiaru planszy do kwadratu
#define SZEROKOSCKONSOLI 85
#define WYSOKOSCKONSOLI 85
#define ESC 27 // Ponizej kody ASCII przyciskow
#define ENTER 13
#define GORA 80 
#define DOL 72
#define PRAWO  77
#define LEWO 75
#define ZERO 48
#define JEDEN 49
#define	N 110
#define	S 115 
#define	L 108 
#define	R 114
#define	O 111
#define	D 100
#define P 112
#define K 107
#define J 106
#define W 119
#define A 97
#define KROPKA 46

struct pola_t
{
	int kolor;
	char wartosc;
	int modyfikowalne;
};

struct pozycje_t
{
	int x;
	int y;
};

struct plansza_t
{
	int x;
	int y;
	int rozmiar;
};

void losoweWypelnianie(plansza_t plansza, pola_t *pola);

void pisz(int x, int y, char wartosc, plansza_t plansza, pola_t *pola);

void uczynPoleModyf(int x, int y, plansza_t plansza, pola_t *pola);

char wartoscWPolu(int x, int y, plansza_t plansza, pola_t *pola);

int modyfWPolu(int x, int y, plansza_t plansza, pola_t *pola);

int zasada1(int x, int y, char wartosc, plansza_t plansza, pola_t *pola);

int zasada2(int x, int y, char wartosc, plansza_t plansza, pola_t *pola);

int zasada3(int x, int y, plansza_t plansza, pola_t *pola);

void przydzielPamiec(plansza_t plansza, pola_t **pola);

void zwolnijPamiec(pola_t **pola);

void zerujPlansze(plansza_t plansza, pola_t *pola);

int zliczajZnakiWKolumnie(char wartosc, int nrKolumny, plansza_t plansza, pola_t *pola);

int zliczajZnakiWeWierszu(char wartosc, int nrWiersza, plansza_t plansza, pola_t *pola);

int automatycznaDetekcjaKonca(plansza_t plansza, pozycje_t legenda, pozycje_t kursor, pola_t *pola);

int ktoryTo(int x, int y, plansza_t plansza, pola_t *pola);
