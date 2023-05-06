#include <tigcclib.h>
#include "statika-infos.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                  INFO-i                                               */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void InfoStabilnost(Duz *d)
{ //RaspOpt * djgr= stanje>=0 ? Dij_(d,stanje) : (RaspOpt *)(d->tata->jos);
  InfoFill(2,A_REVERSE);
  char s[35]; short lin=0;//linija u Info-u od koje pocinje ispis

  sprintf(s,":::: P ::::");                        Info(2,lin++,0,s,A_REPLACE);
  if (d->tata->jos) sprintf(s,"%|7.3f", *((float *)(d->tata->jos)));
  else              sprintf(s,PRAZAN_UNOS);
                                                   Info(2,lin++,0,s,A_REPLACE);lin++;

}//---------------------------------------------------------------------------------


void InfoGranicniUslovi(short tip)
{InfoFill(1,A_NORMAL);
 char s[10];
 switch (tip)
 {case 1:	sprintf(s,"|---K---|"); break;
  case 2: sprintf(s,"o---G---|"); break;
  case 3: sprintf(s,"|---G---o"); break;
  case 4: sprintf(s,"%c-prost-%c",22,21); break;
  case 5: sprintf(s,"|---S---|"); break;
  default:sprintf(s,"nepoznat"); break;
 }
 Info(1,0,0.1,s,A_REVERSE);
}
void InfoKarakteristikeStapa(BOOL konst,short ipp,short imat)
{ InfoFill(2,A_REVERSE);
  char s[35]; short lin=0;//linija u Info-u od koje pocinje ispis

  if (konst){sprintf(s,"   Ic/I"); Info(2,lin++,0,s,A_REPLACE);}
  else      {sprintf(s,"    I,F"); Info(2,lin++,0,s,A_REPLACE);}

  if  (ppres==NULL)//nema nijednog unetog poprecnog preseka
      {sprintf(s,PRAZAN_UNOS);      Info(2,lin++,0   ,s,A_REPLACE);lin++;
       sprintf(s,"  F/Ic");           Info(2,lin++,0.05,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);      Info(2,lin++,0   ,s,A_REPLACE);
      }
  else{if (konst)
           {sprintf(s,"%|7.3f", (*(ppres+uppp)).I/(*(ppres+ipp)).I); Info(2,lin++,0,s,A_REPLACE);lin++;
            sprintf(s,"  F/Ic");                                     Info(2,lin++,0,s,A_REPLACE);
            sprintf(s,"%|7.3f", (*(ppres+ipp)).F/(*(ppres+uppp)).I); Info(2,lin++,0,s,A_REPLACE);
           }
  	   else{sprintf(s,"%cconst",157);   Info(2,lin++,0,s,A_REPLACE);
  	   	    lin++;
  	   	    sprintf(s,"  tip:%d",ipp+1);Info(2,lin++,0,s,A_REPLACE);
  	   	    lin++;
   	       }
      }
  lin++;
  sprintf(s,SEPARATOR);     Info(2,lin++,0   ,s,A_REPLACE);//lin++;
  sprintf(s," E,v,%c",128);       Info(2,lin++,0.1 ,s,A_REPLACE);//lin++;
  if  (materijal==NULL)//nema nijednog unetog poprecnog preseka
      {sprintf(s,PRAZAN_UNOS);  Info(2,lin++,0   ,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);  Info(2,lin++,0   ,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);  Info(2,lin++,0   ,s,A_REPLACE);
      }
  else{sprintf(s,"%-^1.3R", (*(materijal+imat)).E);    Info(2,lin++,0,s,A_REPLACE);
       sprintf(s,"%-^1.3R", (*(materijal+imat)).ni);   Info(2,lin++,0,s,A_REPLACE);
       sprintf(s,"%-^1.3R", (*(materijal+imat)).alfa); Info(2,lin++,0,s,A_REPLACE);
      }
}
void InfoStepeniSlobode(void)
{InfoFill(2,A_REVERSE);
 char s[35]; short lin=5;
 short br =       tacaka_ukupno(); sprintf(s," K=%-2d", br);    Info(2,lin++,0.1 ,s,A_REPLACE);
 /*short m =krutih_uglova_ukupno();*/ //sprintf(s," m=%-2d", br); Info(2,lin++,0.05,s,A_REPLACE);
       br =         duzi_ukupno(); sprintf(s,"zs=%-2d",br);     Info(2,lin++,0.05,s,A_REPLACE);
/*       br =         duzi_ukupno();*/ //sprintf(s,"zk=%-2d",br); Info(2,lin++,0   ,s,A_REPLACE);
/*       br =         duzi_ukupno();*/ //sprintf(s,"zo=%-2d",br); Info(2,lin++,0   ,s,A_REPLACE);
/*       br =         duzi_ukupno();*/ //sprintf(s,"zu=%-2d",br); Info(2,lin++,0   ,s,A_REPLACE);
 //sprintf(s," n=%-2d", m+2*K-zo );Info(2,5,0,s,A_REPLACE);
 ST_stack(1,1);/*br.stat.nepoznatih/br.uslova ravnoteze*/
}//----------------------------------------------------------------------------------
void InfoOpterecenja(Duz *d,short stanje)
{ RaspOpt * djgr= stanje>=0 ? Dij_(d,stanje) : (RaspOpt *)(d->tata->jos);
  InfoFill(2,A_REVERSE);
  char s[35]; short lin=0;//linija u Info-u od koje pocinje ispis
  sprintf(s,":::: pi ::::"); Info(2,lin++,0,s,A_REPLACE); //ST_helpMsg("PitStop");ngetchx();
  if  (Dij_(d,stanje)==NULL)//nema nijednog unetog poprecnog preseka
      {sprintf(s,PRAZAN_UNOS);       Info(2,lin++,0   ,s,A_REPLACE);lin++;
       sprintf(s,"::: pk ::::");     Info(2,lin++,0.05,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);       Info(2,lin++,0   ,s,A_REPLACE);lin++;
       sprintf(s,":::: f ::::");     Info(2,lin++,0.05,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);       Info(2,lin++,0   ,s,A_REPLACE);lin++;
       sprintf(s,":::: %c ::::",128);Info(2,lin++,0.05,s,A_REPLACE);
       sprintf(s,PRAZAN_UNOS);       Info(2,lin++,0   ,s,A_REPLACE);lin++;
      }
  else{sprintf(s,"%|7.3f", djgr->q[0]);   Info(2,lin++,0,s,A_REPLACE);lin++;
       sprintf(s,":::: pk ::::");         Info(2,lin++,0,s,A_REPLACE);
       sprintf(s,"%|7.3f", djgr->q[1]);   Info(2,lin++,0,s,A_REPLACE);lin++;
       sprintf(s,":::: f ::::");          Info(2,lin++,0,s,A_REPLACE);
       sprintf(s,"%|7.3f", djgr->q[2]);   Info(2,lin++,0,s,A_REPLACE);
       sprintf(s,":::: %c ::::",128);     Info(2,lin++,0.05,s,A_REPLACE);
       sprintf(s,"%|7.3f", djgr->alfa);   Info(2,lin++,0,s,A_REPLACE);
      }
}//---------------------------------------------------------------------------------


