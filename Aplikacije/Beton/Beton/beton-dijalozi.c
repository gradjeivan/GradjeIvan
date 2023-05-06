// C Source File
// Created 12.02.2015; 23:58:48
#include <tigcclib.h>
#include "beton-dijalozi.h"

static void padajuci_mb(HANDLE *padmeni,short *tab_mb);

/*
==============================================================================*/
short IzborSlucajaOpterecenja(void)
{
  const char* spisak[]={"stalno","povremeno","seizmika"};
  return IskacuciMeni(3,spisak,"pres.sile");
}
//----------------------------------------------------------------------------
BOOL NMT_dlg(float S[3],const char *naslov)
{char  ff[]="%-^5.4f";
 PodZaDlg pd[3]={{10,S,"N[kN]=", ff},{10,S+1,"M[kNm]=", ff},{10,S+2,"T[kN]=", ff}};
 return Dlg_(3,1,pd,naslov);
}
//----------------------------------------------------------------------------
void Unos_pressila(ABPresek *abp)
{
  short i=0,
        slo=IzborSlucajaOpterecenja() -1;//jer hocemo da krece od 0 zbog ps->so[], kao i zbog enum
  float S[3]={0,0,0};

  if ( slo>=0 )
     { for (i=0; i<3; i++) S[i]= abp->ps->so[slo].S[i] ;
       if ( NMT_dlg(S," ") )
          {
          	abp->ps->so[slo].id=slo;
          	for (i=0; i<3; i++) abp->ps->so[slo].S[i]= S[i];
          }
     }
}

/*
==============================================================================*/
AB_GredaStub   * Unos_GS_dlg    (ABKonstrukcija *k)
{
  //char zaglavlje[17];sprintf(zaglavlje,"Izbor materijala");
 // allocate memory
 short oket,ukbrsl=10,
       id= k->gs[1]? k->gs[1]->id+1 : 1 ;
 short *izbor = NULL; if ((oket=bafer_padmeni(&izbor,1))==0)        return NULL;
 char  s[15],
       *tekst = NULL; if ((oket=bafer_unosteksta(&tekst,ukbrsl))==0){free(izbor);return FALSE;}
 HANDLE dlg=H_NULL;
 if ((oket=novidijalog(&dlg,125,45))==0){free(izbor);free(tekst);return NULL;}

 izbor[0]=1;//pocetne vrednosti i,k, u listama (ne smeju da krecu od 0)
 short tpk = 1; //ovo je uneta/pritisnuta tipka

  sprintf(s,"POS %d",id);
  DialogAddTitle(dlg,s,BT_OK,BT_CANCEL);

  DialogAddRequest(dlg,10,15,"L[cm]",0,9,10);

  HANDLE izbor_ss = PopupNew ("stat.sistem",0);
  short i=0;//char s[25];
  i++; PopupAddText (izbor_ss, -1, "prosta greda", i);
  i++; PopupAddText (izbor_ss, -1, "konzola"     , i);
  DialogAddPulldown (dlg, 25, 25, " ", izbor_ss, 0);

  while ((tpk != KEY_ENTER) && (tpk != KEY_ESC) && tpk > 0)
        { tpk = DialogDo(dlg,CENTER,CENTER,tekst,izbor);
        }
  HeapFree(dlg);

  AB_GredaStub *gs=NULL;
  if (tpk == KEY_ENTER)
     {//*oket=1;
      gs=GS_Novi();//prazna greda/stub
      gs->id= id;//ova dodela id-a je zbog prikaza POS id u Unos_ABPP
      gs->statsist=izbor[0];
      txtuf(tekst+0,&(gs->L));

      ABPresek *abp=Unos_ABPP(k,gs);
      if (abp) {//dodajem u konstrukciju i postavljam za aktuelnu poziciju
                k->ags=gs=GS_Dodaj(k,gs);;
                k->abp=ABpp_Dodaj(gs,abp);
               }
      else     {GS_brisi(gs);
                gs=NULL;
               }
     }
  //else  *oket=0;

 //obavezno oslobadjanje dinamicke memorije
 HeapFree(izbor_ss);free(izbor);free(tekst);

 return gs;
}
/*
----------------------------------------------------------------------------*/
static void PopuniStrInfoomPP(char *s, PoprecniPresek *pp)
{
  switch (pp->tip)
  {
  	case _PROIZVOLJAN:  {sprintf(s,"F=%-8.3f I=%-8.3R ",pp->pp.proiz.F,pp->pp.proiz.I);
                        }break;
  	case _PRAVOUGAONI:  {
                         /*moze se desiti crash ako su uneti podaci nenormalni*/
  	                     sprintf(s,"b/d=%5.3f/%-5.3f",pp->pp.pravoug.b,pp->pp.pravoug.d);
  	                    }break;
  	case _KRUZNI:       {sprintf(s,"r=%-5.3f",pp->pp.pravoug.b);
  	                    }break;
  	case _T_PRESEK:     {sprintf(s,"b/d/dp=%4.2f/%4.2f",pp->pp.Tpresek.b,pp->pp.Tpresek.d);
  	                    }break;
    default:             break;
  }
}
//-----------------------------------------------------------------------------
short IzborABPP(AB_GredaStub *gs, BOOL stavka_novi)
{
  short izbor=0,i=0,j;
  char **stavka=NULL;
  ABPresek *abp=gs->pp[0];

  while ( abp || stavka_novi )
  {
   stavka=   (char**)realloc(stavka,(i+1)*sizeof(char*));
   stavka[i]=(char* ) calloc(    16,      sizeof(char ));

   if ( abp )sprintf(stavka[i],"%c-%c: x=%5.2f", abp->id, abp->id, abp->ksi);
   else {    sprintf(stavka[i],"novi");
             stavka_novi = FALSE;
        }
   i++;//ovo treba na kraju, jer i krece od 0
   abp=abp->sused[1];
  }

  izbor=IskacuciMeni(i,(const char**)stavka,"Izbor pop.pres.");

  if (stavka)
  {   for ( j=0; j<i; j++ )	if (stavka[j]) free(stavka[j]);
      free(stavka);
  }

  return izbor;
}
//------------------------------------------------------
ABPresek * IzmenaABPP(ABKonstrukcija *k, AB_GredaStub *gs)
{
  ABPresek *abp=NULL;
  short rbr=IzborABPP(gs, TRUE);

  abp=Unos_ABPP(k,gs);
  if (rbr > ABpp_ukupno(gs))
     {
       if (abp) ABpp_Dodaj(k->ags,abp);
     }

  return abp;
}
//----------------------------------------------------

ABPresek * Unos_ABPP(ABKonstrukcija *k, AB_GredaStub *gs)
{
/*
 if ( k->pp[0]==NULL )
    { if (!(Unos_PP(k))) return NULL;
    }
*/
 //char zaglavlje[17];sprintf(zaglavlje,"Izbor materijala");
 // allocate memory
 short oket,ukbrsl=12;
 short *izbor = NULL; if ((oket=bafer_padmeni(&izbor,1))==0)        return NULL;
 char  *tekst = NULL; if ((oket=bafer_unosteksta(&tekst,ukbrsl))==0){free(izbor);return FALSE;}
 HANDLE dlg=H_NULL;
 if ((oket=novidijalog(&dlg,125,55))==0){free(izbor);free(tekst);return NULL;}

 char s[25];
 izbor[0]=1;//pocetne vrednosti i,k, u listama (ne smeju da krecu od 0)
 short tpk = 1; //ovo je uneta/pritisnuta tipka

  sprintf(s,"POS %d: novi presek",gs->id);
  DialogAddTitle(dlg,s,BT_OK,BT_CANCEL);

  DialogAddRequest(dlg,10,15,"x[cm]", 0,9,10);
  DialogAddRequest(dlg,24,25,"id"   ,11,1, 2);

  HANDLE izbor_pp = PopupNew ("pp",0);
  short i=0;
  PoprecniPresek *pp=k->pp[0];
  while (pp)
    {i++; PopuniStrInfoomPP(s,pp); PopupAddText (izbor_pp, -1, s, i);
     pp=pp->sused[1];
    }
     i++; PopupAddText (izbor_pp, -1, "novi pp", i);
  DialogAddPulldown (dlg, 20, 35, " ", izbor_pp, 0);

  while ((tpk != KEY_ENTER) && (tpk != KEY_ESC) && tpk > 0)
        { tpk = DialogDo(dlg,CENTER,CENTER,tekst,izbor);
        }
  HeapFree(dlg);

  ABPresek *abp=NULL;
  if (tpk == KEY_ENTER)
     {
      if ( izbor[0]==i )
         { if (!(Unos_PP(k))) return NULL;
         }
      //*oket=1;
      abp=ABpp_Novi();
       txtuf(tekst+0 ,&(abp->ksi));
       abp->id= *(tekst+11);
       abp->pp= PopPresek(k,izbor[0]);
       PS_DodajSO(abp,3);
      /* abp->ps= PS_Prazno();
        short i=0;
        //const char * naslov[3]={"stalno","povremeno","seizmika"};
        float S[3]={0,0,0};
        while ( i<3 )//&& NMT_dlg(S,naslov[i]) )
              { PS_NoviSlucaj(abp->ps, i, S[0], S[1], S[2]);
                i++;
              }*/
     }
  //else  *oket=0;
 //obavezno oslobadjanje dinamicke memorije
 HeapFree(izbor_pp);free(izbor);free(tekst);

 return abp;
}
/*
----------------------------------------------------------------------------*/
PoprecniPresek * Unos_PP(ABKonstrukcija *k)
{
  short tip;
  HANDLE popmeni = PopupNew ("Poprecni presek", 0);
  PopupAddText (popmeni, -1, "proizvoljan", _PROIZVOLJAN);
  PopupAddText (popmeni, -1, "pravougaoni", _PRAVOUGAONI);
  PopupAddText (popmeni, -1, "kruzni",      _KRUZNI);
  PopupAddText (popmeni, -1, "T presek",    _T_PRESEK);
  HANDLE exec_handle = PopupBegin (popmeni, 0);
  tip = PopupBeginDo (exec_handle, CENTER, CENTER, 0);
  MenuEnd (exec_handle);

  PoprecniPresek *pp=PP_Novi(PP_ukupno(k)+1,tip);
  BOOL losipodaci=TRUE, oket=TRUE;
  char  ff[]="%-^5.4f";
  switch (tip)
  {
    case _PROIZVOLJAN:  {PodZaDlg pd[3]={{10,&(pp->pp.proiz.F), "F [cm2]" ,ff},
                                         {10,&(pp->pp.proiz.I), "I [cm4]" ,ff},
                                         {10,&(pp->pp.proiz.J), "J [cm4]" ,ff}};
                         oket= Dlg_(3,1,pd,(const char *){"Proizvoljan"});

                        }break;
  	case _PRAVOUGAONI:  {PodZaDlg pd[2]={{10,&(pp->pp.pravoug.b), "b [cm]" ,ff},
                                         {10,&(pp->pp.pravoug.d), "d [cm]" ,ff}};
                         while (losipodaci && oket)
                               {oket= Dlg_(2,1,pd,(const char *){"Pravougaoni"});
                              	/*ovde treba provera unetih podataka*/
                              	 losipodaci=FALSE;//da se izadje iz petlje
                               }
  	                    }break;
  	case _KRUZNI:       {//PodZaDlg pd[1]={{10,&(pp->pp.pravoug.r), "r [cm]" ,ff}};
                         //oket= Dlg_(1,1,pd,(const char *){"Kruzni"});
  	                    }break;
  	case _T_PRESEK:     {PodZaDlg pd[4]={{10,&(pp->pp.Tpresek.b), "b [cm]" ,ff},
                                         {10,&(pp->pp.Tpresek.d), "d [cm]" ,ff},
                                         {10,&(pp->pp.Tpresek.B), "B [cm]" ,ff},
                                         {10,&(pp->pp.Tpresek.dp),"dp[cm]" ,ff}};
                         while (losipodaci && oket)
                               {oket= Dlg_(4,1,pd,(const char *){"T presek"});
                              	/*ovde treba provera unetih podataka*/
                              	 losipodaci=FALSE;//da se izadje iz petlje
                               }
  	                    }break;
  	default:  oket=FALSE;break;
  }

  if (oket) PP_Dodaj(k,pp,tip);
  else      PP_brisi(pp);

  return pp;

}
//===============================================================================

//-------------------------------------------------------------------------------
/*SACUVAJ FAJL
===============================================================================*/
void Upisi(FILE *fajl, PodZaUpis p[], short kom)
{
  short i;
  for ( i=0; i<kom; i++ )
      { fwrite( p[i].prom, p[i].bajtova, p[i].kom, fajl);
      }
}
//-----------------------------------------------------------------------------
void UpisiABK (ABKonstrukcija *k, FILE * fajl)
{
 short flt=sizeof(float), shr=sizeof(short), chr=sizeof(char),
       tacno=1,netacno=0,
       ukupno;

 PodZaUpis upis[5]={
/*materijal, opterecenja*/{&(k->mb) ,shr,1}, {&(k->arm),shr,1}, {&(k->opt.dg),flt,5},
/*seizmika podaci*/       {&(k->seizmika.zona),shr,3}, {k->seizmika.savremena_kja?&tacno:&netacno,shr,1}
                   };
 Upisi(fajl, upis, 5);

/*Poprecni Preseci*/
 ukupno=PP_ukupno(k); fwrite( &(ukupno), shr, 1, fajl);
 short i=0;
 PoprecniPresek * pp=k->pp[0];
 while(pp)
 {
   fwrite( &(pp->id) , shr, 1, fajl);
   fwrite( &(pp->tip), shr, 1, fajl);

   switch(pp->tip)
   {
   	case _KRUZNI:      i=1; break;
    case _PRAVOUGAONI: i=2; break;
    case _PROIZVOLJAN: i=3; break;
    case _T_PRESEK:    i=4; break;
   }
   fwrite( &(pp->pp.proiz.F), flt, i, fajl);
   pp=pp->sused[1];
 }

/*AB_GredaStub-ovi*/
 ukupno=GS_ukupno(k); fwrite( &(ukupno), shr, 1, fajl);
 AB_GredaStub * gs=k->gs[0];
 while(gs)
 {
   fwrite( &(gs->L)        , flt, 1, fajl);
   fwrite( &(gs->statsist) , shr, 1, fajl);

   ukupno=ABpp_ukupno(gs); fwrite( &(ukupno), shr, 1, fajl);
   ABPresek *abp=gs->pp[0];
   while(abp)
   {
     fwrite( &(abp->ksi)        ,flt, 1, fajl);
     fprintf( fajl,"%c", abp->id );
     //fwrite( &(abp->id)         ,chr, 1, fajl);
     fwrite( &(abp->pp->id)     ,shr, 1, fajl); //jedini nacin da pratimo pp je preko id

     fwrite( &(abp->ps->brs)     ,shr, 1, fajl);
     for (i=0; i<abp->ps->brs; i++)
     {
      fwrite( &(abp->ps->so[i].S[0]) ,flt, 3, fajl);
      fwrite( &(abp->ps->so[i].id)   ,shr, 1, fajl);
     }


     fwrite( &(abp->sipke[0].fi),shr, 5, fajl);
     fwrite( &(abp->sipke[1].fi),shr, 5, fajl);
     fwrite( &(abp->a)          ,flt, 2, fajl);

   	 abp=abp->sused[1];
   }
  gs=gs->sused[1];
 }

/*ZGRADA*/                    //broj etaza,             tezina Wi
 PodZaUpis upiszgr[2]={{&(k->zgrada.bretaza) ,shr,1}, {&(k->zgrada.Wi),flt,1}};
 Upisi(fajl, upiszgr, 2);


 char ekst[]={EKSTENZIJA_FAJLA};
 fputc(0,fajl); fputs(ekst,  fajl);
 fputc(0,fajl); fputc(OTH_TAG,fajl);

}
//=============================================================================
void Sacuvaj(ABKonstrukcija *k, char *ime, char *info)
{
 FILE *fajl=fopen(ime,"wb");

 if (fajl)//ako je uspesno otvoren
    {UpisiABK(k,fajl);fclose(fajl);
     //strcpy(imefajla,ime);
     if (info)sprintf(info,"Fajl je sacuvan.");
    }
 else if (info)sprintf(info,"Neuspesno.Arhiviran fajl?");
}
//=============================================================================
void Sacuvaj_kao(ABKonstrukcija *k)
{
 char info[35];//privremeno: info treba da bude van funkcije, tj. kao ulayna promenljiva
 char ime[18],ext[]={EKSTENZIJA_FAJLA};
 BOOL sacuvaj=FALSE;

      //if ((izbor==13)&&(imefajla[0]!='\0'))//Sacuvaj vec otvoren fajl
        // {strcpy(ime,imefajla); sacuvaj=TRUE;}
      //else//Ili je izabrano Sacuvaj kao.. ili je u pitanju Novi nesacuvan fajl
        {sacuvaj=SacuvajFajl(ime,ext);}

      if (sacuvaj)
         {Sacuvaj(k,ime,info);
         }
      else if (info)sprintf(info,"Cuvanje fajla otkazano.");
      //Info(3,0,0,info,A_NORMAL,rr);
}
//=============================================================================
/*OTVORI
===============================================================================*/
void Procitaj(FILE *fajl, PodZaUpis p[], short kom)
{
  short i;
  for ( i=0; i<kom; i++ )
      { fread( p[i].prom, p[i].bajtova, p[i].kom, fajl);
      }
}
//-----------------------------------------------------------------------------
void ProcitajABK(ABKonstrukcija *k, FILE * fajl)
{
 short flt=sizeof(float),shr=sizeof(short),chr=sizeof(chr),
       tacno=0;

 PodZaUpis upis[5]={
/*materijal, opterecenja*/{&(k->mb) ,shr,1}, {&(k->arm),shr,1}, {&(k->opt.dg),flt,5},
/*seizmika podaci*/       {&(k->seizmika.zona),shr,3},{&tacno,shr,1}
                   };
 Procitaj(fajl, upis, 5); k->seizmika.savremena_kja=(tacno==1);

/*Poprecni Preseci*/
 short i,brpod=0,brp;
 fread(&(brp),shr,1,fajl); //broj popr.preseka

 for ( i=0; i<brp; i++ )
 {
  PoprecniPresek * pp=PP_Prazan(_PROIZVOLJAN);//za pocetak
   fread( &(pp->id) , shr, 1, fajl);
   fread( &(pp->tip), shr, 1, fajl);

   switch(pp->tip)
   {
   	case _KRUZNI:      brpod=1; break;
    case _PRAVOUGAONI: brpod=2; break;
    case _PROIZVOLJAN: brpod=3; break;
    case _T_PRESEK:    brpod=4; break;
   }
   fread( &(pp->pp.proiz.F), flt, brpod, fajl);
  PP_Dodaj( k, pp, pp->tip);
 }

/*AB_GredaStub-ovi*/
 fread(&(brp),shr,1,fajl); //broj popr.preseka
 for ( i=0; i<brp; i++ )
 {
  AB_GredaStub * gs=GS_Novi();
   fread( &(gs->L)        , flt, 1, fajl);
   fread( &(gs->statsist) , shr, 1, fajl);

   fread(&(brpod),shr,1,fajl); //broj popr.preseka
   short j;
   for ( j=0; j<brpod; j++ )
   {
    ABPresek *abp=ABpp_Novi();
     fread( &(abp->ksi)        ,flt, 1, fajl);
     //fread( &(abp->id)         ,chr, 1, fajl);
     short id;
     id=fgetc(fajl);
     abp->id=(char)id;
     fread( &(id), shr, 1, fajl); //jedini nacin da pratimo pp je preko id
     abp->pp= PopPresek(k,id);
     /**/
     abp->ps=PS_Prazno();
     short brs;  //fread( &(abp->ps->brs)     ,shr, 1, fajl);
     fread( &brs     ,shr, 1, fajl);
     PS_DodajSO(abp,brs);
     short ii;
     for (ii=0; ii<brs; ii++)
     {
      fread( &(abp->ps->so[ii].S[0]) ,flt, 3, fajl);
      fread( &(abp->ps->so[ii].id)   ,shr, 1, fajl);
     }

     fread( &(abp->sipke[0].fi),shr, 5, fajl);
     fread( &(abp->sipke[1].fi),shr, 5, fajl);
     fread( &(abp->a)          ,flt, 2, fajl);
    ABpp_Dodaj(gs,abp);
   }
  GS_Dodaj(k,gs);
 }
 
 /*ZGRADA*/                    //broj etaza,             tezina Wi
 PodZaUpis upiszgr[2]={{&(k->zgrada.bretaza) ,shr,1}, {&(k->zgrada.Wi),flt,1}};
 Procitaj(fajl, upiszgr, 2);

 k->ags=k->gs[0];
 if(k->ags)k->abp=k->ags[0].pp[0];
 //fread(&(brp),shr,1,fajl); //broj profila
}
//-----------------------------------------------------------------------------
BOOL Otvori(ABKonstrukcija *k)
{
  BOOL uspesno;
  FILE *fajl;
  char ime[18],ext[]={EKSTENZIJA_FAJLA}; // ST_helpMsg(ime);ngetchx();
  if((uspesno=Otvori_fajl_dlg(ime,ext)))
    {fajl=fopen(ime,"rb");//printf("fajl=%lu",fajl);ngetchx();
     if (fajl!=NULL)
        {//pc.rezim=A_XOR;CrtajNosac(&pc,&kt,&kd);//Izbrisi prethodan crtez
//         strcpy(imefajla,ime);  //ST_helpMsg(imefajla);ngetchx();
	       ProcitajABK(k,fajl);
	       fclose(fajl); //ucitaj podatke i zatvori fajl
    	  }
     else {char s[30];sprintf(s,"Neuspesno citanje fajla.");
      	   //Info(3,0,0,s,A_NORMAL,rr);
      	  }
    }
  return uspesno;
}
//-----------------------------------------------------------------------------
/*
=-=--=-=-=-=-=--=-=-=-= UNOS MB i armature -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void Unos_mbiarm_dlg(short *mb,short *arm,short *oket)
{//char zaglavlje[17];sprintf(zaglavlje,"Izbor materijala");
// allocate memory
short *tab_mb=(short []){MARKA};
short *izbor = NULL;if ((*oket=bafer_padmeni(&izbor,2))==0) return;
HANDLE dlg=H_NULL;
if ((*oket=novidijalog(&dlg,125,50))==0){free(izbor);return;}

*izbor=(*mb)+1;*(izbor+1)=(*arm)+1;//pocetne vrednosti i,k, u listama (ne smeju da krecu od 0)
short tpk = 1; //ovo je uneta/pritisnuta tipka

  DialogAddTitle(dlg,"Izbor materijala",BT_OK,BT_CANCEL);
  HANDLE izbor_mb = PopupNew ("marka betona",0);   // padajuci meniji
  padajuci_mb(&izbor_mb,tab_mb);
  DialogAddPulldown (dlg, 10, 15, "marka betona", izbor_mb, 0);

  HANDLE izbor_arm = PopupNew ("armatura",0);
  PopupAddText (izbor_arm,-1,"RA 400/500" , _RA+1);
  PopupAddText (izbor_arm,-1,"GA 240/360" , _GA+1);
  DialogAddPulldown (dlg, 10, 25, "armatura:", izbor_arm, 1);

  while ((tpk != KEY_ENTER) && (tpk != KEY_ESC) && tpk > 0)
        {
          //char s[25]; sprintf(s,"MB:%d, arm:%d",*izbor,*(izbor+1)); ST_helpMsg(s); ngetchx();
          tpk = DialogDo(dlg,CENTER,CENTER,NULL,izbor);
        }
          //char s[25]; sprintf(s,"MB:%d, arm:%d",*izbor,*(izbor+1)); ST_helpMsg(s); ngetchx();

  if (tpk == KEY_ENTER){ *oket=1; *mb=*izbor-1; *arm=*(izbor+1)-1; }
  else  *oket=0;
//obavezno oslobadjanje dinamicke memorije
HeapFree(dlg);HeapFree(izbor_mb);HeapFree(izbor_arm);free(izbor);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//POPUNA PADAJUCEG MENIJA MB
static void padajuci_mb(HANDLE *padmeni,short *tab_mb)
{short i=0;char mbstr[5];
 for(i=1;i<11;i++)
    {sprintf(mbstr, "MB %d", tab_mb[i-1]);
     PopupAddText ((*padmeni),-1,mbstr , i);
    }
}
