/*MATRICE*///--------------------------------------------------------------------------------------
#ifndef MATRICE_TIPOVI
#define MATRICE_TIPOVI

/* gornja trougaona Matrica M[n-vrste][m-kolone] */
typedef struct
{short   n;//broj vrsta
 short   m;//broj kolona
 float * M;//matrica mxn
}MatricaGT;/*Ista bi bila i definicija matrice, ali cisto da se zna da je trougaona*/

typedef struct
{short   n;
 short   m;
 float * M;
}Matrica;

#endif