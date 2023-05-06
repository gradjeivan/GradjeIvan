#ifndef GEOMETRIJA_TI
#define GEOMETRIJA_TI

//#ifndef GEOMETRIJA_C
#include "geometrija-c.h"
//#endif

#ifndef DIJALOZI_H
#include "dijalozi.h"
#endif


#ifndef GLAVNI_MENI_ENUM
#include "gradjeivan-enums.h"
#endif


#define INFO3_KOORDTACKE 0.35
//#define BR_LINIJA 4;//broj linija za stampu na ekranu
/*_______________________________________________________________*/
/*==================prototipovi funkcija=========================*/
/*crtanja . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void BrisiCrtez        (void);

void CrtajTacku        (float x,float y,char oznaka,short pix);
void CrtajSveTacke     (Tacka *t);
void CrtajPresTacke    (NizTacaka * niz);
void PrikaziGrid       (short rezim);
void CrtajDuz          (Duz *d);
void CrtajSveDuzi      (Duz *d);
void Crtaj             (void);
void CrtajPravougaonik (Tacka_px a,Tacka_px d);
void CrtajOkvirCrt     (void);
void CrtajOkvir        (void);//za debug
//void CrtajGabarit      (void);//za debug

/*unos-dijalozi . . . . . . . . . . . . . . . . . . . . . . . . . .*/
short Grid_dlg         (short *nx,short *ny,float *dx,float *dy);
void  Unos_tacke_dlg   (Tacka *t,short *oket);
//void  Unos_duzi_dlg    (Duz *d,short *oket);

/*stampa  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void Stampaj_tacke(short sortiranje);
//void Stampaj_duzi (short sortiranje);

/*biranja sa crteza . . . . . . . . . . . . . . . . . . . . . . . . . */
BOOL      t_deo_grida   (Tacka *t, short *ix, short *iy);
Tacka *   IzaberiTacku  (Tacka *t,short tackaporedu);
void      OznaciTacku   (float x,float y,char id,short tackaporedu);
Tacka_stv IzaberiIzGrida(short ix,short iy,short *tipka,short tackaporedu);
Tacka *   IzaberiIzUnetihTacaka(Tacka *t,short * tipka);

/*pomocne i info . . . . . . . . . . . . . . . . . . . . . . . . . . . */
//void    ST_PozdravnaPoruka (short polozaj);
short   Hfont              (short font);
void    PravougaonikFill_px(short x1,short y1,short x2,short y2,short rezim);
void    InfoFill           (short i,short rezim);
void    Info               (short i,short k,float prcx,char *info,short rezim);
void    info_duz           (Duz *d);
WINDOW *NoviGlavniProzor   (short x0,short y0,short x1,short y1);
/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
void DozvoliUnosStapova   (HANDLE exec_gmeni);
void DozvoliPrikazMatrica (HANDLE exec_gmeni, short br_stap);
void DozvoliUnosStapova   (HANDLE exec_gmeni);
void DozvoliIzmenuStapova (HANDLE exec_gmeni);
void DozvoliDijagrFunkcije(HANDLE exec_gmeni, short br_dijag);
/*_________________________________________________________________*/

#endif