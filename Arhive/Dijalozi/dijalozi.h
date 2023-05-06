#ifndef DIJALOZI_H
#define DIJALOZI_H
//#define SET_FILE_IN_USE_BIT

#ifndef MIN_AMS
#define MIN_AMS 200
#endif

#define GRR  "Greska!"
#define GRR1 "Nedovoljno memorije za input bafer."
#define GRR2 "Nedovoljno memorije za dijalog."
#define GRR3 "Nisu uneti brojevi. Proveri status ALPHA."

#define XPAD  5
#define XRAST 8
#define YPAD  14
#define YRAST 10

#define FONT_INFOA F_4x6
#define RAZMAK_TEKSTA 2
#define MENU_DUZINA   100
#define MENU_VISINA   18
#define LISTA_DESNO   50
#define KOMAND_       6
#define KOMAND_L      LCD_WIDTH
#define INFO3_H       8

#ifndef DIJALOZI_TIPOVI
#include "dijalozi-tipovi.h"
#endif

/* m,n   - dim.tabele, m odgovara duz.niza za stampu,n odg.duz.strukture(==duz.ft)
   *ft   - format za polja tabele
 *pmem   - formatni niz-string koji sadrzi tip za svaki el.niza *el (prvih n elemenata niza)
           podrazumeva se da je raspored tipova prom. za svaku vrstu isti
           Dozvoljeni tipovi su: f,i,I,c,.. vidi dole switch blok.
           IZBACIO SAM TIP STRING-usporava-mogao bi samo ako je uvek iste duzine u svakom elementu strukture
           Primer: *pmem[]="ffcif" => struktura sadrzi {float,float,char,unsigned short,float}
*/

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*                           PROTOTIPOVI                                  */
/*------------------------------------------------------------------------*/
void          ST_obrisi          (void);
void          Info               (short i,short k,float prcx,char *info,short rezim);
/*                                      i=r.br.info prozora, k=linija za stampu,
                                        prcx={0-1}=odredjuje % duzine od koje krece tekst
                                       *info=string za stampu                   
*/

void          InfoFill           (short i,short rezim);
WINDOW *      NoviProzor         (short x0,short y0,short x1,short y1,BOOL aktiviraj);

short         IskacuciMeni       (short bropcija,const char ** opcije,const char * naslov);
/*            PRIMER:
                 char* xyfi[3];
                    for (i=0;i<3;i++)
                        {xyfi[i]=(char*)calloc(4,sizeof(char));
                         sprintf(xyfi[i],"%c",(i==0?'X':(i==1?'Y':145)) );
                         if (nep[ind+i]) sprintf(xyfi[i],"%s %c",xyfi[i],15);//stikliranje
                        }
                 izbor=IskacuciMeni(3,(const char **)xyfi,"Nepoznata pomeranja");
*/

BOOL          Dlg_               (short n,short m,PodZaDlg *pd,const char *zaglavlje);
/*            PRIMER:
                  char  ff[]="%-^5.4f";
                  PodZaDlg pd[3]={{10,Q,  "Q=" ,ff},{10,n, "n=" ,ff},{10,B,  "B="  ,ff}};
                  char zaglavlje[22];sprintf(zaglavlje,"Inicijalni podaci");
                  return Dlg_(3,1,pd,zaglavlje);
*/
short         bafer_padmeni      (short **bafer,short br_padmenija);
short         bafer_unosteksta   (char  **bafer,short duzina_bafera);
short         novidijalog        (HANDLE *dlg,  short x,short y);
short         txtuf              (char *txt,float *flt);
short         txtui              (char *txt,short *shrt);

BOOL          Otvori_fajl_dlg    (char * ime, char *ekstenzija);
BOOL          SacuvajFajl        (char * ime, char *ekstenzija);
//char * Lista_fajlova      (short brfajlova);

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#endif
