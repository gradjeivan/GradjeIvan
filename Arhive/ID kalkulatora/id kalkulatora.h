// Header File
// Created 18.11.2014; 14:33:17
#ifndef IDCALC
#define IDCALC

#define GRADJEIVAN "gradjeivan@gmail.com"
#define NIJE_REG   "Program nije registrovan za ovaj kalkulator."


BOOL registrovan_kalkulator(char **serbr, BOOL izlaz);
/*dve varijante koriscenja:
       - izlaz=FALSE, serbr=NULL
                kad ne zelimo da imamo ser.br. van funkcije
       - izlaz=TRUE, serbr=char**
                ako se zeli ser.br. van funkcije,
                tad je 1.argument je stvarno char**,
                i mora u glavnom programu na kraju
                da se oslobodi memorija na koju pokazuje
*/

#endif
