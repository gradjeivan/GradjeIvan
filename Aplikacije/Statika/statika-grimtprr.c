#include <tigcclib.h>
#include "statika-grimtprr.h"

/*=============================================================================*/
/*==================== OPERACIJE NAD MATRICAMA ================================*/
//------------------------------------------------------------------------------
void FrmtTab_Izbaci(FrmtTab *ft, short ind, short vk)
{short i,*nm[2]={&(ft->n),&(ft->m)};
 for (i=0; i < (vk==2? 2:1); i++)
 {short dnm=(vk==2? i:vk);
 	*(nm[dnm])=*(nm[dnm])-1;
  short j;
  for (j=0; j<*(nm[dnm]); j++)
      {if (j>=ind)
         {strcpy (((ft->fk)[j]).frmt,     ((ft->fk)[j+1]).frmt);
          strcpy (((ft->fk)[j]).zaglavlje,((ft->fk)[j+1]).zaglavlje);
          (ft->pmem)[j]=(ft->pmem)[j+1];
         }
      }
 }

}
/*
void Mat_Zameni_vk(Matrica *M,short i[2][2])
{short j;

}
*/
/*
void Mat_Zameni_Vr_ili_Kol(Matrica *M,short i[2],short vk)
{*//*zamena samo vrsta:vk=0, ili samo kolona:vk=1,...; nema za obe dimenzije istovremeno*/
/* float pom;
 short j;
 if (vk==0)//zamena vrsta
      for (j=1; j<=M->m; j++)
          {pom=*(eM(M,i[0],j));
 	     	   *(eM(M,i[0],j))=*(eM(M,i[1],j));
 	     	   *(eM(M,i[1],j))=pom;
          }
 else //zamena kolona
      for (j=1; j<=M->n; j++)
          {pom=*(eM(M,j,i[0]));
 	     	   *(eM(M,j,i[0]))=*(eM(M,j,i[1]));
 	     	   *(eM(M,j,i[1]))=pom;
          }

}*/

void FrmtTab_Zameni(FrmtTab *ft,short i[2],short vk)
{/*vk mora biti 0(zamena vrsta) ili 1(kolona)*/
 char *pom;
 pom=ft->fk[i[0]].frmt;
 ft->fk[i[0]].frmt = ft->fk[i[1]].frmt;
 ft->fk[i[1]].frmt = pom;

 pom=ft->fk[i[0]].zaglavlje;
 ft->fk[i[0]].zaglavlje = ft->fk[i[1]].zaglavlje;
 ft->fk[i[1]].zaglavlje = pom;
 /*
 if (vk==0)//zamena vrsta
          {

          }
 else //zamena kolona
      for (j=1; j<=ft->n; j++)
          {
          }
 }
*/
}

/*
void Mat_LinKombinuj_vk(Matrica *M, short i[2], short koef[2], short vk)
{short j;
 if (vk==0)
     for (j=1; j<=M->m; j++)
          *(eM(M,i[0],j))= koef[0] * (*(eM(M,i[0],j))) + koef[1] * (*(eM(M,i[1],j)));
 else
     for (j=1; j<=M->n; j++)
          *(eM(M,j,i[0]))= koef[0] * (*(eM(M,j,i[0]))) + koef[1] * (*(eM(M,j,i[1])));

}*/
//---------------------------------------------------------------------------------------------

/*======================================================================================*/
/*==================== STAMPANJE MATRICE (TABLICNO)=====================================*/
//---------------------------------------------------------------------------------------
void StampajMatricu(Matrica *M,char **zaglavlje,WINDOW *w)
{//WINDOW *w=NoviGlavniProzor(pc->crt[0][0],pc->crt[1][0],LCD_WIDTH,pc->crt[1][1]+KOMAND);
 char naslov[]="MATRICA"; w->Title=naslov;
 //FrmtTab ft; ft.w=w;  // FrmtTab ft={M->n,M->m,M->M,fk,pmem,w};
 char **s=NULL; char pmem[M->m];//,frmt[]="% |4.5G ";
 short poc_dim[2]={M->n,M->m};//potrebno za oslobadjanje memorije s
 FrmtTabKol fk[M->m];
 Matrica_PripremaTabKol(M, zaglavlje, &s, pmem, fk);
 FrmtTab ft={M->n,M->m,M->M,fk,pmem,w};

 Matrica *pM=Nova0Matrica(M->n,M->m);
 matrici_dodeli_vr_druge(pM,M);//pM moze da se menja, M cuvamo neostecenu u ovoj f.ji

 OperacijaTab *op=NULL,*ops=NULL;
 while ((ops=tabela(&ft))!=NULL)
  {op=ops;
   while (op)
   {switch(op->promena)
    {case _LINKOMB:
               {short j;
                for (j=0; j<2; j++)
                     Mat_LinKombinuj_vk( pM, (short[2]){(op->oprtr[0])+1,(op->oprtr[1])+1}, op->koef, j );
                ft.el=pM->M;
               }
           break;
     case _ZAMENI:
             {short j;
              for (j=0; j<2; j++)/* 2 prolaza ako je vr+kol*/
                  Mat_Zameni_Vr_ili_Kol(pM,(short[2]){(op->oprtr[0])+1,(op->oprtr[1])+1},j);
             	//	Mat_Zameni_Vr_ili_Kol(pM,(short[2]){(op->oprtr[0])+1,(op->oprtr[1])+1},op->vk);
              ft.el=pM->M;
              FrmtTab_Zameni (&ft,(short[2]){(op->oprtr[0])  ,(op->oprtr[1])  },op->vk);
             }
           break;
     case _IZBACI:
             {//char s[40];sprintf(s,"n:%d,m:%d",pM->n,pM->m);ST_helpMsg(s);ngetchx();
              Matrica *privremena=NULL;
              privremena=Izbaci_VrKol_IzMat(pM,1,(short[1]){((op->oprtr[0])+1)},op->vk);
              M_brisi(pM);pM=Nova0Matrica(privremena->n,privremena->m);
              matrici_dodeli_vr_druge(pM,privremena);
              ft.el=pM->M;
              FrmtTab_Izbaci(&ft,(op->oprtr)[0],op->vk);
              /*ne brisemo privremenu, ona je presla u pM*/
             }
           break;
     case _IZVEZI:IzveziMatricu(M,op->izvoz);
           break;
    }
    op=ops->sledeca[1];
   }
   free(ops);
  }

 if(pM)M_brisi(pM);
 if(s)
 {short i;
  for (i=0; i<poc_dim[1]; i++) if(s[i]) free(s[i]);
  free(s);
 }
 WinClose(w);
}//-------------------------------------------------------------------------------
/*=============== priprema matrice za tablicu ===============================*/
//---------------------------------------------------------------------------------
void Matrica_PripremaTabKol(Matrica *M, char **zaglavlje, char ***s, char *pmem, FrmtTabKol *fk)
{short j;
 if (zaglavlje==NULL)//automatsko zaglavlje
    {*s=(char**)calloc(M->m,sizeof(char*));
     for (j=0;j<(M->m);j++)
          {(*s)[j]=(char*)calloc(1,sizeof(char));
           sprintf((*s)[j],"  %|d",j+1);
          }
    }
 //popuna fk
 for (j=0;j<(M->m);j++)
     {fk[j]=(FrmtTabKol){(char *)"% ^4.5G ",(zaglavlje==NULL?(*s)[j]:zaglavlje[j])};
      pmem[j]='f';
     }
 //ft->n=M->n; ft->m=M->m; ft->el=M->M; ft->fk=fk; ft->pmem=pmem;
}
