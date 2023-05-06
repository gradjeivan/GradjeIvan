// C Source File
// Created 22.10.2014; 17:29:00

#include <tigcclib.h>
#include "statika-c.h"
 

/*-----------------PROTOTIPOVI LOKALNIH FUNKCIJA--------------------*/


/*_________________________________________________________________________________*/

/*---------------------------    FUNKCIJE   ---------------------------------------*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
void NepoznataPomeranja_inic(BOOL **nep,BOOL rostilj)
//Inicijacija niza koji sadrzi info koja su pomeranja nepoznata
{short //ind,i,
       brtacaka=tacaka_ukupno();

 if(*nep)free(*nep);
 *nep=(BOOL *)calloc(3*brtacaka,sizeof(BOOL));
/*
 Tacka *t=(*kt)[2][0];
 while (t)
 {ind= 3* ( t->id - (*kt)[2][0]->id );
  for (i=0;i<3;i++)
      { nep[ind+i]=FALSE;
      }
  t=t->sused[2][1];
 }
 */
}

/*--------------------------Novi slozeni stap--------------------------------------*/
SlozeniStap * slozeni_stap_novi(short brd,float *ksi,short *pp)
{ br_sstp++;
  sstap=(SlozeniStap*)realloc(sstap,br_sstp*sizeof(SlozeniStap));
  slozeni_stap_izmena(sstap+br_sstp-1,brd,ksi,pp);
  return sstap+br_sstp-1;
}
void slozeni_stap_izmena(SlozeniStap *stap,short brd,float *ksi,short *pp)
{ stap->brd=brd;  stap->ksi=ksi;  stap->pp=pp;
}
/*------------------------------Novi stap------------------------------------*/
Stap * stap_novi(Duz *duz,BOOL konst,short pp,short mat,short gru[2],short tip,void * tata)
{/*br_stp++;*/ Stap *stap=(Stap*) malloc(sizeof(Stap));
  stap_izmena(stap,duz,konst,pp,mat,gru,tip,tata);
  if (br_dijag==0)stap->dij=NULL;//ovo znaci da nema jos nijednog stanja za dijagrame
  else {stap->dij=(RaspOpt **)calloc(br_dijag,sizeof(RaspOpt *));
        short i;for (i=0; i<br_dijag; i++){*(stap->dij+i)=NULL;}
       }//za svako stanje pokazivac na NULL= nema dodatog dijagrama
  stap->jos =NULL;
  return stap;
}
void stap_izmena(Stap *stap,Duz *duz,BOOL konst,short pp,short mat,short gru[2],short tip,void * tata)/*Unosi se rbr pocev od 0 !!*/
{stap->duz=duz;  duz->tata=stap;
 stap->ppkonst=konst;
 stap->pp =pp;   stap->mat=mat;
 stap->gru[0]=gru[0]; stap->gru[1]=gru[1];
 stap->tip=tip;  stap->tata=tata;
}
/*-------------------------Novi poprecni presek------------------------------------*/
PopPres * pop_pres_novi(float A,float I,float J)
{br_pp++;
 if (ppres==NULL) {uppp=0;//Prvi pp postaje uporedni pp
 	                 ppres=(PopPres*) malloc(      sizeof(PopPres));      }
 else              ppres=(PopPres*)realloc(ppres,sizeof(PopPres)*br_pp);
 pop_pres_izmena(A,I,J,br_pp-1);
 return ppres;
}
void pop_pres_izmena(float F,float I,float J,short rbr)/*Unosi se rbr pocev od 0 !!*/
{(ppres+rbr)->F=F;  (ppres+rbr)->I=I; (ppres+rbr)->J=J;}
/*--------------------------- Novi materijal --------------------------------------*/
Materijal * materijal_novi(float me,float ni,float alfa,float G)
{br_mat++;
 if (materijal==NULL){upp_mat=0;
 	                    materijal=(Materijal*) malloc(          sizeof(Materijal));       }
 else                 materijal=(Materijal*)realloc(materijal,sizeof(Materijal)*br_mat);
 materijal_izmena(me,ni,alfa,G,br_mat-1);
 return materijal;
}
void materijal_izmena(float me,float ni,float alfa,float G,short rbr)
{(materijal+rbr)->E=me;      (materijal+rbr)->ni=ni;
 (materijal+rbr)->alfa=alfa; (materijal+rbr)->G =G; }
/*--------------------------- Novi Gr.Uslovi --------------------------------------*/
GrUslovi * gruslovi_novi(BOOL pom[3],BOOL rot[3])
{br_gru++;
 if (gru==NULL)  gru=(GrUslovi*) malloc(    sizeof(GrUslovi));
 else            gru=(GrUslovi*)realloc(gru,sizeof(GrUslovi)*br_gru);
 gruslovi_izmena(pom,rot,br_gru-1);
 return gru;
}
void gruslovi_izmena(BOOL pom[3],BOOL rot[3],short rbr)//rbr pocev od 0
{(gru+rbr)->pom[0]=pom[0]; (gru+rbr)->pom[1]=pom[1]; (gru+rbr)->pom[2]=pom[2];
 (gru+rbr)->rot[0]=rot[0]; (gru+rbr)->rot[1]=rot[1]; (gru+rbr)->rot[2]=rot[2];
}
/*----------------------- novo stanje za dijagrame --------------------------------*/
void DodajNovoStanje(void)
{br_dijag++;
 Duz *d=(*kd)[2][1];
 if (((d->tata))->dij)//nije prvo alociranje
    {while (d)
     {((d->tata))->dij=(RaspOpt **)realloc(
     ((d->tata))->dij,br_dijag*sizeof(RaspOpt *));
      *(((d->tata))->dij+br_dijag-1)=NULL;
   	 d=d->sused[2][0];
     }
    }
 else/*prvo alociranje- prvo(M0) stanje*/
    while (d)
    {//char s[20];sprintf(s,"Stap:%c%c",id_i(d),id_k(d)),ST_helpMsg(s);ngetchx();
     ((d->tata))->dij=(RaspOpt **)calloc(1,sizeof(RaspOpt *));
      *(((d->tata))->dij)=NULL;
   	 d=d->sused[2][0];
    }
}
/*--------------------------- Raspodeljeno opterecenje -------------------------------*/
RaspOpt * rasp_opt_novo (float q[3],float alfa,float N)
{RaspOpt * ro=(RaspOpt *)calloc(1,sizeof(RaspOpt));
 rasp_opt_izmena(ro,q,alfa,N);
 return	ro;
}
void rasp_opt_izmena(RaspOpt *ro,float q[3],float alfa,float N)
{*(ro->q   )=q[0];
 *(ro->q +1)=q[1];
 *(ro->q +2)=q[2];
   ro->alfa =alfa;
   ro->N    =N;
}
/*------------------ maksimalna vrednost dijagrama za zadato stanje ------------------*/
float maks_opt_za_stanje(short stanje)
{//ova funkcija nam je potrebna da bi odredili razmeru za crtanje dijagrama
 Duz *d=(*kd)[0][0];
 float *q=NULL;

 float usredini,maks=0;//ovo ce biti maksimalna apsolutna vrednost opterecenja za zadato stanje i za sve stapove
 while(d)
 {if (stanje>=0) q=Dij_(d,stanje)->q;
  else           q=((RaspOpt *)(d->tata->jos))->q;

  if (maks < abs(q[0]) ) maks=q[0];
 	if (maks < abs(q[1]) ) maks=q[1];
 	usredini = abs(q[0] + q[1])/2 + q[2];
 	if (maks < usredini)   maks=usredini;
 	d=d->sused[0][1];
 }
 return maks;
}
/*--------------pretvori raspodeljeno opterecenje u konc.sile na krajevima------------*/
void M_u_konc(Duz *d, short stanje, float *konc)
{if (d->tata->dij[stanje])
 {float *q= d->tata->dij[stanje]->q;
  float Lred=( (ppres+          uppp) ->I
             /((ppres+((d->tata)->pp))->I)) * l_d(d);

 	konc[0] = fdiv(Lred,6) * ( 2.0 * q[0] + q[1] );
 	konc[1] = fdiv(Lred,6) * ( 2.0 * q[1] + q[0] );
 	konc[0]+= Lred * fdiv(q[2],12);
 	konc[1]+= Lred * fdiv(q[2],12);
 }
 else
 {konc[0]=konc[1]=0;
 }
}
/*-------------------Iniciraj osnovne tipove oslonaca---------------------------------*/
GrUslovi *InicirajGrUslove(void)
{/*OPTERECENJE DELUJE U RAVNI NOSACA-bitni su pom[0],pom[1],rot[0]*/
 GrUslovi *gruslovi; //ST_helpMsg("*gruslovi");ngetchx();
 gruslovi=gruslovi_novi((BOOL[3]){FALSE,FALSE,FALSE},(BOOL[3]){FALSE,FALSE,FALSE});/*0:slobodan kraj*/
 gruslovi=gruslovi_novi((BOOL[3]){TRUE,  TRUE,FALSE},(BOOL[3]){FALSE,FALSE,TRUE}); /*1:ukljestenje*///ST_helpMsg("ukljestenje");ngetchx();
 gruslovi=gruslovi_novi((BOOL[3]){TRUE,  TRUE,FALSE},(BOOL[3]){FALSE,FALSE,FALSE});/*2:nepokr.zglobni oslonac*/
 gruslovi=gruslovi_novi((BOOL[3]){FALSE, TRUE,FALSE},(BOOL[3]){FALSE,FALSE,FALSE});/*3:pokret.zglobni oslonac*/
 gruslovi=gruslovi_novi((BOOL[3]){TRUE, FALSE,FALSE},(BOOL[3]){FALSE,FALSE,FALSE});/*4:prost stap*/
 return gruslovi;
}
short * TipStapaUGrUslove(short tip,short *gru)
{switch (tip)
 {case     K_STAP: gru[0]=KRUTI_UGAO;  gru[1]=KRUTI_UGAO; break;
 	case    oG_STAP: gru[0]=ZGLOB;       gru[1]=KRUTI_UGAO; break;
 	case    Go_STAP: gru[0]=KRUTI_UGAO;  gru[1]=ZGLOB;       break;
 	case PROST_STAP: gru[0]=ZGLOB;       gru[1]=ZGLOB;       break;
 	case     S_STAP: gru[0]=KRUTI_UGAO;  gru[1]=S_VEZA;      break;
 }
return gru;
}
BOOL MogucePomeranjeVeze(short gru,BOOL translacija,short osa)
/*osa je u L.K.S. stapa i moze biti 1,2 ili 3 (x,y,z)*/
{switch(gru)/*dozvoljena pomeranja/obrtanja*/
 {case SLOB_KRAJ:  return TRUE;  break;
  case ZGLOB:
   if ( (!(translacija)) && (osa!=1) ) return TRUE;//rotacija je uvek sprecena oko x-ose
	break;
 	case S_VEZA:
   if ( translacija && (osa==2) ) return TRUE;
 	break;
 }
return FALSE;/*Ostali su samo slucajevi sprecenih pomeranja/rotacija*/
}
/*--------------------------------------------------------------------------------------------*/
/*                          Podela stapova                                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
void stap_podeli_u_prestac(NizTacaka *niz)
{NizTacaka *pniz=niz;
 Duz *d=(*kd)[2][1];
 Tacka_stv pt;
 //char s[40];sprintf(s,"podela duzi:");ST_helpMsg(s);ngetchx();
 while (d!=NULL)//za svaku duz
   { //char s[40];sprintf(s,"podela duzi: %c(%c-%c)",d->id,id_i(d),id_k(d));ST_helpMsg(s);ngetchx();
    pniz=niz;
    while (pniz!=NULL)//i svaku tacku iz zadatog niza presecnih tacaka
          {pt.x=pniz->t->xy[0]; pt.y=pniz->t->xy[1];
           //char s[40];sprintf(s,"pres.tacka:(%4f,%4f)",pniz->t->xy[0],pniz->t->xy[1]);ST_helpMsg(s);ngetchx();
           if (  ( ! t_je_teme_d(t_u_T_stv(pniz->t), d))
               &&(t_pripada_d(pt, d)))
                  {//char s[40];sprintf(s,"delim duz 2:%c%c  ",id_i(d),id_k(d));ST_helpMsg(s);ngetchx();
                   stap_podeli_u_t(d,pniz->t);//dodajem deo -k-
             	     d=(*kd)[2][1];//Ovo mora nakon svake podele, jer i podeljene duzi trebaju da se provere
                  }
 	         pniz=pniz->sused[1];
          }
    d=d->sused[2][0];
	 }
 pres_tacke_osl_mem(niz);
}

Duz * stap_podeli_u_t(Duz *d,Tacka *t)
{//Ova funkcija NE proverava da li tacka stvarno pripada zadatoj duzi
 //Prvo pravimo novu duz: desni, "k deo" podeljene duzi
 Duz * deo_k=duz_dodaj((*kd)[2][1]->id+1, (Tacka *[2]){t,d->ik[1]});
 /*Zasad ova funkcija ne radi za presek sa stapom promenljivog poprecnog preseka*/
 stap_novi(deo_k,TRUE,((d->tata))->pp,((d->tata))->mat,((d->tata))->gru,((d->tata))->tip,((d->tata))->tata);//nasledjivanje karakteristika stapa
 Tacka *ti=d->ik[0]; //Pamtimo podatke o prvoj, levoj, i tacki duzi koju delimo
 char id=d->id; //id stare duzi ce da preuzme levi, "i deo" podeljene duzi
 duz_izbrisi(d); //Zatim brisemo duz koja se deli na dve
 //dodajemo duz, "i deo"
 Duz * deo_i=duz_dodaj(id, (Tacka *[2]){ti,t});// na ovaj nacin ce delovi duzi da prodju kroz sva sortiranja i provere
 stap_novi(deo_i,TRUE,((deo_k->tata))->pp,((deo_k->tata))->mat,((deo_k->tata))->gru,((d->tata))->tip,((d->tata))->tata);//nasledjivanje karakteristika stapa
 /*deo_i preuzima karakteristike od deo_k, zato sto on ima karakteristike pocetnog stapa d*/
 return deo_i; //desni deo, "k", se nalazi preko najveceg id-a (*kd)[2][1]
 //FontSetSys(F_4x6);printf_xy(5,20,"podelio sam duz.     ");ngetchx();
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
//--------------------------------------------------------------------------------
/*
void stapove_stampaj(void)
{Duz *d=(*kd)[2][1];
 clrscr();FontSetSys(F_6x8);
 //printf("id:   E   :   v  :   I   :\n");
 while ((d!=NULL)&&(d->tata!=NULL))
 {printf("%c%c:E=%7r :v=%6f  \n%d%d:I=%7r :F=%6f\n",
          id_i(d),id_k(d),E_(d),ni_(d),
          ((d->tata))->gru[0],((d->tata))->gru[1], I_(d),  F_(d));ngetchx();
 	d=d->sused[2][0];
 }
 if ((d!=NULL)&&(d->tata==NULL))
    {char s[25];sprintf(s,"Duzi %c%c nije dodeljen stap!",id_i(d),id_k(d));
     ST_helpMsg(s);ngetchx();}
}
*/
/*----------------Oslob.memoriju ppres,materijal,gru,greda--------------------------*/
void Oslobodi_memoriju_greda(void)
{if (ppres    ){free(ppres);
                     ppres=NULL;}//ST_helpMsg("Oslobodjena memorija pop.pres.");       ngetchx(); }
 if (materijal){free(materijal);
                     materijal=NULL;}//ST_helpMsg("Oslobodjena memorija materijala");      ngetchx(); }
 if (gru      ){free(gru);
                     gru=NULL;}//ST_helpMsg("Oslobodjena memorija granicnih uslova");ngetchx(); }
                                //ST_helpMsg("Oslobadjanje memorije stapova i duzi:....");
 //grede brisem zajedno sa duzima, koje sa sobom vuku i opterecenja
 Duz * d=(*kd)[0][0];  Duz *dbris;
 while (d!=NULL)
  {//char s[20];sprintf(s,"brisanje duzi %c%c",id_i(d),id_k(d));ST_helpMsg(s);ngetchx();
   dbris=d;//pamtimo adresu za brisanje                       //{char s[40];sprintf(s,"duz:%c%c->dij &=%lp",id_i(d),id_k(d),((d->tata))->dij);ST_helpMsg(s);ngetchx();}
   if (((d->tata))->dij)/*ovaj pointer postoji za svaki stap ako su unoseni dijagrami za bilo koji stap*/
      {short i;
       for (i=0;i<br_dijag;i++)
            if (*(((d->tata))->dij+i))/*ovaj pointer je NULL ako nije unosen dijagram za ovaj stap i ovo stanje*/
               {free(*(((d->tata))->dij+i));	          //{char s[40];sprintf(s,"duz:%c%c, dij:%d izbrisan",id_i(d),id_k(d),i);ST_helpMsg(s);ngetchx();}
               }//brisemo dijagrame vezane za stap
       //Oslobodi memoriju niza pokazivaca na gore oslobodjene dijagrame
       free(((d->tata))->dij);  //{char s[40];sprintf(s,"duz:%c%c,stap->dij brisem...,%lu bajtova",id_i(d),id_k(d),sizeof(((d->tata))->dij));ST_helpMsg(s);ngetchx();}
      }
   //{char s[40];sprintf(s,"duz:%c%c,stap brisem...,%lu bajtova",id_i(d),id_k(d),sizeof((d->tata)));ST_helpMsg(s);ngetchx();}
   free(((d->tata)));//brisem stap vezan za ovu duz
   //{char s[40];sprintf(s,"duz:%c%c brisem...,%lu bajtova",id_i(d),id_k(d),sizeof(*d));ST_helpMsg(s);ngetchx();}//stampanje za debug
   d=d->sused[0][1];//sledeca duz na spisku
   //{char s[40];sprintf(s,"duz:%c%c brisem:%lp,%lu bajtova",id_i(dbris),id_k(dbris),dbris,sizeof(*dbris));ST_helpMsg(s);ngetchx();}//stampanje za debug
   free(dbris);//brisemo aktuelnu duz
  }
 //ST_helpMsg("gotovo brisanje duzi.");ngetchx();
 /*slozeni stapovi*/
 unsigned i;
 for (i=0;i<br_sstp;i++)
     {if ((sstap+i)->ksi)free((sstap+i)->ksi);
      if ((sstap+i)->pp )free((sstap+i)->pp );
     }
 if (sstap)free(sstap);
}
/*----------Oslobadjanje kompletne zauzete memorije----------------------*///nije bas, ima jos ponesto iz main_: d0,d0t
void Oslobodi_memoriju_nosaca(void)
{
 Oslobodi_memoriju_greda(); //ST_helpMsg("Oslobodjena memorija greda");ngetchx();
 Oslobodi_memoriju_tacaka();//ST_helpMsg("Oslobodjena memorija duzi i tacaka");ngetchx();

}

/*---------------Sacuvaj stapove-----------------------------------------*/
void sacuvaj_materijale(FILE * fajl)
{unsigned i;
  fwrite(&(br_mat)       ,sizeof(unsigned short),1,fajl); //broj materijala
  for (i=0;i<br_mat;i++)
	{fwrite(&((materijal+i)->E),   sizeof(float),1,fajl);
	 fwrite(&((materijal+i)->ni),  sizeof(float),1,fajl);
	 fwrite(&((materijal+i)->alfa),sizeof(float),1,fajl);
	 fwrite(&((materijal+i)->G),   sizeof(float),1,fajl);
	}
}/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void sacuvaj_pop_preseke(FILE * fajl)
{unsigned i;
  /*konstantni poprecni preseci*/
  fwrite(&(br_pp)       ,sizeof(unsigned short),1,fajl); //broj materijala
  for (i=0;i<br_pp;i++)
	{fwrite(&((ppres+i)->F),sizeof(float),1,fajl);
	 fwrite(&((ppres+i)->I),sizeof(float),1,fajl);
	 fwrite(&((ppres+i)->J),sizeof(float),1,fajl);
	 //fwrite(&((ppres+i)->h),   sizeof(float),1,fajl);
	}
	/*promenljivi poprecni preseci*/
	fwrite(&(br_sstp)       ,sizeof(unsigned short),1,fajl); //broj materijala
  for (i=0;i<br_sstp;i++)
	{fwrite(&((sstap+i)->brd),sizeof(short),1,fajl);
	 fwrite(  (sstap+i)->ksi ,sizeof(float),((sstap+i)->brd)-1,fajl);
	 fwrite(  (sstap+i)->pp  ,sizeof(short), (sstap+i)->brd   ,fajl);
	}
}/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void sacuvaj_duzi_i_stapove(FILE * fajl)
{ Duz *d=(*kd)[0][0];
  while (d!=NULL)
	{/*Duzi:*/
	 fwrite(&(d->id)       ,sizeof(char),1,fajl);
	 fwrite(&(d->ik[0]->id),sizeof(char),1,fajl); //tacka i
	 fwrite(&(d->ik[1]->id),sizeof(char),1,fajl); //tacka k
	 /*Stapovi:*/
	 fwrite(&(((d->tata))->mat)    ,sizeof(short),1,fajl); //materijal
	 fwrite(&(((d->tata))->ppkonst),sizeof(BOOL ),1,fajl); //?konstantan poprecni presek?
	 fwrite(&(((d->tata))->pp)     ,sizeof(short),1,fajl); //poprecni presek
	 //fwrite(&(((d->tata))->gru)      //granicni uslovi
	 fwrite(&(((d->tata))->tip),sizeof(short),1,fajl); //tip stapa

	 RaspOpt *ro;
	 short stanje;
	 BOOL ima=TRUE,nema=FALSE;
	 for (stanje=0;stanje<br_dijag;stanje++)
	     {ro=*( (((d->tata))->dij) +stanje );
	      if (ro)//postoji neki dijagram za ovo stanje
	      {fwrite(&(ima),sizeof(BOOL)   ,1,fajl);
         fwrite(ro,    sizeof(RaspOpt),1,fajl);
	      }
	      else//nema dij.za ovo stanje:upisujem samo FALSE
	       fwrite(&(nema),sizeof(BOOL),1,fajl);
       }

   d=d->sused[0][1];
	}
}/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void Sacuvaj_Nosac(FILE * fajl, short ver[])
{ //short ver[]={COMMENT_VERSION_NUMBER};

  fwrite(ver,sizeof(short),4,fajl);
  sacuvaj_materijale (fajl);
	sacuvaj_pop_preseke(fajl);
	short s;
	s=tacaka_ukupno(); fwrite(&s,sizeof(short),1,fajl);//printf("\ntacaka=%d\n",s);ngetchx();
  sacuvaj_tacke(fajl);
  s=duzi_ukupno();   fwrite(&s,sizeof(short),1,fajl);//printf("duzi=%d\n",s);ngetchx();
  fwrite(&br_dijag,sizeof(short),1,fajl);
	sacuvaj_duzi_i_stapove(fajl);
  /* dodavanje ekstenzije .gri */
	fputc(0,fajl); fputs("gri",  fajl);
  fputc(0,fajl); fputc(OTH_TAG,fajl);
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/*PROCITAJ . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
void Procitaj_Nosac(FILE *fajl,short minver[])
{//ovde samo citamo, pretpostavljam cisto stanje, tj. kao Novi_ fajl
  short i,verfajla[4];//,        minver[]={MIN_VER_FAJLA};
  for (i=0;i<4;i++)fread(verfajla+i,sizeof(short),1,fajl);
  for (i=0;i<4;i++)
      {if (verfajla[i]<minver[i])
          {DlgMessage ("GRESKA",
                       "Verzija fajla ne odgovara verziji programa",
                        BT_OK, BT_NONE);
           return;
          }
       else {if (verfajla[i]>minver[i])
                i=4;//izlazimo iz dalje provere jer je verzija veca
            }
      }
  procitaj_materijale(fajl);  procitaj_pop_preseke(fajl);
  short ntac;  fread(&ntac,sizeof(short),1,fajl);
  procitaj_tacke(ntac,fajl);
  procitaj_duzi_i_stapove(fajl);
}
void procitaj_materijale(FILE *fajl)
{ /*Podrazumeva se cisto stanje(Novi_)*/
  float E,ni,alfa,G;
  short nmat,i=0;
  fread(&nmat,sizeof(short),1,fajl);
  for(i=0;i<nmat;i++)
  {fread(&E   ,sizeof(float),1,fajl);
   fread(&ni  ,sizeof(float),1,fajl);
   fread(&alfa,sizeof(float),1,fajl);
   fread(&G   ,sizeof(float),1,fajl);
   materijal_novi(E,ni,alfa,G);//printf("dodao materijal\n");ngetchx();
  }
}
void procitaj_pop_preseke(FILE *fajl)
{//Podrazumeva se cisto stanje(Novi_)
 {/*konstantan poprecni presek*/
  short i=0,npp;
  float F,I,J;
  fread(&npp,sizeof(short),1,fajl);
  for(i=0;i<npp;i++)
  {fread(&F ,sizeof(float),1,fajl);
   fread(&I ,sizeof(float),1,fajl);
   fread(&J ,sizeof(float),1,fajl);
   pop_pres_novi(F,I,J);//printf("dodao materijal\n");ngetchx();
  }
 }
 {/*promenljivi poprecni preseci*/
  short i,brs,brd,*pp;
  float *ksi;
  fread(&(brs),sizeof(short),1,fajl); //broj slozenih stapova
  for (i=0;i<brs;i++)
	{fread(&(brd),sizeof(short),1,fajl);
	 ksi=(float*)calloc(brd-1,sizeof(float));
	 fread(ksi,sizeof(float),brd-1,fajl);
	 pp =(short*)calloc(brd  ,sizeof(short));
	 fread(pp ,sizeof(short),brd  ,fajl);
	 slozeni_stap_novi(brd,ksi,pp);
	}
 }
}
void procitaj_duzi_i_stapove(FILE *fajl)
{ /*Podrazumeva se cisto stanje(Novi_) sto se tice duzi,
    kao i da su tacke duzi vec ucitane                   */
  char id;//pomocna za citanje id tacaka ili duzi
  BOOL konst;
  short mat,pp,tip,gru[2]={0,0},
        i=0,nd=0;
  fread(&nd      ,sizeof(short),1,fajl);
  fread(&br_dijag,sizeof(short),1,fajl);
  Duz d,*ucitanaduz;
  for(i=0;i<nd;i++)
  {fread(&id,sizeof(char),1,fajl);  d.id=id;
   fread(&id,sizeof(char),1,fajl);  d.ik[0]=tacku_nadji_po_id(id);
   fread(&id,sizeof(char),1,fajl);  d.ik[1]=tacku_nadji_po_id(id);
   if ((d.ik[0]!=NULL)&&(d.ik[1]!=NULL))
      {ucitanaduz=duz_dodaj(d.id, d.ik);
       fread(&mat  ,sizeof(short),1,fajl);
       fread(&konst,sizeof(BOOL ),1,fajl);
       fread(&pp   ,sizeof(short),1,fajl);
//    fread(&gru[0] ,sizeof(short),1,fajl);
//    fread(&gru[1] ,sizeof(short),1,fajl);
       fread(&tip,sizeof(short),1,fajl);
       TipStapaUGrUslove(tip,gru);
       stap_novi(ucitanaduz,konst,pp,mat,gru,tip,NULL);

       short stanje;
       float pi,pk,f,alfa,N;
       BOOL imadijagrama;
       for (stanje=0;stanje<br_dijag;stanje++)
         {fread(&imadijagrama,sizeof(BOOL),1,fajl);
          if (imadijagrama)
             {fread(&pi  ,sizeof(float),1,fajl);  fread(&pk  ,sizeof(float),1,fajl);
              fread(&f   ,sizeof(float),1,fajl);  fread(&alfa,sizeof(float),1,fajl);
              fread(&N   ,sizeof(float),1,fajl);
              *((ucitanaduz->tata)->dij+stanje)=rasp_opt_novo((float [3]){pi,pk,f},alfa,N);
  	          //rasp_opt_izmena(Dij_(d,stanje),(float [3]){pi,pk,f},alfa);
             }
         }
      }
   //else //mozda neka poruka o losem citanju ovde   //printf("duz=%d  id=%c\n",r,d->id);ngetchx();
  }
}/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-   TIP VERESCAGIN   -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
Veresc * novi_Veresc(void)
{//ST_helpMsg("novi Veresc");ngetchx();
Veresc *v=(Veresc*)calloc(1,sizeof(Veresc));
 v->ukupno=0;
 v->racun=(char **)calloc(5,sizeof(char*));
   v->racun[0]=NULL; v->racun[1]=NULL;
   v->racun[2]=NULL; v->racun[3]=NULL;
   v->racun[3]=NULL;
return v;}
//-------------------------------------------------------
void brisi_baf_Veresc(Duz *d)
{//ST_helpMsg("Brisem Verescagina...");ngetchx();
 short i=0;
 for (i=0;i<5;i++)
     {if  ( *( ((Veresc *)Jos_(d))->racun +i) )
      {//{char s[40];sprintf(s,"Osl.mem.Veresc.:stap %c%c:stanje:%d...",id_i(d),id_k(d),i+1);ST_helpMsg(s);ngetchx();}  //{char s[70];sprintf(s,"...%lp:%s",( ((Veresc *)Jos_(d))->racun +i),*( ((Veresc *)Jos_(d))->racun +i));ST_helpMsg(s);ngetchx();}
       free(*( ((Veresc *)Jos_(d))->racun +i));  //*( ((Veresc *)Jos_(d))->racun +i)=NULL;   //ovo je za debug
      }
     }//Oslobadjanje teksta za 4 kombinacije u mnozenju dijagrama*/
 if (((Veresc *)Jos_(d))->racun) free (((Veresc *)Jos_(d))->racun);
 if (((Veresc *)Jos_(d)))        free  ((Veresc *)Jos_(d));//oslobadjanje memorije za promenljivu tipa Veresc
 ((d->tata))->jos=NULL;
}//--------------------------------------------------------
void brisi_sve_Veresc(void)
{/*Oslobadjanje memorije zauzete Verescaginom*/
 Duz *d=(*kd)[0][0];
 while (d)
  {brisi_baf_Veresc(d);
   d=d->sused[0][1];
  }
}//---------------------------------------------------------
short Veresc_Tip(float q[2])
{if ((q[0]!=0)&&(q[1]!=0))
    {if (jednaki(q[0],q[1]))return PRAVOUGAONIK; //pravougaonik
     else                 	return TRAPEZ; //trapez
    }
 if ((q[0]==0)&&(q[1]!=0)) return TROUGAO_K; //trougao sa k stranom
 if ((q[0]!=0)&&(q[1]==0)) return TROUGAO_I; //trougao sa i stranom
 return 0; //nema linearnog dijagrama   }
}//---------------------------------------------------------

float Prr_Veresc(Duz *d,short stanje1,short stanje2,BOOL txtprr)
{//ST_helpMsg("Prr_Veresc");ngetchx();
 //brisi_baf_Veresc(d);//brisemo prethodni bafer
 if (((Veresc *)(Jos_(d)))==NULL)
      {//ST_helpMsg("Veresc je =NULL");ngetchx();
       (((d->tata))->jos)=(void*)(novi_Veresc());//Napravi ako ne postoji
      } //ST_helpMsg("Veresc");ngetchx();
 Veresc *v=((Veresc *)(Jos_(d))); //ST_helpMsg("Veresc OK");ngetchx();
 //Stap *stp=((d->tata));//ovo je za debug
 short Lbafera=200;
 float q1[3]={0,0,0},q2[3]={0,0,0},N[2]={0,0},
       ukupno=0,qxq=0,
       Lred=0,LredN=0;
 //if (isNan(Lred))Lred=0;
if ((ppres+((d->tata))->pp)->F)
    LredN=((ppres+uppp)->I/(ppres+((d->tata))->pp)->F)*l_d(d);// prost stap => N za verescagina
if ((ppres+((d->tata))->pp)->I)
    Lred =((ppres+uppp)->I/(ppres+((d->tata))->pp)->I)*l_d(d);// M u Verescaginu

 if (Dij_(d,stanje1))
 {q1[0]=(Dij_(d,stanje1))->q[0];
  q1[1]=(Dij_(d,stanje1))->q[1];
  q1[2]=(Dij_(d,stanje1))->q[2];
   N[0]=(Dij_(d,stanje1))->N;
 }
 if (Dij_(d,stanje2))
 {q2[0]=(Dij_(d,stanje2))->q[0];
  q2[1]=(Dij_(d,stanje2))->q[1];
  q2[2]=(Dij_(d,stanje2))->q[2];
   N[1]=(Dij_(d,stanje2))->N;
 }
 //{char s[30];sprintf(s,"q1={%f,%f,%f}",q1[0],q1[1],q1[2]);ST_helpMsg(s);ngetchx();}
 //{char s[30];sprintf(s,"q2={%f,%f,%f}",q2[0],q2[1],q2[2]);ST_helpMsg(s);ngetchx();}
 //ST_helpMsg("lin X lin");ngetchx();
 /*linearno x linearno*//*-----------------------------------------------------------*/
 /*                         PAZNJA NA BROJ ZNAKOVA!!!!!   */
 qxq=Lred*(q1[0]*(2*q2[0]+q2[1])+q1[1]*(q2[0]+2*q2[1]))/6; //if(qxq!=0){char s[30];sprintf(s,"Stap:%c%c, lin x lin, qxq=%f",id_i(d),id_k(d),qxq);ST_helpMsg(s);ngetchx();}
 if ((qxq!=0)&&(txtprr))
 { //ST_helpMsg("calloc...");ngetchx();
 	*(v->racun)=(char*)calloc(Lbafera,sizeof(char));//ST_helpMsg("calloc gotov");ngetchx();
 	switch (Veresc_Tip(q1))
  {case PRAVOUGAONIK:switch (Veresc_Tip(q2))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun), "%|^4.4g %c %|^4.4g %c %|^4.4g",
                              Lred, 215, q1[0],215, q2[0]  );
    break;
   	case TRAPEZ:
      sprintf(*(v->racun),
      "1/2 %c %|^4.4g %c \n       %|^4.4g %c ( %|^4.4g + %|^4.4g )",
          215, Lred, 215,            q1[0],215,  q2[0],   q2[1]     );
    break;
   	case TROUGAO_K:
   	  sprintf(*(v->racun),"1/2 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q1[0],215, q2[1]   );
    break;
    case TROUGAO_I:
  	  sprintf(*(v->racun),"1/2 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q1[0],215, q2[0]   );

    break;
   }
   break;//---------------------------
   case TRAPEZ:switch (Veresc_Tip(q2))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun),
      "1/2 %c %|^4.4g %c \n       %|^4.4g %c ( 2 %c %|^4.4g + %|^4.4g )",
          215, Lred, 215,            q2[0],215,  215, q1[0],   q1[1]     );
    break;
    case TRAPEZ:/*trapezXtrapez*///ST_helpMsg("trapez x trapez");ngetchx();
      if ( (jednaki(q1[0],q2[0])) && (jednaki(q1[1],q2[1])) )/*trapez na kvadrat*/
        sprintf(*(v->racun),
        "1/3 %c %|^4.4g %c \n       ( %|^4.4g%c + %|^4.4g %c %|^4.4g + %|^4.4g%c )",
             215, Lred, 215,           q1[0],178,  q1[0], 215,q1[1],    q1[1],178);
      else /*proizvod razlicitih trapeza*/
        sprintf(*(v->racun),
        "1/6 %c %|^4.4g %c \n       [ %|^4.4g %c ( 2 %c %|^4.4g + %|^4.4g )\n        +%|^4.4g %c ( %|^4.4g + 2 %c %|^4.4g )]",
            215, Lred, 215,            q1[0],215,   215, q2[0],   q2[1],               q1[1], 215,  q2[0],     215, q2[1] );
   break;
    case TROUGAO_K:/*trapezXtrougao sa k*///ST_helpMsg("trapez x trougao k");ngetchx();
      sprintf(*(v->racun),"1/6 %c% |^4.4g %c %|^4.4g %c ( 2 %c %|^4.4g + %|^4.4g)",
                                          215, Lred, 215, q2[1],215,   215, q1[0],  q1[1] );
    break;
    case TROUGAO_I:/*trapezXtrougao sa i*///ST_helpMsg("trapez x trougao i");ngetchx();
      sprintf(*(v->racun),"1/6 %c %|^4.4g %c %|^4.4g %c( 2 %c %|^4.4g + %|^4.4g)",
                                         215, Lred, 215,  q2[0],215,  215,q1[1],   q1[0] );
    break;
   }
   break;//------------------------------
   case TROUGAO_K:switch (Veresc_Tip(q2))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun),"1/2 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q2[0],215, q1[1]   );
    break;
    case TRAPEZ:/*trougao sa k X trapez*///ST_helpMsg("trougao k x trapez");ngetchx();
      sprintf(*(v->racun),"1/6 %c %|^4.4g %c %|^4.4g %c( 2 %c %|^4.4g + %|^4.4g)",
                                          215, Lred, 215, q1[1],215,  215, q2[1],  q2[0]);
    break;
    case TROUGAO_K:/*trougao sa k X trougao sa k*///ST_helpMsg("trougao k x trougao k");ngetchx();
      sprintf(*(v->racun),"1/3 %c %|^4.4g %c %|^4.4g %c %|^4.4g",
                                         215, Lred, 215, q1[1], 215, q2[1]);
    break;
    case TROUGAO_I:/*trougao sa k X trougao sa i*///ST_helpMsg("trougao k x trougao k");ngetchx();
      sprintf(*(v->racun),"1/6 %c %|^4.4g %c %|^4.4g %c %|^4.4g",
                                          215, Lred, 215, q1[1],215, q1[0]);
    break;
   }
   break;//------------------------------
   case TROUGAO_I:switch (Veresc_Tip(q2))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun),"1/2 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q2[0],215, q1[0]   );
    break;
   	case TRAPEZ:/*trougao sa i X trapez*///ST_helpMsg("trougao i x trapez");ngetchx();
      sprintf(*(v->racun),"1/6 %c %|^4.4g %c %|^4.4g %c( 2 %c %|^4.4g + %|^4.4g)",
                               215, Lred, 215,q1[0],215,  215,  q2[0],  q2[1]);
    break;
    case TROUGAO_K:/*trougao sa i X trougao sa k*///ST_helpMsg("trougao i x trougao k");ngetchx();
      sprintf(*(v->racun),"1/6 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                              215, Lred, 215, q1[0],215, q2[1]);
    break;
    case TROUGAO_I:/*trougao sa i X trougao sa i*///ST_helpMsg("trougao i x trougao i");ngetchx();
      sprintf(*(v->racun),"1/3 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                              215, Lred, 215,q1[0], 215, q2[0]);
    break;
   }
   break;
  }//switch Veresc_Tip(q1)
  //ST_helpMsg(*(v->racun));ngetchx();
  //ST_helpMsg("realloc:"); ngetchx();
 *(v->racun)=realloc(*(v->racun),(strlen(*(v->racun))+1)*sizeof(char)); //  ST_helpMsg("Uspesan realloc");ngetchx();
 }/*------------------------------------------------------------------------------*/
 ukupno+=qxq; //printf("%s\n",*(v->racun));//ST_helpMsg(*(v->racun)); ngetchx();
 /*--------------------------------------------------------------------------------*/
 /*parabola x linearno*///--------------------------------------------
 qxq=Lred*q1[2]*(q2[0]+q2[1])/3; //if (qxq!=0){char s[30];sprintf(s,"Stap:%c%c, par x lin, qxq=%f",id_i(d),id_k(d),qxq);ST_helpMsg(s);ngetchx();}
 if ((qxq!=0)&&(txtprr))
 {*(v->racun+1)=(char*)calloc(Lbafera,sizeof(char));//pp max 45 znakova
  switch(Veresc_Tip(q2))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun),"2/3 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q1[2],215, q2[1]   );
    break;
   	case TRAPEZ:/*parabola*trapez*///ST_helpMsg("parabola x trapez");ngetchx();
      sprintf(*(v->racun+1),"1/3 %c %-^4.4g %c %-^4.4g %c (%-^4.4g + %-^4.4g",
                                215, Lred,  215, q1[2],215,  q2[0],   q2[1]);
  	break;
  	case TROUGAO_K:/*parabola*trougao sa k*///ST_helpMsg("parabola x trougao k");ngetchx();
      sprintf(*(v->racun+1),"1/3 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                                 215, Lred, 215, q1[2],215, q2[1]);
  	break;
  	case TROUGAO_I:/*parabola*trougao sa i*///ST_helpMsg("parabola x trougao i");ngetchx();
  	  sprintf(*(v->racun+1),"1/3 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                                 215, Lred, 215, q1[2],215, q2[0]);
  	break;
   } //-------------------------------------------------------------------
 }
  ukupno+=qxq;
 /*linearno x parabola*///--------------------------------------------
 qxq=Lred*q2[2]*(q1[0]+q1[1])/3; // if (qxq!=0){char s[30];sprintf(s,"Stap:%c%c, lin x par, qxq=%f",id_i(d),id_k(d),qxq);ST_helpMsg(s);ngetchx();}
 if ((qxq!=0)&&(txtprr))
 {*(v->racun+2)=(char*)calloc(Lbafera,sizeof(char));//pp max 45 znakova
  switch(Veresc_Tip(q1))
   {case PRAVOUGAONIK:
      sprintf(*(v->racun),"2/3 %c %|^4.4g %c  %|^4.4g %c %|^4.4g ",
                               215, Lred, 215,  q2[2],215, q1[1]   );
    break;
   	case TRAPEZ:/*parabola*trapez*///ST_helpMsg("parabola x trapez");ngetchx();
      sprintf(*(v->racun+2),"1/3 %c %-^4.4g %c %-^4.4g %c (%-^4.4g + %-^4.4g)",
                                 215, Lred, 215, q2[2],215,  q1[0],   q1[1]);
  	break;
  	case TROUGAO_K:/*parabola*trougao sa k*///ST_helpMsg("parabola x trougao k");ngetchx();
      sprintf(*(v->racun+2),"1/3 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                                 215, Lred, 215, q2[2],215, q1[1]);
  	break;
  	case TROUGAO_I:/*parabola*trougao sa i*///ST_helpMsg("parabola x trougao i");ngetchx();
      sprintf(*(v->racun+2),"1/3 %c %-^4.4g %c %-^4.4g %c %-^4.4g",
                                 215, Lred, 215, q2[2],215, q1[0]);
  	break;
   }
  *(v->racun+2)=realloc(*(v->racun+2),(strlen(*(v->racun+2))+1)*sizeof(char));  // ST_helpMsg("Uspesan realloc");ngetchx();
 }
 ukupno+=qxq;
 /*--------------------------------------------------------------------------------*/
 /*parabola x parabola*/
 qxq=Lred*8*q1[2]*q2[2]/15; //{char s[30];sprintf(s,"Stap:%c%c, par x par, qxq=%f",id_i(d),id_k(d),qxq);ST_helpMsg(s);ngetchx();}
 if ((qxq!=0)&&(txtprr))
 {*(v->racun+3)=(char*)calloc(Lbafera,sizeof(char));//pp max 45 znakova
  sprintf(*(v->racun+3),"8/15 %c %-^4.4g %c %-^4.4g",
                             215, q1[2], 215, q2[2]);
  *(v->racun+3)=realloc(*(v->racun+3),(strlen(*(v->racun+3))+1)*sizeof(char));  // ST_helpMsg("Uspesan realloc");ngetchx();
 }
 ukupno+=qxq;
  //---------------------------------------------------------------------------
 /*Mnozenje dijagrama N*/
 qxq=LredN*N[0]*N[1]; //{char s[30];sprintf(s,"Stap:%c%c, par x par, qxq=%f",id_i(d),id_k(d),qxq);ST_helpMsg(s);ngetchx();}
 if ((qxq!=0)&&(txtprr))
 {*(v->racun+4)=(char*)calloc(Lbafera,sizeof(char));//pp max 45 znakova
  sprintf(*(v->racun+4),"%-^4.4g %c %-^4.4g %c %-^4.4g",
                          LredN, 215, N[0], 215, N[1]);
  *(v->racun+4)=realloc(*(v->racun+4),(strlen(*(v->racun+4))+1)*sizeof(char));  // ST_helpMsg("Uspesan realloc");ngetchx();
 }
 ukupno+=qxq;
 //ST_helpMsg("Izlazim iz Prr_Veresc");ngetchx(); //if (ukupno!=0){char s[30];sprintf(s,"Stap:%c%c,ukupno=%f",id_i(d),id_k(d),ukupno);ST_helpMsg(s);ngetchx();}
 v->ukupno=ukupno;
 return ukupno;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
void Prr_VekSlobClanova(float **dnula,float *d0t,BOOL *dotd0)
{if (*dnula)free(*dnula);
 float proizvod=0,
       *d0=(float *)calloc(br_dijag-1,sizeof(float));
 Duz   *d=(*kd)[0][0];
 short stanje1=1;
 if (dotd0[0])
  while(stanje1<br_dijag)
       {proizvod=0;
        d=(*kd)[0][0];
        while (d)
              {proizvod+=Prr_Veresc(d,stanje1,0,FALSE);
               d=d->sused[0][1];
              }
        *(d0+stanje1-1)=proizvod;
        stanje1++;
       }
 stanje1=1;
 if (dotd0[1])
 while(stanje1<br_dijag)
       {*(d0+stanje1-1)+=d0t[stanje1-1];
        stanje1++;
       }
 *dnula=d0;
}
//---------------------------------------------------------------
WINDOW * NoviGlavniProzor(short x0,short y0,short x1,short y1)
{WINDOW *w=(WINDOW *)calloc(1,sizeof(WINDOW));
 WinOpen (w, &(WIN_RECT){x0,y0,x1,y1}, WF_SAVE_SCR | WF_TTY);
 WinActivate(w);
 WinFont (w, F_4x6);
 return w;
}
//----------------------------------------------------------------
void slozstap_listapreseka(Stap *nstap, SlozeniStap *sloz)
 {
   short levo=5, gore=5, red=FontCharWidth('M')+2;
   WINDOW *w = NoviGlavniProzor(pc->crt[0][0],pc->crt[1][0],pc->crt[0][1],pc->crt[1][1]);
   WinStrXY    (w,levo+5,gore, "Stap prom.popr.preseka:");
   short i;char s[15];
   for (i=0;i<sloz->brd;i++)
       {sprintf(s,"%c=%1.3f",138,(i==0?0:sloz->ksi[i-1]));                 WinStrXY (w,levo   ,         gore+(i+1  )*red+2,s);
        sprintf(s, "x=%4.4f",    (i==0?0:sloz->ksi[i-1]*l_d(nstap->duz))); WinStrXY (w,levo+40,         gore+(i+1  )*red+2,s);
       	sprintf(s, "I=%4.4f",    (ppres[sloz->pp[i]]).I );               	 WinStrXY (w,w->Clip.xy.x1-45,gore+(i+1.5)*red+2,s);
       }
   ngetchx ();
   WinClose (w);
 }

void DozvoliRadSaDijagramima(HANDLE exec_meni)
{	MenuSubStat (exec_meni,22,(br_dijag!=0));
}