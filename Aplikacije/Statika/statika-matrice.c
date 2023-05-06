#include <tigcclib.h>
#include "statika-matrice.h"

/*LOKALNE FUNKCIJE*///=============================
/*static float     l_d         (Duz *d);

static float l_d(Duz *d) {return sqrt(pow(x_k(d)-x_i(d),2)+pow(y_k(d)-y_i(d),2));}
*///=================================================

//--------------------------
void StampajMatricu_glprozor(Matrica *M,char **zaglavlje)
{
 StampajMatricu(M,zaglavlje,
                NoviGlavniProzor(pc->crt[0][0],pc->crt[1][0],LCD_WIDTH,pc->crt[1][1]+KOMAND));
}


void StampajMatricuGT(MatricaGT *GT,char **zaglavlje)
{Matrica *M=GTMuM(GT);
 StampajMatricu_glprozor(M,zaglavlje);
 free(M->M); free(M);
}
//--------------------------------------------------------------------------------

void StampajMatTrans(BOOL rostilj)
{Duz     *d=(*kd)[2][1];
 Matrica *T=NULL;
 do {d=Izaberi_stap(d,1,0);
     if (d)
        {T=MatTransf(d,rostilj);//stampajT(T);ngetchx();
         if (Pp_(d)->F == 0)//zanemarivanje N-sile:izbacivanje vrsta/kolona u matrici transformacije
          	  ZanemariNsilu_T(&T,((d->tata))->tip,TRUE);
         StampajMatricu_glprozor(T,NULL);
         char izvoz[3]; sprintf(izvoz,"t%c",d->id -48);
         IzveziMatricu(T,(const char *)izvoz);
         free(T->M);free(T);//ngetchx();
         CrtajOkvirCrt();
        }
    }while(d);
 CrtezOsvezi();
}//---------------------------------------------------------------------------------------

//------------------------------------------------------------------------
void StampajMatKrutGlob(BOOL rostilj,short sttb,float uporedni[3])
{Duz      *d=(*kd)[2][1];
 Matrica  *M=NULL;

 do {d=Izaberi_stap(d,1,0);
     if (d)
        {M = MatKrutGlob_Prr(d,rostilj, materijal,ppres,sstap,uppp,upp_mat,sttb,uporedni);
         StampajMatricu_glprozor(M,NULL);
         M_brisi(M);
         CrtajOkvirCrt();
        }
    }while(d);
 CrtezOsvezi();
}//---------------------------------------------------------------------------------------

void MatRavnoteze_Stampa(Duz *d,short duzina)
{/*matrica ravnoteze*///uzeto u obzir eventualno zanemarivanje N u funkciji MatriceRavnoteze
  Matrica *C=MatricaRavnoteze(d,duzina);  //char zaglavlje[3][5]={"Sik=1","Mik=1","Mki=1"};
  if (Pp_(d)->F == 0) ZanemariNsilu_T(&C,d->tata->tip,FALSE);  //eventualno zanemarivanje N-sile u matrici ravnoteze:
  StampajMatricu_glprozor(C,NULL);
  if(C->M)free(C->M);if(C)free(C);
}

void KonstanteStapa_Stampa(float *elFo, float *konst, Stap *s)
{	WINDOW *w = NoviGlavniProzor(pc->crt[0][0],pc->crt[1][0],pc->crt[0][1],pc->crt[1][1]);
  Prr_KonstStapaPromPP(s->tip,elFo,konst,w);
  ngetchx(); WinClose (w);
}