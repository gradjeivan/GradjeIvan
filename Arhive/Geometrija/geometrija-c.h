#ifndef GEOMETRIJA_C
#define GEOMETRIJA_C

//#ifndef GEOMETRIJA_TIPOVI
#include "geometrija-tipovi.h"
//#endif

/*------------KONSTANTE--------------*/
#define ZUM 1.2//podrazumevano zumiranje gabarita nosaca na crtezu
#define ZUM_FAKTOR 1.1 //mnozilac, tj. kolicnik za rucno zumiranje

/*sledi mnozilac vece dimenzije gabarita, proizvod je
 geometrijska tolerancija: velicina za koju smeju da se razlikuju
                           dve vel. da bi se reklo da su jednake */
#define KOEF_GEOM_TOLERANCIJE 0.00001 //DBL_EPSILON i FLT_EPSILON ne rade iz nekog razloga


#define VEL_OZNAKE 3 //polovina duzine pravougaonika oznake tacke
#define ODALJI_TEKST 2 //koliko da odalji tekst od tacke ili linije za koju je vezan
#define MENUL 115
#define MENUH  15 //ovo je kod TI-89
#define LISTADESNO 28 //sirina prostora za listu desno
#define KOMAND 6 //visina komandne linije na dnu
#define KOMANDL MENUL
#define FONT_INFOA F_4x6


//#if TI89_CLASSIC || TI89_TITANIUM || TI89
enum Strelice_tipke{LEVO=338, DESNO=344, GORE=337, DOLE=340};
/*#else
enum Strelice_tipke{LEVO=337, DESNO=340, GORE=338, DOLE=344}
#endif*/

/*------------GLOBALNE PROMENLJIVE-----------*/
extern Nosac    *nsc;
extern ParamCrt *pc;
extern exttck   *kt;
extern extduz   *kd;
extern float     tol;//za pocetak, kasnije se azurira funkcijom azuriraj_toleranciju

/*======================= PROTOTIPOVI FUNKCIJA ==============================*/
/*--------------------------Parametri crtanja--------------------------------*/
//void  StampajParametreCrtanja(void);//za debug
void  azuriraj_param_crtanja (void);

Tacka_px xy_crt              (float x,float y); //transf.koordinata na ekranske
/*-------------------------------Tacke---------------------------------------*/
void     nuliraj_exttck       (void);
Tacka *  tacku_dodaj          (char id, float xy[2]);
void     tacku_izbrisi        (Tacka *t);
Tacka *  tacku_nadji          (Tacka *t,short i[2],short brmesta);
Tacka *  tacku_nadji_po_id    (char id);
//short  tacke_stampaj        (Tacka *ktac,short dmz[2],short brlinija); //za debug
short    tacaka_ukupno        (void);
Tacka *  tacki_trazi_duplikat (float xy[2]);
BOOL     tacka_vidljiva       (Tacka_px t,short tolerancija);                  //da li je tacka u okvirima zumiranog crteza (sa tolerancijom)
//void     prikazi_ekstreme     (void);
/*------------------------------Pres.tacke-----------------------------------*/
NizTacaka *pres_tacku_dodaj  (NizTacaka *  niz,Tacka *t,Tacka *ti);
void       pres_tacke_osl_mem(NizTacaka * niz);
/*--------------------------------Duzi---------------------------------------*/
void       nuliraj_extduz       (void);

Duz *      duz_prazna           (void);
Duz *      duz_dodaj            (char id, Tacka *ik[2]);
void       duz_izbrisi          (Duz *t);
Duz *      duz_trazi_duplikat   (Duz *d1);
NizTacaka *duz_proveri_preseke  (Duz *d);
NizDuzi   *duzi_podeli_u_prestac(NizTacaka *niz);

Duz *      duz_nadji            (short brmesta);
Duz *      duz_nadji_id         (char id);
//short    duzi_stampaj         (Duz *kduz,short srt[2],short brlinija); //za debug
short      duzi_ukupno          (void);

Tacka *    tacka_duzi_ldgd      (Duz *d, BOOL xpravac, BOOL levo_dole);
float      ugao_duzi_u_tacki    (Duz *d,Tacka *t);
Duz *      duz_sledeca          (Duz *d,short pravac);
/*---------------------- Priblizna matematika -------------------------------*/
BOOL jednaki(float a,float b);

/*-----------------------Analiticka geometrija-------------------------------*/
float     duz_tg      (Duz *d);
float     duz_alfa    (Duz *d);
float     l_d         (Duz *d);
BOOL      t_pripada_d (Tacka_stv t, Duz *d);
BOOL      t_je_teme_d (Tacka_stv t, Duz *d);

/*---------------------------------Grid--------------------------------------*/
short UnesenGrid(void);
BOOL  UslovZaUnosDuzi(void);
/*---------------------------------Fajl--------------------------------------*/
//void Novi_geometrija(void);
//void sacuvaj_geometriju (FILE * fajl);
void sacuvaj_tacke      (FILE * fajl);
//void sacuvaj_duzi       (FILE * fajl);
//void procitaj_geometriju(FILE * fajl);
void procitaj_tacke(short nt,FILE *fajl);
//void procitaj_duzi (short nd,FILE *fajl);
void InicParamCrtanja(void);
/*-----------------------------Oslobodi memoriju-----------------------------*/
void Oslobodi_memoriju       (void);
void Oslobodi_memoriju_tacaka(void);
void Oslobodi_memoriju_duzi  (void);
/*-------------------------------Opste---------------------------------------*/
short promeni10(short i);
/*----------------------------Elementi struktura-----------------------------*/
float  x_ (Tacka *t);  float y_ (Tacka *t);  char id_ (Tacka *t);
Tacka *d_i(Duz   *d);
Tacka *d_k(Duz   *d);
float  x_i(Duz *d);    float y_i(Duz *d);    char id_i(Duz *d);
float  x_k(Duz *d);    float y_k(Duz *d);    char id_k(Duz *d);


/*--------------------------Konvertovanje tipova-----------------------------*/
Tacka_stv t_u_T_stv(Tacka * t);
/*===========================================================================*/
/*___________________________________________________________________________
****************************** FUNKCIJE *************************************
*/


#endif