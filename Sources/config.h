#include "svnrev.h"
#define VERSION_MAJOR	0
#define VERSION_MINOR	2
#define VERSION_PATCH	2
#define VERSION_REV		SVN_REV

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define VERSION_STRING TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "." TOSTRING(VERSION_PATCH) "." TOSTRING(VERSION_REV)

/* STV09xx specific */
//#define STV090X_HWINIT
//#define AUTO_REPEATER
#define SET_TUNER_DELAY 5
//#define FORCE_AUTOTUNER_CUT20
//#define FIX_DVBCI_CLOCK
