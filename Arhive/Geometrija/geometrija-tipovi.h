#ifndef GEOMETRIJA_TIPOVI
#define GEOMETRIJA_TIPOVI

/*PROTOTIPOVI*/
/*Nosac i Stap definisemo u headeru statika.
  Nosac mora da sadrzi: .pc .kt .kd
  ostali elementi strukture su proizvoljni(sto se tice ovog headera) */
typedef struct Nosac Nosac; //deklarisem ga ovde jer treba da bude jedina globalna promenljiva u programu, koja sadrzi sve
typedef struct Stap  Stap;
typedef struct Duz   Duz;
typedef struct Tacka Tacka;

/*DEFINICIJE TIPOVA*/
struct Tacka
{ char id; //oznaka tacke
	float xy[2]; //koordinate
	float *pom; //pomeranja
	struct Tacka *sused[5][2]; //pokazivac na sledecu tacku (vidi beleske)
	//struct tck *najblizi[2][2]; //pokazivac na najblizu tacku
	Duz ** duz;//pokazivac na niz duzi ciji je jedan kraj ova tacka
	short brduzi;//broj duzi u nizu gore
};

typedef Tacka* exttck[3][2];//matrica sa void pokazivacima na extremne tacke (najlevlja,najvisa,...)

struct Duz
{ char         id;           //oznaka duzi
  Tacka      * ik[2];        //krajnje tacke duzi
  struct Duz * sused[3][2];  //pokazivac na sledecu duz (sledeci id)
  Stap       * tata;         //pokazivac na gredu koju ova duz odredjuje
  //Duz_Metode * m;          //pokazivac na strukturu koja sadrzi metode
};

/*struct Duz_Metode
{ float (*duzina)(Duz *d)=&l_d; //pozivanje: d->m.l(d), je isto sto i l_d(d)
}*/

typedef Duz* extduz[3][2];//matrica sa void pokazivacima na extreme duzi(najlevlja,najvisa,...)

typedef struct {short x,y;}Tacka_px;//Tip za cuvanje koordinata ekrana (u [px])
typedef struct {float x,y;}Tacka_stv;//Tip za cuvanje stvarnih koordinata (npr. u [m])
typedef float Vektor[2][2];//Vektor zadat sa dve tacke

/*--------------- PARAMETRI CRTANJA ---------------------------*/
typedef struct //OBJASNJENJE ZA PARAMETRE CRTANJA:vidi beleske
{ float crt[2][2];//coskovi vidljivog crteza:[pravac:X(0)-Y(1)][min(0)-max(1) u px]
	float raz;  //razmera stvarnih jedinica u odnosu na rezoluciju
	//float razxy;//razmera x/y jedinica pox i y osi
	float gab[2][2];//gabarit unete geometrije
	float okv[2][2];//okvir za crtanje [pravac:X(0)-Y(1)][min(0)-max(1)]
	float zum;   //zum koeficijent
	float dok[2]; //vektor pomeranja okvira na zumiranom modelu

	//font pojedinih oznaka na crtezu
	struct { short tacke;	 short tablice; 	   } font;
  //vidljivost pojedinih elemenata crteza (1,0)
  struct { short tacke;  short oznake_tacaka;
           short duzi;   short veze;
                         short grid;
           short dijag;  short srafura;      } vid;
  //Kod promene vidljivosti mora sve da se izbrise pa da se iscrta ispocetka
  short rezim;//rezim crtanja:A_NORMAL(za crtanje),A_XOR(za brisanje-nema azuriranja)

  struct { short nx;     short ny;//komada
           float dx;     float dy;//razmaci
           float x0;     float y0;/*poc.tac.*/}grid;
} ParamCrt;
/*---------------------------------------------------------------*/

typedef struct niztacaka
{ Tacka * t;
	struct niztacaka *sused[2];//0-preth,1-sledeca
} NizTacaka;

typedef struct nizduzi
{ Duz * d;
	struct nizduzi *sused;//0-preth,1-sledeca
} NizDuzi;

#endif