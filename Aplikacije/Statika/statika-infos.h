#include "statika-c.h"
#include "geometrija-ti.h"

#ifndef TXT_STAT_INFO
#define TXT_STAT_INFO

#define SEPARATOR ":::::::::::::"
#define PRAZAN_UNOS  "  . . . . . "

#endif

void            InfoGranicniUslovi     (short tip);
void            InfoKarakteristikeStapa(BOOL konst,short ipp,short imat);
void            InfoStepeniSlobode     (void);
void            InfoOpterecenja        (Duz *d,short stanje);
void            InfoStabilnost         (Duz *d);
