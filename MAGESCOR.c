/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*//*MAGESCOR.c   MAGE scoring for (timed) connect-the-dots tests */
#include "MAGE.h"
#include "MAGEFLAG.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"

int  copynewgroup(void);
int  copyrefgroup(void);
int  copyallsingles(liststruct*,liststruct*);
int  copyallpairs(liststruct*,liststruct*);
int  pointsequal(float, float, float, float, float, float);
int  scorenewgrouppairs(void);
int  scorenewgroupsingles(void);
int  addNEWsets(void);
int  addanswerkeysets(void);

static unsigned int  KEY = 0; /*flag for VECTOR, LABEL, BALL type lists */

/****scoretest()**************************************************************/
int  scoretest()
{
   int  LOK = 0, Nitems = 0, Loldkey = 0;
   if(answerlistptr != NULL)
   {/*there is a putative answer-key list to score against*/
      if(answerkeylistptr != NULL) {Loldkey = 1; LOK = 1;}
      else {LOK = addanswerkeysets();} /*storage for working copy of answer*/
      if(LOK)
      {/*working key*/
         LOK = addNEWsets(); /*storage for working copy of drawn answer*/
         if(LOK && !Loldkey)
         {/*copy ref list, use its type to determine lines, labels, balls*/
            Nitems = copyrefgroup(); /*decide kind of points to be scored*/
            Scoretotal = Nitems; /*total unique items in answer list*/
            Scoretotalold = Nitems; /*total unique items in answer list*/
         }
         else if(LOK && Loldkey)
         {/*use old key and old Scoretotal*/
            Nitems = Scoretotalold;
            Scoretotal = Nitems; /*total unique items in answer list*/
         }
         if(LOK)
         {/*answerkey set up*/
            if(Nitems)
            {/*key finite*/
               Nitems = copynewgroup(); /*in terms of kind of points expected*/
               if(Nitems) /*there are some points in the answer...*/
               {/*get score crt answer answer list*/
                  if(KEY == VECTOR) {LOK = scorenewgrouppairs();}
                  else              {LOK = scorenewgroupsingles();}
                  if(LOK && Lrescoring) 
                  {
                     rescalekinemage();
                     LneedSetsinGrafWindow = 1;
                  }
               }/*get score crt answer answer list*/
            }/*key finite*/
         }/*answerkey set up*/
      }/*working key*/
   }/*there is a putative answer-key list to score against*/
   return(LOK);
}
/*___scoretest()_____________________________________________________________*/

/****copyrefgroup()***********************************************************/
int  copyrefgroup() /*copy new group to NEW group*/
{
   int  Nitems = 0;
   liststruct* triallistptr=NULL;

   if(answerlistptr!=NULL)
   {/*putative answer list to be used for scoring*/ 
      triallistptr = answerlistptr; 
      if(triallistptr->type & LABEL)
      {
         KEY = LABEL;
         Nitems = copyallsingles(answerkeylistptr, triallistptr);
      }
      else if(triallistptr->type & BALL)
      {
         KEY = BALL;
         Nitems = copyallsingles(answerkeylistptr, triallistptr);
      }
      else if(triallistptr->type & VECTOR)
      {
         KEY = VECTOR;
         Nitems = copyallpairs(answerkeylistptr, triallistptr);
         answerkeylistptr->color = numberofcolor("lilac"); /*MAGECOLR.c*/
         answerkeylistptr->owncolor = 1;
      }
   }
   return(Nitems);
}
/*___copyrefgroup()__________________________________________________________*/

/****copynewgroup()*********************************************************/
int  copynewgroup() /*copy new group to NEW group*/
{
   int  Nitems = 0;
   liststruct* triallistptr=NULL;

   if(drawnewlistptr==NULL && resultlistptr!=NULL)
   {/*putative test results to be scored*/ 
      triallistptr = resultlistptr; 
   }
   else if(drawnewlistptr!=NULL)
   {/*score newly drawn points*/
      triallistptr = drawnewlistptr;
   }
   if(KEY == VECTOR)
   {
      Nitems = copyallpairs(NEWlistptr, triallistptr);
   }
   else if(KEY == LABEL || KEY == BALL)
   {
      Nitems = copyallsingles(NEWlistptr, triallistptr);
   }
   return(Nitems);
}
/*___copynewgroup()__________________________________________________________*/

/****copyallpairs()*********************************************************/
int  copyallpairs(liststruct* TARGETlistptr, liststruct* triallistptr)
{
   int  LOK = 0, Npairs = 0;
   float Px=0,Py=0,Pz=0,Lx=0,Ly=0,Lz=0,Qx=0,Qy=0,Qz=0,Rx=0,Ry=0,Rz=0;
   pointstruct* TARGETpointptr=NULL;

   if(   TARGETlistptr != NULL 
      && triallistptr!=NULL 
      && triallistptr->firstpointptr!=NULL
      && (triallistptr->firstpointptr)->nextptr!=NULL
     ) /*pass if at least two points in the list*/
   {/*copy trial list points into TARGET list, filtering out duplicates*/
      TARGETlistptr->type = VECTOR; /*affirm VECTOR in this case*/
      searchpointptr = triallistptr->firstpointptr;
      while(searchpointptr != NULL)
      {/*scan all points in trial list*/
         LOK = 0;
         /*test this point for suitability as first of a PL pair*/
         if(  !( (searchpointptr->type)  & PRUNED) /*not pruned*/
            &&!( (searchpointptr->STATUS) & GHOST) /*not a ghost*/
            &&!( (searchpointptr->type) & STORED)  /*not extra*/
            &&!( (searchpointptr->type) & DOT)     /*not a dot*/
            &&!( (searchpointptr->type) & LABEL)   /*not a label*/
            &&!( (searchpointptr->type) & BALL)    /*not a ball*/
            && (  (searchpointptr->type) & VECTOR  )
            && (  (searchpointptr->type) & VARIANT1) == 0
            && (  (searchpointptr->type) & VARIANT2) == 0
            && (  (searchpointptr->type) & VARIANT3) == 0
            &&!(  searchpointptr==triallistptr->lastpointptr) /*not last point*/
           )
         {/*first point is a possible line start*/
         /*test following point for suitability as second of a PL pair*/
            if(  !( ((searchpointptr->nextptr)->type)  & PRUNED) /*not pruned*/
               &&!( ((searchpointptr->nextptr)->STATUS) & GHOST) /*not a ghost*/
               &&!( ((searchpointptr->nextptr)->type) & STORED)  /*not extra*/
               &&!( ((searchpointptr->nextptr)->type) & DOT)     /*not a dot*/
               &&!( ((searchpointptr->nextptr)->type) & LABEL)   /*not a label*/
               &&!( ((searchpointptr->nextptr)->type) & BALL)    /*not a ball*/
               && ( ((searchpointptr->nextptr)->type) & VECTOR  )
               && ( ((searchpointptr->nextptr)->type) & VARIANT1) == 0
               && ( ((searchpointptr->nextptr)->type) & VARIANT2) == 0
               && ( ((searchpointptr->nextptr)->type) & VARIANT3) == 0
               &&!( ((searchpointptr->nextptr)->type) & MOVETO_P) /*not P type*/
              )
            {/*second point is a possible line end*/
               /*check that points not equal, i.e. finite-length line*/
               Px = searchpointptr->fx;
               Py = searchpointptr->fy;
               Pz = searchpointptr->fz;
               Lx = (searchpointptr->nextptr)->fx;
               Ly = (searchpointptr->nextptr)->fy;
               Lz = (searchpointptr->nextptr)->fz;
               if( !pointsequal(Px, Py, Pz, Lx, Ly, Lz) )
               {/*putative PL pair*/
                  LOK = 1; /*possible pair*/
                  if(   TARGETlistptr != NULL 
                     && TARGETlistptr->firstpointptr != NULL)
                  {/*there are earlier pairs*/
                     TARGETpointptr = TARGETlistptr->firstpointptr;
                     while(TARGETpointptr != NULL)
                     {/*scan over previous pairs looking for duplicates*/
                        Qx = TARGETpointptr->fx;
                        Qy = TARGETpointptr->fy;
                        Qz = TARGETpointptr->fz;
                        Rx = (TARGETpointptr->nextptr)->fx;
                        Ry = (TARGETpointptr->nextptr)->fy;
                        Rz = (TARGETpointptr->nextptr)->fz;
                        if(  (  pointsequal(Px, Py, Pz, Qx, Qy, Qz)
                              &&pointsequal(Lx, Ly, Lz, Rx, Ry, Rz))
                           ||(  pointsequal(Px, Py, Pz, Rx, Ry, Rz)
                              &&pointsequal(Lx, Ly, Lz, Qx, Qy, Qz))
                          )
                        {/*duplicate point, not needed in working list*/
                           LOK = 0;
                           break; /*out of scan over previous pairs*/
                        }
                        if(  (TARGETpointptr->nextptr) != searchpointptr
                           &&(TARGETpointptr->nextptr) != 
                                             TARGETlistptr->lastpointptr
                           &&(TARGETpointptr->nextptr)->nextptr != 
                                                       searchpointptr
                           &&(TARGETpointptr->nextptr)->nextptr != 
                                             TARGETlistptr->lastpointptr
                          )
                           /*using && so penultimate protects ultimate test*/
                        {/*NOT caught up AND NOT reached end: get next pair*/
                           TARGETpointptr = (TARGETpointptr->nextptr)->nextptr;
                        }
                        else
                        {
                           TARGETpointptr = NULL;
                           LOK = 1; /*scan did NOT reveal a duplicate*/
                        }
                     }/*scan over previous pairs looking for duplicates*/
                  }/*there are earlier pairs*/
               }/*putative PL pair*/
               else{LOK=0;} /*zero-length line*/
               if(LOK)
               {/*append this pair to the NEW list*/
                  if(  LOK
                     &&(TARGETpointptr=allocnewpointstructure(TARGETlistptr))
                       !=NULL)
                  {/*make first point of PL pair*/
                     TARGETpointptr->type = VECTOR|MOVETO_P;
                     TARGETpointptr->fx = Px;
                     TARGETpointptr->fy = Py;
                     TARGETpointptr->fz = Pz;
                     getptIDstring(word,searchpointptr);
                     storeptIDstring(word,TARGETpointptr);
                  }/*make first point of PL pair*/
                  else{LOK=0;} /*allocation failed*/
                  if(  LOK
                     &&(TARGETpointptr=allocnewpointstructure(TARGETlistptr))
                       !=NULL)
                  {/*make second point of PL pair*/
                     TARGETpointptr->type = VECTOR;
                     TARGETpointptr->fx = Lx;
                     TARGETpointptr->fy = Ly;
                     TARGETpointptr->fz = Lz;
                     getptIDstring(word,searchpointptr->nextptr);
                     storeptIDstring(word,TARGETpointptr);
                  }/*make second point of PL pair*/
                  else{LOK=0;} /*allocation failed*/
               }/*append this pair to the NEW list*/
            }/*second point is a possible line end*/
            else{LOK=0;} /*second point not suitable*/
         }/*first point is a possible line start*/
         else{LOK=0;} /*first point not suitable*/
         if(LOK) {Npairs++;}
         if(  (searchpointptr->nextptr) != triallistptr->lastpointptr )
            /*already know that searchpointptr is not last point*/
            /*ensure that for next search cycle also*/
         {/*at least one point beyond next for a possible pair*/
            searchpointptr = searchpointptr->nextptr;
         }
         else
         {
            searchpointptr = NULL;
         }
      }/*scan all points in trial list*/
   }/*copy trial list points into TARGET list, filtering out duplicates*/
   return(Npairs);
}
/*___copyallpairs()__________________________________________________________*/

/****copyallsingles()*********************************************************/
int  copyallsingles(liststruct* TARGETlistptr, liststruct* triallistptr)
{
   int  LOK = 0, Npoints = 0;
   float Px=0,Py=0,Pz=0,Qx=0,Qy=0,Qz=0;
   pointstruct* TARGETpointptr=NULL;

   if(   TARGETlistptr != NULL 
      && triallistptr!=NULL 
      && triallistptr->firstpointptr!=NULL
     ) /*pass if at least one point in the list*/
   {/*copy trial list points to TARGET points, filtering out duplicates*/
      if(KEY == LABEL) {TARGETlistptr->type = LABEL;} /*use defined entity*/
      else if(KEY == BALL) {TARGETlistptr->type = BALL;} /*use defined entity*/
      else {TARGETlistptr->type = KEY;} /*punt with unsigned int entity*/
      searchpointptr = triallistptr->firstpointptr;
      while(searchpointptr != NULL)
      {/*scan all points in trial list*/
         LOK = 0;
         /*test this point for suitability as a point*/
         if(  !( (searchpointptr->type)  & PRUNED) /*not pruned*/
            &&!( (searchpointptr->STATUS) & GHOST) /*not a ghost*/
            &&!( (searchpointptr->type) & STORED)  /*not extra*/
            && (  (searchpointptr->type) & KEY  ) /*is a ball or label ... */
           )
         {/*point is possible*/
            Px = searchpointptr->fx;
            Py = searchpointptr->fy;
            Pz = searchpointptr->fz;
            LOK = 1; /*possible point*/
            if(   TARGETlistptr != NULL 
               && TARGETlistptr->firstpointptr != NULL)
            {/*there are earlier points*/
               TARGETpointptr = TARGETlistptr->firstpointptr;
               while(TARGETpointptr != NULL)
               {/*scan over previous points looking for duplicates*/
                  Qx = TARGETpointptr->fx;
                  Qy = TARGETpointptr->fy;
                  Qz = TARGETpointptr->fz;
                  if(pointsequal(Px, Py, Pz, Qx, Qy, Qz))
                  {/*duplicate point, not needed in working list*/
                     LOK = 0;
                     break; /*out of scan over previous points*/
                  }
                  if(  (TARGETpointptr->nextptr) != searchpointptr
                     &&(TARGETpointptr->nextptr) != NULL
                    )
                  {/*NOT caught up AND NOT reached end: get next point*/
                     TARGETpointptr = TARGETpointptr->nextptr;
                  }
                  else
                  {
                     TARGETpointptr = NULL;
                     LOK = 1; /*scan did NOT reveal a duplicate*/
                  }
               }/*scan over previous points looking for duplicates*/
            }/*there are earlier points*/
            if(LOK)
            {/*append this point to the NEW list*/
               if(  (TARGETpointptr=allocnewpointstructure(TARGETlistptr))
                    !=NULL)
               {/*make this point*/
                  TARGETpointptr->type = KEY;
                  TARGETpointptr->fx = Px;
                  TARGETpointptr->fy = Py;
                  TARGETpointptr->fz = Pz;
                  getptIDstring(word,searchpointptr);
                  storeptIDstring(word,TARGETpointptr);
               }/*make this point*/
               else{LOK=0;} /*allocation failed*/
            }/*append this point to the NEW list*/
         }/*point is possible*/
         else{LOK=0;} /*point not suitable*/
         if(LOK) {Npoints++;}
         if(  (searchpointptr->nextptr) != NULL )
         {
            searchpointptr = searchpointptr->nextptr;
         }
         else
         {
            searchpointptr = NULL;
         }
      }/*scan all points in trial list*/
   }/*copy trial list points to TARGET points, filtering out duplicates*/
   return(Npoints);
}
/*___copyallsingles()________________________________________________________*/

/****pointsequal()************************************************************/
int  pointsequal(float Px, float Py, float Pz, float Lx, float Ly, float Lz)
{
   int  Lequal = 0;
   float dx,dy,dz,delta=(float)0.0019;

   if( Px <= Lx ) dx = Lx - Px; 
   else dx = Px - Lx;
   if( Py <= Ly ) dy = Ly - Py; 
   else dy = Py - Ly;
   if( Pz <= Lz ) dz = Lz - Pz; 
   else dz = Pz - Lz;
   if(  (dx < delta) &&(dy < delta) &&(dz < delta)) Lequal = 1;
   return(Lequal);
}
/*___pointsequal()___________________________________________________________*/

/****scorenewgrouppairs()****************************************************/
int  scorenewgrouppairs() /*against answer group*/
{
   int Lreturn=1,Lhit=0,j=0;
   float Px=0,Py=0,Pz=0,Lx=0,Ly=0,Lz=0,Qx=0,Qy=0,Qz=0,Rx=0,Ry=0,Rz=0;

   NEWpointptr = NEWlistptr->firstpointptr;
   while(NEWpointptr != NULL && NEWpointptr->nextptr != NULL) 
   {/*scan all adjacent pairs of points in NEW copy of drawnew list*/
      Px = NEWpointptr->fx; /*PL pair of drawnew group*/
      Py = NEWpointptr->fy;
      Pz = NEWpointptr->fz;
      Lx = (NEWpointptr->nextptr)->fx;
      Ly = (NEWpointptr->nextptr)->fy;
      Lz = (NEWpointptr->nextptr)->fz;
      Lhit = 0;
      /*check this pair against the pairs in the answer list*/
      answerkeypointptr = answerkeylistptr->firstpointptr;
      while(answerkeypointptr != NULL && (answerkeypointptr->nextptr) != NULL)
      {/*scan all adjacent pairs of points in answer list*/
         Qx = answerkeypointptr->fx;/*answer PL pair, finite length line*/
         Qy = answerkeypointptr->fy;
         Qz = answerkeypointptr->fz;
         Rx = (answerkeypointptr->nextptr)->fx;
         Ry = (answerkeypointptr->nextptr)->fy;
         Rz = (answerkeypointptr->nextptr)->fz;
         if(  (  pointsequal(Px, Py, Pz, Qx, Qy, Qz)
               &&pointsequal(Lx, Ly, Lz, Rx, Ry, Rz))
            ||(  pointsequal(Px, Py, Pz, Rx, Ry, Rz)
               &&pointsequal(Lx, Ly, Lz, Qx, Qy, Qz))
           )
         {/*duplicate line to be scored*/
            Lhit = 1;
            break; /*out of scan over answer list*/
         }/*duplicate line*/
         if(  (answerkeypointptr->nextptr) != 
                           answerkeylistptr->lastpointptr
            &&(answerkeypointptr->nextptr)->nextptr != 
                           answerkeylistptr->lastpointptr
           ) /*using && so penultimate condition protects ultimate test*/
         {answerkeypointptr = (answerkeypointptr->nextptr)->nextptr;}
         else
         {answerkeypointptr = NULL;}
      }/*scan all adjacent pairs of points in answer list*/
      if(Lhit) 
      {/*test result NEW list is strictly in PLPLPL... form */
         /*although it is always the second of a drawn sequence*/
         /*determines its color.  So only need to explicitly change*/
         /*color of the second point of this tested line*/
         /*but changing both PL makes reading the file easier!*/ 
         Scorepositive++; /*global*/ 
         NEWpointptr->colorwidth = numberofcolor("green");
         (NEWpointptr->nextptr)->colorwidth = numberofcolor("green");
      }
      else     
      {/*no match*/ 
         Scorenegative++; /*global*/ 
         NEWpointptr->colorwidth = numberofcolor("hotpink");
         (NEWpointptr->nextptr)->colorwidth = numberofcolor("hotpink");
      }
      if(  (NEWpointptr->nextptr) != 
                         NEWlistptr->lastpointptr
         &&(NEWpointptr->nextptr)->nextptr != 
                         NEWlistptr->lastpointptr
        ) /*using && so penultimate condition protects ultimate test*/
      {NEWpointptr = (NEWpointptr->nextptr)->nextptr;}
      else
      {NEWpointptr = NULL;}
   }/*scan all adjacent pairs of points in NEW copy of drawnew list*/
   /*At least one line pair to score if gets to this subroutine*/
   if(scorelistptr != NULL)
   {/*extra list for score exists*/
      scorelistptr->STATUS = scorelistptr->STATUS | STATICFLAG | SCREENFLAG;
      /*write scores as static labels in screen coordinates*/

      for(j=1; j<=2; j++)
      {/*make 2 static labels in screen coordinates for the scores*/
         if((scorepointptr = allocnewpointstructure(scorelistptr))==NULL)
         {/*allocation failed*/
            sprintf(alertstr,"failed to alloc a score label point");
            alertstr2[0]='\0';alertstr3[0]='\0';
            DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/
         }/*allocation failed*/
         else if(j==1)
         {
            sprintf(word,"Correct: %d out of %d",Scorepositive,Scoretotal);
            storeptIDstring(word,scorepointptr);
            scorepointptr->type = LABEL;
            scorepointptr->colorwidth = numberofcolor("green");
            scorepointptr->fx = -180;
            scorepointptr->fy =  160;
            scorepointptr->fz = 0;
         }
         else if(j==2)
         {
            sprintf(word,"Wrong: %d",Scorenegative);
            storeptIDstring(word,scorepointptr);
            scorepointptr->type = LABEL;
            scorepointptr->colorwidth = numberofcolor("hotpink");
            scorepointptr->fx = -180;
            scorepointptr->fy =  140;
            scorepointptr->fz = 0;
         }
      }/*make 2 static labels in screen coordinates for the scores*/
   }/*extra list for score exists*/
   return(Lreturn);
}
/*___scorenewgrouppairs()____________________________________________________*/

/****scorenewgroupsingles)****************************************************/
int  scorenewgroupsingles() /*against answer group*/
{
   int Lreturn=1,Lhit=0,j=0;
   float Px=0,Py=0,Pz=0,Qx=0,Qy=0,Qz=0;

   NEWpointptr = NEWlistptr->firstpointptr;
   while(NEWpointptr != NULL) 
   {/*scan all points in NEW copy of drawnew list*/
      Px = NEWpointptr->fx; 
      Py = NEWpointptr->fy;
      Pz = NEWpointptr->fz;
      Lhit = 0;
      /*check this point against the points in the answer list*/
      answerkeypointptr = answerkeylistptr->firstpointptr;
      while(answerkeypointptr != NULL)
      {/*scan all points in answer list*/
         Qx = answerkeypointptr->fx;
         Qy = answerkeypointptr->fy;
         Qz = answerkeypointptr->fz;
         if( pointsequal(Px, Py, Pz, Qx, Qy, Qz))
         {/*duplicate point to be scored*/
            Lhit = 1;
            break; /*out of scan over answer list*/
         }
         if(  (answerkeypointptr->nextptr) != NULL )
         {answerkeypointptr = answerkeypointptr->nextptr;}
         else
         {answerkeypointptr = NULL;}
      }/*scan all points in answer list*/
      if(Lhit) 
      {/*test result NEW list point-color correct*/
         Scorepositive++; /*global*/ 
         NEWpointptr->colorwidth = numberofcolor("green");
      }
      else     
      {/*no match*/ 
         Scorenegative++; /*global*/ 
         NEWpointptr->colorwidth = numberofcolor("hotpink");
      }
      if(  (NEWpointptr->nextptr) != NULL )
      {NEWpointptr = NEWpointptr->nextptr;}
      else
      {NEWpointptr = NULL;}
   }/*scan all points in NEW copy of drawnew list*/
   /*write scores as static labels in screen coordinates*/
   if(scorelistptr != NULL)
   {/*extra list for score exists*/
      scorelistptr->STATUS = scorelistptr->STATUS | STATICFLAG | SCREENFLAG;

      for(j=1; j<=2; j++)
      {/*make 2 labels for the scores*/
         if((scorepointptr = allocnewpointstructure(scorelistptr))==NULL)
         {/*allocation failed*/
            sprintf(alertstr,"failed to alloc a score label point");
            alertstr2[0]='\0';alertstr3[0]='\0';
            DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/
         }/*allocation failed*/
         else if(j==1)
         {
            sprintf(word,"Correct: %d out of %d",Scorepositive,Scoretotal);
            storeptIDstring(word,scorepointptr);
            scorepointptr->type = LABEL;
            scorepointptr->colorwidth = numberofcolor("green");
            scorepointptr->fx = -180;
            scorepointptr->fy =  160;
            scorepointptr->fz = 0;
         }
         else if(j==2)
         {
            sprintf(word,"Wrong: %d",Scorenegative);
            storeptIDstring(word,scorepointptr);
            scorepointptr->type = LABEL;
            scorepointptr->colorwidth = numberofcolor("hotpink");
            scorepointptr->fx = -180;
            scorepointptr->fy =  140;
            scorepointptr->fz = 0;
         }
      }/*make 2 labels for the scores*/
   }/*extra list for score exists*/
   return(Lreturn);
}
/*___scorenewgroupsingles()__________________________________________________*/

/****addNEWsets()*******************************************************/
int  addNEWsets()
{
   if( !allocgrupstructure() )
   {
      sprintf(alertstr,"failed to alloc group for NEW");
      goto allocerr;
   }
   else
   {
    /*thisgrupptr->on = -99;*/ /*effectively deleted so will not be outputted*/
      thisgrupptr->on = 1; /* ON for verifying the test scoring */
      thisgrupptr->type = 0; /*nothing special*/
      thisgrupptr->firstsgrpptr = NULL;
      thisgrupptr->lastsgrpptr = NULL;
      thisgrupptr->STATUS = ON | DOMINANT;
      thisgrupptr->master = 0; /*no master*/
      NEWgrupptr = thisgrupptr;
      sprintf(thisgrupptr->name," score");
      /*ignoring items like NDIM 060622*/
   }
   if( !allocsgrpstructure() )
   {
      sprintf(alertstr,"failed to alloc subgroup for NEW");
      goto allocerr;
   }
   else
   {
      thissgrpptr->on = 1;
      thissgrpptr->firstlistptr = NULL;
      thissgrpptr->lastlistptr  = NULL;
      thissgrpptr->STATUS = NOBUTTON | NEVERSHOW;
      thissgrpptr->grupptr = thisgrupptr;
      thisgrupptr->firstsgrpptr = thissgrpptr;
      thisgrupptr->lastsgrpptr  = thissgrpptr;
      NEWsgrpptr = thissgrpptr;
      sprintf(thissgrpptr->name,"score");
      /*ignoring items like NDIM 060622*/
   }
   if( !allocliststructure() )
   {
      sprintf(alertstr,"failed to alloc list for NEW vectors");
      goto allocerr;
   }
   else
   {
      thislistptr->on = 1; /*list display on*/
      thislistptr->type = 0; /*maybe not a vector*/
      thislistptr->firstpointptr = NULL;
      thislistptr->lastpointptr = NULL;
      thislistptr->sgrpptr = thissgrpptr;
      thissgrpptr->firstlistptr = thislistptr;
      thissgrpptr->lastlistptr = thislistptr;
      thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
      thislistptr->owncolor = 1;
      sprintf(thislistptr->name,"items");
      NEWlistptr = thislistptr;
      /*ignoring items like NDIM 060622*/
   }
   if( !allocliststructure() )
   {
      sprintf(alertstr,"failed to alloc list for score values");
      goto allocerr;
   }
   else
   {
      thislistptr->on = 1; /*list display on*/
      thislistptr->type = LABEL; 
      thislistptr->firstpointptr = NULL;
      thislistptr->lastpointptr = NULL;
      thislistptr->sgrpptr = thissgrpptr; /*same as for line-list*/
      /*thissgrpptr->firstlistptr = thislistptr; ALREADY DEFINED*/
      thissgrpptr->lastlistptr = thislistptr; /*overrides earlier statement*/
      thislistptr->previousptr = thissgrpptr->firstlistptr;
      (thissgrpptr->firstlistptr)->nextptr = thislistptr;
      thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
      thislistptr->owncolor = 1; 
      sprintf(thislistptr->name,"value");
      scorelistptr = thislistptr;
      /*ignoring items like NDIM 060622*/
   }
                    
   return(1); /*good*/
   allocerr:
   {
      dosinglealert(0); /*shows alertstr*/ /*____DLOG.C*/
      return(0);
   }
}
/*___addNEWsets()______________________________________________________*/

/****addanswerkeysets()****************************************************/
int  addanswerkeysets()
{
   if( !allocgrupstructure() )
   {
      sprintf(alertstr,"failed to alloc group for answerkey");
      goto allocerr;
   }
   else
   {/*establishes a new global thisgrupptr*/
    /*thisgrupptr->on = -99;*/ /*effectively deleted so will not be outputted*/
      thisgrupptr->on = 1; /* ON for verifying the test scoring */
      thisgrupptr->type = 0; /*nothing special*/
      thisgrupptr->firstsgrpptr = NULL;
      thisgrupptr->lastsgrpptr = NULL;
      thisgrupptr->STATUS = ON | DOMINANT;
      thisgrupptr->master = 0; /*no master*/
      answerkeygrupptr = thisgrupptr;
      sprintf(thisgrupptr->name," answer key"); /*beware {name length} */
      /*ignoring items like NDIM 060622*/
   }
   if( !allocsgrpstructure() )
   {
      sprintf(alertstr,"failed to alloc subgroup for answerkey");
      goto allocerr;
   }
   else
   {/*establishes a new global thissgrpptr*/
      thissgrpptr->on = 1;
      thissgrpptr->firstlistptr = NULL;
      thissgrpptr->lastlistptr  = NULL;
      thissgrpptr->STATUS = NOBUTTON | NEVERSHOW;
      thissgrpptr->grupptr = thisgrupptr; /*as alloc just above*/
      thisgrupptr->firstsgrpptr = thissgrpptr;
      thisgrupptr->lastsgrpptr  = thissgrpptr;
      answerkeysgrpptr = thissgrpptr;
      sprintf(thissgrpptr->name,"answer key");
      /*ignoring items like NDIM 060622*/
   }
   if( !allocliststructure() )
   {
      sprintf(alertstr,"failed to alloc list for answerkey vectors");
      goto allocerr;
   }
   else
   {/*establishes a new global thislistptr*/
      thislistptr->on = 1; /*list display on*/
      thislistptr->type = 0; /*maybe not vector*/
      thislistptr->firstpointptr = NULL;
      thislistptr->lastpointptr = NULL;
      thislistptr->sgrpptr = thissgrpptr; /*as alloc just above*/
      thissgrpptr->firstlistptr = thislistptr;
      thissgrpptr->lastlistptr = thislistptr;
      thislistptr->color = numberofcolor("gray"); /*MAGECOLR.c*/
      thislistptr->owncolor = 1;
      sprintf(thislistptr->name,"items");
      answerkeylistptr = thislistptr;
      /*ignoring items like NDIM 060622*/
   }
                    
   return(1); /*good*/
   allocerr:
   {
      dosinglealert(0); /*shows alertstr*/ /*____DLOG.C*/
      return(0);
   }
}
/*___addanswerkeysets()___________________________________________________*/

