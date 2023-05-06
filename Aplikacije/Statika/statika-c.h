#ifndef STATIKA_C
#define STATIKA_C

#ifndef GEOMETRIJA_TIPOVI
#include "geometrija-c.h"
#endif
/* funkcije koje se koriste iz geometrija-c:
      tacaka_ukupno, l_d, t_je_teme_d, t_u_T_stv, t_pripada_d, pres_tacke_osl_mem, duz_dodaj, duz_izbrisi,
      Oslobodi_memoriju_tacaka, sacuvaj_tacke, duzi_ukupno, procitaj_tacke, tacku_nadji_po_id,
      jednaki, NoviGlavniProzor,

*/
#ifndef STATIKA_TIPOVI
#include "statika-tipovi.h"
#endif

/*--------------------------GLOBALNE PROMENLJIVE-----------------------------------*/
//               Moraju da se inicijalizuju i u _main()!!!!
          /*pointer na niz*/  /* ukupan broj el.niza */  /*  aktuelni el. niza  */
PopPres     *ppres;     unsigned br_pp;   unsigned i_pp;
Materijal   *materijal; unsigned br_mat;  unsigned i_mat;
GrUslovi    *gru;       unsigned br_gru;  unsigned i_gru;  //unsigned gru2[2];//ovo poslednje nam sluzi da pamtimo aktuelan tip stapa
                        unsigned br_stp;  unsigned i_st;
SlozeniStap *sstap;     unsigned br_sstp;
RaspOpt     *raspopt;   unsigned br_rop;
KoncOpt     *koncopt;   unsigned br_kop;   /*pocetni indeks je 0 !!*/
                           short br_dijag;/*broj stanja M0,M1,M2,..*/
//POSEBNI UPOREDNI POPRECNI PRESECI ZA I, F. Uporedni materijal.
unsigned short uppp, upp_mat;//Prvi uneti pp i materijal postace uporedni
//-----------------------------
/*Inicijalizacija glob.promenljivih*/
GrUslovi * InicirajGrUslove(void);//deklaracija samo
void InicGlobProm_statika_c(void)//Ova je ovde kompletno:Pozovi iz _main na pocetku
{br_pp   =0;  ppres    =NULL; i_pp =0;
 br_mat  =0;  materijal=NULL; i_mat=0;
 br_stp  =0;  //stap     =NULL; //i_st =0;
 br_gru  =0;  gru      =InicirajGrUslove(); //br_gru obavezno pre inicijacije gr.uslova
 br_sstp =0;  sstap    =NULL;//br.slozenih stapova(sa I!=const)
                              i_gru=0; //gru2[0]=gru2[1]=1;//Po defaultu imam osnovne tipove veza/oslonaca
 br_dijag=0;
 uppp=0; upp_mat=0;
 //tip_stapa=1;//Pocetni tip stapa je K-stap

}
/*-------------Funkcije za lakse nalazenje karakteristika stapova--------------------
             sledece f.je vracaju pokazivace*/
PopPres    *Pp_(Duz *d)       {return (ppres          +d->tata->pp); }
Materijal *Mat_(Duz *d)             {return (materijal+d->tata->mat);}
GrUslovi  *Gru_(Duz *d,short kraj)  {return (gru      +d->tata->gru[kraj]);}
RaspOpt   *Dij_(Duz *d,short stanje){return *(d->tata->dij+stanje);}//vraca pokazivac na dijagram stanja i
void      *Jos_(Duz *d)             {return  (d->tata->jos);}
            /*a sledece vracaju vrednosti*/
float        I_(Duz *d) {return ( Pp_(d))->I;   }
float        F_(Duz *d) {return ( Pp_(d))->F;   }
float        J_(Duz *d) {return ( Pp_(d))->J;   }
float        E_(Duz *d) {return (Mat_(d))->E;   }
float        G_(Duz *d) {return (Mat_(d))->G;   }
float       ni_(Duz *d) {return (Mat_(d))->ni;  }
float    alfat_(Duz *d) {return (Mat_(d))->alfa;}
/*_________________________________________________________________________________*/

/*---------------------------PROTOTIPOVI FUNKCIJA----------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void        NepoznataPomeranja_inic(BOOL **nep,BOOL rostilj);
short       brnep                  (BOOL  *nep, short brtac);
short       ind_nep_u_Knn          (short i,BOOL *nep);
//MatricaGT * MatKrutNosaca_Prr      (BOOL rostilj);
//void        superpozicija_XiMi   (float *Xi);

SlozeniStap * slozeni_stap_novi  (short brd,float *ksi,short *pp);
void          slozeni_stap_izmena(SlozeniStap *stap,short brd,float *ksi,short *pp);
Stap *           stap_novi       (           Duz *duz,BOOL konst,short pp,short mat,short *gru,short tip,void * tata);
void             stap_izmena     (Stap *stap,Duz *duz,BOOL konst,short pp,short mat,short *gru,short tip,void * tata);
PopPres   *  pop_pres_novi       (float A,float I,float J);
void         pop_pres_izmena     (float A,float I,float J,short rbr);
Materijal * materijal_novi       (float E,float ni,float alfa,float G);
void        materijal_izmena     (float E,float ni,float alfa,float G,short rbr);
GrUslovi *   gruslovi_novi       (BOOL pom[3],BOOL rot[3]);
void         gruslovi_izmena     (BOOL pom[3],BOOL rot[3],short rbr);
short *     TipStapaUGrUslove    (short tip,short *gru);
BOOL         MogucePomeranjeVeze (short gru,BOOL translacija,short osa);
void          DodajNovoStanje    (void);
RaspOpt  *    rasp_opt_novo      (float q[3],float alfa,float N);
void          rasp_opt_izmena    (RaspOpt *ro,float q[3],float alfa,float N);
float         maks_opt_za_stanje (short stanje);
void          M_u_konc           (Duz *d, short stanje, float *konc);


void       stap_podeli_u_prestac(NizTacaka *niz);
Duz  *     stap_podeli_u_t      (Duz *d,Tacka *t);
void       stapove_stampaj      (void);

void    Oslobodi_memoriju_nosaca(void);
void     sacuvaj_materijale     (FILE * fajl);
void   	 sacuvaj_pop_preseke    (FILE * fajl);
void     sacuvaj_duzi_i_stapove (FILE * fajl);
void     Sacuvaj_Nosac          (FILE * fajl, short ver[]);
void     Oslobodi_memoriju_greda (void);
void     Procitaj_Nosac         (FILE *fajl,short minver[]);
void     procitaj_materijale    (FILE *fajl);
void     procitaj_pop_preseke   (FILE *fajl);
void     procitaj_duzi_i_stapove(FILE *fajl);

Veresc * novi_Veresc            (void);
void     brisi_baf_Veresc       (Duz *d);
void     brisi_sve_Veresc       (void);
short    Veresc_Tip             (float q[2]);
float    Prr_Veresc             (Duz *d,short stanje1,short stanje2,BOOL txtprr);
void     Prr_VekSlobClanova     (float **dnula,float *d0t,BOOL *dotd0);
void     slozstap_listapreseka  (Stap *nstap, SlozeniStap *sloz);
/*_________________________________________________________________________________*/

#endif