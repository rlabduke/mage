/*MUXMBBOX.c*/
#include "MAGE.h"  
#include "MAGELIST.h"
#include "MAGEBBOX.h"

/****testbutton()************************************************************/
int    testbutton(ControlHandle it, ControlHandle button)
{
    if( it == button ) return(1);

    else return(0);
}
/*___testbutton()___________________________________________________________*/
/****buttonboxcheck()***********************common name: MAC & PC & MUX ***/
void    buttonboxcheck( ControlHandle button,int itch)
{
          if(button != 0) /*box had been created */

     XmToggleButtonSetState(button,itch,0); /* x mark for itch*/
}
/*___buttonboxcheck()_____________________________________________________*/
          
/****buttonscallback()******************************************************/
void buttonscallback(Widget w, XtPointer clientData, XtPointer callData)
{/*should be a message from a checkbox*/
 /*general callback routine for all check box gadgets*/
    int ion;

    XmToggleButtonCallbackStruct *cbs = 
                (XmToggleButtonCallbackStruct * ) callData;
    
    /*Mac Mage button stuff is in DografContentClick() in macmain.c*/ 
    /*PC  Mage button stuff is in MainWndProc() in mpcmain.c*/ 
    /*PC and MUXM toggle buttons change their own state, Mac's don't */
         
    /*first: get the current toggle state of whoever got clicked */
    ion = cbs->set;
    
    /*during the scan through all boxes, unset those that never show X */
          
    /* beware: check boxes actually defined even when not created,*/
    /* so search by same hierarchy that created them */
          
    buttonboxscan(w, ion);  /*MAGEBBOX.c*/

#ifdef UNIX_PEX
    if(w == buttons[zclipbox])
    {
          setpexclipplanes();  /*MUXPHIG.c*/
    }
    updatevisibility();
#endif /*UNIX_PEX*/

    redrawvec(); /*121108 only action of dummy redrawgraphics() MUXMDRAW.c*/

}/*should be a message from a checkbox*/
/*___buttonscallback______________________________________________________*/

/****makebuttonbox()****************common name: MAC, PC, ESV**************/
void makebuttonbox(Widget *buttonbox,int left,int top,int right,int bottom
      ,char name[MAXNAMECHAR+2],int onoff,int nbox,int nn,int index)
      /*041026 name was 13*/
{
    XFontStruct *localbuttonfont = NULL;
    XmFontList   localfontList = NULL;
    XmString     xmstringname;
    Dimension    namelength;
    char localname[MAXNAMECHAR+2]; /*041026 match group name length*/

    strcpy(localname,name); /*041026 mess with localname not actual name*/


    /*    &grpbox[ngrp],             */ /*HANDLE: not used */
    /*&(thisgrupptr->button)*/ /*Handle: needed by Mac && X11, not used by PC*/
    /*    brect->left,               */ /*int left:   x */
    /*    i*fontverticalspace+3,     */ /*int top:    y */
    /*    35+longeststring*fontwidth,*/ /*int right:  width in pixels */
    /*    fontverticalspace,         */ /*int bottom: height in pixels */
    /*    groupname[i],              */ /*char name:  name */
    /*    igrp[ngrp][0],             */ /*int onoff:  flag */
    /*    nbox                       */ /*int      :  number of button */
    /*    nn                         */ /*int      :  height in pixels */
    /*    index                      */ /*int      :  index of button */
/*121108 many places  asssignment = seemed to be used for condition == */
/*121108 So maybe explainwhy font selection has always been wonky*/
/*121108 added many extra () in logic, presuming NOT overload assignment*/
/*121108 HOWEVER that looks bad, maybe did mean assigment as query, revert! */
/*121108 i.e. testing if (localbuttonfont) can be set to a specified font, */
/*121108 thus testing existance of specified font on current system */
/*121108 This works better, use () to relieve warnings */
/*
if(Ltest)
{sprintf(alertstr,"MUXMBBOX/makebuttonbox() make buttonbox for %s\n",name);
 dosinglealert(3);}
*/
/*instead of, e.g., XLoadQueryFont(dpy, "-*-fixed--r-normal--8*")*/
/*see /usr/X11R6/lib/X11/fonts/misc for a listing of font sizes*/
/*cannot name by the .gz form, but fonts.alias & fonts.dir reference those*/
/*see ...misc/ , 100dpi/ , 75dpi */
/*Xlib Programming Manual vol 1, page 444*/
/*xlsfonts    gives list of everything in font path: misc,100dpi,75dpi,?  */
/*naming by font and wild cards seems the best bet...*/
/*naming by size seems to work for simple fonts, e.g. "5x7", but no "4x6"*/
/*presume simple "5x7" definitions more robust than "schumacher..." */
/*but larger simple ones have serifs */
/*but misc/ ones like schumacher denigrated? */
/*try to use size smaller when next larger gets too crowded, i.e. at xn+4*/

 if(!Lbuttonseparate)
 {/*NOT Lbuttonseparate*/
  if((nn < 12) && (localbuttonfont = XLoadQueryFont(dpy, 
    "*schumacher-clean-medium-r-*-60*") ) )
  {/*4x6:just readable as separate lines at 10 pixels, barely ok at 9 pixels*/
      /*found in /usr/X11R6/lib/X11/fonts/misc/fonts.dir*/
      if(Lfontinfo && (nbox==1))
      { printf("Tiny buttons: %d pixels: font "
   "*schumacher-clean-medium-r-*-60*\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 12) && (localbuttonfont = XLoadQueryFont(dpy, "5x7") ) )
  {
      if(Lfontinfo && (nbox==1))
      { printf("Tiny buttons: %d pixels: font 5x7\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 13) && (localbuttonfont = XLoadQueryFont(dpy, 
    "*schumacher-clean-medium-r-*-60*") ) )
  {/*orig -- 5x7*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font "
   "*schumacher-clean-medium-r-*-60*\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 13) && (localbuttonfont = XLoadQueryFont(dpy, "5x7") ) )
  {/*orig -- 5x7*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font 5x7\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 14) && (localbuttonfont = XLoadQueryFont(dpy, "6x9") ) )
  {
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font 6x9\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 16) && (localbuttonfont = XLoadQueryFont(dpy, 
    "*helvetica-medium-r-*-80*") ) )
  {/*orig -- 5x7*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font "
  "*helvetica-medium-r-*-80*\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 16) && (localbuttonfont = XLoadQueryFont(dpy, "6x10") ) )
  {/*orig -- 5x7*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font 6x10\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 17) && (localbuttonfont = XLoadQueryFont(dpy, 
    "*helvetica-bold-r-*-100*") ) )
  {/*orig -- 7x13*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font "
  "*helvetica-bold-r-*-100*\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 17) && (localbuttonfont = XLoadQueryFont(dpy, "6x12") ) )
  {/*orig -- 7x13*/
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font 6x12\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 18) && (localbuttonfont = XLoadQueryFont(dpy, 
        "*helvetica-bold-r-*-100*") ) )
  {
      if(Lfontinfo && (nbox==1))
      { printf("medium buttons: %d pixels: try  "
        "*helvetica-bold-r-*-100*\n",nn);}
      localfontList =
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 18) && (localbuttonfont = XLoadQueryFont(dpy, "7x13") ) )
  {/*orig else -- buttonfont bold12 small screen, 14 if screen>800 */
      if(Lfontinfo && (nbox==1))
      { printf("Small buttons: %d pixels: font 7x13\n",nn);}
      localfontList = 
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 19) && (localbuttonfont = XLoadQueryFont(dpy, 
        "*helvetica-bold-r-*-100*") ) )
  {
      if(Lfontinfo && (nbox==1))
      { printf("medium buttons: %d pixels: try  "
        "*helvetica-bold-r-*-100*\n",nn);}
      localfontList =
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 19) && (localbuttonfont = XLoadQueryFont(dpy, "7x14") ) )
  {
      if(Lfontinfo && (nbox==1))
      { printf("medium buttons: %d pixels: font 7x14\n",nn);}
      localfontList =
          XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 22) && (localbuttonfont = XLoadQueryFont(dpy, 
           "*helvetica-bold-r-*-120*") ) )
  {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
               "*helvetica-bold-r-*-120*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 22) && (localbuttonfont = XLoadQueryFont(dpy, "9x15") ) )
  {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try using 9x15 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if((nn < 28) && (localbuttonfont = XLoadQueryFont(dpy, 
           "*helvetica-bold-r-*-140*") ) )
  {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
              "*helvetica-bold-r-*-140*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 28) && (localbuttonfont = XLoadQueryFont(dpy, "9x18") ) )
  {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try using 9x18 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 32) && (localbuttonfont = XLoadQueryFont(dpy, 
       "*helvetica-bold-r-*-180*") ) )
  {
     if(Lfontinfo && (nbox==1))
     { printf("Enlarge Buttons: %d pixels: try "
       "*helvetica-bold-r-*-180*\n",nn);}
     localfontList =
         XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if((nn < 32) && (localbuttonfont = XLoadQueryFont(dpy, "10x20") ) )
  {
     if(Lfontinfo && (nbox==1))
     { printf("Enlarge Buttons: %d pixels: try using 10x20 font \n",nn);}
     localfontList =
         XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }

  else if(Lmacularfont || (Lforcedfont==4))
  {/*Lmacularfont try for even larger size font*/

        /*top out at this font size*/
        if((localbuttonfont = XLoadQueryFont(dpy,"*helvetica-bold-r-*-240*")) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
             "*helvetica-bold-r-*-240*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
        else if((localbuttonfont = XLoadQueryFont(dpy,
             "-*-Helvetica-Bold-R-Normal--24*") ) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: macular default "
             "-*-Helvetica-Bold-R-Normal--24*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
        else if((localbuttonfont = XLoadQueryFont(dpy, "12x24") ) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: macular default 12x24 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
  }/*Lmacularfont*/
  else 
  {/*top out at this font size */
        if((localbuttonfont = XLoadQueryFont(dpy,"*helvetica-bold-r-*-140*" )))
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
              "*helvetica-bold-r-*-140*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
        else if((localbuttonfont = XLoadQueryFont(dpy, "9x18") ))
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try using 9x18 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
  }/*top out at this font size */
 }/*NOT Lbuttonseparate*/
 else
 {/*Lbuttonseparate separate scrolling window for buttons  041108*/
  if(Lmacularfont || (Lforcedfont==4))
  {/*Lmacularfont try for even larger size font*/
        nn = 32;
        /*top out at this font size*/
        if((localbuttonfont = XLoadQueryFont(dpy,"*helvetica-bold-r-*-240*")) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
             "*helvetica-bold-r-*-240*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
        else if((localbuttonfont = XLoadQueryFont(dpy,
             "-*-Helvetica-Bold-R-Normal--24*") ) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: macular default "
             "-*-Helvetica-Bold-R-Normal--24*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
        else if((localbuttonfont = XLoadQueryFont(dpy, "12x24") ) )
        {
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: macular default 12x24 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
        }
  }/*Lmacularfont*/
  else if((localbuttonfont = XLoadQueryFont(dpy,"*helvetica-bold-r-*-120*") ) )
  {
           nn = 22;
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try "
               "*helvetica-bold-r-*-120*\n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else if( (localbuttonfont = XLoadQueryFont(dpy, "9x15") ) )
  {
           nn = 22;
           if(Lfontinfo && (nbox==1))
           { printf("Enlarge Buttons: %d pixels: try using 9x15 font \n",nn);}
           localfontList =
               XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
  }
  else
  {
     nn = 22; /*force vertical separation for whatever font comes up*/
  }
 }/*Lbuttonseparate separate scrolling window for buttons*/

  if(localbuttonfont == NULL)
  {/*try different set of fonts*/
      if((localbuttonfont = XLoadQueryFont(dpy, 
       "-schumacher-clean-medium-r-normal--14-140-75-75-c-70-iso646.1991-irv")))
      {
         if(Lfontinfo && (nbox==1))
         { printf("medium buttons: %d pixels: font (7x14) "
  "-schumacher-clean-medium-r-normal--14-140-75-75-c-70-iso646.1991-irv\n",nn);}
         localfontList =
            XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
     }
     else if((localbuttonfont = XLoadQueryFont(dpy, "7x14") ) )
     {
        if(Lfontinfo && (nbox==1))
        { printf("medium buttons: %d pixels: font 7x14\n",nn);}
        localfontList =
            XmFontListCreate(localbuttonfont, XmSTRING_DEFAULT_CHARSET);
     }
     else
     {
         if(Lfontinfo && (nbox==1))
         { printf("Regular buttons: %d pixels: using buttonfont \n",nn);}
         localfontList   = buttonfontList;
     }
  }

  
    /*longeststring = 12;*/
    /* buttonbox = buttonbox;*/   /*avoid not used warning */
    if(Lbuttonseparate) /*041108*/
    {
       left = left; /*offset from left side of button window*/
    }
    else
    {
       left = -BWIDTH -ZWIDTH +left; /*offset from drawArea */
    }
    /*top = nn*nbox+MENUHEIGHT;*/ /*offset from form grafwindow top */
    /*top = nn*(nbox-1)+4+MENUHEIGHT;*/ /*offset from form grafwindow top */
    if(Lbuttonseparate) /*041108*/
    {
       top = (nn-2)*(nbox-1)+(nn/2); /*offset from form buttwindow top*/
    }
    else
    {
       top=(nn-2)*(nbox-1)+(nn/2)+MENUHEIGHT;/*offset from form grafwindow top*/
    }
    /* MENUHEIGHT TERM ALLOWS FOR THE MENU BAR, 4 ALLOWS FOR MENUBAR BORDER */
    if(Lbuttonseparate) /*041108*/
    {
       right = 0; /*offset from right edge of buttwindow*/
    }
    else
    {
       right = ZWIDTH; /*offset from form grafwindow right edge*/
    }
    /*bottom = GHEIGHT+MENUHEIGHT+MESSHEIGHT-(top + nn);*/
    /*offset from form grafwindow bottom */
    /*BUT ATTACH BUTTON BOTTOM TO THE TOP OF THE FORM*/

  n = 0;
  /*left offset changes to distinguish groups,subgroups,lists*/

  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  /*XtSetArg(args[n], XmNborderColor, windowborder); n++;*/
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
  XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
  XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;


  XtSetArg(args[n], XmNborderWidth, 0); n++;

  XtSetArg(args[n], XmNmarginWidth, 0); n++;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  XtSetArg(args[n], XmNmarginTop, 0); n++;
  XtSetArg(args[n], XmNmarginBottom, 0); n++;
  XtSetArg(args[n], XmNshadowThickness, 0); n++;
  XtSetArg(args[n], XmNrecomputeSize, False); n++;

/*remove direct reference to drawArea to avoid expose events there*/
/*  XtSetArg(args[n], XmNleftAttachment, XmATTACH_WIDGET); n++;*/
/*  XtSetArg(args[n], XmNleftWidget, drawArea); n++;           */
/*  XtSetArg(args[n], XmNleftOffset, -BWIDTH -ZWIDTH +left); n++;*/

  if(Lbuttonseparate) /*041108*/
  {
     XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  }
  else
  {
     XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
  }
  XtSetArg(args[n], XmNleftOffset, left); n++; 

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;

/*seems that inherent size can cause problems, let slop to right */
/*  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;*/
/*  XtSetArg(args[n], XmNrightOffset, right); n++;*/

/*let each button region slop down by font size, cut off by lower one*/
/*  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;*/
/*  XtSetArg(args[n], XmNbottomOffset, -top-nn-6); n++;*/
/*  XtSetArg(args[n], XmNbottomOffset, -top-nn); n++;*/
/*  XtSetArg(args[n], XmNheight, nn); n++;*/ /*DOES NOT WORK*/

/*  XtSetArg(args[n], XmNselectColor, windowborder  ); n++;*/
  XtSetArg(args[n], XmNselectColor, selectcolor); n++;
  XtSetArg(args[n], XmNfillOnSelect, 1 ); n++;

  if (localfontList && !Lbuttonseparate)
  { /*060613 do not mess with name in separate button window*/ 
     XtSetArg(args[n], XmNfontList, localfontList); n++;
     /*041026 mess with localname NOT with actual group/.../master... names*/
     do
     { 
        xmstringname = XmStringCreate(localname, XmSTRING_DEFAULT_CHARSET);
        /*(XmString) explicit cast gets error in irix: */
        /*   Expression must be a modifiable lvalue   */
        namelength = XmStringWidth(localfontList, xmstringname);
     /*fprintf(stderr,"namelength== %5d, in BWIDTH== %d\n",namelength,BWIDTH);*/
        if(namelength>(BWIDTH-30))
        {
           index = strlen(localname);
           if(index>0){localname[index-1] = '\0';} /*truncate localname*/
        }
     }while(namelength>(BWIDTH-30));
  }


  XtSetArg(args[n], XmNlabelString,
        XmStringCreate(localname, XmSTRING_DEFAULT_CHARSET)); n++;

  if(Lbuttonseparate) /*041108*/
  {
     (*buttonbox) = XtCreateWidget(localname, xmToggleButtonWidgetClass,
                                     buttwindow, args, n);  /*041108*/
  }
  else
  {
     (*buttonbox) = XtCreateWidget(localname, xmToggleButtonWidgetClass,
                                     grafwindow, args, n);
  }
  /* add an activation callback */
  XtAddCallback(*buttonbox,XmNvalueChangedCallback,buttonscallback,NULL);
  XtManageChild(*buttonbox);

  XmToggleButtonSetState(*buttonbox,onoff,0);

}
/*___makebuttonbox()________________________________________________________*/

/****clearbuttonbox()*********************************************************/
void clearbuttonbox(Widget *buttbox)
{
   XtUnmanageChild(*buttbox);
}
/*___clearbuttonbox()________________________________________________________*/

/***mybuttonspacer()************************common name: MAC, PC, ESV********/
void   mybuttonspacer(int left,int top ,char name[13]) /*macbbox.c mpcbbox.c*/
{
    ;
}
/*___mybuttonspacer()_______________________________________________________*/

/****getbuttonplace()*920605 dcr**************common name: MAC, PC, ESV******/
void   getbuttonplace()
{
    /*Since Motif can set toggle buttons crt drawArea edge, does not need to*/
    /*allow for sliders to right of buttons & button box,name,insets*/
    /* top is 0 for the Form Widget the buttons are in */
    /* bottom is the Widget height! See MUXMINIT.c for grafwindow*/

    brect.left = 0;
    brect.right = 0;
    /*brect.top =   0;*/
    brect.top =   4; /*ALLOWANCE FOR MENUBAR BORDER*/
    brect.bottom = GHEIGHT+MENUHEIGHT+MESSHEIGHT;
    /*brect.bottom = GHEIGHT+MENUHEIGHT+MESSHEIGHT;*/
    /* the MENUBAR + artifactual PEX MESSAGE BOX at bottom*/
}
/*___getbuttonplace()_______________________________________________________*/


/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****placewindowresizemark()**************************************************/
void    placewindowresizemark()
{
   ;
}
/*___placewindowresizemark()_____________________________________________*/

/****destroybuttons()******************************************************/
void destroybuttons(ControlHandle it)
{
    if (it != 0) 
    {
        XtUnmanageChild(it);
        XtDestroyWidget(it);
    }
}
/*___destroybuttons()_____________________________________________________*/


