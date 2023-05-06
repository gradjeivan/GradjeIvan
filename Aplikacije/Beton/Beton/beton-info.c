// C Source File
// Created 13.02.2015; 4:39:39

#include <tigcclib.h>
#include "beton-info.h"

void AzurirajInfo()//DijagramInterakcije di, float M[], float N[], float b,float d,float h, short mb)
{
 char s[30];
 short lin=0;
 float uvuci=.2;
 //short info=2,red=-1;

 InfoFill (1,0,0,A_REVERSE,rr);//----------------------------------------------
 //armatura,MB
  sprintf(s,(kja->arm == _RA? "RA 400/500":"GA 240/360"));   Info(1,0,0,s,A_NORMAL,rr);
  sprintf(s,"MB%d",((short[]){MARKA})[kja->mb] ); Info(1,1,0,s,A_NORMAL,rr);

 InfoFill(2,0,0,A_REVERSE,rr);//-----------------------------------------------
 ABPresek     * abp=kja->abp;
 AB_GredaStub *  gs=kja->ags;
 PoprecniPresek *pp=NULL;
          if(abp)pp=abp->pp;
 if(gs)
 {
  lin=0; sprintf(s,"POS %d:"   ,  gs->id );     Info(2,lin++,0    ,s,A_NORMAL,rr);
         sprintf(s,"L=%4.2f"   ,  gs->L  );     Info(2,lin++,uvuci,s,A_NORMAL,rr);
  lin++; sprintf(s,"presek %c" , abp->id );     Info(2,lin++,0    ,s,A_NORMAL,rr);
         sprintf(s,"x=%4.2f"   , abp->ksi);     Info(2,lin++,uvuci,s,A_NORMAL,rr);
  lin++; sprintf(s,"pp:tip %d" ,  pp->id );     Info(2,lin++,0    ,s,A_NORMAL,rr);
  switch(pp->tip)
       {
       	case _PROIZVOLJAN:  {
       // sprintf(s,"F[cm2]");                 Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"F=%-5.2f",pp->pp.proiz.F);   Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"I[cm4]");                 Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"I=%-8.3R",pp->pp.proiz.I);   Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"J[cm4]");                 Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"J=%-8.3R",pp->pp.proiz.J);   Info(2,lin++,uvuci,s,A_NORMAL,rr);
       	                    }break;
      	case _PRAVOUGAONI:  {
        //sprintf(s,"b");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"b=%-5.2f",pp->pp.pravoug.b); Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"d");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"d=%-5.2f",pp->pp.pravoug.d); Info(2,lin++,uvuci,s,A_NORMAL,rr);
      	                    }break;
      	case _KRUZNI:       {
        //sprintf(s,"R");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"R=%-5.2f",pp->pp.pravoug.b); Info(2,lin++,uvuci,s,A_NORMAL,rr);
      	                    }break;
      	case _T_PRESEK:     {
        //sprintf(s,"b");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"b=%-5.2f",pp->pp.Tpresek.b); Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"d");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"d=%-5.2f",pp->pp.Tpresek.d); Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"B");                      Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"B=%-5.2f",pp->pp.Tpresek.B); Info(2,lin++,uvuci,s,A_NORMAL,rr);
        //sprintf(s,"dp");                     Info(2,lin++,0,s,A_NORMAL,rr);
        sprintf(s,"dp=%-5.2f",pp->pp.Tpresek.dp);Info(2,lin++,uvuci,s,A_NORMAL,rr);
        	                  }break;
       	default:             break;
       }
 }

}


//-----------------------------------------------------------------------------
/*
 switch(pp->tip)
       {
       	case _PROIZVOLJAN:  {

       	                    }break;
      	case _PRAVOUGAONI:  {

      	                    }break;
      	case _KRUZNI:       {

      	                    }break;
      	case _T_PRESEK:     {

      		                  }break;
       	default:             break;
       }
*/