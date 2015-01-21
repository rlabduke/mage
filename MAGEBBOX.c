/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEANGL.h"

#define EXTERNBBOX
#include "MAGEBBOX.h"
#undef  EXTERNBBOX
#include "MAGEMENU.h"
#include "MAGEFLAG.h"
#include "MAGETABL.h"
#include "MAGEFOO.h"

int nbox_of_probedotsgrup;
static int nn_size_button;
/*so can delete and update appended groups with minimal button work*/


/****SetsinGrafWindow()******************************************************/
void    SetsinGrafWindow()   /* called from Mac Main.c or PC MENUCMD3.C*/
{
  /* set up check box in on-screen graphics window */
  /* Mac defines a button in terms of a handles to the created button*/
  /* PC defines a button in terms of an index to the created button*/
  /*  PC index is treated like a menucommand ID number: */
  /*    MM_BUTTONINDEX  to  (MM_BUTTONINDEX + MAXG + MAXXG + MAXS + MAXL) */
  /* UNIX PEX uses Widget gadgets as toggle buttons, indexed:  */
  /* lists as nlist, subgroups as nset+MAXL, groups as ngrp+MAXS+MAXL */
  /* i.e. PEX indexing is backwards from PC indexing.  This lets the PEX */
  /* name for picking structures use the list indexed by nlist as the */
  /* simple way */
  /*   920920 maximum extra groups now 8 not 7 so define MAXXG = 8 */
  /* this index has to be worked out here where it is known what kind of */
  /* button, group, subgroup (set), or list, that is being created */

  int              top,right,left,bottom,nbox,nn,indexPC,i,indent;
  int              high,boxhigh,Lcycle,LGO,jlist;
/*  ControlHandle    buttonbox;*/ /*PC typedef as HANDLE, PEX typedef Widget */
  char             name[MAXNAMECHAR+2];
  static int       iyspacer;
  
  LneedSetsinGrafWindow = 0; /*030320 clear flag*/

if(Ltest) 
   {sprintf(alertstr,"MAGEBBOX/SetsinGrafWindow entered\n"); dosinglealert(3);}

  getbuttonplace();       /*___BBOX.C*/ 
  left   = brect.left;
  right  = brect.right;
  top    = brect.top;
  bottom = brect.bottom;
  high = bottom - top;  /*001118 avoid writing off bottom of window*/
  /*Mac gets continual update requests once control boxes written beyond window area*/
if(LTBstereo && Lstereo) bottom = bottom/2;

  
  if(IPC == 1) nn = 17; /* Mac uses 15, but PC font needs more space */
  /*else if(IUX == 1) nn = 17;*/ /*UNIX Helvetica-Bold-R-Normal--14*/
  else if(IUX == 1) 
  {
     /*nn = 22;*/ /*UNIX Helvetica-Bold-R-Normal--14*/
     nn = 28; /*UNIX Helvetica-Bold-R-Normal--18*/
     if(Lmacularfont || Lforcedfont==4) /*031025*/
     {
        nn = 32; /*try for 10x20 or 12x24*/
     }
  }
  else nn = 15;
  indent = 5; /*indentation per level*/
  /* reasonable spacing for button boxes, compressed later if needed */
  jlist = 0;
  Lcycle = 1;
  LGO = 0; /*font adjusted for available space, only make button on 2nd pass*/
  while(Lcycle == 1)
  /*allow button boxes to get stuffed closer if many of them*/
  {/*Lcycle loop*/

    killgrafcontrols(); /*MAGEBBOX.C*/
    destroyallbboxstructures();  /*061128*/

    strcpy(name,"            ");
    mybuttonspacer(left,iyspacer,name); /*___BBOX.c*/


    nbox = 0;    /* absolute box number for spacing boxes on screen */
    nanimate = 0; 
    nnanimate = 0;
    nn_size_button = nn; /*save to refresh append groups in place 030319*/
     
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*groups*/
      /*count actual animation groups here, before any filtering*/
      if(thisgrupptr->animate == 1) nanimate++; 
      if(thisgrupptr->animate == 2) nnanimate++;
      if(  (  ((thisgrupptr->STATUS & NOBUTTON) == 0)
            ||(thisgrupptr->STATUS & RECESSIVEON)    )
         &&( (thisgrupptr->type & TABLEFLAG) == 0    ) )
      {/*make box for this group*/
       /*not a nobutton group with no buttons at all, or permitted table group*/
        nbox++;
        /*indexPC = ngrp; orphan */
        indexPC = 1;
        if(nbox*nn <= high) /*001118 avoid writting off bottom of window*/
        if(LGO){makebuttonbox(
          &(thisgrupptr->button)/*Handle: needed by Mac && X11, not used by PC*/
          ,left              /*int left: x , used straight*/
          ,top               /*int top:  y , not used: top = nbox*nn */
          ,right             /*int right: Mac right, PC width*/
          ,bottom /*NOT USED!!!! int bottom: Mac bottom, PC height=nn */
          ,thisgrupptr->name /*char name MAXNAMECHAR+2 == 14+2*/
          ,thisgrupptr->on   /*int on-off flag */
          ,nbox              /*int number of this box*/
          ,nn                /*int pixel height of box*/
          ,indexPC           /*int index to PC buttons as menu cmds*/
          );}
        if(thisgrupptr->KKIND == PROBEDOTSKKIND)
        {
           nbox_of_probedotsgrup = nbox; /*to refresh probe group in place*/ 
        }
      }/*make box for this group*/
      
      /*now see if any buttons are to be made for this group's subgroups*/
      if(  (  (  ((thisgrupptr->STATUS & DOMINANT   ) == 0)
               &&((thisgrupptr->STATUS & RECESSIVEON) == 0) )
            || ((thisgrupptr->STATUS & RECESSIVEON) && thisgrupptr->on) )
         &&( (thisgrupptr->type & TABLEFLAG) == 0                       ) )
      {/*not a dominant group hiding subgroups and lists, nor part of table*/
          /*recessiveon hides subgroups and lists when OFF*/
          thissgrpptr = thisgrupptr->firstsgrpptr;
          while(thissgrpptr != NULL)
          {/*subgroups*/
            if(   ((thissgrpptr->STATUS & NOBUTTON) == 0) 
               || (thissgrpptr->STATUS & RECESSIVEON)     )
            {/*make box for this subgroup*/
              nbox++;
              /*indexPC = MAXG+MAXXG + nset; orphan*/
              indexPC = 1;
              if(nbox*nn <= high)
              if(LGO){makebuttonbox(&(thissgrpptr->button)    /*____BBOX.c*/
                ,left+indent,top,right,top
                ,thissgrpptr->name,thissgrpptr->on,nbox,nn,indexPC);}
                /*subgroup name: MAXNAMECHAR+1 == 14+1*/
            }/*make box for this subgroup*/
            
            /*now see if any buttons are to be made for this subgroup's lists*/
            if(  (  ((thissgrpptr->STATUS & DOMINANT   ) == 0)
                  &&((thissgrpptr->STATUS & RECESSIVEON) == 0) )
               || ((thissgrpptr->STATUS & RECESSIVEON) && thissgrpptr->on) ) 
            {/*not dominant subgroup, no hidden lists*/
              /*recessiveon hides lists when OFF*/
              /*NOTE: set nobutton allows list buttons*/
              thislistptr = thissgrpptr->firstlistptr;
              while(thislistptr != NULL)
              {/*lists*/

                if((thislistptr->STATUS & NOBUTTON) == 0)
                {/*not nobutton*/
                  jlist++;
                  nbox++;
                  if(nbox*nn <= high)
                  if(LGO){makebuttonbox(&(thislistptr->button) /*____BBOX.c*/
                    ,left+2*indent,top,right,top
                    ,thislistptr->name,thislistptr->on,nbox,nn,indexPC);}
                    /*list name: MAXNAMECHAR+1 == 14+1*/
                }/*not nobutton*/
                if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
                else thislistptr = thislistptr->nextptr;
              }/*lists*/
            }/*not dominant subgroup, no hidden lists*/
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
          }/*subgroups*/
      }/*not a dominant group hiding subgroups and lists*/
 
      thisgrupptr = thisgrupptr->nextptr;
      while(    thisgrupptr != NULL 
             && (    thisgrupptr == markergrupptr 
                  || thisgrupptr == tablemarkergrupptr
                  || thisgrupptr == measuregrupptr
                  || thisgrupptr == drawgrupptr   ) )
      {
          thisgrupptr = thisgrupptr->nextptr;
      }
      /*avoid placing boxes arbitrarily for these special "groups" */
      /* no matter what order the groups are actually defined*/
    }/*groups*/

    if(Ldrawstuff) /*create draw new group button here, see below for controls*/
    {/*draw new group box*/

        nbox++;
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(drawgrupptr->button)     /*____BBOX.c*/
          ,left,top,right,top
          ,drawgrupptr->name  /*name*/
          ,drawgrupptr->on /*on-off state of new group*/ 
          ,nbox,nn,indexPC);}
        if(drawgrupptr->animate==1) nanimate++; 
        if(drawgrupptr->animate==2) nnanimate++;
    }/*draw new group box*/

    if(nmaster > 0)
    {
      nbox++; /*blank spacer between groups and master buttons */

      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
        nbox++;
        nbox = nbox + (thismasterptr->space); /*add an interval for each space*/
        indexPC =1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(thismasterptr->button)     /*____BBOX.c*/
            ,left   + indent*(thismasterptr->indent) /*int left: x */ /*991210*/
            ,top          /*int top:  y */
            ,right                          /*int right: Mac right, PC width*/
            ,bottom /*NOT USED!!!! int bottom: Mac bottom, PC height*/
            ,thismasterptr->name  /*char name */
            ,thismasterptr->on    /*int on-off flag */
            ,nbox              /*int number of this box*/
            ,nn                /*int pixel height of box*/
            ,indexPC           /*int index to PC buttons as menu cmds*/
             );}
        thismasterptr = thismasterptr->nextptr;
      }
    }
    nbox++; /*spacer before operations buttons */
    iyspacer = top + ((nbox)*nn); /*put spacer here at iyspacer, drawn below*/

    if(Lmarkers)  /* create marker button, as markergrp==maxgrp+1 */
    {/*this substitutes for the usual group button that markers avoid*/
        nbox++;
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(markergrupptr->button)    /*____BBOX.c*/
          ,left,top,right,top,markergrupptr->name
          ,markergrupptr->on,nbox,nn,indexPC);}
    }
    if(Ltablemarkers)  /* create tablemarker button, as markergrp+1 */
    {/*these are really just cummulative markers,but first use was with tables*/
        nbox++;
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(tablemarkergrupptr->button)    /*____BBOX.c*/
          ,left,top,right,top,tablemarkergrupptr->name
          ,tablemarkergrupptr->on,nbox,nn,indexPC);}
    }

    /* create pickcenter button after marker button */
    if(Lpickcenter)
    {
        if(pickbboxptr == NULL)
        {
            allocbboxstructure();
            pickbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Pickcenter");   /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(pickbboxptr->button)   /*____BBOX.c*/
          ,left,top,right,top,name,Lpickcenteron,nbox,nn,indexPC);}
    }
    /*create table pick on/off button next to pickcenter button*/
    if(Ltablepresent)
    {
        if(picktablebboxptr == NULL)
        {
            allocbboxstructure();
            picktablebboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"PickToTable");   /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(picktablebboxptr->button)   /*____BBOX.c*/
          ,left,top,right,top,name,Ltablepickactive,nbox,nn,indexPC);}
    }

    /* create zclip after pickcenter & marker buttons */
    if(Lzclip)
    {
        if(zclipbboxptr == NULL)
        {
            allocbboxstructure();
            zclipbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"zclip");  
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(zclipbboxptr->button)     /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lzclipon,nbox,nn,indexPC);}
    }
    /* create onscreeninfo button after pickcenter,marker,zclip buttons */
    if(Lonscreeninfo)
    {
        if(onscreeninfobboxptr == NULL)
        {
            allocbboxstructure();
            onscreeninfobboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"axes info");  
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(onscreeninfobboxptr->button)     /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lonscreeninfoon,nbox,nn,indexPC);}
    }
    /*create measure after zclip, pickcenter, & marker buttons*/
    if(Lmeasures)
    {
        nbox++;
        strcpy(name,"Measure");     /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(measuregrupptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lmeasureson,nbox,nn,indexPC);}
    }
    if(Lpickshow)
    {
        if(pickshowbboxptr == NULL)
        {
            allocbboxstructure();
            pickshowbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Edit Properties");  /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(pickshowbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,Lpickshowon,nbox,nn,indexPC);}
    }
    if(Lmovepoints) /*061126*/
    {
        if(movepointsbboxptr == NULL)
        {
            allocbboxstructure();
            movepointsbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Redef Groupings");  /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(movepointsbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,Lmovepointson,nbox,nn,indexPC);}
    }
    if(Lmeansigmas) /*061126*/
    {
        if(meansigmasbboxptr == NULL)
        {
            allocbboxstructure();
            meansigmasbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"MeanSigmas");  /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(meansigmasbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,Lmeansigmason,nbox,nn,indexPC);}
    }
    if(Lpickcolor)
    {
        if(pickcolorbboxptr == NULL)
        {
            allocbboxstructure();
            pickcolorbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Changecolor");   /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(pickcolorbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,Lpickcoloron,nbox,nn,indexPC);}
    }
    if(Lemphasis)
    {
        if(emphasisbboxptr == NULL)
        {
            allocbboxstructure();
            emphasisbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Emphasis");   /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(emphasisbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,Lemphasison,nbox,nn,indexPC);}
    }
    if(Ldrawstuff)  /* create draw stuff & eraseline buttons after */
        /* measure, zclip, pickcenter, & marker buttons */
    {/*draw new stuff*/

      if(Ldock3on3) /*ref: Ldockingscope */  /*020310*/
      {
          if(dock3on3bboxptr == NULL) /*020305*/
             { allocbboxstructure(); dock3on3bboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," Dock3on3");  /*c*/ 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(dock3on3bboxptr->button) /*____BBOX.c*/
                ,left,top,right,top,name,Ldock3on3on,nbox,nn,indexPC);}
      }
      if(Legamickchoice && !Ltetramers) /*ref: egamickdocking */  /*030217*/
      {
          if(tetrabboxptr == NULL) /*030217*/
             { allocbboxstructure(); tetrabboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," tetrahedron"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(tetrabboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==1,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(lionbboxptr == NULL) 
             { allocbboxstructure(); lionbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," lion"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(lionbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==1,nbox,nn,indexPC);}
      }
      if(Legamickchoice) /*ref: egamickdocking */  /*030217*/
      {
          if(cubebboxptr == NULL) /*030217*/
             { allocbboxstructure(); cubebboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," cube"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(cubebboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==2,nbox,nn,indexPC);}
      }
      if(Legamickchoice && !Ltetramers) /*ref: egamickdocking */  /*030217*/
      {
          if(octabboxptr == NULL) /*030217*/
             { allocbboxstructure(); octabboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," octahedron"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(octabboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==3,nbox,nn,indexPC);}
      }
      if(Legamickchoice && !Ltetramers) /*ref: egamickdocking */  /*030217*/
      {
          if(widgetbboxptr == NULL) /*030217*/
             { allocbboxstructure(); widgetbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," widget"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(widgetbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==4,nbox,nn,indexPC);}
      }
      if(Legamickchoice && !Ltetramers) /*ref: egamickdocking */  /*091127*/
      {
          if(waterbboxptr == NULL) /*091127*/
             { allocbboxstructure(); waterbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," water"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(waterbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==5,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(knightbboxptr == NULL) 
             { allocbboxstructure(); knightbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," knight"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(knightbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==3,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(knavebboxptr == NULL) 
             { allocbboxstructure(); knavebboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," knave"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(knavebboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==4,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(caltropbboxptr == NULL) 
             { allocbboxstructure(); caltropbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," caltrop"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(caltropbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==5,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(wallbboxptr == NULL) 
             { allocbboxstructure(); wallbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," wall"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(wallbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==7,nbox,nn,indexPC);}
      }
      if(Legamickchoice && Ltetramers) /*ref: egamickdocking */  
      {
          if(extrabboxptr == NULL) 
             { allocbboxstructure(); extrabboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," extra"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(extrabboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==6,nbox,nn,indexPC);}
      }
      if(Legamickchoice) /*ref: egamickdocking */  /*030217*/
      {
          if(rejectbboxptr == NULL) /*030217*/
             { allocbboxstructure(); rejectbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," reject"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(rejectbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,Negamickshape==0,nbox,nn,indexPC);}
      }
      if(LegamickrepeatOK) /*ref: egamickdocking */  /*030223*/
      {
          if(repeatbboxptr == NULL) /*030223*/
             { allocbboxstructure(); repeatbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," repeat"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(repeatbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,0,nbox,nn,indexPC);}
      }
      if(Legamickfoo || Lfoocavity) /*ref: egamickdocking */  /*030223*/
      {
          if(foofillbboxptr == NULL) /*030223*/
             { allocbboxstructure(); foofillbboxptr = thisbboxptr; }
          nbox++;
          strcpy(name," wake foo"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(foofillbboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,0,nbox,nn,indexPC);}
          if(foosurfacebboxptr == NULL) /*030329*/
             { allocbboxstructure(); foosurfacebboxptr = thisbboxptr; }
          nbox++;
          strcpy(name,"surface foo"); 
          indexPC = 1; 
          if(nbox*nn <= high)
             if(LGO){makebuttonbox(&(foosurfacebboxptr->button) /*____BBOX.c*/
               ,left,top,right,top,name,0,nbox,nn,indexPC);}
          if(Lfoocavity)
          { 
             if(foosealerbboxptr == NULL) /*030411*/
                { allocbboxstructure(); foosealerbboxptr = thisbboxptr; }
             nbox++;
             strcpy(name,"seal foo"); 
             indexPC = 1; 
             if(nbox*nn <= high)
                if(LGO){makebuttonbox(&(foosealerbboxptr->button) /*____BBOX.c*/
                  ,left,top,right,top,name,Lfoosealeron,nbox,nn,indexPC);}
          }
      }
        
      if(Lmeasureddihedral)
      {
         if(drawdihedralbboxptr == NULL)
         {
            allocbboxstructure();
            drawdihedralbboxptr = thisbboxptr;
         }
         nbox++;
         strcpy(name," Dihedrals"); /*c*/
         indexPC = 1;
         if(nbox*nn <= high)
         if(LGO){makebuttonbox(&(drawdihedralbboxptr->button) /*____BBOX.c*/
           ,left,top,right,top,name,Lmeasureddihedralon,nbox,nn,indexPC);}
      }
      if(Lnewlabels)
      {
         if(drawlabelsbboxptr == NULL)
         {
            allocbboxstructure();
            drawlabelsbboxptr = thisbboxptr;
         }
         nbox++;
         strcpy(name," Labels"); /*c*/
         indexPC = 1;
         if(nbox*nn <= high)
         if(LGO){makebuttonbox(&(drawlabelsbboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Lnewlabelson,nbox,nn,indexPC);}
      }
      if(Lnewballs)
      {
         if(drawballsbboxptr == NULL)
         {
            allocbboxstructure();
            drawballsbboxptr = thisbboxptr;
         }
         nbox++;
         strcpy(name," Balls"); /*c*/
         indexPC = 1;
         if(nbox*nn <= high)
         if(LGO){makebuttonbox(&(drawballsbboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Lnewballson,nbox,nn,indexPC);}
      }
      if(Lnewlines)
      {
         if(drawlinebboxptr == NULL)
         {
             allocbboxstructure();
             drawlinebboxptr = thisbboxptr;
         }        
         nbox++;
         strcpy(name," Drawline"); /*c*/ 
         indexPC = 1;
         if(nbox*nn <= high)
         if(LGO){makebuttonbox(&(drawlinebboxptr->button)  /*____BBOX.c*/
             ,left,top,right,top,name,Lnewlineson,nbox,nn,indexPC);}
      }  
      if(LbondrotHplot == 0 && LbondrotVplot == 0)
      {/*when plotting screen-fixed angles, drawing of lines fails*/
       /*labels work, and can be erased*/
        if(Lconstructline)
        {
           if(construct4bboxptr == NULL) /*971122*/
           {
              allocbboxstructure();
              construct4bboxptr = thisbboxptr;
           }
           nbox++;
           strcpy(name," Construct4");  /*c*/ 
           indexPC = 1; 
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(construct4bboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Lconstruct4on,nbox,nn,indexPC);}
        
           if(construct5bboxptr == NULL) /*971122*/
           {
              allocbboxstructure();
              construct5bboxptr = thisbboxptr;
           }
           nbox++;
           strcpy(name," Construct5");  /*c*/ 
           indexPC = 1; 
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(construct5bboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Lconstruct5on,nbox,nn,indexPC);}
        
           if(construct6bboxptr == NULL) /*140912*/
           {
              allocbboxstructure();
              construct6bboxptr = thisbboxptr;
           }
           nbox++;
           strcpy(name," Construct6");  /*c*/ 
           indexPC = 1; 
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(construct6bboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Lconstruct6on,nbox,nn,indexPC);}
           
//re constitutive   if(Lparamtrigger) /*do or redo most recent parameter lines drawings*/
           {
              if(paramtriggerbboxptr == NULL)
              { 
                 allocbboxstructure();
                 paramtriggerbboxptr = thisbboxptr;
              }
              nbox++;
              strcpy(name,"paramtrigger"); /*c*/ 
              indexPC = 1;
              if(nbox*nn <= high)
              if(LGO){makebuttonbox(&(paramtriggerbboxptr->button)/*____BBOX.c*/
              ,left,top,right,top,name,Lparamtriggeron,nbox,nn,indexPC);}/*lastparams*/
           }
        }
        if(Ldragline)
        {
           if(draglinebboxptr == NULL)
           {
              allocbboxstructure();
              draglinebboxptr = thisbboxptr;
           }        
           nbox++;
           strcpy(name," Dragline");   /*c*/ 
           indexPC = 1;
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(draglinebboxptr->button) /*____BBOX.c*/
             ,left,top,right,top,name,Ldraglineon,nbox,nn,indexPC);}
        }
        
        if(Lmonitor) /*last line length*/
        {
            if(monitorbboxptr == NULL)
            {
                allocbboxstructure();
                monitorbboxptr = thisbboxptr;
            }
            nbox++;
            strcpy(name,"monitor"); /*c*/ 
            indexPC = 1;
            if(nbox*nn <= high)
            if(LGO){makebuttonbox(&(monitorbboxptr->button) /*____BBOX.c*/
              ,left,top,right,top,name,Lmonitoron,nbox,nn,indexPC);}/*lastline*/
        }


      }/*when plotting screen-fixed angles, drawing of lines fails*/
       /*labels work, and can be erased*/

      if(eraselinebboxptr == NULL)
      {
            allocbboxstructure();
            eraselinebboxptr = thisbboxptr;
      }        
      nbox++;
      strcpy(name,"eraselast"); /*c*/ 
      indexPC = 1;
      if(nbox*nn <= high)
      if(LGO){makebuttonbox(&(eraselinebboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,0,nbox,nn,indexPC);}

      if(Lpickerase)
      {
         if(pickerasebboxptr == NULL)
         {
            allocbboxstructure();
            pickerasebboxptr = thisbboxptr;
         }        
         nbox++;
         strcpy(name,"PickEraseNew"); /*c*/ 
         indexPC = 1;
         if(nbox*nn <= high)
         if(LGO){makebuttonbox(&(pickerasebboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,0,nbox,nn,indexPC);}
      }
    }/*draw new stuff*/

    if(Lprune)  /* create prune, punch, undo buttons after */
        /* measure, zclip, pickcenter, & marker buttons */
        /* , & any draw-erase-construct line buttons */
    {
        if(prunebboxptr == NULL)
        {
            allocbboxstructure();
            prunebboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Prune"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(prunebboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lpruneon,nbox,nn,indexPC);}
        
        if(punchbboxptr == NULL)
        {
            allocbboxstructure();
            punchbboxptr = thisbboxptr;
        }        
        nbox++;
        strcpy(name,"Punch"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(punchbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lpunchon,nbox,nn,indexPC);}
        if(augerbboxptr == NULL)
        {
            allocbboxstructure();
            augerbboxptr = thisbboxptr;
        }        
        nbox++;
        strcpy(name,"Auger"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(augerbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lsuperpunchon,nbox,nn,indexPC);}
        if(undopbboxptr == NULL)
        {
            allocbboxstructure();
            undopbboxptr = thisbboxptr;
        }        
        nbox++;
        strcpy(name,"undo p"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(undopbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,0,nbox,nn,indexPC);}
    }

    if(LSELECTINGlist)  /*L7 LNdimensions create select-points-in-group button*/
        /* after measure, zclip, pickcenter, & marker buttons */
        /* , & any draw-erase-construct line buttons, & prune buttons */
    {
        if(SELECTINGlistbboxptr == NULL)
        {
            allocbboxstructure();
            SELECTINGlistbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"Select pts"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(SELECTINGlistbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)LSELECTINGliston,nbox,nn,indexPC);}
    }
    
    if(LoneSELECTING) /*L7 LNdimensions create select just one button */
    {
        if(oneSELECTINGbboxptr == NULL)
        {
            allocbboxstructure();
            oneSELECTINGbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"just one pt"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(oneSELECTINGbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)LoneSELECTINGon,nbox,nn,indexPC);}
    }
    
    if(LUnSELECTING) /*L7 LNdimensions create toggle-un-points-in-group button*/
    {
        if(UnSELECTINGbboxptr == NULL)
        {
            allocbboxstructure();
            UnSELECTINGbboxptr = thisbboxptr;
        }
        nbox++;
        strcpy(name,"UnSelected"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(UnSELECTINGbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,(int)LUnSELECTINGon,nbox,nn,indexPC);}
    }

    if(nanimate > 1)  /* create animate button, after other buttons */
    {
        if(animbboxptr == NULL)
        {
            allocbboxstructure();
            animbboxptr = thisbboxptr;
        }
        nbox++;
        if(Lcompareon) strcpy(name,"switch"); /*c*/
        else strcpy(name,"ANIMATE"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(animbboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,0,nbox,nn,indexPC);}
    }
    if(nnanimate > 1)  /* create animate button, after other buttons */
    {
        if(anim2bboxptr == NULL)
        {
            allocbboxstructure();
            anim2bboxptr = thisbboxptr;
        }
        nbox++;
        if(Lcompareon) strcpy(name,"2switch"); /*c*/
        else strcpy(name,"2ANIMATE"); /*c*/
        indexPC = 1;
        if(nbox*nn <= high)
        if(LGO){makebuttonbox(&(anim2bboxptr->button) /*____BBOX.c*/
          ,left,top,right,top,name,0,nbox,nn,indexPC);}
    }
    if(Lcontrol)
    {
        if(controlbboxptr == NULL)
        {
            allocbboxstructure();
            controlbboxptr = thisbboxptr;
        }
        nbox++;
        indexPC = 1;
        if(Lpipeactive) /*991216*/
        {
           strcpy(name,"next frame");     /*c*/
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(controlbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lpipebufferloaded,nbox,nn,indexPC);}
        }
        else
        {
           strcpy(name,"Control");     /*c*/
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(controlbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lcontrolon,nbox,nn,indexPC);}

           if(switchbboxptr == NULL)
           {
              allocbboxstructure();
              switchbboxptr = thisbboxptr;
           }
           nbox++;
           indexPC = 1;
           strcpy(name,"Switch"); /*090419 HelixBearHair xrays to film*/
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(switchbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Lcontrolon,nbox,nn,indexPC);}
        }
    }
    if(Linhibitbutton)
    {
        if(inhibitbboxptr == NULL)
        {
            allocbboxstructure();
            inhibitbboxptr = thisbboxptr;
        }
        nbox++;
        indexPC = 1;
        if(Lpipeactive)  /*991216*/
        {
           strcpy(name,"1 frame mode");     /*c*/
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(inhibitbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Linhibiton,nbox,nn,indexPC);}
        }
        else
        {
           strcpy(name,"Inhibit");     /*c*/
           if(nbox*nn <= high)
           if(LGO){makebuttonbox(&(inhibitbboxptr->button)  /*____BBOX.c*/
          ,left,top,right,top,name,(int)Linhibiton,nbox,nn,indexPC);}
        }
    }
    
    strcpy(name," -----------");
    mybuttonspacer(left,iyspacer,name); /*___BBOX.C*/

    if(nbox == 0) nbox = 1;
    
    boxhigh = (bottom - top)/nbox; /* needed interval per button box*/
    if(IPC) boxhigh = (bottom - top)/(nbox+1);
        /*PC measures from top of Char, needs one more space allocated*/
#ifdef UNIX_X11
    if(Lfontinfo)
    {
      printf("interval(nn:%d) vs boxhigh(%d)==(bottom(%d)-top(%d)%d)/nbox(%d)\n"
       ,nn,boxhigh,bottom,top,bottom-top,nbox); 
    }
#endif
    if(boxhigh < nn)  /*interval needed less than what used, try again*/
    {/*try again to fit buttons into panel*/
        /*while( boxhigh < nn) nn = nn -1;*/
        /*while( boxhigh <= nn) nn = nn -1;*/
        nn = boxhigh;

        if(boxhigh < 12 && Ltest)
        {
            sprintf(alertstr,"%d buttons more than fill screen,"
                " expect squirrelly buttons",nbox);
          
            sprintf(alertstr2,"boxhigh = (bottom - top)/nbox,"
                "\015 %d = (%d - %d)/%d"
                , boxhigh,bottom,top,nbox);
            sprintf(alertstr3," ");
#ifndef UNIX_X11
            DoMageDLOGreport(2); /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
#else
            printf("%s\n%s\n",alertstr,alertstr2);
#endif

        }
        nn = boxhigh;
        Lcycle = 1;
    }/*try again to fit buttons into panel*/
    else if(LGO)
    {/*made button, stop cycling*/
       Lcycle = 0;
       LGO = 0;
    }
    else
    {/*do another cycle and make the buttons*/
       Lcycle = 1;
       LGO = 1; /*flag to actually make buttons*/
    }
  }/*Lcycle loop*/

  /*Mac needs these below, since it kills all controls each time, */
  /*PC creates once, by a call to NewSliderWindows() from MPCINIT.C */
  /* then dummies these create...s in MPCBAR.C */
  /*UNIX creates once, uses the same names as Mac routines */
#ifdef MAYBEMACINTOSH
/*m*/               KillControls(grafWindow);
#endif
  if(Lzoom)
  {
#ifdef MACINTOSH
        creategrafzoomBar(); /*___BAR.c*/
#endif
        resetgrafzoomBar();  /*___BAR.c*/
  }

  if(Lzclip)
  {
#ifdef MACINTOSH
        creategrafzclipBar(); /*___BAR.c*/
#endif
        resetgrafzclipBar(izclipold[0]);  /*___BAR.c*/ /*970915*/
  }
  if(Lztran)
  {
#ifdef MACINTOSH
        creategrafztranBar(); /*___BAR.c*/
#endif
     if(Lzlimitscope)
     {/*subvert ztran slider for use with zlimited clipping of selected lists*/
        resetgrafztranBar((int)(izlimit/fzclip));  /*___BAR.c*/ /*011201*/
          /*fudge by ratio of actual window size to std kin value*/
     }
     else
     {/*original ztran use*/
        resetgrafztranBar(iztranold[0]);  /*___BAR.c*/ /*970915*/
     }
  }
#ifdef MACINTOSH
  placewindowresizemark();  /*MACBBOX.C, dummied in MPCBBOX.C*/
#endif

  if(nbondrot > 0)
  {
       for(i=1; i<=nbondrot; i++)
       {
          if(bondrotptr[i]->level > 0 || bondrotptr[i]->level== -1)
          {/*independent rotation or first of a ganged set of rotations*/
            erasebondrotvalue(i); /*dummied in UNIX*/
            /*removes any previous kinemage bond rotation text*/

/*#ifdef UNIX_MOTIF 980930 can't destroy because not created at first pass*/
            /*XtDestroyWidget(hgrafbondrot[i]);*/ /*980930*/
            /*XtDestroyWidget(bondrotvalue[i]);*/
            /*XtDestroyWidget(bondrotstart[i]);*/ /*950617*/
            /*XtDestroyWidget(bondrotname[i]);*/ /*950617*/
/*#endif*/
            /*if(hgrafbondrot[i]==NULL) remove test 070617 */ /*030821*/
            { /*always recreate so can change corseness of slider steps 070617*/
               creategrafbondrotBar(i); /*___BAR.c*/
            }
            resetgrafbondrotBar(i);  /*___BAR.c*/
          }
       }
  }
  /*masters no longer slaved to aggregate state of items*/
  /*instead, they have their own idenity, and the items are turned off */
  /*if any one master is off, and a master cannot turn on an item unless*/
  /*all masters of that item agree on being ON */  /*980920*/
  /*each master has a mask position based on "nmaster"*/
  
  
  if(nanimate > 1) 
  {
         Lcompare = 1;
         /*Lcompareon set ==0 initially, may later be set ==1 before here*/
         if(Lcomparestart) 
         {
             Lcompareon = 1;
             Lcomparestart = 0; /*so compare can be turned off later*/
             adjustanimate(0); /*turns off all animate marked groups*/
             adjustanimate(1); /*sets up first two as a comparison*/
         }
         if(Lstereo) Lcompareon = 0; /*Lstereo incompatible with Lcompareon*/
         /*myenableitem(),mycheckitem()  MPCMENU.C, MACMENU.C */
         mycheckitem(optiMenu, compareItem, Lcompareon ); /*check or uncheck*/
         if(Lstereo) myenableitem(optiMenu, compareItem, 0); /*disable*/
         else        myenableitem(optiMenu, compareItem, 1); /*enable*/
         if(Lcompareon) 
         {   
             myenableitem(optiMenu, stereoItem, 0); /*disable stereo*/
         }
  }
  else
  {
         Lcompare = 0;
         Lcompareon = 0;
         mycheckitem( optiMenu, compareItem, 0); /*uncheck compare*/
         myenableitem(optiMenu, compareItem, 0); /*disable compare*/
         myenableitem(optiMenu, stereoItem, 1); /*enable stereo*/
  }
if(Ltest) {sprintf(alertstr,"about to leave SetsinGrafWindow()\n");
 dosinglealert(3);}
}
/*___SetsinGrafWindow()_____________________________________________________*/

/****UpdateAppendedSets()****************************************************/
void UpdateAppendedSets(unsigned int Nkind)
{
  int              top,right,left,bottom,nbox,nn,indexPC;
  getbuttonplace();       /*___BBOX.C*/ 
  left   = brect.left;
  right  = brect.right;
  top    = brect.top;
  bottom = brect.bottom;

    nn = nn_size_button;/*remember button height of last regular button making*/

    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*groups*/
      if(thisgrupptr->KKIND & Nkind)
      {/*the right kind and not a nobutton group with no buttons at all*/
        nbox = nbox_of_probedotsgrup; /*to refresh probe group in place*/ 
        /*indexPC = ngrp; orphan */
        indexPC = 1;
        
        makebuttonbox(
          &(thisgrupptr->button)/*Handle: needed by Mac && X11, not used by PC*/
          ,left              /*int left: x , used straight*/
          ,top               /*int top:  y , not used: top = nbox*nn */
          ,right             /*int right: Mac right, PC width*/

          ,bottom  /*NOT USED !!!! int bottom: Mac bottom, PC height=nn */
          ,thisgrupptr->name /*char name */
          ,thisgrupptr->on   /*int on-off flag */
          ,nbox              /*int number of this box*/
          ,nn                /*int pixel height of box*/
          ,indexPC           /*int index to PC buttons as menu cmds*/
          );

        if((thisgrupptr->STATUS & DOMINANT) == 0)
        {/*not a dominant group hiding subgroups and lists*/
          thissgrpptr = thisgrupptr->firstsgrpptr;
          while(thissgrpptr != NULL)
          {/*sets*/
            if(  ((thissgrpptr->STATUS & NOBUTTON) == 0)
               || (thissgrpptr->STATUS & RECESSIVEON)    )
            {
              nbox++;
              /*indexPC = MAXG+MAXXG + nset; orphan*/
              indexPC = 1;

              makebuttonbox(&(thissgrpptr->button)    /*____BBOX.c*/
                ,left+5,top,right,top
                ,thissgrpptr->name,thissgrpptr->on,nbox,nn,indexPC);
            }
            /* 0 recessive, 1 dominant, 2 recessive&&nobutton*/
            /*             , 3 dominant&&nobutton*/  /*960305*/
            if((thissgrpptr->STATUS & DOMINANT) == 0) 
            {/*not dominant subgroup, no hidden lists*/
              /*NOTE: subgroup nobutton allows list buttons*/
              thislistptr = thissgrpptr->firstlistptr;
              while(thislistptr != NULL)
              {/*lists*/
                if((thislistptr->STATUS & NOBUTTON) == 0)
                {/*not nobutton*/
                  nbox++;

                  makebuttonbox(&(thislistptr->button) /*____BBOX.c*/
                    ,left+10,top,right,top
                    ,thislistptr->name,thislistptr->on,nbox,nn,indexPC);
                }/*not nobutton*/
                if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
                else thislistptr = thislistptr->nextptr;
              }/*lists*/
            }/*not dominant subgroup, no hidden lists*/
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
          }/*sets*/
        }/*not a dominant group hiding subgroups and lists*/
      }/*the right kind and not a nobutton group with no buttons at all*/
      if(thisgrupptr == lastgrupptr) thisgrupptr=NULL;
      else thisgrupptr = thisgrupptr->nextptr;
    }/*groups*/
}
/*___UpdateAppendedSets()___________________________________________________*/

       
/****buttonboxscan()*********************************************************/
void    buttonboxscan(ControlHandle it, int ion)   /*<- ____MAIN*/  
{
     int ifoundit = 0;
    
     thisgrupptr = firstgrupptr;
     while(thisgrupptr != NULL)
     {/*groups quasi-inc markers, which are NOBUTTON as of 020718*/
        if(  ((thisgrupptr->STATUS & NOBUTTON) == 0) 
           || (thisgrupptr->STATUS & RECESSIVEON)     )
        {/*check this one group's box*/
            /*not nobutton group with no buttons*/
            if(testbutton(it, thisgrupptr->button)) /*___BBOX.c*/ 
            {
               thisgrupptr->on = ion;  
               ifoundit = 1;      
               if(thisgrupptr->STATUS & RECESSIVEON)
               {/*recessiveon, shows when ON, hides (dominant) when OFF*/
                  /*so must redo buttons when ON/OFF changed*/
                  LneedSetsinGrafWindow=1;
               }
               if(ifoundit != 0) break;
            }
        }/*check this one group's box*/
         
        /*now see if there are any subgroups to check*/   
        if(  ((thisgrupptr->STATUS & DOMINANT) == 0)
           || (thisgrupptr->STATUS & RECESSIVEON)    )
        {/*not a dominant group hiding subgroups and lists*/
              /*recessiveon shows subgroups and lists when ON, check them*/
              thissgrpptr = thisgrupptr->firstsgrpptr;
              while(thissgrpptr != NULL)
              {/*subgroups*/
                if(  ((thissgrpptr->STATUS & NOBUTTON) == 0)
                   || (thissgrpptr->STATUS & RECESSIVEON)   )
                {/*check this one subgroup's box*/
                    if(testbutton(it, thissgrpptr->button)) /*___BBOX.c*/ 
                    {
                        thissgrpptr->on = ion;
                        ifoundit = 1;
                        if(thissgrpptr->STATUS & RECESSIVEON)
                        {/*recessiveon,shows when ON, hides(dominant) when OFF*/
                           LneedSetsinGrafWindow=1;
                        }
                    }
                    if(ifoundit != 0) break;
                }/*check this one subgroup's box*/
                
                /*now see if there are any lists to check*/       
                if(  ((thissgrpptr->STATUS & DOMINANT) == 0)
                   || (thissgrpptr->STATUS & RECESSIVEON)   )
                {/*not hidden lists */
                    /*NOTE: subgroup nobutton allows list buttons*/
                    /*recessiveon shows lists when ON, check them*/
                    thislistptr = thissgrpptr->firstlistptr;
                    while(thislistptr != NULL)
                    {/*lists*/
                        if((thislistptr->STATUS & NOBUTTON) == 0)
                        {/*not nobutton*/
                           if(testbutton(it, thislistptr->button)) /*___BBOX.c*/
                           {
                              thislistptr->on = ion;
                              ifoundit = 1;
                           }
                        }/*not nobutton*/
                        if(ifoundit != 0) break;
                        if(thislistptr == thissgrpptr->lastlistptr)
                            thislistptr=NULL;
                        else thislistptr = thislistptr->nextptr;
                    }/*lists*/
                }/*not hidden lists */
                if(ifoundit != 0) break;
                if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr=NULL;
                else thissgrpptr = thissgrpptr->nextptr;              
              }/*subgroups*/
        }/*not a dominant group hiding subgroups and lists */

        if(ifoundit != 0) break;
        thisgrupptr = thisgrupptr->nextptr;              
        if(thisgrupptr != NULL && thisgrupptr == measuregrupptr) 
        {
            thisgrupptr = thisgrupptr->nextptr;
        }
          
     }/*groups quasi-inc markers, which are NOBUTTON as of 020718*/
     if(ifoundit == 0) 
     {/*not found the hit button yet*/
        if(markergrupptr && testbutton(it,markergrupptr->button))/*M___BBOX.c*/
        {/*020718 special extra button made for markers in tools button set*/
            markergrupptr->on = ion; 
            if(markergrupptr->on)
            {/*turnoff auger when markers on display  040320*/
               Lsuperpunchon = 0;
            }
        }        
        if(drawgrupptr && testbutton(it,drawgrupptr->button)) /*M___BBOX.c*/ 
        {
            drawgrupptr->on = ion; 
        }        
        if(nmaster > 0)
        {
            thismasterptr = firstmasterptr;
            while(thismasterptr != NULL)
            {
                 if(testbutton(it, thismasterptr->button)) /*M___BBOX.c*/ 
                 {
                      thismasterptr->on = ion; /*980920 */
                      buttonboxcheck(thismasterptr->button,ion); /*____BBOX.C*/
                      adjustmaster(thismasterptr->mask, ion);/*MAGEBBOX.c*/
                 }
                 thismasterptr = thismasterptr->nextptr;
            }
        }

        if((nanimate > 1)&&(animbboxptr)&&(testbutton(it,animbboxptr->button)))
                                             /*M___BBOX.c*/
        {
             adjustanimate(1); /*MAGEBBOX.C*/
             buttonboxcheck(animbboxptr->button,0); /*M___BBOX.c*/
        }
        if((nnanimate>1)&&(anim2bboxptr)&&(testbutton(it,anim2bboxptr->button)))
        {
             adjustanimate(2); /*MAGEBBOX.C*/
             buttonboxcheck(anim2bboxptr->button,0); /*M___BBOX.c*/
        }
        if(pickbboxptr&&testbutton(it, pickbboxptr->button)) /*M___BBOX.c*/ 
        {
            Lpickcenteron = ion;
            if(Lpickcenteron) turnoffallbutoneboxLogical((long)pickbboxptr);
        }        
        if(picktablebboxptr&&testbutton(it, picktablebboxptr->button)) 
        {
            Ltablepickactive = ion;
            buttonboxcheck(picktablebboxptr->button,0); /*M___BBOX.c*/
        }        

        if(pickshowbboxptr&&testbutton(it, pickshowbboxptr->button)) 
        {
            Lpickshowon = ion;
            if(Lpickshowon) turnoffallbutoneboxLogical((long)pickshowbboxptr);
        }        
        if(movepointsbboxptr&&testbutton(it, movepointsbboxptr->button)) 
        {
          Lmovepointson = ion; /*061126*/
          if(Lmovepointson) turnoffallbutoneboxLogical((long)movepointsbboxptr);
        }        
        if(meansigmasbboxptr&&testbutton(it, meansigmasbboxptr->button))
        {
          Lmeansigmason = ion; /*061126*/
          if(Lmeansigmason) turnoffallbutoneboxLogical((long)meansigmasbboxptr);
        }        
        if(pickcolorbboxptr&&testbutton(it, pickcolorbboxptr->button)) 
        {
            Lpickcoloron = ion;
            if(Lpickcoloron) turnoffallbutoneboxLogical((long)pickcolorbboxptr);
        }        
        if(emphasisbboxptr&&testbutton(it, emphasisbboxptr->button)) 
        {
            Lemphasison = ion;
            if(Lemphasison) turnoffallbutoneboxLogical((long)emphasisbboxptr);
        }        
        if(zclipbboxptr&&testbutton(it, zclipbboxptr->button)) /*M___BBOX.c*/ 
        {
            Lzclipon = ion;
        }        
        if(onscreeninfobboxptr&&testbutton(it, onscreeninfobboxptr->button)) 
        {/*M___BBOX.c*/ 
            Lonscreeninfoon = ion;
        }        
        if(measuregrupptr&&testbutton(it, measuregrupptr->button))/*M___BBOX.c*/
        {
            Lmeasureson = ion;
            if(Lmeasureson) 
            {
                Lnewmeasures = 1; /* start again */
                turnoffallbutoneboxLogical((long)measuregrupptr);  
            } 
        }        
        if(drawlabelsbboxptr&&testbutton(it, drawlabelsbboxptr->button)) 
        {
            Lnewlabelson = ion;
            if(Lnewlabelson)turnoffallbutoneboxLogical((long)drawlabelsbboxptr);
        }
        if(drawdihedralbboxptr&&testbutton(it, drawdihedralbboxptr->button)) 
        {
            Lmeasureddihedralon = ion;
            if(Lmeasureddihedralon)
               turnoffallbutoneboxLogical((long)drawdihedralbboxptr);
        }
        if(drawballsbboxptr&&testbutton(it, drawballsbboxptr->button)) 
        {
            Lnewballson = ion;
            if(Lnewballson)turnoffallbutoneboxLogical((long)drawballsbboxptr);
        }
        if(drawlinebboxptr&&testbutton(it, drawlinebboxptr->button))       
        {
            Lnewlineson = ion;
            if(Lnewlineson) turnoffallbutoneboxLogical((long)drawlinebboxptr);
        }
       if(construct4bboxptr&&testbutton(it,construct4bboxptr->button))/*971122*/
       {
            Lconstruct4on = ion;
            Lpoint = 0; /*clear point marked flag*/ /*960812*/
            drawmarker1listptr->on = 0; /*turn off point indicaters*/
            drawmarker2listptr->on = 0;
            drawmarker3listptr->on = 0;
            drawmarker4listptr->on = 0;
            drawmarker5listptr->on = 0;
            drawmarker6listptr->on = 0;
            if(Lconstruct4on) 
                turnoffallbutoneboxLogical((long)construct4bboxptr);
       }        
       if(construct5bboxptr&&testbutton(it,construct5bboxptr->button))/*971122*/
       {
            Lconstruct5on = ion;
            Lpoint = 0; /*clear point marked flag*/ /*960812*/
            drawmarker1listptr->on = 0; /*turn off point indicaters*/
            drawmarker2listptr->on = 0;
            drawmarker3listptr->on = 0;
            drawmarker4listptr->on = 0;
            drawmarker5listptr->on = 0;
            drawmarker6listptr->on = 0;
            if(Lconstruct5on) 
                turnoffallbutoneboxLogical((long)construct5bboxptr);
       }        
       if(construct6bboxptr&&testbutton(it,construct6bboxptr->button))/*140912*/
       {
            Lconstruct6on = ion;
            Lpoint = 0; /*clear point marked flag*/
            drawmarker1listptr->on = 0; /*turn off point indicaters*/
            drawmarker2listptr->on = 0;
            drawmarker3listptr->on = 0;
            drawmarker4listptr->on = 0;
            drawmarker5listptr->on = 0;
            drawmarker6listptr->on = 0;
            if(Lconstruct6on) 
                turnoffallbutoneboxLogical((long)construct6bboxptr);
       }        
       if(paramtriggerbboxptr&&testbutton(it,paramtriggerbboxptr->button))
       {/*141006*/
            Lparamtriggeron = ion;
            Lpoint = 0; /*clear point marked flag*/
            drawmarker1listptr->on = 0; /*turn off point indicaters*/
            drawmarker2listptr->on = 0;
            drawmarker3listptr->on = 0;
            drawmarker4listptr->on = 0;
            drawmarker5listptr->on = 0;
            drawmarker6listptr->on = 0;
       }        
       if(dock3on3bboxptr&&testbutton(it,dock3on3bboxptr->button))/*020305*/
       {
            Ldock3on3on = ion;
            Lpoint = 0; /*clear point marked flag*/
            drawmarker1listptr->on = 0; /*turn off point indicaters*/
            drawmarker2listptr->on = 0;
            drawmarker3listptr->on = 0;
            drawmarker4listptr->on = 0;
            drawmarker5listptr->on = 0;
            drawmarker6listptr->on = 0;
            if(Ldock3on3on) 
                turnoffallbutoneboxLogical((long)dock3on3bboxptr);
       }        
       if(tetrabboxptr&&testbutton(it,tetrabboxptr->button))/*030217*/
       {
          if(Negamickshape != 1)
          {
             Negamickshape = 1; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(cubebboxptr&&testbutton(it,cubebboxptr->button))/*030217*/
       {
          if(Negamickshape != 2)
          {
             Negamickshape = 2; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(octabboxptr&&testbutton(it,octabboxptr->button))/*030217*/
       {
          if(Negamickshape != 3)
          {
             Negamickshape = 3; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(widgetbboxptr&&testbutton(it,widgetbboxptr->button))/*030217*/
       {
          if(Negamickshape != 4)
          {
             Negamickshape = 4; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(waterbboxptr&&testbutton(it,waterbboxptr->button))/*091127*/
       {
          if(Negamickshape != 5)
          {
             Negamickshape = 5; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(lionbboxptr&&testbutton(it,lionbboxptr->button))
       {
          if(Negamickshape != 1)
          {
             Negamickshape = 1; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(knightbboxptr&&testbutton(it,knightbboxptr->button))
       {
          if(Negamickshape != 3)
          {
             Negamickshape = 3; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(knavebboxptr&&testbutton(it,knavebboxptr->button))
       {
          if(Negamickshape != 4)
          {
             Negamickshape = 4; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(caltropbboxptr&&testbutton(it,caltropbboxptr->button))
       {
          if(Negamickshape != 5)
          {
             Negamickshape = 5; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(wallbboxptr&&testbutton(it,wallbboxptr->button))
       {
          if(Negamickshape != 7)
          {
             Negamickshape = 7; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(extrabboxptr&&testbutton(it,extrabboxptr->button))
       {
          if(Negamickshape != 6)
          {
             Negamickshape = 6; 
             LneedSetsinGrafWindow=1;
             if(Lfreeegamickmobile)
             {
                destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
                Lneedannealmobile = 0;
                Lfreeegamickmobile = 0;
             }
          } 
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 
       if(rejectbboxptr&&testbutton(it,rejectbboxptr->button))/*030217*/
       {
          buttonboxcheck(rejectbboxptr->button,0); /*no x mark*/
          if(Negamickshape != 0)
          {
             Negamickshape = 0; 
             LneedSetsinGrafWindow=1;
          } 
          destroyliststructure(mobilegrupptr->lastsgrpptr->lastlistptr);
          Lneedannealmobile = 0;
          Lfreeegamickmobile = 0;
       } 
       if(LegamickrepeatOK&&repeatbboxptr&&testbutton(it,repeatbboxptr->button))
       {/*repeatbboxptr 030223*/
          buttonboxcheck(repeatbboxptr->button,0); /*no x mark*/
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
          repeatmobiledocking();
          if(Lneedannealmobile){annealmobilelist();}
          if(!Lfreeegamickmobile){getmobilelist();}
       } 

       if(foofillbboxptr&&testbutton(it,foofillbboxptr->button))/*030217*/
       {
          buttonboxcheck(foofillbboxptr->button,0); /*no x mark*/
          foofillcomputation();
          rescalekinemage();
          redrawvec();
       }

       if(foosurfacebboxptr&&testbutton(it,foosurfacebboxptr->button))/*030217*/
       {
          buttonboxcheck(foosurfacebboxptr->button,0); /*no x mark*/
          foodotsurface();
          rescalekinemage();
          redrawvec();
       }

       if(foosealerbboxptr&&testbutton(it,foosealerbboxptr->button))/*030411*/
       {
          Lfoosealeron = ion;
          /*buttonboxcheck(foosealerbboxptr->button, Lfoosealeron);*/
          if(Lfoosealeron) 
             {turnoffallbutoneboxLogical((long)foosealerbboxptr);}
       }

        if(draglinebboxptr&&testbutton(it,draglinebboxptr->button)) 
        {
            Ldraglineon = ion;
            if(Ldraglineon) turnoffallbutoneboxLogical((long)draglinebboxptr);
        }
        if(monitorbboxptr&&testbutton(it,monitorbboxptr->button)) /*M___BBOX.c*/ 
        {
            Lmonitoron = ion; /*last line length*/
        }        
        if(eraselinebboxptr&&testbutton(it,eraselinebboxptr->button)) 
        {
            buttonboxcheck(eraselinebboxptr->button,0); /*no x mark*/
            doeraseline(); /*MAGEANGL.C*/
        }        

        if(pickerasebboxptr&&testbutton(it,pickerasebboxptr->button))
        {
            Lpickeraseon = ion;
            if(Lpickeraseon) turnoffallbutoneboxLogical((long)pickerasebboxptr);
        }        

        if(prunebboxptr&&testbutton(it,prunebboxptr->button)) /*M___BBOX.c*/ 
        {
            Lpruneon = ion;
            if(Lpruneon) turnoffallbutoneboxLogical((long)prunebboxptr);
        }        
        if(punchbboxptr&&testbutton(it,punchbboxptr->button) )/*M___BBOX.c*/ 
        {
            Lpunchon = ion;
            if(Lpunchon)  turnoffallbutoneboxLogical((long)punchbboxptr);
        }        
        if(augerbboxptr&&testbutton(it,augerbboxptr->button) )/*M___BBOX.c*/ 
        {
            Lsuperpunchon = ion;
            if(Lsuperpunchon)
            {
               /*auger endless unless undisplay everything not punchable*/
               potentiateMeasures(); /*removes measures stuff from screen*/
               if(markergrupptr) {markergrupptr->on = 0;} /*040320*/
               turnoffallbutoneboxLogical((long)augerbboxptr);
               mousexold = 2*killradius;
               mouseyold = 2*killradius; 
               /*set killzone circle in screen corner*/
               /*last mouse down was offscreen on auger button,*/
               /*so on screen points safe until next pick action*/
            }
        }        
        if(undopbboxptr&&testbutton(it,undopbboxptr->button)) /*M___BBOX.c*/ 
        {
            buttonboxcheck(undopbboxptr->button, 0); /*no x mark*/
            doundoprune(); /*MAGEUTIL.c*/
        }        
        if(SELECTINGlistbboxptr&&testbutton(it,SELECTINGlistbboxptr->button) )
        {/*M___BBOX.c*/ 
            LSELECTINGliston = ion; /*060121*/
            if(LSELECTINGliston) 
               turnoffallbutoneboxLogical((long)SELECTINGlistbboxptr);
        }        
        if(oneSELECTINGbboxptr&&testbutton(it,oneSELECTINGbboxptr->button) )
        {/*M___BBOX.c*/ 
            LoneSELECTINGon = ion; /*060207*/
        }        
        if(UnSELECTINGbboxptr&&testbutton(it,UnSELECTINGbboxptr->button) )
        {/*M___BBOX.c*/ 
            LUnSELECTINGon = ion; /*060206*/
            if(LUnSELECTINGon) {adjustSELECTINGlist(3);}
            else {adjustSELECTINGlist(2);}
        }        
        if(controlbboxptr&&testbutton(it,controlbboxptr->button))/*M___BBOX.c*/ 
        {/*981010*/
            Lcontrolon = ion;
            if(Lpipeactive)  /*991213*/
            {/*prototype update from pipe, just request read from pipe*/
#ifdef UNIX_X11
               if(Lpipebufferloaded) 
               {/*which had turned button ON so user knows buffer is loaded*/
                  getstufffrompipebuffer(); /*____PIPE.c*/
               }
#endif /*UNIX_X11*/
               Lcontrolon = 0; /*reset to OFF no matter what state*/
               Lpipebufferloaded = 0; /*reset to OFF*/
                 /*only Lpipebufferloaded gets to turn button ON */
            }
            else if(Lcontrolon == 0) 
            {/*turned OFF: restore calculation to uncontroled state*/
                dobondrotrouter( icurrentrotation, 0.0); /*MAGEANGL.c*/
                rescalekinemage(); /*MAGEINPT.c*/
                redrawvec(); /*MACDRAW.c*/
            }
        }/*981010*/ 
        if(switchbboxptr&&testbutton(it,switchbboxptr->button))/*M___BBOX.c*/ 
        {/*090419*/
            Lswitchon = ion;
            if(Lswitchon == 0) 
            {/*turned OFF: restore calculation to uncontroled state*/
                dobondrotrouter( icurrentrotation, 0.0); /*MAGEANGL.c*/
                rescalekinemage(); /*MAGEINPT.c*/
                redrawvec(); /*MACDRAW.c*/
            }
        }/*090419*/ 
        if(inhibitbboxptr&&testbutton(it,inhibitbboxptr->button))/*M___BBOX.c*/ 
        {
            Linhibiton = ion;
            if(Linhibiton == 0) 
            {/*turned OFF: restore update refresh*/
               if(Lupdate &&!Linhibiton&&!Lpipeactive)/*merely a flag for active pipe*/
               {/*update from remote program: e.g. dots by PROBE*/
                  /*This is genuine ditto of whatever was the previous command*/
                  sprintf(word,"\""); /*ditto mark*/
                  doupdate(1,word); /*MUXMOUT.c*/  /*970522*/
                  /* 1: sends coords to probe*/
                  rescalekinemage();  /* rescale rebuilds from float arrary*/
                  redrawvec(); /*MACDRAW.c*/
                  makecursoractive();  /*___MAIN.C*/
               }

            }
        } 
     }/*not found the hit button yet*/

     adjustcontrolbuttonboxchecks(); /*for control button boxes*/
   if(Ltablepresent && Ltablemarkers)
   {
      removetablemarkpoints();
      markfromtablecellsID(1); /*MAGETABL.c flag==1 for ptID matching*/      
   }    

}
/*___buttonboxscan()________________________________________________________*/

    
/****turnoffallbutoneboxLogical()*********************************************/
void turnoffallbutoneboxLogical(long box)
{
   /*020220 measures and measureddihedral work together*/
    if((bboxstruct*)box!=pickbboxptr) if(Lpickcenteron) Lpickcenteron=0;
    if((bboxstruct*)box!=drawdihedralbboxptr)
    {
       if((grupstruct*)box!=measuregrupptr) 
       {
          if(Lmeasureson) {Lmeasureson=0;}
       }
    }
    if((grupstruct*)box!=measuregrupptr)   
    {
       if((bboxstruct*)box!=drawdihedralbboxptr)
       {
         if(Lmeasureddihedralon) {Lmeasureddihedralon=0;}
       }
    }
    if((bboxstruct*)box!=drawlinebboxptr) if(Lnewlineson) Lnewlineson=0;
    if((bboxstruct*)box!=drawlabelsbboxptr) if(Lnewlabelson) Lnewlabelson=0;
    if((bboxstruct*)box!=drawballsbboxptr) if(Lnewballson) Lnewballson=0;
    if((bboxstruct*)box!=draglinebboxptr) if(Ldraglineon) Ldraglineon=0;
    if((bboxstruct*)box!=construct4bboxptr) if(Lconstruct4on) 
    {
       Lconstruct4on=0;
       Lpoint = 0; /*clear point marked flag*/
       drawmarker1listptr->on = 0; /*turn off point indicaters*/
       drawmarker2listptr->on = 0;
       drawmarker3listptr->on = 0;
       drawmarker4listptr->on = 0;
       drawmarker5listptr->on = 0;
       drawmarker6listptr->on = 0;
           /*avoid confusing construct and draw */
    } 
    if((bboxstruct*)box!=construct5bboxptr)if(Lconstruct5on)
    {
       Lconstruct5on=0;
       Lpoint = 0; /*clear point marked flag*/
       drawmarker1listptr->on = 0; /*turn off point indicaters*/
       drawmarker2listptr->on = 0;
       drawmarker3listptr->on = 0;
       drawmarker4listptr->on = 0;
       drawmarker5listptr->on = 0;
       drawmarker6listptr->on = 0;
    } 
    if((bboxstruct*)box!=construct6bboxptr)if(Lconstruct6on)
    {
       Lconstruct6on=0;
       Lpoint = 0; /*clear point marked flag*/
       drawmarker1listptr->on = 0; /*turn off point indicaters*/
       drawmarker2listptr->on = 0;
       drawmarker3listptr->on = 0;
       drawmarker4listptr->on = 0;
       drawmarker5listptr->on = 0;
       drawmarker6listptr->on = 0;
    } 
    if((bboxstruct*)box!=dock3on3bboxptr)if(Ldock3on3on)
    {
       Ldock3on3on=0;
       Lpoint = 0; /*clear point marked flag*/
       drawmarker1listptr->on = 0; /*turn off point indicaters*/
       drawmarker2listptr->on = 0;
       drawmarker3listptr->on = 0;
       drawmarker4listptr->on = 0;
       drawmarker5listptr->on = 0;
       drawmarker6listptr->on = 0;
    } 

    if((bboxstruct*)box!=pickshowbboxptr)if(Lpickshowon) Lpickshowon=0;
    if((bboxstruct*)box!=movepointsbboxptr)if(Lmovepointson) Lmovepointson=0;
    if((bboxstruct*)box!=meansigmasbboxptr)if(Lmeansigmason) Lmeansigmason=0;
    if((bboxstruct*)box!=pickcolorbboxptr)if(Lpickcoloron) Lpickcoloron=0; 
    if((bboxstruct*)box!=emphasisbboxptr)if(Lemphasison) Lemphasison=0; 
    if((bboxstruct*)box!=pickerasebboxptr)if(Lpickeraseon) Lpickeraseon = 0;
    if((bboxstruct*)box!=prunebboxptr)if(Lpruneon) Lpruneon = 0;
    if((bboxstruct*)box!=punchbboxptr)if(Lpunchon) Lpunchon = 0;
    if((bboxstruct*)box!=augerbboxptr)if(Lsuperpunchon) Lsuperpunchon = 0;
    if((bboxstruct*)box!=foosealerbboxptr)if(Lfoosealeron) Lfoosealeron = 0;
    if((bboxstruct*)box!=SELECTINGlistbboxptr)if(LSELECTINGliston) 
       LSELECTINGliston = 0;
}
/*___turnoffallbutoneboxLogical()____________________________________________*/

/****adjustcontrolbuttonboxchecks()*******************************************/
void adjustcontrolbuttonboxchecks()
{
    if(markergrupptr != NULL)
        buttonboxcheck(markergrupptr->button,markergrupptr->on);

    if(pickbboxptr != NULL)
        buttonboxcheck(pickbboxptr->button,Lpickcenteron); 
    
    if(picktablebboxptr != NULL)
        buttonboxcheck(picktablebboxptr->button,Ltablepickactive); 

    if(measuregrupptr != NULL)
        buttonboxcheck(measuregrupptr->button,Lmeasureson);

    if(drawlinebboxptr != NULL)
        buttonboxcheck(drawlinebboxptr->button,Lnewlineson);

    if(drawlabelsbboxptr != NULL)
        buttonboxcheck(drawlabelsbboxptr->button,Lnewlabelson);

    if(drawdihedralbboxptr != NULL)
        buttonboxcheck(drawdihedralbboxptr->button,Lmeasureddihedralon);

    if(drawballsbboxptr != NULL)
        buttonboxcheck(drawballsbboxptr->button,Lnewballson);

    if(draglinebboxptr != NULL)
        buttonboxcheck(draglinebboxptr->button,Ldraglineon); 

    if(construct4bboxptr != NULL) /*971122*/
        buttonboxcheck(construct4bboxptr->button,Lconstruct4on);

    if(construct5bboxptr != NULL) /*971122*/
        buttonboxcheck(construct5bboxptr->button,Lconstruct5on);

    if(construct6bboxptr != NULL) /*140912*/
        buttonboxcheck(construct6bboxptr->button,Lconstruct6on);

    if(dock3on3bboxptr != NULL) /*020305*/
        buttonboxcheck(dock3on3bboxptr->button,Ldock3on3on);

    if(foosealerbboxptr != NULL) /*030411*/
        buttonboxcheck(foosealerbboxptr->button,Lfoosealeron);

    if(pickshowbboxptr != NULL)
        buttonboxcheck(pickshowbboxptr->button,Lpickshowon);

    if(movepointsbboxptr != NULL)
        buttonboxcheck(movepointsbboxptr->button,Lmovepointson);

    if(meansigmasbboxptr != NULL)
        buttonboxcheck(meansigmasbboxptr->button,Lmeansigmason);

    if(pickcolorbboxptr != NULL)
        buttonboxcheck(pickcolorbboxptr->button,Lpickcoloron);

    if(emphasisbboxptr != NULL)
        buttonboxcheck(emphasisbboxptr->button,Lemphasison);

    if(pickerasebboxptr != NULL)
        buttonboxcheck(pickerasebboxptr->button, Lpickeraseon);

    if(prunebboxptr != NULL)
        buttonboxcheck(prunebboxptr->button, Lpruneon);
    if(punchbboxptr != NULL)
        buttonboxcheck(punchbboxptr->button, Lpunchon); 
    if(augerbboxptr != NULL)
        buttonboxcheck(augerbboxptr->button, Lsuperpunchon); 
    if(SELECTINGlistbboxptr != NULL)
        buttonboxcheck(SELECTINGlistbboxptr->button, LSELECTINGliston); 
    if(oneSELECTINGbboxptr != NULL)
        buttonboxcheck(oneSELECTINGbboxptr->button, LoneSELECTINGon); 
    if(UnSELECTINGbboxptr != NULL)
        buttonboxcheck(UnSELECTINGbboxptr->button, LUnSELECTINGon); 
    if(controlbboxptr != NULL)
    {
        if(Lpipeactive)
             {buttonboxcheck(controlbboxptr->button, Lpipebufferloaded);}
        else {buttonboxcheck(controlbboxptr->button, Lcontrolon);} 
    }
    if(switchbboxptr != NULL) /*090419*/
        buttonboxcheck(switchbboxptr->button, Lswitchon);  /*090419*/
    if(inhibitbboxptr != NULL)
        buttonboxcheck(inhibitbboxptr->button, Linhibiton); 
    if(paramtriggerbboxptr != NULL)
        buttonboxcheck(paramtriggerbboxptr->button, Lparamtriggeron); /*141006*/ 
}
/*___adjustcontrolbuttonboxchecks()__________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****adjustrock()*************************************************************/
void    adjustrock() 
{
   static float rockangle = 0;
   static int Lrockpositive = 1;
   float rockextent=0,rockstepsize=0;

   double cosang=0,sinang=0;
   if(Lnewrock)
   {
      rockangle = 0;
      Lrockpositive = 1;
      Lnewrock = 0;
   }
   rockextent = rockmax - rockmin;
   rockstepsize = rockextent/rocknumsteps;
   rocksteptime = rockcycletime/rocknumsteps;
   /*rockstepsize is in degrees, getrot expects delta x-position numbers*/
   /*this has to be scaled by effective screen size and zoom level*/
   /*as well as projection of effective motion cycle and postion in cycle*/
   if(Lrockpositive && rockangle < rockmax) rockangle = rockangle + rockstepsize;
   else if(Lrockpositive)
   {/*reached end going positive*/
      Lrockpositive = 0; 
      /*but reposition to exact so effectively get extra beat at the end*/
      rockangle = rockmax;
   }
   else if(!Lrockpositive && rockangle > rockmin) rockangle=rockangle-rockstepsize;
   else if(!Lrockpositive)
   {/*reached end going negative*/
      Lrockpositive = 1;
      /*but reposition to exact so effectively get extra beat at the end*/
      rockangle = rockmin;
   }
   /* sin and cos of current delta angle of this axis */
   sinang = sin(2*3.14159*rockangle/360); /*c*/ 
   cosang = cos(2*3.14159*rockangle/360); /*c*/ 
   /*update:aij-matrix X y-matrix: a11 a12 a13     cosang   0   -sinang  
                                   a21 a22 a23  X    0      1      0
                                   a31 a32 a33     sinang   0    cosang
                                                                       */
   r11 = (a11*cosang + a13*sinang);
   r12 = a12;
   r13 = (a11*(-sinang) + a13*cosang);
   r21 = (a21*cosang + a23*sinang);
   r22 = a22;
   r23 = (a21*(-sinang) + a23*cosang);
   r31 = (a31*cosang + a33*sinang);
   r32 = a32;
   r33 = (a31*(-sinang) + a33*cosang);
   redrawvec();
}
/*___adjustrock()____________________________________________________________*/
/*
EXTERN long  nextanimatetick, nextrocktick;
EXTERN int   Lautoanimate,Lautorock,Lmickeymouse,Lnewrock,Lpausemickeymouse;
EXTERN float animatesteptime,rocksteptime;
EXTERN float rockmin,rockmax,rockcycletime,rocknumsteps;
*/

/****adjustanimate()**********************************************************/
void    adjustanimate(int nthanimate)   /*980707 ANNOTATIONS*/
{
            int    next, ifind, icycle = 1,ihitfirst,nextafter,Lastpass;
static  grupstruct* firstptr;
static  grupstruct* lastptr;
static  grupstruct* currentptr;
/*static  grupstruct* nextanimategroupptr;*/ /*980707*/
static  grupstruct* offptr;

   if(Lshiftkey)
   {
       firstptr = lastgrupptr; /* reversing animation*/
       lastptr = firstgrupptr;
   }
   else
   {
       firstptr = firstgrupptr;
       lastptr = lastgrupptr;
   }
   
   icycle = 0;
   next = 1;
   ifind = 0; /* flag for first on group */
   offptr = NULL; /* 0 not a possible group */
   nextafter = 0; /*flag to look for animate group following current*/     
   ihitfirst = 0; /*flag to find first of these animate groups */
   /*nextanimategroupptr = NULL;*/ /*980707*/
   nextanimategrupptr = NULL;/*980707*/
   while(next == 1)
   {/*begin while next flag set*/
       icycle = icycle + 1;
       if(icycle == 1) next = 0; /* clear flag for first cycle */
       /*first cycle will at least turn all animatable groups OFF*/
       /*Does second cycle if not find a group of the desired */
       /*animate set ON to turn OFF */
       /*Third cycle only if called with nthanimate==0 to turn all*/
          /*animatable groups off.  e.g. when starting Lcompareon*/
       if(icycle == 3){ifind = 1; next = 0; break;}
             
       currentptr = firstptr;
       /*for(ng = first; ng != last; ng = ng + istep)*/
       Lastpass = 0; /*need to touch ALL groups*/
       do /*loop over all groups from first to last, (while !Lastpass)*/
       {/*begin scan over all groups*/
          if(currentptr==lastptr) Lastpass = 1; /*do pass with last group*/
          if(currentptr->animate > 0) 
          /*normally only check those with animate flag*/
          /*020718 control key forces all groups animate: for simulation file*/
          {/*check an animatable group*/             
             if(currentptr->animate == nthanimate && !ihitfirst)
             {/*first animategroup of desired set, first cycle*/
                 /*nextanimategroupptr = currentptr;*/ /*980707*/
                 nextanimategrupptr = currentptr;/*980707*/
                 ihitfirst = 1;
             }
             if(currentptr->on == 1) /* ON: turn OFF ANY ANIMATE,1 or 2*/
             {
                if(Laccumulate == 0) /*030111*/
                {
                   currentptr->on = 0;
                   buttonboxcheck(currentptr->button,0);   /*___BBOX.C */
                }
                if(ifind == 0 && currentptr->animate == nthanimate)
                {/* looking for first ON group of this animate set */
                   next = 1;    /* set flag looking for next*/
                   offptr = currentptr; /*remember which one just turned off*/
                }
             }
             else /* off */
             { 
                if(   next == 1/*already found an ON nanimate group, want next*/
                   && currentptr != offptr /*didn't just turn this one off*/
                   && currentptr->animate == nthanimate)/*is of active series*/
                {/*off, just turned one off, and this is not it,*/
                    /* or in second cycle, where first left all off */
                    /* so turn on if in current nthanimate*/
                    currentptr->on = 1; /*turn group ON*/
                    buttonboxcheck(currentptr->button,1);  /*___BBOX.C */
                    next = 0; /* clear flag, finished looking for next one OFF*/
                    /*check and adjust moview*/
                    if(currentptr->moview>0) 
                       resetrot(currentptr->moview);/*MAGEANGL*/
                    nextafter = 1; /*set to find what would come next*/
                    ifind = 1;    /* found an OFF group to turn ON*/
                }
                else if(   next == 0/*already has next that just got turned ON*/
                        && currentptr->animate == nthanimate 
                        && nextafter == 1) /*BUT now want the nextafter that*/
                {/*is the group that will be turned on next animate*/
                   /*nextanimategroupptr = currentptr;*/ /*980707*/
                   nextanimategrupptr = currentptr;/*980707*/
                   nextafter = 0;/*clear, finished looking for nextafter one*/
                }
             }
          }/*check an animatable group*/ 
          if(!Lastpass)
          {/*NOT Lastpass*/
             if(Lshiftkey)
             {
                 currentptr = currentptr->previousptr;
             }
             else
             {
                 currentptr = currentptr->nextptr;
             }
          }/*NOT Lastpass*/
          if(currentptr==NULL) break;
       }while(!Lastpass);/*endof scan over all groups*/
       if(ifind == 0) next = 1;
   }/*endof while next flag set*/   
   if(Ltablepresent && Ltablemarkers)
   {
      removetablemarkpoints();
      markfromtablecellsID(1); /*MAGETABL.c flag==1 for ptID matching*/      
   }    
}
/*___adjustanimate()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****adjustmaster()**********************************************************/
void    adjustmaster(unsigned long long mask, int ion) /*081120 long long*/
{/*begin adjust master flagged groups, subgroups, lists, points */         
   int OKtoTurnON = 0;
   struct masterstruct* thismasterptr; /*local overrides global*/ /*980921b*/
       /*global is being used as index in loop that calls this one*/
       
         /*master index: 1 == 1, 2 == 2, 3 == 4, 4 == 8, ...     */
         /*                2^0     2^1     2^2     2^3           */
         /*                1<<0   1<<1    1<<2     1<<3          */
         /*mask = 1<<(i-1);*/

   /*check out groups, subgroups, lists, then check points of lists */
   thisgrupptr = firstgrupptr;
   while(thisgrupptr != NULL)
   {/*scan over all groups*/
      if(thisgrupptr->master&mask) /*only check those with this master flag*/
      {/*check this group*/  
         if(ion==0)
         {/*try to turn OFF, only takes one off master*/

            if(thisgrupptr->on != 0)
            {/*item is ON, turn OFF*/
               thisgrupptr->on = 0;
               buttonboxcheck(thisgrupptr->button,0);  /*____BBOX*/  
            }/*item is ON, turn OFF*/
            /*else item is OFF, leave OFF*/
         }/*try to turn OFF*/
         else
         {/*try to turn ON, all masters must be on*/
            if(thisgrupptr->on != 0) ; /*item is ON, leave ON*/
            else
            {/*item is OFF, must have all masters agree to be ON*/
               if((thisgrupptr->master - mask) != 0) /*item has > 1  master*/
               {/*scan over all masters, find applicable ones*/
                  thismasterptr = firstmasterptr;
                  while(thismasterptr != NULL)
                  {/*while loop*/
                     if((thismasterptr->mask&thisgrupptr->master) != 0)
                     {/*mask passed a bit*/
                        if(thismasterptr->on != 0) OKtoTurnON = 1;
                        else
                        {/*failed*/
                           OKtoTurnON = 0;
                           break; /*only takes one OFF to keep it OFF*/
                        }/*failed*/
                     }/*mask passed a bit*/
                     thismasterptr = thismasterptr->nextptr;
                  }/*while loop*/
               }/*scan over all masters, find applicable ones*/
               else OKtoTurnON = 1;/*only 1 master*/
               if(OKtoTurnON)
               {/*Turn it ON*/
                  thisgrupptr->on = 1;
                  buttonboxcheck(thisgrupptr->button,1);  /*____BBOX*/  
               }/*Turn it ON*/
            }/*item is OFF, must have all masters agree to be ON*/
         }/*try to turn ON, all masters must be on*/
      }/*check this group*/
      thisgrupptr = thisgrupptr->nextptr;
   }/*scan over all groups*/

   thissgrpptr = firstsgrpptr;
   while(thissgrpptr != NULL)
   {/*scan over all subgroups*/
      if((thissgrpptr->master&mask) != 0) /*only check those with this master flag*/
      {/*check this subgroup*/  
         if(ion == 0)
         {/*try to turn OFF, only takes one OFF master*/
            if(thissgrpptr->on != 0)
            {/*item is ON, turn OFF*/
               thissgrpptr->on = 0;
               buttonboxcheck(thissgrpptr->button,0);  /*____BBOX*/  
            }/*item is ON, turn OFF*/
            /*else item is OFF, leave OFF*/
         }/*try to turn OFF*/
         else
         {/*try to turn ON, all masters must be on*/
            if(thissgrpptr->on != 0) ; /*item is ON, leave ON*/
            else
            {/*item is OFF, must have all masters agree to be ON*/
               if((thissgrpptr->master - mask) != 0) /*item has > 1  master*/
               {/*scan over all masters, find applicable ones*/
                  thismasterptr = firstmasterptr;
                  while(thismasterptr != NULL)
                  {/*while loop*/
                     if((thismasterptr->mask&thissgrpptr->master) != 0)
                     {/*mask passed a bit*/
                        if(thismasterptr->on != 0) OKtoTurnON = 1;
                        else
                        {/*failed*/
                           OKtoTurnON = 0;
                           break; /*only takes one OFF to keep it OFF*/
                        }/*failed*/
                     }/*mask passed a bit*/
                     thismasterptr = thismasterptr->nextptr;
                  }/*while loop*/
               }/*scan over all masters, find applicable ones*/
               else OKtoTurnON = 1;/*only 1 master*/
               if(OKtoTurnON)
               {/*Turn it ON*/
                  thissgrpptr->on = 1;
                  buttonboxcheck(thissgrpptr->button,1);  /*____BBOX*/  
               }/*Turn it ON*/
            }/*item is OFF, must have all masters agree to be ON*/
         }/*try to turn ON, all masters must be on*/
      }/*check this subgroup*/
      thissgrpptr = thissgrpptr->nextptr;
   }/*scan over all subgroups*/

   thislistptr = firstlistptr;
   while(thislistptr != NULL)
   {/*scan over all lists for list master*/
/*M = thislistptr->master;*/
/*N = 0;*/
/*if(M&1)   N =     1;*/
/*if(M>>1)  N = N + 10;*/
/*if(M>>2)  N = N + 100;*/
/*if(M>>3)  N = N + 1000;*/
/*if(M>>4)  N = N + 10000;*/
/*if(M>>5)  N = N + 100000;*/
/*if(M>>6)  N = N + 1000000;*/
/*if(M>>7)  N = N + 10000000;*/
/*if(M>>8)  N + N + 100000000;*/
/*if(M>>9)  N + N + 1000000000;*/
/*if(M>>10) N + N + 10000000000;*/
   
/*printf("thislistptr->master %d, mask %d, thislistptr->on %d, ion %d\n"  */
/*,N,mask,thislistptr->on,ion);*/
     if((thislistptr->master&mask)!=0)/*only check those with this master flag*/
     {/*check this list*/  
         if(ion == 0)
         {/*try to turn OFF, only takes one off master*/
            if(thislistptr->on != 0)
            {/*item is ON, turn OFF*/
               thislistptr->on = 0;
               buttonboxcheck(thislistptr->button,0);  /*____BBOX*/  

            }/*item is ON, turn OFF*/
            /*else item is OFF, leave OFF*/
         }/*try to turn OFF*/
         else
         {/*try to turn ON, all masters must be on*/
            if(thislistptr->on != 0) ; /*item is ON, leave ON*/
            else
            {/*item is OFF, must have all masters agree to be ON*/
               if((thislistptr->master - mask) != 0) /*item has > 1  master*/
               {/*scan over all masters, find applicable ones*/
                  thismasterptr = firstmasterptr;
                  while(thismasterptr != NULL)
                  {/*while loop*/
                     if((thismasterptr->mask&thislistptr->master) != 0)
                     {/*mask passed a bit*/
                        if(thismasterptr->on != 0) OKtoTurnON = 1;
                        else
                        {/*failed*/
                           OKtoTurnON = 0;
                           break; /*only takes one OFF to keep it OFF*/
                        }/*failed*/
                     }/*mask passed a bit*/
                  thismasterptr = thismasterptr->nextptr;
                  }/*while loop*/
               }/*scan over all masters, find applicable ones*/
               else OKtoTurnON = 1;/*only 1 master*/
               if(OKtoTurnON)
               {/*Turn it ON*/
                  thislistptr->on = 1;
                  buttonboxcheck(thislistptr->button,1);  /*____BBOX*/  
               }/*Turn it ON*/
            }/*item is OFF, must have all masters agree to be ON*/
         }/*try to turn ON, all masters must be on*/
     }/*check this list*/
     thislistptr = thislistptr->nextptr;
   }/*scan over all lists for list master*/
   thislistptr = firstlistptr;
   while(thislistptr != NULL)
   {/*scan over all lists for points with masters*/
      if(thislistptr->STATUS&HASPTMASTER)
      {/*this list has pointmasters*/  
         thispointptr = thislistptr->firstpointptr;
         while(thispointptr != NULL)
         {/*scan over points*/
            if(thispointptr->master&mask)
            {/*point has this master*/
               if(ion == 0)
               {/*try to turn OFF, only takes one off master*/
                  thispointptr->STATUS = thispointptr->STATUS &~ON;
               }/*try to turn OFF*/
               else
               {/*try to turn ON, all masters must be on*/
                  if(thispointptr->STATUS & ON) ; /*item is ON, leave ON*/
                  else
                  {/*item is OFF, must have all masters agree to be ON*/
                     if((thispointptr->master - mask) != 0) /* > 1 master*/
                     {/*scan over all masters, find applicable ones*/
                        thismasterptr = firstmasterptr;
                        while(thismasterptr != NULL)
                        {/*while loop*/
                           if((thismasterptr->mask&thispointptr->master) != 0)
                           {/*mask passed a bit*/
                              if(thismasterptr->on != 0) {OKtoTurnON = 1;}
                              else
                              {/*failed*/
                                 OKtoTurnON = 0;
                                 break; /*only takes one OFF to keep it OFF*/
                              }/*failed*/
                           }/*mask passed a bit*/
                           thismasterptr = thismasterptr->nextptr;
                        }/*while loop*/
                     }/*scan over all masters, find applicable ones*/
                     else {OKtoTurnON = 1;} /*only 1 master*/
                     if(OKtoTurnON)
                     {/*Turn it ON*/
                        thispointptr->STATUS = thispointptr->STATUS | ON; 
                        buttonboxcheck(thislistptr->button,1);  /*____BBOX*/  
                     }/*Turn it ON*/
                  }/*item is OFF, must have all masters agree to be ON*/
               }/*try to turn ON, all masters must be on*/
            }/*point has this master*/
            if(thispointptr == thislistptr->lastpointptr) {thispointptr = NULL;}
            else  {thispointptr = thispointptr->nextptr;}
         }/*scan over points*/
      }/*this list has pointmasters*/  
      thislistptr = thislistptr->nextptr;
   }/*scan over all lists for points with masters*/
      
#ifdef OLDCODE
   thismasterptr = firstmasterptr;
   while(thismasterptr != NULL)
   {/*while loop*/
       if(thismasterptr->mask&mask)
       {/*mask passed a bit*/
           break; /*thismasterptr is the right master*/
       }/*mask passed a bit*/
       thismasterptr = thismasterptr->nextptr;
   }/*while loop*/
   if(thismasterptr->masterchar != '\0')
   {/*this master controls some pointmasters*/
       thislistptr = firstlistptr;
       while(thislistptr != NULL)
       {/*scan over all lists for point master*/
          if(thislistptr->STATUS&HASPTMASTER)
          {/*this list has pointmasters*/  
            thispointptr = thislistptr->firstpointptr;
            while(thispointptr != NULL)
            {/*check this point*/
              if(thispointptr->master&mask)
              {
                 if(ion) thispointptr->STATUS = thispointptr->STATUS | ON; 
                 else    thispointptr->STATUS = thispointptr->STATUS &~ON;
              }
              if(thispointptr == thislistptr->lastpointptr)
                    thispointptr = NULL;
              else  thispointptr = thispointptr->nextptr;
            }/*check this point*/
          }/*this list has pointmasters*/  
          thislistptr = thislistptr->nextptr;
       }/*scan over all lists for point master*/
   }/*this master controls some pointmasters*/
#endif

   if(Ltablepresent && Ltablemarkers)
   {
      removetablemarkpoints();
      markfromtablecellsID(1); /*MAGETABL.c flag==1 for ptID matching*/      
      /*re-establish marks only for items that are ON */
   }    

}/*endof adjust master flagged groups, subgroups, lists, points */
/*___adjustmaster()_________________________________________________________*/

/****resetmaster()***********************************************************/
void    resetmaster(int i)
{/*begin reset master flagged groups, subgroups, lists */
         int    ireset;
         unsigned long long mask; /*081120 long long rather than int*/

/*groups/subgroups/lists/points are not only hierarchical, But*/
/*they also can be created orthogonal to the hierarchy but are still*/
/*linked previous to next within their own set*/
/*Therefore a linked search across groups, then subgroups, then lists*/
/* might hit members that are not in the master-referred hierarchy*/
         
   /*mask = 1<<(i-1);*/
   mask = (unsigned long long)((unsigned long long)1<<(i-1)); /*081120*/
   /*identify this masterptr by its mask */
   thismasterptr = firstmasterptr;
   while(thismasterptr != NULL)
   {
       if(thismasterptr->mask == mask) break;
       thismasterptr = thismasterptr->nextptr;
   }
   /*Now know thismasterptr for the master being reset*/
   
   ireset = 0; /*turn master off unless any one client is on */
   /*only need one ON to set master ON, can break when get one ON*/
   
   /*try groups*/
   thisgrupptr = firstgrupptr;
   while(thisgrupptr != NULL)
   {/*scan over all groups*/
       if(thisgrupptr->master&mask) 
       {/*begin check groups*/             
          if(thisgrupptr->on == 1)
          {
             ireset = 1; /* look for on state */
             break;
          }
       }
       thisgrupptr = thisgrupptr->nextptr;
   }/*scan over all groups*/
   if(!ireset)
   {/*keep trying: subgroups*/
         thissgrpptr = firstsgrpptr;
         while( thissgrpptr != NULL)
         {/*scan over all subgroups*/
             if(thissgrpptr->master&mask) 
             {/*begin check subgroups*/             
                 if(thissgrpptr->on == 1) ireset = 1; /* look for on state */
             }
             thissgrpptr = thissgrpptr->nextptr;
         }
   }/*keep trying: subgroups*/
   if(!ireset)
   {/*keep trying: lists*/
         thislistptr = firstlistptr;
         while(thislistptr != NULL)
         {/*scan over all lists*/
            if(thislistptr->master&mask)
                /*only check those with this master flag*/
            {/*begin check lists*/             
                 if(thislistptr->on == 1)
                 {
                    ireset = 1; /* look for on state */
                    break;
                 }
            }
            thislistptr = thislistptr->nextptr; /*lastlistptr->nextptr==NULL*/
         }
   }/*keep trying: lists*/
   if(!ireset)
   {/*keep trying: points*/
      if(thismasterptr->masterchar != '\0')
      {/*this master controls some pointmasters*/
         thislistptr = firstlistptr;
         while(thislistptr != NULL)
         {/*scan over all points of lists that have points with pointmasters*/
            if(thislistptr->STATUS&HASPTMASTER)
            {/*HAS PT MASTER, check all points of this list*/  
               thispointptr = thislistptr->firstpointptr;
               while(thispointptr != NULL)
               {/*check points with this master mask*/
                 if(thispointptr->master&mask)
                 {
                    if(thispointptr->STATUS&ON)
                    {
                       ireset = 1; /*look for on state*/
                       break;
                    }
                 }
                 if(thispointptr == thislistptr->lastpointptr)
                      thispointptr = NULL;
                 else thispointptr = thispointptr->nextptr;
               }/*check points with this master mask*/
            }/*HAS PT MASTER, check all points of this list*/  
            if(ireset) break;
            thislistptr = thislistptr->nextptr; /*lastlistptr->nextptr==NULL*/
         }/*scan over all points of lists that have points with pointmasters*/
      }/*this master controls some pointmasters*/
   }/*keep trying: points*/

   if(thismasterptr != NULL)
   {
       buttonboxcheck(thismasterptr->button,ireset);   /*____BBOX.C*/
       thismasterptr->on = ireset; /*980920*/
   }

}/*endof reset master flagged groups, subgroups, lists */
/*___resetmaster()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****killgrafcontrols()*******************************************************/
void    killgrafcontrols()
{
        /*clear handles so they can be used as tests*/
        thisgrupptr = firstgrupptr;
        while(thisgrupptr != NULL)
        {
            if(thisgrupptr->button!=NULL)
            {
#ifndef MACINTOSH /*980610*/
                destroybuttons(thisgrupptr->button);
                /*this destroys the screen widget/button*/
                /*but does not kill the allocated button structure*/
#endif /*980610*/
                thisgrupptr->button = NULL; 
            }
            thisgrupptr = thisgrupptr->nextptr;
        }

        thissgrpptr = firstsgrpptr;
        while(thissgrpptr != NULL)
        {
            if(thissgrpptr->button != NULL)
            {
#ifndef MACINTOSH /*980610*/
                destroybuttons(thissgrpptr->button);
#endif /*980610*/
                thissgrpptr->button = NULL; 
            }
            thissgrpptr = thissgrpptr->nextptr;
        }
        thislistptr = firstlistptr;
        while(thislistptr != NULL)
        {
            if(thislistptr->button != NULL)
            {
#ifndef MACINTOSH /*980610*/
                destroybuttons(thislistptr->button);
#endif /*980610*/
                thislistptr->button = NULL; 
            }
            thislistptr = thislistptr->nextptr;
        }

        thismasterptr = firstmasterptr;
        while(thismasterptr != NULL)
        {
            if(thismasterptr->button != NULL)
            {
#ifndef MACINTOSH /*980610*/
                destroybuttons(thismasterptr->button);
#endif /*980610*/
                thismasterptr->button = NULL; 
            }
            thismasterptr = thismasterptr->nextptr;
        }

        /*separate structures hold bbox stuff that doesn't belong to */
        /*otherwise allocated structures*/
        thisbboxptr = firstbboxptr;
        while(thisbboxptr != NULL)
        {
            if(thisbboxptr->button != NULL)
            {
#ifndef MACINTOSH /*980610*/
                destroybuttons(thisbboxptr->button);
#endif /*980610*/
                thisbboxptr->button = NULL; 
            }
            thisbboxptr = thisbboxptr->nextptr;
        }
#ifdef MACINTOSH /*980610*/
        KillControls(grafWindow); /*m*/ /*remove controls */ /*980610*/
#endif /*980610*/        
}
/*___killgrafcontrols()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****allocbboxstructure()*************************************************/
int   allocbboxstructure()
{
    int   ifail = 0;
    int   ireturn = 0;/*if successful, will be set == 1*/
    struct bboxstruct *trialbboxptr;
    trialbboxptr = (bboxstruct*)malloc(sizeof(struct bboxstruct));
    if(trialbboxptr==NULL) ifail++;
    if(ifail)
    {
        trialbboxptr = NULL;
    }  
    else
    {/*successfull allocation*/
        if(firstbboxptr == NULL)
        {
            firstbboxptr = trialbboxptr;
            firstbboxptr->previousptr = NULL;
        }
        else
        {/*set references between last and new bboxs*/
            lastbboxptr->nextptr = trialbboxptr;
            trialbboxptr->previousptr = lastbboxptr;
        }
        /*reset who is current bbox*/
        thisbboxptr = trialbboxptr;
        thisbboxptr->nextptr = NULL; /*no others beyond this one*/  
        lastbboxptr = thisbboxptr; 
            
        thisbboxptr->button  = NULL;
        thisbboxptr->name[0] = '\0';
        thisbboxptr->on      =   1 ;

        ireturn = 1;
    }/*successfull allocation*/    
    
    return(ireturn);

}
/*____allocbboxstructure()________________________________________________*/

/****destroyallbboxstructures()***********************************************/
void destroyallbboxstructures(void)
{
   struct bboxstruct* trybboxptr;
   struct bboxstruct* nextbboxptr;

    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*groups*/

       if(thisgrupptr->button != NULL) {clearbuttonbox(&thisgrupptr->button);}
       thisgrupptr->button = NULL;

       thissgrpptr = thisgrupptr->firstsgrpptr;
       while(thissgrpptr != NULL)
       {/*subgroups*/

         if(thissgrpptr->button != NULL) {clearbuttonbox(&thissgrpptr->button);}
         thissgrpptr->button = NULL;

         thislistptr = thissgrpptr->firstlistptr;
         while(thislistptr != NULL)
         {/*lists*/

           if(thislistptr->button !=NULL){clearbuttonbox(&thislistptr->button);}
           thislistptr->button = NULL;

           if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
           else thislistptr = thislistptr->nextptr;
         }/*lists*/
         if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
         else thissgrpptr = thissgrpptr->nextptr;
       }/*subgroups*/
       thisgrupptr = thisgrupptr->nextptr;
    }/*groups*/

   trybboxptr = firstbboxptr;
   while(trybboxptr != NULL)
   {
      nextbboxptr = trybboxptr->nextptr;
      if(trybboxptr->button != NULL)
      {
#ifndef MACINTOSH /*980610*/
         destroybuttons(trybboxptr->button);
#endif /*980610*/
         trybboxptr->button = NULL; 
      }
      trybboxptr = NULL;
      free(trybboxptr);

      trybboxptr = nextbboxptr;
   }
}
/*___destroyallbboxstructures()______________________________________________*/

/****destroybboxstructure()************************************************/
void destroybboxstructure(bboxstruct* victimptr)
{  /*comment 980610*/
   /*NOT USED BY MACINTOSH, MAC Can't destroy individual buttons anyway*/
        /* points: thisbboxptr->firstpoint && thisbboxptr->lastpoint are*/
        /* orphaned and eventually should be garbage collected */
        /* or cleaned up when all points are recoupled */
if(Ltest) { sprintf(alertstr,"MAGEBBOX/destroybboxstructure()\n");
 dosinglealert(3); }

    if(victimptr->button != NULL)
    {
#ifndef MACINTOSH /*980610*/
        destroybuttons(victimptr->button);
#endif /*980610*/
        victimptr->button = NULL; 
    }


 
    /*reconnect references among bboxs*/
    if(victimptr != firstbboxptr)
    {
        (victimptr->previousptr)->nextptr = victimptr->nextptr;
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = victimptr->previousptr;
        }
        else /*victimptr IS lastbboxptr*/
        {
            lastbboxptr = victimptr->previousptr;
        }
    }
    else /*victimptr IS firstbboxptr*/
    {
        if( (victimptr->nextptr) != NULL)
        {
            (victimptr->nextptr)->previousptr = NULL;
            firstbboxptr =  victimptr->nextptr; 
        }
        else /*victimptr IS also lastbboxptr*/
        {
            firstbboxptr = NULL;
            lastbboxptr  = NULL;
        }
    }
   
    free(victimptr);
        
}
/*___destroybboxstructure()_________________________________________________*/

