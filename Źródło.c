#include<stdio.h>
#include<stdbool.h>
#include <SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#define planszax 20   
#define planszay 25
bool zmiana_pozycji(int *t, char kierunek);
SDL_DisplayMode wymiary_ekran;
int jabko_sizex = 0;
int jabko_sizey = 0;
extern void push_back(struct vektor *dane, int a, int *size);
extern int szukaj(int i, struct vektor dane);
extern int* przypisz(int i, struct vektor *dane);
extern void kasuj(struct vektor *dane, int *size, int i);
extern  void destroy_vecktor(struct vektor *dane, int *size);
struct vektor
{
	int x;
	struct vektor *next;
};
struct pozycje
{
	struct vektor x;
	struct vektor y;

};
struct pozycje jabko;
struct dane_wontku_jabko
{
	bool zjedzone_jabko;
	bool *gra_muzyka;
	bool dzałanie_wontku;

};
struct dane_menu
{
	SDL_Surface *czcionka[4];
	SDL_Texture *tekst[4];
	SDL_Rect pozycja_elementuw_menu[4];
	SDL_Color kolor_napisów[4];
	int znacznik;
	char *słowa[4];
	bool gra;
};
struct wontek_muzyka
{
	bool gra;
	bool dzałanie_wontku;
};
struct wontek_czas
{
	char *a;
	bool dzałanie_wontku;
};
struct przebieg_programu
{
	bool zakończ_program;
	bool pokasz_wynik;
	bool koniec_gry_play1 ;
	bool koniec_gry_play2 ;
};
struct wszystkie_wontki_i_dane
{
	struct wontek_czas *czas;
	struct wontek_muzyka *muzyka;
	SDL_Thread *zegar;
	SDL_Thread *muza;

	
};
struct gracz
{
	const char *nazwa_gracza;
	int sizex;
	int sizey;
	struct pozycje *głowy;
	int lidzba_jabłek;
	int t[3];
	struct dane_wontku_jabko *zjedzone_jabko;
	SDL_Thread *sprawdzanie;
	SDL_Thread *ruch_gry;
	SDL_Thread *wynik;
	bool dzałanie_wontków;
	char *dane_obrazu;
	bool *koniec_gry;

};
void pozyskiwanie_wymiarów_ekramu_na_kturym_jest_wyświetlany(SDL_DisplayMode *wymiary_ekran)
{
	SDL_Window  *tmpscreen = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN);
	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(tmpscreen), wymiary_ekran);
	SDL_DestroyWindow(tmpscreen);
}
struct gracze
{
	struct gracz *player1;
	struct gracz *player2;
	char dane_obrazu[planszax][planszay];
};
SDL_ThreadFunction muzyka(struct wontek_muzyka *muza)
{
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
	Mix_Music *tło = Mix_LoadMUS("lib\\muzyka1.mp3");
	bool koniec = false;
	bool a;
	while (muza->dzałanie_wontku)
	{
		if (muza->gra)
		{
			Mix_PlayMusic(tło, 0);
			muza->gra = false;
		}
		SDL_Delay(10);
	}
	Mix_CloseAudio();
		Mix_Quit();
}
char* zmiana_na_stringa(int lidzba, char *dane)
{
	dane[7] = 48;
	dane[6] = 48;
	dane[5] = 48;

	if (lidzba / 1000 >= 1)
	{
		dane[7] = '9';
		dane[6] = '9';
		dane[5] = '9';
	}
	else
	{
		if (lidzba % 10 != 0)
		{
			dane[7] = (lidzba % 10) + 48;

		}
		if (lidzba % 100 != 0)
		{
			dane[6] = (lidzba % 100) / 10 + 48;
		

		}
		if (lidzba % 1000 != 0)
		{
			dane[5] = (lidzba % 1000) / 100 + 48;
		
		}

	}
	return dane;
}


SDL_ThreadFunction czas(struct wontek_czas *dane)
{
	int i=0,j=0,k=0,suma;
	while (dane->dzałanie_wontku)
	{
		if (dane->a[7] > 47 && dane->a[7] < 57)
		{
			i = dane->a[7] - 47;
			
		}
		else
			if (dane->a[6] > 47 && dane->a[6] < 57)
			{
				j = (dane->a[6] * 10) - 470;
				i = 0;

			}
			else
				if (dane->a[5] > 47 && dane->a[5] < 57)
				{
					k = (dane->a[5] * 100) - 4700;
					i = 0;
					j = 0;
				}

		suma = i + j + k;
		zmiana_na_stringa(suma, dane->a);
		SDL_Delay(1000);
	}

}


void sterowanie_gry_pleyer2(SDL_Event *zdarzenie, struct gracz *player)
{
	char t[4];
	t[0] = 'u';
	t[1] = 'd';
	t[2] = 'l';
	t[3] = 'r';
	if (zdarzenie->type == SDL_KEYDOWN)
	switch (zdarzenie->key.keysym.sym)
	{
	case SDLK_w:
		if (zmiana_pozycji(player, t[0]))
			player->t[2] = 'u';
		break;
	case SDLK_s:
		if (zmiana_pozycji(player, t[1]))
			player->t[2] = 'd';
		break;
	case SDLK_a:
		if (zmiana_pozycji(player, t[2]))
			player->t[2] = 'l';
		break;
	case SDLK_d:
		if (zmiana_pozycji(player, t[3]))
			player->t[2] = 'r';
		break;
	}
}


void sterowanie_gry_pleyer1(SDL_Event *zdarzenie,bool *koniec_gry, struct przebieg_programu *przebieg_programu,struct gracz *player)
{
	char t[4];
	t[0] = 'u';
	t[1] = 'd';
	t[2] = 'l';
	t[3] = 'r';
	if (zdarzenie->type == SDL_KEYDOWN)
		switch (zdarzenie->key.keysym.sym)
		{
		case SDLK_UP:
			if(zmiana_pozycji(player,t[0] ))
				player->t[2] = 'u';
			break;
		case SDLK_DOWN:
			if(zmiana_pozycji( player, t[1]))
				player->t[2] = 'd';
			break;
		case SDLK_LEFT:
			if(zmiana_pozycji( player, t[2]))
				player->t[2] = 'l';
			break;
		case SDLK_RIGHT:
			if(zmiana_pozycji( player, t[3]))
				player->t[2] = 'r';
			break;
		case SDLK_ESCAPE:
			*koniec_gry = true;
			przebieg_programu->pokasz_wynik = false;
			break;
		default:
			break;
		}
}
bool zmiana_pozycji(struct gracz *player, char kierunek )
{
	int i = 1;
	if (player->lidzba_jabłek>0)
	{
		switch (kierunek)
		{
	
	case'u':
		if (player->t[0] > 0 && szukaj(player->sizex-1, player->głowy->x) != (player->t[0] - 1))
		{

			push_back(&player->głowy->x, player->t[0], &player->sizex);
			push_back(&player->głowy->y, player->t[1], &player->sizey);

			player->t[0]--;
			return true;
		}
		break;
	case'd':
		if ((player->t[0] < planszax - 1) && szukaj(player->sizey - 1, player->głowy->x) != (player->t[0] + 1))
		{
			push_back(&player->głowy->x, player->t[0], &player->sizex);
			push_back(&player->głowy->y, player->t[1], &player->sizey);
			player->t[0]++;
			return true;
		}

		break;
	case'l':
		if (player->t[1] > 0 && szukaj(player->sizey - 1, player->głowy->y) != (player->t[1] - 1))
		{
			push_back(&player->głowy->x, player->t[0], &player->sizex);
			push_back(&player->głowy->y, player->t[1], &player->sizey);
			player->t[1]--;
			return true;
		}
		break;
	case'r':
		if ((player->t[1] < planszay - 1) && szukaj(player->sizey - 1, player->głowy->y) != (player->t[1] + 1))
		{
			push_back(&player->głowy->x, player->t[0], &player->sizex);
			push_back(&player->głowy->y, player->t[1], &player->sizey);
			player->t[1]++;
			return true;
		}
		break;

	default:
		break;

		}
		return false;
	}
	else
	{
		switch (kierunek)
		{

		case'u':
			if (player->t[0] > 0 )
			{

				push_back(&player->głowy->x, player->t[0], &player->sizex);
				push_back(&player->głowy->y, player->t[1], &player->sizey);

				player->t[0]--;
				return true;
			}
			break;
		case'd':
			if (player->t[0] < planszax - 1 )
			{
				push_back(&player->głowy->x, player->t[0], &player->sizex);
				push_back(&player->głowy->y, player->t[1], &player->sizey);
				player->t[0]++;
				return true;
			}

			break;
		case'l':
			if (player->t[1] > 0 )
			{
				push_back(&player->głowy->x, player->t[0], &player->sizex);
				push_back(&player->głowy->y, player->t[1], &player->sizey);
				player->t[1]--;
				return true;
			}
			break;
		case'r':
			if (player->t[1] < planszay - 1 )
			{
				push_back(&player->głowy->x, player->t[0], &player->sizex);
				push_back(&player->głowy->y, player->t[1], &player->sizey);
				player->t[1]++;
				return true;
			}
			break;

		default:
			break;

		}
		return false;
	}
}

SDL_ThreadFunction ruch(struct gracz *dane)
{
	int czas = 500;
	czas=czas - (dane->lidzba_jabłek * 10);
	if (czas < 100)
		czas = 100;
	SDL_Thread *ruch[2];
	while (dane->dzałanie_wontków)
	{
		SDL_Delay(czas);
		
		zmiana_pozycji(dane,dane->t[2]);
	}
}
void wyświetlanie_gry(const SDL_Rect *plansza, SDL_Texture *element2, SDL_Texture *element3,SDL_Renderer *ren,char *dane, SDL_Texture *element4)
{
	int i, j;
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	for (i = 0; i < planszax; i++)
	{
		for (j = 0; j < planszay; j++)
		{
			switch (dane[i*planszay + j])
			{
			case'g':
				SDL_RenderCopy(ren, element2, NULL, &plansza[i*planszay + j]);
				break;
			case't':
				SDL_RenderFillRect(ren, &plansza[i*planszay + j]);
				break;
			case'w':
				SDL_RenderCopy(ren, element3, NULL, &plansza[i*planszay + j]);
				break;
			case'j':
				SDL_RenderCopy(ren, element4, NULL, &plansza[i*planszay + j]);
				break;
			}
		}
	}
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
}
void twożenie_zapiskuw_obrazu(char *plansza, struct gracz *player)
{
	int i;	
	for (i = 0; i < player->lidzba_jabłek+1 ; i++)
	{
		
		plansza[planszay*szukaj(i, jabko.x) + szukaj(i, jabko.y)] = 'j';
	}
	for (i = 1; i < player->lidzba_jabłek+1 ; i++)
	{
		plansza[planszay*szukaj(player->sizex-i, player->głowy->x) + szukaj(player->sizey-i, player->głowy->y)] = 'w';
	}
	plansza[player->t[0] * planszay + player->t[1]] = 'g';


}
void czyszczenie_zapiski_obrazu(char *dane)
{
	int i,j;
	for (i = 0; i < planszax; i++)
	{
		for (j = 0; j < planszay; j++)
		{		
			dane[i*planszay+j] = 't';
		}
	}
}
void dodawanie_jabka_zapis_obrazu(char *obraz,int *lidzba_jabłek)
{
	bool prawidłowe_wspułżendne = false;
	int x=-1, y=-1;
	while (!prawidłowe_wspułżendne)
	{
		x = rand() % planszax;
		y = rand() % planszay;
		if (obraz[x*planszay + y] == 't')
			prawidłowe_wspułżendne = true;

	}
	push_back(&jabko.x, x, &jabko_sizex);
	push_back(&jabko.y, y, &jabko_sizey);
	*lidzba_jabłek= *lidzba_jabłek+1;
}
void zmiana_zjedzonego_jabka(char *obraz,int i)
{
	
	bool prawidłowe_wspułżendne = false;
	int x = -1, y = -1;
	while (!prawidłowe_wspułżendne)
	{
		x = rand() % planszax;
		y = rand() % planszay;
		if (obraz[x*planszay + y] == 't')
			prawidłowe_wspułżendne = true;

	}
	*przypisz(i, &jabko.x)=x;
	*przypisz(i, &jabko.y)=y;
}
SDL_ThreadFunction zjedzone_jabko(struct gracz *dane)
{
	int i;
	
	while (dane->dzałanie_wontków)
	{
		
		for (i = 0; i < jabko_sizex; i++)
			if ((dane->t[0] == szukaj(i, jabko.x)) && (dane->t[1] == szukaj(i, jabko.y)))
			{
				zmiana_zjedzonego_jabka(dane->dane_obrazu, i);
				dodawanie_jabka_zapis_obrazu(dane->dane_obrazu, &dane->lidzba_jabłek );
				dane->zjedzone_jabko ->gra_muzyka[0] = true;
			}
		SDL_Delay(10);
	}
	
}
SDL_ThreadFunction wynik_gry(struct gracze *dane)
{
	int i, j;
	int szukajx;
	int szukajy;
	while (dane->player1->dzałanie_wontków)
	{

		for (i = 1; i < dane->player2->lidzba_jabłek + 1; i++)
		{
			szukajx = szukaj(dane->player2->sizex - i, dane->player2->głowy->x);
			szukajy = szukaj(dane->player2->sizey - i, dane->player2->głowy->y);

			if (szukajx == *dane->player1->t && szukajy == dane->player1->t[1])
			{
				*dane->player2->koniec_gry = true;
			}

		}
		for (i = 1; i < dane->player1->lidzba_jabłek + 1; i++)
		{
			szukajx = szukaj(dane->player1->sizex - i, dane->player1->głowy->x);
			szukajy = szukaj(dane->player1->sizey - i, dane->player1->głowy->y);

			if (szukajx == *dane->player2->t && szukajy == dane->player2->t[1])
			{
				*dane->player1->koniec_gry = true;
			}

		}

		for (i = 1; i < dane->player1->lidzba_jabłek + 1; i++)
		{
			szukajx= szukaj(dane->player1->sizex - i, dane->player1->głowy->x);
			szukajy = szukaj(dane->player1->sizey - i, dane->player1->głowy->y);
			
			if (szukajx == *dane->player1->t && szukajy == dane->player1->t[1])
			{
				*dane->player2->koniec_gry= true;
			}

		}
		if (dane->player1->t[0] == dane->player2->t[0] && dane->player1->t[1] == dane->player2->t[1])
		{
			*dane->player1->koniec_gry = true;
			*dane->player2->koniec_gry = true;
		}

		for (i = 1; i < dane->player2->lidzba_jabłek + 1; i++)
		{
			szukajx = szukaj(dane->player2->sizex - i, dane->player2->głowy->x);
			szukajy = szukaj(dane->player2->sizey - i, dane->player2->głowy->y);
			if (szukajx == *dane->player2->t && szukajy == dane->player2->t[1])
			{
				*dane->player1->koniec_gry = true;
			}
		}
		SDL_Delay(1);
	}
}
void przywrucenie_podstawowych_parametruw(struct dane_menu *menu)
{
	menu->pozycja_elementuw_menu[menu->znacznik].x = wymiary_ekran.w/3;
	menu->kolor_napisów[menu->znacznik].a = 200;
	menu->kolor_napisów[menu->znacznik].r = 200;
	menu->kolor_napisów[menu->znacznik].b = 200;
	menu->kolor_napisów[menu->znacznik].g = 200;
	menu->pozycja_elementuw_menu[menu->znacznik].w = wymiary_ekran.w / 3;
	menu->pozycja_elementuw_menu[menu->znacznik].h = wymiary_ekran.h / 5;
}
void parametry_wybranego_tekstu(struct dane_menu *menu)
{
	menu->kolor_napisów[menu->znacznik].a = 255;
	menu->kolor_napisów[menu->znacznik].r = 255;
	menu->kolor_napisów[menu->znacznik].b = 255;
	menu->kolor_napisów[menu->znacznik].g = 255;
	menu->pozycja_elementuw_menu[menu->znacznik].x = wymiary_ekran.w/4;
	menu->pozycja_elementuw_menu[menu->znacznik].w = wymiary_ekran.w/2 ;
	menu->pozycja_elementuw_menu[menu->znacznik].h = wymiary_ekran.h / 4;
}
void zapisywanie_danych(struct gracz *player, FILE *plik )
{
	int i, j;

	fprintf(plik, "%d\n", player->lidzba_jabłek);
	fprintf(plik, "%d\n%d\n%d\n",player->t[0], player->t[1], player->t[2]);
	fprintf(plik, "%d\n%d\n", player->sizex, player->sizey);
	fprintf(plik, "%d\n%d\n", jabko_sizex, jabko_sizey);
    
	for (i = 0; i < player->sizex ; i++)
	{
	
		fprintf(plik, "%d\n%d\n", szukaj(i, player->głowy->x), szukaj(i, player->głowy->y));
	}
	for (i = 0; i < jabko_sizex; i++)
	{
		fprintf(plik, "%d\n%d\n", szukaj(i, jabko.x), szukaj(i,jabko.y));
	}
}
void odzczyt_danych(struct gracz *player, FILE *plik)
{
	int tmp1, tmp2;
	struct pozycje tmpvektor;
	int tmpsize1=0;
	int tmpsize2=0;
	int i, j;

	destroy_vecktor(&player->głowy->x, &player->sizex);
	destroy_vecktor(&player->głowy->y, &player->sizey);
	destroy_vecktor(&jabko.x, &jabko_sizex);
	destroy_vecktor(&jabko.y, &jabko_sizey);

	fscanf_s(plik, "%d", &player->lidzba_jabłek);
	fscanf_s(plik, "%d%d%d", &player->t[0], &player->t[1], &player->t[2]);
	fscanf_s(plik, "%d%d", &player->sizex, &player->sizey);
	fscanf_s(plik, "%d%d", &jabko_sizex, &jabko_sizey);

	for (i = 0; i < player->sizex; i++)
	{
		fscanf_s(plik, "%d%d", &tmp1, &tmp2);
		push_back(&player->głowy->x, tmp1, &tmpsize1);
		push_back(&player->głowy->y, tmp2, &tmpsize2);

	}
	tmpsize1 = 0;
	tmpsize2 = 0;
	for (i = 0; i < jabko_sizex; i++)
	{
		fscanf_s(plik, "%d%d", &tmp1, &tmp2);
		push_back(&jabko.x, tmp1, &tmpsize1);
		push_back(&jabko.y, tmp2, &tmpsize2);

	}
}
int sterowanie_menu(SDL_Event *zdażenie, struct dane_menu *menu,bool *zapis,bool *wznowiene_gry, struct przebieg_programu *przebieg_programu)
{
	if(zdażenie->type==SDL_KEYDOWN)
		switch (zdażenie->key.keysym.sym)
		{
		case SDLK_UP:
			if (menu->znacznik >0 )
			{
				przywrucenie_podstawowych_parametruw(menu);
				menu->znacznik--;
				parametry_wybranego_tekstu(menu);
			
			}
			break;
		case SDLK_DOWN:
			if (menu->znacznik <3)
			{
				przywrucenie_podstawowych_parametruw(menu);
				menu->znacznik++;
				parametry_wybranego_tekstu(menu);
			
			}
			break;
		case SDLK_SPACE:
			if (menu->znacznik == 0)
			{
				menu->gra = true;
				przebieg_programu->pokasz_wynik = true;
			}
			if (menu->znacznik == 1)
			{
				*wznowiene_gry = true;
			}
			if (menu->znacznik == 2)
			{
				*zapis = true;
			}
			if (menu->znacznik == 3)
			{
				menu->gra = true;
				przebieg_programu->zakończ_program = true;
		
			}
		
			break;
		
		}
	
}
void przygotowanie_menu(struct dane_menu *menu)
{
	int i;
	menu->znacznik = 0;
	for (i = 0; i < 4; i++)
	{
		menu->kolor_napisów[i].a = 200;
		menu->kolor_napisów[i].r = 200;
		menu->kolor_napisów[i].b = 200;
		menu->kolor_napisów[i].g = 200;
		menu->pozycja_elementuw_menu[i].x = wymiary_ekran.w/3;
		menu->pozycja_elementuw_menu[i].y = (wymiary_ekran.h/5)* i + (wymiary_ekran.h / 10);
		menu->pozycja_elementuw_menu[i].w = wymiary_ekran.w / 3;
		menu->pozycja_elementuw_menu[i].h = wymiary_ekran.h / 5;
	}
	menu->słowa[0] = malloc(sizeof(char) * 3);
	menu->słowa[1] = malloc(sizeof(char) * 10);;
	menu->słowa[2] = malloc(sizeof(char) * 5);;
	menu->słowa[3] = malloc(sizeof(char) * 7);;
	menu->słowa[0] = "graj";
	menu->słowa[1] = "wznowienie";
	menu->słowa[2] = "zapis";
	menu->słowa[3] = "wyjscie";
	parametry_wybranego_tekstu(menu,wymiary_ekran);
	menu->gra = false;

}
void akcje_menu(struct dane_menu *menu, SDL_Renderer *ren, TTF_Font *font)
{
	int i;
		for (i = 0; i < 4; i++)
	{

		menu->czcionka[i] = TTF_RenderText_Blended(font, menu->słowa[i], menu->kolor_napisów[i]);
		menu->tekst[i] = SDL_CreateTextureFromSurface(ren, menu->czcionka[i]);
		SDL_RenderCopy(ren, menu->tekst[i], NULL, &menu->pozycja_elementuw_menu[i]);

	}
		
}
void rozpoczeńcie_wontków(struct wszystkie_wontki_i_dane *wontek ,struct gracze *player, struct gracze *odwrucony_player)
{
	wontek->zegar = SDL_CreateThread(czas, "zliczanie", wontek->czas );
	player->player1->ruch_gry = SDL_CreateThread(ruch, "ruch", player->player1);
	player->player2->ruch_gry = SDL_CreateThread(ruch, "ruch", player->player2);
	player->player1->sprawdzanie = SDL_CreateThread(zjedzone_jabko, "jabko", player->player1);
	player->player2->sprawdzanie = SDL_CreateThread(zjedzone_jabko, "jabko", player->player2);
	player->player1->wynik = SDL_CreateThread(wynik_gry, "wynik", player);
	player->player2->wynik = SDL_CreateThread(wynik_gry, "wynik", odwrucony_player);
	wontek->muza = SDL_CreateThread(muzyka, "muzyka", wontek->muzyka);
}
void przydotowywanie_danych_wontki(struct wszystkie_wontki_i_dane *wontek,char *a, struct gracze *player)
{
	wontek->czas->a = a;
	wontek->muzyka->gra = false;
	player->player1->zjedzone_jabko->gra_muzyka = &wontek->muzyka->gra;
	player->player2->zjedzone_jabko->gra_muzyka = &wontek->muzyka->gra;

}
przygotowywanie_danych_wontków_do_rozpoczeńcia(struct wszystkie_wontki_i_dane *wontek, struct gracze *player)
{

	player->player1->dzałanie_wontków = true;
	player->player2->dzałanie_wontków = true;
	wontek->czas->dzałanie_wontku = true;
}
void ustawienie_koloru_jasna_biel(SDL_Color *kolor)
{
	kolor->a = 255;
	kolor->b = 255;
	kolor->g = 255;
	kolor->r = 255;

}
void kończenie_dzałania_wontków(struct wszystkie_wontki_i_dane *wontek, struct gracze *player)
{
	player->player1->dzałanie_wontków = false;
	player->player2->dzałanie_wontków = false;
	wontek->czas->dzałanie_wontku = false;
}
void twożenie_zbioru_danych_struktur(struct wszystkie_wontki_i_dane *wontek)
{
	wontek->czas = malloc(sizeof(struct wontek_czas));
	wontek->muzyka = malloc(sizeof(struct wontek_muzyka));
	
}
void ustawienie_pozycji_czasu(SDL_Rect *pozycja_czasu)
{
	pozycja_czasu->x = 520;
	pozycja_czasu->y = 20;
	pozycja_czasu->w = 100;
	pozycja_czasu->h = 20;
}
void ustawienie_pozycji(SDL_Rect *pozycja_centralna,SDL_Rect *pozycjainformujonca_1)
{
	pozycja_centralna->w = wymiary_ekran.w / 2;
	pozycja_centralna->h = wymiary_ekran.h / 2;
	pozycja_centralna->x = wymiary_ekran.w/5;
	pozycja_centralna->y = wymiary_ekran.h/5;
	pozycjainformujonca_1->y = wymiary_ekran.w / 2.5;
	pozycjainformujonca_1->x = wymiary_ekran.h / 3;
	pozycjainformujonca_1->h = wymiary_ekran.h / 6;
	pozycjainformujonca_1->w = wymiary_ekran.w / 4;
}

kasowanie_struktury_wszystkie_wontki_i_dane(struct wszystkie_wontki_i_dane *wontek)
{
	free(wontek->czas);
	free(wontek->muzyka);
	
}
void MENU( SDL_Renderer *ren, struct przebieg_programu *przebieg_programu, struct gracze *player)
{
	int i;
	bool zapis_gry = false;
	bool wznowienie_gry = false;
	SDL_Event zdazenie;
	TTF_Font *font = TTF_OpenFont("lib\\arial.ttf", 500);
	struct dane_menu menu;
	przygotowanie_menu(&menu);
	while (!menu.gra)
	{
		if (SDL_PollEvent(&zdazenie))
			sterowanie_menu(&zdazenie, &menu, &zapis_gry, &wznowienie_gry,przebieg_programu);
		if (zapis_gry)
		{
			FILE *plik = NULL;
			fopen_s(&plik, "plik.txt", "w");
			zapisywanie_danych(player->player1,plik);
			zapisywanie_danych(player->player2, plik);
			fclose(plik);
			zapis_gry = false;
		}
		if (wznowienie_gry)
		{
			FILE *plik = NULL;
			fopen_s(&plik, "plik.txt", "r");
			odzczyt_danych(player->player1,plik);
			odzczyt_danych(player->player2, plik);
			if(plik != NULL)
			fclose(plik);
			wznowienie_gry = false;
		}
		akcje_menu(&menu, ren, font);
		SDL_RenderPresent(ren);
		SDL_Delay(100);
		
		for (i = 0; i < 4; i++)
		{
			SDL_FreeSurface(menu.czcionka[i]);
			SDL_DestroyTexture(menu.tekst[i]);
		}
		SDL_RenderClear(ren);
	}
	TTF_CloseFont(font);
}
void ustawienie_pozycji_planszy(SDL_Rect *plansza)
{
	int i, j;
	for (i = 0; i < planszax; i++)
	{
		for (j = 0; j < planszay; j++)
		{
			plansza[i*planszay + j].w = (wymiary_ekran.w / planszay) - ((wymiary_ekran.w / 5) / planszay);
			plansza[i*planszay + j].h = (wymiary_ekran.h / planszax) - ((wymiary_ekran.h / 5) / planszax);
			plansza[i*planszay + j].x = (j *(plansza[i*planszay + j].w + ((wymiary_ekran.w / 5) / planszay) / 2)) + 100;
			plansza[i*planszay + j].y = (i *(plansza[i*planszay + j].h + ((wymiary_ekran.h / 5) / planszax) / 2)) + 50;
		}
	}
}
void GRA(struct wszystkie_wontki_i_dane *wontek, SDL_Renderer *ren, struct przebieg_programu *przebieg_programu,struct gracze *player)
{
	struct gracze odwrucony_pleyer;
	odwrucony_pleyer.player1= player->player2;
	odwrucony_pleyer.player2 = player->player1;
	SDL_Rect pozycja_czasu;
	ustawienie_pozycji_czasu(&pozycja_czasu);
	SDL_Color kolor;
	ustawienie_koloru_jasna_biel(&kolor);
	SDL_Event zdazenie;
	TTF_Font *font = TTF_OpenFont("lib\\arial.ttf", 500);
	SDL_Surface *glowa = SDL_LoadBMP("lib\\glowa.bmp");
	SDL_Texture *element2 = SDL_CreateTextureFromSurface(ren, glowa);
	SDL_Surface *tułów = SDL_LoadBMP("lib\\Bitmap.bmp");
	SDL_Texture *element3 = SDL_CreateTextureFromSurface(ren, tułów);
	SDL_Surface *jabko = SDL_LoadBMP("lib\\Image.bmp");
	SDL_Texture *element4 = SDL_CreateTextureFromSurface(ren, jabko);
	SDL_Surface *tekst;
	SDL_Texture *tex;
	SDL_Rect plansza[planszax*planszay];
	ustawienie_pozycji_planszy(plansza);
	SDL_DollarGestureEvent zdażenie_android;
	przygotowywanie_danych_wontków_do_rozpoczeńcia(wontek,player);
	 rozpoczeńcie_wontków(wontek,player,&odwrucony_pleyer);
	///rozgrywka
	czyszczenie_zapiski_obrazu(player->dane_obrazu);
	if (player->player1->lidzba_jabłek == -1)
	{
		dodawanie_jabka_zapis_obrazu(player->dane_obrazu, &player->player1->lidzba_jabłek);
		player->player2->lidzba_jabłek++;
	}
	while (!przebieg_programu->koniec_gry_play1 && !przebieg_programu->koniec_gry_play2)
	{

		if (SDL_PollEvent(&zdazenie))
		{
			sterowanie_gry_pleyer1( &zdazenie, &przebieg_programu->koniec_gry_play1, przebieg_programu, player->player1);
			sterowanie_gry_pleyer2(&zdazenie, player->player2);
		}
		tekst = TTF_RenderText_Blended(font, wontek->czas->a, kolor);
		tex = SDL_CreateTextureFromSurface(ren, tekst);
		SDL_RenderCopy(ren, tex, NULL, &pozycja_czasu);
		czyszczenie_zapiski_obrazu(player->dane_obrazu);
		twożenie_zapiskuw_obrazu(player->dane_obrazu, player->player1);
		twożenie_zapiskuw_obrazu(player->dane_obrazu, player->player2);
		wyświetlanie_gry(plansza, element2, element3, ren, player->dane_obrazu, element4);
		SDL_RenderPresent(ren);
		SDL_Delay(10);
		SDL_FreeSurface(tekst);
		SDL_DestroyTexture(tex);
		SDL_RenderClear(ren);
	}
	kończenie_dzałania_wontków(wontek,player);
	SDL_Delay(100);
	SDL_DetachThread( player->player1->ruch_gry);
	SDL_DetachThread(player->player2->ruch_gry);
	SDL_DetachThread(wontek->zegar);
	SDL_DetachThread(player->player1->sprawdzanie);
	SDL_DetachThread(player->player2->sprawdzanie);
	SDL_DetachThread(player->player1->wynik);
	SDL_DetachThread(player->player2->wynik);
	SDL_DetachThread(wontek->muza);
	TTF_CloseFont(font);
	SDL_DestroyTexture(element2);
	SDL_DestroyTexture(element3);
	SDL_DestroyTexture(element4);
	SDL_FreeSurface(glowa);
	SDL_FreeSurface(jabko);
	SDL_FreeSurface(tułów);

}
void WYNIK_GRY(SDL_Renderer *ren,const char *nazwa_zwycienscy)
{
	const char napis_wygral[7] = "wygral";
	const char napis_renis[6] = "remis";
	SDL_Color kolor;
	ustawienie_koloru_jasna_biel(&kolor);
	SDL_Rect pozycja_centralna;
	SDL_Rect pozycjainformujonca_1;
	ustawienie_pozycji(&pozycja_centralna, &pozycjainformujonca_1);
	SDL_Rect pozycja_wyniku;
	TTF_Font *font = TTF_OpenFont("lib\\arial.ttf", 500);
	SDL_Surface *tekst=NULL;
	SDL_Texture *tex=NULL;
	SDL_Surface *tekst2=NULL;
	SDL_Texture *tex2=NULL;
	pozycja_wyniku = pozycjainformujonca_1;
	pozycja_wyniku.x = pozycja_wyniku.x + 500;
	if (nazwa_zwycienscy == NULL)
	{
	tekst = TTF_RenderText_Blended(font, napis_renis, kolor);
	tex = SDL_CreateTextureFromSurface(ren, tekst);
	SDL_RenderCopy(ren, tex, NULL, &pozycja_centralna);
    }
	else
	{
		tekst = TTF_RenderText_Blended(font, napis_wygral, kolor);
		tex = SDL_CreateTextureFromSurface(ren, tekst);
		tekst2 = TTF_RenderText_Blended(font, nazwa_zwycienscy, kolor);
		tex2 = SDL_CreateTextureFromSurface(ren, tekst2);
		SDL_RenderCopy(ren, tex, NULL, &pozycja_centralna);
		SDL_RenderCopy(ren, tex2, NULL, &pozycjainformujonca_1);
	}

	SDL_RenderPresent(ren);
	SDL_Delay(3000);
	SDL_FreeSurface(tekst);
	SDL_FreeSurface(tekst2);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(tex2);
	SDL_RenderClear(ren);
	TTF_CloseFont(font);
}
void zakończenie_gry_po_przegranej(struct gracz *player,int a)
{
	player->t[0] = a * (planszax / 5);
	player->t[1] = a * (planszay / 5);
	player->t[2] = 1;
	player->lidzba_jabłek = -1;
	destroy_vecktor(&player->głowy->x, &player->sizex);
	destroy_vecktor(&player->głowy->y, &player->sizey);
	destroy_vecktor(&jabko.x, &jabko_sizex);
	destroy_vecktor(&jabko.y, &jabko_sizey);
}
void przydotowywanie_danych_gracza(struct gracz *player, int a)
{
	player->głowy = malloc(sizeof(struct pozycje));
	player->zjedzone_jabko = malloc(sizeof(struct dane_wontku_jabko));
	player->zjedzone_jabko->gra_muzyka = NULL;
	player->głowy->x.next = NULL;
	player->sizex = 0;
	player->sizey = 0;
	player->nazwa_gracza = NULL;
    player->lidzba_jabłek = -1;
	player->t[0] = a * (planszax / 5);
	player->t[1] = a * (planszay / 5);
	player->t[2] = 1;	
}
void przygotowywanie_danych_graczy(struct gracze *player)
{
	player->player1 = malloc(sizeof(struct gracz));
	player->player2 = malloc(sizeof(struct gracz));
	przydotowywanie_danych_gracza(player->player1,0);
	przydotowywanie_danych_gracza(player->player2,1);
	player->player1->dane_obrazu = player->dane_obrazu;
	player->player2->dane_obrazu = player->dane_obrazu;
}
void kasowanie_struktury_gracze(struct gracze *player)
{
	destroy_vecktor(&player->player1->głowy->x, &player->player1->sizex);
	destroy_vecktor(&player->player1->głowy->y, &player->player1->sizey);
	destroy_vecktor(&player->player2->głowy->x, &player->player2->sizex);
	destroy_vecktor(&player->player2->głowy->y, &player->player2->sizey);

	free(player->player1->zjedzone_jabko);
	free(player->player2->zjedzone_jabko);
	free(player->player1->nazwa_gracza);
	free(player->player2->nazwa_gracza);
	free(player->player1);
	free(player->player2);

}
void przygotowywanie_struktury_przebirg_programu(struct przebieg_programu *program, struct gracze *player)
{
	program->pokasz_wynik = true;
	program->zakończ_program = false;
	program->koniec_gry_play1 = false;
	program->koniec_gry_play2 = false;
	player->player1->koniec_gry = &program->koniec_gry_play1;
	player->player2->koniec_gry = &program->koniec_gry_play2;
}
int _WCZYTYWANIE_GRACZY_wczytywanie_napisu(SDL_Event *zdazenie ,SDL_Rect *pozycja_liter)
{
	int i ;
	if (zdazenie->type== SDL_MOUSEBUTTONUP)
	{
		for (i = 0; i < 26; i++)
		{
			if (zdazenie->motion.x >= pozycja_liter[i].x && zdazenie->motion.x <= (pozycja_liter[i].x + pozycja_liter[i].w))
				if (zdazenie->motion.y >= pozycja_liter[i].y && zdazenie->motion.y <= (pozycja_liter[i].y + pozycja_liter[i].h))
					return i;
		}

	}
	if (zdazenie->type == SDL_KEYDOWN)
	{
		for (i = 97; i <= 122; i++)
		{
			if (zdazenie->key.keysym.sym == i)
				return i - 97;
		}
	}
	return -1;
}
void czyszczenie_char(char *tablica, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		*(tablica + i) = ' ';
	}
}
void _wczytywanie_graczy_przydotowywanie_tekstur(SDL_Renderer *ren,SDL_Rect *pozycja_liter,SDL_Rect *pozycja_wpisanej_litery,SDL_Texture **T_alfabet,SDL_Texture **T_znak)
{
	TTF_Font *font = TTF_OpenFont("lib\\arial.ttf", 500);
	SDL_Color kolor;
	ustawienie_koloru_jasna_biel(&kolor);
	SDL_Surface *S_alfabet[26];
	SDL_Surface *S_znak;
	int j,i,k=0;
	const char *litera = &j;
	for (j = 65; j <= 90; j++)
	{
		i = j - 65;

		S_alfabet[i] = TTF_RenderText_Blended(font, litera, kolor);
		T_alfabet[i] = SDL_CreateTextureFromSurface(ren, S_alfabet[i]);
		if (i == 10 || i == 20)
			k++;
		pozycja_liter[i].h = (wymiary_ekran.h / 15);
		pozycja_liter[i].w = (wymiary_ekran.w / 15);
		pozycja_liter[i].x = (i - (k * 10)) *(wymiary_ekran.w / 15) + (wymiary_ekran.w / 5);
		pozycja_liter[i].y = (k + 1)* (wymiary_ekran.h / 6);
	}

	S_znak = TTF_RenderText_Blended(font, "__", kolor);
	*T_znak = SDL_CreateTextureFromSurface(ren, S_znak);
	for (i = 0; i < 10; i++)
	{
		pozycja_wpisanej_litery[i].w = (wymiary_ekran.w / 15);
		pozycja_wpisanej_litery[i].h = (wymiary_ekran.h / 15);
		pozycja_wpisanej_litery[i].x = (((wymiary_ekran.w / 15)+10)*(i)) + (wymiary_ekran.w / 5);
		pozycja_wpisanej_litery[i].y = (wymiary_ekran.h / 6) * 4 ;

	}
	for (i = 0; i < 26; i++)
	{
		SDL_FreeSurface(S_alfabet[i]);
	}
	TTF_CloseFont(font);
}
char* wczytywanie_graczy(SDL_Renderer *ren)
{
	bool wykonywanie_wpisywania = true;
	int długość_napisiu = 0;
	char *słowa=(char*)malloc(sizeof(char)*10);
	czyszczenie_char(słowa, 10);
	SDL_Event zdazenie;
	SDL_Rect pozycja_liter[26];
	SDL_Rect pozycja_wpisanej_litery[26];
	SDL_Texture *T_alfabet[26];
	SDL_Texture *T_znak=NULL;
	_wczytywanie_graczy_przydotowywanie_tekstur(ren, pozycja_liter, pozycja_wpisanej_litery, T_alfabet, &T_znak);
	int i, j, index;
	while (wykonywanie_wpisywania)
	{
		for (i= 0; i <= 25; i++)
		{
			SDL_RenderCopy(ren, T_alfabet[i], NULL, &pozycja_liter[i]);
		}
		for (i = 0; i < 10; i++)
		{
			SDL_RenderCopy(ren, T_znak, NULL, &pozycja_wpisanej_litery[i]);
		}
		for (i = 0; i < długość_napisiu; i++)
		{	
			SDL_RenderCopy(ren, T_alfabet[słowa[i]], NULL, &pozycja_wpisanej_litery[i]);
		}
		 SDL_WaitEvent(&zdazenie);
			index = _WCZYTYWANIE_GRACZY_wczytywanie_napisu(&zdazenie, pozycja_liter);
			if (index != -1)
			{
				SDL_RenderCopy(ren, T_alfabet[index], NULL, &pozycja_wpisanej_litery[długość_napisiu]);
				słowa[długość_napisiu] = index;
				długość_napisiu++;
				
			}
			if (zdazenie.key.keysym.sym == SDLK_SPACE && długość_napisiu>0)
				wykonywanie_wpisywania=false;
		
		SDL_RenderPresent(ren);
		SDL_RenderClear(ren);
	}

	for (i = 0; i < 26; i++)
	{
		SDL_DestroyTexture(T_alfabet[i]);
	}
	SDL_RenderClear(ren);
	for (i = 0; i < długość_napisiu; i++)
	{
		słowa[i] += 65;
	}
	słowa[długość_napisiu] = '\0';
	const char *nazwa_gracza = słowa;
	return nazwa_gracza;

}

int SDL_main(int argc, char * args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	char a[9] = "czas:000";
	int i = 0, j = 0;
	struct gracze player;
	przygotowywanie_danych_graczy(&player);
	struct przebieg_programu program;
	przygotowywanie_struktury_przebirg_programu(&program,&player);
	struct wszystkie_wontki_i_dane wontek;
	twożenie_zbioru_danych_struktur(&wontek);
	przydotowywanie_danych_wontki(&wontek,a, &player);
	pozyskiwanie_wymiarów_ekramu_na_kturym_jest_wyświetlany(&wymiary_ekran);
	SDL_Window  *screen = SDL_CreateWindow("Moje okno gry", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wymiary_ekran.w, wymiary_ekran.h,SDL_WINDOW_SHOWN);
	SDL_SetWindowTitle(screen,"ekran gry");
	SDL_Renderer *ren = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	while (!program.zakończ_program)
	{  
		MENU(ren,&program,&player);

		if (!program.zakończ_program)
		{
			if (player.player1->nazwa_gracza == NULL)
			{
				player.player1->nazwa_gracza = wczytywanie_graczy(ren);
				player.player2->nazwa_gracza = wczytywanie_graczy(ren);
			}
		GRA(&wontek, ren,&program,&player);
			if (program.pokasz_wynik)
			{
				//remis
				if (program.koniec_gry_play1 && program.koniec_gry_play2)
				{
					WYNIK_GRY(ren, NULL);
				}
				else
				{ ///wygrana i przegrana

					if (program.koniec_gry_play1)
					{
						WYNIK_GRY(ren,player.player1->nazwa_gracza);
					}
					if (program.koniec_gry_play2)
					{
						WYNIK_GRY(ren, player.player2->nazwa_gracza);
					}
				}
				zakończenie_gry_po_przegranej(player.player1,0);
				zakończenie_gry_po_przegranej(player.player2,1);
				
			}
		
			program.koniec_gry_play1 = false;
			program.koniec_gry_play2 = false;
			program.pokasz_wynik = false;
		}
	}
	SDL_DestroyWindow(screen);
	SDL_DestroyRenderer(ren);
	kasowanie_struktury_wszystkie_wontki_i_dane(&wontek);
	kasowanie_struktury_gracze(&player);
	destroy_vecktor(&jabko.x, &jabko_sizex);
	destroy_vecktor(&jabko.y, &jabko_sizey);
	TTF_Quit();
	SDL_Quit();
	exit(0);
}
