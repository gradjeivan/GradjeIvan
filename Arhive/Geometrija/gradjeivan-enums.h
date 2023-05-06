#ifndef GLAVNI_MENI_ENUM
#define GLAVNI_MENI_ENUM
enum GlavniMeni_Unosi
{_IZLAZ=99,
 _FAJL      =1,  _NOVI      =11, _OTVORI      =12, _SACUVAJ=13, _SACUVAJ_KAO=14,
 _GEOMETRIJA=2,  _TACKE     =21, _MATERIJAL   =23,
                 _POP_PRESEK=24, _PPRES_IZMENA=241, _PPRES_UPOREDNI=242,
                 _OSLONCI   =26,  _OSL_DODAJ  =261,     _OSL_IZMENI=262,  _OSL_IZBRISI=263,
                 _STAPOVI   =22, _STAP_DODAJ  =221,    _STAP_IZMENI=222, _STAP_IZBRISI=223,
                 _CRTEZ     =25, _MREZA       =251,    _VIDLJIVOST =252, _OSVEZI      =253,
 _NOSAC    =5,
                 _RAVANROSTILJ=51, _NEP_POM=52,
                 _STABILNOST=53,   _STABILNOST_UPOREDNI=531, _STABILNOST_AKSIJALNE=532,
 _MATRICE  =4,  _MAT_K      =41, _MAT_T=42, _MAT_KGLOB=43,  _MAT_KNOSACA=44, _MAT_KNN=45,
 _MATRICE_STAB=46, _MAT_K_STAB=461, _MAT_KG=462, _MAT_K_STAB_NOSACA=463, _MAT_KG_NOSACA=464,
 _DIJAGRAMI=6,  _DIJAG_ZADAJ=61,
        _DIJAG_TEMP_SLEGANJE=62, _DIJAG_TEMP_SLEG_ZADAJ=621, _DIJAG_TEMP_SLEG_SABIRCI=622,
                _DIJAG_GRUPE=63,
              _USL_JEDNACINE=64,
              _SUPERPOZICIJA=65,
                  _EL_TEZINE=66,
                 _VERESCAGIN=600
};
#endif
