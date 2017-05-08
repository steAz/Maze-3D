#define _CRT_SECURE_NO_WARNINGS
#include "conio2.h"
#include "funkcje.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

bool czySciana(Labirynt_str* lab, int x, int y)      //funkcja sprawdzajaca czy w danym miejscu jest sciana
{                          
	if (x<1 || x>lab->width)
		return true;
	if (y<1 || y>lab->height)
		return true;

	return (lab->tab[x - 1][y - 1] != ' ');                     // Zwraca negacje spacji, czyli jakis znak oznaczajacy sciane
}


unsigned int podajCzasGry(Gracz_str* gracz)
{
	return time(NULL) - gracz->czasGry;
}


void ustawPozycjeGracza(Gracz_str* gracz,Labirynt_str* lab)
{
	gracz->x = lab->start_x;  //STARTOWE POZYCJE GRACZA
	gracz->y = lab->start_y;
	gracz->kierunek = DOWN;
	gracz->czasGry = time(NULL);
	gracz->liczbaRuchow = 0;
}


void zakonczGre(Gracz_str* gracz, Labirynt_str* lab)
{
	if(lab->end_x == gracz->x && lab->end_x == gracz->y)
	lab->ON = false;
}


void ustawRozmiar(Labirynt_str* lab, int width, int height) 
{
	lab->tab = 0;

	if (lab->tab != NULL) 
	{
		for (int i = 0; i < lab->width; i++) 
		{
			delete[] lab->tab[i];
		}

		delete[] lab->tab;
		lab->tab = NULL;
	}

	lab->tab = new char*[width];  //tworzymy dynamicznie szerokosc (wiersze)

	for (int i = 0; i < width; i++) 
	{
		lab->tab[i] = new char[height];  //tworzymy dynamicznie wysokosc (kolumny)
	}

	lab->width = width;
	lab->height = height;
}


void zapisz(Labirynt_str* lab, char * sciezka)
{
	FILE * plik = fopen(sciezka, "w");

	if (plik)
	{
		char buffor[64];
		for (int y = 0; y < lab->height; y++) 
		{
			for (int x = 0; x < lab->width; x++) 
			{
				if (lab->tab[x][y] == ' ')
				{
					if (lab->start_x - 1 == x && lab->start_y - 1 == y)
					{
						buffor[x] = 's';
					}
					else if (lab->end_x - 1 == x && lab->end_y - 1 == y)
					{
						buffor[x] = 'k';
					}
					else
					{
						buffor[x] = '0';
					}
				}
				else
				{
					buffor[x] = '1';
				}
			}

			buffor[lab->width] = NULL;
			fprintf(plik, buffor);
			fprintf(plik, "\n");  //przejdz do nowej linii
		}

		fclose(plik);
	}
	else
	{
		printf(sciezka);
		printf("Error! Nie mozesz dostac sie do pliku");
		system("Pause");
	}
}


void laduj(Labirynt_str* lab,Gracz_str* gracz, const char* sciezka)
{
	FILE * plik = fopen(sciezka, "r");

	if (plik)
	{
		srand(time(NULL));
		char liniaTekstu[256]; 
		char tab[100][100];        //tablica buforowa maksymalna
		char znak[] = { '#', '$', '&' , '*', '@' };
		int y = 0;
		int x = 0;

		while(fscanf(plik, "%s", liniaTekstu) != EOF) 
		{
			x = 0;
			while (liniaTekstu[x] != '\0')  //czytaj, az nie najedziesz na znak konca linii
			{
				if (liniaTekstu[x] == '1')   
				{
					tab[x][y] = znak[rand() % 5];  //losowanie sciany gdy element tablicy bedzie jedynka
				}
				else 
				{
					tab[x][y] = ' ';        //jak bedzie zerem, wstaw spacje
					if (liniaTekstu[x] == 's') 
					{
						lab->start_x = x+1; 
						lab->start_y = y+1;
						ustawPozycjeGracza(gracz, lab);
					}
					else if (liniaTekstu[x] == 'k')
					{
						lab->end_x = x+1 ;
						lab->end_y = y+1 ;
						zakonczGre(gracz, lab);
					}
				}
				x++;
			}
			y++;
		}

		int width = x;
		int height = y;
		ustawRozmiar(lab, width, height);

		for (x = 0; x < width; x++)
		{
			for (y = 0; y < height; y++)
			{
				lab->tab[x][y] = tab[x][y];
			}
		}

		fclose(plik);
	}
	else 
	{
		printf(sciezka);
		printf("Error! Nie mozesz dostac sie do pliku");
		system("Pause");
	}
}


char podaj(Labirynt_str* lab, int x, int y)
{
	return lab->tab[x - 1][y - 1];
}


void podajRozmiar(Labirynt_str* lab)
{
	int width;
	int height;
	gotoxy(30, 20);
	cputs("Podaj wysokosc: ");
	cin >> height;
	gotoxy(30, 21);
	cputs("Podaj szerokosc: ");
	cin >> width;
	ustawRozmiar(lab, width, height);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			lab->tab[i][j] = '#';
		}
	}

	lab->start_x = 4; //przypadkowe okreslenia startu gracza i konca gracza
	lab->start_y = 1;
	lab->tab[3][0] = ' ';
	lab->end_x = 5;  
	lab->end_y = 1;
	lab->tab[4][0] = ' '; 
}


void rysujLabirynt(Labirynt_str* lab)           //funkcja rysujaca widok z gory
{ 
	for (int y = 1; y <= lab->height; y++) 
	{
		for (int x = 1; x <= lab->width; x++) 
		{
			gotoxy(x, y);
			putch(podaj(lab, x, y));
		}
	}
}


void rysujSciane(int x, int y, int h, bool przedluzenie, bool lewa, bool srodkowaSciana, char tekstura) //funkcja rysujaca sciane w 3D (domyslnie rysuje prawa sciane)
{
	const int X = 4;  //kolumny
	const int Y = h;   //wiersze
	char tab[18][4];

	for (int i = 0; i<Y; i++)
	{
		for (int j = 0; j<X; j++)
		{
			if (i != 0 && j == X - 1)  //ostatnia kolumna = |
			{
				tab[i][j] = '|';
			}

			else if (i > 1 && i < Y - 1 && j == 0 && !przedluzenie)
			{
				tab[i][j] = '|';
			}

			else if ((i == 0 || i == Y - 1) && (j == 1 || j == 2 || (j == 0 && przedluzenie)))
			{
				if (srodkowaSciana)
					tab[i][j] = ' ';
				else
					tab[i][j] = '_';
			}

			else if (i == Y - 1 && j == 0)
			{
				if (lewa)
					tab[i][j] = '/';
				else
					tab[i][j] = '\\';
			}

			else if (i == 1 && j == 0 && !przedluzenie)
			{
				if (lewa)
					tab[i][j] = '\\';
				else
					tab[i][j] = '/';
			}

			else if (i == 0 && (j == 0 || j == 3))
			{
				tab[i][j] = ' ';
			}

			else
			{
				tab[i][j] = tekstura; //tekstura bedzie wypelniona znakami jak w scianach w widoku z gory
			}
		}
	}

	int startowa_x = x; //pozycja startowa x
	int startowa_y = (17 - h) / 2 + y; //pozycja startowa y  ( liczba 17 okresla maksymalna mozliwa sciane, pierwsza po lewej i po prawej ma wysokosc 17 )
	for (int y = 0; y<Y; y++)
	{
		for (int x = 0; x<X; x++)
		{
			gotoxy(startowa_x + x, startowa_y + y);

			if (lewa)
				putch(tab[y][X - 1 - x]);  //jesli lewa sciana to zmienione indeksy wierszy  i rysujemy na odwrót
			else
				putch(tab[y][x]);
		}
	}
}


void rysujPrzedluzenie(int x, int y, int h,int w, char tekstura)
{
	const int X = w;
	const int Y = h;
	char tab[18][46];

	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{
			if ((i == 0 || i == Y - 1) && (j>0 && j < X - 1))
			{
				tab[i][j] = '_';
			}

			else if ((j == 0 || j == X - 1) && (i > 0))
			{
				tab[i][j] = '|';
			}

			else if (j == 0 && i==0)
			{

				tab[i][j] = ' ';
			
			}

			else if (j==X-1 && i==0)
			{
				tab[i][j] = ' ';
			}

			else
			{
				tab[i][j] = tekstura;
			}
		}
	}

	int startowa_x = (46 - w) / 2 + x;   //nrSciany =8  w=nrSciany*6-2
	int startowa_y = (17 - h) / 2 + y;

	for (int y = 0; y < Y; y++)
	{
		for (int x = 0; x < X; x++)
		{
			gotoxy(startowa_x + x, startowa_y + y);
			putch(tab[y][x]);
		}
		
	}
}


void przesun(Kierunek_en kierunek, int *dx, int *dy)
{
	switch (kierunek)
	{
	case UP:
		*dy = -1;
		break;

	case DOWN:
		*dy = 1;
		break;

	case RIGHT:
		*dx = 1;
		break;

	case LEFT:
		*dx = -1;
		break;
	}
}


void rysujEdytor(Edytor_str* edytor,Labirynt_str* lab) 
{
	gotoxy(edytor->x, edytor->y );
	putch('+');
	gotoxy(lab->start_x, lab->start_y);
	putch('s');
	gotoxy(lab->end_x, lab->end_y);
	putch('k');
}


void rzutuj(Widok_str* widok, Labirynt_str* lab, Gracz_str* gracz)
{
	int dx = 0;
	int dy = 0;
	int dw = 0;
	int dh = 0;
	int d_osX = 0;
	int d_osY = 0;
	przesun(gracz->kierunek, &dx, &dy);

	switch (gracz->kierunek)  //okreslenie pomocniczych dla wybrania strony scian i odpowiednich scian w widoku 3D
	{
	case UP:
		dw = -1;
		d_osY = 1;
		break;

	case DOWN:
		dw = 1;
		d_osY = -1;
		break;

	case RIGHT:
		dh = -1;
		d_osX = -1;
		break;

	case LEFT:
		dh = 1;
		d_osX = 1;
		break;
	}

	widok->e[7] = SCIANA;      // Srodkowy element jest okreslany jako sciana, zeby tez m�c go w roznych sytuacjach wypelniac
	widok->t[7] = ' ';
	int x = gracz->x + dx;
	int y = gracz->y + dy;
	int i = 0;
	char tekstura;

	for (i = 0; i < 7; i++)
	{
		tekstura = podaj(lab, x, y);  //wypelnienie tekstury

		if (czySciana(lab, x, y))
		{
			widok->t[7] = tekstura;
			break;
		}

		widok->e[i] = czySciana(lab, x + dw, y + dh) ? SCIANA : BRAK;
		if (widok->e[i] == SCIANA)
		{
			widok->t[i] = podaj(lab, x + dw, y + dh);
		}

		widok->e[14 - i] = czySciana(lab, x - dw, y - dh) ? SCIANA : BRAK;
		if (widok->e[14 - i] == SCIANA)
		{
			widok->t[14 - i] = podaj(lab, x - dw, y - dh);
		}

		widok->p[i] = czySciana(lab, x + d_osX, y + d_osY) ? ZAJETE : PUSTE;
		if (widok->p[i] == ZAJETE)
		{
			widok->t[i] = podaj(lab, x + d_osX, y + d_osY);
		
		}
		x += dx;
		y += dy;
	}

	while (i < 7)
	{
		widok->p[i] = ZAJETE;
		widok->e[i] = SCIANA;
		widok->t[i] = tekstura;
		widok->e[14 - i] = SCIANA;
		widok->t[14 - i] = tekstura;
		i++;
	}
}


void rysujWidok(Widok_str* widok)
{
	int x = 30;  //NA TYCH POZYCJACH ZACZYNAM 
	int y = 2;                     //RYSOWAC WIDOK 3D
	int h;
	int w = 0;
	char tekstura = ' '; 
	int nrSciany = 1;  //najmniejsza sciana ( ta posrodku ) ma nr 1

	for (int i = 7; i >= 0; i--)   //zaczynam rysowanie lewej strony  ( od srodka do poczatku )
	{
		if (widok->e[i] == SCIANA)
		{
			h = nrSciany * 2 + 1;  //3, 5, 7 .... , 17 - wysokosci scian
			tekstura = widok->t[i];
			rysujSciane(x + i * 3, y, h, false, true, false, tekstura);

			if (tekstura == ' ')
			{
				rysujSciane(x + i * 3, y, h, false, true, true, tekstura);
			}
		}
		else
		{
			rysujSciane(x + i * 3, y, h, true, true, false, tekstura);
			if (tekstura == ' ')
			{
				rysujSciane(x + i * 3, y, h, true, true, true, tekstura);
			}
		}
		nrSciany++;
	}

	h = 3;
	nrSciany = 2;
	tekstura = ' ';
	for (int i = 8; i < 15; i++)   //rysowanie prawej strony
	{
		if (widok->e[i] == SCIANA)
		{
			h = nrSciany * 2 + 1;
			tekstura = widok->t[i];
			rysujSciane(x + i * 3, y, h, false, false, false, tekstura);

			if (tekstura == ' ')
			{
				rysujSciane(x + i * 3, y, h, false, false, true, tekstura);
			}
		}
		else
		{
			rysujSciane(x + i * 3, y, h, true, false, false, tekstura);

			if (tekstura == ' ')
			{
				rysujSciane(x + i * 3, y, h, true, false, true, tekstura);
			}
		}
		nrSciany++;
	}

	h = 3;
	nrSciany = 8;
	tekstura = ' ';
	for (int i = 0; i < 7; i++)  //rysowanie przedluzenia w zaleznosci od wysokosci i szerokosci kolumn po bokach
	{
		if (widok->p[i] == ZAJETE)
		{
			w = nrSciany * 6 - 2;
			h = nrSciany * 2 + 1;
			tekstura = widok->t[i];
			rysujPrzedluzenie(x, y, h, w, tekstura);
			break;
		}
		nrSciany--;
	}
}


void skrec(Gracz_str* gracz, Kierunek_en kierunek) 
{
	if (kierunek == LEFT)                          //jesli obracamy sie w lewo 
	{                                         
		if (gracz->kierunek == LEFT)          // i jesli gracz skierowany jest w lewa strone, to ...
		{
			gracz->kierunek = DOWN;               // ustaw jego skierowanie w dol      
		}
		else if (gracz->kierunek == RIGHT)
		{
			gracz->kierunek = UP;
		}
		else if (gracz->kierunek == UP)
		{
			gracz->kierunek = LEFT;
		}
		else if (gracz->kierunek == DOWN)
		{
			gracz->kierunek = RIGHT;
		}		
	}

	else if (kierunek == RIGHT) 
	{
		if (gracz->kierunek == LEFT)
		{
			gracz->kierunek = UP;
		}
		else if (gracz->kierunek == RIGHT)
		{
			gracz->kierunek = DOWN;
		}
		else if (gracz->kierunek == UP)
		{
			gracz->kierunek = RIGHT;
		}
		else if (gracz->kierunek == DOWN)
		{
			gracz->kierunek = LEFT;
		}
	}
}


void idz(Gracz_str* gracz, Kierunek_en kierunek) 
{
	int dx = 0; 
	int dy = 0;
	Labirynt_str* lab = gracz->lab;
	przesun(gracz->kierunek, &dx, &dy);
	
	if (kierunek == DOWN)
	{
		dy = -dy;                      // Przesuniecia maja przeciwne wartosci (kierunek zmienia sie o 90stopni )
		dx = -dx;
	}

	if (!czySciana(lab, gracz->x + dx, gracz->y + dy))         // Jesli nie ma sciany
	{
		gracz->x += dx;                                    //  pozycja gracza zmienia si� o przesuniecie dx w osi poziomej 
		gracz->y += dy;                                        // i o przesuniecie dy w osi pionowej
	}
	
	
	gracz->liczbaRuchow++;
}


void rysujGracza(Gracz_str* gracz)
{
	gotoxy(gracz->x, gracz->y);
	switch (gracz->kierunek)
	{
	case UP:
		putch(CHAR_UP);
		break;

	case DOWN:
		putch(CHAR_DOWN);
		break;

	case LEFT:
		putch(CHAR_LEFT);
		break;

	case RIGHT:
		putch(CHAR_RIGHT);
		break;
	}

	gotoxy(1, 22);
	cputs("czas gry: ");
	cout << podajCzasGry(gracz);
	gotoxy(1, 23);
	cputs("Liczba ruchow: ");
	cout << gracz->liczbaRuchow;
}


void wstawZnak(Edytor_str* edytor, Labirynt_str* lab, char znak) {
	int x = edytor->x;
	int y = edytor->y;

	if (znak == ' ')   //warunek wykluczajacy rysowanie pol 2x2 (sprawdza trzy pola wokol gracza)
	{

		if (!czySciana(lab, x - 1, y) && !czySciana(lab, x - 1, y - 1) && !czySciana(lab, x, y - 1))
			return;
		if (!czySciana(lab, x + 1, y) && !czySciana(lab, x + 1, y - 1) && !czySciana(lab, x, y - 1))
			return;

		if (!czySciana(lab, x - 1, y) && !czySciana(lab, x - 1, y + 1) && !czySciana(lab, x, y + 1))
			return;

		if (!czySciana(lab, x + 1, y) && !czySciana(lab, x + 1, y + 1) && !czySciana(lab, x, y + 1))
			return;
	}

	lab->tab[edytor->x - 1][edytor->y - 1] = znak;
}


void powitanie()
{
	int zn, attr = 7;
	gotoxy(50, 1);
	cputs("WITAMY W GRZE LABIRYNT");
}


void wyswietlPomoc()
{
	clrscr();
	textcolor(7);
	gotoxy(20, 8);
	cputs("POMOC:");
	gotoxy(20, 9);
	cputs("q = wyjscie");
	gotoxy(20, 10);
	cputs("e = edytor ON/OFF");
	gotoxy(20, 11);
	cputs("Edytor: 1,2,3,4 -> sciana, spacja, start, koniec");
	gotoxy(20, 12);
	cputs("r = reset gry");
	gotoxy(20, 13);
	cputs("s = zapisanie do pliku");
	gotoxy(20, 14);
	cputs("o = wczytanie z pliku");
	gotoxy(20, 15);
	cputs("strzalki do przodu i do tylu = poruszanie");
	gotoxy(20, 16);
	cputs("strzalki w lewo i w prawo = skrecanie");
	getch();
}


void zapiszDoPliku(Labirynt_str* lab)
{
	char nazwa[32];
	gotoxy(30, 20);
	cputs("Podaj nazwe do zapisu: ");
	cin >> nazwa;
	zapisz(lab, nazwa);
}


void wczytajZpliku(Labirynt_str* lab, Gracz_str* gracz)
{
	char nazwa[32];
	gotoxy(30, 21);
	cputs("Podaj nazwe pliku wczytywanego: ");
	cin >> nazwa;
	laduj(lab, gracz, nazwa);
}


void menu(Gra_str* gra, Labirynt_str* lab, Gracz_str* gracz, Edytor_str* edytor)
{
	int zn = getch();
	int attr = 7;

	if (zn == 's')
	{
		zapiszDoPliku(lab);
	}

	if (zn == 'o')
	{
		wczytajZpliku(lab, gracz);
	}

	if (zn == 'i')
	{
		lab->ON = true;
		laduj(lab, gracz, "lab_przykladowy.txt");
	}

	if (zn == 'r')
	{
		ustawPozycjeGracza(gracz, lab);
	}

	if (zn == 'h')
	{
		wyswietlPomoc();
	}

	if (zn == 'e')
	{
		edytor->ON = !edytor->ON;
		lab->ON = !lab->ON;
	}

	if (zn == 'q')
	{
		gra->ON = false;
	}

	if (edytor->ON)
	{
		if (zn == 0x48) edytor->y--;
		else if (zn == 0x50) edytor->y++;
		else if (zn == 0x4b) edytor->x--;
		else if (zn == 0x4d) edytor->x++;
		else if (zn == '1') wstawZnak(edytor, lab, '#');
		else if (zn == '2') wstawZnak(edytor, lab, ' ');
		else if (zn == '3')
		{
			lab->start_x = edytor->x;
			lab->start_y = edytor->y;
		}
		else if (zn == '4')
		{
			lab->end_x = edytor->x;
			lab->end_y = edytor->y;
		}
		else if (zn == 'n')

			podajRozmiar(lab);
	}

	else if (lab->ON)
	{
		if (zn == 0x48) idz(gracz, UP);
		else if (zn == 0x50) idz(gracz, DOWN);
		else if (zn == 0x4b) skrec(gracz, LEFT);
		else if (zn == 0x4d) skrec(gracz, RIGHT);
	}
}


void rysowanie(Labirynt_str* lab, Gracz_str* gracz, Widok_str* widok, Edytor_str* edytor)
{
	rysujLabirynt(lab);

	if (edytor->ON == false && lab->ON == true)
	{
		rysujGracza(gracz);
		rzutuj(widok, lab, gracz);
		rysujWidok(widok);
		if (gracz->x == lab->end_x && gracz->y == lab->end_y)
		{
			lab->ON = false;
		}
	}

	else if (lab->ON == false && edytor->ON == true)
	{
		rysujEdytor(edytor, lab);
	}
}