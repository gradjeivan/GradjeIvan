/*U ovom delu  pripremam funkcije za prebacivanje u <grimtprr.h> */

#include "gradjeivan-tipovi.h"
#include "grimtprr-load.h"

void Matrica_PripremaTabKol (Matrica    *M, char **zaglavlje, char ***s, char *pmem, FrmtTabKol *fk);

void StampajMatricu         (Matrica    *M, char **zaglavlje, WINDOW *w);

void FrmtTab_Izbaci   (FrmtTab *ft, short ind,  short vk);
void FrmtTab_Zameni   (FrmtTab *ft, short i[2], short vk);
