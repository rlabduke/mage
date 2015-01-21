/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "MAGE.h"
#include "MAGEMENU.h"
#include "MAGELIST.h"
#include "MAGETABL.h"
#include <Xm/ScrolledW.h>
/****SetUptableWindow()*******************************************************/
void SetUptableWindow(void)
{
  /*int ncommon;*/
#define COLORMESS
/*printf("SetUptableWindow() entering\n");*/

/*create toptable as a topLevel shell with mage topLevel as parent*/
  n = 0;
#ifdef COLORMESS
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
  XtSetArg(args[n], XmNtitle, "TABLE: MAGE"); n++; /*title is a regular string*/
  XtSetArg(args[n], XmNx, 0); n++;
  XtSetArg(args[n], XmNy, 600); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  XtSetArg(args[n], XmNallowResize, True); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; /*use quit_CB*/

  toptable = XtCreatePopupShell(NULL,
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);

/*create tableWindow as a main widget to hold drawArea, and Scroll bars*/
  n = 0;  /* window title is a regular string */
#ifdef COLORMESS
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
  XtSetArg(args[n], XmNtitle, "TABLE text"); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  /*XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;*/
  XtSetArg(args[n], XmNscrollingPolicy, XmAPPLICATION_DEFINED); n++;

  tableWindow = 
    XtCreateManagedWidget("table",xmScrolledWindowWidgetClass,toptable, args, n);

  XtPopup(toptable,XtGrabNone);
  XtRealizeWidget(toptable);    /* realize widget hierarchy */

/*create a drawing area in which to put the table cells*/
  n = 0;
#ifdef COLORMESS
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
#endif /*COLORMESS*/
  XtSetArg(args[n], XmNtitle, "TABLE"); n++; /*title is a regular string*/
  XtSetArg(args[n], XmNx, 0); n++;
  XtSetArg(args[n], XmNy, 600); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 200); n++;

  tabledrawArea = XtCreateWidget("tabledrawArea", xmDrawingAreaWidgetClass,
				   tableWindow, args, n);

/* add exposure and input callbacks */
  XtAddCallback(tabledrawArea,XmNinputCallback,(XtCallbackProc)tablepick_CB,NULL); 
                                                        /*MUX_MAIN.c*/
  XtAddCallback(tabledrawArea,XmNexposeCallback,(XtCallbackProc)tabledrawArea_CB,NULL);
                                                            /*MUX_MENU.c*/
  XtAddCallback(tabledrawArea, XmNresizeCallback,(XtCallbackProc)tableresize_CB, NULL);

  XtManageChild(tabledrawArea);



   tabledrawWindow = XtWindow(tabledrawArea); 
   tabledpy = XtDisplay(tabledrawArea); /*in our case: tabledpy == dpy*/
      /*get the table d.a. widget's window ID*/
   tablegc = XCreateGC(tabledpy,tabledrawWindow,0L,NULL);

   XSetFont(tabledpy,tablegc,textfont->fid);
   XSetForeground(tabledpy,tablegc,WhitePixel(dpy,DefaultScreen(dpy)));
   XSetBackground(tabledpy,tablegc,BlackPixel(dpy,DefaultScreen(dpy)));


  XtRealizeWidget(topLevel);    /* realize widget hierarchy */
/*printf("SetUptableWindow() leaving\n");*/
}
/*___SetUptableWindow()_______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*table scroll CONTROLS */
/****createtablescrollbars()**************************************************/
void createtablescrollbars()
{
    /*char    name[13];*/
    int     left,top,right,bottom;

/*printf("createtablescrollbars() tablehigh %d, tablewide %d\n"*/
/*,tablehigh,tablewide);*/
 gettablewinedges(&left,&top,&right,&bottom);/*MUXMWIND*/
   if(tablewrapcol > 0)
   {/*table is to be wrapped*/
      ;
   }
   /* define horizontal scroll bar in UNIX separate table window*/
        left = 0; /*offset*/
        right = +16; /*offset*/
        top = -16;      /*offset from bottom of table window*/
        bottom = 0;
        maketablescrollbar(&htableScroll
                        ,left,top,right,bottom
                        ,0,tablewide,'H');   /*___BAR.C*/

        XtAddCallback(htableScroll,XmNvalueChangedCallback,
		      (XtCallbackProc)htablescroll_CB,NULL);

   /* define vertical scroll bar in UNIX separate table window*/
        left = -16; /*offset*/
        right = 0; /*offset*/
        top = 0;      /*offset from bottom of table window*/
        bottom = +16;
        maketablescrollbar(&vtableScroll
                        ,left,top,right,bottom
                        ,0,tablehigh,'V');   /*___BAR.C*/

        XtAddCallback(vtableScroll,XmNvalueChangedCallback,
		      (XtCallbackProc)vtablescroll_CB,NULL);

   /*XtRealizeWidget(topLevel); */   /* realize widget hierarchy */
/*printf("createtablescrollbars() leaving with tablehigh %d, tablewide %d\n"*/
/*,tablehigh,tablewide);*/

}
/*___createtablescrollbars()_________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****maketablescrollbar()****************************************************/

void maketablescrollbar(Widget *cntl
                        ,int left,int top,int right,int bottom
                        ,int minValue, int maxValue, char Orient)
{                                        
/*printf("maketablescrollbar() max== %d\n",maxValue);*/

  n = 0;
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
  XtSetArg(args[n], XmNhighlightColor, highlightcolor); n++;
  XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
  XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;
  XtSetArg(args[n], XmNselectColor, selectcolor); n++;
  XtSetArg(args[n], XmNtroughColor, troughcolor); n++;
  XtSetArg(args[n], XmNarmColor, armedfillcolor); n++;

if(Orient == 'H')
{
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_FORM); n++;
}
if(Orient == 'V')
{
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
}
  XtSetArg(args[n], XmNleftOffset, left); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset, right); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomOffset, bottom); n++;


  XtSetArg(args[n], XmNsliderSize,1); n++; 
    /*also thumb size in resettablescrollbar()*/
  XtSetArg(args[n], XmNmaximum, maxValue+1); n++; /*maxValue+SliderSize*/
  XtSetArg(args[n], XmNminimum, minValue); n++;
 
  XtSetArg(args[n], XmNincrement, 1); n++;
  XtSetArg(args[n], XmNpageIncrement, 5); n++;

  (*cntl)=XtCreateWidget("NAME",xmScrollBarWidgetClass,tableWindow,args,n);
  XtManageChild((*cntl));
}
/*___maketablescrollbar()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****resettablescrollbar()***************************************************/
void resettablescrollbar(ControlHandle *cntl,int shiftPlace)
{
    int maxValue;
    int thumb; /*size*/
    int step; /*step when click in arrow*/
    int page; /*page move when click in body*/
    int notify; /*1 notifies changed-value call back, 0 does not */
    thumb = 1;
    step = 1;
    page = 5;
    notify = 0;
if(*cntl == htableScroll) maxValue = tablewide;
if(*cntl == vtableScroll) maxValue = tablehigh;
/*printf("resettablescrollbar()\n");*/
    /*Motif slidervalues need to be fudged by slider width*/

    if(shiftPlace > (maxValue) ) shiftPlace = maxValue ;
    if(shiftPlace < (0) ) shiftPlace = 0;
    XmScrollBarSetValues(*cntl,shiftPlace,thumb,step,page,notify);
}
/*___resettablescrollbar()__________________________________________________*/

/****htablescroll_CB()*******************************************************/
XtCallbackProc htablescroll_CB(Widget w, XtPointer client_data, XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
    int      shiftPlace, movePlace;
    int    maxValue=0,minValue=0;   
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    int left,top,right,bottom,tablesize,tablewindowsize;

    shiftPlace = cbs->value; /*between minValue==0 and maxValue*/
    gettablewinedges(&left,&top,&right,&bottom);
    tablesize = tablewide;
    tablewindowsize = right - left - 22;
    maxValue = tablesize;

    if((tablesize-tablewindowsize) > 0)
    {/*something offscreen to scroll to, set zero point of table*/
     /*ratio of slider space == shiftPlace/maxValue*/
        movePlace
               = -((tablesize-tablewindowsize)*(shiftPlace))/(maxValue);
    }
    if(movePlace > 0) movePlace = 0; /*offset of absolute table zero coord*/

    if(shiftPlace == minValue) movePlace = 0;
    else if(shiftPlace == maxValue)
    { 
         if((tablesize - tablewindowsize) > 0)
             movePlace = -(tablesize-tablewindowsize);
         else movePlace = 0;
    }
    tablezerowide = movePlace;
    /*X does its own placement, no need to resettablescrollbar()*/
    redrawtable();

/*printf("htablescroll_CB shiftPlace %d\n",shiftPlace);*/
    return(NULL);
}
/*___htablescroll_CB()_______________________________________________________*/

/****vtablescroll_CB()*******************************************************/
XtCallbackProc vtablescroll_CB(Widget w, XtPointer client_data, XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
    int      shiftPlace, movePlace;
    int    maxValue=0,minValue=0;   
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    int left,top,right,bottom,tablesize,tablewindowsize;

    shiftPlace = cbs->value; /*between minValue==0 and maxValue*/
    gettablewinedges(&left,&top,&right,&bottom);
    tablesize = tablehigh;
    tablewindowsize = bottom - top - 22;
    maxValue = tablesize;

    if((tablesize-tablewindowsize) > 0)
    {/*something offscreen to scroll to, set zero point of table*/
     /*ratio of slider space == shiftPlace/maxValue*/
        movePlace
               = -((tablesize-tablewindowsize)*(shiftPlace))/(maxValue);
    }
    if(movePlace > 0) movePlace = 0; /*offset of absolute table zero coord*/

    if(shiftPlace == minValue) movePlace = 0;
    else if(shiftPlace == maxValue)
    { 
         if((tablesize - tablewindowsize) > 0)
             movePlace = -(tablesize-tablewindowsize);
         else movePlace = 0;
    }
    tablezerohigh = movePlace;
    /*X does its own placement, no need to resettablescrollbar()*/
    redrawtable();
/*printf("vtablescroll_CB shiftPlace %d\n",shiftPlace);*/
    return(NULL);
}
/*___vtablescroll_CB()_______________________________________________________*/


/****gettablewinedges()*******************************************************/
void gettablewinedges(int* left, int* top, int* right, int* bottom)
{
        /*UNIX drawArea width,height: gright,gbottom*/
  Dimension width, height;

  n=0;
  XtSetArg(args[n], XtNwidth, &width); n++;
  XtSetArg(args[n], XtNheight, &height); n++;

  /*XtGetValues(tabledrawArea,args,n);*/
  XtGetValues(tableWindow,args,n);

      *left = 0;
      *top = 0;
      *right =  width;
      *bottom =  height;
/*printf("gettablewinedges: %d, %d, %d, %d\n",*left,*top,*right,*bottom);*/
}
/*___gettablewinedges()_____________________________________________________*/

/****tablepick_CB()***********************************************************/
XtCallbackProc tablepick_CB(Widget tabledrawArea,caddr_t client_data,
                       XmDrawingAreaCallbackStruct *call_data)
{
/*equivalent to MPCMAIN/WMLtableButtonDown(), MACMAIN/DotableContentClick() */

XButtonPressedEvent *bpress = (XButtonPressedEvent *) call_data->event;
    
   if (call_data->reason == XmCR_INPUT)
   {/*call_data->reason == XmCR_INPUT*/
      switch (call_data->event->type)
      {/*switch call_data->event->type*/
	case ButtonPress:
	  if (bpress->button == Button1)
          {/*Button1*/
             /*may be a click within the table itself*/
             tablepickx = bpress->x;
             tablepicky = bpress->y;
             Ltablepicked = 2; /*lurk for a pick*/
             redrawtable();
             if(Ltablepicked==1) 
             {/*table initiated pick operation*/
                 /*tabletographics always active in this direction*/
#ifdef OLDPICKBYCOORDCODE
                 pickx=-32000;/*set graphics mouseposition to something absurd*/
                 picky = -32000; /*since we are going to invoke the */
                 ipick= 2;/*regular picking mechanism with a known pickpoint */
                 Lpick= 1;/*and depend on it not getting a valid graphics pick*/
                      /* which would override the previous known point*/
                 redrawvec();/*establishes where pick is*/
                 redrawvec();/*another update of graphics to do picked effect*/
                    /*finished operations on the graphics window*/
                    /*all table initiated operations done*/
#endif /*OLDPICKBYCOORDCODE*/
                 if(Ltablemarkers) removetablemarkpoints();
                 markfromtablecellsID(1);/*MAGETABL flag==1 for ptID matching*/
                 
                 redrawvec(); /*establishes where pick is*/
                 Ltablepicked = 0; /*used in MAGEDRAW.c/drawvec()*/
                 Ltablecellnewlypicked = 0;
             }/*table initiated pick operation*/
             /*may be a click within the table itself*/
             Ltablepicked = 0;
          }/*Button1*/
	break; /*from case ButtonPress */
      }/*switch call_data->event->type*/
   }/*call_data->reason == XmCR_INPUT*/
   return(NULL);
}
/*___tablepick_CB()__________________________________________________________*/

/****tabledrawArea_CB()*******************************************************/
XtCallbackProc tabledrawArea_CB(Widget tabledrawArea,XtPointer client_data
                                          ,XtPointer call_data)
{
   int   j;
   XExposeEvent* theeventptr;

  /*why bother to check to see if expose_CB was called by an expose cmd????*/
  if(((XmAnyCallbackStruct *) call_data)->reason == XmCR_EXPOSE)
  {/*real expose event*/


    theeventptr 
      = (XExposeEvent *)(((XmDrawingAreaCallbackStruct *)call_data)->event);
    j = theeventptr->count;

    if(j==0)
    {
       redrawtable();
    }
  }/*real expose event*/
  return(NULL);
}
/*___tabledrawArea_CB________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****stringtotable()*********************common name: MAC & PC**************/
void    stringtotable(char string[256],int ix,int iy,int icolor)
{
    if(tablefontsize <= littlefontsize)
    {
       if(littlefont) XSetFont(tabledpy,tablegc,littlefont->fid);
    }
    else if(tablefontsize <= mediumfontsize)
    {
       if(mediumfont) XSetFont(tabledpy,tablegc,mediumfont->fid);
    }
    else if(tablefontsize >= largefontsize)
    {
       if(largefont) XSetFont(tabledpy,tablegc,largefont->fid);
    }
    XSetForeground(tabledpy,tablegc,incolor[BLACKINDEX]);
    XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy,string,strlen(string));
}
/*___stringtoscreen_________________________________________________________*/

/****wordstotable()************************common name: MAC & PC*************/
int wordstotable(char commentstr[256],int ix,int iy,int icolor,int bcolor,int ipass)
{ 
   int   workingfontsize,m;
   int   j,k,kk,oldright,newright,maxright,cellleft,width,Lbkg,Lembedded;
 /*int   Thisfont;*/
 /*int   Thisface;*/
   int   Thissize;
   int   linehigh;
   XFontStruct *currentfont;

   if(icolor >= 0 && icolor <= 255)
   {/*within 256 palette: a visible color*/
      if(Ltablecellgetwidth || Ltablecellispicked)
      {
           Lbkg = 1;
      }
      else Lbkg = 0;

      XSetForeground(tabledpy,tablegc,incolor[icolor]);

      if(commentstr[3] != '\0')
      {/*string longer than zero length*/

         /*Thisfont = commentstr[0];*/
         /*Thisface = commentstr[1];*/
         Thissize = commentstr[2];

         if(Thissize != 0) workingfontsize = Thissize;
         else workingfontsize = tablefontsize;
         linehigh = workingfontsize;

/*010916 outline higlight has problems with proportional font and embedded <> */
         
         if(workingfontsize <= littlefontsize && littlefont)
         {
             XSetFont(tabledpy,tablegc,littlefont->fid);
             currentfont = littlefont;
         }
         else if(workingfontsize <= mediumfontsize && mediumfont)
         {
             XSetFont(tabledpy,tablegc,mediumfont->fid);
             currentfont = mediumfont;
         }
         else if(workingfontsize >= largefontsize && largefont)
         {
             XSetFont(tabledpy,tablegc,largefont->fid);
             currentfont = largefont;
         }
         else /*most likely to exist*/
         {
             XSetFont(tabledpy,tablegc,littlefont->fid);
             currentfont = littlefont;
         }
          
         Lembedded = 0;
         width = 0;
         maxright = ix;
         oldright = ix;
         newright = ix;
         cellleft = ix;
         k = 3;
         for(j=3;j<=255;j++)
         {/*loop over all characters in the string*/
             
            if(   Lembedded==1
               && (   !(Lsuperpick && Ltablecellispicked)
                    || (Ltablecellgetwidth) ) )
            {
               if(commentstr[j]=='>')
               {
                   k = j+1;
                   Lembedded = 0;
               }
            }
            else
            {/*accept this character*/
               kk = j;
               if(commentstr[j]=='\\')
               {/*write out, skip this \, draw next character as special*/
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  /*dump out earlier characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(currentfont,word,strlen(word));
                  /*draw special character*/
                  word[0] = commentstr[j+1]; 
                  word[1] = '\0';
                  if(Lbkg)
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(littlefont,word,strlen(word));
                  j=j+1; /*artificially increase j to that of the special char */
                  k=j+2;/*k is one more so when j loop augmented j-k==0*/
               }
               else if(commentstr[j]==EOL) /*defined in MAGE.h*/
               {
                  if(k==kk)
                  {/*EOL as only character to be printed on this table line*/
                     word[0] = ' ';
                     word[1] = '\0';
                  }
                  else
                  {/*drop back so not print the EOL*/
                     kk = j-1;
                     for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                     word[m] = '\0';
                  }
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = cellleft;
                     newright = cellleft;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  if(ipass != 0)   
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = cellleft;
                  iy = iy + linehigh;
                  /*j will be augmented at top of cycle*/
                  k=j+1; /*so k==j at top of cycle*/
               }
               else if(commentstr[j]=='{' /*}*/) 
               {
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     iy = iy + linehigh/3; /*keep box in sync with pen*/
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  /*dump out earlier characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(currentfont,word,strlen(word));
                  iy = iy + linehigh/3; 
                  /*j will be augmented at top of cycle*/
                  k=j+1; /*so k==j at top of cycle*/
               }
               else if(/*{*/ commentstr[j]=='}')
               {
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     iy = iy - linehigh/3; /*keep box in sync with pen*/
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  /*dump out earlier characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(currentfont,word,strlen(word));
                  iy = iy - linehigh/3; 
                  /*j will be augmented at top of cycle*/
                  k=j+1; /*so k==j at top of cycle*/
               }
               else if(commentstr[j]=='<')
               {
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                     Ltableextrapass = 1; /*< here means extra in a picked cell*/
                  }
                  /*dump out earlier characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(currentfont,word,strlen(word));
                  /*j will be augmented at top of cycle*/
                  k=j+1; /*so k==j at top of cycle*/
                  Lembedded = 1;
               }
               else if(commentstr[j]=='>')
               {
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  /*dump out earlier characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
                  ix = ix + XTextWidth(currentfont,word,strlen(word));
                  /*j will be augmented at top of cycle*/
                  k=j+1; /*so k==j at top of cycle*/
                  Lembedded = 0;
               }
               else if(commentstr[j] == '\0')
               {
                    break;
               }
            }/*accept this character*/
         }/*loop over all char in string*/
         if(k<kk)
         {/*reached end of string, finish out anything still to be written*/
                  kk = j-1;
                  for(m=0; m<=(kk-k); m++) word[m] = commentstr[k+m];
                  word[m] = '\0';
                  if(Lbkg) 
                  {/*bkg rectangle from previous right side to new right edge*/
                     width = XTextWidth(currentfont,word,strlen(word));
                     newright = oldright + width;
                     if(maxright < newright) maxright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[bcolor]);
                     rectangletoscreen(oldright,iy-linehigh,newright,iy,bcolor);
                     oldright = newright;
                     XSetForeground(tabledpy,tablegc,incolor[icolor]);
                  }
                  /*dump out characters*/
                  if(ipass != 0)
                     XDrawString(tabledpy,tabledrawWindow,tablegc,ix,iy
                              ,word,strlen(word));
         }
         if(tablewordwidth < maxright-cellleft)
         {
            tablewordwidth = maxright-cellleft;
         }
      }/*string longer than zero length*/
   }/*within 256 palette: a visible color*/
   return(iy); /*latest y coord used*/
}
/*___wordstotable()__________________________________________________________*/

/****rectangletoscreen()******************************************************/
void rectangletoscreen(int left, int top, int right, int bottom, int icolor)
{
  XPoint aPoints[4];

  aPoints[0].x = left;
  aPoints[0].y = bottom;
  aPoints[1].x = right;
  aPoints[1].y = bottom;
  aPoints[2].x = right;
  aPoints[2].y = top;
  aPoints[3].x = left;
  aPoints[3].y = top;

  XSetForeground(tabledpy,tablegc,incolor[icolor]);
  /*setpencolor(icolor);*/ /*____DRAW.c*/

  XFillPolygon(tabledpy,tabledrawWindow,tablegc
              ,aPoints
              ,sizeof(aPoints)/sizeof(aPoints[0])
              ,Convex
              ,CoordModeOrigin);
}
/*___rectangletoscreen()_____________________________________________________*/
/****tableresize_CB()*********************************************************/
XtCallbackProc tableresize_CB(Widget tabledrawArea,XtPointer client_data
                                        ,XtPointer call_data)
{
  int left, top, right, bottom;
  /*Dimension width, height;*/

  /*why bother to check to see if resize_CB was called by a resize cmd????*/
  if(((XmAnyCallbackStruct *) call_data)->reason == XmCR_RESIZE)
  {/*real resize event*/
     XFreeGC(tabledpy,tablegc);

     tablegc = XCreateGC(tabledpy,tabledrawWindow,0L,NULL);
     XSetFont(tabledpy,tablegc,textfont->fid);
     XSetForeground(tabledpy,tablegc,WhitePixel(dpy,DefaultScreen(dpy)));
     XSetBackground(tabledpy,tablegc,BlackPixel(dpy,DefaultScreen(dpy)));

     gettablewinedges(&left,&top,&right,&bottom);
     /*MUXMBAR.c sets L,T,R,B = 0,0,width,height */
/*printf("resize to %d, %d, %d, %d\n",left,top,right,bottom);*/
     redrawtable();
  }/*real resize event*/
  return(NULL);
}
/*___tableresize_CB()________________________________________________________*/

/****redrawtable()*****************common name: MAC & PC*********************/
void    redrawtable()
{

      dodrawtable(1);

      if(Ltablepicked>0 || Ltablegraphpicked>0)
      {/*pick invoked call needs second pass to show full state of picks*/
         dodrawtable(1); /*put flag on picked cell(s) only*/
         /*finished operations on the table*/
         /*all graph initiated operations done*/
         Ltablegraphpicked = 0;
         /*but still may have table initiated operations to pass to graph*/
         /*Ltablepicked is the flag for table to graph picking*/
      }
}
/*___redrawtable()__________________________________________________________*/

/****dodrawtable()*****************common name: MAC & PC*********************/
void    dodrawtable(int ipass)
{
   int left=0, top=0, right=0, bottom=0;
   gettablewinedges(&left, &top, &right, &bottom);

   /*erase*/
   if(Ltableblackbkg) /*010924*/
   {
      XSetForeground(tabledpy,tablegc,incolor[BLACKINDEX]);
      XFillRectangle(tabledpy,tabledrawWindow,tablegc,0,0,right,bottom);
      XSetForeground(tabledpy,tablegc,incolor[WHITEINDEX]);
      XSetBackground(tabledpy,tablegc,incolor[BLACKINDEX]);
   }
   else
   {
      XSetForeground(tabledpy,tablegc,incolor[WHITEINDEX]);
      XFillRectangle(tabledpy,tabledrawWindow,tablegc,0,0,right,bottom);
      XSetForeground(tabledpy,tablegc,incolor[BLACKINDEX]);
      XSetBackground(tabledpy,tablegc,incolor[WHITEINDEX]);
   }
/*resettablescrollbar(vtableScroll,shiftPlace);*/
/*resettablescrollbar(htableScroll,shiftPlace);*/

   Ltableextrapass = 0;
   drawtable(ipass);  /*MAGETABL.c*/
   if(ipass==1 && Lsuperpick && Ltableextrapass)
   {
      drawtable(2);
      Ltableextrapass = 0;
   }
}
/*___dodrawtable()__________________________________________________________*/

