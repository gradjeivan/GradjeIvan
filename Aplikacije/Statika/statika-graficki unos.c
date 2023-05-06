#include <tigcclib.h>
#include "statika-graficki unos.h"



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
//      info:1-matrice, 2-opterecenja, 3-verescagin, 4-elasticne tezine
Duz * Izaberi_stap(Duz * du,short info,short stanje)
{/*crtez fokus na sve*/
 float razmera=0;//razmera za crtanje dijagrama
 pc->zum=  ZUM;pc->dok[0]=0;pc->dok[1]=0;
 BrisiCrtez();pc->rezim=A_NORMAL; CrtajNosac();
 if (((info==2)||(info==3))||(info==4))razmera=CrtajSveDijagrame(stanje);

 Duz * d=(du?du:(*kd)[2][1]);//da izbegnemo address error u sl. da je du==NULL
 char s[40];
 short tipka=0; //short i=0;
 BOOL nastavi=TRUE;
 while (nastavi)
 { /*podebljaj/oznaci aktuelnu duz*/
   OznaciStap(d);
   /*Ispisujemo osnovne informacije o stapu*/
   unsigned short ipp=((d->tata))->pp;
   unsigned short im =((d->tata))->mat;
   /* INFO 1 i 2 *///
   switch (info)//popuni info 2
   {case 2: InfoOpterecenja(d,stanje);break;
    case 3: InfoOpterecenja(d,stanje);break;
    case 4: InfoOpterecenja(d,stanje);break;
    case 5: InfoStabilnost (d);       break;
    default:InfoKarakteristikeStapa(((d->tata))->ppkonst,ipp,im);
            InfoGranicniUslovi(((d->tata))->tip);
    break;
   }//ST_helpMsg("Gotov Info 2");ngetchx();
   //PAZNJA NA BROJ ZNAKOVA!!!
   /* INFO 3 */
   short i; s[0]='\0'; for(i=0;i<37;i++){strcat(s," ");};
   Info(3,0,0,s,A_REPLACE);Info(3,0,0.5,s,A_REPLACE);//brisem rep od prethodnog teksta
   switch (info)
   {case 3:/*verescagin*/sprintf(s,"%c=%f",133,((Veresc *)(Jos_(d)))->ukupno);
   	break;
   	case 4:/*el.tezine*/
   	      {float konc[2];
   	       M_u_konc(d,stanje,konc);
   	       sprintf(s,"Ri=%f",konc[0]);
   	      }
   	break;
    default://duzina, redukovane duzine
        	  sprintf(s,"L=%-^5.4f",l_d(d));
            if (((d->tata))->ppkonst)//konstantan popr.presek
                 sprintf(s,"%s, L'=%-^5.4f, L\"=%-^5.4f",s,
                        ((ppres+uppp)->I/(ppres+ipp)->I)*l_d(d),((ppres+uppp)->I/(ppres+ipp)->F)*l_d(d));
            else sprintf(s,"%s, I,F%cconst",s,157);
    break;
   }Info(3,0,0,s,A_REPLACE);

   /* STATUSNA LINIJA*/
   switch (info)
   {case 3:/*Verescagin*///sprintf(s,"ENTER:ceo racun");//Ovo cemo ipak iz vise funkcije
    break;
    case 4:
          {float konc[2];
   	       M_u_konc(d,stanje,konc);
   	       sprintf(s,"Rk=%f",konc[1]);
          }
    break;
    default:{if (((d->tata))->ppkonst)//konstantan popr.presek
                    sprintf(s,"%c=%-^4.3f%c,  I=%-^1.4r,  F=%-^1.4r",128,
                           (180/PI)*duz_alfa(d),176,(ppres+ipp)->I,(ppres+ipp)->F);
             else   //promenljiv poprecni presek
                    sprintf(s,"%c=%-^4.3f%c,  I,F =%cconst",128,
                           (180/PI)*duz_alfa(d),176,157);
            }
    break;
   }
   ST_helpMsg(s);

   /*cekanje na tipke*/
   ST_busy(0); tipka=0;
 	 while ((tipka!=KEY_ENTER)&&(tipka!=KEY_ESC)/*Izlaz na strelice,ENTER i ESC*/
 	      &&(tipka!=344)      &&(tipka!=338)    &&(tipka!=337)&&(tipka!=340))
           {tipka=ngetchx();}
   /*povratak starog crteza duzi i promena aktuelne duzi*/
   pc->rezim=A_REVERSE;OznaciStap(d);
   pc->rezim=A_NORMAL;  CrtajDuz(d); CrtajVezeuTacki(d->ik[0]); CrtajVezeuTacki(d->ik[1]);
   if ((info==2)||(info==3)) CrtajDijagram(d,stanje,razmera);

   switch (tipka)
   {case DESNO:if (d->sused[0][1]!=NULL)d=d->sused[0][1];break;
   	case LEVO:  if (d->sused[0][0]!=NULL)d=d->sused[0][0];break;
   	case GORE:  if (d->sused[1][1]!=NULL)d=d->sused[1][1];break;
   	case DOLE:  if (d->sused[1][0]!=NULL)d=d->sused[1][0];break;
   	default:break;
   }   //i++;char s[20];sprintf(s,"loop:%d",i);ST_helpMsg(s);//ngetchx();

   //d=duz_sledeca(d,tipka);
   if (tipka==KEY_ESC){d=NULL;nastavi=FALSE;}
   if (tipka==KEY_ENTER){nastavi=FALSE;}
 }//ST_helpMsg("Kraj izbora duzi");ngetchx();
 return d;
}//----------------------------------------------------------------------------------------

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*========================= IZBOR TACKE =============================================*/
Tacka * IzaberiUnetuTackuNosaca(Tacka *t)
//Vraca ili pokazivac na izabranu tacku, ili NULL ako je odustato:) =ESC
{pc->zum=ZUM; Tacka_stv xy;              //Tacka_px xypx=xy_crt(x_(t),y_(t),pc);//koord. tacke na crtezu
 short tipka=0;
 do
 {/*prvo da potvrdimo/odredimo pocetnu tacku 	 */
  xy.x=x_(t);xy.y=y_(t);//krecemo od zadate tacke
  OznaciTacku(xy.x,xy.y,t->id,1);//invertuj izabranu tacku na crtezu

 	tipka=0; //unosimo sledecu tipku
 	while (((tipka!=344)&&(tipka!=338))&&((tipka!=337)&&(tipka!=340))
 	     &&((tipka!=KEY_ESC)&&(tipka!=KEY_ENTER)&&(tipka!=265)&&(tipka!=278)))
 	         tipka=ngetchx();//dok se ne unese strelica,ESC,ENTER ili APPS
	OznaciTacku(xy.x,xy.y,t->id,1);//normalizuj gore invertovanu tacku,
	                  //kad je u pitanju pocetna tacka duzi, bice ponovo invertovana u f-ji za unos duzi

  /*kretanje kroz vec unete tacke(tipke:strelice)*/
  if  ((*kt)[0][0]!=NULL) {t=IzaberiIzUnetihTacaka(t,&tipka);}

 }while ((tipka!=KEY_ESC/*ESC*/)&&(tipka!=KEY_ENTER/*ENTER*/));

 if (tipka==KEY_ESC/*ESC*/) t=NULL;
 return t;
}/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
/*========================= IZBOR TACKE =============================================*/
Tacka * IzaberiTackuNosaca(Tacka *t,short tackaporedu)
//Vraca ili pokazivac na izabranu tacku, ili NULL ako je odustato:) =ESC
{pc->zum=ZUM; Tacka_stv xy;              //Tacka_px xypx=xy_crt(x_(t),y_(t),pc);//koord. tacke na crtezu
 short ix=0,iy=0,tipka=0;
 do
 {/*prvo da potvrdimo/odredimo pocetnu tacku 	 */
  if (pc->vid.grid==0){xy.x=x_(t);xy.y=y_(t);}//ako grid nije vidljiv krecemo od zadate tacke
  else //grid je vidljiv
     {if ((*kt)[0][0]==NULL)//ako nema tacaka krecemo od donje leve u gridu
           { xy.x=pc->grid.x0; xy.y=pc->grid.y0;} //
      else {//ima tacaka-da li je zadata u gridu?
            if (!(t_deo_grida(t,&ix,&iy)))//zad.tacka nije deo grida
                 { xy.x=pc->grid.x0; xy.y=pc->grid.y0;} //krecemo od donje leve u gridu
            else { //zadata tacka je deo grida, ix tacaka desno, iy gore, pocev od donje levo
                   xy.x=x_(t);    xy.y=y_(t); //char s[27];printf(s,"(%4f,%4f)u gridu(%d,%d)",xy.x,xy.y,ix,iy);ST_helpMsg(s);ngetchx();
     }     }     }
  OznaciTacku(xy.x,xy.y,t->id,tackaporedu);//invertuj izabranu tacku na crtezu

 	tipka=0; //unosimo sledecu tipku
 	while (((tipka!=344)&&(tipka!=338))&&((tipka!=337)&&(tipka!=340))
 	     &&((tipka!=KEY_ESC)&&(tipka!=KEY_ENTER)&&(tipka!=265)&&(tipka!=278)))
 	         tipka=ngetchx();//dok se ne unese strelica,ESC,ENTER ili APPS
	OznaciTacku(xy.x,xy.y,t->id,tackaporedu);//normalizuj gore invertovanu tacku,
	                  //kad je u pitanju pocetna tacka duzi, bice ponovo invertovana u f-ji za unos duzi

  /*unosenje iz grida(tipke:strelice)*///NE IDE U SWITCH BLOK!!!
 	if (pc->vid.grid==1)
 	   {xy=IzaberiIzGrida(ix,iy,&tipka,tackaporedu);
         	if (tipka==KEY_ENTER)/*izabrana tacka-*/
         	{if ((t=tacki_trazi_duplikat((float [2]){xy.x,xy.y}))==NULL)//t pokazuje na duplikat ili ima vrednost NULL ako nema takve tacke
         	    { if ((*kt)[0][0]==NULL)
         	           tacku_dodaj('A'              ,(float[2]){xy.x,xy.y});//dodajemo PRVU tacku
         	      else tacku_dodaj((*kt)[2][1]->id+1,(float[2]){xy.x,xy.y});//dodajemo NOVU
         	      t=(*kt)[2][1];
         	      //pc->rezim=A_NORMAL; CrtajTacku(t->xy[0],t->xy[1],t->id,1);
     }   	}   }
  /*kretanje kroz vec unete tacke(tipke:strelice)*/
  else {if  ((*kt)[0][0]!=NULL) {t=IzaberiIzUnetihTacaka(t,&tipka);}
        //else
       }
  /*provera ostalih tipki: APPS*/
  switch (tipka)
         {case 265:/*APPS*///promena moda unosa grid/postojece tacke
                pc->vid.grid=promeni10(pc->vid.grid);
                if ((*kt)[0][0]==NULL) pc->vid.grid=1;//vracam se na grid ako nema unetih tacaka
                else {BrisiCrtez();pc->rezim=A_NORMAL;Crtaj();}//crtez sa/bez grida
          break;
          case 278:/*CATALOG*//*Dijalog za unos tacke*///ST_helpMsg("CATALOG");ngetchx();
               {short oket=0; Tacka unos; unos.xy[0]=unos.xy[1]=0;
                if ((*kt)[2][1]!=NULL) unos.id=(*kt)[2][1]->id+1;
                else                   unos.id='A';
             	  Unos_tacke_dlg(&unos,&oket);

             	  if (oket==1)
             	     {tipka=KEY_ENTER;
             	      tacku_dodaj(unos.id,unos.xy);    	  t=(*kt)[2][1];
                    BrisiCrtez();//pc.rezim=A_XOR;Crtaj(&pc,&kt);
                    pc->rezim=A_NORMAL;CrtajNosac();
                   }
                //else tipka=KEY_ESC;
	             }
          break;
         }
 }while ((tipka!=KEY_ESC/*ESC*/)&&(tipka!=KEY_ENTER/*ENTER*/));

 if (tipka==KEY_ESC/*ESC*/) t=NULL;
 return t;
}/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*/
