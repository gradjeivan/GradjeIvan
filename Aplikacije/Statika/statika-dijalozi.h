/*--------------------------------------------------------------------------------*/
#ifndef DIJALOZI_H
#include "dijalozi.h"
#endif


/*--------------------------------------------------------------------------------*/
/*-----------------------PROTOTIPOVI FUNKCIJA-------------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
BOOL  Unos_P_stab_dlg     (float *P);
short IzborNacinaUnosaIlpc(void);
BOOL  Unos_IlPc_dlg       (float *Ic,float *lc,float *Pc);
short IzabNacinZadTackePostStapa(void);//Izbor nacina zadavanja tacke na stapu:procentualno ili duzinski

BOOL  Unos_ksi_stapa      (float *ksi,char znak,short rbr);

BOOL  Unos_dijag_dlg      (float *pi ,float *pk  ,float *f,float *alfa,float *N);
BOOL  Unos_koncopt_dlg    (float *F  ,float *alfa);
BOOL  Unos_materijala_dlg (float *me ,float *ni  ,float *alfa,float *G,short rbr);
BOOL  Unos_preseka_dlg    (float *mi ,float *mf  ,float *jt,           short rbr);
BOOL  Delta_0t_dlg        (float *d0t,short br_dijag);

void  IzborRavanRostilj   (BOOL *rostilj);
void  DozvoliPrikazMatrica(HANDLE exec_gmeni, short br_stap);
/*--------------------------------------------------------------------------------*/
