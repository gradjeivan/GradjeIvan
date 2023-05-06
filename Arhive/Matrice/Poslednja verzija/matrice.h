#include "matrice-tipovi.h"

short       brel_Tmatrice  (short n);//ukupan broj elemenata u gornjem trouglu, uklj. i dijagonalu

/*Memorija*/
Matrica   * Nova0Matrica   (short n,short m);
MatricaGT * NovaTMatrica   (short n);
void        M_brisi        (Matrica * M);

/*Indeksi i,j,n pocinju od 1, a indeks koji je rezultat pocinje od 0, jer se odnosi na C-niz*/
short       ieM               (short i,short j,short n);//Indeks el.matrice u C-nizu
short       ieGTM             (short i,short j,short n);//Indeks el.trougaone matrice u C-nizu
float     *  eM               (Matrica   * M,short i,short j);//pokazivac na vrednost el. trougaone matrice
float     * eGTM              (MatricaGT * K,short i,short j);//pokazivac na vrednost el. trougaone matrice
Matrica   * GTMuM             (MatricaGT * K);/*Posle ove funkcije obavezno free(M->M);free(M);*///Pretvara trougaonu matricu u pravu matricu

/*Operacije sa matricama*///---------------------------------------------
/*Za funkcije:Uvek prvo dodeliti rezultat nekoj promenljivoj
              da bi kasnije mogao da oslobodis alociranu memoriju*/
Matrica * mnozi_M                 (Matrica   *A,Matrica *B);
Matrica * mnozi_M_skalarom        (Matrica   *A,float skalar);//makro
Matrica * mnozi_GTM_M             (MatricaGT *K,Matrica *T);
void      matrici_dodeli_vr_druge (Matrica   *A,Matrica *B);//makro
Matrica * NovaTransp_M            (Matrica   *M);
//-----------------------------------------------------------------------
Matrica * InverznaMatrica     (Matrica *D);
float **  Matrica_u_Matrix    (Matrica *M);
Matrica * Matrix_u_Matrica    (float **mat,short n,short m);
float **  Matrix_nova         (short n);
void      MatrixInversion     (float **A, int order, float **Y);//prepisano sa neta
//-----------------------------------------------------------------------

MatricaGT * Izbaci_VrKol_IzMatGT(MatricaGT *M,short kom,short *ind,short kv);
Matrica   * Izbaci_VrKol_IzMat  (Matrica *M,  short kom,short *ind,short kv);

BOOL     IzveziMatricuGT        (MatricaGT * GTM,const char *ime);
BOOL     IzveziMatricu          (Matrica *M,const char *ime);

void stampajGTM(MatricaGT * K);
void stampajT  (Matrica   * T);

void     Mat_Zameni_Vr_ili_Kol  (Matrica *M,short i[2],short vk);
void     Mat_LinKombinuj_vk     (Matrica *M, short i[2], short koef[2], short vk);