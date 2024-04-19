//POP_2020_12_07_projekt_1_Szczerba_Katarzyna_IBM_1_184385  DEV-C++ 5.11  Kompilator:TDM-GCC 4.9.2 64-bit

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string.h>

using namespace std;

const int liczba_dolkow = 12;
const int liczba_baz = 2;
const int liczba_dolkow_jeden_rzad = liczba_dolkow / 2; 
const int liczba_na_start_dolkow = 6;
const int liczba_na_start_baz = 0;
const int dolek_szer = 11;
const int dolek_wys = 6;
const int baza_szer = 12;
const int baza_wys = 8;
const int wysokosc_planszy = dolek_wys*2;
//zakladamy ze dolki beda elementami tablicy jednowymiarowej o 12 polach
//bazy beda tablica o 2 elementach
const int indeks_startowy_rzedu_gracza_tablica=6;
const int indeks_startowy_rzedu_komputera_tablica=0;
const int indeks_bazy_gracza_tablica=1;
const int indeks_bazy_komputera_tablica=0;
const int pierwszy_ind_tab_dolkow=0;
const int ostatni_ind_tab_dolkow=liczba_dolkow-1;
const int rzad_dolny_pierwszy_ind=6;
const int rzad_gorny_ostatni_ind=5;


int dlugosc_liczby(int liczba);
void rysuj_fragment(int wysokosc, int szerokosc, int pozycja_y, int liczba);
void wyswietl_plansze(int dolki[liczba_dolkow], int bazy[liczba_baz]);
void wypelniacz_tablicy(int tab[], const int wypelnienie,const int rozmiar);
int wyjmij_kamienie(int dolki[liczba_dolkow], int indeks);
void osadz_kamienie_w_dolnym_rzedzie(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz]);
void osadz_kamienie_w_gornym_rzedzie(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz]);
void osadz_kamienie_krance_tablicy(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz], int &dodatkowa_tura);
void osadz_kamienie(int dolki[liczba_dolkow], int indeks_dolka, int bazy[liczba_baz], int indeks_wlasnej_bazy, int &czyjs_ruch);
int znajdz_numer_dolka(char oznaczenie_dolka);
bool poprawnosc_wejscia(char tekst[]);
bool czy_mozna_wykonac_ruch(int dolki[liczba_dolkow], int indeks_startowy_wlasnego_rzedu);
void koniec_gry(const int indeks_wlasnej_bazy, int dolki[liczba_dolkow], int bazy[liczba_baz]);
void co_po_koncu(int &co_robic );
void delay(int sekundy);
void poczatek_gry_ustawianie(int dolki[liczba_dolkow], int bazy[liczba_baz]);
bool wykonaj_ruch_gracza(int dolki[liczba_dolkow], int bazy[liczba_baz], int &ruch_gracza);
bool wykonaj_ruch_komputera(int dolki[liczba_dolkow], int bazy[liczba_baz], int &ruch_komputera);


int main()
{
    srand(time(NULL));
    int program_co_robic=3; //wartosc=3 gra w toku, wartosc=2 po skonczeniu jeszcze raz, wartosc=1 wyjscie z programu
    while (program_co_robic==3 or program_co_robic==2) 
    {
    	system("CLS");//do czyszczenia okna gdy gracz chce zagrac jeszcze raz
        program_co_robic=3;//jesli gracz chcial zagrac jeszcze raz po skonczeniu zmieniamy stan na gre w toku
        int dolki_tablica[liczba_dolkow];
        int bazy_tablica[liczba_baz];
		poczatek_gry_ustawianie(dolki_tablica, bazy_tablica);

        while (true)
        {
            int ruch_gracza = 1;
            while (ruch_gracza > 0)
            {
                if (wykonaj_ruch_gracza(dolki_tablica, bazy_tablica, ruch_gracza) == false)
                {
                    koniec_gry(indeks_bazy_gracza_tablica,dolki_tablica, bazy_tablica);
					co_po_koncu(program_co_robic);
                    break;
                }
            }
            
            if (program_co_robic<3) break;

            int ruch_komputera = 1;
            while (ruch_komputera > 0)
            {
                if (wykonaj_ruch_komputera(dolki_tablica, bazy_tablica, ruch_komputera) == false)
                {
                    koniec_gry(indeks_bazy_komputera_tablica,dolki_tablica, bazy_tablica);
					co_po_koncu(program_co_robic);
                    break;
                }
            }
            
            if (program_co_robic<3) break;
        }
    }
}


void poczatek_gry_ustawianie(int dolki[liczba_dolkow], int bazy[liczba_baz])
{
		wypelniacz_tablicy(dolki, liczba_na_start_dolkow, liczba_dolkow);
        wypelniacz_tablicy(bazy, liczba_na_start_baz, liczba_baz);
        cout <<"Oto rzut z gory planszy do gry w kalaha'e. Dolki nalezace do ciebie znajduja sie na dole rysunku, a twoja baza po prawej."<<endl<<"Rozpoczynasz gre z komputerem. Powodzenia!"<<endl;
        wyswietl_plansze(dolki, bazy);
        cout<<endl;
}


void wypelniacz_tablicy(int tab[], const int wypelnienie,const int rozmiar) //wypelnia tablice wybrana liczba startowa
{
	for (int i = 0; i < rozmiar; i++)
	{
		tab[i] = wypelnienie;
	}
}


void wyswietl_plansze(int dolki[liczba_dolkow], int bazy[liczba_baz])
{
	int aktualizacja_indeksow = 0; // zaczynamy od gornego rzedu planszy a wniej rysujemy elementy tablicy dolkow od 0 do 5, pozniej do startowego dodamy 6

	for (int y = 0; y < wysokosc_planszy; y++) //rysujemy tyle wierszy ile ma wysokosc planszy, y jest licznikiem przemieszczenia od pierwszej linii
	{
		if (y == dolek_wys) aktualizacja_indeksow =liczba_dolkow_jeden_rzad; //sprawdzamy czy gorny rzad planszy juz jest narysowany, czyli czy wlasnie zwiekszamy startowy indeks rzedu tablicy dolkow o 6

		if (y >= ((wysokosc_planszy - baza_wys)/2) and y <= (wysokosc_planszy - 1 - (wysokosc_planszy - baza_wys)/2 )) //jesli spelnimy warunek rysujemy lewa baze
		{
			rysuj_fragment(baza_wys, baza_szer, (y - ((wysokosc_planszy - baza_wys)/2)), bazy[indeks_bazy_komputera_tablica]); //funkcja okresla jaki charakterystyczny fragment obiektu rysowac
			cout << ' ';//spacja po narysowaniu lewej bazy zeby przed dolkami byl przerwa
		}
		else
		{
			for (int i = 0; i < baza_szer + 1; i++) cout << ' ';  //jesli nie spelnimy warunku rysowania bazy to w tym miejscu wypisujemy spacje az pominiemy szerokosc bazy
		}
		
		for (int i = 0; i < liczba_dolkow_jeden_rzad; i++) // tyle razy rysujemy charakterystyczny fragment dolka ile jest dolkow w rzedzie
		{
			rysuj_fragment(dolek_wys, dolek_szer, y, dolki[i + aktualizacja_indeksow]); //przekazujemy parametry wielkosciowe dolka funckji aby okreslila ona ktory fragment szkicu dolka rysowac
			cout << ' '; //po kazdym dolku mamy przerwe
		}
		
		if ((y >= ((wysokosc_planszy - baza_wys)/2)) and y <= (wysokosc_planszy - 1 - (wysokosc_planszy - baza_wys)/2)) //rysujemy prawa baze jesli jestesmy na dobrej wspolrzednej y
		{
			rysuj_fragment(baza_wys, baza_szer, (y - (wysokosc_planszy - baza_wys) / 2), bazy[indeks_bazy_gracza_tablica]);
		}

		cout << endl;// przechodzimy do kolejnego wersu
	}

	cout << "                  a           b           c           d           e           f" << endl;  //po narysowaniu calej planszy oznaczenia dolkow
}


void rysuj_fragment(int wysokosc, int szerokosc, int pozycja_y, int liczba)
{
	if (pozycja_y >= wysokosc)  pozycja_y = pozycja_y - wysokosc; //w przypadku baz funkcji juz przekazalismy zrelatywizowany y, ten fragment odnosi sie do dolkow
	int dodatkowe_miejsce_dla_liczby = 0; //uzywane gdy liczba dluzsza niz 1 znak do srodkowania jej w obiekcie
	int a = dlugosc_liczby(liczba);

	for (int x = 0; x < szerokosc; x++) // x to wspolrzedna w poziomie, poruszamy od lewej do prawej przy rysowaniu
	{
		if (dodatkowe_miejsce_dla_liczby > 0) //jesli musimy zagospodarowac wiecej mniejsca dla liczby to pomijamy dana wspolrzedna x zeby finalnie narysowac mniej bialych znakow
			{ 	
				dodatkowe_miejsce_dla_liczby--; 
				continue;
			}
			 
		if ((x == 0 or x == szerokosc-1) and (pozycja_y == 0 or pozycja_y == wysokosc-1)) cout << '+'; // rysowanie rogow obiektu

		else if (pozycja_y == 0 or pozycja_y == wysokosc-1) cout << '-'; //rysowanie gornej i dolnej krawedzi obiektu bez rogow

		else if (x == 0 or x == szerokosc-1)  cout << '|';  //rysowanie lewej i prawej krawedzi obiektu bez gornego i dolnego boku

		else if (x == (szerokosc - a + 1)/2 and pozycja_y == wysokosc/2) // gdy jestesmy posrodku wysokosci obiektu oraz posrodku szerokosci obiektu po uwzglednienu dlugosci liczby z tablicy wyswietlamy ten element, 
		{
			cout << liczba;
			dodatkowe_miejsce_dla_liczby = a - 1;
		}

		else  cout << ' ';  //jesli zadne ze szczegolnych miejsc w obiekcie rysujemy bialy znak
	}

}


int dlugosc_liczby(int liczba)
{
	int licznik_dlugosci = 0;

	do
	{
		++licznik_dlugosci;
		liczba /= 10;
	} while (liczba); //przerywa gdy liczba po zaaokragleniu do int daje 0

	return licznik_dlugosci;
}


bool wykonaj_ruch_gracza(int dolki[liczba_dolkow], int bazy[liczba_baz], int &ruch_gracza)
{
	int max=3;
	char kontrola_tab[max]={'1','1','1'};

	if (czy_mozna_wykonac_ruch(dolki, indeks_startowy_rzedu_gracza_tablica))
	{
		cout << "Twoj ruch. Wybierz dolek, z ktorego chcesz zabrac wszystkie kamienie (a-f): ";
		cin.getline(kontrola_tab,max);
		cin.clear();
		cin.sync();
		
		while (poprawnosc_wejscia(kontrola_tab) == false or dolki[znajdz_numer_dolka(kontrola_tab[0])] == 0)
		{
			cout << "Bledne dane. ";
			if (dolki[znajdz_numer_dolka(kontrola_tab[0])] == 0 and poprawnosc_wejscia(kontrola_tab)==true) cout << "Nie mozesz tego zrobic, w dolku jest zero kamieni. ";
			cout << endl << "Wybierz dolek, z ktorego chcesz zabrac wszystkie kamienie (a-f): ";
			cin.getline(kontrola_tab,max);
			cin.clear();
			cin.sync();
		}

		int numer_dolka = znajdz_numer_dolka(kontrola_tab[0]);
		osadz_kamienie(dolki, numer_dolka, bazy, indeks_bazy_gracza_tablica , ruch_gracza);
		cout <<endl << "Oto wyglad planszy po twoim ruchu: " << endl;
		wyswietl_plansze(dolki, bazy);

		ruch_gracza--;
		if (ruch_gracza > 0) cout << endl << "Dodatkowa tura! Ostatni kamien wpadl do wlasnej bazy gracza. "<<endl;
		return true;
	}
	else
	{
		return false;
	}
}


bool czy_mozna_wykonac_ruch(int dolki[liczba_dolkow], int indeks_startowy_wlasnego_rzedu)
{
	int suma = 0;//suma kamieni we wszystkich danego rzedu dolkach
	for (int i = 0; i < liczba_dolkow_jeden_rzad; i++)
	{

		suma = suma + dolki[indeks_startowy_wlasnego_rzedu + i];

	}
	if (suma > 0) return true;

	else return false;
}


bool poprawnosc_wejscia(char tekst[])
{
	if(tekst[1]!='\0')
	{
		return false;
	}
	
	char pierwszy_znak = tekst[0]; 

	if (pierwszy_znak == 'a' or pierwszy_znak == 'b' or pierwszy_znak == 'c' or pierwszy_znak == 'd' or pierwszy_znak == 'e' or pierwszy_znak == 'f')
	{
		return true;
	}
	else
	{
		return false;
	}
}


int znajdz_numer_dolka(char oznaczenie_dolka) //"konwertuje" litere podana przez gracza na wejsciu na indeks tablicy dolkow
{
	switch (oznaczenie_dolka) {
	case 'a': 
		return indeks_startowy_rzedu_gracza_tablica;
	case 'b':
		return indeks_startowy_rzedu_gracza_tablica+1;
	case 'c':
		return indeks_startowy_rzedu_gracza_tablica+2;
	case 'd':
		return indeks_startowy_rzedu_gracza_tablica+3;
	case 'e':
		return indeks_startowy_rzedu_gracza_tablica+4;
	case 'f':
		return indeks_startowy_rzedu_gracza_tablica+5;
	}
}


void osadz_kamienie(int dolki[liczba_dolkow], int indeks_dolka, int bazy[liczba_baz], int indeks_wlasnej_bazy, int &czyjs_ruch)
{
	int dodatkowa_tura = 0;//w trakcie gry moze sie okazac, ze gracz otrzyma dodatkowa ture, funkcja ta zwraca wiec wartosc tego licznika
	int pula_kamieni = wyjmij_kamienie(dolki, indeks_dolka);
	while (pula_kamieni > 0)//tutaj okreslam kierunek iteracji oraz postepowanie w przypadkach szczeg�lnych, czyli gdy pole poprzedza baze, calosc ma sie wykonywac az skoncza sie kamienie do rozlozenia
	{

		if (indeks_dolka >= rzad_dolny_pierwszy_ind and indeks_dolka <ostatni_ind_tab_dolkow and pula_kamieni != 0) osadz_kamienie_w_dolnym_rzedzie(indeks_wlasnej_bazy, pula_kamieni, dolki, indeks_dolka, bazy);

		if (indeks_dolka > pierwszy_ind_tab_dolkow and indeks_dolka <= rzad_gorny_ostatni_ind and pula_kamieni != 0) osadz_kamienie_w_gornym_rzedzie(indeks_wlasnej_bazy, pula_kamieni, dolki, indeks_dolka, bazy);

		if ((indeks_dolka == pierwszy_ind_tab_dolkow or indeks_dolka == ostatni_ind_tab_dolkow) and pula_kamieni > 0) osadz_kamienie_krance_tablicy(indeks_wlasnej_bazy, pula_kamieni, dolki, indeks_dolka, bazy, dodatkowa_tura);

	}
	czyjs_ruch = czyjs_ruch + dodatkowa_tura; //informacja przekazywana potem dalej innym funkcjom decydujacym o tym czyj jest ruch


}


int wyjmij_kamienie(int dolki[liczba_dolkow], int indeks) //funkcja pomocnicza dla osadz kamienie, przekazuje informacje ile jest kamieni do rozlozenia i zeruje dolek z ktorego wyjmuje
{
	int bufor = dolki[indeks];
	dolki[indeks] = 0;
	return bufor;
}


void osadz_kamienie_w_dolnym_rzedzie(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz])
{
	while (indeks_dolka >= rzad_dolny_pierwszy_ind and indeks_dolka < ostatni_ind_tab_dolkow and pula_kamieni != 0)//w tej czesci tablicy indeksy przy rozkladaniu sa zwiekszane,poruszamy sie po dolnym rzedzie planszy
	{
		//tu zastosowanie zasady �e jak ostatni kamien ma wpasc do mojego dolka w ktorym jest 0 kamieni to zabieram kamienie komputera z przeciwnego dolka i wkladam do swojej bazy
		if (pula_kamieni == 1 and dolki[indeks_dolka + 1] == 0 and indeks_wlasnej_bazy == indeks_bazy_gracza_tablica)
		{
			int bufor = dolki[indeks_dolka + 1 - liczba_dolkow_jeden_rzad];
			dolki[indeks_dolka + 1 - liczba_dolkow_jeden_rzad] = 0;
			bazy[indeks_wlasnej_bazy] = bazy[indeks_wlasnej_bazy] + bufor;
		}

		indeks_dolka++; //przechodzimy do kolejnego dolka bo mamy jeszcze kamienie do rozlozenia
		dolki[indeks_dolka]++; //dodajemy 1 kamien do tego dolka
		pula_kamieni--; //odejmujemy jeden od puli

	}
}


void osadz_kamienie_w_gornym_rzedzie(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz])
{
	while (indeks_dolka > pierwszy_ind_tab_dolkow and indeks_dolka <= rzad_gorny_ostatni_ind and pula_kamieni != 0)//w tej czesci tablicy indeksy przy rozkladaniu sa zmniejszane
	{
		if (pula_kamieni == 1 and dolki[indeks_dolka - 1] == 0 and indeks_wlasnej_bazy == indeks_bazy_komputera_tablica)//zasada zabierania kamieni przeciwnika gdy ostatni kamien wpadnie do wlasnego dolka bez kamieni,tu perspektywa komputera
		{
			int bufor = dolki[indeks_dolka - 1 + liczba_dolkow_jeden_rzad];
			dolki[indeks_dolka - 1 + liczba_dolkow_jeden_rzad] = 0;
			bazy[indeks_wlasnej_bazy] = bazy[indeks_wlasnej_bazy] + bufor;
		}

		indeks_dolka--;
		dolki[indeks_dolka]++;
		pula_kamieni--;
	}
}


void osadz_kamienie_krance_tablicy(const int indeks_wlasnej_bazy, int &pula_kamieni, int dolki[liczba_dolkow], int &indeks_dolka, int bazy[liczba_baz], int &dodatkowa_tura)
{
	//rozlozenie kamieni z dolka nr 0 do bazy lewej i przejscie do dolnej czesci tablicy
	if (indeks_dolka == pierwszy_ind_tab_dolkow)
	{
		//tutaj zasada ze jak ostatni kamien wpada do wlasnej bazy komputera to otrzymuje on kolejny ruch
		if (indeks_wlasnej_bazy == indeks_bazy_komputera_tablica and pula_kamieni == 1)
		{
			dodatkowa_tura++;
		}

		bazy[indeks_bazy_komputera_tablica]++;
		pula_kamieni--;
		indeks_dolka = indeks_dolka + liczba_dolkow_jeden_rzad; //przejscie do kolejnego rzedu gdzie pierwszy indeks to 6
		
		if (pula_kamieni != 0)//dodajemy od razu do dolka nr 6 jesli jeszcze zostaly nam jakies kamienie do rozlozenia
		{
			dolki[indeks_dolka]++;
			pula_kamieni--;
		}
	}
	
	//rozlozenie kamieni z dolka nr 11 do bazy prawej i przejscie do gornej czesci tablicy
	if (indeks_dolka == ostatni_ind_tab_dolkow)
	{
		//tutaj zasada ze ostatni kamien gracza wpada do wlasnej bazy =dodatkowa tura
		if (indeks_wlasnej_bazy == indeks_bazy_gracza_tablica and pula_kamieni == 1)
		{
			dodatkowa_tura++;
		}

		bazy[indeks_bazy_gracza_tablica]++;
		pula_kamieni--;
		indeks_dolka = indeks_dolka - liczba_dolkow_jeden_rzad; //przejscie do poprzedniego rzedu
		
		if (pula_kamieni != 0)//dodajemy od razu do dolka nr 5 jesli jeszcze sa jakies kamienie
		{
			dolki[indeks_dolka]++;
			pula_kamieni--;
		}
	}
}


bool wykonaj_ruch_komputera(int dolki[liczba_dolkow], int bazy[liczba_baz], int &ruch_komputera)
{
	if (czy_mozna_wykonac_ruch(dolki,indeks_bazy_komputera_tablica))
	{

		int numer_dolka = rand() % liczba_dolkow_jeden_rzad;//losowanie od 0 do 5 wlacznie
		while (dolki[numer_dolka] == 0) numer_dolka = rand() % liczba_dolkow_jeden_rzad; //gdy wylosowany dolek pusty, powtorne losowanie
		osadz_kamienie(dolki, numer_dolka, bazy, indeks_bazy_komputera_tablica, ruch_komputera);
		delay(3); //symulacja wizualna myslenia komputera
		cout << endl << "Ruch komputera. Plansza po ruchu:  " << endl;
		wyswietl_plansze(dolki, bazy);
		cout<<endl;
		
		ruch_komputera--;
		if (ruch_komputera > 0) cout << endl << "Dodatkowa tura komputera! Ostatni kamien wpadl do jego wlasnej bazy. ";
		return true;
	}
	else
	{
		return false;
	}
}


void delay(int sekundy){

int milisekundy = 1000 * sekundy;

clock_t startczas = clock();

while(clock() < (startczas + milisekundy));

}


void koniec_gry(const int indeks_wlasnej_bazy, int dolki[liczba_dolkow], int bazy[liczba_baz])//nastepuje okreslanie zwyciescy i wyswietlanie koncowej planszy
{
    if (indeks_wlasnej_bazy==indeks_bazy_gracza_tablica) cout<<endl<<"Gra skonczona, nie mozesz wykonac ruchu. Plansza po przelozeniu pozostalych w dolkach kamieni do bazy: "<<endl<<endl;
	else cout<<endl<<"Gra skonczona, komputer nie moze wykonac ruchu. Plansza po przelozeniu pozostalych w dolkach kamieni do bazy: "<<endl<<endl;
	
	int wynik_komputera = bazy[indeks_bazy_komputera_tablica];
	for (int i = indeks_startowy_rzedu_komputera_tablica; i < liczba_dolkow_jeden_rzad; i++)
	{
		wynik_komputera += dolki[i];
	}

	bazy[indeks_bazy_komputera_tablica] = wynik_komputera;

	int wynik_gracza = bazy[indeks_bazy_gracza_tablica];
	for (int i = indeks_startowy_rzedu_gracza_tablica; i < liczba_dolkow; i++)
	{
		wynik_gracza += dolki[i];
	}

	bazy[indeks_bazy_gracza_tablica] = wynik_gracza;

	wypelniacz_tablicy(dolki, 0,liczba_dolkow); //czyszczenie tablicy z pozostalosci, zastepowanie zerami

	wyswietl_plansze(dolki, bazy);

	if (wynik_gracza > wynik_komputera) cout << endl << "Zwyciestwo! ";
	if (wynik_gracza == wynik_komputera) cout << endl << "Remis. ";
	if (wynik_gracza < wynik_komputera) cout << endl << "Przegrana. ";
}

void co_po_koncu(int &co_robic )
{
	char klawisz[3];
	cout<<"Jesli chcesz wyjsc wcisnij 'q', jesli chcesz zagrac jeszcze raz wcisnij dowolny inny klawisz: ";
    cin.getline(klawisz,3);
    cin.clear();
    cin.sync();
    if (klawisz[0]=='q' and klawisz[1]=='\0') co_robic=1;
    else co_robic=2;	
} 


