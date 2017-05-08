#pragma once

#ifndef funkcje_h
#define funkcje_h

#define CHAR_LEFT 17
#define CHAR_RIGHT 16
#define CHAR_UP 30
#define CHAR_DOWN 31
#define PRZOD 0
#define TYL 1


enum Kierunek_en
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};


enum Element_en
{
	SCIANA,
	BRAK
};


enum PolePrzedGraczem_en
{
	ZAJETE,
	PUSTE
};


struct Edytor_str
{
	bool ON;
	int x;
	int y;
};


struct Labirynt_str 
{
	char **tab;
	int height;
	int width;
	int start_x;
	int start_y;
	bool ON;
	int end_x;
	int end_y;
};


struct Gracz_str 
{
	unsigned int czasGry;
	unsigned int liczbaRuchow;
	int x;           
	int y;                
	Kierunek_en kierunek;   
	Labirynt_str* lab;

};


struct Widok_str        
{
	Element_en e[15];  // Podzielenie rysowania na 15 elementow, pierwsze 7 to lewa strona widoku 3D, jeden srodkowy i kolejne 7 to prawa strona
	PolePrzedGraczem_en p[7];          
	char t[15];           //znak do wypelnienia scian w 3D tym samym co z rzutu z gory
	Labirynt_str* lab;
};


struct Gra_str
{
	bool ON;
};




void powitanie();
void menu(Gra_str* gra, Labirynt_str* lab, Gracz_str* gracz, Edytor_str* edytor);
void rysowanie(Labirynt_str* lab, Gracz_str* gra, Widok_str* widok, Edytor_str* edytor);
void wyswietlPomoc();
unsigned int podajCzasGry(Gracz_str* gracz);
void ustawPozycjeGracza(Gracz_str* gracz,Labirynt_str* lab);
bool czySciana(Labirynt_str* lab, int x, int y);
void ustawRozmiar(Labirynt_str* lab, int szerokosc, int wysokosc);
void laduj(Labirynt_str* lab,Gracz_str* gracz, const char * sciezka);
char podaj(Labirynt_str* lab, int x, int y);
void rysujLabirynt(Labirynt_str* lab);
void rzutuj(Widok_str* widok, Labirynt_str* lab, Gracz_str* gracz);
void skrec(Gracz_str* gracz, Kierunek_en kierunek);
void idz(Gracz_str* gracz, Kierunek_en kierunek);
void rysujGracza(Gracz_str* gracz);
void przesun(Kierunek_en kierunek, int *dx, int *dy);
void rysujWidok(Widok_str* widok);
void rysujEdytor(Edytor_str* edytor,Labirynt_str* lab);
void wstawZnak(Edytor_str* edytor,Labirynt_str* lab, char znak);
void zapisz(Labirynt_str* lab, char * sciezka);
void podajRozmiar(Labirynt_str*);
void zapiszDoPliku(Labirynt_str* lab);
void wczytajZpliku(Labirynt_str* lab, Gracz_str* gracz);

#endif
