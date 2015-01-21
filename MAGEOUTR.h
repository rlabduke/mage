/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*                                MAGEOUTR.h                                  */

/*                  PDB format and PDB standard order Libraries               */
/*                  for output of rotated coordinates */
/*  PDB v3.0 names  071004 */

#ifdef  EXTERNOUTR
#undef  DECLARATIONS
#define DEFINITIONS  /*storage and initializations done for suiteninit.c*/
#undef  EXTERNOUTR
#define EXTERNOUTR   /*no prefix, so do definition */
#else
#undef  DEFINITIONS
#define DECLARATIONS /*just declare existance for everybody*/
#undef  EXTERNOUTR
#define EXTERNOUTR extern  /*extern prefix so just declaration*/
#endif

#ifdef DEFINITIONS
#endif /*DEFINITIONS*/

#ifdef DECLARATIONS
#endif /*DECLARATIONS*/


EXTERNOUTR char OUTRalternate[32][4];

/*prototypes*/
void writeOUTRotordered(void);


