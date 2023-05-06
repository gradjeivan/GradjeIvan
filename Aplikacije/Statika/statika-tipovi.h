/*------------------------------------------------------------------------------------*/
#ifndef STATIKA_TIPOVI
#define STATIKA_TIPOVI

/*--------------------------------KONSTANTE----------------------------------------*/
enum TipStapa  {K_STAP=1, oG_STAP=2, Go_STAP=3, PROST_STAP=4,
                S_STAP=5, K_CISTO_SAV=6, ZIDNO_PLATNO=7};
                //Ako ovde menjas, moraces i redosled na listi u IzborGranicnihUslova
enum TipDijag  {  TROUGAO_I=1,      TROUGAO_K=2,       TRAPEZ=3, PRAVOUGAONIK=4};
enum TipGrUsl  { KRUTI_UGAO=1,          ZGLOB=2,    SLOB_KRAJ=3,       S_VEZA=4};
enum TipOslonca{UKLJESTENJE=1, NEPOKRETNI_OSL=2, POKRETNI_OSL=3};

/*PROTOTIPOVI*///redosled: od najmladjeg ka najstarijem
/*typedef struct Nosac Nosac;*///prototip dat u headeru geometrija-tipovi
//typedef struct Duz_Metode   Duz_Metode;

/*GEOMETRIJA-C*///--------------------------------------------------------------------------
#ifndef GEOMETRIJA_TIPOVI
#include "geometrija-tipovi.h"
#endif

/*STATIKA-C*///-----------------------------------------------------------------------
typedef struct
{ float F;
  float I;
  float J;
}PopPres;
typedef struct
{ float E;
  float ni;
  float alfa;
  float G;
}Materijal;
typedef struct
{ //char id;
  BOOL pom[3];//TRUE za dozvoljeno pomeranje/rotaciju
  BOOL rot[3];//u pravcu[i]=[0,1,2]=[x,y,z]
  /*float *zadato;*/ /*POKAZIVAC NA NIZ ZADATIH POMERANJA I ROTACIJA
                 == NULL ako ih nema*/
}GrUslovi;/*Granicni uslovi u tacki*/

typedef struct
{float   q[3];//trebalo je da bude jednacina promene opterecenja: ax^2+bx+c, a=q[2],c=q[0]
 /*q[0]=pi, q[1]=pk, trapez;   q[2]=f parabole*/
 float   alfa;//ugao pod kojim deluje opterecenje, alfa=0 upravno na stap
 float   N;
 //Stap  * stap;//stap kome pripada opterecenje
}RaspOpt;

typedef struct
{float   F;//intenzitet
 float   alfa;//ugao
 //Tacka * t;	//tacka u kojoj deluje
 BOOL    sila;//TRUE ako je u pitanju sila, FALSE ako je moment
}KoncOpt;

struct Stap
{Duz    * duz;
 BOOL     ppkonst;//TRUE za konstantan pop.presek
 short    pp;/*redni broj tipa popr.preseka POCEV OD 0*/
 short    mat;//redni broj materijala
 short    gru[2];//granicni uslovi za tacke i i k
 short    tip;
 RaspOpt **dij;//pokazuje na pokazivace dijagrama po stanjima M0,M1,...
 void   * jos;//pomocni void pointer-multipraktik; npr. moze da pokazuje na Verescagina za taj stap
 void   * tata;//ili NULL ili pokazivac na SlozeniStap kome pripada
};

typedef struct
{short  brd;//broj duzi/stapova koji cine slozeni stap
 float *ksi;//ksi medjutacaka: ukupno brd-1
 short *pp; //poprecni preseci svakog dela: ukupno brd
}SlozeniStap;

typedef struct
{char **racun;  //pokazivac na niz[4] tj. matricu[2x2] stringova sa racunom: linearnoXlinearno,linXparab. za svako stanje
 float  ukupno; //ukupna vrednost proizvoda dijagrama
}Veresc;

typedef struct
{PopPres     *ppres;     unsigned br_pp;   unsigned i_pp;
 Materijal   *materijal; unsigned br_mat;  unsigned i_mat;
 GrUslovi    *gru;       unsigned br_gru;  unsigned i_gru;  //unsigned gru2[2];//ovo poslednje nam sluzi da pamtimo aktuelan tip stapa
                         unsigned br_stp;  unsigned i_st;
 SlozeniStap *sstap;     unsigned br_sstp;
 RaspOpt     *raspopt;   unsigned br_rop;
 KoncOpt     *koncopt;   unsigned br_kop;   //pocetni indeks je 0 !!
                            short br_dijag;//broj stanja M0,M1,M2,..
 //POSEBNI UPOREDNI POPRECNI PRESECI ZA I, F. Uporedni materijal.
 unsigned short uppp, upp_mat;//Prvi uneti pp i materijal postace uporedni
}SlucajOpt;

struct Nosac
{exttck     kt;
 extduz     kd;
 ParamCrt   pc;
 BOOL       rostilj;
 SlucajOpt  so;
};

#endif
//kraj #ifndef STATIKA_TIPOVI