// C Source File
// Created 17.02.2015; 4:18:24

#include <tigcclib.h>
#include "di-dijalozi.h"

BOOL Izbor_karaktlin (char *klin)
{ short izbor;
  const char* spisak[]={"(a):-10/-10","(b):0/-10","(c):3.5/-10","(d):3.5/-3",
                        "(e):3.5/-ev","(f):3.5/0(ea)","(g):3.5/0(eb)","(h):2/2"};
      izbor=IskacuciMeni(8,spisak,"Karakteristicne linije");
  if (izbor!=0) *klin='a'+izbor-1;
  return (izbor!=0);
}
//--------------------------------------------------------------------------------
BOOL Izbor_alfa(DijagramInterakcije *di)
{ char stavka[7];
  float f;
  short izbor;
  BOOL sedamalfi=((di->k==1)||(di->k==0.5));

  if (sedamalfi)
     { const char* spisak[]={"0.05","0.075","0.1","0.125","0.15","0.2","0.25"};
       izbor=IskacuciMeni(7,spisak,"Izbor a/d");
       if(izbor)strcpy(stavka,spisak[izbor-1]);
       else return FALSE;
     }
  else
     { const char* spisak[]={"0.05","0.1","0.15","0.2"};
       izbor=IskacuciMeni(4,spisak,"Izbor a/d");
       if(izbor)strcpy(stavka,spisak[izbor-1]);
       else return FALSE;
     }

  if (txtuf(stavka,&f))
     { di->alfa=f;
       return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------
BOOL Izbor_armature(short *armatura)
{ short izbor= IskacuciMeni (2,(const char* []){"RA 400/500","GA 240/360"},"Izbor armature");
  if (izbor)
     {*armatura=(izbor==1? _RA:_GA);
       return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------
BOOL Izbor_markebetona(short *mb)
{ const char *spisak[10]={"MB 15","MB 20","MB 25","MB 30","MB 35","MB 40","MB 45","MB 50","MB 55","MB 60"};
  short izbor= IskacuciMeni (10,spisak,"Marka betona");
  if (izbor)
     {*mb=izbor-1;
       return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------
BOOL Izbor_k_odnosa(float *k_arm)
{
  const char* spisak[6]={"1","0.75","0.50","0.40","0.25","0.10"};
  char stavka[6];
  float f;
  short izbor= IskacuciMeni (6,spisak,"Aa2/Aa1");

  if (izbor==0) return FALSE;
  else strcpy(stavka,spisak[izbor-1]);

  if (txtuf(stavka,&f))
     { *k_arm=f;
        return TRUE;
     }
  else return FALSE;

}
//--------------------------------------------------------------------------------
BOOL Izbor_delovanjaG(BOOL *povoljnoG)
{ char spisak[2][12];
  sprintf(spisak[0],"nepovoljno");// (%c:1.6,1.8)",134,131);
  sprintf(spisak[1],"povoljno");// (%c:1.9,2.1)",134,131);
  short izbor= IskacuciMeni (2,(const char* []){spisak[0],spisak[1]},"Delovanje G");
  if (izbor)
     {if (izbor==1) *povoljnoG=FALSE;
      else          *povoljnoG=TRUE;
      return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------'
BOOL Izbor_kombinacije(short *komb)
{ char spisak[3][20];
  sprintf(spisak[0],"automatski");
  sprintf(spisak[1],"1.3%c(G+0.5P+S)",215);
  sprintf(spisak[2],"1.0%cG+1.3%c(0.5P+S)",215,215);
  short izbor= IskacuciMeni (3,(const char* []){spisak[0],spisak[1],spisak[2]},"Kombinacija");
  if (izbor)
     {*komb=izbor;
      return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------
BOOL Izbor_koefsig(BOOL *pritisnuta_A1)
{ char spisak[2][20];
  sprintf(spisak[0],"%ca1<-3%%.",134);// (%c:1.6,1.8)",134,131);
  sprintf(spisak[1],"%ca1>0%%." ,134);// (%c:1.9,2.1)",134,131);
  short izbor= IskacuciMeni (2,(const char* []){spisak[0],spisak[1]},"PP.oblast dilatacija");
  if (izbor)
     {if (izbor==1) *pritisnuta_A1=FALSE;
      else          *pritisnuta_A1=TRUE;
      return TRUE;
     }
  else return FALSE;
}
//--------------------------------------------------------------------------------