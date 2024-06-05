#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>
#include "struktura.h"

#define PI 3.14159

//Funkcja odpowiedzialna za stworzenie 10 pracownikow, ustawienie ich identyfikatorów i początkowej dostępności
void utworzPracownikow(Pracownik* pracownicy, int rozmiar) {

    char* imiona[10] = { "Alicja", "Agata", "Marek", "Janusz", "Piotr", "Ewelina", "Tomasz", "Maria", "Magda", "Grzegorz" };

    for (int i = 0; i < rozmiar; i++) {
        snprintf(pracownicy[i].Imie, sizeof(pracownicy[i].Imie), "%s", imiona[i]);
        pracownicy[i].Identyfikator = i + 1;
        pracownicy[i].Dostepnosc = wolny;
        pracownicy[i].CzasRozmowy = 0.0;
        pracownicy[i].LiczbaRozmow = 0; //zmiana
    }

}
//Funkcja generuje losowa dlugosc polaczenia zgodnie z rozkladem normalnym
double DlugoscPolaczen() {

    double u1;
    double u2;
    double p;

    do {

        u1 = (double)rand() / RAND_MAX;
        u2 = (double)rand() / RAND_MAX;
        p = sqrt(-2 * log(u1)) * cos(2 * PI * u2) * 100;

    } while (p <= 0 || p > 120);

    return p;
}

//Funkcja generuje losowa ilosc przychodzacych polaczen zgodnie z rozkladem Poissona
int PrzychodzacePolaczenia() {

    int lambda = 2;
    double limit = exp(-lambda);
    double p = 1;
    int i = 0;

    do {
        i++;
        p *= (double)rand() / RAND_MAX;

    } while (p > limit);

    return i;

}

//Funkcja dodaje elementy na koniec kolejki
void DodajDoKolejki(Kolejka** head, int number) {
    if (*head == NULL) {
        *head = (Kolejka*)malloc(sizeof(Kolejka));
        if (*head == NULL) {
            printf("Blad alokacji pamięci");
            exit(EXIT_FAILURE);
        }
        (*head)->data = number;
        (*head)->nastepny = NULL;
    }
    else {
        Kolejka* element = *head;
        while (element->nastepny != NULL) {
            element = element->nastepny;
        }
        element->nastepny = (Kolejka*)malloc(sizeof(Kolejka));
        if (element->nastepny == NULL) {
            printf("Blad alokacji pamięci");
            exit(EXIT_FAILURE);
        }
        element->nastepny->data = number;
        element->nastepny->nastepny = NULL;
    }
}

//Funkcja usuwa element z poczatku kolejki
void UsunElementZPoczatkuKolejki(Kolejka** head) {
    Kolejka* temp = NULL;
    if (*head != NULL) {
        temp = (*head)->nastepny;
        free(*head);
        *head = temp;
    }
}

//Funkcja wypisuje cala kolejke
void WypiszKolejke(Kolejka* head) {

    if (head == NULL) {
        printf("Kolejka jest pusta\n");
    }
    else {
        Kolejka* element = head;
        while (element != NULL) {
            printf("%d ", element->data);
            element = element->nastepny;
        }
        printf("\n");
    }
}

//Funkcja zwraca wartosc dlugosci kolejki
int DlugoscKolejki(Kolejka* head) {
    int counter = 0;
    while (head != NULL) {
        counter++;
        head = head->nastepny;
    }
    return counter;
}

//Funkcja generuje przychodze polaczenua za pomoca PrzychodzacePolaczenia(), tworzy klientow, nadaje im identyfikator i przypisuje klienta do danego polaczenia
//Funkcja sprawdza dostepnosc pracownikow, jesli znajdzie dostepnego, przypisuje go do klienta, jesli nie, dodaje klienta do kolejki
//Funkcja zwraca idklienta, aby po nastepnym wywolaniu idklienta nie zaczynalo sie od 0, a od wartosci na ktorej zakonczylo sie ostatnie wywolanie funkcji
int ObslugaPolaczen(Pracownik* pracownicy, int LiczbaPracownikow, Kolejka** kolejka, int idKlientaCounter) {

    int liczbaPolaczen = PrzychodzacePolaczenia();

    for (int i = 0; i < liczbaPolaczen; i++) {

        Klient nowyKlient;
        nowyKlient.IdKlienta = idKlientaCounter++;

        int pracownikZnaleziony = 0;

        for (int j = 0; j < LiczbaPracownikow; j++) {

            if (pracownicy[j].Dostepnosc == wolny) {

                nowyKlient.IdPracownika = pracownicy[j].Identyfikator;

                pracownicy[j].Dostepnosc = zajety;
                pracownicy[j].CzasRozmowy = DlugoscPolaczen();
                pracownicy[j].LiczbaRozmow++;//zmiana

                pracownikZnaleziony = 1;
                break;

            }
        }
        if (!pracownikZnaleziony) {
            DodajDoKolejki(kolejka, nowyKlient.IdKlienta);
        }


    }

    return idKlientaCounter;

}

//Funkcja zbiera dane dotyczace sredniej dlugosci polaczenia potrzebne do statystyk
//Funkcja sprawdza, czy dany pracownik zakonczyl juz rozmowe z klientem, jesli tak - ustawia jego status na "wolny"
//Funkcja sprawdza, czy ktos stoi w kolejce, jesli tak, przypisuje klientow do wolnych pracownikow
double SprawdzPracownikow(Pracownik* pracownicy, int LiczbaPracownikow, Kolejka** kolejka) {

    double suma_rozmow = 0;
    double srednia;
    int przejecie = 0;

    for (int i = 0; i < LiczbaPracownikow; i++) {

        if (pracownicy[i].Dostepnosc = zajety && pracownicy[i].CzasRozmowy <= 0) {
            pracownicy[i].Dostepnosc = wolny;

        }

        if (*kolejka != NULL) {

            przejecie++;
            int IdKlienta = (*kolejka)->data;

            UsunElementZPoczatkuKolejki(kolejka);

            pracownicy[i].Dostepnosc = zajety;
            pracownicy[i].CzasRozmowy = DlugoscPolaczen();
            pracownicy[i].LiczbaRozmow++;//zmiana

            suma_rozmow += pracownicy[i].CzasRozmowy;

            printf("Pracownik %d przejal klienta %d z kolejki, rozmowa bedzie trwac %.02f min\n", pracownicy[i].Identyfikator, IdKlienta, pracownicy[i].CzasRozmowy);


        }


    }
    if (przejecie > 0) {
        srednia = suma_rozmow / przejecie;
    }
    else {
        srednia = 0;
    }

    return srednia;

}
//dodana funkcja
void ZliczRozmowy(Pracownik* pracownicy, int LiczbaPracownikow) {
    int liczbaWszystkichRozmow = 0;
    for (int i = 0; i < LiczbaPracownikow; i++) {
        printf("Pracownik %d (%s) przeprowadzil %d rozmow\n", pracownicy[i].Identyfikator, pracownicy[i].Imie, pracownicy[i].LiczbaRozmow);
        liczbaWszystkichRozmow += pracownicy[i].LiczbaRozmow;
    }
    printf("Laczna liczba rozmow: %d\n", liczbaWszystkichRozmow);
}

//Funkcja symuluje uplyw czasu i wypisuje stan kolejki przez 8 godzin co 15 minut
//Liczy statystyki dotyczace dlugosci przeprowadzanych rozmow
void Symulacja(Pracownik* pracownicy, int liczbaPracownikow) {


    Kolejka* kolejka = NULL;
    int IdKlientaCounter = 1; //i tu zmiana
    double suma_srednich_czasu_rozmowy = 0;
    int licznik = 0;

    for (int t = 0; t < 480; t += 15) {

        licznik++;

        printf("Kolejka po %d min: ", t);

        IdKlientaCounter = ObslugaPolaczen(pracownicy, liczbaPracownikow, &kolejka, IdKlientaCounter);

        WypiszKolejke(kolejka);

        printf("\n");

        for (int i = 0; i < liczbaPracownikow; i++) {
            if (pracownicy[i].Dostepnosc = zajety) {
                pracownicy[i].CzasRozmowy -= 15;
            }
        }


        double temp = SprawdzPracownikow(pracownicy, liczbaPracownikow, &kolejka);
        suma_srednich_czasu_rozmowy += temp;

    }

    printf("\n\n\n");
    printf("Sredni czas rozmowy wynosi: %0.2f min\n\n", suma_srednich_czasu_rozmowy / licznik);
   ZliczRozmowy(pracownicy, liczbaPracownikow);//wywolanie


}

//Funkcja odpowiedzialna za wyswietlanie strony startowej, mozliwosc wyboru wyswietlenia symulacji/pracownikow
void tekst_startowy(Pracownik* pracownicy, int liczbaPracownikow) {



    while (1) {
        printf("Witamy w centrali telefonicznej\n\n\n   Jak mozemy ci pomoc? \n\n");
        printf(" 1. Symulacja\n 2. Lista pracownikow \n 3. Wyjscie\n");

        while (1) {
            char przycisk = _getch();

            if (isalpha(przycisk)) {
                printf("Wpisano litere, wpisz liczbe i sproboj ponownie.\n\n");
                continue;
            }


            switch (przycisk) {
            case '1':
                system("cls");
                printf("Wybrano: \n * Generator liczb *\n\n");
                Symulacja(pracownicy, liczbaPracownikow);
                printf("Powrot do menu.\n\n");
                printf("---------------\n\n\n");
                break;
            case '2':
                system("cls");
                printf("Wybrano: \n * Lista pracownikow *\n\n");

                while (1) {
                    printf("Czy chcesz wyswietlic liste pracownikow? \n\n");
                    printf("1. Tak\n2. Nie\n\n");

                    char wybor = _getch();

                    if (isalpha(wybor)) {
                        printf("Wpisano litere, wpisz liczbe i sproboj ponownie.\n\n");
                        continue;
                    }

                    switch (wybor) {
                    case '1':
                        system("cls");
                        printf("Lista pracownikow: \n\n");
                        for (int i = 0; i < liczbaPracownikow; i++) {
                            printf("%s\n", pracownicy[i].Imie);
                        }
                        printf("\nPowrot do menu.\n\n\n");
                        printf("---------------\n\n\n");
                        break;
                    case '2':
                        system("cls");
                        printf("Powrot do menu.\n\n\n");
                        printf("---------------\n\n\n");
                        break;
                    default:
                        system("cls");
                        printf("Wprowadzono bledna liczbe. Sproboj ponownie.\n\n");

                        continue;
                    }
                    break;
                }
                break;

            case '3':
                system("cls");
                while (1) {
                    printf("Czy na pewno chcesz opuscic program?\n\n");
                    printf("1. Tak\n2. Nie\n\n");
                    char odp = _getch();
                    if (isalpha(odp)) {
                        printf("Wpisano litere, wpisz liczbe i sproboj ponownie.\n\n");
                        continue;
                    }
                    switch (odp) {
                    case '1':
                        printf("Wyjscie z programu\n\n");
                        return;
                    case '2':
                        printf("Powrot do menu.\n\n\n");
                        printf("---------------\n\n\n");
                        break;
                    default:
                        printf("Wprowadzono bledna liczbe. Sproboj ponownie.\n");
                        continue;
                    }
                    break;
                }
                break;
            default:

                printf("Wprowadzono bledna liczbe. Sproboj ponownie.\n\n");
                continue;
            }
            break;
        }
    }
}



int main() {

    srand((unsigned int)time(0));

    Pracownik pracownicy[10];
    utworzPracownikow(pracownicy, 10);

    tekst_startowy(pracownicy, 10);
   

}
