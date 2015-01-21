                   /*MAGEBALL.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"
#include "MAGEMENU.h"
#include "MAGEANGL.h"
#include "MAGEFOO.h"
#define EXTERNBALL
#include "MAGEBALL.h"


/****makeDotBallAtxyz()*******************************************************/
void makeDotBallAtxyz()
{
   int  icolor;
   float radius;
   float density;
   int   j;

   if(firstdotlistptr == NULL)   /*100407 know where to start hiding ball dots*/
   {
      firstdotlistptr = thislistptr;
   }
   /* This is done after addanothervector has placed a point in current list*/
   /* so we can read from 'this' common point storage to get current info. */
   /*ATOM from a dotlist...*/
   /*make a dot list around thispointptr->fx,fy,fz of */
     icolor = (thispointptr->colorwidth);
     radius = thispointptr->radius;
     if(radius < 0.001) {radius = thislistptr->radius;}  /*121111*/
     density = 16.0;
   /*  with dot density abitrarily set to 10, re FOO*/
   /* or 16, the usual density of molprobity dots...  111024 */

   /*first make the dotball...*/ 
   freeDotSphere(&dotsphereset);

   dotSphere(&dotsphereset, radius, density); /*MAGEFOO.c*/

   /*then move it to current point, which is currently held in common*/
   for(j=0;j<dotsphereset.n;j++)
   {
      dotsphereset.p[j].x = (thispointptr->fx) + dotsphereset.p[j].x; 
      dotsphereset.p[j].y = (thispointptr->fy) + dotsphereset.p[j].y;
      dotsphereset.p[j].z = (thispointptr->fz) + dotsphereset.p[j].z;
   }
   
   for(j=0;j<dotsphereset.n;j++)
   {
      if((thispointptr = allocpointstructure(thislistptr)) == NULL)/*MAGELIST*/
      {/*allocation failed*/
           fprintf(stderr,"failed to alloc a dotball point\n");
      }/*allocation failed*/
      else
      {
          thispointptr->fx = dotsphereset.p[j].x; 
          thispointptr->fy = dotsphereset.p[j].y; 
          thispointptr->fz = dotsphereset.p[j].z; 
          thispointptr->colorwidth = icolor;          
          storeptIDstring(thislistptr->name,thispointptr);
          thislistptr->lastpointptr->nextptr = thispointptr; /*old last ptptr*/
          thispointptr->previousptr = thislistptr->lastpointptr;
          thispointptr->nextptr = NULL; /*new last point*/
          thislistptr->lastpointptr = thispointptr; /*new last ptptr*/
      }
   }
}
/*___makeDotBallAtxyz()______________________________________________________*/

/****makeInteriorDotsInvisible()**********************************************/
void makeInteriorDotsInvisible()
{
   struct liststruct* balldotlistptr;
   struct liststruct* ctrdotlistptr;
   struct pointstruct* balldotpointptr;
   struct pointstruct* ctrdotpointptr;

   balldotlistptr = firstdotlistptr;
   while(balldotlistptr != NULL)
   {/*balldotlistptr*/

      /*for each dot of a ball, */
      /*scan through all points in this list, */
      /*  try distance to ctr of all other lists*/

      balldotpointptr = balldotlistptr->firstpointptr->nextptr;
      while(balldotpointptr != NULL)
      {/*balldotpointptr*/

        /*look at distance to all center dots, i.e. first dot of each dot list*/
         ctrdotlistptr = firstdotlistptr;
         while(ctrdotlistptr != NULL)
         {/*ctrdotlistptr*/

            if(ctrdotlistptr != balldotlistptr)  /*avoid self*/
            {
               ctrdotpointptr = ctrdotlistptr->firstpointptr;
               if(  distancef(
                    balldotpointptr->fx,balldotpointptr->fy,balldotpointptr->fz,
                    ctrdotpointptr->fx,ctrdotpointptr->fy,ctrdotpointptr->fz)
                  < ctrdotpointptr->radius)
               {
                  balldotpointptr->colorwidth = 31; /*invisible*/
               }
            }
            ctrdotlistptr = ctrdotlistptr->nextptr; /*cycle until next is NULL*/
         }/*ctrdotlistptr*/

         if(balldotpointptr == balldotlistptr->lastpointptr)
         {
            balldotpointptr = NULL;
         }
         else
         {
            balldotpointptr = balldotpointptr->nextptr;
         }
      }/*balldotpointptr*/
      balldotlistptr = balldotlistptr->nextptr;
   }/*balldotlistptr*/ 
}
/*___makeInteriorDotsInvisible()_____________________________________________*/

#ifdef REPLACEFOOLIB
/****foodotsurface()**********************************************************/
int foodotsurface() /*foosurface surfacefoo*/
{
   int Nsurfacedots=0;
   int j=0;
   float distsq=0; /*041210*/
   struct pointstruct* beginpointptr=NULL;
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* testpointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   struct pointstruct* thispointptr=NULL;
   float twofoo=0,ctrx=0,ctry=0,ctrz=0;

         ctrx = thepointptr->fx;
         ctry = thepointptr->fy;
         ctrz = thepointptr->fz;
         /*clear flags on stored dotball*/
         for(j=1;j<=dotsphereset.n;j++)
         {
            dotsphereset.p[j].number = 1;
         }


/*code from MAGEDLOG.c */
   freeDotSphere(&dotsphereset);
   dotSphere(&dotsphereset, fooradius, foodotdensity); /*MAGEFOO.c*/
}
/*___foodotsurface()_________________________________________________________*/
/*****************************************************************/
/* dots.c : dotSphere(),freeDotSphere(),estNumDots(),makeDots()  */
/* author: J. Michael Word (port from dcr and mez fortran code)  */
/* date written: 2/20/96                                         */
/* purpose: generate points on a sphere                          */
/*from probe.2.8.011009 and hacked for mage.6.28.030402 by dcr   */
/*****************************************************************/
/* NOTICE: This is free software and the source code is freely   */
/* available. You are free to redistribute or modify under the   */
/* conditions that (1) this notice is not removed or modified    */
/* in any way and (2) any modified versions of the program are   */
/* also available for free.                                      */
/*               ** Absolutely no Warranty **                    */
/* Copyright (C) 1999 J. Michael Word                            */
/*****************************************************************/

void dotSphere(poyntSet *set, float radius, float density) 
{/*entry to construct a dotball*/
   int m;

   if (set) 
   {
      m = estNumDots(radius, density);
      set->n = 0;
      set->p = (poynt3dn *)malloc(m*sizeof(poynt3dn));
      if (set->p) 
      {
         set->n = makeDots(radius, set->p, m);
      }
   }
}

void freeDotSphere(poyntSet *set)
{
   if (set) 
   {
      set->n = 0;
      if (set->p) 
      {
         free(set->p);
         set->p = NULL;
      }
   }
}

int estNumDots(float radius, float density) 
{
#define PI 3.14159265359
   float sizefact = 1.0;

   /* overestimate of the number of dots */
   return (int)floor(4.0 * PI * density * sizefact * (radius * radius));
   /*math.h/floor(number) returns largest integer not > number*/
}

int makeDots(float radius, poynt3dn points[], int maxpnts) 
{
#define PI 3.14159265359
   float offset = (float)0.2;
   double ang, cosang, sinang, phi, theta, xy0, x0, y0, z0;
   int i, j, k, odd, nequator, nvert, nhoriz;

   nequator = (int)floor(sqrt(maxpnts * PI));

   odd = 1;
   ang = 5.0 * PI / 360.0;
   cosang = cos(ang);
   sinang = sin(ang);

   i = 0;
   nvert = nequator / 2;
   for (j = 0; j <= nvert; j++) 
   {
      phi = (PI * j) / nvert;
      z0 = cos(phi) * radius;
      xy0= sin(phi) * radius;

      nhoriz = (int)floor(nequator * sin(phi));
      if (nhoriz < 1) nhoriz = 1;
      for (k = 0; k < nhoriz; k++) 
      {
         if(odd) {theta = (2.0 * PI * k + offset)/nhoriz; }
         else    {theta = (2.0 * PI * k         )/nhoriz; }
         x0 = cos(theta) * xy0;
         y0 = sin(theta) * xy0;

         if (i >= maxpnts) return i;
         points[i].x = x0;
         points[i].y = y0*cosang - z0*sinang;
         points[i].z = y0*sinang + z0*cosang;
         points[i].number = 0;
         i++;
      }
      odd = !odd;
   }
   return i;
}
#endif

