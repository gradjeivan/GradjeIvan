// C Source File
// Created 13.02.2015; 3:09:42

#include <tigcclib.h>
#include "pgbk2-dijalozi.h"

//-=-=-=-=-=--=-=-=-= UNOS ZADATIH PODATAKA -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL Unos_opterecenja_dlg(ABKonstrukcija *k)
{
	char  ff[]="%-^5.4f";
  PodZaDlg pd[4]={{10, &(k->opt.pregzid),"preg.zidovi+podovi=" ,ff},
                  {10, &(k->opt.fasada) ,"fasada="             ,ff},
                  {10, &(k->opt.p)      ,"korisno opt.="       ,ff},
                  {10, &(k->opt.w)      ,"vetar="              ,ff}
                 };
  char zaglavlje[22];sprintf(zaglavlje,"Opterecenja");
  return Dlg_(4,1,pd,zaglavlje);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL Unos_zgrada(ABKonstrukcija *k)
{
  char  ff[]="%-^5.4f",
        dd[]="%-^2d";
  PodZaDlg pd[2]={{ 2, &(k->zgrada.bretaza),"broj etaza:" ,dd},
                  {10, &(k->zgrada.Wi)     ,"Wi[kN] sprata(za din.analizu):",ff}
                 };
  return Dlg_(2,1,pd,(const char *){"Zgrada"});
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL Unos_periodoscilovanja(float *T)
{
  char  ff[]="%-^5.4f";
  PodZaDlg pd[1]={{10, T,"T[s]=" ,ff}};
  return Dlg_(1,1,pd,(const char *){"Period oscilovanja"});
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
BOOL Unos_seizmika_dlg(ABKonstrukcija *k)
{
//char zaglavlje[17];sprintf(zaglavlje,"Izbor materijala");
// allocate memory
 short oket,
       brpadmenija=4;
 short *izbor = NULL;if ((oket=bafer_padmeni(&izbor,brpadmenija))==0) return FALSE;
 HANDLE dlg=H_NULL;
 if ((oket=novidijalog(&dlg,125,70))==0){free(izbor);return FALSE;}

 //pocetne vrednosti i,k, u listama (ne smeju da krecu od 0)
 izbor[0] = k->seizmika.zona - 6;
 izbor[1] = k->seizmika.kattla;
 izbor[2] = k->seizmika.katobj;
 izbor[3] = k->seizmika.savremena_kja? 1:2;

 short tpk = 1; //ovo je uneta/pritisnuta tipka

  DialogAddTitle(dlg,"Seizmika:podaci",BT_OK,BT_CANCEL);
  HANDLE izbor_zona = PopupNew ("zona",0);   // padajuci meniji
  padajuci_popuni(izbor_zona,(const char*[]){" VII","VIII","  IX"},3);
  DialogAddPulldown (dlg, 10, 15, "zona", izbor_zona, 0);

  HANDLE izbor_ktla = PopupNew ("kategorija tla",0);   // padajuci meniji
  padajuci_popuni(izbor_ktla,(const char*[]){"  I"," II","III"},3);
  DialogAddPulldown (dlg, 10, 25, "kategorija tla", izbor_ktla, 1);

  HANDLE izbor_kobj = PopupNew (NULL,0);   // padajuci meniji
  padajuci_popuni(izbor_kobj,(const char*[]){"  I"," II"},2);
  DialogAddPulldown (dlg, 10, 35, "kategorija objekta", izbor_kobj, 2);

  HANDLE izbor_savkja = PopupNew (NULL,0);   // padajuci meniji
  padajuci_popuni(izbor_savkja,(const char*[]){"DA","NE"},2);
  DialogAddPulldown (dlg, 10, 45, "savremena k-ja?", izbor_savkja, 3);

  while ((tpk != KEY_ENTER) && (tpk != KEY_ESC) && tpk > 0)
        {
          //char s[25]; sprintf(s,"MB:%d, arm:%d",*izbor,*(izbor+1)); ST_helpMsg(s); ngetchx();
          tpk = DialogDo(dlg,CENTER,CENTER,NULL,izbor);
        }
          //char s[25]; sprintf(s,"MB:%d, arm:%d",*izbor,*(izbor+1)); ST_helpMsg(s); ngetchx();
  if (tpk == KEY_ENTER){ oket=1;
                         k->seizmika.zona=izbor[0]+6;
                         k->seizmika.kattla=izbor[1];
                         k->seizmika.katobj=izbor[2];
                         k->seizmika.savremena_kja=(izbor[3]==1);
                       }
  else  oket=0;
 //obavezno oslobadjanje dinamicke memorije
 HeapFree(dlg);HeapFree(izbor_zona);HeapFree(izbor_ktla);HeapFree(izbor_kobj);HeapFree(izbor_savkja);free(izbor);
 return (oket!=0);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Unos_dimenzija_dlg(short *dpl,short *bgr,short *dgr,short *bs,short *ds,short *oket)
{
char *unos = NULL;if ((*oket=bafer_unosteksta(&unos,30))==0) return;
HANDLE dlg=H_NULL;if ((*oket=novidijalog(&dlg,145,60))==0){free(unos);return;}
sprintf(unos,"%-5d",*dpl);
sprintf(unos+6,"%-5d",*bgr);sprintf(unos+12,"%-5d",*dgr);
sprintf(unos+18,"%-5d",*bs);sprintf(unos+24,"%-5d",*ds);
DialogAddTitle(dlg,"Pretpostavke dimenzija [cm]",BT_OK,BT_CANCEL);
short tpk = 1; //ovo je uneta/pritisnuta tipka
  DialogAddRequest(dlg,11,15,"           dpl:",0,5,6);//DialogAddText(dlg,92,15,"[m2]");
  DialogAddRequest(dlg,10,25,"grede b/d:",6,11,6);DialogAddRequest(dlg,90,25,"",12,17,6);
  DialogAddRequest(dlg,10,35,"   stub b/d:",18,23,6);DialogAddRequest(dlg,90,35,"",24,29,6);
short dobarunos=0;*oket=0;
while (dobarunos==0)
{ while ((tpk != KEY_ENTER) && (tpk != KEY_ESC) && tpk > 0)
        {	tpk = DialogDo(dlg,CENTER,CENTER,unos,NULL);   }
  if (tpk == KEY_ENTER)
     { dobarunos=txtui(unos,dpl);
       if (dobarunos==1) dobarunos=txtui(unos+6,bgr);
       if (dobarunos==1) dobarunos=txtui(unos+12,dgr);
       if (dobarunos==1) dobarunos=txtui(unos+18,bs);
       if (dobarunos==1) dobarunos=txtui(unos+24,ds);
       if (dobarunos==0) tpk=1; else *oket=1;
     }
  else  {*oket=0; dobarunos=1;}
}
//obavezno oslobadjanje dinamicke memorije
HeapFree(dlg);free(unos);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=