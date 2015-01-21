/*MAGECLON.H*/ 
#ifdef EXTERNCLON
#undef  EXTERNCLON
#define EXTERNCLON
#else
#define EXTERNCLON extern
#endif
 
/*MAGECLON.c routines*/
EXTERNCLON void clonegroupsubgroups(grupstruct*);
EXTERNCLON void clonesubgrouplists(sgrpstruct*);
EXTERNCLON void clonelistpoints(liststruct*);
EXTERNCLON void repositionthispoint();
