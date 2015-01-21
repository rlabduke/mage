                       /*MAGEBALL.h*/
#ifdef EXTERNBALL
#undef EXTERNBALL
#define EXTERNBALL
#else
#define EXTERNBALL extern
#endif


EXTERNBALL void  makeDotBallAtxyz(void);
EXTERNBALL void  makeInteriorDotsInvisible(void);

#ifdef REPLACEFOO

/*dotSphere stuff */
/* swiped from MAGEFOO.h vintage mage.6.54.091212 on 100406 */
/* from probe.2.8.011009 dots.h Copyright (C) 1999 J. Michael Word    */
/*NOTE: extention to point3d ---> point3dn to hold an interger flag...*/

/*#define PI 3.14159265359*/
/*#include "geom3d.h"*/

typedef struct point3dn {
      double x, y, z;
      int number;
}point3dn;

typedef struct pointSet {
        int n;      /* number of three-dimensional points */
        point3dn *p; /* array of points */
}pointSet; 

EXTERNBALL void dotSphere(pointSet *set, float radius, float density);
EXTERNBALL void freeDotSphere(pointSet *set);
EXTERNBALL int  estNumDots(float radius, float density);
EXTERNBALL int  makeDots(float radius, point3dn points[], int maxpnts);

EXTERNBALL struct pointSet dotsphereset;

#endif 
