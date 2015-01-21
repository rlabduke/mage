                       /*MAGEFOO.h*/
#ifdef EXTERNFOO
#undef EXTERNFOO
#define EXTERNFOO
#else
#define EXTERNFOO extern
#endif

/*constants*/
#define SIMPLECUBIC        1
#define FACECENTERCLOSEST  2
#define HEXAGONALCLOSEST   3

#define FOOXM  2048
#define FOOXP  1024
#define FOOYM   512
#define FOOYP   256
#define FOOZM   128
#define FOOZP    64
#define TRYXM    32
#define TRYXP    16
#define TRYYM     8
#define TRYYP     4
#define TRYZM     2
#define TRYZP     1

/*varibles*/
EXTERNFOO unsigned long  FOOGRID;
EXTERNFOO unsigned long  FOOPOO;
EXTERNFOO unsigned long  FOOMOO;
EXTERNFOO unsigned long  FOOPPO;
EXTERNFOO unsigned long  FOOMPO;
EXTERNFOO unsigned long  FOOPMO;
EXTERNFOO unsigned long  FOOMMO;
EXTERNFOO unsigned long  FOOPPP;
EXTERNFOO unsigned long  FOOMPP;
EXTERNFOO unsigned long  FOOOMP;
EXTERNFOO unsigned long  FOOOPM;
EXTERNFOO unsigned long  FOOPMM;
EXTERNFOO unsigned long  FOOMMM;
EXTERNFOO unsigned long  TRYPOO;
EXTERNFOO unsigned long  TRYMOO;
EXTERNFOO unsigned long  TRYPPO;
EXTERNFOO unsigned long  TRYMPO;
EXTERNFOO unsigned long  TRYPMO;
EXTERNFOO unsigned long  TRYMMO;
EXTERNFOO unsigned long  TRYPPP;
EXTERNFOO unsigned long  TRYMPP;
EXTERNFOO unsigned long  TRYOMP;
EXTERNFOO unsigned long  TRYOPM;
EXTERNFOO unsigned long  TRYPMM;
EXTERNFOO unsigned long  TRYMMM;
EXTERNFOO unsigned long  ALLFOO;
EXTERNFOO unsigned long  ALLTRY;
EXTERNFOO unsigned long  SEALED;

EXTERNFOO float xo,zo,yo,yp,ym;
EXTERNFOO int   Lslimemode; /*030709*/
EXTERNFOO int   LFOOinBOX; /*100818*/

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

EXTERNFOO struct pointSet dotsphereset;

/*prototypes*/
EXTERNFOO int  mobilehitfoocheck(void);
EXTERNFOO float distancef(float, float, float, float, float, float);
EXTERNFOO float distancefsq(float, float, float, float, float, float);/*041210*/
EXTERNFOO int  cagehitfoocheck(float, float, float);
EXTERNFOO int  allhitfoocheck(float*, float*, float*, int); /*030709*/
EXTERNFOO int  oldfoohere(float, float, float);
EXTERNFOO void releasefoos(void);
EXTERNFOO void foogridconstruct(void);
EXTERNFOO int  foodotsurface(void); 
EXTERNFOO int  foofillcomputation(void);
EXTERNFOO void foosealer(pointstruct*);
EXTERNFOO void annealmobilelist(void);
EXTERNFOO void getmobilelist(void);
EXTERNFOO void repeatmobiledocking(void);
EXTERNFOO int  gridFOOBOXbounds(float,float,float,float,float,float);/*100818*/

/* dcr,mez,jmw: dots.c : dotSphere(),freeDotSphere(),estNumDots(),makeDots()*/
EXTERNFOO void dotSphere(pointSet *set, float radius, float density);
EXTERNFOO void freeDotSphere(pointSet *set);
EXTERNFOO int  estNumDots(float radius, float density);
EXTERNFOO int  makeDots(float radius, point3dn points[], int maxpnts);

