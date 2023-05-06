#include <tigcclib.h>

#ifndef BETON_TIPOVI
#include "beton-tipovi.h"
#endif

#include "beton-proracun.h"

/*---------------------------- BETON  -----------------------------------------*/
/*--- mbu ------------------------------*/
float mbu(DijagramInterakcije di, Dilatacije_PP d)
{/*mbu je sabirak u zbiru mu=mbu+mau*mi, koji potice od sile u betonu, i ne zavisi od koef.arm.*/
 if (!(d.p.arm))//ima vrednost samo ako je zadata dilatacija prit.ivice betona
      if ((!(d.z.arm))&&(d.z.eb==2))//linija "h" , zb=0
           return 0;
      else return ( nbu(di,d) * zb_bezdim(eps_a1(d,di.alfa), d.p.eb, di.alfa) );
 else return 0;
}
/*---KRAK SILE U BETONU-----------------*/
float zb_bezdim(float ea1, float eb2, float alfa)
{//koeficijent kojim treba pomnoziti d da se dobije krak sile u betonu
 float h=1-alfa, s=s_nl(eb2,ea1), eta_=eta(eb2);
// if ((ea1<=0) || ((!(d.z.arm))&&(d.z.eb==0))) //do linije "g"
     return (0.5 - h*s*eta_);
// else
    {//interpolacija po dilataciji; po istom principu kao za nbu
     /*
     float  zb035= 1/2 - (1-alfa)*s_nl(3.5,0)*eta(3.5);
     return (1 - ea1/2)*zb035;
     */
    }
}
/*--- nbu --------------------------*/
float nbu(DijagramInterakcije di, Dilatacije_PP d )
{/*nbu je sabirak u zbiru nu=nbu+nau*mi, koji potice od sile u betonu, i ne zavisi od koef.arm.*/
 if (!(d.p.arm))//ima vrednost samo ako je zadata dilatacija prit.ivice betona
    {
     float ea1=eps_a1(d,di.alfa);

     if ((ea1<=0) || ((!(d.z.arm))&&(d.z.eb==0))) //do linije "g"
           return alfab(d.p.eb)*s_nl(d.p.eb,ea1)*(1-di.alfa);
     else{//za ovaj deo ovde nemam tacne formule, radim interpolaciju po dilataciji eb1
          // izmedju dva granicna slucaja, linija g i h: eb1/eb2=0/3.5%. i eb1/eb2=2/2%.
           float  nbu035=alfab(3.5)*s_nl(3.5,0)*(1-di.alfa);//vrednost nbu za 0/3.5
           return nbu035+(d.z.eb/2)*(1-nbu035);//za e1=2 dostize se vrednost 1: dbu=Ab*fb za liniju h
         }
    }
 else return 0;//beton ne ucestvuje
}
/*---- koef.punoce naponskog dijagrama u betonu------*/
float alfab(float eb2)// eb2 je u promilima; tj. za 5 promila unesi 5
{if (eb2>2) return (3*eb2-2)/(3*eb2);
 else       return (eb2/12*(6-eb2));
}
/*---- bezdim.koef.polozaja sile u betonu------------*/
float eta(float eb2)// eb2 je u promilima
{if (eb2>2) return ( (3*pow(eb2,2)-4*eb2+2) / (2*eb2*(3*eb2-2)) );
 else       return (8-eb2)/(4*(6-eb2));
}
/*---- bezdim.koef.polozaja neutralne linije---------*/
float s_nl(float eb2, float ea1)
{return eb2/(eb2-ea1);
}
/*----DILATACIJE U BETONU-------------------------*/
//U sustini kada nisu zadate, nisu ni potrebne


/*--------------------ARMATURA--------------------------------------------------*/
/*--- nau --------------------------*/
float nau(DijagramInterakcije di, Dilatacije_PP d )
{/*nau je mnozioc u sabirku zbira nu=nbu+nau*mi, koji potice od sile u armaturi, i mnozi koef.arm.*/
//  float eps_v=epsilon_v(di.armatura);
	return (      sigma_a(eps_a1(d,di.alfa),di.armatura)
	        +di.k*sigma_a(eps_a2(d,di.alfa),di.armatura))
	        / sigma_v(di.armatura);
}
/*--- mau --------------------------*/
float mau(DijagramInterakcije di, Dilatacije_PP d )
{/*mau je mnozioc u sabirku zbira mu=mbu+mau*mi, koji potice od sile u armaturi, i mnozi koef.arm.*/
//  float eps_v=epsilon_v(di.armatura);
	return (0.5-di.alfa)*
	       (     -sigma_a(eps_a1(d,di.alfa),di.armatura)
	        +di.k*sigma_a(eps_a2(d,di.alfa),di.armatura))
	        / sigma_v(di.armatura);
}

/*----NAPON U ARMATURI------------------------*/
float sigma_v(short arm)
{	return ((short[]){SIGMA_V})[arm];
}

float sigma_a(float epsa, short armatura)
{ return (abs(epsa)>=epsilon_v(armatura)? (epsa/abs(epsa))*sigma_v(armatura) : epsa*EA/1000);
}

/*----DILATACIJE U ARMATURI-------------------*/
float epsilon_v(short arm)
{	return 1000*sigma_v(arm)/EA;
}

float interpolacija(float a,float b,float l,float x)
//ordinate a i b, l rastojanje mereno od a, x rastojanje(mereno od a) na kome se trazi ordinata
{float tangens=(b-a)/l;
 return a+x*tangens;
}
//------------------------------
/*float eps_a1_NM( float N,float M, ABPresek *ABpp , short mb)
//pozitivni smerovi: N pritisak, M zateze donju stranu
{ float n=(float[]){SIGMA_V}[mb] / (float[]){_EB}[mb],//broj ekvivalencije
        b=ABpp->pp->pravoug.b,
        d=ABpp->pp->pravoug.d,
        h=d - ABpp->a[0];
  return (N - M/(b*d*d/12) );
}*/
//-------------------------------
float eps_a1(Dilatacije_PP d, float alfa)
{if (d.z.arm)
          return d.z.ea;
 else//zadata je dilatacija u betonu (zategnuta ivica)
    {if (d.p.arm)//zadata dilatacija u pritisnutoj armaturi(ova kombinacija ne bi trebalo da postoji)
          return interpolacija( d.z.eb, d.p.ea, 1-alfa, alfa );
     else//zadata je dilatacija u pritisnutoj ivici betona(ovo je realna kombinacija)
          return interpolacija( d.z.eb, d.p.eb,      1, alfa );
    }
}
//------------------------------
float eps_a2(Dilatacije_PP d, float alfa)
{if (d.p.arm)
          return d.p.ea;
 else //zadata je dilatacija u betonu (pritisnuta ivica)
    {if (d.z.arm)//zadata dilatacija u zategnutoj armaturi
          return interpolacija( d.p.eb, d.z.ea, 1-alfa, alfa );
     else//zadata je dilatacija u zategnutoj ivici betona
          return interpolacija( d.p.eb, d.z.eb,      1, alfa );
    }

}
//------------------------------


//------------------------------------------------------------------------------
/*           nu i mu                    */
float nu_t(float k, float alfab, float s, float alfa, float siga1, float siga2, float sigv, float mi1nad)
{float rez =( k*siga2/sigv + siga1/sigv )*mi1nad; //deo koji potice od armature
       rez+= alfab * s * (1-alfa);   //deo koji potice od betona
 return rez;
}

float mu_t(float k, float alfab, float s, float alfa, float siga1, float siga2, float sigv, float mi1nad, float eta)
{float rez =( k*siga2/sigv + siga1/sigv )*mi1nad *(0.5-alfa); //deo koji potice od armature
       rez+= alfab * s * (1-alfa) * (0.5-eta*s*(1-alfa));   //deo koji potice od betona
 return rez;
}

/*----------------------------------------------------------------------*/
float koef_sig(float eps_a, short slopt)
//vraca koeficijent sigurnosti za zadatu dilataciju u armaturi i slucaj opterecenja:stalno, povremeno
{

  if (eps_a <= -3.0) //zategnuta armatura
     {switch (slopt)
         {case _STALNO:   return 1.6; break;
     	    case _POVREMENO:return 1.8; break;
     	    case _SEIZMIKA: return 0;   break;
         }
     }
  else
     {if (eps_a >= 0) //pritisnuta armatura
         {switch (slopt)
            {case _STALNO:   return 1.9; break;
        	   case _POVREMENO:return 2.1; break;
       	     case _SEIZMIKA: return 0;   break;
          	}
         }
      else//INTERPOLACIJA
         {float interp=(eps_a+3)/3;
          switch (slopt)
            {case _STALNO:   return 1.6+0.3*interp; break;
       	     case _POVREMENO:return 1.8+0.3*interp; break;
     	       case _SEIZMIKA: return 0;   break;
            }
         }
     }
 return 0;// OVAJ SLUCAJ NE SME DA SE DOGODI
}
//--------------------------------------------------------------------------

/*-----------------------------------------------------------------------------*/

float KoefSeizmickeSile( short zona, short kattla, short katobj, BOOL savremena_kja, float T, WINDOW *w)
{
 /*proracun*/
 float ko=(katobj==1?1.5:1),
       ks=(zona==7? 0.025:(zona==8? 0.05:0.1)),
       kp=(savremena_kja? (T<2? 1.0:1.6):1.3 ),
       kd,minkd,koefzakd,k;

 if       (kattla==1) {minkd=0.333; koefzakd=0.5;}
 else{ if (kattla==2) {minkd=0.47;  koefzakd=0.7;}
      else            {minkd=0.6;   koefzakd=0.9;}
     }
 kd=min(1.0, max(koefzakd/T, minkd));
 k=ko*ks*kp*kd;

 if (w)
 {/*stampa*///--------------------------------------------------------------------------------
  short x=2, dx=83, y=2, dy=9;
  char s[40];  WinClr(w); w->CurFont=F_4x6;
           sprintf(s,"%d. kat.objekta:",katobj);      WinStrXY (w,x,   y,s);
           sprintf(s,"ko=%-4.3f",ko);                 WinStrXY (w,x+dx,y,s);
  y+=dy;   sprintf(s,"%d. seizm.zona:",zona);         WinStrXY (w,x,   y,s);
           sprintf(s,"ks=%-4.3f",ks);                 WinStrXY (w,x+dx,y,s);
  y+=dy;   sprintf(s,"%d. kat.tla:",kattla);          WinStrXY (w,x,   y,s);
  y+=dy;   sprintf(s,"%4.3f %c kd=%2.1f/T %c 1  %c",minkd,156,koefzakd,156,22);
                                                      WinStrXY (w,x+11,y,s);
           sprintf(s,"kd=%-4.3f",kd);                 WinStrXY (w,x+dx,y,s);
  y+=dy;   WinStrXY (w,x,y,(savremena_kja?"savremena k.ja:":"k.ja od AB zidova:"));
       if (savremena_kja)
          {sprintf(s,"T%c2s %c",(T<2?'<':'>'),22);    WinStrXY (w,x+60,y,s);
           sprintf(s,"kp=%-4.3f",kp);
          }
      else sprintf(s,"kp=%-4.3f",kp);                 WinStrXY (w,x+dx,y,s);
  y+=dy+2; sprintf(s,"k= ko%cks%ckd%ckp =%-4.3f %c 0.02",215,215,215,k,(k>0.02?158:60));
                                                      WinStrXY (w,x,   y,s);
  y+=dy;   sprintf(s,"k=%-4.3f",max(k,0.02));         WinStrXY (w,x+dx,y,s);
 }

 k=max(k,0.02);
 return k;
}