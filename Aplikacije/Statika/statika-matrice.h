#ifndef STATIKA_MATRICE
#define STATIKA_MATRICE

#include "gradjeivan-tipovi.h"

/*zbog globalnih mora geometrija-c i statika-c, resice se kasnije sa Nosac globalnom*/
#include "geometrija-c.h"
#include "statika-c.h"

/**/
#include "geometrija-ti.h"
#include "statika-graficki unos.h"

/*ovo ne moze da se izbegne, a i ne mora jer ide ionako kasnije u grimtprr*/
#include "grimtprr-load.h"
#include "statika-grimtprr.h"

void StampajMatricu_glprozor(Matrica    *M, char **zaglavlje);
void StampajMatricuGT       (MatricaGT *GT, char **zaglavlje);

void StampajMatTrans        (BOOL rostilj);
void StampajMatKrutGlob     (BOOL rostilj,short sttb,float uporedni[3]);

void MatRavnoteze_Stampa    (Duz *d,short duzina);
void KonstanteStapa_Stampa   (float *elFo, float *konst, Stap *s);

#endif