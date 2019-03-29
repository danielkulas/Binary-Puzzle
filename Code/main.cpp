#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio2.h"
#include "naglow.h"
#pragma warning(disable:4996) // Brak powoduje blad kompilacji


void wyswietlanie(plansza_t *plansza, pozycje_t *legenda, pozycje_t kursor, pola_t *pola, int trybAuto)
{
	int i, j;
	legenda->x = 2, legenda->y = 2;

	char pozycja[32], napisWartosc[2];
	clrscr();
	textcolor(WHITE);

	/* Ustawienie miejsca wyswietlania legendy i planszy */
	if (LEGENDA_PO_PRAWEJ == PRAWDA)
	{
		(*legenda).x = 70;
		plansza->x = 4;
	}

	/* Wyswietlanie legendy */
	gotoxy((*legenda).x, legenda->y++);
	cputs("Daniel Kulas 168813");
	gotoxy((*legenda).x, legenda->y++);
	legenda->y++;
	cputs("Zaimplementowano: abcdefghi!jklmn!");
	gotoxy((*legenda).x, legenda->y++);
	cputs("Strzalki: poruszanie");
	gotoxy((*legenda).x, legenda->y++);
	cputs("ESC: wyjscie z gry");
	gotoxy((*legenda).x, legenda->y++);
	cputs("0, 1: wpisywanie cyfr");
	gotoxy((*legenda).x, legenda->y++);
	cputs("n: nowa gra");
	gotoxy((*legenda).x, legenda->y++);
	cputs("s: zapis stanu gry");
	gotoxy((*legenda).x, legenda->y++);
	cputs("l: wczytanie stanu gry");
	gotoxy((*legenda).x, legenda->y++);
	cputs("r: zmiana rozmiaru planszy");
	gotoxy((*legenda).x, legenda->y++);
	cputs("o: losowe wypelnienie planszy");
	gotoxy((*legenda).x, legenda->y++);
	cputs("p: prosta podpowiedz");
	gotoxy((*legenda).x, legenda->y++);
	cputs("k: sprawdzenie mozliwosci ukonczenia gry");
	gotoxy((*legenda).x, legenda->y++);
	cputs("j: podswietlanie pol jednoznacznych");
	gotoxy((*legenda).x, legenda->y++);
	cputs("w: wypelnianie podswietlonych pol jednoznacznych");
	gotoxy((*legenda).x, legenda->y++);
	cputs("a: automatyczna detekcja konca gry: ");
	if(trybAuto == 1)
		cputs("wlaczono");
	if (trybAuto == 0)
		cputs("wylaczono");
	sprintf(pozycja, "Pozycja kursora: (%d, %d)", kursor.x - (*plansza).x + 1, kursor.y - (*plansza).y + 1);
	gotoxy((*legenda).x, legenda->y++);
	cputs(pozycja);
	if (automatycznaDetekcjaKonca(*plansza, *legenda, kursor, pola) == SPELNIONA && trybAuto == SPELNIONA)
	{
		gotoxy((*legenda).x, legenda->y++);
		textcolor(RED);
		cputs("Nie ma mozliwosci ukonczenia planszy!");
		textcolor(WHITE);
	}

	/* Wyswietlanie planszy */
	for (i = 0, j = 0; i < (*plansza).rozmiar * (*plansza).rozmiar; i++)
	{
		if ((pola + i)->modyfikowalne == FALSZ)
			textcolor(LIGHTRED);

		if ((pola + i)->kolor == RED)
			textbackground(RED);

		if ((pola + i)->kolor == 1 || (pola + i)->kolor == 2)
			textbackground(YELLOW);

		if (i % (*plansza).rozmiar == 0)
		{
			gotoxy((*plansza).x, (*plansza).y + j);
			j++;
		}
		sprintf(napisWartosc, "%c", (pola + i)->wartosc);
		cputs(napisWartosc);
		textcolor(WHITE);
		textbackground(BLACK);
	}

	/* Wyswietlanie ramki */
	for (i = -1; i <= (*plansza).rozmiar; i++)
	{
		for (j = -1; j <= (*plansza).rozmiar; j++)
		{
			if (j == -1 || j == (*plansza).rozmiar)
			{
				gotoxy((*plansza).x + i, (*plansza).y + j);
				textcolor(BROWN);
				putch('-');
			}
			else if (i == -1 || i == (*plansza).rozmiar)
			{
				gotoxy((*plansza).x + i, (*plansza).y + j);
				putch('|');
			}
		}
	}

	/* Wyswietlanie kursora */
	gotoxy(kursor.x, kursor.y);
	textcolor(GREEN);
	if (modyfWPolu(kursor.x, kursor.y, *plansza, pola) == FALSZ)
		textcolor(RED);

	if (wartoscWPolu(kursor.x, kursor.y, *plansza, pola) == ZERO || wartoscWPolu(kursor.x, kursor.y, *plansza, pola) == JEDEN)
		putch(wartoscWPolu(kursor.x, kursor.y, *plansza, pola));
	else
		putch('X');
}


void nowaGra(plansza_t *plansza, pola_t **pola)
{
	char tablicaPoczatkowa[] = "...1.......1.0......1........0.......0.1.0.0.0.01.1....0..1......0..1.....11....1...0.....0....0.1..1.....1...0..0.0......0..00....0...1..0..1..";
	plansza->rozmiar = ROZMIARPODSTAWOWY;
	zwolnijPamiec(pola);
	przydzielPamiec(*plansza, pola);

	for (int i = 0; i < (*plansza).rozmiar * (*plansza).rozmiar; i++)
	{
		(*pola + i)->wartosc = tablicaPoczatkowa[i];

		if ((*pola + i)->wartosc == KROPKA)
			(*pola + i)->modyfikowalne = PRAWDA;
		else if ((*pola + i)->wartosc == JEDEN || (*pola + i)->wartosc == ZERO)
			(*pola + i)->modyfikowalne = FALSZ;
	}
}


void zapisGry(plansza_t plansza, pola_t *pola)
{
	FILE *zapisPlik = fopen("zapis.txt", "w+");
	if (zapisPlik == NULL)
	{
		cputs("Nie moge otworzyc pliku!");
		exit(1);
	}

	fprintf(zapisPlik, "%2d", plansza.rozmiar);
	fprintf(zapisPlik, "\n");

	for (int i = 0; i < plansza.rozmiar * plansza.rozmiar; i++)
		fprintf(zapisPlik, "%c", (pola + i)->wartosc);

	fprintf(zapisPlik, "\n");

	for (int i = 0; i < plansza.rozmiar * plansza.rozmiar; i++)
		fprintf(zapisPlik, "%1d", (pola + i)->modyfikowalne);

	fclose(zapisPlik);
}


void wczytanieGry(plansza_t *plansza, pola_t **pola)
{
	FILE *wczytPlik = fopen("zapis.txt", "r");
	if (wczytPlik == NULL)
	{
		cputs("Nie moge otworzyc pliku!");
		exit(2);
	}

	fscanf(wczytPlik, "%2d", &(plansza)->rozmiar);
	zwolnijPamiec(pola);
	(plansza)->rozmiar++;
	przydzielPamiec(*plansza, pola);
	(plansza)->rozmiar--;

	fscanf(wczytPlik, "\n");

	for (int i = 0; i < (*plansza).rozmiar * (*plansza).rozmiar; i++)
		fscanf(wczytPlik, "%c", &((*pola + i)->wartosc));

	fscanf(wczytPlik, "\n");

	for (int i = 0; i < (*plansza).rozmiar * (*plansza).rozmiar; i++)
		fscanf(wczytPlik, "%1d", &((*pola + i)->modyfikowalne));

	fclose(wczytPlik);
}


void zmianaRozmiaruPlanszy(plansza_t *plansza, pozycje_t legenda, pola_t **pola)
{
	char poleRozmiar[32], maxRozmiar[32];
	int rozmiar1 = 0, rozmiar2 = 0, rozmiar3 = 0, nowyRozmiar = 0, pozycjaNapisu = 21;
	textcolor(WHITE);

	do {
		gotoxy(legenda.x, ++pozycjaNapisu);
		cputs("Podaj rozmiar planszy i zatwierdz enterem");
		gotoxy(legenda.x, ++pozycjaNapisu);
		cputs("Rozmiar planszy musi byc wiekszy niz 1 i conajwyzej ");
		if ((SZEROKOSCKONSOLI - (*plansza).x) > (WYSOKOSCKONSOLI - (*plansza).y))
		{
			sprintf(maxRozmiar, "%d", WYSOKOSCKONSOLI - (*plansza).y);
			cputs(maxRozmiar);
		}
		else
		{
			sprintf(maxRozmiar, "%d", SZEROKOSCKONSOLI - (*plansza).x);
			cputs(maxRozmiar);
		}
		
		gotoxy(legenda.x, ++pozycjaNapisu);
		cputs("Rozmiar planszy musi byc liczba parzysta! ");

		rozmiar1 = getch() - '0';
		sprintf(poleRozmiar, "%d", rozmiar1);
		cputs(poleRozmiar);
		rozmiar2 = getch() - '0';
		if (rozmiar2 + '0' == ENTER)
		{
			rozmiar2 = 0;
			nowyRozmiar = rozmiar1;
		}
		else
		{
			sprintf(poleRozmiar, "%d", rozmiar2);
			cputs(poleRozmiar);

			rozmiar3 = getch() - '0';
			if (rozmiar3 + '0' == ENTER)
			{
				nowyRozmiar = (rozmiar1 * 10) + rozmiar2;
			}
			else
			{
				sprintf(poleRozmiar, "%d", rozmiar3);
				cputs(poleRozmiar);
				nowyRozmiar = (rozmiar1 * 100) + (rozmiar2 * 10) + rozmiar3;
			}
		}
		clrscr();
	} while (nowyRozmiar < 2 || nowyRozmiar > (SZEROKOSCKONSOLI - (*plansza).x) || nowyRozmiar > (WYSOKOSCKONSOLI - (*plansza).y) || nowyRozmiar % 2 != 0);

	plansza->rozmiar = nowyRozmiar;

	zwolnijPamiec(pola);
	przydzielPamiec(*plansza, pola);
	losoweWypelnianie(*plansza, *pola);
}


void losoweWypelnianie(plansza_t plansza, pola_t *pola)
{
	zerujPlansze(plansza, pola);
	int liczbaPol = 0, minLiczbaPol = 0, maxLiczbaPol = 0, maxLiczaProb = (plansza.rozmiar * plansza.rozmiar * MAXLICZBAPROB) / 100, x = 0, y = 0, max = 0, wartosc = 0;
	srand(time(0));

	minLiczbaPol = plansza.rozmiar * plansza.rozmiar * PROGMINIMALNY / 100;
	maxLiczbaPol = (plansza.rozmiar * plansza.rozmiar * PROGMAKSYMALNY / 100) - minLiczbaPol;
	liczbaPol = minLiczbaPol + rand() % maxLiczbaPol;

	for (int i = 0; i < liczbaPol; i++)
	{
		max++;
		if (max == maxLiczaProb)
			break;

		x = rand() % plansza.rozmiar;
		y = rand() % plansza.rozmiar;
		wartosc = rand() % 2;

		if (wartoscWPolu(plansza.x + x, plansza.y + y, plansza, pola) == ZERO || wartoscWPolu(plansza.x + x, plansza.y + y, plansza, pola) == JEDEN)
			i--;

		if (wartosc == 0)
			pisz(plansza.x + x, plansza.y + y, ZERO, plansza, pola);
		if (wartosc == 1)
			pisz(plansza.x + x, plansza.y + y, JEDEN, plansza, pola);

		if (wartoscWPolu(plansza.x + x, plansza.y + y, plansza, pola) == KROPKA)
		{
			i--;
		}
		else
		{
			uczynPoleModyf(plansza.x + x, plansza.y + y, plansza, pola);
		}
	}
}


void podpowiedz(plansza_t plansza, pozycje_t legenda, pozycje_t kursor, pola_t *pola)
{
	int pozycjaY = 21, swap = 0, swap2, nRozmiar = plansza.rozmiar / 2;
	char poleR[32], ilosc[32], kol[32];
	sprintf(poleR, "%d", nRozmiar);

	textcolor(WHITE);
	gotoxy(legenda.x, pozycjaY);
	cputs("Podpowiedz: ");

	if (modyfWPolu(kursor.x, kursor.y, plansza, pola) == FALSZ)
	{
		textcolor(RED);
		gotoxy(legenda.x, ++pozycjaY);
		cputs("To pole jest niemodyfikowalne!");
	}
	else
	{
		textcolor(GREEN);
		if (wartoscWPolu(kursor.x, kursor.y, plansza, pola) == ZERO)
		{
			gotoxy(legenda.x, ++pozycjaY);
			cputs("W wybranym polu znajduje sie '0'!");
		}
		else if (wartoscWPolu(kursor.x, kursor.y, plansza, pola) == JEDEN)
		{
			gotoxy(legenda.x, ++pozycjaY);
			cputs("W wybranym polu znajduje sie '1'!");
		}
		else
		{
			if (zasada1(kursor.x, kursor.y, ZERO, plansza, pola) == SPELNIONA && zasada2(kursor.x, kursor.y, ZERO, plansza, pola) == SPELNIONA && zasada3(kursor.x, kursor.y, plansza, pola) == SPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Dla tego pola dostepna jest wartosc: 0");
			}

			if (zasada1(kursor.x, kursor.y, JEDEN, plansza, pola) == SPELNIONA && zasada2(kursor.x, kursor.y, JEDEN, plansza, pola) == SPELNIONA && zasada3(kursor.x, kursor.y, plansza, pola) == SPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Dla tego pola dostepna jest wartosc: 1");
			}

			if ((zasada1(kursor.x, kursor.y, JEDEN, plansza, pola) != SPELNIONA || zasada2(kursor.x, kursor.y, JEDEN, plansza, pola) != SPELNIONA || zasada3(kursor.x, kursor.y, plansza, pola) != SPELNIONA) 
				&& (zasada1(kursor.x, kursor.y, ZERO, plansza, pola) != SPELNIONA || zasada2(kursor.x, kursor.y, ZERO, plansza, pola) != SPELNIONA || zasada3(kursor.x, kursor.y, plansza, pola) != SPELNIONA))
			{
				textcolor(RED);
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Nie mozna wpisac zadnej z wartosci. Powody:");
			}

			textcolor(RED);
			if (zasada1(kursor.x, kursor.y, ZERO, plansza, pola) == NIESPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Regula pierwsza nie jest spelniona(dla 0)!");
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Wpisanie wartosci 0, spowodowaloby, ze");
				gotoxy(legenda.x, ++pozycjaY);
				cputs(" w miejscu kursora bylyby wiecej niz dwa '0' z rzedu!");
			}

			if (zasada2(kursor.x, kursor.y, ZERO, plansza, pola) == NIESPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Regula druga nie jest spelniona(dla 0)!");
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Wpisanie wartosci 0, spowodowaloby, ze");
				gotoxy(legenda.x, ++pozycjaY);
				cputs(" liczba zer bylaby wieksza niz ");
				cputs(poleR);
			}

			if (zasada1(kursor.x, kursor.y, ZERO, plansza, pola) == SPELNIONA && zasada2(kursor.x, kursor.y, ZERO, plansza, pola) == SPELNIONA)
			{
				if (zasada3(kursor.x, kursor.y, plansza, pola) == NIESPELNIONA)
				{
					swap2 = ktoryTo(kursor.x, kursor.y, plansza, pola);

					gotoxy(legenda.x, ++pozycjaY);
					cputs("Regula trzecia nie jest spelniona!");
					gotoxy(legenda.x, ++pozycjaY);
					cputs("Wpisanie wartosci 0, spowodowaloby, ze");
					gotoxy(legenda.x, ++pozycjaY);

					if (swap2 < plansza.rozmiar)
					{
						cputs("wiersz ten bylby taki sam jak nr: ");
						swap2++;
					}
					else if (swap2 >= plansza.rozmiar)
					{
						swap2 = swap2 - plansza.rozmiar;
						swap2++;
						cputs("kolumna ta bylaby taka sama jak nr: ");
					}

					sprintf(kol, "%d", swap2);
					cputs(kol);
				}
			}

			if (zasada1(kursor.x, kursor.y, JEDEN, plansza, pola) == NIESPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Regula pierwsza nie jest spelniona(dla 1)!");
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Wpisanie wartosci 1, spowodowaloby, ze");
				gotoxy(legenda.x, ++pozycjaY);
				cputs(" w miejscu kursora bylyby wiecej niz dwie '1' z rzedu!");
			}

			if (zasada2(kursor.x, kursor.y, JEDEN, plansza, pola) == NIESPELNIONA)
			{
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Regula druga nie jest spelniona(dla 1)!");
				gotoxy(legenda.x, ++pozycjaY);
				cputs("Wpisanie wartosci 1, spowodowaloby, ze");
				gotoxy(legenda.x, ++pozycjaY);
				cputs(" liczba jedynek bylaby wieksza niz ");
				cputs(poleR);
			}

			if (zasada1(kursor.x, kursor.y, JEDEN, plansza, pola) == SPELNIONA && zasada2(kursor.x, kursor.y, JEDEN, plansza, pola) == SPELNIONA)
			{
				if (zasada3(kursor.x, kursor.y, plansza, pola) == NIESPELNIONA)
				{
					swap2 = ktoryTo(kursor.x, kursor.y, plansza, pola);
					
					gotoxy(legenda.x, ++pozycjaY);
					cputs("Regula trzecia nie jest spelniona!");
					gotoxy(legenda.x, ++pozycjaY);
					cputs("Wpisanie wartosci 1, spowodowaloby, ze");
					gotoxy(legenda.x, ++pozycjaY);

					if (swap2 < plansza.rozmiar)
					{
						cputs("wiersz ten bylby taki sam jak nr: ");
						swap2++;
					}
					else if (swap2 >= plansza.rozmiar)
					{
						swap2 = swap2 - plansza.rozmiar;
						swap2++;
						cputs("kolumna ta bylaby taka sama jak nr: ");
					}
					
					sprintf(kol, "%d", swap2);
					cputs(kol);
				}
			}
		}
	}

	// Zliczanie wartosci
	textcolor(WHITE);
	for (int i = 0; i < plansza.rozmiar; i++)
	{
		gotoxy(plansza.x + i, plansza.y - 3);
		swap = zliczajZnakiWKolumnie(ZERO, i, plansza, pola);
		if (swap > 9)
		{
			swap = swap - 10;
			cputs("1");
			gotoxy(plansza.x + i, plansza.y - 2);
		}
		sprintf(ilosc, "%d", swap);
		cputs(ilosc);

		gotoxy(plansza.x - 3, plansza.y + i);
		swap = zliczajZnakiWeWierszu(ZERO, i, plansza, pola);
		sprintf(ilosc, "%d", swap);
		cputs(ilosc);

		gotoxy(plansza.x + i, plansza.y + plansza.rozmiar + 1);
		swap = zliczajZnakiWKolumnie(JEDEN, i, plansza, pola);
		if (swap > 9)
		{
			swap = swap - 10;
			cputs("1");
			gotoxy(plansza.x + i, plansza.y + plansza.rozmiar + 2);
		}
		sprintf(ilosc, "%d", swap);
		cputs(ilosc);

		gotoxy(plansza.x + plansza.rozmiar + 1, plansza.y + i);
		swap = zliczajZnakiWeWierszu(JEDEN, i, plansza, pola);
		sprintf(ilosc, "%d", swap);
		cputs(ilosc);
	}

	textcolor(WHITE);
	gotoxy(legenda.x, ++pozycjaY);
	cputs("Wcisnij dowolny klawisz, aby kontynuowac");
	getch();
	clrscr();
}


void sprawdzUkonczenie(plansza_t plansza, pozycje_t legenda, pozycje_t kursor, pola_t *pola, int trybAuto)
{
	int przesuniecie = 0;
	for (int y = 0; y < plansza.rozmiar; y++)
	{
		for (int x = 0; x < plansza.rozmiar; x++)
		{
			if (wartoscWPolu(plansza.x + x, plansza.y + y, plansza, pola) == KROPKA)
			{
				if ((zasada1(plansza.x + x, plansza.y + y, ZERO, plansza, pola) == NIESPELNIONA || zasada2(plansza.x + x, plansza.y + y, ZERO, plansza, pola) == NIESPELNIONA || zasada3(plansza.x + x, plansza.y + y, plansza, pola) == NIESPELNIONA) 
					&& (zasada1(plansza.x + x, plansza.y + y, JEDEN, plansza, pola) == NIESPELNIONA || zasada2(plansza.x + x, plansza.y + y, JEDEN, plansza, pola) == NIESPELNIONA || zasada3(plansza.x + x, plansza.y + y, plansza, pola) == NIESPELNIONA))
				{
					przesuniecie = (plansza.rozmiar*y) + x;
					(pola + przesuniecie)->kolor = RED;
				}
			}
		}
	}

	wyswietlanie(&plansza, &legenda, kursor, pola, trybAuto);
	gotoxy(legenda.x, 21);
	textcolor(WHITE);
	cputs("Nacisnij dowolny klawisz aby kontynuowac");
	getch();
	clrscr();

	for (int i = 0; i < plansza.rozmiar * plansza.rozmiar; i++)
	{
		(pola + i)->kolor = 0;
	}
}


void polaJednoznaczne(pozycje_t legenda, pozycje_t kursor, plansza_t plansza, pola_t *pola, int trybAuto)
{
	int przesuniecie = 0, klawisz = 0;
	for (int y = 0; y < plansza.rozmiar; y++)
	{
		for (int x = 0; x < plansza.rozmiar; x++)
		{
			if (zasada1(x + plansza.x, y + plansza.y, ZERO, plansza, pola) == NIESPELNIONA || zasada2(x + plansza.x, y + plansza.y, ZERO, plansza, pola) == NIESPELNIONA)
			{
				if(zasada1(x + plansza.x, y + plansza.y, JEDEN, plansza, pola) == SPELNIONA && zasada2(x + plansza.x, y + plansza.y, JEDEN, plansza, pola) == SPELNIONA && zasada3(x + plansza.x, y + plansza.y, plansza, pola) == SPELNIONA && wartoscWPolu(x + plansza.x, y + plansza.y, plansza, pola) != JEDEN && modyfWPolu(x + plansza.x, y + plansza.y, plansza, pola) == PRAWDA)
					(pola + (x + (plansza.rozmiar * y)))->kolor = KOLORJEDNOZNACZNEJEDEN;
			}
			if (zasada1(x + plansza.x, y + plansza.y, JEDEN, plansza, pola) == NIESPELNIONA || zasada2(x + plansza.x, y + plansza.y, JEDEN, plansza, pola) == NIESPELNIONA)
			{
				if (zasada1(x + plansza.x, y + plansza.y, ZERO, plansza, pola) == SPELNIONA && zasada2(x + plansza.x, y + plansza.y, ZERO, plansza, pola) == SPELNIONA && zasada3(x + plansza.x, y + plansza.y, plansza, pola) == SPELNIONA && wartoscWPolu(x + plansza.x, y + plansza.y, plansza, pola) != ZERO && modyfWPolu(x + plansza.x, y + plansza.y, plansza, pola) == PRAWDA)
					(pola + (x + (plansza.rozmiar * y)))->kolor = KOLORJEDNOZNACZNEZERO;
			}
		}
	}

	wyswietlanie(&plansza, &legenda, kursor, pola, trybAuto);
	textcolor(WHITE);
	gotoxy(legenda.x, 20);
	cputs("Nacisnij 'w', aby wpisac pola jednoznaczne");
	gotoxy(legenda.x, 21);
	cputs("Nacisnij dowolny inny klawisz aby kontynuowac");
	klawisz = getch();

	if (klawisz == W)
	{
		for (int i = 0; i < plansza.rozmiar * plansza.rozmiar; i++)
		{
			if ((pola + i)->kolor == 1)
				(pola + i)->wartosc = ZERO;

			if ((pola + i)->kolor == 2)
				(pola + i)->wartosc = JEDEN;
		}
	}

	clrscr();
	for (int i = 0; i < plansza.rozmiar * plansza.rozmiar; i++)
	{
		(pola + i)->kolor = 0;
	}
}


void akcja(int *klawisz, plansza_t plansza, pozycje_t *kursor, pola_t *pola)
{
	/* Ruch kursora po ekranie */
	*klawisz = getch();

	if (*klawisz == 0) // Wczytywanie znaku specjalnego
	{
		*klawisz = getch();
		if (*klawisz == GORA) kursor->y++;
		else if (*klawisz == DOL) kursor->y--;
		else if (*klawisz == PRAWO) kursor->x++;
		else if (*klawisz == LEWO) kursor->x--;
	}

	/* Umiejscowienie kursora stale na planszy */
	if ((*kursor).x < plansza.x)
	{
		kursor->x = plansza.x + plansza.rozmiar - 1;
		kursor->y--;
	}

	if ((*kursor).x >= plansza.x + plansza.rozmiar)
	{
		kursor->x = plansza.x;
		kursor->y++;
	}
	if ((*kursor).y < plansza.y) kursor->y = plansza.y + plansza.rozmiar - 1;
	if ((*kursor).y >= plansza.y + plansza.rozmiar) kursor->y = plansza.y;

	/* Wpisywanie 1 i 0 do pol */
	if (*klawisz == JEDEN)
		pisz((*kursor).x, (*kursor).y, JEDEN, plansza, pola);
	if (*klawisz == ZERO)
		pisz((*kursor).x, (*kursor).y, ZERO, plansza, pola);
}


void pisz(int x, int y, char wartosc, plansza_t plansza, pola_t *pola)
{
	/* Nalezy podac wspolrzedne globalne */
	if (modyfWPolu(x, y, plansza, pola) == PRAWDA && zasada1(x, y, wartosc, plansza, pola) == SPELNIONA && zasada2(x, y, wartosc, plansza, pola) == SPELNIONA && zasada3(x, y, plansza, pola) == SPELNIONA)
		(pola + ((x - plansza.x) + (plansza.rozmiar * (y - plansza.y))))->wartosc = wartosc;
}


void uczynPoleModyf(int x, int y, plansza_t plansza, pola_t *pola)
{
	/* Nalezy podac wspolrzedne globalne */
	(pola + ((x - plansza.x) + (plansza.rozmiar * (y - plansza.y))))->modyfikowalne = FALSZ;
}


char wartoscWPolu(int x, int y, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca wartosc pola "pod kursorem" */
	char wartosc;
	wartosc = (pola + ((x - plansza.x) + (plansza.rozmiar * (y - plansza.y))))->wartosc;
	return wartosc;
}


int modyfWPolu(int x, int y, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca "1", gdy pole "pod kursorem" jest modyfikowalne. "0" gdy nie jest */
	if ((pola + ((x - plansza.x) + (plansza.rozmiar * (y - plansza.y))))->modyfikowalne == FALSZ)
		return 0;
	else
		return 1;
}


int zasada1(int x, int y, char wartosc, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca "1", gdy zasada pierwsza gry jest spelniona. "0", gdy nie jest */

	if (x > plansza.x + 1)
		if (wartoscWPolu(x - 2, y, plansza, pola) == wartosc && wartoscWPolu(x - 1, y, plansza, pola) == wartosc)
			return 0;

	if (x < plansza.x + plansza.rozmiar - 2)
		if (wartoscWPolu(x + 2, y, plansza, pola) == wartosc && wartoscWPolu(x + 1, y, plansza, pola) == wartosc)
			return 0;

	if (x > plansza.x && x < plansza.x + plansza.rozmiar - 1)
		if (wartoscWPolu(x - 1, y, plansza, pola) == wartosc && wartoscWPolu(x + 1, y, plansza, pola) == wartosc)
			return 0;

	if (y > plansza.y + 1)
		if (wartoscWPolu(x, y - 2, plansza, pola) == wartosc && wartoscWPolu(x, y - 1, plansza, pola) == wartosc)
			return 0;

	if (y < plansza.y + plansza.rozmiar - 2)
		if (wartoscWPolu(x, y + 2, plansza, pola) == wartosc && wartoscWPolu(x, y + 1, plansza, pola) == wartosc)
			return 0;

	if (y > plansza.y && y < plansza.y + plansza.rozmiar - 1)
		if (wartoscWPolu(x, y - 1, plansza, pola) == wartosc && wartoscWPolu(x, y + 1, plansza, pola) == wartosc)
			return 0;

	return 1;
}


int zasada2(int x, int y, char wartosc, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca "1", gdy zasada druga gry jest spelniona. "0", gdy nie jest */
	int iloscZer = 0, iloscJedynek = 0, i = 0;
	if (wartosc == ZERO) iloscZer++;
	if (wartosc == JEDEN) iloscJedynek++;

	for (i = 0; i < plansza.rozmiar; i++)
	{
		if (wartoscWPolu(plansza.x + i, y, plansza, pola) == ZERO) iloscZer++;
		if (wartoscWPolu(plansza.x + i, y, plansza, pola) == JEDEN) iloscJedynek++;
	}
	if (iloscZer > plansza.rozmiar / 2 || iloscJedynek > plansza.rozmiar / 2) return 0;

	iloscZer = 0, iloscJedynek = 0;
	if (wartosc == ZERO) iloscZer++;
	if (wartosc == JEDEN) iloscJedynek++;
	for (i = 0; i < plansza.rozmiar; i++)
	{
		if (wartoscWPolu(x, plansza.y + i, plansza, pola) == ZERO) iloscZer++;
		if (wartoscWPolu(x, plansza.y + i, plansza, pola) == JEDEN) iloscJedynek++;
	}
	if (iloscZer > plansza.rozmiar / 2 || iloscJedynek > plansza.rozmiar / 2) return 0;

	return 1;
}


int zasada3(int x, int y, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca "1", gdy zasada trzecia gry jest spelniona. "0", gdy nie jest */
	int ilosc = 0, i = 0, zwiekszaj = 0;

	// Sprawdzenie dla wierszy
	for (i = 0; i < plansza.rozmiar; i++)
	{
		if (wartoscWPolu(plansza.x + i, y, plansza, pola) == ZERO || wartoscWPolu(plansza.x + i, y, plansza, pola) == JEDEN)
			ilosc++;
		if (plansza.x + i == x)
			ilosc++;
	}

	if (ilosc == plansza.rozmiar)
	{
		for (i = 0; i < plansza.rozmiar; i++)
		{
			ilosc = 0;
			for (int j = 0; j < plansza.rozmiar; j++)
			{
				if (wartoscWPolu(plansza.x + j, plansza.y + i, plansza, pola) == ZERO || wartoscWPolu(plansza.x + j, plansza.y + i, plansza, pola) == JEDEN)
					ilosc++;
			}
			if (ilosc == plansza.rozmiar)
			{
				zwiekszaj = 0;
				for (int a = 0; a < plansza.rozmiar; a++)
				{
					if (wartoscWPolu(plansza.x + a, y, plansza, pola) == wartoscWPolu(plansza.x + a, plansza.y + i, plansza, pola))
						zwiekszaj++;
					if (plansza.x + a == x)
						zwiekszaj++;
				}
				if (zwiekszaj == plansza.rozmiar)
					return 0;
			}
		}
	}

	//Sprawdzanie dla kolumn
	ilosc = 0;
	for (i = 0; i < plansza.rozmiar; i++)
	{
		if (wartoscWPolu(x, plansza.y + i, plansza, pola) == ZERO || wartoscWPolu(x, plansza.y + i, plansza, pola) == JEDEN)
			ilosc++;
		if (plansza.y + i == y)
			ilosc++;
	}

	if (ilosc == plansza.rozmiar)
	{
		for (i = 0; i < plansza.rozmiar; i++)
		{
			ilosc = 0;
			for (int j = 0; j < plansza.rozmiar; j++)
			{
				if (wartoscWPolu(plansza.x + i, plansza.y + j, plansza, pola) == ZERO || wartoscWPolu(plansza.x + i, plansza.y + j, plansza, pola) == JEDEN)
					ilosc++;
			}
			if (ilosc == plansza.rozmiar)
			{
				zwiekszaj = 0;
				for (int a = 0; a < plansza.rozmiar; a++)
				{
					if (wartoscWPolu(x, plansza.y + a, plansza, pola) == wartoscWPolu(plansza.x + i, plansza.y + a, plansza, pola))
						zwiekszaj++;
					if (plansza.y + a == y)
						zwiekszaj++;
				}
				if (zwiekszaj == plansza.rozmiar)
					return 0;
			}
		}
	}

	return 1;
}


void przydzielPamiec(plansza_t plansza, pola_t **pola)
{
	*pola = (pola_t*)malloc(sizeof(pola_t) * plansza.rozmiar * plansza.rozmiar);
	if (pola == NULL)
	{
		cputs("Brak pamieci!");
		exit(0);
	}
}


void zwolnijPamiec(pola_t **pola)
{
	if (pola != NULL)
		free(*pola);
}


void zerujPlansze(plansza_t plansza, pola_t *pola)
{
	for (int i = 0; i < plansza.rozmiar*plansza.rozmiar; i++)
	{
		(pola + i)->wartosc = KROPKA;
		(pola + i)->modyfikowalne = PRAWDA;
	}
}


int zliczajZnakiWKolumnie(char wartosc, int nrKolumny, plansza_t plansza, pola_t *pola)
{
	int ilosc = 0, i = 0;
	for (int i = 0; i < plansza.rozmiar; i++)
		if (wartoscWPolu(plansza.x + nrKolumny, plansza.y + i, plansza, pola) == wartosc) ilosc++;

	return ilosc;
}


int zliczajZnakiWeWierszu(char wartosc, int nrWiersza, plansza_t plansza, pola_t *pola)
{
	int ilosc = 0, i = 0;
	for (int i = 0; i < plansza.rozmiar; i++)
		if (wartoscWPolu(plansza.x + i, plansza.y + nrWiersza, plansza, pola) == wartosc) ilosc++;

	return ilosc;
}


int automatycznaDetekcjaKonca(plansza_t plansza, pozycje_t legenda, pozycje_t kursor, pola_t *pola)
{
	/* Zwraca '1', gdy wykryje koniec gry */
	for (int y = 0; y < plansza.rozmiar; y++)
	{
		for (int x = 0; x < plansza.rozmiar; x++)
		{
			if (wartoscWPolu(plansza.x + x, plansza.y + y, plansza, pola) == KROPKA)
			{
				if ((zasada1(plansza.x + x, plansza.y + y, ZERO, plansza, pola) == NIESPELNIONA || zasada2(plansza.x + x, plansza.y + y, ZERO, plansza, pola) == NIESPELNIONA || zasada3(plansza.x + x, plansza.y + y, plansza, pola) == NIESPELNIONA)
					&& (zasada1(plansza.x + x, plansza.y + y, JEDEN, plansza, pola) == NIESPELNIONA || zasada2(plansza.x + x, plansza.y + y, JEDEN, plansza, pola) == NIESPELNIONA || zasada3(plansza.x + x, plansza.y + y, plansza, pola) == NIESPELNIONA))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}


int ktoryTo(int x, int y, plansza_t plansza, pola_t *pola)
{
	/* Funkcja zwraca nr. wiersza/kolumny */
	int nrKolumny = 0, zwiekszaj = 0;

	//Sprawdzanie dla wierszy
	for (int i = 0; i < plansza.rozmiar; i++)
	{
		zwiekszaj = 0;
		for (int a = 0; a < plansza.rozmiar; a++)
		{
			if (wartoscWPolu(plansza.x + a, plansza.y + i, plansza, pola) != KROPKA)
			{
				if (wartoscWPolu(plansza.x + a, y, plansza, pola) == wartoscWPolu(plansza.x + a, plansza.y + i, plansza, pola))
					zwiekszaj++;
				if (plansza.x + a == x)
					zwiekszaj++;
			}
		}
		if (zwiekszaj == plansza.rozmiar)
			return i;
	}

	//Sprawdzanie dla kolumn
	for (int i = 0; i < plansza.rozmiar; i++)
	{
		zwiekszaj = 0;
		for (int a = 0; a < plansza.rozmiar; a++)
		{
			if(wartoscWPolu(plansza.x + i, plansza.y + a, plansza, pola) != KROPKA)
			{
				if (wartoscWPolu(x, plansza.y + a, plansza, pola) == wartoscWPolu(plansza.x + i, plansza.y + a, plansza, pola))
					zwiekszaj++;
				if (plansza.y + a == y)
					zwiekszaj++;
			}
		}
		if (zwiekszaj == plansza.rozmiar)
		{
			nrKolumny = i + plansza.rozmiar;
			return nrKolumny;
		}
	}
	return -1;
}


int main()
{
	struct plansza_t plansza;
	plansza.rozmiar = ROZMIARPODSTAWOWY;
	struct pozycje_t legenda;
	struct pozycje_t kursor;
	struct pola_t *pola = (pola_t*) malloc(plansza.rozmiar * plansza.rozmiar * sizeof(pola_t));
	int klawisz = 0, trybAuto = 0;
	plansza.x = 60, plansza.y = 4, kursor.x = plansza.x, kursor.y = plansza.y;

#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("Daniel Kulas 168813");

	nowaGra(&plansza, &pola);
	while (klawisz != ESC)
	{
		if (klawisz == N) nowaGra(&plansza, &pola);
		if (klawisz == S) zapisGry(plansza, pola);
		if (klawisz == L) wczytanieGry(&plansza, &pola);
		if (klawisz == R) zmianaRozmiaruPlanszy(&plansza, legenda, &pola);
		if (klawisz == O) losoweWypelnianie(plansza, pola);
		if (klawisz == P) podpowiedz(plansza, legenda, kursor, pola);
		if (klawisz == K) sprawdzUkonczenie(plansza, legenda, kursor, pola, trybAuto);
		if (klawisz == J) polaJednoznaczne(legenda, kursor, plansza, pola, trybAuto);
		if (klawisz == A) ++trybAuto = trybAuto % 2;
		wyswietlanie(&plansza, &legenda, kursor, pola, trybAuto);
		akcja(&klawisz, plansza, &kursor, pola);
	}
	zwolnijPamiec(&pola);
	return 0;
}
