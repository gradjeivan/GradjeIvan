// C Source File
// Created 15.02.2015; 19:15:37

#include <tigcclib.h>
#include "beton-tipovi.h"

/* GENERALNO
========================================================================*/

//======================================================================
/* PRESECNE SILE
========================================================================*/
PresSile * PS_Prazno(void)
{
  PresSile *ps=(PresSile *)calloc(1, sizeof(PresSile));
  ps->brs=0;
  ps->so=NULL;
  return ps;
}
//------------------------------------------------------------------------
PresSile * PS_NoviSlucaj(PresSile*  ps, short id, float N, float M, float T)
{//dodaje novi slucaj opterecenja u PresSile
  ps->brs++;
  ps->so=(slucajopterecenja *)calloc(ps->brs, sizeof(slucajopterecenja));
  ps->so[ps->brs-1].S[0]=N;
  ps->so[ps->brs-1].S[1]=M;
  ps->so[ps->brs-1].S[2]=T;
  ps->so[ps->brs-1].id  =id;
  return ps;
}
//------------------------------------------------------------------------
void PS_DodajSO (ABPresek *abp, short kom)
{
	if ( abp->ps==NULL ) abp->ps=PS_Prazno();

  short i=0;
  //const char * naslov[3]={"stalno","povremeno","seizmika"};
  float S[3]={0,0,0};
  while ( i<kom )//&& NMT_dlg(S,naslov[i]) )
        { PS_NoviSlucaj(abp->ps, i, S[0], S[1], S[2]);
          i++;
        }
}
//------------------------------------------------------------------------
void PS_Brisi(PresSile **ps)
{
  if (*ps)
     {if ((*ps)->so)free((*ps)->so);
      free(*ps);
      *ps=NULL;
     }
}
//------------------------------------------------------------------------
/* POPRECNI PRESEK
========================================================================*/
PoprecniPresek * PP_Prazan(short tip)
{
  PoprecniPresek *pp=(PoprecniPresek *)calloc(1, sizeof(PoprecniPresek));
  pp->tip=tip;
  switch (tip)
  {
  	case _PROIZVOLJAN: pp->pp.proiz.F= pp->pp.proiz.I= pp->pp.proiz.J= 0; break;
  	case _PRAVOUGAONI: pp->pp.pravoug.b= pp->pp.pravoug.d= 0;             break;
  	case _KRUZNI:                                                         break;
  	case _T_PRESEK:    pp->pp.Tpresek.b= pp->pp.Tpresek.d =
  	                   pp->pp.Tpresek.B= pp->pp.Tpresek.dp= 0;            break;
  	default:           free(pp);                            	            break;
  }
  pp->sused[0]=pp->sused[1]=NULL;
  return pp;
}
//------------------------------------------------------------------------
PoprecniPresek * PP_Novi(short id, short tip)
{
  PoprecniPresek *pp=PP_Prazan(tip);
  if (pp) pp->id=id;
  return pp;
}
//------------------------------------------------------------------------
PoprecniPresek * PP_Dodaj(ABKonstrukcija * k, PoprecniPresek *pp, short tip)
{//staviti pp=NULL ako se zeli da se napravi nov poprecni presek i onda doda u zbirku,
 // a ako se dodaje postojeci popr.pres. onda taj pp
  if (pp==NULL) pp=PP_Novi(PP_ukupno(k)+1,tip);

  pp->id=PP_ukupno(k)+1;//OVU LINIJU IZBRISI AKO id DODELJUJE SPOLJNJA F-JA

  if (k->pp[0]==NULL)//prvi poprecni presek
     {
      k->pp[0]=k->pp[1]=pp;
     }
  else
     {
      //zasad ovako, inace ovde treba da ide sortiranje
      pp->sused[0]=k->pp[1];
      k->pp[1]->sused[1]=pp;
      k->pp[1]=pp;
     }

  return(pp);
}
//------------------------------------------------------------------------
BOOL PP_brisi(PoprecniPresek *pp)
{
	if (pp)
	{
  	if (pp->sused[0]) pp->sused[0]->sused[1]=pp->sused[1];
	  if (pp->sused[1]) pp->sused[1]->sused[0]=pp->sused[0];
    free(pp);
	}
	return TRUE;
}
//------------------------------------------------------------------------
short PP_ukupno(ABKonstrukcija *k)
{
	PoprecniPresek *pp=k->pp[0];
	short bp=0;
	while (pp)
	      {bp++;
         pp=pp->sused[1];
	      }
	return bp;
}
//------------------------------------------------------------------------
PoprecniPresek * PopPresek(ABKonstrukcija *k,short id)
{
  PoprecniPresek *pp=k->pp[0];
	while (pp->id != id) pp=pp->sused[1];
	return pp;
}
/*
========================================================================*/

/* AB Poprecni Presek
========================================================================*/
ABPresek * ABpp_Novi(void)
{
  ABPresek *abp=(ABPresek*)calloc(1,sizeof(ABPresek));
  abp->id='?';
  abp->ksi=0;
  abp->pp=NULL;
  abp->ps=NULL;
  abp->sipke[0].fi=abp->sipke[1].fi=0;
  abp->a[0]= abp->a[1]= 0;
  abp->sused[0]= abp->sused[1]= NULL;
  return abp;
}
//------------------------------------------------------------------------
ABPresek * ABpp_Dodaj(AB_GredaStub *gs, ABPresek *abp)
{
  if (abp==NULL) abp=ABpp_Novi();

  if (gs->pp[0]==NULL)//prvi poprecni presek
     {
      gs->pp[0]=gs->pp[1]=abp;
     }
  else
     {
      //zasad ovako, inace ovde treba da ide sortiranje prema ksi
      abp->sused[0]=gs->pp[1];
      gs->pp[1]->sused[1]=abp;
      gs->pp[1]=abp;
     }

  return(abp);
}
//------------------------------------------------------------------------
BOOL ABpp_brisi(ABPresek *pp)
{
	if (pp)
	{
  	if (pp->sused[0]) pp->sused[0]->sused[1]=pp->sused[1];
	  if (pp->sused[1]) pp->sused[1]->sused[0]=pp->sused[0];

    //short i; for (i=0; i< pp->ps->brs; i++)
    //free(pp->ps->so);
    PS_Brisi(&(pp->ps));
    free(pp);
	}
	return TRUE;
}
//------------------------------------------------------------------------
ABPresek * ABpp_poredu(AB_GredaStub *gs, short rbr)
{//rbr krece od 0
  ABPresek *abp=gs->pp[0];
  short i=0;
  while ( abp && i<=rbr)
  {
    i++;
   	abp=abp->sused[1];
  }
  return abp;
}
//------------------------------------------------------------------------
short ABpp_ukupno(AB_GredaStub *gs)
{
  short ukupno=0;
  ABPresek *abp=gs->pp[0];
  while(abp)
  {
    ukupno++;
  	abp=abp->sused[1];
  }
  return ukupno;
}
//------------------------------------------------------------------------
/* AB Greda/Stub
========================================================================*/
AB_GredaStub * GS_Novi  (void)
{
  AB_GredaStub *gs=(AB_GredaStub*)calloc(1,sizeof(AB_GredaStub));
  gs->id=0;  gs->L=0; gs->statsist=0;
  gs->pp[0]=gs->pp[1]=NULL;
  return gs;
}
//------------------------------------------------------------------------
AB_GredaStub * GS_Dodaj(ABKonstrukcija *k, AB_GredaStub *gs)
{
  if (gs==NULL) gs=GS_Novi();

  if (k->gs[0]==NULL)//prva greda/stub
     {
      gs->id=1;
      k->gs[0]=k->gs[1]=gs;
     }
  else
     {//zasad ovako, inace ovde treba da ide sortiranje prema ksi
      gs->id= k->gs[1]->id + 1;
      gs->sused[0]=k->gs[1];
      k->gs[1]->sused[1]=gs;
      k->gs[1]=gs;
     }
  return(gs);
}
//------------------------------------------------------------------------
BOOL GS_brisi(AB_GredaStub *gs)
{
	if (gs)
	{
  	if (gs->sused[0]) gs->sused[0]->sused[1]=gs->sused[1];
	  if (gs->sused[1]) gs->sused[1]->sused[0]=gs->sused[0];

	  //Izbrisati ovde i sve ABPreseke za ovu gredu/stub
    ABPresek *pom, *abp=gs->pp[0];
	  while(abp)
	  {
      pom=abp;
      abp=pom->sused[1];
      ABpp_brisi(pom);
	  }
    free(gs);
	}
	return TRUE;
}
//------------------------------------------------------------------------
short GS_ukupno(ABKonstrukcija *k)
{
  short ukupno=0;
  AB_GredaStub *gs=k->gs[0];
  while(gs)
  {
    ukupno++;
  	gs=gs->sused[1];
  }
  return ukupno;
}
//------------------------------------------------------------------------
/* KONSTRUKCIJA
========================================================================*/
void InicABKonstr( ABKonstrukcija *k)
{
  k->mb=_30; k->arm=_RA;//pocetne vrednosti: MB30, RA400/500

  //SEIZMIKA
  k->seizmika.zona  =-1;  k->seizmika.katobj=-1;  k->seizmika.kattla=-1;
  k->seizmika.savremena_kja=TRUE;
  //OPTERECENJA
  k->opt.dg= k->opt.p= k->opt.pregzid= k->opt.fasada= k->opt.w =0;
  //Poprecni preseci
  k->pp[0]=k->pp[1]=NULL;
  //AB Poprecni preseci

  //Grede/Stubovi
  k->gs[0]=k->gs[1]=NULL;
  //aktuelna pozicija:
  k->ags=NULL; k->abp=NULL;
}


//------------------------------------------------------------------------
