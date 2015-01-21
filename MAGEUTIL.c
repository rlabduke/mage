                       /*MAGEUTIL.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"
#include "MAGEMENU.h"
#include "MAGEANGL.h"
#include "MAGETABL.h"
#ifdef PCWINDOWS
#include "MPCPIPE.h"
#endif

/*prototypes of routines only referred to within this source file*/
int     normalizevector_vector(double[3],double, double, double);
int     normalizevector_points(double[3],int,int,int,int,int,int);

/*****floatfromstr()*********************************************************/
float    floatfromstr(char ins[256])
{
        int            Lstart,n,m,OK;
        char         s[256];
        float        freturn;

   /*970703 now only handles C-strings*/

        freturn = 0.0;
        Lstart = 1;
        n = 0;
        m = 0;
        OK = 1;
        if(ins[0] != '\0')  
        {/* not zero length C string */
            while(OK)
            {
                /* start at position 0 of c str */
                if(ins[n] == ' ' && Lstart)  ;
                else if(ins[n] == '0' || ins[n] == '1' || ins[n] == '2' ||
                        ins[n] == '3' || ins[n] == '4' || ins[n] == '5' ||
                        ins[n] == '6' || ins[n] == '7' || ins[n] == '8' ||
                        ins[n] == '9' || ins[n] == '.' || ins[n] == '-'   ) 
                {
                    s[m] = ins[n];
                    m++;
                    Lstart = 0;
                }
                else
                {
                    s[m] = '\0'; 
                    OK = 0;
                }
                n++;
            }

/*c*/       sscanf(s,"%f",&freturn);
        }
        return(freturn);
}
/*___floatfromstr()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****intfromstr()************************************************************/
int   intfromstr(char  ins[256])
{
        int            Lstart,n,m,OK;
        char         s[256];
        int            ireturn;
   
   /*970703 now only handles C-strings*/
   
        ireturn = 0;
        Lstart = 1;
        n = 0;
        m = 0;
        OK = 1;
        if(ins[0] != '\0') 
        {/* not zero length C string */
            while(OK)
            {
                /* start at position 0 of c str */
                if(ins[n] == ' ' && Lstart)  ;
                else if(ins[n] == '0' || ins[n] == '1' ||
                        ins[n] == '2' || ins[n] == '3' || 
                        ins[n] == '4' || ins[n] == '5' ||
                        ins[n] == '6' || ins[n] == '7' || 
                        ins[n] == '8' || ins[n] == '9'    ) 
                {
                    s[m] = ins[n];
                    m++;
                    Lstart = 0;
                }
                else
                {
                    s[m] = '\0'; 
                    OK = 0;
                }
                n++; 
            }
/*c*/       sscanf(s,"%d",&ireturn);
        }
        return(ireturn);
}
/*___intfromstr()___________________________________________________________*/

/*****doundoprune()**********************************************************/
void    doundoprune()
{
#define ANTIPRUNE      (32767-PRUNED)
#define ANTIMOVETO_P   (32767-MOVETO_P)

    if(nundop > 0)
    {
        thispointptr = prunepoint1ptr[nundop];
        while(thispointptr != NULL)
        {
            thispointptr->type = (thispointptr->type) & ANTIPRUNE;

            if(thispointptr == prunepoint2ptr[nundop])thispointptr=NULL;
            else thispointptr = thispointptr->nextptr;
        }
        
        if(prunepointflag[nundop] == 1) 
        {/*following vector had been changed from draw to move*/
            ((prunepoint2ptr[nundop])->nextptr)->type 
             = ((((prunepoint2ptr[nundop])->nextptr)->type) & ANTIMOVETO_P);
        }
        nundop = nundop - 1;
    }
    else
    {
        sprintf(alertstr,"reached limit of undo for pruning & punching");
        dosinglealert(0);
    }
}
/*___doundoprune()__________________________________________________________*/

/*****mysearchpointID()******************************************************/
int    mysearchpointID(int justone) /* <- MACDLOG/SearchDialog() */
{/*search graphics objects that are visible in the graphics window*/
  int   kmax=0,ifoundpart=0,ifoundone=0,ireturn=0;
#ifdef OLDCODE
  int   k=0,kk=0,j = 0; 
#endif

  ireturn = 0;
  ifoundpart = 0;
  if(isearch1>0) uctolcstr(search1str); /*ignore case*/
  if(Lsearchbegin) searchgrupptr = firstgrupptr;
  while(searchgrupptr != NULL)
  {/*begin-loop-over-groups*/
    if(  ((searchgrupptr->type & TABLEFLAG) == 0)
        &&(   (searchgrupptr->on > 0 && !Lcompareon)
                                     /*ON-OFF flag*/  /*ANIMATE flag*/
           || (Lcompareon && searchgrupptr->on>0)  
               /*first pass put left everything that is ON */
           || (Lcompareon && searchgrupptr->on>0  
                          && searchgrupptr->animate==0)
               /*second pass put right, all non-animate groups that are ON */
           || (Lcompareon && searchgrupptr->on==0 
                          && searchgrupptr == nextanimategrupptr) ) )
               /* and the OFF animate group that is flagged as being next */
    {/*begin-visible-group*/
      if(Lsearchbegin) searchsgrpptr = searchgrupptr->firstsgrpptr; 
      while(searchsgrpptr != NULL)
      {/*begin-loop-over-subgroups*/
        if(searchsgrpptr->on > 0)
        {/*begin-visible-set*/
          if(Lsearchbegin) searchlistptr = searchsgrpptr->firstlistptr;
          while(searchlistptr != NULL)
          {/*begin-loop-over-lists*/              
            if(searchlistptr->on > 0)
            {/*begin-visible-list*/
              if(Lsearchbegin) searchpointptr = searchlistptr->firstpointptr;
              else if(searchpointptr != NULL)
              {
                 if(searchpointptr != searchlistptr->lastpointptr)
                 {
                    searchpointptr = searchpointptr->nextptr;/*Next point*/
                 }
                 else 
                 {
                    searchpointptr = NULL; /*force going to next list*/
                 }
              }
                
              Lsearchbegin = 1;
              /*at previous point: revert to looking at first member of sets*/

              while(searchpointptr != NULL)
              {/*begin-loop-over-points*/
                if( !((searchpointptr->type) & PRUNED)) /*not been pruned*/
                {/*begin-valid-point*/   
                  /*copy char str from storage */
                  getptIDstring(trystr, searchpointptr); /*trystr[256]*/
                  for(kmax=0;kmax<256;kmax++) if(trystr[kmax]=='\0') break;
                  /*stupid way to find size of ptIDstr*/
                  if(kmax > 0) 
                  {/*trial str not null*/ /*000410*/
                    uctolcstr(trystr); /*ignore case */ 
                    ifoundpart = 0;
                    ifoundone = 0;
                    if(isearch1>0)
                    {/*compare with first search string*/ 
                      ifoundpart = matchstrings(trystr,search1str); /*MAGEUTIL*/
                    }/*compare with first search string*/ 
                    if(ifoundpart && isearch2>0)
                    {/*compare with second search string*/
                      ifoundpart = matchstrings(trystr,search2str); /*MAGEUTIL*/
                    }/*compare with second search string*/
                    if(ifoundpart)
                    {
                       ifoundone = 1;
                       ifoundpart = 0;
                    }
                  }/*trial str not null*/ /*000410*/

                }/*end-valid-point*/  
                if(ifoundone)
                {/*set a special mark at this point*/
                  if( (searchpointptr->type & UNPICKABLE) != UNPICKABLE)
                  {/*only mark pickable points*/
                    /*historically, a mark based on a selected table cell*/
                    if(Ltablepresent && Ltablesearchgraphics) 
                    {/*table pick initiated search puts marker on graphics point*/
                       addtablemarker(searchpointptr);
                    }
                    if(justone)
                    {
                       break;
                    }
                    else
                    {
                       ifoundone = 0;
                    }
                  }/*only mark pickable points*/
                }
                if(searchpointptr == searchlistptr->lastpointptr) 
                  searchpointptr = NULL;
                else searchpointptr = searchpointptr->nextptr;
              }/*end-loop-over-points*/                  
            }/*end-visible-lists*/
            if(justone && ifoundone) break;
            if(searchlistptr == searchsgrpptr->lastlistptr) 
                   searchlistptr = NULL;
            else searchlistptr = searchlistptr->nextptr;
          }/*end-loop-over-lists*/
          if(justone && ifoundone) break;              
        }/*end-visible-sets*/





        if(justone && ifoundone) break;
        if(searchsgrpptr == searchgrupptr->lastsgrpptr) searchsgrpptr = NULL;
        else searchsgrpptr = searchsgrpptr->nextptr;
      }/*end-loop-over-subgroups*/
      if(justone && ifoundone) break;
    }/*end-visible-group*/
    if(justone && ifoundone) break;
    searchgrupptr = searchgrupptr->nextptr; /*==NULL if last group*/
  }/*end-loop-over-groups*/
  if(justone && ifoundone)
  {/*setup regular pick simulation on this single found point*/
        Lsearchatend = 0;
        Lpick = 1; /* flag for successful pick */
        ipick = 0; /*so will not try to find a pickpoint by cursor x,y*/
        Lprepick = 1; /*so will know to apply pick process to pickvec */
        pickedpointptr = searchpointptr;
        pickedgrupptr = searchgrupptr;
        pickedsgrpptr = searchsgrpptr; 
        pickedlistptr = searchlistptr; 
        temppickedpointptr = searchpointptr; /*030210*/
        temppickedgrupptr = searchgrupptr;
        temppickedsgrpptr = searchsgrpptr; 
        temppickedlistptr = searchlistptr; 
  }
  if(ifoundone)
  {/*signel that at least one point was found*/
        ireturn = 1;
  }
  return(ireturn);
}
/*____mysearchpointID()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****matchstrings()***********************************************************/
int matchstrings(char trystr[256],char searchstr[256])/*051004 FIND whole word*/
{
   int ifound = 0, k=0, kk=0, tmax=0, smax=0, j=0;
   int first = 0; /*051004 for Lfindwholeword match at beginning of string*/
   for(tmax=0;tmax<256;tmax++)
   {
      if(trystr[tmax]=='\0') break;
   }      
   for(smax=0;smax<256;smax++)
   {
      if(searchstr[smax]=='\0') break;
   }      
   for(k=0; k<tmax; k++)/*check each trial str char with first search char*/
   {/*loop over one trial str */
      /*get back to here only if kth trial str char NOT = 1st search char*/
      if(k==0 && Lfindwholeword && trystr[k]!= ' ') 
      {
         first = 1;  /*searchstr[0] always==' '*/
      } 
      else {first = 0;} /*index into search string*/
      if(trystr[k] == searchstr[first])   /*051004 whole word vs exact string*/
      {/*at least first char of search str matches one in trial str*/
         /*so get here only if at least the 1st char matches in trial str*/
         for(kk = first; kk < smax ;kk++) /*later positions honor lead space*/
         {/*loop over search str*/
            /*repeat [0] to deal with single character*/
            /*does all isearch  chars if all good*/
            if(k+kk>=(tmax) && ifound)/* run off end of trystr*/
            {/*search runs off end of trial string*/
              /*end of trial str valid to match with ' ' of search str*/
              if(searchstr[kk]==' ')/* blank==end of trial str*/
              {/*so far OK and can match search str blank with trial str EOL*/
                if(!Lfindwholeword) /*051004*/
                {  /* 051004 now entire exact string must match*/
                   ifound = 0;
                   break;  /*out of loop over end of search str*/
                }
                else
                {
                   for(j=kk; j<smax; j++)
                   {/*are any remaining characters of search str NOT blank?*/
                      if(searchstr[j]!=' ')
                      {/*loop over end of search str*/
                         ifound = 0;
                         break; /*out of loop over end of search str*/
                      }/*loop over end of search str*/
                   }/*are any remaining characters of search str NOT blank*/
                }
                /*ifound had been 1, but now maybe set to 0*/
                break;/*loop over search str*/
              }/*so far OK and can match search str blank with trial str EOL*/
              else
              {/*so far OK but search str still has unmatched char(s)*/
                 ifound = 0;/*search str still has unmatched char(s)*/
                 break;/*loop over search str*/
              }/*so far OK but search str still has unmatched char(s)*/
            }/*search runs off end of trial string*/
            else if(trystr[k+kk] == searchstr[kk])
            {/*found a match, only condition that continues loop*/
               ifound = 1;/* char==char*/
            }
            else
            {/*any mismatch means failure and forces break*/
               ifound = 0;/*any char!=char breaks out*/
               break;/*loop over search str*/
            }
         }/*loop over search str*/
         if(ifound)
         {/*matches search str starting at kth trial char*/
            break;/*loop over one trial str */
         }
      }/*at least first char of search str matches one in trial str*/
      if(ifound)  /*is this redundant????*/
      {/*matches search str starting at kth trial char*/
         break;/*loop over one trial str */
      }
   }/*loop over one trial str */
   return(ifound);
}
/*___matchstrings()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoSearchAgain()*********************************************************/
void DoSearchAgain()  /*971204*/
{
   if(Lsearchatend == 0)
   {
      Lsearchbegin = 0;  /*Not from beginning, but search AGAIN*/
      if(mysearchpointID(1)) /*MAGEUTIL.c for justone hit*/
          redrawvec();
   }
}
/*___DoSearchAgain()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****uctolcstr()***********************************************************/
void  uctolcstr(char texts[256])
{
  int  j,k;
static  char  uc[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static  char  lc[] = " abcdefghijklmnopqrstuvwxyz";
  
  /*converts upper case alphabetic char to lower case */
  /*test everything for alphabetic character*/
  for(k=0 ;texts[k]!='\0'&&k<256 ; k++) 
  {
    for(j=1 ; j<= 26 ; j++)
    {
      if(texts[k] == uc[j])
      {
        texts[k] = lc[j];
        break;
      }
    }
  }  
  return;
}
/*__uctolcstr()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****checksurvival()*********************************************************/
int    checksurvival()  
{    
    int    isurvived,jsurvived,ksurvived,idied;

    idied = 0;
    thisgrupptr = firstgrupptr;
    while( thisgrupptr != NULL)
    {/*check all groups*/
      ksurvived = 0;/*presume this group is dead*/
      if(thisgrupptr->on > -50) /*survived pickshow deletion*/
      {/*group not deleted*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*check all subgroups*/
          jsurvived = 0;/*presume this subgroup is dead*/
          if(thissgrpptr->on > -50) /*survived pickshow deletion*/
          {/*subgroup not deleted*/
              thislistptr = thissgrpptr->firstlistptr;
              while(thislistptr != NULL)
              {/*check all lists*/
                  isurvived = 0;/*presume this list dead*/
                  if(thislistptr->on > -50) /*survived pickshow deletion*/
                  {/*list not deleted*/
                      thispointptr = thislistptr->firstpointptr;
                      while(thispointptr != NULL)
                      {
                         if( !((thispointptr->type) & PRUNED)) /*not been pruned*/
                             isurvived = 1;/*survived pruning*/
                         
                         if( thispointptr == thislistptr->lastpointptr) 
                             thispointptr = NULL;
                         else thispointptr = thispointptr->nextptr;
                      }
                      if(isurvived) 
                      {
                          jsurvived = 1;/*some list alive in this subgroup*/
                      }
                      else /*(!isurvived)*/ 
                      {/*this list died*/   /*very off for output*/
                          thislistptr->on = thislistptr->on-100;
                          thislistptr->STATUS = thislistptr->STATUS | NOBUTTON;
                          idied = 1;                  
                      }/*this list died*/
                  }/*list not deleted*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
              else thislistptr = thislistptr->nextptr;
              }/*check all lists*/
              if(jsurvived) 
              {
                 ksurvived = 1; /*some subgroup alive in this group*/
              }
              else  /*!jsurvived*/ 
              {/*this subgroup died*/
                 thissgrpptr->on = thissgrpptr->on-100;/*very off for output*/
                 thissgrpptr->STATUS = thissgrpptr->STATUS | DOMINANT | NOBUTTON;
                    /*nobutton && dominant: no buttons below*/
                 idied = 1;
              }/*this subgroup died*/
          }/*subgroup not deleted*/ 
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*check all subgroups*/
        if(!ksurvived) /*no alive lists or subgroups*/
        {/*this group died*/
          thisgrupptr->on = thisgrupptr->on-100;/*very off for output*/
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT | NOBUTTON;
             /*nobutton && dominant: no buttons below*/
          thisgrupptr->animate = 0;/*remove from any animation sequence*/
          idied = 1;
        }/*this group died*/
      }/*group not deleted*/
      thisgrupptr = thisgrupptr->nextptr;
    }/*check all groups*/
    return(idied);
}
/*___checksurvival()________________________________________________________*/

/****doundopickcenter()******************************************************/
void doundopickcenter()    /*960809*/
{
    float fcentertemp[3];
    
    fcentertemp[0] = fxcenternew; /*current center coord*/
    fcentertemp[1] = fycenternew;
    fcentertemp[2] = fzcenternew;
    
    fxcenternew = fxcenterold[0]; /*set new from previous center*/
    fycenternew = fycenterold[0];
    fzcenternew = fzcenterold[0];
    
    fxcenterold[0] = fcentertemp[0]; /*setup for undo-undo*/
    fycenterold[0] = fcentertemp[1];
    fzcenterold[0] = fcentertemp[2];
               
    rescalekinemage(); /*MAGEINPT.C*/
    redrawvec(); /*____DRAW.c*/
}
/*___doundopickcenter()_____________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****consoleshowbits()*******************************************************/
void consoleshowbits(unsigned xyzflag)  /*980804*/
{
  long lowbitflag = 0;
  long highbitflag = 0;
  if(xyzflag &     1) lowbitflag = lowbitflag +               1;
  if(xyzflag &     2) lowbitflag = lowbitflag +              10;
  if(xyzflag &     4) lowbitflag = lowbitflag +             100;
  if(xyzflag &     8) lowbitflag = lowbitflag +            1000;
  if(xyzflag &    16) lowbitflag = lowbitflag +           10000;
  if(xyzflag &    32) lowbitflag = lowbitflag +          100000;
  if(xyzflag &    64) lowbitflag = lowbitflag +         1000000;
  if(xyzflag &   128) lowbitflag = lowbitflag +        10000000;
  if(xyzflag &   256) highbitflag = highbitflag +       1;
  if(xyzflag &   512) highbitflag = highbitflag +      10;
  if(xyzflag &  1024) highbitflag = highbitflag +     100;
  if(xyzflag &  2048) highbitflag = highbitflag +    1000;
  if(xyzflag &  4096) highbitflag = highbitflag +   10000;
  if(xyzflag &  8192) highbitflag = highbitflag +  100000;
  if(xyzflag & 16384) highbitflag = highbitflag + 1000000;
  if(xyzflag & 32768) highbitflag = highbitflag +10000000;
  fprintf(stderr,"bits: %08ld,%08ld",highbitflag,lowbitflag);
}
/*___consoleshowbits()________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****checksideclip()******************************************/
void    checksideclip(int xyzflag)
{
  int    L1,L2,idelx,ifudgx,idely,ifudgy,idel,ifudg;

  /*don't do this for triangles, since clipping algorithm does not result*/
  /*in complete triangles, and writing outside of drawing area is tolerated*/
  if(    ((xyzflag & TRIANGLE) == TRIANGLE)
     &&  !(Lstereo || Lcompareon)                  ) 
  {/*triangle component in MONO: accept all, let OS clip edges*/
    LOK = 1;
    return;
  }/*triangle component in MONO*/
  else
  {/*everything except triangle and ribbonlists in MONO*/

    LOK = 0;
    L1 = 0;
    L2 = 0;       

    if(jx1 >= xmin && jx1 <= xmax && jy1 >= ymin && jy1 <= ymax) L1 = 1;
    if(jx2 >= xmin && jx2 <= xmax && jy2 >= ymin && jy2 <= ymax) L2 = 1;
    
    if( L1 && L2) {LOK = 1; return;} /* all inside, accept */
    
    if(jx1 < xmin && jx2 < xmin) return;     /* all outside, reject */
    if(jy1 < ymin && jy2 < ymin) return;     /* return: LOK remains == 0 */
    if(jx1 > xmax && jx2 > xmax) return;
    if(jy1 > ymax && jy2 > ymax) return;
    
    /* Easy ones done, now have to do some work */
    /* cases: y1 < ymin,  y1 > ymax */
    /*        y2 < ymin,  y2 > ymax */
    /*        x1 < xmin,  x1 > xmax  */
    /*        x2 < xmin,  x2 > xmax  */
    /* first bring y in to y bounds , always work on adjusted coordinates */
    if(jy2 > ymax) /* ymax fault */
    {
            idel = ymax - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;
            jy2 = ymax;
            jx2 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    else if(jy2 < ymin) /* ymin fault */
    {
            idel = ymin - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;

            jy2 = ymin;
            jx2 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    if(jy1 > ymax) /* ymax fault */
    {
            idel = ymax - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;                    
            jy1 = ymax;
            jx1 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    else if(jy1 < ymin) /* ymin fault */
    {
            idel = ymin - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;    
            jy1 = ymin;
            jx1 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    /* y coord adjusted on to y bounds, now adjust x coord */
    if(jx2 > xmax) /* xmax fault */
    {
      idel = xmax - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                    
      jx2 = xmax;
      jy2 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    else if(jx2 < xmin) /* xmin fault */
    {
      idel = xmin - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                        
      jx2 = xmin;
      jy2 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    if(jx1 > xmax) /* xmax fault */
    {
      idel = xmax - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                            
      jx1 = xmax;
      jy1 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    else if(jx1 < xmin) /* xmin fault */
    {
      idel = xmin - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                            
      jx1 = xmin;
      jy1 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    LOK = 1;
  }/*everything except triangle and ribbonlists*/
  return;
}
/*___checksideclip()_________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****checkmiddleclip()********************************************************/
void    checkmiddleclip(int xyzflag, int limitz) /*051214 Lclipmiddle*/
{
  int  izmin,izmax;

  /*don't do this for triangles, since clipping algorithm does not result*/
  /*in complete triangles, and writing outside of drawing area is tolerated*/
  if(    ((xyzflag & TRIANGLE) == TRIANGLE)
     &&  !(Lstereo || Lcompareon)                  ) 
  {/*triangle component in MONO: accept all, let OS clip edges*/
    LOK = 1;
    return;
  }/*triangle component in MONO*/
  else
  {/*everything except triangle and ribbonlists in MONO*/

     LOK = 0; /*set global to NOT OK */

     if(Lzclipon && (thislistptr->type & NOZCLIPFLAG) == 0)
     {
        if(limitz)
        {/*list has invoked extra limited z clipping planes*/
          izmin = izctr - izlimit;
          izmax = izctr + izlimit;
        }
        else
        {
          izmin = izctr - izclip;
          izmax = izctr + izclip;
        }

        if(Lperspec)
        {
           if(izmax > izctr + ieyeposition){izmax = izctr + ieyeposition;}
        }
        /*clip middle if both ends out of clipping box*/
             /* i.e. not show lines just passing through visible region*/
           if(  (   jx1 < xmin 
                 || jx1 > xmax 
                 || jy1 < ymin 
                 || jy1 > ymax 
                 || jz1 < izmin 
                 || jz1 > izmax )
              &&(   jx2 < xmin 
                 || jx2 > xmax 
                 || jy2 < ymin 
                 || jy2 > ymax
                 || jz2 < izmin 
                 || jz2 > izmax ))
           {return; /*LOK==0*/}
     }
     else
     {  
           if(  (   jx1 < xmin 
                 || jx1 > xmax 
                 || jy1 < ymin 
                 || jy1 > ymax )
              &&(   jx2 < xmin 
                 || jx2 > xmax 
                 || jy2 < ymin 
                 || jy2 > ymax ))
           {return; /*LOK==0*/}
     }
     LOK = 1;
  }/*everything except triangle and ribbonlists in MONO*/
}
/*___checkmeiddleclip()______________________________________________________*/

/****checkzclip()************************************************************/
void checkzclip(int ix1,int iy1,int iz1,int ix2,int iy2,int iz2,int xyzflag,int limitz) 
            /* passed altered coord values, returns in global j__ coords*/
{
  int            L1,L2;
  int            izmin,izmax,idelx,idely,ifudgx,ifudgy;
  int            idelmin1,idelmax1,ifudgmin1,ifudgmax1;
  int            idelmin2,idelmax2,ifudgmin2,ifudgmax2;
    
  if(limitz)
  {/*list has invoked extra limited z clipping planes*/
    izmin = izctr - izlimit;
    izmax = izctr + izlimit;
  }
  else
  {
    izmin = izctr - izclip;
    izmax = izctr + izclip;
  }

  if(Lperspec)
  {
     if(izmax > izctr + ieyeposition){izmax = izctr + ieyeposition;}
  }

  L1 = 0;
  L2 = 0;
  if(iz1 >= (izmin) && iz1 <= (izmax) ) L1 = 1;
  if(iz2 >= (izmin) && iz2 <= (izmax) ) L2 = 1;
    
  if(L1 && L2) {LOK = 1; return;}
    
  if(iz1 < izmin && iz2 < izmin) {LOK = 0; return;}
  if(iz1 > izmax && iz2 > izmax) {LOK = 0; return;}
    
  /* Easy ones done, now have to do some work */
  /*don't do this for triangles, since clipping algorithm does not result*/
  /*in complete triangles, and writing outside of drawing area is tolerated*/
  if(!(xyzflag & VECTOR && xyzflag & VARIANT1)  ) 
  {/*everything except triangle and ribbonlists*/
    /*so neither fully accepted, nor fully rejected, so got to do some work:*/
    idelx = ix2 - ix1;ifudgx = 1;
    while(idelx > 181 || idelx < -181){idelx = idelx/2; ifudgx = ifudgx*2;}
    idely = iy2 - iy1;ifudgy = 1;
    while(idely > 181 || idely < -181){idely = idely/2; ifudgy = ifudgy*2;}
    idelmin1 = izmin-iz1; ifudgmin1 = 1;
    while(idelmin1 > 181 || idelmin1 < -181)
    {idelmin1 = idelmin1/2; ifudgmin1 = ifudgmin1*2;}
    idelmax1 = izmax-iz1; ifudgmax1 = 1;
    while(idelmax1 > 181 || idelmax1 < -181)
    {idelmax1 = idelmax1/2; ifudgmax1 = ifudgmax1*2;}
    idelmin2 = izmin-iz2; ifudgmin2 = 1;
    while(idelmin2 > 181 || idelmin2 < -181)
    {idelmin2 = idelmin2/2; ifudgmin2 = ifudgmin2*2;}
    idelmax2 = izmax-iz2; ifudgmax2 = 1;
    while(idelmax2 > 181 || idelmax2 < -181)
    {idelmax2 = idelmax2/2; ifudgmax2 = ifudgmax2*2;}
    
    if(L1) /* 2 fault */
    {
      if(iz2 < izmin)  /* back plane */
      {
        jx2 = ix1 + ((idelx)*(idelmin1)/(iz2-iz1))*(ifudgx*ifudgmin1);
        jy2 = iy1 + ((idely)*(idelmin1)/(iz2-iz1))*(ifudgy*ifudgmin1);
        LOK = 1;
        return;
      }
      else /* front plane */
      {
        jx2 = ix1 + ((idelx)*(idelmax1)/(iz2-iz1))*(ifudgx*ifudgmax1);
        jy2 = iy1 + ((idely)*(idelmax1)/(iz2-iz1))*(ifudgy*ifudgmax1);
        LOK = 1;
        return;
      }
    }
    else if(L2) /* 1 fault */
    {
      if(iz1 < izmin)  /* back plane */
      {
        jx1 = ix2 + (( idelx)*(idelmin2)/(iz2-iz1))*(ifudgx*ifudgmin2);
        jy1 = iy2 + (( idely)*(idelmin2)/(iz2-iz1))*(ifudgy*ifudgmin2);
        LOK = 1;
        return;
      }
      else /* front plane */
       {
        jx1 = ix2 + (( idelx)*(idelmax2)/(iz2-iz1))*(ifudgx*ifudgmax2);
        jy1 = iy2 + (( idely)*(idelmax2)/(iz2-iz1))*(ifudgy*ifudgmax2);
        LOK = 1;
        return;
      }
    }
    else /* vector spans both planes, just leave the whole damn thing */
    {
      if(iz1 < izmin)  /* 1 at back plane, 2 at front plane */
      {
        jx1 = ix2 + ( ( idelx)*(idelmin2)/(iz2-iz1) )*(ifudgx*ifudgmin2);
        jy1 = iy2 + ( ( idely)*(idelmin2)/(iz2-iz1) )*(ifudgy*ifudgmin2);
        jx2 = ix1 + ( ( idelx)*(idelmax1)/(iz2-iz1) )*(ifudgx*ifudgmax1);
        jy2 = iy1 + ( ( idely)*(idelmax1)/(iz2-iz1) )*(ifudgy*ifudgmax1);

        LOK = 1;
        return;
      }
      else /* 1 at front plane, 2 at back plane */
      {
        jx1 = ix2 + ( ( idelx)*(idelmax2)/(iz2-iz1) )*(ifudgx*ifudgmax2);
        jy1 = iy2 + ( ( idely)*(idelmax2)/(iz2-iz1) )*(ifudgy*ifudgmax2);
        jx2 = ix1 + ( ( idelx)*(idelmin1)/(iz2-iz1) )*(ifudgx*ifudgmin1);
        jy2 = iy1 + ( ( idely)*(idelmin1)/(iz2-iz1) )*(ifudgy*ifudgmin1);

        LOK = 1;
        return;
      }
    }
  }/*everything except triangle and ribbonlists*/
}
/*___checkzclip()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/  
/****getrot()****************************************************************/
void    getrot(int idelx,int idely,int mode) /*mode x,y,z,stereo 1234 031127*/
{    /*called from MACDRAW, MUXMDRAW, MPCMAIN:Mousemove */
    double b11,b12,b13,b21,b22,b23,b31,b32,b33;
    double sinang,cosang,fvalue;

/* 157.07963 = 100*(90*PI/180)==100*90*3.14159/180==50*PI== angle*.5555*PI */

   /*if((idelx == 157 && idely == 157)||(idelx == -157 && idely == -157))*/
     if(mode == 3 || mode == -3)
         irotxy = 0; /*rotation by 90 in plane of screen*/
   /*else if(idelx == 157 || idelx == -157)*/ /*virtual horizontal pull*/
     else if(mode == 2 || mode == -2) /*virtual horizontal pull*/
         irotxy = 2; /*rotation by 90 around y, screen vertical*/
   /*else if(idely == 157 || idely == -157)*/ /*virtual vertical pull*/
     else if(mode == 1 || mode == -1) /*virtual vertical pull*/
         irotxy = 3; /*rotation by 90 around x, screen horizontal*/
     else if(mode == 4 || mode == -4) /*rotation by stereoangle*/
         irotxy = 2; /*rotation around y, the screen vertical*/

     /*Lshiftkey overloaded, means both keep horizontal/vetical irotxy 2/3*/
     /* and change coordinates of mobile group(s) in dockingscope */
     /*Ldockingscope takes precedence, so docking not x or y limited*/
     /*if( (Lflat || Lscroll)  && irotxy >= 1  WHY irotxy LIMITATION HERE??? */
     if( (Lflat || Lscroll) && mode==0)
        /*&& idelx !=  157 && idely !=  157 */
        /*&& idelx != -157 && idely != -157)*/
     {/*Lscroll set by f key*/

        if(Ldockingscope && Lshiftkey && !Legamickdocking)
        {/*mouse movements use same sign for dodocktran*/
           if(irotxy == 0) /*mouse in upper part of screen*/ /*030205*/
           {/*in---out of screen translation controlled by x-motion*/
              bondrotbyoption(9,(float)idelx);
           }
           else
           {
              if(idelx != 0) 
              {/*dampen mouse movement --> translation amount 001029*/
               /*011201 universal finestep can be set by dialog*/
               /*so arbitrary divide by 2 is no longer needed here*/
                 bondrotbyoption(7,(float)idelx);
              }/*dodocktran(1,(float)idelx)*/ /*001005*/
              if(idely != 0) 
              {
                 bondrotbyoption(8,(float)idely);
              }/*dodocktran(2,(float)idely)*/ /*001005*/
           }
        }
        else
        {
           if(irotxy == 0) /*mouse in upper part of screen*/ /*030205*/
           {
              iztran = iztran + idelx;
           }
           else
           {
              ixtran = ixtran + idelx;
              iytran = iytran + idely;
           }
        }
     }
     else if (irotxy >= 1)
     {/* xy rotation around center */

      if(Ldockingscope && Lshiftkey && !Legamickdocking)
      {    /*mouse movements need opposite sign for rotation around y axis*/
           if(idelx != 0) 
           {
              bondrotbyoption(5,-(float)idelx); /*del x units around y*/
           }/*dodockrot(2,-(float)idelx)*/ /*001005*/
           if(idely != 0) 
           {
              bondrotbyoption(4, (float)idely); /*del y units around x*/
           }/*dodockrot(1, (float)idely)*/ /*001005*/
      }
      else
      {/* xy rotation around center */
       /* get sin and cos of angle from change in mouse position */
       if(irotxy <= 2 || Lhorizon)
       {/*do rotation around y axis, exclusively if irotxy == 2 */
          /*971204 special case for 90 deg around y axis*/
        /*if     (idelx == 157) fvalue = 157.07963;*/     /*971204*/
        /*else if(idelx ==-157) fvalue =-157.07963;*/     /*971204*/
          if     (mode == -2) fvalue = -157.07963; /*031127*/
          else if(mode ==  2) fvalue =  157.07963; /*031127*/
          else if(mode == -4) fvalue = -stereoangle*0.55555*3.14159; /*031201*/
          else if(mode ==  4) fvalue =  stereoangle*0.55555*3.14159; /*031127*/
          else                fvalue = idelx;     
          fvalue = fvalue/100.0;
/* 157.07963 = 100*(90*PI/180)==100*90*3.14159/180==50*PI== angle*.5555*PI */
          /* sin and cos of current delta angle of this axis */
          sinang = sin(fvalue); /*c*/ 
          cosang = cos(fvalue); /*c*/ 
          /*update:aij-matrix X y-matrix: a11 a12 a13     cosang   0   -sinang  
                                          a21 a22 a23  X    0      1      0
                                          a31 a32 a33     sinang   0    cosang
                                                                              */
          b11 = (a11*cosang + a13*sinang);
          b12 = a12;
          b13 = (a11*(-sinang) + a13*cosang);
          b21 = (a21*cosang + a23*sinang);
          b22 = a22;
          b23 = (a21*(-sinang) + a23*cosang);
          b31 = (a31*cosang + a33*sinang);
          b32 = a32;
          b33 = (a31*(-sinang) + a33*cosang);
          
          a11 = b11; a12 = b12; a13 = b13;
          a21 = b21; a22 = b22; a23 = b23;
          a31 = b31; a32 = b32; a33 = b33;
       }/*do rotation around y axis, exclusively if irotxy == 2 */

       if(!Lhorizon && irotxy != 2 )
       {/*do rotation around x axis, exclusively if irotxy == 3 */
            /* get sin and cos of angle from change in mouse position */
          /*if     (idely == 157) fvalue = 157.07963;*/
          /*else if(idely ==-157) fvalue =-157.07963;*/
            if     (mode < 0) fvalue = -157.07963; /*031127*/
            else if(mode > 0) fvalue =  157.07963; /*031127*/
            else              fvalue = -idely;   
            fvalue = fvalue/100.0;
         /* sin and cos of current delta angle of this axis */
            sinang = sin(fvalue); /*c*/
            cosang = cos(fvalue); /*c*/

         /* update: aij-matrix X x-matrix: a11 a12 a13      1       0      0     
                                        a21 a22 a23  X   0     cosang sinang
                                        a31 a32 a33      0    -sinang cosang
                                                                            */
         b11 = a11;
         b12 = (a12*cosang - a13*sinang);
         b13 = (a12*sinang + a13*cosang);
         b21 = a21;
         b22 = (a22*cosang - a23*sinang);
         b23 = (a22*sinang + a23*cosang);
         b31 = a31;
         b32 = (a32*cosang - a33*sinang);
         b33 = (a32*sinang + a33*cosang);

         a11 = b11; a12 = b12; a13 = b13;
         a21 = b21; a22 = b22; a23 = b23;
         a31 = b31; a32 = b32; a33 = b33;
       }/*do rotation around x axis, exclusively if irotxy == 3 */
      }/* xy rotation around center */
     }/* xy rotation around center */
     else  /*irotxy==0*/
     {/* z rotation around center */
      if(Ldockingscope && Lshiftkey && !Legamickdocking)
      {/*mouse movements need opposite sign for dodockrot*/
         if(idelx != 0) 
         {
            bondrotbyoption(6,-(float)idelx); /*del x units around z*/
         }/*dodockrot(3,-(float)idelx)*/ /*001005*/
      }
      else
      {/* z rotation around center */
      /* get sin and cos of angle from change in mouse position */
          /*if     (idelx == 157) fvalue = 157.07963;*/
          /*else if(idelx ==-157) fvalue =-157.07963;*/
            if     (mode < 0) fvalue = -157.07963; /*031127*/
            else if(mode > 0) fvalue =  157.07963; /*031127*/
            else              fvalue = idelx;   
            fvalue = fvalue/100.0;
      /* sin and cos of current delta angle of this axis */
            sinang = sin(fvalue); /*c*/
            cosang = cos(fvalue); /*c*/
      /* update: aij-matrix X z-matrix: a11 a12 a13     cosang sinang   0
                                        a21 a22 a23  X -sinang cosang   0
                                        a31 a32 a33       0      0      1 */
      /* accummulate into temporary matrix */
      b11 = (a11*cosang - a12*sinang);
      b12 = (a11*sinang + a12*cosang);
      b13 = a13;
      b21 = (a21*cosang - a22*sinang);
      b22 = (a21*sinang + a22*cosang);
      b23 = a23;
      b31 = (a31*cosang - a32*sinang);
      b32 = (a31*sinang + a32*cosang);
      b33 = a33;

      /* restore working matrix */
      a11 = b11; a12 = b12; a13 = b13;
      a21 = b21; a22 = b22; a23 = b23;
      a31 = b31; a32 = b32; a33 = b33;
      }/* z rotation around center */
     }/* z rotation around center */
     if(Ldockingscope && Lshiftkey && !Legamickdocking)
     {
        rescalekinemage(); /*MAGEINPT.C*/
     }
}
/*___getrot()_______________________________________________________________*/
 
/****normalizevector_points()*************************************************/
int normalizevector_points(double a[3],
                           int x2,int y2,int z2,int x1,int y1,int z1)
{
  int ireturn;
  
  a[0] = (double)(x2-x1);
  a[1] = (double)(y2-y1);
  a[2] = (double)(z2-z1);
  if(normalizevector_vector(a,a[0],a[1],a[2]))
  {
     ireturn = 1;
  }
  else
  { 
     ireturn = 0;
  }
  return(ireturn);
}
/*___normalizevector_points()________________________________________________*/

/****normalizevector_vector()*************************************************/
int normalizevector_vector(double a[3],double x, double y, double z)
{
  int ireturn;
  double mag;
  
  a[0] = x;
  a[1] = y;
  a[2] = z;
  mag = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
  if(mag > 0.001)
  {
     a[0] = a[0]/mag;
     a[1] = a[1]/mag;
     a[2] = a[2]/mag;
     ireturn = 1;
  }
  else
  { 
     ireturn = 0;
  }
  return(ireturn);
}
/*___normalizevector_vector()________________________________________________*/
 
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****surfacetriangle()********************************************************/
void surfacetriangle(int xyzflag,int jx0,int jy0,int jz0
,int jx1,int jy1,int jz1,int jx2,int jy2,int jz2
,int pointcounter
,pointstructptr pt0ptr,pointstructptr pt1ptr,pointstructptr pt2ptr)
{
  double aa[3],bb[3],cc[3],dd[3],faa[3],fbb[3],fdd[3];
  static double bulb[3],dblxyz[3];
  double thisrectnormal[3];
  double thisnormaldotted;
  int    Lshade,Lconnect=0;
  static int newrun = 1;
  int Lribbon, LocalLraster3Drender;
  static long   ncountxyz;
  static pointstructptr ptptr[5],pt1stptr,pt2ndptr;
  int ideep=0; /*040917*/
  float fcxyz[3][7];  /*080912 for pixelated triangle*/
  float AP=0,BP=0,CP=0,DP=0,T=0; /*080912 eq of plane for pixelated triangle*/
  int   minx=0,maxx=0,miny=0,maxy=0,x=0,y=0,z=0,k=0,Lok=0; /*080912 pixelated*/
  float fx=0,fy=0,fz=0; /*080912 pixelated...*/
  int   itcolor=0,itdepth=0; /*080912 pixelated...*/
  float trianglept[4][3], edgemiddle[4][3], edgenormal[4][3]; /*080916*/
  float punitivept[3]; /*080916*/



 if( (xyzflag & VARIANT2) ) Lribbon = 1; /*RIBBON_VARIENT of TRIANGLE*/
 else Lribbon = 0; /*TRIANGLE */

  if(Lraster3Drender) LocalLraster3Drender = 1; 
  else LocalLraster3Drender = 0; /*jumper for testing */

  if(LocalLraster3Drender)
  {/*LocalLraster3Drender: compute float normal in original orientation*/
      /*surfacetriangle() called only if Lpointsarein set by pointcounter >= 3*/
      /*so this first condition is first encountered with pointcounter==3*/
      if(pointcounter==3)
      {/*start of a new strip*/    
         /*ptptr[1,2,3,4] is local static storage */
         if(   ptptr[3] != NULL
            && ptptr[4] != NULL
            && ptptr[3]->fx == pt0ptr->fx /*pt0ptr 1st of current points*/
            && ptptr[3]->fy == pt0ptr->fy
            && ptptr[3]->fz == pt0ptr->fz
            && ptptr[4]->fx == pt1ptr->fx /*pt1ptr 2nd of current points*/
            && ptptr[4]->fy == pt1ptr->fy
            && ptptr[4]->fz == pt1ptr->fz)
         {/*continuation of the same ribbon strip, need to average normals*/
            pt1stptr = ptptr[3]; /*keep old points in working average points*/
            pt2ndptr = ptptr[4];
            Lconnect = 1;
         }
         else
         {/*presumably a new and separate ribbon section*/
            Lconnect = 0;
         }
         ptptr[1] = pt0ptr; /*double-up on first new point*/
         ptptr[2] = pt0ptr;
         ptptr[3] = pt1ptr;
         if(ptptr[2]->moreinfoptr==NULL) /*now first new point*/
         {
             ptptr[2]->moreinfoptr = allocmoreinfostructure();
         }
         if(ptptr[3]->moreinfoptr==NULL) /*now second new point*/
         {
             ptptr[3]->moreinfoptr = allocmoreinfostructure();
         }
      }/*start of a new strip*/
      else
      {/*pointcounter > 3*/
        ptptr[1] = ptptr[2]; /*shift local point storage*/
        ptptr[2] = ptptr[3];
        ptptr[3] = ptptr[4];
      }
      ptptr[4] = pt2ptr; /*count from zero third point passed to subroutine*/
           /*set 4th local point to the current 3rd, truely new, one*/
      if(ptptr[4]->moreinfoptr==NULL)
      {
          ptptr[4]->moreinfoptr = allocmoreinfostructure();
      }

      if(    ptptr[4]->fx == ptptr[2]->fx
          && ptptr[4]->fy == ptptr[2]->fy
          && ptptr[4]->fz == ptptr[2]->fz )
      {/*degenerate triangle e.g. at repeat of crosstie between residues*/
         /*copy old normal to new point*/
         if(ptptr[4]->moreinfoptr!=NULL && ptptr[2]->moreinfoptr!=NULL)
         {
           ptptr[4]->moreinfoptr->f1 = ptptr[2]->moreinfoptr->f1;
           ptptr[4]->moreinfoptr->f2 = ptptr[2]->moreinfoptr->f2;
           ptptr[4]->moreinfoptr->f3 = ptptr[2]->moreinfoptr->f3;         
         }
         LOK = 0; /*signel not to use this degenerate triangle*/
      }/*degenerate triangle e.g. at repeat of crosstie between residues*/
      else if(    ptptr[4]->fx == ptptr[3]->fx
               && ptptr[4]->fy == ptptr[3]->fy
               && ptptr[4]->fz == ptptr[3]->fz )
      {/*degenerate triangle e.g. at repeat of adjacent points*/
         /*copy old normal to new point*/
         if(ptptr[4]->moreinfoptr!=NULL && ptptr[3]->moreinfoptr!=NULL)
         {
           ptptr[4]->moreinfoptr->f1 = ptptr[3]->moreinfoptr->f1;
           ptptr[4]->moreinfoptr->f2 = ptptr[3]->moreinfoptr->f2;
           ptptr[4]->moreinfoptr->f3 = ptptr[3]->moreinfoptr->f3;         
         }
         LOK = 0; /*signel not to use this degenerate triangle*/
      }/*degenerate triangle e.g. at repeat of adjacent points*/
      else if(    ptptr[3]->fx == ptptr[2]->fx
               && ptptr[3]->fy == ptptr[2]->fy
               && ptptr[3]->fz == ptptr[2]->fz )
      {/*degenerate triangle e.g. at repeat of adjacent points*/
        /*copy old normal to new point*/ /*can one ever get this condition????*/
         if(ptptr[3]->moreinfoptr!=NULL && ptptr[2]->moreinfoptr!=NULL)
         {
           ptptr[3]->moreinfoptr->f1 = ptptr[2]->moreinfoptr->f1;
           ptptr[3]->moreinfoptr->f2 = ptptr[2]->moreinfoptr->f2;
           ptptr[3]->moreinfoptr->f3 = ptptr[2]->moreinfoptr->f3;         
         }
         LOK = 0; /*signel not to use this degenerate triangle*/
      }/*degenerate triangle e.g. at repeat of adjacent points*/
      else
      {/*passed first degeneracy test*/
       /*get normal to the plane of the 3 current points*/
       faa[0] = pt1ptr->fx - pt0ptr->fx;
       faa[1] = pt1ptr->fy - pt0ptr->fy;
       faa[2] = pt1ptr->fz - pt0ptr->fz;
       fbb[0] = pt2ptr->fx - pt1ptr->fx;
       fbb[1] = pt2ptr->fy - pt1ptr->fy;
       fbb[2] = pt2ptr->fz - pt1ptr->fz;
       crossproduct( fbb[0],fbb[1],fbb[2],faa[0],faa[1],faa[2]);
       /*crossproduct returns normal vector as globals: dblx, dbly, dblz */ 
       normalizevector_vector(fdd, dblx, dbly, dblz); /*MAGEDRAW*/
       /*store this normal with the current (last) point*/
       if(ptptr[4]->moreinfoptr!=NULL)
       {/*can store extra info*/
         if( Lribbon && fmod(pointcounter,2)==0.0 && pointcounter>3)
         {/*even number of points and enough points for a rectangle*/
           /*average normals of last 2 triangles to make normal of rectangle*/
           /*reversing normal to previous triangle so direction is consistent*/
           fdd[0] = (-(ptptr[3]->moreinfoptr->f1)+fdd[0])/2.0;
           fdd[1] = (-(ptptr[3]->moreinfoptr->f2)+fdd[1])/2.0;
           fdd[2] = (-(ptptr[3]->moreinfoptr->f3)+fdd[2])/2.0;

           ptptr[3]->moreinfoptr->f1 = (float)fdd[0];
           ptptr[3]->moreinfoptr->f2 = (float)fdd[1];
           ptptr[3]->moreinfoptr->f3 = (float)fdd[2];
           ptptr[4]->moreinfoptr->f1 = (float)fdd[0];
           ptptr[4]->moreinfoptr->f2 = (float)fdd[1];
           ptptr[4]->moreinfoptr->f3 = (float)fdd[2];    
           
           ptptr[4]->moreinfoptr->it = 4; /*flag for last point of a rectangle*/
           if(pointcounter == 4)
           {/*load normals for first two triangle points*/
              if(Lconnect)
              {/*average normals with last triangle of previous strip*/
                 Lconnect = 0;
                 fdd[0] = ((pt1stptr->moreinfoptr->f1)+fdd[0])/2.0;
                 fdd[1] = ((pt1stptr->moreinfoptr->f2)+fdd[1])/2.0;
                 fdd[2] = ((pt1stptr->moreinfoptr->f3)+fdd[2])/2.0;
                 /*store old strip triangle normals*/
                 pt1stptr->moreinfoptr->f1 = (float)fdd[0];
                 pt1stptr->moreinfoptr->f2 = (float)fdd[1];
                 pt1stptr->moreinfoptr->f3 = (float)fdd[2];
                 pt2ndptr->moreinfoptr->f1 = (float)fdd[0];
                 pt2ndptr->moreinfoptr->f2 = (float)fdd[1];
                 pt2ndptr->moreinfoptr->f3 = (float)fdd[2];
              }
              /*store in present strip triangle*/
              ptptr[1]->moreinfoptr->f1 = (float)fdd[0];
              ptptr[1]->moreinfoptr->f2 = (float)fdd[1];
              ptptr[1]->moreinfoptr->f3 = (float)fdd[2];
              ptptr[2]->moreinfoptr->f1 = (float)fdd[0];
              ptptr[2]->moreinfoptr->f2 = (float)fdd[1];
              ptptr[2]->moreinfoptr->f3 = (float)fdd[2];           
           }
           else if(pointcounter >= 6)
           {/*average normals for previous rectangle junction points*/
              ptptr[1]->moreinfoptr->f1 = ( (ptptr[3]->moreinfoptr->f1)
                                           +(float)fdd[0])/2;
              ptptr[1]->moreinfoptr->f2 = ( (ptptr[3]->moreinfoptr->f2)
                                           +(float)fdd[1])/2;
              ptptr[1]->moreinfoptr->f3 = ( (ptptr[3]->moreinfoptr->f3)
                                           +(float)fdd[2])/2;
              ptptr[2]->moreinfoptr->f1 = ptptr[1]->moreinfoptr->f1;
              ptptr[2]->moreinfoptr->f2 = ptptr[1]->moreinfoptr->f2;
              ptptr[2]->moreinfoptr->f3 = ptptr[1]->moreinfoptr->f3;           
           }
         }/*even number of points and enough points for a rectangle*/
         else
         {/*just store this normal with the current (last) point*/
           ptptr[4]->moreinfoptr->f1 = (float)fdd[0];
           ptptr[4]->moreinfoptr->f2 = (float)fdd[1];
           ptptr[4]->moreinfoptr->f3 = (float)fdd[2];
           if( Lribbon )
           {/*flag as rect part but not last part*/
              ptptr[4]->moreinfoptr->it = 3; /*NOT last point of a rectangle*/
           }
           else
           {
              ptptr[4]->moreinfoptr->it = 1; /*NOT part of a rectangle*/
           }
           if(pointcounter==3 && Lconnect==0)
           {/*first triangle of a new strip, first two points need normals*/
              ptptr[2]->moreinfoptr->f1 = (float)fdd[0];
              ptptr[2]->moreinfoptr->f2 = (float)fdd[1];
              ptptr[2]->moreinfoptr->f3 = (float)fdd[2];
              ptptr[3]->moreinfoptr->f1 = (float)fdd[0];
              ptptr[3]->moreinfoptr->f2 = (float)fdd[1];
              ptptr[3]->moreinfoptr->f3 = (float)fdd[2];
           }
         }/*DO THESE HAVE TO BE REVERSED EVERY OTHER ONE FOR SAME SIDEDNESS???*/
       }/*can store extra info*/
      }/*passed first degeneracy test*/

  }/*LocalLraster3Drender: compute float normal in original orientation*/
  /*end of raster3D section*/
  /*regular display section*/
  /*not done as if/else since other parts of the triangle code are also intact*/ 
  if(newrun)
  {
      /*lighting-vector*/
      normalizevector_vector(bulb, -0.5, -0.5, 1.0);  /*.*/
      newrun = 0;
  }

 if(   normalizevector_points(aa,jx1,jy1,jz1,jx0,jy0,jz0)    /*.*/
    && normalizevector_points(bb,jx2,jy2,jz2,jx1,jy1,jz1)    /*.*/
    && normalizevector_points(cc,jx2,jy2,jz2,jx0,jy0,jz0) )  /*.*/
 {/*LOK still OK, real triangle, not 2 or 3 identical points*/

   crossproduct(bb[0],bb[1],bb[2],aa[0],aa[1],aa[2]);
   /*crossproduct returns normal vector as globals: dblx, dbly, dblz */
   normalizevector_vector(dd, dblx, dbly, dblz);
   /*This is the normalized normal vector to this triangle*/


   /*color depthcue by angle to lighting_vector*/
   /*triangle list and ribbon list done as linked triangles*/
   /*BUT ribbon is lighted as successive rectangles between cross-ties*/


   if( Lribbon && fmod(pointcounter,2)==0.0 && pointcounter>3)
   {/*even number of points and enough points*/
     /*average normals of last 2 triangles to make normal of rectangle*/
     normalizevector_vector(thisrectnormal
       , dd[0] + dblxyz[0], dd[1] + dblxyz[1], dd[2] + dblxyz[2]);

     /*bulb[] is normalized lighting-vector */
     thisnormaldotted =   thisrectnormal[0]*bulb[0]
             + thisrectnormal[1]*bulb[1]
             + thisrectnormal[2]*bulb[2];
     Lshade = 1;

   }/*even number of points and enough points*/
   else if(Lribbon && pointcounter>2)
   {/*first triangle in for a rectangle*/
     dblxyz[0] = -dd[0];  /*store flipped triangle normal*/
     dblxyz[1] = -dd[1];
     dblxyz[2] = -dd[2];
     ncountxyz = ncount;
     Lshade = 0;
   }/*first triangle in for a rectangle*/
   else if(pointcounter>2)
   {/*a triangle*/
     if(fmod(pointcounter,2)==0.0)
     {/*flip normal*/
       dd[0] = -dd[0];  /*triangle normal*/
       dd[1] = -dd[1];
       dd[2] = -dd[2];
     }
     /*bulb[] is normalized lighting-vector */
     thisnormaldotted =   dd[0]*bulb[0]
               + dd[1]*bulb[1]
               + dd[2]*bulb[2];
     Lshade = 1;
   }/*a triangle*/
   if(Lshade)
   {/*rect or triangle to be shaded*/
      /*clear depth cue 3-bit space 01 111 111 000 11111*/
      if(Lribbon) it(0,ncountxyz) = it(0,ncountxyz) & 32543;
      it(0,ncount) = it(0,ncount) & 32543;
      /* 1:001=32, 2:010=64, 3:011=96, 4:100=128, 5:101=160, 110=192,111=224*/
#ifdef WHITEBKGRESORT
      if(Lwhitebkg) /*040917 change progression and interval limits*/
      {/*WHITE background*/
         if(  thisnormaldotted < -0.95 || thisnormaldotted > 0.95 )
         {/*toward light*/
            ideep =  64; /*040917*/
         }
         else if(  thisnormaldotted < -0.80 || thisnormaldotted > 0.80 )
         {/*sort of toward light*/
            ideep =  96; /*040917*/
         }
         else if(  thisnormaldotted < -0.60 || thisnormaldotted > 0.60 )
         {/*side to light*/
            ideep = 128; /*040917*/
         }
         else
         {/*back from light*/
            ideep = 160; /*040917*/
         }
      }/*WHITE background*/
      else
#endif
      {/*BLACK background*/
         if(  thisnormaldotted < -0.85 || thisnormaldotted > 0.85 )
         {/*toward light*/
            ideep = 160; 
         }
         else if(  thisnormaldotted < -0.70 || thisnormaldotted > 0.70 )
         {/*sort of toward light*/
            ideep = 128; 
         }
         else if(  thisnormaldotted < -0.60 || thisnormaldotted > 0.60 )
         {/*side to light*/
            ideep =  96; 
         }
         else
         {/*back from light*/
            ideep =  64; 
         }
      }/*BLACK background*/
      if(Lribbon) it(0,ncountxyz) = it(0,ncountxyz) | ideep;
      it(0,ncount) = it(0,ncount) | ideep;
      
      /*--------------------------------080912-----------------------------*/
      /*080912 pixelatedlist area of triangle filled with z-buffered dots*/
         itdepth = ideep;
         itcolor = it(0,ncount)&31;
/*
fprintf(stderr,"it(0,%d)==%d, color#: %d, depth: %d, itcolor: %d, itdepth: %d\n",ncount,it(0,ncount),it(0,ncount)&31,it(0,ncount)&224,itcolor,itdepth);
*/
      if(it(7,ncount) & PIXELATED)
      {/*PIXELATED,pixelated*/   
         trianglept[1][0] = fcxyz[0][1] = (float)jx0; /* 1st of current points*/
         trianglept[1][1] = fcxyz[1][1] = (float)jy0;
         trianglept[1][2] = fcxyz[2][1] = (float)jz0;
         trianglept[2][0] = fcxyz[0][2] = (float)jx1; /* 2nd of current points*/
         trianglept[2][1] = fcxyz[1][2] = (float)jy1;
         trianglept[2][2] = fcxyz[2][2] = (float)jz1;
         trianglept[3][0] = fcxyz[0][3] = (float)jx2; /* 3rd of current points*/
         trianglept[3][1] = fcxyz[1][3] = (float)jy2;
         trianglept[3][2] = fcxyz[2][3] = (float)jz2;
         EquationOfPlane(fcxyz); 
            /*080912 returns A B C D of eq of plane in fcxyz[][5],fcxyz[0][6]*/
            /*  AP x  +  BP y  +  CP z  +  DP  =  0  */
         AP = fcxyz[0][5];
         BP = fcxyz[1][5];
         CP = fcxyz[2][5];
         DP = fcxyz[0][6];
/*
         fprintf(stderr,"MAGEUTIL:equation of triangle plane: %fx + %fy + %fz + %f = 0\n",AP,BP,CP,DP); 
*/
         /*pprevious arrays and following arrays in different order... */

         TriangleParams(trianglept, edgemiddle, edgenormal);  /*080916*/
         /*screen x,y bounds of triangle*/
         minx = jx0;
         if(jx1 < minx) minx = jx1;
         if(jx2 < minx) minx = jx2;
         miny = jy0;
         if(jy1 < miny) miny = jy1;
         if(jy2 < miny) miny = jy2;
         maxx = jx0;
         if(jx1 > maxx) maxx = jx1;
         if(jx2 > maxx) maxx = jx2;
         maxy = jy0;
         if(jy1 > maxy) maxy = jy1;
         if(jy2 > maxy) maxy = jy2;
/*
fprintf(stderr,"minx==%d, maxx==%d, miny==%d, maxy==%d\n",minx,maxx,miny,maxy);
*/
         /*scan over x,y bounds, find all points within the triangle*/
         for(x = minx; x <= maxx; x++)
         {/*x*/
            for(y = miny; y <= maxy; y++)
            {/*y*/
               fx = (float)x;
               fy = (float)y;
               fz = (-DP - AP*fx - BP*fy)/CP; 
               z  = (int)fz;
               Lok = 1;
               if(Lzclipon)
               {
/*fprintf(stderr,"Lzclipon== %d, limitz== %d\n",Lzclipon,limitz);*/
                 if(limitz)
                 {
                    if(z <izminlimit || z >izmaxlimit) {Lok=0;}
                 }
                 else
                 {
                    if( z < izmin || z > izmax) {Lok=0;}
                 }
               }
               if(Lok != 0)
               {/*survived z-clipping*/
                 punitivept[0] = fx;
                 punitivept[1] = fy;
                 punitivept[2] = fz;
                 Lok = InTriangle2(punitivept,edgemiddle,edgenormal); 
/*
{fprintf(stderr,"x== %d, y== %d, z== %d, InTriangle== %d\n",x,y,z,Lok);}
*/

                 if(Lok != 0)
                 {/*InTriangle*/
                    /*add a z-buffered dot to the display list*/
                    if(ncount >= MAXDRAWS-10)
                    {
                      allocdisplayarrays(5000);
                    }
                    if(ncount < MAXDRAWS-10)
                    {/*room for another point in displaylist*/
                      ncount++;
                      /*it(0,ncount) = it(0,ncount-1) | ideep;*/ /*effective color*/
                      it(0,ncount) = itdepth | itcolor;
                      it(1,ncount) = x;
                      it(2,ncount) = y;
                      it(3,ncount) = x;
                      it(4,ncount) = y;
                      it(5,ncount) = x;
                      it(6,ncount) = y;
                      it(7,ncount) = 6; /*dot*/

                      /*from MAGEDRAW z-buffer code...*/
                      /*place in z-buffer bin by screen z coord, actual ideep*/
                      ideep=(1000L*(long)( (z) + izadjust))/izbinsize;
                      if(ideep <= 0) ideep = 0;
                      /*from MAGEDRAW: maxdeep = ((Nbins)-1); */
                      if(ideep >  maxdeep ) ideep = maxdeep;
                      if(thelink(0,ideep)==0)
                      {
                        thelink(0,ideep) = ncount;
                        /*establish the first point at this ideep*/
                        /*ideep is current z-buffer bin*/
                      }
                      else
                      {
                        k = thelink(1,ideep);
                        plinkit((long)k) = ncount;
                        /*inform previous point at this ideep about*/
                        /* its next point*/
                      }
                      thelink(1,ideep) = ncount;
                      /*record current point as the*/
                      /*effective last point at this ideep*/
                      plinkit((long)ncount) = 0;
                      /*flag current point that it is, at*/
                      /*least for now, the last point at ideep z level*/
                    }/*room for another point in displaylist*/
                 }/*InTriangle*/
               }/*survived z-clipping*/
            }/*y*/
         }/*x*/
      }/*PIXELATED,pixelated*/   
      /*__________________________________080912___________________________*/

   }/*rect or triangle to be shaded*/
  }/*LOK still OK, real triangle, not 2 or 3 identical points*/
  else if(jx2==jx0 && jy2==jy0 && jz2==jz0)
  {/*current point == 2nd point back */
     it(0,ncount) = it(0,ncount) | it(0,ncount-2);
  }
  else if(jx2==jx1 && jy2==jy1 && jz2==jz1)
  {/*current point == 2nd point back */
     it(0,ncount) = it(0,ncount) | it(0,ncount-1);
  }
  else
  {
    LOK = 0;/*failed to handle triangle*/
  }

}
/*coded in thispointptr->colorwidth*/
/* 2 byte integer: "colordepthwidthtype"
extra types:     8,     9,    10,    11,    12,    13,    14,    15
             16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720

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

/*___surfacetriangle()_______________________________________________________*/

/****stackeddiskssphere()*****************************************************/
void stackeddiskssphere(int jradius,int izadjust,long izbinsize, int maxdeep )
{            
  int    jdeep[10]; /*040918b 9 disks rather than 5 */
  static int depthquelock[10],jradiusfudge[10],jzoffsetfudge[10];
  static int jradiusstandard;
  static int  newrun=1;
  int    i,colordepthwidthtype;

  /*"colordepthwidthtype"  (at least) 2 byte integer                 */
  /*|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|*/
  /*    |    point type |pen width  |depth cue  | color code        |*/
  /*                                |128| 64| 32|                   |*/
/*32543 MASK for everything except depth cue code and negative bit*/

/*color = (it(0,ncount)     & 31);*/
/*depth = (it(0,ncount)>> 5 &  7);*/
/*width = (it(0,ncount)>> 8 &  7);*/
/*type  = (it(0,ncount)>>11 & 15);*/ /*000804, 080406 type = it(7,ncount) */
/*
printf("ncount== %ld, color== %d, depth== %d, width== %d, type== %d\n"
,ncount,(it(0,ncount)     & 31),(it(0,ncount)>> 5 &  7)
     ,(it(0,ncount)>> 8 &  7),(it(0,ncount)>>11 & 15) ); 
printf("ncount== %ld, color== %d, depth== %d, width== %d, type== %d\n"
,ncount,(it(0,ncount)     & 31),(it(0,ncount)>> 5 &  7)
     ,(it(0,ncount)>> 8 &  7),(it(7,ncount)) );  
*/
#ifdef OLDCODE
  /*if(newrun)*/ /*always do this so can change between blk & wht bkg*/
  {/*initialize*/
    /* 1:001=32, 2:010=64, 3:011=96, 4:100=128, 5:101=160 */
    if(Lwhitebkg)  /*040917*/
    {
      depthquelock[1] = 160;
      depthquelock[2] = 160;
      depthquelock[3] = 128;
      depthquelock[4] =  96;
      depthquelock[5] =  96;
    }
    else
    {                        /* old*/
      depthquelock[1] =  64; /*  64 ____DRAW sets to blk 040918*/
      depthquelock[2] =  64; /*  96 */
      depthquelock[3] =  96; /* 128 */
      depthquelock[4] = 128; /* 160 */
      depthquelock[5] = 128; /* 160 */
    }
      jradiusfudge[1] = 20;
      jradiusfudge[2] = 19;
      jradiusfudge[3] = 16;
      jradiusfudge[4] = 12;
      jradiusfudge[5] =  6;

      jzoffsetfudge[1] =  0;
      jzoffsetfudge[2] =  6;
      jzoffsetfudge[3] = 12;
      jzoffsetfudge[4] = 16;
      jzoffsetfudge[5] = 19;

      jradiusstandard  = 20;
      newrun = 0;
  }/*initialize*/
#else
  /*if(newrun)*/ /*always do this so can change between blk & wht bkg*/
  {/*initialize*/
    /* 1:001=32, 2:010=64, 3:011=96, 4:100=128, 5:101=160 */
#ifdef WHITEBKGRESORT
    if(Lwhitebkg)  /*040917*/
    {
      depthquelock[1] = 160; /*____DRAW sets to blk 040918*/
      depthquelock[2] = 160;
      depthquelock[3] = 128;
      depthquelock[4] =  96;
      depthquelock[5] =  64;
      depthquelock[6] =  64;
      depthquelock[7] =  32;
      depthquelock[8] =  32;
      depthquelock[9] =  32;
      /*160,160,160,128,128,96,64,64,64 not as good separation*/
      /*but all suffer from colors too unsaturated 040918 */
    }
    else
#endif
    {                        /* old*/
      depthquelock[1] =  32; /*  64 ____DRAW sets to blk 040918*/
      depthquelock[2] =  32; /*  96 */
      depthquelock[3] =  64; /* 128 */
      depthquelock[4] =  96; /* 160 */
      depthquelock[5] = 128; /* 160 */
      depthquelock[6] = 128; /*  96 */
      depthquelock[7] = 160; /* 128 */
      depthquelock[8] = 160; /* 160 */
      depthquelock[9] = 160; /* 160 */
    }
      jradiusfudge[1] = 40;
      jradiusfudge[2] = 39;
      jradiusfudge[3] = 36;
      jradiusfudge[4] = 32;
      jradiusfudge[5] = 28;
      jradiusfudge[6] = 24;
      jradiusfudge[7] = 18;
      jradiusfudge[8] = 12;
      jradiusfudge[9] =  6;

      jzoffsetfudge[1] =  0;
      jzoffsetfudge[2] =  8;
      jzoffsetfudge[3] = 18;
      jzoffsetfudge[4] = 24;
      jzoffsetfudge[5] = 29;
      jzoffsetfudge[6] = 32;
      jzoffsetfudge[7] = 36;
      jzoffsetfudge[8] = 38;
      jzoffsetfudge[9] = 40;

      jradiusstandard  = 40;
      newrun = 0;
  }/*initialize*/
#endif

  /*full disk has just been specified, */
  /*declare it a base disk == disk 1*/
  it(3,ncount) = 1; /*1st of 5 stacked disks, 9 as of 040918b*/
  
  jdeep[1]=1000L*(long)(jz2+izadjust )/izbinsize;
  if(Lzclipon && (jdeep[1]<0 || jdeep[1]>maxdeep))
    it(4,ncount) = 0; /*minimize radius so base disk obscured*/
    /*990710 what is the purpose of this????*/
    
  colordepthwidthtype = (it(0,ncount) & 32543);  
  /*3bit depth cue space cleared, to be set below*/
  /*5 bits colorcode, 3 bits depthcue,3 bits pen width*/
  it(0,ncount) = (colordepthwidthtype|depthquelock[1]);  
  /*make disk 1 "dark" for contrast, i.e. lock it to*/
  /*penultimate deepest depthcue bin*/

  /*Now specify stacked disks at their own depth*/
  /*with graduated radii*/
  /*parent sphere divided into jradiusstandard parts*/
  /*040918 use 40ths, same sort of idea about divisions*/
  /*9 disk code must also be in platform specific ____DRAW/balltoscreen() */
  /*e.g. parent sphere divided in 20ths, disks designed*/
  /*to give annuli in projection as if 0-90 lattitude*/
  /*were divided ~evenly into five 18 deg bands*/
  /*Note highlight is on penultimate top annulus*/
  /*040918b: use 9 disks, with highlight on appropriate disk */
  /*040918b: 9 disk code change in platform specific ____DRAW/balltoscreen()*/
  /*(multiply first in integer arith.) */

  for(i=2; i<=9; i++)  /*had been 5 040918b*/
  {/*disk i: 2-5, now 2-9 040918b*/ /*create new display points*/
    jdeep[i]=
       1000L 
       *(long)(jz2+( ( jzoffsetfudge[i]*jradius )/jradiusstandard )+izadjust )
       /izbinsize;
    /*(long)izbinsize is 1000* to avoid integer problems*/
    /*if(ideep[i] <= jdeep[i-1]) jdeep[i]=jdeep[i-1]+1;*/
    /*doesn't seem to need this assurance that disks be*/
    /*in separate z-buffered bins.  NOTE however, that*/
    /*the number of z-buffer bins is 1000 and they are*/
    /*strobed out by a link-list mechanism.*/
    
    if(     jdeep[i] <  0   ) jdeep[i] = 0; /*behind far clipping plane*/
    else if(jdeep[i]>maxdeep) jdeep[i] = maxdeep;/*in front of near one*/

    ncount = ncount +1;
    it(0,ncount) = (colordepthwidthtype|depthquelock[i]);
    it(1,ncount) = it(1,ncount-1);/*same ctr x, or PS offset*/ 
    it(2,ncount) = it(2,ncount-1);/*same ctr y, or R3D ideep*/
    it(3,ncount) = i; /*disk of stack*/
    it(4,ncount) = (jradiusfudge[i]*jradius)/jradiusstandard;
    it(5,ncount) = it(5,ncount-1);/*whatever, or PS offest*/
    it(6,ncount) = it(6,ncount-1);/*whatever, or PS L-R eye*/
    it(7,ncount) = it(7,ncount-1);/*type now here, not part of 0, 080406*/
    itpointptr[ncount] = itpointptr[ncount-1]; /*ref to same pt*/
        
    plinkit(ncount) = 0;
    if(thelink(0,jdeep[i])==0) thelink(0,jdeep[i])=ncount;
    else plinkit(thelink(1,jdeep[i])) = ncount;
    thelink(1,jdeep[i]) = ncount;
    /* see above for how link list is set up */
  }/*disk i: 2-5, now 2-9 040918b*/
}
/*___stackeddiskssphere()____________________________________________________*/

/****emphasistoscreen()*******************************************************/
void emphasistoscreen( int x1,int x2,int y1,int y2,int z1,int z2
   ,int ipen, int icolor
   ,void* passedptr)
{              /*emphasistoscreen(): 020112xrc*/
   /*select_ed vector is drawn as disk with no hilite on rearmost point, */
   /* double triangle along shaft, and disk on front point. */
   /*disk radius and shaft width = radius that of 2nd point (itpointptr) */
   /*or defaulted.  (For now, ignore hilite and back/front point order*/
   /*float angle = 0;*/
   float radius = 0;
   float scale = 1.0;
   /*float fx0=0, fy0=0, fz0=0;*/
   /*float cmag=0,fx3=0,fy3=0,fz3=0,fx4=0,fy4=0,fz4=0;*/
   /*int   j=0;*/
   float fx1=(float)x1;
   float fy1=(float)y1;
   /*float fz1=(float)z1;*/
   float fx2=(float)x2;
   float fy2=(float)y2;
   /*float fz2=(float)z2;*/
   /*float dxs=0, dys=0, dzs=0;*/
   pointstruct* itpointptr;
   float dx=0, dy=0, ex=0, ey=0, proj=0;

   itpointptr = (struct pointstruct *)passedptr;
   z1=z1; z2=z2; /*020304 avoid Mac compiler warning*/
   /*angle = (itpointptr->listptr)->angle;*/
   
   /*radius = itpointptr->radius; 030405 point radius needed for foo collision*/
   radius = 0; /*030405 still could use list radius, or just use default*/
   
   /*static lists only get the original, place-on-screen, scaling*/
   if( ( ( (itpointptr->listptr)->STATUS & STATICFLAG) == STATICFLAG ) )
        scale = Scale;/*global*/ /*fixed list, i.e. "static" */
   else scale = scalenew;/*global*/
   if(radius < .01) /*radius from point*/
   {
      if((itpointptr->listptr)->radius < .001) /*radius from list*/ 
      {
         radius = (float)(scale*0.04); /*020217*/
         /*radius = (float)(scale*0.1);*/
         /*radius = (float)(scale*0.2);*/
      }
      else 
      {
         radius = (float)(scale*(itpointptr->listptr)->radius);
      }
   }
   else
   {
      radius = scale*radius;
   }

   radius = radius + (float)(ipen);
   ipen = 1; /*since radius already includes ipen   031127*/
   if( (x2 == x1) && (y2 == y1) ) /*end on: just show disk*/
   {
      balltoscreen(x2,y2,0,(int)radius,0,ipen,icolor,0,0); /*040925*/
      /*(x,y,idisk,iradius,Lblackrim,ipen,icolor,type) 040925*/
   }
   else
   {
      balltoscreen(x1,y1,0,(int)radius,0,ipen,icolor,0,0); /*040925*/
      dx = fx2 - fx1;
      dy = fy2 - fy1;

      if(   (dx > 0.0001 || dx < -0.0001) 
          &&(dy > 0.0001 || dy < -0.0001) )
      {
         proj = (float)sqrt((double)(dx*dx + dy*dy) );
         ey = -dx/proj; /*screen space has inverted y direction*/
         ex = dy/proj;
      }
      else if(dx < 0.0001 && dx > -0.0001) {ey = 0; ex = 1;}
      else if(dy < 0.0001 && dy > -0.0001) {ex = 0; ey = 1;}
      triangletoscreen( (int)(fx1 - radius*ex),(int)(fy1 - radius*ey)
                       ,(int)(fx1 + radius*ex),(int)(fy1 + radius*ey)
                       ,(int)(fx2 - radius*ex),(int)(fy2 - radius*ey)
                       ,icolor);
      triangletoscreen( (int)(fx1 + radius*ex),(int)(fy1 + radius*ey)
                       ,(int)(fx2 - radius*ex),(int)(fy2 - radius*ey)
                       ,(int)(fx2 + radius*ex),(int)(fy2 + radius*ey)
                       ,icolor);
      balltoscreen(x2,y2,0,(int)radius,0,ipen,icolor,0,0); /*040925*/
   }
}
/*___emphasistoscreen()______________________________________________________*/

/****arrowtoscreen()**********************************************************/
void arrowtoscreen( int x1,int x2,int y1,int y2,int z1,int z2,void* passedptr)
/*void arrowtoscreen( int x1,int x2,int y1,int y2,int z1,int z2,pointstruct* itpointptr)*/
{              /*000804*/
   /*arrow head is drawn on second of two points at angle=angle and */
   /*spine length = radius  specified by that 2nd point (itpointptr) */
   float angle = 0;
   float radius = 0;
   float scale = 1.0;
   float fx0=0, fy0=0, fz0=0;
   float cmag=0,fx3=0,fy3=0,fz3=0,fx4=0,fy4=0,fz4=0;
   int   j=0;
   float fx1=(float)x1;
   float fy1=(float)y1;
   float fz1=(float)z1;
   float fx2=(float)x2;
   float fy2=(float)y2;
   float fz2=(float)z2;
   float dxs=0, dys=0, dzs=0;
   pointstruct* itpointptr;

   itpointptr = (struct pointstruct *)passedptr;

   angle = (itpointptr->listptr)->angle;
   radius = itpointptr->radius;

   /*always output the arrow shaft vector, even if just a point*/

   linetoscreen(x1,x2,y1,y2);  

   if( (x2 == x1) && (y2 == y1) && (z2 == z1) ) return; /*can't do arrow*/
   
   /*static lists only get the original, place-on-screen, scaling*/
   if( ( ( (itpointptr->listptr)->STATUS & STATICFLAG) == STATICFLAG ) )
        scale = Scale;/*global*/ /*fixed list, i.e. "static" */
   else scale = scalenew;/*global*/
   if(radius < .01) 
   {
      if((itpointptr->listptr)->radius < .01) 
      {
         if(distarrow > .01)
         {
            radius = (float)(scale*distarrow);
         }
         else
         {
            radius = (float)(scale*0.2);
         }
      }
      else 
      {
         radius = (float)(scale*(itpointptr->listptr)->radius);
      }
   }
   else
   {
      radius = scale*radius;
   }
   if(angle < .01) 
   {
      if((itpointptr->listptr)->angle < .01) 
      {
         if(anglearrow > .01)
         {
            angle = anglearrow;
         }
         else
         {
            angle = (float)(30.0);
         }
      }
      else 
      {
         angle = (float)((itpointptr->listptr)->angle);
      }
   }
   /* 1-------------->2   */
   /*Need 3rd point to give a dihedral direction for the arrow spines*/
   /*Make off-axis point offset from the two points:*/
   /* use lesser of dimension change as the dimension to offset*/
   /*this stupid routine ignores sign change in any dimension*/
     
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   dxs = (fx2 - fx1)*(fx2 - fx1);
   dys = (fy2 - fy1)*(fy2 - fy1);
   dzs = (fz2 - fz1)*(fz2 - fz1);
   if( dxs < dys)
   {/*del x < del y*/
      if(dxs < dzs)
      {/*x smallest*/
         fx0 = fx1 + 10;
         fy0 = fy1;
         fz0 = fz1;
      }
      else
      {/*z smallest*/
         fx0 = fx1;
         fy0 = fy1;
         fz0 = fz1 + 10;
      }
   }
   else
   {/* del y < del x*/
      if(dys < dzs)
      {/*y smallest*/
         fx0 = fx1;
         fy0 = fy1 + 10;
         fz0 = fz1;
      }
      else
      {/*z smallest*/
         fx0 = fx1;
         fy0 = fy1;
         fz0 = fz1 + 10;
      }
   }
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */

   /*calculate a new line from 2 using points 0,1,2 */
   
   crossproduct( (double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1)
                ,(double)(fx1-fx0),(double)(fy1-fy0),(double)(fz1-fz0));
      /*return vector: dblx,dbly,dblz (globals)*/   
   cmag = (float)sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) );
   /*normalize and make c vector the desired length*/
   if(cmag < 0.000001) return; /*can't do arrow*/
   fx3 = (float)dblx*radius/cmag;
   fy3 = (float)dbly*radius/cmag;
   fz3 = (float)dblz*radius/cmag;
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   /*                 |   */
   /*                 3   , where 2-->3 perpendicular to 0--1--2 */

   /*place point 3 at correct distance from point #2 */
   /* as xproduct it is at a dihedral and angle of 90 to 1--->2*/
   fx3 = fx3 + fx2;
   fy3 = fy3 + fy2;
   fz3 = fz3 + fz2;
   
   /*find axis around which to rotate to put point at spine angle*/
   /* 0                 4 , where 2-->4 perpendicular to 1--2--3 */
   /* |                /  */
   /* 1-------------->2   */
   /*                 |   */
   /*                 3   , where 2-->3 perpendicular to 0--1--2 */
   
   /* cross:  (2->3)X(2->1) ==> (2->4), aXb ==> d at dihedral 90*/
   crossproduct( (double)(fx3-fx2),(double)(fy3-fy2),(double)(fz3-fz2)
                ,(double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1));
   cmag = (float)sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) );
   /*normalize, scale, and place crt point #2*/
   if(cmag < 0.000001) return; /*can't do arrow*/
   fx4 = (float)dblx*radius/cmag;
   fy4 = (float)dbly*radius/cmag;
   fz4 = (float)dblz*radius/cmag;

   fx4 = fx2 + fx4;
   fy4 = fy2 + fy4;
   fz4 = fz2 + fz4;

   /*rotate around 2->4 for correct angle*/
   doaxisrot( &fx3,&fy3,&fz3
             ,angle - 90
             ,(double)x2,(double)y2,(double)z2
             ,(double)(fx4),(double)(fy4),(double)(fz4));
   /*NOW 2-->c is one of the arrow spines at dihedral of 90 */
   /* 0                 4 , where 2-->4 perpendicular to 1--2--3 */
   /* |                /  */
   /* 1-------------->2   */
   /*                /    */
   /*               3     , where 2-->3 at arrow angle to 1-->2 */

   /* so can output this spine*/
   /*output the first arrow spine vector*/
   linetoscreen(x2,(int)fx3,y2,(int)fy3);  
    
   /*then calculate the 3 other spines*/
   for(j=1; j<=3; j++)
   {/*do 3 more arrow spines*/
      /*rotate around 1-->2 for correct dihedral*/
      doaxisrot( &fx3,&fy3,&fz3
                ,90
                ,(double)x1,(double)y1,(double)z1
                ,(double)x2,(double)y2,(double)z2);
      /*output the arrow spine vector*/
      linetoscreen(x2,(int)fx3,y2,(int)fy3);
   }/*do 3 more arrow spines*/
}
/*___arrowtoscreen()_________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****suppressthisgroup()******************************************************/
void suppressthisgroup(grupstruct* victimgrupptr)
{
   grupstruct* previousgroupptr; 
   sgrpstruct* thissgrpptr; 

   /*cannot anneal first group to a previous group*/
   if(victimgrupptr != firstgrupptr)
   {
      previousgroupptr = victimgrupptr->previousptr;
      previousgroupptr->nextptr = victimgrupptr->nextptr;
      thissgrpptr = victimgrupptr->firstsgrpptr;
      thissgrpptr->previousptr = previousgroupptr->lastsgrpptr;
      previousgroupptr->lastsgrpptr->nextptr = thissgrpptr; /*redundant??*/
      while(thissgrpptr != NULL)
      {
         thissgrpptr->grupptr = previousgroupptr;

         if(thissgrpptr == victimgrupptr->lastsgrpptr) thissgrpptr=NULL;
         else thissgrpptr = thissgrpptr->nextptr;           
      }      
      previousgroupptr->lastsgrpptr = victimgrupptr->lastsgrpptr;
      if(victimgrupptr->button != NULL) destroybuttons(victimgrupptr->button);
      free(victimgrupptr);
      victimgrupptr = NULL;
   }
}
/*___suppressthisgroup()_____________________________________________________*/

/****appendscreentext()*******************************************************/
void appendscreentext()
{
   char    ptIDstr[256];
   int     Lfile=0, n=0;


   /*append screen oriented word list onto last appended group*/
   /*thisgrupptr = lastappendgrupptr;*/
   thisgrupptr = lastgrupptr;
   thissgrpptr = thisgrupptr->lastsgrpptr;
   thislistptr = NULL; /*to be honest*/
   Listtype = WORDS;  /*MAGEFLAG.H*/
   Liststyle = 0;

   Lnew = 1;
   /*getlist(c,Lnew);*/ /*MAGESETS*/
   getnewlist(); /*MAGESETS*/
   Lnew = 0; /* reset for later calls */



   if(thislistptr != NULL)
   {/*list was allocated*/
      thislistptr->STATUS = thislistptr->STATUS | STATICFLAG;
      thislistptr->STATUS = thislistptr->STATUS | SCREENFLAG;
      /*special display flag, screen== no rotation and fits on screen*/
      thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
      sprintf(thislistptr->name,"score"); /*MAXNAMECHAR == 14*/
      thislistptr->on = 1;

      /*Need a point to hold the text*/
      if( (thispointptr = allocpointstructure(thislistptr))!=NULL)/*MAGELIST*/
      {/*actually can add another vector */
         thispointptr->type= WORDS;
         thispointptr->STATUS = (thispointptr->STATUS | STATICFLAG);
         thispointptr->STATUS = (thispointptr->STATUS | SCREENFLAG);
         sprintf(ptIDstr,"score");
         storeptIDstring(ptIDstr, thispointptr); /*MAGELIST*/
         thispointptr->fx = -180;
         thispointptr->fy =  180;
         thispointptr->fz =    0;
         /*bring in the text, character by character, from the pipe*/
         iword = 0;
         
         fpin = fpappend; /*reset at end of this routine*/ 
         Lfile = 1;
         while (Lfile) 
         {/*loop-while-file-not-at-EOF*/
#ifdef UNIX_X11
            if(Lpipeactive)
            {/*get characters out of Mage local pipebuffer*/
               c = getfrompipebuffer();
            }
            else
#endif 
#ifdef PCWINDOWS
            if (Lupdate)
            {
               c = winHandleGetc(readpipe);
            }
            else 
#endif 
            {/*regular input routines*/
               if(Linternal)  /*000506*/
               {
                   c = getinternalkinemage(Ninternal); /*MAGEHELP.c*/
               }
               else
               {/*regular flow from external file*/
                   c = getnextchar(); /* defined as getc(fpin)*/
               }
            }/*regular input routines*/
            if(c==EOF)
            {
                Lfile = 0;
            }
            else
            { /*store character in word*/
                word[iword++] = c;
            }
         }/*loop-while-file-not-at-EOF*/
         word[iword] = '\0'; /*iword incremented after each char added*/
         commentstr[0]=thislistptr->wordsfont; /*font*/
         commentstr[1]=thislistptr->wordsface; /*face*/
         commentstr[2]=thislistptr->wordssize; /*size*/
         for(n=0; n<=iword; n++)
         {
             commentstr[n+3] = word[n];
         }
         storecommentstring(commentstr, thispointptr); /*MAGELIST*/
         rescalekinemage();
         fpin = fp; /*always reset from Lappend case to be sure */
      }/*actually can add another vector */
   }/*list was allocated*/
   /*close the text and return to the update calling routine*/

}
/*___appendscreentext()______________________________________________________*/

/****replacecaptiontext()*****************************************************/
void replacecaptiontext()
{
   char capts[256];
   int  icapt=0;
   int  Lfile=0;
   
   capts[0] = '\0';
   clearcapts();
   
   fpin = fpappend; /*reset at end of this routine*/ 
   Lfile = 1;
   while (Lfile) 
   {/*loop-while-file-not-at-EOF*/
#ifdef UNIX_X11
      if(Lpipeactive)
      {/*get characters out of Mage local pipebuffer*/
          c = getfrompipebuffer();
      }
      else
#endif 
#ifdef PCWINDOWS
      if(Lupdate)
      {
          c = winHandleGetc(readpipe);
      }
      else 
#endif 
      {/*regular input routines*/
         if(Linternal)  /*000506*/
         {
             c = getinternalkinemage(Ninternal); /*MAGEHELP.c*/
         }
         else
         {/*regular flow from external file*/
             c = getnextchar(); /* defined as getc(fpin)*/
         }
      }/*regular input routines*/
      if(c==EOF)
      {
          Lfile = 0;
      }
      else
      {
          capts[icapt] = (char)c;
          if (icapt >= 253 || c == '\r' || c == '\n')
          { 
              capts[icapt+1] = '\0';/*make array of characters*/
                                    /*into properly ended C string*/
              insertcapts(capts); /*MACWIND.C*/ /*MPCWIND.C*/
              icapt = 0;
          }
          else
          {
              icapt++;
          }
      }
   }/*loop-while-file-not-at-EOF*/
   docompletecapt();

   rescalekinemage();
   fpin = fp; /*always reset from Lappend case to be sure */
}
/*___replacecaptiontext()____________________________________________________*/

#ifdef TOKENIZER
/****tokenizeline()***********************************************************/
void tokenizeline(char cmdline[256]) /* 030316*/
{/*void dissectcommandline(char cmdline[256]) from PKININIT.c 030316*/
   
    int    i=0,argc=0,whitespace=0,inquotes=0,j=0,k=0;
    char   tempstr[256];
    char*  argv[33]; /* 990331 - used for commandline parsing */
    
    /*dissect the cmdline*/
    argc = 2; /*C commandline 1st argument is at 0 for program name */
    j = 0; /*character counter within an argument starts at 0 */
    whitespace = 0; /*state indicator*/
    inquotes = 0; /*quoted blocks are taken as is*/
    for(i=0; i<256; i++)
    {/*scan in whole commandline*/
       if(inquotes==0 && (cmdline[i]==' '||cmdline[i]==','||cmdline[i]=='\0'))
       {
           if(i==0 || whitespace == 1) /*successive whitespace, NOP*/
           {
               whitespace = 1; /*set or reset*/
           }
           else
           {
               whitespace = 1; /*ends a non-whitespace field*/
               tempstr[j] = '\0'; /*end this argument string*/
               argv[argc-1] = (char *)malloc(sizeof(char)*(j+1));
               for(k=0; k<=j; k++)
               {
                   argv[argc-1][k] = tempstr[k];
               }
               j = 0; /*reset argument character counter*/
               argc++; /*set for next argument, later reset -- for last one*/
           }
       }
       else if(cmdline[i]=='"')
       {
           if(inquotes==0)
           {
               inquotes = 1;
           }
           else
           {
               inquotes = 0; /*next whitespace will close this arg*/
           }            
       }
       else
       {
           whitespace = 0;
           tempstr[j] = cmdline[i]; /*put char into argument string*/
           j++;
       }
       if(cmdline[i] == '\0') /*reached end of cmdline*/
       {
           break; 
       }
    }/*scan in whole commandline*/
    argc--; /*was set for another argument, reset for the last one*/

  /*for(i=1; i<argc; i++) {fprintf(stderr,"%s\n",argv[i]);}*/

    if(argc>1)
    {
        parsecommandline(&argc, argv);
        /*parsecommandline will set Lnewfile=1 if it finds an input file name*/
        /*specific main loops now required to handle routing inc. Lcommanded*/
        /*if(Lcommanded != 0)*/
        /*{*/
        /*   Lnewpass = 0;*/ /*no need for user interactive choices*/
        /*   Lnewpassflow = 1;*/ /*enter program flow to make kinemage*/
        /*}*/
    }
    if(argc>=1)
    {/*free fake argv allocated char strings*/
       for(i=1; i<argc; i++) /*argc==0 not used, implied for program name*/
       {/*loop over arguments*/ /*free(argv[argc-1]);*/
            free(argv[i]);
       }/*loop over arguments*/
    }/*free fake argv allocated char strings*/
}
/*___tokenizeline()__________________________________________________________*/

#endif /* TOKENIZER*/

#ifdef FROMPREKIN
/****dissectcommandline()*****************************************************/
void dissectcommandline(char cmdline[256])
{
    int             i,argc,whitespace,inquotes,j,k;
    char  tempstr[256];
    char*  argv[33]; /* 990331 - used for commandline parsing */
    
    /*dissect the cmdline*/
    argc = 2; /*C commandline 1st argument is at 0 for program name */
    j = 0; /*character counter within an argument starts at 0 */
    whitespace = 0; /*state indicator*/
    inquotes = 0; /*quoted blocks are taken as is*/
    for(i=0; i<256; i++)
    {/*scan in whole commandline*/
       if(inquotes==0 && (cmdline[i]==' '||cmdline[i]==','||cmdline[i]=='\0'))
       {
           if(i==0 || whitespace == 1) /*successive whitespace, NOP*/
           {
               whitespace = 1; /*set or reset*/
           }
           else
           {
               whitespace = 1; /*ends a non-whitespace field*/
               tempstr[j] = '\0'; /*end this argument string*/
               argv[argc-1] = (char *)malloc(sizeof(char)*(j+1));
               for(k=0; k<=j; k++)
               {
                   argv[argc-1][k] = tempstr[k];
               }
               j = 0; /*reset argument character counter*/
               argc++; /*set for next argument, later reset -- for last one*/
           }
       }
       else if(cmdline[i]=='"')
       {
           if(inquotes==0)
           {
               inquotes = 1;
           }
           else
           {
               inquotes = 0; /*next whitespace will close this arg*/
           }            
       }
       else
       {
           whitespace = 0;
           tempstr[j] = cmdline[i]; /*put char into argument string*/
           j++;
       }
       if(cmdline[i] == '\0') /*reached end of cmdline*/
       {
           break; 
       }
    }/*scan in whole commandline*/
    argc--; /*was set for another argument, reset for the last one*/

/*#define COMMANDLINEREPORT*/
#ifdef COMMANDLINEREPORT
    alertstr[0] = '\0'; /*Null initial alertstr*/
    for(i=0; i<argc; i++)
    {
       sprintf(alertstr,"%s %s",alertstr,argv[i]);
       DoReportDialog();
    }
#endif /*COMMANDLINEREPORT*/

    if(argc>1)
    {
        parsecommandline(&argc, argv);
        /*parsecommandline will set Lnewfile=1 if it finds an input file name*/
        /*specific main loops now required to handle routing inc. Lcommanded*/
        /*if(Lcommanded != 0)*/
        /*{*/
        /*   Lnewpass = 0;*/ /*no need for user interactive choices*/
        /*   Lnewpassflow = 1;*/ /*enter program flow to make kinemage*/
        /*}*/
    }
    if(argc>=1)
    {/*free fake argv allocated char strings*/
       for(i=1; i<argc; i++)
       {/*loop over arguments*/
          /*free(argv[argc-1]);*/
            free(argv[i]);
       }/*loop over arguments*/
    }/*free fake argv allocated char strings*/
}
/*___dissectcommandline()____________________________________________________*/
#endif /*FROMPREKIN*/

