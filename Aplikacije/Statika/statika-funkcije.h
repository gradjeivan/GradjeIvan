#include "gradjeivan-tipovi.h"
#include "dijalozi.h"
#include "grimtprr-load.h"
#include "statika-c.h"
#include "statika-dijalozi.h"
#include "statika-infos.h"
#include "statika-grafika.h"
#include "statika-graficki unos.h"
#include "statika-matrice.h"
/*--------------------------------------------------------------------------------*/
/*-----------------------PROTOTIPOVI FUNKCIJA-------------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
//void            IzbrisiSvePc           (void);
BOOL            Unos_P_stabilnost      (void);
BOOL            Unos_IlPc              (float *Ic, float *lc, float *Pc);
void            StampajMatKg_stabilnost(float *uporedni);
void            StampajMatK_stabilnost (float *uporedni);
//MatricaGT * MatK_stabilnost        (Duz * d,float Ic,float lc,Materijal *materijal,PopPres *ppres);

void            NepoznataPomeranja_unos (BOOL *nep);
void            MatKrutNosacaKnn_Stampaj(BOOL rostilj,short sttb,float uporedni[3]);
void            MatKrutNosaca_Stampaj   (BOOL rostilj,short sttb,float uporedni[3]);
void            StampajMatKrut          (BOOL rostilj);

void            SuperpozicijaM         (float *Xi);
void            Uporedni_pp            (void);
void            ElasticneTezine_Prikaz (void);

//void            MatKrut                (Duz *d,BOOL rostilj);

void            Novi_                  (void);

MatricaGT *     MatKoefGT              (void);
Matrica   *     MatKoef                (void);
void            Verescagin             (void);
void            UslovneJednacine_Prr   (Matrica **D,float **d0,float **Xi,Matrica **Dinv,float *d0t,BOOL *dotd0);
void            UslovneJednacine_Prikaz(Matrica  *D,float  *d0,float  *Xi,Matrica  *Dinv,float *d0t,BOOL *dotd0);

void            Izbor_sabiraka_za_d0   (BOOL *dotd0);
BOOL            Zadaj_delta_0t         (float **d0t);
void            GrupeDijagrama         (void);
void            ZadajDijagram          (void);
short           IzborDijagrama         (void);
void            UnosDijagrama          (short stanje);
BOOL            UslovZaUnosStapova     (void);
BOOL            Unos_materijala        (void);
BOOL            Unos_preseka           (void);
unsigned short  IzborMaterijala        (void);
unsigned short  IzborPoprecnogPreseka  (BOOL slozenistap);
unsigned short  IzborGranicnihUslova   (void);

BOOL            IzmenaPoprecnogPreseka (Stap *nstap,SlozeniStap *slst);//menja prosledjene promenljive, dodaje nove pp i slozene stapove u nizove ppres i sstap
BOOL                KarakteristikeStapa(Duz *d, SlozeniStap *slst, short karak);//Pozivanje dijaloga za unos/promenu ODREDJENIH karakteristika stapa
void          IzmeniKarakteristikeStapa(Duz *d);//Izbor karakteristika vec unetog stapa, a to sve za unos/promenu
void            Stapovi_izmena         (void);  //Izbor i izmena karakteristika vec unetih stapova

short           Unos_stapa_ekr         (Duz *d);
void            Unos_stapova           (void);

BOOL            FormirajSlozeniStap    (Stap *nstap,SlozeniStap *slst);//Upis u sstap se NE vrsi ovde