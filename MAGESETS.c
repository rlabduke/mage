/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGESETS == MAGEGRUP + MAGESGRP + MAGEVECT + MAGEVSUB  */
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEFLAG.h"  /*960808*/
#include "MAGEANGL.h"  /*970918 for Lbondrotationsetup */
#include "MAGECLON.h"  /*clone rather than instance*/
#include "MAGETABL.h"  /*for Ltablepresent flag*/
#include "MAGEBALL.h"  /*100406*/
#include <string.h>
 
#define MAXIPMS 32
int     Lcolnam,icolor,nblev,Lbrot,Lbang,Lbplot;/*970918*/
int     Lbnoslider; /*070617*/
int     Lbselect; /*071215*/
int     Lwordsfont,Lwordsface,Lwordssize;
int     Lradius,Lballhere,Lballthere,Lcomment,LBallflag,Lmove;
int     Lstartpt; /*060523 for X start of independent triangle*/
int     Lwidth;
char    ptIDstr[256];
float   pointradius; /*980804*/
int     pointnumber; /*071005 for atom ordering of suitefit alternate bases*/
int     Leyeposition; /*990821*/
int     Lscaledvalue; /*130309*/
int     Lmoview;
int     Ldimension; /*060121  group,subgroup,list parameter transient flag*/
/*int     Ndimname; 060622*/   /*060129 group param # of dimension name */
int     Lwrap; /*060123  group,subgroup,list parameter transient flag*/
int     LwrapNumber; /*060121,23  parameter flag, only used during input*/
float   Number; /*060121  parameter value, only used during input*/
int     Ltriangle; /*991128*/
int     Larrow; /*010311*/
int     Lsquare; /*010311*/
int     Lring; /*010311*/
int     Lsphere; /*010311*/
int     Lballpoint; /*030425*/
int     Ldummy; /*991203*/
int     Lghost; /*001009*/
int     Lnoxyz; /*000327*/
int     Lpointcomplete; /*000327*/
int     Lpointabandon; /*061210*/
int     Lexclamation; /*991210*/
static char pointmasterstr[MAXIPMS],parenstr[256];
static int  IONpointmaster=0; /*030425*/
static unsigned long long MASKpointmaster=0; /*030528 int, 081120 long long*/
static int  iparen; /*loaded in getlist, used in addanothervector*/

int     ipms=0;
int      Langle = 0; /*000804*/
int      Lresname=0; /*071005*/
int      Lresnumb=0; /*071005*/
/*NOTE: global grup, sgrp, list ptr's are set == NULL in MAGEINIT */
/*mageGrup.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getgroup(c,Lnew)*********************************************/
void    getgroup(int c,int Lnew)
{
/*need some of these static since getgroup() called repeatedly during input*/
enum    { idenID = 1, numbID = 2, skipID = 3 };
struct grupstruct* testgrupptr;

static    int     Lfirst,gotinstance;
static    int     flowID,n;
static    char    instancename[MAXNAMECHAR+2],ana1name[MAXNAMECHAR+2];
static    char    ana2name[MAXNAMECHAR+2]; /*allows animation * or % */
  
  if(Lnew)   /* initialize */
  {/*Lnew*/
        getnewgroup();
        Lfirst = 1;
        iword = 0;
        Linstance = 0;
        Lclone = 0;
        Ldimension = 0; /*060121*/
        Lwrap = 0; /*060121*/
        LwrapNumber = 0; /*060121*/
        Lmaster = 0;
        Lidn = 0;
        Leyeposition = 0;
        Lscaledvalue = 0; /*130309*/
        
        flowID = skipID;
  }/*Lnew*/
  if(thisgrupptr != NULL)
  {/*this group is allocated*/
    /* test character and set flow controls  */
    /* screen for button identifier */
    if(c == '{')  /* start a button identifier */
    {
        Lidn = 0; /* Lidn only set = 1 after next identifier char read */
        iword = 0;
        flowID = idenID;
    }
    else if(c == '}')  /* end a button identifier, require both {...} ??*/
    {/*found right bracket*/
        for(n=iword ; n<MAXNAMECHAR ; n++) word[n] = '\0';
          /*revision930207 pad out identifier with '\0' rather than ' '*/
        word[MAXNAMECHAR] = '\0';  /* pad & end the string in c manner */
        if(Linstance || Lclone)
        {/*instance: should hold identifier of a previously defined group */
            /* group named, copy it to instancename */ 
            for(n=0; n<=MAXNAMECHAR ; n++) /*inc ending '\0' */
            {    
                instancename[n+1] = word[n];
                ana1name[n+1] = word[n]; 
                ana2name[n+1] = word[n];
            }
            instancename[0] = ' ';
            ana1name[0] = '*';
            ana2name[0] = '%';
            /*condition(instancename);*/ 
                      /*MACMAIN.C,dummied in other ___MAIN.C*/
            /*condition(ana1name);*/  
            /*condition(ana2name);*/ 
            gotinstance = 0; /*flag*/               
            testgrupptr = firstgrupptr;
            while(testgrupptr != NULL)
            {
             if(  (strncmp(instancename,testgrupptr->name,MAXNAMECHAR+1)==0)
                ||(strncmp(ana1name,testgrupptr->name,MAXNAMECHAR+1)==0)
                ||(strncmp(ana2name,testgrupptr->name,MAXNAMECHAR+1)==0)   )
                {
                    thisgrupptr->on = 1;/*do not inherit on-off! start enabled*/
                    thisgrupptr->type = testgrupptr->type; /*971109*/
                    thisgrupptr->firstsgrpptr = testgrupptr->firstsgrpptr;
                        /* starting subgroup of this group */
                    thisgrupptr->lastsgrpptr = testgrupptr->lastsgrpptr;
                        /* ending subgroup of this group */
                        /* animate flag of this new group has to be explicit*/
                        /* animate flag character for this group is blank*/
                    thisgrupptr->STATUS = testgrupptr->STATUS;
                        /*dominance, etc. of this group set to be same as orig*/
                    if(thisgrupptr->master == 0) 
                    {/*no master set yet for new grp*/
                       thisgrupptr->master = testgrupptr->master;
                        /* master button flag for this group set as old one*/
                        /*this would be overridden by later new master */
                    }
                    thisgrupptr->bondrot = testgrupptr->bondrot;
                        /* bondrot flag for this group set as old one*/
                    thisgrupptr->instanceptr = testgrupptr; 
                              /*identity of original group*/ 
                    thisgrupptr->moview = testgrupptr->moview;
                        /* moview flag of this group set as old one*/
                    gotinstance = 1;
                    break; /*out of while... loop*/
                }
                testgrupptr = testgrupptr->nextptr;
                if(thisgrupptr == testgrupptr) testgrupptr = NULL;/*avoid self*/
            }
            if(!gotinstance)
            {
              if(Linstance)
                sprintf(alertstr,
                        "instance failed, no previous group has name: %s"
                        ,instancename);
              if(Lclone)
                sprintf(alertstr,
                        "cloning failed, no previous group has name: %s"
                        ,instancename);
                
                errorreport(); /*alertstr and word*/ /*MAGESETS.c*/
            }
            else if(Lclone)
            {/*clone all members of this group*/
                thisgrupptr->instanceptr = NULL;/*reset, NOT an instance*/
                clonegroupsubgroups(testgrupptr);  /*MAGECLON*/
            }
            gotinstance = 0;
            Linstance = 0;
            Lclone = 0;
        }/*instance: should hold identifier of a previously defined group */
        else if(Lmaster)
        {/*should hold identifier of a master button for this group */

            masterfinder( &(thisgrupptr->master) );
            Lmaster = 0;
        }/*should hold identifier of a master button for this group */
        else
        {/*should hold identifier of this group */
            /*group name: copy it to groupname */
            for(n=0; n<=MAXNAMECHAR; n++)/*just copy it all, word[iword]='\0'*/
            {  
                thisgrupptr->name[n+1] = word[n];/*group name starts at char 1*/
            }
            /*groupname was blanked earlier, don't mess with [0] so an*/
            /*animate before groupname could leave its * in the [0]th place*/
            thisgrupptr->name[MAXNAMECHAR+1] = '\0' ; /*insurance*/
                /* array has MAXNAMECHAR+2 chars indexed: 0,...,MAXNAMECHAR+1 */
                /*truncate to MAXNAMECHAR chars, [0] reserved for animate flag*/
                /* array is defined as MAXNAMECHAR+2 to allow for flag and \0 */
            if(Ltest)
            {
                 sprintf(alertstr,"debug test for groupname");
                 errorreport(); /*alertstr and word*/ 
            }
            /*don't condition groupname yet, wait for possible animate flags */
        }/*should hold identifier of this group */

        iword = 0;
        Lidn = 0;
        flowID = skipID;
    }/*found right bracket*/

    /* now either in a comment in which any other char accepted */
    /* or must check the char */
    if( !Lidn && (flowID != idenID) )
    {
        if(Lfirst)  
        /* first line with @group can have {name} & animate control */
        /* also off control */
        {/*first line*/
            if(  c == '=')
            {/*special treatment for '=': treat as whitespace*/
                c = ' '; /*make it a white space*/
                /*NOTE: isolated = is thus ignored and changes no states*/
            }
            if(  c == ' '||c == ','||c == ';'||c == '\t'
 /*'{'balance*/  ||c == '\015'||c == '\012'||c == EOL || c == '}') 
                    /* white space */
            {
                if(iword > 0)  /* after a word */
                {  /*------- GROUP PARAMETER --------*/
                    word[iword] = '\0';  /* end string in c manner */
                    if(Lmoview)
                    {/*expecting an integer from 1 - 60*/
                         sscanf(word,"%d",&n);
                         if(n < 1 || n > 60) n = 1;
                         thisgrupptr->moview = n;
                         Lmoview = 0;
                    }
                    else if(Ldimension) /*060121, all sets 060622*/
                    {
                       sscanf(word,"%d",&n);
                       thisgrupptr->NDIM = n;
                       NDIM = thisgrupptr->NDIM; /*set global with latest NDIM*/
                       Ldimension = 0;
                       thisgrupptr->KKIND = thisgrupptr->KKIND | DIMENSIONKKIND;
                       LNdimensions = 1; /*060622*/
                       Lonscreeninfo = 1;
                       Lonscreeninfoon = 1;
                       /*NX,NY,NZ, onscreeninfostr  MAGEINIT/mageintialvalues*/
                       /*dimensionDialog sets defaults if names not inputed*/
                       defaultdimnames(); /*MAGEDLOG 060622*/
                    }
                    else if(Lwrap) /*060121, all sets 060622*/
                    {
                       sscanf(word,"%f",&Number);
                       LwrapNumber = 1;
                       Lwrap = 0;
                       thisgrupptr->fwrap = Number; /*060525*/
                    }
                    else if(Leyeposition)
                    {
                         sscanf(word,"%d",&n);
                         thisgrupptr->info = n;
                         Leyeposition = 0;
                    }
                    else if(Lscaledvalue) /*130309*/
                    {
                         sscanf(word,"%f",&Number);
                         thisgrupptr->scaled = Number; /*130309*/
                         Lscaledvalue = 0;
                    }
                    else
                    {
/*.*/                    interpretgroupkeyword(word,iword);
                    }
                    iword = 0;
                }
            }
            else  /* accummulate character in word */
            {
/*c*/            c = tolower(c);  /*make lowercase so case independent */
                word[iword] = c;
                iword = iword + 1;
            }
            if(c == EOL)   /*950113*/
            {
              Lfirst = 0;  /* ends first line */
              /*condition(thisgrupptr->name);*/  /*____MAIN*/
              if(LNdimnames)   /*060129 EOL reset axes names*/
              {
                strcpy(NXstr,dimensionptr[0].name); /*060622 7 to s etc*/
                strcpy(NYstr,dimensionptr[1].name);
                strcpy(NZstr,dimensionptr[2].name);
                /*resetrot() calls sprintf(onscreeninfostr,...) */
              }
            }
        }/*first line*/
        else 
        {
            flowID = skipID;  
            /* if not first line, skip until upper routine finds an @ */
        }
    }

    /* Now control by flowID cases */
    /*begin-of-switch(flowID)*/                        
    switch(flowID)
    {
        case idenID:
            if(Lidn)
            {
                if(iword >= MAXNAMECHAR) 
                {
                    word[iword] = '\0';
                    {
                        sprintf(alertstr,
                           "next char: %c, exceeds %d char name limit"
                           "\015expect truncated group {Name}"
                           ,c,MAXNAMECHAR);
                        errorreport(); /*alertstr and word*/ 
                        flowID = skipID;  /*ignore char until end of name*/
                    }
                }
                else
                {
                    word[iword] = c;
                    iword = iword + 1;
                }
            }
            else
            {
                Lidn = 1; /* get ready for the actual comment characters*/                               

            }
        break;
                            
        case skipID:
        break;
    }
    /*end-of-switch(flowID)*/
  }/*this group is allocated*/
}
/*___getgroup(c,Lnew)____________________________________________________*/

/****interpretgroupkeyword()*************************************************/
void    interpretgroupkeyword(char keyns[256],int ikeyn)
{
    /*NOTE  char =  acts as white space and can end these parameters*/
    /*so only need to test for parent word, not varient ending with = */

         ikeyn = ikeyn; /*to avoid warning of no use */

    if(strncmp(keyns,"off",3) == 0) thisgrupptr->on = 0;
                                /* starts with display disabled */ 
    else if(strncmp(keyns,"-",1  ) == 0) thisgrupptr->on = 0; /* historic */
    else if(   (strncmp(keyns,"*",1  ) == 0) 
       || (strncmp(keyns,"animat",6) == 0) )
    {
         thisgrupptr->animate = 1;  /* flag for animation group */
            
         if(thisgrupptr->name[0] != '*')  nanimate++;    
                        /* double flags, eg "* animate" only count once */
            
         thisgrupptr->name[0] = '*';  /* otherwise char[0] left blank */

         if(nanimate > 1 ) thisgrupptr->on = 0; 
                        /*only first animated group starts visible*/
    }
    else if( (strncmp(keyns,"2animat",7) == 0) )
    {
         thisgrupptr->animate = 2;  /* flag for animation group */
         nnanimate++;    
         thisgrupptr->name[0] = '%';  /* otherwise char[0] left blank */
              /*revision930206 %2animate: PC menu '&' does next char underline*/
         if(nanimate > 1 || nnanimate > 1 ) thisgrupptr->on = 0; 
                        /*only first animated group starts visible*/
    }
    else if( (strncmp(keyns,"dominant",8) == 0) )
    {
       thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT;
       /* dominant group: hides all subgroups and lists below itself */
    }
    else if( (strncmp(keyns,"nobutton",8) == 0) )
    {
       thisgrupptr->STATUS = thisgrupptr->STATUS | NOBUTTON;
    }
    else if(  (strncmp(keyns,"recessiveon",11)== 0)
            ||(strncmp(keyns,"collapsable",11)== 0)
            ||(strncmp(keyns,"collapsible",11)== 0) ) /*040320*/
    {
       thisgrupptr->STATUS = thisgrupptr->STATUS | RECESSIVEON;
    }
    else if(strncmp(keyns,"static",6) == 0) /*121211*/
    {
        thisgrupptr->STATUS = thisgrupptr->STATUS | STATICFLAG;
        /*special display flag, static== no rotation, but scaled*/
    }
    else if(strncmp(keyns,"screen",6) == 0) /*121211*/
    {
        thisgrupptr->STATUS = thisgrupptr->STATUS | STATICFLAG;
        thisgrupptr->STATUS = thisgrupptr->STATUS | SCREENFLAG;
        /*special display flag, screen== no rotation and fits on screen*/
        LSCREENpoints = 1; /*existence of such points 121212*/
    }
    else if(strncmp(keyns,"master" ,6) == 0) Lmaster = TRUE;
    else if(strncmp(keyns,"instance" ,8) == 0) Linstance = TRUE;
    else if(strncmp(word,"clone",5) == 0) Lclone = 1;
    else if(strncmp(keyns,"moview" ,6) == 0) Lmoview = TRUE;
    else if(strncmp(keyns,"dimension" ,9) == 0) Ldimension = 1; /*060121*/
    else if(  (strncmp(keyns,"wrap" ,4) == 0) 
            ||(strncmp(keyns,"add" ,3) == 0) )
    {
       Lwrap = 1; 
       /*060123, was add 060121*/ /*060123 use wrap instead*/
       thisgrupptr->KKIND = thisgrupptr->KKIND | WRAPPINGKKIND; /*060525*/
       /*following number stored as thisgrupptr->fwrap */
    }
    else if(strncmp(keyns,"select" ,6) == 0) /*060121*/
    {
       thisgrupptr->KKIND = thisgrupptr->KKIND | SELECTINGKKIND;
       LSELECTINGlist = 1; /*global potentiates select-points-in-group*/
       LUnSELECTING = 1; /*global potentiates toggle-Unselected-points*/
       LoneSELECTING = 1; /*global potentiates select just one point*/
    }
    else if(strncmp(keyns,"centered" ,8) == 0) /*130115 group to keep centered*/
    {
       thisgrupptr->KKIND = thisgrupptr->KKIND | CENTEREDKKIND;
       Lcentered = 1; /*global potentiates keeping this group centered*/
    }
    else if(strncmp(keyns,"scaled" ,6) == 0) /*130309 scaled group*/
    {
       thisgrupptr->KKIND = thisgrupptr->KKIND | SCALEDKKIND; /*130309*/
       Lscaledvalue = 1; /*global potentiates reading next field as a value*/
    }
    
    else if(strncmp(word,"lens",4) == 0) /*971109*/
    {
        thisgrupptr->type = thisgrupptr->type | LENSFLAG;
        /*subgroups and lists inherit type*/
        /*special display flag, list points only show in center lens */
        LLensflagset = 1;  /*971109*/
    }
    else if(strncmp(keyns,"float" ,5) == 0)
    {/*for this group, use floating point options in MAGEDRAW inner loop*/
        thisgrupptr->type = thisgrupptr->type | FLOATFLAG;
        Leyeposition = 1;
    }
    else if(strncmp(keyns,"table" ,5) == 0)
    {/*for this group, put information only into a table, not into graphics*/
        thisgrupptr->type = thisgrupptr->type | TABLEFLAG; 
           /*NOT shown in MAGEDRAW.c/drawvec() unless Ltablegroupgraphed==1*/
        Ltablepresent = 2; /*later reduced to 1 when a table window is created*/
        /*group can only set Ltablepresent, so non-table group can't unset it*/
    }
    else if(strncmp(keyns,"header" ,6) == 0)
    {/*for this group, put information only into a table, not into graphics*/
        thisgrupptr->type = thisgrupptr->type | TABLECOLHEAD; 
    }

}
/*___interpretgroupkeyword()_________________________________________________*/

/****getnewgroup()************************************************************/
void getnewgroup()
{
      if( !allocgrupstructure() )  /*MAGELIST.c*/
      {/*allocation failed*/
       sprintf(alertstr,"failed to alloc a group of vectors, labels, or dots!");
             sprintf(word," ");
             errorreport(); /*alertstr and word*/ 
        thisgrupptr = NULL;
      }/*allocation failed*/
      else /*thisgrupptr is now allocated*/
      {/*initialize newly allocated group*/

        thisgrupptr->on = 1;    /* start with display enabled */
        thisgrupptr->type = 0; /*971109*/
        /*assignment of sgrpptr must wait until a sgrp is allocated*/ /*971121*/
        thisgrupptr->firstsgrpptr = NULL;/* starting subgroup of this group*/
        thisgrupptr->lastsgrpptr =  NULL;/*ending subgroup of this group */
        /*the subgroup routine updates the last sgrp ptr within the group*/
        
        thisgrupptr->animate = 0;  /* initialize as non-animated group, */
                            /* flag in first line will set animate */
        thisgrupptr->STATUS = 0;    
            /* initialize as recessive; does not hide things below it*/
        thisgrupptr->bondrot = 0; /* initially not flagged for special output*/
        thisgrupptr->instanceptr =NULL;/*presume an original, not an instance*/ 
        thisgrupptr->moview = 0; /* initialize as a regular group, */
                           /* flag in first line will set moview if animate*/
        thisgrupptr->info = 0; /*990821*/
        thisgrupptr->balllists = 0; /*presume no balllists to start*/ 

        memset(thisgrupptr->name,'\0',MAXNAMECHAR+2); /* optimized */
        /* for(n=0;n<=MAXNAMECHAR;n++) thisgrupptr->name[n] = '\0' ; */
           /* clear group name which has MAXNAMECHAR+2 characters*/
        thisgrupptr->name[0] = ' ';/*1st char animate flag or blank*/
        thisgrupptr->master = 0; /*unsigned long long for master button flags*/

      }/*initialize newly allocated group*/
}
/*___getnewgroup()___________________________________________________________*/

/****getnewmaster()***********************************************************/
int getnewmaster()
{
    int ireturn = 0;
     
    if(nmaster+1 > 8*((long)sizeof(long long))) /*081120 8 long (old 8int)*/
    {
         fprintf(stderr,"#master buttons limited to less than %ld in %s\n",
            8*((long)sizeof(long long)),version); /*081120*/ /*121108 long*/
         sprintf(alertstr,"#master buttons limited to less than %ld in %s",
            8*((long)sizeof(long long)),version); /*081120*/ /*121108 long*/
         errorreport(); /*alertstr and word*/ 
    }
    else
    {
        if( !allocmasterstructure() )  /*MAGELIST.c*/
        {/*allocation failed*/
            sprintf(alertstr,"failed to alloc a master structure!");
            sprintf(word," ");
            errorreport(); /*alertstr and word*/ 
            thismasterptr = NULL;
        }/*allocation failed*/
        else /*thismasterptr is now allocated*/
        {/*initialize newly allocated master */
            nmaster++;

           /*HERE BE THE PROBLEM !!!! 081120*/
           /*left shift needs long long to work past 32 */
           
           thismasterptr->mask = 
               (unsigned long long)((unsigned long long)1<<(nmaster-1));
           

/*
fprintf(stderr,"getnewmaster for nmaster== %d sets mask== %llu\n",(int)nmaster,thismasterptr->mask);
*/

            thismasterptr->name[0] = '\0';
            thismasterptr->masterchar = '\0';
            thismasterptr->on = 1; /*ALL MASTERS START ON  030509*/
            ireturn = 1;
        }
    }
    return(ireturn);
}
/*___getnewmaster()__________________________________________________________*/

/****longpower()**************************************************************/
unsigned long long longpower( int base , int n )  /*081120*/
{
   unsigned long long p = 0;
   int  i = 0;

   p = (unsigned long long)1;
   if(n > 0)
   {
      for(i = 1; i <= n; ++i)
      {
         p = p * base;
      }
   }
   return(p);   
}
/*___longpower()_____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****mastermaker()***********************************************************/
int  mastermaker(char masterchar) /*int 051114*/
{/*used for input master assignments*/
   int  masterfound=0,n=0;
   
   /* master named,*/
   masterfound = 0;  /*nmaster==0 when firstmasterptr == NULL*/
   if(firstmasterptr != NULL) /*previous master button requested */ 
   {/*see if this one matches a previous master name */ 
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         if(strncmp(word,thismasterptr->name,MAXNAMECHAR) == 0)
         {
            thismasterptr->masterchar = masterchar;
            masterfound = 1;  
            break;    
         }
         thismasterptr = thismasterptr->nextptr;
      }
   }
   if( nmaster == 0 || masterfound == 0)
   {
      if(getnewmaster()) /*mastermaker()-->MAGESETS*/
      {
         for(n=0; n<=MAXNAMECHAR ; n++)
         {    
            thismasterptr->name[n] = word[n]; 
         }
         thismasterptr->masterchar = masterchar;
         masterfound = 1;
/*
fprintf(stderr,"MAGESETS/mastermaker for nmaster==%d, master={%s}, masterchar=%s, set master mask== %ld\n",nmaster,thismasterptr->name,thismasterptr->masterchar,thismasterptr->mask); 
*/
      }
      else
      {
          masterfound = 0;
         sprintf(alertstr,"getnewmaster failed for name== %s",word); /*051114*/
         errorreport(); /*alertstr and word*/ 
      }
   }       
   Lmaster = 0;
   return(masterfound);
}/*___mastermaker()__________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****masterfinder()**********************************************************/
void masterfinder(unsigned long long* setmask) /*081120 long */
{/*used for group, subgroup, list  masters*/
   unsigned long long  mask = 0; /*081120 long long rather than int*/
   int  masterfound,n;
   
/*
fprintf(stderr,"******MAGESETS/masterfinder: enter: nmaster==%d, set mask %ld\n",nmaster,*setmask);
*/
   /* master named,*/
   masterfound = 0;  /*nmaster==0 when firstmasterptr == NULL*/
   if(firstmasterptr != NULL) /*previous master button requested */ 
   {/*see if this one matches a previous master name */ 
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         if(strncmp(word,thismasterptr->name,MAXNAMECHAR) == 0)
         {
            mask = thismasterptr->mask; /*mask = 1<<(j-1);*/
            *setmask = mask | *setmask; /*add to any others*/
            masterfound = 1;      
         }
         thismasterptr = thismasterptr->nextptr;
      }
   }
   if( nmaster == 0 || masterfound == 0)
   {
      if(getnewmaster()) /*masterfinder()-->MAGESETS*/
      {
         for(n=0; n<=MAXNAMECHAR ; n++)
         {    
            thismasterptr->name[n] = word[n]; 
         }
         mask = thismasterptr->mask; /*mask = 1<<(nmaster-1);*/
         *setmask = mask | *setmask; /*add to any others*/
/*
fprintf(stderr,"***MAGESETS/masterfinder: nmaster-1== %d, mask %ld\n",nmaster-1,mask);
*/
      } 

      /*failure just fails to register this master with this grup/sgrp/list */
   }                
   Lmaster = 0;
/*
fprintf(stderr,"MAGESETS/masterfinder: set mask now== %ld\n",*setmask);
*/
}/*___masterfinder()_________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****masterconfirmer()********************************************************/
int  masterconfirmer(char masterchar) /*030425 rtn ON/OFF pointmaster STATUS*/
{/*master by masterchar code for pointmasters*/
   int  masterfound=0, ireturn=0;
   
   masterfound = 0;  /*nmaster==0 when firstmasterptr == NULL*/
   if(firstmasterptr != NULL) /*previous master button requested */ 
   {/*see if this one matches a previous master name */ 
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         if(masterchar == thismasterptr->masterchar)
         {
            ireturn = thismasterptr->on; /*point masters on/off at input*/
            /*all masters can be ON or OFF at input   030509*/
            masterfound = 1; 
            break;     
         }
         thismasterptr = thismasterptr->nextptr;
      }
   }
   if( nmaster == 0 || masterfound == 0)
   {
      if(getnewmaster()) /*masterconfirmer()-->MAGESETS*/
      {/*just use the one char masterchar as master name*/
         thismasterptr->masterchar = masterchar; 
         thismasterptr->name[0] = masterchar; 
         thismasterptr->name[1] = '\0';
         ireturn = thismasterptr->on; /*point masters on when created 040928*/
      }
      else
      {
         ireturn = -1; /* thismasterptr not defined  051128*/
      }
   }
   return(ireturn); /*return ON state (1,0) or -1 for NO master 051128*/
}/*___masterconfirmer()_______________________________________________________*/

/****colorsetfinder()********************************************************/
int colorsetfinder(unsigned int* colorsetnumber)
{       
   int  icolor = 0;
   int  found = 0;
   
   /* colorset named, string held in word, which can be any length*/
   if(firstcolorsetptr != NULL) /*previous colorset requested */ 
   {/*see if this one matches a previous colorset name */ 
                
      thiscolorsetptr = firstcolorsetptr;
      while(thiscolorsetptr != NULL)
      {
         if(strncmp(word,thiscolorsetptr->name,MAXNAMECHAR) == 0)
         {
            *colorsetnumber = thiscolorsetptr->number;
            icolor = thiscolorsetptr->color;
            found = 1;
            break; /*only one colorset per list*/
         }
         thiscolorsetptr = thiscolorsetptr->nextptr;
      }
   }
   if(!found)
   {/*no colorset matches this colorset name, be kind and create one*/
      /*first, save overloaded global word*/
      strcpy(temps,word);
      if(thislistptr->color != 0)
      {/*somehow got a color value for this list, use it */
         interpretcolor(thislistptr->color); /*returns color name in word*/
      }
      else 
      {
         strcpy(word,"white"); /*stupid default color*/
      }
      icolor = getnewcolorset(temps,word); /*defines thiscolorsetptr */
      /*icolor = thiscolorsetptr->color;*/
   }
   Lcolnam = 0;
   return(icolor);
}
/*___colorsetfinder()_______________________________________________________*/

/****getnewcolorset()********************************************************/
int  getnewcolorset(char* name, char* color)
{
   int icolor = 0, n=0;

   if(ncolorset < 32000)
   {/*room for another colorset*/
      /*historically bits used as mask for colorset identity, */
      /*now use simple number instead of mask since only one per list 031227*/
      if(!alloccolorsetstructure())
      {/*allocation failed*/
         sprintf(alertstr,"failed to alloc a colorset!");
         sprintf(word," ");
         errorreport(); /*alertstr and word*/ /*MAGESETS.c*/
         thiscolorsetptr = NULL;
      }/*allocation failed*/
      else
      {
         ncolorset++;
         /*thiscolorsetptr->mask = 1<<(ncolorset-1);*/
         thiscolorsetptr->number = ncolorset;
         for(n=0; n<MAXNAMECHAR ; n++)
         {/*index from 0 to MAXNAMECHAR-1, i.e. MAXNAMECHAR char, \0 is ...+1*/ 
            thiscolorsetptr->name[n] = name[n]; /*size: [MAXNAMECHAR+1]*/
         }
         thiscolorsetptr->name[n] = '\0'; /*ending in index MAXNAMECHAR*/
         icolor = numberofcolor((char*)color);
         if(icolor==0)
         {/*failed to decode color name, use default*/
            thiscolorsetptr->color = numberofcolor("default"); /*MAGECOLR.c*/
         }
         else
         {
            thiscolorsetptr->color = icolor; 
         }
      }
   }/*room for another colorset*/
   else
   {
      icolor = numberofcolor("default");
   }
   return(icolor);
}
/*___getnewcolorset()_______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*mageSgrp.c*/

/****getsubgroup()************************************************************/
void    getsubgroup(int c,int Lnew)
{
/*need some of these static since getsubgroup() called repeatedly during input*/
enum    { idenID = 1, numbID = 2, skipID = 3 };
struct sgrpstruct* testsgrpptr;

static    int     Lfirst,gotinstance;
static    int     flowID,n;
static    char    instancename[MAXNAMECHAR+1];

    if(Lnew)  /* initialize */
    {
        if(thisgrupptr == NULL)  
        {/* @set without a preceeding @group, construct first group */
             getnewgroup(); /*MAGESETS*/
             if(thisgrupptr != NULL)
                thisgrupptr->STATUS = thisgrupptr->STATUS | NOBUTTON;    
                /*nobutton and recessive*/
        }
        Lfirst = 1;
        iword = 0;
        Linstance = 0;
        Lclone = 0;
        Ldimension = 0; /*060121*/
        Lwrap = 0; /*060121*/
        LwrapNumber = 0; /*060121*/
        Lmaster = 0;
        Lidn = 0;
        flowID = skipID;
        
        thissgrpptr = NULL; /*so can tell if successfully allocated*/
        if(thisgrupptr != NULL) 
            getnewsubgroup();
    }
  if(thissgrpptr != NULL)
  {/*this subgroup has been allocated*/
    /* test character and set flow controls  */
    /* screen for a button identifier */
    if(c == '{' /*balance }*/)  /* start a button identifier */ 
    {
        Lidn = 0; /* Lidn only set = 1 after next identifier char read */
        iword = 0;
        flowID = idenID;
    }
    else if(/*balance { */ c == '}')  /* end a button identifier */
    {/*closing brace found*/
        for(n=iword ; n<MAXNAMECHAR ; n++) word[n] = '\0';
          /*revision930207 pad out identifier with '\0' rather than ' '*/
        word[MAXNAMECHAR] = '\0';  /* pad & end the string in c manner */
        if(Linstance || Lclone)
        {/*should hold identifier of a previously defined subgroup */
            /* subgroup named, copy it to instancename */ 
            for(n=0; n<=MAXNAMECHAR ; n++)
            {    
                instancename[n] = word[n]; 
            }
            instancename[MAXNAMECHAR] = '\0'; 
            /*array has MAXNAMECHAR+1 values indexed: 0,...,MAXNAMECHAR*/
            /*condition(instancename);*/ /*MACMAIN.C, dummied in MPCMAIN.C*/
            testsgrpptr = firstsgrpptr;
            while(testsgrpptr != NULL)
            {
                if(strncmp(instancename,testsgrpptr->name,MAXNAMECHAR)==0)
                {
                    thissgrpptr->on = 1; /* start with display enabled*/
                    thissgrpptr->type = testsgrpptr->type; /*971109*/
                    thissgrpptr->firstlistptr = testsgrpptr->firstlistptr;
                        /* starting list number of this set */
                    thissgrpptr->lastlistptr = testsgrpptr->lastlistptr;
                        /* ending list number of this set */
                    thissgrpptr->STATUS = testsgrpptr->STATUS;
                        /* dominance, etc.  of this set */
                    if(thissgrpptr->master == 0)
                       thissgrpptr->master = testsgrpptr->master;
                        /* master button flag for this set */
                        /*if not previously defined on this line, set to old*/
                        /*new master can also be set later on this 1st line*/
                    thissgrpptr->bondrot = testsgrpptr->bondrot;
                        /* special output flag of this set */ 
                    thissgrpptr->instanceptr = testsgrpptr; 
                         /*identity of original set*/ 
                    gotinstance = 1;
                    break;
                }
                testsgrpptr = testsgrpptr->nextptr;
                if(thissgrpptr == testsgrpptr) testsgrpptr = NULL;/*avoid self*/            
            }
            if(!gotinstance)
            {
              if(Linstance)
                sprintf(alertstr,
                    "instance failed, no previous subgroup has name: %s"
                    ,instancename);
              if(Lclone)
                sprintf(alertstr,
                    "cloning failed, no previous subgroup has name: %s"
                    ,instancename);
                errorreport(); /*alertstr and word*/ /*MAGESETS.c*/
            }
            if(Lclone)
            {
                thissgrpptr->instanceptr = NULL;/*reset, NOT an instance*/
                clonesubgrouplists(testsgrpptr);
            }
            gotinstance = 0;
            Linstance = 0;
            Lclone = 0;
        }
        else if(Lmaster)
        {/*should hold identifier of a master button for this subgroup */     

            masterfinder( &(thissgrpptr->master) );
            Lmaster = 0;
        }
        else
        {/*should hold identifier of this subgroup */
            /* subgroup name: copy it to subgroupname */
            for(n=0; n<=MAXNAMECHAR ; n++)
            {
                thissgrpptr->name[n] = word[n]; 
            }
            thissgrpptr->name[MAXNAMECHAR] = '\0'; /*for insurance*/
            if(Ltest) /*??????*/
            {
                 sprintf(alertstr,"debug test for subgroupname");
                 errorreport(); /*alertstr and word*/ 
            }
        }
        iword = 0;
        word[0] = ' ';
        word[1] = '\0';
        Lidn = 0;
        flowID = skipID;
    }/*closing brace found*/
    /* now either in an identifier in which any other char accepted*/
    /* or must check the char */
    if( !Lidn && (flowID != idenID) )
    {
        if(Lfirst)  
        /* first line with @subgroup can have {name}, dominant, instance= */
        {/*Lfirst*/
            if(  c == '=')
            {/*special treatment for '=': treat as whitespace*/
               c = ' '; /*make it a white space*/
                 /*NOTE: isolated = is thus ignored and changes no states*/
            }
            if(  c == ' '||c == ','||c == ';'||c == '\t'
/*{ balance*/  ||c == '\015' || c == '\012' || c == EOL || c == '}') 
                            /* white space */
            {
                if(iword > 0)  /* after a word */
                {  /*--------- SUBGROUP PARAMETERS ---------*/
                    word[iword] = '\0';  /* end string in c manner */
                    if(Ldimension) /*060121, all sets 060622*/
                    {
                       sscanf(word,"%d",&n);
                       thissgrpptr->NDIM = n;
                       NDIM = thissgrpptr->NDIM; /*set global with latest NDIM*/
                       Ldimension = 0;
                       thissgrpptr->KKIND = thissgrpptr->KKIND | DIMENSIONKKIND;
                       LNdimensions = 1; /*060622*/
                       Lonscreeninfo = 1;
                       Lonscreeninfoon = 1;
                       /*NX,NY,NZ, onscreeninfostr  MAGEINIT/mageintialvalues*/
                       /*dimensionDialog sets defaults if names not inputed*/
                       defaultdimnames(); /*MAGEDLOG 060622*/
                    }
                    else if(Lwrap) /*060121, all sets 060622*/
                    {
                       sscanf(word,"%f",&Number);
                       LwrapNumber = 1;
                       Lwrap = 0;
                       thissgrpptr->fwrap = Number; /*060525*/
                    }
/*.*/               interpretsubgroupkeyword(word,iword);
                    iword = 0;
                }
            }
            else  /* accummulate character in word */
            {
/*c*/            c = tolower(c);  /*make lowercase so case independent */
                word[iword] = c;
                iword = iword + 1;
            }
            /*if(c == '\015')  */
            if(c == EOL)  
            {
                Lfirst = 0;  /* ends first line */
            }
        }/*Lfirst*/
        else 
        {
            flowID = skipID;  
            /* if not first line, skip until upper routine finds an @ */
        }
    }
    /* Now control by flowID cases */
    /*begin-of-switch(flowID)*/                        
    switch(flowID)
    {
        case idenID:
            if(Lidn)
            {
                if(iword >= MAXNAMECHAR) 
                {
                    word[iword] = '\0';
                    sprintf(alertstr,
                       "next char: %c, exceeds %d name limit"
                       "\015expect truncated subgroup {Name}"
                       ,c,MAXNAMECHAR);
                    errorreport(); /*alertstr and word */ /*MAGESGRP.c*/
                    flowID = skipID;
                }
                else
                {
                    word[iword] = c;
                    iword = iword + 1;
                }
            }
            else
            {
                Lidn = 1; /* get ready for the actual comment characters*/  
            }
        break;
                            
        case skipID:
        break;
    }
    /*end-of-switch(flowID)*/
  }/*this subgroup has been allocated*/
}
/*___getsubgroup()__________________________________________________________*/

/****interpretsubgroupkeyword()**********************************************/
void    interpretsubgroupkeyword(char keyns[256],int ikeyn)
{        
    /*NOTE  char =  acts as white space and can end these parameters*/
    /*so only need to test for parent word, not varient ending with = */
    ikeyn = ikeyn;  /*to avoid warning of not used*/

    if(strncmp(keyns,"off",3) == 0) thissgrpptr->on = 0;
                            /* starts with display disabled */ 
    else if(strncmp(keyns,"-",1  ) == 0) thissgrpptr->on = 0; /* historic */
    else if(   (strncmp(keyns,"*",1  ) == 0) 
            || (strncmp(keyns,"dominant",8) == 0) )
    {
       thissgrpptr->STATUS = thissgrpptr->STATUS | DOMINANT;
    }
    /*0 recessive, 1 dominant, 2 recessive&&nobutton, 3 dominant&&nobutton*/ 
    else if(strncmp(keyns,"nobutton",8) == 0)
    {
       thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
    }
    else if(  (strncmp(keyns,"recessiveon",11)== 0)
            ||(strncmp(keyns,"collapsable",11)== 0)
            ||(strncmp(keyns,"collapsible",11)== 0) ) /*040320*/
    {
       thissgrpptr->STATUS = thissgrpptr->STATUS | RECESSIVEON;
    }
    else if(strncmp(keyns,"static",6) == 0) /*121211*/
    {
        thissgrpptr->STATUS = thissgrpptr->STATUS | STATICFLAG;
        /*special display flag, static== no rotation, but scaled*/
    }
    else if(strncmp(keyns,"screen",6) == 0) /*121211*/
    {
        thissgrpptr->STATUS = thissgrpptr->STATUS | STATICFLAG;
        thissgrpptr->STATUS = thissgrpptr->STATUS | SCREENFLAG;
        /*special display flag, screen== no rotation and fits on screen*/
        LSCREENpoints = 1; /*existence of such points 121212*/
    }
    else if(strncmp(keyns,"instance" ,8) == 0) Linstance = TRUE;
    else if(strncmp(word,"clone",5) == 0) Lclone = 1;
    else if(strncmp(keyns,"master" ,6) == 0) Lmaster = TRUE;
    else if(strncmp(word,"lens",4) == 0) /*971109*/
    {
        thissgrpptr->type = thissgrpptr->type | LENSFLAG;
        /*lists inherit type*/
        /*special display flag, list points only show in center lens */
        LLensflagset = 1;  /*971109*/
    }
    else if(strncmp(keyns,"dimension" ,9) == 0) Ldimension = 1; /*060121*/
    else if(  (strncmp(keyns,"wrap" ,4) == 0) 
            ||(strncmp(keyns,"add" ,3) == 0) )
    {
       Lwrap = 1; 
       /*060123, was add 060121*/ /*060123 use wrap instead*/
       thissgrpptr->KKIND = thissgrpptr->KKIND | WRAPPINGKKIND; /*060525*/
       /*following number stored as thislistptr->fwrap */
    }
    else if(strncmp(keyns,"select" ,6) == 0) /*060121*/
    {
       thissgrpptr->KKIND = thissgrpptr->KKIND | SELECTINGKKIND;
       LSELECTINGlist = 1; /*global potentiates select-points-in-group*/
       LUnSELECTING = 1; /*global potentiates toggle-Unselected-points*/
       LoneSELECTING = 1; /*global potentiates select just one point*/
    }
}
/*___interpretsubgroupkeyword()_____________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getnewsubgroup()*********************************************************/
void getnewsubgroup()
{        
      if( !allocsgrpstructure() )  /*MAGELIST.c*/
      {/*allocation failed*/
             sprintf(alertstr,"failed to alloc a subgroup!");
             sprintf(word," ");
             errorreport(); /*alertstr and word*/ /*MAGESETS.c*/
             thissgrpptr = NULL;
      }/*allocation failed*/
      else /*thissgrpptr is now allocated*/
      {/*initialize newly allocated subgroup*/

        thissgrpptr->on = 1;    /* start with display enabled */

        thissgrpptr->type = 0; /*971109*/
        thissgrpptr->type = thisgrupptr->type; /*inherit*/ /*971109*/

        thissgrpptr->firstlistptr = NULL;/* starting list of this set*/
        thissgrpptr->lastlistptr = NULL;/* ending list number of this set*/
           /*the list routines must adjust the list pointers*/
        
        thissgrpptr->STATUS = 0;  
            /* initialize as recessive; does not hide things below it*/
 
        thissgrpptr->bondrot = 0; /* initially not flagged for special output*/
        thissgrpptr->instanceptr = NULL;/*presume an original, not an instance*/ 
        thissgrpptr->balllists = 0; /*presume no balllists to start*/ 
        thissgrpptr->grupptr = thisgrupptr; /*acknowledge parent*/
        thisgrupptr->lastsgrpptr = thissgrpptr;
            /* adjust pointer in parent group to include this set */
        if(thisgrupptr->firstsgrpptr ==NULL)/*parent had no previous subgroups*/
            thisgrupptr->firstsgrpptr = thissgrpptr;

        memset(thissgrpptr->name,'\0',MAXNAMECHAR+1); /* optimized */
          /* for(n=0;n<=MAXNAMECHAR;n++) thissgrpptr->name[n] = '\0'; */
          /*clear subgroup name which has MAXNAMECHAR+1 characters*/     
        thissgrpptr->master = 0; /*unsigned int for master button flags*/
        thissgrpptr->NDIM = thissgrpptr->grupptr->NDIM; /*inherit 060622*/
        thissgrpptr->KKIND = thissgrpptr->grupptr->KKIND; /*inherit 060622*/
        thissgrpptr->fwrap = thissgrpptr->grupptr->fwrap; /*inherit 060622*/
        /*121211 allow inheritance of static and screen*/
        if(thissgrpptr->grupptr->STATUS & STATICFLAG) 
        {   thissgrpptr->STATUS = thissgrpptr->STATUS | STATICFLAG; }/*121211*/ 
        if(thissgrpptr->grupptr->STATUS & SCREENFLAG) 
        {   thissgrpptr->STATUS = thissgrpptr->STATUS | SCREENFLAG; }/*121211*/
      }/*initialize newly allocated subgroup*/
}
/*___getnewsubgroup()________________________________________________________*/        

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*mageVect.c*/
/****getlist(c,Lnew)*********************************************************/
void    getlist(int c,int Lnew)

{
/*need some of these static since getlist() called repeatedly during input*/
enum   {   idenID = 1
        ,  numbID = 2
        ,  skipID = 3
        , wordsID = 4
        , charsID = 5
        , parenID = 6
       };

struct liststruct *origlistptr;
static int pointwidth;

static int  Liden,Lnum,Lfirst,Lstartiden;
static int  Lsinglequotetoggle=0;
static int  Lwords,Lstartwords,Lembeddedcomment;
static int  ivect,nvect,LgotpointID,gotinstance,Lchar;
static int  Ndimension; /*060121*/
static int  flowID,n,j,monitornxyz,monitornxyzold=0;
static int  toggle;
static char vects[256],instancename[MAXNAMECHAR+1],ch,previouschar;
static int  Lparen;
   int ION=0;

  /*Iitialize Lmove by Lnew for first point of any list */
  /*This is very important, since  MOVETO_P flag is then*/
  /*robustly on the first point of any list and pure successive*/
  /*point searches can avoid flowing into adjacent lists*/
  if(Lnew)
  {/*Lnew*/
     if(thisgrupptr == NULL)  
     {/* @____list without a preceeding @group, construct first group */     
        getnewgroup(); /*MAGESETS*/
        if(thisgrupptr != NULL)
           thisgrupptr->STATUS = thisgrupptr->STATUS | NOBUTTON;    
           /*nobutton and recessive*/
     }
     if(thisgrupptr != NULL)
     {/*this group is allocated*/
        if(thisgrupptr->firstsgrpptr == NULL)  
        {/* @____list without a preceeding @subgroup in this group*/
            thissgrpptr = NULL;
            getnewsubgroup(); /*MAGESETS*/
            if(thissgrpptr != NULL)
            {
               thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON | NEVERSHOW;    
               /*nobutton and recessive and nevershow in output*/
            }
        }
        if(thissgrpptr != NULL)
        {/*this subgroup is allocated*/
           Lfirst = 1;
           Lmove = 1;
           Lunpickable = 0;
           nvect = 0;  /* counts up to 3 to get vector triplet */
           Linstance = 0;
           Lclone = 0;
           Ldimension = 0; /*060121*/
           Lwrap = 0; /*060121*/
           LwrapNumber = 0; /*060121*/
           Lmaster = 0;
           thislistptr = NULL;
           getnewlist(); /*MAGESETS*/
           if(thislistptr != NULL)
           {/*this list is just now allocated*/
              thislistptr->STYLE =  thislistptr->STYLE | Liststyle;

              thislistptr->type =  thislistptr->type | Listtype;
              /*thislistptr->type may have inherited from subgroup*/
              /*Listtype is set in MAGEINPT by list KeyWord*/

              /*lists and points share type basic type flags*/
              /*BUT lists can be flagged  DETAILFLAG  */
              /*and NOT INVENTED | STORED | UNPICKABLE | MOVETO_P etc. */
              /* so DETAILFLAG overloads INVENTED */
              Newscale = FALSE;  /* set scale logicals */
        
              flowID = skipID;
              iword = 0;
              word[0] = ' ';  /* give it a blank pointID to start */
              word[1] = '\0';
              Lnewlist = TRUE;
              /* need to know this so check for duplicate triples which */
                          /*cannot carry across from previous list */
        
              Lsinglequotetoggle = 0; /* allows numbers as comments: '#' */
                                /* allows chaos labels in kinemage */
                                /*SUBVERTED FOR POINTMASTER 991210 */
              LgotpointID = 0; /* flags when something in that will be used */
                         /* as a point ID */
              Lmustkeeppoint = 0; /* flags when comment or wordlist forces */
                            /*one to keep this point even if a duplicate. */
              Lcomment = 0;
              Lballhere = 0;    /*flags when a ball is expected at this point*/
                          /*vectorline is to be shortened by radiuslist */
              Lballthere = 0;
              /*stacked disks really need shortened lines*/
              if(Listtype&BALL)  
              {/*SPHERE is a VARIANT of BALL, test for primative*/
                 thissgrpptr->balllists = 1; /*flag balllist for this subgroup*/
                 thisgrupptr->balllists = 1; /*flag balllist for this group*/  
              }
              Lwordsfont = 0;
              Lwordsface = 0;
              Lwordssize = 0;
              thislistptr->wordsfont = 0; 
              thislistptr->wordsface = 0;
              thislistptr->wordssize = 0;

              listfirstline(c,Lnew); /*MAGEVSUB.C*/ /*for more initializations*/
              pointcolor = 0;  
              pointwidth = 0;
              itemps = 0;       /*temps[256] used for colorname*/
              temps[0] = '\0';
              Lchar = 0;       
              Lstartpt = 0; /*060523*/
           }/*this list is just now allocated*/
        }/*this subgroup is allocated*/
     }/*this group is allocated*/
     
  }/*Lnew*/
  if(thislistptr != NULL)
  {/*this list is allocated*/
    /* test character and set flow controls  */
    /* screen for comment strings, may be used as wordlist to screen */
    if(c == '<' )
    {/*< <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
       if(flowID != idenID && flowID != wordsID)  
       {/*start a comment or wordlist entry*/ /*don't if inside curly brackets*/
          Lstartwords = 1; /*signels start of <...> characters*/
          flowID = wordsID;
          Lwords = 0; /* Lwords set = 1 in case wordsID below */
          Lnum = 0; /* in any case, not still within a number*/
          Lembeddedcomment = 0;
       }
       else if(flowID == wordsID && previouschar != '\\') 
       {/*embedded comment in a comment: this is valid as of 991013*/
          Lembeddedcomment = 1; /*set flag, flow stays with wordsID*/
       }
    }/*< <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    else if(c == '>')
    {/*> >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
       if(flowID==wordsID && Lembeddedcomment && previouschar != '\\')
       {
          Lembeddedcomment = 0; /*drop flag, flow stays with wordsID*/
       }
       else if(flowID != idenID && previouschar != '\\') 
       {/*end a comment or wordlist entry */ /*don't if inside curly brackets*/
          if(!Lwords)
          {
              sprintf(alertstr,"> without preceeding <,"
                              "\015 may be accumulating chars:");
              word[iword+1] = '\0';
              errorreport(); /*alertstr and word*/ /*MAGEVECT.c*/
          }
          Lwords = 0; /*reset flag*/
          Lnum = 0;
          flowID = skipID;
          if(!Lfirst) /* <comments> on first line are ignored */
          {/*got <comments> on other lines*/
           /*store possible comment stuff, make word[last] == '\0' */
              word[iword] = '\0'; 
                /*iword incremented after each char added*/
              /*firstline may have defined: */
              commentstr[0]=thislistptr->wordsfont; /*font*/
              commentstr[1]=thislistptr->wordsface; /*face*/
              commentstr[2]=thislistptr->wordssize; /*size*/
              for(n=0; n<=iword; n++) 
              {
                  commentstr[n+3] = word[n];
              }
              Lmustkeeppoint = 1;
              Lcomment = 1;
          }
          /*clear the word[] */
          iword = 0;
          word[0] = ' ';
          word[1] = '\0'; 
       }/*end a comment or wordlist entry */
    }/*> >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* screen for identifier strings */
    if(c == '{' && flowID != wordsID /*} comes later*/)/*balancing brace in if*/
    {/* start an identifier */ /*don't if inside <> */   /*start CURLEY BRACES*/
        Lstartiden = 1;

        /*check to see if too few coords*/ /*061201*/
        /*the numbID option is when next ptID curley brace ends the last coord*/
        /* this seems to be useless since such points mess up anyway */ 
        if(    LgotpointID 
            && (nvect != 0)
            && (  ((flowID==numbID) && (nvect+1 < Ndimension)) 
                ||((flowID==skipID) && (nvect   < Ndimension)) ) )
          
        { /*set flag to clean out the old partial point*/
           Lpointabandon = 1;
        }
        if(flowID == skipID) {flowID = idenID;}
        nvect = 0; /*forget any coord that comes before the pointID*/
        Liden = 0; /* Liden set = 1 in case idenID below */
        Lnum = 0; /* in any case, not still within a number, could be numbID*/
    }/* start an identifier */ /*don't if inside <> */   /*start CURLEY BRACES*/
    else if(/*{ came earlier*/ c == '}' && flowID != wordsID)/*balancing brace*/
    {/*dealing with brackets*/ /*end of an identifier */ /*don't if inside <> */
        if(!Liden)
        {
           sprintf(alertstr,"didn't find preceeding { for this }"
                            ",\015 may be accumulating chars:");
           word[iword+1] = '\0';
           errorreport(); /*alertstr and word*/ /*MAGEVECT.c*/
        }
        j = 256; /* point ID & labels now 0 to 256 char */ /*970410*/
               /*pattern can be 37, let list and button names be limited later*/

        word[iword] = '\0';
        Liden = 0;
        Lnum = 0;
        flowID = skipID;
        if(Lfirst)
        {
            if(Linstance || Lclone)
            {/*should hold identifier of a previously defined ____list */
                /* _____list named, copy it to instancename */
                {
                    for(n=0; n<=MAXNAMECHAR ; n++)
                    {
                        instancename[n] = word[n];
                    }
                    instancename[MAXNAMECHAR] = '\0'; /*insurance*/
                    iword = 0;
                    word[0] = ' ';
                    word[1] = '\0'; 
                    
                }
                origlistptr = firstlistptr;
                gotinstance = 0;
                while(origlistptr != NULL)
                {
                    if(strncmp(instancename,origlistptr->name
                                               ,MAXNAMECHAR) == 0)
                    {
                        thislistptr->on = 1; /* display enabled*/     
                        thislistptr->type = origlistptr->type; /*971109*/
                        thislistptr->firstpointptr = origlistptr->firstpointptr;    
                            /* starting vector number of this list */
                        thislistptr->lastpointptr = origlistptr->lastpointptr;
                            /* ending vector number of this list */
                        if(thislistptr->owncolor == 0)
                           /*flag for list's own color*/ 
                           thislistptr->color = origlistptr->color;
                            /*unless a new color= has been read, */
                            /* use old color of original list*/
                        thislistptr->width = origlistptr->width;  /*990203*/
                            /* use old pen width of original list*/
                        if(thislistptr->master == 0)
                           thislistptr->master = origlistptr->master; 
                            /* master button flag for this list*/
                            /*unless a new master= has been read, */
                            /* use master of original list*/
                            /*this can be reset later on this first line*/
                        thislistptr->bondrot = origlistptr->bondrot;
                            /* special output flag for this list*/  
                           /*as a bond rotation, instance might behave wierdly*/   
                        thislistptr->STATUS = origlistptr->STATUS;
                            /* nobutton flag for this list*/ 
                        thislistptr->STYLE = origlistptr->STYLE;
                        thislistptr->instanceptr = origlistptr; 
                            /*identity of original list*/   
                        if(thislistptr->radius == 0)        /*970926*/
                           /*flag for list's own radius*/ 
                           thislistptr->radius = origlistptr->radius;
                            /*unless a new radius= has been read, */
                            /* use radius of original list*/
                        gotinstance = 1;
                        break;
                    }
                    origlistptr = origlistptr->nextptr;
                    if(origlistptr == thislistptr) origlistptr = NULL;
                            /*avoid self*/
                 }
                 if(!gotinstance)
                 {
                   if(Linstance)
                   {
                     sprintf(alertstr,
                        "instance failed, no previous list has name: %s",
                        instancename);
                   }
                   if(Lclone)
                   {
                     sprintf(alertstr,
                        "instance failed, no previous list has name: %s",
                        instancename);
                   }
                   errorreport(); /*alertstr and word*/ /*MAGESETS.c*/

                 }
                 else if(Lclone)
                 {
                     thislistptr->instanceptr = NULL;/*reset, NOT an instance*/
                     clonelistpoints(origlistptr);
                 }
                 gotinstance = 0;
                 Linstance = 0;
                 Lclone = 0;
            }
            else if(Lmaster)
            {/*should hold identifier of a master button for this list */

               masterfinder( &(thislistptr->master) );
               Lmaster = 0;
            }
            else if(Lpattern>1) /*overloaded flag, 2 expected, 1 in, 0 none*/
            {/*should hold char str pattern of rotation restrictions */
             /*or the keyword that describes a builtin pattern*/
                for(n=0; n<MAXBONDPATT ; n++)
                {
                   bondrotpattern[0][n] = word[n];
                }
                bondrotpattern[0][MAXBONDPATT] = '\0'; /*insurance*/
                /*can't do any more until EOL when bondrot# is resolved*/
                Lpattern = 1; /*now means pattern is stored*/
            }
            else if(Lcolnam)
            {/*color name in global word[]*/
                thislistptr->color = colorsetfinder( &(thislistptr->colorset) );
                thislistptr->owncolor=1; /*flag for this instance's own color=*/
                Lcolnam = 0;
            }
            else if(Lresname) /*071005 alternate KKIND list suitefit base name*/
            {
               for(n=0; n<4; n++) /*PDB residue name 3char (+\0) */
               {
                  thislistptr->resname[n] = word[n]; /*071005 PDB resname[4]*/
               }
               thislistptr->resname[3] = '\0';
            }
            else /*global word should hold identifier of this ____list */    
            {
                /* _____list has a name, copy it to listname */
                {
                    for(n=0; n<=MAXNAMECHAR ; n++)
                    {
                        thislistptr->name[n] = word[n];
                    }
                    thislistptr->name[MAXNAMECHAR] = '\0'; /*insurance*/
                  /*array has MAXNAMECHAR+1 values indexed: 0,...,MAXNAMECHAR*/
                    iword = 0;
                    word[0] = ' ';
                    word[1] = '\0'; 
                    if(Ltest)
                    {
                         sprintf(alertstr,"debug test for listname");
                         errorreport(); /*alertstr and word*/ 
                    }

                }
            }
        }/*Lfirst for doing bracketed stuff on first line */
        else
        {/*got bracketed stuff on other lines*/
            if(LgotpointID) /*already have a current pointID */
            {/*check to see if anything useful in these brackets*/
                if(iword == 1 && (word[0] == 'U' || word[0] == 'u') ) 
                   Lunpickable = 1;
                   /*tightly bracketed U can be used for unpickable point*/
            }
            else
            {/*store possible pointID stuff*/
                n=-1;
                do{
                     n++;
                     ptIDstr[n] = word[n];
                  }while(word[n] != '\0');
                LgotpointID = 1; /*reset back to 0 after addanothervector()*/
            }

            /*clear the word[] */
            iword = 0;
            word[0] = ' ';
            word[1] = '\0'; 
        }
        /*clear the word[] */
        iword = 0;
        word[0] = ' ';
        word[1] = '\0'; 

    }/*dealing with brackets*/
    if(c == '\'' && !Lfirst && !Liden && flowID != wordsID)
    {/*dealing with single quotes*/
        if(Lsinglequotetoggle)
        {
           /*pre-050220 makes probe contact dots of a ptmaster category be off*/
           /*when the previous update had no dots in this category which */
           /*sets that master off in resetmaster() where a master is set ON*/
           /*only when an ON grup,sgrp,list,point is actually encountered! */
           /*This could be fixed by having resetmaster not change state of */
           /*missing entries, BUT how could it know if a particular master */
           /*controled entity was never encountered???? */
           /*OR should points always be ON when read in???? */
           /*BUT there must have been some reason that multiple-mastered pts*/
           /*are tested out here vs the point masters!!!!*/
           /*Perhaps the patch would be to force points that have */
           /*only one master character to be set to be ON when read in*/
           /*This will mess up a situation where some points have one master*/
           /*but others have more than one and control is over a number of*/
           /*different master-controled types*/

           Lsinglequotetoggle = 0;
           pointmasterstr[ipms] = '\0';
           IONpointmaster = 1; /*presume point is ON*/
           MASKpointmaster = 0; /*clear mask 030528*/
           for(j=0; j<ipms; j++)
           {/*check pointmaster characters one at a time*/
              ION = masterconfirmer(pointmasterstr[j]);
              if(ION >= 0) /* ION < 0 means thismasterptr NOT defined 051128*/
              {
                 MASKpointmaster = MASKpointmaster | thismasterptr->mask;
                 /*all masters must be ON to have point be ON*/
                 /*ipms==1 for point of a probe update with contact categories*/
                 if(ipms > 1)
                 {   /*050220 only can turn off if multiple point masters*/
                    if(!ION) { IONpointmaster = 0; }
                 }
              }
           }
        }
        else
        {
            Lsinglequotetoggle = 1;
        }
    }/*dealing with single quotes*/
    if(Lparen)
    {
       if(c != '(' && c == ')') /*balance parens*/
       {
          Lparen = 0; /*presume () delineated string is in*/
          /*always terminated, so if iparen > 0, finite parenstr */
          flowID = skipID;
       }
       else
       {/*accumulate character for () delineated string*/
          flowID = parenID;  /*in parenID case */
       }
    }
    else if(   c == '(' && c != ')'   /*balance parens*/
        &&!Lfirst && !Liden && flowID != wordsID
        &&!Lsinglequotetoggle)     
    {/*dealing with parens*/
       /*start looking for () delineated string*/
       iparen = 0;
       Lparen = 0; /* Lparens set = 1 in case parenID below */
       Lnum = 0; /* in any case, not still within a number*/
       flowID = parenID;
    }/*dealing with parens*/
    /* now either in an identifier in which any other char accepted ... */
    /* or just coming out of an identifier, */
    /* or must check the char */
    if(   !Lstartiden  && !Liden  && (flowID != idenID )  
       && !Lstartwords && !Lwords && (flowID != wordsID)    
       && !Lparen && (flowID != parenID)
       && /*{ balance brace*/ c != '}' && c != '>'  && /*(*/ c != ')'      )      
    {
        if(Lfirst)  /* first line with @----list can have {button identifier}*/
                    /* & color=colorname & off flag, etc.  */
        {
            Lfirst = (int)listfirstline(c,Lnew); /*MAGESETS.C*/
        }
        else if(Lsinglequotetoggle)
        {
            /*flowID = skipID;*/ /*skip*/
            if(c != '\'')
            {/*after the initial single quote mark*/
               /*ACCUMMULATE PUTATIVE POINTMASTER IDENTIFIER*/
               if(ipms < MAXIPMS-1) pointmasterstr[ipms++] = c; /*-1 051128*/
            }
        }
        else if(  c==' '||c==','||c==';'||c=='\t' ||c==':'   /*':' 040320*/
/*{balance*/    ||c=='\015'||c=='\012'||c==EOL || c == '}') 
        {/* generic white space (whitespace)*/
            
            if(Lchar==1) 
            {/*interpret single character, may be a flag*/
                ch = temps[0];
                if(ch == 'm'||ch=='M'||ch=='p'||ch=='P') 
                {/*flag a Move instruction*/
                     Lmove = 1;
                     Lnum = 0;
                     flowID = skipID;
                }
                if(ch == 'd'||ch=='D'||ch=='l'||ch=='L') /*for completeness*/
                {/*but do not bother to flag a DraW instruction, i.e.ignore*/
                     Lnum = 0;
                     flowID = skipID;
                }
                else if(ch == 'u'||ch=='U')  /* flag an Unpickable triple */
                {
                     Lunpickable = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 'b'||ch=='B')  
                {/* flag a ball expected at this point */
                   if(Listtype&BALL)/*060208 ignore B flag avoid doubled balls*/
                   {/*ignore flag which causes shortened lines in vectorlists*/
                     flowID = skipID;
                   }
                   else if(thisgrupptr->KKIND & DIMENSIONKKIND)
                   {/*shortened lines storage does not account for 7D */
                     flowID = skipID;
                   }
                   else
                   {
                     LBallflag = 1;
                          /*actual specific entry request for shortened line*/
                     Lballhere = 1; /*generic expectation of shortened line*/
                     Lmustkeeppoint = 1;
                     /*Lunpickable = 1;this dealt with later*/ /*980804*/
                     Lballpoint = 1; /*030425 specifically sets point type*/
                     Lnum = 0;
                     flowID = skipID;
                   }
                }
                else if(ch == 't'||ch=='T')  /* flag a triangle point */
                {
                     Ltriangle = 1;
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 'x'||ch=='X')  /* flag a starting point 060523*/
                {
                     Lstartpt = 1; /* e.g. starting pt of independent triangle*/
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 'a'||ch=='A')  /* flag an arrow head point */
                {
                     Larrow = 1;
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 'r'||ch=='R')  /* flag a ring point */
                {
                     Lring = 1;
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 'q'||ch=='Q')  /* flag a square point */
                {
                     Lsquare = 1;
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else if(ch == 's'||ch=='S')  /* flag a sphere point */
                {
                     Lsphere = 1;
                     Lmustkeeppoint = 1;
                     Lnum = 0;
                     flowID = skipID;
                }        
                else 
                {/*ignore*/
                     Lnum = 0;
                     flowID = skipID;
                }        

            }/*single character, may be a flag*/
            else if(Lchar > 1)
            {/*multiple characters in string, try to interpret*/
              temps[itemps] = '\0'; /*end string in c manner*/
              if(temps[itemps-1] == '!')
              {
                   Lexclamation = 1;
                   temps[itemps-1] = '\0'; /*shorten string*/
              } 
              else Lexclamation = 0; 
                 /*used only to force pointcolor completely dominant*/
                 /*set here at the only place pointcolor can be set*/
              
              j = numberofcolor(temps); /*MAGECOLR.c*/
              /*colorwidth packed with*/
              /*5 bit color index, 3 bit color depth code, 3 bit linewidth*/
              if(j > 0)
              {    /*pointcolor might contain earlier width info */ /*980818*/
                   /*pointcolor = j | pointwidth;*/ /*980818*/
                   pointcolor = j | pointcolor;/*980818*/
              }
              else
              {/*see if this char string codes for a line width or radius*/
               if(  !strncmp(temps,"thin" ,strlen(temps))
                  ||!strncmp(temps,"width1",strlen(temps))
                 ) /*single char before number incompatible with special case*/
               {
                  pointwidth = (1<<8);
               }
               else if(  !strncmp(temps,"width2" ,strlen(temps)))
               {
                  pointwidth = (2<<8);
               }
               else if(  !strncmp(temps,"width3" ,strlen(temps)))
               {
                  pointwidth = (3<<8);
               }
               else if(  !strncmp(temps,"width4" ,strlen(temps)))
               {
                  pointwidth = (4<<8);
               }
               else if(  !strncmp(temps,"width5" ,strlen(temps)))
               {
                  pointwidth = (5<<8);
               }
               else if(  !strncmp(temps,"width6" ,strlen(temps)))
               {
                  pointwidth = (6<<8);
               }
               else if(  !strncmp(temps,"width7" ,strlen(temps)))
               {
                  pointwidth = (7<<8);
               }
               else pointwidth = 0;

               if(pointwidth > 0)
               {/*did code for a line width*/ /*980804*/
                  if(pointcolor > 0) /*already have a color*/
                  {
                     pointcolor = pointcolor | pointwidth;
                  }
                  else
                  {
                     pointcolor = pointwidth;
                  }
               }/*did code for a line width*/ /*980804*/
               else 
               {/*see if this char string codes for something else*/ 
                  if(!strncmp(temps,"r=" ,2))
                  {/*starts:  r= */
                     temps[0] = ' '; 
                     temps[1] = ' '; /*strip off r= characters*/
                     pointradius = floatfromstr(temps);    /*MAGEUTIL.C*/
                  }/*starts:  r= */
                  if(!strncmp(temps,"n=" ,2)) /*atom order suitefit base071005*/
                  {/*starts:  n= */
                     temps[0] = ' '; 
                     temps[1] = ' '; /*strip off n= characters*/
                     pointnumber = intfromstr(temps);    /*MAGEUTIL.C*/
                  }/*starts:  n= */
                  else if(  !strncmp(temps,"dummy",5)
                          ||!strncmp(temps,"DUMMY",5)
                         )
                  {/*dummy point*/
                     Ldummy = 1;
                  }/*dummy point*/
                  else if(  !strncmp(temps,"ghost",5)
                          ||!strncmp(temps,"GHOST",5)
                         )
                  {/*ghost point*/
                     Lghost = 1;
                  }/*ghost point*/

                  else if(  !strncmp(temps,"tablecell",9)
                          ||!strncmp(temps,"noxyz",5)
                          ||!strncmp(temps,"NOXYZ",5)
                         )
                  {/*noxyz point*/
                     Lnoxyz = 1;
                     Lpointcomplete = 1;
                  }/*noxyz point*/

               }/*see if this char string codes for something else*/  /*991203*/


              }/*see if this char string codes for a line width or radius*/ /*980804*/
               
              flowID = skipID; /*done its work*/                
            }/*multiple characters in string, try to interpret*/
            else
            {
                if(Lnum >0)
                {/* might end a number, leave flowID alone */
                     Lnum = 0;
                }
                else
                {
                     flowID = skipID;
                } 
            }
            Lchar = 0;  /*any white space ends putative char string*/
            itemps = 0;
            temps[0] = '\0';

        }/* generic white space */
        else if(Lchar==0 && (  c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'
                             ||c=='6'||c=='7'||c=='8'||c=='9'||c=='.'||c=='-'
                             ||c=='+'||(Lnum>0&&(c=='e'||c=='E'))            /*990125*/
                            )
               )
        {
            Lnum = 1;          /* actually got a digit of a number */
            flowID = numbID;
        }
        else if(Lchar==1 && (c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9'||c=='.'||c=='-'||c=='+')) /*971003*/
        {/*single character followed by a number, used in some Voet&Voet kins*/
         /*actually allows most compressed format: no space after curly brace,P and no L */
         /*also must allow for single digit coord, so {}P5,4,3 or P5,4,3  valid.*/
            if(temps[0]=='P')
            {/*allow at least this single char flag to work as Px y z*/
               Lmove = 1;/*flag a Move instruction*/
               Lnum = 1; /* probably got a digit of a number, set number reading flag*/
               Lchar = 0; /*unset char reading flag*/ /*990114*/
               temps[0] = ' ';/*clear char so a number could be scanned correctly*/
               flowID = numbID;
            }
        }
        else
        {/*is an arbitrary character, perhaps part of a parameter*/
            Lchar++;
            flowID = charsID;
        } 
    }

    /* Now control by flowID cases */
    /*begin-of-switch(flowID)*/                        
    switch(flowID)
    {/*switch(flowID)*/
        case idenID:
            if(Liden) /*expect identifier characters in curley braces */
            {
               /*if(Lfirst) j=MAXNAMECHAR;*/ /*list & button names MAXNAMECHAR char*/
               /*else j = MAXPTIDCHAR; */ /* point ID & labels MAXPTIDCHAR char*/
               j = 255;   /* point ID & labels now 0 to 256 char */
                       /*patterns can have 37, limit names later*/
               if(iword >= j)
               {
                    word[j] = '\0'; /*970410*/
                    if(Lfirst)
                    {
                        sprintf(alertstr,
                          "limited to %d char in {list name}"
                          "\015next char: %c, would make string below too long."
                           ,j,c);/*970410*/
                        errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                    }
                    else if(LpointIDtoolong==0)
                    {
                        sprintf(alertstr,
                          "#char limited to %d for {pointIDs} and {labels}." 
                          "\015next char: %c, would make string below too long."
                          "\015 only first occurrance flagged, check for others"
                           ,j,c);/*970410*/
                        errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                        LpointIDtoolong = 1;
                    }
                    flowID = skipID; /*NOTE Liden still == 1 */
               }
               else
               {
                    word[iword] = c;
                    iword = iword + 1;
               }
            }
            else  /* just got the left curley brace char, so... */
            {
                Liden = 1; /* get ready for the actual identifier characters*/
                Lstartiden = 0;    
                iword = 0;
                word[0] = ' ';  /* give it a blank pointID to start */
                word[1] = '\0';
                ivect = 0;  
                /*{identifier} forces initialization of number accummulation*/
        
            }
        break;

        case wordsID:
            if(Lwords)/*expect wordlist or comment characters in <...>brackets*/
            {
              if(!Lfirst) /*first line <comments> are ignored */
              {/*not a first line*/
                if(iword > 254)
                {      /*index: 0,...,255  for 256 entries */
                     word[iword] = '\0';
                     sprintf(alertstr,"next char: \"%c\" "
                         "makes string (below) too long:"
                         "\015ignore input characters until \">\" bracket"
                         ,c);
                     errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                     flowID = skipID; /*NOTE Lwords still == 1 */
                }
                else
                {
                    word[iword] = c;
                    iword = iword + 1;                
                }
              }/*not a first line*/
            }
            else  /* just got the < char, so... */
            {
                Lwords = 1; /* get ready for the actual comment or */
                            /*wordlist characters*/
                Lstartwords = 0;    
                iword = 0;
                word[0] = '\0';
                ivect = 0;  
                /*<comment> forces initialization of number accummulation*/
        
            }
        break;

        case parenID:
            if(Lparen)/*expect characters in (...) parentheses*/
            {
              if(!Lfirst) /*first line (...) are ignored */
              {/*not a first line*/
                if(iparen > 254)
                {      /*index: 0,...,255  for 256 entries */
                     parenstr[iparen] = '\0';
                     sprintf(alertstr,"next char: \"%c\" "
                         "makes string (below) too long:"
                         "\015ignore input characters until \"(...)\" ends"
                         ,c);
                     errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                     flowID = skipID; /*NOTE Lparen still == 1 */
                }
                else
                {
                    parenstr[iparen++] = c;
                    parenstr[iparen] = '\0'; /*keep it terminated*/     
                }
              }/*not a first line*/
            }
            else  /* just got the first () char, so... */
            {
                Lparen = 1; /* get ready for the actual string */
                iparen = 0;
                parenstr[0] = '\0';
                ivect = 0;  
                /*(string) forces initialization of number accummulation*/
            }
        break;
                            
        case numbID:
            if(Lnum)
            {
                if(ivect > 254)
                {
                    vects[ivect] = '\0';
                    sprintf(alertstr,
                        "next char: %c in vect string for numbers"
                        CRLF" excessive length, something really wrong"
                        CRLF" string is:",c);
                    sprintf(word,"%s",vects);
                    errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                    ivect--;    
                }            
                vects[ivect] = c;
                ivect++;    
            }
            else  /* Lnum = 0, should be at end of a number */
            {/*-process-number*/
                vects[ivect] = '\0'; /* str end */
                if(Lstartiden) flowID = idenID; 
                    /*number was ended by next {...} */
                else      flowID = skipID; 
                   /*new number will have to reset flowID*/
                nvect++;
                /*sscanf(vects,"%f",&x[nvect]);*/ /*read char str as a float*/
                x[nvect] = (float)atof(vects); /*requires #include <stdlib.h> */
                   /*atof handles all float formats, inc e form*/ 

 /*060121*/     /*if(LwrapNumber && x[nvect]<0){x[nvect] = x[nvect] + Number;}*/
                if(thislistptr->KKIND & WRAPPINGKKIND) /*061023*/
                {
                   if(x[nvect]<0) {x[nvect] = x[nvect] + thislistptr->fwrap;}
                }

                ivect = 0;  /* reset for next number */
                if(thislistptr->KKIND & DIMENSIONKKIND) /*060121,060622 list*/
                {
                   if(thislistptr->NDIM > 3) /*safety 060622*/
                        {Ndimension = thislistptr->NDIM;}
                   else {Ndimension = 3;}
                   Lmustkeeppoint = 1; /*avoid trouble in addanothervector()*/
                }
                else Ndimension = 3;
                if(nvect == Ndimension) /* all coords for vector in, store it */
                {
                   Lpointcomplete = 1;
                }
            }/*-process-number*/
        break;

        case charsID:
                if(itemps > 254)
                {
                    temps[itemps] = '\0';
                    sprintf(alertstr,
                        "next char: %c in char string"
                        CRLF" excessive length, something really wrong"
                        CRLF" string is:",c);
                    sprintf(word,"%s",temps);
                    errorreport(); /*alertstr and word */ /*MAGESETS.c*/
                    itemps--;    
                }            
                temps[itemps] = c;
                itemps++;    
        break;
        
        case skipID:
        break;
   
    }/*switch(flowID)*/
    previouschar = c;
    if(Lpointcomplete || Lpointabandon) /*061201*/
    {/*Lpointcomplete*/
      /*point allocation is done below in addanothervector() */
      {/*begin try to add another vector */
         monitornxyz = nxyz/100;
         if(monitornxyzold > monitornxyz) 
            monitornxyzold = monitornxyz;
         if(monitornxyz >= monitornxyzold + 5)
         {
            toggle = !toggle;   /*981015*/
            if(toggle)
            {
               if(LGUI) makecursorwait();
            }
            else  
            {
               if(LGUI) makecursorzrot();
            }
            monitornxyzold = monitornxyz;
         }  
         if(!Lpointabandon)  /*061201*/
         {
            addanothervector(Ndimension); /*MAGESETS.c*/ 

            if((thislistptr->STYLE & DOTBALLSTYLE) == DOTBALLSTYLE)
            {
               makeDotBallAtxyz(); /*MAGEBALL.c 100406 build dotball here*/
               if((thislistptr->STYLE & ENDBALLSTYLE) == ENDBALLSTYLE)
               {
                  makeInteriorDotsInvisible(); /*MAGEBALL.c 100407*/
               }
            }
         }
         Lsinglequotetoggle = 0; /*initialize this after each */
         ipms = 0; /*ith char in Point Master String*/
            /*triple so things can't go too wrong */ 
         LgotpointID = 0; /*expects pointID to be in ptIDstr[] */
             /* when it puts away a triple as a vector */  
         /*clear pointID*/
         ptIDstr[0] = '\0';
         Lmustkeeppoint = 0;
         Lstartpt = 0; /*060523*/
         pointcolor = 0;
         pointwidth = 0; /*980818*/
         pointmasterstr[0] = '\0'; /*991210*/
         ipms = 0;  /*991210*/
         iparen = 0;
         parenstr[0] = '\0';
         
      }/*endof try to add another vector */
      
      nvect = 0;
      iword = 0;
      word[0] = ' ';
      word[1] = '\0';
      ivect = 0;
      Lmove = 0;
      Lunpickable = 0;
      LBallflag = 0; /*must be flagged for each point*/
      pointradius = 0.0; /*must be brought in for each point*/ /*980804*/
      pointnumber = 0; /*must be brought in for each point*/ /*071005*/
      Ltriangle = 0; /*must be flagged for each point*/
      Larrow = 0; /*must be flagged for each point*/
      Lsphere = 0; /*must be flagged for each point*/
      Lballpoint = 0; /*must be flagged for each point*/
      Lring = 0; /*must be flagged for each point*/
      Lsquare = 0; /*must be flagged for each point*/
      Lexclamation = 0; /*must be flagged for each point*/
      pointmasterstr[0] = '\0'; /*991210*/
      ipms = 0;  /*991210*/
      Ldummy = 0; /*must be flagged for each point*/
      Lghost = 0; /*must be flagged for each point*/
      Lnoxyz = 0; /*must be flagged for each point*/
      Lpointabandon = 0; /*061201*/
      Lpointcomplete = 0; /*must be flagged for each point*/
    }/*Lpointcomplete*/
  }/*this list is allocated*/
}
/*___getlist(c,Lnew)________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getnewlist()*************************************************************/
void getnewlist()
{        
    if( !allocliststructure() )  /*MAGELIST.c*/
    {/*allocation failed*/
        sprintf(alertstr,"failed to alloc a list of vectors, labels, or dots!");
        sprintf(word," ");
        errorreport(); /*alertstr and word*/ /*MAGEVECT.c*/
        thislistptr = NULL;
    }/*allocation failed*/
    else 
    {/*initialize newly allocated list*/

        thislistptr->on = -1;/*start with display tentatively disabled*/
          /* NOTE: If valid coord triples are to be displayed or instanced */
          /*  this vectorlist must be explicitly turned ON */
        thislistptr->firstpointptr = NULL;  /* starting point of this list */
        thislistptr->lastpointptr  = NULL; /*ending point of this list, so far*/
        thislistptr->color = numberofcolor("default"); /*MAGECOLR.c*/ 
           /* default is whiteFore on blackBack */
        thislistptr->width = 0; /* default 0 lets global width rule*/ /*990202*/
        thislistptr->owncolor =0;/*flag for this instance's own declared color*/   
        thislistptr->NDIM = 0; /*060622*/
        thislistptr->NDIM = thissgrpptr->NDIM; /*inherit 060622*/
        thislistptr->KKIND = thissgrpptr->KKIND; /*inherit 060622*/
        thislistptr->fwrap = thissgrpptr->fwrap; /*inherit 060622*/
        thislistptr->bondrot = 0;/* initially not flagged for special output*/
                            /* used for bond-rotated vectors */
        thislistptr->STATUS = thissgrpptr->STATUS; /*inherit 121211*/ 
        /*do not inherit NOBUTTON, from implicit subgroup 130304 */
        /*remove NOBUTTON by 2's complement of NOBUTTON 130304 */
        thislistptr->STATUS = thislistptr->STATUS & ~NOBUTTON;
        thislistptr->STYLE = 0; /*see MAGEFLAG.h*/                  
        thislistptr->instanceptr =NULL;/*presume an original, not an instance*/ 
        thislistptr->type = 0;  /*see MAGEFLAG.h*/  
        thislistptr->type = thissgrpptr->type; /*inherit*/ /*971109*/
        thislistptr->radius = 0.0; /*float property associated with this list*/
        thislistptr->angle = 0.0; /*float property associated with this list*/
		memset(thislistptr->name,'\0',MAXNAMECHAR+1); /* optimized */
        /* for(n=0;n<=MAXNAMECHAR;n++) thislistptr->name[n] = '\0'; */
            /* clear listname button id which has MAXNAMECHAR+1 characters*/
        thislistptr->master = 0; /*unsigned int for master button flags*/

        thislistptr->sgrpptr = thissgrpptr; /*acknowledge parent*/
        thissgrpptr->lastlistptr = thislistptr;
            /* adjust pointer in parent subgroup to include this list */
        if(thissgrpptr->firstlistptr == NULL) /*parent had no previous lists*/
           thissgrpptr->firstlistptr = thislistptr;
    }/*initialize newly allocated list*/
}
/*___getnewlist()____________________________________________________________*/        


/****errorreport()***********************************************************/
void errorreport()
{
  if(Lformattest || Ltest || Lallocerror) /*Lallocerror 051114*/
  {
    sprintf(alertstr2,"%s",word);
    if( (thisgrupptr !=NULL) && (thissgrpptr !=NULL) && (thislistptr !=NULL) )
        sprintf(alertstr3,"\015in group: %s,  (subgroup: %s ), list: %s"
          ,thisgrupptr->name,thissgrpptr->name,thislistptr->name);
    else if( (thisgrupptr !=NULL) && (thissgrpptr !=NULL) )
        sprintf(alertstr3,"\015in group: %s,  (subgroup: %s )"
          ,thisgrupptr->name,thissgrpptr->name);
    else if( (thisgrupptr !=NULL) )
        sprintf(alertstr3,"\015in group: %s",thisgrupptr->name);
    else
        sprintf(alertstr3,"\015error before any groups defined");

#ifndef UNIX_X11
    DoMageDLOGreport(2); /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
#else
    fprintf(stderr,"%s\n%s\n%s\n",alertstr,alertstr2,alertstr3);/*stderr051114*/
#endif
  }
}
/*___errorreport()__________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*mageVsub.c*/

/****listfirstline()*********************************************************/
int        listfirstline(int c,int Lnew)
{
          int        j,ireturn,iwordsfont,iwordsface,iwordssize;
  static  float      angle; 
  bondrotliststruct* thebondrotlistptr;
         
  if(Lnew)  /* initialize first line array counters */
  {/*initialize for new ___list*/
        iword = 0;
        word[iword] = '\0'; 
        icolor = 0;
        Lcolnam = 0;
        Lbrot = 0;
        Lbang = 0;
        Lbplot = 0;
        Lbnoslider = 0; /*070617*/
        Lbselect = 0; /*071215*/
        Lradius = 0;
        Lwidth = 0; 
        Lballhere = 0;
        Lballthere = 0;
        LBallflag = 0;
        Ltriangle = 0;
        Larrow = 0; 
        Lsphere = 0; 
        Lballpoint = 0; 
        Lring = 0; 
        Lsquare = 0; 
        Ldummy = 0; /*must be flagged for each point*/
        Lghost = 0; /*must be flagged for each point*/ 
        Lnoxyz = 0; /*must be flagged for each point*/
        Lpointcomplete = 0; /*must be flagged for each point*/
        Lpointabandon = 0; /*061201*/
        Lexclamation = 0; /*must be flagged for each point*/
        pointmasterstr[0] = '\0'; 
        ipms = 0; 
        pointradius = 0.0;
        pointnumber = 0; /*071005*/
        Lbrotoption = 0;
        angle = 0.0;
        Langle = 0; 
        Lresnumb = 0; /*071005*/
        Lresname = 0; /*071005*/
        Lsamescope = 0;
        Lpattern = 0; /*bondrot energy pattern*/
        ireturn = 0;
  }/*initialize for new ___list*/
  /*character in hand might be meaningfull, process it*/
  {/*internal first line info*/
    if(  c == '=')
    {/*special treatment for '=': treat as whitespace*/
       c = ' '; /*make it a white space*/
         /*NOTE: isolated = is thus ignored and changes no states*/
    }
    if(  c == ' '||c == ','||c == ';'||c == '\t'
       ||c == '\015' || c == '\012' || c == EOL ) /* white space */
    {/*white space*/
        if(iword > 0)  /* after a word, i.e. not before any char of a word */
        {/*one or more non-white-space characters have been accummulated*/

           /* --------- LIST PARAMETERS  --------------*/

            if( !(c == ' ' && icolor == 1) )  /* not blank within "color =" */
            {    
                word[iword] = '\0';  /* end string in c manner */
                if(Lcolnam)
                {
                   thislistptr->color = numberofcolor(word); /*MAGECOLR.c*/
                   if(thislistptr->color == 0)
                   {
                      thislistptr->color = numberofcolor("default");
                   }
                     /*thislistptr->color = color number of MAGE palette*/
                     /*thislistptr->color can also be set by instance*/
                   thislistptr->owncolor = 1; 
                     /*flag for this instance's own color=*/    
                   Lcolnam = 0;
                }
                else if( (Lbrot && !Lbang) || Langle)
                { /* interpret as float angle value*/               
                    sscanf(word,"%f",&angle);
                    Lbang = 1;
                    while(angle < -180.0) angle = angle + 360;
                    while(angle >  180.0) angle = angle - 360;
                    thislistptr->angle = angle;
                    Langle = 0;
                }
                else if(Lwordsfont)
                {/*interpret as an int, convert to short??, pack in char*/
                    sscanf(word,"%d",&iwordsfont);
                    thislistptr->wordsfont = iwordsfont;
                    Lwordsfont = 0;
                }
                else if(Lwordsface)
                {/*interpret as an int, convert to short??, pack in char*/
                    sscanf(word,"%d",&iwordsface);
                    thislistptr->wordsface = iwordsface;
                    Lwordsface = 0;
                }
                else if(Lwordssize)
                {/*interpret as an int, convert to short??, pack in char*/
                    sscanf(word,"%d",&iwordssize);
                    thislistptr->wordssize = iwordssize;
                    Lwordssize = 0;
                }
                else if(Lradius>1) /* interpret as float value*/
                {
                    sscanf(word,"%f",&thislistptr->radius);
                     Lradius = 1; /*need this set>0 for ball labels */
                }
                else if(Lwidth>0) /* interpret as int value*/ /*990202*/
                {
                    sscanf(word,"%d",&thislistptr->width);
                     Lwidth = 0; 
                }
                else if(Lresnumb>0) /* interpret as int value*/ /*071005*/
                {
                    sscanf(word,"%d",&thislistptr->resnumb);
                    Lresnumb = 0; 
                }
                /* working in ------- LIST PARAMETERS -----------*/
                else if(Ldimension) /*060121, all sets 060622*/
                {
                       sscanf(word,"%d",&n);
                       thislistptr->NDIM = n;
                       NDIM = thislistptr->NDIM; /*set global with latest NDIM*/
                       Ldimension = 0;
                       thislistptr->KKIND = thislistptr->KKIND | DIMENSIONKKIND;
                       LNdimensions = 1; /*060622*/
                       Lonscreeninfo = 1;
                       Lonscreeninfoon = 1;
                       /*NX,NY,NZ, onscreeninfostr  MAGEINIT/mageintialvalues*/
                       /*dimensionDialog sets defaults if names not inputed*/
                       defaultdimnames(); /*MAGEDLOG 060622*/
                }
                else if(Lwrap) /*060121, all sets 060622*/
                {
                       sscanf(word,"%f",&Number);
                       LwrapNumber = 1;
                       Lwrap = 0;
                       thislistptr->fwrap = Number; /*060525*/
                }
                else
                {
/*.*/                interpretlistkeyword();
                }
                iword = 0;
            }
        }/*one or more non-white-space characters have been accummulated*/
        else
        {/*white space with no characters accummulated: iqnore*/
           ; /*NOP code included to make iqnoring this char explicit*/
        }/*white space with no characters accummulated: iqnore*/
    }/*white space*/


    /*  curly brace will balance the next, allowing THINK C scope check*/
    else if(/*{*/c == '}') ; /*curly brace leaked through, ignore it*/
    else  /* accummulate character in word */
    {
/*c*/    c = tolower(c);  /*make lowercase so case independent */
        word[iword] = c;
        iword = iword + 1;
    }
    ireturn = 1;
   /*if(c == '\015')*/ /*must hit CR or newline before can get on with vectors*/
    if(c == EOL ) /* must hit clean newline */
    {    /* first line ending stuff */
        ireturn = 0;  /* ends first line */
        if(Lbrot) /*bondrot flag */ 
        {/*bondrot flag */
          if(nbondrot < MAXBONDROT)
          {/*enough bondrot ControlHandle typedef--> HWND(PC), -->Widget(MUX)*/ 
            
            /*index of sliders indexed to number of bondrot structures*/
            /*this is not necessary, but rather compilicated to reprogram*/
            
            if(nblev != 0) /* not just an ending*/
            {/*set up new bond rotation stuff*/
              /*inc. ganged rotations (nblev== -1)  970618*/
              if(nblev== 99) /*NOTE levels orthogonal to nbondrot*/ /*020305*/
              {/*prototype for monotonically increasing branches*/
                 /*e.g. along the mainchain of a protein*/
                 nblev = nbondrot+1; /*so nblev==1 when nbondrot==0*/
              }
              if(nbondrot >= 1 ) 
              {/*there are previous bond rotations */ 
/*use dummy noslider to end numbered bondrots sets within ganged sets 130119*/
                if(nblev > 0) /*restored 130119*/
                {/*end scope of any previous bondrots if necessary*/
                  thisbondrotlistptr = firstbondrotlistptr;
                  while(thisbondrotlistptr != NULL)
                  {
                     if(thisbondrotlistptr->level >= nblev)
                     {/*any lower level ended by start of higher rank*/
                        if(thisbondrotlistptr->lastpointptr == NULL) 
                        {/*scope not ended yet, end at last inputed point*/
                          /*thisbondrotlistptr->lastpointptr = lastpointptr;*/
                          thisbondrotlistptr->lastpointptr = 
                             (thislistptr->previousptr)->lastpointptr;
                          /*last point was in previous list*/
                        }
                     }
                     thisbondrotlistptr = thisbondrotlistptr->nextptr;
                  }
                }/*end scope of any previous bondrots if necessary*/
                else if(nblev== -1)
                {/*see if this is a new, or a continuing, ganged rotation*/
                  thisbondrotlistptr = firstbondrotlistptr;
                  while(thisbondrotlistptr != NULL)
                  {/*scan all bondrots*/
                     if(thisbondrotlistptr->level < 0)
                     {/*ganged rotations are levels -1, -2*/
                        if(strncmp(thislistptr->name
                                      ,thisbondrotlistptr->name
                                      ,MAXNAMECHAR             )  == 0 )
                        {/*previous ganged rotation has same name*/
                            nblev = -2; /*ganged to an earlier one*/
                        }/*previous ganged rotation has same name*/
                     }/*ganged rotations are levels -1, -2*/
                     thisbondrotlistptr = thisbondrotlistptr->nextptr;
                  }/*scan all bondrots*/
                  /*when no earlier name matches, nblev remains == -1 */
                }/*see if this is a new, or a continuing, ganged rotation*/
              }/*there are previous bond rotations */ 
              
              Lbondrotscope = 1;
                         
              if((thisbondrotlistptr = allocbondrotliststructure()) == NULL)
                                          /*MAGELIST*/
              {/*allocation failed*/
                   sprintf(alertstr,"failed to alloc %dth bondrotlist"
                       ,nbondrot+1);
                   alertstr2[0]='\0';alertstr3[0]='\0';
                   DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
              }/*allocation failed*/
              else
              {/*can construct another bondrotlist */
                nbondrot = nbondrot+1;
                bondrotptr[nbondrot] = thisbondrotlistptr;
                thisbondrotlistptr->origangle  = angle;
                thisbondrotlistptr->angle  = angle;
                
                if(Lbnoslider) thisbondrotlistptr->slider = 0; /*070617*/
                else           thisbondrotlistptr->slider = 1; /*070617*/

                if(Lbselect) thisbondrotlistptr->select = 1; /*071215*/
                else         thisbondrotlistptr->select = 0; /*071215*/

                if(Lbplot > 0) 
                {
                  if(Lbplot==1){LbondrotHplot = nbondrot;} /*970918  global*/
                  else if(Lbplot==2){LbondrotVplot = nbondrot;}/*970918global*/
                  else           {LbondrotZplot = nbondrot;} /*030819  global*/
                }
                sprintf(thisbondrotlistptr->name,"%s",thislistptr->name);
                
                thisbondrotlistptr->level = nblev;
                thisbondrotlistptr->firstpointptr = NULL;  
                    /*starting point of scope is expected after 2 points*/
                    /* that define the "bond"==axis of rotation */
                thisbondrotlistptr->lastpointptr = NULL; 
                    /*ending point of scope will be set when scope ends*/
                /*this presumes there will be something attached to rotate*/
                /* the end of scope will be reset later by a bondrot command*/
                thisbondrotlistptr->tailpointptr = NULL;
                /*starting point of bond will be set to the next input point*/
                thisbondrotlistptr->headpointptr = NULL;
                /*ending point of bond will be set to the 2nd next input point*/
                /*this is the vector of the rotation, its points don't change*/
                /* by its own action, note that the scope starts just beyond*/
                thisbondrotlistptr->scopeinstance = 0;

                /*NOTE: This structure holds information about two sets of*/
                /*points: those that define the axis, and those that define*/
                /*the scope.  BUT it does not create those points, and it */
                /*does NOT own those points: i.e. is NOT a parent */
 /*------------------*/               
                thisbondrotlistptr->option = 0; /*initialize option to N0P */
                if(Lbrotoption!=0) thisbondrotlistptr->option = Lbrotoption;
                /*1: use as param for previous option, e.g. precession angle*/
                /*2: link last 2 angles vs this one,   e.g. precession motion*/
                  /*   get order of rotations correct, or use samescope local*/
                  /*option with previos rotations. */
                  /*Linking is done in ___BAR/grafbondrotter()*/
                /*3:use selection by parameters on things in scope of this rot*/
                if(Lsamescope>0) 
                {/*give same scope to last rotation(s)*/
                    thebondrotlistptr = thisbondrotlistptr;
                    for(j=1;j<=Lsamescope;j++)
                    {
                        thebondrotlistptr = thebondrotlistptr->previousptr;
                        thebondrotlistptr->scopeinstance = nbondrot;
                    }
                }
 /*------------------*/
                thisbondrotlistptr->sgrpptr = thissgrpptr;
                /*set flags so rotated things can be identified & written out*/
                if(thisgrupptr->bondrot==0)
                {/*first rotation belonging to this group*/
                   thisgrupptr->bondrotmin = nbondrot;
                }
                thisgrupptr->bondrot = nbondrot; /*just T/F flag*/ /*020212max*/
                thissgrpptr->bondrot = nbondrot; /*just T/F flag*/ /*020212max*/
                thislistptr->bondrot = nbondrot;/*ID tied to specific rotation*/

                if(Lpattern == 1)
                {/*rotation restriction pattern has been read in*/
                    strcpy(bondrotpattern[nbondrot],bondrotpattern[0]);
                }/*rotation restriction pattern has been read in*/
                else bondrotpattern[nbondrot][0] = '\0'; /*so can know if present*/
                
              }/*can construct another point */
            }/*set up new bond rotation stuff*/
            else /*Lblev == 0, just an ending, or dummy to get prerot output*/
            {/*end all current bond rotation stuff*/
              if(firstbondrotlistptr != NULL && nbondrot >= 1)
              {/*there had been earlier bondrots defined*/
                thisbondrotlistptr = firstbondrotlistptr;
                while(thisbondrotlistptr != NULL)
                {
                  if(thisbondrotlistptr->lastpointptr==NULL) 
                  {/*scope not ended yet, end at last inputed point*/
                      /*thisbondrotlistptr->lastpointptr = lastpointptr;*/
                      thisbondrotlistptr->lastpointptr = 
                             (thislistptr->previousptr)->lastpointptr;
                      /*of course, no points are yet alloc for current*/
                      /* dummy list */
                  }
                  thisbondrotlistptr = thisbondrotlistptr->nextptr;
                }
                Lbondrotscope = 0;
              }/*there had been earlier bondrots defined*/
              thislistptr->bondrot = -1;
              /*flags dummy vectlist that ends rotations*/
              /*nbondrot==-1 for end-of-scope-dummy */
              /*NB: bondrotptr[thislistptr->bondrot]->level == -1,-2*/
              /*    for ganged rotations*/
              /*Or allow writerotated to output atoms preceeding real rotation*/
            }/*end all current bond rotation stuff*/
            Lbrot = 0; /*clear flag*/
          }/*enough bondrot ControlHandle typedef--> HWND(PC), -->Widget(MUX)*/ 
          else
          {/*warning*/
             sprintf(alertstr,"bondrot limit < %dth bondrotlist"
                       ,nbondrot+1);
             alertstr2[0]='\0';alertstr3[0]='\0';
             DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
          }/*warning*/

        }/*bondrot flag */
        else if(Lbondrotscope)  
        {/*____list imbedded in a rotation*/
            /*set flags so rotated things can be identified and written out*/
            thisgrupptr->bondrot = nbondrot; /*rather than 1; 020212*/    
            thissgrpptr->bondrot = nbondrot; /*rather than 1; 020212*/
            thislistptr->bondrot=100;/*flagged >0 but not a separate rotation*/   
        }/*____list imbedded in a rotation*/
        
        if(Ltest)  /*MAGE_3_2*/
        {
            sprintf(alertstr,"reached new list: %s, in subgrp: %s, in group:%s"
               ,thislistptr->name,thissgrpptr->name,thisgrupptr->name);
            dosinglealert(3); /*MACDLOG.C*/ /*MPCDLOG.C*/
        }
    }
  }/*internal first line info*/
  return(ireturn);
}
/*___listfirstline()________________________________________________________*/

/****interpretlistkeyword()**************************************************/
void    interpretlistkeyword()
{
   char trystr[3] = {'\0','\0','\0'};
   char wordstr[256];
   int  j=0;

    /*subroutine strncmp() is in MAGEINPT.c*/
    /*NOTE  char =  acts as white space and can end these parameters*/
    /*so only need to test for parent word, not varient ending with = */
    if(strncmp(word,"off",3) == 0) thislistptr->on = 0;
                                /* starts with display disabled */ 
    else if(strncmp(word,"-",1) == 0) thislistptr->on = 0; /*historic*/
    else if(strncmp(word,"*",1) == 0) thislistptr->on = 0; /*historic*/
    else if(strncmp(word,"nobutton",8) == 0) 
       thislistptr->STATUS = thislistptr->STATUS | NOBUTTON;
    else if(strncmp(word,"color",5) == 0)  Lcolnam = 1;
    else if(strncmp(word,"colour",6) == 0)  Lcolnam = 1;
    else if(strncmp(word,"instance", 8) == 0) Linstance = TRUE;
    else if(strncmp(word,"clone",5) == 0) Lclone = 1;
    else if(strncmp(word,"master", 6) == 0) Lmaster = TRUE;
         /* variables with scope of file MAGEVSUB.C: nblev,Lbrot,Lbang*/
    else if(strncmp(word, "bondrot",7) == 0)
    {
       nblev = -1; Lbrot = 1; Lbang = 0;
       Lbrotoption = 10; /*incompatible with other Lbrotoptions*/ /*020310*/
    }
    else if(isdigit(word[0]))
    {/*listkeyword starts with a number*/
       if(isdigit(word[1]))
       {
          for(j=0;j<256;j++)
          {
             wordstr[j] = word[j+2];
             if(wordstr[j]== '\0'){break;}
          }
          trystr[0] = word[0]; trystr[1] = word[1];
       }
       else
       {
          for(j=0;j<256;j++)
          {
             wordstr[j] = word[j+1];
             if(wordstr[j]== '\0'){break;}
          }
          trystr[0] = word[0];
       }
       if(strncmp(wordstr,"bondrot",7) == 0)
       {
          nblev= atoi(trystr);
          Lbrot=1;Lbang=0;
       }
    }/*listkeyword starts with a number*/
#ifdef OLDCODE
    else if(strncmp(word,"0bondrot",8) == 0){nblev= 0;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"1bondrot",8) == 0){nblev= 1;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"2bondrot",8) == 0){nblev= 2;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"3bondrot",8) == 0){nblev= 3;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"4bondrot",8) == 0){nblev= 4;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"5bondrot",8) == 0){nblev= 5;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"6bondrot",8) == 0){nblev= 6;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"7bondrot",8) == 0){nblev= 7;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"8bondrot",8) == 0){nblev= 8;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"9bondrot",8) == 0){nblev= 9;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"10bondrot",9) == 0){nblev= 10;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"11bondrot",9) == 0){nblev= 11;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"12bondrot",9) == 0){nblev= 12;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"13bondrot",9) == 0){nblev= 13;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"14bondrot",9) == 0){nblev= 14;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"15bondrot",9) == 0){nblev= 15;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"16bondrot",9) == 0){nblev= 16;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"17bondrot",9) == 0){nblev= 17;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"18bondrot",9) == 0){nblev= 18;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"19bondrot",9) == 0){nblev= 19;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"20bondrot",9) == 0){nblev= 20;Lbrot=1;Lbang=0;}
    else if(strncmp(word,"99bondrot",9) == 0){nblev= 99;Lbrot=1;Lbang=0;}
#endif
    else if(strncmp(word,"face", 4) == 0) Lwordsface = TRUE;
    else if(strncmp(word,"font", 4) == 0) Lwordsfont = TRUE;
    else if(strncmp(word,"size", 4) == 0) Lwordssize = TRUE;
    else if(strncmp(word,"radius", 6) == 0) Lradius=2;/*used in 2 places*/
    else if(strncmp(word,"width", 5) == 0) Lwidth=1;
    else if(strncmp(word,"angle", 5) == 0) Langle=1;

    else if(strncmp(word,"answer",6) == 0) 
    {
       answerlistptr = thislistptr; /*020830*/
       /*answer list from which to make answerkey: connect-the-dots timed test*/
    }
    else if(strncmp(word,"result",6) == 0) 
    {
       resultlistptr = thislistptr; /*020830*/
       /*list from drawnew of earlier mage run which to score vs answer list*/
    }
    else if(strncmp(word,"static",6) == 0) 
    {
        thislistptr->STATUS = thislistptr->STATUS | STATICFLAG;
        /*special display flag, static== no rotation, but scaled*/
    }
    else if(strncmp(word,"screen",6) == 0) 
    {
        thislistptr->STATUS = thislistptr->STATUS | STATICFLAG;
        thislistptr->STATUS = thislistptr->STATUS | SCREENFLAG;
        /*special display flag, screen== no rotation and fits on screen*/
        LSCREENpoints = 1; /*existence of such points 121212*/
    }
    else if(strncmp(word,"nozclip",7) == 0) 
        thislistptr->type = thislistptr->type | NOZCLIPFLAG;
        /*special display flag, no zclipping */
    else if(strncmp(word,"lens",4) == 0) 
    {
        thislistptr->type = thislistptr->type | LENSFLAG;
        /*special display flag, list points only show in center lens */
        LLensflagset = 1;  /*970917*/
    }
    else if(strncmp(word,"nohi",4) == 0) /*nohilite, nohilight, nohighlight*/ 
    {
        thislistptr->STATUS = thislistptr->STATUS | NOHILITEFLAG;
        /*NOTE Steiff bears (expensive European teddy bears) have a */
        /*gold earing, just like kinemage dancing bears*/
    }
    else if(strncmp(word,"nobl",4) == 0) /*noblkrim, noblackrim*/
    {
        thislistptr->STATUS = thislistptr->STATUS | NOBLKRIMFLAG;
    }
 /*----------*/
    /*thisbondrotlistptr->option = Lbrotoption*/
    else if(strncmp(word,"parameter",9) == 0) Lbrotoption =  1;
    else if(strncmp(word,"precession",10) == 0) Lbrotoption =  2;
    else if(strncmp(word,"selection",9) == 0) Lbrotoption =  3;
    else if(strncmp(word,"xrotation",9) == 0) Lbrotoption =  4;
    else if(strncmp(word,"yrotation",9) == 0) Lbrotoption =  5;
    else if(strncmp(word,"zrotation",9) == 0) Lbrotoption =  6;
    else if(strncmp(word,"xtranslation",12) == 0) Lbrotoption =  7;
    else if(strncmp(word,"ytranslation",12) == 0) Lbrotoption =  8;
    else if(strncmp(word,"ztranslation",12) == 0) Lbrotoption =  9;

    else if(strncmp(word,"samescope2",10) == 0) Lsamescope =  1; 
    else if(strncmp(word,"samescope3",10) == 0) Lsamescope =  2;
    else if(strncmp(word,"samescope4",10) == 0) Lsamescope =  3;
    else if(strncmp(word,"hplot",5) == 0) Lbplot =  1;
    else if(strncmp(word,"vplot",5) == 0) Lbplot =  2;
    else if(strncmp(word,"zplot",5) == 0) Lbplot =  3;
    else if(strncmp(word,"noslider",8) == 0) Lbnoslider =  1; /*070617*/
    else if(strncmp(word,"select",6) == 0) Lbselect =  1; /*071215*/

    else if(strncmp(word,"pattern",7) == 0) Lpattern =  2; /*2 for expected*/

    else if(strncmp(word,"resnumb",7) == 0) Lresnumb =  1; /*071005*/
    else if(strncmp(word,"resname",7) == 0) Lresname =  1; /*071005*/
 /*----------*/
    else if(strncmp(word,"detail",6) == 0) 
    {
        Levelofdetail = 1; /*global allows detail level choice*/
        thislistptr->type = thislistptr->type | DETAILFLAG;
    }
    else if(strncmp(word,"dimension" ,9) == 0) Ldimension = 1; /*060121*/
    else if(  (strncmp(word,"wrap" ,4) == 0) 
            ||(strncmp(word,"add" ,3) == 0) )
    {
       Lwrap = 1; 
       /*060123, was add 060121*/ /*060123 use wrap instead*/
       thislistptr->KKIND = thislistptr->KKIND | WRAPPINGKKIND; /*060525*/
       /*following number stored as thislistptr->fwrap */
    }
    else if(strncmp(word,"select" ,6) == 0) /*060121*/
    {
       thislistptr->KKIND = thislistptr->KKIND | SELECTINGKKIND;
       LSELECTINGlist = 1; /*global potentiates select-points-in-list*/
       LUnSELECTING = 1; /*global potentiates toggle-Unselected-points*/
       LoneSELECTING = 1; /*global potentiates select just one point*/
    }
    else if(strncmp(word,"average" ,7) == 0) /*061129*/
    {
       thislistptr->KKIND = thislistptr->KKIND | AVERAGEKKIND;
       thislistptr->KKIND = thislistptr->KKIND & ~SELECTINGKKIND; /*061201*/
    }
    else if(strncmp(word,"sigma" ,7) == 0) /*061129*/
    {
       thislistptr->KKIND = thislistptr->KKIND | SIGMASKKIND;
       thislistptr->KKIND = thislistptr->KKIND & ~SELECTINGKKIND; /*061201*/
    }
    else if(strncmp(word,"alternate" ,9) == 0) /*071005 for suitefit bases*/
    {
       thislistptr->KKIND = thislistptr->KKIND | ALTERNATEKKIND; /*071005*/
    }
    else if(strncmp(word,"rear",4) == 0) /*090703 for blk outline on worms*/
    {
        thislistptr->STYLE = thislistptr->STYLE | REARSTYLE;
        /*special display flag for vectors: zdepth at rear of 2 points*/
    }
    else if(strncmp(word,"fore",4) == 0) /*090703 for blk outline on worms*/
    {
        thislistptr->STYLE = thislistptr->STYLE | FORESTYLE;
        /*special display flag for vectors: zdepth at fore of 2 points*/
    }
    else if(strncmp(word,"dotball",7) == 0) /*100406 dotballs at each point*/
    {
        thislistptr->STYLE = thislistptr->STYLE | DOTBALLSTYLE; /*100406*/
        /*each point gives a center dot here and a dotlist in new group 100406*/
        /*branch to make dotball list just after addanothervector 100406*/
    }
    else if(strncmp(word,"endball",7) == 0) /*100407 last dotball */
    {
        thislistptr->STYLE = thislistptr->STYLE | ENDBALLSTYLE; /*100406*/
        /*each point gives a center dot here and a dotlist in new group 100406*/
        /*branch to make dotball list just after addanothervector 100406*/
    }
   
#ifdef OLDGANGEDOPTION /*020310*/
    else if(iword>=8 && strncmp(&word[iword-8],"rotation",8)==0 )
    {/*e.g. phirotation0   \0 in iword==11, r in iword-8 ==3*/
       /*make preceeding characters into a string, NULL if none*/
       iword = iword-8;
       word[iword] = '\0'; /*970623 not using this name*/
       Lbrotoption = 10;
    }
#endif
}
/*___interpretlistkeyword()_________________________________________________*/

/*#define TESTBALLS*/
/****addanothervector()******************************************************/
void    addanothervector(int Ndim)  /*060121 Ndim usually == 3 */
{
struct pointstruct* shortpointptr;
struct pointstruct* targetpointptr;
struct liststruct*  targetlistptr;
struct sgrpstruct*  targetsgrpptr;

        int     iendsearch;
        int     j;
        float   xx[4],xxx[4];
        float   fvectorx,fvectory,fvectorz,veclength;
static  float   hereradius,thereradius;
        float   usehereradius,usethereradius; 
        int     Lparended = 0;
static  int     nmess1=0;

/* duplicate triples are a bit complicated:  if a duplicate move point after*/
/* a draw  P a,b,c   L d,e,f   P d,e,f   L g,h,i   then don't do middle move*/
/*   so in a vectorlist;  if a move duplicates a draw, don't do the move    */
/* if a null vector:  P a,b,c   L a,b,c   then do the pair of move-draw so */
/*                                                    get the point */
/*   so in a vectorlist:  if a draw duplicate a move, do the draw */
/* if the sequence bridges over from one list to another:         */
/*  Vectorlist: P a,b,c   L d,e,f      Dotlist:  P d,e,f         */
/*   so if not a vectorlist:  do all specified moves &  draws         */
/* final logic: if vectorlist and if a move then don't do duplicate */
/*
fprintf(stderr,"addanothervector entered %ld times\n",++nmess1);
*/

    /*default radius 0.2, see ___DRAW/balltoscreen()*/
    if(pointradius < .0001) /*980804-011024*/
    { /*980804-011024*/
        if(thislistptr->radius < .0001) hereradius = (float).2;
        else hereradius = thislistptr->radius; /*make local so can override */
                               /*if gets a new radius from a balllist*/
    }
    else
    {
        hereradius = pointradius;
    } /*980804*/
    /*search over all previous lists in the current group to see if any */
    /*balls coincide with the current coordinates */
    /*BEWARE: since the current group/subgroup/list is actively growing */
    /* the test to end the search must trigger when the current list is */
    /* reached */
    /*BEWARE: only searching in the current group, targetgrupptr not needed*/

    if((Listtype==VECTOR||Listtype==ARROW) && thisgrupptr->balllists == 1)
    {/*balllist in this group*/
      /*060121 one hopes never to get here when Ndimension != 3 */
/*sprintf(mess2,"addanothervector ball search tried %ld times\n",++nmess2);*/
      iendsearch = 0;
      targetsgrpptr = thisgrupptr->firstsgrpptr;
      while(targetsgrpptr != NULL)
      {/*check subgroups*/   /*n indexes subgroups within this group*/
        if(targetsgrpptr->balllists != 0) /*is a balllist in this subgroup*/
        {/*balllist in this subgroup*/
          targetlistptr = targetsgrpptr->firstlistptr;
          /*presume lists in this subgroup are contiguous*/
          while(targetlistptr != NULL) 
          {/*check lists while targetlistptr != NULL*/
            if(targetlistptr == thislistptr)
            {
                iendsearch = 1;
                targetlistptr = NULL;
                break;  /*don't look for balls in the current list*/
            }


            targetpointptr = targetlistptr->firstpointptr;
            while(targetpointptr != NULL)
            {/*check list entries*/
                if(   ((targetpointptr->type)&BALL  ) )
                {/*a ball or sphere*/
                     if(   (    x[1] <= targetpointptr->fx+.001 
                             && x[1] >= targetpointptr->fx-.001)
                        && (    x[2] <= targetpointptr->fy+.001 
                             && x[2] >= targetpointptr->fy-.001)
                        && (    x[3] <= targetpointptr->fz+.001 
                             && x[3] >= targetpointptr->fz-.001) )
                     {/*close enough!*/
                         Lballhere = 1;
                         Lmustkeeppoint = 1;
                         /*Lunpickable = 1;*/ /*not forced here*/
                         iendsearch = 1;
                         /*default radius 0.2, see ___DRAW/balltoscreen()*/
                         /*already applied to hereradius, if radius not */
                         /* defined for this vectorlist */
                         if(targetlistptr->radius > .0001) 
                             /*radius of ball at point*/ 
                             hereradius = targetlistptr->radius; /* override */

#ifdef TESTBALLS
fprintf(stderr,"grup %s, sgrp %s, list %s, here: %.2f,%.2f,%.2f Lballhere %.2f, %.2f, %.2f\n"
,thisgrupptr->name, targetsgrpptr->name, targetlistptr->name,targetpointptr->type
, x[1],x[2],x[3]
,targetpointptr->fx,targetpointptr->fy,targetpointptr->fz
);
#endif
                     }
                }/*a ball*/
                if(iendsearch==1)targetpointptr=NULL;/*simulates end of points*/
                else if(targetpointptr == targetlistptr->lastpointptr) 
                    targetpointptr=NULL;
                else targetpointptr = targetpointptr->nextptr;
            }/*check list entries*/
            if(iendsearch==1) targetlistptr=NULL; /*simulates end of lists*/
            else if(targetlistptr == targetsgrpptr->lastlistptr) 
                targetlistptr=NULL;
            else targetlistptr = targetlistptr->nextptr;

            if(targetlistptr == thislistptr)
            {/*search for balls that match the current coord*/
                /*has caught up to the current list*/
                targetlistptr = NULL;
                iendsearch = 1;
            }

          }/*check lists while targetlistptr != NULL*/
        }/*balllist in this subgroup*/


        if(iendsearch==1)
        {
            targetsgrpptr=NULL; /*simulates end of subgroups*/
        }
        else if(targetsgrpptr == thisgrupptr->lastsgrpptr) 
        {
            targetsgrpptr=NULL;
        }
        else 
        {
            targetsgrpptr = targetsgrpptr->nextptr; 
        }
      }/*check subgroups while targetsgrpptr != NULL*/
    }/*balllist in this group*/
    if(   !Lmustkeeppoint && !Lnewlist && (Listtype==VECTOR) && Lmove 
       && (x[1] == x2) && (x[2] == y2) && (x[3] == z2) )
    {
        ;  /* skip */
    }
    else
    {/*--not duplicate vector-move triple, so enter it */
        
      if( (thispointptr = allocpointstructure(thislistptr))!=NULL)/*MAGELIST*/
      {/*actually can add another vector */
        thispointptr->number = pointnumber; /*atom order suitefit bases 071005*/
        if(pointradius > .0001) thispointptr->radius = pointradius;  /*980804*/
        /*bondrot, bond rotation, monitoring and set ups */
        if(Lbondrotscope) /*within scope of at least one bondrot*/
        {/*bondrot stuff*/ 
            thisbondrotlistptr = firstbondrotlistptr;
            while(thisbondrotlistptr != NULL)
            {                   
                /* 3 points make a hierarchical sequence,*/
                /* two define the rotation axis, the 3rd starts the scope */
                /* nested rotations can be imbedded !! */
                
                if(thisbondrotlistptr->tailpointptr == NULL)
                    thisbondrotlistptr->tailpointptr = thispointptr;
                else if(thisbondrotlistptr->headpointptr == NULL)
                    thisbondrotlistptr->headpointptr = thispointptr;
                else if(thisbondrotlistptr->firstpointptr == NULL)
                    thisbondrotlistptr->firstpointptr = thispointptr;
                
                /*thisbondrotlistptr->lastpointptr is defined when scope ends*/
                
                thisbondrotlistptr = thisbondrotlistptr->nextptr;
            }
        }/*bondrot stuff*/                    
        
        if(Lbeginselection)
        {
            beginselectionpointptr = thispointptr;
            Lbeginselection = 0;
        }


/*****definition of parent group by nature of this current point ************/
/*noticed this on 040202, but do not know why it is done for every point */
 
        if(Ninternal==3 || Ninternal==4 || Ninternal==5 )  /*egam 030217*/
        {
            referencegrupptr = thisgrupptr; /*egam 030215*/
        }
        if(Ninternal>=10 && Ninternal<20 && Legamickdocking)  /*egam 030217*/
        {
            mobilegrupptr = thisgrupptr; /*egam 030215*/
            /*thisgrupptr->bondrot = DOCKSET;*/  /*egam 030215*/
            thisgrupptr->bondrot = thisgrupptr->bondrot|DOCKSET;  /*040320*/
        }
        if(Ninternal>=20 && Ninternal<30 && Legamickdocking)  /*egam 030217*/
        {
            /*thisgrupptr->bondrot = DOCKSET;*/  /*egam 030215*/
            thisgrupptr->bondrot = thisgrupptr->bondrot|DOCKSET;  /*040320*/
        }
        /*if(Ninternal==31 && (Legamickdocking || Lfoocavity))*/ /*egam 030330*/
        if((Ninternal==31 || Ninternal==32) && (Legamickdocking || Lfoocavity))  /*rev 050816*/
        {
            foogrupptr = thisgrupptr;  /*egam 030223*/
        }

/*____definition of parent group by nature of this current point ____________*/

        
        /*do point ID stuff first, since might change nxyz to duplicate point*/
        /* ptIDstr holds either the pointID or a blank */
        /*or " == ditto*/  /*970514*/
        if(ptIDstr[0]=='\"' && ptIDstr[1]=='\0')
        {
           /*ptIDstr as ditto at thislistptr->firstpointptr ?from editing fatality*/
           if(thislistptr->firstpointptr != thispointptr)  /*990209*/
           {/*posible valid anticedent point*/           
              /*recoverptIDstring(ptIDstr,thispointptr->previousptr);*/ /*970920*/
              /*calls getptIDstring, if NULL, returns x,y,z in ptIDstr*/
              getptIDstring(ptIDstr,thispointptr->previousptr); /*970929*/
              /*just replicate a NULL, use recover of xyz just as screen display*/
           }
        }
        storeptIDstring(ptIDstr, thispointptr); /*MAGELIST*/
        if(iparen > 0)
        {/*store parenstr as aspects of this point*/
           Lparended = 0;
           for(j=0; j<MAXPOINTASPECTS+1; j++)
           {/*storage limitation on number of aspects*/
              if(parenstr[j] == '\0')
              {/*blank from here on to end of possible aspects*/
                 Lparended = 1;
              }
              if(Lparended)
              {
                 thispointptr->aspects[j] = ' ';
              }
              else
              {
                 thispointptr->aspects[j] = parenstr[j];
                 if(j+1 > maxaspectsperpoint) {maxaspectsperpoint = j+1;}
              }
           }
        }
        
        if(Lmustkeeppoint && Lcomment) /*for a comment or wordlist*/
        {
/*
k=-1;do{k++;temps[k]=commentstr[k+3];}while(temps[k]!='\0'); 
printf("possible comment %d,%d,%d<%s>\n"
,commentstr[0],commentstr[1],commentstr[2],temps);
*/

            storecommentstring(commentstr, thispointptr); /*MAGELIST*/
/*
recoverptIDstring(word,thispointptr);  
getcommentstring(temps,thispointptr); 
k=-1;do{k++;commentstr[k]=temps[k+3];}while(commentstr[k]!='\0'); 
printf("{%s} %d,%d,%d<%s>\n"
,word,(int)temps[0],(int)temps[1],(int)temps[2],commentstr);
*/
        }
        
        /*Now set type and STYLE flags for this point, see MAGEFLAG.h*/
        
    /*Iitialize Lmove by Lnew for first point of any list */
    /*This is very important, since  MOVETO_P flag is then*/
    /*robustly on the first point of any list and pure successive*/
    /*point searches can avoid flowing into adjacent lists*/

        /* -1 set so an empty vector list will not try to display*/
        if(thislistptr->on == -1) thislistptr->on = 1; /*turn on*/
        Lnewlist = FALSE;
        /*if(Lunpickable) n = 1;*/ /*960513*/
        /*else n = 0;*/

        if      (Listtype==WORDS)           thispointptr->type= WORDS;/*971003*/
        else if((Listtype==LABEL)&&Lradius) thispointptr->type=(LABEL|VARIANT1);
        else if(Listtype==LABEL)            thispointptr->type=LABEL;
        else if(Listtype==DOT)              /*incl STYLE=DOTBALLSTYLE  100406*/            
        {
                                            thispointptr->type=DOT;
            if(Liststyle==DOTBALLSTYLE)     thispointptr->STYLE=DOTBALLSTYLE;
        }
        else if(Listtype==MARKTYPE)
        {
                                            thispointptr->type=MARKTYPE;

            if(Liststyle==RINGSTYLE)        thispointptr->STYLE=RINGSTYLE;
            else if(Liststyle==FANSTYLE)    thispointptr->STYLE=FANSTYLE;
        }
        else if(Listtype==BALL)             thispointptr->type=BALL;
        else if(Listtype==SPHERE)           thispointptr->type=SPHERE;

        /*otherwise, some sort of VECTOR based object. i.e. defined by >=2 pts*/

        else if(Listtype==TRIANGLE) /*060523 triangle controls rearranged*/
        {
           thispointptr->type=TRIANGLE;
           if(Lmove)
           {
              thispointptr->type=(thispointptr->type|MOVETO_P);
              /*so P,L can be used if Levelofdetail precludes solid triangles*/
           }
           if(Lstartpt) /*060523 e.g. starting pt of an independent triangle*/
           {
              thispointptr->STATUS = (thispointptr->STATUS| STARTFLAG);
           }
        }
        else if((Listtype==RIBBON)&&Lmove)  
                                  thispointptr->type=(RIBBON|MOVETO_P);
        else if(Listtype==RIBBON)         thispointptr->type=RIBBON;
            /*so P,L can be used if Levelofdetail precludes a solid ribbon*/
            /*ribbon builds with triangles but lighted as rectangles*/
            /*use cross-tie vectors to define triangles and their rectangles*/
        else if(Lmove)    thispointptr->type=(VECTOR|MOVETO_P); /* move  */
        else              thispointptr->type=VECTOR;    /* draw  */

        /*from here on, thispointptr->type should only be modified with |  */

        if( Lemphasison && Listtype==VECTOR) /*020112xrc*/
        {
             thispointptr->STYLE = thispointptr->STYLE|EMPHASIS;/*020113xrc*/
        }  
        if( Liststyle==ELLIPSOID && Listtype==VECTOR) /*080406*/
        {
             thispointptr->STYLE = ELLIPSOID;/*080406*/
             /*P center, L major axes radii, multiD 080406 ellipsoidtoscreen()*/
        }  
        if(  (Liststyle==PIXELATED && Listtype==TRIANGLE)  /*080912*/
           ||(Liststyle==PIXELATED && Listtype==RIBBON)  ) /*080912*/
        {
             thispointptr->STYLE = PIXELATED;/*080912*/
             /*area filled with z-buffered dots 080912*/
        }  

        if(Listtype==ARROW) thispointptr->type=thispointptr->type|VARIANT3; /*000804*/
        /*but ARROW head only makes sense on an L type vector point*/
        
        if(Lunpickable)
        {
            thispointptr->type= (thispointptr->type| UNPICKABLE);
        }
        if(Ltriangle)
        {
          thispointptr->type= (thispointptr->type| TRIANGLE);
          if((thispointptr->previousptr) != NULL) /*010517*/
          {
            if(  (((thispointptr->previousptr)->type & TRIANGLE) != TRIANGLE)
               || Lstartpt)  /*060523 X for start of independent triangle*/
               thispointptr->STATUS = (thispointptr->STATUS| STARTFLAG);
               /*New list also starts of a set of triangles*/
               /*so do not have to protect against previous list last point*/
          }
        }
#ifdef EXPLICITBALL
        if(Lballpoint)
        {
            thispointptr->type= (thispointptr->type| BALL);
        }
#endif
        if(Lsphere)
        {
            thispointptr->type= (thispointptr->type| SPHERE | MOVETO_P);
        }
        if(Larrow)
        {
            thispointptr->type= (thispointptr->type| ARROW);
        }
        if(Lsquare)
        {   /*screen oriented square is the primative MARK-TYPE*/
            thispointptr->type= (thispointptr->type| MARKTYPE | MOVETO_P);
        }
        if(Lring)
        {
            thispointptr->type= (thispointptr->type| MARKTYPE | MOVETO_P);
            thispointptr->STYLE = (thispointptr->STYLE | RINGSTYLE);
        }

        if(   Listtype==SPHERE || Listtype==TRIANGLE || Listtype==RIBBON
           ||(Listtype==ARROW) )
            Levelofdetail = 1;
        /*so why is a list level setting embedded at the point input level????*/
        /*Maybe so can drop down if rotation rate too slow, i.e. */
        /*sphere-->ball, triangle,ribbon-->vectors, arrow-->vector*/
        
        if(Ldummy)
        {/*DUMMY points are NOT used in scaling, nor displayed*/
            thispointptr->STATUS = (thispointptr->STATUS|DUMMY);
        }
        if(Lghost)
        {/*GHOST points are seen but NOT real for special output, e.g. PDB*/
            thispointptr->STATUS = (thispointptr->STATUS|GHOST);
        }
        if(Lnoxyz) /*as of 000327 NOXYZ redundant with DUMMY*/
        {/*flag as having no valid x,y,z and not to be used in scaling*/
            thispointptr->STATUS = (thispointptr->STATUS|NOXYZ|DUMMY);
        }

        if( (thislistptr->STATUS & STATICFLAG) == STATICFLAG) 
        {
           thispointptr->STATUS = (thispointptr->STATUS | STATICFLAG);
        }
        if( (thislistptr->STATUS & SCREENFLAG) == SCREENFLAG) 
        {
           thispointptr->STATUS = (thispointptr->STATUS | SCREENFLAG);
        }

        /*point color*/   /*960121*/
        if(pointcolor > 0)
        {
           thispointptr->colorwidth = pointcolor;
           if(Lexclamation)
              thispointptr->STYLE = thispointptr->STYLE | FORCEPTCOLOR;
        }
        else thispointptr->colorwidth = 0;
        
        if(pointmasterstr[0] != '\0')  /*991210,001002,030425*/
        {
           thispointptr->master = MASKpointmaster;
           if(IONpointmaster) thispointptr->STATUS = thispointptr->STATUS | ON;
           else               thispointptr->STATUS = thispointptr->STATUS &~ON;
           thislistptr->STATUS = thislistptr->STATUS | HASPTMASTER;
        }
        if(Legamickdocking && thisgrupptr==foogrupptr) /*030323*/
        {/*subvert thispointptr->number for foo neighbor indicator flags*/
           thispointptr->number = 0; /*i.e. clear flags*/
           /*note: also subverted for suitefit nucleotide atom order 071005*/
        }
        
       if(Ndim == 3)  /*060121 NDIM, Ndimension*/
       {/*Ndim == 3 only */
        /*--------------- local rotation and centering ----------------*/
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
       }/*Ndim == 3 only */
       else
       {/*Ndim != 3 060121, protected earlier so >3 here  */
          /*alloc storage for extra dimensions, & fill x,y,z with default set*/
          thispointptr->coordNptr =(float*)calloc((size_t)(Ndim),sizeof(float));
                      /*coordNptr 060622,061028*/
          if(thispointptr->coordNptr != NULL)
          {/*copy values*/
             for(j=0; j<Ndim; j++)
             {
                thispointptr->coordNptr[j] = x[j+1];
             }
             if(NX >= Ndim) {NX = 0;} /*Ndim 1..., NX 0...  060622*/
             if(NY >= Ndim) {NY = 1;}
             if(NZ >= Ndim) {NZ = 2;}
             xx[1] = thispointptr->coordNptr[NX];
             xx[2] = thispointptr->coordNptr[NY];
             xx[3] = thispointptr->coordNptr[NZ];
          }
          else
          {
             xx[1] = x[1];
             xx[2] = x[2];
             xx[3] = x[3];
          }
       }/*Ndim != 3 */

        thispointptr->fx = xx[1];
        thispointptr->fy = xx[2];
        thispointptr->fz = xx[3];
        if(Lscale)
        {/*get xyz scope for scaling*/
           if(  ((thispointptr->STATUS & DUMMY)!=DUMMY)
              &&((thispointptr->STATUS & SCREENFLAG)!=SCREENFLAG) )
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
           else
           {/*DUMMY point not used for min, max limits*/
            /*screen point not used for min, max limits*/
            /*static point is used for min, max limits*/
              ;        
           }
        }/*get xyz scope for scaling*/

        if(!(thisgrupptr->KKIND & DIMENSIONKKIND))  /*060604*/
        {/*shorten lines for balls only in usual 3D 060604*/
        
#ifdef TESTBALLS
fprintf(stderr,"alloc new point %ld:type==%d,%.3f,%.3f,%.3f, Lmove=%d,HERE=%d,THERE=%d\n"
,thispointptr
,thispointptr->type
,thispointptr->fx
,thispointptr->fy
,thispointptr->fz
,Lmove
,Lballhere
,Lballthere);
fprintf(stderr,"max,min xyz: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n"
,fxmax,fxmin,fymax,fymin,fzmax,fzmin);
#endif
   
          /* LOOKING BACKWARDS FROM THIS POINT TO THE PREVIOUS POINT: */
          /*A "P" (MoveTo) point doesn't have a line from a previous point*/

          if( !Lmove && (Lballhere || Lballthere) ) /*DrawTo with a ball*/
            /*presume a previous move-to point from which to shorten*/
          {/*drawn line needs shortening*/
           /*unfortunately there are three logical possibilities for this*/
           /*point: The problem is the case of a "Lballthere" so an invented*/
           /* point is needed as the previous point.  The only safe thing */
           /*is for any such possible point to already have been made. Then*/
           /*if not used, it is an isolated "P" which is not displayed */
           shortpointptr = thispointptr;
           if(Lballhere)
           {/*build a new draw-to point so can truncate this one*/

             if((thispointptr = allocpointstructure(thislistptr)) == NULL)/*MAGELIST*/
             {/*allocation failed*/
                 sprintf(alertstr,"failed to alloc a shortenline point");
                 sprintf(word," ");
                 errorreport(); /*alertstr and word*/ /*MAGEVECT.c*/
             }/*allocation failed*/
             else 
             {/*can construct another point */
                nxyz = nxyz + 1; /*keep track of number of points*/
                /*copy from previous point*/
                thispointptr->fx = (thispointptr->previousptr)->fx;
                thispointptr->fy = (thispointptr->previousptr)->fy;
                thispointptr->fz = (thispointptr->previousptr)->fz;
                thispointptr->ix = (thispointptr->previousptr)->ix;
                thispointptr->iy = (thispointptr->previousptr)->iy;
                thispointptr->iz = (thispointptr->previousptr)->iz;
                thispointptr->colorwidth 
                    = (thispointptr->previousptr)->colorwidth;
                
                /*assign new point as a stored point, */
                /*with same characteristics as the original point*/
                thispointptr->type
                     = (thispointptr->previousptr)->type;
                thispointptr->type  = ((thispointptr->type) | STORED) ; 
                                           /*Optional Display*/ 
                thispointptr->STATUS
                     = (thispointptr->previousptr)->STATUS;
                     
                thispointptr->radius = (thispointptr->previousptr)->radius; /*980804*/                                        
/*printf("B: %d, store new point: radius: %f at %.3f, %.3f, %.3f\n",LBallflag,thispointptr->radius*/
/*,thispointptr->fx,thispointptr->fy,thispointptr->fz);*/ /*980804*/  
                if(LBallflag)
                {/*LBallflag*/ /*980804 much rearrangment as well as new Ball at point stuff*/
                    (thispointptr->previousptr)->type
                              =(((thispointptr->previousptr)->type)|BALLHERE); 
                    /*Forces Display to use this point, balls or no balls*/ 
                    /*Ball flag expects a Ball to appear and need shortened line*/
                    /*this "previous pt" is a displayed draw point, not for output*/
                    
                    thispointptr->type 
                        = ((thispointptr->type)|BALLHERE);
                    /* BALLHERE claims a BALL always here so Never Display*/
                    /*a line into this STORED center place.*/  
                    /*980804 instead, try to show a ball in this place */
                    if(thispointptr->radius > .0001)
                    {/*Force a Ball here*/
/*030626*/            thispointptr->type = ((thispointptr->type)|BALL|MOVETO_P);
                       /*Ball remains PICKABLE*/
                       /*980804*/ /*Ball gets point color, vectors get list color*/
                       (thispointptr->previousptr)->colorwidth = 
                          (thispointptr->previousptr)->colorwidth & ~31; /*strip point color*/
                    }/*Force a Ball here*/
                    /*NOTE: if NOT a ball here, then irrelevant if PICKABLE*/
                  
                }/*LBallflag*/ /*980804*/
                /*In any case, assign the old point as a modifiable, displayable L point*/
                /*this was already a DrawTo, make it UNPICKABLE and */
                /*flag it as an INVENTED point for MAGE display only */
                (thispointptr->previousptr)->type 
                   = (((thispointptr->previousptr)->type)|UNPICKABLE|INVENTED);  
                     /*allows Optional dismissal if noballs are in group */
                /*980804 end*/
                storeptIDstring(ptIDstr, thispointptr); /*MAGELIST*/
                    /* use same point Id for this new point */
                thispointptr->wordoffset 
                        = (thispointptr->previousptr)->wordoffset;
                /*same comment for this invented point, copy index*/
                
                Lmove = 1; /*this is now a move for the next point*/
                
                /*idea is to shorten line from the just duplicated point*/
                /*back to its previous one, this point to be involved in */
                /*shortening is now previous to the current point */
                /*Remember this:*/
                shortpointptr = thispointptr->previousptr;
#ifdef TESTBALLS 
fprintf(stderr,"alloc extra  -draw to- here point %ld: %d, %.3f, %.3f, %.3f\n"
,thispointptr
,thispointptr->type
,thispointptr->fx
,thispointptr->fy
,thispointptr->fz);
#endif           
             
             }/*can construct another point */
           }/*build a new draw-to point so can truncate this one*/
           
           /*Now one does NOT know how many, if any extra points were made*/
           /*since defining the two that may need a shortened line*/
           /* so have to trust that shortpointptr has been set to the most*/
           /*recent of them */
           {/*shorten line between last two entered triples and update*/
               fvectorx = shortpointptr->fx - (shortpointptr->previousptr)->fx;
               fvectory = shortpointptr->fy - (shortpointptr->previousptr)->fy;
               fvectorz = shortpointptr->fz - (shortpointptr->previousptr)->fz;
               veclength = (float)sqrt(fvectorx*fvectorx +
                                     fvectory*fvectory +
                                     fvectorz*fvectorz  );
     /*beware of points too close together!*/
     if(  (veclength > 0.000001) )/*this protects from divide-by-zero*/
     {/*vector long enough for shortening to be meaningful*/
       /*Now check working radius*/
       if(   ( Lballthere &&  Lballhere && (veclength>thereradius+hereradius))
           ||( Lballthere && !Lballhere && (veclength>thereradius)           )
           ||(!Lballthere &&  Lballhere && (veclength>hereradius)            )
         )
       {
          usethereradius = thereradius; /*vector will show between balls*/
          usehereradius  =  hereradius; /*vector will show between balls*/
       }
       else 
       {
           usethereradius = veclength/3; /*need point but line won't show*/
           usehereradius  = veclength/3; /*need point but line won't show*/
       }
      /*960813*/
      /*basic problem with rotation definitions which depend on anticipated*/
      /*index of future (first two) points and with rotation scope which */
      /*starts at index anticipated to be just past the two points that*/
      /*define the rotation axis.  Since rotation scope is sometimes */
      /*extended backwards to earlier rotations, it is not simple to mess*/
      /*with the defining and starting indicies when extra points are created*/
      /*to deal with the shortened vectors for balls and spheres.*/
      /*The easiest patch is to insist that once a new point is created for*/
      /*a shortened ine, that it be used - i.e. that there be a Non-Zero */
      /* interval between the first two points of a list so a rotation axis*/
      /*if needed, will be defined.   Scope of rotation is OK since the extra*/
      /*points between what was originally anticipated as the first vector*/
      /*are in-line on the axis that defines the rotation - and so are */
      /*invarient to that rotation anyway.  */ 
             
               /*normalize: */
               fvectorx = fvectorx/veclength;
               fvectory = fvectory/veclength;
               fvectorz = fvectorz/veclength;
               /*add to n-1, subtract from n to make */
               /*shortened line end positions, where n is previousptr*/
               if(Lballthere) 
               {
                     (shortpointptr->previousptr)->fx 
                   = (shortpointptr->previousptr)->fx 
                    + usethereradius*fvectorx;
                     (shortpointptr->previousptr)->fy 
                   = (shortpointptr->previousptr)->fy 
                    + usethereradius*fvectory;
                     (shortpointptr->previousptr)->fz 
                   = (shortpointptr->previousptr)->fz 
                    + usethereradius*fvectorz;
#ifdef TESTBALLS 
fprintf(stderr,"Lballthere changed there point %ld: %d, %.3f, %.3f, %.3f\n"
,(shortpointptr->previousptr)
,(shortpointptr->previousptr)->type
,(shortpointptr->previousptr)->fx
,(shortpointptr->previousptr)->fy
,(shortpointptr->previousptr)->fz);
#endif   
                 /* reload integer display list */
                 if(  (shortpointptr->previousptr)->fx>=0.0) 
                      (shortpointptr->previousptr)->ix
                     =(int)((shortpointptr->previousptr)->fx+.5);
                 else
                      (shortpointptr->previousptr)->ix
                     =(int)((shortpointptr->previousptr)->fx-.5);
                         
                 if(  (shortpointptr->previousptr)->fy>=0.0) 
                      (shortpointptr->previousptr)->iy
                     =(int)((shortpointptr->previousptr)->fy+.5);
                 else
                      (shortpointptr->previousptr)->iy
                     =(int)((shortpointptr->previousptr)->fy-.5);
                         
                 if(  (shortpointptr->previousptr)->fz>=0.0) 
                      (shortpointptr->previousptr)->iz
                     =(int)((shortpointptr->previousptr)->fz+.5);
                 else
                      (shortpointptr->previousptr)->iz
                     =(int)((shortpointptr->previousptr)->fz-.5);
               }
               if(Lballhere) 
               {
                 /* where thispointptr->previousptr is nth point*/
                 shortpointptr->fx = shortpointptr->fx - usehereradius*fvectorx;
                 shortpointptr->fy = shortpointptr->fy - usehereradius*fvectory;
                 shortpointptr->fz = shortpointptr->fz - usehereradius*fvectorz;

#ifdef TESTBALLS 
fprintf(stderr,"Lballhere changed  here point %ld: %d, %.3f, %.3f, %.3f\n"
,thispointptr->previousptr
,shortpointptr->type
,shortpointptr->fx
,shortpointptr->fy
,shortpointptr->fz);
#endif   
                 /* reload integer display list */
                 /*j = n;*/ /*980610*/
                 /*index into data struct: j and n now not used */ /*980610*/
                 if( shortpointptr->fx>=0.0) 
                     shortpointptr->ix
                    =(int)(shortpointptr->fx+.5);
                 else
                     shortpointptr->ix
                    =(int)(shortpointptr->fx-.5);
                    
                 if( shortpointptr->fy>=0.0) 
                     shortpointptr->iy
                    =(int)(shortpointptr->fy+.5);
                 else
                     shortpointptr->iy
                    =(int)(shortpointptr->fy-.5);
                    
                 if( shortpointptr->fz>=0.0) 
                     shortpointptr->iz
                    =(int)(shortpointptr->fz+.5);
                 else
                     shortpointptr->iz
                    =(int)(shortpointptr->fz-.5);
               }
             }/*vector long enough for shortening to be meaningful*/
           }/*shorten line between last two entered triples and update*/
          
          }/*drawn line needs shortening*/
        
        /* LOOKING FORWARDS FROM THIS POINT TO A PUTATIVE NEXT POINT: */
        
          if(Lballhere &&  Lmove) /*note creating stored point sets Lmove=1*/
          {/*set up to shorten the vector from here when get drawn-to-point */
            /*The logically easy case if it is a "P" with */
            /*no earlier point that is expecting to end a vector here*/
            /*However, if this point is an "L" (DrawTo) it might NOT get*/
            /*a following point to drawto - leaving a dangling invented */
            /* point.  This will not be displayed, but could foul up a */
            /* rotation axis definition!! */
            Lballthere = 1;/* flag that line is to be shortened*/
            thereradius = hereradius;
            if((thispointptr = allocpointstructure(thislistptr)) == NULL)/*MAGELIST*/
            {/*allocation failed*/
                sprintf(alertstr,"failed to alloc a shortenline point");
                sprintf(word," ");
                errorreport(); /*alertstr and word*/ /*MAGEVECT.c*/
            }/*allocation failed*/
            else 
            {/*build a new move-to point to be truncated later*/
                /*n = nxyz;*/ /*current last entry at this point number*/ /*980610*/
                /*index into data struct: j and n now not used */ /*980610*/
                nxyz = nxyz + 1; /*keep track of number of points*/
                /*copy from the previous point*/
                thispointptr->fx = (thispointptr->previousptr)->fx;
                thispointptr->fy = (thispointptr->previousptr)->fy;
                thispointptr->fz = (thispointptr->previousptr)->fz;
                thispointptr->ix = (thispointptr->previousptr)->ix;
                thispointptr->iy = (thispointptr->previousptr)->iy;
                thispointptr->iz = (thispointptr->previousptr)->iz;
                thispointptr->colorwidth 
                    = (thispointptr->previousptr)->colorwidth;

                /*assign the new point as a modifiable, displayable P point*/
                thispointptr->type 
                    = (VECTOR | MOVETO_P | UNPICKABLE | INVENTED); 
                                         /*unpickable (U), Optional display*/
                thispointptr->radius  = (thispointptr->previousptr)->radius;/*980804*/

                if(LBallflag) thispointptr->type  
                     = ((thispointptr->type) | BALLHERE); 
                    /*Forced to be displayed if ever part of a vector*/
                
                /*assign old point as stored point,*/
                /* flagged by original attributes, which will resurface*/
                /*if this kin is written out as a "modified" kin*/
                (thispointptr->previousptr)->type  
                     = (((thispointptr->previousptr)->type) | STORED); 
                
                if(LBallflag)
                {/*LBallflag*/ /*980804*/
                    (thispointptr->previousptr)->type  
                         = (((thispointptr->previousptr)->type) | BALLHERE);
                    /* BALLHERE claims a BALL always here so Never Display*/
                    /*a line into this STORED center place*/  
                    /*980804 instead, try to show a ball in this place */
                    if((thispointptr->previousptr)->radius > .0001)
                    {/*force a Ball here*/
                       (thispointptr->previousptr)->type 
/*030626*/               = (((thispointptr->previousptr)->type)|BALL|MOVETO_P);
                       /*Ball remains PICKABLE*/
                       /*Need this if original point was a MoveTo*/
                       /*If original point was a DrawTo, then this "previous pt"*/
                       /*is already set to type|BALL */
                       /*In this case, the Ball will get point color */
                       /*Any continuing, shortened line will get list color*/
                       thispointptr->colorwidth = thispointptr->colorwidth & ~31;/*strip off color*/
/*printf("B: %d, confirm stored pt: radius: %f at %.3f, %.3f, %.3f\n"*/
/*,LBallflag,(thispointptr->previousptr)->radius*/
/*,(thispointptr->previousptr)->fx,(thispointptr->previousptr)->fy,(thispointptr->previousptr)->fz);*/ /*980804*/  
                    }/*force a Ball here*/   
                    /*NOTE: if NOT a ball here, then irrelevant if PICKABLE*/ 
                    /*980804*/
                }/*LBallflag*/ /*980804*/

                
                
                
                
                /* use same point Id for this new point */
                storeptIDstring(ptIDstr, thispointptr); /*MAGELIST*/

                thispointptr->wordoffset 
                    = (thispointptr->previousptr)->wordoffset;
                    /*hse same comment for this invented point*/
                Lmove = 1; /*this is now a move for the next point*/
#ifdef TESTBALLS 
fprintf(stderr,"alloc extra  -draw from- here point %ld: %d, %.3f, %.3f, %.3f\n"
,thispointptr
,thispointptr->type
,thispointptr->fx
,thispointptr->fy
,thispointptr->fz);
fprintf(stderr,"and changes type of point %ld: %d, %.3f, %.3f, %.3f\n"
,(thispointptr->previousptr)
,(thispointptr->previousptr)->type
,(thispointptr->previousptr)->fx
,(thispointptr->previousptr)->fy
,(thispointptr->previousptr)->fz);
#endif   

            }/*build a new move-to point to be truncated later*/
          }/*set up to shorten the vector from here when get drawn-to-point */
          else Lballthere = 0; /*clears flag in all other cases*/
        }/*shorten lines for balls only in usual 3D 060604*/

        Lballhere = 0; /*must be flagged for each point*/
        LBallflag = 0; /*must be flagged for each point*/
        pointradius = 0; /*must be brought in for each point*/ /*980804*/
        pointnumber = 0; /*must be brought in for each point*/ /*071005*/

        x2 = x[1]; y2 = x[2]; z2 = x[3];
        nxyz++;        /* increment counter for next vector */        
        if(Lplotonly)
        {
            nxyz--;  /* restore counter */
            Lplotpoint = TRUE;
            redrawvec();  /* plot point, point not stored! */
            Lplotpoint = FALSE;
        }                                                             
      }/*end actually adding another vector */
      else
      {/*allocation failed, can't add another vector */
          sprintf(alertstr,"failed to alloc point # %ld" /*121108 long*/
                          CRLF"ptID string: %s",nxyz,ptIDstr);
          sprintf(word,"No further warnings, expect truncated image");
          errorreport();/*alertstr and word*/ /*MAGESETS.c*/
          Lexcessivepoints = 1;
      }/*allocation failed, can't add another vector */

    }/*--not duplicate vector-move triple, so enter it */
}
/*___addanothervector()_____________________________________________________*/
