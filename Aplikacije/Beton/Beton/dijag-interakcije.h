#include "beton-tipovi.h"
#include "beton-proracun.h"
#include "dijalozi.h"

#define FNULA 0.000001 //float nula

#define DI_BAB_K_UKUPNO 6                                      //broj razlicitih k u tablicama
#define DI_BAB_K        1, 0.75, 0.5, 0.4, 0.25, 0.1           //k za koje postoje tablice u BAB-u
#define DI_BAB_RBR      1,  1  ,  9 ,  23,  31 ,  39           //redni broj dijagrama u BAB-u za zadato k

#define DI_BAB_ALFE   0.05, 0.075, 0.1, 0.125, 0.15, 0.2, 0.25 //moguce vrednosti alfa u tablicama

//oblast u okviru kojih nam je dilatacija zat.armature (za koef.sigurnosti)
enum Oblast { _ZAT_A1=0, _PRIT_A1=1, _INTERPOLACIJA=2};

enum DIMeni{ _DJG      =1, _ALFA    =11,  _ODNOS_A1A2=12, _KARLIN=13,
             _NUMU     =3, _NM      =31,  _BDH       =32, _KOEFSIG =33, _POVOLJNOG=34,
             _PRORACUN =4
           };
/*-----------------------------------------------------------------------------*/
void          Iniciraj_DI_meni (HANDLE *gmeni);

void          mu_nu            (float mnu[2][2], float mi1, float A[2]);

BOOL          DI_BAB_ime       ( DijagramInterakcije *di, char *ime);
BOOL          DI_nadji_mnu     ( float nuDI[9][8], float muDI[9][8], float nu, float mu, short *klin,short *mi1 );
void          DI_epsilon_linija( DijagramInterakcije di,  Dilatacije_PP d, float mnu[2][2] );
Dilatacije_PP DI_Karakt_linija ( DijagramInterakcije di, char linija, float koef[2][2] );

void          Tabela_nu_mu_prr ( DijagramInterakcije di, float nu[][8], float mu[][8]);
void          DI_Crtaj         ( DijagramInterakcije di, WINDOW *w);
float         DI_mi            ( DijagramInterakcije di,float nu,float mu, char prit_granica_oblasti);

char          LinijaOblasti_mnu( DijagramInterakcije di, float nu, float mu);
BOOL          LinijaEpsilon_mnu( DijagramInterakcije di, float nu, float mu, float epsA1[2], short brint );
short         StanjePP_Stampa  ( DijagramInterakcije di, Dilatacije_PP d, WINDOW *w, short y);