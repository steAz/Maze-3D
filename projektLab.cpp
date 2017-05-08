#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "conio2.h"
#include "funkcje.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main()
{
	int attr = 7;
	settitle("Michal Kazanowski 160512");
	textbackground(BLACK);
	Gra_str gra;
	Labirynt_str lab;
	Gracz_str gracz;
	Edytor_str edytor;
	Widok_str widok;
	gracz.lab = &lab;
	widok.lab = &lab;
	edytor.x = 1;  //STARTOWE 
	edytor.y = 1;    // WSPOLRZEDNE KURSORA W EDYTORZE
	gra.ON = true;
	edytor.ON = false;
	lab.ON = false;
	powitanie();

	do
	{
		menu(&gra, &lab, &gracz, &edytor);
		clrscr();
		rysowanie(&lab, &gracz, &widok, &edytor);
	} while (gra.ON);

	return 0;
}

