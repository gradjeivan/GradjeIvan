// Header File
// Created 29.12.2014; 22:33:33

// Header File
//NE RADI KAKO TREBA

#ifndef STANDARDNO
#define STANDARDNO

//globalne promenljive
#define F_EPS FLT_EPSILON //za TI je ovako, FLT_EPSILON varira zavisno od uredjaja

//Prototipovi
float rel_razl(float a, float b);
BOOL  fjdnk   (float a, float b);
BOOL  fveci   (float a, float b);
BOOL  fmanji  (float a, float b);

short fznak   (float x);

#endif