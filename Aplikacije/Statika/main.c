#define COMMENT_STRING         "gradjeivan.blogspot.com"
#define COMMENT_PROGRAM_NAME   "GradjeIvan Statika 2"
#define COMMENT_VERSION_STRING ""
#define COMMENT_VERSION_NUMBER 5,0,0,0	 /* major, minor, revision, subrevision */
#define COMMENT_AUTHORS        "Radovanovic Ivan"
#define MIN_VER_FAJLA          3,8,3,2 /*minimalna verzija programa koji je sacuvao fajl*/

/*------------------konstante----------------------------*/
#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization
//#define MIN_AMS 200         // Compile for AMS 2.00 or higher, treba za Menubegin
//#define SAVE_SCREEN         // Save/Restore LCD Contents
//#define RETURN_VALUE        // Return a Value
#define EKSTENZIJA "gri"

/*-------------------headeri-------------------------------*/
#include <tigcclib.h>
//#include <stdlib.h>

/*-------- konstante zavisne od headera -------------------*/
//#define INFO {{MENUL,0}{LCD_WIDTH-LISTADESNO,MENUH}} //gornji levi uglovi info prozora
char VarBuffer[20] = "";

/*----------------moji headeri-----------------------------*/
#include "gri-text.h"
#include "gradjeivan-tipovi.h"
#include "statika-tipovi.h"
#include "geometrija-tipovi.h"
#include "griinic-load.h"
#include "standardno.h"

#include "dijalozi.h"
//#include "tabela.h"

#include "geometrija-c.h"
#include "geometrija-ti.h"
#include "unosgeom-funkcije.h"
#include "statika-c.h"
#include "grimtprr-load.h"
#include "statika-dijalozi.h"
#include "statika-grafika.h"
#include "statika-matrice.h"
#include "statika-funkcije.h"

/*--------------------GLOBALNE PROMENLJIVE----------------------------*/
Nosac *nsc;//takodje deklarisan i kao extern u headerima
/*--------------------------FUNKCIJE---------------------------------*/
short UcitajDLL(const char *imedlla, unsigned long ID, unsigned long major, unsigned long minor)
{/*funkcija pokusava da ucita zadati dll, uz komentar kroz dijalog,
  a kao output dobija se kod greske pri otvaranju ili DLL_OK=0 ako je sve OK*/

 short rezultat=LoadDLL(imedlla,ID,major,minor);
 //printf("%d",rezultat); ngetchx();
 if (rezultat != DLL_OK)
   {
    switch (rezultat)
    {
     case DLL_OUTOFMEM:     printf("Mem");        break;
     case DLL_WRONGVERSION: printf("Ver");        break;
     case DLL_NOTFOUND:     printf("Nema");       break;
     case DLL_ALREADYLOADED:printf("Zauzeto");    break;
     default:               printf("%d",rezultat);break;
     ngetchx();
    }
    DlgMessage (GRESKA, GRESKA_DLL, BT_OK, BT_NONE);
   }
 return rezultat;
}

static void ST_PozdravnaPoruka(short polozaj)
{short ver[]={COMMENT_VERSION_NUMBER};
 char s[35];sprintf(s,"GRA%cEIVAN Statika v %d.%d.%d.%d",208,ver[0],ver[1],ver[2],ver[3]);
 if (polozaj==0)ST_helpMsg(s);
 else  Info(3,0,0,s,A_REPLACE);
}

static void DozvoliOpcijuMenija(HANDLE exec_gmeni, short opcija)
{BOOL uslov=TRUE;
 switch (opcija)
 {case _STAPOVI:         uslov=UslovZaUnosDuzi();        break;
  case _DIJAGRAMI:       opcija=4;
 	case _STAP_IZMENI:
 	case _STAP_IZBRISI:    uslov=((*kd)[2][1]!=NULL);      break;

  case _PPRES_UPOREDNI:  uslov=(ppres!=NULL);            break;
  case _OSL_IZMENI:
  case _OSL_IZBRISI:     /*uslov=*/                      break;

  case _MATRICE:         uslov=((*kd)[0][0]!=NULL);
                         opcija=3;                       break;

  case _SUPERPOZICIJA:   uslov=TRUE;/*ovde bez break*///dodaje se i donji uslov
  case _VERESCAGIN:
  case _EL_TEZINE:
  case _USL_JEDNACINE:   uslov=((uslov)&&(br_dijag>1));  break;
 }
 if (opcija<10)MenuTopStat(exec_gmeni, opcija , uslov);//TopOpcija
 else          MenuSubStat(exec_gmeni, opcija , uslov);//SubOpcija
}

static void MeniAzurirajListu(HANDLE exec_gmeni, short bropcija, short *opcije)
{//Azurira sve elemente menija iz zadate liste
 short i;
 for (i=0;i<bropcija;i++) DozvoliOpcijuMenija(exec_gmeni,opcije[i]);
 MenuOn(exec_gmeni);
}

static void MeniAzuriraj(HANDLE exec_gmeni, short opcija)
{//Azurira meni na osnovu poslednje birane funkcije iz gl.menija
 switch (opcija)
 {case -1:/*pocetna podesavanja*/
  case _NOVI:case _OTVORI:
          MeniAzurirajListu(exec_gmeni,19,(short[19])
          { //_FAJL,          _NOVI,         _OTVORI,     _SACUVAJ,     _SACUVAJ_KAO,
            //_GEOMETRIJA,    _TACKE,        _MATERIJAL,  _POP_PRESEK,  _PPRES_IZMENA,
            _PPRES_UPOREDNI,_OSLONCI,      _OSL_DODAJ,  _OSL_IZMENI,  _OSL_IZBRISI,
            _STAPOVI,       _STAP_DODAJ,   _STAP_IZMENI,_STAP_IZBRISI,//_CRTEZ,
            //_MREZA,         _VIDLJIVOST,   _OSVEZI,     _NOSAC,
            _MATRICE,
            //_MAT_K,         _MAT_T,        _MAT_KGLOB,  _MAT_KNOSACA, _MAT_KNN,
            _DIJAGRAMI,     _DIJAG_ZADAJ,  _DIJAG_TEMP_SLEGANJE, _DIJAG_TEMP_SLEG_ZADAJ, _DIJAG_TEMP_SLEG_SABIRCI,
            _USL_JEDNACINE, _SUPERPOZICIJA,_EL_TEZINE,  _VERESCAGIN
          });
       break;
  case _MREZA:
 	case _STAP_DODAJ:MeniAzurirajListu(exec_gmeni,5,(short[5])
 	         {_STAPOVI, _STAP_IZMENI, _STAP_IZBRISI, _MATRICE, _DIJAGRAMI});
       break;
 	case _POP_PRESEK:MeniAzurirajListu(exec_gmeni,2,(short[2])
 	         {_PPRES_IZMENA, _PPRES_UPOREDNI});
       break;
 	case _DIJAG_ZADAJ:MeniAzurirajListu(exec_gmeni,3,(short[3])
 	         {_USL_JEDNACINE, _EL_TEZINE, _VERESCAGIN});
       break;
  case _USL_JEDNACINE:MeniAzurirajListu(exec_gmeni,1,(short[1])
 	         {_SUPERPOZICIJA});
       break;

  default: break;
 }

}
/*________________________________________________________________________________________________*/
/* * * * * * * * * * * * * * * *  M A I N  F U N C T I O N  * * * * * * * * * * * * * * * * * * * */
void _main(void)
{

/*Inicijalizacija promenljivih:*///U ovom delu dodeljujem nulte vrednosti
InicGlobProm_statika_c();//inicijalizacija statickih tipova
Nosac nosac;
nosac.rostilj=FALSE;//BOOL     rostilj=FALSE;
nsc=&(nosac);
kt= &(nosac.kt); nuliraj_exttck();
kd= &(nosac.kd); nuliraj_extduz();//obavezno pre Novi_ na pocetku
pc= &(nosac.pc);

Novi_();
   /*Novi fajl*///U ovom delu se kompletan nosac brise iz memorije, pa se ponovi inic
   //Prethodni inic je potreban jer su potrebne makar i nulte vrednosti za prolaz kroz oslobadjanje memorije

/*Otvori poslednji koriscen fajl*///ako i dalje postoji
char imefajla[18];
FILE *fajl=fopen(IME_POSLEDNJEG_FAJLA,"rb");
if (fajl){Procitaj_Nosac(fajl,(short[]){MIN_VER_FAJLA}); fclose(fajl);}

/*Pokazivaci potrebni za matrice D,d0,d0t,Xi*/
Matrica *D=NULL;//matrica koeficijenata
float *d0 =NULL,//vektor slobodnih clanova
      *d0t=NULL,//niz zbirova delti od temperature i sleganja/obrtanja oslonaca
      *Xi =NULL;//niz staticki nepoznatih velicina
/*Mnozioc za d0*/// dot.mnozenjem d0 sa ovim vektorom odredjujem koja spolj.opterecenja
                 // se uzimaju u obzir pri proracunu [Xi]: spoljasnje opterecenje silama, temperatura+sleganje
BOOL dotd0[2]={TRUE,FALSE};//pri ovakvom vektoru se uzima samo opt.silama, a temp+sleg se ne uzima

/*stabilnost*/
float uporedni[3]={0,0,0};//Ic,Lc,Pc - vrednosti

short srt[2]={0,1},tipka,izbor;//uneta tipka,izabrano iz menija

clrscr();CrtajOkvirCrt();
/*Iniciranje glavnog menija*///pri gresci ucitavanja dll-a izadji iz programa
HANDLE gmeni = MenuNew (2, 240, 18);
if (UcitajDLL("griinic",604224826,1,1)!=0) exit(0);/* PAZNJA !! Izlazi se iz programa, prevremeno
                                     treba resiti oslobadjanje memorije funkcijom atexit()  */
Iniciraj_glavni_meni(&gmeni);
UnloadDLL();

/*Ucitaj DLL za matricnu analizu*///pri gresci ucitavanja dll-a izadji iz programa
if (UcitajDLL("grimtprr",631910222,1,1)!=0) exit(0); /* treba osloboditi memoriju!!*/

HANDLE exec_gmeni = MenuBegin (NULL, 0, 0, 0x20, gmeni);
MeniAzuriraj(exec_gmeni,-1);
InfoStepeniSlobode(); ST_PozdravnaPoruka(1);//InfoKarakteristikeStapa();
CrtezOsvezi();//Ako smo ucitali neki fajl automatski

do/*===============================================================================================*/
{ ST_busy(0);
  {char s[35];sprintf(s,"%s",imefajla);ST_helpMsg(s);}
  tipka=ngetchx();
  char info[51]="                                                  ";
  Info(3,0,0,info,A_REPLACE); //brisemo info liniju nakon prve pritisnute tipke //ngetchx();
  while ((izbor=MenuKey(exec_gmeni,tipka))==M_NOTMENUKEY)
     {/* --*-*- ovde razmatramo tipke koje nisu iz menija --*-*-*-*-*-*-*-*/   //printf("izbor=%d",tipka);ngetchx();
      switch(tipka)
        {case 277:case 45:case 43:case 'U':case 'O':
              if (*kt[0][0]!=NULL)//ako ima sta da se zumira, tj. ako je uneta bar 1 tacka
                 { BrisiCrtez();   Zumiranja(tipka);   (*pc).rezim=A_NORMAL;CrtajNosac(); }
            break;
         case 337:case 340:case 338:case 344:case 8530:case 8536:case 8529:case 8532:
              if (*kt[0][0]!=NULL)
                 { BrisiCrtez(); Strelice(tipka); (*pc).rezim=A_NORMAL;CrtajNosac(); }
            break;
         case 265:/*APPS*/
                 { Vidljivost();CrtezOsvezi();}
            break;
         default:pomocni_prozor(exec_gmeni,(short[]){COMMENT_VERSION_NUMBER});break;
        }
      tipka=ngetchx();//cekamo sledecu tipku
     }
  /*a sad, akcija na osnovu unosa iz menija, tj. =izbor=  */
  /*-----------------------------------------------------------------------------------------*/
  MenuOff(exec_gmeni);
  switch(izbor)/*___________________________________________________________*/
  { /*F1:   .   .   .   .   .   .   .  FAJL   .   .   .   .   .   .   .   . */
    case _NOVI:/*Novi fajl*/
            Novi_(); BrisiCrtez();
            strcpy(imefajla,"");
            Grid_podesavanje();CrtezOsvezi();
        break;
    case _OTVORI:{char ime[18]; // ST_helpMsg(ime);ngetchx();
             if(Otvori_fajl_dlg(ime,(char*)EKSTENZIJA))
               {fajl=fopen(ime,"rb");//printf("fajl=%lu",fajl);ngetchx();
                if (fajl!=NULL)
     	           {//pc.rezim=A_XOR;CrtajNosac(&pc,&kt,&kd);//Izbrisi prethodan crtez
		              strcpy(imefajla,ime);  //ST_helpMsg(imefajla);ngetchx();
		              Novi_();
		              Procitaj_Nosac(fajl,(short[]){MIN_VER_FAJLA});
		              fclose(fajl); //ucitaj podatke i zatvori fajl
		              BrisiCrtez();(*pc).rezim=A_NORMAL;CrtajNosac();//novi crtez
            	   }
            	  else {char s[30];sprintf(s,GRESKA_UCITAVANJE);
            	  	    Info(3,0,0,s,A_NORMAL);
            	  	   }
               }
            }
        break;
    case _SACUVAJ:/*Sacuvaj fajl*/
    case _SACUVAJ_KAO:/*Sacuvaj kao...*/
            {char ime[18]; char info[35];
             BOOL sacuvaj=FALSE;
             if ((izbor==13)&&(imefajla[0]!='\0'))//Sacuvaj vec otvoren fajl
                 {strcpy(ime,imefajla); sacuvaj=TRUE;}
             else//Ili je izabrano Sacuvaj kao.. ili je u pitanju Novi nesacuvan fajl
                 {sacuvaj=SacuvajFajl(ime,(char*)EKSTENZIJA);}

             if (sacuvaj)
                 {fajl=fopen(ime,"wb");
                  if (fajl)//ako je uspesno otvoren
                  {Sacuvaj_Nosac(fajl,(short[]){COMMENT_VERSION_NUMBER});fclose(fajl);
                   strcpy(imefajla,ime);
                   sprintf(info,FAJL_SACUVAN);
                  }
                  else sprintf(info,FAJL_NEUSPESNO_CUVANJE);
                 }
             else sprintf(info,FAJL_OTKAZANO_CUVANJE);
             Info(3,0,0,info,A_NORMAL);
            }
        break;/*________________________________________________________________*/
    /*F2:   .   .   .   .   .   .   .  UNOS   .   .   .   .   .   .   .   . */
    case _TACKE:/*Unos tacaka*/
            Unos_tacaka();//azuriraj_param_crtanja(&pc,&kt);
            if (*kt[0][0]!=NULL)
            {BrisiCrtez();//Stampaj_tacke(2);
             srt[0]=0;srt[1]=1;
            }
            BrisiCrtez();//pc.rezim=A_XOR;CrtajNosac(&pc,&kt);
            (*pc).rezim=A_NORMAL;CrtajNosac();
        break;
    case _STAP_DODAJ:/*Unos duzi*/
            if (UslovZaUnosDuzi())
            {(*pc).zum=ZUM;(*pc).dok[0]=(*pc).dok[1]=0;//zasad mora ovako, kasnije cemo da napravimo unos sa zumiranjem
             BrisiCrtez();(*pc).rezim=A_NORMAL;CrtajNosac();
             Unos_stapova();//printf("main");
             //if (*kd[0][0]!=NULL) Stampaj_duzi(0);//srt[0]=0;srt[1]=1;//duzi_stampaj(kd[0][0],srt,5);
             //stapove_stampaj();
            }
            else //ovo ne bi trebalo da se desava-samo za debug
            {char info[20]=UNOS_TACAKA_NEMA;Info(3,0,0,info,A_NORMAL);  //ngetchx();
             //sprintf(info, "  tacaka");  Info(1,1,0,info,A_NORMAL);
            }
        break;
    case _STAP_IZMENI:/*Izmena duzi*/
             Stapovi_izmena();
        break;
    case _STAP_IZBRISI:/*Brisanje duzi*/
        break;
    case _MATERIJAL:Unos_materijala();break;
    case _PPRES_IZMENA:Unos_preseka();break;
    case _PPRES_UPOREDNI:Uporedni_pp();  break;
    /*_____________________________________________________________________*/
    /*  .   .   .   .   .   .   .    CRTEZ   .   .   .   .   .   .   .   . */
    case _MREZA:{Grid_podesavanje();CrtezOsvezi();
                }//Grid_pod(&pc);printf("grid gotov");ngetchx();
        break;
    case _VIDLJIVOST:Vidljivost();CrtezOsvezi();
        break;
    case _OSVEZI:CrtezOsvezi();
        break;/*_______________________________________________________________*/
    /*F4:   .   .   .   .   .   .   NOSAC    .   .   .   .   .   .   .   . */
    case _RAVANROSTILJ:IzborRavanRostilj(&(nsc->rostilj));break;
    case _STABILNOST_UPOREDNI: Unos_IlPc (&(uporedni[0]),&(uporedni[1]),&(uporedni[2]));
        break;
    case _STABILNOST_AKSIJALNE: //Unos_P_stabilnost ();
        break;/*_______________________________________________________________*/
    /*F5:   .   .   .   .   .   .  PRORACUN  .   .   .   .   .   .   .   . */
    case _DIJAG_ZADAJ:ZadajDijagram();break;
    case _DIJAG_TEMP_SLEG_ZADAJ:Zadaj_delta_0t(&d0t);break;
    case _DIJAG_TEMP_SLEG_SABIRCI:Izbor_sabiraka_za_d0(dotd0);
        break;
    case _DIJAG_GRUPE:GrupeDijagrama();
        break;
    case _USL_JEDNACINE:/*USLOVNE JEDNACINE METODE SILA*/
            {Matrica *Dinv;
             UslovneJednacine_Prr   (&D,&d0,&Xi,&Dinv,d0t,dotd0);
             UslovneJednacine_Prikaz( D, d0, Xi, Dinv,d0t,dotd0);
             if(Dinv->M)free(Dinv->M);
             if(Dinv)   free(Dinv);
            }
         break;
    case _SUPERPOZICIJA:SuperpozicijaM(Xi);break;
    case _EL_TEZINE:ElasticneTezine_Prikaz();break;
    case _VERESCAGIN://IzbrisiSvePc();//ovo je ovde privremeno, dok ne prebacim Pc iz stabilnosti u nesto nevezano za stap->jos
                     Verescagin();
        break;/*_______________________________________________________________*/

    /*F4:   .   .   .   .   MATRICE/DEBUG    .   .   .   .   .   .   .   . */
    case _MAT_K:       StampajMatKrut          (nsc->rostilj);  break;
    case _MAT_T:       StampajMatTrans         (nsc->rostilj);  break;
    case _MAT_KGLOB:   StampajMatKrutGlob      (nsc->rostilj,0,uporedni);  break;
    case _MAT_KNOSACA: MatKrutNosaca_Stampaj   (nsc->rostilj,0,uporedni);  break;
    case _MAT_KNN:     MatKrutNosacaKnn_Stampaj(nsc->rostilj,0,uporedni);  break;

    case _MAT_K_STAB:  StampajMatK_stabilnost  (uporedni);      break;
    case _MAT_KG:      StampajMatKg_stabilnost (uporedni);      break;
    case _MAT_K_STAB_NOSACA: MatKrutNosacaKnn_Stampaj(nsc->rostilj,1,uporedni);break;
    case _MAT_KG_NOSACA:     MatKrutNosacaKnn_Stampaj(nsc->rostilj,2,uporedni);break;

    case 410:/*StampajParametreCrtanja();*/break;
    case 420:/*clrscr();prikazi_ekstreme();ngetchx();*/break;
    case 430:/*Zauzece_memorije_po_tipovima();*/break;
    /*____________________________________________________________________________*/
  }/*------------------------------------------------------------------------------------------*/
  MenuOn(exec_gmeni);
  MeniAzuriraj(exec_gmeni,izbor);
}
while (izbor !=99);/*=================================================================================*/
//izbor=99 je za izlaz iz programa

MenuEnd (exec_gmeni);//printf("....");ngetchx();clrscr();//if (fajl!=NULL){fclose(fajl);}//printf("...");ngetchx();clrscr();
MenuUpdate ();//printf("..");ngetchx();clrscr();

/*-------------Zapamti aktuelni fajl za sledeci put-------------------*/
fajl=fopen(IME_POSLEDNJEG_FAJLA,"wb");
Sacuvaj_Nosac(fajl,(short[]){COMMENT_VERSION_NUMBER});
fclose(fajl);

/*-------------Oslobadjanje zauzete memorije--------------------------*///bitan je redosled !!!
srt[0]=0;srt[1]=1; //ngetchx();
//brisi_sve_Veresc();//brisanje memorije zauzete verescaginom
/*-------------brisanje Pc-----------------*/
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
/*------------------------------------------*/
Oslobodi_memoriju_nosaca();
if (d0t) free(d0t);
if (d0 ) free(d0 );
if (D  ){if (D->M)free(D->M);
         free(D);
        }
/*--------------------------------------------------------------------*/
UnloadDLL ();
/*--------------------------------------------------------------------*/
restoreAlphaLock(&(char){FALSE});
ST_helpMsg(GRADJEIVAN_BLOGSPOT);
}//_main


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/



