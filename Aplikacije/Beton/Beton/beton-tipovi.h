#ifndef BETON_TIPOVI
#define BETON_TIPOVI

/*JEDINICE: kN, cm*/

#define EA 21000
enum Armatura { _RA=0, _GA=1};
#define SIGMA_V   40,    24
//enum FiArmat  { _
enum MarkaBetona          {_15=0, _20=1, _25=2, _30=3, _35=4, _40=5, _45=6, _50=7, _55=8, _60=9};
               #define MARKA  15,   20 ,   25 ,   30 ,   35 ,   40 ,   45 ,  50  ,   55 ,  60
/*kN/cm2*/     #define _FB  1.05,   1.4, 1.725,  2.05,   2.3,  2.55, 2.775,  3   ,  3.15,  3.3
/*GPa=10^6*/   #define _EB    27,  28.5,   30 ,  31.5,   33 ,   34 ,   35 ,  36  ,   37 ,  38
/*kN/cm2*/     #define _TAUR 0.06, 0.08, 0.095,  0.11,  0.12,  0.13,  0.14,  0.15, 0.155,  0.16

enum SlOpter{ _STALNO=0, _POVREMENO=1, _SEIZMIKA=2 };//_KORISNO=1, _VETAR=2,

//======================================================================
/*DEFINICIJE TIPOVA=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

//======================================================================
/* Prototipovi
========================================================================*/
typedef struct ABKonstrukcija ABKonstrukcija;
typedef struct PoprecniPresek PoprecniPresek;
typedef struct ABPresek       ABPresek;
typedef struct AB_GredaStub   AB_GredaStub;
typedef struct Zid            Zid;
//======================================================================
/* PRESECNE SILE
========================================================================*/
typedef struct{
  float S[3];//N,M,T
  short id;//slucaj opterecenja
}slucajopterecenja;//pres.sile za odredjeni slucaj opterecenja

typedef struct{
  short brs;
  slucajopterecenja *so;
}PresSile;

PresSile * PS_Prazno    (void);
PresSile * PS_NoviSlucaj(PresSile*  ps, short id, float N, float M, float T);
void       PS_DodajSO   (ABPresek *abp, short kom);
void       PS_Brisi     (PresSile** ps);
//======================================================================
/* POPRECNI PRESEK - opsti (odnosno, ovde: betonski deo)
========================================================================*/
// Upisuje se u zbirku koja vazi na nivou cele konstrukcije, tako da moze
// da se upotrebi za vise razlicitih preseka (stednja memorije, ne ponavljamo)
//----------------------------------------------------------------------
enum TipoviPoprecnihPreseka{
	_PROIZVOLJAN=1, _PRAVOUGAONI=2, _KRUZNI=3, _T_PRESEK=4
};
//---------------------------------------------
typedef struct{
	float F,I,J;  //float I; //float J;
}PP_Proizvoljni;
//---------------------------------------------
typedef struct{
	float b,d; //float d;
}PP_Pravougaoni;
//---------------------------------------------
typedef struct{
	float b,d;
	float B,dp;  //float dp;
}PP_T_Presek;
//---------------------------------------------
struct PoprecniPresek
{
  short id,//pocinje od 1
        tip;
  union{
  	PP_Proizvoljni proiz;
  	PP_Pravougaoni pravoug;
  	PP_T_Presek    Tpresek;
  }pp;

  PoprecniPresek *sused[2];
};
//---------------------------------------------
//FUNKCIJE:---Konstruktor, destruktor...-------
PoprecniPresek * PP_Prazan (          short tip);
PoprecniPresek * PP_Novi   (short id, short tip);
PoprecniPresek * PP_Dodaj  (ABKonstrukcija * k, PoprecniPresek *pp, short tip);
                //staviti pp=NULL ako se zeli da se napravi nov poprecni presek i onda doda u zbirku,
               // a ako se dodaje postojeci popr.pres. onda taj pp
short            PP_ukupno (ABKonstrukcija *k);
BOOL             PP_brisi  (PoprecniPresek *pp);
PoprecniPresek * PopPresek (ABKonstrukcija *k,short rbr);
              //vraca popr.pres. pod zadatim rednim brojem (pocev od 0)

//======================================================================
/* AB Poprecni Presek: opsti pp + sipke + presecne sile
========================================================================*/
//Upisuje se u zbirku na nivou jedne pozicije greda/stub,
//tako mozemo da imamo bilo koliko karakteristicnih preseka za 1 gredu/stub
//----------------------------------------------------------------------
typedef struct{
  short fi,
        ukupno,
 	      brredova,
 	      u1redu,
 	      uposlredu;
}Sipke;
//Sipke InicSipke()
//---------------------------------------------
struct ABPresek
{
  char id;
  float ksi;//relativni polozaj pp na gredi (0<ksi<1)
  PoprecniPresek *pp;
  PresSile       *ps;
  Sipke          sipke[2];
  float          a[2];      //polozaj zategnute i pritisnute armature
  ABPresek      *sused[2];
};
//-----------------------------------------------
//FUNKCIJE:---Konstruktor, destruktor...-------
ABPresek * ABpp_Novi  (void);
ABPresek * ABpp_Dodaj (AB_GredaStub *gs, ABPresek *abp);
BOOL       ABpp_brisi (                  ABPresek *pp);
ABPresek * ABpp_poredu(AB_GredaStub *gs, short rbr);
short      ABpp_ukupno(AB_GredaStub *gs);
//======================================================================
/* AB Greda/Stub
========================================================================*/
struct AB_GredaStub
{
  short id;
  float L;        //duzina[cm]
  short statsist; //staticki sistem grede/stuba
  ABPresek *pp[2];//krajnji preseci
  AB_GredaStub *sused[2];
};
//-----------------------------------------------
//FUNKCIJE:---Konstruktor, destruktor...-------
//AB_GredaStub * GS_Prazan(void);
AB_GredaStub * GS_Novi  (void);
AB_GredaStub * GS_Dodaj (ABKonstrukcija *k, AB_GredaStub *gs);
BOOL           GS_brisi (                   AB_GredaStub *gs);
short          GS_ukupno(ABKonstrukcija *k);
//======================================================================
/* ZID
========================================================================*/
struct Zid{
  float xy[2][2];//koordinate krajeva zida [ik][xy] = [01][01]
  float dz;      //debljina zida
};
//======================================================================
/* KONSTRUKCIJA
========================================================================*/
//typedef
struct ABKonstrukcija{
//ako neki short nije jos unet, postavljen je na -1 da bi se znalo
  short mb, arm;//indeksi u tablicama

  struct{
    float dg,p,pregzid,fasada,w;
  }opt;//opterecenje

  struct{
    short zona,kattla,katobj;
    BOOL savremena_kja;
  }seizmika;

  struct{
    short bretaza;
    float Wi;
  }zgrada;

  PoprecniPresek *pp[2];//pokazuje na pocetak i kraj niza unetih poprecnih preseka
  AB_GredaStub   *gs[2];//                      -||- niza unetih greda/stubova

  //aktuelna pozicija:
  AB_GredaStub   *ags;
  ABPresek       *abp;
};
//-----------------------------------------------
void InicABKonstr( ABKonstrukcija *k);
//-----------------------------------------------

//======================================================================
/* DIJAGRAM INTERAKCIJE
========================================================================*/
typedef struct {
	short armatura;
	float alfa,
	      k;//alfa=a/d  //	float k;//odnos Aa2/Aa1
	//float **nu, **mu //pokazivaci na matrice sa karakteristicnim tackama dijagrama
}DijagramInterakcije;
//---------------------------------------
typedef struct{
	BOOL arm;//TRUE ako je zadata dilatacija armature a1
	union {
		float ea,eb;
	};
}DilatacijeNaIvici;

typedef struct{
	DilatacijeNaIvici z,p;
}Dilatacije_PP;
//---------------------------------------

#endif
