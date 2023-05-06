#include "beton-tipovi.h"
/*-----------------------------------------------------------------------------*/
float     mbu          (DijagramInterakcije di, Dilatacije_PP d);
float     zb_bezdim    (float ea1,float eb2,float alfa);
float     nbu          (DijagramInterakcije di, Dilatacije_PP d);
float     alfab        (float eb2);
float     eta          (float eb2);
float     s_nl         (float eb2, float ea1);

float     nau          (DijagramInterakcije di, Dilatacije_PP d );
float     mau          (DijagramInterakcije di, Dilatacije_PP d );

float     sigma_v      (short arm);
float     sigma_a      (float epsa, short armatura);
float     epsilon_v    (short arm);
float     eps_a1       (Dilatacije_PP d, float alfa);
float     eps_a2       (Dilatacije_PP d, float alfa);

//float     nu_t         (float k, float alfab, float s, float alfa, float siga1, float siga2, float sigv, float mi1nad);
//float     mu_t         (float k, float alfab, float s, float alfa, float siga1, float siga2, float sigv, float mi1nad, float eta);

float     koef_sig     (float eps_a, short slopt);
//float     Su           (float S[2],float eps_a1);

float KoefSeizmickeSile( short zona, short kattla, short katobj, BOOL savremena_kja, float T, WINDOW *w);