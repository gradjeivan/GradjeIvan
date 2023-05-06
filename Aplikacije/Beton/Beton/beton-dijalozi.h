// Header File
// Created 20.11.2013; 2:11:31

#define GRR3 "Nisu uneti brojevi. Proveri status ALPHA."

#include "beton-tipovi.h"
#include "dijalozi.h"

#define EKSTENZIJA_FAJLA "bab"

void             Unos_pressila  (ABPresek *abp);
AB_GredaStub   * Unos_GS_dlg    (ABKonstrukcija *k);
ABPresek       * IzmenaABPP     (ABKonstrukcija *k, AB_GredaStub *gs);
ABPresek       * Unos_ABPP      (ABKonstrukcija *k, AB_GredaStub *gs);
PoprecniPresek * Unos_PP        (ABKonstrukcija *k);
BOOL             NMT_dlg        (float S[3],const char *naslov);
void             Sacuvaj        (ABKonstrukcija *k, char *ime, char *info);
void             Sacuvaj_kao    (ABKonstrukcija *k);
BOOL             Otvori         (ABKonstrukcija *k);
void             Unos_mbiarm_dlg(short *mb,short *arm,short *oket);
