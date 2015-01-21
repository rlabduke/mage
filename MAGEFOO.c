                   /*MAGEFOO.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"
#include "MAGEMENU.h"
#include "MAGEANGL.h"
#define EXTERNFOO
#include "MAGEFOO.h"

/****mobilehitfoocheck()******************************************************/
int mobilehitfoocheck()
{
   struct pointstruct* thepointptr;
   struct pointstruct* foopointptr;
   struct liststruct* mobilelistptr;
   struct liststruct* foolistptr;
   double distsq=0.0; /*041210*/
   int LHIT=0;
   
 mobilelistptr = mobilegrupptr->lastsgrpptr->lastlistptr;
 foolistptr = foogrupptr->lastsgrpptr->firstlistptr;
 foopointptr = foolistptr->firstpointptr;
 while(foopointptr != NULL)
 {
   thepointptr = mobilelistptr->firstpointptr;
   while(thepointptr!=NULL)
   {
    distsq = (
     ((thepointptr->fx)-(foopointptr->fx))*((thepointptr->fx)-(foopointptr->fx))
    +((thepointptr->fy)-(foopointptr->fy))*((thepointptr->fy)-(foopointptr->fy))
    +((thepointptr->fz)-(foopointptr->fz))*((thepointptr->fz)-(foopointptr->fz))
    );
    if(distsq < fooradius*fooradius)
    {
      LHIT = 1;
      break;
    }  
      if(thepointptr == mobilelistptr->lastpointptr) thepointptr=NULL;
      else thepointptr = thepointptr->nextptr;           
   }
   if(foopointptr == foolistptr->lastpointptr){foopointptr = NULL;}
   else {foopointptr = foopointptr->nextptr;}
 }
 return(LHIT);
}
/*___mobilehitfoocheck()_____________________________________________________*/

/****distancef()**************************************************************/
float distancef(float ax, float ay, float az, float bx, float by, float bz)
{
   float freturn=0;
   freturn = (float)sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by)+(az-bz)*(az-bz));
   return(freturn);
}
/*___distancef()_____________________________________________________________*/

/****distancefsq()*************************************************************/
float distancefsq(float ax, float ay, float az, float bx, float by, float bz)
{/*distancefsq 041201*/
   float freturn=0;
   freturn = (float)((ax-bx)*(ax-bx)+(ay-by)*(ay-by)+(az-bz)*(az-bz));
   return(freturn);
}
/*___distancefsq()____________________________________________________________*/

/****cagehitfoocheck()********************************************************/
int  cagehitfoocheck(float tx, float ty, float tz)
{
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   struct liststruct* thelistptr=NULL;
   struct liststruct* endlistptr=NULL;
   struct sgrpstruct* thesgrpptr=NULL;
   struct sgrpstruct* endsgrpptr=NULL;
   struct grupstruct* thegrupptr=NULL;
   float distsq=0; /*sq 041210*/
   int   ireturn=0;
   
   thegrupptr = referencegrupptr;
   thesgrpptr = thegrupptr->firstsgrpptr;
   endsgrpptr = thegrupptr->lastsgrpptr;
   while(thesgrpptr != NULL)
   {
      thelistptr = thesgrpptr->firstlistptr;
      endlistptr = thesgrpptr->lastlistptr;
      while(thelistptr != NULL)
      {
         thepointptr = thelistptr->firstpointptr;
         endpointptr = thelistptr->lastpointptr;
         while(thepointptr != NULL)
         {
          distsq =
          distancefsq(tx,ty,tz,thepointptr->fx,thepointptr->fy,thepointptr->fz);
           if(distsq < fooradius*fooradius)
           {
               ireturn = 1; /*hit a wall*/
               break; /*only takes one*/
           }
           if(thepointptr == endpointptr) {thepointptr = NULL;}
           else {thepointptr = thepointptr->nextptr;}
         }
         if(ireturn==1) {break;}
         if(thelistptr == endlistptr) {thelistptr = NULL;}
         else {thelistptr = thelistptr->nextptr;}
      }
      if(ireturn==1) {break;}
      if(thesgrpptr == endsgrpptr) {thesgrpptr = NULL;}
      else {thesgrpptr = thesgrpptr->nextptr;}
   }
   return(ireturn);
}
/*___cagehitfoocheck()_______________________________________________________*/

/****allhitfoocheck()********************************************************/
int  allhitfoocheck(float* ptx, float* pty, float* ptz, int Lfirstplacement)
{
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   struct pointstruct* nearestpointptr=NULL; /*030709*/
   struct liststruct* thelistptr=NULL;
   struct liststruct* endlistptr=NULL;
   struct sgrpstruct* thesgrpptr=NULL;
   struct sgrpstruct* endsgrpptr=NULL;
   struct grupstruct* thegrupptr=NULL;
   struct grupstruct* endgrupptr=NULL;
   float  dist=0,distsq=0; /*sq 041210*/
   int    ireturn=0;
   int    Lvoid=1; /*slimemode presumes out in void until find a distal point*/
   int    Loutofbox=0,Lwayoutofbox=0; /*030630 slimemode*/
   float  proximal=0,distal=0,nearestdist=32000;
   int    Lcollision=0,Ntimes=0,Lsearch=1; /*030709*/
   float  foovecx=0,foovecy=0,foovecz=0; /*030709*/
   float  pointradius = 0; /*040402*/
   int    Lfoobarrier = 0; /*090912*/
   
 while(Lsearch)
 {/*one pass for production run, up to Ntimes for first placement of foo*/
   Lcollision = 0; /*presume not in collosion 030709*/
   if(Lslimemode) Lvoid = 1; /*start with presumption that foo is out in void*/
   else Lvoid = 0; /*not slimemold: start anywhere not in collision 040402*/
   nearestdist = 32000; /*reset each pass  030709*/
   thegrupptr = firstgrupptr;
   endgrupptr = lastgrupptr;
   while(thegrupptr != NULL)
   {/*search through all reference groups, subgroups, lists, points*/
      /*if(thegrupptr == referencegrupptr)*/
      if(  ( thegrupptr != foogrupptr )
         &&((thegrupptr->KKIND & FOOKKIND)!=FOOKKIND )
         &&((thegrupptr->KKIND & IGNORABLEKKIND)!=IGNORABLEKKIND )
         &&( thegrupptr != markergrupptr )
         &&( thegrupptr != measuregrupptr )
         &&( thegrupptr != drawgrupptr )
         &&( thegrupptr != mobilegrupptr )
        )
      {/*group to be checked against*/
/*
fprintf(stderr,"thegrupptr->name: %s, thegrupptr->KKIND: %d, FOOBARRIERKKIND: %d, AND: %d\n"
,thegrupptr->name,thegrupptr->KKIND,FOOBARRIERKKIND,thegrupptr->KKIND & FOOBARRIERKKIND);
*/
         if((thegrupptr->KKIND & FOOBARRIERKKIND)!=FOOBARRIERKKIND )/*090912*/
         {Lfoobarrier = 0;} /*foo barrier like Lvoid: NOT surfaced 090912*/
         else
         {Lfoobarrier = 1;}
         thesgrpptr = thegrupptr->firstsgrpptr;
         endsgrpptr = thegrupptr->lastsgrpptr;
         while(thesgrpptr != NULL)
         {
            thelistptr = thesgrpptr->firstlistptr;
            endlistptr = thesgrpptr->lastlistptr;
            while(thelistptr != NULL)
            {
               thepointptr = thelistptr->firstpointptr;
               endpointptr = thelistptr->lastpointptr;
               while(thepointptr != NULL)
               {
                  if( (thepointptr->type & PRUNED) != PRUNED ) /*030405*/
                  {/*existing point*/
                     Loutofbox=0;
                     Lwayoutofbox=0; /*030630 slimemode*/
                     if((thepointptr->radius==0)&&(thepointptr->listptr!= NULL))
                     {
                        pointradius = (thepointptr->listptr)->radius;
                     }
                     else
                     {
                        pointradius = thepointptr->radius; /*040402*/
                     }
                     proximal = fooradius + pointradius;
                     distal = proximal + 2*foogridstep;
                     /*out of neighborhood box if too big in any coord*/
                     if(   fabs(*ptx - thepointptr->fx) > proximal
                        || fabs(*pty - thepointptr->fy) > proximal
                        || fabs(*ptz - thepointptr->fz) > proximal )
                     {/*out-of-neighborhood*/
                        Loutofbox = 1;
                        if(Lslimemode
                            && (   fabs(*ptx - thepointptr->fx) > distal
                                || fabs(*pty - thepointptr->fy) > distal
                                || fabs(*ptz - thepointptr->fz) > distal ) )
                        {/*wayout-of-neighborhood*/
                           Lwayoutofbox = 1;
                        }
                     }
                     if(!Lwayoutofbox || !Loutofbox || Lfirstplacement)
                     {/*exact distance check*/
                        distsq =
                           distancefsq(*ptx,*pty,*ptz
                              ,thepointptr->fx,thepointptr->fy,thepointptr->fz);
                        if(!Loutofbox && (distsq < proximal*proximal) )
                        {
                           Lcollision = 1; /*hit a wall*/
                           if(Lfirstplacement)
                           {
                              dist = (float)sqrt(distsq); /*041210*/
                              if(   (nearestpointptr == NULL)
                                  ||(dist < nearestdist) )
                              {
                                 nearestdist = dist;
                                 nearestpointptr = thepointptr;
                              }
                           }
                           else
                           {
                              break; /*only takes one in a production run*/
                           }
                        }
                        else if(   Lslimemode && Lvoid) /*slimemode 030630*/
                        {/*slimemode and not yet near anything, check distal*/
                           if(distsq < distal*distal)
                           {/*this one is near something*/
                              Lvoid = 0;/*only 1 <distal to be not in void*/
                           }
                           else if(Lfirstplacement)
                           {/*find closest approach*/
                              dist = (float)sqrt(distsq); /*041210*/
                              if(   (nearestpointptr == NULL)
                                  ||(dist < nearestdist) )
                              {
                                 nearestdist = dist;
                                 nearestpointptr = thepointptr;
                              }
                           }
                        }
                     }/*exact distance check*/
/*if(Lcollision)fprintf(stderr,"past exact distance check: Lcollision==1\n");*/
                  }/*existing point*/
                  if(thepointptr == endpointptr) {thepointptr = NULL;}
                  else {thepointptr = thepointptr->nextptr;}
               }
/*if(Lcollision)fprintf(stderr,"past points in list: Lcollision==1\n");*/
               if(Lcollision==1 && !Lfirstplacement) {break;}
               if(thelistptr == endlistptr) {thelistptr = NULL;}
               else {thelistptr = thelistptr->nextptr;}
            }
/*if(Lcollision)fprintf(stderr,"past lists in subgroup: Lcollision==1\n");*/
            if(Lcollision==1 && !Lfirstplacement) {break;}
            if(thesgrpptr == endsgrpptr) {thesgrpptr = NULL;}
            else {thesgrpptr = thesgrpptr->nextptr;}
         }
/*if(Lcollision)fprintf(stderr,"past subgroups in group: Lcollision==1\n");*/
/*
fprintf(stderr,"thegrupptr->name: %s, Lfoobarrier== %d, Lcollision== %d, Lvoid== %d\n",thegrupptr->name,Lfoobarrier,Lcollision,Lvoid);
*/
      }/*group to be checked against*/
      if(Lcollision==1 && !Lfirstplacement) {break;} /*090912*/
      if(thegrupptr == endgrupptr) {thegrupptr = NULL;}
      else {thegrupptr = thegrupptr->nextptr;}
   }/*search through all reference groups, subgroups, lists, points*/
/*if(Lcollision)fprintf(stderr,"past groups: Lcollision==1\n");*/
   if(   nearestpointptr != NULL /*avoid pathologic all group ignorable 050916*/
      && Lfirstplacement 
      && ((Lcollision==1)||(Lvoid==1)) 
      && (Ntimes < 10))
   {/*trouble doing first placement of the foo*/
      foovecx = *ptx - nearestpointptr->fx;
      foovecy = *pty - nearestpointptr->fy;
      foovecz = *ptz - nearestpointptr->fz;
      if((nearestpointptr->radius == 0)&&(nearestpointptr->listptr != NULL))
      {
         pointradius = (nearestpointptr->listptr)->radius;
      }
      else
      {
         pointradius = nearestpointptr->radius; /*040402*/
      }
      *ptx = nearestpointptr->fx 
        + (fooradius + pointradius+(float)0.05)*foovecx/nearestdist;
      *pty = nearestpointptr->fy 
        + (fooradius + pointradius+(float)0.05)*foovecy/nearestdist;
      *ptz = nearestpointptr->fz 
        + (fooradius + pointradius+(float)0.05)*foovecz/nearestdist;
      Ntimes++;
   }
   else
   {
      Lsearch = 0;
   }
 }/*one pass for production run, up to Ntimes for first placement of foo*/
   if(Lcollision==1)
   {
      if(Lfoobarrier)/*090912*/
      {ireturn = -1;} /*foo barrier acts like Lvoid: NOT surfaced  090912*/
      else
      {ireturn = 1;} /*collision engenders surface */
   }
   else if(Lslimemode && Lvoid) /*slimemode 030630*/
   {
      ireturn = -1;
   }
   else
   {
      ireturn = 0;
   }
/*
fprintf(stderr,"\n*****************Lcollision== %d, Lfoobarrier== %d, ireturn== %d\n\n",Lcollision,Lfoobarrier,ireturn);
*/
   return(ireturn);
}
/*___allhitfoocheck()_______________________________________________________*/

/****oldfoohere()*************************************************************/
int  oldfoohere(float tx, float ty, float tz)
{
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   float distsq=0; /*041210*/
   int   ireturn=0;
   
   thepointptr = foogrupptr->lastsgrpptr->lastlistptr->firstpointptr;
   endpointptr = foogrupptr->lastsgrpptr->lastlistptr->lastpointptr;
   while(thepointptr != NULL)
   {
   distsq=distancefsq(tx,ty,tz,thepointptr->fx,thepointptr->fy,thepointptr->fz);
      if(distsq < 0.0001) /*041210 was dist < 0.01*/
      {
         ireturn = 1; /*is an old foo here*/
         break; /*only takes one*/
      }
      if(thepointptr == endpointptr) {thepointptr = NULL;}
      else {thepointptr = thepointptr->nextptr;}
   }
   return(ireturn);
}
/*___oldfoohere()____________________________________________________________*/

/****releasefoos()************************************************************/
void releasefoos()
{
   struct pointstruct* thispointptr=NULL;
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   
   endpointptr = foogrupptr->lastsgrpptr->lastlistptr->firstpointptr;
   thepointptr = foogrupptr->lastsgrpptr->lastlistptr->lastpointptr;
   while(thepointptr != NULL)
   {/*work backwards, leaving first point*/
      if(thepointptr == endpointptr) 
      {
         thepointptr->type = BALL; /*insure is a ball*/
         thepointptr->number = 0; /*reclear neighbor flags*/
         thepointptr = NULL;
      }
      else 
      {
         thispointptr = thepointptr->previousptr;
         destroypointstructure(thepointptr);
         thepointptr = thispointptr;
      }
   }
}
/*___releasefoos()___________________________________________________________*/

/****foogridconstruct()*******************************************************/
void foogridconstruct()
{
   unsigned long ACCUMM=0;
   
   /*calc bit flags in unsigned long flag holder*/
   FOOPOO = ACCUMM = 1; 
   FOOMOO = ACCUMM = 2*ACCUMM;
   FOOPPO = ACCUMM = 2*ACCUMM;
   FOOMPO = ACCUMM = 2*ACCUMM;
   FOOPMO = ACCUMM = 2*ACCUMM;
   FOOMMO = ACCUMM = 2*ACCUMM;
   FOOPPP = ACCUMM = 2*ACCUMM;
   FOOMPP = ACCUMM = 2*ACCUMM;
   FOOOMP = ACCUMM = 2*ACCUMM;
   FOOOPM = ACCUMM = 2*ACCUMM;
   FOOPMM = ACCUMM = 2*ACCUMM;
   FOOMMM = ACCUMM = 2*ACCUMM;
   TRYPOO = ACCUMM = 2*ACCUMM;
   TRYMOO = ACCUMM = 2*ACCUMM;
   TRYPPO = ACCUMM = 2*ACCUMM;
   TRYMPO = ACCUMM = 2*ACCUMM;
   TRYPMO = ACCUMM = 2*ACCUMM;
   TRYMMO = ACCUMM = 2*ACCUMM;
   TRYPPP = ACCUMM = 2*ACCUMM;
   TRYMPP = ACCUMM = 2*ACCUMM;
   TRYOMP = ACCUMM = 2*ACCUMM;
   TRYOPM = ACCUMM = 2*ACCUMM;
   TRYPMM = ACCUMM = 2*ACCUMM;
   TRYMMM = ACCUMM = 2*ACCUMM;
   SEALED = ACCUMM = 2*ACCUMM;
   
   ALLFOO =  FOOPOO|FOOMOO|FOOPPO|FOOMPO|FOOPMO|FOOMMO
            |FOOPPP|FOOMPP|FOOOMP|FOOOPM|FOOPMM|FOOMMM;
   ALLTRY =  TRYPOO|TRYMOO|TRYPPO|TRYMPO|TRYPMO|TRYMMO
            |TRYPPP|TRYMPP|TRYOMP|TRYOPM|TRYPMM|TRYMMM;

   xo = foogridstep*(float)(1.414/1.414); /* sqrt2        1.0   */
   zo = foogridstep*(float)(1.155/1.414); /* 2*sqrt3/3    0.8168*/
   yo = foogridstep*(float)(1.225/1.414); /* sqrt3/sqrt2  0.8663*/
   yp = foogridstep*(float)(0.408/1.414); /* yo - ym      0.2885*/
   ym = foogridstep*(float)(0.817/1.414); /* sqrt2/sqrt3  0.5778*/

   
}
/*___foogridconstruct()______________________________________________________*/

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

   if(FOOGRID == SIMPLECUBIC)
   {
      unsigned int  ALLFOO = FOOXM|FOOXP|FOOYM|FOOYP|FOOZM|FOOZP;
      unsigned int  ALLTRY = TRYXM|TRYXP|TRYYM|TRYYP|TRYZM|TRYZP;
   }
   else
   {
      foogridconstruct();
   }
   
   twofoo = 2*fooradius;
   /*dots added to the end of the drawnew group*/
   if(Lnewdrawline)
   {/*also done in, e.g., dodock3on3() */
      numberline = 0;
      Lnewdrawline = 0;
   }
   drawnewlistptr->on = 1; /*insure display enabled*/
   drawgrupptr->on = 1; /* enable display */

   /*foogrupptr is set to be the last foo group defined*/ 
   beginpointptr = foogrupptr->lastsgrpptr->lastlistptr->firstpointptr;
   endpointptr = foogrupptr->lastsgrpptr->lastlistptr->lastpointptr;
   thepointptr = beginpointptr;
   while(thepointptr != NULL)
   {/*target each foo*/
      /*ALLFOO: foo surrounded by foo, ALLTRY: foo neighbors all tried*/
      if(  ( (thepointptr->number & ALLFOO)!=ALLFOO)
         &&( (thepointptr->number & SEALED)!=SEALED)  /*030630*/
         &&( (thepointptr->number & ALLTRY)==ALLTRY) )
      {/*target surface foo that are completely blocked*/
         ctrx = thepointptr->fx;
         ctry = thepointptr->fy;
         ctrz = thepointptr->fz;
         /*clear flags on stored dotball*/
         for(j=0;j<dotsphereset.n;j++)  /*BEWARE: construction 0,n-1*/
         {
            dotsphereset.p[j].number = 1;
         }
         testpointptr = beginpointptr;
         while(testpointptr != NULL)
         {/*test against all other foos*/
            /*dirty coord check before serious distance check*/
            if(   (  testpointptr->fx>=ctrx-twofoo
                   &&testpointptr->fx<=ctrx+twofoo )
               && (  testpointptr->fy>=ctry-twofoo
                   &&testpointptr->fy<=ctry+twofoo )
               && (  testpointptr->fz>=ctrz-twofoo
                   &&testpointptr->fz<=ctrz+twofoo )
               )
            {/*close enough for gomint work*/
              if(testpointptr != thepointptr) /*NOT self*/
              {/*do centers distance check*/
                distsq = distancefsq(ctrx,ctry,ctrz
                     ,testpointptr->fx,testpointptr->fy,testpointptr->fz);
                if(distsq <= twofoo*twofoo)
                {/*check individual dots of positioned target dot ball*/
                  for(j=0;j<dotsphereset.n;j++) /*BEWARE: construction 0,n-1*/
                  {
                     if(dotsphereset.p[j].number == 1)
                     {
                        distsq = distancefsq(ctrx+(float)dotsphereset.p[j].x
                                            ,ctry+(float)dotsphereset.p[j].y
                                            ,ctrz+(float)dotsphereset.p[j].z
                         ,testpointptr->fx,testpointptr->fy,testpointptr->fz);
                        if(distsq <= fooradius*fooradius)
                        {/*inside the test foo, change flag to NO*/
                           dotsphereset.p[j].number = 0;
                        }
                     }
                  }
                }/*check individual dots of positioned target dot ball*/
              }/*do centers distance check*/
            }/*close enough for gomint work*/
            
            if(testpointptr == endpointptr) {testpointptr = NULL;}
            else {testpointptr = testpointptr->nextptr;}
         }/*test against all other foos*/
         for(j=0;j<dotsphereset.n;j++) /*BEWARE: construction 0,n-1*/
         {/*read out flagged dots on stored dotball==dotsphere from probe, sp*/
            if(dotsphereset.p[j].number == 1)
            {/*allocate dot points in drawnew group*/
               if((thispointptr=allocpointstructure(drawnewlistptr))==NULL)
               {/*allocation failed*/
                  sprintf(alertstr,"failed to alloc a dot ball surface point");
                  dosinglealert(3);
               }/*allocation failed*/
               else
               {/*memory room for another surface dot in drawnew group */
                  thispointptr->fx = ctrx+(float)dotsphereset.p[j].x;
                  thispointptr->fy = ctry+(float)dotsphereset.p[j].y;
                  thispointptr->fz = ctrz+(float)dotsphereset.p[j].z;
                  thispointptr->type = DOT;
                  numberline++;
                  Nsurfacedots++;
               }
            }
         }/*read out flagged dots on stored dotball==dotsphere from probe, sp*/
      }/*target surface foo*/
      if(thepointptr == endpointptr) {thepointptr = NULL;}
      else {thepointptr = thepointptr->nextptr;}
   }/*target each foo*/
#ifdef UNIX_X11
fprintf(stderr,"surface dots==  %d\n",Nsurfacedots);
#endif
   return(Nsurfacedots);
}
/*___foodotsurface()_________________________________________________________*/

/****foofillcomputation()*****************************************************/
int foofillcomputation()
{
   static unsigned long  TRIAL=0, FOOAL=0;
   int Lescape=0,Lcollision=0,Loldfoohere=0;
   int j=0,icolor=0,gridmax=0;
   float trialx=0,trialy=0,trialz=0,DX=0,DY=0,DZ=0;
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* thispointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   struct liststruct* thislistptr=NULL;

   if(FOOGRID == SIMPLECUBIC)
   {
      unsigned int  ALLFOO = FOOXM|FOOXP|FOOYM|FOOYP|FOOZM|FOOZP;
      unsigned int  ALLTRY = TRYXM|TRYXP|TRYYM|TRYYP|TRYZM|TRYZP;
      gridmax = 6;
   }
   else
   {
      foogridconstruct();
      gridmax = 12;
   }
   thislistptr = foogrupptr->lastsgrpptr->lastlistptr;
   thepointptr = foogrupptr->lastsgrpptr->lastlistptr->firstpointptr;
   endpointptr = foogrupptr->lastsgrpptr->lastlistptr->lastpointptr;
   while(thepointptr != NULL)
   {/*scan over all FOO's known at entering foofillcomputation()*/
      /*remember entering endpointptr, so new FOO's not scanned in this pass*/

      if(  (thepointptr->number & ALLTRY)!=ALLTRY
         &&(thepointptr->number & SEALED)!=SEALED )
      {/*yet an untried direction from this foo*/
       /*and not sealed against further expansion*/
         for(j=1; j<=gridmax; j++)
         {/*scan over -+xyz directions from this foo*/
            DX = 0; DY = 0; DZ = 0;
            if(FOOGRID == SIMPLECUBIC)
            {
               switch(j)
               {
                  case 1: TRIAL=TRYXM; FOOAL=FOOXM; DX = -foogridstep; break;
                  case 2: TRIAL=TRYXP; FOOAL=FOOXP; DX = +foogridstep; break;
                  case 3: TRIAL=TRYYM; FOOAL=FOOYM; DY = -foogridstep; break;
                  case 4: TRIAL=TRYYP; FOOAL=FOOYP; DY = +foogridstep; break;
                  case 5: TRIAL=TRYZM; FOOAL=FOOZM; DZ = -foogridstep; break;
                  case 6: TRIAL=TRYZP; FOOAL=FOOZP; DZ = +foogridstep; break;
               }
            }
            else if(FOOGRID == FACECENTERCLOSEST)
            {
       switch(j)
       {
        case  1: TRIAL=TRYPOO; FOOAL=FOOPOO; DX= +xo; break;
        case  2: TRIAL=TRYMOO; FOOAL=FOOMOO; DX= -xo; break;
        case  3: TRIAL=TRYPPO; FOOAL=FOOPPO; DX= +xo/2; DY= +yo; break;
        case  4: TRIAL=TRYMPO; FOOAL=FOOMPO; DX= -xo/2; DY= +yo; break;
        case  5: TRIAL=TRYPMO; FOOAL=FOOPMO; DX= +xo/2; DY= -yo; break;
        case  6: TRIAL=TRYMMO; FOOAL=FOOMMO; DX= -xo/2; DY= -yo; break;
        case  7: TRIAL=TRYPPP; FOOAL=FOOPPP; DX= +xo/2; DY= +yp; DZ= +zo; break;
        case  8: TRIAL=TRYMPP; FOOAL=FOOMPP; DX= -xo/2; DY= +yp; DZ= +zo; break;
        case  9: TRIAL=TRYOMP; FOOAL=FOOOMP; DX= 0;     DY= -ym; DZ= +zo; break;
        case 10: TRIAL=TRYOPM; FOOAL=FOOOPM; DX= 0;     DY= +ym; DZ= -zo; break;
        case 11: TRIAL=TRYPMM; FOOAL=FOOPMM; DX= +xo/2; DY= -yp; DZ= -zo; break;
        case 12: TRIAL=TRYMMM; FOOAL=FOOMMM; DX= -xo/2; DY= -yp; DZ= -zo; break;
       }
            }
            else if(FOOGRID == HEXAGONALCLOSEST)
            {
       switch(j)
       {
        case  1: TRIAL=TRYPOO; FOOAL=FOOPOO; DX= +xo; break;
        case  2: TRIAL=TRYMOO; FOOAL=FOOMOO; DX= -xo; break;
        case  3: TRIAL=TRYPPO; FOOAL=FOOPPO; DX= +xo/2; DY= +yo; break;
        case  4: TRIAL=TRYMPO; FOOAL=FOOMPO; DX= -xo/2; DY= +yo; break;
        case  5: TRIAL=TRYPMO; FOOAL=FOOPMO; DX= +xo/2; DY= -yo; break;
        case  6: TRIAL=TRYMMO; FOOAL=FOOMMO; DX= -xo/2; DY= -yo; break;
        case  7: TRIAL=TRYPPP; FOOAL=FOOPPP; DX= +xo/2; DY= +yp; DZ= +zo; break;
        case  8: TRIAL=TRYMPP; FOOAL=FOOMPP; DX= -xo/2; DY= +yp; DZ= +zo; break;
        case  9: TRIAL=TRYOMP; FOOAL=FOOOMP; DX= 0;     DY= -ym; DZ= +zo; break;
        case 10: TRIAL=TRYOPM; FOOAL=FOOOPM; DX= +xo/2; DY= +yp; DZ= -zo; break;
        case 11: TRIAL=TRYPMM; FOOAL=FOOPMM; DX= -xo/2; DY= +yp; DZ= -zo; break;
        case 12: TRIAL=TRYMMM; FOOAL=FOOMMM; DX= 0;     DY= -ym; DZ= -zo; break;
       }/*redefining FOOOPM,FOOPMM,FOOMMM in a simple manner*/
            }
            if( (thepointptr->number & TRIAL)!=TRIAL)
            {/*try this direction from this foo*/
               thepointptr->number = thepointptr->number | TRIAL;/*in any case*/
               trialx = DX + thepointptr->fx;
               trialy = DY + thepointptr->fy;
               trialz = DZ + thepointptr->fz;
               Loldfoohere = oldfoohere(trialx,trialy,trialz);
               if(Loldfoohere)
               {/*register this direction as having a foo*/
                  thepointptr->number = thepointptr->number | FOOAL;
               }
               else
               {/*try to place a new foo here*/
                  if(Lfoocavity) 
                  {
                     Lcollision = allhitfoocheck(&trialx,&trialy,&trialz,0);
                  }
                  else 
                  {
                     Lcollision = cagehitfoocheck(trialx,trialy,trialz);
                  }
                  if(Lcollision <= 0) /*slimemode can place first foo in void*/
                  {/*No collision: can place a new foo here*/
                     thepointptr->number = (thepointptr->number) | FOOAL;
                     if(Lwildfoo &&
                         (   trialx > fxmax || trialx < fxmin
                          || trialy > fymax || trialy < fymin
                          || trialz > fzmax || trialz < fzmin ) )
                     {/*escape from bounding box*/
                        Lescape = 1;
                     }
                     else
                     {/*place another foo*/
                       if((thispointptr=allocpointstructure(thislistptr))==NULL)
                       {/*allocation failed*/
                          sprintf(alertstr,"failed to alloc a foo point");
                          dosinglealert(3);
                       }/*allocation failed*/
                       else
                       {/*memory room for another foo */
                          thispointptr->fx = trialx;
                          thispointptr->fy = trialy;
                          thispointptr->fz = trialz;
                          thispointptr->number = 0; /*i.e. clear flags*/
                          thispointptr->radius = fooradius;
                          thispointptr->type=BALL;
                          Negamickfoocount++;
                          if(Lcollision == -1) /*020630*/
                          {/*slimemode and this foo is out in the void*/
                             thispointptr->number =thispointptr->number|SEALED;
                             icolor = numberofcolor("brown");
                             thispointptr->colorwidth
                                =icolor|((thispointptr->colorwidth)&~31);
                          }
                       }
                     }/*place another foo*/
                  }/*No collision: can place a new foo here*/
               }/*try to place a new foo here*/
               if(Lescape){break;}
            }/*try this direction from this foo*/
         }/*scan over -+xyz directions from this foo*/
      }/*yet an untried direction from this foo*/
      /*NOW have tried all directions from the foo... */
      if( (thepointptr->number & SEALED)!=SEALED) /*not a surface sealed foo*/
      {/*exhausted foo*/
         if((thepointptr->number & ALLFOO)==ALLFOO)
         {/*completely buried: use dot for more efficient drawing*/
            /*thepointptr->type = DOT;*/
            icolor = numberofcolor("yellow");
            thepointptr->colorwidth
               =icolor|((thepointptr->colorwidth)&~31);
         }
         else
         {/*surface barred foo*/
            icolor = numberofcolor("sky");
            thepointptr->colorwidth
               =icolor|((thepointptr->colorwidth)&~31);
         }
      }
      if(Lescape){break;}
      if(thepointptr == endpointptr) {thepointptr = NULL;}
      else {thepointptr = thepointptr->nextptr;}
   }/*scan over all FOO's known at entering foofillcomputation()*/
   if(Lescape)
   {
      releasefoos();
      Negamickfoocount = 1;
      Negamickpass++;
   }
   return(Lescape);
}
/*___foofillcomputation()____________________________________________________*/

/****foosealer()**************************************************************/
void foosealer(pointstruct* victimptr)
{
   int icolor=0;

   if( (victimptr->number & ALLTRY)!=ALLTRY )
   {/*pink growing surface...*/
      victimptr->number = victimptr->number | SEALED;
      /*seal off this foo, block growth in this direction*/
      icolor = numberofcolor("brown");
      victimptr->colorwidth = icolor|((victimptr->colorwidth)&~31);
      victimptr->type = victimptr->type | UNPICKABLE; /*avoid pick masking*/
   }
}
/*___foosealer()_____________________________________________________________*/

/****annealmobilelist()*******************************************************/
void annealmobilelist() /*egam 030215 for egamickdocking*/
{
   struct pointstruct* thepointptr=NULL;
   struct liststruct* mobilelistptr=NULL;
   struct liststruct* mobilelistpreviousptr=NULL;
   struct liststruct* mobilelistnextptr=NULL;
   struct liststruct* reflistptr=NULL;
   struct liststruct* reflistpreviousptr=NULL;
   struct liststruct* reflistnextptr=NULL;
 
 if(Legamickfoo && mobilehitfoocheck() )
 {/*mobile oject collided with foo, reject this annealling*/
    destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
    Lneedannealmobile = 0;
    Lfreeegamickmobile = 0;                                          
 }
 else
 {
   /*save hierarchy of reference group*/
   reflistptr = referencegrupptr->lastsgrpptr->lastlistptr;
   reflistpreviousptr = referencegrupptr->lastsgrpptr->lastlistptr->previousptr;
   reflistnextptr = referencegrupptr->lastsgrpptr->lastlistptr->nextptr;
   /*save hierarchy of mobile group*/
   mobilelistptr = mobilegrupptr->lastsgrpptr->lastlistptr;
   mobilelistpreviousptr = mobilegrupptr->lastsgrpptr->lastlistptr->previousptr;
   mobilelistnextptr = mobilegrupptr->lastsgrpptr->lastlistptr->nextptr;

   /*Mage hierarchy not only maintained as group/subgroup/list sets, but*/
   /* lists are linked previous and next for efficient linked-list scanning*/
   /*(and as historical artifact from time when everything was in one list)*/

   /*reassign and fit mobile list into reference group hierarchy*/
   /*mobile list is going to be placed just after ref list */
   /*so what was the mobile list will be the last list in reference group*/
   referencegrupptr->lastsgrpptr->lastlistptr = mobilelistptr;
   mobilelistptr->sgrpptr = referencegrupptr->lastsgrpptr;



   /*for all cases: in the linked-list of lists, this will be true: */
   reflistptr->nextptr = mobilelistptr;
   mobilelistptr->previousptr = reflistptr;
   mobilelistptr->color = reflistptr->color;
   
   /*in addition, there are 3 cases: */
   if(reflistpreviousptr == mobilelistptr)
   {/*mobile list immediately preceeded ref list, and will be swapped*/
      reflistptr->previousptr = mobilelistpreviousptr; 
      /*mobile list will now be just after ref list*/
      mobilelistptr->nextptr = reflistnextptr;
   }
   else if(reflistnextptr == mobilelistptr)
   {/*mobile list immediately follows ref list*/
      /*No reorganization needed of the linkage of the linked lists*/
      ;
   }
   else
   {/*general case, fits originally non-contiguous mobile list into ref group*/
      mobilelistptr->nextptr = reflistnextptr;
      reflistnextptr->previousptr = mobilelistptr; /*030320*/
      /*and the linked-list around the excised mobilelist must be repaired*/
      if(mobilelistpreviousptr != NULL)
      {
         mobilelistpreviousptr->nextptr = mobilelistnextptr;
      }
      else
      {/*mobilelist must have been first list*/
         firstlistptr = mobilelistnextptr;
      }
   }

   /*Now need to unhook this particular mobile list from the mobile group*/
   /*where it had been the last member of that group! */
   /*The tricky bit is whether there are any other mobile lists to track*/
   if(mobilegrupptr->lastsgrpptr->firstlistptr == mobilelistptr)
   {/*just the one  mobile group here, so subgroub is now empty*/
    /*since the mobilelist was the lastlist*/
      mobilegrupptr->lastsgrpptr->firstlistptr = NULL;
      mobilegrupptr->lastsgrpptr->lastlistptr = NULL;
      beginselectionpointptr = NULL;
      endselectionpointptr = NULL;
   }
   else
   {/*the list preceeding the last one must now become the last one*/
      mobilegrupptr->lastsgrpptr->lastlistptr = mobilelistpreviousptr;
      endselectionpointptr = mobilegrupptr->lastsgrpptr->lastlistptr->lastpointptr;
   }
   Lneedannealmobile = 0;

   thepointptr = mobilelistptr->firstpointptr; /*defined when entered above*/
   while(thepointptr != NULL)
   {/*expand the known limits of protected space*/
      if(thepointptr->fx < fxmin) fxmin = thepointptr->fx;
      if(thepointptr->fx > fxmax) fxmax = thepointptr->fx;
      if(thepointptr->fy < fymin) fymin = thepointptr->fy;
      if(thepointptr->fy > fymax) fymax = thepointptr->fy;
      if(thepointptr->fz < fzmin) fzmin = thepointptr->fz;
      if(thepointptr->fz > fzmax) fzmax = thepointptr->fz;
      if(thepointptr == mobilelistptr->lastpointptr){thepointptr = NULL;}
      else {thepointptr = thepointptr->nextptr;}
   }
 }
 if(Legamickfoo)
 {
    Negamickbarcount = Negamickbarcount + Negamickvalue;
    foofillcomputation();
 }
}
/*___annealmobilelist()______________________________________________________*/

/****getmobilelist()**********************************************************/
void getmobilelist(void)
{
   Lbeginselection = 1;
   Linternal = 1;
   Lappend = 1;
   Ldock3on3on = 1;
   if(Ltetramers)
   {
      Ninternal = 40 + Negamickshape; /*40 for list append to mobile group*/
      if(Negamickshape==2) {Negamickvalue = 1;}
      else {Negamickvalue = 4;}
   }
   else
   {
      Ninternal = 20 + Negamickshape; /*20 for list append to mobile group*/
      /*tetrahedron 1, cube 2, octahedron 3, widget 4, water 5 091127,...*/
      Negamickvalue = 1;
   }
   endselectionpointptr = NULL;
   entry();  /*MAGEFILE.c calls GetStuffFromFile()*/
   Linternal = 0;
   Lnewstuffactive = 1;
   Lfreeegamickmobile = 1;
   AdjustMenus();
}
/*___getmobilelist()_________________________________________________________*/

/****repeatmobiledocking()****************************************************/
void repeatmobiledocking(void)
{
   int j=0;
   struct liststruct* thislistptr;
   struct pointstruct* thispointptr;
   float fxyz[7][3];

   for(j=1; j<=6; j++)
   {

      if(j <=3) {thislistptr = referencegrupptr->lastsgrpptr->lastlistptr;}
      if(j > 3) {thislistptr = mobilegrupptr->lastsgrpptr->lastlistptr;}
      thispointptr = thislistptr->firstpointptr;
      while(thispointptr != NULL)
      {
         if(thispointptr->number == dockpointptr[j]->number)
         {
            fxyz[j][0] = thispointptr->fx;
            fxyz[j][1] = thispointptr->fy;
            fxyz[j][2] = thispointptr->fz;
         }
         if(thispointptr != thislistptr->lastpointptr) 
            {thispointptr = thispointptr->nextptr;}
         else
            {thispointptr = NULL;}
      }
   }
   if(j>=6)
   {
      threepointdock(fxyz);
      Ldock3on3on = 0; /*so  dock3on3 is single shot*/
      Lneedannealmobile = 1;
      Lfreeegamickmobile = 0;
      LegamickrepeatOK = 1; /*030225*/
   }
   else
   {
      LegamickrepeatOK = 0;
   }
}
/*___repeatmobiledocking()___________________________________________________*/

/****gridFOOBOXbounds()********************************************************/
int  gridFOOBOXbounds(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax) /*100818*/
{
   struct pointstruct* thepointptr=NULL;
   struct pointstruct* endpointptr=NULL;
   struct liststruct* thelistptr=NULL;
   struct liststruct* endlistptr=NULL;
   struct sgrpstruct* thesgrpptr=NULL;
   struct sgrpstruct* endsgrpptr=NULL;
   struct grupstruct* thegrupptr=NULL;
   struct grupstruct* endgrupptr=NULL;
   int    ireturn=0;
   float  proximal=0,distal=0;
   float  pointradius = 0; /*040402*/
   int    Loutofbox=0,Lwayoutofbox=0; /*030630 slimemode, added 111002*/
   int    Lfoobarrier = 0; /*090912, added 111002*/

   /*100818 calc coord box that just surrounds non-ignorable atoms*/
   /* rather than use coord min,max of all inputed points */
   xmax=-(float)1E37, xmin=(float)1E37; /*100818*/
   ymax=-(float)1E37, ymin=(float)1E37; /*100818*/
   zmax=-(float)1E37, zmin=(float)1E37; /*100818*/

   /*one pass for bounds: foo starts on grid origin at corner (min,min,min)*/
   thegrupptr = firstgrupptr;
   endgrupptr = lastgrupptr;
   while(thegrupptr != NULL)
   {/*search through all reference groups, subgroups, lists, points*/
      /*if(thegrupptr == referencegrupptr)*/
      if(  ( thegrupptr != foogrupptr )
         &&((thegrupptr->KKIND & FOOKKIND)!=FOOKKIND )
         &&((thegrupptr->KKIND & IGNORABLEKKIND)!=IGNORABLEKKIND )
         &&( thegrupptr != markergrupptr )
         &&( thegrupptr != measuregrupptr )
         &&( thegrupptr != drawgrupptr )
         &&( thegrupptr != mobilegrupptr )
        )
      {/*group to be checked against*/
         if((thegrupptr->KKIND & FOOBARRIERKKIND)!=FOOBARRIERKKIND )/*090912*/
         {Lfoobarrier = 0;} /*foo barrier like Lvoid: NOT surfaced 090912*/
         else
         {Lfoobarrier = 1;}
         thesgrpptr = thegrupptr->firstsgrpptr;
         endsgrpptr = thegrupptr->lastsgrpptr;
         while(thesgrpptr != NULL)
         {
            thelistptr = thesgrpptr->firstlistptr;
            endlistptr = thesgrpptr->lastlistptr;
            while(thelistptr != NULL)
            {
               thepointptr = thelistptr->firstpointptr;
               endpointptr = thelistptr->lastpointptr;
               while(thepointptr != NULL)
               {
                  if( (thepointptr->type & PRUNED) != PRUNED ) /*030405*/
                  {/*existing point*/
                     /*describe properties of the point*/
                     Loutofbox=0;
                     Lwayoutofbox=0; /*030630 slimemode*/
                     if((thepointptr->radius==0)&&(thepointptr->listptr!= NULL))
                     {
                        pointradius = (thepointptr->listptr)->radius;
                     }
                     else
                     {
                        pointradius = thepointptr->radius; /*040402*/
                     }
                     proximal = fooradius + pointradius;
                     distal = proximal + 2*foogridstep;
                     
                     /*get generous bounds of BOX for the FOO grid*/
                     /*NOT trying to establish an internal starting place*/
                     /*FOOinBOX will start at box corner and find molecule*/
                     if(((thepointptr->fx) + distal) > xmax)
                     {
                        xmax = ((thepointptr->fx) + distal);
                     }
                     else if(((thepointptr->fx) + distal) < xmin)
                     {
                        xmin = ((thepointptr->fx) - distal);
                     }
                     if(((thepointptr->fy) + distal) > ymax)
                     {
                        ymax = ((thepointptr->fy) + distal);
                     }
                     else if(((thepointptr->fy) + distal) < ymin)
                     {
                        ymin = ((thepointptr->fy) - distal);
                     }
                     if(((thepointptr->fz) + distal) > zmax)
                     {
                        zmax = ((thepointptr->fz) + distal);
                     }
                     else if(((thepointptr->fz) + distal) < zmin)
                     {
                        zmin = ((thepointptr->fz) - distal);
                     }
                  }/*existing point*/
                  if(thepointptr == endpointptr) {thepointptr = NULL;}
                  else {thepointptr = thepointptr->nextptr;}
               }
               if(thelistptr == endlistptr) {thelistptr = NULL;}
               else {thelistptr = thelistptr->nextptr;}
            }
            if(thesgrpptr == endsgrpptr) {thesgrpptr = NULL;}
            else {thesgrpptr = thesgrpptr->nextptr;}
         }
      }/*group to be checked against*/
      if(thegrupptr == endgrupptr) {thegrupptr = NULL;}
      else {thegrupptr = thegrupptr->nextptr;}
   }/*search through all reference groups, subgroups, lists, points*/

   /*test that bounds were extablished*/
   if(
          xmax> -(float)1E37 && xmin< (float)1E37
       && ymax> -(float)1E37 && ymin< (float)1E37
       && zmax> -(float)1E37 && zmin< (float)1E37)
   {/*100818*/
      ireturn = 1;
   }
   else {ireturn = 0;}  /*failed to find box bounds*/
   return(ireturn);
}
/*___gridFOOBOXbounds()_______________________________________________________*/

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

void dotSphere(pointSet *set, float radius, float density) 
{/*entry to construct a dotball*/
   int m;

   if (set) 
   {
      m = estNumDots(radius, density);

      set->n = 0;
      set->p = (point3dn *)malloc(m*sizeof(point3dn));
      if (set->p) 
      {
         set->n = makeDots(radius, set->p, m);
      }
   }
}

void freeDotSphere(pointSet *set)
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

int makeDots(float radius, point3dn points[], int maxpnts) 
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

