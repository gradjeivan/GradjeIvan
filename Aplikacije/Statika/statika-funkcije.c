#include <tigcclib.h>
#include "statika-funkcije.h"
/*--------------------------------------------------------------------------------*/
/*-----------------------      FUNKCIJE      -------------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*
MatricaGT * ZameniVrKolMatGT(MatricaGT *M,short v1,short v2,short dimenz)
//v1,v2 su indeksi vrsta/kolona za zamenu, dimenz: 0=vrste, 1=kolone, 2=oba
{

}
*/

/*======================================================*/

//static void IzbrisiSvePc      (void);


/*======================================================*/

BOOL Unos_P_stabilnost(void)
{
 Duz *d=(*kd)[2][1];
 float P,*Pi=NULL;
 BOOL uneto;
 while (d)
 {
  if ((d=Izaberi_stap(d,5,0)))
      {
       uneto=(d->tata->jos!=NULL);
       if (uneto)
          {
            Pi=(float *)(d->tata->jos);
          }
       else
          { Pi=(float *)calloc(1,sizeof(float));
            d->tata->jos=(void *)Pi;
            *Pi=0;
          }

       P=*Pi;
       if ((Unos_P_stab_dlg(&P))) {*Pi=P;}
       else if (!(uneto))         {free(Pi);}
      }
 }
return TRUE;
}
//-----------------------------------
/*
void IzbrisiSvePc(void)
{
 float *Pi;
 Duz *d=(*kd)[2][1];
 while (d)
 {if ((d->tata->jos))
  {
   Pi=(float *)(d->tata->jos);
   free(Pi);
   d->tata->jos=NULL;
  }
  d=d->sused[2][0];//predji na sledeci stap
 }

}*/
//----------------------------------------------------
BOOL Unos_IlPc(float *Ic, float *lc, float *Pc)
{short nacin=IzborNacinaUnosaIlpc();
 if (nacin==0) return FALSE;

 if (nacin==1)
    { Duz *d=(*kd)[2][1];
      if ((d=Izaberi_stap(d,5,0)))
         {*Ic=I_(d);
          *lc=l_d(d);
          *Pc=1;
         }
      else return FALSE;
    }
 else return (Unos_IlPc_dlg(Ic,lc,Pc));

 return TRUE;
}
/*=======================================================================================*/

/*=======================================================================================*/
void StampajMatKg_stabilnost(float * uporedni)
{Duz       *d=(*kd)[2][1];
 MatricaGT *K=NULL;
 float lc=uporedni[1],Pc=uporedni[2];

 //if (Unos_IlPc(&Ic,&lc,&Pc))
 do {d=Izaberi_stap(d,1,0);
     if (d)
        {
         if(!(d->tata->ppkonst))
           {//ST_helpMsg("Nisu podrzani stapovi promenljivog popr.pres.");
           }
         else
           {K =MatKg_stabilnost(d,lc,Pc);
            if(K)
            {StampajMatricuGT(K,NULL);
             //char izvoz[3]; sprintf(izvoz,"k%c",d->id -48);
             //IzveziMatricuGT(K,(const char *)izvoz);
             if(K->M)free(K->M); free(K);
             CrtajOkvirCrt();
      	    }
      	    else
      	    {

      	    }
           }
        }
    }while(d);
 CrtezOsvezi();
}//--------------------------------------------------------------------------------------
/*=======================================================================================*/
void StampajMatK_stabilnost(float *uporedni)
{Duz       *d=(*kd)[2][1];
 MatricaGT *K=NULL;
 float Ic=uporedni[0],lc=uporedni[1];

 //if (Unos_IlPc(&Ic,&lc,&Pc))
 do {d=Izaberi_stap(d,1,0);
     if (d)
        {
         if(!(d->tata->ppkonst))
           {//ST_helpMsg("Nisu podrzani stapovi promenljivog popr.pres.");
           }
         else
           {K =MatK_stabilnost(d,Ic,lc,ppres);
            if(K)
            {StampajMatricuGT(K,NULL);
             //char izvoz[3]; sprintf(izvoz,"k%c",d->id -48);
             //IzveziMatricuGT(K,(const char *)izvoz);
             if(K->M)free(K->M); free(K);
             CrtajOkvirCrt();
      	    }
      	    else
      	    {

      	    }
           }
        }
    }while(d);
 CrtezOsvezi();
}//--------------------------------------------------------------------------------------
/*=======================================================================================*/
/*
MatricaGT * MatK_stabilnost(Duz * d,float Ic,float lc,Materijal *materijal,PopPres *ppres)
{Stap      * s=((Stap*)(d->tata));
 MatricaGT * K=(MatricaGT *)malloc(sizeof(MatricaGT));
 float       l=sqrt(pow(d->ik[1]->xy[0] - d->ik[0]->xy[0],2)
                   +pow(d->ik[1]->xy[1] - d->ik[0]->xy[1],2)),//duzina stapa
            I=( ppres    +((Stap*)(d->tata))->pp)->I,
            ilc=(I/Ic)*pow((lc/l),3);

     K->m=K->n=4;
     K->M=(float*)calloc(21,sizeof(float));

       *(eGTM(K,1,1))=*(eGTM(K,3,3))= 12          *ilc;
       *(eGTM(K,2,2))=*(eGTM(K,4,4))=  4*pow(l ,2)*ilc;
       *(eGTM(K,2,3))=*(eGTM(K,3,4))= -6          *ilc;
       *(eGTM(K,1,2))=*(eGTM(K,1,4))=  6        *l*ilc;
                      *(eGTM(K,1,3))=-12          *ilc;
                      *(eGTM(K,2,4))=  2*pow(l, 2)*ilc;

 return K;
}*///------------------------------------------------------------------------

/*----------------------------------------------------------------------*/
void NepoznataPomeranja_unos(BOOL *nep)
/**/
{Tacka *t=(*kt)[2][1];
 short izbor,i,ind;
 char* xyfi[3];

 while((t=IzaberiUnetuTackuNosaca(t)))
 {ind= 3* ( t->id - (*kt)[2][0]->id );
  izbor=0;
  do{//tekst+stikliranje ako je u pitanju nepoznato pomeranje
     if (izbor!=0){nep[ind+izbor-1]=(!(nep[ind+izbor-1]));}//izbor=0 u prvom ulazu, posle ima vrednost 1,2,3(x,y,fi)
     for (i=0;i<3;i++)
         {xyfi[i]=(char*)calloc(4,sizeof(char));
          sprintf(xyfi[i],"%c",(i==0?'X':(i==1?'Y':145)) );
          if (nep[ind+i]) sprintf(xyfi[i],"%s %c",xyfi[i],15);//stikliranje
         }
     izbor=IskacuciMeni(3,(const char **)xyfi,"Nepoznata pomeranja");
    }while(izbor);
  for (i=0;i<3;i++) { free(xyfi[i]); }
 }

}
/*----------------------------------------------------------------*/
void StampajMatKrut(BOOL rostilj)
{Duz       *d=(*kd)[2][1];
 MatricaGT *K=NULL;
 do {d=Izaberi_stap(d,1,0);
     if (d)
        {float *elFo=NULL, *konst=NULL;
         K =MatKrutLok_Prr(d,rostilj,&elFo,&konst, materijal,ppres,sstap,uppp,upp_mat);
         if(K)
          {if(!(d->tata->ppkonst))
             {MatRavnoteze_Stampa(d,l_d(d));
              KonstanteStapa_Stampa(elFo, konst, d->tata);
              if(elFo) free(elFo);
              if(konst)free(konst);
             }
           StampajMatricuGT(K,NULL);
           char izvoz[3]; sprintf(izvoz,"k%c",d->id -48);
           IzveziMatricuGT(K,(const char *)izvoz);
           if(K->M)free(K->M); free(K);
           CrtajOkvirCrt();
      	  }
      	 else ST_helpMsg("Matrica krutosti je prazna! Proveri unos.");
        }
    }while(d);
 CrtezOsvezi();
}//--------------------------------------------------------------------------------------
/*----------------------------------------------------------------------*/
void MatKrutNosacaKnn_Stampaj(BOOL rostilj,short sttb,float uporedni[3])
{
 BOOL *nep=NULL;
 NepoznataPomeranja_inic(&nep,rostilj);
 NepoznataPomeranja_unos(nep);
 MatricaGT *KN =MatKrutNosaca_Prr((*kd)[2][0],(*kt)[2][0],tacaka_ukupno(),rostilj,
                                  materijal,ppres,sstap,uppp,upp_mat, gru,
                                  sttb,uporedni);
 char **zaglavlje=NULL;
 MatricaGT *Knn=MatKrutKnn_Prr(KN,tacaka_ukupno(),nep,&zaglavlje);
 StampajMatricuGT(Knn,zaglavlje);
 IzveziMatricuGT(Knn,(const char *)"knn");

 short i; for (i=0; i<Knn->n ;i++) if(zaglavlje[i])free(zaglavlje[i]);
 if(zaglavlje)free(zaglavlje);
 free(KN->M);  free(KN);
 free(Knn->M); free(Knn);
 if(nep)free(nep);
}

void MatKrutNosaca_Stampaj(BOOL rostilj,short sttb,float uporedni[3])
{MatricaGT *KN=MatKrutNosaca_Prr((*kd)[2][0],(*kt)[2][0],tacaka_ukupno(),rostilj,
                                  materijal,ppres,sstap,uppp,upp_mat, gru
                                  ,sttb,uporedni);
 StampajMatricuGT(KN,NULL);
 IzveziMatricuGT(KN,(const char *)"kn");
 free(KN->M); free(KN);
}
/*===============================================*/
void SuperpozicijaM(float *Xi)
{superpozicija_XiMi(Xi,(*kd)[2][1],br_dijag);
 //superpozicija_XiMi(Xi);
 //CrtajSveDijagrame(-1);//ngetchx();
 RaspOpt *Muk=NULL;
 Duz *d=(*kd)[2][1];
 Izaberi_stap((*kd)[2][1],2,-1);
 while (d)
   {Muk=(RaspOpt *)(d->tata->jos);
    free(Muk);
    d->tata->jos=NULL;
 	  d=d->sused[2][0];
   }
}
/*-------------------------------------------------*/
void Uporedni_pp(void)
{short pp;
 if ((pp=IzborPoprecnogPreseka(TRUE))) uppp=pp-1;
}
//------------------------------------------
void ElasticneTezine_Prikaz (void)
{Duz *d=NULL;
 short stanje=br_dijag;
 while (stanje)
 {stanje=IzborDijagrama();
  if (stanje)
   do{d=Izaberi_stap(d,4,stanje-1);
     }while(d);
 }
}
/*-------------------------------------------------*/
void Novi_(void)
{//Oslobodi_memoriju_duzi();
 Oslobodi_memoriju_nosaca();
 nuliraj_exttck();  nuliraj_extduz();
 InicParamCrtanja();
 InicGlobProm_statika_c();
}//---------------------------------------------------------

/*-------------------------------------------------------------------------------------------------*/
void UslovneJednacine_Prikaz(Matrica *D,float *d0,float *Xi,Matrica *Dinv,float *d0t,BOOL *dotd0)
{	HANDLE popmeni = PopupNew ("metoda sila",0);
	char txt[15];
	sprintf(txt,"D =[%cij]",133);           PopupAddText (popmeni, -1, txt, 1);
	sprintf(txt,"d =[%ci%c]",133,248);      PopupAddText (popmeni, -1, txt, 2);
  sprintf(txt,"D%c=[%cij]",180,129);      PopupAddText (popmeni, -1, txt, 3);
  sprintf(txt,"X =%cD%c%cd",173,180,215); PopupAddText (popmeni, -1, txt, 4);

  HANDLE exec_handle= PopupBegin (popmeni, 0);
  short result=1;
  while (result!=0)
  {result = PopupBeginDo (exec_handle, CENTER, CENTER, 0);
   switch (result)
      {case 1: StampajMatricu_glprozor(D,NULL);   break;
       case 2:{
       	       {InfoFill(1,A_NORMAL); char info1[30];
                sprintf(info1,"[%ci%c]=",133,248); Info(1,0,0.3,info1,A_REVERSE);
                if (dotd0[0]) sprintf(info1,"[%cio",133);
                else          sprintf(info1,"[");
                if (dotd0[1]) sprintf(info1,"%s%c%cit+%cic]",info1,(dotd0[0]?'+':' '),133,133);
                else          sprintf(info1,"%s]",info1);
                Info(1,1,0,info1,A_REVERSE);
               }
               Matrica dnula=(Matrica){br_dijag-1,1,d0};
               StampajMatricu_glprozor(&dnula,NULL);
              }
       break;
       case 3: StampajMatricu_glprozor(Dinv,NULL);break;
       case 4:{Matrica X=(Matrica){br_dijag-1,1,Xi};
               StampajMatricu_glprozor(&X,NULL);
              }
       break;
       default:
       break;
      }
  }
 MenuEnd (exec_handle);
 CrtezOsvezi();
}//---------------------------------------------------------
/*-----------------------------------------------------------------------------------------------------*/
void UslovneJednacine_Prr(Matrica **D,float **d0,float **Xi,Matrica **Dinv,float *d0t,BOOL *dotd0)
{char s[40],tackice[]="::::::::::";
 ST_helpMsg("Proracun u toku"); Info(3,0,0.30,tackice,A_REPLACE);
 sprintf(s,"D =[%cij]",133);    Info(3,0,0,s,A_REPLACE);
 /*Matrica koeficijenata*/
 if (*D){if((*D)->M)free((*D)->M);
                    free(*D);
        }
 *D=MatKoef();
    Info(3,0,0.45,tackice,A_REPLACE);Info(3,0,0.55,tackice,A_REPLACE);
    sprintf(s,"D =[%cij]",133);      Info(3,0,0,s,A_REPLACE);
 Prr_VekSlobClanova(d0,d0t,dotd0);   //Vektor slobodnih clanova
    Info(3,0,0,tackice,A_REPLACE); sprintf(s,"d =[%ci%c]",133,248);
    Info(3,0,0,s,A_REPLACE); Info(3,0,0.65,tackice,A_REPLACE);
 *Dinv=InverznaMatrica(*D);//Inverzna matrica koeficijenata usl.jed.
    Info(3,0,0,tackice,A_REPLACE); sprintf(s,"D%c=[%cij]",180,129);
    Info(3,0,0,s,A_REPLACE); Info(3,0,0.75,tackice,A_REPLACE);Info(3,0,0.85,tackice,A_REPLACE);
 Matrica dnula=(Matrica){br_dijag-1,1,*d0};
// Matrica *XXi=mnozi_M( mnozi_M_skalarom(*Dinv,-1.0), &dnula );
 Matrica *XXi=mnozi_M_skalarom( mnozi_M(*Dinv,&dnula), -1 );
    Info(3,0,0,tackice,A_REPLACE); sprintf(s,"X =%cD%c%cd",173,180,215);
    Info(3,0,0,s,A_REPLACE); Info(3,0,0.95,tackice,A_REPLACE);
 if (*Xi)free(*Xi); *Xi=XXi->M;
 sprintf(s,"                                       ");
 Info(3,0,0,s,A_REPLACE);Info(3,0,0.5,s,A_REPLACE);
}//----------------------------------------------------------------------------------


Matrica * MatKoef(void)//
{//alocira memoriju i proracunava matricu koeficijenata
 MatricaGT *MK=MatKoefGT();
 Matrica *D=GTMuM(MK);
 free(MK->M); free(MK);//Vise nije potrebna
 return D;
}
//-----------------------------------------------------------------------------------
MatricaGT * MatKoefGT(void)//Ova funkcija zavisi od <statika-matrice> inace bi isla u <statika-c>
{//alocira memoriju i proracunava matricu koeficijenata kao gornju trougaonu
 float proizvod=0;
 Duz *d=(*kd)[0][0];
 short stanje1=1,stanje2=1;
 MatricaGT *MK=calloc(1,sizeof(MatricaGT));
            MK->n=MK->m=br_dijag-1;
            MK->M=(float*)calloc(brel_Tmatrice(MK->n),sizeof(float));
 while (stanje1<br_dijag)
 {while (stanje2<br_dijag)
        {proizvod=0;
         d=(*kd)[0][0];
         while (d)
               {proizvod+=Prr_Veresc(d,stanje1,stanje2,FALSE);
                d=d->sused[0][1];
               }
         (*(eGTM(MK,stanje1,stanje2)))=proizvod;
         //{char s[30];sprintf(s,"%d%d:%f",stanje1,stanje2,proizvod);ST_helpMsg(s);ngetchx();}
         stanje2++;
        }
  stanje1++;stanje2=stanje1;
 }
 return MK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
void Verescagin(void)
{/*Izbor 2 dijagrama za mnozenje*/
 if (br_dijag==0){ST_helpMsg("Nema nijednog unetog dijagrama!");ngetchx();return;}
 short i,s1=0,s2=0;char s[6];
 /*popmeni 1*/
 HANDLE pop1 = PopupNew ("dijag.1", 0);
 for (i=1;i<=br_dijag;i++)
     {sprintf(s,"M%d",i-1);PopupAddText(pop1,-1, s,i);}
 HANDLE exec_1 = PopupBegin (pop1, 0);
 s1 =PopupBeginDo(exec_1,45,CENTER,0);
 MenuEnd (exec_1); //HeapFree(pop1);//ovo izaziva CRASH
 if (s1==0)return;
 /*popmeni 2*/
 HANDLE pop2 = PopupNew ("dijag.2", 0);
 for (i=1;i<=br_dijag;i++)
     {sprintf(s,"M%d",i-1);PopupAddText(pop2,-1, s,i);}
 HANDLE exec_2 = PopupBegin (pop2, 0);
 s2 =PopupBeginDo(exec_2,75,CENTER,0);
 MenuEnd (exec_2); //HeapFree(pop2);//ovo izaziva CRASH
 if (s2==0)return;

 /*Proracun i upis stringova za stampu*/
 float proizvod=0;
 Duz *d=(*kd)[0][0];  //ST_helpMsg("Idemo na Prr_Veresc");ngetchx();
 while (d) {proizvod+=Prr_Veresc(d,s1-1,s2-1,TRUE); d=d->sused[0][1];}

 /*Info 1*///obavezno ostavi unutar posebnog bloka-imao sam CRASH bez {}
 {InfoFill(1,A_NORMAL);
  char info1[30];
  sprintf(info1,"Verescagin");                       Info(1,0,0,info1,A_REVERSE);
  sprintf(info1,"%c%d%d=%f",133,s1-1,s2-1,proizvod); Info(1,1,0,info1,A_REVERSE);
 }

 /*Kretanje kroz stapove sa stampom racuna*/
 short vidljivost[3]={pc->vid.tacke,pc->vid.oznake_tacaka,pc->vid.veze};//pamtim vidljivost
 pc->vid.tacke=0; pc->vid.oznake_tacaka=0; pc->vid.veze=0;
 d=(*kd)[0][0];
 while (d)
  {short i;
 	 {char s[40];sprintf(s,"%c%d%d=%f, ENTER za ceo racun",133,s1-1,s2-1,proizvod);ST_helpMsg(s);}
 	 d=Izaberi_stap(d,3,s1-1);
 	 if (d)
 	    {/*prozor za ispis proracuna*/
       WINDOW wind;
       WinOpen (&wind, &(WIN_RECT){pc->crt[0][0],pc->crt[1][0],
                                   LCD_WIDTH,pc->crt[1][1]},WF_SAVE_SCR | WF_TTY);
       WinFont (&wind, F_4x6);
       WinActivate (&wind);
       char s[100];BOOL staviplus=FALSE;
       sprintf(s,"M%d%cM%d\n",s1-1,215,s2-1);
       WinStr (&wind,s );
       for (i=0;i<4;i++)
           {if ((((Veresc*)(Jos_(d)))->racun[i])!=NULL)
               {if (staviplus)sprintf(s,"     +%s\n",*(((Veresc*)(Jos_(d)))->racun+i));
                else         {sprintf(s,"    = %s\n",*(((Veresc*)(Jos_(d)))->racun+i));
                              staviplus=TRUE;
                             }
                WinStr (&wind, s);
               }
           }//ovo je bio detaljniji proracun
       if ((((Veresc*)(Jos_(d)))->racun+4))
          {sprintf(s,"N%d%cN%d\n",s1-1,215,s2-1);                 WinStr (&wind,s );
       	   sprintf(s,"    = %s",*(((Veresc*)(Jos_(d)))->racun+4));WinStr (&wind,s );
          }

        /*
        if (staviplus)//znaci da ima vise sabiraka, pa ajde i njih da prikazemo
           {staviplus=FALSE;
            for (i=0;i<4;i++)
                {if ((((Veresc*)(Jos_(d)))->racun[i])!=NULL)
                    {if (staviplus)sprintf(s,"     +%f\n",*(((Veresc*)(Jos_(d)))->ukupno));
                     else         {sprintf(s,"    = %f\n",*(((Veresc*)(Jos_(d)))->ukupno));
                                   staviplus=TRUE;
                                  }
                     WinStr (&wind, s);
                    }
                }
           }//a ovo samo sabirci
           */
       {char s[40];sprintf(s,"\n    = %f",((Veresc*)(Jos_(d)))->ukupno);WinStr (&wind,s);}
       ngetchx();
       WinClose (&wind);
      }
  }
 /*Oslobadjanje memorije zauzete Verescaginom*/
 brisi_sve_Veresc();
 /*Vrati vidljivost koja je vazila pre ulaska u funkciju*/
 pc->vid.tacke        =vidljivost[0];
 pc->vid.oznake_tacaka=vidljivost[1];
 pc->vid.veze         =vidljivost[2];
 CrtezOsvezi();
}
/*--------------------------------------------------------------------------------*/
void Izbor_sabiraka_za_d0(BOOL *dotd0)
{short izbor=1;
 HANDLE pop1 = PopupNew ("Spolj.opterecenje", 0);
 {char s[10];sprintf(s,"%cio",133);         PopupAddText(pop1,-1, s,1);}
 {char s[10];sprintf(s,"%cit+%cic",133,133);PopupAddText(pop1,-1, s,2);}
 HANDLE exec_ = PopupBegin (pop1, 0);
 if (dotd0[0])MenuCheck(exec_, 1, MC_CHECK);
 if (dotd0[1])MenuCheck(exec_, 2, MC_CHECK);
 while (izbor!=0)
  {izbor =PopupBeginDo(exec_,45,CENTER,0);
   MenuCheck(exec_, izbor, MC_FLIP);
   if (izbor!=0)dotd0[izbor-1]=(!(dotd0[izbor-1]));
  }
 MenuEnd (exec_);
 return;
}//---------------------------------------------------------------------------------
BOOL Zadaj_delta_0t(float **d0t)
{float *novo_d0t=(float *)calloc(br_dijag-1,sizeof(float));
 if (!(Delta_0t_dlg(novo_d0t,br_dijag)))
      {
       free(novo_d0t);
       novo_d0t=NULL;
       return FALSE;
      }
 else {
 	     if(*d0t)free(*d0t);//oslobodi memoriju prethodnog d0t
 	     *d0t=novo_d0t;
       return TRUE;
      }
}
/*--------------------------------------------------------------------------------*/
void GrupeDijagrama(void)
{

}
/*---------------------------------------------------------------------------------*/
void ZadajDijagram(void)
{
 short stanje=IzborDijagrama();
 if (stanje!=0)//Nije ESC
 {
  if (stanje>br_dijag) DodajNovoStanje();/*dodaje se novo stanje*/
	//else/*izmena postojeceg stanja*/
 	UnosDijagrama(stanje-1);//Jer cu da pozivam funkcije sa pointerom,
 	                      //a izbor nam stize iz PopUp-a sa 1 za pocetni indeks
 }
}//---------------------------------------------------------------------------------

//-----------Izbor dijagrama-----------------
short IzborDijagrama(void)
/*Vraca stvarni redni broj, pocev od 1.
  Rezultat je 0 ako je ESC, a veci od br_dijag ako treba uneti novo stanje*/
{if ((*kd)[0][0]==NULL){ST_helpMsg("Nema unetih stapova!");ngetchx();return 0;}
 short i=0;char s[6];
 HANDLE popmeni = PopupNew ("Izbor dijagrama", 0);
 for (i=1;i<=br_dijag;i++){sprintf(s,"M%d",i-1);  PopupAddText(popmeni,-1, s,i);}
                                                  PopupAddText(popmeni,-1, "dodaj",i);
 HANDLE exec_handle = PopupBegin (popmeni, 0);//oznacavam novi pp
 i =PopupBeginDo (exec_handle, CENTER, CENTER, i_pp+1);
 MenuEnd (exec_handle);
 return i;
}
/*--------------------------------------------------------------------------------*/
void UnosDijagrama(short stanje)
{Duz *d=(*kd)[2][1];
 float pi,pk,f,alfa,N;//ugao pod kojim deluje opterecenje
 short vid[3]={pc->vid.tacke,pc->vid.oznake_tacaka,pc->vid.veze};
 pc->vid.tacke=pc->vid.oznake_tacaka=pc->vid.veze=0;
 do {d=Izaberi_stap(d,2,stanje);
     if (d)
        {/*ima li vec opt. na stapu?*/
         if (Dij_(d,stanje)==NULL)/*nema->novo*/
           *((d->tata)->dij+stanje)=rasp_opt_novo((float [3]){0,0,0},0,0);
         pi  =(Dij_(d,stanje))->q[0];
         pk  =(Dij_(d,stanje))->q[1];
         f   =(Dij_(d,stanje))->q[2];
         alfa=(Dij_(d,stanje))->alfa;
         N   =(Dij_(d,stanje))->N;
         if (Unos_dijag_dlg(&pi,&pk,&f,&alfa,&N))
                           {rasp_opt_izmena(Dij_(d,stanje),(float [3]){pi,pk,f},alfa,N);
                           //char s[40];sprintf(s,"%f,%f,%f",pi,pk,f);ST_helpMsg(s);ngetchx();
                           }
        }
    }while (d);//(((ngetchx())!=KEY_ESC)&&(d));
 pc->vid.tacke        =vid[0];
 pc->vid.oznake_tacaka=vid[1];
 pc->vid.veze         =vid[2];
}
/*-----------------------    Unos stapova    -------------------------------------*/
BOOL UslovZaUnosStapova(void)
{BOOL uslov=UslovZaUnosDuzi();
 if (uslov)
    {short oket=1;//0 znaci da je ESC
     uslov=(materijal!=NULL);
     if (!uslov) {oket=IzborMaterijala();}
     uslov=(ppres!=NULL);
     if ((oket!=0)&&(!uslov)){oket=IzborPoprecnogPreseka(FALSE);}
     return ((uslov)||(oket!=0));//unet materijal i pop.pres.(jer nije ESC u dijalozima)
    }
  else//U ovaj deo ne bi trebalo nikad ni da se udje jer je u tom slucaju zatamnjen izbor funkcije za unos stapova
    {DlgMessage("Unos stapova",
                "Prvo se mora ili definisati pomocna mreza ili uneti bar 1 tacka.",BT_NONE,BT_OK);
     return FALSE;//nije ispunjen uslov za unos duzi
    }
}
/*--------------------------------------------------------------------------------*/
BOOL  Unos_materijala(void)
{float alfa=0, ni=0, me=1, G=0;         //rbr za stampu je +1     //ST_helpMsg("Unos_materijala");ngetchx();
 if (materijal!=NULL) alfa=ni=me=G=0;//predlozene vrednosti
 if (Unos_materijala_dlg(&me,&ni,&alfa,&G,br_mat+1))//rbr za stampu je +1
     {materijal_novi(me,ni,alfa,G);
      return TRUE;
     }
 else return FALSE;
}

BOOL  Unos_preseka(void)
{float mi,fpp,jt;
 if (ppres==NULL) {mi=fpp=1;jt=0;} else mi=fpp=jt=0;//predlozene vrednosti
 if (Unos_preseka_dlg(&mi,&fpp,&jt,br_pp+1))//rbr za stampu je +1
     {pop_pres_novi(fpp,mi,jt);
      return TRUE;
     }
 else return FALSE;
}
/* . . . . . . .  . . Izbor poprecnog preseka . . . . . . . . . . . . . . . . . .  .*/
unsigned short IzborPoprecnogPreseka(BOOL slozenistap)
/*Vraca stvarni redni broj, pocev od 1.
  Rezultat je 0 ako je ESC. Rezultat je =br_pp za novi pp, >br_pp za slozeni stap */
{unsigned short i=0;
 if (ppres==NULL) {if (Unos_preseka()) {i=br_pp; i_pp=br_pp-1;}
                  }
 else
    { HANDLE popmeni = PopupNew ("Popr.presek: I,F", 0);
    	char txt[18]; char strlc=32;char romb=32;//32 je spejs,127 je romb-kod uporednog p.p.,22 je strelica desno-za aktuelni
    	for (i=0;i<br_pp;i++)
    	    {if (uppp==i) romb=127;  else romb=32;
    	     if (   i_pp==i) strlc=22; else strlc=32;
        	 sprintf(txt,"%c%-^1.3r%c%^-1.3r",strlc,(ppres+i)->I,romb,(ppres+i)->F);/*format %1.3r zauzima najvise 8 znakova:primer:1.659E-1*/
        	 PopupAddText (popmeni, -1, txt, i+1);
     	    }//indeksi liste su od 1, a preseci se broje od 0
     	    sprintf(txt,"dodaj novi presek");PopupAddText (popmeni, -1, txt, i+1);
     	    if(slozenistap)
     	        {sprintf(txt,"I%cconst",157);PopupAddText (popmeni, -1, txt, i+2);}
	    HANDLE exec_handle = PopupBegin (popmeni, 0);//oznacavam novi pp
      i =(unsigned) PopupBeginDo (exec_handle, CENTER, CENTER,0);// i_pp+1);
      MenuEnd (exec_handle);
    }
 return i;
}
/* . . . . . . . . . .  . Izbor materijala . . . . . . . . . . . . . . . . . . . . .*/
unsigned short IzborMaterijala(void)
/*Vraca stvarni redni broj, pocev od 1.
  Rezultat je 0 ako je ESC. Rezultat je >=br_pp za novi pp */
{unsigned short i_m=i_mat,i=0;
 if (materijal==NULL) {if (Unos_materijala()) {i=br_mat; i_m=br_mat-1;}
                      }
 else
    { HANDLE popmeni = PopupNew ("Materijal: E,v", 40);
    	char txt[18];
    	for (i=0;i<br_mat;i++)
    	    {sprintf(txt,"%-^1.3r,%^-1.3r",(materijal+i)->E,(materijal+i)->ni);
    	     PopupAddText (popmeni, -1, txt, i+1);
     	    }//indeksi liste su od 1, a preseci se broje od 0
     	    sprintf(txt,"dodaj novi mat.");PopupAddText (popmeni, -1, txt, i+1);
	    HANDLE exec_handle = PopupBegin (popmeni, 0);//oznacavam novi pp
      i =(unsigned) PopupBeginDo (exec_handle, CENTER, CENTER, 0);//i_mat+1);
      MenuEnd (exec_handle);
    }

 if (i!=0)//Ako nije ESC pri izboru
    {if (i<=br_mat) i_m=i-1; //Izabran je postojeci pp (PAZI-izbor je iz liste sa indeksima od 1)
     else {if (Unos_materijala()) i_m=br_mat-1;
           else i=0;
          }
    }
 i_mat=i_m;//i_mat je globalna promenljiva zasad, inace bi bio ulazni pointer ka spoljasnjoj promenljivoj
 return i;
}

/* . . . . . . . . . .  . Izbor granicnih uslova . . . . . . . . . . . . . . . . . . . . .*/
unsigned short IzborGranicnihUslova(void)
/*Vraca stvarni redni broj, pocev od 1.
  Rezultat je 0 ako je ESC. Rezultat je >=br_pp za novi pp */
{//ICON ikona;
 unsigned short i=0;
 /*if (gru==NULL) {if (Unos_materijala()) {i=br_mat; i_mat=br_mat-1;}
                      }
 else	*/
    { HANDLE popmeni = PopupNew ("Granicni uslovi:", 0);
    	char txt[18];
      sprintf(txt," |---K---|");          PopupAddText (popmeni, -1, txt, K_STAP);/*K*/
      sprintf(txt," o---G---|");          PopupAddText (popmeni, -1, txt, oG_STAP);/*Ga*/
      sprintf(txt," |---G---o");          PopupAddText (popmeni, -1, txt, Go_STAP);/*Gb*/
      sprintf(txt," %c-prost-%c",22,21);  PopupAddText (popmeni, -1, txt, PROST_STAP);/*prost*/
      sprintf(txt," |---S---x");          PopupAddText (popmeni, -1, txt, S_STAP);/*S*/
        //indeksi liste su od 1, a preseci se broje od 0
	    HANDLE exec_handle = PopupBegin (popmeni, 0);//oznacavam novi pp
      //MenuCheck (exec_handle, i_pp+1, MC_CHECK);//chekiram aktuelni pop.pres.
      i =(unsigned) PopupBeginDo (exec_handle, CENTER, CENTER, i_gru+1);
      MenuEnd (exec_handle);
    }
 return i;
}
/*-------------------------*/
BOOL IzmenaPoprecnogPreseka(Stap *nstap,SlozeniStap *slst)
//Rezultat je TRUE ako je izvrsena izmena
{BOOL rez=FALSE;//rezultat je TRUE ako se izvrsi neka izmena
 unsigned izbor=IzborPoprecnogPreseka(TRUE);//char s[20];sprintf(s,"izbor=%d",izbor);ST_helpMsg(s);ngetchx();
 if ((rez=(izbor!=0)))//Ako nije ESC pri izboru
    {if (izbor<=br_pp)//Izabran je postojeci pp (PAZI-izbor je iz liste sa indeksima od 1)
        {nstap->pp=izbor-1;
         nstap->ppkonst=TRUE;
        }
     else {if (izbor==br_pp+1)/*Novi poprecni presek*/
              {if (Unos_preseka())
                    {nstap->pp=br_pp-1;
                     nstap->ppkonst=TRUE;
                    }
               else {izbor=0;//{char s[20];sprintf(s,"izbor=0");ST_helpMsg(s);ngetchx();}
                    }
              }
          else/*Stap promenljivog poprecnog preseka*/
              {//if (slst->ksi)free(slst->ksi);  if (slst->pp) free(slst->pp);
               if (!(nstap->ppkonst))slozstap_listapreseka(nstap,slst);
               if (FormirajSlozeniStap(nstap,slst))
                  {//ako je pocetni stap bio konst.pp, menja se indeks pp.
                   if (nstap->ppkonst) nstap->pp=br_sstp;//kasnije ce pri dodavanju slozenog stapa br_stapa da se poveca za 1, pa nema oduzimanja
                   nstap->ppkonst=FALSE;
                  }
               else izbor=0;
              }
          }
    }
 if (izbor) i_pp=nstap->pp;
 return rez;
}
/*-----------------------*/
BOOL KarakteristikeStapa(Duz *d, SlozeniStap *slst, short karak)
/*Pozivanje dijaloga za izbor karakteristika stapa za unos/izmenu*/
//Rezultat je TRUE ako je izvrsena izmena
{short promena;
	switch (karak)
  {case 1:return IzmenaPoprecnogPreseka(d->tata,slst);
          break;
 	 case 2:if ((promena=IzborMaterijala()))
 	                   d->tata->mat = promena-1;
 	                   return TRUE;
          break;
   case 3:if ((promena=IzborGranicnihUslova()))
                     d->tata->tip = promena;
                     return TRUE;
          break;
   default:break;
  }
 return FALSE;
}
//----------------------------
void IzmeniKarakteristikeStapa(Duz *d)
/*Izbor i izmena karakteristika vec unetog stapa, a to sve za unos/promenu*/
{short karak;
 while ((karak=IskacuciMeni(3,(const char* [3])
               {"Popr.presek","Materijal","Granicni uslovi"},"Izmena stapa")))
       {if (d->tata->ppkonst)//stap trenutno ima konstantan popr.presek
             {SlozeniStap slst;//pomocna privremena promenljiva
              slozeni_stap_izmena(&slst,0,NULL,NULL); //za slucaj da se izabere slozeni pp
    	        if (KarakteristikeStapa(d,&slst,karak))
    	           {//Upis pomocne slst u stap (tj. d->tata) ako je sada unet prom.pp
    	            if (!(d->tata->ppkonst))
    	               {slozeni_stap_novi(slst.brd,slst.ksi,slst.pp);
    	                d->tata->pp=br_sstp-1;
    	               }
    	           }
             }
        else KarakteristikeStapa(d,&(sstap[d->tata->pp]),karak);//ako je slozeni pp, samo izmena postojeceg
       }
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
void Stapovi_izmena(void)
/*Izbor i izmena karakteristika vec unetih stapova*/
{Duz *d=(*kd)[2][1];
 do { if ((d=Izaberi_stap(d,1,0)))
         {IzmeniKarakteristikeStapa(d);
         }
    }while(d);
 CrtezOsvezi();
}
/*                    Unos stapa_ekr, Unos_stapova                                  */
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//Samo za unos/promenu tacaka duzi d kroz crtez
/*Duzi d mora vec da je dodeljena oznaka i pocetna tacka*/
short Unos_stapa_ekr(Duz *d)
{Tacka *ti=d->ik[0];//pocetna tacka se mora zadati!! kroz d->ik[0]
 Tacka *tk=d->ik[1];//krajnja tacka ne mora da se zada
 ST_helpMsg("Izaberi tacku -i- ili APPS/CATALOG/ESC"); ST_busy(0) ;

 if ((ti=IzaberiTackuNosaca(ti,1))!=NULL)//pritisnut je ENTER, tj. izabrana tacka
    { //sprintf(txt,"i: %c",ti->id); Info(1,1,0.1,txt,A_REVERSE);
      char txt[20];
      sprintf(txt,"%c(%3.2f,%3.2f) -",id_(ti),x_(ti),y_(ti));Info(3,0,0,txt,A_REPLACE);
      ST_helpMsg("Izaberi tacku -k- ili APPS/CATALOG/ESC"); ST_busy(0) ;
     BOOL duplikat=TRUE;
     while (duplikat)
     {if ((tk=IzaberiTackuNosaca(ti,2))!=NULL)//ako je pritisnut ENTER, tj. izabrana tacka
         { d->ik[0]=ti;  d->ik[1]=tk; //ovo je sad uneta duz
           if (duz_trazi_duplikat(d)==NULL)//nema duplikata
              {//sprintf(txt,"k: %c",tk->id);                          Info(1,1,0.55,txt,A_REVERSE);
               sprintf(txt,"%c(%3.2f,%3.2f) ",id_(tk),x_(tk),y_(tk));Info(3,0,INFO3_KOORDTACKE,txt,A_REPLACE);
               //Info(3,0,0.35,txt,A_NORMAL);
               return 1; //Rezultat f-je je 1 za unetu duz, koja se pritom i menja(duz)
              }
    	     else ST_helpMsg("Vec postoji! Izaberi drugu tacku ili ESC.");//ovako se ide na ponovni unos tacke
         }
      else return 0; //Za ESC nema promene duzi, a rezultat f-je je 0, izlaz iz f.je
     }
    }else return 0;//nije uneta ni prva tacka, ESC, izlaz bez promena
 return 0;//dovde ne bi trebalo nikad ni da dodje, al da se C ne bi zalio evo mu ga
}

void Unos_stapova(void)
{if (UslovZaUnosStapova()) //mora da ima unetih tacaka ili def.grida da bi se unosile duzi
  {//Nova duz: //Krecemo od najlevlje tacke
   Duz d; d.ik[0]=(*kt)[0][0]; d.ik[1]=NULL; d.tata=NULL;//d mora biti promenljiva,ne pokazivac na Duz
   short tipka=1;//cisto da bi usli u while petlju

 	 while (((tipka)!=0)&&((tipka)!=KEY_ESC))//unosimo duzi sve dok su dobri unosi i nije ESC
   	{//prvo dodela id-a za novu duz
   	 if ((*kd)[2][1]==NULL) d.id='a';//prvi unos duzi:=>pocetni ID za duz
   	 else                   d.id=((*kd)[2][1]->id)+1;//sledeci ID na spisku a->z

     //InfoKarakteristikeStapa(i_pp,i_mat);//popuni info 2
 	   //Unos i eventualno dodavanje nove duzi(ako nije ESC-tj. oket=0)
 	   if((tipka=Unos_stapa_ekr(&d))==1) //ovde je 1 pokazatelj za uspesan unos
 	      {
 	        /*privremene promenljive i inic.vrednosti */
 	        SlozeniStap slst; slozeni_stap_izmena(&slst,0,NULL,NULL);
 	        Stap nstap;  stap_izmena(&nstap, &d, TRUE, i_pp, i_mat,/*Obavezno pp konstantan!!!*///Zbog indeksa slozenog stapa-vidi IzmenaPoprecnogPreseka
 	                                  TipStapaUGrUslove(1,(short[2]){0,0}), 1 , &d);
          unsigned izbor=0;

          InfoFill(1,A_NORMAL);InfoGranicniUslovi(nstap.tip);//zacrni+popuni info 1
 	        pc->rezim=A_NORMAL;  CrtajStap(&d);
 	        //tipka=1;

 	        while ((tipka!=KEY_ENTER)&&(tipka!=KEY_ESC)/*Izlaz na strelice,ENTER i ESC*/
 	             &&(tipka!=344)      &&(tipka!=338)    &&(tipka!=337)&&(tipka!=340))
 	        {
 	         ST_busy(0);ST_helpMsg("I / E /gr.uslovi : APPS/CATALOG/ F5 /ESC");//ST_busy(ST_IDLE);
 	         tipka=ngetchx();
 	         switch (tipka)
 	          {case KEY_ENTER:case 344:case 338:case 337:case 340:
 	            /*DODAVANJE NOVOG STAPA*/
 	            {Duz * nduz=duz_dodaj(d.id,d.ik);//nasa duz je poslednja uneta pa je ona.. 	             //{char s[35]; sprintf(s,"pp:%d, mat:%d, gru:%d",i_pp,i_mat,i_gru);ST_helpMsg(s);ngetchx();}
      	       SlozeniStap *slozstap=NULL;
      	       if ((slst.ksi)&&(slst.pp)) // ev.dodavanje slst u sstap
 	                {slozstap=slozeni_stap_novi(slst.brd,slst.ksi,slst.pp);
 	                 slozeni_stap_izmena(&slst,0,NULL,NULL);//nizksi=NULL; nizpp=NULL;
 	                }
 	             //dodavanje novog stapa i provera preseka njene duzi sa ostalim unetim
 	             stap_novi(nduz,(slozstap==NULL),nstap.pp,nstap.mat,TipStapaUGrUslove(nstap.tip,(short[2]){0,0}),nstap.tip,NULL);//vezujem za ovu duz podatke o stapu               //{char s[35]; sprintf(s,"E:%5r, ni:%5r, I:%5r",E_(nduz),ni_(nduz),I_(nduz));ST_helpMsg(s);ngetchx();}
	             NizTacaka * nizpt=duz_proveri_preseke((*kd)[2][1]); ST_helpMsg("Proveravam preseke...");
 	             stap_podeli_u_prestac(nizpt); //CrtajPresTacke(nizpt);
 	             {char s[35]; sprintf(s,"%35s"," "); Info(3,0,0,s,A_REPLACE);}//brisem potvrdjene koordinate iz Info3
	             if (slozstap) i_pp=0;//ponovo postavljam podrazumevan pp akko je bio unet stap sa promenljivim pp
              }
             break;
             case 265:/*APPS*/
              {if (IzmenaPoprecnogPreseka(&nstap,&slst))
                  InfoKarakteristikeStapa(nstap.ppkonst,nstap.pp,i_mat);
  	          }
             break;
             case 278:/*CATALOG*/
              {if ((izbor=IzborMaterijala())) nstap.mat=izbor-1;
               InfoKarakteristikeStapa(nstap.ppkonst,i_pp,i_mat);
  	          }
             break;
             case 272:/*F5*/
              {//ST_busy(0);ST_helpMsg("Promena granicnih uslova stapa");
               if ((izbor=IzborGranicnihUslova())) nstap.tip=izbor;//char s[20];sprintf(s,"izbor=%d",izbor);ST_helpMsg(s);ngetchx();
               InfoFill(1,A_NORMAL);/*Info1*/InfoGranicniUslovi(nstap.tip);
  	          }
             break;
             case KEY_ESC:
 	            {pc->rezim=A_XOR;  CrtajStap(&d);}//izbrisi iscrtanu duz od koje se odustalo
 	           break;
 	          }
 	        }
      	}
     //poslednje uneta tacka da bude pocetna za sledeci unos
     d.ik[0]=d.ik[1];
 	 	}
  }
 else
  {DlgMessage("Unos stapova",
  "Prvo moraju biti uneti bar 1 materijal i 1 tip popr.preseka.",BT_NONE,BT_OK);
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*========================= FORMIRANJE SLOZENOG STAPA  ===================================*/
BOOL FormirajSlozeniStap(Stap *nstap,SlozeniStap *slst)/*Upis u sstap se NE vrsi ovde*/
/*na kraju nizksi sadrzi ksi za medjutacke(ukupno brdelova-1), a nizpp rbr pop.preseka za delove (ukupno brdelova)*/
{SlozeniStap ps; ps.brd=0; ps.pp=NULL; ps.ksi=NULL;
 float del=1,//delioc: del=1 za unosenje ksi, a del=L za unosenje duzine
       levo=0,unos=0; //leva granica, unos, relativni polozaj na stapu u odnosu na kraj -i-
 char  znak;
 short ksi=IzabNacinZadTackePostStapa(); if (ksi==0)return FALSE;
 unsigned p;

 if (ksi==1) { znak='X'; del=l_d(nstap->duz); }
 else        { znak=138; del=1; }
 levo=0;

 /*unos ksi i popr.pres. za sve delove osim poslednjeg*/
 BOOL  oket=TRUE;
 do    {/*poprecni presek za deo*/
 	      p=IzborPoprecnogPreseka(FALSE);//ovo je indeks popr.preseka pocev od 1
 	      if (p>br_pp)/*Novi poprecni presek*/
 	         {if ((oket=Unos_preseka())) p=br_pp;
           }
 	      if (oket)/*tacka -k- za deo*/
 	         {oket=Unos_ksi_stapa(&unos,znak,ps.brd+1);
 	          while((oket)&&((unos<=levo)||(unos/del>=1)))
 	               {char greska[40];sprintf(greska,"Uneto %c nije u prihvatljivim granicama.",znak);
                  if ((oket=(DlgMessage("Greska!",greska,BT_OK,BT_CANCEL)))!=KEY_ESC)
                      oket=Unos_ksi_stapa(&unos,znak,ps.brd+1);
	               }
            if((oket)&&(oket=(p!=0)))//unos dobar
 	               {/*upis unetih podataka u pomocne nizove*/
    	            ps.brd++;
    	            ps.ksi =(float *)realloc(ps.ksi,(ps.brd)*sizeof(float)); ps.ksi[ps.brd-1]=unos/del; levo=unos;
 	                ps.pp  =(short *)realloc(ps.pp ,(ps.brd)*sizeof(short)); ps.pp [ps.brd-1]=p-1;
                 }
	      	 }
       }
 while (oket);
 ps.brd++;//poslednji deo upisujem ovde, jer je karakteristican ....
 ps.pp=(short *)realloc(ps.pp,(ps.brd)*sizeof(short)); ps.pp[ps.brd-1]=p-1;//.... niz .pp je duzi za 1 od nizksi
 slozstap_listapreseka(nstap,&ps);
 short enter=DlgMessage("Promenljiv poprecni presek","Potvrdi unos.",BT_OK,BT_CANCEL);
 oket=(enter==KEY_ENTER);

 if (oket)/*upisujem nove ppreseke sa zadatim karakteristikama, kao i novi slozeni stap*/
      {if (slst->ksi)free(slst->ksi); slst->ksi=ps.ksi;
       if (slst->pp) free(slst->pp);  slst->pp =ps.pp;
                                      slst->brd=ps.brd;
      }
 else {free(ps.ksi); free(ps.pp);
       //*nizksi=NULL; *nizpp=NULL; *br_delova=0;
 	    }
 return oket;
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

/*
void Zauzece_memorije_po_tipovima(void)
{clrscr();
 FontSetSys(F_4x6);
 printf("short:%ld, short*:%ld\n",        sizeof(short),         sizeof(short*));
 printf("unsig:%ld, unsig*:%ld\n",        sizeof(unsigned short),sizeof(unsigned short*));
 printf("long:%ld, long*:%ld\n",          sizeof(long),          sizeof(long*));
 printf("unsig long:%ld, uns l*:%ld\n",   sizeof(unsigned long), sizeof(unsigned short*));
 printf("float:%ld, float*:%ld\n",        sizeof(float),         sizeof(float*));
 printf("double:%ld, double*:%ld\n",      sizeof(double),        sizeof(double*));
 printf(" char:%ld,  char*:%ld\n",        sizeof(char),          sizeof(char*));
 ngetchx();
 printf("exttck:%ld, exttck*:%ld\n",      sizeof(exttck),   sizeof(exttck*));
 printf("extduz:%ld, extduz*:%ld\n",      sizeof(extduz),   sizeof(extduz*));
 printf("extduz:%ld, extduz*:%ld\n",      sizeof(ParamCrt), sizeof(ParamCrt*));
 printf("Tacka:%ld, Tacka*:%ld\n",        sizeof(Tacka),    sizeof(Tacka*));
 printf("  Duz:%ld,   Duz*:%ld\n",        sizeof(Duz),      sizeof(Duz*));
 printf("Materijal:%ld, Materijal*:%ld\n",sizeof(Materijal),sizeof(Materijal*));
 printf("PopPres:%ld, PopPres*:%ld\n",    sizeof(PopPres),  sizeof(PopPres*));
 printf("GrUsl:%ld, GrUsl*:%ld\n",        sizeof(GrUslovi), sizeof(GrUslovi*));
 printf("Stap:%ld, Stap*:%ld\n",          sizeof(Stap),     sizeof(Stap*));
 printf("PodZaDlg:%ld, PodZaDlg*:%ld\n",sizeof(PodZaDlg), sizeof(PodZaDlg*));
 printf("FrmtTab:%ld, FrmtTab*:%ld\n",    sizeof(FrmtTab),  sizeof(FrmtTab*));
 printf("ParamCrt:%ld, ParamCrt*:%ld\n",  sizeof(ParamCrt), sizeof(ParamCrt*));
 printf("Crtez:%ld, Crtez*:%ld\n",        sizeof(crt),      sizeof(&crt)); //Ovo nije tip nego globalna promenljiva
 printf("ICON:%ld, ICON*:%ld\n",          sizeof(ICON),     sizeof(ICON*));
 ngetchx();
}
*/
