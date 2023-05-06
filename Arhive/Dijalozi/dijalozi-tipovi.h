//#include <asmtypes.h>
/*DIJALOZI*///---------------------------------------------------------------------------------
#ifndef DIJALOZI_TIPOVI
#define DIJALOZI_TIPOVI
//--------------------------------------------------------------------------
typedef struct
{short         brsl;//broj slova za svako polje
 void        * unos;//pokazivac ka promenljivoj koja se unosi
 const char  * upit;//tekst kojim se pita za unos promenljive
       char  * frmt;//pokazivac na formatni niz za prikaz teksta
}PodZaDlg;/*PODACI ZA DIJALOG*/


/*                    ENUMERACIJA ZA MENI TABELE                          */
/*------------------------------------------------------------------------*/

enum Meni{
	_OPSTE=1,      _IZVEZI=11,  _FONT=12,   _RAZMAK=13,
	_OPERACIJE=2,  _LINKOMB=21, _ODUZMI=22, _ZAMENI=23, _IZBACI=24,
	_POTVRDI=3
};

//Formatiranje tabele
typedef struct
{//short sirina;
 char	*frmt;
 char *zaglavlje;
}FrmtTabKol;

typedef struct
{
 char fnt; char sysfnt;

 short   dx; short   dy;
 short padx; short pady;
 short xpoc; short ypoc;
 short zagl;

 short *sirine;
 short maxvrsta;

 short *memi;
 short  mem1;

}FrmtTabCrt;

typedef struct
{short n;
 short m;
 void *el;
 FrmtTabKol *fk;
 char *pmem;
 WINDOW *w;
}FrmtTab;


/*Parametri koji opisuju jednu operaciju nad elementima tablice*/

typedef struct OperacijaTab OperacijaTab;
struct OperacijaTab
{short promena; //tip promene:zamena,izbacivanje,sabiranje....definisano u enum
 short vk;      //dimenzija na koju se odnosi promena
 short oprtr[2];//operatori=indeksi vrsta/kolona koji su predmet promene
 short koef[2]; //koeficijenti za lin.kombinaciju
 char  izvoz[8];
 OperacijaTab *sledeca[2];
};

#endif
