// C Source File
// Created 22.10.2014; 22:30:24

#include <tigcclib.h>
#include "statika-dijalozi.h"

/*-----------------------      FUNKCIJE      -------------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/*                       stabilnost                                               */
/*--------------------------------------------------------------------------------*/
BOOL Unos_P_stab_dlg(float *P)
{char  ff[]="%-^5.4f";
 PodZaDlg pd[1]={{10,P,"P=",ff}};
 return Dlg_(1,1,pd,"Aksijalna sila");
}
//------------------------------------
short IzborNacinaUnosaIlpc(void)
{short i=0;
 { HANDLE popmeni = PopupNew ("Uporedne vrednosti", 0);
   PopupAddText (popmeni, -1, "Izaberi sa stapa", i);
   PopupAddText (popmeni, -1, "Rucni unos",   i);//indeksi liste su od 1,
	 HANDLE exec_handle = PopupBegin (popmeni, 0);
   i = PopupBeginDo (exec_handle, CENTER, CENTER, 0);
   MenuEnd (exec_handle);
 }
return i;
}
//---------------------------------------------------
BOOL Unos_IlPc_dlg(float *Ic,float *lc,float *Pc)
{/*Ovde je redni broj pravi, tj. pocev od 1*/
char  ff[]="%-^5.4f";
 /*Unos kroz dijaloge*/
 PodZaDlg pd[3]={{10,Ic,  "Ic="   ,ff},{10,lc,  "lc="  ,ff},{10,Pc,  "Pc="  ,ff}}; //{short i=0;for (i=0;i<3;i++)printf("polje=%d, br.slova=%d\nupit='%s'\n",i+1,(pd+i)->brsl,(pd+i)->upit);}
 char zaglavlje[22];sprintf(zaglavlje,"Uporedne velicine");
 return Dlg_(3,1,pd,zaglavlje);
}//-----------------------------------------------------------------------------------

/*--------------------------------------------------------------------------------*/
short IzabNacinZadTackePostStapa(void)
{	char s[15];
  HANDLE popmeni = PopupNew ("Nacin zadavanja tacke:", 0);
  sprintf(s,"x%c[0,L]"     ,31); PopupAddText (popmeni, -1, s, 1);
  sprintf(s,"%c%c[0,1]",138,31); PopupAddText (popmeni, -1, s, 2);//indeksi liste su od 1,
  HANDLE exec_handle = PopupBegin (popmeni, 0);
  short i = PopupBeginDo (exec_handle, CENTER, CENTER, 0);
  MenuEnd (exec_handle);
 return i;
}//--------------------------------------------------------------

BOOL Unos_ksi_stapa(float *ksi,char znak,short rbr)
{char  ff[]="%-^5.4f";
 char s[4];      sprintf(s,"%c="   ,znak);
 char naslov[7]; sprintf(naslov,"DEO %d",rbr);
 PodZaDlg pd[1]={{10,ksi,s,ff}};
 return (Dlg_(1,1,pd,naslov));
}//--------------------------------------------------------------

BOOL  Delta_0t_dlg(float *d0t,short br_dijag)
{BOOL izlaz=FALSE;
 short rbr=0,i,brd;
 char  ff[]="%-^5.4f";

 while (rbr<br_dijag-1)
 {brd=min(br_dijag-1,4);//ne moze vise od 4 unosa po 1 dijalogu
  char  s[brd][6];
  PodZaDlg pd[brd];
  for (i=0;i<brd;i++)
      {sprintf(s[i],"%c%c%d%c=",132,133,i+1,248);
       pd[i]=(PodZaDlg){10,&(d0t[rbr+i]),s[i],ff};
      }
  if ((izlaz=Dlg_(brd,1,pd,"Unos dijagrama")))
 	   {rbr+=brd;
 	   }
 	else return FALSE;
 }
 return izlaz;
}

BOOL Unos_dijag_dlg(float *pi,float *pk,float *f,float *alfa,float *N)
{char  ff[]="%-^5.4f";
 PodZaDlg pd[5]={{10,pi,"pi=",ff},{10,pk  ,"pk=",  ff},
                 {10, f,"f=", ff},{10,alfa,"alfa=",ff},{10,N,"N=",ff}};
 return Dlg_(5,1,pd,"Unos dijagrama");
}

/*
BOOL  Unos_koncopt_dlg    (float *F, float *alfa,BOOL sila)
{

}*/

/*--------------------------------------------------------------------------------*/
/*                          NOVI MATERIJAL                                        */
/*--------------------------------------------------------------------------------*/
BOOL Unos_materijala_dlg(float *me,float *ni,float *alfa,float *G,short rbr)
{/*Ovde je redni broj pravi, tj. pocev od 1*/
 char  ff[]="%-^5.4f"; char salfa[3];sprintf(salfa,"%ct=",128);
 /*Unos kroz dijaloge*/
 PodZaDlg pd[4]={{10,me,  "E="   ,ff},{10,ni,  "ni="  ,ff},
                 {10,alfa,salfa,  ff},{10, G,  "G="   ,ff}}; //{short i=0;for (i=0;i<3;i++)printf("polje=%d, br.slova=%d\nupit='%s'\n",i+1,(pd+i)->brsl,(pd+i)->upit);}
 char zaglavlje[15];sprintf(zaglavlje,"Materijal br.%d",rbr);//ST_helpMsg("Unos_materijala_dlg");ngetchx();
 return Dlg_(4,1,pd,zaglavlje);
}
/*--------------------------------------------------------------------------------*/
/*                       NOVI POPRECNI PRESEK                                     */
/*--------------------------------------------------------------------------------*/
BOOL Unos_preseka_dlg(float *mi,float *mf,float *jt,short rbr)
{/*Ovde je redni broj pravi, tj. pocev od 1*/
char  ff[]="%-^5.4f";
 /*Unos kroz dijaloge*/
 PodZaDlg pd[3]={{10,mi,  "I="   ,ff},{10,mf,  "F="  ,ff},{10,jt,  "J="  ,ff}}; //{short i=0;for (i=0;i<3;i++)printf("polje=%d, br.slova=%d\nupit='%s'\n",i+1,(pd+i)->brsl,(pd+i)->upit);}
 char zaglavlje[22];sprintf(zaglavlje,"Tip popr.preseka br.%d",rbr);//ST_helpMsg("Unos_materijala_dlg");ngetchx();
 return Dlg_(3,1,pd,zaglavlje);
}//-----------------------------------------------------------------------------------
/* . . . . . . . . . .  . Izbor RAVAN/ROSTILJ . . . . . . . . . . . . . . . . . . . . .*/
void IzborRavanRostilj(BOOL *rostilj)
{short i=0;
 { HANDLE popmeni = PopupNew ("Tip nosaca:", 0);
   PopupAddText (popmeni, -1, "RAVANSKI", i);
   PopupAddText (popmeni, -1, "ROSTILJ",  i);//indeksi liste su od 1,
	 HANDLE exec_handle = PopupBegin (popmeni, 0);
   i = PopupBeginDo (exec_handle, CENTER, CENTER, 0);
   MenuEnd (exec_handle);
 }
 if (i==1) {*rostilj=FALSE;/*char info[30]="Tip nosaca:RAVANSKI NOSAC";Info(3,0,0,info,A_NORMAL);*/};
 if (i==2) {*rostilj= TRUE;/*char info[30]="Tip nosaca:ROSTILJ";       Info(3,0,0,info,A_NORMAL);*/};

}
/*------------------------------------------------------------------*/
void DozvoliPrikazMatrica(HANDLE exec_gmeni, short br_stp)
{ if (br_stp!=0) MenuTopStat(exec_gmeni, 3,TRUE);
  else           MenuTopStat(exec_gmeni, 3,FALSE);
  char s[10];sprintf(s,"%d",br_stp);ST_helpMsg(s);ngetchx();
}
/*---------------------------------------------------------------------*/