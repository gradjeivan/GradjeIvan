/*Enumeracije ------------------------------------------------*/
#ifndef GLAVNI_MENI_ENUM
#include "gradjeivan-enums.h"
#endif

#include "geometrija-ti.h"

#ifndef TXT_UNOS_GEOM
#define TXT_UNOS_GEOM
#define TXT_UNOS_DUZI "Unos duzi"
#define IZAB_TACKU "Izaberi tacku"
#define APP_CAT_ESC "ili APPS/CATALOG/ESC"
#define TACKA_POSTOJI "Vec postoji! Izaberi drugu tacku ili ESC."

#endif

/*Prototipovi funkcija----------------------------------------*/

//void Iniciraj_glavni_meni(HANDLE *gmeni);
void Unos_tacaka(void);

short Unos_duzi_graficki(Duz *d);
short Unos_duzi_ekr(Duz *d);
void  Unos_duzi(void);

void Zumiranja(short tipka);
void Strelice (short tipka);

void Vidljivost(void);
BOOL Grid_podesavanje(void);
void pomocni_prozor(HANDLE exec_handle, short ver[]);
