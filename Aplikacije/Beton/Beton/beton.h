#define COMMENT_STRING         "GradjeIvan BAB"
#define COMMENT_PROGRAM_NAME   "GradjeIvan BAB"
#define COMMENT_VERSION_STRING "0"
#define COMMENT_VERSION_NUMBER 0,2,2,5 /* major, minor, revision, subrevision */
#define COMMENT_AUTHORS        "Radovanovic Ivan"

#include <tigcclib.h>

#include "id kalkulatora.h"
#include "dijalozi.h"
#include "standardno.h"

#include "beton-tipovi.h"
#include "beton-dijalozi.h"
#include "beton-info.h"
#include "beton-proracun.h"
#include "pgbk2-dijalozi.h"
#include "dijag-interakcije.h"
#include "di-dijalozi.h"

enum GlMeni{ _FAJL=1,       _NOVI=11,        _OTVORI=12,        _SACUVAJ=13,       _IZLAZ=19,
             _OSNOVNO=2,    _MATERIJAL=21,   _OPTERECENJE=22,   _SEIZMIKA_UNOS=23, _ZGRADA_UNOS=24,
             _SG=3,         _SG_IZMENA=31,   _SG_PP=32,         _SG_PRESSILE=33,   _SG_DIMENZ=34,
             _ANALIZA=4,    _POMERANJE=41,   _PROBIJANJE=42,    _IZVIJANJE=43,     _SEIZMIKA_ANLZ=44,
             _ALATI=5,      _DIJ_INT=51
           };
#define GLAVNI_MENI_ENUM //potrebno da se izbegne enum u gradjeivan-enums

RasporedRazmaci *rr;
ABKonstrukcija *kja;