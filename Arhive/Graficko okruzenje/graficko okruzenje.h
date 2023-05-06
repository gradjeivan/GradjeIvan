/*
VERZIJA: 0
*/

#ifndef GRAF_OKR
#define GRAF_OKR

typedef struct
{
  short vel_oznake,
        odalji_tekst,
        menuL,
        menuH,
        lista_desno,
        komand,
        komandL,
        font_infoa,
        razmak_teksta,
        info3H;

   float info3_koord_tacke;

} RasporedRazmaci;

void InicRasporedRazmaci(RasporedRazmaci *rr)
{
 rr->vel_oznake=     3;
 rr->odalji_tekst=   2;
 rr->komandL=
 rr->menuL=        115;
 rr->menuH=         18;
 rr->lista_desno=   40;
 rr->komand=         6;
 rr->font_infoa=    F_4x6;
 rr->razmak_teksta=  2;
 rr->info3H=         8;
 rr->info3_koord_tacke=0.35;
}

#endif