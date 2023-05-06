// C Source File
// Created 14.9.2015; 4:05:30

#include <tigcclib.h>

void Zgrada()
{

/*CRTEZ iniciraj sve---------------------------------------------*/
 //pamtim crtez iz vise instance, a iniciram novi za ovu funkciju
  ParamCrt *pc_stari=pc, parametri_crtanja; pc=&parametri_crtanja;
  exttck   *kt_stari=kt, krajnjetacke;      kt=&krajnjetacke;  nuliraj_exttck();
  extduz   *kd_stari=kd, krajnjeduzi;       kd=&krajnjeduzi;   nuliraj_extduz();
  //Crtez_ucitaj(&parametri_crtanja,&krajnjetacke,&krajnjeduzi);
  InicParamCrtanja();
  pc->rr.lista_desno=1;
  pc->rr.menuH=0;
  azuriraj_param_crtanja();
  CrtajOkvirCrt();
 /*---------------------------------------------------------------*/

  /*Unos grida*/
  if (!(UnesenGrid()))
     {  uspesno=Grid_podesavanje();
        if ((pc->rezim!=A_XOR)||(pc->rezim!=A_REVERSE))azuriraj_param_crtanja();//azuriranje ne treba za brisanje
     }
  if(!(uspesno))return FALSE;//odustajanje prilikom unosa grida


}