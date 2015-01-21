/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*#include "MAGEFLAG.H"*/  /*960808*/
 
/* 2 byte integer: "xyzflag" == xyz(0,j); TYPE*/
/*type flags*/
#define PRUNED      16384  /*Group,Subgroup,List,point type*/
#define MARKTYPE     8192  /*pointtype, listtype */
#define DETAILFLAG   4096  /*Listtype only, overload*/
#define INVENTED     4096  /*pointtype only, overload*/
#define LENSFLAG     2048  /*List,Group,Subgroup type*/
#define STORED       2048  /*pointtype only, overload*/
#define NOZCLIPFLAG  1024  /*Listtype only, overload*/
#define BALLHERE     1024  /*pointtype only, overload*/
#define UNPICKABLE    512  /*pointtype, listtype overload*/
#define MOVETO_P      256  /*pointtype only, overload, MOVE TO Point*/ 

#define FLOATFLAG     128  /*grouptype, overload*/
#define VARIANT3      128  /*pointtype, listtype overload*/
#define TABLEFLAG      64  /*grouptype only, overload*/ 
#define VARIANT1       64  /*pointtype, listtype overload*/
#define TABLECOLHEAD   32  /*grouptype only, overload*/ 
#define VARIANT2       32  /*pointtype, listtype overload*/
#define WORDS          16  /*pointtype, listtype overload*/
#define LABEL           8  /*pointtype, listtype overload*/
#define BALL            4  /*pointtype, listtype overload*/
#define DOT             2  /*pointtype, listtype overload*/
#define VECTOR          1  /*pointtype, listtype overload*/

/*define some objects as variants of more primative, faster drawn objects*/
#define SPHERE   (BALL   | VARIANT1)            /*pointtype, listtype overload*/
#define DOTBALL  (BALL   | VARIANT2)            /*pointtype, listtype overload*/
#define TRIANGLE (VECTOR | VARIANT1)            /*pointtype, listtype overload*/
#define RIBBON   (VECTOR | VARIANT1 | VARIANT2) /*pointtype, listtype overload*/
#define ARROW    (VECTOR | VARIANT3)            /*pointtype, listtype overload*/
/*32767: 15 bits all 1's*/
#define ANTIVARIANT (~(VARIANT1+VARIANT2+VARIANT3)) /*invoked by level-of-detail*/
extern int     Listtype;  /*declared in MAGEINPT.c*/
extern int     Liststyle;  /*declared in MAGEINPT.c*/
extern int     LLensflagset; /*dclared in MAGEINPT.c*/ /*970917*/
extern int     LLenson; /*declared in MAGEINPT.c*/ /*971109*/
extern char    masterchar; /*declared in MAGEINPT.c*/ /*991210*/
extern int     Leavingflatland; /*declared in MAGEMENU.c*/ /*991117*/

/*unsigned int STATUS flags*/
#define PRUNED      16384  /*pruned state: groups,subgroups,lists,points*/
#define NOXYZ        8192  /*pointstatus: no xyz coord, e.g. table cell*/
#define DUMMY        4096  /*pointstatus: not used in scaling */
#define STATICFLAG   2048  /*List, can inherit from sgrp and grup 121211*/
#define STARTFLAG    1024  /*Point that starts triangle,ribbon sequence of pts*/
#define NOBLKRIMFLAG 1024  /*List ball,sphere not have a black rim drawn*/
#define NOHILITEFLAG  512  /*List ball,sphere not have a high-light drawn*/ 
#define HASPTMASTER   256  /*List has points that have a point master */

#define SCREENFLAG    128  /*List, can inherit from sgrp and grup 121211*/
#define GHOST          64  /*pointstatus: seen but not for some output (PDB)*/ 
#define ZLIMITED       32  /*list obeys extra z limit clipping planes 011201*/
#define NEVERSHOW      16  /*0: created in .kip output  */
#define RECESSIVEON     8  /*implies DOMINANT when OFF, 0:no change ON vs OFF */
#define DOMINANT        4  /*0: RECESSIVE shows buttons of subsets*/
#define NOBUTTON        2  /*0: BUTTON */
#define ON              1  /*display ON/OFF state, 0: OFF*/

/*integer colorwidth member of a point, color in bits 1-5, width in bits 6,7,8*/
/*#define FORCEPTCOLOR 16384 */ /*pointptr->colorwidth color dominant, 15th bit*/
/*make part of point's STYLE; this field ued in  expanded colordepthwidthtype*/

/* 2 byte integer: "colordepthwidthtype"
 it(0,ncount) carries from MAGEDRAW 3D 1st inner to 2D list 2nd inner loop
                 0,     1,     2,     3,     4,     5,     6,     7 
                     2048,  4096,  6144,  8192, 10240, 12288, 14336 
       vector 0,ball sph ,triang, label, word , ribb ,  dot ,nohilte 

extra types:     8,     9,    10,    11,    12,    13,    14,    15
             16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720
          MARKTYPE, RING ,  FAN , ARROW,DIAMOND,SQUARE,EMPHASIS,NOBLKRIM

- 32768
- 16384  8 point type code (shift >>11, mask 15) 4 bit new usage: 000804
-  8192  4 point type code (shift >>11, mask  7) old 3 bit codes:
-  4096  2 "     1,     2,     3,     4,     5,     6,     7
-  2048  1 "  2048,  4096,  6144,  8192, 10240, 12288, 14336
-  1024  4 pen width code (shift >>8, mask 7)
-   512  2 "
-   256  1 "
-   128  4 depth cue code (shift >>5, mask 7) colordepth mask 255
-    64  2 "    1    2    3    4    5    6    7  only 1-5 match depthcues
-    32  1 "   32,  64,  96, 128, 160, 192, 224
-    16 color code (mask 31)
-     8 "
-     4 "
-     2 "
-     1 "
*/
/*unsigned int STYLE flags*/
#define FORCEPTCOLOR 16384  /* */
#define UNUSED8192    8192  /* */
#define SQUARESTYLE   4096  /* */
#define DIAMONDSTYLE  2048  /* 010916 */
#define FANSTYLE      1024  /*pointtype, listtype */
#define RINGSTYLE      512  /*pointtype, listtype */
#define DOTBALLSTYLE   256  /*pointtype, listtype 100406*/

#define SPACED         128  /*masterptr 060212 */
#define ENDBALLSTYLE    64  /*pointtype, listtype 100407 */ 
#define PIXELATED       32  /* ribbon,triangle listtype 080912 */
#define ELLIPSOID       16  /*pointtype, listtype 080406 */
#define REARSTYLE        8  /*vector liststyle 090703 zdepth at rear point */
#define FORESTYLE        4  /*vector liststyle 090703 zdepth at fore point */
#define UNUSED2          2  /* */
#define EMPHASIS         1  /* */

/*090703 REARSTYLE,FORESTYLE vector zbuffer at an end, NOT usual midpoint*/
/*090703 helps put black outline on ribbon vector worms.*/
 
/*int flag for group assigned to be a docking group*/
/*thisgrupptr->bondrot == DOCKSET */
#define DOCKSET      8192  /* */

/*unsigned int KKIND flags*/ /*030312 established for KKIND of grup,sgrp,list*/
/*KKIND unique search string fragment; 130309 presume unsigned int >16bits */
#define SCALEDKKIND    32768  /*130309 SCALED GROUP: ->scaled*dist picked pts*/
#define IGNORABLEKKIND 16384  /* */
#define DIMENSIONKKIND  8192  /*060121 for 7D rnabb datapoints, ND pts */
#define SELECTINGKKIND  4096  /*060121 for 7D rnabb datapoints, ND pts */
#define WRAPPINGKKIND   2048  /*060525 for wrap <0 values, e.g. 360 */
#define AVERAGEKKIND    1024  /*061129 list average, i.e. mean, of a cluster */
#define SIGMASKKIND      512  /*061129 list sigma lines: extent of a cluster */
#define FOOKKIND         256  /* */

#define ALTERNATEKKIND   128  /*071005 for alternate bases on suitefit */
#define FOOBARRIERKKIND   64  /*090912 foobar == foo barrier == NOT surface */ 
#define CENTEREDKKIND     32  /*130115 CENTERED GROUP uses gnomon mechanism */
#define NOEDISTANCESKKIND 16  /* */
#define PIPEINPUTKKIND     8  /* */
#define PREKINAPPENDKKIND  4  /* */
#define MAPCONTOURSKKIND   2  /* */
#define PROBEDOTSKKIND     1  /* */

#ifdef ARCHIVEOLDSYSTEM
/* Listtype: */
/*#define LABEL    40 */
/*#define WORDS    45 */
/*#define DOT      20 */
/*#define BLOT     21 */  /*990209 de-blot*/
/*#define BALL     25 */
/*#define SPHERE   26 */
/*#define TRIANGLE 30 */
/*#define RIBBON   32 */
/*#define VECTOR    0 */
/*960808*/ /*MAGEFLAG.H*/

 0  VECTOR
 1  VECTOR          UNPICKABLE
 2  VECTOR                     INVENTED          50 STORED (instead of INVENTED)
 3  VECTOR          UNPICKABLE INVENTED          51 STORED (instead of INVENTED)
 4  VECTOR                     INVENTED BALLHERE 52 STORED (instead of INVENTED)
 5  VECTOR          UNPICKABLE INVENTED BALLHERE 53 STORED (instead of INVENTED)


10  VECTOR MOVETO_P
11  VECTOR MOVETO_P UNPICKABLE
12  VECTOR MOVETO_P            INVENTED          60 STORED (instead of INVENTED)
13  VECTOR MOVETO_P UNPICKABLE INVENTED          61 STORED (instead of INVENTED)
14  VECTOR MOVETO_P            INVENTED BALLHERE 62 STORED (instead of INVENTED)
15  VECTOR MOVETO_P UNPICKABLE INVENTED BALLHERE 63 STORED (instead of INVENTED)

20  DOT
/*21  DOT    VARIANT1  (BLOT (esv only ??)*/  /*990209 de-blot*/

25  BALL

26  BALL   VARIANT1  (SPHERE)

30  VECTOR          VARIANT1          (TRIANGLE)

31  VECTOR MOVETO_P VARIANT1          (TRIANGLE
                , P only operative when reverted to VECTOR)
32  VECTOR          VARIANT1 VARIANT2 (RIBBON
                , has all TRIANGLE properties except extra lighting step)
33  VECTOR MOVETO_P VARIANT1 VARIANT2 (RIBBON
                , P only operative when reverted to VECTOR)
40  LABEL

45  WORDS

#endif /*ARCHIVEOLDSYSTEM*/
