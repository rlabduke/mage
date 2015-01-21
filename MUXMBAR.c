/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUXMBAR.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include "MAGESYNC.h"
#include "MAGEFLAG.h"


/****zoom_CB*****************************************************************/
XtCallbackProc zoom_CB(Widget w, XtPointer client_data, XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
    int    shiftPlace;
    int    maxValue=8192,minValue=32;    /*over-rides for zoom range */
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    shiftPlace = cbs->value;
    zoom = shiftPlace/( (maxValue - minValue)/10.0 );
    adjustzoom(); /*MAGESYNC.c*/
    return(NULL);
}
/*___zoom_CB________________________________________________________________*/

/****zclip_CB****************************************************************/
XtCallbackProc zclip_CB(Widget w, XtPointer client_data, XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */

{
    int    shiftPlace;
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    shiftPlace = cbs->value;
    izclipold[0] = (shiftPlace);
    izclip = (int)(shiftPlace*fzclip);
    resetgrafzclipBar(shiftPlace); /*___BAR.C*/
/*    writezclipvalue(shiftPlace);*/ /*___BAR.C*/
    redrawvec(); /*MACDRAW.C*/
    return(NULL);
}
/*___zclip_CB_______________________________________________________________*/

/****ztran_CB****************************************************************/
XtCallbackProc ztran_CB(Widget w, XtPointer client_data, XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
    int      shiftPlace;
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    shiftPlace = cbs->value;
  if(Lzlimitscope)
  {/*subvert this slider for special zlimited clipping of selected lists*/
    izlimit = (int)(shiftPlace*fzclip);
    resetgrafztranBar(shiftPlace);
      /*___BAR.C, calc ztran setting from shift, writetranzvalue*/
    redrawvec(); 
  }
  else
  {/*original ztran use*/
    iztranold[0] = shiftPlace; /*970915*/
    iztran = shiftPlace*fzclip; /*970910*/ /*fzclip==fudge_zclip to window*/
    adjusttran(shiftPlace); /*MAGESYNC.c*/
  }
    return(NULL);
}
/*___ztran_CB_______________________________________________________________*/

/*#define DEGSTEP 0.25 now DEGSTEP controled by finestep DLOG 020312,070617*/
/****bondrot_CB**************************************************************/
XtCallbackProc bondrot_CB(Widget w,XtPointer client_data,XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *)call_data;
    int i;
    float theta;
    /*now cbs->value in DEGSTEP degree steps*/
    i = (int)client_data; 
    theta = ((float)cbs->value)*DEGSTEP - bondrotptr[i]->angle; 
    icurrentrotation = i; /*970917*/
    if(L0to360 && ((float)cbs->value)*DEGSTEP > 180)
       bondrotptr[i]->angle = (((float)cbs->value)*DEGSTEP) -360;
    else
       bondrotptr[i]->angle = ((float)cbs->value)*DEGSTEP;
    writebondrotvalue(i); /*___BAR.C*/
    dobondrotrouter(i,theta); /*MAGEANGL.c*/ /*970917*/

    bondrotptr[i]->changed = 1; /*track for suitefitroc output 071215*/
    
    if(Lupdate &&!Linhibiton && Lprobeactive)
    {/*update from remote program: e.g. dots by PROBE*/
       Nthisgroupkind = PROBEDOTSKKIND; /*bondrot change needs probe run*/
       Ldeleteprobedots = 1;
       doupdate(1,update_probe_command_line); /*MUXMPIPE.c*/  /*970522*/
       /* 1: calls writerotated() e.g. for probe*/
       Nthisgroupkind = 0;
    }
    if(Lupdate &&!Linhibiton && LNOEdisplayactive)
    {/*update from remote program: e.g. dots by noe-display*/
       Nthisgroupkind = NOEDISTANCESKKIND; /*bondrot change needs NOE run*/
       LdeleteNOEinput = 1;
       doupdate(1,update_NOE_command_line); /*MUXMPIPE.c*/  /*970522*/
       /* 1: calls writerotated() e.g. for NOE display*/
       Nthisgroupkind = 0;
    }
    rescalekinemage();  /* rescale rebuilds from float arrary*/ /*MAGEINPT.C*/
    redrawvec();
    makecursoractive();  /*___MAIN.C*/
    return(NULL);
}
/*___bondrot_CB_____________________________________________________________*/

/****recreatesliderBars()****************************************************/
void recreatesliderBars()
{
   erasezoomvalue();
   erasezclipvalue();
   eraseztranvalue();
 /*  XtDestroyWidget(vgrafzoom);*/
 /*  XtDestroyWidget(vgrafzclip);*/
 /*  XtDestroyWidget(vgrafztran);*/
   creategrafzoomBar();
   creategrafzclipBar();
   creategrafztranBar();
   resetgrafzoomBar();
   resetgrafzclipBar((int)(izclipold[0]));  /*___BAR.c*/ /*970915*/
  if(Lzlimitscope)
  {/*subvert ztran slider for use with zlimited clipping of selected lists*/
    resetgrafztranBar((int)(izlimit/fzclip)); /*___BAR.c*/ /*011201*/
      /*where setting value is fudged to match window size*/
  }
  else
  {/*original ztran use*/
    resetgrafztranBar((int)(iztranold[0])); /*___BAR.c*/ /*970915*/
  }
}
/*___recreatesliderBars()___________________________________________________*/

/****creategrafzoomBar()*****************************************************/
void    creategrafzoomBar() 
{
    int   maxValue=8192,minValue=32;    /*over-rides for zoom range */
    /*originally 8192, 391 */
    int     left,top,right,bottom;
    int     gleft,gtop,gright,gbottom;    
    
      /* define vertical scroll bar */
      getgrafwinedges(&gleft,&gtop,&gright,&gbottom); /*___BAR.C*/
      /*UNIX drawArea width,height: gright,gbottom*/
        
      left = ZWIDTH;/*offset of left with respect to right grafwindow*/
      /*right = ZWIDTH-ZWIDTH/3;*/ /*offset from right side of grafwindow,*/
      right = ZWIDTH-ZSLIDERWIDTH;/*offset from right side of grafwindow,*/
      if(LTBstereo && Lstereo)
      {
        top = 2; /* % offset from top of grafWindow*/
        bottom = 16; /* % offset from top of grafWindow*/
      }
      else
      {
        top = 5; /* % offset from top of grafWindow*/
        bottom = 33; /* % offset from top of grafWindow*/
      }
      /* upper third for zoom bar */
            
      makegrafcntlbar(&vgrafzoom
                      ,left,top,right,bottom
                      ,minValue,maxValue);    /*___BAR.C*/
      XtAddCallback(vgrafzoom,XmNvalueChangedCallback
                                      ,(XtCallbackProc)zoom_CB,NULL);
      makegrafcntllabel(&zoomvalue,&vgrafzoom); /*___BAR.C*/
      writezoomvalue(); /*___BAR.C*/
      left = right;
      right = 0;
      makegrafcntltitle(&zoomtitle,left,top,right,bottom,"-ZOOM+");
      /*makegrafcntltitle(&zoomtitle,left,top,right,bottom,"- ZOOM +");*/
}
/*___creategrafzoomBar()____________________________________________________*/

/****resetgrafzoomBar()***************************************************/
void    resetgrafzoomBar()
{
    int    shiftPlace;
    int    maxValue=8192,minValue=32;    /*over-rides for zoom range */
    int thumb; /*size*/
    int step; /*step when click in arrow*/
    int page; /*page move when click in body*/
    int notify; /*1 notifies changed-value call back, 0 does not */

    thumb = 20;
    /*step =  16;*/ /*for factor of 5*/
    /*page = 163;*/
    step =   8;
    page = 163;
    notify = 0;
    /*originally 8192, 391 */
          /*shiftPlace = zoom*( (maxValue - minValue)/5.0 );*/
          shiftPlace = zoom*( (maxValue - minValue)/10.0 );
          if(shiftPlace > (maxValue-20) ) shiftPlace = maxValue - 20;
          if(shiftPlace < (minValue+20) ) shiftPlace = minValue + 20;
          /*Motif slidervalues need to be fudged by slider width*/
          /*slider width is an arbitrary number otherwise considered*/
          /* local to makegrafcntlbar() */
    XmScrollBarSetValues(vgrafzoom,shiftPlace,thumb,step,page,notify);
    /*      resetgrafcntlbar(&vgrafzoom,shiftPlace);*/ /*___BAR.C*/
    writezoomvalue(); /*___BAR.C*/
}
/*___resetgrafzoomBar()__________________________________________________*/

/****writezoomvalue()****************************************************/
void    writezoomvalue()
{

/*c*/   sprintf(word,"-ZOOM+");  /* word is a global variable*/

/*c*/   sprintf(numstr,"%4.2f",zoom); /*numstr is a global variable*/
                        
        writecntlbarvalue(&zoomvalue); /*___BAR.C*/
}
/*___writezoomvalue()___________________________________________________*/

/****erasezoomvalue()********************************************************/
void erasezoomvalue()
{
   sprintf(numstr,"        "); /*numstr is a global variable*/
   writecntlbarvalue(&zoomvalue); /*___BAR.C*/
}
/*___erasezoomvalue()_______________________________________________________*/


/****creategrafzclipBar()*****************************************************/
void    creategrafzclipBar()
{
    int   maxValue=800,minValue=0;    /*over-rides for zclip range */
    int     left,top,right,bottom;
    int     gleft,gtop,gright,gbottom;    
    
      /* define vertical scroll bar */
      getgrafwinedges(&gleft,&gtop,&gright,&gbottom); /*___BAR.C*/
      /*UNIX drawArea width,height: gright,gbottom*/

      left = +ZWIDTH;/*offset of left with respect to right grafwindow*/
      /*right = ZWIDTH-ZWIDTH/3;*/ /*offset from right side of grafwindow,*/
      /*right = ZWIDTH-15;*/ /*offset from right side of grafwindow,*/
      right = ZWIDTH-ZSLIDERWIDTH;/*offset from right side of grafwindow,*/
      if(LTBstereo && Lstereo)
      {
        top = 19; /* % offset from top of grafwindow*/
        bottom = 33; /* % offset from top of grafwindow*/
      }
      else
      {
        top = 38; /* % offset from top of grafwindow*/
        bottom = 67; /* % offset from top of grafwindow*/
      }
      /* middle third for zclip bar */
      makegrafcntlbar(&vgrafzclip
                      ,left,top,right,bottom
                      ,minValue,maxValue);   /*___BAR.C*/
      XtAddCallback(vgrafzclip,XmNvalueChangedCallback,
                     (XtCallbackProc)zclip_CB,NULL);

      makegrafcntllabel(&zclipvalue,&vgrafzclip); /*___BAR.C*/
      writezclipvalue( izclipold[0] ); /*___BAR.C*/ /*970915*/
      left = right;
      right = 0;
      makegrafcntltitle(&zcliptitle,left,top,right,bottom,"-ZSLAB+");
      /*makegrafcntltitle(&zcliptitle,left,top,right,bottom,"- ZSLAB +");*/
}
/*___creategrafzclipBar()____________________________________________________*/

/****resetgrafzclipBar()***************************************************/
void    resetgrafzclipBar(int shiftPlace)
{
  int thumb; /*size*/
  int step; /*step when click in arrow*/
  int page; /*page move when click in body*/
  int notify; /*1 notifies changed-value call back, 0 does not */
  int actualPlace;
  thumb = 20;
  step =   1;
  page =  10;
  notify = 0;
  if(shiftPlace + thumb > 800) actualPlace = 800 - thumb;
  else actualPlace = shiftPlace;
  XmScrollBarSetValues(vgrafzclip,actualPlace,thumb,step,page,notify);
  writezclipvalue(shiftPlace); /*___BAR.C*/
}
/*___resetgrafzclipBar()__________________________________________________*/

/****writezclipvalue()****************************************************/
void    writezclipvalue(int shiftPlace)
{

/*c*/   sprintf(word,"-ZSLAB+");  /*word is a global variable*/

/*c*/   sprintf(numstr,"%4d",shiftPlace); 
              /*numstr is a global variable*/
                        

        writecntlbarvalue(&zclipvalue); /*___BAR.C*/
}
/*___writezclipvalue()___________________________________________________*/

/****erasezclipvalue()****************************************************/
void erasezclipvalue()
{
   sprintf(numstr,"        "); /*numstr is a global variable*/
   writecntlbarvalue(&zclipvalue); /*___BAR.C*/
}
/*___erasezclipvalue()___________________________________________________*/


/****creategrafztranBar()*****************************************************/
void    creategrafztranBar()
{
    int    maxValue,minValue;    /*over-rides for ztran range */
    int     left,top,right,bottom;
    int     gleft,gtop,gright,gbottom;    
    
  if(Lzlimitscope)
  {/*subvert this slider for zlimited clipping of selected lists*/
     maxValue=800;
     minValue=0;    /*over-rides for ztran range */
  }
  else
  {/*original ztran use*/
     maxValue=500;
     minValue=-500;    /*over-rides for ztran range */
  }
      /* define vertical scroll bar */
      getgrafwinedges(&gleft,&gtop,&gright,&gbottom); /*___BAR.C*/
      /*UNIX drawArea width,height: gright,gbottom*/

      left = +ZWIDTH;/*offset of left with respect to right grafwindow*/
      /*right = ZWIDTH-ZWIDTH/3;*/ /*offset from right side of grafwindow,*/
      /*right = ZWIDTH-15;*/ /*offset from right side of grafwindow,*/
      right = ZWIDTH-ZSLIDERWIDTH;/*offset from right side of grafwindow,*/
      if(LTBstereo && Lstereo)
      {
        top = 36; /* % offset from top of grafWindow*/
        bottom = 48; /* % offset from top of grafWindow*/
      }
      else
      {
        top = 72; /* % offset from top of grafWindow*/
        bottom = 99; /* % offset from top of grafWindow*/
      }
      /* bottom third for zclip bar */
      makegrafcntlbar(&vgrafztran
                      ,left,top,right,bottom
                      ,minValue,maxValue);   /*___BAR.C*/
      XtAddCallback(vgrafztran,XmNvalueChangedCallback,
                      (XtCallbackProc)ztran_CB,NULL);

      makegrafcntllabel(&ztranvalue,&vgrafztran); /*___BAR.C*/
      left = right;
      right = 0;
  if(Lzlimitscope)
  {/*subvert this slider for zlimit clipping of selected lists*/
      writeztranvalue(izlimit/fzclip); /*___BAR.C*/ /*970915*/
        /*where setting value is fudged to match window size*/
      makegrafcntltitle(&ztrantitle,left,top,right,bottom,"-ZSLABB+");
      /*makegrafcntltitle(&ztrantitle,left,top,right,bottom,"- ZSLABB +");*/
  }
  else
  {/*original ztran use*/
      writeztranvalue(iztranold[0]); /*___BAR.C*/ /*970915*/
      makegrafcntltitle(&ztrantitle,left,top,right,bottom,"-ZTRAN+");
      /*makegrafcntltitle(&ztrantitle,left,top,right,bottom,"- ZTRAN +");*/
  }
}
/*___creategrafztranBar()____________________________________________________*/

/****resetgrafztranBar()***************************************************/
void    resetgrafztranBar(int nztran) /*970905*/
{
    int thumb; /*size*/
    int step; /*step when click in arrow*/
    int page; /*page move when click in body*/
    int notify; /*1 notifies changed-value call back, 0 does not */
    int actualPlace; /*for zlimit use*/
    thumb = 20;
    step =   1;
    page =  10;
    notify = 0;
  if(Lzlimitscope)
  {/*subvert this slider for zlimited clipping of selected lists*/
    if(nztran + thumb > 800) actualPlace = 800 - thumb;
    else actualPlace = nztran;
    XmScrollBarSetValues(vgrafztran,actualPlace,thumb,step,page,notify);
    writeztranvalue(nztran); /*___BAR.C*/ 
  }
  else
  {/*original ztran use*/
    XmScrollBarSetValues(vgrafztran,nztran,thumb,step,page,notify);/*970905*/
    writeztranvalue(nztran); /*___BAR.C*/ /*970905*/
  }
}
/*___resetgrafztranBar()__________________________________________________*/


/****writeztranvalue()****************************************************/
void    writeztranvalue(int nztran) /*970905,011201*/
{
  if(Lzlimitscope)
  {/*subvert this slider for zlimited clipping of selected lists*/
    sprintf(word,"ZSLABB+");  /* word is a global variable*/
  }
  else
  {/*original ztran use*/
    sprintf(word,"-ZTRAN+");  /* word is a global variable*/
  }                        
  sprintf(numstr,"%4d",nztran); /*numstr is a global variable*/
  writecntlbarvalue(&ztranvalue); /*___BAR.C*/
}
/*___writeztranvalue()___________________________________________________*/

/****eraseztranvalue()****************************************************/
void eraseztranvalue()
{
   sprintf(numstr,"        "); /*numstr is a global variable*/
   writecntlbarvalue(&ztranvalue); /*___BAR.C*/
}
/*___eraseztranvalue()___________________________________________________*/

/****makegrafcntlbar()********************************************************/
void    makegrafcntlbar(Widget *vgrafcntl
                        ,int left,int top,int right,int bottom
                        ,int minValue,int maxValue)
{
/*printf("makegrafcntlbar():L,T,R,B: %d, %d, %d, %d\n",left,top,right,bottom);*/
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



  XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, -left); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNtopPosition, top); n++; /*percent offset ????*/

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset, right); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNbottomPosition, bottom); n++; /*percent offset ????*/

  XtSetArg(args[n], XmNmaximum, maxValue); n++;
  XtSetArg(args[n], XmNminimum, minValue); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNsliderSize,1); n++;
  /*XtSetArg(args[n], XmNsliderSize,ZSLIDERWIDTH); n++;*/
  /*XtSetArg(args[n], XmNsliderSize,20); n++;*/
  /*arbitrary sliderSze, also used to get sliderPlace in resetgrafzoomBar()*/
  (*vgrafcntl) = XtCreateWidget("NAME", xmScrollBarWidgetClass,
                                     grafwindow, args, n);
  /* add an activation callback */
  /*XtAddCallback((*vgrafcntl),XmNvalueChangedCallback,buttonscallback,NULL);*/
  XtManageChild((*vgrafcntl));
}
/*___makegrafcntlbar()_______________________________________________________*/

/****makegrafcntllabel()******************************************************/
void    makegrafcntllabel(Widget *cntlvalue, Widget *vgrafcntl)
{

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

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNleftWidget, *vgrafcntl); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET); n++;
  XtSetArg(args[n], XmNtopWidget, *vgrafcntl); n++;

  /*XtSetArg(args[n], XmNtopOffset, -25); n++;*/ /*031025 largefont conflicts*/
  XtSetArg(args[n], XmNtopOffset, -35); n++; /*031116 avoids font conflicts*/

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset,0); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
  XtSetArg(args[n], XmNbottomWidget, *vgrafcntl); n++;
  XtSetArg(args[n], XmNbottomOffset,2); n++;
  if (titlefontList){ XtSetArg(args[n], XmNfontList, titlefontList); n++;}

  (*cntlvalue) = XtCreateWidget(" ", xmLabelWidgetClass, grafwindow, args, n);
  XtManageChild(*cntlvalue);
}
/*___makegrafcntllabel()_____________________________________________________*/

/****makegrafcntltitle()******************************************************/
void    makegrafcntltitle(Widget *cntltitle
                          ,int left,int top,int right,int bottom
                          , char* title)
{
int j,k;
char testtitle[256];

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

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNleftOffset, -left); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNtopPosition, top); n++;

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset, right); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_POSITION); n++;
  XtSetArg(args[n], XmNbottomPosition, bottom); n++;

  if (titlefontList){ XtSetArg(args[n], XmNfontList, titlefontList); n++;}

  (*cntltitle) = XtCreateWidget(" ", xmLabelWidgetClass, grafwindow, args, n);
  XtManageChild(*cntltitle);

  k = 0;
  for(j = 0; j<(strlen(title)); j++)
  {
    testtitle[k] = title[j];
    k++;
    testtitle[k] = EOL;
    k++;
  }
  testtitle[k] = '\0';
  n = 0;
  XtSetArg(args[n], XmNlabelString
           ,XmStringCreateLtoR(testtitle,XmSTRING_DEFAULT_CHARSET) ); n++;
  XtSetValues(*cntltitle, args, n);
}
/*___makegrafcntltitle()_____________________________________________________*/

/****resetgrafcntlbar()*******************************************************/
void    resetgrafcntlbar(ControlHandle *vgrafcntl,int shiftPlace)
{
    int thumb; /*size*/
    int step; /*step when click in arrow*/
    int page; /*page move when click in body*/
    int notify; /*1 notifies changed-value call back, 0 does not */
    thumb = 20;
    step = 10;
    page = 50;
    notify = 0;
    XmScrollBarSetValues(*vgrafcntl,shiftPlace,thumb,step,page,notify);
}
/*___resetgrafcntlbar()______________________________________________________*/

/****getgrafwinedges()********************************************************/
void    getgrafwinedges(int *left,int *top,int *right,int *bottom)
{
        /*UNIX drawArea width,height: gright,gbottom*/
  Dimension width, height;

  n=0;
  XtSetArg(args[n], XtNwidth, &width); n++;
  XtSetArg(args[n], XtNheight, &height); n++;
  
  XtGetValues(drawArea,args,n);

      *left = 0;
      *top = 0;
      *right =  width;
      *bottom =  height;
}
/*___getgrafwinedges()_______________________________________________________*/

/****writecntlbarvalue********************************************************/
void    writecntlbarvalue(Widget *cntllabel )
{
    n = 0;
    XtSetArg(args[n], XmNlabelString
           ,XmStringCreate(numstr,XmSTRING_DEFAULT_CHARSET) ); n++;
    XtSetValues(*cntllabel, args, n);
}
/*____writecntlbarvalue______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*BOND ROTATION CONTROLS */
/****creategrafbondrotBar()*********common name: MAC and PC and UNIX *********/
/*only called in one place by MAGEBBOX.c/SetsinGrafWindow() as of 070617*/
void    creategrafbondrotBar(int i)
{
   /*over-rides for bond rotation range*/
    int  maxValue = (int)( 180/DEGSTEP);
    int  minValue = (int)(-180/DEGSTEP); 
         /*now slider index is in DEGSTEP degrees*/
    int  left,top,right,bottom,width;
    static int N = 0; /*actual number of shown sliders 070617 */
    
 if(i==1) {N=0;} /*initiate counter of visible sliders 070617*/
 if(Lallbondrotsliders || bondrotptr[i]->slider == 1) /*070617*/ 
 {  /*slider in bondrot window optional 070617*/
    N++;  /*augment counter: N for i, 070617*/
    if(L0to360)
    {
       maxValue = maxValue - minValue;
       minValue = 0;
    }
    
        /* define horizontal scroll bar in UNIX separate window*/
        /*width = 3*ROTSLIDERWIDTH;*/ /*thrice the bar width*/
        width = 2*ROTSLIDERWIDTH+5+6; 
        left = 0; /*offset*/
        right = 0; /*offset*/
        top = (N*width)-width;      /*offset from top of separate window*/
        bottom = (N*width)-width+ROTSLIDERWIDTH; 
           /*offset from top of separate window */
        makebondrotlabelwidget(&bondrotvalue[i]
                        ,&hgrafbondrot[i]
                        ,left,top,right,bottom);

        top = (N*width)-width+ROTSLIDERWIDTH; 
           /*offset from top of separate window */
        bottom = (N*width)-width+2*ROTSLIDERWIDTH; 
           /*offset from top of separate window*/
        makegrafbondrotbar(&hgrafbondrot[i]
                        ,left,top,right,bottom
                        ,minValue,maxValue);   /*___BAR.C*/

        top = (N*width)-width+2*ROTSLIDERWIDTH;
           /*offset from top of separate window */
        bottom = (N*width-width+2*ROTSLIDERWIDTH+5);
           /*offset from top of separate window*/
        makebondrotscopewidget(&bondrotscope[i]
                        ,&hgrafbondrot[i]
                        ,left,top,right,bottom, i );

        top = (N*width)-width+2*ROTSLIDERWIDTH+5; 
           /*offset from top of separate window */
        bottom = (N*width);        /*offset from top of separate window */
        makebondrotdividewidget(&bondrotdivide[i]
                        ,&hgrafbondrot[i]
                        ,left,top,right,bottom);

        XtAddCallback(hgrafbondrot[i],XmNvalueChangedCallback,
		      (XtCallbackProc)bondrot_CB, (void *) i);
        writebondrotvalue(i); /*___BAR.C*/
        /*hilitegrafcntlbar(&hgrafbondrot[i);*/ /*MACBAR.C*/
 }
}
/*___creategrafbondrotBar()_________________________________________________*/

/****makegrafbondrotbar()*********common name: MAC and PC and UNIX **********/
void    makegrafbondrotbar(Widget *hgrafcntl
                        ,int left,int top,int right,int bottom
                        ,int minValue,int maxValue)
{                                        

  n = 0;
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
  XtSetArg(args[n], XmNhighlightColor, highlightcolor); n++;
  /*XtSetArg(args[n], XmNtopShadowColor, troughcolor); n++;*/
  XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
  XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;
  /*XtSetArg(args[n], XmNbottomShadowColor, troughcolor); n++;*/
  XtSetArg(args[n], XmNselectColor, selectcolor); n++;
  XtSetArg(args[n], XmNtroughColor, troughcolor); n++;
  XtSetArg(args[n], XmNarmColor, armedfillcolor); n++;

  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNbottomOffset, -bottom); n++;

  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNsliderSize,1); n++; 
    /*also thumb size in resetgrafbondrotcntlbar()*/
  XtSetArg(args[n], XmNmaximum, maxValue+1); n++; /*maxValue+SliderSize*/
  XtSetArg(args[n], XmNminimum, minValue); n++;
 
  XtSetArg(args[n], XmNincrement, 1); n++;
  XtSetArg(args[n], XmNpageIncrement, 5); n++;
  (*hgrafcntl)=XtCreateWidget("NAME",xmScrollBarWidgetClass,brotwindow,args,n);
  XtManageChild((*hgrafcntl));
}
/*___makegrafbondrotbar()___________________________________________________*/

/****makebondrotlabels()*********common name: MAC and PC and UNIX **********/
void    makebondrotlabels(int i)
{                                        
 ;
}
/*___makebondrotlabels()___________________________________________________*/

/****makebondrotlabelwidget()********* just UNIX *****************************/
void    makebondrotlabelwidget(Widget *thiswidget, Widget *lastwidget
                        ,int left,int top,int right,int bottom)
{                                        
  /*lastwidget not used: 950616*/
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


  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNbottomOffset, -bottom); n++;

  if (titlefontList){ XtSetArg(args[n], XmNfontList, titlefontList); n++;}
  (*thiswidget)=XtCreateWidget("NAME",xmLabelWidgetClass,brotwindow,args,n);
  XtManageChild((*thiswidget));
}
/*___makebondrotlabelwidget()________________________________________________*/

/****makebondrotscopewidget()********* just UNIX *****************************/
void    makebondrotscopewidget(Widget *thiswidget, Widget *lastwidget
                        ,int left,int top,int right,int bottom, int i)
{                                        
    Display* display;

  n = 0;
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, troughcolor); n++;
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;
  XtSetArg(args[n], XmNhighlightColor, highlightcolor); n++;
  XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
  XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;
  XtSetArg(args[n], XmNselectColor, selectcolor); n++;
  XtSetArg(args[n], XmNtroughColor, troughcolor); n++;
  XtSetArg(args[n], XmNarmColor, armedfillcolor); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNbottomOffset, -bottom); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  if (titlefontList){ XtSetArg(args[n], XmNfontList, titlefontList); n++;}
  XtSetArg(args[n], XmNresizePolicy, XmRESIZE_ANY); n++;
  *thiswidget = XmCreateDrawingArea(brotwindow," ",args,n);
  XtManageChild(*thiswidget);

    display = XtDisplay(*thiswidget);
    bondrotpatterngc[i] = XCreateGC(display,XtWindow(*thiswidget),0L,(XGCValues*) NULL);
 XtAddCallback(*thiswidget,XmNexposeCallback,(XtCallbackProc)bondrotpattern_CB,(void *)i);
 XtAddCallback(*thiswidget,XmNresizeCallback,(XtCallbackProc)bondrotpattern_CB,(void *)i);

}
/*___makebondrotscopewidget()________________________________________________*/

/****bondrotpattern_CB()******************************************************/
XtCallbackProc bondrotpattern_CB(Widget widget, XtPointer client_data, XtPointer call_data)
{
   int i;
   /*XmDrawingAreaCallbackStruct* ptr;*/
   Dimension                    width, height;

   i = (int)client_data;
   /*ptr = (XmDrawingAreaCallbackStruct*)call_data;*/
   
   /*always redraw pattern for bondrot (slider bar) regions*/
   XtVaGetValues(widget, XmNwidth, &width, XmNheight, &height, NULL);

   /*pattern for bondrot (slider bar) regions*/
   DrawPattern(XtDisplay(widget), XtWindow(widget), bondrotpatterngc[i]
     , (int)width, (int)height, i);

   return(NULL);
}
/*___bondrotpattern_CB()_____________________________________________________*/

/****DrawPattern_CB()*********************************************************/
void DrawPattern(Display* display, Window window, GC gc, int width, int height, int i)
{/*pattern for bondrot (slider bar) regions, prototype here*/
  int j,k,num,x1,x2,y;
  float xdel;

  /*always draw at least the tick marks at +-120*/ /*010920*/

  /*if(bondrotpattern[i][0] != '\0')*/  /*010920*/
  /*{*/ /*is a pattern*/
      /*test values*/
      /*bondrotgoodcolor[i] = 202;*/    /*green4*/
      /*bondrotgoodcolor[i] = 167;*/      /*sea3*/
      bondrotgoodcolor[i] = incolor[numberofcolor("sea")+50*3]; /*sea3*/
      /*bondrotbadcolor[i] = 201;*/     /*red4*/
      /*bondrotbadcolor[i] = 158;*/       /*pink3*/
      bondrotbadcolor[i] = incolor[numberofcolor("pink")+50*3]; /*pink3*/
      /*bondrotblahcolor[i] = 205;*/    /*yellow4*/
      /*bondrotblahcolor[i] = 155;*/      /*yellow3*/
      bondrotblahcolor[i] = incolor[numberofcolor("yellow")+50*3];  /*yellow3*/
      bondrotgoodmax[i] = 1;
      bondrotbadmin[i] = 9;
      if(  bondrotpattern[i][0] == 'R'
         ||bondrotpattern[i][0] == 'Y'
         ||bondrotpattern[i][0] == 'G')
      {/*simple scale*/
         bondrotgoodmax[i] = 1;
         bondrotbadmin[i]  = 9;
      }/*simple scale*/
   xdel = ( ( (float)width - 2.0*(float)(ROTSLIDERWIDTH+3))/36.0);
      /*thumb 6 pixels across narrow width*/

   XSetLineAttributes(display,gc,3,LineSolid,CapButt,JoinRound);

   for(j=0; j<=36; j++)
   {/*draw pattern, at least the tick marks at +-120*/
      if(j==0) x1 = ROTSLIDERWIDTH+3; /*arrow + 1/2 thumb width*/
         /*start at edge of arrow, 37 total intervals, inc 2 halves*/
      else x1 = x2;
      if(j<18) x2 = ROTSLIDERWIDTH+3+(int)(xdel/2.0 +xdel*(float)j); 
                              /*1st interval is 1/2*/
      else if(j<36) x2 = ROTSLIDERWIDTH+4+(int)(xdel/2.0 +xdel*(float)j); 
                              /*loses a pixel, round off??*/
      else x2 = ROTSLIDERWIDTH+4+(int)(xdel*(float)(j)); 
           /*36 full intervals, last is 1/2*/

      if(L0to360) k = j;
      else if(j < 18 ) k = j + 18; /*adjust 0 to 360 as -180, 0, +180*/
      else k = j - 18;

      if(bondrotpattern[i][0] != '\0')  /*010920*/
      {/*angle pattern in addition to tick marks at +-120*/
         if     (bondrotpattern[i][k] == '0') num = 0;
         else if(bondrotpattern[i][k] == '1') num = 1;
         else if(bondrotpattern[i][k] == '2') num = 2;
         else if(bondrotpattern[i][k] == '3') num = 3;
         else if(bondrotpattern[i][k] == '4') num = 4;
         else if(bondrotpattern[i][k] == '5') num = 5;
         else if(bondrotpattern[i][k] == '6') num = 6;
         else if(bondrotpattern[i][k] == '7') num = 7;
         else if(bondrotpattern[i][k] == '8') num = 8;
         else if(bondrotpattern[i][k] == '9') num = 9;
         else if(bondrotpattern[i][k] == 'A') num =10;
         else if(bondrotpattern[i][k] == 'R') num = 9;
         else if(bondrotpattern[i][k] == 'Y') num = 5;
         else if(bondrotpattern[i][k] == 'G') num = 1;
         else num = 10;
      
         if(num <= bondrotgoodmax[i]) 
         {
             XSetForeground(display, gc, bondrotgoodcolor[i]);
             y = 4;
         }
         else if(num >= bondrotbadmin[i]) 
         {
             XSetForeground(display, gc, bondrotbadcolor[i]);
             y = 0;
         }
         else 
         {
             XSetForeground(display, gc, bondrotblahcolor[i]);
             y = 2;
         }
      }/*angle pattern in addition to tick marks at +-120*/
      else
      {/*fill in blahish color*/
         XSetForeground(display, gc, incolor[numberofcolor("white")+50*2] );
             y = 2;
      }
      XFillRectangle(display, window, gc, x1, 0, x2, height);
            /*y = height - num*height/10;*/
      
      if(j==0||j==12||j==24||j==36)
      {/*marks at 60 degrees*/
         XSetForeground(display, gc, 0); /*0 full black in all schemes*/
         XDrawLine(display, window, gc, x1, y, x2, y);
      }/*marks at 60 degrees*/
      XFlush(display);
   }/*draw pattern, at least the tick marks at +-120*/

  /*}*/ /*is a pattern*/
  /*else*/
  /*{*/ /* no pattern*/
  /*    x2 = 0;*/
  /*}*/
  XSetForeground(display, gc, troughcolor); /*fill out region*/
  XFillRectangle(display, window, gc, x2, 0, width, height);
  XFlush(display);
   
}
/*___DrawPattern()___________________________________________________________*/

/****makebondrotdividewidget()********* just UNIX *****************************/
void    makebondrotdividewidget(Widget *thiswidget, Widget *lastwidget
                        ,int left,int top,int right,int bottom)
{                                        
  n = 0;
  XtSetArg(args[n], XmNvisual, magevisual); n++;
  XtSetArg(args[n], XmNdepth, magedepth); n++;
  XtSetArg(args[n], XmNcolormap, colormap); n++;
  XtSetArg(args[n], XmNbackground, windowbackground); n++;
  /*XtSetArg(args[n], XmNbackground, troughcolor); n++;*/
  XtSetArg(args[n], XmNborderColor, windowborder); n++;
  /*XtSetArg(args[n], XmNborderColor, windowborder); n++;*/
  XtSetArg(args[n], XmNborderWidth, 2); n++;
  XtSetArg(args[n], XmNforeground, windowforeground); n++;

  XtSetArg(args[n], XmNhighlightColor, highlightcolor); n++;
  XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
  XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;
  XtSetArg(args[n], XmNselectColor, selectcolor); n++;
  XtSetArg(args[n], XmNtroughColor, troughcolor); n++;
  XtSetArg(args[n], XmNarmColor, armedfillcolor); n++;


  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, top); n++;

  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;

  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_OPPOSITE_FORM); n++;
  XtSetArg(args[n], XmNbottomOffset, -bottom); n++;

  if (titlefontList){ XtSetArg(args[n], XmNfontList, titlefontList); n++;}
  (*thiswidget)=XtCreateWidget(" ",xmLabelWidgetClass,brotwindow,args,n);
  XtManageChild((*thiswidget));
}
/*___makebondrotdividewidget()________________________________________________*/

/****resetgrafbondrotBar()**************common name: MAC and PC and UNIX *****/
/*resetgrafbondrotBar called by MAGEANGL.c MAGEBBOX.c MAGEFILE.c as of 070617*/
void    resetgrafbondrotBar(int i) 
{
   int   shiftPlace;

   /*over-rides for bond rotation range*/
   int  maxValue = (int)( 180/DEGSTEP);
   int  minValue = (int)(-180/DEGSTEP); 
        /*now slider index is in DEGSTEP degrees*/
 if(Lallbondrotsliders || bondrotptr[i]->slider == 1) /*070617*/
 { /*slider in bondrot window optional 070617*/
   if(L0to360)
   {
      maxValue = maxValue - minValue;
      minValue = 0;
   }

   /*Motif slidervalues need to be fudged by slider width*/
   /*slider width is an arbitrary number otherwise considered*/
   /* local to makegrafcntlbar() */
   /*shiftPlace = (bondrotptr[i]->angle+.5);*/
   if(L0to360 && bondrotptr[i]->angle < 0) 
        shiftPlace = (360.0 + bondrotptr[i]->angle)/DEGSTEP; /*i.e. 360 -angle*/
   else shiftPlace = (bondrotptr[i]->angle)/DEGSTEP; /*or >=0 +.5,<0 -.5 ????*/

   if(shiftPlace > (maxValue) ) shiftPlace = maxValue ;
   if(shiftPlace < (minValue) ) shiftPlace = minValue;
   resetgrafbondrotcntlbar(&hgrafbondrot[i],shiftPlace); /*___BAR.C*/
   writebondrotvalue(i); /*___BAR.C*/
   resetgraphedrotamer(i); /*MAGEANGL.c, checks for this graphing*/
 }
}
/*___resetgrafbondrotBar()__________________________________________________*/

/****resetgrafbondrotcntlbar()************************************************/
void    resetgrafbondrotcntlbar(ControlHandle *vgrafcntl,int shiftPlace)
{
    int thumb; /*size*/
    int step; /*step when click in arrow*/
    int page; /*page move when click in body*/
    int notify; /*1 notifies changed-value call back, 0 does not */
    thumb = 1;
    /*thumb = 20;*/
    step = 1;
    page = 10;
    notify = 0;
    XmScrollBarSetValues(*vgrafcntl,shiftPlace,thumb,step,page,notify);
}
/*___resetgrafbondrotcntlbar()_______________________________________________*/

/****writebondrotvalue()************common name: MAC and PC and UNIX ********/
/*also called by MAGEANGL.c as of 070617*/
void    writebondrotvalue(int i)  
{
 if(Lallbondrotsliders || bondrotptr[i]->slider == 1) /*070617*/
 { /*Slider in bondrot window optional 070617*/
        /*numstr global variable*/
        /*001009 bondrotptr[i]->angle stored as -180 <-> +180*/
    if(L0to360 && bondrotptr[i]->angle < 0.0)
        /*sprintf(numstr,"%s, %4.2f=%4.2f, %4.2f"*/
        sprintf(numstr,"%s, %8.3f=%8.3f, %8.3f"/*080309*/
        ,bondrotptr[i]->name,360+bondrotptr[i]->angle,bondrotptr[i]->angle
        ,bondrotptr[i]->origangle);
    else
        /*sprintf(numstr,"%s, %4.2f, %4.2f"*/
        sprintf(numstr,"%s, %8.3f, %8.3f"/*080309*/
        ,bondrotptr[i]->name,bondrotptr[i]->angle,bondrotptr[i]->origangle);
        writecntlbarvalue(&bondrotvalue[i]);
        /*writes string numstr into widget text area bondrotvalue[i]*/
 }
}
/*___writebondrotvalue()____________________________________________________*/

/****erasebondrotvalue()************common name: MAC and PC******************/
void erasebondrotvalue(int i){;}
/*___erasebondrotvalue()____________________________________________________*/
/*BOND ROTATION CONTROLS */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/


