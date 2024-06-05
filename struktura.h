
#include <stdio.h>
#include <stdlib.h>


typedef enum {
	zajety, wolny
}av;


typedef struct {

	char Imie[20];
	int Identyfikator;
	av Dostepnosc;
	double CzasRozmowy;
	int LiczbaRozmow; //zmiana

}Pracownik;

typedef struct {

	int IdKlienta;
	int IdPracownika;

}Klient;

typedef struct Kolejka {

	int data;
	struct Kolejka* nastepny;

} Kolejka;
