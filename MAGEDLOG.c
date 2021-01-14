/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*                            MAGEDLOG.c                                      */
/*121107 deleted \0, unneeded at end of sprintf()*/   
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEANGL.h"
#include "MAGEFLAG.h"  /*971025 perpendicular*/
#include "MAGEBBOX.h"

#define EXTERNDLOG
#include "MAGEDLOG.h"

#include "MAGEMENU.h"
#include "MAGETABL.h"
#include "MAGESYNC.h"
#include "MAGEFOO.h"

#include "MAGESCRT.h" /*070901*/
#include "MAGEOUTR.h" /*070901*/

int  tokenize(char thestr[256], int*, char**);

/****cleargenericflags()*****************************************************/
void cleargenericflags()
{
 Ldlog_cancel  = 1; /*presume Dialogs have cancel button as well as OK button*/
 Ldlog_option  = 0;
 Ldlog_extra   = 0;
 Ldlog_extra2  = 0;
 Ldlog_subject = 0;
 Ldlog_paramA  = 0;
 Ldlog_textA   = 0;
 Ldlog_paramB  = 0;
 Ldlog_textB   = 0;
 Ldlog_paramC  = 0;
 Ldlog_textC   = 0;
 Ldlog_paramD  = 0;
 Ldlog_textD   = 0;
 Ldlog_paramE  = 0;
 Ldlog_textE   = 0;
 Ldlog_paramF  = 0; /*041031*/
 Ldlog_textF   = 0;

 Ldlog_info    = 0;
 
 Ldlog_checkA  = 0;
 Ldlog_checkB  = 0;
 Ldlog_checkC  = 0;
 Ldlog_checkD  = 0;
 Ldlog_checkE  = 0;
 Ldlog_checkF  = 0;
 
 Ldlog_radioA  = 0;
 Ldlog_radioB  = 0;
 Ldlog_radioC  = 0;
 Ldlog_radioD  = 0;
 Ldlog_radioE  = 0;
 Ldlog_radioF  = 0;
 Ldlog_radioG  = 0;
 Ldlog_radioH  = 0;
 Ldlog_radioI  = 0;
 Ldlog_radioJ  = 0;
 
 Ldlog_radioK  = 0;
 Ldlog_radioL  = 0;
 Ldlog_radioM  = 0;
 Ldlog_radioN  = 0;
 Ldlog_radioO  = 0;
 Ldlog_radioP  = 0;
 Ldlog_radioQ  = 0;
 Ldlog_radioR  = 0;
 
 Ldlog_radioS  = 0;
 Ldlog_radioT  = 0;
 Ldlog_radioU  = 0;
 Ldlog_radioV  = 0;
 Ldlog_radioW  = 0;
 Ldlog_radioX  = 0;
 Ldlog_radioY  = 0;
 Ldlog_radioZ  = 0;
 
 sprintf(dlog_OKstr,    "______OK______");/*spaces are half width, vs _ */
 sprintf(dlog_cancelstr,"____CANCEL____");/*pad to help dialog box min width*/
 sprintf(dlog_optionstr," ");      /*970408*/
 sprintf(dlog_extrastr," ");      /*030312*/
 sprintf(dlog_extra2str," ");      /*030930*/
 sprintf(dlog_textAstr,"           "); /*can use for additional message*/
 sprintf(dlog_textBstr,"           "); /*can use for additional message*/
 sprintf(dlog_textCstr,"           "); /*can use for additional message*/
 sprintf(dlog_textDstr,"           "); /*can use for additional message*/
 sprintf(dlog_textEstr,"           "); /*can use for additional message*/
 
 Ldlog_listboxA = 0; /*060619*/
 Ldlog_listboxB = 0;
 Ldlog_listboxC = 0;

 ACTIVE_DLOG = 0;
 Ldlog_OKHIT = 0;
 Ldlog_cancelHIT = 0;
 Ldlog_optionHIT = 0;
 Ldlog_extraHIT = 0;
 Ldlog_extra2HIT = 0;
}
/*___cleargenericflags()____________________________________________________*/

/****colorshow_update()******************************************************/
void colorshow_update(int listcolor, int pointcolor)
{
       if(pickedlistptr->colorset != 0)
       {/*this list is part of a named colorset, change color of all members*/
          adjustcolorset(pickedlistptr->colorset, listcolor);
       }

	pickedlistptr->color = listcolor;
	pickedpointptr->colorwidth
		=pointcolor|((pickedpointptr->colorwidth)&~31);
		/* ~31:one's compliment of 31, i.e. bits 1->0,0->1*/
	redrawvec(); /* redraw to show changes */
}
/*___colorshow_update()_____________________________________________________*/

/****dosinglealert()*********************************************************/
void    dosinglealert(int mode) /*only alertstr has current information*/
{
 if(Ltest||Lformattest)
  {
     alertstr2[0]='\0'; 
            alertstr3[0]='\0'; 
#ifdef UNIX_X11
            printf("%s\n",alertstr); /*UNIX has functional console window*/
                                     /*which doesn't overlap graphics*/
                                     /*and allows program to be ^c killed*/
#else
            DoMageDLOGreport(mode); /*mode 2 for abort, 3 for toggle debug*/
#endif
 }
}
/*___dosinglealert()________________________________________________________*/

/****DokineDialog()**********************************************************/
void    DokineDialog(void)
{
  if(!ACTIVE_DLOG)
     {
         cleargenericflags(); /*MAGEDLOG.c*/
         kineDialog();        /*MAGEDLOG.c*/
         DogenericDialog();   /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
         EndkineDialog();     /*MAGEDLOG*/
#endif
     }
}
/*___DokineDialog()_________________________________________________________*/

/****kineDialog()************************************************************/
void    kineDialog()   /*KINE_DLOG*/
{
    sprintf(dlog_subjectstr,"Kinemage choose");
    Ldlog_subject = 1;
    
    sprintf(dlog_paramAstr,"0"); 
        /*set 0 so simple return will return to first kinemage*/
    Ldlog_paramA = 1;
    sprintf(dlog_textAstr,"choose kinemage number"); 
    Ldlog_textA = 1;
    sprintf(dlog_optionstr,"NEXT");
    Ldlog_option = 1;
    Lnextkine = 0;
    Lcancel = 0;
    if(atEOF) sprintf(dlog_infostr,"this kinemage = %d, next = %d, at EOF",
                            thiskinenum,kinemagenum);
    else      sprintf(dlog_infostr,"this kinemage = %d, next = %d "       ,
                            thiskinenum,kinemagenum);
    Ldlog_info = 1;
    ACTIVE_DLOG = KINE_DLOG;
}
/*___kineDialog()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndkineDialog()*********************************************************/    
void    EndkineDialog()
{    
 if(Ldlog_OKHIT)
 {
     /* interpret dlog_paramstr as kinemage number */
     nextkinenum = intfromstr(dlog_paramAstr);
     /*if(nextkinenum < 0) nextkinenum = 0;*/ /*971129 become permissive*/
     findkinemage();  /*MAGEFILE.C*/  /*971129*/
 }
 else if(Ldlog_optionHIT)
 {
     /* just take the next kinemage */
     nextkinenum = kinemagenum; /* There is a next number known*/
     Lnextkine = 1; /* take whatever is next */
     findkinemage();  /*MAGEFILE.C*/  /*971129*/
 }
 else if(Ldlog_cancelHIT)
 {
     /* don't change anything, just stay with current kinemage */
     Lcancel = 1;
 }
 ACTIVE_DLOG = 0;
}
/*___EndkineDialog()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DostereoDialog()********************************************************/
void    DostereoDialog()
{
   if(!ACTIVE_DLOG)
  {
      cleargenericflags();  /*MAGEDLOG.c*/
      stereoDialog();       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
            EndstereoDialog();    /*MAGEDLOG.c*/
            /*970408*/ /*build redrawvec() etc. into EndstereoDialog*/
#endif
        }
}
/*___DostereoDialog()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****stereoDialog()**********************************************************/
void    stereoDialog()   /*STEREO_DLOG*/
{
 sprintf(dlog_subjectstr,"Stereo and Display size parameters"); /*041031*/
 Ldlog_subject = 1;
 
 sprintf(dlog_paramAstr,"%.2f",stereoangle);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"set full stereo angle (degrees)");
 Ldlog_textA = 1;
 
 sprintf(dlog_paramBstr,"%d",ieyewide);
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"set full separation (pixels)");
 Ldlog_textB = 1;
 
 sprintf(dlog_paramCstr,"%d",TBYoffset); /*970603*/
 Ldlog_paramC = 1;
 sprintf(dlog_textCstr,"TB Y offset (pixels)");
 Ldlog_textC = 1;

 sprintf(dlog_paramDstr,"%d",ieyeposition);
 Ldlog_paramD = 1;
 sprintf(dlog_textDstr,"set perspective eye position (pixels)");
 Ldlog_textD = 1;

#ifdef UNIX_X11
 sprintf(dlog_paramEstr,"%d",GWIDTH);
 Ldlog_paramE = 1;
 sprintf(dlog_textEstr,"graphics width (pixels)"); /*041031*/
 Ldlog_textE = 1;

 sprintf(dlog_paramFstr,"%d",GHEIGHT);
 Ldlog_paramF = 1;
 sprintf(dlog_textFstr,"graphics height (pixels)");
 Ldlog_textF = 1;
#endif

 dlog_checkAint = LTBstereo;
 sprintf(dlog_checkAstr,"Top/Bottom for StereoGraphics CrystalEyes");
 Ldlog_checkA = 1;
 
/*rotation around eye position adds considerable load to inner loop*/
/*requires back calc of eyeposition as a center in object space*/
/*which then has to be subtracted before, added after, each points rotation*/
/*
 dlog_checkBint = Lrotateye;
 sprintf(dlog_checkBstr,"rotate around eye position");
 Ldlog_checkB = 1;
*/ 
 dlog_checkBint = Lcenterinfo;
 sprintf(dlog_checkBstr,"ptID and dist near center line");
 Ldlog_checkB = 1;

 sprintf(dlog_optionstr,"use defaults");
 Ldlog_option = 1;

 sprintf(dlog_infostr,"                             default  current"
                        CRLF" full angle:            %.2f     %.2f"
                        CRLF" full separation:    %d         %d"
                   CRLF CRLF"graphics Height== %d, Width== %d"
                        ,defaultangle,stereoangle,ieyedefault,ieyewide
                        ,GHEIGHT,GWIDTH);
 Ldlog_info = 1;
 ACTIVE_DLOG = STEREO_DLOG;
}
/*___stereoDialog()_________________________________________________________*/

/****EndstereoDialog()*******************************************************/
void    EndstereoDialog()
{
 int newwidth=0,newheight=0;  /*041031*/

 ACTIVE_DLOG = 0;
 if(Ldlog_OKHIT) 
 {
      stereoangle = floatfromstr(dlog_paramAstr);
      ieyewide = intfromstr(dlog_paramBstr);
      LTBstereo = dlog_checkAint;
      TBYoffset = intfromstr(dlog_paramCstr);
      ieyeposition = intfromstr(dlog_paramDstr);

#ifdef UNIX_X11
      newwidth  = intfromstr(dlog_paramEstr);
      newheight = intfromstr(dlog_paramFstr);
      if(newwidth != GWIDTH || newheight != GHEIGHT)
      {
         resizegrafwindow(newwidth,newheight); /*041031*/
      }
#endif

      /*Lrotateye = dlog_checkBint;*/
      Lcenterinfo = dlog_checkBint;
         /*ON: write ptID and dist near center line*/
      adjuststereo(); /*MAGEMENU*/
 }
 else if(Ldlog_optionHIT)
 {
      stereoangle = defaultangle;
      ieyewide = ieyedefault;
      /*does NOT change stereo state, so windows, etc. stays the same*/
      
      ieyeposition = ieyeposdefault;
      /*Lrotateye = 0;*/ /*default: rotate around image object's center*/
      Lcenterinfo = 0; /*ON: write ptID and dist near center line*/
      redrawvec();  /*on spec.*/
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
}
/*___EndstereoDialog()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*****DodrawlineDialog*******************************************************/
void    DodrawlineDialog()
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();  /*MAGEDLOG.c*/
         drawlineDialog();     /*MAGEDLOG.c*/
         DogenericDialog();    /*MACDLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
         EnddrawlineDialog();  /*MAGEDLOG.c*/
#endif
         LneedSetsinGrafWindow = 1; /*DrawControls(grafWindow); called in MAC*/
         redrawvec();              /*____DRAW*/ 
     }
}
/*___DodrawlineDialog_______________________________________________________*/

/****drawlineDialog()********************************************************/
void    drawlineDialog()  /*971129*/     /*DRAWNEW_DLOG*/
{
 /*initial preferences if draw new not previously invoked */
 if(!Ldrawstuff)
 {
      if(shortenline >.00001 || shortenline < -.00001) Ldrawunpickable = 1;
      else Ldrawunpickable = 0;
 }
 
 sprintf(dlog_subjectstr
         ,"draw new: New Group of user-drawn lines and/or labels");
 Ldlog_subject = 1;
 sprintf(dlog_paramAstr,"%.2f",shortenline);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"shorten lines by this amount");
 Ldlog_textA = 1;
 dlog_checkAint = Ldrawunpickable;
 sprintf(dlog_checkAstr,"line ends unpickable");
 Ldlog_checkA = 1;
 dlog_checkBint = Lmonitor; /*last line length*/
 sprintf(dlog_checkBstr,"Monitor last line length");
 Ldlog_checkB = 1;

 dlog_checkCint = LSplitLine;/*971124SplitLine*/
 sprintf(dlog_checkCstr,"DrawLine in 2 parts");
 Ldlog_checkC = 1;

 dlog_checkDint = Ldottedline;
 sprintf(dlog_checkDstr,"dotted line");
 Ldlog_checkD = 1;

 dlog_checkEint = Larrowline;
 sprintf(dlog_checkEstr,"arrow");
 Ldlog_checkE = 1;

 dlog_checkFint = Lmeasureddihedral;
 sprintf(dlog_checkFstr,"measured dihedral labeled");
 Ldlog_checkF = 1;

 sprintf(dlog_OKstr,"OK");  /*971129*/
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = DRAWNEW_DLOG;
}
/*___drawlineDialog()_______________________________________________________*/

/****EnddrawlineDialog()*****************************************************/
void    EnddrawlineDialog()  /*971129*/
{
 
 if(Ldlog_cancelHIT) ; /*do nothing, ignore any changes*/ 
 else if(Ldlog_OKHIT) 
 {
                /*maybe wanted to change shortenline value*/
                /*maybe unpickable is set/unset*/
                /*maybe monitor last line is set/unset*/
           shortenline = floatfromstr(dlog_paramAstr); /*MAGEUTIL.C*/
           Ldrawunpickable = dlog_checkAint;
           Lmonitor = dlog_checkBint; /*last line length*/
           if(Lmonitor) Lmonitoron = 1; /*last line length*/
           else         Lmonitoron = 0; /*last line length*/

           LSplitLine  = dlog_checkCint; /*971124SplitLine*/
           Ldottedline = dlog_checkDint; /*990710*/
           Larrowline  = dlog_checkEint;
           Lmeasureddihedral  = dlog_checkFint;
 }
 resetmenuchecks(); /*MAGEMENU*/  /*971010*/ 
 ACTIVE_DLOG = 0; 
}
/*___EnddrawlineDialog()____________________________________________________*/

/****DoConstructFifthDialog()************************************************/
void    DoConstructFifthDialog()  /*971122*/
{
 if(!ACTIVE_DLOG) 
 {
    cleargenericflags();  /*MAGEDLOG.c*/
    ConstructFifthDialog();       /*MAGEDLOG.c*/
    DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
    EndConstructFifthDialog();    /*MAGEDLOG.c*/
#endif
 }
}
/*___DoConstructFifthDialog()_______________________________________________*/

/****ConstructFifthDialog()**************************************************/
void    ConstructFifthDialog()  /*971122 construct5*/  /*CONSTRUCTFIFTH_DLOG*/
{
 sprintf(dlog_subjectstr
    ,"Construct line to 5th pt, or line 5--6 based on previous 4 points");
 Ldlog_subject = 1;
 
    /*121110 MAGEDLOG.h: box strings are 64 char*/
    /*1234567890123456789012345678901234567890123456789012345678901234*/
 dlog_radioAint = LPerpendicularToPlane; 
 sprintf(dlog_radioAstr
    ,"Perpendicular from 4th pt to plane 1--2--3  ___________________");
 Ldlog_radioA = 1;
 
 dlog_radioBint = LPerpendicularBetweenLines;  
 sprintf(dlog_radioBstr,"Perpendicular between lines 1--2 & 3--4");
 Ldlog_radioB = 1;
 
 dlog_radioCint = LShortestBetweenLinesegments;  
 sprintf(dlog_radioCstr,"Shortest distance between segments 1--2 & 3--4");
 Ldlog_radioC = 1;
 
 dlog_radioDint = LAngleBetweenLines;  /*140518*/
 sprintf(dlog_radioDstr,"Angle between vectors 1--2 & 3--4"); /*140518*/
 Ldlog_radioD = 1; /*140518*/

   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = CONSTRUCTFIFTH_DLOG;
}
/*___ConstructFifthDialog()_________________________________________________*/

/****EndConstructFifthDialog()***********************************************/
void    EndConstructFifthDialog()  /*971122*/
{
  if(Ldlog_OKHIT) 
  {
    LPerpendicularToPlane = dlog_radioAint;
    LPerpendicularBetweenLines = dlog_radioBint;
    LShortestBetweenLinesegments = dlog_radioCint;
    LAngleBetweenLines = dlog_radioDint; /*140518*/
    Lconstructperpendicular = 1;
    doconstructline();    /*MAGEANGL.C*/
  }
  else if(Ldlog_cancelHIT)
  {
    LPerpendicularToPlane = 0;
    LPerpendicularBetweenLines = 0;
    LShortestBetweenLinesegments = 0;
    LAngleBetweenLines = 0; /*140518*/
  }
  Lconstructperpendicular = 0;
  LPerpendicularToPlane = 0;
  LPerpendicularBetweenLines = 0;
  LShortestBetweenLinesegments = 0;
  LAngleBetweenLines = 0; /*140518*/
  drawmarker1listptr->on = 0; /*turn off point indicaters*/
  drawmarker2listptr->on = 0;
  drawmarker3listptr->on = 0;
  drawmarker4listptr->on = 0;
  Lpoint = 0;

  ACTIVE_DLOG = 0;
  redrawvec();              /*141126*/
}
/*___EndConstructFifthDialog()______________________________________________*/

/****DoConstructSixthDialog()************************************************/
void    DoConstructSixthDialog()  /*140912*/
{
 if(!ACTIVE_DLOG)
 {
    LNucleicAcidParameters = 1; /*140915 this default choice in dialog*/
    LParameterlinekinemage = 0; /*140915 default choice in dialog*/
    cleargenericflags();  /*MAGEDLOG.c*/
    ConstructSixthDialog();       /*MAGEDLOG.c*/

    DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */
    EndConstructSixthDialog();    /*MAGEDLOG.c*/
#endif
 }
}
/*___DoConstructSixthDialog()_______________________________________________*/

/****ConstructSixthDialog()**************************************************/
void    ConstructSixthDialog()  /*140912 construct6*/  /*CONSTRUCTSIXTH_DLOG*/
{
 sprintf(dlog_subjectstr
    ,"140912, Nucleic acid backbone pick: -1 n1, -1 c1', 1 c1', 1 n1, -1 P, 1 P"
     CRLF"base n, ribose c1' of -1 residue, ribose c1', base n of next,"
     CRLF"then P of intervening PO4, and P of next (#1) residue"
     CRLF"IF NEXT P MISSING, CLICK ON EARLIER P AGAIN!"
//     CRLF"6 atoms from which to compute (new) parameter set"
//     CRLF"returns backbone params defined 140912 vintage..."
//     CRLF"Construct6 name from historic drawline code model"
//     CRLF"monitorparam lines constructed from 6 points"
        ); /*limited to 256 characters*/
 Ldlog_subject = 1;

    /*121110 MAGEDLOG.h: box strings are 64 char*/
    /*1234567890123456789012345678901234567890123456789012345678901234*/
 dlog_radioAint = LNucleicAcidParameters;
 sprintf(dlog_radioAstr
    ,"NucleicAcidParameters to screen and param as text to stdout");
 Ldlog_radioA = 1;

 dlog_radioBint = LParameterlinekinemage;
 sprintf(dlog_radioBstr
    ,"LParameterlinekinemage: param to screen, kinemage code: stdout");
 Ldlog_radioB = 1;

   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = CONSTRUCTSIXTH_DLOG;
}
/*___ConstructSixthDialog()_________________________________________________*/

/****EndConstructSixthDialog()***********************************************/
void    EndConstructSixthDialog()  /*971122*/
{
  if(Ldlog_OKHIT)
  {
    LNucleicAcidParameters = dlog_radioAint;
    LParameterlinekinemage = dlog_radioBint;
    /*Lpoint++;*/ /*so now will be routed rather than have picked point stored*/
          /*not the place for this... */
    doconstructline();    /*MAGEANGL.c uses LNucleicAcidParameters*/
  }
  else if(Ldlog_cancelHIT)
  {
    LNucleicAcidParameters = 0;
    LParameterlinekinemage = 0;
  }
  /*reset Logical Flags: */
  LNucleicAcidParameters = 0;
  LParameterlinekinemage = 0;
  drawmarker1listptr->on = 0; /*turn off point indicaters*/
  drawmarker2listptr->on = 0;
  drawmarker3listptr->on = 0;
  drawmarker4listptr->on = 0;
  drawmarker5listptr->on = 0;
  drawmarker6listptr->on = 0; /*as of 140912 only 6 defined, 6 used here*/
  Lpoint = 0;

  ACTIVE_DLOG = 0;
  redrawvec();              /*141126*/
}
/*___EndConstructSixthDialog()______________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoSearchDialog*********************************************************/
/* is done in a platform specific way because it is reentrant               */
/*___DoSearchDialog_________________________________________________________*/  
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****SearchDialog()**********************************************************/
void    SearchDialog()   /*FIND_DLOG*/ /*whole word option 051004*/
{
 sprintf(dlog_subjectstr
   // ,"Search pointID fields for words, numbers, or sets of characters"
   //CRLF"Default mode matches single word or number, ignores spaces at ends,"
   //CRLF"uncheck to match exact string, including spaces."    
   //CRLF"Each box can take one word or number,"
   //CRLF"if both given, both matched."
    ,"Search pointID fields for words, numbers, or sets of characters"
   CRLF"Default match exact string fragment, including spaces."    
   CRLF"check for isolated single word or number, ignores spaces at ends,"
   CRLF"One fragment, word, number per box:"
   CRLF"if both given, both matched."
   ); /*121109 NOTE: limited to 256 characters*/

 Ldlog_subject = 1;
 sprintf(dlog_paramAstr,"%s",user1str);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"Primary word, number, or set of characters");
 Ldlog_textA = 1;
 sprintf(dlog_paramBstr,"%s",user2str);
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"Optional additional entry");
 Ldlog_textB = 1;

 sprintf(dlog_textCstr,"%s",searchstatusstr);
 Ldlog_paramC = 0; /*do not show this box at first! */
 Ldlog_textC = 1; /*use dlog_textCstr for, e.g., NOT FOUND message*/
 
 if( Lmarkers) /*markers are in maxgrp+1 if there was room */
 {       
     /*if there are markers available and nothing is on that*/
     /*will flag that a search was successful, turn markers on*/
     if(   !Lnewlabelson 
        && !Lnewballson 
        && !Lpickcenteron 
        && !markergrupptr->on)
     {
          dlog_checkAint = 1;
       /*markergrupptr->on = 1;*/ /*force markers group on*/ /*not yet, 990119*/
     }
     else dlog_checkAint = markergrupptr->on; /*as it was*/
     sprintf(dlog_checkAstr,"markers on");
     Ldlog_checkA = 1;
 }
 else
 {     
     dlog_checkAint = 0; /*no markers possible*/
     sprintf(dlog_checkAstr," ");
     Ldlog_checkA = 0;
 }
 Lpickcenteron = 1; /*020608 default*/
 dlog_checkBint = Lpickcenteron;
 sprintf(dlog_checkBstr,"pickcenter on");
 Ldlog_checkB = 1;

 if(Ltablepresent)
 {
    dlog_checkCint = Ltablesearchcells; /*000324*/
    sprintf(dlog_checkCstr,"search table cells");
    Ldlog_checkC = 1;
 }

 /*checkA ---- checkE all in one horizontal row */
 /*checkF is in its own separate row below the A----E row */

 dlog_checkFint = Lfindwholeword;  /*051004, 121109 MAGEINIT/initialvalues  */
 /*sprintf(dlog_checkFstr,"search will match entire word or number");*/
 sprintf(dlog_checkFstr,"search for isolated entire word or number");
 Ldlog_checkF = 1; 

 dlog_radioAint = Lsearchbegin=1; 
 sprintf(dlog_radioAstr,"Search from beginning");
 Ldlog_radioA = 1;
 dlog_radioBint = !Lsearchbegin;  
 sprintf(dlog_radioBstr,"Find Next occurrence");
 Ldlog_radioB = 1;
 
 sprintf(dlog_OKstr,"Find");
 sprintf(dlog_cancelstr,"CANCEL");
 
 
 if(Lsearchstatus)
 {
     sprintf(dlog_optionstr,"re-hit last picked point");
     Ldlog_option = 1;
 }
 
 sprintf(dlog_infostr,"marker, pickcenter, or label can be placed"
         CRLF"  use draw-new to set up for label"
         CRLF" shortcut keys:   F  find,   G  find next"
        );
 Ldlog_info = 1;
 ACTIVE_DLOG = FIND_DLOG;
}
/*___SearchDialog()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndSearchDialog()*******************************************************/
void    EndSearchDialog()   /*FIND  incl  whole word*/
{
 int Lfound = 0; /*000324*/
 int Nblank = 0; /*000410*/
 int j=0,k=0,n=0;    /*051004*/
 char whatstr[256],lookstr[256]; /*121109 for NOT FOUND info*/

 if(Ldlog_OKHIT) 
 {
    markergrupptr->on = dlog_checkAint; 
    Lpickcenteron = dlog_checkBint;
    if(Ltablepresent) {Ltablesearchcells = dlog_checkCint;}
    Lfindwholeword = dlog_checkFint;  /*051004*/
      /*search will match entire word or number*/
    
    strcpy(user1str,dlog_paramAstr);  /*save users original strings 051005*/
    strcpy(user2str,dlog_paramBstr);  /*051005*/

    Lsearchbegin = dlog_radioAint; /* 1 search from beginning*/
                                   /* 0 find next occurrence*/

    if(Lsearchbegin || !Lsearchatend)
    {/*can do a search*/
      
      /*MAGEUTIL/matchstrings() has to honor Lfindwholeword 051004*/
      if(Lfindwholeword) /*pad and/or strip space characters from ends*/
      {
          k = 0; /*index into search string*/
          search1str[k++] = ' '; /*force leading blank on search string*/ 
          n = 0; /*counter of non-space characters*/
          for(j=0; j<32; j++)
          {
             if(dlog_paramAstr[j] == '\0') /*end of this string*/
             {
                break;
             }
             else if(dlog_paramAstr[j] == ' ') /*blank space encountered*/
             {
                if(n==0) /*no non-space character yet encountered */
                {
                   continue; /*ignore leading spaces in typed input string*/
                }
                else /*space after non-space character */
                {
                   break; /*end of non-space set of characters*/
                }
             }
             else /*non-space character*/
             {
                search1str[k++] = dlog_paramAstr[j];
                n++;
             }
         }
         search1str[k++] = ' '; /*force trailing space*/
         search1str[k]  = '\0'; /*terminate string*/
      }
      else
      {
         sprintf(search1str,"%s",dlog_paramAstr); /*take exactly as inputed*/
      }
      for(isearch1=0; search1str[isearch1]!='\0';isearch1++) /*count char*/ 
      {
         if(search1str[isearch1]==' ') Nblank++; /*000410*/
      }
      if(Nblank == isearch1) isearch1=0;/*Do NOT search for field of blank(s)*/

      if(Lfindwholeword) /*pad and/or strip space characters from ends*/
      {
          k = 0; /*index into search string*/
          search2str[k++] = ' '; /*force leading blank on search string*/ 
          n = 0; /*counter of non-space characters*/
          for(j=0; j<32; j++)
          {
             if(dlog_paramBstr[j] == '\0') /*end of this string*/
             {
                break;
             }
             else if(dlog_paramBstr[j] == ' ') /*blank space encountered*/
             {
                if(n==0) /*no non-space character yet encountered */
                {
                   continue; /*ignore leading spaces in typed input string*/
                }
                else /*space after non-space character */
                {
                   break; /*end of non-space set of characters*/
                }
             }
             else /*non-space character*/
             {
                search2str[k++] = dlog_paramBstr[j];
                n++;
             }
         }
         search2str[k++] = ' '; /*force trailing space*/
         search2str[k]  = '\0'; /*terminate string*/
      }
      else
      {
         sprintf(search2str,"%s",dlog_paramBstr); /*take exactly as inputed*/
      }
      for(isearch2=0; search2str[isearch2]!='\0';isearch2++){ ; } /*count char*/
      
      if(isearch1 > 0) /*at least one char in essential string */
      {
        if(Ltablepresent && Ltablesearchcells) /*DO SEARCH IN TABLE*/
        {/*search table cell contents: <comment> flag==1*/
           Lfound = searchtablecells(1); /*MAGETABL.c*/  /*000324*/
           if(Lfound) /*000411*/
          {/*back pick this table cell to its corresponding points in graphics*/
              removetablemarkpoints();
              markfromtablecellsID(1);/*MAGETABL.c flag==1 for ptID matching*/ 
          }
        }
        else
        {/*DO SEARCH ON POINT IDs of GRAPHICS ITEMS*/
           Lfound = mysearchpointID(1);/*MAGEUTIL.c for justone hit*/
        }
        if(Lfound) /*000324*/
        {
            sprintf(searchstatusstr," ");
            Lsearched = 0; /*successful, so let dialog end*/
            Lsearchstatus = 1;
        }
        else 
        {
            /*121109 need more info back to user about a failed search*/
            if(Lfindwholeword) {sprintf(whatstr,"Isolated TEXT ");}
            else {sprintf(whatstr,"fragment TEXT ");}
            if((isearch1 > 0) && (isearch2 > 0)) 
                {sprintf(lookstr," %s %s ",search1str,search2str);}
            else if(isearch1 > 0) {sprintf(lookstr," %s ",search1str);}
            else if(isearch2 > 0) {sprintf(lookstr," %s ",search2str);}
            sprintf(searchstatusstr,"%s %s NOT FOUND",whatstr,lookstr);

            /*sprintf(searchstatusstr,"NOT FOUND"); 121109 replaced*/
            Lsearchatend = 1;
            Lsearched = 1; /*still searching, do dialog again*/
            Lsearchstatus = 0;
        }
      }
      else 
      {
          sprintf(searchstatusstr,"NO SEARCH STRING");
          Lsearched = 1; /*still searching, do dialog again*/
          Lsearchstatus = 0;
      }

    }/*can do a search*/
    else
    {
        sprintf(searchstatusstr,"AT END OF FILE");
        Lsearched = 1; /*still searching, do dialog again*/
        Lsearchstatus = 0;
    }
 }
 else if(Ldlog_optionHIT && Lsearchstatus)
 {
     Lpick = 1; /* flag for successful pick */
     ipick = 0; /*so will not try to find a pickpoint by cursor x,y*/
     Lprepick = 1; /*so will know to apply pick process to pickvec */
     pickedpointptr = searchpointptr;
     pickedgrupptr = searchgrupptr;
     pickedsgrpptr = searchsgrpptr;
     pickedlistptr = searchlistptr;

     sprintf(searchstatusstr," ");
     Lsearched = 0; /*supposedly this point does exist, so let dialog end*/
     Lsearchstatus = 1;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just quit */
      Lsearched = 0; /* let dialog end */
 }
 else
 {
      sprintf(searchstatusstr,"TRY AGAIN");
      Lsearched = 1; /*still searching, do dialog again*/
      Lsearchstatus = 0;
 }

 if(Lpickcenteron && !Lcancel)
 {
     if(Lmeasureson)  Lmeasureson=0; 
     if(Lnewlineson)  Lnewlineson=0; 
     if(Lnewlabelson) Lnewlabelson=0; 
     if(Lnewballson) Lnewballson=0; 
     if(Ldraglineon)  Ldraglineon=0; 
     if(Lconstruct4on) Lconstruct4on=0;  /*971122*/
     if(Lconstruct5on) Lconstruct5on=0;  /*971122*/
     if(Lconstruct6on) Lconstruct6on=0;  /*140912*/
     if(Lpickshowon) Lpickshowon=0;  /*990408*/
     if(Lmovepointson) Lmovepointson=0;  /*061126*/
     if(Lmeansigmason) Lmeansigmason=0;  /*061126*/
     if(Lpickcoloron) Lpickcoloron=0;
     if(Lpruneon) Lpruneon = 0;
     if(Lpunchon) Lpunchon = 0;
     if(Lsuperpunchon) Lsuperpunchon = 0;
 }

 ACTIVE_DLOG = 0;
 if(Lsearched)
 {
    /*do some minimal house cleaning for another cycle*/
    Ldlog_OKHIT = 0;
    Ldlog_optionHIT = 0;
    Ldlog_cancelHIT = 0;

    /*SearchDialog();*/ /*put up dialog box again*/ /*971001*/
    /*DogenericDialog();*/    /*____DLOG.c*/ /*971001*/
 }
}
/*___EndSearchDialog()______________________________________________________*/

/****DoViewsDialog()*********************************************************/
void    DoViewsDialog(int j)
{
     if(!ACTIVE_DLOG)
     {
         cleargenericflags();  /*MAGEDLOG.c*/
         ViewsDialog(j);/*NOTE the int j */   /*MAGEDLOG.c*/
         DogenericDialog();    /*MPCDLOG.c*/
#ifndef UNIX_X11  /*called after DLOG_OKHIT_CB*/
         EndViewsDialog();     /*MAGEDLOG.c*/
#endif
     }
}
/*___DoViewsDialog()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****ViewsDialog()***********************************************************/
void    ViewsDialog(int j) /*VIEW_DLOG*/ /*adjust view is MAGEANGL/resetrot()*/
{

 if(j==0)
 {/*work out best choice for this next view number*/
     for(j=1;j<=MAXRESETS;j++)      
         if(Lview[j]==0) break;
     if(j>MAXRESETS) j=MAXRESETS;
     sprintf(dlog_subjectstr
        ,"Make current screen image a numbered view, or delete old view");
     Ldlog_subject = 1;
 }/*work out best choice for this next view number*/
 else
 {/*use the given number which was probably out of range*/  
    /*so the user can see why the earlier view update request failed*/
    sprintf(dlog_subjectstr
     ,"%d Out-Of-Range: Erase then type a number between 1 and %d",j,MAXRESETS);
    Ldlog_subject = 1;
 }/*use the given number which was probably out of range*/  

 sprintf(dlog_paramAstr,"%d",j);  /*old Mac 6 */
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"Make view as currently seen be this number");
 Ldlog_textA = 1;

 sprintf(dlog_paramBstr,"%s",viewcom[iviewset]); /*old Mac 7 */
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"View ID");
 Ldlog_textB = 1;



 sprintf(dlog_paramCstr,"last selected: %d",iviewset); 
 Ldlog_paramC = 1;
 if(iviewset == 1) 
      sprintf(dlog_textCstr,"can't delete view1");  /*old Mac 4 */      
 else if(iviewset == MAXRESETS)
      sprintf(dlog_textCstr,"view%-d (reader's view)",MAXRESETS);
 else sprintf(dlog_textCstr,"View%1d %s",iviewset,viewcom[iviewset]);
 Ldlog_textC = 1;

#ifdef UNIX_X11
 sprintf(dlog_textDstr,"last selected-view size %d, %d (0,0 normal, other valuesstandardly ignored)"
     ,gwidthview[iviewset],gheightview[iviewset]);
 Ldlog_textD = 1;

 dlog_checkAint = 0;
 sprintf(dlog_checkAstr,"save an expert-use view with current size %d, %d",GWIDTH,GHEIGHT);
 Ldlog_checkA = 1;

 sprintf(dlog_extrastr,"APPLY last view size"); /*041031*/
 Ldlog_extra = 1;
#endif
 
 if(LNdimensions) /*was L7dimensions 060622*/
 {
   dlog_checkFint = 1; /*A,B,C,D,E one row, F starts a new row of check boxes*/
   sprintf(dlog_checkFstr,"make view include axischoice %d %d %d"
     ,NX+1,NY+1,NZ+1); /*061112 axes count from 1, internally stored from 0*/
   Ldlog_checkF = 1;
 }


 sprintf(dlog_optionstr,"Delete last selected view");
 Ldlog_option = 1;        

 sprintf(dlog_radioAstr,"store zoom value directly as a scaling value");
 Ldlog_radioA = 1;
 sprintf(dlog_radioBstr,"calculate span of image area from zoom value");
 Ldlog_radioB = 1;
 if(Lzoomer[j] >= 0)
 {
     dlog_radioAint = 1;
     dlog_radioBint = 0;
 }
 else
 {
     dlog_radioAint = 0;
     dlog_radioBint = 1;
 }
 sprintf(dlog_infostr
 ,"File/Save Special/Current View  writes zoom or span"
 CRLF"depending on the last setting OKed from here.");
 Ldlog_info = 1;
 ACTIVE_DLOG = VIEW_DLOG;
}
/*___ViewsDialog()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****EndViewsDialog()********************************************************/
void    EndViewsDialog() /*adjust view is MAGEANGL/resetrot()*/
{
 int    i=0,j=0,k=0;
 if(Ldlog_OKHIT) 
 {/*OK: make this view be a new & current one*/
 
     j = intfromstr(dlog_paramAstr);

   /*recondition(dlog_paramBstr);*/ /*MAC: P to C string,(dummied)in ___MAIN.C*/
   /*970818 done in MACDLOG/genericDialog() */
     sprintf(temps,"%s",dlog_paramBstr); 
     if(j>0 && j< MAXRESETS)
     {/*requested view number in range*/
         if(dlog_checkAint) /*041031 write view with current size*/
         {
            Lgwidthheightview[j] = 1;
            gwidthview[j]  = GWIDTH;
            gheightview[j] = GHEIGHT;
         }
          
         /*was L7dimensions 060622*/
         if(LNdimensions && dlog_checkFint) /*axischoice 060613*/
         {
            Laxischoice[j] = 1;
            axischoice[j][0] = NX;
            axischoice[j][1] = NY;
            axischoice[j][2] = NZ;
         }

         if(dlog_radioAint)
         {
             Lzoomer[j] =  1; /*zoom itself*/ /*980929*/
             Lspanfromzoom = 0;
         }
         else
         {
             Lzoomer[j] = -1; /*span from which to calculate zoom*/
             Lspanfromzoom = 1;
         }     
         /*MAC & PC use integers, MUXM uses Widget for theitem*/
         if     (j==1) theitem = reset1Item;
         else if(j==2) theitem = reset2Item;
         else if(j==3) theitem = reset3Item;
         else if(j==4) theitem = reset4Item;
         else if(j==5) theitem = reset5Item;
         else if(j==6) theitem = reset6Item;
         else if(j==7) theitem = reset7Item;
         else if(j==8) theitem = reset8Item;
         else if(j==9) theitem = reset9Item;
         else if(j==10) theitem = reset10Item;
         else if(j==11) theitem = reset11Item;
         else if(j==12) theitem = reset12Item;
         else if(j==13) theitem = reset13Item;
         else if(j==14) theitem = reset14Item;
         else if(j==15) theitem = reset15Item;
         else if(j==16) theitem = reset16Item;
         else if(j==17) theitem = reset17Item;
         else if(j==18) theitem = reset18Item;
         else if(j==19) theitem = reset19Item;
         else if(j==20) theitem = reset20Item;
         else if(j==21) theitem = reset21Item;
         else if(j==22) theitem = reset22Item;
         else if(j==23) theitem = reset23Item;
         else if(j==24) theitem = reset24Item;
         else if(j==25) theitem = reset25Item;
         else if(j==26) theitem = reset26Item;
         else if(j==27) theitem = reset27Item;
         else if(j==28) theitem = reset28Item;
         else if(j==29) theitem = reset29Item;
         else if(j==30) theitem = reset30Item;
         else if(j==31) theitem = reset31Item;
         else if(j==32) theitem = reset32Item;
         else if(j==33) theitem = reset33Item;
         else if(j==34) theitem = reset34Item;
         else if(j==35) theitem = reset35Item;
         else if(j==36) theitem = reset36Item;
         else if(j==37) theitem = reset37Item;
         else if(j==38) theitem = reset38Item;
         else if(j==39) theitem = reset39Item;
         else if(j==40) theitem = reset40Item;
         else if(j==41) theitem = reset41Item;
         else if(j==42) theitem = reset42Item;
         else if(j==43) theitem = reset43Item;
         else if(j==44) theitem = reset44Item;
         else if(j==45) theitem = reset45Item;
         else if(j==46) theitem = reset46Item;
         else if(j==47) theitem = reset47Item;
         else if(j==48) theitem = reset48Item;
         else if(j==49) theitem = reset49Item;
         else if(j==50) theitem = reset50Item;
         else if(j==51) theitem = reset51Item;
         else if(j==52) theitem = reset52Item;
         else if(j==53) theitem = reset53Item;
         else if(j==54) theitem = reset54Item;
         else if(j==55) theitem = reset55Item;
         else if(j==56) theitem = reset56Item;
         else if(j==57) theitem = reset57Item;
         else if(j==58) theitem = reset58Item;
         else if(j==59) theitem = reset59Item;
         else if(j==60) theitem = reset60Item;
         else if(j==61) theitem = resetRItem;
         for(i=0;i<MAXVIEWIDCHAR;i++) viewcom[j][i] = temps[i];
         viewcom[j][MAXVIEWIDCHAR] = '\0';
         Lview[j] = 1;
         sprintf(temps,"View%d %s",j,viewcom[j]);
         condition(temps);/*MAC: C to P string, (dummied) in ___MAIN.C*/
         setviewidentifier(j,temps);  /*980919*/
         /*uncheck all Views, check new one */
         checkcurrentviewmenu(j); /*____MENU.C, goes by integer*/
         AdjustMenus();           /*____MENU.C*/        
         setrotview(j);           /*MAGEANGL.C*/
         Lnewviewactive = 1; /*flag on exit for changed kin*/ /*971129*/
       ACTIVE_DLOG = 0;/*NOTE: case Ldlog_OKHIT may have to open dialog again!*/
     }/*requested view number in range*/
     else
     {/*view number out of range, e.g. mouse select didn't clear field*/
#ifdef UNIX_X11
         fprintf(stderr,"%d : view number out of range, try again...\n",j);
#endif
         ACTIVE_DLOG = 0;/*NOTE: set==0 so system will open dialog again!*/
         DoViewsDialog(j); /*send the out-of-range j value */
     }/*view number out of range, e.g. mouse select didn't clear field*/
/*
printf("just did reset view %d\n",j);
*/
 }/*OK: make this view be a new & current one*/
 else if(Ldlog_optionHIT) 
 {/*delete view*/
     if(iviewset>1)
     {
         if(iviewset==1) theitem = reset1Item;
         if(iviewset==2) theitem = reset2Item;
         if(iviewset==3) theitem = reset3Item;
         if(iviewset==4) theitem = reset4Item;
         if(iviewset==5) theitem = reset5Item;
         if(iviewset==6) theitem = reset6Item;
         if(iviewset==7) theitem = reset7Item;
         if(iviewset==8) theitem = reset8Item;
         if(iviewset==9) theitem = reset9Item;
         if(iviewset==10) theitem = reset10Item;
         if(iviewset==11) theitem = reset11Item;
         if(iviewset==12) theitem = reset12Item;
         if(iviewset==13) theitem = reset13Item;
         if(iviewset==14) theitem = reset14Item;
         if(iviewset==15) theitem = reset15Item;
         if(iviewset==16) theitem = reset16Item;
         if(iviewset==17) theitem = reset17Item;
         if(iviewset==18) theitem = reset18Item;
         if(iviewset==19) theitem = reset19Item;
         if(iviewset==20) theitem = reset20Item;
         if(iviewset==21) theitem = reset21Item;
         if(iviewset==22) theitem = reset22Item;
         if(iviewset==23) theitem = reset23Item;
         if(iviewset==24) theitem = reset24Item;
         if(iviewset==25) theitem = reset25Item;
         if(iviewset==26) theitem = reset26Item;
         if(iviewset==27) theitem = reset27Item;
         if(iviewset==28) theitem = reset28Item;
         if(iviewset==29) theitem = reset29Item;
         if(iviewset==30) theitem = reset30Item;
         if(iviewset==31) theitem = reset31Item;
         if(iviewset==32) theitem = reset32Item;
         if(iviewset==33) theitem = reset33Item;
         if(iviewset==34) theitem = reset34Item;
         if(iviewset==35) theitem = reset35Item;
         if(iviewset==36) theitem = reset36Item;
         if(iviewset==37) theitem = reset37Item;
         if(iviewset==38) theitem = reset38Item;
         if(iviewset==39) theitem = reset39Item;
         if(iviewset==40) theitem = reset40Item;
         if(iviewset==41) theitem = reset41Item;
         if(iviewset==42) theitem = reset42Item;
         if(iviewset==43) theitem = reset43Item;
         if(iviewset==44) theitem = reset44Item;
         if(iviewset==45) theitem = reset45Item;
         if(iviewset==46) theitem = reset46Item;
         if(iviewset==47) theitem = reset47Item;
         if(iviewset==48) theitem = reset48Item;
         if(iviewset==49) theitem = reset49Item;
         if(iviewset==50) theitem = reset50Item;
         if(iviewset==51) theitem = reset51Item;
         if(iviewset==52) theitem = reset52Item;
         if(iviewset==53) theitem = reset53Item;
         if(iviewset==54) theitem = reset54Item;
         if(iviewset==55) theitem = reset55Item;
         if(iviewset==56) theitem = reset56Item;
         if(iviewset==57) theitem = reset57Item;
         if(iviewset==58) theitem = reset58Item;
         if(iviewset==59) theitem = reset59Item;
         if(iviewset==60) theitem = reset60Item;
         if(iviewset==61) theitem = resetRItem;
         Lview[iviewset] = 0;
         for(i=0;i<=MAXVIEWIDCHAR;i++) viewcom[iviewset][i] = '\0'; 
         sprintf(temps,"View%d %s",iviewset,viewcom[iviewset]);
         condition(temps);
         setviewidentifier(j,temps);  /*980919*/
         /*uncheck all Views, not check any: i=0 */
         checkcurrentviewmenu(0); /*____MENU.C, goes by integer*/
         AdjustMenus(); /*____MENU.C*/
         Lnewviewactive = 1; /*flag on exit for changed kin*/ /*971129*/
     }
     ACTIVE_DLOG = 0;/*NOTE: case Ldlog_OKHIT may have to open dialog again!*/
 }/*delete view*/
#ifdef UNIX_X11 /*rearranged 050121*/
 else if(Ldlog_extraHIT) /*apply last selected view width,height  041031*/
 {
    if(gwidthview[iviewset] > 0 && gheightview[iviewset] > 0)
    {
        resizegrafwindow(gwidthview[iviewset],gheightview[iviewset]);
        ACTIVE_DLOG = 0;
    }
    else
    {/*view did not specifiy a size*/
        fprintf(stderr,"%d view did not specifiy a size\n",iviewset);
        ACTIVE_DLOG = 0;/*NOTE: set==0 so system will open dialog again!*/
        DoViewsDialog(iviewset);
    }
 }
#endif
 else if(Ldlog_cancelHIT) /*cancel*/
 {
     ; /*no action */
     ACTIVE_DLOG = 0;/*NOTE: case Ldlog_OKHIT may have to open dialog again!*/
 }
 if(Lnewviewactive) /*there may have been a change in number of views*/
 {
     /*find the highest number now used for views (not incl readers view) */
     for(k=MAXRESETS-1; k>=0; k--) /* reader's view == MAXRESETS */
     {
         if(Lview[k]==0) {;}
         else { break; } /*with value of k == highest view number */
     }
     if(k < 0) {k=0;}  /*protection if run off end when no views defined*/
     nviewsthiskinemage = k; /*new appended views go after this 051120*/
 }
}
/*___EndViewsDialog()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoChangeCheckDialog****************************************************/
void    DoChangeCheckDialog()
{
   if(!ACTIVE_DLOG)
  {
     cleargenericflags();    /*MAGEDLOG.c*/
     ChangeCheckDialog();    /*MAGEDLOG.c*/
     DogenericDialog();      /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
     EndChangeCheckDialog(); /*MAGEDLOG.c*/
#endif
  }
}
/*___DoChangeCheckDialog____________________________________________________*/

/****ChangeCheckDialog()*****************************************************/
void     ChangeCheckDialog()  /*971129  modified*/  /*CHANGE_DLOG*/
{
 /*figure out why this change-check was invoked*/
 if(Lpruneactive) /*prune-punch-recolored and maybe pick-showed*/
 {
     intpass = 5; /*modified kinemage: write out whole thing*/
      sprintf(dlog_subjectstr
              ,"kinemage has been modified: write out whole thing?");
 }
 else if(Ldrawactive || Lclosedrawstuff) /* has new draw stuff*/
 {
     intpass = 5; /* draw new: write out whole thing*/
     sprintf(dlog_subjectstr
             ,"draw_new items have been made: write new kinemage?");
 }
 else if(Lnewviewactive) /*has new views created*/
 {
     intpass = 5; /* new views: write out whole thing*/
     sprintf(dlog_subjectstr
             ,"new views have been made: write new kinemage?");
 }
 else if(Lnewstuffactive) /*has new stuff added to kinemage*/
 {
     intpass = 5; /* new stuff: write out whole thing*/
     sprintf(dlog_subjectstr
             ,"new additions or changes have been made: write new kinemage?");
 }
 else 
 {
     intpass = 5; /* changes: write out whole kinemage*/
     sprintf(dlog_subjectstr
             ,"changes have been made: write new kinemage?");
 }

 Ldlog_subject = 1;
 
 sprintf(dlog_optionstr,"proceed, no write         ");
 Ldlog_option = 1;
 
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = CHANGE_DLOG;
}
/*___ChangeCheckDialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndChangeCheckDialog()**************************************************/
void     EndChangeCheckDialog()  /*971129 modified*/
{        

/*LChangeCheckCode (global) 3 for turnoff draw-new*/
/*intpass (global) 3 for write draw-new parts*/
 ACTIVE_DLOG = 0;
 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
      Lpruneactive = 0;
      Ldrawactive = 0;
      Lnewviewactive = 0;
      Lnewstuffactive = 0;
      LquitMAGE = 0;
      Lgetnewfile = 0;
      Lclosedrawstuff = 0;
      Lgetnextkinemage = 0;
      Lgetkinemagechoice = 0;
 }
 else if(Ldlog_optionHIT) 
 {
      Lpruneactive = 0;
      Ldrawactive = 0;
      Lnewviewactive = 0;
      Lnewstuffactive = 0;

      if(LquitMAGE) quitMAGE(); /*MACMENU,MPCMAIN,MUXMMENU*/
      else if(Lgetnewfile) OpenNewFile(); /*____DLOG.c*/
      else if(Lclosedrawstuff) closedrawstuff(); /*MAGEMENU*/
      else if(Lgetnextkinemage) getnextkinemage(); /*MAGEFILE.C*/
      else if(Lgetkinemagechoice) getkinemagechoice(); /*MAGEUTIL*/
 }
 else if(Ldlog_OKHIT) 
 {
      writeoutput(intpass); /*____OUT.c*/
      Lpruneactive = 0;
      Ldrawactive = 0;
      Lnewviewactive = 0;
      Lnewstuffactive = 0;
 }
}
/*___EndChangeCheckDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoSetgraffontsDialog()**************************************************/
void    DoSetgraffontsDialog()
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
       SetgraffontsDialog();        /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
      EndSetgraffontsDialog();     /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
       if(Ltablepresent) redrawtable();
  }
}
/*___DoSetgraffontsDialog()_________________________________________________*/

/****SetgraffontsDialog()****************************************************/
void    SetgraffontsDialog()    /*FONT_DLOG*/
{
 sprintf(dlog_subjectstr,"On-Screen characters, font sizes.");
 Ldlog_subject = 1;
 
 sprintf(dlog_paramAstr,"%d",(int)infofontsize);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"info font size: pointID, distance measures");
 Ldlog_textA = 1;
 
 sprintf(dlog_paramBstr,"%d",(int)labelsfontsize);
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"labels font size");
 Ldlog_textB = 1;
 
 sprintf(dlog_paramCstr,"%d",(int)wordsfontsize);
 Ldlog_paramC = 1;
 sprintf(dlog_textCstr,"words text font size");
 Ldlog_textC = 1;

 sprintf(dlog_paramDstr,"%d",(int)tablefontsize);
 Ldlog_paramD = 1;
 sprintf(dlog_textDstr,"table cell font size");
 Ldlog_textD = 1;

   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = FONT_DLOG;
}
/*___SetgraffontsDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndSetgraffontsDialog()*************************************************/
void    EndSetgraffontsDialog()
{
 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_OKHIT) 
 {
      infofontsize   = intfromstr(dlog_paramAstr);
      labelsfontsize = intfromstr(dlog_paramBstr);
      wordsfontsize  = intfromstr(dlog_paramCstr);
      tablefontsize  = intfromstr(dlog_paramDstr);
      Linfofontsize   = 1; /*can't tell which one(s) changed*/
      Llabelsfontsize = 1; /*so flag all of them as protected*/
      Lwordsfontsize  = 1; /*by virtue of being deliberately set*/
      Ltablefontsize  = 1;

 }
 ACTIVE_DLOG = 0;
}
/*___EndSetgraffontsDialog()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoKipOutputDialog*****************************************************/
void    DoKipOutputDialog() /*NOT USED 001118*/
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();    /*MAGEDLOG.c*/
         KipOutputDialog();     /*MAGEDLOG.c*/
         DogenericDialog();      /*____DLOG.c*/
#ifndef UNIX_X11  /* called after DLOG_OKHIT_CB*/ 
         EndKipOutputDialog();  /*MAGEDLOG.c*/
#endif
     }
}
/*___DoKipOutputDialog_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****KipOutputDialog()********************************************************/
void    KipOutputDialog() /*NOT USED 001118*/  /*KIPOUTPUT_DLOG*/
{
 sprintf(dlog_subjectstr,"Write new file with this kinemage,"
 CRLF" along with all others in the current kinemage file."
 );
 Ldlog_subject = 1;
 
 dlog_checkAint = Lvisibleout;
 sprintf(dlog_checkAstr,"Write only the Visible Parts currently on screen.");
 Ldlog_checkA = 1;


 
 sprintf(dlog_infostr  
 ,"MAGE then will read from the newly created kinemage file and resume at"
 CRLF"view 1 of what had been the current kinemage.");
 Ldlog_info = 1;
 ACTIVE_DLOG = KIPOUTPUT_DLOG;
}
/*___KipOutputDialog()_______________________________________________________*/

/****EndKipOutputDialog()*****************************************************/
void    EndKipOutputDialog() /*NOT USED 001118*/
{
 if(Ldlog_OKHIT) 
 {
    Lvisibleout = dlog_checkAint;
    ireturnflag = 1; /*global, to deal with both modal & non-modal Dialogs*/
    /*Mac and PC specific routines call writeoutput(5)*/
    /*UNIX genericdlogrouter calls writeoutput(5)*/
 }
 else /*(Ldlog_cancelHIT),(Ldlog_optionHIT)*/
 {
      ireturnflag = 0;   /*no action */
 }
 ACTIVE_DLOG = 0;
}
/*___EndKipOutputDialog()_____________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoPostScriptDialog*****************************************************/
void    DoPostScriptDialog()
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();    /*MAGEDLOG.c*/
         PostScriptDialog();     /*MAGEDLOG.c*/
         DogenericDialog();      /*____DLOG.c*/
#ifndef UNIX_X11  /* called after DLOG_OKHIT_CB*/ 
         EndPostScriptDialog();  /*MAGEDLOG.c*/
#endif
     }
}
/*___DoPostScriptDialog_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****PostScriptDialog()******************************************************/
void    PostScriptDialog()    /*POSTSCR_DLOG*/
{
 sprintf(dlog_subjectstr,"Write file with Postscript graphics image,"
 CRLF" default use RGB palette and regular triangles (if any)."
 );
 Ldlog_subject = 1;
 
 dlog_checkAint = LRGBPS;
 /*sprintf(dlog_checkAstr,"RGB Mage palette instead of derived CMYK");*/
 sprintf(dlog_checkAstr,"RGB Mage palette (CMYK not implemented)");/*041001*/
 Ldlog_checkA = 1;

 dlog_radioAint = !LspecialPS;
 sprintf(dlog_radioAstr,"standard regular triangles");
 Ldlog_radioA = 1;
 dlog_radioBint =  LspecialPS;
 sprintf(dlog_radioBstr,"special effect, swoopy triangles");
 Ldlog_radioB = 1;
 
 sprintf(dlog_infostr  
 ,"Postscript recalculates image using floating point rotation."
 CRLF"swoopy triangles make perforated ribbon faces");
 Ldlog_info = 1;
 ACTIVE_DLOG = POSTSCR_DLOG;
}
/*___PostScriptDialog()_____________________________________________________*/

/****EndPostScriptDialog()***************************************************/
void    EndPostScriptDialog()
{
 if(Ldlog_OKHIT) 
 {
      /*LRGBPS = dlog_checkAint;*/
      LRGBPS = 1; /*041001*/
      dlog_checkAint = 1; /*041001*/
      LspecialPS = dlog_radioBint;
      /*Lpostscript = 1;*/  /*DO NOT set this drawvec controls until*/
      /*Lspecialloop = 1;*/ /*a valid PostScript output file is opened*/
      ireturnflag = 1;
      /*UNIX genericdlogrouter calls writeoutput(6)*/
 }
 else /*(Ldlog_cancelHIT),(Ldlog_optionHIT)*/
 {
      ireturnflag = 0;   /*no action */
 }
 ACTIVE_DLOG = 0;
}
/*___EndPostScriptDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoRaster3DDialog()*****************************************************/
void    DoRaster3DDialog()
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();    /*MAGEDLOG.c*/
         Raster3DDialog();     /*MAGEDLOG.c*/
         DogenericDialog();      /*____DLOG.c*/
#ifndef UNIX_X11  /* called after DLOG_OKHIT_CB*/ 
         EndRaster3DDialog();  /*MAGEDLOG.c*/
#endif
     }
}
/*___DoRaster3DDialog()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****Raster3DDialog()********************************************************/
void    Raster3DDialog()    /*RASTER3D_DLOG*/
{
 sprintf(dlog_subjectstr
 ,"Write file for rendering in "
 CRLF" (default) Raster3D (http://www.bmsc.washington.edu/raster3d)"
 CRLF" or POV-Ray (http://www.povray.org)");
 Ldlog_subject = 1;
 
 dlog_checkAint = 0;
 sprintf(dlog_checkAstr,"Output to file for POV-Ray");
 Ldlog_checkA = 1;

 dlog_checkBint = LR3Dappend;
 sprintf(dlog_checkBstr,"expect to append to existing render file");
 Ldlog_checkB = 1;

 dlog_checkCint = LR3Dsquaredcolor;
 sprintf(dlog_checkCstr,"Dull Down: use square of fractional color values");
 Ldlog_checkC = 1;

 dlog_radioAint = LR3Dstrokedtext;
 sprintf(dlog_radioAstr,"Use stroked font, for text in rendered image");
 Ldlog_radioA = 1;

 dlog_radioBint = !LR3Dstrokedtext;
 sprintf(dlog_radioBstr,"bit mapped INVISIBLE text embedded in Raster3D render file");
 Ldlog_radioB = 1;

 sprintf(dlog_infostr  
 ,"MAGE recalculates image using floating point rotation."
 CRLF"render file header can be edited for further options");
 Ldlog_info = 1;
 ACTIVE_DLOG = RASTER3D_DLOG;
}
/*___Raster3DDialog()_______________________________________________________*/

/****EndRaster3DDialog()*****************************************************/
void    EndRaster3DDialog()
{
 if(Ldlog_OKHIT) 
 {
      LR3Dappend = dlog_checkBint;
      LR3Dsquaredcolor = dlog_checkCint;
      LR3Dstrokedtext =  dlog_radioAint; /*optional for Raster3D*/
      /*LR3Dshadow = dlog_radioDint;*/
      Lspecialloop = 1; /*control within MAGEDRAW/drawvec()*/
      ireturnflag = 1;
      if(dlog_checkAint)
      {/*over-ride for output for POV_ray renderer*/
         Lraster3Drender = 2;
         LR3Dstrokedtext = 1; /*forced for POV-Ray*/
#ifndef UNIX_X11
         writeoutput(9); /*____OUT.C*/
#endif
      }
      else
      {
         Lraster3Drender = 1;
#ifndef UNIX_X11
         writeoutput(8); /*____OUT.C*/
#endif
      }
 }
 else /*(Ldlog_cancelHIT),(Ldlog_optionHIT)*/
 {
      Lspecialloop = 0;
      Lraster3Drender = 0;
 }
 ACTIVE_DLOG = 0;
}
/*___EndRaster3DDialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DocoordformatDialog****************************************************/
void    DocoordformatDialog()
{
  if(!ACTIVE_DLOG)
  {
     cleargenericflags();       /*MAGEDLOG.c*/
     coordformatDialog();       /*MAGEDLOG.c*/
     DogenericDialog();         /*____DLOG.c*/
#ifndef UNIX_X11 /* called after DLOG_OKHIT_CB*/ 
    EndcoordformatDialog();    /*MAGEDLOG.c*/
#endif
   }
}
/*___DocoordformatDialog____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****coordformatDialog()*****************************************************/
void    coordformatDialog()    /*ROTFORMAT_DLOG*/
{
   int Lrotformat = 0;

 sprintf(dlog_subjectstr,"Output rotated coordinates"
   CRLF"in kinemage format or PDB format,"
   CRLF" or just the angles of visible sliders.");
 Ldlog_subject = 1;

    if(Lkinformat) {Lrotformat = 0;}
    else if(Lrotangleout==0) {Lrotformat = 1;}

 dlog_radioAint = Lkinformat;
 sprintf(dlog_radioAstr,"kinemage format");
 Ldlog_radioA = 1;
#ifdef HYDROGENS  /*rearranged, simplified, 071215*/
 dlog_radioCint =  (LPDBformat && Lhydrogensout && LhydrogensSep);
 sprintf(dlog_radioCstr,"PDB format, hydrogens separate");
 Ldlog_radioC = 1;
 dlog_radioEint =  (LPDBformat && !Lhydrogensout && LrotoutSORTED);
 sprintf(dlog_radioEstr,"PDB format, no hydrogens, SORTED by atom number");
 Ldlog_radioE = 1;
#endif
 dlog_radioBint =  (LPDBformat && Lhydrogensout && !LhydrogensSep);
 sprintf(dlog_radioBstr,"PDB format coords all atoms");
 Ldlog_radioB = 1;
 dlog_radioCint =  (LPDBformat && !Lhydrogensout);
 sprintf(dlog_radioCstr,"PDB format, no hydrogens");
 Ldlog_radioC = 1;
 dlog_radioDint =  Lrotangleout;
 sprintf(dlog_radioDstr,"visible slider angles as hypertext");
 Ldlog_radioD = 1;

 dlog_checkAint = Lappendout; /*020303*/
 sprintf(dlog_checkAstr,"append to selected output file");
 Ldlog_checkA = 1;

 sprintf(dlog_infostr,"kinemage format gives "
       CRLF"all rotated parts as a .kin file"
       CRLF"PDB format just gives the atoms "
       CRLF"at vector heads (L type points)");
 Ldlog_info = 1;
 ACTIVE_DLOG = ROTFORMAT_DLOG;
}
/*___coordformatDialog()____________________________________________________*/

/****EndcoordformatDialog()**************************************************/
void    EndcoordformatDialog()
{
 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_optionHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_OKHIT) /*logic rearranged 070829*/
 {
      Lappendout = dlog_checkAint; /*071215*/

      if(dlog_radioAint) /* "kinemage format" */
      {
         Lkinformat = 1;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 1;
         LhydrogensSep = 0;
         Lvisibleout = 0;
         Lrotangleout = 0; 
         LrotoutSORTED = 0; /*070901*/
      }
#ifdef HYDROGENS /*071215*/
      else if(dlog_radioCint) /*PDB format, hydrogens separate*/
      {
         Lkinformat = 0;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 1;
         LhydrogensSep = 1;
         Lvisibleout = 1;
         Lrotangleout = 0; 
         LrotoutSORTED = 0; /*070901*/
      }
      else if(dlog_radioEint)/*PDB format, no hydrogens, sorted by atom number*/
      {
         Lkinformat = 0;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 0;
         LhydrogensSep = 0;
         Lvisibleout = 1;
         Lrotangleout = 0; 
         LrotoutSORTED = 1; /*070901, suitefit alternate bases 071005*/
         if(&rotoutscratch != NULL) {disposetextblock(&rotoutscratch);}
         else {inittextblock(&rotoutscratch);} /*MAGESCRT 070901*/
         /*070901 need another dialog for residue numbers ????*/
      }
#endif
      else if(dlog_radioBint) /*PDB format coords*/
      {
         Lkinformat = 0;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 1;
         LhydrogensSep = 0;
         Lvisibleout = 1;
         Lrotangleout = 0; 
         LrotoutSORTED = 0; /*070901*/
      }
      else if(dlog_radioCint) /*PDB format, no hydrogens*/
      {
         Lkinformat = 0;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 0;
         LhydrogensSep = 0;
         Lvisibleout = 1;
         Lrotangleout = 0; 
         LrotoutSORTED = 0; /*070901*/
      }
      else if(dlog_radioDint) /*visible slider angles as hypertext*/
      {
         Lkinformat = 0;
         LPDBformat = 0; /*NOT use here the general PDB instead of kin flag*/
         Lhydrogensout = 0;
         LhydrogensSep = 0;
         Lvisibleout = 0;
         Lrotangleout = 1; /*070705*/
         LrotoutSORTED = 0; /*070901*/
      }
      ireturnflag = 1;
      resetmenuchecks();
 }
 ACTIVE_DLOG = 0;
}
/*___EndcoordformatDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DodetailDialog*********************************************************/
void    DodetailDialog()
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();  /*MAGEDLOG.c*/
         detailDialog();       /*MAGEDLOG.c*/
         DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11  /*called after DLOG_OKHIT_CB */
         EnddetailDialog();    /*MAGEDLOG.c*/
#endif
     }
}
/*___DodetailDialog_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****detailDialog()**********************************************************/
void    detailDialog()   /*960808*/     /*DETAIL_DLOG*/
{
 sprintf(dlog_subjectstr,"level of detail: current full update rate %f"
             ,updaterate);
 Ldlog_subject = 1;
 sprintf(dlog_paramAstr,"%.2f",updateratelimit );
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"update rate limit ( updates per second)");
 Ldlog_textA = 1;
 sprintf(dlog_optionstr,"use default");
 Ldlog_option = 1;
 sprintf(dlog_infostr,"level of detail lowered when update rate lower than:"
                        CRLF"                           default  current"
                        CRLF" update rate limit:      %.2f     %.2f"
                         ,updateratelimitdefault, updateratelimit);
 Ldlog_info = 1;
 ACTIVE_DLOG = DETAIL_DLOG;
}
/*___detailDialog()_________________________________________________________*/

/****EnddetailDialog()*******************************************************/
void    EnddetailDialog() /*960808*/
{
 if(Ldlog_OKHIT) 
 {
      updateratelimit = floatfromstr(dlog_paramAstr);
 }
 else if(Ldlog_optionHIT)
 {
      updateratelimit = updateratelimitdefault;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
 ACTIVE_DLOG = 0;
}
/*___EnddetailDialog()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DousernameDialog********************************************************/
void    DousernameDialog() /*020816*/
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();  /*MAGEDLOG.c*/
         usernameDialog();       /*MAGEDLOG.c*/
         DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11  /*called after DLOG_OKHIT_CB */
         EndusernameDialog();    /*MAGEDLOG.c*/
#endif
     }
}
/*___DousernameDialog________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****usernameDialog()*********************************************************/
void    usernameDialog()   /*020816*/    /*USERNAME_DLOG*/   /*3Dlit 3DLit*/
{
   int nchar = 0;
#ifdef MACINTOSH
   nchar = 8;
#else
   nchar = 20;
#endif
   sprintf(dlog_subjectstr
        ,"Please type a short, unique name or personal identifier in the box:"
     CRLF"(only the first %d (or so) characters will be used in file name)."
        ,nchar );
   Ldlog_subject = 1;
   usernamestr[31] = '\0'; /*safety*/
   sprintf(dlog_paramAstr,"%s",usernamestr );
   Ldlog_paramA = 1;
/*sprintf(dlog_textAstr,"<---NAME-----(up to %d characters)",(MAXUSERCHAR-1));*/
   sprintf(dlog_textAstr,"<---NAME----- (up to 20 characters will be used)");
   Ldlog_textA = 1;
   sprintf(dlog_OKstr,
      "______________________________OK______________________________");
      /*extra length button box expands parent box for text length*/
   Ldlog_cancel = 0; /*cancel NOT an option for forced username*/
   if(magetimeticklimit > 0)
   {
      sprintf(dlog_infostr,
            "Output file name uses first part of user name plus a time stamp"
        CRLF"(Name must have at least one reasonable character or digit.)"
        CRLF"Clock reset and started when this dialog closes."
        CRLF"Test time is %.2f minutes."
            ,magetesttime);
   }
   else
   {
      sprintf(dlog_infostr,
            "Output file name uses first part of user name plus a time stamp"
        CRLF"(Name must have at least one reasonable character or digit.)"
             );
   }
   Ldlog_info = 1;
   ACTIVE_DLOG = USERNAME_DLOG;
}
/*___usernameDialog()________________________________________________________*/

/****EndusernameDialog()******************************************************/
void    EndusernameDialog() /*020816*/
{
 int LOK=0,j=0;
 
 if(Ldlog_OKHIT) 
 {
    for(j=0;j<MAXUSERCHAR;j++)
    {/*should <= array size of MAGEDLOG.h/dlog_paramAstr[]*/
        if(dlog_paramAstr[j]=='\0') break;
        else if( isalpha(dlog_paramAstr[j]) || isdigit(dlog_paramAstr[j]) ) 
        {
           LOK = 1;
           break;
        }
    }
    if(LOK)
    {
       strcpy(usernamestr,dlog_paramAstr);
       Lcancel = 0; /*this name acceptable*/
    }
    else
    {
       Lcancel = 1; /* NOT acceptable name, call DousernameDialog again */
    }
 }
 /*Ldlog_optionHIT and Ldlog_cancelHIT not used for forced name dialog*/
 
 ACTIVE_DLOG = 0;
}
/*___EndusernameDialog()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DofilenameDialog********************************************************/
void    DofilenameDialog() /*020829*/      /*3Dlit*/  /*3DLit*/
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      filenameDialog();       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11  /*called after DLOG_OKHIT_CB */
      EndfilenameDialog();    /*MAGEDLOG.c*/
#endif
   }
}
/*___DofilenameDialog________________________________________________________*/

/****filenameDialog()*********************************************************/
void    filenameDialog()     /*FILENAME_DLOG*/
{
  int j=0;

  /*Macintosh ResEdit dialog text fields need to be pre-filled with blanks*/
  if( Ltestscoreavailable && Ltestnext)
  {/*continue: should be test score available*/
    sprintf(dlog_subjectstr,
           "Score: %d correct out of %d,  %d wrong, for kinemage %d"
       CRLF""
       CRLF"The next kinemage will start when this dialog box closes"
       CRLF""
       CRLF"This should be the unique file name for your answers:"
       ,Scorepositiveold,Scoretotalold,Scorenegativeold,kinold);
  }
  else if(!Ltestscoreavailable && Ltestnext)
  {/*continue: test score not available*/
    sprintf(dlog_subjectstr,
           "kinemage %d not scored"
       CRLF""
       CRLF"The next kinemage will start when this dialog box closes"
       CRLF""
       CRLF"This should be the unique file name for your answers:"
       ,kinold);
  }
  else if( Ltestscoreavailable && !Ltestnext)
  {/*end: should be test score available*/
    sprintf(dlog_subjectstr,
           "Score: %d correct out of %d,  %d wrong, for kinemage %d"
       CRLF""
       CRLF"You are finished, Mage will quit when this dialog box closes"
       CRLF""
       CRLF"This should be the unique file name for your answers:"
       ,Scorepositiveold,Scoretotalold,Scorenegativeold,kinold);
  }
  else if(!Ltestscoreavailable && !Ltestnext)
  {/*end: test score not available*/
    sprintf(dlog_subjectstr,
           "kinemage %d not scored"
       CRLF""
       CRLF"You are finished, Mage will quit when this dialog box closes"
       CRLF""
       CRLF"This should be the unique file name for your answers:"
       ,kinold);
  }
  Ldlog_subject = 1;
  

  sprintf(dlog_textAstr,"%s",OutStr);
  Ldlog_textA = 1;

  sprintf(dlog_OKstr,
      "______________________________OK______________________________");
      /*extra length button box expands parent box for text length*/
  Ldlog_cancel = 0; /*cancel NOT an option for forced output*/

  if(Ltestnext)
  {
     j = getcaptiontop(dlog_infostr, 256); /* str*, max-char-in-str */
     if(j>255){j=255;} /*safety*/
     dlog_infostr[j] = '\0'; /*which may be redundant*/
  }
  else {sprintf(dlog_infostr,"END");}
  Ldlog_info = 1; /*in any case, makes an info box in the dialog box*/
  ACTIVE_DLOG = FILENAME_DLOG;
}
/*___filenameDialog()________________________________________________________*/

/****EndfilenameDialog()******************************************************/
void    EndfilenameDialog()
{
  if(Ldlog_OKHIT || Ldlog_optionHIT || Ldlog_cancelHIT)
  {/*all actions result in dismissal of this dialog , file is already written*/
    Lcancel = 0; /*this name acceptable by default*/
    Ltestreportpending = 0;
  }
  ACTIVE_DLOG = 0;
}
/*___EndfilenameDialog()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DofinestepDialog*********************************************************/
void    DofinestepDialog()
{
     if(!ACTIVE_DLOG) 
     {
         cleargenericflags();  /*MAGEDLOG.c*/
         finestepDialog();       /*MAGEDLOG.c*/
         DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11  /*called after DLOG_OKHIT_CB */
         EndfinestepDialog();    /*MAGEDLOG.c*/
#endif
     }
}
/*___DofinestepDialog_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****finestepDialog()**********************************************************/
void    finestepDialog()   /*011201*/     /*FINESTEP_DLOG*/
{
 sprintf(dlog_subjectstr,"current finest step %f, (default== %f)"
   "CRTLcurrent finest degree step %f, (default== %f)" /*070617*/
    ,finestep,finestepdefault,DEGSTEP,degreestepdefault); /*070617*/
 Ldlog_subject = 1;
 sprintf(dlog_paramAstr,"%.3f",finestep );
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"translation fine step amount (coarse is 10x finestep)");
 Ldlog_textA = 1;
 sprintf(dlog_paramBstr,"%.3f",DEGSTEP ); /*070617*/
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"angle-slider fine degree step amount (<slider> ends)"); /*070617*/
 Ldlog_textB = 1;
 sprintf(dlog_optionstr,"use default");
 Ldlog_option = 1;
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = FINESTEP_DLOG;
}
/*___finestepDialog()_________________________________________________________*/

/****EndfinestepDialog()*******************************************************/
void    EndfinestepDialog() /*960808*/
{
 if(Ldlog_OKHIT) 
 {
      finestep = floatfromstr(dlog_paramAstr);
      DEGSTEP = floatfromstr(dlog_paramBstr); /*070617*/
 }
 else if(Ldlog_optionHIT)
 {
      finestep = finestepdefault;
      DEGSTEP = degreestepdefault; /*070617*/
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
 ACTIVE_DLOG = 0;
}
/*___EndfinestepDialog()______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoimportDialog()********************************************************/
void    DoimportDialog()
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      importDialog();       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndimportDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
   }
}
/*___DoimportDialog()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****importDialog()**********************************************************/
void    importDialog()      /*IMPORT_DLOG*/
{
   if(Lpipeactive)
   {/*active pipe takes precedence*/
      sprintf(dlog_subjectstr,"a data import pipe is active...");
      Ldlog_subject = 1;

      dlog_radioAint = 1; /*Prekin*/
      dlog_radioBint = 0; /*generic*/
   }
   else
   {/*no active pipe, try for ,e.g., Prekin*/
      sprintf(dlog_subjectstr,"Invoke another program to update kinemage"
         CRLF"A possible commandline is provided for Prekin... ");
      Ldlog_subject = 1;

      dlog_radioAint = 1; /*Prekin*/
      dlog_radioBint = 0; /*generic*/
   }

   sprintf(dlog_radioAstr
           ,"Import from Prekin through active pipe.");
   Ldlog_radioA = 1;

   sprintf(dlog_radioBstr
           ,"Import from generic other program through active pipe.");
   Ldlog_radioB = 1;

   sprintf(dlog_OKstr,"OK");
   sprintf(dlog_cancelstr,"CANCEL dialog");
   sprintf(dlog_optionstr,"QUIT IMPORTING"); 
   Ldlog_option = 1;

   sprintf(dlog_infostr,"MAGE sends a command to a data-providing program,"
     CRLF"then can receive kinemage formated data back via an active pipe."
     CRLF"The other program collects its own data, then sends stuff to mage.");
   Ldlog_info = 1;
   ACTIVE_DLOG = IMPORT_DLOG;
}
/*___importDialog()_________________________________________________________*/

/****EndimportDialog()*******************************************************/
void    EndimportDialog()
{
 ACTIVE_DLOG = 0; /*so another dialog can be called*/

/*011212 rearrange so that all other business done before call another dialog*/
/*so return from next dialog can drop cleanly through the last of this routine*/

 if(Ldlog_OKHIT) 
 {
    if(Lkinpdbfile)
    {/*kinemage contained original data file name*/
        sprintf(update_file_name,"%s",kinpdbfilestr);
    }/*kinemage contained original data file name*/
    else
    {/*try to get data file name from group name*/
        if(firstgrupptr != NULL)
        {
           sprintf(update_file_name,"%s"
              ,firstgrupptr->name); 
        }
        else
        {
           update_file_name[0] = '\0'; /*nothing sent as a file name*/
        }
    }/*try to get data file name from group name*/

    if     (dlog_radioAint)
    { /* import from Prekin */
       sprintf(import_command_line,"prekin %s -mage ",update_file_name);
          /* Prekin -mage implies prekin writes to stdout */
    }

    else if(dlog_radioBint)
    { /* import from generic program */
       sprintf(import_command_line,"foobar %s -mage -flag ",update_file_name);
    }
    /*Now need to prepare Mage for piecemeal imported kinemage*/
    /*eventually set up supersets with one for the thumbnail*/
    /*for now, just blop on the frame that has no scaleable points*/
    /*so scaling of the first kinemage stuff will not be prohibited.*/
    createinternalkinemage(3); /*3 is the plain screen frame*/
    /*DoupdatePipeDialog(Lpipeactive);*/
      /*0: initiate, 1: could close*/
    NEXT_DLOG = UPDATEPIPE_DLOG;
    NEXT_DLOG_PARAM = Lpipeactive;
 }
 else if(Ldlog_optionHIT)
 {
    /*stop importing process*/
    Lupdate = 0; /*turn off updating*/ /*Is this flag set for active pipes???*/
    if(Lpipeactive)
    {/*active pipe, should be killed properly*/
       /*DoupdatePipeDialog(Lpipeactive);*/ /*0: initiate, 1: could close*/
       NEXT_DLOG = UPDATEPIPE_DLOG;
       NEXT_DLOG_PARAM = Lpipeactive;
    }
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
}
/*___EndimportDialog()______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdateDialog()********************************************************/
void    DoupdateDialog()
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      updateDialog();       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdateDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
   }
}
/*___DoupdateDialog()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updateDialog()**********************************************************/
void    updateDialog()       /*UPDATE_DLOG*/
{

 Lgetmapfilename = 0;

   if(!Lpipeactive && nbondrot > 0)
   {/*has rotations for Probe to calculate dots*/
       sprintf(dlog_subjectstr,"Invoke another program to update kinemage");
       Ldlog_subject = 1;

      if(input_command_line[0] == '\0') 
      {
         dlog_radioAint = 0; 
         dlog_radioBint = 1; /*new probe*/
         dlog_radioCint = 0;
         dlog_radioDint = 0;
         dlog_radioEint = 0;
         dlog_radioFint = 0;
         dlog_radioGint = 0;
         dlog_radioHint = 0;
         dlog_radioIint = 0;
      }
      else
      {
         dlog_radioAint = 1; /*presume stored probe*/
         dlog_radioBint = 0;
         dlog_radioCint = 0;
         dlog_radioDint = 0;
         dlog_radioEint = 0;
         dlog_radioFint = 0;
         dlog_radioGint = 0;
         dlog_radioHint = 0;
         dlog_radioIint = 0;
      }

      sprintf(dlog_radioAstr,"Probe command from kinemage (if available)");
      Ldlog_radioA = 1;
 
      sprintf(dlog_radioBstr
              ,"Probe to calc dots around present rotation group");
      Ldlog_radioB = 1;
   }/*has rotations for Probe to calculate dots*/
#ifdef UNIX_X11
   else if(Lpipeactive)
   {/*active pipe takes precedence*/
      sprintf(dlog_subjectstr,"a pipe is active...");
      Ldlog_subject = 1;

      dlog_radioAint = 0; 
      dlog_radioBint = 0;
      dlog_radioCint = 0;
      dlog_radioDint = 0;
      dlog_radioEint = 1; /*update from live pipe*/
      dlog_radioFint = 0;
      dlog_radioGint = 0;
      dlog_radioHint = 0;
      dlog_radioIint = 0;
   }
#endif /*UNIX_X11*/
   else
   {/*can't do Probe, no active pipe, try for Prekin*/
      sprintf(dlog_subjectstr,"Invoke another program to update kinemage"
      CRLF" NO rotations, internal data not available to invoke Probe.");
      Ldlog_subject = 1;

      dlog_radioAint = 0; 
      dlog_radioBint = 0;
      dlog_radioCint = 0; /*prekin rotation*/
      dlog_radioDint = 1; /*prekin mutation*/
      dlog_radioEint = 0;
      dlog_radioFint = 0;
      dlog_radioGint = 0;
      dlog_radioHint = 0;
      dlog_radioIint = 0;
   }

   sprintf(dlog_radioCstr,"Prekin to set up rotations for sidechain as is");
   Ldlog_radioC = 1;
 
   sprintf(dlog_radioDstr
           ,"Prekin to set up rotations of idealized or mutated sidechain");
   Ldlog_radioD = 1;

#ifdef UNIX_X11
   sprintf(dlog_radioEstr
           ,"update through active pipe from virtual file");
   Ldlog_radioE = 1;

   if(Lsockets == 0)
   {/*no socket active*/
      sprintf(dlog_radioFstr
           ,"initiate a socket server to receive from any remote mage");
      Ldlog_radioF = 1;
   }
   else if(Lsockets == 1 || Lsockets == 2)
   {
      sprintf(dlog_radioFstr
           ,"initiate a socket client to send to a named remote mage");
      Ldlog_radioF = 1;
      sprintf(dlog_radioGstr
           ,"terminate whatever socket stuff is going");
      Ldlog_radioG = 1;
   }
   else if(Lsockets == 3)
   {
      sprintf(dlog_radioFstr
           ,"terminate sockets between mages");
      Ldlog_radioF = 1;
   }

   sprintf(dlog_checkAstr,"1st local mage");
   Ldlog_checkA = 1;
   if(Lsocketsame == 1) dlog_checkAint = 1;
   else dlog_checkAint = 0;

   sprintf(dlog_checkBstr,"2nd local mage");
   Ldlog_checkB = 1;
   if(Lsocketsame == 2) dlog_checkBint = 1;
   else dlog_checkBint = 0;
   
   sprintf(dlog_checkCstr,"3rd local mage");
   Ldlog_checkC = 1;
   if(Lsocketsame == 3) dlog_checkCint = 1;
   else dlog_checkCint = 0;

   sprintf(dlog_checkDstr,"pointID search");
   Ldlog_checkD = 1;
   if(Lsocketidsearch == 1) dlog_checkDint = 1;
   else dlog_checkDint = 0;

   sprintf(dlog_radioHstr,"kin3Dcont to update contours");
   Ldlog_radioH = 1;

   sprintf(dlog_radioIstr,"noe-display to update NOEs");
   Ldlog_radioI = 1;

#endif /*UNIX_X11*/

   sprintf(dlog_OKstr,"OK");
   sprintf(dlog_cancelstr,"CANCEL dialog");
   sprintf(dlog_optionstr,"QUIT UPDATING"); /*001029*/
   Ldlog_option = 1;

   if(dlog_radioCint==1 || dlog_radioDint==1)
   {
      sprintf(dlog_infostr,"Prekin Rotation does not idealize conformation."
         CRLF"Mutation, even to same res, gives ideal geometry,"
         CRLF" as well as all rotamers of that sidechain as Mage hypertext.");
   }
   else
   {
     sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   }
   Ldlog_info = 2; /*filler==2 060619*/
   ACTIVE_DLOG = UPDATE_DLOG;
}
/*___updateDialog()_________________________________________________________*/

/****EndupdateDialog()*******************************************************/
void    EndupdateDialog()
{
 ACTIVE_DLOG = 0; /*so another dialog can be called*/

/*011212 rearrange so that all other business done before call another dialog*/
/*so return from next dialog can drop cleanly through the last of this routine*/

 if(Ldlog_OKHIT) 
 {
#ifdef UNIX_X11
   if(dlog_radioFint)
   {/*dealing with sockets, deal with check boxes before doing radios*/
      if(dlog_checkAint) { Lsocketsame = 1; }
      if(dlog_checkBint) { Lsocketsame = 2; }
      if(dlog_checkCint) { Lsocketsame = 3; }
      if(dlog_checkDint) { Lsocketidsearch = 1; }
   }
#endif
   if     (dlog_radioAint)
   { /* 0: inputed probe setup*/
      /*DoupdateProbeDialog(0);*/
      NEXT_DLOG = UPDATEPROBE_DLOG;
      NEXT_DLOG_PARAM = 0;
   }

   else if(dlog_radioBint)
   { /* 1: new probe setup*/
      /*DoupdateProbeDialog(1);*/
      NEXT_DLOG = UPDATEPROBE_DLOG;
      NEXT_DLOG_PARAM = 1;
   }

   else if(dlog_radioCint)
   { /* 1: new prekin setup, just rotation*/
      /*DoupdatePrekinDialog(1);*/
      NEXT_DLOG = UPDATEPREKIN_DLOG;
      NEXT_DLOG_PARAM = 1;
   }

   else if(dlog_radioDint)
   { /* 2: new prekin setup, mutation*/
      /*DoupdatePrekinDialog(2);*/
      NEXT_DLOG = UPDATEPREKIN_DLOG;
      NEXT_DLOG_PARAM = 2;
   }

#ifdef UNIX_X11
   else if(dlog_radioEint)
   { /*0: initiate, 1: could close*/
      /*DoupdatePipeDialog(Lpipeactive);*/
      NEXT_DLOG = UPDATEPIPE_DLOG;
      NEXT_DLOG_PARAM = Lpipeactive;
   }

   else if(dlog_radioFint)
   {/*if--else avoids dangling code...*/
         if(Lsockets==0)
         {/*first set up to be a server for any incoming mage stuff*/
           setsockets(Lsockets); /*0, so initiate server function*/
         }
         else if(Lsockets==1 || Lsockets==2)
         {/*second: name a remote computer to send mage stuff to*/
           /*DoupdateSocketDialog(Lsockets);*/ /*1: initiate client function*/
           NEXT_DLOG = UPDATESOCKET_DLOG;
           NEXT_DLOG_PARAM = Lsockets;
         }
         else
         {/*terminate socket stuff*/
           setsockets(Lsockets); /*2: terminate sockets*/
         }
   }
   else if(dlog_radioGint)
   {
      {/*terminate socket stuff*/
        setsockets(2); /*2: terminate sockets*/
      }
   }      
   else if(dlog_radioHint)
   {/*kin3Dcont*/
      NEXT_DLOG = UPDATECONTOURS_DLOG;
      NEXT_DLOG_PARAM = 0;
   }
   else if(dlog_radioIint)
   {/*noe distances*/
      NEXT_DLOG = UPDATENOES_DLOG;
      NEXT_DLOG_PARAM = 0;
   }
#endif /*UNIX_X11*/
 
 }
 else if(Ldlog_optionHIT)
 {
    /*stop doing one-at-a-time updates*/
    Lupdate = 0; /*turn off updating*/
    if(Lpipeactive)
    {/*active pipe, should be killed properly*/
       /*DoupdatePipeDialog(Lpipeactive);*/ /*0: initiate, 1: could close*/
       NEXT_DLOG = UPDATEPIPE_DLOG;
       NEXT_DLOG_PARAM = Lpipeactive;
    }
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
}
/*___EndupdateDialog()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****tokenize()***************************************************************/
int  tokenize(char thestr[256], int *argc, char** argv) /*040210*/
{
   int i=0,j=0,k=0,n=0;
   char toks[256];

   *argc = 0;
   for(j=0; j<256; j++)
   {
      if(thestr[j]==' '|| thestr[j]=='\t'|| thestr[j]=='\n'|| thestr[j]=='\0')
      {/*whitespace*/
         if(k==0)
         {/*not yet a valid character in this token, presume a leading blank*/
            ; /*ignore whitespace before possible valid character*/
         }
         else
         {/*white space ends this nth token of k characters*/
            toks[k] = '\0'; /*end this str*/
            argv[n] = (char *)malloc(sizeof(char)*(k+1));
            for(i=0; i<=k; i++){argv[n][i] = toks[i];}
            k = 0; /*reset character counter*/
            n++; /*augment token index*/
         }
         if(thestr[j]=='\0')
         {/*at end of string to be tokenized*/
            break;
         }
      }
      else
      {/*valid character for this nth token*/
         toks[k] = thestr[j];
         k++;
      }
   }
   *argc = n;
   return(n);
}
/*___tokenize()______________________________________________________________*/

/****getrotationfocus()*******************************************************/
int getrotationfocus(float* fdistptr,float* fxmidptr,float* fymidptr,float* fzmidptr)
{
   float fxmin=0,fxmax=0,fymin=0,fymax=0,fzmin=0,fzmax=0;
   float fxmid=0,fymid=0,fzmid=0,fdist=0;
   int  i=0,j=0,k=0,numrestemp=0;
   char* token[128]; /*so every other of a 256 line could be a token*/
   int  Ntoken=0;
   int  LOK = 0; /*040609 use explicit local rather than global*/
   char numberstr[256]; /*hold putative nmr model number*/

   update_res_probe = 0; /*track residue number*/
   update_res_num_index = 0; /*count characters of string of res nums*/
   fxmin =  9999; /*scope of mobile coordinates limits probe's search*/
   fymin =  9999;
   fzmin =  9999;
   fxmax = -9999;
   fymax = -9999;
   fzmax = -9999;
   thisgrupptr = firstgrupptr;
   while( thisgrupptr != NULL)
   {/*check all groups*/
      if(thisgrupptr->bondrot != 0) /*has a subgroup with bondrots*/
      {/*has a subgroup with bondrots*/
      /*thelastbondrotgrupptr=thisgrupptr;*/ /*remember last one for its name*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*check all subgroups*/
           if(thissgrpptr->bondrot != 0)
           {/*may be a subgroup describing the rotations of a residue*/
             /*abstract residue number and nmr model number from subgroup name*/
             /*first find res num in this subgroup name*/
             i = 0;
             for(j=0; j<MAXNAMECHAR; j++)
             {
                 if( isdigit( thissgrpptr->name[j] ) )
                 {
                    word[i] = thissgrpptr->name[j];
                    i++;
                 } 
                 else if( thissgrpptr->name[j] == '\0')
                 {
                    word[i] = '\0';
                    break;
                 }
             }
             numrestemp   = intfromstr(word);
             if(numrestemp != update_res_probe)
             {/*new number, add to set of names of rotating sidechains*/
                 update_res_probe   = numrestemp;
                 k=update_res_num_index; 
                 if(k > 0)
                 {
                    update_res_num_str[k] = ','; 
                      /*separator in str of res names*/
                    k++;
                 }
             /*int  tokenize(char thestr[256], int *argc, char** argv) */ 
                 if(tokenize(thissgrpptr->name, &Ntoken, token)) /*040210*/
                 {/*subgroup name tokenized*/
                    for(j=0; j<MAXNAMECHAR; j++)
                    {/*get that residue number and add to string of res nums*/
                       /*as of prekin.6.31.030925, rotatorname incl chainID*/
                       /*distinguishs rotating sidechains from several chains*/
                       /*e.g. |trp 4 22| yielding 422 as update_res_num_index*/
                       /*since only and all digits gleaned from sgrp name*/
                       if(isdigit( token[Ntoken-1][j] ) )
                       {
                          update_res_num_str[k] = token[Ntoken-1][j];
                          k++;
                       }
                       else if( token[Ntoken-1][j] == '\0')
                       {
                          update_res_num_str[k] = '\0';
                          update_res_num_index = k;
                          break;
                       }
                    }

#ifdef OLDCODE
                    if( isdigit( thissgrpptr->name[j] ) )
                    {
                       update_res_num_str[k] = thissgrpptr->name[j];
                       k++;
                    } 
                    else if( thissgrpptr->name[j] == '\0')
                    {
                       update_res_num_str[k] = '\0';
                       update_res_num_index = k;
                       break;
                    }
#endif
                 }/*subgroup name tokenized*/
             }/*new number, add to set of rotating sidechains*/
             LOK = 1; /*OK did find a rotation subgroup*/
             /*now find the scope of coordinates of mobile points*/
             thislistptr = thissgrpptr->firstlistptr;
             while(thislistptr != NULL)
             {/*scan over lists*/
               if(thislistptr->bondrot != 0)
               {
                 thispointptr = thislistptr->firstpointptr;
                 while(thispointptr != NULL)
                 {/*scan over points*/
                   if(   ((thispointptr->STATUS & NOXYZ) != NOXYZ)
                        &&((thispointptr->STATUS & DUMMY) != DUMMY) )
                   {/*presumeably point has a valid coordinate*/
                     if     (thispointptr->fx < fxmin) fxmin = thispointptr->fx;
                     else if(thispointptr->fx > fxmax) fxmax = thispointptr->fx;
                     if     (thispointptr->fy < fymin) fymin = thispointptr->fy;
                     else if(thispointptr->fy > fymax) fymax = thispointptr->fy;
                     if     (thispointptr->fz < fzmin) fzmin = thispointptr->fz;
                     else if(thispointptr->fz > fzmax) fzmax = thispointptr->fz;
                   }
                    
                   if(thispointptr == thislistptr->lastpointptr) 
                        {thispointptr = NULL;}
                   else {thispointptr = thispointptr->nextptr;}
                 }/*scan over points*/
               }
               if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
               else thislistptr = thislistptr->nextptr;
             }/*scan over lists*/

             /*then try to identify an nmr model number in this subgroup name*/
             if(update_res_model == -1) /*model number not known*/
             {
                j = strlen(numstr); /*so last index is j-1 */
                for(k=(j-2); k>0; k--) /*at least one char past M for number*/
                {/*work backwards through name to find a model number*/
                   numberstr[0] = '\0'; /*end the str for now*/
                   if(thissgrpptr->name[k] == 'M')
                   {/*model number might be in following position(s) */
                      if(isdigit(thissgrpptr->name[k+1]))
                      {/*at least a starting digit*/
                         numberstr[0] = thissgrpptr->name[k+1];
                         numberstr[1] = '\0'; /*end the str for now*/
                      }
                      if( ((k+2)<j) && isdigit(thissgrpptr->name[k+2]) )
                      {/*second digit could be part of a model number*/
                         numberstr[1] = thissgrpptr->name[k+2];
                         numberstr[2] = '\0'; /*end the str for now*/
                         if( ((k+3)<j) && isdigit(thissgrpptr->name[k+3]) )
                         {/*third digit could be part of a model number*/
                            numberstr[2] = thissgrpptr->name[k+3]; 
                            numberstr[3] = '\0'; /*end the str for now*/
                            /*quit, model numbers > 999 not reasonable*/
                         }
                      }
                      break; /*out of for loop*/
                   }/*model number might be in following position(s) */
                }/*work backwards through name to find a model number*/
                if(numberstr[0] != '\0')
                {
                   update_res_model = intfromstr(numberstr);
                }
                else
                {
                   update_res_model = -1; /*flag not to send model number*/
                }
             }/*maybe room in pointID for model number*/

           }/*may be a subgroup describing the rotations of a residue*/

          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*check all subgroups*/
      }/*has a subgroup with bondrots*/
      thisgrupptr = thisgrupptr->nextptr;
   }/*check all groups*/
   if(LOK)
   {
      /*scope of coordinates defines a box*/
      fxmid = (fxmax + fxmin)/2;
      fymid = (fymax + fymin)/2;
      fzmid = (fzmax + fzmin)/2;

      fdist =   (fxmax-fxmin)*(fxmax-fxmin)
               +(fymax-fymin)*(fymax-fymin)
               +(fzmax-fzmin)*(fzmax-fzmin); /*sq of diagonal of box*/
      if(fdist > 0) fdist = (float)sqrt((double)(fdist/2)); /* half diagonal*/
      else fdist = 0;
      fdist = fdist + 10; /*arbitrary additional radius of probe interest*/
      *fxmidptr = fxmid;
      *fymidptr = fymid;
      *fzmidptr = fzmid;
      *fdistptr = fdist;
   }
   return(LOK);
}
/*___getrotationfocus()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdateProbeDialog()***************************************************/
void    DoupdateProbeDialog(int Lnew)
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      updateProbeDialog(Lnew);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdateProbeDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
   }
}
/*___DoupdateProbeDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updateProbeDialog()*****************************************************/
void    updateProbeDialog(int Lnew)     /*UPDATEPROBE_DLOG*/
{
 char local_command_line[256];
 float fxmid=0,fymid=0,fzmid=0,fdist=0;
 int LOK=0; /*040609 use explicit local rather than global*/

 sprintf(dlog_subjectstr,"Invoke another program (Probe) to update kinemage"
                         "(can edit command line)"
    CRLF"IF EDIT FAILS: move mouse cursor out of then back into dialog box. "
       );
 Ldlog_subject = 1;

 dlog_checkAint = Ldeleteprobedots = 1; 
 sprintf(dlog_checkAstr,"delete last probe dots");
 Ldlog_checkA = 1;
 
 dlog_checkBint = Lreplacecaptiontext; /*030311*/; 
 sprintf(dlog_checkBstr,"double run puts score in caption window");
 Ldlog_checkB = 1;
 
 /*presumption that doing an update of dots around a rotation group sc */
 if(Lnew ==1)
 {/*Lnew ==1: see if can identify all sc that have rotatable bonds*/
   LOK = getrotationfocus(&fdist,&fxmid,&fymid,&fzmid);

    if(LOK)
    {/*rotation found*/
       if(Lkinpdbfile)
       {/*kinemage contained original pdbfile name*/
          sprintf(update_file_name,"%s",kinpdbfilestr);
       }
       else
       {/*try to get pdb file name from group name*/
          sprintf(update_file_name,"%s"
             ,firstgrupptr->name); /*better bet than later, appended sc group*/
       }
       /*041112 -nomodeltest for probe.2.11.041112 allow update into nmr model*/
       /*050121 supersedes nomodeltest of 041112 now removed from probe!*/
       
      if(update_res_model < 0)
      {/*not an nmr model*/
 sprintf(local_command_line,"probe -quiet -drop -both -stdbonds "
"\"(file1 within %.2f of %.2f, %.2f, %.2f not water not(%s sc)),file2\""
" \"file2\" %s - "
        ,fdist,fxmid,fymid,fzmid,update_res_num_str
        ,update_file_name);
      }
      else
      {/*has a possible nmr model specified*/
sprintf(local_command_line,"probe -quiet -drop -both -stdbonds "
"\"(file1 within %.2f of %.2f, %.2f, %.2f not water not(%s sc) model%d),file2\""
" \"file2\" %s - "
        ,fdist,fxmid,fymid,fzmid,update_res_num_str
        ,update_res_model
        ,update_file_name);
      }
       /*inside first pattern might be: "within %.2f of %.2f,%.2f,%.2f"*/
       /*where distance ~8 and x,y,z from ctr of rot or last picked pt*/
       /*now use center of box holding all mobile points*/
    }/*rotation found*/
    else
    {
        /*local_command_line[0] = '\0';*/
        sprintf(local_command_line,"probe -quiet -self all -"); /*050123*/
        /*generic probe command, e.g. for docking scope group*/
    }
    Lnew = 0;
 }/*Lnew ==1: see if can identify all sc that have rotatable bonds*/
 else if(Lnew==0  && input_command_line[0] != '\0')
 {/*Show inputed command line*/
    sprintf(local_command_line,"%s",input_command_line);
 }
 else 
 {/*Show same commandline as used before*/
   Lnew = 0; /*don't mess with update_command_line str*/
   sprintf(local_command_line,"%s",update_probe_command_line);
 }

 sprintf(dlog_optionstr,"show previously used command string");
 Ldlog_option = 1;

 sprintf(dlog_infostr,"%s",local_command_line);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATEPROBE_DLOG;
}
/*___updateProbeDialog()____________________________________________________*/

/****EndupdateProbeDialog()**************************************************/
void    EndupdateProbeDialog()
{
 ACTIVE_DLOG = 0;
 Lreplacecaptiontexton = 0;

 if(Ldlog_OKHIT) 
 {
      sprintf(update_probe_command_line,"%s",dlog_infostr);
      Lupdate = 1; /*so updating can occur upon demand*/
      mycheckitem(kludgeMenu,remoteupdateItem,Lupdate); /*___MENU.C*/
      Linhibitbutton = 1; /*probe called each angle update*/
      Linhibiton = 0; /*this can be inhibited by right panel button*/
      /*Lappend = 1;*/ /*taken care of by doupdate()*/

      Ldeleteprobedots = dlog_checkAint; 
      Lreplacecaptiontext = dlog_checkBint; 
      
      Lignorecaption = 1; 
      if(nbondrot>0)
      { 
         Lprobeactive = 1;/*tracks probe as last called update, flag is needed*/
         Nthisgroupkind = PROBEDOTSKKIND;
         doupdate(1, update_probe_command_line); /*____OUT.c*/
         Nthisgroupkind = 0;
         Lnewstuffactive = 1; /*protection: asks for output before closing*/
         /*Ldeleteprobedots=1;*/ 
            /*maybe didn't delete appended stuff from earlier*/
            /*but after this, delete probe dots before getting updated dots*/
      }
      /* 1 for put out rotation group coords to remote program*/
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
 else if(Ldlog_optionHIT)
 {
      /*DoupdateProbeDialog(-1);*/ /*redo the dialog,-1: show same commandline*/
      /* 0 flags inputed commandline*/
      NEXT_DLOG = UPDATEPROBE_DLOG;
      NEXT_DLOG_PARAM = -1;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
}
/*___EndupdateProbeDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdatePrekinDialog()***************************************************/
void    DoupdatePrekinDialog(int Lnew)
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      updatePrekinDialog(Lnew);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdatePrekinDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
   }
}
/*___DoupdatePrekinDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updatePrekinDialog()*****************************************************/
void    updatePrekinDialog(int Laction)     /*UPDATEPREKIN_DLOG*/
{
 int   j,k,kk,Lnum;
 char local_command_line[256],res_name[4],res_chain,atom_alt,type_str[8]; 
 static char *aaList = ":gly:ala:val:phe:pro:met:ile:leu:asp:glu:lys:arg:\
ser:thr:tyr:his:cys:asn:gln:trp:mse:"; /*mse 041011*/
 char numberstr[256]; /*050121 used to get a model number, if any*/

 
 if(Laction != 2)
 {/*just setup for general case, e.g. with a rotation*/
    sprintf(dlog_subjectstr,"Invoke another program (Prekin) to update kinemage"
                         "(can edit command line)"
    CRLF
    CRLF"IF EDIT FAILS: move mouse cursor out of then back into dialog box. "
    CRLF"IF EDIT FAILS TRY BUTTON: \"show stored command string\" "
    );
 }/*just setup for general case, e.g. with a rotation*/
 else
 {/*setup for doing a mutation*/
    sprintf(dlog_subjectstr
       ,"Invoke Prekin to mutate a residue and give it rotatable bonds"
    CRLF"Edit the command line: replace res_name with desired mutation."
    CRLF
    CRLF"IF EDIT FAILS: move mouse cursor out of then back into dialog box. "
    CRLF"IF EDIT FAILS TRY BUTTON: \"show stored command string\" "
    );
 }/*setup for doing a mutation*/
 Ldlog_subject = 1;

 if(Lprobeactive)
 {/*last appended stuff most likely are obselete dots from a probe run*/
     dlog_checkAint = Ldeleteprobedots = 1; 
     sprintf(dlog_checkAstr,"delete last probe dots");
     Ldlog_checkA = 1;
 }
 else
 {   
     dlog_checkAint = Ldeleteprobedots; 
     sprintf(dlog_checkAstr,"delete last probe dots");
     Ldlog_checkA = 1;
 }
 dlog_checkBint = Ldeleteprekinappend; 
 sprintf(dlog_checkBstr,"delete last appended prekin input.");
 Ldlog_checkB = 1;
 
 /*presumption: make a rotation group sc */

 if(Laction >= 1)
 {/*see if can identify a putative sc from the last picked point*/
    update_res_model = -1; /*flag not to send model number*/
    if(pickedpointptr != NULL)
    {/*find res num, name, etc...  in pointID*/
       /*copy char str from storage */
       getptIDstring(numstr, pickedpointptr);
       /*PREKIN makes only 2 types of pointID: */
       /*{ ca Aarg Cnnnn } for rotation std pdb fragment */
       /* 012345678901234 */
       /*{ ca Aarg C_n..._ ....} for expanded regular prekin point ID*/
       /* where _ is forced blank and ... is continuation of number*/
       /* and C is the one-character ChainID, or is blank*/
       /* and A is the one-character Alt COnformation identifier, if any.*/
       /*Start at index 10 which is blank or the first of 4 digit res number*/
       /* go on to either 13 or 14, or hit blank or non-digit character to */
       /* get the full residue number.*/
       k=0;
       Lnum = 0;
       for(j=10; j<15; j++)
       {/*attempt to get residue number out of pointID*/
          /*look only in prekin field of digits in this pointID*/
          if( Lnum == 0 && isdigit( numstr[j] ) )
          {/*field starts with a number, maybe it is a residue number*/
             k = 0; /*reset here for emphasis*/
             Lnum = 1;
             word[k] = numstr[j];
             k++;
          } 
          else if( Lnum == 1 && isdigit( numstr[j] ))
          {/*continuing a numeric field*/
             word[k] = numstr[j];
             k++;
          }
          else if(   Lnum && (!isdigit(numstr[j])) ) 
          {/*anyother char ends a numeric field*/
             word[k] = '\0';
             break; /*successfully found a number*/
          }
          else  /*Lnum NOT set*/
          {/*yet another character not contributing to a number */
             ; /*NOP*/
          }
       }/*attempt to get residue number out of pointID*/
       if(Lnum)
       {/*dropped through when still accummulating a number*/
           k++;
           word[k] = '\0'; /*use this as a residue number*/
       }
       else
       {/*dropped through and never found a number*/
           word[0] = '\0'; /*null string, should yield residue = 0 */
       }
       /*010902*/
       /*Try to identify a residue name in the point ID to use as default*/
       /*copy the characters that are most likely to be the residue name*/
       for(j=0; j<3; j++)
       {
          res_name[j] = numstr[j+5]; /*PREKIN puts res name in j==5,6,7 */
       }
       res_name[3] = '\0';
       if(!strstr(aaList, res_name) )
       {
          res_name[0] = 'a';res_name[1] = 'l';res_name[2] = 'a';
       }
       /*010908*/
       /*Try to identify a chain ID in the point ID to use as a default*/
       /*There is only one place that is sure enough to try*/
       res_chain = numstr[9];
       /*020608*/
       /*Try to identify an alt conf in the point ID to use as a default*/
       /*There is only one place that is sure enough to try*/
       atom_alt = numstr[4]; /*character preceeding res_name triplet*/
       
       /*050121*/
       /*Try to identify a model number in the point ID to use as a default*/
       /*prekin as of earlier than 050121 puts M# at the end of the pointID*/
       /* 012345678901234 */
       /*{ ca Aarg C_n..._ .... M##} expanded regular prekin point ID*/
       j = strlen(numstr); /*so last index is j-1 */
       /*at least blank,M,1 :3 more characters than minimal, but */
       /*regular prekin pointID compresses such that atom designation*/
       /*can be less than 15 char, so there is no safe limit to avoid search*/
       /*but probably can't ever have less than about 10 char of atom stuff*/

       if(j > 10)
       {/*maybe room in pointID for model number*/
          /*likely Bval before M anyway, so work backwards*/
          for(k=(j-2); k>8; k--) /*at least one char past M for model number*/
          {/*work backwards through pointID to find a model number*/
             numberstr[0] = '\0'; /*end the str for now*/
             if(numstr[k] == 'M')
             {/*model number might be in following position(s) */
                if(isdigit(numstr[k+1]))
                {/*at least a starting digit*/
                   numberstr[0] = numstr[k+1];
                   numberstr[1] = '\0'; /*end the str for now*/
                }
                if( ((k+2)<j) && isdigit(numstr[k+2]) )
                {/*second digit could be part of a model number*/
                   numberstr[1] = numstr[k+2];
                   numberstr[2] = '\0'; /*end the str for now*/
                   if( ((k+3)<j) && isdigit(numstr[k+3]) )
                   {/*third digit could be part of a model number*/
                      numberstr[2] = numstr[k+3]; 
                      numberstr[3] = '\0'; /*end the str for now*/
                      /*might as well quit, model numbers > 999 not reasonable*/
                   }
                }
                break; /*out of for loop*/
             }/*model number might be in following position(s) */
          }/*work backwards through pointID to find a model number*/
          if(numberstr[0] != '\0')
          {
             update_res_model = intfromstr(numberstr);
          }
          else
          {
             update_res_model = -1; /*flag not to send model number*/
          }
       }/*maybe room in pointID for model number*/
    }/*find res num, name, etc...  in pointID*/
    else
    {
       word[0] = '\0';  /*NO picked point, let residue = 0 */
       res_name[0] ='a';res_name[1] ='l';res_name[2] ='a';res_name[3] ='\0';
       res_chain = ' ';
    }
    update_res_num   = intfromstr(word);
    
    if(Lkinpdbfile)
    {/*kinemage file contained name for original pdb file*/
       sprintf(update_file_name,"%s",kinpdbfilestr);
    }/*kinemage file contained name for original pdb file*/
    else
    {/*try to generate pdb file name from group name*/
       if(pickedpointptr != NULL)  /*971223*/
          sprintf(numstr,"%s"
             ,(((pickedpointptr->listptr)->sgrpptr)->grupptr)->name);
       else if(firstgrupptr != NULL)
          sprintf(numstr,"%s",firstgrupptr->name);
       else numstr[0] = '\0';
       k=0;
       kk=0;
       if(numstr[0] != '\0') /*971223*/
       {
          if(numstr[0] == '*' ||numstr[0] == '%' ||numstr[0] == ' ') kk=1;
          /*animate controls or leading blank*/
          for(j=kk; j<MAXNAMECHAR; j++)
          {
             if( (numstr[j] == ' '&& k>0) || numstr[j] == '\0') 
             {/*blanks after characters or end-of-string*/
                break;
             }
             else
             {
                word[k] = numstr[j];
                k++;
             }
          }
       }
       word[k] = '\0';
       sprintf(update_file_name,"%s",word);
    }/*try to generate pdb file name from group name*/

    /*prekin matches '_'  to ' ' i.e. NO identifier at all */
    /* there is NO wild-card symbol that matches any identifier*/
    if(res_chain == ' ') {res_chain = '_';} /*a prekin convention*/
    if(atom_alt  == ' ') {atom_alt  = '_';} /*a prekin convention*/
    if(Laction == 1) {sprintf(type_str,"-rotate");/* set up for rotation*/}
    else             {sprintf(type_str,"-mutate");/* set up for mutation*/}
    if(update_res_model < 0)
    {/*do not specify a model: prekin will allow any and all models...*/
       sprintf(local_command_line,"prekin %s - -scope -show  \"sc,hy\" "
         " %s %d %s -alt%c -chainID %c -append -atomradii"
         ,update_file_name,type_str,update_res_num,res_name,atom_alt,res_chain
              );
    }
    else
    {/*a particular model is requeste*/
       sprintf(local_command_line,"prekin %s - -scope -show  \"sc,hy\" "
         " %s %d %s -alt%c -chainID %c -model %d -append -atomradii"
         ,update_file_name,type_str,update_res_num,res_name,atom_alt,res_chain
         ,update_res_model
              );
    }
 }/*see if can identify a putative sc from the last picked point*/
 else if(Laction==0 && input_command_line[0] != '\0')
 {/*Show inputed command line*/
    sprintf(local_command_line,"%s",input_command_line);
 }
 else /*Laction==0 but no input_command_line*/
 {/*Show same commandline as used before*/
   sprintf(local_command_line,"%s",update_prekin_command_line);
 }
 sprintf(update_prekin_command_line,"%s",local_command_line); /*remember unedited*/

 sprintf(dlog_optionstr,"show stored command string");
 Ldlog_option = 1;

 sprintf(dlog_infostr,"%s",local_command_line);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATEPREKIN_DLOG;
}
/*___updatePrekinDialog()____________________________________________________*/

/****EndupdatePrekinDialog()**************************************************/
void    EndupdatePrekinDialog()
{
 ACTIVE_DLOG = 0;
 if(Ldlog_OKHIT) 
 {
      sprintf(update_prekin_command_line,"%s",dlog_infostr);
      /*Lappend = 1;*/ /*will be taken care of by doupdate*/
      Lupdate = 1; /*so at least ONE updating can occur*/
      /*mycheckitem(kludgeMenu,remoteupdateItem,Lupdate);*/ /*___MENU.C*/
         /*do NOT check menu since prekin update is single shot*/
      Ldeleteprobedots = dlog_checkAint; 
      Ldeleteprekinappend = dlog_checkBint; 
      Linhibitbutton = 0; /*prekin only called once*/
      Linhibiton = 0; /*NO need for inhibition by right panel button*/

      Nthisgroupkind = PREKINAPPENDKKIND; 
      doupdate(0, update_prekin_command_line); /*____OUT.c*/
      /* 0 for NO put out of coords to remote program*/
      Nthisgroupkind = 0; 
      /*Lupdate = 0;*/ /*030312 allow interbedding of kinds of updating*/
      Lnewstuffactive = 1; /*protection: asks for output before closing*/
      /*Lprobeactive = 0;*/ /*probe will have no longer done the last update*/
         /*but deleting probe dots now separated from generic append 030312*/
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
 else if(Ldlog_optionHIT)
 {
      /*DoupdatePrekinDialog(-1);*/ /*redo the dialog,-1:show same commandline*/
      /* 0 flags inputed commandline*/
      NEXT_DLOG = UPDATEPREKIN_DLOG;
      NEXT_DLOG_PARAM = -1;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
}
/*___EndupdatePrekinDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdatePipeDialog()*****************************************************/
void    DoupdatePipeDialog(int Lpiper)
{
    if(!ACTIVE_DLOG)
    {
      cleargenericflags();  /*MAGEDLOG.c*/
      updatePipeDialog(Lpiper);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdatePipeDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
        }
}
/*___DoupdatePipeDialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updatePipeDialog()*******************************************************/
void    updatePipeDialog(int Lpiper)    /*UPDATEPIPE_DLOG*/
{

 if(Lpiper == 0)
 {/*expect to do an initialization*/
    sprintf(dlog_subjectstr
          ,"Initiate an active pipe from which to update kinemage"
          CRLF"         (can edit command line)");
 }/*expect to do an initialization*/
 else if(Lpiper > 0)
 {/*active pipe*/
    sprintf(dlog_subjectstr
       ,"pipe is active"
    CRLF"command line not an issue");
 }/*active pipe*/
 else if(Lpiper < 0)
 {
    ;
 }
 Ldlog_subject = 1;

 dlog_checkAint = Ldeletepipeinput; 
 sprintf(dlog_checkAstr,"delete last appended pipe input, if any.");
 Ldlog_checkA = 1;
 
 if(Lpipeactive)
 { 
     dlog_checkBint = 0; 
     sprintf(dlog_checkBstr,"flush pipe");
     Ldlog_checkB = 1;

     dlog_checkCint = 0; 
     sprintf(dlog_checkCstr,"KILL active pipe!");
     Ldlog_checkC = 1;

 } 
 
     dlog_checkEint = Linhibiton; 
     sprintf(dlog_checkEstr,"Single frame mode");
     Ldlog_checkE = 1;

 /*no presumptions: incoming update could be anything*/

 if(Lpiper==0 && input_command_line[0] != '\0')
 {/*Show inputed command line*/
    sprintf(update_command_line,"%s",input_command_line);
 }
 else if(Lpiper==0)
 {/*invent command line to initiate pipe*/
    if(Limporton)
    {
       sprintf(update_command_line,"%s",import_command_line); /*020514*/
    }
    else
    {
       sprintf(update_command_line,"tail -f simout.kin");
    } 
}/*invent command line to initiate pipe*/
 else
 {/*unclear what to put here*/
    ;
 }
 if(Lpiper==0)
 {
    sprintf(dlog_optionstr,"show command string again");
 }
 else
 {
    sprintf(dlog_optionstr," ");
 }
 Ldlog_option = 1;

 sprintf(dlog_infostr,"%s",update_command_line);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATEPIPE_DLOG;
}
/*___updatePipeDialog()______________________________________________________*/

/****EndupdatePipeDialog()****************************************************/
void    EndupdatePipeDialog()
{
 ACTIVE_DLOG = 0;
 if(Ldlog_OKHIT) 
 {
   if(Lpipeactive && dlog_checkBint)
   {/*request to flush pipe*/
      flushpipe(); /*____PIPE.c*/
   }
   else if(Lpipeactive && dlog_checkCint)
   {/*request to KILL pipe*/
      doupdatefrompipe(999, "\""); /*____PIPE.c*/
   }
   else
   {
      sprintf(update_command_line,"%s",dlog_infostr);
      /*Lappend = 1;*/ /*will be taken care of by doupdatefrompipe()*/
      Lupdate = 1; /*so ONE updating can occur*/
      mycheckitem(kludgeMenu,remoteupdateItem,Lupdate); /*___MENU.C*/
      Ldeletepipeinput = dlog_checkAint; 
      Lcontrol = 1; /*request another update from the pipe, i.e. single step*/
      Lcontrolon = 0; /*Lpipebufferloaded used as flag*/
      Linhibitbutton = 1; /*pipe can pause, if running on auto update*/
      if(dlog_checkEint)
         Linhibiton = 1; /*start in single step mode*/
      else
         Linhibiton = 0; /*start out running, otherwise: single step mode*/
      
      Nthisgroupkind = PIPEINPUTKKIND; 
      doupdatefrompipe(-1, update_command_line); /*____OUT.c*/
      /*-1 to initiate pipe*/
      /* !=1 for NO put out of coords to remote program*/
      Nthisgroupkind = 0; 
      /*successfule open sets Lpipeactive==1*/
      
      Lnewstuffactive = 1; /*protection: asks for output before closing*/
      /*Lprobeactive = 0;*/ /*probe will have no longer done the last update*/
      /*but probe dots now controlled by Ldeleteprobedots 030312*/
   }    
   LneedSetsinGrafWindow = 1;
	redrawvec();
 }
 else if(Ldlog_optionHIT)
 {
      sprintf(update_command_line,"%s",dlog_infostr);
      /*DoupdatePipeDialog(0);*/ /*redo the dialog,-1: show same commandline??*/
      NEXT_DLOG = UPDATEPIPE_DLOG;
      NEXT_DLOG_PARAM = 0;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
 }
}
/*___EndupdatePipeDialog()___________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdateSocketDialog()***************************************************/
void    DoupdateSocketDialog(int Lsockets)
{
    if(!ACTIVE_DLOG)
    {
      cleargenericflags();  /*MAGEDLOG.c*/
      updateSocketDialog(Lsockets);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdateSocketDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
        }
}
/*___DoupdateSocketDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updateSocketDialog()*****************************************************/
void    updateSocketDialog(int isetsocket)   /*UPDATESOCKET_DLOG*/
{
 char secondmessage[64];

 isetsocket = isetsocket; /*dummy, not using this control here*/
 /*be a client to a specific server which will mirror mage functions*/
 if(Lsockets==2 && Lsocketout == 1 && Lsocketoutextra == 0 )
 {/*already has normal remote, can connect to an extra remote*/
    sprintf(secondmessage
    ,"Already have one remote, can connect to extra remote.");
 }
 else
 {
    secondmessage[0] = '\0';
 }

 sprintf(dlog_subjectstr
          ,"Name a computer to which to send mage update stuff"
          CRLF"(please edit suggested remote computer name:)"
          CRLF" %s",secondmessage);
 Ldlog_subject = 1;
 
 if(remote_computer_name[0] == '\0')
 {/*invent a computer name*/
      sprintf(remote_computer_name,"val.biochem.duke.edu");
 }
 sprintf(dlog_optionstr,"show name string again");
 Ldlog_option = 1;

 sprintf(dlog_infostr,"%s",remote_computer_name);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATESOCKET_DLOG;
}
/*___updateSocketDialog()____________________________________________________*/

/****EndupdateSocketDialog()**************************************************/
void    EndupdateSocketDialog()
{
 ACTIVE_DLOG = 0;
 if(Ldlog_OKHIT) 
 {
    sprintf(remote_computer_name,"%s",dlog_infostr);
    setsockets(Lsockets); /*uses remote_computer_name*/
    if(Lsocketsame == 1)
    {/*mages must be initiated in reverse order, so 1-->2 and 1-->3*/
       /*-->2 just done, now send for a return connection from 2*/
       Lsocketreturn = 1; /*syncsocketout to request a return connection*/
       Lsocketoutactive = 1;
       setsockets(Lsockets); /*-->3 which does not use a return connection*/
    }

 }
 else if(Ldlog_optionHIT)
 {
    sprintf(remote_computer_name,"%s",dlog_infostr);
    /*DoupdateSocketDialog(Lsockets);*/  /*redo the dialog*/
    NEXT_DLOG = UPDATESOCKET_DLOG;
    NEXT_DLOG_PARAM = Lsockets;
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything */
 }
}
/*___EndupdateSocketDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdateContoursDialog()*************************************************/
void    DoupdateContoursDialog(int Lcontours)
{
    if(!ACTIVE_DLOG)
    {
      cleargenericflags();  /*MAGEDLOG.c*/
      updateContoursDialog(Lcontours);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdateContoursDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
        }
}
/*___DoupdateContoursDialog()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updateContoursDialog()***************************************************/
void    updateContoursDialog(int Laction)   /*UPDATECONTOURS_DLOG*/
{
 char local_command_line[256];

 Lgetmapfilename = 0;

 sprintf(dlog_subjectstr,"Invoke kin3Dcont to update contours"
       CRLF"-slevel sigma color -nosmooth -group -dom -name \"E.D.map\" "
       CRLF"-xplor mapfilename.xmap -clip x y z halfwidth -noperspective");
 Ldlog_subject = 1;

 dlog_checkAint = Ldeletemapcontours; 
 sprintf(dlog_checkAstr,"delete last appended map contours");
 Ldlog_checkA = 1;

 if(Laction==0 && input_command_line[0] != '\0')
 {/*Show inputed command line*/
    sprintf(local_command_line,"%s",input_command_line);
 }
 else if(Laction < 0)
 {/*Show same commandline as used before*/
   sprintf(local_command_line,"%s",update_contour_command_line);
 }
 else
 {/*no input commandline OR later cycles*/
    if(update_contour_command_line[0] == '\0')
    {/*first run, make guesses for commandline*/
       if(!Lkinmapfile && Lgetmapfilename!=2)
       {
          sprintf(kinmapfilestr,"mapfilenamestr.xmap");
       }
       sprintf(mapcolorstr,"gray");
       mapsigma = (float)1.2;
       maphalfwidth = (float)7.0;
    }
    sprintf(mapfocusxyzstr,"%s",focusxyzstr); /*from last left-mouse pick*/
   sprintf(local_command_line,"kin3Dcont -xplor %s -slevel %.2f %s -clip %s %.2f -nosmooth -noperspective -group -dom -name \"EDmap\" ",kinmapfilestr,mapsigma,mapcolorstr,mapfocusxyzstr,maphalfwidth);

 }
 sprintf(update_contour_command_line,"%s",local_command_line); /*remember*/

 sprintf(dlog_optionstr,"show stored command string");
 Ldlog_option = 1;

 sprintf(dlog_extrastr,"find a map file");
 Ldlog_extra = 1;

 sprintf(dlog_infostr,"%s",local_command_line);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATECONTOURS_DLOG;
}
/*___updateContoursDialog()__________________________________________________*/

/****EndupdateContoursDialog()************************************************/
void    EndupdateContoursDialog()
{
   int i=0,j=0,k=0,Lnew=0;
   
   ACTIVE_DLOG = 0;
 
 if(Ldlog_OKHIT) 
 {
      sprintf(update_contour_command_line,"%s",dlog_infostr);
      /*capture variable info of the commandline*/
      for(j=0; j<256; j++)
      {/*capture map file name*/
         if(update_contour_command_line[j]=='\0') {break;}
         if(strncmp(&update_contour_command_line[j],"-xplor",6) == 0)
         {/*next field is mapfilename*/
            Lnew = 1;
            for(k=j+7; k<256; k++)
            {
               if(getcharstring(update_contour_command_line[k],&Lnew) )
               {
                  strcpy(kinmapfilestr,temps);
                  break;
               }
            }
         }
      }
      for(j=0; j<256; j++)
      {/*capture contouring info*/
         if(update_contour_command_line[j]=='\0') {break;}
         if(strncmp(&update_contour_command_line[j],"-slevel",7) == 0)
         {/*next fields are sigma contour level and contour color*/
            Lnew = 1;
            for(k=j+8; k<256; k++)
            {
               if(getcharstring(update_contour_command_line[k],&Lnew) )
               {
                  mapsigma = (float)atof(temps);
                  break;
               }
            }
            Lnew = 1;
            for(i=k+1; i<256; i++)
            {
               if(getcharstring(update_contour_command_line[i],&Lnew) )
               {
                  strcpy(mapcolorstr,temps);
                  break;
               }
            }
         }
      }
      for(j=0; j<256; j++)
      {/*capture contour center xyz and width*/
         if(update_contour_command_line[j]=='\0') {break;}
         if(strncmp(&update_contour_command_line[j],"-clip",5) == 0)
         {/*next fields are sigma contour level and contour color*/
            Lnew = 1;
            for(k=j+6; k<256; k++)
            {
               if(getcharstring(update_contour_command_line[k],&Lnew) )
               {
                  mapfx = (float)atof(temps);
                  break;
               }
            }
            Lnew = 1;
            for(i=k+1; i<256; i++)
            {
               if(getcharstring(update_contour_command_line[i],&Lnew) )
               {
                  mapfy = (float)atof(temps);
                  break;
               }
            }
            Lnew = 1;
            for(k=i+1; k<256; k++)
            {
               if(getcharstring(update_contour_command_line[k],&Lnew) )
               {
                  mapfz = (float)atof(temps);
                  break;
               }
            }
            Lnew = 1;
            for(i=k+1; i<256; i++)
            {
               if(getcharstring(update_contour_command_line[i],&Lnew) )
               {
                  maphalfwidth = (float)atof(temps);
                  break;
               }
            }
            sprintf(mapfocusxyzstr,"%.3f %.3f %.3f",mapfx,mapfy,mapfz);
                   
         }
      }

      Lupdate = 1; /*so at least ONE updating can occur*/
      Ldeletemapcontours = dlog_checkAint; 
      Linhibitbutton = 0; /*kin3Dcont only called once*/
      Linhibiton = 0; /*NO need for inhibition by right panel button*/

      Nthisgroupkind = MAPCONTOURSKKIND;
      doupdate(0, update_contour_command_line); /*____OUT.c*/
      /* 0 for NO put out of coords to remote program*/
      Nthisgroupkind = 0;
      /*Lupdate = 0;*/  /*030312 allow interbedding of update kinds*/
      Lnewstuffactive = 1; /*protection: asks for output before closing*/
      /*Lprobeactive = 0;*/ /*probe will have no longer done the last update*/
      /*but probe dots now controlled by Ldeleteprobedots 030312*/
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
 else if(Ldlog_optionHIT)
 {
      /*redo the dialog,-1:show same commandline*/
      NEXT_DLOG = UPDATECONTOURS_DLOG;
      NEXT_DLOG_PARAM = -1;
 }
 else if(Ldlog_extraHIT)
 {
      Lcancel = 0; /* but don't change anything */
      Lgetmapfilename = 1; /*get new name of a map file*/
      FindFileName(); /*____DLOG.c*/
 }
 else if(Ldlog_cancelHIT)
 {
      Lcancel = 1; /* don't change anything, just stay with current values */
      LneedSetsinGrafWindow = 1;
      redrawvec();
 }
}
/*___EndupdateContoursDialog()_______________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoupdateNOEsDialog()*****************************************************/
void    DoupdateNOEsDialog(int LNOEs)
{
    if(!ACTIVE_DLOG)
    {
      cleargenericflags();  /*MAGEDLOG.c*/
      updateNOEsDialog(LNOEs);       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndupdateNOEsDialog();    /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
        }
}
/*___DoupdateNOEsDialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****updateNOEsDialog()*******************************************************/
void    updateNOEsDialog(int Laction)    /*UPDATENOES_DLOG*/
{
 char local_command_line[256];
 float  fdist=0,fxmid=0,fymid=0,fzmid=0;
 int LOK=0; /*explicit local*/

 LgetNOEfilename = 0;

 sprintf(dlog_subjectstr,"Invoke noe-display to update NOEs"
  CRLF"noe-display -f -s \"within R of X, Y, Z & RES_NUM\" PDB_FILE NOE_FILE"
        );
 Ldlog_subject = 1;

 if(Laction==0 && input_command_line[0] != '\0')
 {/*Show inputed command line*/
    sprintf(local_command_line,"%s",input_command_line);
 }
 else if(Laction < 0)
 {/*Show same commandline as used before*/
   sprintf(local_command_line,"%s",update_NOE_command_line);
 }
 else
 {/*no input commandline OR later cycles*/
    if(update_NOE_command_line[0] == '\0')
    {/*first run, make guesses for commandline*/
       if(!LkinNOEfile && LgetNOEfilename!=2)
       {
          sprintf(kinNOEfilestr,"NOEfilenamestr.tbl");
       }
       if(!Lkinpdbfile && Lgetpdbfilename!=2)
       {
          sprintf(kinpdbfilestr,"pdbfilenamestr.pdb");
       }
    }
    LOK = getrotationfocus(&fdist,&fxmid,&fymid,&fzmid);
    LOK = LOK; /*040609 not testing success at this time, avoid no-use warning*/
       sprintf(local_command_line,"noe-display -f -s "
        " \"within %.2f of %.2f, %.2f, %.2f & %s\" %s %s"
        ,fdist,fxmid,fymid,fzmid,update_res_num_str
        ,kinpdbfilestr,kinNOEfilestr
       );
       /*inside first pattern might be: "within %.2f of %.2f,%.2f,%.2f"*/
       /*where distance ~8 and x,y,z from ctr of rot or last picked pt*/
       /*now use center of box holding all mobile points*/
 }
 sprintf(update_NOE_command_line,"%s",local_command_line); /*remember*/

 sprintf(dlog_optionstr,"show stored command");
 Ldlog_option = 1;

 sprintf(dlog_extrastr,"find NOE file");
 Ldlog_extra = 1;
 sprintf(dlog_extra2str,"find pdb file");
 Ldlog_extra2 = 1;

 sprintf(dlog_infostr,"%s",local_command_line);
 Ldlog_info = 1;
 ACTIVE_DLOG = UPDATENOES_DLOG;
}
/*___updateNOEsDialog()______________________________________________________*/

/****EndupdateNOEsDialog()****************************************************/
void    EndupdateNOEsDialog()
{
   ACTIVE_DLOG = 0;
 
   if(Ldlog_OKHIT) 
   {
      sprintf(update_NOE_command_line,"%s",dlog_infostr);
      Lupdate = 1; /*so at least ONE updating can occur*/
      Linhibitbutton = 0; /*noe-display only called once*/
      Linhibiton = 0; /*NO need for inhibition by right panel button*/
      LdeleteNOEinput = 1; /*delete and replace earlier NOEs displayed*/
      Nthisgroupkind = NOEDISTANCESKKIND;
      doupdate(1, update_NOE_command_line); /*____OUT.c*/
      /* 1 to put out coords to remote program*/
      Nthisgroupkind = 0;
      /*Lupdate = 0;*/  /*030312 allow interbedding of update kinds*/
      Lnewstuffactive = 1; /*protection: asks for output before closing*/
      /*Lprobeactive = 0;*/ /*probe will have no longer done the last update*/
      /*but probe dots now controlled by Ldeleteprobedots 030312*/
      LNOEdisplayactive = 1;
      LneedSetsinGrafWindow = 1;
      redrawvec();
   }
   else if(Ldlog_optionHIT)
   {
      /*redo the dialog,-1:show same commandline*/
      NEXT_DLOG = UPDATENOES_DLOG;
      NEXT_DLOG_PARAM = -1;
   }
   else if(Ldlog_extraHIT)
   {
      Lcancel = 0; /* but don't change anything */
      LgetNOEfilename = 1; /*get new name of a NOE file*/
      RETURN_DLOG = UPDATENOES_DLOG;
      FindFileName(); /*____DLOG.c*/
   }
   else if(Ldlog_extra2HIT)
   {
      Lcancel = 0; /* but don't change anything */
      Lgetpdbfilename = 1; /*get new name of a pdb file*/
      RETURN_DLOG = UPDATENOES_DLOG;
      FindFileName(); /*____DLOG.c*/
   }
   else if(Ldlog_cancelHIT)
   {
      Lcancel = 1; /* don't change anything, just stay with current values */
      LneedSetsinGrafWindow = 1;
      redrawvec();
   }
}
/*___EndupdateNOEsDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DomickeymouseDialog()****************************************************/
void    DomickeymouseDialog()
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
       mickeymouseDialog();         /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
      EndmickeymouseDialog();     /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
  }
}
/*___DomickeymouseDialog()_________________________________________________*/

/****mickeymouseDialog()****************************************************/
void    mickeymouseDialog()     /*MICKEYMOUSE_DLOG*/
{
 sprintf(dlog_subjectstr,"Mickey Mouse animation and rocking rotation."
                     CRLF"r toggles autorock active state,"
                     CRLF"R toggles autoanimate active state,"
                     CRLF"if auto mode empowered."); /*011023*/
 Ldlog_subject = 1;

 dlog_checkAint = Lautoanimate;
 sprintf(dlog_checkAstr,"auto-animation");
 Ldlog_checkA = 1;
 
 sprintf(dlog_paramAstr,"%.2f",animatesteptime);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"animation time per step (seconds)");
 Ldlog_textA = 1;
 
 dlog_checkBint = Lautorock;
 sprintf(dlog_checkBstr,"auto-rocking rotation");
 Ldlog_checkB = 1;
 
 sprintf(dlog_paramBstr,"%.2f",rockmin);
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"rock minimum");
 Ldlog_textB = 1;
 
 sprintf(dlog_paramCstr,"%.2f",rockmax);
 Ldlog_paramC = 1;
 sprintf(dlog_textCstr,"rock maximum");
 Ldlog_textC = 1;

 sprintf(dlog_paramDstr,"%.2f",rockcycletime);
 Ldlog_paramD = 1;
 sprintf(dlog_textDstr,"rock time per cycle (seconds)");
 Ldlog_textD = 1;

 sprintf(dlog_paramEstr,"%d",(int)rocknumsteps);
 Ldlog_paramE = 1;
 sprintf(dlog_textEstr,"rock steps per cycle");
 Ldlog_textE = 1;

 dlog_checkCint = Lmickeymouse;
 sprintf(dlog_checkCstr,"empower auto mode");
 Ldlog_checkC = 1;
 
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = MICKEYMOUSE_DLOG;
}
/*___mickeymouseDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndmickeymouseDialog()*************************************************/
void    EndmickeymouseDialog()
{
 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*????*/
      /*Lmickeymouse = 0;*/ /*cancel this state*/
      /*Lmickeymouserun = 0;*/ /*cancel this state*/
 }
 else if(Ldlog_OKHIT) 
 {
      animatesteptime = floatfromstr(dlog_paramAstr);
      rockmin         = floatfromstr(dlog_paramBstr);
      rockmax         = floatfromstr(dlog_paramCstr);
      rockcycletime   = floatfromstr(dlog_paramDstr);
      rocknumsteps    = floatfromstr(dlog_paramEstr);
      Lautoanimate    = dlog_checkAint;
      Lautorock       = dlog_checkBint;
      Lmickeymouse    = dlog_checkCint;
      
      if(Lmickeymouse &&(Lautorock || Lautoanimate))
      {
         Lmickeymouserun = 1;
      }
      else
      {
         Lmickeymouserun = 0;
      }
 }
 resetmenuchecks();
 ACTIVE_DLOG = 0;
}
/*___EndmickeymouseDialog()________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DopickmarkerDialog()***************************************************/
void    DopickmarkerDialog()
{
    if(!ACTIVE_DLOG)
    {
      cleargenericflags();  /*MAGEDLOG.c*/
      pickmarkerDialog();       /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
            EndpickmarkerDialog();    /*MAGEDLOG.c*/
#endif
        }
}
/*___DopickmarkerDialog()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****pickmarkerDialog()*****************************************************/
void    pickmarkerDialog()      /*PICKMARKER_DLOG*/
{
   sprintf(dlog_subjectstr,"optional extended axial line through pick marker"
                     CRLF"Use for correlating across 2D, flatland, graphs");
   Ldlog_subject = 1;

   if((Lpickmarkcriteria & 64) ==  64) dlog_checkAint = 1;
   else dlog_checkAint = 0; 
   sprintf(dlog_checkAstr,"pick markers made of narrow lines");
   Ldlog_checkA = 1;

   if((Lpickmarkcriteria & 128) == 128) dlog_checkBint = 1;
   else dlog_checkBint = 0; 
   sprintf(dlog_checkBstr,
      "pick markers seem behind, rather than in front of things");
   Ldlog_checkB = 1;


   dlog_radioAint = 0; 
   dlog_radioBint = 0;
   dlog_radioCint = 0;
   dlog_radioDint = 0;
   dlog_radioEint = 0;

   if     ((Lpickmarkcriteria &  2049) ==  2049) dlog_radioAint = 1; 
   else if((Lpickmarkcriteria &  4098) ==  4098) dlog_radioBint = 1;
   else if((Lpickmarkcriteria &  8196) ==  8196) dlog_radioCint = 1;
   else if((Lpickmarkcriteria &  8200) ==  8200) dlog_radioDint = 1;
   else {Lpickmarkcriteria = 0; dlog_radioEint = 1;}

   sprintf(dlog_radioAstr,
      "pick marker with extended line in X axial directions.");
   Ldlog_radioA = 1;
   sprintf(dlog_radioBstr,
      "pick marker with extended line in Y axial directions.");
   Ldlog_radioB = 1;
   sprintf(dlog_radioCstr,
      "pick marker with extended line in Z axial directions.");
   Ldlog_radioC = 1;
   sprintf(dlog_radioDstr,
      "pick marker with extended lines in +X,+Z axial directions.");
   Ldlog_radioD = 1;
   sprintf(dlog_radioEstr,
      "Usual pick marker with NO extended lines.");
   Ldlog_radioE = 1;

 
   sprintf(dlog_infostr,
     "pick marker line extends to actual data limits in that axial direction.");
   Ldlog_info = 1;
   ACTIVE_DLOG = PICKMARKER_DLOG;
}
/*___pickmarkerDialog()____________________________________________________*/

/****EndpickmarkerDialog()**************************************************/
void    EndpickmarkerDialog()
{
  ACTIVE_DLOG = 0;
  if(Ldlog_OKHIT) 
  {
     /*first set the axial extent, if any, of the pick markers*/
     if     (dlog_radioAint == 1) Lpickmarkcriteria = 1+ 2048; /*2049*/
     else if(dlog_radioBint == 1) Lpickmarkcriteria = 2+ 4096; /*4098*/
     else if(dlog_radioCint == 1) Lpickmarkcriteria = 4+ 8192; /*8196*/
     else if(dlog_radioDint == 1) Lpickmarkcriteria = 8+ 8192; /*8200*/
     else Lpickmarkcriteria = 0;
     /*then OR in width and apparant postion */
     if(dlog_checkAint == 1) Lpickmarkcriteria = Lpickmarkcriteria |  64;
     if(dlog_checkBint == 1) Lpickmarkcriteria = Lpickmarkcriteria | 128;
	 redrawvec();
  }
  else if(Ldlog_cancelHIT)
  {
     Lcancel = 1; /* don't change anything, just stay with current values */
  }
}
/*___EndpickmarkerDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DoAdvancedHelpDialog()***************************************************/
void    DoAdvancedHelpDialog(int mode)
{
   if(!ACTIVE_DLOG)
   {
      cleargenericflags();  /*MAGEDLOG.c*/
      AdvancedHelpDialog(mode);         /*MAGEDLOG.c*/
      DogenericDialog();    /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB */ 
      EndAdvancedHelpDialog(mode);      /*MAGEDLOG.c*/
      if(NEXT_DLOG){DonextanotherDialog();}  /*____DLOG.c*/
#endif
   }
}
/*___DoAdvancedHelpDialog()__________________________________________________*/

/****AdvancedHelpDialog()*****************************************************/
void    AdvancedHelpDialog(int mode)       /*MAKETEXTHELP_DLOG*/
{                                          /*MAKEKINEMAGE_DLOG*/
#define TEXTWINHELP  1                     /*MAKEHTMLHELP_DLOG*/
#define MAKEKINHELP  2                     /*MAKEDOCKOBJECT_DLOG*/
#define HTMLOUTHELP  3
#define DOCKKINHELP  4
#define DOCKPLATONICS 5
#define DOCKTETRAMERS 6
   if(mode == TEXTWINHELP) /*TEXT into text window*/
   {
     sprintf(dlog_subjectstr, /*121110 subject box size width of text below it*/
     "WARNING: THIS WILL ADD THINGS TO YOUR TEXT,"
     CRLF" if in doubt: CANCEL."
     CRLF"Help information is appended to text window, scroll down to see it.");
   }
   else if(mode==MAKEKINHELP) /*internally generated kinemage*/
   {
     sprintf(dlog_subjectstr, /*121110 subject box size width of text below it*/
     "WARNING: THIS WILL ADD THINGS TO YOUR TEXT,"
     CRLF" if in doubt: CANCEL."
      CRLF"Internally generated kinemage put into graphics.");
   }
   else if(mode==HTMLOUTHELP) /* help written to html file 020929*/
   {
     /*Beware: dlog widget strings are often just 64 char 121110*/
     sprintf(dlog_subjectstr,/*121110 subject box size width of text below it*/
     "HELP IN HTML FORMAT TO READ IN A BROWSER."
     CRLF
     CRLF"Various Mage Internal help stuff put into html format.");
   }
   else /*if(mode>=DOCKKINHELP) choice of starting dock object shape 030217*/
   {
      sprintf(dlog_subjectstr,
          "CHOOSE STARTING SHAPE"
      CRLF" FOR DOCKING OBJECTS"
      CRLF" ");
   }
   Ldlog_subject = 1;
   
   dlog_radioAint = 1; 
   dlog_radioBint = 0;
   dlog_radioCint = 0;
   dlog_radioDint = 0;
   dlog_radioEint = 0;
   dlog_radioFint = 0;
   dlog_checkAint = 0;

   if(mode == TEXTWINHELP) /*TEXT into text window*/
   {   
      sprintf(dlog_radioAstr,"Mage @keywords listed: in text window");
      Ldlog_radioA = 1; 
      sprintf(dlog_radioBstr,
         "Group, Subgroup, List, and Point Properties: in text window");
      Ldlog_radioB = 1;
      sprintf(dlog_radioCstr,"Mage Help Menu Items: in text window");
      Ldlog_radioC = 1;
      sprintf(dlog_radioDstr,"About: Mage Hypertext: in text window");
      Ldlog_radioD = 1;
      sprintf(dlog_radioEstr,"About: Mage Docking: in text window");
      Ldlog_radioE = 1;
      sprintf(dlog_radioFstr
      ,"About: setting up two remote instances of Mage: in text window");
      Ldlog_radioF = 1;
   
      ACTIVE_DLOG = MAKETEXTHELP_DLOG;
      NEXT_DLOG_PARAM = TEXTWINHELP;
   }
   else if(mode == MAKEKINHELP) /*internally generated kinemage*/
   {
       /*121110 MAGEDLOG.h: box strings are 64 char*/
       /*1234567890123456789012345678901234567890123456789012345678901234*/
      sprintf(dlog_checkAstr /*121110 MAGEDLOG.h: dlog_checkAstr[64] */
       ,"FOO _______capture____the____foo____along___with____Docking____");
      Ldlog_checkA = 1; 
      sprintf(dlog_radioAstr,"Internally generated palette kinemage");
      Ldlog_radioA = 1; 
      sprintf(dlog_radioBstr,"Internally generated demo kinemage");
      Ldlog_radioB = 1;
      sprintf(dlog_radioCstr,"Practice Docking with cubes");
      Ldlog_radioC = 1;
      sprintf(dlog_radioDstr,"Practice Docking with widgets");
      Ldlog_radioD = 1;
      sprintf(dlog_radioEstr,"Practice Docking with Platonics+");
      Ldlog_radioE = 1;
      sprintf(dlog_radioFstr,"Practice Docking with tetramers");
      Ldlog_radioF = 1;
   
      ACTIVE_DLOG = MAKEKINEMAGE_DLOG;
      NEXT_DLOG_PARAM = MAKEKINHELP;
   }
   else if(mode==HTMLOUTHELP) /* help written to html file 020929*/
   {
       /*1234567890123456789012345678901234567890123456789012345678901234*/
      sprintf(dlog_radioAstr /*121110 MAGEDLOG.h: dlog_radioAstr[64] */
       ,"Write Mage internal help out to a new html file.______________");
      Ldlog_radioA = 1;
#ifdef TRYLAUNCH
      sprintf(dlog_radioBstr,"As above but also launch netscape...");
      Ldlog_radioB = 1;
      sprintf(dlog_radioCstr,"As above but also launch mozilla...");
      Ldlog_radioC = 1;
      sprintf(dlog_radioDstr,"As above but also launch explorer...");
      Ldlog_radioD = 1;
      sprintf(dlog_radioEstr,"As above but also launch safari...");
      Ldlog_radioE = 1;
#endif
      ACTIVE_DLOG = MAKEHTMLHELP_DLOG;
      NEXT_DLOG_PARAM = HTMLOUTHELP;
   }
   else if(mode == DOCKPLATONICS) /*dock object choices*/
   {
      sprintf(dlog_radioAstr,"tetrahedron");
      Ldlog_radioA = 1; 
      sprintf(dlog_radioBstr,"cube");
      Ldlog_radioB = 1;
      sprintf(dlog_radioCstr,"octahedraon");
      Ldlog_radioC = 1;
      sprintf(dlog_radioDstr,"widget");
      Ldlog_radioD = 1;
      sprintf(dlog_radioEstr,"water");
      Ldlog_radioE = 1;
      sprintf(dlog_checkAstr,"FOO");
      Ldlog_checkA = Legamickfoo; 
   
      ACTIVE_DLOG = MAKEDOCKOBJECT_DLOG;
      NEXT_DLOG_PARAM = DOCKPLATONICS;
   }
   else if(mode == DOCKTETRAMERS) /* dock object choices*/
   {/*not used 030329, archive for future starting shape choice*/
      sprintf(dlog_radioAstr,"(lion)");
      Ldlog_radioA = 1; 
      sprintf(dlog_radioBstr,"cube always starts!");
      Ldlog_radioB = 1;
      sprintf(dlog_radioCstr,"(knight)");
      Ldlog_radioC = 1;
      sprintf(dlog_radioDstr,"(knave)");
      Ldlog_radioD = 1;
      sprintf(dlog_radioEstr,"(caltrop)");
      Ldlog_radioE = 1;
   
      ACTIVE_DLOG = MAKEDOCKOBJECT_DLOG;
      NEXT_DLOG_PARAM = DOCKTETRAMERS;
   }
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
}
/*___AdvancedHelpDialog()____________________________________________________*/

/****EndAdvancedHelpDialog()**************************************************/
void    EndAdvancedHelpDialog(int mode)
{
#define TEXTWINHELP  1
#define MAKEKINHELP  2
#define HTMLOUTHELP  3
#define DOCKKINHELP  4
#define DOCKPLATONICS 5
#define DOCKTETRAMERS 6
#define MAKEPALETTE 1
#define MAKEEXAMPLE 2
#define MAKEDOCKCUBE 3
#define MAKEDOCKWIDGET 4
#define MAKEDOCKMULTI 5
  int Nchoice = 0, IOK=0;
   
  ACTIVE_DLOG = 0; /*so another dialog can be called*/

  if(Ldlog_OKHIT) 
  {/*Ldlog_OKHIT*/
    if     (Ldlog_radioA && dlog_radioAint) {Nchoice = 1;}
    else if(Ldlog_radioB && dlog_radioBint) {Nchoice = 2;}
    else if(Ldlog_radioC && dlog_radioCint) {Nchoice = 3;}
    else if(Ldlog_radioD && dlog_radioDint) {Nchoice = 4;}
    else if(Ldlog_radioE && dlog_radioEint) {Nchoice = 5;}
    else if(Ldlog_radioF && dlog_radioFint) {Nchoice = 6;}

    if(mode == TEXTWINHELP) /*TEXT into text window*/
    {
       dohelpintextwin(Nchoice);  /*MAGEHELP.c*/
       Lnewstuffactive = 1; /*so will be warned to save kinemage*/
    }
    else if(mode==HTMLOUTHELP)
    {
       if(Nchoice==1||Nchoice==2||Nchoice==3||Nchoice==4||Nchoice==5)
       {
          writeoutput(14); /*____OUT.c*/
          if(Nchoice!=1)
          {
             if(Nchoice == 2) {sprintf(word,"netscape %s\n",OutStr);}
             if(Nchoice == 3) {sprintf(word,"mozilla file:///%s\n",OutStr);}
             if(Nchoice == 4) {sprintf(word,"explorer %s\n",OutStr);}
             launchnleave(0,word);
          }
       }
       else if(Nchoice==6)
       {
          sprintf(word,"mozilla");
          launchnleave(1,word); /*which will call writehelptohtmlfile()*/
       }
    }
    else 
    {/*make an internal kinemage*/
#ifndef OLD68KMAC /*defined in MAGE.h*/
       IOK = 1;
#else  /*restrictions on program size in OLD 68K type Mac OS */
       if(Nchoice == 1) IOK = 1;
       else
       {
           sprintf(alertstr
              ,"68K Macintosh lacks room to store this example kinemage");
           alertstr2[0]='\0';alertstr3[0]='\0';
           DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
           IOK = 0;
           Lcancel=1; /*don't change anything, just stay with current values*/
       }
#endif /*NOT OLD68KMAC defined in MAGE.h*/
       if(IOK)
       {/*make one of a variety of kinemages*/
         if( mode == MAKEKINHELP )
         {/* MAKEKINHELP, a first pass mode*/
            if(Nchoice == MAKEPALETTE )
            {
               createinternalkinemage(MAKEPALETTE); /*.*/
            }
            else if(Nchoice == MAKEEXAMPLE )
            {
               createinternalkinemage(MAKEEXAMPLE); /*.*/
            }
            else if(Nchoice == MAKEDOCKCUBE )
            {
               Legamickfoo = dlog_checkAint;
               Negamickvalue = 1; /*Platonics each just one item*/
               Negamickshape = 2; /*2 cube*/
               createinternalkinemage(MAKEDOCKCUBE); /*.*/
            }
            else if(Nchoice == MAKEDOCKWIDGET )
            {
               Legamickfoo = dlog_checkAint;
               Negamickvalue = 1; /*Platonics each just one item*/
               Negamickshape = 4; /*4 widget*/
               createinternalkinemage(MAKEDOCKWIDGET); /*.*/
            }
            else if(Nchoice==DOCKPLATONICS) /*choice NOT mode*/
            {/*do another pass: AdvancedHelpDialog to get starting shape*/
               /*Invokes DoAdvancedHelpDialog() for starting shape*/
               NEXT_DLOG = MAKEDOCKOBJECT_DLOG;
               NEXT_DLOG_PARAM = DOCKPLATONICS;
            }
            else if(Nchoice==DOCKTETRAMERS) /*choice NOT mode*/
            {
               Legamickfoo = dlog_checkAint;
               Negamickvalue = 1; /*cube is just one item*/
               Negamickshape = 2; /*always starts with a cube*/
               createinternalkinemage(DOCKTETRAMERS); /*.*/
            }
         }/* MAKEKINHELP, a first pass mode*/
         else if( mode == DOCKPLATONICS )
         {
               Legamickfoo = dlog_checkAint;
            Negamickvalue = 1; /*Platonics each just one item*/
            Negamickshape = Nchoice;
            createinternalkinemage(DOCKPLATONICS); /*.*/
         }
         else if( mode == DOCKTETRAMERS )
         {/*NOT used 030329, archive for future starting shape choice*/
               Legamickfoo = dlog_checkAint;
            Negamickvalue = 1; /*cube is just one item*/
            Negamickshape = 2; /*always starts with cube*/
            createinternalkinemage(DOCKTETRAMERS); /*.*/
         }
       }/*make one of a variety of kinemages*/
    }/*make an internal kinemage*/
      
  }/*Ldlog_OKHIT*/
  else if(Ldlog_cancelHIT)
  {
     Lcancel = 1; /*don't change anything, just stay with current values*/
  }
}
/*___EndAdvancedHelpDialog()_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****createinternalkinemage()************************************************/
void    createinternalkinemage(int mode)
{
#define MAKEPALETTE 1
#define MAKEEXAMPLE 2
#define MAKEDOCKCUBE 3
#define MAKEDOCKWIDGET 4
#define MAKEDOCKMULTI 5
#define DOCKPLATONICS 5
#define DOCKTETRAMERS 6
   
   if(Lkinfile==0 || Lkinfile==2 || Lkinfile==3)
   {/*No kinfile OR no kinfile but internally generated text*/
      Lappend = 0;
      Lkinfile = 3; /*for internally generated kinemage*/
   }
   else 
   {/*Lkinfile==1, and there is a kinemage file open*/
      /*note: Lkinfile==3 for an internally generated kinemage*/
      /*leave Lkinfile set to what it was and append new stuff*/
      Lappend = 1;
   }

   sprintf(KineName,"Kinemage  0 - "); 
           /*must add this extra - so output can start just after it*/
   sprintf(NameStr,"magedemo");

   Lnewstuffactive = 1; /*so will be warned to save kinemage*/
   Linternal = 1; 
   Ninternal = mode;
   if(mode == MAKEPALETTE || mode == MAKEEXAMPLE)
   {
      entry(); /*for palette or kinemage example*/
      /* MAGEFILE/entry() calls MAGEINPT/GetStuffFromFile() which tests */
      /* Linternal to get coords from MAGEHELP/getinternalkinemage(Ninternal) */
   }
   else if(   mode == MAKEDOCKCUBE  || mode == MAKEDOCKWIDGET
           || mode == DOCKPLATONICS || mode == DOCKTETRAMERS )
   {/*static & mobile thing for egamickdocking egam 030216,7 rev 050816*/

      /* 1: call entry() for an extra pass to make a static starting structure*/
      /* Ninternal==5 case: Negamickshape sets selected starting shape*/
      /* MAGEFILE/entry() calls MAGEINPT/GetStuffFromFile() which tests */
      /* Linternal to get coords from MAGEHELP/getinternalkinemage(Ninternal) */

      if(mode == MAKEDOCKCUBE || mode == MAKEDOCKWIDGET){Ninternal = mode;}
      else {Ninternal = 5;} /* reference structure set by Negamickshape*/ 
/*1*/ entry(); /*reference group*/

      /* 2: setup for the mobile group, and call entry() again.*/
      if(mode==DOCKPLATONICS)
      {
         Negamickshape = 2; /*always start with a cube*/
         Ltetramers = 0;
         Legamickchoice = 1;
         Lfreeegamickmobile = 1; /*presume mobile list will be created*/
      } 
      if(mode==DOCKTETRAMERS)
      {
         Negamickshape = 2; /*always start with a cube*/
         Ltetramers = 1;
         Legamickchoice = 1;
         Lfreeegamickmobile = 1; /*presume mobile list will be created*/
      }
      if(mode== MAKEDOCKWIDGET)
      {
         Negamickshape = 4; /*4 widget*/
         Ltetramers = 0;
         Legamickchoice = 0;
      }
      if(mode== MAKEDOCKCUBE)
      {
         Negamickshape = 2; /*2 cube*/
         Ltetramers = 0;
         Legamickchoice = 0;
      }

      setdrawstuff(16); /*MAGEMENU 00010000 Ldrawstuff, Ldock3on3, Ldock3on3on*/
      Lappend = 1;
      Legamickdocking = 1;
      Ldockingscope = 1;
      Legamickrepeat = 1;
      LegamickrepeatOK = 0;
      Lbeginselection = 1;
      Ninternal = 10 + Negamickshape; /*tetra 1, cube 2, octa 3, widget 4*/
         /*water 5 091127*/
      /* MAGEFILE/entry() calls MAGEINPT/GetStuffFromFile() which tests */
      /* Linternal to get coords from MAGEHELP/getinternalkinemage(Ninternal) */
/*2*/ entry();  /*mobile group*/

      if(Legamickfoo) /*capture the foo*/
      {/*set up for foo group*/
         /* 3: setup for the foo group, and call entry() again.*/
         Legamickfooon = 1; /*compute foo packing*/
         Lwildfoo = 1; /*wild foo will try to escape*/
         Negamickfoocount = 1;
         Negamickbarcount = 1;
         Negamickpass = 1;
         fooradius = (float)2;

         FOOGRID = FACECENTERCLOSEST; /*050816 default choice*/

         if(FOOGRID == SIMPLECUBIC) { foogridstep = (float)1; }
         else {foogridstep = (float)1.1; }

         Ninternal = 31;
        /*MAGEFILE/entry() calls MAGEINPT/GetStuffFromFile() which tests */
        /*Linternal to get coords from MAGEHELP/getinternalkinemage(Ninternal)*/
/*3*/    entry();  /*foo group, Ninternal==31 defines foogrupptr*/
         foogrupptr->KKIND = FOOKKIND; /*distinguish foo groups*/
      
         /*set up for dotball surface dots group*/
         foodotdensity = 10;
         freeDotSphere(&dotsphereset);
         dotSphere(&dotsphereset, fooradius, foodotdensity); /*MAGEFOO.c*/
      }
   }
   Linternal = 0; /*restore*/
   AdjustMenus(); /*clean up work*/
}
/*___createinternalkinemage()_______________________________________________*/

/****DofoocavityDialog()******************************************************/
void    DofoocavityDialog()
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
       foocavityDialog();        /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
      EndfoocavityDialog();     /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
  }
}
/*___DofoocavityDialog()_____________________________________________________*/

/****foocavityDialog()********************************************************/
void    foocavityDialog()          /*FOOCAVITY_DLOG*/
{
   sprintf(dlog_subjectstr,"foo in cavity parameters"
   CRLF"FOO will try to be placed on last picked point!"
   );
   Ldlog_subject = 1;
 
   if(   FOOGRID != SIMPLECUBIC 
      && FOOGRID != FACECENTERCLOSEST 
      && FOOGRID != HEXAGONALCLOSEST ) {FOOGRID = FACECENTERCLOSEST;}
   if(fooradius == 0) {fooradius = (float)1.0;}
   if(foodotdensity == 0) {foodotdensity = (float)16;}
   if(foogridstep ==0) 
   {
      if(FOOGRID == SIMPLECUBIC) { foogridstep = (float)0.3; }
      else {foogridstep = (float)0.4; }
   }
   sprintf(dlog_paramAstr,"%.3f",fooradius);
   Ldlog_paramA = 1;
   sprintf(dlog_textAstr,"foo radius");
   Ldlog_textA = 1;
 
   sprintf(dlog_paramBstr,"%.3f",foogridstep);
   Ldlog_paramB = 1;

   if(FOOGRID == SIMPLECUBIC)
      {sprintf(dlog_textBstr,"foo grid spacing in simple cubic lattice");}
   else /*either FACECENTERCLOSEST or HEXAGONALCLOSEST */
      {sprintf(dlog_textBstr,"foo grid spacing in closest packed lattice");}

   Ldlog_textB = 1;
 
   sprintf(dlog_paramCstr,"%.3f",foodotdensity);
   Ldlog_paramC = 1;
   sprintf(dlog_textCstr,"foo dot density");
   Ldlog_textC = 1;

   dlog_checkAint = Lwildfoo = 0; /*default domestic for foo cavity*/
   sprintf(dlog_checkAstr,"wild foo: will try to escape");
   Ldlog_checkA = 1;
   
   dlog_checkBint = Lslimemode; /*foo not calc at distance 030709*/
   sprintf(dlog_checkBstr,"foo stays close, does not stray out into void");
   Ldlog_checkB = 1;
  
   dlog_checkCint = LFOOinBOX; /*FOOinBOX  100818*/
   sprintf(dlog_checkCstr,"foo grid calc to encase whole molecule");
   Ldlog_checkC = 1;
  
   dlog_radioAint = 0;
   dlog_radioBint = 0;
   dlog_radioCint = 0;
   if(FOOGRID == SIMPLECUBIC)            {dlog_radioAint = 1;} 
   else if(FOOGRID == FACECENTERCLOSEST) {dlog_radioBint = 1;} 
   else if(FOOGRID == HEXAGONALCLOSEST)  {dlog_radioCint = 1;} 
   sprintf(dlog_radioAstr,"Simple Cubic Lattice");
   Ldlog_radioA = 1;
 
   sprintf(dlog_radioBstr,"Face Centered Closest Packed Lattice");
   Ldlog_radioB = 1;
 
   sprintf(dlog_radioCstr,"Hexagonal Closest Packed Lattice");
   Ldlog_radioC = 1;

   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = FOOCAVITY_DLOG;
}
/*___foocavityDialog()_______________________________________________________*/

/****EndfoocavityDialog()*****************************************************/
void    EndfoocavityDialog()
{
   static int Firstpass = 1;
   struct pointstruct* thepointptr=NULL;
   int    Lcollision=0; /*030630*/
   int    LrawFOOkin=0; /*050816*/
   
 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_OKHIT) 
 {
      fooradius     = floatfromstr(dlog_paramAstr);
      foogridstep   = floatfromstr(dlog_paramBstr);
      foodotdensity = floatfromstr(dlog_paramCstr);
      Lwildfoo = dlog_checkAint;
      Lslimemode = dlog_checkBint; /*030709*/
      LFOOinBOX = dlog_checkCint; /*100818*/
      
   if (dlog_radioAint == 1)      {FOOGRID = SIMPLECUBIC;}   /*050816*/         
   else if (dlog_radioBint == 1) {FOOGRID = FACECENTERCLOSEST;} 
   else if (dlog_radioCint == 1) {FOOGRID = HEXAGONALCLOSEST;}  

   /*place foo at picked point*/
   Lfoocavity = 1;
   Lfoosealer = 1;
   Lappend = 1;
   Linternal = 1;
   /*Ninternal = 31; */ /*the foo*/
   Ninternal = 32; /*the raw foo*/

   if(Lkinfile == 0) {LrawFOOkin = 1;}/*050816 raw invocation, first kin stuff*/

   entry(); /*calls createinternalkinemage(Ninternal): appends new foo group*/
   /*MAGESETS/addanothervector() (re)defines foogrupptr*/
   foogrupptr->KKIND = FOOKKIND; /*distinguish foo groups from other groups*/

   freeDotSphere(&dotsphereset);
   dotSphere(&dotsphereset, fooradius, foodotdensity); /*MAGEFOO.c*/
#ifdef UNIX_X11
   fprintf(stderr,"%d dots per %.3f radius ball of density %.3f per sq A\n",dotsphereset.n,fooradius,foodotdensity);
#endif
   Lappend = 0;
   Linternal = 0;
   /*if(foogrupptr != NULL && pickedpointptr != NULL)*/
   if(foogrupptr != NULL) /*050816*/
   {/*first foo point has already been allocated*/
       thepointptr = foogrupptr->firstsgrpptr->firstlistptr->firstpointptr;

       if(pickedpointptr != NULL)
       {
          thepointptr->fx = pickedpointptr->fx;
          thepointptr->fy = pickedpointptr->fy;
          thepointptr->fz = pickedpointptr->fz;
       }
       else  /*050816*/
       {
          thepointptr->fx = 0;
          thepointptr->fy = 0;
          thepointptr->fz = 0;
       }
       thepointptr->radius = fooradius;  /*040402 set radius of first point*/
       /*each foo in cavity has its own point radius*/
       /* thus a small list radius allows the grid of foos to be seen*/
       /*foogrupptr->firstsgrpptr->firstlistptr->radius = fooradius;*/
       foogrupptr->firstsgrpptr->firstlistptr->radius = (float)0.1; /*040402*/
       /*shift L-key toggles list radius vs point radius! 040402*/

       Lcollision = allhitfoocheck( &(thepointptr->fx)
                 ,&(thepointptr->fy),&(thepointptr->fz),1); /*1 for first try*/
       if(Lcollision!=0) /*030630*/
       {/*can't put a foo here! */
#ifdef UNIX_X11
          if(Lcollision > 0)
             fprintf(stderr,"Too close to put a foo here!\n");
          else
             fprintf(stderr,"Too distant to put a foo here!\n");
#endif
          destroygrupstructure(foogrupptr);
       }
       else
       {
         if(!Ldrawstuff)
         { 
            setdrawstuff(32);/*MAGEMENU 00100000 only foo wake, foo surface*/
         }
         if(Firstpass)
         {
            Negamickfoocount = 1;
            Firstpass = 0; /*local static*/
         }
       }
      SetsinGrafWindow();
      rescalekinemage();
      redrawvec();
   }
 }
 ACTIVE_DLOG = 0;
}
/*___EndfoocavityDialog()____________________________________________________*/

/****DobackgroundDialog()*****************************************************/
void    DobackgroundDialog()
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
       backgroundDialog();        /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
      EndbackgroundDialog();     /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
  }
}
/*___DobackgroundDialog()_________________________________________________*/

/****backgroundDialog()****************************************************/
void    backgroundDialog()       /*BACKGND_DLOG*/
{
 sprintf(dlog_subjectstr,"BackGround and ForeGround Color indices.");
 Ldlog_subject = 1;
 
 sprintf(dlog_paramAstr,"%d",(int)BLACKINDEX);
 Ldlog_paramA = 1;
 sprintf(dlog_textAstr,"MAGE palette entry index for BLACKINDEX");
 Ldlog_textA = 1;
 
 sprintf(dlog_paramBstr,"%d",(int)WHITEINDEX);
 Ldlog_paramB = 1;
 sprintf(dlog_textBstr,"MAGE palette entry index for WHITEINDEX");
 Ldlog_textB = 1;
 
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 ACTIVE_DLOG = BACKGND_DLOG;
}
/*___backgroundDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndbackgroundDialog()*************************************************/
void    EndbackgroundDialog()
{
 int itestA,itestB;

 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_OKHIT) 
 {
      itestA = intfromstr(dlog_paramAstr);
      itestB = intfromstr(dlog_paramBstr);
      if(itestA >= 0 && itestA <= 255) BLACKINDEX = itestA;
      if(itestB >= 0 && itestB <= 255) WHITEINDEX = itestB;
 }
 ACTIVE_DLOG = 0;
}
/*___EndbackgroundDialog()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getcharstring(c,Lnew)****************************************************/
int  getcharstring(char c,int* Lnew) /*MAGEDLOG.h 030312*/
{
   int ireturn = 0;
   int j=0;

   if(*Lnew > 0)
   {
      itemps = 0; /*global*/
      for(j=0; j<256; j++) {temps[j]='\0';} /*global char[256]*/
      *Lnew = 0;
   }

   if(itemps==0 && (c==' ' || c==',') )
   {
      ;  /* skip beginning spaces */
   }
   else if(c==' ' || c=='\0' || c==EOL )
   {/* ends the string*/
      temps[itemps] = '\0';
      ireturn = 1;  /* some string was retrieved, could be NULL */
   }
   else
   {
      temps[itemps] = c;
      itemps++;
   }
   return(ireturn);
}
/*___getcharstring()_________________________________________________________*/

/****defaultdimnames()********************************************************/
void defaultdimnames(void) /*060622*/
{/*alloc and use default names*/
   int i=0;

   if(dimensionptr == NULL)
   {
      dimensionptr =
         (dimensionstruct*)calloc((size_t)MAXNDIM,sizeof(dimensionstruct));
      if(dimensionptr != NULL)
      {
         if(NDIM < 3) {NDIM = 3;}  /*safety 060622*/
         if(NDIM == 7)
         {/*use Rlab rna7D defaults*/
            strcpy(dimensionptr[0].name,"DELTA-1");
            strcpy(dimensionptr[1].name,"EPSILON-1");
            strcpy(dimensionptr[2].name,"ZETA-1");
            strcpy(dimensionptr[3].name,"ALPHA");
            strcpy(dimensionptr[4].name,"BETA");
            strcpy(dimensionptr[5].name,"GAMMA");
            strcpy(dimensionptr[6].name,"DELTA");
         }
         else
         {
            for(i=0; i<MAXNDIM; i++)
            {
               sprintf(dimensionptr[i].name,"AXIS %d",i+1);
            }
         }
         LNdimnames = 1;
      }
      else {LNdimnames = 0;}
   }
}
/*___defaultdimnames()_______________________________________________________*/

/****DodimensionDialog()******************************************************/
void    DodimensionDialog() /*LNdimensions axischoice was L7dimensions 060622*/
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
     if(dimensionptr == NULL) /*060715*/
     {/*alloc and use default names*/
        defaultdimnames(); /*success sets dimensionptr and LNdimnames */
     }

     if(dimensionptr != NULL) /*060715*/
     {
       dimensionDialog();           /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
       EnddimensionDialog();        /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
     }
  }
}
/*___DodimensionDialog()_____________________________________________________*/

/****dimensionDialog()********************************************************/
void    dimensionDialog()       /*DIMENSION_DLOG*/ /*LNdimensions axischoice*/
{
 int i=0,j=0; /*060619*/

   sprintf(dlog_subjectstr,"Choose axes assigned to X Y Z ");
   Ldlog_subject = 1; 

   sprintf(dlog_optionstr,"invert Y Z 360"); /*121212*/
   Ldlog_option = 1; /*121212*/

   /*dimname7ptr becomes dimnameNptr becomes dimensionptr 060622,061028,061121*/
   NlistboxAitems = NDIM; /*060619 7, 060622 NDIM*/
   listboxAitem = NX+1;
   /*061111 make listbox names work for alloc at runtime number of dimensions*/
   /* using, in effect, a 2D array.  Number of dim is known at this point*/
   /*this seems to require a lot of Mickey Mouse malloc-ing */
   listboxAitemName = (char**)malloc( (NDIM+1)*sizeof(char*) ); 
   if(listboxAitemName != NULL)
   {
      Ldlog_listboxA = 1;
      for(i=1; i<=NDIM; i++) 
      {
         listboxAitemName[i] = (char*)malloc(sizeof(char)*(MAXNAMECHAR+1));
         if(listboxAitemName[i] != NULL)
            {strcpy((char*)listboxAitemName[i],dimensionptr[i-1].name);}
      }
   }
   NlistboxBitems = NDIM;
   listboxBitem = NY+1;
   listboxBitemName = (char**)malloc( (NDIM+1)*sizeof(char*) ); 
   if(listboxBitemName != NULL)
   {
      Ldlog_listboxB = 1;
      for(i=1; i<=NDIM; i++) 
      {
         listboxBitemName[i] = (char*)malloc(sizeof(char)*(MAXNAMECHAR+1));
         if(listboxBitemName[i] != NULL)
            {strcpy((char*)listboxBitemName[i],dimensionptr[i-1].name);}
      }
   }

   NlistboxCitems = NDIM;
   listboxCitem = NZ+1;
   listboxCitemName = (char**)malloc( (NDIM+1)*sizeof(char*) ); 
   if(listboxCitemName != NULL)
   {
      Ldlog_listboxC = 1;
      for(i=1; i<=NDIM; i++) 
      {
         listboxCitemName[i] = (char*)malloc(sizeof(char)*(MAXNAMECHAR+1));
         if(listboxCitemName[i] != NULL)
            {strcpy((char*)listboxCitemName[i],dimensionptr[i-1].name);}
      }
   }
 
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/

   ACTIVE_DLOG = DIMENSION_DLOG;
}
/*___dimensionDialog()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EnddimensionDialog()*****************************************************/
void    EnddimensionDialog() /*LNdimensions axischoice*/
{
   int i=0;
   /*free the allocated names that were used this time 061111*/
   /* order of free() seems unimportant, all these items seems to be alloc-ed*/
   /*all these pass through malloc call, so either alloc or == NULL*/
   for(i=1; i<= NDIM; i++)
   {
      if(listboxAitemName[i] != NULL) {free(listboxAitemName[i]);}
      if(listboxBitemName[i] != NULL) {free(listboxBitemName[i]);}
      if(listboxCitemName[i] != NULL) {free(listboxCitemName[i]);}
   }
   if(listboxAitemName != NULL) {free(listboxAitemName);}
   if(listboxBitemName != NULL) {free(listboxBitemName);}
   if(listboxCitemName != NULL) {free(listboxCitemName);}

   if(Ldlog_cancelHIT)
   {
      ireturnflag = 0;   /*no action */
   }
   else if(Ldlog_optionHIT) /*121212*/
   {
      Linvert360yzon = !Linvert360yzon; /*121212*/
      Lreloadkinemage = 1; 
   }
   else if(Ldlog_OKHIT) 
   {

      NX = listboxAitem - 1; 
      strcpy(NXstr,dimensionptr[listboxAitem - 1].name); /*060622*/
      NY = listboxBitem - 1;
      strcpy(NYstr,dimensionptr[listboxBitem - 1].name);
      NZ = listboxCitem - 1;
      strcpy(NZstr,dimensionptr[listboxCitem - 1].name);

      sprintf(onscreeninfostr,"X: %s,  Y: %s,  Z: %s",NXstr,NYstr,NZstr);
      Lonscreeninfo = 1;
      Lonscreeninfoon = 1;

      Lreloadkinemage = 1; 
   }
   ACTIVE_DLOG = 0;
}
/*___EnddimensionDialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DoSELECTINGlistDialog()**************************************************/
void DoSELECTINGlistDialog() /*LNdimensions was L7dimensions 060622*/
{
  if(!ACTIVE_DLOG)
  {
       cleargenericflags();         /*MAGEDLOG.c*/
       SELECTINGlistDialog();       /*MAGEDLOG.c*/
       DogenericDialog();           /*____DLOG.c*/
#ifndef UNIX_X11 /*called after DLOG_OKHIT_CB*/
       EndSELECTINGlistDialog();    /*MAGEDLOG.c*/
       redrawvec();/*????????*/
#endif
  }
}
/*___DoSELECTINGlistDialog()_________________________________________________*/

/****SELECTINGlistDialog()****************************************************/
void SELECTINGlistDialog()  /*LNdimensions*/     /*SELECTING_DLOG*/
{
   sprintf(dlog_subjectstr, /*121110 subject box size width of text below it*/
    "Select points in SELECTINGKKIND list "
    CRLF" color is used to distinguish selected from not-selected"
    CRLF" operates only on points in lists with select parameter."
    CRLF"Select pts within a radius, OR just one pt:"
    CRLF" SHIFTclick deselects!");
   Ldlog_subject = 1;
   
   interpretcolor(ncolorSELECTING); /*returns color name in global word[]*/
   sprintf(dlog_paramAstr,"%s",word);
   Ldlog_paramA = 1;
   sprintf(dlog_textAstr,"selected points to be this color");
   Ldlog_textA = 1;
   
   interpretcolor(ncolornotSELECTING); /*returns color name in global word[]*/
   sprintf(dlog_paramBstr,"%s",word);
   Ldlog_paramB = 1;
   sprintf(dlog_textBstr,"common color for points not selected");
   Ldlog_textB = 1;

   sprintf(dlog_paramCstr,"%.2f",radiusSELECTING);
   Ldlog_paramC = 1;
   sprintf(dlog_textCstr,"radius of selection sphere ");
   Ldlog_textC = 1;

   dlog_checkAint = LNDparallelaxes; /*061114*/
   sprintf(dlog_checkAstr,"kludge: plot on parallel axes");
   Ldlog_checkA = 1;

   dlog_radioAint = 1;
   sprintf(dlog_radioAstr
      ,"just register the above values, take NO further action");
   Ldlog_radioA = 1;

   dlog_radioBint = 0;
   sprintf(dlog_radioBstr
      ,"show just the selected points with any select-color");
   Ldlog_radioB = 1;

   dlog_radioCint = 0;
   sprintf(dlog_radioCstr
      ,"show all points, no matter what color");
   Ldlog_radioC = 1;

   dlog_radioDint = 0;
   sprintf(dlog_radioDstr
      ,"show only the points with this select-color"); /*061029*/
   Ldlog_radioD = 1;

   dlog_radioEint = 0;
   sprintf(dlog_radioEstr
      ,"force NOT-selected points to have this common-color");
   Ldlog_radioE = 1;

   dlog_radioFint = 0;
   sprintf(dlog_radioFstr
      ,"force ALL points to have this common-color");
   Ldlog_radioF = 1;

 
   sprintf(dlog_infostr," ");/*filler so row of button boxes above have size*/
   Ldlog_info = 2; /*filler==2 060619*/
 
   ACTIVE_DLOG = SELECTING_DLOG;
}
/*___SELECTINGlistDialog()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****EndSELECTINGlistDialog()*************************************************/
void    EndSELECTINGlistDialog()  /*L7*/
{
 int  IOP = 0;

 if(Ldlog_cancelHIT)
 {
      ireturnflag = 0;   /*no action */
 }
 else if(Ldlog_OKHIT) 
 {
    ncolorSELECTING    = numberofcolor(dlog_paramAstr);
    ncolornotSELECTING = numberofcolor(dlog_paramBstr);
    radiusSELECTING    = floatfromstr(dlog_paramCstr);  

    LNDparallelaxes = dlog_checkAint;  /*061114*/
 
    if     (dlog_radioFint){IOP = 6;} /*force all points to common-color*/
    else if(dlog_radioEint){IOP = 5;} /*set un-selected points to common-color*/
    else if(dlog_radioDint){IOP = 4;} /*show points with this select-color*/
    else if(dlog_radioCint){IOP = 3;} /*show all points irespective of color*/
    else if(dlog_radioBint){IOP = 2;} /*show only points with any select-color*/
    else if(dlog_radioAint){IOP = 1;} /*nop*/
    if(IOP > 1) {adjustSELECTINGlist(IOP);}
 }
 ACTIVE_DLOG = 0;
}
/*___EndSELECTINGlistDialog()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****adjustSELECTINGlist()****************************************************/
void adjustSELECTINGlist(int IOP) /*060121 adjusts SELECTINGKKING list points*/
{/*case 7 now is actualSELECTINGtask()*/          /*061029*/             /*L7*/
    int  iptcolor=0;
    float distance=0;

    /*within a sphere*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*loop-over-groups*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*loop-over-sets*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*loop-over-lists*/
              if(thislistptr->KKIND & SELECTINGKKIND)/*selecting at list level*/
              {/*SELECTINGKKIND*/
                thispointptr = thislistptr->firstpointptr;
                while(thispointptr != NULL)
                {/*loop-over-points*/
                   /*all operations involved point color*/
                   iptcolor = ((thispointptr->colorwidth) & 31);
                   switch(IOP)
                   {
                      case 2: /*only show points with a select-color*/
                         if(  (iptcolor == ncolornotSELECTING)
                            ||(iptcolor == 0) )
                         {
                            thispointptr->STATUS = thispointptr->STATUS &~ON;
                         }
                         else
                         {
                            thispointptr->STATUS = thispointptr->STATUS | ON;
                         }
                      break;
                      case 3: /*show all points irespective of color*/
                         {
                            thispointptr->STATUS = thispointptr->STATUS | ON;
                         }
                      break;
                      case 4: /*only show points with this select-color 061029*/
                         if(iptcolor != ncolorSELECTING)
                         {
                            thispointptr->STATUS = thispointptr->STATUS &~ON;
                         }
                         else
                         {
                            thispointptr->STATUS = thispointptr->STATUS | ON;
                         }
                      break;
                      case 5: /*set un-selected points to common-color*/
                         if(iptcolor != ncolorSELECTING)
                         {
                            thispointptr->colorwidth = 
                                 thispointptr->colorwidth&~31;
                            thispointptr->colorwidth = 
                                 thispointptr->colorwidth | ncolornotSELECTING;
                         }
                      break;
                      case 6: /*force all points to common-color*/
                         {
                            thispointptr->colorwidth = 
                                 thispointptr->colorwidth&~31;
                            thispointptr->colorwidth = 
                                 thispointptr->colorwidth | ncolornotSELECTING;
                         }
                      break;
                      case 7: /*only show points with same color as picked pt*/
                        if(pickedpointptr != NULL)
                        {
                         if(iptcolor != (pickedpointptr->colorwidth & 31))
                         {
                            thispointptr->STATUS = thispointptr->STATUS &~ON;
                         }
                         else
                         {
                            thispointptr->STATUS = thispointptr->STATUS | ON;
                         }
                        }
                      break;
                   }
                   if(thispointptr == thislistptr->lastpointptr)
                       thispointptr=NULL;
                   else thispointptr = thispointptr->nextptr;
                }/*loop-over-points*/
              }/*SELECTINGKKIND*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*loop-over-lists*/
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
        }/*loop-over-sets*/
      thisgrupptr = thisgrupptr->nextptr;
    }/*loop-over-groups*/
    Lredrawvector = 1;
}
/*___adjustSELECTINGlist()___________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****actualSELECTINGtask()****************************************************/
void actualSELECTINGtask(void) /*061029 selects SELECTINGKKING list points*/
{                                 /*L7*/
  int  iptcolor=0;
  float distance=0;

  if(LoneSELECTINGon && (pickedpointptr->listptr->KKIND & SELECTINGKKIND))
  {
     pickedpointptr->colorwidth = 
        pickedpointptr->colorwidth&~31;
     if(Lshiftkey) /*060207*/
     {
        pickedpointptr->colorwidth = 
            pickedpointptr->colorwidth | ncolornotSELECTING;
     }
     else
     {
        pickedpointptr->colorwidth = 
            pickedpointptr->colorwidth | ncolorSELECTING;
     }
  }
  else
  {/*within a sphere*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*loop-over-groups*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*loop-over-sets*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*loop-over-lists*/
              if(thislistptr->KKIND & SELECTINGKKIND)/*selecting at list level*/
              {/*SELECTINGKKIND*/
                thispointptr = thislistptr->firstpointptr;
                while(thispointptr != NULL)
                {/*loop-over-points*/
                   /*all operations involved point color*/
                   iptcolor = ((thispointptr->colorwidth) & 31);
                   /*color all points within radius of picked point*/
                   if(    thisgrupptr->on && thissgrpptr->on  /*060613*/
                       && thislistptr->on && (thispointptr->STATUS & ON) )
                   {/*only select points that are ON   060203*/
                     distance = (float)sqrt(
                       (   ((thispointptr->fx) - (pickedpointptr->fx))
                         * ((thispointptr->fx) - (pickedpointptr->fx)) )
                      +(   ((thispointptr->fy) - (pickedpointptr->fy))
                         * ((thispointptr->fy) - (pickedpointptr->fy)) )
                      +(   ((thispointptr->fz) - (pickedpointptr->fz))
                         * ((thispointptr->fz) - (pickedpointptr->fz)) ));
                     if(distance <= radiusSELECTING)
                     {
                        thispointptr->colorwidth = 
                           thispointptr->colorwidth&~31;
                        if(Lshiftkey) /*060207*/
                        {
                          thispointptr->colorwidth = 
                           thispointptr->colorwidth | ncolornotSELECTING;
                        }
                        else
                        {
                          thispointptr->colorwidth = 
                           thispointptr->colorwidth | ncolorSELECTING;
                        }
                     }
                   }
                   if(thispointptr == thislistptr->lastpointptr)
                       thispointptr=NULL;
                   else thispointptr = thispointptr->nextptr;
                }/*loop-over-points*/
              }/*SELECTINGKKIND*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*loop-over-lists*/
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
        }/*loop-over-sets*/
      thisgrupptr = thisgrupptr->nextptr;
    }/*loop-over-groups*/
  }/*within a sphere*/
  Lredrawvector = 1;
}
/*___actualSELECTINGtask()___________________________________________________*/

/****movepointsbycolor()******************************************************/
void movepointsbycolor(liststruct* srclistptr, liststruct* trglistptr, int ptcolor)
{
   pointstruct* testpointptr=NULL;
   pointstruct* nextpointptr=NULL;

   if(   srclistptr != NULL && trglistptr != NULL 
      && srclistptr->NDIM == trglistptr->NDIM )
   {
      testpointptr = srclistptr->firstpointptr;      
      while(testpointptr != NULL)
      {/*while loop*/
         nextpointptr = testpointptr->nextptr; /*must remember who is next*/
         if( (testpointptr->colorwidth & 31) == ptcolor)
         {
            /*disconnect this point out of source list */
            /* list only knows first and last pointptr's */
            
            if(   (srclistptr->firstpointptr == testpointptr)
                ||(srclistptr->lastpointptr == testpointptr)  )
            {/*beginning and end are special cases*/

               if(srclistptr->firstpointptr == testpointptr)
               { /*at beginning*/
                  srclistptr->firstpointptr = testpointptr->nextptr;
                  if(testpointptr->nextptr != NULL)
                  { /*there still is at least one other point in the list*/
                     srclistptr->firstpointptr->previousptr = NULL;
                     /*still connected OK to the next point*/
                  }
                  else /*have just set srclistptr->firstpointptr = NULL */
                  { /* list is now empty */
                     srclistptr->lastpointptr = NULL;
                  }
               }
               if(srclistptr->lastpointptr == testpointptr)
               { /* at end */
                  srclistptr->lastpointptr = testpointptr->previousptr;
                  if(testpointptr->previousptr != NULL)
                  { /*there still is at least one other point in the list*/
                     srclistptr->lastpointptr->nextptr = NULL;
                     /*still connected OK to the previous point*/
                  }
                  else /*have just set srclistptr->lastpointptr = NULL */
                  { /* list is now empty */
                     srclistptr->firstpointptr = NULL;
                  }
               }
            }/*beginning and end are special cases*/
            else
            {/*in middle of src points, connect pts across point to be removed*/
               testpointptr->previousptr->nextptr = testpointptr->nextptr;
               testpointptr->nextptr->previousptr = testpointptr->previousptr;
            }/*in middle of src points*/

            /*connect this point into target list and reset its previous,next*/
            if(trglistptr->firstpointptr == NULL) /*first pt in this list*/
            {
               trglistptr->firstpointptr = testpointptr;
               trglistptr->lastpointptr  = testpointptr;
               testpointptr->previousptr = NULL;
               testpointptr->nextptr = NULL;
            }
            else /*add point to end of target list*/
            {
               trglistptr->lastpointptr->nextptr = testpointptr;
               testpointptr->previousptr = trglistptr->lastpointptr;
               testpointptr->nextptr = NULL; /*at end*/
               trglistptr->lastpointptr  = testpointptr;
            }
            testpointptr->listptr = trglistptr;
         }
         if(   (testpointptr == srclistptr->lastpointptr)
             ||(nextpointptr == NULL) )
              /*this works even if srclistptr->lastpointptr has been set NULL*/
              {testpointptr = NULL;}
         else {testpointptr = nextpointptr;}
      }/*while loop*/
   }
}
/*___movepointsbycolor()_____________________________________________________*/

/****maketargetlist()*********************************************************/
int  maketargetlist(sgrpstruct* trgsgrpptr, liststruct* srclistptr, char* name)
{
   /*alloc a new list, make it be the last list of a specific subgroup*/
   /*allocliststructure() presumes the list added to end of all lists*/
   /*that's may be OK for a new list in an empty subgroup */
   /*but the wrong hookup for an inserted list into a non-empty subgroup*/

   int ireturn=0;
   int n=0;
   int Lok = 0;
 
   if(trgsgrpptr->lastlistptr == NULL)
   {/*new list in an empty subgroup*/
      Lok = allocliststructure(); /*creates global thislistptr*/
   }
   else
   {
      Lok = allocinsertedliststructure(trgsgrpptr->lastlistptr);
         /*creates global thislistptr*/
   }
   if(Lok)
   {/*connect to trgsgrpptr, and set properties*/
      thislistptr->sgrpptr = trgsgrpptr;
/*
fprintf(stderr,"target thislistptr== %0x, in trgsgrpptr==%0x\n",thislistptr,trgsgrpptr);
*/
      if(trgsgrpptr->lastlistptr == NULL) /*first must be NULL also*/
      { /*thislistptr will be its first*/
         trgsgrpptr->firstlistptr = thislistptr;
         trgsgrpptr->lastlistptr = thislistptr;
      }
      else
      { /*connect this list to the list(s) already in trgsgrpptr*/
         trgsgrpptr->lastlistptr->nextptr = thislistptr; 
         thislistptr->previousptr = trgsgrpptr->lastlistptr;
         trgsgrpptr->lastlistptr = thislistptr; /* redefine */
      }
      /*horizontal gene transfer*/
      thislistptr->type  = srclistptr->type; 
      thislistptr->STATUS  = srclistptr->STATUS;
      thislistptr->STYLE  = srclistptr->STYLE;
      thislistptr->KKIND = srclistptr->KKIND; 
      thislistptr->fwrap  = srclistptr->fwrap;
      thislistptr->NDIM  = srclistptr->NDIM;
      thislistptr->on  = srclistptr->on;
      thislistptr->color  = srclistptr->color;
      thislistptr->width  = srclistptr->width;
      thislistptr->owncolor  = srclistptr->owncolor;
      thislistptr->colorset  = srclistptr->colorset;
      thislistptr->radius  = srclistptr->radius;
      thislistptr->angle  = srclistptr->angle;
      for(n=0;n<=MAXNAMECHAR;n++) {thislistptr->name[n] = name[n];}

      ireturn = 1;
   } 
   return(ireturn);
}
/*___maketargetlist()________________________________________________________*/

/****maketargetsgrp()*********************************************************/
int  maketargetsgrp(grupstruct* trggrupptr, sgrpstruct* srcsgrpptr, char* name)
{
   int ireturn=0, n=0;
 
   if(allocsgrpstructure()) /*creates global thissgrpptr*/
   {/*connect to trggrupptr, and set properties*/
      thissgrpptr->grupptr = trggrupptr;
/*
fprintf(stderr,"target ssgrpptr== %0x, in trggrupptr==%0x\n",thissgrpptr,trggrupptr);
*/
      if(trggrupptr->lastsgrpptr == NULL)  /*first must be NULL also*/
      { /*thissgrpptr will be its first*/
         trggrupptr->firstsgrpptr = thissgrpptr;
         trggrupptr->lastsgrpptr = thissgrpptr;
      }
      else
      {
         trggrupptr->lastsgrpptr->nextptr = thissgrpptr; /*connect this sgrp*/
         thissgrpptr->previousptr = trggrupptr->lastsgrpptr;
         trggrupptr->lastsgrpptr = thissgrpptr; /* redefine */
      }
      thissgrpptr->STATUS  = srcsgrpptr->STATUS;
      thissgrpptr->type  = srcsgrpptr->type;
      thissgrpptr->KKIND = srcsgrpptr->KKIND; /*horizontal gene transfer*/
      thissgrpptr->NDIM  = srcsgrpptr->NDIM;
      thissgrpptr->on  = srcsgrpptr->on;

      for(n=0;n<=MAXNAMECHAR;n++) {thissgrpptr->name[n] = name[n];}
      ireturn = 1;
   } 
   return(ireturn);
}
/*___maketargetsgrp()________________________________________________________*/

/****maketargetgrup()*********************************************************/
int  maketargetgrup(grupstruct* srcgrupptr, char* name)
{
   int ireturn=0, n=0;
 
   if(allocgrupstructure()) /*creates global thisgrupptr*/
   {/* set properties (alloc routine positions this in set of all groups*/
/*
fprintf(stderr,"target thisgrupptr== %0x\n",thisgrupptr);
*/     
      thisgrupptr->STATUS  = srcgrupptr->STATUS;
      thisgrupptr->type  = srcgrupptr->type;
      thisgrupptr->KKIND = srcgrupptr->KKIND; /*horizontal gene transfer*/
      thisgrupptr->NDIM  = srcgrupptr->NDIM;
      thisgrupptr->on  = srcgrupptr->on;

      for(n=0;n<=(MAXNAMECHAR+1);n++) {thisgrupptr->name[n] = name[n];}
      ireturn = 1;
   } 
   return(ireturn);
}
/*___maketargetgrup()________________________________________________________*/

/****calcmeansigmas()*********************************************************/
void calcmeansigmas(liststruct* srclistptr, liststruct* trglistptr) /*061126*/
{
   /* 2 passes: */
   /*first pass: accummulate sums for averages accumsums()*/
   /*  getmeans() */
   /*second pass: accummulate deviations from averages accumdevi()*/
   /*  getsigmas() */
   /*meansigmasout() */

   pointstruct* tryptptr = NULL;
   int ipass=0, i=0, j=0, k=0, Lok=1;
   double del=0;
   char onestr[256],twostr[256],newstr[256],ptIDstr[256];
   float pm=0;
   float* fxyzptr=NULL;

   meansNptr = alloccalcNstructure();
   if(meansNptr == NULL) 
   { 
       Lok=0; 
#ifdef UNIX_X11
       fprintf(stderr,"failed to allocate storage for averages\n"); 
#endif
   }
   else { Lok=1;}
   if(Lok)
   {
      sigmaNptr = alloccalcNstructure();
      if(sigmaNptr == NULL)
      { 
          Lok=0; 
#ifdef UNIX_X11
          fprintf(stderr,"failed to allocate storage for sigmas\n"); 
#endif
      }
      else { Lok=1;}
   }
   if(Lok)
   { /*OK to do passes*/
      { /* pass 1 */
         tryptptr = srclistptr->firstpointptr;
         while(tryptptr != NULL)
         {
           /*filter by color, ON status, punch state*/
           if(  ((tryptptr->colorwidth & 31) == sourcepointcolor)
               &&((tryptptr->STATUS & ON) == ON)
               &&((tryptptr->type & PRUNED) != PRUNED)  )
           { /*valid point*/
             if(NDIM == 3)
             {
                meansNptr->dimn[0] = meansNptr->dimn[0] + tryptptr->fx;
                meansNptr->dimn[1] = meansNptr->dimn[1] + tryptptr->fy;
                meansNptr->dimn[2] = meansNptr->dimn[2] + tryptptr->fz;
             }
             else if(NDIM > 0 && (tryptptr->coordNptr != NULL) )
             {
                for(i=0; i<NDIM; i++)
                {
                   meansNptr->dimn[i] = 
                             meansNptr->dimn[i] + tryptptr->coordNptr[i];
                }
             }
             meansNptr->number++;
 
           } /*valid point*/
           if(tryptptr == srclistptr->lastpointptr)
                {tryptptr = NULL;}
           else {tryptptr = tryptptr->nextptr;}
         }
         for(i=0; i<NDIM; i++) /*get means*/
         {
            meansNptr->dimn[i] = meansNptr->dimn[i] / meansNptr->number;
         }
      } /* pass 1 */
      { /* pass 2 */
         tryptptr = srclistptr->firstpointptr;
         while(tryptptr != NULL)
         {
           /*filter by color, ON status, punch state*/
           if(  ((tryptptr->colorwidth & 31) == sourcepointcolor)
               &&((tryptptr->STATUS & ON) == ON)
               &&((tryptptr->type & PRUNED) != PRUNED)  )
           { /*valid point*/
             if(NDIM == 3)
             {
                del = (tryptptr->fx) - meansNptr->dimn[0];
                sigmaNptr->dimn[0] = sigmaNptr->dimn[0] + del*del;
                del = (tryptptr->fy) - meansNptr->dimn[1];
                sigmaNptr->dimn[1] = sigmaNptr->dimn[1] + del*del;
                del = (tryptptr->fz) - meansNptr->dimn[2];
                sigmaNptr->dimn[2] = sigmaNptr->dimn[2] + del*del;
             }
             else if(NDIM > 0 && (tryptptr->coordNptr != NULL) )
             {
                for(i=0; i<NDIM; i++)
                {
                   del = (tryptptr->coordNptr[i]) - meansNptr->dimn[i];
                   sigmaNptr->dimn[i] = sigmaNptr->dimn[i] + del*del;
                }
             }
             sigmaNptr->number++;
           } /*valid point*/
 
           if(tryptptr == srclistptr->lastpointptr)
                {tryptptr = NULL;}
           else {tryptptr = tryptptr->nextptr;}
         }
         if(sigmaNptr->number > 1)
         {
            for(i=0; i<NDIM; i++)
            {/*sigmaNptr->dimn[] holds the accum sum of differences squared*/
               sigmaNptr->dimn[i] = 
                   sqrt( (sigmaNptr->dimn[i])/((sigmaNptr->number)-1) );
            }
         }
      } /* pass 2 */

      sprintf(word,"avsigma"); /*mastermaker uses the string in global word*/
      if(mastermaker(0))
      {
         trglistptr->master = trglistptr->master|thismasterptr->mask;
         trglistptr->nextptr->master = 
                     trglistptr->nextptr->master|thismasterptr->mask;
         SetsinGrafWindow(); /*on spec   061128*/
      }

      /*force KKIND for this list, should already be OK for mage made lists */
      trglistptr->KKIND = trglistptr->KKIND & ~SELECTINGKKIND;
      trglistptr->KKIND = trglistptr->KKIND | AVERAGEKKIND;
      trglistptr->nextptr->KKIND = trglistptr->nextptr->KKIND & ~SELECTINGKKIND;
      trglistptr->nextptr->KKIND = trglistptr->nextptr->KKIND | SIGMASKKIND;

      /*first list gets a point for a ring at mean position*/
      tryptptr = allocpointstructure(trglistptr);
      if(tryptptr != NULL)
      {
/*
fprintf(stderr,"averag tryptptr== %0x, trglistptr==%0x\n",tryptptr,trglistptr);
*/
         /*average (mean) point in a RINGLIST */
         tryptptr->type = MARKTYPE;
         tryptptr->STYLE = RINGSTYLE;
         tryptptr->radius = 10; /*average of sigmas was too wierd*/
         sprintf(ptIDstr,"%s %d pt",srclistptr->name,(int)meansNptr->number);
         storeptIDstring(ptIDstr, tryptptr); /*MAGELIST*/

         /*comment string of sigma values*/
         /*comment strings start with 3 characters for font,face,size*/
         onestr[0] = ' '; onestr[1] = ' '; onestr[2] = ' '; 
         onestr[3] = 's'; onestr[4] = 'i'; onestr[5] = 'g'; /*just sig 061216*/
         onestr[6] = 'm'; onestr[7] = 'a'; onestr[8] = 's'; onestr[9] = ' '; 
         onestr[10] = '\0'; 
         twostr[0] = '\0';
         newstr[0] = '\0';
         for(i=0; i<NDIM; i++)
         {
            sprintf(twostr,"%.1f ",sigmaNptr->dimn[i]); /*was .3 061216*/
            {
               sprintf(newstr,"%s%s",onestr,twostr);
               strcpy(onestr,newstr);
            }
         }
         /*onestr now is a mage-type commentstr*/
         storecommentstring(onestr, tryptptr);
         /*NOW store the coords of the mean position*/
         if(NDIM > 3)
         {            
           tryptptr->coordNptr 
              = (float*)calloc((size_t)(NDIM),sizeof(float));
           if(tryptptr->coordNptr != NULL)
           {
             for(i=0; i<NDIM; i++)
             { /* NDIM coords for this point */
                /*coordinates at the mean point*/
                tryptptr->coordNptr[i] = meansNptr->dimn[i];
             }
           }
         } /* NDIM > 3 */
         else
         { /* NDIM == 3 */
             for(i=0; i<3; i++)
             { /* 3 coords for this P point */
                if     (i==0) {fxyzptr = &(tryptptr->fx);}
                else if(i==1) {fxyzptr = &(tryptptr->fy);}
                else if(i==2) {fxyzptr = &(tryptptr->fz);}

                /*coordinates at the mean point*/
                {
                   *fxyzptr = meansNptr->dimn[i];
                }
             }
         }
      }
      {/*2nd list of - --> + sigma extents */
         /*the other points are vectors in a VECTORLIST*/
         for(j=0; j<NDIM; j++)
         { /*j: NDIM lines: sigmas in each dimension extend - --> +*/
            /*line - to + sigma in each dimension*/
            for(k=1; k<=2; k++)
            { k: /* P --- L line is 2 points */

              tryptptr = allocpointstructure(trglistptr->nextptr);
              if(tryptptr != NULL)
              { /* one line from - to + sigma */
/*
fprintf(stderr,"sigmas tryptptr== %0x, trglistptr->nextptr==%0x\n",tryptptr,trglistptr->nextptr);
*/
                 if(k==1) /*start P */
                 {
                    tryptptr->type = VECTOR | MOVETO_P | UNPICKABLE;
                    pm = -1;
                 }
                 else /* L */
                 {
                    tryptptr->type = VECTOR | UNPICKABLE; /*UNPICKABLE 061216*/
                    pm = +1;
                 } 
                 sprintf(ptIDstr,"%.3f 1sig in %s"
                   ,(pm*sigmaNptr->dimn[j]), dimensionptr[j].name);
                 storeptIDstring(ptIDstr, tryptptr); /*MAGELIST*/
    
                 if(NDIM > 3)
                 {            
                   tryptptr->coordNptr 
                      = (float*)calloc((size_t)(NDIM),sizeof(float));
                   if(tryptptr->coordNptr != NULL)
                   {
                     for(i=0; i<NDIM; i++)
                     { /* NDIM coords for this P point */
                        if(i==j) /*line is drawn in this direction*/
                        { /* i coords for jth vector*/
                           tryptptr->coordNptr[i] = 
                               ( meansNptr->dimn[i] + (pm*sigmaNptr->dimn[i]) );
                        }
                        else /*coordinates stay at the mean point*/
                        {
                           tryptptr->coordNptr[i] = meansNptr->dimn[i];
                        }
                     }
                   }
                 } /* NDIM > 3 */
                 else
                 { /* NDIM == 3 */
                     for(i=0; i<3; i++)
                     { /* 3 coords for this P point */
                        if     (i==0) {fxyzptr = &(tryptptr->fx);}
                        else if(i==1) {fxyzptr = &(tryptptr->fy);}
                        else if(i==2) {fxyzptr = &(tryptptr->fz);}

                        if(i==j) /*line is drawn in this direction*/
                        {
                         *fxyzptr =(meansNptr->dimn[i]+(pm*sigmaNptr->dimn[i]));
                        }
                        else /*coordinates stay at the mean point*/
                        {
                           *fxyzptr = meansNptr->dimn[i];
                        }
                     }
                 }
              } /* one line from - to + sigma */

            } /* k: P --- L line is 2 points */

         } /*j: NDIM lines: sigmas in each dimension extend - --> +*/
      }/*2nd list of - --> + sigma extents */
   } /*OK to do passes*/
}
/*___calcmeansigmas()________________________________________________________*/

