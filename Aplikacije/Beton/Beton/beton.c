// C Source File
// Created 20.11.2013; 2:10:53

#include "beton.h"
/*================================================================*/
//Dijagrami Interakcije-------------------------------------------
//static BOOL  NM_dlg                 (float N[3], float M[3]);
//static BOOL  bdh_dlg                (float *b, float *d, float *h);
//static void  AzurirajInfo           (DijagramInterakcije di, float M[], float N[], float b,float d,float h, short mb);
//static float AzurirajPrr            (WINDOW *w, DijagramInterakcije di, short br_NM, float M[],float N[],
  //                                   float e, float b,float d, short mb, float epsA1, BOOL povoljnoG);
//float StubEkscentricitet_Prr (WINDOW *w[], short br_NM, float M[],float N[],float b,float d,float hi,short mb);
//static void  Karakteristicne_linije (DijagramInterakcije di);

//----------------------------------------------------------------
void        DI_Alat              (DijagramInterakcije *di);
void        Seizmika_Analiza     (ABKonstrukcija *k);

static void Iniciraj_glavni_meni (HANDLE *gmeni);
void        OslobodiMemoriju     (void);
void        Novi                 (void);
/*================================================================*/
void Pozdrav(WINDOW *w,short x,short y)
{
 char s[35];
 short ver[]={COMMENT_VERSION_NUMBER};

 WinFont(w,F_8x10); WinStrXY (w, x, y, "BAB 87");
 WinFont(w,F_4x6);
 sprintf(s,"v %d.%d.%d.%d",ver[0],ver[1],ver[2],ver[3]);
 WinStrXY (w, x+10, y+12, s);
 sprintf(s,"GRA%cEIvan",208); WinStrXY (w, x+5, y-10, s);

}
void NapustiProgram(LCD_BUFFER *home_ekran)
{
 char ime[]="poslednj";
 Sacuvaj(kja,ime,NULL);
 //Oslobodi_mem(r); //WinClose(w); //HeapFreePtr(w);
 OslobodiMemoriju();
 LCD_restore(home_ekran);
 ST_helpMsg(GRADJEIVAN);ST_busy(0);
 exit(0);
}
//------------------------------------------------------------------

/*GLAVNA FUNKCIJA
====================================================================*/
void _main(void)
{

 /*Provera serijskog broja*///--------
  if (!(registrovan_kalkulator(NULL,FALSE)))return;
  //-----------------------------------

 /*CRTEZ iniciraj sve-----------------------------------------------*/
  RasporedRazmaci rasraz;  rr=&rasraz; InicRasporedRazmaci(rr);
  rr->lista_desno=50;  rr->menuH=18;  rr->menuL=115;
  //RasporedRazmaci rr=pc->rr;

  LCD_BUFFER home_ekran;  LCD_save(&home_ekran);
  clrscr();   //CrtajOkvirCrt();
  //ST_PozdravnaPoruka(1);
 /*-----------------------------------------------------------------*/

 /*Grafika--------------------------------------------------------*/
  WINDOW *w1=NoviProzor(0, rr->menuH-3, LCD_WIDTH -rr->lista_desno, LCD_HEIGHT -rr->komand -rr->info3H,TRUE);
  Pozdrav(w1,45,25);
  HANDLE gmeni = MenuNew (MF_NO_NUMS, rr->menuL, rr->menuH);
  Iniciraj_glavni_meni(&gmeni);
  HANDLE exec_gmeni = MenuBegin (NULL, 0, 0, 0x20, gmeni);
  /*==============================================================*/

  /*Beton-----------------------------------------------------------*/
  ABKonstrukcija konstrukcija; kja=&konstrukcija;  InicABKonstr(kja);
  //AB_GredaStub *gs=NULL;
  DijagramInterakcije di={kja->arm, 0.15, 1};
  short dpl,bgr,dgr,bs,ds;
        dpl=15;bgr=30;dgr=70;bs=ds=50;
  /*================================================================*/
  //char  info[40];
  short izmenija=0, tipka=0, oket=0;
  //tipka=cekaj_tpk();  izmenija=MenuKey(exec_gmeni,tipka);

  MenuOn(exec_gmeni);
  InfoFill(3,0,0,A_REVERSE,rr);
  Info(3,0,0,(char[]){"Nema unetih podataka."},A_REPLACE,rr);


  while (izmenija!=99)//==================================================================================
  {
    tipka=0;
    AzurirajInfo();
    /*ovde idu azuriranja infoa, crteza...*/
    //tipka=cekaj_tpk();
    tipka=ngetchx();//cekamo sledecu tipku

   // InfoFill(3,0,0,A_REVERSE,&(rr));
    if (tipka==KEY_ESC)break;

    izmenija=MenuKey(exec_gmeni,tipka);

    if (izmenija==M_NOTMENUKEY)
       {//ovde razmatramo tipke koje nisu iz menija
         short smer;
         switch(tipka)
           {case 265://APPS
                    {//WINDOW * wtab=NoviProzor(0,0,LCD_WIDTH,LCD_HEIGHT,TRUE);
                     //Vidljivost();//CrtezOsvezi();  //WinClose(wtab); //ST_obrisi();
                    }break;

            case 337://gore
            case 340://dole
                    { smer=(tipka==337?1:0);
                      if(kja->ags->sused[smer])
                        {kja->ags=kja->ags->sused[smer];
                         kja->abp=kja->ags->pp[0];
                        }
                    }break;

            case 338://levo
            case 344://desno
                    { smer=(tipka==338?1:0);
                      if(kja->abp->sused[smer])kja->abp=kja->abp->sused[smer];

                    }break;
            case 43://plus
            case 45://minus
                    {
                    }break;
            default:break;
           }//switch

       }//if (izmenija)
   else
       {
         //a sad, akcija na osnovu unosa iz menija, tj. =izbor=
         MenuOff(exec_gmeni);

         switch(izmenija)
         {
           case _NOVI:    Novi();                  break;
           case _OTVORI:  Novi(); Otvori(kja);     break;
           case _SACUVAJ: Sacuvaj_kao(kja);        break;

           case _MATERIJAL:     Unos_mbiarm_dlg(&(kja->mb),&(kja->arm),&oket);  break;
           case _OPTERECENJE:   Unos_opterecenja_dlg(kja);                      break;
           case _SEIZMIKA_UNOS: Unos_seizmika_dlg(kja);                         break;
           case _ZGRADA_UNOS:   Unos_zgrada(kja);                               break;
           case _SG_IZMENA:     Unos_GS_dlg(kja);                               break;
           case _SG_PP:         IzmenaABPP(kja,kja->ags);                       break;
           case _SG_PRESSILE:   Unos_pressila(kja->abp);                        break;
           case _PROBIJANJE:                                                    break;
           case _SEIZMIKA_ANLZ: Seizmika_Analiza(kja);                          break;
           case _DIJ_INT:       DI_Alat(&di);                                   break;
           default:break;
         }//kraj switch(izmenija)===============================================================================

         MenuOn(exec_gmeni);  //MeniAzuriraj(exec_gmeni,izbor);
         InfoFill(3,0,0,A_REVERSE,rr);
       }//else (if (izmenija)

  }//while izmenija

  MenuEnd (exec_gmeni);
  MenuUpdate ();

  WinClose (w1);  HeapFreePtr(w1);
  NapustiProgram(&home_ekran);
}
//-------------------------------------------------------------------------------------------
void DI_Alat(DijagramInterakcije *di)
{
  //DijagramInterakcije di=*dij;
  //Izbor_alfa(&di); Izbor_k_odnosa(&(di.k));
  HANDLE DImeni = MenuNew (MF_NO_NUMS, rr->menuL, rr->menuH); Iniciraj_DI_meni(&DImeni);
  HANDLE exec_DImeni = MenuBegin (NULL, 0, 0, 0x20, DImeni);
  WINDOW *w=NoviProzor(0,rr->menuH,LCD_WIDTH-rr->lista_desno,LCD_HEIGHT-rr->komand-rr->info3H,TRUE);

  DI_Crtaj(*di, w);

  BOOL  //pritisnutaA1=FALSE,
        //povoljnoG=FALSE,
        izabrano=FALSE;

  short izmenija=0, tipka=0;
 while (izmenija!=99)
 {tipka=0;
  while ((izmenija=MenuKey(exec_DImeni,tipka))==M_NOTMENUKEY)
        {//* --*-*- ovde razmatramo tipke koje nisu iz menija --*-*-*-*-*-*-*-
         switch(tipka)
           {case 265://APPS-Dijagram preko celog ekrana
               {WINDOW * wtab=NoviProzor(0,0,LCD_WIDTH,LCD_HEIGHT,TRUE);
                DI_Crtaj(*di, wtab); ngetchx();
                WinClose(wtab);
                ST_obrisi();
               }break;
            default:break;
           }
         if (tipka==KEY_ESC)break;
         tipka=ngetchx();//cekamo sledecu tipku
        }
   if (tipka==KEY_ESC)break;
   //a sad, akcija na osnovu unosa iz menija, tj. =izbor=
  MenuOff(exec_DImeni);
  switch(izmenija)//___________________________________________________________
   {//F1:   .   .   .   .   .   .   .  FAJL   .   .   .   .   .   .   .   .
    case _ALFA:       izabrano=Izbor_alfa(di);               break;
    case _ODNOS_A1A2: izabrano=Izbor_k_odnosa(&(di->k));       break;
//    case _KARLIN:     Karakteristicne_linije(di);             break;
//    case _NM:         izabrano=NM_dlg(N,M);                   break;
//    case _BDH:        izabrano=bdh_dlg(&b, &d, &hi);          break;
/*    case _KOEFSIG:    izabrano=Izbor_koefsig(&pritisnutaA1);
                      epsA1=(pritisnutaA1?0:-3);
         break;*/
//    case _POVOLJNOG:  izabrano=Izbor_delovanjaG(&povoljnoG);  break;
    default:break;
   }
  MenuOn(exec_DImeni);
  InfoFill(3,0,0,A_REVERSE,rr);
  //----AZURIRANJA------
//  BOOL crtajDI=FALSE, azurirajinfo=FALSE;
  switch (izmenija)
  {case _ALFA:case _ODNOS_A1A2:
      if(izabrano) DI_Crtaj(*di, w);
/*   case _NM:case _KOEFSIG:case _BDH:
      if(izabrano) AzurirajInfo(di, M,N, b,d,hi, mb);
      break;*/
  }

 }//while izmenija

  MenuEnd(exec_DImeni);
  WinClose(w); free(w);
}
//-------------------------------------------------------------------------------------------
/*---- SEIZMICKA ANALIZA --------------------*/
void SeizmickiKoef(ABKonstrukcija *k, float T);
void Seizmika_Analiza(ABKonstrukcija *k)
{
  float T;
  if (((Unos_periodoscilovanja(&T)) && (Unos_zgrada(k))) && (Unos_seizmika_dlg(k)))
     {
                         // prozor zauzima celu sirinu ekrana
       WINDOW * w=NoviProzor(0,rr->menuH,LCD_WIDTH,LCD_HEIGHT-rr->komand-rr->info3H,TRUE);
       WinClr(w);

       float S,W,
            ks=KoefSeizmickeSile( k->seizmika.zona, k->seizmika.kattla, k->seizmika.katobj, k->seizmika.savremena_kja, T, w);

       cekaj_tpk();//da se procita proracun ks

       W=k->zgrada.bretaza*k->zgrada.Wi; //tezina cele zgrade
       S=ks*W;                           //ukupna seizm.sila


 			{/*stampa*///--------------------------------------------------------------------------------
  			char s[50];  WinClr(w); w->CurFont=F_4x6;
		  	short x=2, dx=83, y=2, dy=9;
  				     sprintf(s,"W=n%cWi=%2d%c%5.2f=%5.2f kN",215,k->zgrada.bretaza,215,k->zgrada.Wi,W);   WinStrXY (w,x,   y,s);
   			y+=dy; sprintf(s,"S=K%cW=%5.4f%c%5.2f=%5.2f kN",215,ks,215,W,S);                            WinStrXY (w,x,   y,s);
//   			y+=dy; sprintf(s,"S=K%cW=%5.4f%c%5.2f=%5.2f kN",215,ks,215,W,S);                            WinStrXY (w,x,   y,s);
 			}

       cekaj_tpk(); WinClose(w); free(w);
     }
}
//---------------------
void SeizmickiKoef(ABKonstrukcija *k, float T)
{

}
/*-----kraj seizmicke analize-----------------*/
//-------------------------------------------------------------------------------------------
void Novi(void)
{
  OslobodiMemoriju();

}
//-------------------------------------------------------------------------------------------
void OslobodiMemoriju(void)
{
  AB_GredaStub *pomgs,
               *gs=kja->gs[0];
  while(gs)
  {/*ABPresek *pom,
            *abp=gs->pp[0];

   while(abp)
        {
         pom=abp;
         abp=abp->sused[1];
         ABpp_brisi(pom);
        }
   */
   pomgs=gs;
   gs=gs->sused[1];
   GS_brisi(pomgs);
  }

  PoprecniPresek *pom,
       *pp=kja->pp[0];
  while(pp)
       {
        pom=pp;
        pp=pp->sused[1];
        PP_brisi(pom);
       }


}
//-------------------------------------------------------------------------------------------
static void Iniciraj_glavni_meni(HANDLE *gmeni)
{
  MenuAddText (*gmeni,       0, "BAB",          _FAJL          ,DMF_TOP_SUB);
  MenuAddText (*gmeni,   _FAJL, "Novi",         _NOVI          ,DMF_CHILD);
  MenuAddText (*gmeni,   _FAJL, "Otvori",       _OTVORI        ,DMF_CHILD);
  MenuAddText (*gmeni,   _FAJL, "Sacuvaj",      _SACUVAJ       ,DMF_CHILD);
  //MenuAddText (*gmeni,   _FAJL, "Izlaz",      _IZLAZ         ,DMF_CHILD);

  MenuAddText (*gmeni,        0,"Konstr",       _OSNOVNO       ,DMF_TOP_SUB);
  MenuAddText (*gmeni, _OSNOVNO,"Materijal",    _MATERIJAL     ,DMF_CHILD);
//  MenuAddText (*gmeni, _OSNOVNO,"Opterecenje",  _OPTERECENJE   ,DMF_CHILD);
  MenuAddText (*gmeni, _OSNOVNO,"Seizm.podaci", _SEIZMIKA_UNOS ,DMF_CHILD);
  MenuAddText (*gmeni, _OSNOVNO,"Zgrada",       _ZGRADA_UNOS   ,DMF_CHILD);

  MenuAddText (*gmeni,       0, "POS",             _SG         ,DMF_TOP_SUB);
  MenuAddText (*gmeni,     _SG, "Nova/Izmena",     _SG_IZMENA  ,DMF_CHILD);
  MenuAddText (*gmeni,     _SG, "Popr.presek",     _SG_PP      ,DMF_CHILD);
  MenuAddText (*gmeni,     _SG, "Pres.sile",       _SG_PRESSILE,DMF_CHILD);
  MenuAddText (*gmeni,     _SG, "Dimenzionisanje", _SG_DIMENZ  ,DMF_CHILD);

  MenuAddText (*gmeni,       0, "Analiza",      _ANALIZA       ,DMF_TOP_SUB);
  MenuAddText (*gmeni,_ANALIZA, "Pomeranje",    _POMERANJE     ,DMF_CHILD);
  MenuAddText (*gmeni,_ANALIZA, "Izvijanje",    _IZVIJANJE     ,DMF_CHILD);
  MenuAddText (*gmeni,_ANALIZA, "Probijanje",   _PROBIJANJE    ,DMF_CHILD);
  MenuAddText (*gmeni,_ANALIZA, "Seizmika",     _SEIZMIKA_ANLZ ,DMF_CHILD);

  MenuAddText (*gmeni,       0, "Alati"          ,  _ALATI     ,DMF_TOP_SUB);
  MenuAddText (*gmeni,  _ALATI, "Dij.Interakcije", _DIJ_INT    ,DMF_CHILD);

}