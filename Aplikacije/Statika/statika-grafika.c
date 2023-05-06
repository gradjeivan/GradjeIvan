#include <tigcclib.h>
#include "statika-grafika.h"

/*                                    F U N K C I J E                                           */
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=*/
void CrtezOsvezi (void)
{BrisiCrtez(); pc->rezim=A_NORMAL;
 CrtajNosac();
 CrtajOkvirCrt();
}//--------------------------------------------------------------------------------

/*void CrtajOpterecenje (Duz * d)
{

}*/
void CrtajSimbolVeze(short x, short y, short veza)
{ short v=veza-1;
  if (tacka_vidljiva((Tacka_px){x,y},-5))
     {Sprite8 (x-crt[v].x, y-crt[v].y, crt[v].vel, crt[v].and, LCD_MEM, SPRT_AND);//ngetchx();
      Sprite8 (x-crt[v].x, y-crt[v].y, crt[v].vel, crt[v].or,  LCD_MEM, SPRT_OR);
     }
}
void CrtajSveZglobove(void)
{Duz *d=(*kd)[2][1];
 Tacka_px xy;
 while (d!=NULL)
 {short i=0;
 	for (i=0;i<2;i++)
 	    if (MogucePomeranjeVeze(d->tata->gru[i],FALSE,3))
 	       {xy=xy_crt(d->ik[i]->xy[0], d->ik[i]->xy[1]);
          CrtajSimbolVeze(xy.x,xy.y,2);
         }
 	d=d->sused[2][0];
 }
}
//-----------------
void CrtajVezu(Duz *d,short kraj)
{float dmax=2.5,//pretpostavljam da je kraj -i- levo/dole od kraja -k-
       a=ugao_duzi_u_tacki(d,d->ik[kraj]);//{char s[30];sprintf(s,"ugao%d=%f",kraj,a*180/PI);ST_helpMsg(s);ngetchx();}

 if ((pc->vid.veze==1) && (d->tata))//drugi uslov je za situaciju gde jos nismo uneli karakteristike stapa (Unos_stapa)
    {Tacka_px xy=xy_crt(d->ik[kraj]->xy[0], d->ik[kraj]->xy[1]);
     if (MogucePomeranjeVeze(d->tata->gru[0],FALSE,3))
     CrtajSimbolVeze(xy.x +  (short)( dmax * (cos(a)) ),
                     xy.y -  (short)( dmax * (sin(a)) ), 2);
                  /*za y je minus jer su pravci y osa ekrana i crteza suprotni za TI89*/
    }//{char s[25];sprintf(s,"dx=%d,dy=%d",(short)( dmax * (cos(a))),(short)( dmax * (sin(a))));ST_helpMsg(s);ngetchx();}
}
//---------------------------------
void CrtajVezeuTacki(Tacka *t)
{short i;
 for (i=0;i<t->brduzi-1;i++)
     CrtajVezu(t->duz[i],((t==t->duz[i]->ik[0])?0:1));
}
//---------------------------------
void CrtajStap(Duz *d)
{CrtajDuz(d); CrtajVezu(d,0); CrtajVezu(d,1);
}
//--------
void OznaciStap(Duz *d)
{Tacka_px t1=xy_crt(x_i(d),y_i(d));
 Tacka_px t2=xy_crt(x_k(d),y_k(d));
 short xx=0,yy=0;
 float ug=duz_tg(d);
 /*za ugao veci od 45 dodajemo debljinu po y, a za veci ugao dodajemo po x*/
 if (abs(ug>1)) xx=1; else yy=1;
 short i=-1;
 for (i=-1;i<2;i++){
 DrawClipLine (&(WIN_RECT){t1.x+i*xx,t1.y+i*yy,t2.x+i*xx,t2.y+i*yy},
               &(SCR_RECT){{pc->crt[0][0],pc->crt[1][0],
                            pc->crt[0][1],pc->crt[1][1]}},
               pc->rezim);}
 CrtajVezu(d,0);CrtajVezu(d,1);
}
//--------------------------------------------------------------
void CrtajDijagram(Duz *d,short dij,float razmera)
{RaspOpt *djgrm=(dij>=0? Dij_(d,dij) : (RaspOpt *)(d->tata->jos) );
 if (djgrm)//samo ako je dijagram zadat ima si sta da se crta
    {Tacka_px ti=xy_crt(x_i(d),y_i(d));
     Tacka_px tk=xy_crt(x_k(d),y_k(d));
     SCR_RECT crtez={{pc->crt[0][0],pc->crt[1][0],
                   pc->crt[0][1],pc->crt[1][1]}};
     float alfa=duz_alfa(d),
            pi =djgrm->q[0],
            pk =djgrm->q[1];
             //f =Dij_(d,dij)->q[2];
     short  dxi=(short)(razmera * pi * sin(alfa));
     short  dyi=(short)(razmera * pi * cos(alfa));
     short  dxk=(short)(razmera * pk * sin(alfa));
     short  dyk=(short)(razmera * pk * cos(alfa));
     //{char s[30];sprintf(s,"dxi=%d,dyi=%d,dxk=%d,dyk=%d",dxi,dyi,dxk,dyk);ST_helpMsg(s);     }

     /*odredjivanje nagiba srafure*/
     short srafura,rezim=A_NORMAL;
     if ( abs(cos(alfa)) > 0.866025 ) //0-30 stepeni od horizontale
          srafura=A_SHADE_V;
     else {if ( abs(cos(alfa)) < 0.5 )//0-30 stepeni od vertikale
     	         srafura=A_SHADE_H;
     	     else {if ( tan(alfa) > 0 )//I i III kvadrant
     	                srafura=A_SHADE_NS;
     	           else srafura=A_SHADE_PS;
     	          }
          }
      //{char s[30];sprintf(s,"xi=%d,yi=%d,xk=%d,yk=%d",ti.x,ti.y,tk.x,tk.y); ST_helpMsg(s);}
     /*linearni dijagram*///sastoji se iz dva trougla
      DrawClipLine (&(WIN_RECT){ti.x    , ti.y    , ti.x+dxi, ti.y+dyi},&crtez,rezim);
      DrawClipLine (&(WIN_RECT){ti.x+dxi, ti.y+dyi, tk.x+dxk, tk.y+dyk},&crtez,rezim);
      DrawClipLine (&(WIN_RECT){tk.x+dxk, tk.y+dyk, tk.x    , tk.y    },&crtez,rezim);
      /*srafura-crtanje*/
       if ((pi/pk)<0)//razliciti znaci na krajevima:trouglovi I-pi-NULA i NULA-K-pk
          { short nulax=( ti.x + (cos(alfa)>0?1:-1) * abs((pi/(pi-pk))*(tk.x-ti.x))),
                  nulay=( ti.y - (sin(alfa)>0?1:-1) * abs((pi/(pi-pk))*(tk.y-ti.y)));
            //{char s[30];sprintf(s,"x0=%d,y0=%d",nulax,nulay); ST_helpMsg(s);}
            FillTriangle (ti.x,  ti.y, ti.x+dxi, ti.y+dyi, nulax, nulay, &crtez, srafura);
            FillTriangle (nulax,nulay, tk.x+dxk, tk.y+dyk,  tk.x,  tk.y, &crtez, srafura);
          }
       else //isti znaci na krajevima:trouglovi I-pi-pk i xi-pk-K
          { FillTriangle (ti.x,  ti.y, ti.x+dxi, ti.y+dyi, tk.x, tk.y, &crtez, srafura);
            FillTriangle (ti.x+dxi, ti.y+dyi, tk.x+dxk, tk.y+dyk, tk.x, tk.y, &crtez, srafura);
          }
    }
}

float CrtajSveDijagrame(short dij)
{short maxpx=7;//maksimalna vrednost ce da bude LCD_HEIGHT/maxpx
 float razmera=(pc->crt[1][1]-pc->crt[1][0]) / (maxpx * maks_opt_za_stanje(dij));
 Duz *d=(*kd)[0][0];
 while (d)
 {CrtajDijagram(d,dij,razmera);
 	d=d->sused[0][1];
 }
 return razmera;
}
/*void GrUsloviInfoCrtez(short tip)
{

}*/

void CrtajNosac(void)/*modifikacija funkcije Crtaj() iz <geometrija-ti.h>*/
{ if ((pc->rezim!=A_XOR)||(pc->rezim!=A_REVERSE))azuriraj_param_crtanja();//azuriranje ne treba za brisanje
  //CrtajOkvirCrt(pc);
  if ((((*kt)[0][0])!=NULL)&&(pc->vid.tacke==1))//ako ima unetih tacaka i vidljivost je =1 crtamo ih
     {Tacka *t=(*kt)[0][0];CrtajSveTacke(t);//CrtajGabarit(pc);
     }//ngetchx();*/
  if (pc->vid.duzi==1)//ako ima unetih duzi i vidljivost je =1 crtamo ih
     {Duz *d=(*kd)[2][0];
      while (d!=NULL)
            {CrtajStap(d);
             d=d->sused[2][1];
            }//ngetchx();
     }
  if (pc->vid.grid==1)PrikaziGrid(A_NORMAL);
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
