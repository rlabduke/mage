#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEFLAG.h"
#define EXTERNCLON
#include "MAGECLON.h"
 
/****clonegroupsubgroups()***************************************************/
void clonegroupsubgroups(grupstruct* originalgrupptr)
{
   sgrpstruct* originalsgrpptr;
   int j;
   
   /*comes in with global thisgrupptr allocated*/
   /*first clear the new group to be ready to create new subgroups*/
   thisgrupptr->firstsgrpptr = NULL;
   thisgrupptr->lastsgrpptr = NULL;
   /*get starting subgroup*/
   originalsgrpptr = originalgrupptr->firstsgrpptr;
   while(originalsgrpptr != NULL)
   {/*copy each point of the original list into the current list*/
      getnewsubgroup(); /*same routine as regular flow in MAGESETS*/
      if(thissgrpptr != NULL)
      {/*allocation valid for another subgroup */
         thissgrpptr->on = 1; /*do not inherit on-off! start enabled*/
         if(thissgrpptr->master == 0) 
             thissgrpptr->master = originalsgrpptr->master;
         /*copy things that stay the same*/
         thissgrpptr->type = originalsgrpptr->type;
         thissgrpptr->STATUS = originalsgrpptr->STATUS;
         thissgrpptr->balllists = originalsgrpptr->balllists;
         thissgrpptr->bondrot = originalsgrpptr->bondrot;
         /*copy name*/
         for(j=0; j<MAXNAMECHAR+2 ; j++)
            thissgrpptr->name[j] = originalsgrpptr->name[j];
         /*mechanics of allocation should properly set */
         /* grupptr, firstlistptr, lastlistptr */
         /* previousptr, nextptr */
         /*but this is NOT a common memory referenced instance*/
         thissgrpptr->instanceptr = NULL;

         clonesubgrouplists(originalsgrpptr);

         /*advance in originallist to see if there are more points*/
         if(originalsgrpptr == originalgrupptr->lastsgrpptr)
              originalsgrpptr = NULL;
         else originalsgrpptr = originalsgrpptr->nextptr;         
      }/*allocation valid for another subgroup */
   }/*copy each list of the original subgroup into the current subgroup*/
}
/*___clonegroupsubgroups()__________________________________________________*/

/****clonesubgrouplists()****************************************************/
void clonesubgrouplists(sgrpstruct* originalsgrpptr)
{
   liststruct* originallistptr;
   int j;
      
   /*first clear the new subgroup to be ready to create new lists*/
   thissgrpptr->firstlistptr = NULL;
   thissgrpptr->lastlistptr = NULL;
   /*get starting list*/
   originallistptr = originalsgrpptr->firstlistptr;
   while(originallistptr != NULL)
   {/*copy each list of the original subgroup into the current subgroup*/
      getnewlist(); /*same routine as regular flow in MAGESETS*/
      if(thislistptr != NULL)
      {/*allocation valid for another list */
         thislistptr->on = 1; /*do not inherit on-off! start enabled*/
         if(thislistptr->master == 0)
         {
             thislistptr->master = originallistptr->master;
         }
         if(thislistptr->owncolor == 0)
         {
            thislistptr->color = originallistptr->color;
            thislistptr->owncolor = 1; /*now it owns the color*/
         }
         /*copy things that stay the same*/
         thislistptr->STATUS = originallistptr->STATUS;
         thislistptr->STYLE  = originallistptr->STYLE;
         thislistptr->type = originallistptr->type;
         thislistptr->width = originallistptr->width;
         thislistptr->bondrot = originallistptr->bondrot;
         thislistptr->wordsfont = originallistptr->wordsfont;
         thislistptr->wordsface = originallistptr->wordsface;
         thislistptr->wordssize = originallistptr->wordssize;
         thislistptr->radius = originallistptr->radius;
         /*copy name*/
         for(j=0; j<MAXNAMECHAR+2 ; j++)
            thislistptr->name[j] = originallistptr->name[j];
         /*mechanics of allocation should properly set */
         /* sgrpptr, firstpointptr, lastpointptr */
         /* previousptr, nextptr */
         /*but this is NOT a common memory referenced instance*/
         thislistptr->instanceptr = NULL;
         /*button will be set in MAGEBBOX.c*/
         
         clonelistpoints(originallistptr);
         
         /*advance in originalsubgroup to see if there are more lists*/
         if(originallistptr == originalsgrpptr->lastlistptr)
              originallistptr = NULL;
         else originallistptr = originallistptr->nextptr;         
      }/*allocation valid for another list */
   }/*copy each list of the original subgroup into the current subgroup*/
}
/*___clonesubgrouplists()___________________________________________________*/

/****clonelistpoints()*******************************************************/
void clonelistpoints(liststruct* originallistptr)
{
   pointstruct* originalpointptr;
   
   /*first clear the new list to be ready to create new points*/
   thislistptr->firstpointptr = NULL;
   thislistptr->lastpointptr = NULL;
   /*get starting point*/
   originalpointptr = originallistptr->firstpointptr;
   while(originalpointptr != NULL)
   {/*copy each point of the original list into the current list*/
      if( (thispointptr = allocpointstructure(thislistptr))!=NULL)/*MAGELIST*/
      {/*allocation valid, can add another vector */
         /*copy things that stay the same*/
         thispointptr->STATUS = originalpointptr->STATUS;
         thispointptr->STYLE  = originalpointptr->STYLE;
         thispointptr->type = originalpointptr->type;
         thispointptr->colorwidth = originalpointptr->colorwidth;
         thispointptr->radius = originalpointptr->radius;
         /*reuse reference to names and words*/
         thispointptr->nameoffset = originalpointptr->nameoffset;
         thispointptr->wordoffset = originalpointptr->wordoffset;
         /*copy in old coordinates, these may be repostioned below*/
         thispointptr->fx = originalpointptr->fx;
         thispointptr->fy = originalpointptr->fy;
         thispointptr->fz = originalpointptr->fz;
         /*mechanics of allocation should properly set */
         /* previousptr, nextptr, listptr */
         
         repositionthispoint(); /*for local rotations, centerings, ... */
                       /*also makes ix,iy,iz integer coords */         
         
         /*advance in originallist to see if there are more points*/
         if(originalpointptr == originallistptr->lastpointptr)
              originalpointptr = NULL;
         else originalpointptr = originalpointptr->nextptr;
      }/*allocation valid, can add another vector */
   }/*copy each point of the original list into the current list*/
}
/*___clonelistpoints()______________________________________________________*/

/****repositionthispoint()***************************************************/
void repositionthispoint()
{
   float   x[4],xx[4],xxx[4];

   x[1] = thispointptr->fx;  
   x[2] = thispointptr->fy;
   x[3] = thispointptr->fz;  
        /*local rotation and centering */
        if(Llocalprecenter)
        {    /*acts like pickcenter*/
             /*use this with x,y,z to do rotation about axis through*/
             /* a point not at 0,0,0 : see Llocalpostcenter below */
             xx[1] = x[1] - fxcenterold[0];
             xx[2] = x[2] - fycenterold[0];
             xx[3] = x[3] - fzcenterold[0];
        }
        else
        {
             xx[1] = x[1];
             xx[2] = x[2];
             xx[3] = x[3];        
        }
        if(Llocalrotation)
        {
            a11 = ma[0][1];
            a12 = ma[0][2];
            a13 = ma[0][3];
            a21 = ma[0][4];
            a22 = ma[0][5];
            a23 = ma[0][6];
            a31 = ma[0][7];
            a32 = ma[0][8];
            a33 = ma[0][9];
            xxx[1] = (float)(xx[1]*a11 + xx[2]*a21 + xx[3]*a31);
            xxx[2] = (float)(xx[1]*a12 + xx[2]*a22 + xx[3]*a32);
            xxx[3] = (float)(xx[1]*a13 + xx[2]*a23 + xx[3]*a33);
            xx[1] = xxx[1];
            xx[2] = xxx[2];
            xx[3] = xxx[3];
        }
        if(Llocalpostcenter)
        {    /*further recentering, use this with -x,-y,-z to do */
             /* rotation around point x,y,z */
             xx[1] = xx[1] - fxcenterold[MAXRESETS+1];
             xx[2] = xx[2] - fycenterold[MAXRESETS+1];
             xx[3] = xx[3] - fzcenterold[MAXRESETS+1];
        }

        /*pointstructure alloc above in addanothervector()*/
        if(xx[1]>=0.0) thispointptr->ix = (int)(xx[1] + .5);
        else           thispointptr->ix = (int)(xx[1] - .5);
        if(xx[2]>=0.0) thispointptr->iy = (int)(xx[2] + .5);
        else           thispointptr->iy = (int)(xx[2] - .5);
        if(xx[3]>=0.0) thispointptr->iz = (int)(xx[3] + .5);
        else           thispointptr->iz = (int)(xx[3] - .5);

        thispointptr->fx = xx[1];  /*971011*/
        thispointptr->fy = xx[2];
        thispointptr->fz = xx[3];
        if(Lscale && ((thispointptr->STATUS & DUMMY)!=DUMMY))
        {
            if(xx[1] < fxmin) fxmin = xx[1];
            if(xx[1] > fxmax) fxmax = xx[1];
            if(xx[2] < fymin) fymin = xx[2];
            if(xx[2] > fymax) fymax = xx[2];
            if(xx[3] < fzmin) fzmin = xx[3];
            if(xx[3] > fzmax) fzmax = xx[3];

            lastscaledpointptr = thispointptr;
            nscale = nxyz; /*still used as a counter*/
        }
}
/*___repositionthispoint()__________________________________________________*/
