/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
                              /*MAGELIST.c*/
 
#include "MAGE.h"
#define   MAGELIST 
#include "MAGELIST.h"    /*incudes MAGELIST.HDR prototypes*/
#undef    MAGELIST 
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"


/****destroydimensionstructure()**********************************************/
void destroydimensionstructure(dimensionstruct* victimptr)
{
   if(victimptr != NULL)
   {/*structure was allocated*/
      free(victimptr);  
   }/*structure was allocated*/
}
/*___destroydimensionstructure()_____________________________________________*/

/****allocdimensionstructure()************************************************/
struct dimensionstruct*  allocdimensionstructure(void)
{
    int j=0,k=0;
    struct dimensionstruct* thereturnptr = NULL;

    thereturnptr = (dimensionstruct*)calloc( (size_t)MAXNDIM, sizeof(dimensionstruct));
    if(thereturnptr!=NULL)
    {/*successfull allocation*/
       for(k=0; k<MAXNDIM; k++)
       {
          for(j=0; j<(MAXNAMECHAR+1); j++) { thereturnptr[k].name[j] = '\0'; }
          thereturnptr[k].wrap  = 0;
          thereturnptr[k].Lwrap = 0;
          thereturnptr[k].min =   0;
          thereturnptr[k].max = 360;
          thereturnptr[k].scale = 1;
       }
    }/*successfull allocation*/    
    return(thereturnptr);
}
/*____allocdimensionstructure()_____________________________________________*/

/****destroycalcNstructure()***********************************************/
void destroycalcNstructure(calcNstruct* victimptr)
{
   if(victimptr != NULL)
   {/*structure was allocated*/
      free(victimptr);  
   }/*structure was allocated*/
}
/*___destroycalcNstructure()______________________________________________*/

/****allocclacNstructure()*************************************************/
struct calcNstruct*  alloccalcNstructure(void)
{
    int j=0;
    struct calcNstruct* thereturnptr = NULL;

    thereturnptr = (calcNstruct*)malloc(sizeof(struct calcNstruct));
    if(thereturnptr!=NULL)
    {/*successfull allocation*/
      for(j=0; j<MAXNDIM; j++) { thereturnptr->dimn[j] = 0.0; }
      thereturnptr->number = 0;
    }/*successfull allocation*/    
    return(thereturnptr);
}
/*____alloccalcNstructure()______________________________________________*/

/*this section handles points as separately allocated objects*/
/****destroymoreinfostructure()**********************************************/
void destroymoreinfostructure(moreinfostruct* victimptr)
{
   if(victimptr != NULL)
   {/*structure was allocated*/
      if(victimptr->moreinfoptr != NULL)
         destroymoreinfostructure(victimptr->moreinfoptr);
      free(victimptr);  
   }/*structure was allocated*/
}
/*___destroymoreinfostructure()_____________________________________________*/

/****allocmoreinfostructure()************************************************/
struct moreinfostruct*  allocmoreinfostructure(void)
{
    struct moreinfostruct* thereturnptr = NULL;

    thereturnptr = (moreinfostruct*)malloc(sizeof(struct moreinfostruct));
    if(thereturnptr!=NULL)
    {/*successfull allocation*/
        thereturnptr->it = 0;
        thereturnptr->f1 = 0.0;
        thereturnptr->f2 = 0.0;
        thereturnptr->f2 = 0.0;
        thereturnptr->moreinfoptr = NULL; /*can be embedded moreinfostruct's */
    }/*successfull allocation*/    
    return(thereturnptr);
}
/*____allocmoreinfostructure()______________________________________________*/

/*this section handles points as separately allocated objects*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****allocnewpointstructure()*************************************************/
struct pointstruct*   allocnewpointstructure(struct liststruct* parentlistptr)
{/*NEW does NOT involve global thispointptr, re: old allocpointstructure()*/
    struct pointstruct* trialpointptr;
    int   ifail = 0;
    int j=0; /*010603*/
/*970503 all points must explicitly belong to a list*/
/*the old mechanism of a single continuous sequence of points is abandoned*/
    
/*NOTE calling routine must make sure that proper list structure is cross*/
/* referrenced */ /*970503 now the calling routine must pass a pointer to the*/
/*proper list so that it can be referenced here*/

    trialpointptr = (pointstruct*)malloc(sizeof(struct pointstruct));
    if(trialpointptr==NULL) ifail++;

    if(ifail)
    {
        trialpointptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(parentlistptr->firstpointptr == NULL)
        {
            parentlistptr->firstpointptr = trialpointptr;
            (parentlistptr->firstpointptr)->previousptr = NULL;
            parentlistptr->lastpointptr = trialpointptr;/*only 1 point in list*/
            /*trialpointptr->number = 1;*/ /*number 1 in this list*/
            /*071005 pointptr->number doesn't seem to be used as sequential*/
            /*030225,030403 subverted for FOO, 071005 subverted for suitefit*/ 
        }
        else
        {/*set references between last and new points*/
            /*let old last point in the list refer to the new point*/
            (parentlistptr->lastpointptr)->nextptr = trialpointptr; 
            /*let the new point refer back to the old last point*/
            trialpointptr->previousptr = parentlistptr->lastpointptr;
            /*redefine the new point to be the list's last point*/
            parentlistptr->lastpointptr = trialpointptr;
            /*set number in this list, NOT DO THIS 071005*/
            /*trialpointptr->number = trialpointptr->previousptr->number + 1;*/
        }
        trialpointptr->number = 0; /*030225,030403, subverted 071005 suitefit*/
        /*reset who is current point*/
        trialpointptr->nextptr = NULL; /*no others beyond this one*/  
        trialpointptr->listptr = parentlistptr; /*passed from calling routine*/

        /*now set some standard starting conditions*/
        trialpointptr->nameoffset = 0;
        trialpointptr->wordoffset = 0;
        trialpointptr->type = 0; /*clear type flags*/
        trialpointptr->STYLE = 0; /*clear STYLE flags*/
        trialpointptr->STATUS = ON; /*presume just ON, no other STATUS set*/
        trialpointptr->colorwidth = 0; 
        trialpointptr->radius = 0;
        trialpointptr->moreinfoptr = NULL;
        trialpointptr->master = 0;
        trialpointptr->fx = 0;/*even a noxyz point will have fx,fy,fz defined*/
        trialpointptr->fy = 0;
        trialpointptr->fz = 0;
        trialpointptr->ix = 0;/*even a noxyz point will have ix,iy,iz defined*/
        trialpointptr->iy = 0;
        trialpointptr->iz = 0;
        for(j=0;j<MAXPOINTASPECTS+1;j++) {trialpointptr->aspects[j] = '\0';}
        trialpointptr->coordNptr = NULL; /*061028*/
    }/*successfull allocation*/    
    return(trialpointptr);
}
/*____allocnewpointstructure()_______________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****allocpointstructure()***************************************************/
struct pointstruct*   allocpointstructure(struct liststruct* parentlistptr)
{
    struct pointstruct* thereturn = NULL;
    struct pointstruct* trialpointptr;
    int   ifail = 0;
    int j=0; /*010603*/

/*970503 all points must explicitly belong to a list*/
/*the old mechanism of a single continuous sequence of points is abandoned*/
    
/*NOTE calling routine must make sure that proper list structure is cross*/
/* referrenced */ /*970503 now the calling routine must pass a pointer to the*/
/*proper list so that it can be referenced here*/

    trialpointptr = (pointstruct*)malloc(sizeof(struct pointstruct));
    if(trialpointptr==NULL) ifail++;

    if(ifail)
    {
        trialpointptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(parentlistptr->firstpointptr == NULL)
        {
            parentlistptr->firstpointptr = trialpointptr;
            (parentlistptr->firstpointptr)->previousptr = NULL;
            parentlistptr->lastpointptr = trialpointptr;/*only 1 point in list*/
            /*trialpointptr->number = 1;*/ /*number 1 in this list*/
            /*071005 pointptr->number doesn't seem to be used as sequential*/
            /*030225,030403 subverted for FOO, 071005 subverted for suitefit*/ 
        }
        else
        {/*set references between last and new points*/
            /*let old last point in the list refer to the new point*/
            (parentlistptr->lastpointptr)->nextptr = trialpointptr; 
            /*let the new point refer back to the old last point*/
            trialpointptr->previousptr = parentlistptr->lastpointptr;
            /*redefine the new point to be the list's last point*/
            parentlistptr->lastpointptr = trialpointptr;
            /*set number in this list, NOT DO THIS 071005*/
            /*trialpointptr->number = trialpointptr->previousptr->number + 1;*/
        }
        trialpointptr->number = 0; /*030225,030403, subverted 071005 suitefit*/
        /*reset who is current point*/
        thispointptr = trialpointptr;
        thispointptr->nextptr = NULL; /*no others beyond this one*/  
        thispointptr->listptr = parentlistptr; /*passed from calling routine*/

        /*now set some standard starting conditions*/
        thispointptr->nameoffset = 0;
        thispointptr->wordoffset = 0;
        thispointptr->type = 0; /*clear type flags*/
        thispointptr->STYLE = 0; /*clear STYLE flags*/
        thispointptr->STATUS = ON; /*presume just ON, no other STATUS set*/
        thispointptr->colorwidth = 0;
        thispointptr->radius = 0;
        thispointptr->moreinfoptr = NULL;
        thispointptr->master = 0;
        thispointptr->fx = 0;/*even a noxyz point will have fx,fy,fz defined*/
        thispointptr->fy = 0;
        thispointptr->fz = 0;
        thispointptr->ix = 0;/*even a noxyz point will have ix,iy,iz defined*/
        thispointptr->iy = 0;
        thispointptr->iz = 0;
        thereturn = thispointptr;
        for(j=0;j<MAXPOINTASPECTS+1;j++) {thispointptr->aspects[j] = '\0';} 
        trialpointptr->coordNptr = NULL; /*061028*/
   }/*successfull allocation*/    

   return(thereturn);
}
/*____allocpointstructure()________________________________________________*/

/****alloclonelypointstructure()********************************************/
struct pointstruct*   alloclonelypointstructure()
{/*point with no parent list*/
    struct pointstruct* thereturn = NULL;
    struct pointstruct* trialpointptr;
    int   ifail = 0;
    int j=0; /*010603*/
    trialpointptr = (pointstruct*)malloc(sizeof(struct pointstruct));
    if(trialpointptr==NULL) ifail++;

    if(ifail)
    {
        trialpointptr = NULL;
    }  
    else
    {/*successfull allocation*/
        trialpointptr->previousptr = NULL;
        trialpointptr->nextptr = NULL;
        trialpointptr->listptr = NULL; 

        /*now set some standard starting conditions*/
        trialpointptr->nameoffset = 0;
        trialpointptr->wordoffset = 0;
        trialpointptr->type = 0; /*clear type flags*/
        trialpointptr->STYLE = 0; /*clear STYLE flags*/
        trialpointptr->STATUS = ON;  /*presume just ON, no other STATUS set*/
        trialpointptr->colorwidth = 0;
        trialpointptr->radius = 0;
        trialpointptr->moreinfoptr = NULL;
        trialpointptr->master = 0;
        trialpointptr->fx = 0;/*even a noxyz point will have fx,fy,fz defined*/
        trialpointptr->fy = 0;
        trialpointptr->fz = 0;
        trialpointptr->ix = 0;/*even a noxyz point will have ix,iy,iz defined*/
        trialpointptr->iy = 0;
        trialpointptr->iz = 0;
        thereturn = trialpointptr;
        for(j=0;j<MAXPOINTASPECTS+1;j++) {trialpointptr->aspects[j] = '\0';} /*010603*/
        trialpointptr->coordNptr = NULL; /*061028*/
    }/*successfull allocation*/    
    return(thereturn);
}
/*____alloclonelypointstructure()__________________________________________*/

/****copypointstructure()***************************************************/
int copypointstructure(pointstruct* newpointptr, pointstruct* oldpointptr)
{
   int ireturn = 0;
   int j=0; /*010603*/
   if(newpointptr != NULL && oldpointptr != NULL)
   {/*both structures have been allocated*/
        newpointptr->previousptr = oldpointptr->previousptr;
        newpointptr->nextptr = oldpointptr->nextptr;
        /*newpointptr->listptr = oldpointptr->listptr; */ /*020830*/
          /*this is a BOMB: the copied point might be part of a new list*/
          /*originally this was used to save deleted table cells, */
          /*so saved point still was associated with its orig list */
          /*but MAGESCOR/copy drawnew builds whole new group/subgroup/list*/

        /*now set some standard starting conditions*/
        newpointptr->nameoffset = oldpointptr->nameoffset;
        newpointptr->wordoffset = oldpointptr->wordoffset;
        newpointptr->type = oldpointptr->type; 
        newpointptr->STATUS = oldpointptr->STATUS;
        newpointptr->STYLE  = oldpointptr->STYLE;
        newpointptr->colorwidth = oldpointptr->colorwidth;
        newpointptr->radius = oldpointptr->radius;
        newpointptr->moreinfoptr = oldpointptr->moreinfoptr;
        newpointptr->master = oldpointptr->master;
        /*and the specific contents*/
        newpointptr->fx = oldpointptr->fx; 
        newpointptr->fy = oldpointptr->fy;
        newpointptr->fz = oldpointptr->fz;
        newpointptr->ix = oldpointptr->ix;
        newpointptr->iy = oldpointptr->iy;
        newpointptr->iz = oldpointptr->iz;
        for(j=0;j<MAXPOINTASPECTS+1;j++)
           {newpointptr->aspects[j] = oldpointptr->aspects[j];}/*010603*/
#ifdef OLD7ptr
        if(oldpointptr->angle7ptr != NULL)/*060622 should've been here earlier*/
        {  /*alloc the struct*/
           newpointptr->angle7ptr = allocangle7structure();
           if(newpointptr->angle7ptr != NULL)
           {  /*copy values*/
              newpointptr->angle7ptr->ANG[0] = oldpointptr->angle7ptr->ANG[0];
              newpointptr->angle7ptr->ANG[1] = oldpointptr->angle7ptr->ANG[1];
              newpointptr->angle7ptr->ANG[2] = oldpointptr->angle7ptr->ANG[2];
              newpointptr->angle7ptr->ANG[3] = oldpointptr->angle7ptr->ANG[3];
              newpointptr->angle7ptr->ANG[4] = oldpointptr->angle7ptr->ANG[4];
              newpointptr->angle7ptr->ANG[5] = oldpointptr->angle7ptr->ANG[5];
              newpointptr->angle7ptr->ANG[6] = oldpointptr->angle7ptr->ANG[6];
           }
        }
        else {newpointptr->angle7ptr = NULL;}
#endif

        if(oldpointptr->coordNptr != NULL && oldpointptr->listptr->NDIM > 3)
        {  /*alloc the coordNptr*/ /*060622,061028*/
           newpointptr->coordNptr = 
            (float*)calloc((size_t)(oldpointptr->listptr->NDIM), sizeof(float));
           if(newpointptr->coordNptr != NULL)
           {/*copy values*/
              for(j=0; j<oldpointptr->listptr->NDIM; j++)
              {
                 newpointptr->coordNptr[j] = oldpointptr->coordNptr[j];
              }
           }
        }
        else {newpointptr->coordNptr = NULL;}

      ireturn = 1;
   }/*both structures have been allocated*/
   return(ireturn);
}
/*___copypointstructure()__________________________________________________*/

/*this section handles points as separately allocated objects*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****insertpointstructure()***************************************************/
struct pointstruct*   insertpointstructure(pointstruct* neighborpointptr, int where)
{
    struct pointstruct* thereturn = NULL;
    struct pointstruct* trialpointptr = NULL;
    struct liststruct*  parentlistptr = NULL;
    int   ifail = 0;
    int j=0; /*010603*/
/*970503 all points must explicitly belong to a list*/
/*the old mechanism of a single continuous sequence of points is abandoned*/
    
/*NOTE calling routine must make sure that proper list structure is cross*/
/* referrenced */ /*970503 now the calling routine must pass a pointer to the*/
/*proper list so that it can be referenced here*/
/*for inserting, the parent list is inherited from the neighbor point*/
/*which 99101, is before where <= 0, after where  > 0 */

    trialpointptr = (pointstruct*)malloc(sizeof(struct pointstruct));
    if(trialpointptr==NULL) ifail++;

    if(ifail)
    {
        trialpointptr = NULL;
    }  
    else
    {/*successfull allocation*/
       parentlistptr = neighborpointptr->listptr;
       if(parentlistptr != NULL)
       {/*found parent list to put this new point in*/
          trialpointptr->listptr = parentlistptr;
          /*4 connections to make or remake*/
          if(where > 0)
          {/*after: where + */
             /*in middle or end, since can't be first in a list*/
        /*1*/  trialpointptr->nextptr = neighborpointptr->nextptr; /*NULL: end*/
                
        /*2*/  neighborpointptr->nextptr = trialpointptr;
             
        /*3*/  trialpointptr->previousptr = neighborpointptr;
             
               if(trialpointptr->nextptr != NULL)
               {/*connect back from later point*/
        /*4*/     (trialpointptr->nextptr)->previousptr = trialpointptr;
               }
               else 
               {/*at end, has to be list's last point*/
                  parentlistptr->lastpointptr = trialpointptr;
               }
               /*if points linked continuously, have to check for end of list*/
               if(parentlistptr->lastpointptr == neighborpointptr)
                  parentlistptr->lastpointptr = trialpointptr;
          }/*after: where + */
          else
          {/*before: where - or 0 */
             /*in middle or start, since can't be last in a list*/
        /*1*/  trialpointptr->previousptr = neighborpointptr->previousptr; 
                     /*NULL: end*/
        /*2*/  neighborpointptr->previousptr = trialpointptr;
             
        /*3*/  trialpointptr->nextptr = neighborpointptr;
             
               if(trialpointptr->previousptr != NULL)
               {/*connect forward from earlier point*/
        /*4*/     (trialpointptr->previousptr)->nextptr = trialpointptr;
               }
               else 
               {/*at beginning, has to be list's first point*/
                  parentlistptr->firstpointptr = trialpointptr;
               }
               /*if points linked continuously, have to check for end of list*/
               if(parentlistptr->firstpointptr == neighborpointptr)
                  parentlistptr->firstpointptr = trialpointptr;
          }/*before: where - or 0 */
          
          /*now set some standard starting conditions*/
          /*keep word str: */
          /*  at the beginning of row 1 it serves as a size pattern*/
          /*invisible, name just helps keep track of where coords came from*/
          trialpointptr->nameoffset = neighborpointptr->nameoffset; 
          trialpointptr->wordoffset = neighborpointptr->wordoffset;
          
          trialpointptr->colorwidth = numberofcolor("invisible");
          trialpointptr->radius = 0;
          trialpointptr->moreinfoptr = NULL;
          trialpointptr->type = (neighborpointptr->type | UNPICKABLE);
          trialpointptr->STATUS = ON; /*presume just ON, no other STATUS set*/
          trialpointptr->master = 0;
          for(j=0;j<MAXPOINTASPECTS+1;j++) {trialpointptr->aspects[j] = '\0';} 
            /*010603, change from 16 031227*/
          trialpointptr->coordNptr = NULL; /*060622,061028*/
          /*calculate a position in space for this inserted point*/
          if(   trialpointptr->nextptr == NULL 
             || trialpointptr->previousptr == NULL
             || parentlistptr->firstpointptr == trialpointptr
             || parentlistptr->lastpointptr == trialpointptr  )
          {/*inserted at start or end of a list, just use coord of neighbor*/
             trialpointptr->fx = neighborpointptr->fx;
             trialpointptr->fy = neighborpointptr->fy;
             trialpointptr->fz = neighborpointptr->fz;
          }
          else
          {/*average between neighbors*/
             trialpointptr->fx = (  (trialpointptr->previousptr)->fx
                                  + (trialpointptr->nextptr)->fx)/2;
             trialpointptr->fy = (  (trialpointptr->previousptr)->fy
                                  + (trialpointptr->nextptr)->fy)/2;
             trialpointptr->fz = (  (trialpointptr->previousptr)->fz
                                  + (trialpointptr->nextptr)->fz)/2;
          }
          calcintegerpoints(trialpointptr); /*MAGELIST*/

          thereturn = trialpointptr;
        }/*found parent list to put this new point in*/
    }/*successfull allocation*/    
    return(thereturn);
}
/*____insertpointstructure()________________________________________________*/

/****destroypointstructure()************************************************/
void destroypointstructure(pointstruct* victimptr)
{

    /*NOTE: ptID char strings are left orphaned*/
    /* eventually need garbage collection */
 if(victimptr != NULL)
 {/*point was allocated*/
  if( (victimptr->listptr) != NULL)
  {/*point has a parent list*/ /*970503 all points must be members of lists*/ 
    /*excise and anneal references to this point in its parent list*/
    /*960605 level, this is just checking first and last points of the list*/
    /*970503 do reconnections of the surrounding points in this section*/
    if(  (victimptr->listptr)->firstpointptr == victimptr
       &&(victimptr->listptr)->lastpointptr  == victimptr )
    {/*this point is the only one in its list*/
        (victimptr->listptr)->firstpointptr = NULL;
        (victimptr->listptr)->lastpointptr  = NULL;  
        /*No points remaining to reconnect within this list*/ 
    }          
    else if(  (victimptr->listptr)->firstpointptr == victimptr )
    {/*this point is first of 2 or more points in its list*/
        (victimptr->listptr)->firstpointptr = victimptr->nextptr;
        /*a surviving point is now first in this list: reset its previousptr*/
        ((victimptr->listptr)->firstpointptr)->previousptr = NULL;
    } 
    else if(  (victimptr->listptr)->lastpointptr == victimptr )
    {/*this point is last of 2 or more points in its list*/
        (victimptr->listptr)->lastpointptr = victimptr->previousptr;
        /*a surviving point is now last in this list: reset its nextptr*/
        ((victimptr->listptr)->lastpointptr)->nextptr = NULL;
    } 
    else
    {/*this point is embedded in the midst of the points in this list*/
        /*connect the preceeding and following points*/
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        (victimptr->nextptr)->previousptr = victimptr->previousptr;
    }
    /*Now can destroy this point with impunity*/
  }/*point has a parent list*/
  if(victimptr->moreinfoptr != NULL)
  {
      destroymoreinfostructure(victimptr->moreinfoptr);
  }
  if(victimptr->coordNptr != NULL) /*061028*/
  {
      free(victimptr->coordNptr);
  }
  free(victimptr);
        
 }/*point was allocated*/
}
/*___destroypointstructure()________________________________________________*/

/****allocspecialintpoint()**************************************************/
int allocspecialintpoint(liststruct* thelistptr, int type
                       , int ix, int iy, int iz)
{
    int  ireturn = 0;
    
    if( !allocpointstructure(thelistptr) ) 
    {
        sprintf(alertstr,"failed to alloc special point in list: %s"
                ,thelistptr->name);
        dosinglealert(2);
    }
    else /*alloc: firstpointptr,lastpointptr,previousptr,nextptr  set*/
    {       
        /*thispointptr->listptr=thelistptr;*/ /*970503 taken care of in alloc*/ 
        thispointptr->type =  type;
        thispointptr->master = 0; 
        thispointptr->STATUS = ON; /*presume ON*/ /*991210*/
        thispointptr->ix=  ix;
        thispointptr->iy=  iy;
        thispointptr->iz=  iz;
        
        /*build pseudo-original floating point coordinates*/
        thispointptr->fx = (thispointptr->ix)/Scale;  
        thispointptr->fy = (thispointptr->iy)/Scale; 
        thispointptr->fz = (thispointptr->iz)/Scale;   

        thispointptr->colorwidth = 0; /*presume use list color*/
        thispointptr->nameoffset = 0;
        thispointptr->wordoffset = 0;
        ireturn = 1;
    }
    return(ireturn);
}
/*___allocspecialintpoint()__________________________________________________*/

/*Now do list stuff*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****allocinsertedliststructure()*********************************************/
int   allocinsertedliststructure(liststruct* previouslistptr) /*061216*/
{
  struct liststruct *triallistptr;
  struct liststruct *nextlistptr;
  int   ireturn = 0;/*if successful, will be set == 1*/

  if(previouslistptr != NULL) /*only inserts after previous list*/
  {
    triallistptr = (liststruct*)malloc(sizeof(struct liststruct));

    if(triallistptr != NULL) 
    {/*successfull allocation*/
        /*set references between previous, next, and new lists*/
        nextlistptr = previouslistptr->nextptr; /*save*/
        previouslistptr->nextptr = triallistptr;
        triallistptr->previousptr = previouslistptr;
        if(nextlistptr != NULL) 
        {
            triallistptr->nextptr = nextlistptr;
            nextlistptr->previousptr = triallistptr;
        }
        else 
        {
            triallistptr->nextptr = NULL;
            lastlistptr = thislistptr; /*lists all refer to each other*/
        }
        
        /*reset who is current list*/
        thislistptr = triallistptr;
        ireturn = 1;   
    
        thislistptr->on = 0;
        thislistptr->STATUS = 0;
        thislistptr->STYLE = 0;
        thislistptr->KKIND = 0; /* 061028, should've been here earlier */
        thislistptr->NDIM = 0; /* 061028, should've been here earlier */
        thislistptr->type = 0; 
        thislistptr->width = 0;
        thislistptr->color = 0;
        thislistptr->owncolor = 0;
        thislistptr->bondrot = 0;
        thislistptr->wordsfont = 0;
        thislistptr->wordsface = 0;
        thislistptr->wordssize = 0;
        thislistptr->master = 0;
        thislistptr->colorset = 0;
        thislistptr->radius = 0;
        thislistptr->angle = 0;
        thislistptr->name[0] = '\0'; /*so identifying by name won't blow up*/
        thislistptr->firstpointptr = NULL;
        thislistptr->lastpointptr = NULL;
        thislistptr->sgrpptr = NULL; /*calling routine connects as needed*/
    
        thislistptr->instanceptr = NULL;
        thislistptr->button = NULL;
    }/*successfull allocation*/     
    return(ireturn);
  }
  return(ireturn); /*121108 for unsuccessful return*/
}
/*____allocinsertedliststructure()___________________________________________*/

/****allocliststructure()*****************************************************/
int   allocliststructure() /*only works if list is at the end of all lists*/
{
	struct liststruct *triallistptr;
    int   ifail = 0;
    int   ireturn = 0;/*if successful, will be set == 1*/

    triallistptr = (liststruct*)malloc(sizeof(struct liststruct));
    if(triallistptr==NULL) ifail++;

    if(ifail)
    {
        triallistptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(firstlistptr == NULL)
        {
            firstlistptr = triallistptr;
            firstlistptr->previousptr = NULL;
        }
        else
        {/*set references between last and new lists*/
            lastlistptr->nextptr = triallistptr;
            triallistptr->previousptr = lastlistptr;
        }
        /*reset who is current list*/
        thislistptr = triallistptr;
        thislistptr->nextptr = NULL; /*no others beyond this one*/  
        lastlistptr = thislistptr; 
            /*place holder when thislistptr is not open*/
        ireturn = 1;   
    
        thislistptr->on = 0;
        thislistptr->STATUS = 0;
        thislistptr->STYLE = 0;
        thislistptr->KKIND = 0; /* 061028, should've been here earlier */
        thislistptr->NDIM = 0; /* 061028, should've been here earlier */
        thislistptr->type = 0; 
        thislistptr->width = 0;
        thislistptr->color = 0;
        thislistptr->owncolor = 0;
        thislistptr->bondrot = 0;
        thislistptr->wordsfont = 0;
        thislistptr->wordsface = 0;
        thislistptr->wordssize = 0;
        thislistptr->master = 0;
        thislistptr->colorset = 0;
        thislistptr->radius = 0;
        thislistptr->angle = 0;
        thislistptr->name[0] = '\0'; /*so identifying by name won't blow up*/
        thislistptr->firstpointptr = NULL;
        thislistptr->lastpointptr = NULL;
        thislistptr->sgrpptr = NULL; /*calling routine connects as needed*/
        thislistptr->resnumb = 0; /*alternate suitefit base residue 071005*/
        thislistptr->resname[0] = '\0'; /*alternate suitefit base 071005*/
    
        thislistptr->instanceptr = NULL;
        thislistptr->button = NULL;
    }/*successfull allocation*/     
    return(ireturn);
}
/*____allocliststructure()________________________________________________*/

/****destroyliststructure()************************************************/
void destroyliststructure(liststruct* victimptr)
{
  /*BEWARE: must check that this is a primary list, not instancing one*/
  if(victimptr->instanceptr == NULL)
  {/*A primary list that can have points as children*/
    /*first destroy member points*/    
    if (victimptr->firstpointptr != NULL)  /*981212*/
    {/*first member exists*/
       do   /*test after loop so "==" case will also be destroyed*/
       {
          destroypointstructure(victimptr->firstpointptr);
          /*this redefines the list's first and last points*/
        }while(    victimptr->firstpointptr != NULL );
     }/*first member exists*/  /*981212*/
  }/*A primary list that can have points as children*/
  /*destroy button if one had been made for it*/
  if(victimptr->button != NULL) destroybuttons(victimptr->button);

  if(victimptr->sgrpptr != NULL) 
  {/*this list is a member of a subgroup*/
    /*excise and anneal references to this list in its parent subgroup*/
    /*960605 level, this is just checking first and last lists of the sgrp*/
    if(  (victimptr->sgrpptr)->firstlistptr == victimptr
       &&(victimptr->sgrpptr)->lastlistptr  == victimptr )
    {/*this list is the only one in its subgroup*/
        (victimptr->sgrpptr)->firstlistptr = NULL;
        (victimptr->sgrpptr)->lastlistptr  = NULL;   
    }          
    else if(  (victimptr->sgrpptr)->firstlistptr == victimptr )
    {/*this list is first of 2 or more lists in its subgroup*/
        (victimptr->sgrpptr)->firstlistptr = victimptr->nextptr;
    } 
    else if(  (victimptr->sgrpptr)->lastlistptr == victimptr )
    {/*this list is last of 2 or more lists in its subgroup*/
        (victimptr->sgrpptr)->lastlistptr = victimptr->previousptr;
    } 
  }/*this list is a member of a subgroup*/

    /*reconnect references among lists*/
    if(victimptr != firstlistptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastlistptr*/
        {
            lastlistptr = victimptr->previousptr;
            (victimptr->previousptr)->nextptr = NULL;
        }
    }
    else /*victimptr IS firstlistptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstlistptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastlistptr*/
        {
            firstlistptr = NULL;
            lastlistptr  = NULL;
        }
    }
    free(victimptr);
}
/*___destroyliststructure()_________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****allocsgrpstructure()*************************************************/
int   allocsgrpstructure()
{
	struct sgrpstruct *trialsgrpptr;
    int   ifail = 0;
    int   ireturn = 0;/*if successful, will be set == 1*/

    trialsgrpptr = (sgrpstruct*)malloc(sizeof(struct sgrpstruct));
    if(trialsgrpptr==NULL) ifail++;

    if(ifail)
    {
        trialsgrpptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(firstsgrpptr == NULL)
        {
            firstsgrpptr = trialsgrpptr;
            firstsgrpptr->previousptr = NULL;
        }
        else
        {/*set references between last and new sgrps*/
            lastsgrpptr->nextptr = trialsgrpptr;
            trialsgrpptr->previousptr = lastsgrpptr;
        }
        /*reset who is current sgrp*/
        thissgrpptr = trialsgrpptr;
        thissgrpptr->nextptr = NULL; /*no others beyond this one*/  
        lastsgrpptr = thissgrpptr; 
            /*place holder when thissgrpptr is not open*/
        ireturn = 1;

        thissgrpptr->name[0] = '\0'; /*so identifying by name won't blow up*/
        thissgrpptr->NDIM = 0; /*might inherit from parent group 060622*/
        thissgrpptr->on = 0;
        thissgrpptr->KKIND = 0; /* 061028, should've been here earlier */
        thissgrpptr->STATUS = 0;
        thissgrpptr->balllists = 0;
        thissgrpptr->bondrot = 0;
    
        thissgrpptr->grupptr = NULL; /*calling routine connects as needed*/
        thissgrpptr->firstlistptr = NULL;
        thissgrpptr->lastlistptr = NULL;
    
        thissgrpptr->master = 0;
    
        thissgrpptr->name[0] = '\0';
        thissgrpptr->instanceptr = NULL;
    
        thissgrpptr->button = NULL;
    }/*successfull allocation*/    
    return(ireturn);
}
/*____allocsgrpstructure()________________________________________________*/

/****destroysgrpstructure()************************************************/
void destroysgrpstructure(sgrpstruct* victimptr)
{
  /*BEWARE: must check that this is a primary subgroup, not instancing one*/
  if(victimptr->instanceptr == NULL)
  {/*A primary subgroup that can have lists as children*/
    /*first destroy member lists*/
    if (victimptr->firstlistptr != NULL)  /*981212*/
    {/*first member exists*/
       do   /*test after loop so "==" case will also be destroyed*/
       {
          destroyliststructure(victimptr->firstlistptr);
          /*this redefines the subgroup's first and last lists*/
       }while(    victimptr->firstlistptr != NULL );
    }/*first member exists*/  /*981212*/
  }/*A primary subgroup that can have lists as children*/
  /*destroy button if one had been made for it*/
  if(victimptr->button != NULL) destroybuttons(victimptr->button);
    
  if(victimptr->grupptr != NULL)
  {/*this subgroup is a member of a group*/
    /*excise and anneal references to this sgrp in its parent group*/
    /*960605 level, this is just checking first and last sgrps of the grup*/
    if(  (victimptr->grupptr)->firstsgrpptr == victimptr
       &&(victimptr->grupptr)->lastsgrpptr  == victimptr )
    {/*this subgroup is the only one in its group*/
        (victimptr->grupptr)->firstsgrpptr = NULL;
        (victimptr->grupptr)->lastsgrpptr  = NULL;   
    }          
    else if(  (victimptr->grupptr)->firstsgrpptr == victimptr )
    {/*this subgroup is first of 2 or more subgroups in its group*/
        (victimptr->grupptr)->firstsgrpptr = victimptr->nextptr;
    } 
    else if(  (victimptr->grupptr)->lastsgrpptr == victimptr )
    {/*this subgroup is last of 2 or more subgroups in its group*/
        (victimptr->grupptr)->lastsgrpptr = victimptr->previousptr;
    } 
  }/*this subgroup is a member of a group*/

    /*reconnect references among subgroups*/
    if(victimptr != firstsgrpptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastsgrpptr*/
        {
            lastsgrpptr = victimptr->previousptr;
            (victimptr->previousptr)->nextptr = NULL;
        }
        
    }
    else /*victimptr IS firstsgrpptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstsgrpptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastsgrpptr*/
        {
            firstsgrpptr = NULL;
            lastsgrpptr  = NULL;
        }
    }

    free(victimptr);
        
   /* points: thissgrpptr->firstlistptr && thissgrpptr->lastlistptr are*/
   /* orphaned and eventually should be garbage collected */
}
/*___destroysgrpstructure()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****allocgrupstructure()****************************************************/
int   allocgrupstructure()
{
    struct grupstruct *trialgrupptr;
    int   ifail = 0;
    int   ireturn = 0;/*if successful, will be set == 1*/
    trialgrupptr = (grupstruct*)malloc(sizeof(struct grupstruct));
    if(trialgrupptr==NULL) ifail++;

    if(ifail)
    {
        trialgrupptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(firstgrupptr == NULL)
        {
            firstgrupptr = trialgrupptr;
            firstgrupptr->previousptr = NULL;
        }
        else
        {/*set references between last and new grups*/
            lastgrupptr->nextptr = trialgrupptr;
            trialgrupptr->previousptr = lastgrupptr;
        }
        /*reset who is current grup*/
        thisgrupptr = trialgrupptr;
        thisgrupptr->nextptr = NULL; /*no others beyond this one*/  
        lastgrupptr = thisgrupptr; 
            /*place holder when thisgrupptr is not open*/
        ireturn = 1;
        thisgrupptr->NDIM = 0; /* 060622, should've been here earlier */
        thisgrupptr->on = 0;
        thisgrupptr->type = 0; /*nothing special*/
        thisgrupptr->STATUS = 0;
        thisgrupptr->KKIND = Nthisgroupkind; /*030312*/
        thisgrupptr->balllists = 0;
        thisgrupptr->bondrot = 0;
        thisgrupptr->bondrotmin = 0;
        thisgrupptr->animate = 0;
        thisgrupptr->moview = 0;
        thisgrupptr->pointx[0] = 1; /*unit on each axis, at origin*/
        thisgrupptr->pointx[1] = 0;
        thisgrupptr->pointx[2] = 0;
        thisgrupptr->pointy[0] = 0;
        thisgrupptr->pointy[1] = 1;
        thisgrupptr->pointy[2] = 0;
        thisgrupptr->pointz[0] = 0;
        thisgrupptr->pointz[1] = 0;
        thisgrupptr->pointz[2] = 1;
        thisgrupptr->position[0] = 0;
        thisgrupptr->position[1] = 0;
        thisgrupptr->position[2] = 0;
        
        thisgrupptr->firstsgrpptr = NULL;
        thisgrupptr->lastsgrpptr = NULL;
    
        thisgrupptr->master = 0;
        thisgrupptr->name[0] = '\0';/*so identifying by name won't blow up*/
        thisgrupptr->pdbfilestr[0]='\0';/*groups can refer to a pdbfile*/

        thisgrupptr->instanceptr = NULL;
        thisgrupptr->button = NULL;
    }/*successfull allocation*/    
    return(ireturn);
}
/*____allocgrupstructure()________________________________________________*/

/****destroygrupstructure()************************************************/
void destroygrupstructure(grupstruct* victimptr)
{
  /*BEWARE: must check that this is a primary group, not instancing one*/
  if(victimptr->instanceptr == NULL)
  {/*A primary group that can have subgroups as children*/
    /*first destroy member subgroups*/
    if (victimptr->firstsgrpptr != NULL)  /*981212*/
    {/*first member exists*/
       do   /*test after loop so "==" case will also be destroyed*/
       {
          destroysgrpstructure(victimptr->firstsgrpptr);
          /*this redefines the group's first and last subgroups*/
       }while(    victimptr->firstsgrpptr != NULL);
    }/*first member exists*/  /*981212*/
  }/*A primary group that can have subgroups as children*/
  /*destroy button if one had been made for it*/
  if(victimptr->button != NULL) destroybuttons(victimptr->button);

  /*reconnect references among groups*/
  if(victimptr != firstgrupptr)
  {
      (victimptr->previousptr)->nextptr = victimptr->nextptr;
      if( (victimptr->nextptr) != NULL)
      {
          (victimptr->nextptr)->previousptr = victimptr->previousptr;
      }
      else /*victimptr IS lastgrupptr*/
      {
          lastgrupptr = victimptr->previousptr;
          (victimptr->previousptr)->nextptr = NULL;
      }
  }
  else /*victimptr IS firstgrupptr*/
  {
      if( (victimptr->nextptr) != NULL)
      {
          (victimptr->nextptr)->previousptr = NULL;
          firstgrupptr =  victimptr->nextptr; 
      }
      else /*victimptr IS also lastgrupptr*/
      {
          firstgrupptr = NULL;
          lastgrupptr  = NULL;
      }
  }

  free(victimptr);
  
  /* points: thisgrupptr->firstsgrpptr && thisgrupptr->lastsgrpptr are*/
  /* orphaned and eventually should be garbage collected */
}
/*___destroygrupstructure()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****killkinemage()***********************************************************/
void killkinemage() /*030320*/
{
   /*Note: markers original points do not belong to a group*/
   /*these kills should all be safely conditional on existance*/
   killmarkers(); /*MAGEINPT*/
   killpickedpointers(); /*MAGEINPT 050916 avoid PC crash pick in new kinemage*/
   killmeasures(); /*MAGEINPT*/
   killdrawstuff(); /*MAGEINPT*/
   killmasters(); /*MAGELIST.c*/
   killcolorsets(); /*MAGELIST.c*/
   killgroups(); /*MAGELIST.c*/
}
/*___killkinemage()__________________________________________________________*/

/****killgroups()*************************************************************/
void killgroups()
{
    if(firstgrupptr != NULL)
    {
        do   /*test after loop so "==" case will also be destroyed*/
        {
            destroygrupstructure(firstgrupptr);
            /*this redefines the first and last group pointers*/
        }while(    firstgrupptr != NULL); /*eventually, both NULL*/
        /*destroy group destroys all its subgroups which in turn destroy */
        /* all their member lists, which in turn destroy all their points*/
    }
/*
{
printf("firstgrupptr %ld, lastgrupptr %ld\n",firstgrupptr,lastgrupptr);
printf("firstsgrpptr %ld, lastsgrpptr %ld\n",firstsgrpptr,lastsgrpptr);
printf("firstlistptr %ld, lastlistptr %ld\n",firstlistptr,lastlistptr);
}
*/
}
/*___killgroups()____________________________________________________________*/

/****killkindofgroups()*******************************************************/
void killkindofgroups(unsigned int KILLKIND)
{
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {
        if(thisgrupptr->KKIND & KILLKIND) {destroygrupstructure(thisgrupptr);}
        if(thisgrupptr == lastgrupptr) thisgrupptr = NULL; /*quit*/
        else thisgrupptr = thisgrupptr->nextptr;
    }
}
/*___killkindofgroups()______________________________________________________*/

/****allocmasterstructure()*************************************************/
int   allocmasterstructure()
{
    struct masterstruct *trialmasterptr = NULL;
    int   ireturn = 0;/*if successful, will be set == 1*/

    trialmasterptr = (masterstruct*)malloc(sizeof(struct masterstruct));
    if(trialmasterptr==NULL)
    {
       ireturn = 0;
    }  
    else
    {/*successfull allocation*/
        if(firstmasterptr == NULL)
        {
            firstmasterptr = trialmasterptr;
            firstmasterptr->previousptr = NULL;
        }
        else
        {/*set references between last and new masters*/
            lastmasterptr->nextptr = trialmasterptr;
            trialmasterptr->previousptr = lastmasterptr;
        }
        /*reset who is current master*/
        thismasterptr = trialmasterptr;
        thismasterptr->nextptr = NULL; /*no others beyond this one*/  
        lastmasterptr = thismasterptr; 
            /*place holder when thismasterptr is not open*/

        thismasterptr->on = 0; 
        thismasterptr->indent = 0; /*991210*/
        thismasterptr->space = 0; /*991210*/
        thismasterptr->mask = 0; 
        thismasterptr->name[0] = '\0'; /*so identifying by name won't blow up*/ 
        thismasterptr->button = NULL; 
        thismasterptr->masterchar='\0';/*single char pointmaster code,991210*/
        thismasterptr->STYLE = 0; /*060212 replaces previously unused STATUS*/
                                  /*060212 use for SPACED for space,indent */
        ireturn = 1;
    }/*successfull allocation*/    
    
    return(ireturn);

}
/*____allocmasterstructure()________________________________________________*/

/****destroymasterstructure()************************************************/
void destroymasterstructure(masterstruct* victimptr)
{

        /* points: thismasterptr->firstpoint && thismasterptr->lastpoint are*/
        /* orphaned and eventually should be garbage collected */
        /* or cleaned up when all points are recoupled */

    /*destroy button if one had been made for it*/
    if(victimptr->button != NULL) destroybuttons(victimptr->button);
 
 
    /*reconnect references among masters*/
    if(victimptr != firstmasterptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastmasterptr*/
        {
            lastmasterptr = victimptr->previousptr;
        }
    }
    else /*victimptr IS firstmasterptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstmasterptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastmasterptr*/
        {
            firstmasterptr = NULL;
            lastmasterptr  = NULL;
        }
    }

    free(victimptr);
        
}
/*___destroymasterstructure()_________________________________________________*/

/****killmasters()************************************************************/
void killmasters()
{
    if(firstmasterptr != NULL)
    {
        do   /*test after loop so "==" case will also be destroyed*/
        {
            destroymasterstructure(firstmasterptr);
            /*this redefines the first and last master pointers*/
        }while(firstmasterptr != NULL); /*eventually, both NULL*/
    }
}
/*___killmasters()___________________________________________________________*/

/****alloccolorsetstructure()*************************************************/
int   alloccolorsetstructure()
{
    struct colorsetstruct *trialcolorsetptr;
    int   ifail = 0;
    int   ireturn = 0;/*if successful, will be set == 1*/

    trialcolorsetptr = (colorsetstruct*)malloc(sizeof(struct colorsetstruct));
    if(trialcolorsetptr==NULL) ifail++;

    if(ifail)
    {
        trialcolorsetptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(firstcolorsetptr == NULL)
        {
            firstcolorsetptr = trialcolorsetptr;
            firstcolorsetptr->previousptr = NULL;
        }
        else
        {/*set references between last and new colorsets*/
            lastcolorsetptr->nextptr = trialcolorsetptr;
            trialcolorsetptr->previousptr = lastcolorsetptr;
        }
        /*reset who is current colorset*/
        thiscolorsetptr = trialcolorsetptr;
        thiscolorsetptr->nextptr = NULL; /*no others beyond this one*/  
        lastcolorsetptr = thiscolorsetptr; 
            /*place holder when thiscolorsetptr is not open*/

        thiscolorsetptr->color = 0; 
        thiscolorsetptr->number = 0; 
        thiscolorsetptr->name[0] = '\0';/*so identifying by name won't blow up*/

        ireturn = 1;
    }/*successfull allocation*/    
    
    return(ireturn);

}
/*____alloccolorsetstructure()______________________________________________*/

/****destroycolorsetstructure()**********************************************/
void destroycolorsetstructure(colorsetstruct* victimptr)
{

    /* points: thiscolorsetptr->firstpoint && thiscolorsetptr->lastpoint are*/
    /* orphaned and eventually should be garbage collected */
    /* or cleaned up when all points are recoupled */

    /*reconnect references among colorsets*/
    if(victimptr != firstcolorsetptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastcolorsetptr*/
        {
            lastcolorsetptr = victimptr->previousptr;
        }
    }
    else /*victimptr IS firstcolorsetptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstcolorsetptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastcolorsetptr*/
        {
            firstcolorsetptr = NULL;
            lastcolorsetptr  = NULL;
        }
    }

    free(victimptr);
        
}
/*___destroycolorsetstructure()_______________________________________________*/

/****killcolorsets()**********************************************************/
void killcolorsets()
{
    if(firstcolorsetptr != NULL)
    {
        do   /*test after loop so "==" case will also be destroyed*/
        {
            destroycolorsetstructure(firstcolorsetptr);
            /*this redefines the first and last colorset pointers*/
        }while(firstcolorsetptr != NULL); /*eventually, both NULL*/
    }
}
/*___killcolorsets()_________________________________________________________*/

/****adjustcolorset()**********************************************************/
void adjustcolorset(unsigned int number, int icolor)
{ 
   /*colorsets only pertain to lists*/
   thislistptr = firstlistptr;
   while(thislistptr != NULL)
   {/*scan over all lists*/
      if(thislistptr->colorset == number)
      {  
         thislistptr->color = icolor;
      }
      thislistptr = thislistptr->nextptr;
   }/*scan over all lists*/

   thiscolorsetptr = firstcolorsetptr;
   while(thiscolorsetptr != NULL)
   {/*scan through the colorsets*/
      if(thiscolorsetptr->number == number)
      {/*reset this colorset's color*/
         thiscolorsetptr->color = icolor;
         break; /*only expect one*/
      }
      thiscolorsetptr = thiscolorsetptr->nextptr;
   }
}
/*___adjustcolorset()_________________________________________________________*/
 
/****allocbondrotliststructure()**********************************************/
bondrotliststruct*   allocbondrotliststructure()
{
   bondrotliststruct*   returnptr = NULL;/*if successful, will be set */
   bondrotliststruct*   trialbondrotlistptr;
   int   ifail = 0;

   trialbondrotlistptr = 
       (bondrotliststruct*)malloc(sizeof(struct bondrotliststruct));
   if(trialbondrotlistptr==NULL) ifail++;

   if(ifail)
   {
      trialbondrotlistptr = NULL;
   }  
   else
   {/*successfull allocation*/
      if(firstbondrotlistptr == NULL)
      {
         firstbondrotlistptr = trialbondrotlistptr;
         firstbondrotlistptr->previousptr = NULL;
      }
      else
      {/*set references between last and new bondrotlists*/
         lastbondrotlistptr->nextptr = trialbondrotlistptr;
         trialbondrotlistptr->previousptr = lastbondrotlistptr;
      }
      /*reset who is current bondrotlist*/
      thisbondrotlistptr = trialbondrotlistptr;
      thisbondrotlistptr->nextptr = NULL; /*no others beyond this one*/  
      lastbondrotlistptr = thisbondrotlistptr; 
          /*place holder when thisbondrotlistptr is not open*/
      returnptr = thisbondrotlistptr;
    
      thisbondrotlistptr->sgrpptr = NULL; /*calling routine connects as needed*/
      thisbondrotlistptr->name[0] ='\0';/*so identifying by name won't blow up*/
      thisbondrotlistptr->firstpointptr = NULL;
      thisbondrotlistptr->lastpointptr = NULL;
      thisbondrotlistptr->slider = 1; /*default is to draw slider 070617*/
      thisbondrotlistptr->select = 0; /*default is NOT special select 071215*/
      thisbondrotlistptr->changed = 0; /*set=1 by this bondrot callback 071215*/
   }/*successfull allocation*/    
   return(returnptr);
}
/*____allocbondrotliststructure()____________________________________________*/

/****destroybondrotliststructure()********************************************/
void destroybondrotliststructure(bondrotliststruct* victimptr)
{
    /*The bondrotliststruct does NOT have children points */
    /*It only refers to points that have other parents */

  if(victimptr != NULL)
  {/*bondrotliststruct exits*/
    /*reconnect references among bondrotlists*/
    if(victimptr != firstbondrotlistptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastbondrotlistptr*/
        {
            lastbondrotlistptr = victimptr->previousptr;
            (victimptr->previousptr)->nextptr = NULL;
        }
    }
    else /*victimptr IS firstbondrotlistptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstbondrotlistptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastbondrotlistptr*/
        {
            firstbondrotlistptr = NULL;
            lastbondrotlistptr  = NULL;
        }
    }

    free(victimptr);
  }/*bondrotliststruct exits*/
        
}
/*___destroybondrotliststructure()___________________________________________*/


/****storeptIDstring()*******************************************************/
void storeptIDstring(char* thestring, pointstruct* thepointptr)
{
    int    j;
    
    /*store directly in giant character array */
    j=-1;do{j++;}while(thestring[j]!='\0');
    if(!Limitmaxcom && (ncom+j >= MAXCOM-1)  )
    {/*try to allocate more storage and increase MAXCOM*/ /*010509 so double alloc each try*/
        alloccomarray(MAXCOM); /*MAGEINIT.c*/ /*try for (n) more */
        /*this is the com array of ptID char strings*/
    }
    if(!Limitmaxcom && (ncom+j < MAXCOM-1) )/*insurance */
    {
        /*store starting offset of address in long place in pointstruct*/
        thepointptr->nameoffset = ncom; /*current offset into com array */
           /*this is the starting address for the current ptID*/
        j = -1;
        do{
             j++;
             com[ncom] = thestring[j];
             ncom++;
          }while(thestring[j] != '\0');
    }
    else
    {/*no room for new ptID, point it to where \0 stored*/
        thepointptr->nameoffset = 0; /*com[0]=='\0' */
    }
}
/*___storeptIDstring()______________________________________________________*/

/****getptIDstring()*********************************************************/
void getptIDstring(char* thestring, pointstruct* thepointptr)
{
    int k;
              /* offset for giant character array */
              /*16*2048=32768 so if number > 2048, index must be long!*/
              /*even worse with (MAXPTIDCHAR+1) which started at 21*/
              /* use pointer arith to avoid using a long integer as an*/
              /* array index: see K&R A7.7 pg 205 2nd ed.: */        
              /* pointer + integer: integer promoted to make new */
              /*  pointer work for its type */
     
    /*stored pointID string NOT conditioned */
    /*copy char str from storage */
    k=-1;/*so first used k is == 0 */
    do{       /*at very least thestring[0] will be set to \0 */
          k++;/*increment first so test is on current k */
          thestring[k] = com[(thepointptr->nameoffset) + (long)k];
      }while(thestring[k] != '\0'); 
}
/*___getptIDstring()________________________________________________________*/

/****recoverptIDstring()*****************************************************/
void recoverptIDstring(char* thestring, pointstruct* thepointptr)
{
   getptIDstring(thestring, thepointptr);
   if(thestring[0] == '\0')                  /*970919,970920*/
   {/*no ptIDstring for this point, return x,y,z*/
       sprintf(thestring,"%.2f,%.2f,%.2f"
       ,thepointptr->fx,thepointptr->fy,thepointptr->fz);
   }
}
/*___recoverptIDstring()____________________________________________________*/

/****storecommentstring()****************************************************/
void storecommentstring(char* thestring, pointstruct* thepointptr)
{
    int    j;
    
  /*store directly in giant character array */
    
  /*the first 3 char describe font, face, size of the charstring*/
  /*thus if thestring[3] == '\0' this is a zero length string and*/
  /*might as well have its nameoffset = 0 which points to a '\0' char*/
  if(thestring[3] == '\0') 
  {
      thepointptr->wordoffset = 0;/*wordcom[0]=='\0'*/
  }
  else
  {/*finite length comment of at least 1 char beyond 3 that set attributes*/
    /*check the length automatically including the 3 attribute chars */
    j=2;do{j++;}while(thestring[j]!='\0');
    if(nwordcom+j >= MAXWORDCOM-1) 
    {/*try to allocate more storage and increase MAXCOM*/
        allocwordcomarray(1000); /*MAGEINIT*/ /*try for (n) more */
        /*this is the wordcom array of pointer pwordcom  */
        /* and offset index nwordcom  */
    }
    if(nwordcom+j < MAXWORDCOM-1) /*insurance */
    {
        /*store starting offset of address in long place in pointstruct*/
        thepointptr->wordoffset = nwordcom;/*current offset into wordcom array*/
           /*this is the starting address for the current comment*/
        j = -1;
        do{
             j++;
             wordcom[nwordcom] = thestring[j];
             nwordcom++;
          }while(j<3 || thestring[j] != '\0');
    }
    else
    {/*no room for new comment, point it to where \0 stored*/
        thepointptr->wordoffset = 0; /*wordcom[0]=='\0' */
    }
  }/*finite length comment of at least 1 char beyond 3 that set attributes*/
}
/*___storecommentstring()____________________________________________________*/

/****getcommentstring()*******************************************************/
void getcommentstring(char* thestring, pointstruct* thepointptr)
{
    int k;
     
    /*stored pointID string NOT conditioned */
    /*copy char str from storage */
    /*unless its offset== 0 for a zero length string*/
    if(thepointptr->wordoffset > 0)
    {/*finite length string*/
        k=-1;/*so first used k is == 0 */
        do{       /*at very least thestring[0] will be set to \0 */
              k++;/*increment first so test is on current k */
              thestring[k] = wordcom[(thepointptr->wordoffset) + (long)k];
          }while( k<3 || thestring[k] != '\0'); 
          /*the first 3 char set attributes*/
    }
    else
    {/*zero length string*/
        /*for(k=0;k<=3;k++) thestring[k]=='\0';*/ /*980630*/
        for(k=0;k<=3;k++) thestring[k]='\0';/*980630*/
    }
}
/*___getcommentstring()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****calcintegerpoints()******************************************************/
void calcintegerpoints(pointstruct* thepointptr)
{
    if(  thepointptr->fx >= 0.0)
         thepointptr->ix = (int)(scalenew*(thepointptr->fx-fxcenternew) +.5);
    else thepointptr->ix = (int)(scalenew*(thepointptr->fx-fxcenternew) -.5);

    if(  thepointptr->fy >= 0.0)
         thepointptr->iy = (int)(scalenew*(thepointptr->fy-fycenternew) +.5);
    else thepointptr->iy = (int)(scalenew*(thepointptr->fy-fycenternew) -.5);

    if(  thepointptr->fz >= 0.0)
         thepointptr->iz = (int)(scalenew*(thepointptr->fz-fzcenternew) +.5);
    else thepointptr->iz = (int)(scalenew*(thepointptr->fz-fzcenternew) -.5);
}
/*___calcintegerpoints()_____________________________________________________*/

/****calcintegerpointscentered()**********************************************/
void calcintegerpointscentered(pointstruct* thepointptr, float xgctr, float ygctr, float zgctr)
{/*xyzgctr is offset of group center to newcenter at screen center 130115*/

   /*thepointptr->ix = (int)(scalenew*(thepointptr->fx - xgctr +fxcenternew));*/
   /*thepointptr->iy = (int)(scalenew*(thepointptr->fy - ygctr +fycenternew));*/
   /*thepointptr->iz = (int)(scalenew*(thepointptr->fz - zgctr +fzcenternew));*/
   thepointptr->ix = (int)(((thepointptr->fx - xgctr)*scalenew ));
   thepointptr->iy = (int)(((thepointptr->fy - ygctr)*scalenew ));
   thepointptr->iz = (int)(((thepointptr->fz - zgctr)*scalenew ));

/*
fprintf(stderr,"ixyz %d %d %d = scalenew %.3f *(fxyz %.3f %.3f %.3f - PO4 %.3f %.3f %.3f center: %.3f %.3f %.3f \n",thepointptr->ix,thepointptr->iy,thepointptr->iz,scalenew,thepointptr->fx,thepointptr->fy,thepointptr->fz,xgctr,ygctr,zgctr,fxcenternew,fycenternew,fzcenternew); 
*/
}
/*___calcintegerpointscentered()_____________________________________________*/

/****calcintegerfixedpoints()*************************************************/
void calcintegerfixedpoints(pointstruct* thepointptr)
{
    if(  thepointptr->fx >= 0.0)
         thepointptr->ix = (int)(Scale*(thepointptr->fx-fxcenter) +.5);
    else thepointptr->ix = (int)(Scale*(thepointptr->fx-fxcenter) -.5);

    if(  thepointptr->fy >= 0.0)
         thepointptr->iy = (int)(Scale*(thepointptr->fy-fycenter) +.5);
    else thepointptr->iy = (int)(Scale*(thepointptr->fy-fycenter) -.5);

    if(  thepointptr->fz >= 0.0)
         thepointptr->iz = (int)(Scale*(thepointptr->fz-fzcenter) +.5);
    else thepointptr->iz = (int)(Scale*(thepointptr->fz-fzcenter) -.5);
}
/*___calcintegerfixedpoints()________________________________________________*/

/****calcintegerscreenpoints()************************************************/
void calcintegerscreenpoints(pointstruct* thepointptr) /*971011, 001002*/
{
    float fudge=0;
    fudge = (float)oldmaxwide/(float)400;

  float fpfx,fpfy,fpfz;
  if(NDIM && LSCREENpoints && Linvert360yzon) /* 121212 */
  {
    fpfx = thepointptr->fx;
    fpfy = 360 - thepointptr->fy;
    fpfz = 360 - thepointptr->fz;

    if(  fpfx >= 0.0)
         thepointptr->ix =(int)((fpfx)*(fudge) +.5);
    else thepointptr->ix =(int)((fpfx)*(fudge) -.5);

    if(  fpfy >= 0.0)
         thepointptr->iy =(int)((fpfy)*(fudge) +.5);
    else thepointptr->iy =(int)((fpfy)*(fudge) -.5);

    if(  fpfz >= 0.0)
         thepointptr->iz =(int)((fpfz)*(fudge) +.5);
    else thepointptr->iz =(int)((fpfz)*(fudge) -.5);
  }
  else
  {
    if(  thepointptr->fx >= 0.0)
         thepointptr->ix =(int)((thepointptr->fx)*(fudge) +.5);
    else thepointptr->ix =(int)((thepointptr->fx)*(fudge) -.5);

    if(  thepointptr->fy >= 0.0)
         thepointptr->iy =(int)((thepointptr->fy)*(fudge) +.5);
    else thepointptr->iy =(int)((thepointptr->fy)*(fudge) -.5);

    if(  thepointptr->fz >= 0.0)
         thepointptr->iz =(int)((thepointptr->fz)*(fudge) +.5);
    else thepointptr->iz =(int)((thepointptr->fz)*(fudge) -.5);
  }
}  
/*___calcintegerscreenpoints()_______________________________________________*/
