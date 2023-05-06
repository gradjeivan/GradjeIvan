#include <tigcclib.h>

#include "dijag-interakcije.h"
/*----------- LOKALNE FUNKCIJE ------------------------------------------------*/
static void d_tacke_od_p( float T[2], float A[2], float B[2], float d[2]);
static BOOL tacka_obuhvacena(float nu,float mu, float A[2],float B[2]);
// BOOL unutar4tacke(float nu,float mu,float A[2],float B[2],float C[2],float D[2]);
//-----------------------------------------------------------------------
static short xduw(float mn, float razmera, short x0, short sirinaw)//pretvara koordinate sa dijagrama(mn) u koordinate na prozoru w
{return (short)(sirinaw - x0 - razmera*mn);	//crtam kontra jer je tako i u BAB-u
}
static short yduw(float mn, float razmera, short y0, short visinaw)//pretvara koordinate sa dijagrama(mn) u koordinate na prozoru w
{return (short)(visinaw - y0 - razmera*mn);
}//-----------------------------------------------------------------------
//static void Tabela_nu_mu_priprema(float *mnu, WINDOW *wtab)
static BOOL fjednaki(float a, float b)
{	return ( abs( a - b ) <= FNULA );
}
/*------------------------------------------------------------------------------*/

void Iniciraj_DI_meni(HANDLE *gmeni)
{
  MenuAddText (*gmeni,      0, "Dijag",      _DJG         ,DMF_TOP_SUB);
  {char s[7];       sprintf(s,"%c=a/d",128);
  MenuAddText (*gmeni,   _DJG,     s,        _ALFA        ,DMF_CHILD);}
  MenuAddText (*gmeni,   _DJG, "Aa2/Aa1",    _ODNOS_A1A2  ,DMF_CHILD);
  MenuAddText (*gmeni,   _DJG, "Karakt.lin.",_KARLIN      ,DMF_CHILD);

  MenuAddText (*gmeni,      0, "nu/mu",      _NUMU        ,DMF_TOP_SUB);
  MenuAddText (*gmeni,  _NUMU, "N,M",        _NM          ,DMF_CHILD);
  MenuAddText (*gmeni,  _NUMU, "b/d/h",      _BDH         ,DMF_CHILD);
  {char s[9];        sprintf(s,"pp.ea1,");
  MenuAddText (*gmeni,  _NUMU,     s,        _KOEFSIG     ,DMF_CHILD);}
  {char s[9];        sprintf(s,"pp.%cg",131);
  MenuAddText (*gmeni,  _NUMU,     s,        _POVOLJNOG   ,DMF_CHILD);}

}


/*----------KOEFICIJENTI JEDNACINA nu i mu------------------------------------- */
void mu_nu(float mnu[2][2], float mi1, float A[2])
{//daje tacku na karakt.liniji odredjenoj sa mnu, za koef.armiranja mi1
 A[0] = mi1*mnu[0][0] + mnu[0][1];
 A[1] = mi1*mnu[1][0] + mnu[1][1];
}//---------------------------------------------------------------------------

/*----------------IME DIJAGRAMA IZ BAB-A----------------------------------------*/
static short rbr_dijagrama(DijagramInterakcije *di)
{//funkcija podrazumeva da je alfa pozitivno i deljivo sa 0.025
 short i=0,pocrbr,//rbr dijagrama sa GA i alfa=0.05 za zadato k
       rbr=0;   //rbr dijagrama mereno od pocrbr
 BOOL  sedamalfi=( fjednaki(di->k, 1) || fjednaki(di->k, 0.5) );

 do
 {if ( fjednaki( ((float[]){DI_BAB_K})[i], di->k) ) break;
  i++;
 }while(i<DI_BAB_K_UKUPNO+1);

 if (i<DI_BAB_K_UKUPNO)//nasli smo odgovarajuce k
 { pocrbr=((short[]){DI_BAB_RBR})[i];
   if (di->armatura==_RA) pocrbr+=(sedamalfi?7:4);//pocetni rbr zavisi od tipa armature

   if (sedamalfi)//za ovo k postoji 7 dijagrama za 7 alfi DI_BAB_ALFE
      {do { if (fjednaki( ((float[]){DI_BAB_ALFE})[rbr], di->alfa )) break;
            rbr++;
          }while(rbr<7);
       if (rbr==7) return -1;//nije nadjen odgovarajuci dijagram ako bude rbr=7, postavljamo rbr=-1
      }
  else{//za ovo k postoji 4 dijagrama za alfe: 0.05, 0.1, 0.15, 0.2
       float kolicnik=fdiv(di->alfa,0.05);
  	   rbr= (short)(kolicnik);

  	   //ne postoje u BAB-u:
  	   if (!(fjednaki(kolicnik,rbr)))return -1;
   	   if (abs(rbr) > 4) return -1;

   	   rbr--;//posto rbr krece od 0
      }
 }
 else return -1;

 return rbr+pocrbr;
}
//--------------------------------------------------
BOOL DI_BAB_ime(DijagramInterakcije *di, char *ime)
{short rbr=(short)(di->alfa/0.025);
 if ( di->alfa/0.025 - rbr <= FNULA )//tablicni dijagrami imaju alfa sa korakom 0.025
    { if ( (rbr=rbr_dijagrama(di)) > 0 )
         { sprintf(ime,"2.%1d.%1d", (di->k==1? 4 : 5), rbr);
         	 return TRUE;
         }
      else return FALSE;
    }
 else      return FALSE;
}
/*----------KAR.LINIJA DI KOJA OBUHVATA ZADATE nu,mu----------------------------*/

char LinijaOblasti_mnu(DijagramInterakcije di, float nu, float mu)
{//vraca karakteristicnu liniju DI koja je sledeca u smeru ka liniji "h" posle zadate tacke (nu,mu)
 float mnu[2][2],A[2],B[2];
 char klin='a'; //krecemo od linije "a" ka "h"
 do
  { if  (klin+1 > 'g') break;//nema vise linija (nerealan slucaj)
    else klin++;

    DI_Karakt_linija(di,klin,mnu);
    mu_nu(mnu,0.1,A); mu_nu(mnu,0.8,B); //tacke na kar.liniji koje odgovaraju koef.armiranja 0.1 i 0.8
  }
 while (!(tacka_obuhvacena(nu,mu,A,B)));//dok linija ne obuhvati tacku (nu,mu)

 return klin;
}//---------------------------------------------------------------------------

BOOL LinijaEpsilon_mnu( DijagramInterakcije di, float nu, float mu, float epsA1[2], short brsec )
{//Funkcija je predvidjena da se koristi u oblasti interpolacije koef.sigurnosti
//Zadat je raspon-interval dilatacija zat. armature, epsA1. Polovim interval u kome je tacka brsec puta,
//ako nekom od njih pripada tacka nu,mu onda zapisujem te dilatacije nazad u epsA1,
//a rezultat funkcije je TRUE, ako ne pripada nijednom od intervala rez=FALSE
 float mnu[2][2],A[2],B[2],
       eps[2]={min(epsA1[0],epsA1[1]), max(epsA1[0],epsA1[1])}; //krajnje granice intervala:0-zategnuta,1-pritisnuta
 short i=0;//za cikluse
 Dilatacije_PP d;  d.z.arm=TRUE; d.p.arm=FALSE; d.p.ea=3.5;

 for (i=0; i<2; i++)//provera da li je tacka nu,mu u granicama zadatog intervala
     {d.z.ea=eps[i];  DI_epsilon_linija(di,d,mnu);  mu_nu(mnu,0.1,A); mu_nu(mnu,0.8,B);
      if ((i==0)&&(tacka_obuhvacena(nu,mu,A,B)))  return FALSE; //tacka je izvan granica intervala
     }

 for (i=0; i<brsec; i++)//polovljenje intervala
     {d.z.ea=(eps[1]+eps[0])/2;//polovina intervala dilatacija zat.armature
      DI_epsilon_linija(di,d,mnu);  mu_nu(mnu,0.1,A); mu_nu(mnu,0.8,B); //tacke na kar.liniji koje odgovaraju koef.armiranja 0.1 i 0.8
      eps[tacka_obuhvacena(nu,mu,A,B)?1:0]= d.z.ea;//pomeram odgovarajucu granicu sledeceg intervala na polovinu trenutnog
     }

 epsA1[0]=eps[0]; epsA1[1]=eps[1];
 return TRUE;
}//---------------------------------------------------------------------------

/*------------KOEF.ARMIRANJA ZA ZADATO nu,mu i DI-------------------------------*/
static BOOL izmedju_prava(float nu,float mu,float A[2],float C[2],float B[2],float D[2])
{//proverava da li je (nu,mu) izmedju prava AC i BD (tacke sa istim mi)
 float dAC[2],dBD[2];
 d_tacke_od_p( (float[2]){nu,mu}, A,C,dAC); //{char s[30]; sprintf(s,"dAC={%1.3f, %1.3f}",dAC[0],dAC[1]); ST_helpMsg(s); ngetchx();}
 d_tacke_od_p( (float[2]){nu,mu}, B,D,dBD); //{char s[30]; sprintf(s,"dBD={%1.3f, %1.3f}",dBD[0],dBD[1]); ST_helpMsg(s); ngetchx();}
 return (dAC[1]/dBD[1] < 0);//rastojanja po Y su suprotnog znaka ako je tacka izmedju
}
//-----------------------------------------------------
static float trazi_mi(float nu, float mu, float mnu[2][2], float mnuCD[2][2], float mi, float korak, short brkoraka)
{/*OVO NE BI TREBALO OVAKO, VEC POLOVLJENJEM INTERVALA-MNOGO JE SPORO OVAKO*/
 float A[2],B[2],C[2],D[2]; //koordinate po 2 tacke karakteristicnih pravih za susedni korak mi
 short i=0;//varijacija koef.armiranja

    mu_nu(mnu,  mi+i*korak,B);
    mu_nu(mnuCD,mi+i*korak,D);
 do{//nalazenje karakteristicnih tacaka za dato mi (tacke na kar. linijama za dato mi)
 	  memcpy(A,B,sizeof(B)); mu_nu(mnu,  mi+(i+1)*korak,B); //prethodna B postaje A, a novo B se racuna
 	  memcpy(C,D,sizeof(D)); mu_nu(mnuCD,mi+(i+1)*korak,D); //-||-
 	  i++;
   }
 while (  (!(izmedju_prava(nu,mu,A,C,B,D)))
        &&(i<brkoraka));

 return mi+(i-1)*korak;//vracamo nizi koef.armiranja
}
//----------------------------------------------------------------------
float DI_mi(DijagramInterakcije di,float nu,float mu, char prit_granica_oblasti)
{
 float mnu[2][2], //{nau,mau}{nba,mbu}.. au,bu u funkcijama nu(mi) i mu(mi)  ...=au*mi+bu
       mnuCD[2][2],
       mi1;       //koef.armiranja zat.armature

 //trazimo izmedju kojih kar.linija se nalazi (nu,mu)
 char klin= prit_granica_oblasti;//LinijaOblasti_mnu( di, nu, mu); //linija klin (AB) je bliza "h"

 //koeficijenti au,bu funkcija nu(mi) i mu(mi)  =au*mi+bu
 DI_Karakt_linija(di,klin  ,mnu  );
 DI_Karakt_linija(di,klin-1,mnuCD);         //linija (CD) je bliza "a"

 mi1=trazi_mi(nu,mu, mnu,mnuCD,   0, 0.10,  9);//korak mi=0.1, za 9 linija mi=0-0.8
 mi1=trazi_mi(nu,mu, mnu,mnuCD, mi1, 0.01, 10);//unutar prethodnog koraka, u desetinu

 return mi1+0.005; //dodajem polovinu poslednjeg koraka varijacije koef.armiranja
}/*-------------------------------------------------------------------------------*/

/*----------------- CRTANJE DIJAGRAMA INTERAKCIJE --------------------------------*/
void DI_Crtaj(DijagramInterakcije di, WINDOW *w)
{
 float nu[9][8], nu_max=0, nu_min=0, //[ variranje mi1 ] [ karakter. linija ]
       mu[9][8], mu_max=0, mu_min=0;
 short imi,ikl; //brojaci: koef.armiranja, karakteristicna linija

 Tabela_nu_mu_prr(di,nu,mu);

 /*min,max vrednosti nu,mu */
 for (ikl=0; ikl<8; ikl++)
 for (imi=0; imi<9; imi++)
     {
      if ( nu[imi][ikl] > nu_max ) nu_max=nu[imi][ikl];
   	  if ( nu[imi][ikl] < nu_min ) nu_min=nu[imi][ikl];
   	  if ( mu[imi][ikl] > mu_max ) mu_max=mu[imi][ikl];
   	  if ( mu[imi][ikl] < mu_min ) mu_min=mu[imi][ikl];
     }
 /*Crtanje*/
  //Odredjivanje razmere za crtanje
  float razmerayx=2.5;//razmera jedinica dijagrama po y i x osi
  short xw= w->Window.xy.x1 - w->Window.xy.x0,
        yw= w->Window.xy.y1 - w->Window.xy.y0;//sirina i visina prozora
  float xd= nu_max - nu_min,
        yd= mu_max - mu_min,        //sirina i visina dijagrama
        xyw=fdiv(xw,yw),
        xyd=xd/yd, //odnos sirine i visine za prozor, tj. dijagram
        R=(xyw>xyd? yw/yd : xw/xd),  //razmera jedinica dijagram/crtez - koristimo za x-koordinatu
        Ry=R*razmerayx;              //razmera za y-koordinatu
  short x0= (short)(-nu_min*R),
        y0= (short)(-mu_min*Ry);//polozaj koordinatnog pocetka dijagrama na prozoru w

  /*//Debug
  char s[30];
  short y=2,dy=6;
          sprintf(s,"xw=%d, yw=%d",xw,yw);  WinStrXY(w,5,y,s);
  y+=dy;  sprintf(s,"xw/yw=%f",xyw);        WinStrXY(w,5,y,s);
  y+=dy+2;sprintf(s,"nu,max/min=%1.3f/%1.3f",nu_max,nu_min);  WinStrXY(w,5,y,s);
  y+=dy;  sprintf(s,"mu,max/min=%1.3f/%1.3f",mu_max,mu_min);  WinStrXY(w,5,y,s);
  y+=dy+2;sprintf(s,"xd=%f, yd=%f",xd,yd);  WinStrXY(w,5,y,s);
  y+=dy;  sprintf(s,"xd/yd=%f",xyd);        WinStrXY(w,5,y,s);
  y+=dy;  sprintf(s,"R=%f",R);              WinStrXY(w,5,y,s);
  y+=dy;  sprintf(s,"x0=%d, y0=%d",x0,y0);  WinStrXY(w,5,y,s);
  ngetchx();
  */

  WinClr(w);
  //Koordinatne ose
  WinLine(w,&(WIN_RECT){xduw(nu_min,R,x0,xw), yduw(0     ,Ry,y0,yw),
                        xduw(nu_max,R,x0,xw), yduw(0     ,Ry,y0,yw)});
  WinLine(w,&(WIN_RECT){xduw(0     ,R,x0,xw), yduw(mu_min,Ry,y0,yw),
                        xduw(0     ,R,x0,xw), yduw(mu_max,Ry,y0,yw)});

  //Crtanje karakteristicnih linija
  for (ikl=0; ikl<8; ikl++)
       WinLine(w,&(WIN_RECT){xduw(nu[0][ikl],R,x0,xw), yduw(mu[0][ikl],Ry,y0,yw),
                             xduw(nu[8][ikl],R,x0,xw), yduw(mu[8][ikl],Ry,y0,yw)});

  //Povezivanje tacaka sa istim koef.armiranja
  for (imi=0; imi<9; imi++)
  for (ikl=0; ikl<7; ikl++)
       WinLine(w,&(WIN_RECT){xduw(nu[imi][ikl]  ,R,x0,xw), yduw(mu[imi][ikl]  ,Ry,y0,yw),
                             xduw(nu[imi][ikl+1],R,x0,xw), yduw(mu[imi][ikl+1],Ry,y0,yw)});

  char s[20],ime[6];
  if (DI_BAB_ime(&di,ime)) sprintf(s,"BAB %6s",ime);
  else                     sprintf(s,"ne postoji u BAB-u");
  WinFont(w,F_4x6);
  WinStrXY(w,2,2,s);

}//----------------------------------------------------------------------------------

/*--------STAMPA STANJA U PRESEKU ODREDJENOG ZADATIM DILATACIJAMA--------------------*/
short StanjePP_Stampa(DijagramInterakcije di,Dilatacije_PP d, WINDOW *w, short y)
{ short dy=6;
  char s[40];
  float ea1=eps_a1(d,di.alfa), ea2=eps_a2(d,di.alfa);

  //zadate dilatacije
  y+=dy+2;sprintf(s,"%c%c2 / %c%c1 = %1.3f / %1.3f %%.",//eps2/eps1
                   134,(d.p.arm?'a':'b'),134,(d.z.arm?'a':'b'),
                   (d.p.arm? ea2 : d.p.eb), (d.z.arm? ea1 : d.z.eb));  WinStrXY(w,5,y,s);
  //izracunate dilatacije
  y+=dy; sprintf(s,"%c%c2 / %c%c1 = %1.3f / %1.3f %%.",//eps2/eps1
                   134,(d.p.arm?'b':'a'),134,(d.z.arm?'b':'a'),
                       (d.p.arm? 0 :ea2),    (d.z.arm? 0 :ea1));       WinStrXY(w,5,y,s);

  //y+=dy+1;sprintf(s,"%cv = %1.3f %%."  ,134, epsilon_v(di.armatura));  WinStrXY(w,5,y,s);

 //armatura-naponi
     {float sa1=sigma_a(ea1,di.armatura), sa2=sigma_a(ea2,di.armatura);
      y+=dy+2; sprintf(s,"%ca1 = %f kN/cm%c",143, sa1, 178); WinStrXY(w,5,y,s);
      y+=dy;   sprintf(s,"%ca2 = %f kN/cm%c",143, sa2, 178); WinStrXY(w,5,y,s);
     }
  //beton-naponi
  if ( (eps_a2(d,di.alfa) > 0) && (!(d.p.arm)) )
     {float alfa_b=alfab(d.p.eb),
            eta_=eta(d.p.eb),
            s_=s_nl(d.p.eb,ea1);

      //y+=dy;   sprintf(s,"%cb1=%f",134, d.z.eb); WinStrXY(w,0,y,s);
      //y+=dy;   sprintf(s,"%cb2=%f",134, d.p.eb); WinStrXY(w,0,y,s);
      y+=dy+2; sprintf(s,"%cb = %1.3f" ,128, alfa_b);        WinStrXY(w,5,y,s);
      y+=dy;   sprintf(s,"eta = %1.3f, s = %1.3f" ,eta_,s_); WinStrXY(w,5,y,s);
      y+=dy;   sprintf(s,"zb  = %1.3f %c d" ,zb_bezdim(ea1,d.p.eb,di.alfa),215);
                                                             WinStrXY(w,5,y,s);
     }
 return y;
}
/*-----------------------------------------------------------------------------*/

/*------------------TABELA SA KARAKTERISTICNIM TACKAMA DI----------------------*/
void Tabela_nu_mu_prr(DijagramInterakcije di, float nu[][8], float mu[][8])
{
 float mi1,//koeficijent armiranja zategnute armature
       mnu[2][2]; //ovde drzim proracunate nau, mau, nbu, mbu
 char  klin='a';//karakteristicna linija
 short imi,ikl; //brojaci: koef.armiranja, karakteristicna linija

 /*za svaku karakteristicnu liniju: */
 for (ikl=0; ikl<8; ikl++)
 {DI_Karakt_linija(di,klin,mnu);
 	/*variranje koeficijenta armiranja*/
  for (imi=0; imi<9; imi++)
      {
       mi1 = imi*0.1;
   	   nu[imi][ikl] = mi1*mnu[0][0] + mnu[0][1];
   	   mu[imi][ikl] = mi1*mnu[1][0] + mnu[1][1];
      }
  klin++;//sledeca oznaka karakt.linije
 }
}//----------------------------------------------------------------------
//------------ STAMPA TABELE---------------------------------------------
static void Tabela_nu_mu_priprema(float *mnu, WINDOW *wtab);

void Tabela_nu_mu(float nu[][8], float mu[][8])
{
  WINDOW * wtab=NoviProzor(0,2,/*rr->menuH-2,*/LCD_WIDTH,LCD_HEIGHT-8,TRUE);
  Tabela_nu_mu_priprema(&(nu[0][0]),wtab);
  Tabela_nu_mu_priprema(&(mu[0][0]),wtab);
  WinClose(wtab);
}//----------------------------------------------------------------------

static void Tabela_nu_mu_priprema(float *mnu, WINDOW *wtab)
{
	char pmem[8]="ffffffff",
       frmt[8][6],zagl[8][6];
  FrmtTabKol ftk[8];
  short m=0;
  for (m=0; m<8; m++)
      {
       strcpy(frmt[m],"%1.3f");       (ftk[m]).frmt     =frmt[m];
       sprintf(zagl[m],"(%c)",'a'+m);	(ftk[m]).zaglavlje=zagl[m];
      }
 FrmtTab ft={9,8,mnu,ftk,pmem,wtab};
  //tabela(&ft);
}

/*----------- KARAKTERISTICNE LINIJE DIJAGRAMA INTERAKCIJE -------------------------*/
void DI_epsilon_linija( DijagramInterakcije di,  Dilatacije_PP d, float mnu[2][2] )
{
 mnu[0][0]= nau(di,d);  mnu[0][1]= nbu(di,d);
 mnu[1][0]= mau(di,d);  mnu[1][1]= mbu(di,d);
}
//------------------------------------
Dilatacije_PP DI_Karakt_linija( DijagramInterakcije di, char linija, float mnu[2][2] )
{
 Dilatacije_PP d;  d.z.arm=d.p.arm=TRUE;
 switch (linija)//-------------------------------------
 {case 'a'://  -10/-10
           d.p.arm=TRUE;  d.z.arm=TRUE;
           d.p.ea =-10;   d.z.ea =-10;
 	     break;
 	case 'b'://    0/-10
           d.p.arm=FALSE; d.z.arm=TRUE;
           d.p.eb =0;     d.z.ea =-10;
       break;
 	case 'c'://  3.5/-10
           d.p.arm=FALSE; d.z.arm=TRUE;
           d.p.eb =3.5;   d.z.ea =-10;
 	     break;
 	case 'd'://  3.5/-3        // odavde krece interpolacija koef.sigurnosti
           d.p.arm=FALSE; d.z.arm=TRUE;
           d.p.eb =3.5;   d.z.ea =-3;
 	     break;
 	case 'e'://  3.5/eps_v
           d.p.arm=FALSE; d.z.arm=TRUE;
           d.p.eb =3.5;   d.z.ea = -epsilon_v(di.armatura);
	     break;
 	case 'f'://  3.5/0 (ea1)
           d.p.arm=FALSE; d.z.arm=TRUE;
           d.p.eb =3.5;   d.z.ea =0;
 	     break;
 	case 'g'://  3.5/0 (eb1)   // dovde je interpolacija koef.sigurnosti
           d.p.arm=FALSE; d.z.arm=FALSE;
           d.p.eb =3.5;   d.z.eb =0;
 	     break;
 	case 'h'://    2/2
           d.p.arm=FALSE; d.z.arm=FALSE;
           d.p.eb =2;     d.z.eb =2;
 	     break;
 	default:
 	     break;
 }//--------------------------------------------------
 DI_epsilon_linija( di, d, mnu );
 return d;
}


/*---------------------------------------------------------------------------------*/

/*--------------------LOKALNE FUNKCIJE---------------------------------------------*/
static void d_tacke_od_p( float T[2], float A[2], float B[2], float d[2])
{/*pozitivno rastojanje: tacka iznad i desno*/
 float k=0,n=0;

 if      (abs(B[0]-A[0])<=FNULA)
         {//VERTIKALNA LINIJA: k je beskonacno
    	     if (n==T[0]) d[0]=d[1]=0.; //tacka pripada pravoj; n je odsecak prave na X-osi
           else         d[0]=T[0]-A[0]; d[1]=UNSIGNED_INF;
         }
 else
    { if (abs(B[1]-A[1])<=FNULA)
         {//HORIZONTALNA LINIJA: k=0
           n =  A[1] - k*A[0]; //n je odsecak prave na Y-osi
           if (n==T[1]) d[0]=d[1]=0.; //tacka pripada pravoj
           else         d[1]=UNSIGNED_INF; d[1]=T[1]-n;
         }
      else //LINIJA PROIZVOLJNOG PRAVCA
    	   { k = (B[1]-A[1]) / (B[0]-A[0]);
           n =  A[1] - k*A[0];
           if ( abs( T[1] - (k*T[0] + n) ) <=FNULA )// T pripada AB
                d[0]= d[1]= 0.;
           else{
         	   	  d[0]= T[0] - (T[1]-n)/k ;
    	      	  d[1]= k*T[0] + n - T[1] ;
           	   }
    	   }
    }
}//-----------------------------------------------------------------------------------

static BOOL tacka_obuhvacena(float nu,float mu, float A[2],float B[2])
{//Provera da li tacka (nu,mu) obuhvacena oblascu koju cini ugao izmedju linije "a" i zadate linije AB
 float d[2]={0,0};
 d_tacke_od_p( (float[2]){nu,mu}, A,B, d );//upis u d rastojanja dx i dy tacke (nu,mu) od AB

 if ((abs(d[0]) <= FNULA) || (abs(d[1]) <= FNULA)) return TRUE;//tacka pripada pravoj
 else//tacka ne pripada pravoj:
     return (d[0]<0);//mora biti levo od prave da bi smo rekli da je obuhvacena
                     /*PAZNJA!! levo ovde znaci ka -, sto je u BAB-u desno !! */
}//---------------------------------------------------------------------------
