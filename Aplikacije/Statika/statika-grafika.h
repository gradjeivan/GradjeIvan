#ifndef STATIKA_GRAFIKA
#define STATIKA_GRAFIKA

#include "gradjeivan-tipovi.h"
#include "geometrija-c.h"
#include "geometrija-ti.h"
#include "statika-c.h"

#define HDJG 25//Max visina dijagrama
/*                                  PROTOTIPOVI FUNKCIJA                                        */
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void  CrtezOsvezi         (void);

void  CrtajOpterecenje    (Duz * d);
void  CrtajSimbolVeze     (short x, short y, short veza);
void  CrtajVezeuTacki     (Tacka *t);
void  CrtajSveZglobove    (void);
void  CrtajStap           (Duz *d);
void  CrtajNosac          (void);
//void GrUsloviInfoCrtez   (short tip);
void  OznaciStap          (Duz * d);
void  CrtajDijagram       (Duz *d, short dij, float razmera);
float CrtajSveDijagrame   (short dij);
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-EKSTERNE PROMENLJIVE=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
extern unsigned char    zgl5_and[5], zgl5_or[5],//Veliki zglob
                        zgl4_and[4], zgl4_or[4];//Mali zglob
extern unsigned short   pokr_osl[16];

import_binary("zglob8x5.bin",zgl5_or); import_binary("zglob-iza8x5.bin",zgl5_and);
import_binary("zglob8x4.bin",zgl4_or); import_binary("zglob-iza8x4.bin",zgl4_and);
import_binary("pokretni.bin",pokr_osl);

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-GLOBALNE PROMENLJIVE=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
struct Crtez
{unsigned short   vel; //velicina ikone u px
 unsigned char  * and; //pokazivac na Sprite AND
 unsigned char  * or;  //pokazivac na Sprite XOR
 short            x;
 short            y;   //ekscentricitet u odnosu na tacku crtanja
}crt[2]={
         {5,zgl5_and,zgl5_or,2,2},
         {4,zgl4_and,zgl4_or,4,2}
        };

#endif