/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "MAGE.h"    
#include "MAGELIST.h"    
#include "MAGETABL.h"    
#include "MAGEBBOX.h" /*121108 for killgrafcontrols() */    


/*MUXMWIND.C*/

/****showallwindows()****************************************/
void    showallwindows()
{
        /* fprintf(stderr,"entered showallwindows()\n");*/
        /* XRaiseWindow(dpy,captwindow);*/
        /* XRaiseWindow(dpy,textwindow);*/
        /* XRaiseWindow(dpy,grafwindow);*/
    ;
}
/*___showallwindows()_______________________________________*/

/****choosefrontwindow()****************************************/
void    choosefrontwindow()
{
   /* fprintf(stderr,"entered choosefrontwindow()\n");*/
   /* if(Lnewfile)*/
   /*      XRaiseWindow(dpy,toptext);*/
   /* else*/
   /*      XRaiseWindow(dpy,topgraf);*/
}
/*___choosefrontwindow()_______________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****cleartexts()*************************************************************/
void    cleartexts() /*<-GetTextFromFile(),MAGEFILE*/
{
   if(LGUI)
   {/*LGUI*/
    XmTextReplace(textwindow,
                   0,
                   XmTextGetLastPosition(textwindow),
                   ""); /*990209*/
    maxtextchar = 0;
    maxtextcharcurrent = 0; /*030910*/
   }/*LGUI*/
}
/*___cleartexts()____________________________________________________________*/                                

/****inserttexts()************************************************************/
void    inserttexts(char texts[256]) /*<-GetTextFromFile(),MAGETEXT*/
{
  if(LGUI)
  {/*LGUI*/
     if(!Limittext)
     {
         maxtextchar = maxtextchar + (long)strlen(texts);
         maxtextcharcurrent = maxtextcharcurrent + (long)strlen(texts);
         /*if(maxtextcharcurrent >= 32766L)*//*leave 2 char for \n\0*//*030910*/
         if(maxtextcharcurrent >= MAXTEXT)  /*070617*/
         {/*MAGEINPT reset Limittext, maxtextcharcurrent if maxtextchar<65534*/
            Limittext = 1; 
            fprintf(stderr,"Current Text input approaching %d characters,\n"
                 "   no more text will be put into Text Window!\n",MAXTEXT);
         }
         else if(maxtextchar >= MAXTEXT) /*070622*/
         {
            Limittext = 1;
            fprintf(stderr,"Total Text approaching %d characters,\n"
                 "   no more text will be put into Text Window!\n",MAXTEXT);
         }
         else
         {
            XmTextInsert(textwindow,
                 XmTextGetLastPosition(textwindow),
                 texts);
            /*for(j=0;j<256;j++) texts[j] = '\0';*/
            /*texts[0] = '\0';*/ /*020904 this crashes call with constant*/
            /*????can see NO reason why inserttexts should null input str*/
            XmTextSetSelection(textwindow,0,0,0); /*990209*/
         }
     }
  }/*LGUI*/
}
/*___inserttexts()___________________________________________________________*/

/****completetext()*********************************************************/
void    completetext() /*<-GetTextFromFile(),MAGETEXT*/
{
   char texts[256];
   if(LGUI)
   {/*LGUI*/
                texts[0] = '\n';
                texts[1] = '\0';
                XmTextInsert(textwindow,
                    XmTextGetLastPosition(textwindow),
                    texts);
   /* put in trailing new line character to adjust text to Left edge*/
   /* of text window*/
   }/*LGUI*/
}
/*___completetext()________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****clearcapts()*************************************************************/
void    clearcapts()
{
   if(LGUI)
   {/*LGUI*/
    XmTextReplace(captwindow,
                   0,
                   XmTextGetLastPosition(captwindow),
                   ""); /*990209*/
   }/*LGUI*/
}
/*___clearcapts()____________________________________________________________*/                                

/****insertcapts()************************************************************/
void    insertcapts(char capts[256])
{
   if(LGUI)
   {/*LGUI*/
    XmTextInsert(captwindow,
                 XmTextGetLastPosition(captwindow),
                 capts);
    XmTextSetSelection(captwindow,0,0,0); /*990209*/
   }/*LGUI*/
}
/*___insertcapts()___________________________________________________________*/

/****docompletecapt()*********************************************************/
void    docompletecapt()
{
   if(LGUI)
   {/*LGUI*/
   char texts[256];
                texts[0] = '\n';
                texts[1] = '\0';
                XmTextInsert(captwindow,
                    XmTextGetLastPosition(captwindow),
                    texts);
   /* put in trailing new line character to adjust text to Left edge*/
   /* of capt window*/
   }/*LGUI*/
}
/*___docompletecapt()________________________________________________________*/

/****destroybondrotwindow()***************************************************/
void destroybondrotwindow()  
{/*destroy the bondrot window  031001 */
   int i=0;

   XtPopdown(topbrot); /*must popdown so next popup will recognize mouse*/
   for(i=1; i<=MAXBONDROT; i++)
   {/*scan over all possible bondrot structures*/ /*030927*/
      if(bondrotptr[i] != NULL)
      {/*only destroy those used*/ /*030927*/
         if(bondrotptr[i]->level > 0 || bondrotptr[i]->level== -1)
         {/*independent rotation or first of a ganged set of rotations*/
            erasebondrotvalue(i); /*dummied in UNIX*/
            /*removes any previous kinemage bond rotation text*/
            if(hgrafbondrot[i] != NULL)
            {
               XtDestroyWidget(hgrafbondrot[i]);
               hgrafbondrot[i] = NULL;
            }
            if(bondrotvalue[i] != NULL)
            {
               XtDestroyWidget(bondrotvalue[i]);
               bondrotvalue[i] = NULL;
            }
            /*XtDestroyWidget(bondrotstart[i]);  not in use */
            /*XtDestroyWidget(bondrotname[i]);   not in use */
         }
      }/*only destroy those used*/
   }/*scan over all possible bondrot structures*/
   XtDestroyWidget(topbrot);
   topbrot = NULL; /*020617 safety*/
   brotwinholder = NULL; /*020617 safety*/
   brotwindow = NULL; /*020617 safety*/
}
/*___destroybondrotwindow()__________________________________________________*/

/****adjustwindowwidth()*****************************************************/
void    adjustwindowwidth()
{      /* MAC and PC adjust window width for extra slider bars*/
       /* UNIX creates or destroys a separate window with extra slider bars*/
#define COLORMESS

  if(topbrot != NULL)
  {
     destroybondrotwindow();
  }

  /*docking scope sets isavewidth=0 even when adding to other bondrots*/
  /* so all sliders created including the docking ones*/
  /*if(RWIDTH > isavewidth) always destroybondrotwindow even for adjustments*/
  {/*create window when RWIDTH > isavewidth*/
    /*create topbrot as a popup shell under topLevel*/
    n = 0;
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "Bond Rotations"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, ROTWINDWIDTH); n++; 
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;
    XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    topbrot = XtCreatePopupShell("bondrot",
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);
    /*create brotwinholder as a scrolled window widget to hold brotwindow*/
    n = 0;  /* window title is a regular string */
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "BOND ROT"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, ROTWINDWIDTH); n++;
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;

    XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;

    brotwinholder =  /* create the brotwinholder */
       XtCreateManagedWidget("brotwinholder",xmScrolledWindowWidgetClass
                             ,topbrot, args, n);

    /*create brotwindow as a form widget to hold bond rot sliders*/
    n = 0;  /* window title is a regular string */
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "BOND ROT"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, ROTWINDWIDTH); n++;
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
    XtSetArg(args[n], XmNleftOffset, -16); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopOffset, 0); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightOffset, 0); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomOffset, +16); n++;

    brotwindow =         /* create the brotwindow for layout */
    XtCreateManagedWidget("brotwin",xmFormWidgetClass,brotwinholder, args, n);

    XtPopup(topbrot,XtGrabNone);
    XtRealizeWidget(topbrot);    /* realize widget hierarchy */

  }/*create window when RWIDTH > isavewidth*/

  isavewidth = RWIDTH;
}
/*___adjustwindowwidth()____________________________________________________*/

/****destroybuttonwindow()****************************************************/
void destroybuttonwindow()  
{/*destroy the button window  041108 */
   int i=0;

   XtPopdown(topbutt); /*must popdown so next popup will recognize mouse*/
  
  /*individual buttons belong to various groups, destroyed in killgrafcontrols*/
   XtDestroyWidget(topbutt);
   topbutt = NULL;       /*safety*/
   buttwinholder = NULL; /*safety*/
   buttwindow = NULL;    /*safety*/
}
/*___destroybuttonwindow()___________________________________________________*/

/****createbuttonwindow()*****************************************************/
void createbuttonwindow()
{ 
  /* UNIX option to create or destroy a separate scrolling window for button*/
#define COLORMESS

  if(topbutt != NULL)
  {
     killgrafcontrols();
     destroybuttonwindow();
  }

  {
    /*create topbutt as a popup shell under topLevel*/
    n = 0;
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "Button Panel"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, BUTTWINWIDTH); n++; 
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;
    XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    topbutt = XtCreatePopupShell("buttons",
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);
    /*create buttwinholder as a scrolled window widget to hold buttwindow*/
    n = 0;  /* window title is a regular string */
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "BUTTON PANEL"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, BUTTWINWIDTH); n++;
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;

    XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;

    buttwinholder =  /* create the buttwinholder */
       XtCreateManagedWidget("buttwinholder",xmScrolledWindowWidgetClass
                             ,topbutt, args, n);

    /*create buttwindow as a form widget to hold buttons*/
    n = 0;  /* window title is a regular string */
#ifdef COLORMESS
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
    XtSetArg(args[n], XmNtitle, "BUTTON PANEL"); n++;
    XtSetArg(args[n], XmNx, 10); n++;
    XtSetArg(args[n], XmNy, 10); n++;
    XtSetArg(args[n], XmNwidth, BUTTWINWIDTH); n++;
    XtSetArg(args[n], XmNheight,700); n++; 
    XtSetArg(args[n], XmNallowResize, True); n++;
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
    XtSetArg(args[n], XmNleftOffset, -16); n++;
    XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNtopOffset, 0); n++;
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNrightOffset, 0); n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetArg(args[n], XmNbottomOffset, +16); n++;

    buttwindow =         /* create the buttwindow for layout */
    XtCreateManagedWidget("buttwindow",xmFormWidgetClass,buttwinholder, args, n);

    XtPopup(topbutt,XtGrabNone);
    XtRealizeWidget(topbutt);    /* realize widget hierarchy */

  }

}
/*___createbuttonwindow()____________________________________________________*/

/****DotextContentClick()*****************************************************/
void DotextContentClick(long where)
{/*move of cursor within the text itself*/ /*981005*/
    /*UNIX where is already the index into the text character array*/
    long    icharhit,icharend,icharbegin,j,k,jth; 
    char*   mytextptr; 
   /* Time sel_time = NULL;  */ /*should be time of the event*/


    if(!Ltexteditable) 
    {/*NOT Ltexteditable*/
        mytextptr = XmTextGetString(textwindow);
        icharhit = where;
        icharbegin = -1;
        icharend = -1;
        jth = 0;
            
        for(j=0;j<(MAXHYPER-1);j++) /*031002,070622*/
        {/*look at earlier characters for hypertext beginning point*/
            if(   (char)(*(mytextptr+icharhit-j)) == '*'
                   && (char)(*(mytextptr+icharhit-j-1)) == '}' ) 
            {/*hit an end before hit a beginning*/
                 icharbegin = -1; /*restore pristine state*/
                 break;
            }
            if(   (char)(*(mytextptr+icharhit-j)) == '{'
                && (char)(*(mytextptr+icharhit-j-1)) == '*' ) 
            {
                 icharbegin = icharhit-j+1;
                 break;
            }
            jth = j; /*001006*/
        }/*look at earlier characters for hypertext beginning point*/
        if(icharbegin >= 0)
        {/*possible hypertext region*/
            for(j=-1;j<(MAXHYPER -1 -jth);j++) /*031002,070622*/
            {/*look at later characters for hypertext ending point*/
                 if(   (char)(*(mytextptr+icharhit+j)) == '}'
                    && (char)(*(mytextptr+icharhit+j+1)) == '*' ) 
                 {
                      icharend = icharhit+j-1;
                      break;
                 }
            }/*look at later characters for hypertext ending point*/
            if(icharend >= 0)
            {/*hypertext region*/
                 k = 0;
                 for(j=icharbegin; j<=icharend; j++)
                        Qstring[k++] = (char)(*(mytextptr+j));
                 Qstring[k] = '\0';
                 iQstring = k; /*global for Qstring[] char count*/
           /*XmTextSetSelection(textwindow,icharbegin,icharend+1, sel_time);*/
                 itextoffset = icharend+1; /*position just after question*/
                 Dotexthyperbutton(1); /* 1 for now, 0 when first setup */
            }/*hypertext region*/
        }/*possible hypertext region*/
    }/*NOT Ltexteditable*/
}/*move of cursor within the text itself*/ /*981005*/
/*____DotextContentClick()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DocaptContentClick()*****************************************************/
void DocaptContentClick(long where) /*040306*/
{/*move of cursor within the text itself*/
    long    icharhit,icharend,icharbegin,j,k,hyperplace; 
    char*   mytextptr; 
    char    Captstring[256];


    if(!Ltexteditable) 
    {/*NOT Ltexteditable*/
        mytextptr = XmTextGetString(captwindow);
        icharhit = where;
        icharbegin = -1;
        icharend = -1;
            
        for(j=0;j<(MAXHYPER-1);j++) /*031002,070622*/
        {/*look at earlier characters for hypertext beginning point*/
            if(   (char)(*(mytextptr+icharhit-j)) == '*'
                   && (char)(*(mytextptr+icharhit-j-1)) == '}' ) 
            {/*hit an end before hit a beginning*/
                 icharbegin = -1; /*restore pristine state*/
                 break;
            }
            if(   (char)(*(mytextptr+icharhit-j)) == '{'
                && (char)(*(mytextptr+icharhit-j-1)) == '*' ) 
            {
                 icharbegin = icharhit-j+1;
                 break;
            }
        }/*look at earlier characters for hypertext beginning point*/
        if(icharbegin >= 0)
        {/*possible hypertext region*/
            for(j = -1; j<256; j++) /*identifier limited to 256 char*/
            {/*look at later characters for hypertext ending point*/
                 if(   (char)(*(mytextptr+icharhit+j)) == '*'
                    && (char)(*(mytextptr+icharhit+j+1)) == '{' ) 
                 {/*hit another beginning before hit an end*/
                      icharend = -1; /*restore pristine state*/
                      icharbegin = -1; /*restore pristine state*/
                      break;
                 }
                 if(   (char)(*(mytextptr+icharhit+j)) == '}'
                    && (char)(*(mytextptr+icharhit+j+1)) == '*' ) 
                 {
                      icharend = icharhit+j-1;
                      break;
                 }
            }/*look at later characters for hypertext ending point*/
            if(icharend >= 0)
            {/*hypertext region*/
                 k = 0;
                 for(j=icharbegin; j<=icharend; j++)
                        Captstring[k++] = (char)(*(mytextptr+j));
                 Captstring[k] = '\0';
                 hyperplace = Dohypertextfind(Captstring); 
                 if(hyperplace > -1)
                 {/*place in textwindow's hypertext matches captiontext*/
                    /*report hypertextfind to info on screen NOT WORK 060622*/
                    /*sprintf(dstring,"hypertextfind: %s",Captstring);*/
                    /*special specialized suitefitroc bondrot patch 071215*/
                    
                    if(k > 2 && Captstring[0]=='3' && Captstring[1]=='\'')
                    {
                       strcpy(ribose3hyperstr,Captstring);
                    }
                    else if(k > 2 && Captstring[0]=='5' && Captstring[1]=='\'')
                    {
                       strcpy(ribose5hyperstr,Captstring);
                    }
                    else
                    {
                       strcpy(conformerstr,Captstring);
                    }

                    DotextContentClick(hyperplace);
                 }
                 else
                 {/*report failed hypertextfind to info on screen  NOT 060622*/
                    /*sprintf(dstring,"hypertextfind failed: %s",Captstring);*/
                    /*rescalekinemage();*/ /*MAGEINPT.c*/
                 }
            }/*hypertext region*/
        }/*possible hypertext region*/
    }/*NOT Ltexteditable*/
}/*move of cursor within the text itself*/ 
/*____DocaptContentClick()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****Dohypertextfind()********************************************************/
long Dohypertextfind(char Captstring[256])
{

   long    icharhit,icharend,j,k,where; 
   char*   mytextptr; 
   int     flowID=0,iCaptstring=0,Lprekey=1,ikeyw=0,Lmatch=0;
   char    wkeys[256];
   enum
   {
       neutID = 1
      ,keywID = 2
   };

   if(!Ltexteditable) 
   {/*NOT Ltexteditable*/
      iCaptstring = strlen(Captstring);
      mytextptr = XmTextGetString(textwindow);
      icharhit = -1; /*nothing found, yet*/
      where = -1; /*nothing found, yet*/
      /*icharend = XmTextGetMaxLength(textwindow); maximum size possible*/
      icharend = XmTextGetLastPosition(textwindow); /*index of last char060622*/
      
      flowID = neutID;           
      /*begin one past first char index allows back test for "*{" */
      /*for(j=1; (long)(mytextptr+j)<=icharend; j++)*/
      for(j=1; j<=icharend; j++)
      {/*scan through textwindow text for identified hypertext*/
         switch(flowID)
         {/*switch*/
            case neutID: /*looking for a hypertext field*/      
               if(   (char)(*(mytextptr+j)) == '{'
                  && (char)(*(mytextptr+j-1)) == '*' )
               {/*test back one char: (ok for pathelogical nested hypertext?)*/
                  flowID = keywID;
                  Lprekey = 1; /*ignore leading blanks*/
               }
            break;

            case keywID: /*looking in a hypertext field*/
               c = (char)(*(mytextptr+j));
               /* skip blank spaces, or tabs, in the beginning */
               /* Lprekey set = 1 where keywID set */
               if((Lprekey && (c == ' ')) || (Lprekey && (c == '\t')))
               {
                  ;  /* skip white space */
               }
               else
               {/*non-blank character*/
                  if(Lprekey)
                  {/*first non-blank character of a putative keyword*/
                     Lprekey = 0;  /* hit a non-blank character */
                     ikeyw = 0;
                     icharhit = j; /*hit point for hypertext field*/
                  }
                  else
                  {
                     ikeyw++; /* ikeyw set = -1 when flowID set = keywID */
                  }
                  wkeys[ikeyw] = c; /*case sensitive! */

                  if(  (c == ' ')||(c =='\015')||(c == ',')||(c == ';')
                     ||(c =='\t')||(c =='\n')||(c == '?')||(ikeyw >= 21)
                     ||(c =='\0')||(c =='=') ) 
                  {/*stop looking when hit white space or 20 char max*/
                     /* pad ends so can treat wkeys as a real string */
                     wkeys[ikeyw] = '\0';
                     /* try to match keyword string with search string*/
                     if(ikeyw == iCaptstring)
                     {/*at least character count matches*/
                        Lmatch = 1;
                        for(k=0; k<iCaptstring; k++)
                        {
                           if(wkeys[k] != Captstring[k])
                           {
                              Lmatch = 0;
                              break;
                           }
                        }
                     }
                     else
                     {
                        Lmatch = 0;
                     }
                     flowID = neutID;
                  }/*stop looking when hit white space or 20 char max*/
               }/*non-blank character*/
            break;  /*looking in a hypertext field*/
        }/*switch*/
        if(Lmatch) {break;}
      }/*scan through textwindow text for identified hypertext*/
   }/*NOT Ltexteditable*/
   if(Lmatch){where = icharhit;}
   else {where = -1;} /*nothing found*/
   return(where); /*simulate where hypertext field was hit*/
}
/*____Dohypertextfind()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

