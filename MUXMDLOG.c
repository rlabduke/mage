/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/                     
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEDLOG.h"
#include "MAGEMENU.h"
#include "MAGEFLAG.h"
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include "MAGEBBOX.h" /*for SetsinGrafWindow() 121108 */
#include <Xm/DialogS.h>

XtTimerCallbackProc nextanotherDialog(XtPointer);

int Lreporteditactivity=0; /* = 1 to test movepoints & meansigmas protection*/

/*forced message*/ 
/*
   sprintf(alertstr,"thisanswer[%d]",nQuestion);
   sprintf(alertstr2,"%s",thisanswer[nQuestion]); 
       alertstr3[0]='\0';
        DoMageDLOGreport(2); 
*/

/****resetmagepalette()*******************************************************/
void      resetmagepalette() /*010604*/
{
   ;
}
/*___resetmagepalette()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DoMageinitDLOGreport()***************************************************/
void    DoMageinitDLOGreport()
{
    DoMageDLOGreport(3);
}
/*___DoMageinitDLOGreport()__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****reportOK_CB**************************************************************/
XtCallbackProc reportOK_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XtDestroyWidget(reportbox);
    /*clear alertstrs*/
    alertstr[0] = '\0';
    alertstr2[0] = '\0';
    alertstr3[0] = '\0';
    return(NULL);
}
/*___reportOK_CB_____________________________________________________________*/

/****DoMageDLOGreport()*******************************************************/
void   DoMageDLOGreport(int mode)
{                   /*create a dialogbox for reports and alerts, etc */
  /*dialog box management problem, redo each call,*/
  /*so no need to remember if dialog created*/
  int ncommon;

/*fprintf(stderr,"DoMageDLOGreport...\n"); 121222*/
/*overall reportbox*/
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
  ncommon = n;
  /*now n = ncommon, all widgets share these first n args */
/*  if(mode==4) {XtSetArg(args[n], XmNheight, 4*ALERTHEIGHT + 100); n++;}*/ /*650*/
/*  else        {XtSetArg(args[n], XmNheight, 3*ALERTHEIGHT + 100); n++;}*/ /*550*/
  XtSetArg(args[n], XmNwidth, 800); n++; 
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;
  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "MAGE INFORMATION"); n++;
/*  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; */
    /*ignore close commands from frame menu or X button*/
  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; 
    /*try to deal with close commands from frame menu or X button*/
  reportbox = XmCreateFormDialog(grafwindow,"MAGE",args,n); /* --- FORM --- */

/*  reportbox = XmCreateDialogShell(grafwindow,"MAGE",args,n);*/
/*  XtCreateManagedWidget("MAGE",xmDialogShellWidgetClass,grafwindow,args,n);*/

/*reportbox = */
    /*XtCreateManagedWidget("MAGE",xmFormWidgetClass,grafwindow,args,n);*/
  /*BUT cannot move this around to get it out of the way*/

  /*XtCreateWidget("MAGE" ,xmRowColumnWidgetClass,grafwindow,args,n);*/
  /*XtManageChild(reportbox);*/
  XtAddCallback(reportbox, XmNdestroyCallback
                               ,(XtCallbackProc)reportOK_CB, NULL); 

/*container for alertstrs and buttons: reports*/
  n = ncommon; 
/*  if(mode==4) {XtSetArg(args[n], XmNheight, 4*ALERTHEIGHT + 100); n++;}*/ /*650*/
/*  else        {XtSetArg(args[n], XmNheight, 3*ALERTHEIGHT + 100); n++;}*/ /*550*/
/*  XtSetArg(args[n], XmNwidth, 800); n++; */

XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 


  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  reports = XtCreateWidget("alerts", xmRowColumnWidgetClass,reportbox,args,n);
  XtManageChild(reports);

/*the alert regions: report1,report2,report3, and mode==4: report4 020904*/
  n = ncommon;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNheight, ALERTHEIGHT); n++; 
/*  XtSetArg(args[n], XmNwidth, 800); n++; */

/*XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;*/
/*XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;*/


  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  report1 = XtCreateWidget("alert1", xmTextWidgetClass,reports,args,n);
  XmTextSetString(report1, alertstr);
  XtManageChild(report1);

  n = ncommon;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNheight, ALERTHEIGHT); n++; 
/*  XtSetArg(args[n], XmNwidth, 800); n++; */
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  report2 = XtCreateWidget("alert2", xmTextWidgetClass,reports,args,n);
  XmTextSetString(report2, alertstr2);
  XtManageChild(report2);

  n = ncommon;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNheight, ALERTHEIGHT); n++; 
/*  XtSetArg(args[n], XmNwidth, 800); n++; */
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  report3 = XtCreateWidget("alert3", xmTextWidgetClass,reports,args,n);
  XmTextSetString(report3, alertstr3);
  XtManageChild(report3);

 if(mode==4)
 {/*forth report area  020904*/
  n = ncommon;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNheight, ALERTHEIGHT); n++; 
/*  XtSetArg(args[n], XmNwidth, 800); n++; */
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  report4 = XtCreateWidget("alert4", xmTextWidgetClass,reports,args,n);
  XmTextSetString(report4, alertstr4);
  XtManageChild(report4);
 }

/*container for Proceed and Abort buttons*/
  n = ncommon; 
  XtSetArg(args[n], XmNheight, 60); n++; 
/*  XtSetArg(args[n], XmNwidth, 800); n++; */
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  reportbuts=XtCreateWidget("buttons",xmRowColumnWidgetClass,reports,args,n);
  XtManageChild(reportbuts);
/*the Proceed and Abort buttons */
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  reportbutOK = XmCreatePushButton(reportbuts,"Proceed",args,n);
  XtAddCallback(reportbutOK, XmNactivateCallback
                               ,(XtCallbackProc)reportOK_CB, NULL); 
  XtManageChild(reportbutOK);

  if(mode==2||mode==3)
  {
     n = ncommon; 
     if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
     reportbutabort = XmCreatePushButton(reportbuts,"Abort Program",args,n);
     XtAddCallback(reportbutabort, XmNactivateCallback
                                  ,(XtCallbackProc)quit_CB, NULL); 
     XtManageChild(reportbutabort);
  }
#ifdef OLDCODE
  XmTextSetString(report1, alertstr);
  XmTextSetString(report2, alertstr2);
  XmTextSetString(report3, alertstr3);
  if(mode==4)
  {
     XmTextSetString(report4, alertstr4);
  }
#endif
  XtManageChild(reportbox);
}
/*___DoMageDLOGreport()______________________________________________________*/


/****DoSELECTINGcolorDialog()*************************************************/
void  DoSELECTINGcolorDialog() /*cribbed from DocolorshowDialog() 061029*/
{                
  XmString    xmstring;
  int item=0, ncommon=0;
  float widthfactor=0;
  int scrHeight = 0, dialogHeight=0, scrollHeight=0;

if(!ACTIVE_DLOG)
{

/*overall DLOG box*/ /*originally 040x0y used 6+28, 050816 use 8+28, re caveat*/
  scrHeight = DisplayHeight( dpy, DefaultScreen(dpy));
  if((8+28)*FONTSIZE > scrHeight) {  dialogHeight = GHEIGHT; }
  else { dialogHeight = (8+28)*FONTSIZE; }
  scrollHeight = dialogHeight - 8*FONTSIZE;

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
  ncommon = n;
  XtSetArg(args[n], XmNwidth, (130+(1*FONTSIZE))*widthfactor); n++;/*050816*/
  XtSetArg(args[n], XmNheight, dialogHeight); n++; 
    /*this is the only explicit height setting...*/
  XtSetArg(args[n], XmNx, 0); n++;
  XtSetArg(args[n], XmNy, 0); n++;

  XtSetArg(args[n], XmNtitle, "COLOR"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    /*ignore close commands from frame menu or X button*/
/*  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; */
    /*try to deal with close commands from frame menu or X button*/

  SELECTINGcolorDLOG = XmCreateFormDialog(grafwindow," ",args,n);
/*SELECTINGcolorbox: row column container for everything*/
  n = ncommon;
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNtitle, "SHOW"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    /*ignore close commands from frame menu or X button*/
/*  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; */
    /*try to deal with close commands from frame menu or X button*/
  SELECTINGcolorbox
    = XtCreateWidget(" ",xmRowColumnWidgetClass,SELECTINGcolorDLOG,args,n);
  XtManageChild(SELECTINGcolorbox);
/*  row box for subject */
  n = ncommon; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++; /*050816*/
  /*colorshowtextrow*/
  SELECTINGcolortextrow
      = XtCreateWidget(" ", xmRowColumnWidgetClass,SELECTINGcolorbox,args,n);
  XtManageChild(SELECTINGcolortextrow);

/*Subject*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*colorshowtext*/
  SELECTINGcolortext 
    = XmCreatePushButtonGadget(SELECTINGcolortextrow,"color selection",args,n); /*050816 cheat...*/
  XtManageChild(SELECTINGcolortext);/*050816 text does not work placed here in*/
  /*xmTextWidgetClass, however, can cheat and put it in a PushButtonGadget*/


/*control buttons: Cancel (OK implied by choice)*/
  n = ncommon; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  /*colorshowbuttons*/
  SELECTINGcolorbuttons 
    = XtCreateWidget("but", xmRowColumnWidgetClass,SELECTINGcolorbox,args,n);
  XtManageChild(SELECTINGcolorbuttons); /*all DLOG have place for OK, etc. buttons*/

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  /*colorshow_CANCEL*/
  SELECTINGcolor_CANCEL 
    = XmCreatePushButtonGadget(SELECTINGcolorbuttons,"Cancel",args,n);
  XtAddCallback(SELECTINGcolor_CANCEL, XmNactivateCallback
                              ,(XtCallbackProc)SELECTINGcolor_CANCEL_CB, NULL); 
  XtManageChild(SELECTINGcolor_CANCEL); 

/*Scrolled List*/
  n = ncommon; 
  XtSetArg(args[n], XmNheight, scrollHeight); n++;
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNselectionPolicy, XmBROWSE_SELECT); n++;
  XtSetArg(args[n], XmNvisibleItemCount, 28); n++;
  /*colorshowlist*/
  SELECTINGcolorlist 
    = XmCreateScrolledList(SELECTINGcolorbox,"color",args,n);
  XtAddCallback(SELECTINGcolorlist,XmNbrowseSelectionCallback
                                   ,(XtCallbackProc)SELECTINGcolorOK_CB, NULL); 
  XtManageChild(SELECTINGcolorlist); 

/*load list*/
  sprintf(word,"red"); item=1; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"orange"); item=2; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"gold"); item=3; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"yellow"); item=4; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lime"); item=5; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"green"); item=6; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"sea"); item=7; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"cyan"); item=8; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"sky"); item=9; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"blue"); item=10; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"purple"); item=11; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"magenta"); item=12; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"hotpink"); item=13; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"pink"); item=14; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lilac"); item=15; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"peach"); item=16; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"peachtint"); item=17; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"yellowtint"); item=18; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"greentint"); item=19; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"bluetint"); item=20; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lilactint"); item=21; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"pinktint"); item=22; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"white"); item=23; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"gray"); item=24; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);  
  sprintf(word,"brown"); item=25; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"invisible"); item=26; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"deadwhite"); item=27; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"deadblack"); item=28; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(SELECTINGcolorlist,xmstring,item); XmStringFree(xmstring);


 /*COLORSHOW_DLOG*/
 ACTIVE_DLOG = SELECTINGCOLOR_DLOG;
 SELECTINGcolorDialog();
  XtManageChild(SELECTINGcolorDLOG);
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DoSELECTINGcolorDialog()________________________________________________*/

/****SELECTINGcolorDialog()***************************************************/
void SELECTINGcolorDialog()
{
    XmString    xmstring;

    interpretcolor(ncolorSELECTING); /* word <- */
    xmstring=XmStringCreateSimple(word);
    XmListSelectItem(SELECTINGcolorlist,xmstring,0); /*select by name !!!! */
    XmStringFree(xmstring);
}
/*___SELECTINGcolorDialog()__________________________________________________*/

/****SELECTINGcolorOK_CB()****************************************************/
XtCallbackProc SELECTINGcolorOK_CB(Widget w,XtPointer client_data,XtPointer call_data)
{   /* choice implies OK */
    XmListCallbackStruct *cbs = (XmListCallbackStruct *)call_data;
    ncolorSELECTING = cbs->item_position; 
    if(trialcolor==26) ncolorSELECTING = 31; /*invisible actual colorcode*/
    if(trialcolor==27) ncolorSELECTING = 29; /*deadwhite actual colorcode*/
    if(trialcolor==28) ncolorSELECTING = 30; /*deadblack actual colorcode*/
    XtDestroyWidget(SELECTINGcolorDLOG);
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___SELECTINGcolorOK_CB_____________________________________________________*/

/****SELECTINGcolor_CANCEL_CB()***********************************************/
XtCallbackProc SELECTINGcolor_CANCEL_CB(Widget w,XtPointer client_data
                                           ,XtPointer call_data)
{
    XtDestroyWidget(SELECTINGcolorDLOG);
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___SELECTINGcolor_CANCEL_CB________________________________________________*/


/****DocolorshowDialog()******************************************************/
void  DocolorshowDialog() /*reworked between 031012 and 040217 for font resize*/
{                
  /*050816 unfortunately, short windows (argent) cause color-choice to be */
  /*shown to the right of top control buttons, and off-screen out-of-sight */
  /*unless one deliberately expands the window, or retracts linux bottom panel*/
  /*Do NOT remember that the scrolling actually worked on any platform!*/

  XmString    xmstring;
  int item=0, ncommon=0;
  float widthfactor=0;
  int scrHeight = 0, dialogHeight=0, scrollHeight=0;

if(!ACTIVE_DLOG)
{

/*overall DLOG box*/ /*originally 040x0y used 6+28, 050816 use 8+28, re caveat*/
  scrHeight = DisplayHeight( dpy, DefaultScreen(dpy));
  if((8+28)*FONTSIZE > scrHeight) {  dialogHeight = GHEIGHT; }
  else { dialogHeight = (8+28)*FONTSIZE; }
  scrollHeight = dialogHeight - 8*FONTSIZE;

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
  ncommon = n;
/*  XtSetArg(args[n], XmNwidth, 150); n++;*/
/*XtSetArg(args[n], XmNwidth, (130+(1*FONTSIZE))*widthfactor); n++;*/ /*040x0y*/
  XtSetArg(args[n], XmNwidth, (130+(1*FONTSIZE))*widthfactor); n++;/*050816*/
    /*this is the only explicit width setting..., 050816 try absurd width...*/
    /*This width setting seems to have NO effect on anything! */
/*  XtSetArg(args[n], XmNheight, 660); n++;*/
/*  XtSetArg(args[n], XmNheight, 36+28*FONTSIZE); n++;*/
/*  XtSetArg(args[n], XmNheight, (6+28)*FONTSIZE); n++;*/ 
  XtSetArg(args[n], XmNheight, dialogHeight); n++; 
    /*this is the only explicit height setting...*/
  XtSetArg(args[n], XmNx, 0); n++;
  XtSetArg(args[n], XmNy, 0); n++;

/*  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;*/
  XtSetArg(args[n], XmNtitle, "COLOR"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    /*ignore close commands from frame menu or X button*/
/*  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; */
    /*try to deal with close commands from frame menu or X button*/

  colorshowDLOG = XmCreateFormDialog(grafwindow," ",args,n);
  /*XtManageChild(colorshowDLOG);*/
/*  XtAddCallback(colorshowDLOG, XmNdestroyCallback
                               ,(XtCallbackProc)colorshow_CANCEL_CB, NULL); 
*/
/*colorshowbox: row column container for everything*/
  n = ncommon;
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 
/*  XtSetArg(args[n], XmNwidth, 150); n++;  */
/*  XtSetArg(args[n], XmNheight, 660); n++; */
/*  XtSetArg(args[n], XmNx, 10); n++;*/
/*  XtSetArg(args[n], XmNy, 10); n++;*/
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
/*  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;*/
  XtSetArg(args[n], XmNtitle, "SHOW"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++;
    /*ignore close commands from frame menu or X button*/
/*  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; */
    /*try to deal with close commands from frame menu or X button*/
  colorshowbox=XtCreateWidget(" ",xmRowColumnWidgetClass,colorshowDLOG,args,n);
  XtManageChild(colorshowbox);
/*
  XtAddCallback(colorshowbox, XmNdestroyCallback
                               ,(XtCallbackProc)colorshow_CANCEL_CB, NULL); 
*/
/*  row box for subject */
  n = ncommon; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++; /*050816*/
  colorshowtextrow
      = XtCreateWidget(" ", xmRowColumnWidgetClass,colorshowbox,args,n);
  XtManageChild(colorshowtextrow);

/*Subject*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 120); n++;*/
/*XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;*/ /*needed*/
/*XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;*/ /*needed*/
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
/*  colorshowtext = XtCreateWidget(" ",xmTextWidgetClass,colorshowtextrow,args,n);*/
  colorshowtext = XmCreatePushButtonGadget(colorshowtextrow,"color selection",args,n); /*050816 cheat...*/
  XtManageChild(colorshowtext); /*050816 text does not work placed here in*/
  /*xmTextWidgetClass, however, can cheat and put it in a PushButtonGadget*/

/*  row of radio buttons */
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 120); n++;*/
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  colorshowrowradios
      = XtCreateWidget(" ", xmRowColumnWidgetClass,colorshowbox,args,n);
  XtManageChild(colorshowrowradios);

/*radio box A */
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 60); n++;*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  colorshow_RADIOA = XmCreateToggleButtonGadget(colorshowrowradios,"List",args,n);
  XtAddCallback(colorshow_RADIOA,XmNvalueChangedCallback
                                  ,(XtCallbackProc)colorshow_RADIOA_CB, NULL); 
  XtManageChild(colorshow_RADIOA);
/*radio box B */
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 60); n++;*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  colorshow_RADIOB = XmCreateToggleButtonGadget(colorshowrowradios,"Point",args,n);
  XtAddCallback(colorshow_RADIOB,XmNvalueChangedCallback
                                  ,(XtCallbackProc)colorshow_RADIOB_CB, NULL); 
  XtManageChild(colorshow_RADIOB);

/*control buttons: OK, Option, Cancel */
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 120); n++;*/
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  colorshowbuttons = 
             XtCreateWidget("but", xmRowColumnWidgetClass,colorshowbox,args,n);
  XtManageChild(colorshowbuttons); /*all DLOG have place for OK, etc. buttons*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 60); n++;*/
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  colorshow_OK = XmCreatePushButtonGadget(colorshowbuttons,"OK",args,n);
  XtAddCallback(colorshow_OK,XmNactivateCallback
                               ,(XtCallbackProc)colorshow_OK_CB,NULL); 
  XtManageChild(colorshow_OK);
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNwidth, 60); n++;*/
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  colorshow_CANCEL = XmCreatePushButtonGadget(colorshowbuttons,"Cancel",args,n);
  XtAddCallback(colorshow_CANCEL, XmNactivateCallback
                                   ,(XtCallbackProc)colorshow_CANCEL_CB, NULL); 
  XtManageChild(colorshow_CANCEL); 

/*row box for message to the user about expanding window to see choices 050816*/
  n = ncommon; 
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++; /*050816*/
  colorshowcaveatrow
      = XtCreateWidget(" ", xmRowColumnWidgetClass,colorshowbox,args,n);
  XtManageChild(colorshowcaveatrow);

  /*caveat to reader*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  colorshowcaveat = XmCreatePushButtonGadget(colorshowcaveatrow,"expand window->",args,n); /*050816 cheat...*/
  XtManageChild(colorshowcaveat); 
 
/*Scrolled List*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNheight, 12); n++;*/
/*  XtSetArg(args[n], XmNheight, 28*FONTSIZE); n++;*/
  XtSetArg(args[n], XmNheight, scrollHeight); n++;
/*  XtSetArg(args[n], XmNwidth, 120); n++;*/
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNselectionPolicy, XmBROWSE_SELECT); n++;
/*  XtSetArg(args[n], XmNselectionPolicy, XmSINGLE_SELECT); n++;*/
  XtSetArg(args[n], XmNvisibleItemCount, 28); n++;
/*  colorshowlist = XmCreateList(colorshowbox,"color",args,n);*/
  colorshowlist = XmCreateScrolledList(colorshowbox,"color",args,n);
/*  XtAddCallback(colorshowlist,XmNsingleSelectionCallback*/
  XtAddCallback(colorshowlist,XmNbrowseSelectionCallback
                                          ,(XtCallbackProc)colorshow_CB, NULL); 
  XtManageChild(colorshowlist); 

/*load list*/
  sprintf(word,"red"); item=1; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"orange"); item=2; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"gold"); item=3; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"yellow"); item=4; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lime"); item=5; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"green"); item=6; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"sea"); item=7; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"cyan"); item=8; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"sky"); item=9; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"blue"); item=10; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"purple"); item=11; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"magenta"); item=12; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"hotpink"); item=13; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"pink"); item=14; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lilac"); item=15; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"peach"); item=16; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"peachtint"); item=17; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"yellowtint"); item=18; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"greentint"); item=19; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"bluetint"); item=20; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"lilactint"); item=21; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"pinktint"); item=22; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"white"); item=23; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"gray"); item=24; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);  
  sprintf(word,"brown"); item=25; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"invisible"); item=26; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"deadwhite"); item=27; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);
  sprintf(word,"deadblack"); item=28; xmstring=XmStringCreateSimple(word); 
  XmListAddItem(colorshowlist,xmstring,item); XmStringFree(xmstring);


 ACTIVE_DLOG = COLORSHOW_DLOG;
 colorshowDialog();
  XtManageChild(colorshowDLOG);
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DocolorshowDialog()_____________________________________________________*/

int newlistcolor,newpointcolor;

/****colorshowDialog()********************************************************/
void colorshowDialog()
{
 XmString    xmstring;

 /*  sprintf(word,"color selection");*/  /*050816 can only do text here!!??*/
 /*  XmTextReplace(colorshowtext,0,XmTextGetLastPosition(colorshowtext),word);*/

     origlistcolor = pickedlistptr->color; /*remember list color in a global*/
     newlistcolor = origlistcolor;
     trialcolor = 0; /*global to hold new color value*/
     origpointcolor = pickedpointptr->colorwidth & 31; /*hold point color also*/
     newpointcolor = origpointcolor;
     if(!Lpointcolor &&(Listcolordominant || origpointcolor==0))
     {
        trialcolor = origlistcolor;
        colorshow_radioAint = 1;
        colorshow_radioBint = 0;
     }
     else 
     {
        trialcolor = origpointcolor;
        colorshow_radioAint = 0;
        colorshow_radioBint = 1;
     }
     interpretcolor(trialcolor); /* word <- */
     xmstring=XmStringCreateSimple(word);
     XmListSelectItem(colorshowlist,xmstring,0); /*select by name !!!! */
     XmStringFree(xmstring);
     XmToggleButtonSetState(colorshow_RADIOA,colorshow_radioAint, 0);
     XmToggleButtonSetState(colorshow_RADIOB,colorshow_radioBint, 0);
}
/*___colorshowDialog()_______________________________________________________*/

/****colorshow_CB()***********************************************************/
XtCallbackProc colorshow_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XmListCallbackStruct *cbs = (XmListCallbackStruct *)call_data;
    trialcolor = cbs->item_position; /*remember selection in a global*/
    if(trialcolor==26) trialcolor = 31; /*invisible actual colorcode*/
    if(trialcolor==27) trialcolor = 29; /*deadwhite actual colorcode*/
    if(trialcolor==28) trialcolor = 30; /*deadblack actual colorcode*/
    if(colorshow_radioAint)
    {
       Lpointcolor = 0; /*memory of current choice*/
       newlistcolor = trialcolor;
       colorshow_update(newlistcolor,origpointcolor);
    }
    else if(colorshow_radioBint)
    {
       Lpointcolor = 1; /*memory of current choice*/
       newpointcolor = trialcolor;
       colorshow_update(origlistcolor,newpointcolor);
    }
    return(NULL);
}
/*___colorshow_CB____________________________________________________________*/

/****colorshow_RADIOA_CB()****************************************************/
XtCallbackProc colorshow_RADIOA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{ /*change list color */
   colorshow_radioAint = 1;  /* touch means select this radio */
   XmToggleButtonSetState(colorshow_RADIOA,colorshow_radioAint, 0);
   XmToggleButtonSetState(colorshow_RADIOB,colorshow_radioBint=0, 0);
   if(trialcolor==0) trialcolor = origlistcolor;
   colorshow_update(trialcolor,origpointcolor);
    return(NULL);
}
/*___colorshow_RADIOA_CB()___________________________________________________*/

/****colorshow_RADIOB_CB()****************************************************/
XtCallbackProc colorshow_RADIOB_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{ /*change point color */
   colorshow_radioBint = 1;  /* touch means select this radio */
   XmToggleButtonSetState(colorshow_RADIOB,colorshow_radioBint, 0);
   XmToggleButtonSetState(colorshow_RADIOA,colorshow_radioAint=0, 0);
   if(trialcolor==0) trialcolor = origpointcolor;
   colorshow_update(origlistcolor,trialcolor);
    return(NULL);
    
}
/*___colorshow_RADIOB_CB()___________________________________________________*/

/****colorshow_OK_CB()********************************************************/
XtCallbackProc colorshow_OK_CB(Widget w,XtPointer client_data
                                       ,XtPointer call_data)
{
    XtDestroyWidget(colorshowDLOG);/*030304 destroy before doing other things*/
    Lpruneactive = 1; /*971129 for ChangeCheck*/
    if(colorshow_radioAint)
    {/*parameters: listcolor,pointcolor*/
       colorshow_update(trialcolor,origpointcolor);
    }
    else if(colorshow_radioBint)
    {
       colorshow_update(origlistcolor,trialcolor);
    }
    /*XtDestroyWidget(colorshowDLOG);*/ 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___colorshow_OK_CB_________________________________________________________*/

/****colorshow_CANCEL_CB()****************************************************/
XtCallbackProc colorshow_CANCEL_CB(Widget w,XtPointer client_data
                                           ,XtPointer call_data)
{
    XtDestroyWidget(colorshowDLOG);/*030304 destroy before doing other things*/
    colorshow_update(origlistcolor,origpointcolor);
       /*restore all lists for safety as well as point*/
    /*XtDestroyWidget(colorshowDLOG);*/ 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___colorshow_CANCEL_CB_____________________________________________________*/

/*****DopickshowDialog*******************************************************/
void    DopickshowDialog()  /* <- MUX_MAIN/pick_CB() */
{
  int ncommon;

 if(!ACTIVE_DLOG)
 {
  /*overall DLOG box********************************/
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
  ncommon = n;
  XtSetArg(args[n], XmNwidth, 600); n++; /*was 210*/
  XtSetArg(args[n], XmNheight, 600); n++; /*was 250*/
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;

  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "SHOW PROPERTIES"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/

  pickshowDLOG = XmCreateFormDialog(grafwindow," ",args,n);
  XtManageChild(pickshowDLOG);

  /*pickshowBIGbox: row column container for everything*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNwidth, 210); n++;*/
/*  XtSetArg(args[n], XmNheight, 250); n++;*/
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshowBIGbox=XtCreateWidget(" ",xmRowColumnWidgetClass,pickshowDLOG,args,n);
  XtManageChild(pickshowBIGbox);

  /*Subject******************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 200); n++;
  XtSetArg(args[n], XmNrows, 3); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  pickshowtext = XtCreateWidget(" ", xmTextWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowtext); 

  /* A row of columns *************************************************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 280); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  pickshowrowA =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowrowA);

  /* B column for grp name and check boxes**BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolB =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolB);
  /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_grptitle = XmCreateLabel(pickshowcolB,"group name",args,n);
  XtManageChild(pickshow_grptitle);
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_grpname = 
           XtCreateWidget(" ", xmTextWidgetClass, pickshowcolB,args,n);
  XtManageChild(pickshow_grpname);

  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_grpdom = XmCreateToggleButtonGadget(   pickshowcolB," ",args,n);
  XtManageChild(pickshow_grpdom);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_grpnobut = XmCreateToggleButtonGadget( pickshowcolB," ",args,n);
  XtManageChild(pickshow_grpnobut);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_grpnoshow = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_grpnoshow);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_grpselect = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_grpselect);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_grpdelete = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_grpdelete);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_1animate = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_1animate);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_2animate = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_2animate);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_ignorable = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_ignorable);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_foobarrier = XmCreateToggleButtonGadget(pickshowcolB," ",args,n);
  XtManageChild(pickshow_foobarrier);

 /* C column for sgrp name and check boxes**CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolC =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolC);
 /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_sgrptitle = XmCreateLabel(pickshowcolC,"subgroup name",args,n);
  XtManageChild(pickshow_sgrptitle);
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_sgrpname = 
           XtCreateWidget(" ", xmTextWidgetClass, pickshowcolC,args,n);
  XtManageChild(pickshow_sgrpname);

  /*check boxes */
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_sgrpdom = XmCreateToggleButtonGadget(  pickshowcolC," ",args,n);
  XtManageChild(pickshow_sgrpdom);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_sgrpnobut = XmCreateToggleButtonGadget(pickshowcolC," ",args,n);
  XtManageChild(pickshow_sgrpnobut);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_sgrpnoshow = XmCreateToggleButtonGadget(pickshowcolC," ",args,n);
  XtManageChild(pickshow_sgrpnoshow);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_sgrpselect = XmCreateToggleButtonGadget(pickshowcolC," ",args,n);
  XtManageChild(pickshow_sgrpselect);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_sgrpdelete = XmCreateToggleButtonGadget(pickshowcolC," ",args,n);
  XtManageChild(pickshow_sgrpdelete);

 /* D for list name and check boxes DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolD =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolD);
 /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_listtitle = XmCreateLabel(pickshowcolD,"list name",args,n);
  XtManageChild(pickshow_listtitle);
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_listname = 
           XtCreateWidget(" ", xmTextWidgetClass, pickshowcolD,args,n);
  XtManageChild(pickshow_listname);

  /*check boxes */
  n = ncommon;  /*dummy*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_listdom = XmCreateLabel(               pickshowcolD," ",args,n);
  XtManageChild(pickshow_listdom);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_listnobut = XmCreateToggleButtonGadget(pickshowcolD," ",args,n);
  XtManageChild(pickshow_listnobut);
  n = ncommon;  /*dummy*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_listnoshow = XmCreateLabel(            pickshowcolD," ",args,n);
  XtManageChild(pickshow_listnoshow);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_listselect = XmCreateToggleButtonGadget(pickshowcolD," ",args,n);
  XtManageChild(pickshow_listselect);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_listdelete = XmCreateToggleButtonGadget(pickshowcolD," ",args,n);
  XtManageChild(pickshow_listdelete);

 /* E row for pointID name and  check boxes**EEEEEEEEEEEEEEEEEEEEEEEEEEEE*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolE =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolE);
 /*name*/
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_ptIDtitle = XmCreateLabel(pickshowcolE,"point IDentifier",args,n);
  XtManageChild(pickshow_ptIDtitle);
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_pointID = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowcolE,args,n);
  XtManageChild(pickshow_pointID);

  /*check boxes */
  n = ncommon;  /*dummy*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_ptIDdom =               XmCreateLabel(  pickshowcolE," ",args,n);
  XtManageChild(pickshow_ptIDdom);
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_ptunpick = XmCreateToggleButtonGadget(  pickshowcolE," ",args,n);
  XtManageChild(pickshow_ptunpick);

 /*always show point color*/ 
  n = ncommon;  /*dummy*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_pointcolor = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowcolE,args,n);
  XtManageChild(pickshow_pointcolor);
  XtVaSetValues(pickshow_pointcolor,XmNsensitive,0,NULL); /*disable 061129*/

  /*accept or cancel properties modifications */

  /*control buttons: OK, Cancel *****************************************/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  pickshowbuttons = 
           XtCreateWidget("but", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowbuttons); /*all DLOG have place for OK, etc. buttons*/
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_OK = 
      XmCreatePushButtonGadget(pickshowbuttons,"Accept changes",args,n);
  XtAddCallback(pickshow_OK,XmNactivateCallback
       ,(XtCallbackProc)pickshow_OK_CB,NULL); 
  XtManageChild(pickshow_OK);
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_CANCEL = 
      XmCreatePushButtonGadget(pickshowbuttons,"Ignore changes",args,n);
  XtAddCallback(pickshow_CANCEL,XmNactivateCallback
       ,(XtCallbackProc)pickshow_CANCEL_CB,NULL); 
  XtManageChild(pickshow_CANCEL);

  ACTIVE_DLOG = PICKSHOW_DLOG;
  pickshowDialog();
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DopickshowDialog_______________________________________________________*/

/****pickshowDialog()*********************************************************/
void pickshowDialog()
{
 XmString    xmstring;
 int         thestate=0;
int        Localdomgroup=0,Localdomsubgroup=0,Localneversubgroup=0;
int        Localnobuttongroup=0,Localnobuttonsubgroup=0,Localnobuttonlist=0;
int        ideletegroup=0,ideletesubgroup=0,ideletelist=0;
int        iselectgroup=0,iselectsubgroup=0,iselectlist=0;  /*061125*/
int        iunpickable=0;
int        Local1animategroup=0, Local2animategroup=0;
int        Localsuppressgroup=0, Localignorablegroup=0, Localfoobarriergroup=0;
int        j=0;

     sprintf(word, "Edit Object Properties:\nHierarchy of the last picked point.\n can be edited to change names, or various properties of the group, subgroup, list, and point.");
     /*XmTextReplace(pickshowtext,0,XmTextGetLastPosition(pickshowtext),word);*/
     XmTextSetString(pickshowtext,word);

     sprintf(word,"%s",pickedgrupptr->name);
     /*group name extra leading character, '*' or '%' or ' ', shows animate*/
     /*Dialog explicit animate choice so do not show this 1st char here*/
     for(j=1; j<=MAXNAMECHAR+2; j++)/*just copy it all, word[iword]='\0'*/
     {
        temps[j-1] = word[j];/*group name starts at char 1*/
     }
     XmTextReplace(             pickshow_grpname,0
         ,XmTextGetLastPosition(pickshow_grpname),temps);
     
     sprintf(word,"%s",pickedsgrpptr->name);
     XmTextReplace(             pickshow_sgrpname,0
         ,XmTextGetLastPosition(pickshow_sgrpname),word);
     sprintf(word,"%s",pickedlistptr->name);
     XmTextReplace(             pickshow_listname,0
         ,XmTextGetLastPosition(pickshow_listname),word);
     sprintf(word,"%s",comstr); /*pickshow picked pointID already in comstr*/
     XmTextReplace(             pickshow_pointID,0
         ,XmTextGetLastPosition(pickshow_pointID),word);

     /*always show point color*/
 
     if((pickedpointptr->colorwidth & 31) >0)
     {
        pickedpointcolor = (pickedpointptr->colorwidth & 31);
        interpretcolor(pickedpointcolor); /*returns color name in global word*/
     }
     else
     {
        pickedpointcolor = 0; /*NO point color*/
        sprintf(word,"NO point color");
     }
     XmTextReplace(             pickshow_pointcolor,0
         ,XmTextGetLastPosition(pickshow_pointcolor),word);

     /*dominance and button making features*/
     if(pickedgrupptr->STATUS & DOMINANT)    Localdomgroup = 1;
     if(pickedgrupptr->STATUS & NOBUTTON)    Localnobuttongroup = 1;
     if(pickedsgrpptr->STATUS & DOMINANT)    Localdomsubgroup = 1;
     if(pickedsgrpptr->STATUS & NOBUTTON)    Localnobuttonsubgroup = 1;
     if(pickedsgrpptr->STATUS & NEVERSHOW)   Localneversubgroup = 1;
     if(pickedlistptr->STATUS & NOBUTTON)    Localnobuttonlist = 1;

     /*animate*/
     if(pickedgrupptr->animate==1) Local1animategroup = 1;
     else if(pickedgrupptr->animate==2) Local2animategroup = 1;
     if(pickedgrupptr->KKIND & IGNORABLEKKIND) Localignorablegroup = 1;
     if(pickedgrupptr->KKIND & FOOBARRIERKKIND) Localfoobarriergroup = 1;
     
     sprintf(word,"dominant"); xmstring = XmStringCreateSimple(word); 
     XtVaSetValues(         pickshow_grpdom,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_grpdom,Localdomgroup, 0);
     XtVaSetValues(         pickshow_sgrpdom,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_sgrpdom,Localdomsubgroup, 0);
     XmStringFree(xmstring);

     sprintf(word,"nobutton"); xmstring = XmStringCreateSimple(word); 
     XtVaSetValues(         pickshow_grpnobut,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_grpnobut,Localnobuttongroup, 0);
     XtVaSetValues(         pickshow_sgrpnobut,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_sgrpnobut,Localnobuttonsubgroup, 0);
     XtVaSetValues(         pickshow_listnobut,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_listnobut,Localnobuttonlist, 0);
     XmStringFree(xmstring);

     /*pickable-unpickable*/
     if(   pickedpointptr->type & UNPICKABLE )
           iunpickable = 1;
     else  iunpickable = 0;

     sprintf(word,"unpickable"); xmstring = XmStringCreateSimple(word); 
     XtVaSetValues(         pickshow_ptunpick,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_ptunpick,iunpickable, 0);
     XmStringFree(xmstring);

     /*noshow check boxes*/ /*030220 grpnoshow to remove/suppress group level*/

     if(pickedgrupptr != firstgrupptr)
     {
        sprintf(word,"suppress"); xmstring = XmStringCreateSimple(word);
     }
     else
     {
        sprintf(word," "); xmstring = XmStringCreateSimple(word);
     }
     XtVaSetValues(         pickshow_grpnoshow,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_grpnoshow,Localsuppressgroup, 0);
     XmStringFree(xmstring);
     sprintf(word,"never show"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_sgrpnoshow,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_sgrpnoshow,Localneversubgroup, 0);
     XmStringFree(xmstring);

     /*select whole things*/
     sprintf(word,"selectable by color"); xmstring = XmStringCreateSimple(word);
     if(pickedgrupptr->KKIND & SELECTINGKKIND) {iselectgroup = 1;}
     else {iselectgroup = 0;}
     XtVaSetValues(         pickshow_grpselect,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_grpselect,iselectgroup, 0);
     if(pickedsgrpptr->KKIND & SELECTINGKKIND) {iselectsubgroup = 1;}
     else {iselectsubgroup = 0;}
     XtVaSetValues(         pickshow_sgrpselect,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_sgrpselect,iselectsubgroup, 0);
     if(pickedlistptr->KKIND & SELECTINGKKIND) {iselectlist = 1;}
     else {iselectlist = 0;}
     XtVaSetValues(         pickshow_listselect,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_listselect,iselectlist, 0);
     XmStringFree(xmstring);

     /*delete whole things*/
     sprintf(word,"delete"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_grpdelete,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_grpdelete,ideletegroup, 0);
     XtVaSetValues(         pickshow_sgrpdelete,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_sgrpdelete,ideletesubgroup, 0);
     XtVaSetValues(         pickshow_listdelete,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_listdelete,ideletelist, 0);
     XmStringFree(xmstring);

     /*animate group*/
     sprintf(word,"animate"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_1animate,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_1animate,Local1animategroup, 0);
     XmStringFree(xmstring);
     sprintf(word,"2animate"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_2animate,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_2animate,Local2animategroup, 0);
     XmStringFree(xmstring);
     sprintf(word,"ignorable"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_ignorable,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_ignorable,Localignorablegroup, 0);
     XmStringFree(xmstring);
     sprintf(word,"foobarrier"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_foobarrier,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_foobarrier,Localfoobarriergroup, 0);
     XmStringFree(xmstring);

}
/*___pickshowDialog()________________________________________________________*/

/****pickshow_OK_CB()********************************************************/
XtCallbackProc pickshow_OK_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
  int  Localdomgroup=0,Localdomsubgroup=0;
  int  Localnobuttongroup=0,Localnobuttonsubgroup=0,Localnobuttonlist=0; 
  int  iunpickable=0,Localneversubgroup=0;
  int  ideletegroup=0,ideletesubgroup=0,ideletelist=0; 
  int  iselectgroup=0,iselectsubgroup=0,iselectlist=0; 
  char *thestr;
  int  j=0,k=0,ichanged=0;
  int  Localsuppressgroup=0, Localignorablegroup=0, Localfoobarriergroup;
  int  Local1animategroup=0, Local2animategroup=0, ianimate=0;

  if(pickedgrupptr != firstgrupptr)
  {
     Localsuppressgroup = XmToggleButtonGetState(pickshow_grpnoshow);
  }
  Localdomgroup =   XmToggleButtonGetState(pickshow_grpdom); 
  Localdomsubgroup =   XmToggleButtonGetState(pickshow_sgrpdom); 
  Localnobuttongroup =   XmToggleButtonGetState(pickshow_grpnobut); 
  Localnobuttonsubgroup =   XmToggleButtonGetState(pickshow_sgrpnobut); 
  Localnobuttonlist =  XmToggleButtonGetState(pickshow_listnobut); 
  iunpickable =   XmToggleButtonGetState(pickshow_ptunpick); 
  Localneversubgroup = XmToggleButtonGetState(pickshow_sgrpnoshow); 
  iselectgroup =    XmToggleButtonGetState(pickshow_grpselect); 
  iselectsubgroup =    XmToggleButtonGetState(pickshow_sgrpselect); 
  iselectlist =   XmToggleButtonGetState(pickshow_listselect); 
  ideletegroup =    XmToggleButtonGetState(pickshow_grpdelete); 
  ideletesubgroup =    XmToggleButtonGetState(pickshow_sgrpdelete); 
  ideletelist =   XmToggleButtonGetState(pickshow_listdelete); 
  Local1animategroup = XmToggleButtonGetState(pickshow_1animate);
  Local2animategroup = XmToggleButtonGetState(pickshow_2animate);
  Localignorablegroup = XmToggleButtonGetState(pickshow_ignorable);
  Localfoobarriergroup = XmToggleButtonGetState(pickshow_foobarrier);

  if(Local1animategroup) ianimate = 1;
  else if(Local2animategroup) ianimate = 2;

  if(Localignorablegroup) 
  {/*insure flag is set*/
     pickedgrupptr->KKIND = pickedgrupptr->KKIND | IGNORABLEKKIND;
  }
  else
  {/*insure flag is NOT set with two's complement of flag*/
     pickedgrupptr->KKIND = pickedgrupptr->KKIND & ~IGNORABLEKKIND;
  }
  if(Localfoobarriergroup) /*090912*/
  {/*insure flag is set*/
     pickedgrupptr->KKIND = pickedgrupptr->KKIND | FOOBARRIERKKIND;
  }
  else
  {/*insure flag is NOT set with two's complement of flag*/
     pickedgrupptr->KKIND = pickedgrupptr->KKIND & ~FOOBARRIERKKIND;
  }
     Lpruneactive = 1; /*global flag to ask for write when quitting*/
    if(iunpickable==1 ) pickedpointptr->type = pickedpointptr->type|UNPICKABLE ;
     /*cleanly either add or leave UNPICKABLE flag in place*/
     /*however, getting rid of a putative UNPICKABLE is more of a trick*/
     /*filter type through 15 bit mask with a no-copy at UNPICKABLE*/
     else pickedpointptr->type = (pickedpointptr->type & ~UNPICKABLE);

     /*ibuttshit stuff*/
         /*clear STATUS */
         /*REMEMBER: except for ON/OFF  (&mask == ON flag)*/
         /*RESERVED FOR FUTURE WHEN STATUS MIGHT INCLUDE ON FLAG*/
         pickedgrupptr->STATUS = 0;
         if(Localdomgroup == 1) 
            pickedgrupptr->STATUS = pickedgrupptr->STATUS | DOMINANT;
         if(Localnobuttongroup == 1) 
            pickedgrupptr->STATUS = pickedgrupptr->STATUS | NOBUTTON;
         pickedsgrpptr->STATUS = 0;
         if(Localdomsubgroup == 1) 
            pickedsgrpptr->STATUS = pickedsgrpptr->STATUS | DOMINANT;
         if(Localnobuttonsubgroup == 1) 
            pickedsgrpptr->STATUS = pickedsgrpptr->STATUS | NOBUTTON;
         if(Localneversubgroup==1) 
            pickedsgrpptr->STATUS = pickedsgrpptr->STATUS | NEVERSHOW;
         pickedlistptr->STATUS = 0;
         if(Localnobuttonlist ==1) 
            pickedlistptr->STATUS  = pickedlistptr->STATUS | NOBUTTON;

     if(iselectgroup==1)  /*061125*/
          { pickedgrupptr->KKIND = pickedgrupptr->KKIND |  SELECTINGKKIND; }
     else { pickedgrupptr->KKIND = pickedgrupptr->KKIND & ~SELECTINGKKIND; }
     if(iselectsubgroup==1)  /*061125*/
          { pickedsgrpptr->KKIND = pickedsgrpptr->KKIND |  SELECTINGKKIND; }
     else { pickedsgrpptr->KKIND = pickedsgrpptr->KKIND & ~SELECTINGKKIND; }
     if(iselectlist==1)  /*061125*/
          { pickedlistptr->KKIND = pickedlistptr->KKIND |  SELECTINGKKIND; }
     else { pickedlistptr->KKIND = pickedlistptr->KKIND & ~SELECTINGKKIND; }

     if(ideletegroup==1)
     {
        pickedgrupptr->on = pickedgrupptr->on-100;/*very off for output*/
        pickedgrupptr->STATUS = pickedgrupptr->STATUS | DOMINANT | NOBUTTON;
           /*nobutton && dominant: no buttons below*/
        pickedgrupptr->animate = 0;/*remove from any animation sequence*/
        if(   nbondrot > 0
           && pickedgrupptr->bondrot >= nbondrot) /*max should be just ==*/
        {/*group to be deleted is last rotatable group: prune back rotations*/
           if(pickedgrupptr->bondrotmin > 0)
           { 
              nbondrot = pickedgrupptr->bondrotmin - 1;
           }
           else
           {
              nbondrot = 0;
           }
           pickedgrupptr->bondrot = 0;
           pickedgrupptr->bondrotmin = 0;
           /*destroy the bondrot window*/
           /*which will destroy all of the bondrot sliders*/
           /*actually, it doesn't, so use a routine that does the dirty work*/
           /*XtDestroyWidget(topbrot);*/
           /*topbrot = NULL;*/ /*020617 safety*/
           if(topbrot != NULL)
           {
              destroybondrotwindow(); /*031001*/
           }
           /*force recreation of sliders for any bondrots that are left over*/
           isavewidth = 0;
           if(nbondrot > 0)
           {
              RWIDTH = ROTSWIDTH;
              adjustwindowwidth(); /*MACWIND.C,MPCWIND.C,MUXMWIND.c*/
              icurrentrotation = 1;  /*reset to first rotation*/
              /*so will always be defined either initially or by append*/
           }
           else
           {
              RWIDTH =   0;
              icurrentrotation = 0;
           }
        }/*group to be deleted is last rotatable group: prune back rotations*/
     }
     if(ideletesubgroup==1)
     {
         pickedsgrpptr->on = pickedsgrpptr->on-100;/*very off for output*/
         pickedsgrpptr->STATUS = pickedsgrpptr->STATUS | DOMINANT | NOBUTTON;
            /*nobutton && dominant: no buttons below*/
     }
     if(ideletelist==1)
     {                      /*very off for output*/
         pickedlistptr->on = pickedlistptr->on-100; 
         pickedlistptr->STATUS = pickedlistptr->STATUS | DOMINANT | NOBUTTON;
            /*nobutton for sure*/
     }

     thestr = (char *)XmTextGetString(pickshow_grpname);
           /*MAGE.H:groupname[][MAXNAMECHAR+2]*/
     pickedgrupptr->name[0] = ' '; /*initially, no animation*/
     pickedgrupptr->animate = 0;
     for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR+1;j++) 
     {
         pickedgrupptr->name[j+1] = thestr[j];
     }
     pickedgrupptr->name[j+1] = '\0'; /*j increased, points to '\0' */
     /*6.23: override by animate buttons   030220 */
     if(ianimate==2) {pickedgrupptr->name[0]='%'; pickedgrupptr->animate = 2;}
     if(ianimate==1) {pickedgrupptr->name[0]='*'; pickedgrupptr->animate = 1;}

     thestr = (char *)XmTextGetString(pickshow_sgrpname);
             /*MAGELIST.H:pickedsgrpptr->name[MAXNAMECHAR+1]*/
     for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR;j++) 
         pickedsgrpptr->name[j] = thestr[j];
     pickedsgrpptr->name[j] = '\0'; /*j increased, points to '\0' */

     thestr = (char *)XmTextGetString(pickshow_listname);
            /*MAGELIST.H:pickedlistptr->name[MAXNAMECHAR+1]*/
     for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR;j++) 
         pickedlistptr->name[j] = thestr[j];
     pickedlistptr->name[j] = '\0'; /*j increased, points to '\0' */

     thestr = (char *)XmTextGetString(pickshow_pointID);
              /*MAGE.h: ptIDstr[MAXPTIDCHAR+1]*/
     /*for(j=0;thestr[j]!='\0'&&j<MAXPTIDCHAR;j++) */
     for(j=0;thestr[j]!='\0';j++) /*980927 trust that thestr ends niCely*/
         comstr[j] = thestr[j];
     comstr[j] = '\0'; /*j increased, points to '\0' */

     /*first see if the ptID has really been changed*/
     ichanged = 0;
     getptIDstring(word,pickedpointptr);
     k=-1;
     do{
           k++;
           if(word[k] != comstr[k]) ichanged = 1;
       }while(word[k] != '\0');
     if(ichanged)
     {/*ptID changed*/
         k=-1;do{k++;}while(word[k]!='\0');
         j=-1;do{j++;}while(comstr[j]!='\0');
         if(j <= k)
         {/*store in old location*/
             k=-1;
             do{
                 k++;
                 (com[(pickedpointptr->nameoffset) + (long)k] = comstr[k]);
               }while(comstr[k] != '\0');
         }
         else
         {/*make new place to store ptID*/
            /*try to store characters directly into giant character array */
            storeptIDstring(comstr, pickedpointptr);
         }/*make new place to store ptID*/
     }/*ptID changed*/
   XtFree(thestr);

   if(Localsuppressgroup)
   {
      suppressthisgroup(pickedgrupptr); /*MAGEUTIL.c 030220*/
   }

    XtDestroyWidget(pickshowDLOG);/*030304 destroy before doing other things*/
    LneedSetsinGrafWindow = 1; 
    redrawvec();              /*MUXMDRAW.c*/
    /*XtDestroyWidget(pickshowDLOG);*/ 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___pickshow_OK_CB_________________________________________________________*/

/****pickshow_CANCEL_CB()****************************************************/
XtCallbackProc pickshow_CANCEL_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
    XtDestroyWidget(pickshowDLOG);/*030304 destroy before doing other things*/
    redrawvec();
    /*XtDestroyWidget(pickshowDLOG); */
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___pickshow_CANCEL_CB_____________________________________________________*/

/****DopickpointsDialog*******************************************************/
void DopickpointsDialog()  /* 061129 */
{
  int ncommon;

 if(!ACTIVE_DLOG)
 {
  /*overall DLOG box********************************/
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
  ncommon = n;
  XtSetArg(args[n], XmNwidth, 600); n++; /*was 210*/
  XtSetArg(args[n], XmNheight, 600); n++; /*was 250*/
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;

  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "SHOW PROPERTIES"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/

  pickshowDLOG = XmCreateFormDialog(grafwindow," ",args,n);
  XtManageChild(pickshowDLOG);

  /*pickshowBIGbox: row column container for everything*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNwidth, 210); n++;*/
/*  XtSetArg(args[n], XmNheight, 250); n++;*/
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshowBIGbox=XtCreateWidget(" ",xmRowColumnWidgetClass,pickshowDLOG,args,n);
  XtManageChild(pickshowBIGbox);

  /*Subject******************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 200); n++;
  XtSetArg(args[n], XmNrows, 3); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  pickshowtext = XtCreateWidget(" ", xmTextWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowtext); 

  /* A row of columns *************************************************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 280); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  pickshowrowA =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowrowA);

  /* B column for grp name **BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolB =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolB);

  /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++; /*disable 061129*/
  pickshow_grptitle = XmCreateLabel(pickshowcolB,"group name",args,n);
  XtManageChild(pickshow_grptitle);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  /*XtSetArg(args[n], XmNsensitive, False); n++;*/ /*disable 061129*/
  XtSetArg(args[n], XmNsensitive, True); n++; 
  pickshow_grpname = 
           XmCreateLabel(pickshowcolB,pickedgrupptr->name,args,n);
           /*XtCreateWidget(" ", xmTextWidgetClass, pickshowcolB,args,n);*/
  XtManageChild(pickshow_grpname);


 /* C column for sgrp name **CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolC =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolC);

 /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_sgrptitle = XmCreateLabel(pickshowcolC,"subgroup name",args,n);
  XtManageChild(pickshow_sgrptitle);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_sgrpname = 
           XmCreateLabel(pickshowcolC,pickedsgrpptr->name,args,n);
  XtManageChild(pickshow_sgrpname);


 /* D for list name  DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolD =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolD);

 /*name  */
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_listtitle = XmCreateLabel(pickshowcolD,"list name",args,n);
  XtManageChild(pickshow_listtitle);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  pickshow_listname = 
           XmCreateLabel(pickshowcolD,pickedlistptr->name,args,n);
  XtManageChild(pickshow_listname);

 /* E row for pointID name **EEEEEEEEEEEEEEEEEEEEEEEEEEEE*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 45); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  pickshowcolE =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowrowA,args,n);
  XtManageChild(pickshowcolE);

 /*name*/
  n = ncommon;  /*type of name*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_ptIDtitle = XmCreateLabel(pickshowcolE,"point IDentifier",args,n);
  XtManageChild(pickshow_ptIDtitle);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  getptIDstring(word, pickedpointptr);
  pickshow_pointID = 
           XmCreateLabel(pickshowcolE,word,args,n);
  XtManageChild(pickshow_pointID);

 /*always show point color*/ 
     if((pickedpointptr->colorwidth & 31) >0)
     {
        pickedpointcolor = (pickedpointptr->colorwidth & 31);
        interpretcolor(pickedpointcolor); /*returns color name in global word*/
     }
     else
     {
        pickedpointcolor = 0; /*NO point color*/
        sprintf(word,"NO point color");
     }
  n = ncommon;  /*dummy*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  pickshow_pointcolor = XmCreateLabel(pickshowcolE,word,args,n);
  XtManageChild(pickshow_pointcolor);

 if(Lmovepointson || Lmeansigmason)
 {

  /*SOURCE---TARGET notes ******************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 200); n++;
  XtSetArg(args[n], XmNrows, 3); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  pickshow_srctrgnotes = XtCreateWidget(" ", xmTextWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshow_srctrgnotes); 

  /*source group,subgroup,list names */
  /* M row of names *************************************************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 280); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  pickshowrowM =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowrowM);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_srcgrupname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowM,args,n);
  XtManageChild(pickshow_srcgrupname);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_srcsgrpname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowM,args,n);
  XtManageChild(pickshow_srcsgrpname);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_srclistname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowM,args,n);
  XtManageChild(pickshow_srclistname);

  n = ncommon;  /*SOURCE title*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_srctitle = 
       XtCreateWidget("SOURCE", xmTextWidgetClass,  pickshowrowM,args,n);
  XtManageChild(pickshow_srctitle);

  /*target group,subgroup,list names */
  /* N row of names *************************************************/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 280); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  pickshowrowN =
        XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowrowN);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_trggrupname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowN,args,n);
  XtAddCallback(pickshow_trggrupname,XmNvalueChangedCallback
                                ,(XtCallbackProc)pickshow_EDITactive_CB, NULL); 
  XtManageChild(pickshow_trggrupname);

  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_trgsgrpname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowN,args,n);
  XtAddCallback(pickshow_trgsgrpname,XmNvalueChangedCallback
                                ,(XtCallbackProc)pickshow_EDITactive_CB, NULL); 
  XtManageChild(pickshow_trgsgrpname);

  /*move points to a designated list; mean,sigmas are in one designated list*/
  n = ncommon; 
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, False); n++;
  pickshow_trglistname = 
           XtCreateWidget(" ", xmTextWidgetClass,  pickshowrowN,args,n);
  XtAddCallback(pickshow_trglistname,XmNvalueChangedCallback
                                ,(XtCallbackProc)pickshow_EDITactive_CB, NULL); 
  XtManageChild(pickshow_trglistname);
 

  n = ncommon;  /*TARGET title*/
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNsensitive, True); n++;
  if(Lmovepointson)
     { pickshow_trgtitle = XmCreateLabel(pickshowrowN,"TARGET for points",args,n); }
  else if(Lmeansigmason)
     { pickshow_trgtitle = XmCreateLabel(pickshowrowN,"TARGET for mean,sigmas",args,n); }
  XtManageChild(pickshow_trgtitle);

  /*  row of radio buttons */
  n = ncommon; 
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  pickshowrowradios
      = XtCreateWidget(" ", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshowrowradios);

  /*radio box D */
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_RADIOD = XmCreateToggleButtonGadget(pickshowrowradios,"define new group and",args,n);
  XtAddCallback(pickshow_RADIOD,XmNvalueChangedCallback
                                  ,(XtCallbackProc)pickshow_RADIOD_CB, NULL); 
  XtManageChild(pickshow_RADIOD);

  /*radio box C */
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_RADIOC = XmCreateToggleButtonGadget(pickshowrowradios,"define new subgroup and",args,n);
  XtAddCallback(pickshow_RADIOC,XmNvalueChangedCallback
                                  ,(XtCallbackProc)pickshow_RADIOC_CB, NULL); 
  XtManageChild(pickshow_RADIOC);

  /*move points to a list; mean,sigmas are in one list*/
  /*radio box B */
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_RADIOB = XmCreateToggleButtonGadget(pickshowrowradios,"define new list",args,n);
  XtAddCallback(pickshow_RADIOB,XmNvalueChangedCallback
                                  ,(XtCallbackProc)pickshow_RADIOB_CB, NULL); 
  XtManageChild(pickshow_RADIOB);
 
  /*radio box A */
  n = ncommon; 
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  pickshow_RADIOA = XmCreateToggleButtonGadget(pickshowrowradios,"use this list",args,n);
  XtAddCallback(pickshow_RADIOA,XmNvalueChangedCallback
                                  ,(XtCallbackProc)pickshow_RADIOA_CB, NULL); 
  XtManageChild(pickshow_RADIOA);

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_NEWTARGET = 
      XmCreatePushButtonGadget(pickshowrowradios,"accept TARGET definition",args,n);
  XtAddCallback(pickshow_NEWTARGET,XmNactivateCallback
       ,(XtCallbackProc)pickshow_NEWTARGET_CB,NULL); 
  XtManageChild(pickshow_NEWTARGET);

  /*control buttons: SOURCE TARGET *****************************************/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  pickshow_srctrgbuttons = 
           XtCreateWidget("but", xmRowColumnWidgetClass,pickshowBIGbox,args,n);
  XtManageChild(pickshow_srctrgbuttons);
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_SOURCE = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"picked becomes SOURCE",args,n);
  XtAddCallback(pickshow_SOURCE,XmNactivateCallback
       ,(XtCallbackProc)pickshow_SOURCE_CB,NULL); 
  XtManageChild(pickshow_SOURCE);
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_TARGET = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"picked becomes TARGET",args,n);
  XtAddCallback(pickshow_TARGET,XmNactivateCallback
       ,(XtCallbackProc)pickshow_TARGET_CB,NULL); 
  XtManageChild(pickshow_TARGET);

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_getTARGET = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"pick a (template)TARGET",args,n);
  XtAddCallback(pickshow_getTARGET,XmNactivateCallback
       ,(XtCallbackProc)pickshow_CANCEL_CB,NULL); /*leave dlog for now...*/
  XtManageChild(pickshow_getTARGET); /*expect to invoke show-object-properties*/

   if(Lmovepointson)
   {
    n = ncommon;  /*move points SOURCE to TARGET*/
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    pickshow_srctrgMOVE = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"NO TARGET",args,n);
                                            /* GO: move POINTs! */
    XtAddCallback(pickshow_srctrgMOVE,XmNactivateCallback
       ,(XtCallbackProc)pickshow_MOVE_CB,NULL); /*closes dlog */
    XtManageChild(pickshow_srctrgMOVE); /* does move */
   }
   else if(Lmeansigmason)
   {
    n = ncommon;  /*CALC mean sigmas in SOURCE, store in meansigmasTARGET*/
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    pickshow_srctrgCALC = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"NO TARGET",args,n);
                                            /* CALC mean,sigmas */
    XtAddCallback(pickshow_srctrgCALC,XmNactivateCallback
       ,(XtCallbackProc)pickshow_MEANSIGMAS_CB,NULL); /*closes dlog */
    XtManageChild(pickshow_srctrgCALC); /* does calculation */
   }

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  pickshow_CONTINUE = 
      XmCreatePushButtonGadget(pickshow_srctrgbuttons,"continue",args,n);
  XtAddCallback(pickshow_CONTINUE,XmNactivateCallback
       ,(XtCallbackProc)pickshow_CANCEL_CB,NULL); /*leave dlog for now...*/
  XtManageChild(pickshow_CONTINUE); /*expect to invoke show-object-properties*/

 } /*Lmovepointson || Lmeansigmason)*/

  ACTIVE_DLOG = PICKPOINTS_DLOG;
  pickpointsDialog(1);
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DopickpointsDialog______________________________________________________*/

/****pickpointsDialog()*******************************************************/
void pickpointsDialog(int Lok)
{
 XmString    xmstring;
 int         thestate=0;
int        Localdomgroup=0,Localdomsubgroup=0,Localneversubgroup=0;
int        Localnobuttongroup=0,Localnobuttonsubgroup=0,Localnobuttonlist=0;
int        ideletegroup=0,ideletesubgroup=0,ideletelist=0;
int        iselectgroup=0,iselectsubgroup=0,iselectlist=0;  /*061125*/
int        iunpickable=0;
int        Local1animategroup=0, Local2animategroup=0;
int        Localsuppressgroup=0, Localignorablegroup=0, Localfoobarriergroup=0;
int        j=0, LtargetlistOK = 1, LtargetsgrpOK = 1, LtargetgrupOK = 1;
int        Lproposedname=0;

Lreporteditactivity = 0; /*re Ltargeteditactivity MUXMDLOG.c file logical flag*/
if(Lreporteditactivity)
{
fprintf(stderr,"enter pickpointsDialog with Lok== %d, Lmovepointson== %d, Lmeansigmason== %d, LtargetlistOK== %d\n",Lok,Lmovepointson,Lmeansigmason,LtargetlistOK);
}

   if(Lmovepointson && Lok)
   {
     sprintf(word, "Redefine Point Groupings:  Hierarchy of the last picked point.\n can be made the SOURCE group,subgroup,list as is,\n can become TARGET group,subgroup,list or new TARGET list, or subgroup, or group created for moved points.");
   }
   else if(Lmeansigmason && Lok)
   {
     sprintf(word, "Calc Mean & Sigmas:  Hierarchy of the last picked point.\n can be made the SOURCE group,subgroup,list as is,\n can become TARGET group,subgroup,list or new TARGET list, or subgroup, or group created for mean & sigmas.");
   }
   else /*NOT Lok*/
   {
      sprintf(word, "NOT OK:  --- probably problem allocating TARGET list");
   }
     /*XmTextReplace(pickshowtext,0,XmTextGetLastPosition(pickshowtext),word);*/
     XmTextSetString(pickshowtext,word);

  if(Lok)
  {
     /*SOURCE group,subgroup,list names 061124 */
     if(sourcegrupptr != NULL)
     {
        sprintf(word,"%s",sourcegrupptr->name);
        /*group name extra leading character, '*' or '%' or ' ', shows animate*/
        /*Dialog explicit animate choice so do not show this 1st char here*/
        for(j=1; j<=MAXNAMECHAR+2; j++)/*just copy it all, word[iword]='\0'*/
        {
           temps[j-1] = word[j];/*group name starts at char 1*/
        }
     }
     else
     {
        temps[0] = '\0';
     }
     XmTextReplace(             pickshow_srcgrupname,0
         ,XmTextGetLastPosition(pickshow_srcgrupname),temps);
     
     if(sourcesgrpptr != NULL)
          { sprintf(word,"%s",sourcesgrpptr->name); }
     else { word[0] = '\0'; }
     XmTextReplace(             pickshow_srcsgrpname,0
         ,XmTextGetLastPosition(pickshow_srcsgrpname),word);
     if(sourcelistptr != NULL)
          { sprintf(word,"%s",sourcelistptr->name); }
     else { word[0] = '\0'; }
     XmTextReplace(             pickshow_srclistname,0
         ,XmTextGetLastPosition(pickshow_srclistname),word);
     if(sourcepointcolor >0)
     {
        interpretcolor(sourcepointcolor); /*returns color name in global word*/
        sprintf(temps,"SOURCE %s",word);
     }
     else
     {
        sprintf(temps,"SOURCE: NO point color");
     }
     XmTextReplace(             pickshow_srctitle,0
         ,XmTextGetLastPosition(pickshow_srctitle),temps);

     if(Lmovepointson)
     {
        /*SOURCE TARGET notes*/
        sprintf(word, "pick SOURCE (group,subgroup,list) before making new TARGET (group,subgroup,list) \n TARGET list must have same dimensions as SOURCE list");
        XmTextSetString(pickshow_srctrgnotes,word);

        /*movepoints TARGET group,subgroup,list names 061124 */
        LtargetgrupOK = 1; /*presume OK, see if can sustain that presumption */
        LtargetsgrpOK = 1; /*presume OK, see if can sustain that presumption */
        LtargetlistOK = 1; /*presume OK, see if can sustain that presumption */
        if(targetgrupptr != NULL)
        {
           sprintf(word,"%s",targetgrupptr->name);
            /*group name leading character, '*' or '%' or ' ', shows animate*/
            /*Dialog explicit animate choice so do not show this 1st char here*/
           for(j=1; j<=MAXNAMECHAR+2; j++)/*just copy it all, word[iword]='\0'*/
              { temps[j-1] = word[j];/*group name starts at char 1*/ }
        }
        else /* GROUP NOT OK */
        { 
           temps[0] = '\0';  LtargetgrupOK = 0; 
if(Lreporteditactivity)
{fprintf(stderr,"GROUP NOT OK: LtargetgrupOK== %d\n",LtargetgrupOK);}
        }  
        XmTextReplace(             pickshow_trggrupname,0
         ,XmTextGetLastPosition(pickshow_trggrupname),temps);
     
        if(targetsgrpptr != NULL)
             { sprintf(word,"%s",targetsgrpptr->name); }
        else /* SUBGROUP NOT OK */
        { 
           word[0] = '\0';  LtargetsgrpOK = 0; 
if(Lreporteditactivity)
{fprintf(stderr,"SUBGROUP NOT OK: LtargetsgrpOK== %d\n",LtargetsgrpOK);}
        }  
        XmTextReplace(             pickshow_trgsgrpname,0
            ,XmTextGetLastPosition(pickshow_trgsgrpname),word);

        if(targetlistptr != NULL)
             { sprintf(word,"%s",targetlistptr->name); }
        else /* LIST NOT OK */
        { 
           word[0] = '\0';  LtargetlistOK = 0; 
if(Lreporteditactivity)
{fprintf(stderr,"LIST NOT OK: LtargetlistOK== %d\n",LtargetlistOK);}
        }  
        XmTextReplace(             pickshow_trglistname,0
            ,XmTextGetLastPosition(pickshow_trglistname),word);

if(Lreporteditactivity)
{fprintf(stderr,"CRITICAL: Ltargeteditactivity== %d, LtargetgrupOK== %d, LtargetsgrpOK== %d, LtargetlistOK== %d, targetlistptr== %p\n",Ltargeteditactivity,LtargetgrupOK,LtargetsgrpOK,LtargetlistOK,targetlistptr);} /*121108 int and pointer*/

        if(targetlistptr != NULL && LtargetlistOK)
             { sprintf(temps,"GO: move POINTs!"); thestate = 1;}
        else { sprintf(temps,"target not defined"); thestate = 0;}
        xmstring = XmStringCreateSimple(temps); 
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,thestate,NULL);
        XmStringFree(xmstring);

     }/*Lmovepointson*/

     else if(Lmeansigmason)
     {

        /*SOURCE TARGET notes*/
        sprintf(word, "pick SOURCE (group,subgroup,list) before making new TARGET (group,subgroup,list) \n TARGET list must have same dimensions as SOURCE list\n separate lists: ringlist for average, vectorlist for sigma-lines");
        XmTextSetString(pickshow_srctrgnotes,word);

        /*meansigmas TARGET group,subgroup,list names 061126 */
        LtargetgrupOK = 1; /*presume OK, see if can sustain that presumption */
        LtargetsgrpOK = 1; /*presume OK, see if can sustain that presumption */
        LtargetlistOK = 1; /*presume OK, see if can sustain that presumption */
        if(meansigmasgrupptr != NULL)
        {
           sprintf(word,"%s",meansigmasgrupptr->name);
           /*group name leading character, '*' or '%' or ' ', shows animate*/
           /*Dialog explicit animate choice so do not show this 1st char here*/
           for(j=1; j<=MAXNAMECHAR+2; j++)/*just copy it all, word[iword]='\0'*/
           {
           temps[j-1] = word[j];/*group name starts at char 1*/
           }
        }
        else /* GROUP NOT OK */
        { 
           temps[0] = '\0';  LtargetgrupOK = 0; 
if(Lreporteditactivity)
{fprintf(stderr,"GROUP NOT OK: LtargetgrupOK== %d\n",LtargetgrupOK);}
        }  
        XmTextReplace(             pickshow_trggrupname,0
         ,XmTextGetLastPosition(pickshow_trggrupname),temps);
     
        if(meansigmassgrpptr != NULL)
             { sprintf(word,"%s",meansigmassgrpptr->name); }
        else /* SUBGROUP NOT OK */
        { 
           word[0] = '\0';  LtargetsgrpOK = 0; 
if(Lreporteditactivity)
{fprintf(stderr,"SUBGROUP NOT OK: LtargetsgrpOK== %d\n",LtargetsgrpOK);}
        }  
        XmTextReplace(             pickshow_trgsgrpname,0
            ,XmTextGetLastPosition(pickshow_trgsgrpname),word);

        /* designated target list, mean,sigmas in separate lists*/
        if(meansigmaslistptr != NULL) 
        {
           if(   (meansigmaslistptr->KKIND & AVERAGEKKIND)
              && (meansigmaslistptr->nextptr->KKIND & SIGMASKKIND) )
           {/*1st of two valid lists in proper order*/
              ;
           }
           else if(   (meansigmaslistptr->KKIND & SIGMASKKIND)
                   && (meansigmaslistptr->previousptr->KKIND & AVERAGEKKIND) )
           {/*2nd of two valid lists in proper order*/
              /*redefine meansigmaslistptr to point to average (mean) list*/
              meansigmaslistptr = meansigmaslistptr->previousptr;
           }
           else /* LIST NOT OK */
           { 
              word[0] = '\0';  LtargetlistOK = 0; 
if(Lreporteditactivity)
{
fprintf(stderr,"LIST NOT OK, wrong lists: LtargetlistOK== %d\n",LtargetlistOK);
fprintf(stderr,"meansigmaslistptr== %p, meansigmaslistptr->nextptr== %p\n",meansigmaslistptr,meansigmaslistptr->nextptr); /*121108 %p for pointers */
}
           }  
        }
        else /* LIST NOT OK */
        { 
           word[0] = '\0';  LtargetlistOK = 0; 
if(Lreporteditactivity)
{
fprintf(stderr,"LIST NOT OK, not defined: LtargetlistOK== %d\n",LtargetlistOK);
}
        }  
  
        if(LtargetgrupOK && LtargetsgrpOK && LtargetlistOK)
             {   sprintf(word,"%s",meansigmaslistptr->name); Lproposedname=0; }
        else if(   LtargetgrupOK && LtargetsgrpOK && !LtargetlistOK 
                && (sourcelistptr != NULL) ) 
             {/*only the list NOT OK, propose specific name based on source*/
                 sprintf(word,"avsg %s",sourcelistptr->name); Lproposedname=1; }
        else if(   LtargetgrupOK && LtargetsgrpOK && !LtargetlistOK )
             {/*only the list NOT OK, propose generic name */
                 sprintf(word,"avsg"); Lproposedname=1; }
        else /*target not well enough defined to do anything with it */
             {   word[0] = '\0'; Lproposedname=0; }
        XmTextReplace(             pickshow_trglistname,0
            ,XmTextGetLastPosition(pickshow_trglistname),word);

if(Lreporteditactivity)
{fprintf(stderr,"CRITICAL: Ltargeteditactivity== %d, LtargetgrupOK== %d, LtargetsgrpOK== %d, LtargetlistOK== %d, targetlistptr== %p\n",Ltargeteditactivity,LtargetgrupOK,LtargetsgrpOK,LtargetlistOK,targetlistptr);} /*121108 %d and %p */

        if(    meansigmaslistptr != NULL 
            && !Ltargeteditactivity
            && LtargetgrupOK && LtargetsgrpOK && LtargetlistOK )
        { 
           sprintf(temps,"GO: calc meansigmas"); 
           thestate = 1;
        }
        else 
        { 
           sprintf(temps,"target not defined"); 
           thestate = 0;
        }
        xmstring = XmStringCreateSimple(temps); 
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XmStringFree(xmstring);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,thestate,NULL);

     }/*Lmeansigmason*/

     if(LtargetlistOK)
     {
        pickshow_radioAint = 1;
        pickshow_radioBint = 0;
        XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
     }
     else if(Lproposedname)
     {
        pickshow_radioAint = 0;
        pickshow_radioBint = 1;
        XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
        XtVaSetValues(pickshow_trglistname,XmNsensitive,1,NULL); 
     }
     else
     {
        pickshow_radioAint = 0;
        pickshow_radioBint = 0;
        XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,0,NULL);
        XtVaSetValues(pickshow_trglistname,XmNsensitive,0,NULL); 
     }
     pickshow_radioCint = 0;
     pickshow_radioDint = 0;

     sprintf(word,"use this list"); xmstring = XmStringCreateSimple(word);
     XtVaSetValues(         pickshow_RADIOA,XmNlabelString,xmstring,NULL);
     XmToggleButtonSetState(pickshow_RADIOA,pickshow_radioAint, 0);
     XmToggleButtonSetState(pickshow_RADIOB,pickshow_radioBint, 0);
     XmToggleButtonSetState(pickshow_RADIOC,pickshow_radioCint, 0);
     XmToggleButtonSetState(pickshow_RADIOD,pickshow_radioDint, 0);

  }/*Lok*/
}
/*___pickpointsDialog()______________________________________________________*/

/****pickshow_EDITactive_CB()************************************************/
XtCallbackProc pickshow_EDITactive_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
  XmString    xmstring;
   Ltargeteditactivity++;
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_EDITactive_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
   sprintf(temps,"target not defined"); 
   xmstring = XmStringCreateSimple(temps); 
   if(Lmovepointson)
   {
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,0,NULL);
   }
   else if(Lmeansigmason)
   {
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,0,NULL);
   }
   XmStringFree(xmstring);
   return(NULL);
}
/*___pickshow_EDITactive_CB()_______________________________________________*/

/****pickshow_RADIOA_CB()****************************************************/
XtCallbackProc pickshow_RADIOA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{  /*option keep this group, subgroup, and list*/
  XmString    xmstring;
   XmToggleButtonSetState(pickshow_RADIOA,pickshow_radioAint=1, 0); /*NO EDIT*/
   XmToggleButtonSetState(pickshow_RADIOB,pickshow_radioBint=0, 0); /*list*/
   XmToggleButtonSetState(pickshow_RADIOC,pickshow_radioCint=0, 0); /*subgroup*/
   XmToggleButtonSetState(pickshow_RADIOD,pickshow_radioDint=0, 0); /*group*/
   XtVaSetValues(pickshow_trglistname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_trgsgrpname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_trggrupname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
   Ltargeteditactivity = 1;
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_RADIOA_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
   sprintf(temps,"target not defined"); 
   xmstring = XmStringCreateSimple(temps); 
   if(Lmovepointson)
   {
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,0,NULL);
   }
   else if(Lmeansigmason)
   {
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,0,NULL);
   }
   XmStringFree(xmstring);
   return(NULL);
}
/*___pickshow_RADIOA_CB()___________________________________________________*/

/****pickshow_RADIOB_CB()****************************************************/
XtCallbackProc pickshow_RADIOB_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{  /*option for new list*/
  XmString    xmstring;
   XmToggleButtonSetState(pickshow_RADIOA,pickshow_radioAint=0, 0); /*no edit*/
   XmToggleButtonSetState(pickshow_RADIOB,pickshow_radioBint=1, 0); /*LIST*/
   XmToggleButtonSetState(pickshow_RADIOC,pickshow_radioCint=0, 0); /*subgroup*/
   XmToggleButtonSetState(pickshow_RADIOD,pickshow_radioDint=0, 0); /*group*/
   XtVaSetValues(pickshow_trglistname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_trgsgrpname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_trggrupname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
   Ltargeteditactivity = 1;
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_RADIOB_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
   sprintf(temps,"target not defined"); 
   xmstring = XmStringCreateSimple(temps); 
   if(Lmovepointson)
   {
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,0,NULL);
   }
   else if(Lmeansigmason)
   {
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,0,NULL);
   }
   XmStringFree(xmstring);
   return(NULL);
}
/*___pickshow_RADIOB_CB()___________________________________________________*/

/****pickshow_RADIOC_CB()****************************************************/
XtCallbackProc pickshow_RADIOC_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{  /*option for new subgroup, and list*/
  XmString    xmstring;
   XmToggleButtonSetState(pickshow_RADIOA,pickshow_radioAint=0, 0); /*no edit*/
   XmToggleButtonSetState(pickshow_RADIOB,pickshow_radioBint=1, 0); /*LIST*/
   XmToggleButtonSetState(pickshow_RADIOC,pickshow_radioCint=1, 0); /*SUBGROUP*/
   XmToggleButtonSetState(pickshow_RADIOD,pickshow_radioDint=0, 0); /*group*/
   XtVaSetValues(pickshow_trglistname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_trgsgrpname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_trggrupname,XmNsensitive,0,NULL);
   XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
   Ltargeteditactivity = 1;
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_RADIOC_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
   sprintf(temps,"target not defined"); 
   xmstring = XmStringCreateSimple(temps); 
   if(Lmovepointson)
   {
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,0,NULL);
   }
   else if(Lmeansigmason)
   {
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,0,NULL);
   }
   XmStringFree(xmstring);
   return(NULL);
}
/*___pickshow_RADIOC_CB()___________________________________________________*/

/****pickshow_RADIOD_CB()****************************************************/
XtCallbackProc pickshow_RADIOD_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{  /*option for new group, subgroup, and list*/ 
  XmString    xmstring;
   XmToggleButtonSetState(pickshow_RADIOA,pickshow_radioAint=0, 0); /*no edit*/
   XmToggleButtonSetState(pickshow_RADIOB,pickshow_radioBint=1, 0); /*LIST*/
   XmToggleButtonSetState(pickshow_RADIOC,pickshow_radioCint=1, 0); /*SUBGROUP*/
   XmToggleButtonSetState(pickshow_RADIOD,pickshow_radioDint=1, 0); /*GROUP*/
   XtVaSetValues(pickshow_trglistname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_trgsgrpname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_trggrupname,XmNsensitive,1,NULL);
   XtVaSetValues(pickshow_NEWTARGET,XmNsensitive,1,NULL);
   Ltargeteditactivity = 1;
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_RADIOD_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
   sprintf(temps,"target not defined"); 
   xmstring = XmStringCreateSimple(temps); 
   if(Lmovepointson)
   {
        XtVaSetValues(pickshow_srctrgMOVE,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgMOVE,XmNsensitive,0,NULL);
   }
   else if(Lmeansigmason)
   {
        XtVaSetValues(pickshow_srctrgCALC,XmNlabelString,xmstring,NULL);
        XtVaSetValues(pickshow_srctrgCALC,XmNsensitive,0,NULL);
   }
   XmStringFree(xmstring);
   return(NULL);
}
/*___pickshow_RADIOD_CB()___________________________________________________*/

/****pickshow_SOURCE_CB()****************************************************/
XtCallbackProc pickshow_SOURCE_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{  /*picked becomes SOURCE*/
   sourcegrupptr = pickedgrupptr;
   sourcesgrpptr = pickedsgrpptr;
   sourcelistptr = pickedlistptr;
   sourcepointcolor = pickedpointcolor;
   pickpointsDialog(1);
   return(NULL); /*121108*/
}
/*___pickshow_SOURCE_CB()___________________________________________________*/

/****pickshow_TARGET_CB()****************************************************/
XtCallbackProc pickshow_TARGET_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{  /*picked becomes TARGET*/
   if(Lmovepointson)
   {
      targetgrupptr = pickedgrupptr;
      targetsgrpptr = pickedsgrpptr;
      targetlistptr = pickedlistptr;
   }
   else if(Lmeansigmason)
   {
      meansigmasgrupptr = pickedgrupptr;
      meansigmassgrpptr = pickedsgrpptr;
      meansigmaslistptr = pickedlistptr;
   }
   Ltargeteditactivity = -3; /*dialog update triggers editactivity*/
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_TARGET_CB sets Ltargeteditactivity= %d\n",Ltargeteditactivity);}
        XtVaSetValues(pickshow_trglistname,XmNsensitive,0,NULL); /*uneditable*/
        XtVaSetValues(pickshow_trgsgrpname,XmNsensitive,0,NULL); /*uneditable*/
        XtVaSetValues(pickshow_trggrupname,XmNsensitive,0,NULL); /*uneditable*/
   pickpointsDialog(1);
   return(NULL); /*121108*/
}
/*___pickshow_TARGET_CB()___________________________________________________*/

/****pickshow_NEWTARGET_CB()*************************************************/
XtCallbackProc pickshow_NEWTARGET_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{  /*accept TARGET definition    i.e. go make new target wrt radios */
   char *thestr;
   char trggrupname[MAXNAMECHAR+2];
   char trgsgrpname[MAXNAMECHAR+1];
   char trglistname[MAXNAMECHAR+1];
   int j=0,Lneedgrup=0, Lneedsgrp=0, Lneedlist=0, Lok=1;
   grupstruct* trygrupptr = NULL;
   sgrpstruct* trysgrpptr = NULL;

   if(sourcelistptr == NULL)
   {
      fprintf(stderr,"no SOURCE, target dimensions undefined\n");
   }
   else
   {/*source list defined, OK to make target*/
      thestr = (char *)XmTextGetString(pickshow_trggrupname);
            /*MAGE.H:groupname[][MAXNAMECHAR+2]*/
      trggrupname[0] = ' '; /*initially, no animation*/
      for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR+1;j++) /* doing j+1 */
      {
          trggrupname[j+1] = thestr[j];
      }
      trggrupname[j+1] = '\0'; /*j increased, points to '\0' */

      thestr = (char *)XmTextGetString(pickshow_trgsgrpname);
            /*MAGE.H:subgroupname[][MAXNAMECHAR+1]*/
      for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR+1;j++) 
      {
          trgsgrpname[j] = thestr[j];
      }
      trgsgrpname[j] = '\0'; /*j increased, points to '\0' */

      thestr = (char *)XmTextGetString(pickshow_trglistname);
            /*MAGE.H:listname[][MAXNAMECHAR+1]*/
      for(j=0;thestr[j]!='\0'&&j<MAXNAMECHAR+1;j++) 
      {
          trglistname[j] = thestr[j];
      }
      trglistname[j] = '\0'; /*j increased, points to '\0' */

      if(pickshow_radioDint) 
      {
         Lneedgrup=1; Lneedsgrp=1; Lneedlist=1;
      }
      else if(pickshow_radioCint) 
      {
         Lneedsgrp=1; Lneedlist=1;
      }
      else if(pickshow_radioBint) 
      {
         Lneedlist=1;
      }
      else /*pickshow_radioAint*/
      {
         ; /* No needs: target group,subgroup,list stay as set before*/
      }
      /*"need's" routines call alloc routines that make "this" general globals*/
      /* then here copy specific global from this general global*/
      /*safe to use ...on flags since can only get here from pickshow dialog*/
      Lok = 1; /*presume will be good*/
      if(Lneedgrup)
      {
         if(maketargetgrup(sourcegrupptr, trggrupname) )
         {
            if(Lmovepointson)
              {targetgrupptr = thisgrupptr;} 
            else if(Lmeansigmason)
              {meansigmasgrupptr = thisgrupptr;}
            Lok = 1;
            LneedSetsinGrafWindow = 1;
         }
         else { Lok = 0; }
      }
      if(Lneedsgrp && Lok)
      {
         if     (Lmovepointson) {trygrupptr = targetgrupptr;}
         else if(Lmeansigmason) {trygrupptr = meansigmasgrupptr;}
         if(maketargetsgrp(trygrupptr, sourcesgrpptr, trgsgrpname) )
         {
            if(Lmovepointson)
              {targetsgrpptr = thissgrpptr;}
            else if(Lmeansigmason)
              {meansigmassgrpptr = thissgrpptr;}
            Lok = 1;
            LneedSetsinGrafWindow = 1;
         }
         else { Lok = 0; }
      }
      if(Lneedlist && Lok)
      {
         if     (Lmovepointson) {trysgrpptr = targetsgrpptr;}

         else if(Lmeansigmason) {trysgrpptr = meansigmassgrpptr;}

         if(maketargetlist(trysgrpptr, sourcelistptr, trglistname) )
         { /*alloc new list referred to as global thislistptr */
            if(Lmovepointson)
            {
               targetlistptr = thislistptr;
               Lok = 1;
               
            }
            else if(Lmeansigmason)
            {  /* make TWO lists, use 1st targetlistptr for average(mean) */
               meansigmaslistptr = thislistptr; /* 1st LIST for average */
               /*target list inherits most properties from source list,*/
               /* however: */
               meansigmaslistptr->type  = MARKTYPE;
               meansigmaslistptr->STYLE = RINGSTYLE;
               meansigmaslistptr->KKIND = meansigmaslistptr->KKIND|AVERAGEKKIND;
               meansigmaslistptr->STATUS = meansigmaslistptr->STATUS | NOBUTTON;
               meansigmaslistptr->STATUS = 
                     meansigmaslistptr->STATUS & ~NOHILITEFLAG; /*not needed*/
               meansigmaslistptr->color = numberofcolor("gray");
               meansigmaslistptr->owncolor  = 1;
               meansigmaslistptr->width = 1;
/*
fprintf(stderr,"pickshow_NEWTARGET_CB: meansigmaslistptr== %0x, in trysgrpptr== %0x\n",meansigmaslistptr,trysgrpptr);
*/
               if(maketargetlist(trysgrpptr, sourcelistptr, trglistname) )
               { /* 2nd LIST for sigma extent bars in all directions*/
                  thislistptr->type   = VECTOR;
                  thislistptr->KKIND =  thislistptr->KKIND | SIGMASKKIND;
                  thislistptr->STATUS = thislistptr->STATUS | NOBUTTON;
                  thislistptr->STATUS = 
                       thislistptr->STATUS & ~NOHILITEFLAG; /*not needed*/
                  thislistptr->color  = numberofcolor("gray");
                  thislistptr->owncolor  = 1;
                  thislistptr->width  = 1;
 
                  Lok = 1; 
/*
fprintf(stderr,"pickshow_NEWTARGET_CB: meansigmaslistptr->nextptr== %0x, in trysgrpptr== %0x\n",meansigmaslistptr->nextptr,trysgrpptr);
*/
               }
               else { Lok = 0; }
            }
            LneedSetsinGrafWindow = 1;
         }
         else { Lok = 0; }
      }
      if(Lok)
      {
          SetsinGrafWindow();
/*
fprintf(stderr,"maketargetlist returned for trysgrpptr== %0x, trysgrpptr->firstlistptr== %0x, trysgrpptr->lastlistptr== %0x\n",trysgrpptr,trysgrpptr->firstlistptr,trysgrpptr->lastlistptr);
*/
      }
   }/*source list defined, OK to make target*/
   Ltargeteditactivity = -3; /*dialog update triggers editactivity*/
if(Lreporteditactivity)
{fprintf(stderr,"pickshow_NEWTARGET_CB sets Ltargeteditactivity= %d, Lok== %d\n",Ltargeteditactivity,Lok);}
   pickpointsDialog(Lok);
   return(NULL); /*121108*/
}  /*accept TARGET definition    i.e. go make new target wrt radios */
/*___pickshow_NEWTARGET_CB()_______________________________________________*/

/****pickshow_MOVE_CB()*****************************************************/
XtCallbackProc pickshow_MOVE_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
   if(sourcelistptr == NULL || targetlistptr == NULL)
   {
      fprintf(stderr,"no SOURCE or TARGET\n");
   }
   else
   {
      if(sourcelistptr->NDIM != targetlistptr->NDIM)
      {
         fprintf(stderr,"mismatched dimensions of SOURCE and TARGET\n");
      }
      else
      {
         /* GO: move the POINTs! */
         movepointsbycolor(sourcelistptr, targetlistptr, sourcepointcolor);
         XtDestroyWidget(pickshowDLOG);
         redrawvec();
         ACTIVE_DLOG = 0;
      }
   }
   return(NULL);  
}
/*___pickshow_MOVE_CB()____________________________________________________*/

/****pickshow_MEANSIGMAS_CB()*************************************************/
XtCallbackProc pickshow_MEANSIGMAS_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
   if(sourcelistptr == NULL || meansigmaslistptr == NULL)
   {
      fprintf(stderr,"no SOURCE list or mean&sigmas subgroup TARGET\n");
   }
   else
   {
     
     if(sourcelistptr->NDIM != meansigmaslistptr->NDIM)
     {
      fprintf(stderr,"mismatched dimensions of SOURCE and meansigmas TARGET\n");
     }
     else
     {
         /* GO: calc mean and sigmas */
/*fprintf(stderr,"GO:  calcmeansigmas(sourcelistptr, meansigmaslistptr)\n");*/
         calcmeansigmas(sourcelistptr, meansigmaslistptr); /*MAGEDLOG.c*/
         XtDestroyWidget(pickshowDLOG);

         Lreloadkinemage = 1;
         Lredrawvector   = 1;
         LneedSetsinGrafWindow = 1;
         ACTIVE_DLOG = 0;      
     }
   }
   return(NULL);  
}
/*___pickshow_MEANSIGMAS_CB()________________________________________________*/

/****DoconstructDialog()******************************************************/
void  DoconstructDialog() /* <- MUX_MAIN/pick_CB() */
{                
  int ncommon=0;
  int n=0;
  XmString    xmstring;

/*pick process goes through MUXMMAIN/pick_CB 2 times with Lpick set */
/*!ACTIVE_DLOG filter here only allows creating dialog box on first call*/
if(!ACTIVE_DLOG)
{/*no other dlog active*/
/*overall DLOG box*/
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
  XtSetArg(args[n], XmNautoUnmanage, False); n++;
  if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  ncommon = n;
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
/*  XtSetArg(args[n], XmNx, 10); n++;*/
/*  XtSetArg(args[n], XmNy, 10); n++;*/

  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "MAGE CONSTRUCTION"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/

  constructdlog = XmCreateFormDialog(grafwindow,"FORM",args,n);
  XtManageChild(constructdlog);

/*row column box for everything that packs contents in order of creation*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNwidth, 300); n++;*/ /*expected maximum width*/
/*  XtSetArg(args[n], XmNheight, 200); n++;*/ /*minimal nominal height*/
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  constructbox=XtCreateWidget("co",xmRowColumnWidgetClass,constructdlog,args,n);
  XtManageChild(constructbox);

/*Subject*/  /*121110 text NOT being written to subject box by XmTextReplace*/
  n = ncommon; 
  XtSetArg(args[n], XmNwidth, 300); n++; /*expected maximum width*/
  XtSetArg(args[n], XmNheight, 100); n++; /*minimal nominal height*/
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;                                  
  constructtext = XtCreateWidget(" ", xmTextWidgetClass,constructbox,args,n);
  XtManageChild(constructtext); 
  sprintf(word,"Construct line:           Distance,             Angle,"
                             "             Dihedral");
/*XmTextReplace(constructtext, 0, XmTextGetLastPosition(constructtext),word);*/
  XmTextSetString(constructtext,word); /*121110*/

/* A row of parameters & radio button for A set of parameters */
/* construction by distance, angle, dihedral*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowA=XtCreateWidget("A",xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowA);

  /*radio box A */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructRADIOA =  XmCreateToggleButtonGadget(constructrowA,"RA",args,n);
  XtManageChild(constructRADIOA);

  sprintf(word,"Last measures");
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(constructRADIOA,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XtAddCallback(constructRADIOA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_A_CB, NULL); 

  /*parameter values */
  n = ncommon; 
  constructdistA=XtCreateWidget(" ",xmTextWidgetClass,constructrowA,args,n);
  XtManageChild(constructdistA);
  sprintf(word," %.3f",distmeasure);
/*either XmTextReplace or XmTextSet works here*/
/*
  XmTextReplace(constructdistA,0,XmTextGetLastPosition(constructdistA),word);
*/
  XmTextSetString(constructdistA,word);
  XtAddCallback(constructdistA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructxA_CB,NULL); 
  /*put XtAddCallback AFTER the text setting, the Linux system crashes*/
  /*presumably related to using the SAME Callback function for the set*/
  /*of 4 related boxes so that the RADIO button will be set if any one*/
  /*of the boxes is modified: radio-button, distance, angle, dihedral */

  n = ncommon; 
  constructangleA = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowA,args,n);
  XtManageChild(constructangleA);
  sprintf(word,"%.3f",   anglemeasure);
  XmTextReplace(              constructangleA, 0
          ,XmTextGetLastPosition(constructangleA),word);
  XtAddCallback(constructangleA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructyA_CB,NULL); 
  n = ncommon; 
  constructdihedralA = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowA,args,n);
  XtManageChild(constructdihedralA);
  sprintf(word,"%.3f",dihedralmeasure);
  XmTextReplace(           constructdihedralA, 0
       ,XmTextGetLastPosition(constructdihedralA),word);
  XtAddCallback(constructdihedralA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructzA_CB,NULL); 
     
/* B row of parameters & radio button for B set of parameters */
/* construction by distance, angle, dihedral*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowB
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowB);

 /*radio box B */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructRADIOB =  XmCreateToggleButtonGadget(constructrowB," ",args,n);
  XtManageChild(constructRADIOB);
  sprintf(word,"C tetrahedral");
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructRADIOB
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XtAddCallback(constructRADIOB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_B_CB, NULL); 

  /*parameter values */
  n = ncommon; 
  constructdistB = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowB,args,n);
  XtManageChild(constructdistB);
  sprintf(word,"%.3f",    diststored);
  XmTextReplace(               constructdistB, 0
           ,XmTextGetLastPosition(constructdistB),word);
  XtAddCallback(constructdistB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructxB_CB,NULL); 

  n = ncommon; 
  constructangleB = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowB,args,n);
  XtManageChild(constructangleB);
  sprintf(word,"%.3f",   anglestored);
  XmTextReplace(              constructangleB, 0
          ,XmTextGetLastPosition(constructangleB),word);
  XtAddCallback(constructangleB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructyB_CB,NULL); 

  n = ncommon; 
  constructdihedralB = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowB,args,n);
  XtManageChild(constructdihedralB);
  sprintf(word,"%.3f",dihedralstored);
  XmTextReplace(           constructdihedralB, 0
       ,XmTextGetLastPosition(constructdihedralB),word);
  XtAddCallback(constructdihedralB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructzB_CB,NULL); 

/* C row of parameters & radio button for C set of parameters */
/* construction by distance, angle, dihedral*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowC
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowC);

  /*radio box C */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructRADIOC =  XmCreateToggleButtonGadget(constructrowC," ",args,n);
  XtManageChild(constructRADIOC);
  sprintf(word,"User specify:");
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructRADIOC
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XtAddCallback(constructRADIOC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_C_CB, NULL); 

  /*parameter values */
  n = ncommon; 
  constructdistC = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowC,args,n);
  XtManageChild(constructdistC);
  sprintf(word,"%.3f",    distuser);
  XmTextReplace(               constructdistC, 0
           ,XmTextGetLastPosition(constructdistC),word);
  XtAddCallback(constructdistC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructxC_CB,NULL); 

  n = ncommon; 
  constructangleC = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowC,args,n);
  XtManageChild(constructangleC);
  sprintf(word,"%.3f",   angleuser);
  XmTextReplace(              constructangleC, 0
          ,XmTextGetLastPosition(constructangleC),word);
  XtAddCallback(constructangleC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructyC_CB,NULL); 

  n = ncommon; 
  constructdihedralC = 
           XtCreateWidget(" ", xmTextWidgetClass,constructrowC,args,n);
  XtManageChild(constructdihedralC);
  sprintf(word,"%.3f",dihedraluser);
  XmTextReplace(           constructdihedralC, 0
       ,XmTextGetLastPosition(constructdihedralC),word);
  XtAddCallback(constructdihedralC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)constructzC_CB,NULL); 
     
/* D row box for one radio button for D choice draw perpendicular */
/* construction of perpendicular to a line from last point picked*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowD
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowD);

  /*radio box D */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructRADIOD =  XmCreateToggleButtonGadget(constructrowD," ",args,n);
  XtManageChild(constructRADIOD);
  sprintf(word,"Perpendicular from 3rd pt to line 1--2"); 
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructRADIOD
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XtAddCallback(constructRADIOD,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_D_CB, NULL); 


/* E row box for one check box for E choice draw dot instead of a line*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowE
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowE);

  /*check box E */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructCHECKE =  XmCreateToggleButtonGadget(constructrowE," ",args,n);
  XtAddCallback(constructCHECKE,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_E_CB, NULL); 
  XtManageChild(constructCHECKE);
  sprintf(word,"Dot at 4th pt instead of a line to it"); 
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructCHECKE
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XmToggleButtonSetState(constructCHECKE,Lconstructdot, 0);
     

/* F row box for one check box for F Multiple lines: dihedral*(360/dihedral) */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowF
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowF);

  /*check box F */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructCHECKF =  XmCreateToggleButtonGadget(constructrowF," ",args,n);
  XtAddCallback(constructCHECKF,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_F_CB, NULL); 
  XtManageChild(constructCHECKF);
  sprintf(word,"Multiple lines: dihedral*(360/dihedral)");
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructCHECKF
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XmToggleButtonSetState(constructCHECKF,LconstructMultiple, 0);


/* G row box for one check box for G choice extend by 1/cos(angle) */
/* extension from sphere to tangent plane*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowG
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowG);

  /*check box G */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructCHECKG =  XmCreateToggleButtonGadget(constructrowG," ",args,n);
  XtAddCallback(constructCHECKG,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_G_CB, NULL); 
  XtManageChild(constructCHECKG);
  sprintf(word,"Extend by 1/cos(angle): sphere to tangent plane"); 
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructCHECKG
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XmToggleButtonSetState(constructCHECKG,Lconstructprojection, 0);

/* H row box for one check box for H choice Ghost: not for written output */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  constructrowH
      = XtCreateWidget(" ", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructrowH);

  /*check box H */
  n = ncommon; 
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, windowborder); n++;
  constructCHECKH =  XmCreateToggleButtonGadget(constructrowH," ",args,n);
  XtAddCallback(constructCHECKH,XmNvalueChangedCallback
                                        ,(XtCallbackProc)construct_H_CB, NULL); 
  XtManageChild(constructCHECKH);
  sprintf(word,"Ghost: not for written output"); 
  xmstring=XmStringCreateSimple(word); 
  XtVaSetValues(              constructCHECKH
         ,XmNlabelString,xmstring,NULL);
  XmStringFree(xmstring);
  XmToggleButtonSetState(constructCHECKH,Ldrawnewghost=0, 0); 
        /*011007 always start with default false*/

/* row of control buttons: OK, Cancel */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  constructbuttons = 
             XtCreateWidget("but", xmRowColumnWidgetClass,constructbox,args,n);
  XtManageChild(constructbuttons); /*all DLOG have place for OK, etc. buttons*/

  /*control button: OK */
  n = ncommon; 
  construct_OK = XmCreatePushButtonGadget(constructbuttons,"OK",args,n);
  XtAddCallback(construct_OK,XmNactivateCallback
                               ,(XtCallbackProc)construct_OK_CB,NULL); 
  XtManageChild(construct_OK);

  /*control button: Cancel */
  n = ncommon; 
  construct_CANCEL = XmCreatePushButtonGadget(constructbuttons,"Cancel",args,n);
  XtAddCallback(construct_CANCEL, XmNactivateCallback
                                 ,(XtCallbackProc)construct_CANCEL_CB, NULL); 
  XtManageChild(construct_CANCEL); 

  /*Information box needed below OK,Cancel buttons for them to be robust*/
    n = ncommon; 
    XtSetArg(args[n], XmNwidth, 300); n++;
    XtSetArg(args[n], XmNheight, 20); n++;
    if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
    XtSetArg(args[n], XmNeditable, True); n++;
    DLOG_INFO = XtCreateWidget(" ", xmTextWidgetClass,constructbox,args,n);
    XtManageChild(DLOG_INFO);
/*    XtAddCallback(DLOG_INFO,XmNvalueChangedCallback */
/*                                        ,(XtCallbackProc)DLOG_INFO_CB,NULL);*/
/*    XmTextSetString(DLOG_INFO,dlog_infostr); */

  ACTIVE_DLOG = CONSTRUCT_DLOG;
  constructDialog(); /*protected from being called once ACTIVE_DLOG is set*/
 }/*no other dlog active*/
}
/*___DoconstructDialog()_____________________________________________________*/

/****constructDialog()********************************************************/
void constructDialog()
{
   intpass = 1; /*construct based on last measures is default*/
   constructsetstate(); /*sets radio button states*/
}
/*___constructDialog()_______________________________________________________*/

/****construct_A_CB()*********************************************************/
XtCallbackProc construct_A_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO A construction by distance, angle, dihedral*/
   intpass = 1;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___construct_A_CB()________________________________________________________*/
/****constructxA_CB()*********************************************************/
XtCallbackProc constructxA_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO A construction by distance, angle, dihedral*/
   intpass = 1;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructxA_CB()________________________________________________________*/
/****constructyA_CB()*********************************************************/
XtCallbackProc constructyA_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO A construction by distance, angle, dihedral*/
   intpass = 1;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructyA_CB()________________________________________________________*/
/****constructzA_CB()*********************************************************/
XtCallbackProc constructzA_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO A construction by distance, angle, dihedral*/
   intpass = 1;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructzA_CB()________________________________________________________*/

/****construct_B_CB()*********************************************************/
XtCallbackProc construct_B_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO B construction by distance, angle, dihedral*/
   intpass = 2;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___construct_B_CB()________________________________________________________*/
/****constructxB_CB()*********************************************************/
XtCallbackProc constructxB_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO B construction by distance, angle, dihedral*/
   intpass = 2;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructxB_CB()________________________________________________________*/
/****constructyB_CB()*********************************************************/
XtCallbackProc constructyB_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO B construction by distance, angle, dihedral*/
   intpass = 2;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructyB_CB()________________________________________________________*/
/****constructzB_CB()*********************************************************/
XtCallbackProc constructzB_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO B construction by distance, angle, dihedral*/
   intpass = 2;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructzB_CB()________________________________________________________*/

/****construct_C_CB()*********************************************************/
XtCallbackProc construct_C_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO C construction by distance, angle, dihedral*/
   intpass = 3;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___construct_C_CB()________________________________________________________*/
/****constructxC_CB()*********************************************************/
XtCallbackProc constructxC_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO C construction by distance, angle, dihedral*/
   intpass = 3;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructxC_CB()________________________________________________________*/
/****constructyC_CB()*********************************************************/
XtCallbackProc constructyC_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO C construction by distance, angle, dihedral*/
   intpass = 3;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructyC_CB()________________________________________________________*/
/****constructzC_CB()*********************************************************/
XtCallbackProc constructzC_CB(Widget w,XtPointer client_d,XtPointer call_data)
{/*RADIO C construction by distance, angle, dihedral*/
   intpass = 3;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___constructzC_CB()________________________________________________________*/

/****construct_D_CB()*********************************************************/
XtCallbackProc construct_D_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*RADIO D construction of perpendicular to a line*/
   intpass = 4;  /*any touch selects this set, intpass is global */
   constructsetstate(); /*text fields maintain themselves, retrieve later*/
   return(NULL);
}
/*___construct_D_CB()________________________________________________________*/

/****construct_E_CB()*********************************************************/
XtCallbackProc construct_E_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*CHECK E dot instead of a line*/
   Lconstructdot = !Lconstructdot;
   XmToggleButtonSetState(constructCHECKE,Lconstructdot, 0);
   return(NULL);
}
/*___construct_E_CB()________________________________________________________*/

/****construct_F_CB()*********************************************************/
XtCallbackProc construct_F_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*CHECK F multiple lines*/
   LconstructMultiple = !LconstructMultiple;
   XmToggleButtonSetState(constructCHECKF,LconstructMultiple, 0);
   return(NULL);
}
/*___construct_F_CB()________________________________________________________*/

/****construct_G_CB()*********************************************************/
XtCallbackProc construct_G_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*CHECK G extension from sphere to tangent plane*/
   Lconstructprojection = !Lconstructprojection;
   XmToggleButtonSetState(constructCHECKG,Lconstructprojection, 0);
   return(NULL);
}
/*___construct_G_CB()________________________________________________________*/

/****construct_H_CB()*********************************************************/
XtCallbackProc construct_H_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*CHECK H Ghost: not for written output*/
   Ldrawnewghost = !Ldrawnewghost;
   XmToggleButtonSetState(constructCHECKH,Ldrawnewghost, 0);
   return(NULL);
}
/*___construct_H_CB()________________________________________________________*/

/****constructsetstate()******************************************************/
void constructsetstate()
{
   /*select if state==True: (Widget,             state, notify) */
   XmToggleButtonSetState(constructRADIOA, intpass==1, 0);
   XmToggleButtonSetState(constructRADIOB, intpass==2, 0);
   XmToggleButtonSetState(constructRADIOC, intpass==3, 0);
   XmToggleButtonSetState(constructRADIOD, intpass==4, 0);
}
/*___constructsetstate()_____________________________________________________*/

/****construct_OK_CB()********************************************************/
XtCallbackProc construct_OK_CB(Widget w,XtPointer client_data
                                       ,XtPointer call_data)
{
 char *thestr;
 int  j;


 /* use globals:  numstr for dist, word for angle, temps for dihedral */
 if(intpass==1)
 {/*RADIO A construction by distance, angle, dihedral*/
     thestr = (char *)XmTextGetString(    constructdistA);
     for(j=0;thestr[j]!='\0';j++) numstr[j] = thestr[j];
     numstr[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(   constructangleA);
     for(j=0;thestr[j]!='\0';j++) word[j] = thestr[j];
     word[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(constructdihedralA);
     for(j=0;thestr[j]!='\0';j++) temps[j] = thestr[j];
     temps[j] = thestr[j]; /*j increased, points to '\0' */
     Lconstructperpendicular = 0; 
 }
 else if(intpass==2)
 {/*RADIO B construction by distance, angle, dihedral*/
     thestr = (char *)XmTextGetString(    constructdistB);
     for(j=0;thestr[j]!='\0';j++) numstr[j] = thestr[j];
     numstr[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(   constructangleB);
     for(j=0;thestr[j]!='\0';j++) word[j] = thestr[j];
     word[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(constructdihedralB);
     for(j=0;thestr[j]!='\0';j++) temps[j] = thestr[j];
     temps[j] = thestr[j]; /*j increased, points to '\0' */
     Lconstructperpendicular = 0;
 }
 else if(intpass==3)
 {/*RADIO C construction by distance, angle, dihedral*/
     thestr = (char *)XmTextGetString(    constructdistC);
     for(j=0;thestr[j]!='\0';j++) numstr[j] = thestr[j];
     numstr[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(   constructangleC);
     for(j=0;thestr[j]!='\0';j++) word[j] = thestr[j];
     word[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(constructdihedralC);
     for(j=0;thestr[j]!='\0';j++) temps[j] = thestr[j];
     temps[j] = thestr[j]; /*j increased, points to '\0' */
     Lconstructperpendicular = 0;
 }
 else if(intpass==4)
 {/*RADIO D construction of perpendicular to a line*/
     /*define thestr even though not going to use it, else*/
     /*get obscure crash probably because of XtFree(thestr)*/
     /*but only when the subroutine ends*/
     thestr = (char *)XmTextGetString(    constructdistB);
     for(j=0;thestr[j]!='\0';j++) numstr[j] = thestr[j];
     numstr[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(   constructangleB);
     for(j=0;thestr[j]!='\0';j++) word[j] = thestr[j];
     word[j] = thestr[j]; /*j increased, points to '\0' */
     thestr = (char *)XmTextGetString(constructdihedralB);
     for(j=0;thestr[j]!='\0';j++) temps[j] = thestr[j];
     temps[j] = thestr[j]; /*j increased, points to '\0' */
     Lconstructperpendicular = 1;
 }
 else
 {
     printf("constructDLOG failed, intpass== %d\n",intpass);
 }
 XtFree(thestr);
 XtDestroyWidget(constructdlog);

 distconstruct = floatfromstr(numstr);
 angleconstruct = floatfromstr(word);
 dihedralconstruct = floatfromstr(temps);
 if(intpass==3)
 {/*remember these as the values given by the user*/
     distuser = floatfromstr(numstr);    /*MAGEUTIL.C*/
     angleuser = floatfromstr(word);
     dihedraluser = floatfromstr(temps);
 }
 Lcancel = 0;
 /*gets here when (Lconstructon && Lpoint>=4)*/
 doconstructline();    /*MAGEANGL.C*/
     /*stores points for 3 calls, then does construction on 4th*/
     /*called here to construct line, non-modal dialog, as needed by UNIX*/
 /*point indicator left on until line actually constructed */
 drawmarker1listptr->on = 0; /*turn off point indicaters*/
 drawmarker2listptr->on = 0;
 drawmarker3listptr->on = 0;
 drawmarker4listptr->on = 0; /*971122*/
 drawmarker5listptr->on = 0; /*020305*/
 drawmarker6listptr->on = 0; /*020305*/

 Lpoint = 0;

 ACTIVE_DLOG = 0;
 redrawvec();              /*141126 efficacy unclear*/
 return(NULL);
}
/*___construct_OK_CB()_______________________________________________________*/

/****construct_CANCEL_CB()****************************************************/
XtCallbackProc construct_CANCEL_CB(Widget w,XtPointer client_data
                                           ,XtPointer call_data)
{
  Lcancel = 1;
  XtDestroyWidget(constructdlog);
  /*do not call doconstructline(); */   /*MAGEANGL.C*/
  drawmarker1listptr->on = 0; /*turn off point indicaters*/
  drawmarker2listptr->on = 0;
  drawmarker3listptr->on = 0;
  drawmarker4listptr->on = 0; /*971122*/
  drawmarker5listptr->on = 0; /*020305*/
  drawmarker6listptr->on = 0; /*020305*/

  Lpoint = 0; /*clear this attempt to do a constructline */
  ACTIVE_DLOG = 0;
  redrawvec();              /*141126 efficacy unclear*/
  return(NULL);
}
/*___construct_CANCEL_CB()___________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*****DodrawlineDialog*******************************************************/
/* DodrawlineDialog() moved to MAGEDLOG.c 971122*/
/*___DodrawlineDialog_______________________________________________________*/

/*****DoSearchDialog*********************************************************/
void    DoSearchDialog() /*FIND*/
{
     if(!ACTIVE_DLOG)
     {
         Lsearched = 0; /*global, cycle until satisfied or quit ...*/
         /*121109 Lsearched set 0 here, will be set 1 if search fails, */
         /*121109 put 1st: avoid system cycle MUXMMAIN/main reinvoke search*/
         cleargenericflags();  /*MAGEDLOG.c*/
         /*NOT USE while(Lsearched)*/
         {
             SearchDialog();       /*MAGEDLOG.c*/
             DogenericDialog();    /*____DLOG.c*/
             /* EndSearchDialog(); called after DLOG_OKHIT_CB*/ /*MAGEDLOG.c*/
         }
     }
     /*121109 ACTIVE_DLOG just drops through so no Further Operations*/ 
}
/*___DoSearchDialog_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoQuesDialog************************************************************/
void    DoQuesDialog()
{
  int ncommon;
   int j=0;

 if(!ACTIVE_DLOG)
 {
/*overall DLOG box********************************/
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
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  ncommon = n;
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;

  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "QUESTION_ANSWER"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/

  questiondlog = XmCreateFormDialog(grafwindow," ",args,n);
  XtManageChild(questiondlog);

/*questionbox: row column container for everything*/
  n = ncommon; 
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  questionbox=XtCreateWidget(" ",xmRowColumnWidgetClass,questiondlog,args,n);
  XtManageChild(questionbox);

/*Subject******************/
  n = ncommon; 
  XtSetArg(args[n], XmNwidth, 600); n++; /*dominant over questionbox width*/
  XtSetArg(args[n], XmNrows, 3); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  questiontext = XtCreateWidget(" ", xmTextWidgetClass,questionbox,args,n);
  for(j=0;j<=iQstring;j++) word[j] = Qstring[j]; 
  XmTextSetString(questiontext, word);  
  XtManageChild(questiontext); 

/* A row for Current Answer */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowA=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowA);

  /*left box: the answer, so far...*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;

  questionanswer=XtCreateWidget(" ",xmTextWidgetClass,questionrowA,args,n);
  for(j=0; thisanswer[nQuestion][j]!='\0'&&j<MAXQCHAR; j++)
  {
     word[j] = thisanswer[nQuestion][j]; 
  }
  word[j] = '\0';
  XmTextSetString(questionanswer, word);  
  XtManageChild(questionanswer); 

  /*right box: question number header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionheader=XtCreateWidget(word,xmTextWidgetClass,questionrowA,args,n);
  sprintf(word,"Current Answer: Question # %d",nQuestion);
  XmTextSetString(questionheader, word); 
  XtManageChild(questionheader);

/* B row for Current Point ID */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowB=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowB);

  /*left box: Current Point ID*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questionnewpt=XtCreateWidget(" ",xmTextWidgetClass,questionrowB,args,n);
  XmTextSetString(questionnewpt, grafselection[1]);  
  XtManageChild(questionnewpt); 

  /*right box: current PtID  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionnewptH=XtCreateWidget(" ",xmTextWidgetClass,questionrowB,args,n);
  sprintf(word,"Current Point ID");
  XmTextSetString(questionnewptH, word); 
  XtManageChild(questionnewptH);

/* C row for Previous Point ID */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowC=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowC);

  /*left box: Previous Point ID*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questionoldpt=XtCreateWidget(" ",xmTextWidgetClass,questionrowC,args,n);
  XmTextSetString(questionoldpt, grafselection[0]);  
  XtManageChild(questionoldpt); 

  /*right box: previous PtID  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionoldptH=XtCreateWidget(" ",xmTextWidgetClass,questionrowC,args,n);
  sprintf(word,"Previous Point ID");
  XmTextSetString(questionoldptH, word); 
  XtManageChild(questionoldptH);

/* D row for distance between last two picked points*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowD=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowD);

  /*left box: distance between last two points */
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questiondist=XtCreateWidget(" ",xmTextWidgetClass,questionrowD,args,n);
  sprintf(word,"%.3f",distbetween);
  XmTextSetString(questiondist, word);  
  XtManageChild(questiondist); 

  /*right box: distance  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questiondistH=XtCreateWidget(" ",xmTextWidgetClass,questionrowD,args,n);
  sprintf(word,"distance between last two points");
  XmTextSetString(questiondistH, word); 
  XtManageChild(questiondistH);

/* E row for distance from measure tool*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowE=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowE);

  /*left box: distance*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questionmdist=XtCreateWidget(" ",xmTextWidgetClass,questionrowE,args,n);
  sprintf(word,"%.3f",distmeasure);
  XmTextSetString(questionmdist, word);  
  XtManageChild(questionmdist); 

  /*right box: distance  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionmdistH=XtCreateWidget(" ",xmTextWidgetClass,questionrowE,args,n);
  sprintf(word,"measure tool: last distance");
  XmTextSetString(questionmdistH, word); 
  XtManageChild(questionmdistH);

/* F row for angle from measure tool*/
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowF=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowF);

  /*left box: angle*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questionmangl=XtCreateWidget(" ",xmTextWidgetClass,questionrowF,args,n);
  sprintf(word,"%.3f",anglemeasure);
  XmTextSetString(questionmangl, word);  
  XtManageChild(questionmangl); 

  /*right box: angle  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionmanglH=XtCreateWidget(" ",xmTextWidgetClass,questionrowF,args,n);
  sprintf(word,"measure tool: last angle");
  XmTextSetString(questionmanglH, word); 
  XtManageChild(questionmanglH);

/* G row for dihedral */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  questionrowG=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowG);

  /*left box: dihedral*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  XtSetArg(args[n], XmNwidth, 200); n++;
  questionmdihd=XtCreateWidget(" ",xmTextWidgetClass,questionrowG,args,n);
  sprintf(word,"%.3f",dihedralmeasure);
  XmTextSetString(questionmdihd, word);  
  XtManageChild(questionmdihd); 

  /*right box: dihedral  header*/
  n = ncommon;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNwidth, 400); n++;
  questionmdihdH=XtCreateWidget(" ",xmTextWidgetClass,questionrowG,args,n);
  sprintf(word,"measure tool: last dihedral");
  XmTextSetString(questionmdihdH, word); 
  XtManageChild(questionmdihdH);


/*control buttons: ACCEPT, Cancel, go to current graphics window */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  questionbuttons=XtCreateWidget(" ",xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionbuttons); 

  n = ncommon; 
  question_OK=XmCreatePushButtonGadget(questionbuttons,"ACCEPT",args,n);
  XtAddCallback(question_OK,XmNactivateCallback
       ,(XtCallbackProc)question_OK_CB,NULL); 
  XtManageChild(question_OK);

  n = ncommon; 
  question_CANCEL=XmCreatePushButtonGadget(questionbuttons,"Cancel",args,n);
  XtAddCallback(question_CANCEL,XmNactivateCallback
       ,(XtCallbackProc)question_CANCEL_CB,NULL); 
  XtManageChild(question_CANCEL);

  n = ncommon; 
  question_GOGRAPH = XmCreatePushButtonGadget(questionbuttons
                                 ,"go to current graphics window",args,n);
  XtAddCallback(question_GOGRAPH,XmNactivateCallback
       ,(XtCallbackProc)question_GOGRAPH_CB,NULL); 
  XtManageChild(question_GOGRAPH);

  ACTIVE_DLOG = QUESTION_DLOG;
  questionDialog();
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DoQuesDialog____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****questionDialog()*********************************************************/
void questionDialog()
{
  ;
}
/*___questionDialog()________________________________________________________*/

/****question_OK_CB()********************************************************/
XtCallbackProc question_OK_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
   char* thestr; 
   int j=0;
   int LstillOK=0;

   thestr = (char *)XmTextGetString(questionanswer);
   for(j=0; thestr[j]!='\0'&&j<MAXQCHAR; j++)
   {
      thisanswer[nQuestion][j] = thestr[j]; 
   }
   thisanswer[nQuestion][j] = '\0'; 

   itextoffset=itextoffset+3; /* bracket,*,_ then 1st char of answer*/

   /*replace old answer (or ____...) with thisanswer in text field*/
   LstillOK = 1; /*OK: real answer characters to write*/
   for(j=0;j<MAXQCHAR;j++)
   {/*thestr is of arbitrary length, copy it global word string*/
      word[j] = thestr[j]; 
      if(thestr[j] == '\0') LstillOK = 0;

      if(!LstillOK) word[j] = '_';
   }
   word[j] = '\0'; /*now has MAXQCHAR of current answer padded with ___*/
/*XmTextReplace(Widget w,XmTextPosition a,XmTextPosition b,char *value)*/
   XmTextReplace(textwindow,itextoffset,itextoffset+MAXQCHAR,word);

   XtFree(thestr);
   XtDestroyWidget(questiondlog); /*030304 destroy before doing other things*/

   LneedSetsinGrafWindow = 1;
   redrawvec();              /*MUXMDRAW.c*/
   /*XtDestroyWidget(questiondlog);*/ 
   ACTIVE_DLOG = 0;
   return(NULL);
}
/*___question_OK_CB_________________________________________________________*/

/****question_CANCEL_CB()****************************************************/
XtCallbackProc question_CANCEL_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
    XtDestroyWidget(questiondlog); /*030304 destroy before doing other things*/
    redrawvec();
    /*XtDestroyWidget(questiondlog);*/ 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___question_CANCEL_CB_____________________________________________________*/

/****question_GOGRAPH_CB()***************************************************/
XtCallbackProc question_GOGRAPH_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
    XtDestroyWidget(questiondlog); /*030304 destroy before doing other things*/ 
    redrawvec();
    /*XtDestroyWidget(questiondlog);*/ 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___question_GOGRAPH_CB____________________________________________________*/

/****OpenNewFile()***********************************************************/
void OpenNewFile()
{
    if(!ACTIVE_DLOG)
    {
        /*DoOpenFileDialog();*/ /*121222 channel through FindFileName */
        FindFileName(); /*tries to set a reasonable directory 121222*/
        /*MUXMMENU/fileok_CB sets new file name to NameStr or AppendNameStr*/
        /*then calls newfile()*/
    }
}
/*___OpenNewFile()__________________________________________________________*/

/****FindFileName()***********************************************************/
void FindFileName() /*030312*/
{
   determinedirectory(); /*returns directory in DirStr 121222*/
   DoOpenFileDialog(); 
}
/*___FindFileName()__________________________________________________________*/

/****DoOpenFileDialog()*******************************************************/
void DoOpenFileDialog()
{
    XmString    xmstring3=NULL; /*121222*/
    int n=0; /*121222*/

 Lgetnewfile = 0; /*unset flag so masterloop only tries once*/
 if(!ACTIVE_DLOG)
 {
   /*Create new one to get latest files listed */

   xmstring3 = XmStringCreateSimple(DirStr); /*121222*/
   n = 0;
#ifdef MAGECOLORMAP
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
#endif /*MAGECOLORMAP*/
   XtSetArg(args[n], XmNvisual,DefaultVisual(dpy,DefaultScreen(dpy))); n++;
   XtSetArg(args[n], XmNdepth, DefaultDepth(dpy,DefaultScreen(dpy))); n++;
   XtSetArg(args[n], XmNcolormap, DefaultColormap(dpy,DefaultScreen(dpy))); n++;

/*BUT THIS FONT THING DOES NOT WORK FOR THE FILEOPEN DIALOG BOX !!!!*/
   if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }

   openfiledlog = XmCreateFileSelectionDialog(topLevel,"openFileDialog",args,n);

   XtAddCallback(openfiledlog,XmNokCallback,(XtCallbackProc)fileok_CB,NULL);
   XtAddCallback(openfiledlog,XmNcancelCallback
                                 ,(XtCallbackProc)filecancel_CB,NULL);
   /*Callback Procs are in MUXMMENU.c*/
   XtManageChild(openfiledlog); /*puts the file dialog widget on screen */

    /*force the filter Directory and file mask as specified for this task*/
    XmFileSelectionDoSearch(openfiledlog,xmstring3); /*121222*/

   ACTIVE_DLOG = 1; /*My trick so I can use the keyboard for controls*/
        /*during graphics operations, this flag lets the key info go on */
 }
}
/*___DoOpenFileDialog()______________________________________________________*/




/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****DogenericDialog()*******************************************************/
void  DogenericDialog()
{                   /*create a generic dialogbox for multiple use */
  int ncommon = 0;
  XmString    xmstring;
  int scrHeight=0, dialogHeight=0, scrollHeight=0, i=0, listHeight=0; /*060619*/

/*DogenericDialog() creates and manages all components*/
/*  genericDialog() now a dummy */

/*overall DLOG box: the containing FORM */
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
  ncommon = n;
  /*now n = ncommon, all widgets share these first n args */
XtSetArg(args[n], XmNheight, 600); n++; /*minimal nominal height*/
  XtSetArg(args[n], XmNwidth, 600); n++; /*expected maximum width*/
/*  XtSetArg(args[n], XmNx, 10); n++;*/
/*  XtSetArg(args[n], XmNy, 10); n++;*/
  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "MAGE DIALOG"); n++;
/*  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; */
    /*ignore close commands from frame menu or X button*/
    /*BUT this does not work on LINUX*/
/*  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; */
    /*try to deal with close commands from frame menu or X button*/
/*THIS DOES NOT WORK EITHER*/
/*and to make matters worse, it gets DLOG_CANCEL_CB called after */
/*DLOG_OK_CB so dialog effects are canceled*/

  genericdlog = XmCreateFormDialog(grafwindow,"FORM",args,n);
  XtManageChild(genericdlog); /*use this to turn DLOGs on and off */
/*  XtAddCallback(genericdlog, XmNdestroyCallback
                               ,(XtCallbackProc)DLOG_CANCEL_CB, NULL); 
*/
/*row column box for everything that packs contents in order of creation*/
  n = ncommon; 
/*  XtSetArg(args[n], XmNx, 10); n++;*/
/*  XtSetArg(args[n], XmNy, 10); n++;*/
/*  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;*/
/*  XtSetArg(args[n], XmNwidth, 600); n++;*/ /*expected maximum width*/
/*  XtSetArg(args[n], XmNheight, 100); n++;*/ /*minimal nominal height*/
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++; 
XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++; 
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDESTROY); n++; 
  genericbox=XtCreateWidget("alerts",xmRowColumnWidgetClass,genericdlog,args,n);
  XtManageChild(genericbox);
/*  XtAddCallback(genericbox, XmNdestroyCallback
                               ,(XtCallbackProc)DLOG_CANCEL_CB, NULL); 
*/
/*Subject*/
 if(Ldlog_subject)
 {
   n = ncommon; 
/*   XtSetArg(args[n], XmNwidth, 600); n++;  */
   XtSetArg(args[n], XmNheight, 150); n++; 
/*XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;*/ /*needed*/
/*XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;*/ /*needed*/
   if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
   XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
   XtSetArg(args[n], XmNeditable, False); n++;
   DLOG_SUBJECT = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
   XtManageChild(DLOG_SUBJECT);
   XmTextSetString(DLOG_SUBJECT,dlog_subjectstr);
 }

/* A row of parameter & text for parameter */
 if(Ldlog_paramA || Ldlog_textA)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowA
      = XtCreateWidget("A", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowA);
/*parameter value */
   if(Ldlog_paramA)
   {
     n = ncommon; 
     if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAMA = XtCreateWidget(" ", xmTextWidgetClass,genericrowA,args,n);
     XtAddCallback(DLOG_PARAMA,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMA_CB,NULL); 
     XtManageChild(DLOG_PARAMA);
     XmTextReplace(DLOG_PARAMA, 0, XmTextGetLastPosition(DLOG_PARAMA)
                   ,dlog_paramAstr);
   }
/*parameter text*/
   if(Ldlog_textA)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTA = XtCreateWidget(" ", xmLabelGadgetClass,genericrowA,args,n);
     XtManageChild(DLOG_TEXTA);
     xmstring = XmStringCreateSimple(dlog_textAstr); 
     XtVaSetValues(DLOG_TEXTA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }

/* B row of parameter & text for parameter */
 if(Ldlog_paramB || Ldlog_textB)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowB
      = XtCreateWidget("B", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowB);
/*parameter value */
   if(Ldlog_paramB)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAMB = XtCreateWidget(" ", xmTextWidgetClass,genericrowB,args,n);
     XtAddCallback(DLOG_PARAMB,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMB_CB,NULL); 
     XtManageChild(DLOG_PARAMB);
     XmTextReplace(DLOG_PARAMB, 0, XmTextGetLastPosition(DLOG_PARAMB)
                   ,dlog_paramBstr);
   }
/*parameter text*/
   if(Ldlog_textB)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTB = XtCreateWidget(" ", xmLabelGadgetClass,genericrowB,args,n);
     XtManageChild(DLOG_TEXTB);
     xmstring = XmStringCreateSimple(dlog_textBstr); 
     XtVaSetValues(DLOG_TEXTB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }

/* C row of parameter & text for parameter */
 if(Ldlog_paramC || Ldlog_textC)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowC
      = XtCreateWidget("C", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowC);
/*parameter value */
   if(Ldlog_paramC)
   {
     n = ncommon; 
     if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAMC = XtCreateWidget(" ", xmTextWidgetClass,genericrowC,args,n);
     XtAddCallback(DLOG_PARAMC,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMC_CB,NULL); 
     XtManageChild(DLOG_PARAMC);
     XmTextReplace(DLOG_PARAMC, 0, XmTextGetLastPosition(DLOG_PARAMC)
                   ,dlog_paramCstr);
   }
/*parameter text*/
   if(Ldlog_textC)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTC = XtCreateWidget(" ", xmLabelGadgetClass,genericrowC,args,n);
     XtManageChild(DLOG_TEXTC);
     xmstring = XmStringCreateSimple(dlog_textCstr); 
     XtVaSetValues(DLOG_TEXTC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }

/* D row of parameter & text for parameter */
 if(Ldlog_paramD || Ldlog_textD)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowD
      = XtCreateWidget("D", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowD);
/*parameter value */
   if(Ldlog_paramD)
   {
     n = ncommon; 

     if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAMD = XtCreateWidget(" ", xmTextWidgetClass,genericrowD,args,n);
     XtAddCallback(DLOG_PARAMD,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMD_CB,NULL); 
     XtManageChild(DLOG_PARAMD);
     XmTextReplace(DLOG_PARAMD, 0, XmTextGetLastPosition(DLOG_PARAMD)
                   ,dlog_paramDstr);
   }
/*parameter text*/
   if(Ldlog_textD)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTD = XtCreateWidget(" ", xmLabelGadgetClass,genericrowD,args,n);
     XtManageChild(DLOG_TEXTD);
     xmstring = XmStringCreateSimple(dlog_textDstr); 
     XtVaSetValues(DLOG_TEXTD,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }

/* E row of parameter & text for parameter */
 if(Ldlog_paramE || Ldlog_textE)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowE
      = XtCreateWidget("E", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowE);
/*parameter value */
   if(Ldlog_paramE)
   {
     n = ncommon; 

     if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAME = XtCreateWidget(" ", xmTextWidgetClass,genericrowE,args,n);
     XtAddCallback(DLOG_PARAME,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAME_CB,NULL); 
     XtManageChild(DLOG_PARAME);
     XmTextReplace(DLOG_PARAME, 0, XmTextGetLastPosition(DLOG_PARAME)
                   ,dlog_paramEstr);
   }
/*parameter text*/
   if(Ldlog_textE)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTE = XtCreateWidget(" ", xmLabelGadgetClass,genericrowE,args,n);
     XtManageChild(DLOG_TEXTE);
     xmstring = XmStringCreateSimple(dlog_textEstr); 
     XtVaSetValues(DLOG_TEXTE,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }

/* F row of parameter & text for parameter 041031*/
 if(Ldlog_paramF || Ldlog_textF)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowF
      = XtCreateWidget("F", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowF);
/*parameter value */
   if(Ldlog_paramF)
   {
     n = ncommon; 

     if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_PARAMF = XtCreateWidget(" ", xmTextWidgetClass,genericrowF,args,n);
     XtAddCallback(DLOG_PARAMF,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_PARAMF_CB,NULL); 
     XtManageChild(DLOG_PARAMF);
     XmTextReplace(DLOG_PARAMF, 0, XmTextGetLastPosition(DLOG_PARAMF)
                   ,dlog_paramFstr);
   }
/*parameter text*/
   if(Ldlog_textF)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     DLOG_TEXTF = XtCreateWidget(" ", xmLabelGadgetClass,genericrowF,args,n);
     XtManageChild(DLOG_TEXTF);
     xmstring = XmStringCreateSimple(dlog_textFstr); 
     XtVaSetValues(DLOG_TEXTF,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }


/*  row of check boxes */
 if(Ldlog_checkA||Ldlog_checkB||Ldlog_checkC||Ldlog_checkD||Ldlog_checkE)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowchecks
      = XtCreateWidget("CHECK", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowchecks);
/*check box A */
   if(Ldlog_checkA)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKA = XmCreateToggleButtonGadget(genericrowchecks," ",args,n);
     XtAddCallback(DLOG_CHECKA,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKA_CB, NULL); 
     XtManageChild(DLOG_CHECKA);
     XmToggleButtonSetState(DLOG_CHECKA,dlog_checkAint, 0);
     xmstring = XmStringCreateSimple(dlog_checkAstr); 
     XtVaSetValues(DLOG_CHECKA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*check box B */
   if(Ldlog_checkB)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKB = XmCreateToggleButtonGadget(genericrowchecks," ",args,n);
     XtAddCallback(DLOG_CHECKB,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKB_CB, NULL); 
     XtManageChild(DLOG_CHECKB);
     XmToggleButtonSetState(DLOG_CHECKB,dlog_checkBint, 0);
     xmstring = XmStringCreateSimple(dlog_checkBstr); 
     XtVaSetValues(DLOG_CHECKB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*check box C */
   if(Ldlog_checkC)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKC = XmCreateToggleButtonGadget(genericrowchecks," ",args,n);
     XtAddCallback(DLOG_CHECKC,XmNvalueChangedCallback
                                     ,(XtCallbackProc)DLOG_CHECKC_CB, NULL); 
     XtManageChild(DLOG_CHECKC);
     XmToggleButtonSetState(DLOG_CHECKC,dlog_checkCint, 0);
     xmstring = XmStringCreateSimple(dlog_checkCstr); 
     XtVaSetValues(DLOG_CHECKC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*check box D */
   if(Ldlog_checkD)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKD = XmCreateToggleButtonGadget(genericrowchecks," ",args,n);
     XtAddCallback(DLOG_CHECKD,XmNvalueChangedCallback
                                     ,(XtCallbackProc)DLOG_CHECKD_CB, NULL); 
     XtManageChild(DLOG_CHECKD);
     XmToggleButtonSetState(DLOG_CHECKD,dlog_checkDint, 0);
     xmstring = XmStringCreateSimple(dlog_checkDstr); 
     XtVaSetValues(DLOG_CHECKD,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*check box E */
   if(Ldlog_checkE)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKE = XmCreateToggleButtonGadget(genericrowchecks," ",args,n);
     XtAddCallback(DLOG_CHECKE,XmNvalueChangedCallback
                                     ,(XtCallbackProc)DLOG_CHECKE_CB, NULL); 
     XtManageChild(DLOG_CHECKE);
     XmToggleButtonSetState(DLOG_CHECKE,dlog_checkEint, 0);
     xmstring = XmStringCreateSimple(dlog_checkEstr); 
     XtVaSetValues(DLOG_CHECKE,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }
/*  row of check boxes */
 if(Ldlog_checkF)
 {
   n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
   genericrowchecks2
      = XtCreateWidget("CHECK", xmRowColumnWidgetClass,genericbox,args,n);
   XtManageChild(genericrowchecks2);
/*check box F */
   if(Ldlog_checkF)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_CHECKF = XmCreateToggleButtonGadget(genericrowchecks2," ",args,n);
     XtAddCallback(DLOG_CHECKF,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_CHECKF_CB, NULL); 
     XtManageChild(DLOG_CHECKF);
     XmToggleButtonSetState(DLOG_CHECKF,dlog_checkFint, 0);
     xmstring = XmStringCreateSimple(dlog_checkFstr); 
     XtVaSetValues(DLOG_CHECKF,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }
/*stack of radio buttons*/
 if(  Ldlog_radioA||Ldlog_radioB||Ldlog_radioC||Ldlog_radioD||Ldlog_radioE
    ||Ldlog_radioF||Ldlog_radioG||Ldlog_radioH||Ldlog_radioI||Ldlog_radioJ
    ||Ldlog_radioK||Ldlog_radioL||Ldlog_radioM||Ldlog_radioN||Ldlog_radioO
    ||Ldlog_radioP||Ldlog_radioQ||Ldlog_radioR
    ||Ldlog_radioS||Ldlog_radioT||Ldlog_radioU||Ldlog_radioV||Ldlog_radioW
    ||Ldlog_radioX||Ldlog_radioY||Ldlog_radioZ)
 {
  /* A row of columns *************************************************/
  n = ncommon;
  if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 280); n++;
   XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
   XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
   XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  genericcolradios =
        XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericcolradios);
 }
 if(  Ldlog_radioA||Ldlog_radioB||Ldlog_radioC||Ldlog_radioD||Ldlog_radioE
    ||Ldlog_radioF||Ldlog_radioG||Ldlog_radioH||Ldlog_radioI||Ldlog_radioJ)
 {
   n = ncommon; 
   genericrowradios
      = XtCreateWidget("RADIO", xmRowColumnWidgetClass,genericcolradios,args,n);
   XtManageChild(genericrowradios);

/*radio box A */
   if(Ldlog_radioA)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOA = XmCreateToggleButtonGadget(genericrowradios,"RA",args,n);
     XtAddCallback(DLOG_RADIOA,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOA_CB, NULL); 
     XtManageChild(DLOG_RADIOA);
     XmToggleButtonSetState(DLOG_RADIOA,dlog_radioAint, 0);
     xmstring = XmStringCreateSimple(dlog_radioAstr); 
     XtVaSetValues(DLOG_RADIOA,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*radio box B */
   if(Ldlog_radioB)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOB = XmCreateToggleButtonGadget(genericrowradios,"RB",args,n);
     XtAddCallback(DLOG_RADIOB,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOB_CB, NULL); 
     XtManageChild(DLOG_RADIOB);
     XmToggleButtonSetState(DLOG_RADIOB,dlog_radioBint, 0);
     xmstring = XmStringCreateSimple(dlog_radioBstr); 
     XtVaSetValues(DLOG_RADIOB,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*radio box C */
   if(Ldlog_radioC)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOC = XmCreateToggleButtonGadget(genericrowradios,"RC",args,n);
     XtAddCallback(DLOG_RADIOC,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOC_CB, NULL); 
     XtManageChild(DLOG_RADIOC);
     XmToggleButtonSetState(DLOG_RADIOC,dlog_radioCint, 0);
     xmstring = XmStringCreateSimple(dlog_radioCstr); 
     XtVaSetValues(DLOG_RADIOC,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

/*radio box D */
   if(Ldlog_radioD)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOD = XmCreateToggleButtonGadget(genericrowradios,"RD",args,n);
     XtAddCallback(DLOG_RADIOD,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOD_CB, NULL); 
     XtManageChild(DLOG_RADIOD);
     XmToggleButtonSetState(DLOG_RADIOD,dlog_radioDint, 0);
     xmstring = XmStringCreateSimple(dlog_radioDstr); 
     XtVaSetValues(DLOG_RADIOD,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

/*radio box E */
   if(Ldlog_radioE)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOE = XmCreateToggleButtonGadget(genericrowradios,"RE",args,n);
     XtAddCallback(DLOG_RADIOE,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOE_CB, NULL); 
     XtManageChild(DLOG_RADIOE);
     XmToggleButtonSetState(DLOG_RADIOE,dlog_radioEint, 0);
     xmstring = XmStringCreateSimple(dlog_radioEstr); 
     XtVaSetValues(DLOG_RADIOE,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

/*radio box F */
   if(Ldlog_radioF)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOF = XmCreateToggleButtonGadget(genericrowradios,"RF",args,n);
     XtAddCallback(DLOG_RADIOF,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOF_CB, NULL); 
     XtManageChild(DLOG_RADIOF);
     XmToggleButtonSetState(DLOG_RADIOE,dlog_radioFint, 0);
     xmstring = XmStringCreateSimple(dlog_radioFstr); 
     XtVaSetValues(DLOG_RADIOF,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }

/*radio box G */
   if(Ldlog_radioG)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOG = XmCreateToggleButtonGadget(genericrowradios,"RG",args,n);
     XtAddCallback(DLOG_RADIOG,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOG_CB, NULL); 
     XtManageChild(DLOG_RADIOG);
     XmToggleButtonSetState(DLOG_RADIOG,dlog_radioGint, 0);
     xmstring = XmStringCreateSimple(dlog_radioGstr); 
     XtVaSetValues(DLOG_RADIOG,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*radio box H */
   if(Ldlog_radioH)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOH = XmCreateToggleButtonGadget(genericrowradios,"RH",args,n);
     XtAddCallback(DLOG_RADIOH,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOH_CB, NULL); 
     XtManageChild(DLOG_RADIOH);
     XmToggleButtonSetState(DLOG_RADIOH,dlog_radioHint, 0);
     xmstring = XmStringCreateSimple(dlog_radioHstr); 
     XtVaSetValues(DLOG_RADIOH,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
/*radio box I */
   if(Ldlog_radioI)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOI = XmCreateToggleButtonGadget(genericrowradios,"RI",args,n);
     XtAddCallback(DLOG_RADIOI,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOI_CB, NULL); 
     XtManageChild(DLOG_RADIOI);
     XmToggleButtonSetState(DLOG_RADIOI,dlog_radioIint, 0);
     xmstring = XmStringCreateSimple(dlog_radioIstr); 
     XtVaSetValues(DLOG_RADIOI,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box J */
   if(Ldlog_radioJ)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOJ = XmCreateToggleButtonGadget(genericrowradios,"RJ",args,n);
     XtAddCallback(DLOG_RADIOJ,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOJ_CB, NULL); 
     XtManageChild(DLOG_RADIOJ);
     XmToggleButtonSetState(DLOG_RADIOJ,dlog_radioJint, 0);
     xmstring = XmStringCreateSimple(dlog_radioJstr); 
     XtVaSetValues(DLOG_RADIOJ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }
 if(  Ldlog_radioK||Ldlog_radioL||Ldlog_radioM||Ldlog_radioN||Ldlog_radioO
    ||Ldlog_radioP||Ldlog_radioQ||Ldlog_radioR)
 {
   n = ncommon;
   genericrow2radios
      = XtCreateWidget("RADIO", xmRowColumnWidgetClass,genericcolradios,args,n);
   XtManageChild(genericrow2radios);
   /*radio box K */
   if(Ldlog_radioK)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOK = XmCreateToggleButtonGadget(genericrow2radios,"RK",args,n);
     XtAddCallback(DLOG_RADIOK,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOK_CB, NULL); 
     XtManageChild(DLOG_RADIOK);
     XmToggleButtonSetState(DLOG_RADIOK,dlog_radioKint, 0);
     xmstring = XmStringCreateSimple(dlog_radioKstr); 
     XtVaSetValues(DLOG_RADIOK,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box L */
   if(Ldlog_radioL)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOL = XmCreateToggleButtonGadget(genericrow2radios,"RL",args,n);
     XtAddCallback(DLOG_RADIOL,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOL_CB, NULL); 
     XtManageChild(DLOG_RADIOL);
     XmToggleButtonSetState(DLOG_RADIOL,dlog_radioLint, 0);
     xmstring = XmStringCreateSimple(dlog_radioLstr); 
     XtVaSetValues(DLOG_RADIOL,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box M */
   if(Ldlog_radioM)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOM = XmCreateToggleButtonGadget(genericrow2radios,"RM",args,n);
     XtAddCallback(DLOG_RADIOM,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOM_CB, NULL); 
     XtManageChild(DLOG_RADIOM);
     XmToggleButtonSetState(DLOG_RADIOM,dlog_radioMint, 0);
     xmstring = XmStringCreateSimple(dlog_radioMstr); 
     XtVaSetValues(DLOG_RADIOM,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box N */
   if(Ldlog_radioN)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADION = XmCreateToggleButtonGadget(genericrow2radios,"RN",args,n);
     XtAddCallback(DLOG_RADION,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADION_CB, NULL); 
     XtManageChild(DLOG_RADION);
     XmToggleButtonSetState(DLOG_RADION,dlog_radioNint, 0);
     xmstring = XmStringCreateSimple(dlog_radioNstr); 
     XtVaSetValues(DLOG_RADION,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box O */
   if(Ldlog_radioO)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOO = XmCreateToggleButtonGadget(genericrow2radios,"RO",args,n);
     XtAddCallback(DLOG_RADIOO,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOO_CB, NULL); 
     XtManageChild(DLOG_RADIOO);
     XmToggleButtonSetState(DLOG_RADIOO,dlog_radioOint, 0);
     xmstring = XmStringCreateSimple(dlog_radioOstr); 
     XtVaSetValues(DLOG_RADIOO,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioP */
   if(Ldlog_radioP)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOP =XmCreateToggleButtonGadget(genericrow2radios,"RADIOP",args,n);
     XtAddCallback(DLOG_RADIOP,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOP_CB, NULL); 
     XtManageChild(DLOG_RADIOP);
     XmToggleButtonSetState(DLOG_RADIOP,dlog_radioPint, 0);
     xmstring = XmStringCreateSimple(dlog_radioPstr); 
     XtVaSetValues(DLOG_RADIOP,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioQ */
   if(Ldlog_radioQ)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOQ =XmCreateToggleButtonGadget(genericrow2radios,"RADIOQ",args,n);
     XtAddCallback(DLOG_RADIOQ,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOQ_CB, NULL); 
     XtManageChild(DLOG_RADIOQ);
     XmToggleButtonSetState(DLOG_RADIOQ,dlog_radioQint, 0);
     xmstring = XmStringCreateSimple(dlog_radioQstr); 
     XtVaSetValues(DLOG_RADIOQ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioR */
   if(Ldlog_radioR)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOR =XmCreateToggleButtonGadget(genericrow2radios,"RADIOR",args,n);
     XtAddCallback(DLOG_RADIOR,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOR_CB, NULL); 
     XtManageChild(DLOG_RADIOR);
     XmToggleButtonSetState(DLOG_RADIOR,dlog_radioRint, 0);
     xmstring = XmStringCreateSimple(dlog_radioRstr); 
     XtVaSetValues(DLOG_RADIOR,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }
 if(  Ldlog_radioS||Ldlog_radioT||Ldlog_radioU||Ldlog_radioV||Ldlog_radioW
    ||Ldlog_radioX||Ldlog_radioY||Ldlog_radioZ)
 {
   n = ncommon;
   genericrow3radios
      = XtCreateWidget("RADIO", xmRowColumnWidgetClass,genericcolradios,args,n);
   XtManageChild(genericrow3radios);

   /*radio box radioS */
   if(Ldlog_radioS)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOS =XmCreateToggleButtonGadget(genericrow3radios,"RADIOS",args,n);
     XtAddCallback(DLOG_RADIOS,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOS_CB, NULL); 
     XtManageChild(DLOG_RADIOS);
     XmToggleButtonSetState(DLOG_RADIOS,dlog_radioSint, 0);
     xmstring = XmStringCreateSimple(dlog_radioSstr); 
     XtVaSetValues(DLOG_RADIOS,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioT */
   if(Ldlog_radioT)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOT =XmCreateToggleButtonGadget(genericrow3radios,"RADIOT",args,n);
     XtAddCallback(DLOG_RADIOT,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOT_CB, NULL); 
     XtManageChild(DLOG_RADIOT);
     XmToggleButtonSetState(DLOG_RADIOT,dlog_radioTint, 0);
     xmstring = XmStringCreateSimple(dlog_radioTstr); 
     XtVaSetValues(DLOG_RADIOT,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioU */
   if(Ldlog_radioU)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOU =XmCreateToggleButtonGadget(genericrow3radios,"RADIOU",args,n);
     XtAddCallback(DLOG_RADIOU,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOU_CB, NULL); 
     XtManageChild(DLOG_RADIOU);
     XmToggleButtonSetState(DLOG_RADIOU,dlog_radioUint, 0);
     xmstring = XmStringCreateSimple(dlog_radioUstr); 
     XtVaSetValues(DLOG_RADIOU,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioV */
   if(Ldlog_radioV)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOV =XmCreateToggleButtonGadget(genericrow3radios,"RADIOV",args,n);
     XtAddCallback(DLOG_RADIOV,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOV_CB, NULL); 
     XtManageChild(DLOG_RADIOV);
     XmToggleButtonSetState(DLOG_RADIOV,dlog_radioVint, 0);
     xmstring = XmStringCreateSimple(dlog_radioVstr); 
     XtVaSetValues(DLOG_RADIOV,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioW */
   if(Ldlog_radioW)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOW =XmCreateToggleButtonGadget(genericrow3radios,"RADIOW",args,n);
     XtAddCallback(DLOG_RADIOW,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOW_CB, NULL); 
     XtManageChild(DLOG_RADIOW);
     XmToggleButtonSetState(DLOG_RADIOW,dlog_radioWint, 0);
     xmstring = XmStringCreateSimple(dlog_radioWstr); 
     XtVaSetValues(DLOG_RADIOW,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioX */
   if(Ldlog_radioX)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOX =XmCreateToggleButtonGadget(genericrow3radios,"RADIOX",args,n);
     XtAddCallback(DLOG_RADIOX,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOX_CB, NULL); 
     XtManageChild(DLOG_RADIOX);
     XmToggleButtonSetState(DLOG_RADIOX,dlog_radioXint, 0);
     xmstring = XmStringCreateSimple(dlog_radioXstr); 
     XtVaSetValues(DLOG_RADIOX,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioY */
   if(Ldlog_radioY)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOY =XmCreateToggleButtonGadget(genericrow3radios,"RADIOY",args,n);
     XtAddCallback(DLOG_RADIOY,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOY_CB, NULL); 
     XtManageChild(DLOG_RADIOY);
     XmToggleButtonSetState(DLOG_RADIOY,dlog_radioYint, 0);
     xmstring = XmStringCreateSimple(dlog_radioYstr); 
     XtVaSetValues(DLOG_RADIOY,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
   /*radio box radioZ */
   if(Ldlog_radioZ)
   {
     n = ncommon; 
     if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
     XtSetArg(args[n], XmNindicatorOn, True); n++;
     XtSetArg(args[n], XmNfillOnSelect, True); n++;
     XtSetArg(args[n], XmNselectColor, windowborder); n++;
     DLOG_RADIOZ =XmCreateToggleButtonGadget(genericrow3radios,"RADIOZ",args,n);
     XtAddCallback(DLOG_RADIOZ,XmNvalueChangedCallback
                                  ,(XtCallbackProc)DLOG_RADIOZ_CB, NULL); 
     XtManageChild(DLOG_RADIOZ);
     XmToggleButtonSetState(DLOG_RADIOZ,dlog_radioZint, 0);
     xmstring = XmStringCreateSimple(dlog_radioZstr); 
     XtVaSetValues(DLOG_RADIOZ,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
   }
 }
/*control buttons: OK, Option, Cancel */
  n = ncommon; 
   XtSetArg(args[n], XmNheight, 16); n++; 
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  genericbuttons = 
         XtCreateWidget("buttons", xmRowColumnWidgetClass,genericbox,args,n);
  XtManageChild(genericbuttons);
  /*all DLOG have place for OK, etc. buttons, manage it after ones above it*/

  /*OK button*/
    n = ncommon; 
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    DLOG_OK = XmCreatePushButtonGadget(genericbuttons,"OK",args,n);
    XtAddCallback(DLOG_OK, XmNactivateCallback
                           ,(XtCallbackProc)DLOG_OK_CB, NULL); 
    XtManageChild(DLOG_OK); /*all DLOG have OK button*/
    xmstring = XmStringCreateSimple(dlog_OKstr); 
    XtVaSetValues(DLOG_OK,XmNlabelString,xmstring,NULL);
    XmStringFree(xmstring);

  /*OPTION button*/
  if(Ldlog_option)
  {
    n = ncommon; 
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    DLOG_OPTION = XmCreatePushButtonGadget(genericbuttons,"Option",args,n);
    XtAddCallback(DLOG_OPTION, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_OPTION_CB, NULL); 
    XtManageChild(DLOG_OPTION);
    xmstring = XmStringCreateSimple(dlog_optionstr); 
    XtVaSetValues(DLOG_OPTION,XmNlabelString,xmstring,NULL);
    XmStringFree(xmstring);
  }

  if(Ldlog_cancel)
  {/*CANCEL button*/
    n = ncommon; 
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    DLOG_CANCEL = XmCreatePushButtonGadget(genericbuttons,"Cancel",args,n);
    XtAddCallback(DLOG_CANCEL, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_CANCEL_CB, NULL); 
    XtManageChild(DLOG_CANCEL); /*all DLOG have cancel button*/
    xmstring = XmStringCreateSimple(dlog_cancelstr); 
    XtVaSetValues(DLOG_CANCEL,XmNlabelString,xmstring,NULL);
    XmStringFree(xmstring);
  }
  if(Ldlog_extra)
  {/*extra button*/
    n = ncommon; 
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    DLOG_EXTRA = XmCreatePushButtonGadget(genericbuttons,"Extra",args,n);
    XtAddCallback(DLOG_EXTRA, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_EXTRA_CB, NULL); 
    XtManageChild(DLOG_EXTRA); /*rare DLOG have extra buttons*/
    xmstring = XmStringCreateSimple(dlog_extrastr); 
    XtVaSetValues(DLOG_EXTRA,XmNlabelString,xmstring,NULL);
    XmStringFree(xmstring);
  }
  if(Ldlog_extra2)
  {/*extra button*/
    n = ncommon; 
    if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
    DLOG_EXTRA2 = XmCreatePushButtonGadget(genericbuttons,"Extra",args,n);
    XtAddCallback(DLOG_EXTRA2, XmNactivateCallback
                               ,(XtCallbackProc)DLOG_EXTRA2_CB, NULL); 
    XtManageChild(DLOG_EXTRA2); /*rare DLOG have extra buttons*/
    xmstring = XmStringCreateSimple(dlog_extra2str); 
    XtVaSetValues(DLOG_EXTRA2,XmNlabelString,xmstring,NULL);
    XmStringFree(xmstring);
  }

/*List Boxes 060619*/
  if(Ldlog_listboxA || Ldlog_listboxB || Ldlog_listboxC)
  {/* row of list boxes, which extend down from this row*/
    /*list box*/
    scrHeight = DisplayHeight( dpy, DefaultScreen(dpy));
    if((8+28)*FONTSIZE > scrHeight) {  dialogHeight = GHEIGHT; }
    else { dialogHeight = (8+28)*FONTSIZE; }
    scrollHeight = dialogHeight - 8*FONTSIZE;

    n = ncommon; 
    XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
    XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
    XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
    XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++; /*050816*/
    genericrowlistbox
      = XtCreateWidget(" ", xmRowColumnWidgetClass,genericbox,args,n);
    XtManageChild(genericrowlistbox);

    if(Ldlog_listboxA)
    {/*Scrolled List A */
      if(NlistboxAitems * FONTSIZE < scrHeight) 
           {listHeight = NlistboxAitems * FONTSIZE;}
      else {listHeight = scrollHeight;}
      n = ncommon; 
      XtSetArg(args[n], XmNheight, listHeight); n++;
      if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
      XtSetArg(args[n], XmNselectionPolicy, XmBROWSE_SELECT); n++;
      XtSetArg(args[n], XmNvisibleItemCount, NlistboxAitems); n++;
      genericlistboxA = XmCreateScrolledList(genericrowlistbox,"X",args,n);
      XtAddCallback(genericlistboxA,XmNbrowseSelectionCallback
                                          ,(XtCallbackProc)listboxA_CB, NULL); 
      XtManageChild(genericlistboxA); 

      /*load list*/
      for(i=1; i <= NlistboxAitems; i++) /*indexing from 1*/
      {
        if(listboxAitemName[i] != NULL) /*061111 run time alloc-ed*/
             {sprintf(word,"%s",listboxAitemName[i]);}
        else {sprintf(word,"foo");}
        xmstring=XmStringCreateSimple(word); 
        XmListAddItem(genericlistboxA,xmstring,i); XmStringFree(xmstring);
      }
      /*select item in the list*/
      XmListSelectPos(genericlistboxA, listboxAitem, 0);
    }
    if(Ldlog_listboxB)
    {/*Scrolled List B */
      if(NlistboxBitems * FONTSIZE < scrHeight) 
           {listHeight = NlistboxBitems * FONTSIZE;}
      else {listHeight = scrollHeight;}
      n = ncommon; 
      XtSetArg(args[n], XmNheight, listHeight); n++;
      if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
      XtSetArg(args[n], XmNselectionPolicy, XmBROWSE_SELECT); n++;
      XtSetArg(args[n], XmNvisibleItemCount, NlistboxBitems); n++;
      genericlistboxB = XmCreateScrolledList(genericrowlistbox,"Y",args,n);
      XtAddCallback(genericlistboxB,XmNbrowseSelectionCallback
                                          ,(XtCallbackProc)listboxB_CB, NULL); 
      XtManageChild(genericlistboxB); 

      /*load list*/
      for(i=1; i <= NlistboxBitems; i++) /*indexing from 1*/
      {
        if(listboxBitemName[i] != NULL) /*061111 run time alloc-ed*/
             {sprintf(word,"%s",listboxBitemName[i]);}
        else {sprintf(word,"foo");}
        xmstring=XmStringCreateSimple(word); 
        XmListAddItem(genericlistboxB,xmstring,i); XmStringFree(xmstring);
      }
      /*select item in the list*/
      XmListSelectPos(genericlistboxB, listboxBitem, 0);
    }
    if(Ldlog_listboxC)
    {/*Scrolled List C */
      if(NlistboxCitems * FONTSIZE < scrHeight) 
           {listHeight = NlistboxCitems * FONTSIZE;}
      else {listHeight = scrollHeight;}
      n = ncommon; 
      XtSetArg(args[n], XmNheight, listHeight); n++;
      if(textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
      XtSetArg(args[n], XmNselectionPolicy, XmBROWSE_SELECT); n++;
      XtSetArg(args[n], XmNvisibleItemCount, NlistboxCitems); n++;
      genericlistboxC = XmCreateScrolledList(genericrowlistbox,"Z",args,n);
      XtAddCallback(genericlistboxC,XmNbrowseSelectionCallback
                                          ,(XtCallbackProc)listboxC_CB, NULL); 
      XtManageChild(genericlistboxC); 

      /*load list*/
      for(i=1; i <= NlistboxCitems; i++) /*indexing from 1*/
      {
        if(listboxCitemName[i] != NULL) /*061111 run time alloc-ed*/
             {sprintf(word,"%s",listboxCitemName[i]);}
        else {sprintf(word,"foo");}
        xmstring=XmStringCreateSimple(word); 
        XmListAddItem(genericlistboxC,xmstring,i); XmStringFree(xmstring);
      }
      /*select item in the list*/
      XmListSelectPos(genericlistboxC, listboxCitem, 0);
    }
  }/* row of list boxes */


/*Information*/
  if(Ldlog_info)
  {
    n = ncommon; 
    /* XtSetArg(args[n], XmNwidth, 600); n++; */
    if(Ldlog_info==2) {XtSetArg(args[n], XmNheight, 10); n++;} /*filler*/
    else {XtSetArg(args[n], XmNheight, 150); n++;} /*has content 060619*/
XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++; /*needed*/
XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++; /*needed*/
    if (textfontList) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
    XtSetArg(args[n], XmNeditable, True); n++;
    DLOG_INFO = XtCreateWidget(" ", xmTextWidgetClass,genericbox,args,n);
    XtManageChild(DLOG_INFO);
    XtAddCallback(DLOG_INFO,XmNvalueChangedCallback
                                        ,(XtCallbackProc)DLOG_INFO_CB,NULL);
    XmTextSetString(DLOG_INFO,dlog_infostr);
  }

  genericDialog();
}
/*___DogenericDialog()______________________________________________________*/

/****genericDialog()*********************************************************/
void genericDialog()
{
  ;
}
/*___genericDialog()_________________________________________________________*/

/****DLOG_INFO_CB()*********************************************************/
XtCallbackProc DLOG_INFO_CB(Widget w,XtPointer client ,XtPointer call)
{
 strcpy( dlog_infostr, XmTextGetString(DLOG_INFO));
    return(NULL);
}
/*___DLOG_INFO_CB()________________________________________________________*/

/*MAC and PC dialogs retain control during dialog interaction */
/*UNIX X dialogs pass control to the separate callback routines */
/* so the various callback routines servicing the dialogs using generic */
/* dialog boxes must each be able to know which specific dialog is current*/
/* fortunately, the routines that just respond to keystrokes or mouse clicks */
/* need only to maintain generic parameters.  Only routines that end the */
/* dialog need to pass control back to the correct current specific dialog */
/* these are the OK, OPTION, and CANCEL callbacks which set global flags, and*/
/* have to get things plugged back into the currect specific dialog logic */

/****DLOG_PARAMA_CB()*********************************************************/
XtCallbackProc DLOG_PARAMA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAMA);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramAstr[j] = thestr[j];
 }
 dlog_paramAstr[j+1] = '\0';
 XtFree(thestr);
  return(NULL);
}
/*___DLOG_PARAMA_CB()________________________________________________________*/

/****DLOG_PARAMB_CB()*********************************************************/
XtCallbackProc DLOG_PARAMB_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAMB);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramBstr[j] = thestr[j];
 }
 dlog_paramBstr[j+1] = '\0';
 XtFree(thestr);
 return(NULL);
}
/*___DLOG_PARAMB_CB()________________________________________________________*/

/****DLOG_PARAMC_CB()*********************************************************/
XtCallbackProc DLOG_PARAMC_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAMC);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramCstr[j] = thestr[j];
 }
 dlog_paramCstr[j+1] = '\0';
 XtFree(thestr);
 return(NULL);
}
/*___DLOG_PARAMC_CB()________________________________________________________*/

/****DLOG_PARAMD_CB()*********************************************************/
XtCallbackProc DLOG_PARAMD_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAMD);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramDstr[j] = thestr[j];
 }
 dlog_paramDstr[j+1] = '\0';
 XtFree(thestr);
 return(NULL);
}
/*___DLOG_PARAMD_CB()________________________________________________________*/

/****DLOG_PARAME_CB()*********************************************************/
XtCallbackProc DLOG_PARAME_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAME);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramEstr[j] = thestr[j];
 }
 dlog_paramEstr[j+1] = '\0';
 XtFree(thestr);
 return(NULL);
}
/*___DLOG_PARAME_CB()________________________________________________________*/

/****DLOG_PARAMF_CB()*********************************************************/
XtCallbackProc DLOG_PARAMF_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{    /*041031*/
 char *thestr;
 int  j;

 thestr = XmTextGetString(DLOG_PARAMF);
 for(j=0;thestr[j]!='\0';j++)
 {
     dlog_paramFstr[j] = thestr[j];
 }
 dlog_paramFstr[j+1] = '\0';
 XtFree(thestr);
 return(NULL);
}
/*___DLOG_PARAMF_CB()________________________________________________________*/

/****DLOG_CHECKA_CB()*********************************************************/
XtCallbackProc DLOG_CHECKA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkAint = XmToggleButtonGetState(DLOG_CHECKA);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKA_CB()________________________________________________________*/

/****DLOG_CHECKB_CB()*********************************************************/
XtCallbackProc DLOG_CHECKB_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkBint = XmToggleButtonGetState(DLOG_CHECKB);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKB_CB()________________________________________________________*/

/****DLOG_CHECKC_CB()*********************************************************/
XtCallbackProc DLOG_CHECKC_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkCint = XmToggleButtonGetState(DLOG_CHECKC);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKC_CB()________________________________________________________*/

/****DLOG_CHECKD_CB()*********************************************************/
XtCallbackProc DLOG_CHECKD_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkDint = XmToggleButtonGetState(DLOG_CHECKD);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKD_CB()________________________________________________________*/

/****DLOG_CHECKE_CB()*********************************************************/
XtCallbackProc DLOG_CHECKE_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkEint = XmToggleButtonGetState(DLOG_CHECKE);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKE_CB()________________________________________________________*/

/****DLOG_CHECKF_CB()*********************************************************/
XtCallbackProc DLOG_CHECKF_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*toggle button maintains its own state*/
 dlog_checkFint = XmToggleButtonGetState(DLOG_CHECKF);  /* ?? */
 return(NULL);
}
/*___DLOG_CHECKF_CB()________________________________________________________*/

/****clearradioA_J()**********************************************************/
void clearradioA_J()
{
 if(Ldlog_radioA) XmToggleButtonSetState(DLOG_RADIOA,dlog_radioAint=0, 0);
 if(Ldlog_radioB) XmToggleButtonSetState(DLOG_RADIOB,dlog_radioBint=0, 0);
 if(Ldlog_radioC) XmToggleButtonSetState(DLOG_RADIOC,dlog_radioCint=0, 0);
 if(Ldlog_radioD) XmToggleButtonSetState(DLOG_RADIOD,dlog_radioDint=0, 0);
 if(Ldlog_radioE) XmToggleButtonSetState(DLOG_RADIOE,dlog_radioEint=0, 0);
 if(Ldlog_radioF) XmToggleButtonSetState(DLOG_RADIOF,dlog_radioFint=0, 0);
 if(Ldlog_radioG) XmToggleButtonSetState(DLOG_RADIOG,dlog_radioGint=0, 0);
 if(Ldlog_radioH) XmToggleButtonSetState(DLOG_RADIOH,dlog_radioHint=0, 0);
 if(Ldlog_radioI) XmToggleButtonSetState(DLOG_RADIOI,dlog_radioIint=0, 0);
 if(Ldlog_radioJ) XmToggleButtonSetState(DLOG_RADIOJ,dlog_radioJint=0, 0);
}
/*___clearradioA_J()_________________________________________________________*/

/****DLOG_RADIOA_CB()*********************************************************/
XtCallbackProc DLOG_RADIOA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioAint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOA,dlog_radioAint, 0);
    return(NULL);
}
/*___DLOG_RADIOA_CB()________________________________________________________*/

/****DLOG_RADIOB_CB()*********************************************************/
XtCallbackProc DLOG_RADIOB_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioBint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOB,dlog_radioBint, 0);
    return(NULL);
}
/*___DLOG_RADIOB_CB()________________________________________________________*/

/****DLOG_RADIOC_CB()*********************************************************/
XtCallbackProc DLOG_RADIOC_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioCint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOC,dlog_radioCint, 0);
    return(NULL);
}
/*___DLOG_RADIOC_CB()________________________________________________________*/

/****DLOG_RADIOD_CB()*********************************************************/
XtCallbackProc DLOG_RADIOD_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioDint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOD,dlog_radioDint, 0);
    return(NULL);
}
/*___DLOG_RADIOD_CB()________________________________________________________*/

/****DLOG_RADIOE_CB()*********************************************************/
XtCallbackProc DLOG_RADIOE_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioEint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOE,dlog_radioEint, 0);
    return(NULL);
}
/*___DLOG_RADIOE_CB()________________________________________________________*/

/****DLOG_RADIOF_CB()*********************************************************/
XtCallbackProc DLOG_RADIOF_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioFint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOF,dlog_radioFint, 0);
    return(NULL);
}
/*___DLOG_RADIOF_CB()________________________________________________________*/

/****DLOG_RADIOG_CB()*********************************************************/
XtCallbackProc DLOG_RADIOG_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioGint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOG,dlog_radioGint, 0);
    return(NULL);
}
/*___DLOG_RADIOG_CB()________________________________________________________*/

/****DLOG_RADIOH_CB()*********************************************************/
XtCallbackProc DLOG_RADIOH_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioHint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOH,dlog_radioHint, 0);
    return(NULL);
}
/*___DLOG_RADIOH_CB()________________________________________________________*/

/****DLOG_RADIOI_CB()*********************************************************/
XtCallbackProc DLOG_RADIOI_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioIint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOI,dlog_radioIint, 0);
    return(NULL);
}
/*___DLOG_RADIOI_CB()________________________________________________________*/

/****DLOG_RADIOJ_CB()*********************************************************/
XtCallbackProc DLOG_RADIOJ_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioA_J(); 
 dlog_radioJint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOJ,dlog_radioJint, 0);
    return(NULL);
}
/*___DLOG_RADIOI_CB()________________________________________________________*/

/****clearradioK_R()**********************************************************/
void clearradioK_R()
{
 if(Ldlog_radioK) XmToggleButtonSetState(DLOG_RADIOK,dlog_radioKint=0, 0);
 if(Ldlog_radioL) XmToggleButtonSetState(DLOG_RADIOL,dlog_radioLint=0, 0);
 if(Ldlog_radioM) XmToggleButtonSetState(DLOG_RADIOM,dlog_radioMint=0, 0);
 if(Ldlog_radioN) XmToggleButtonSetState(DLOG_RADION,dlog_radioNint=0, 0);
 if(Ldlog_radioO) XmToggleButtonSetState(DLOG_RADIOO,dlog_radioOint=0, 0);
 if(Ldlog_radioP) XmToggleButtonSetState(DLOG_RADIOP,dlog_radioPint=0, 0);
 if(Ldlog_radioQ) XmToggleButtonSetState(DLOG_RADIOQ,dlog_radioQint=0, 0);
 if(Ldlog_radioR) XmToggleButtonSetState(DLOG_RADIOR,dlog_radioRint=0, 0);
}
/*___clearradioK_R()_________________________________________________________*/

/****DLOG_RADIOK_CB()*********************************************************/
XtCallbackProc DLOG_RADIOK_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioKint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOK,dlog_radioKint, 0);
    return(NULL);
}
/*___DLOG_RADIOK_CB()________________________________________________________*/

/****DLOG_RADIOL_CB()*********************************************************/
XtCallbackProc DLOG_RADIOL_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioLint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOL,dlog_radioLint, 0);
    return(NULL);
}
/*___DLOG_RADIOL_CB()________________________________________________________*/

/****DLOG_RADIOM_CB()*********************************************************/
XtCallbackProc DLOG_RADIOM_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioMint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOM,dlog_radioMint, 0);
    return(NULL);
}
/*___DLOG_RADIOM_CB()________________________________________________________*/

/****DLOG_RADION_CB()*********************************************************/
XtCallbackProc DLOG_RADION_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioNint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADION,dlog_radioNint, 0);
    return(NULL);
}
/*___DLOG_RADION_CB()________________________________________________________*/

/****DLOG_RADIOO_CB()*********************************************************/
XtCallbackProc DLOG_RADIOO_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioOint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOO,dlog_radioOint, 0);
    return(NULL);
}
/*___DLOG_RADIOO_CB()________________________________________________________*/

/****DLOG_RADIOP_CB()*********************************************************/
XtCallbackProc DLOG_RADIOP_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioPint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOP,dlog_radioPint, 0);
    return(NULL);
}
/*___DLOG_RADIOP_CB()________________________________________________________*/

/****DLOG_RADIOQ_CB()*********************************************************/
XtCallbackProc DLOG_RADIOQ_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioQint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOQ,dlog_radioQint, 0);
    return(NULL);
}
/*___DLOG_RADIOQ_CB()________________________________________________________*/

/****DLOG_RADIOR_CB()*********************************************************/
XtCallbackProc DLOG_RADIOR_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioK_R(); 
 dlog_radioRint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOR,dlog_radioRint, 0);
    return(NULL);
}
/*___DLOG_RADIOR_CB()________________________________________________________*/

/****clearradioS_Z()**********************************************************/
void clearradioS_Z()
{
 if(Ldlog_radioS) XmToggleButtonSetState(DLOG_RADIOS,dlog_radioSint=0, 0);
 if(Ldlog_radioT) XmToggleButtonSetState(DLOG_RADIOT,dlog_radioTint=0, 0);
 if(Ldlog_radioU) XmToggleButtonSetState(DLOG_RADIOU,dlog_radioUint=0, 0);
 if(Ldlog_radioV) XmToggleButtonSetState(DLOG_RADIOV,dlog_radioVint=0, 0);
 if(Ldlog_radioW) XmToggleButtonSetState(DLOG_RADIOW,dlog_radioWint=0, 0);
 if(Ldlog_radioX) XmToggleButtonSetState(DLOG_RADIOX,dlog_radioXint=0, 0);
 if(Ldlog_radioY) XmToggleButtonSetState(DLOG_RADIOY,dlog_radioYint=0, 0);
 if(Ldlog_radioZ) XmToggleButtonSetState(DLOG_RADIOZ,dlog_radioZint=0, 0);
}
/*___clearradioS_Z()_________________________________________________________*/

/****DLOG_RADIOS_CB()*********************************************************/
XtCallbackProc DLOG_RADIOS_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioSint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOS,dlog_radioSint, 0);
    return(NULL);
}
/*___DLOG_RADIOS_CB()________________________________________________________*/

/****DLOG_RADIOT_CB()*********************************************************/
XtCallbackProc DLOG_RADIOT_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioTint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOT,dlog_radioTint, 0);
    return(NULL);
}
/*___DLOG_RADIOT_CB()________________________________________________________*/

/****DLOG_RADIOU_CB()*********************************************************/
XtCallbackProc DLOG_RADIOU_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioUint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOU,dlog_radioUint, 0);
    return(NULL);
}
/*___DLOG_RADIOU_CB()________________________________________________________*/

/****DLOG_RADIOV_CB()*********************************************************/
XtCallbackProc DLOG_RADIOV_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioVint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOV,dlog_radioVint, 0);
    return(NULL);
}
/*___DLOG_RADIOV_CB()________________________________________________________*/

/****DLOG_RADIOW_CB()*********************************************************/
XtCallbackProc DLOG_RADIOW_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioWint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOW,dlog_radioWint, 0);
    return(NULL);
}
/*___DLOG_RADIOW_CB()________________________________________________________*/

/****DLOG_RADIOX_CB()*********************************************************/
XtCallbackProc DLOG_RADIOX_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioXint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOX,dlog_radioXint, 0);
    return(NULL);
}
/*___DLOG_RADIOX_CB()________________________________________________________*/

/****DLOG_RADIOY_CB()*********************************************************/
XtCallbackProc DLOG_RADIOY_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioYint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOY,dlog_radioYint, 0);
    return(NULL);
}
/*___DLOG_RADIOY_CB()________________________________________________________*/

/****DLOG_RADIOZ_CB()*********************************************************/
XtCallbackProc DLOG_RADIOZ_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
 clearradioS_Z(); 
 dlog_radioZint = 1;  /* touch means select this radio */
 XmToggleButtonSetState(DLOG_RADIOZ,dlog_radioZint, 0);
    return(NULL);
}
/*___DLOG_RADIOZ_CB()________________________________________________________*/


/****DLOG_OK_CB()**** aka DLOG_OKHIT_CB  *************************************/
XtCallbackProc DLOG_OK_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*need separate callback so can set flag that OK button was hit*/
  Ldlog_OKHIT = 1; 
  genericdlogrouter();
    return(NULL);
}
/*___DLOG_OK_CB()____________________________________________________________*/

/****DLOG_CANCEL_CB()********************************************************/
XtCallbackProc DLOG_CANCEL_CB(Widget w,XtPointer client_data 
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that CANCEL button was hit*/
  Ldlog_cancelHIT = 1; 
  genericdlogrouter();
    return(NULL);
}
/*___DLOG_CANCEL_CB()_______________________________________________________*/

/****DLOG_OPTION_CB()********************************************************/
XtCallbackProc DLOG_OPTION_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that OPTION button was hit*/
  Ldlog_optionHIT = 1; 
  genericdlogrouter();
    return(NULL);
}
/*___DLOG_OPTION_CB()_______________________________________________________*/

/****DLOG_EXTRA_CB()*********************************************************/
XtCallbackProc DLOG_EXTRA_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that EXTRA button was hit*/
  Ldlog_extraHIT = 1; 
  genericdlogrouter();
    return(NULL);
}
/*___DLOG_EXTRA_CB()________________________________________________________*/

/****DLOG_EXTRA_CB2()********************************************************/
XtCallbackProc DLOG_EXTRA2_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{/*need separate callback so can set flag that EXTRA2 button was hit*/
  Ldlog_extra2HIT = 1; 
  genericdlogrouter();
    return(NULL);
}
/*___DLOG_EXTRA2_CB()_______________________________________________________*/

/****listboxA_CB()***********************************************************/
XtCallbackProc listboxA_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XmListCallbackStruct *cbs = (XmListCallbackStruct *)call_data;
    listboxAitem = cbs->item_position; /*remember selection in a global*/
    return(NULL);
}
/*___listboxA_CB____________________________________________________________*/

/****listboxB_CB()***********************************************************/
XtCallbackProc listboxB_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XmListCallbackStruct *cbs = (XmListCallbackStruct *)call_data;
    listboxBitem = cbs->item_position; /*remember selection in a global*/
    return(NULL);
}
/*___listboxB_CB____________________________________________________________*/

/****listboxC_CB()***********************************************************/
XtCallbackProc listboxC_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XmListCallbackStruct *cbs = (XmListCallbackStruct *)call_data;
    listboxCitem = cbs->item_position; /*remember selection in a global*/
    return(NULL);
}
/*___listboxC_CB____________________________________________________________*/

/****genericdlogrouter()*****************************************************/ 
void  genericdlogrouter()
{
  int CURRENT_DLOG;
  XtDestroyWidget(genericdlog); 
  
  /*now the generic parameters have been adjusted by the generic dialog*/
  /*need to get back to the specific dialog and set specific parameters */

 CURRENT_DLOG = ACTIVE_DLOG;
 if(ACTIVE_DLOG==KINE_DLOG)
 {
     EndkineDialog();     /*MAGEDLOG.c*/
 /*findkinemage();THIS GOTTA BE WRONG BUT HERE SINCE BEFORE 98*//*MAGEFILE.c*/
 }
 else if(ACTIVE_DLOG==STEREO_DLOG)  EndstereoDialog();
 else if(ACTIVE_DLOG==DRAWNEW_DLOG) EnddrawlineDialog();
 else if(ACTIVE_DLOG==FIND_DLOG)    EndSearchDialog();
 else if(ACTIVE_DLOG==VIEW_DLOG)    EndViewsDialog();
 else if(ACTIVE_DLOG==CHANGE_DLOG)  EndChangeCheckDialog();
 else if(ACTIVE_DLOG==FONT_DLOG)    EndSetgraffontsDialog();
 else if(ACTIVE_DLOG==ROTFORMAT_DLOG)
 {
     EndcoordformatDialog();
     if(ireturnflag==1)
         if(nbondrot > 0)   writeoutput(4); /*MUX_OUT.C*/
 }
 else if(ACTIVE_DLOG==KIPOUTPUT_DLOG)
 {
     EndKipOutputDialog();/*NOT USED 001118*/
     if(ireturnflag==1)
     {
         writeoutput(5); /*MUX_OUT.C*/
     }
 }   
 else if(ACTIVE_DLOG==POSTSCR_DLOG)
 {
     EndPostScriptDialog();
     if(ireturnflag==1)
     {
         writeoutput(6); /*MUX_OUT.C*/
         /*calls inventoutputname(), then dooutputfiledialog(), */
         /*which then bops through here on the way to the masterloop*/
         /*where its waits for a fileout_OK_CB , which calls*/
         /*routeoutput() to get on track to write the postscript file*/
         /*this calls writepostscript(), */
         /* then SetsinGrafWindow() & redrawvec()*/
     }
 }   
 else if(ACTIVE_DLOG==RASTER3D_DLOG)
 {
     EndRaster3DDialog();
     if(ireturnflag==1)
     {
        if(Lraster3Drender == 1)
        {/*really is for Raster3D*/
           writeoutput(8); /*____OUT.C*/
        }
        else
        {/*really is for POV-Ray*/
           writeoutput(9); /*____OUT.C*/
        }
     }
 }   
 else if(ACTIVE_DLOG==DETAIL_DLOG)  EnddetailDialog();
 else if(ACTIVE_DLOG==USERNAME_DLOG)  EndusernameDialog();
 else if(ACTIVE_DLOG==FILENAME_DLOG)  EndfilenameDialog();
 else if(ACTIVE_DLOG==FINESTEP_DLOG)  EndfinestepDialog();
 else if(ACTIVE_DLOG==UPDATE_DLOG)  EndupdateDialog();
 else if(ACTIVE_DLOG==UPDATEPROBE_DLOG)   EndupdateProbeDialog();
 else if(ACTIVE_DLOG==UPDATEPREKIN_DLOG)  EndupdatePrekinDialog();
 else if(ACTIVE_DLOG==UPDATEPIPE_DLOG)  EndupdatePipeDialog(); 
 else if(ACTIVE_DLOG==UPDATESOCKET_DLOG)  EndupdateSocketDialog();
 else if(ACTIVE_DLOG==UPDATECONTOURS_DLOG)  EndupdateContoursDialog();
 else if(ACTIVE_DLOG==UPDATENOES_DLOG)  EndupdateNOEsDialog();
 else if(ACTIVE_DLOG==CONSTRUCTFIFTH_DLOG)  EndConstructFifthDialog();
 else if(ACTIVE_DLOG==CONSTRUCTSIXTH_DLOG)  EndConstructSixthDialog(); //140912
 else if(ACTIVE_DLOG==PICKMARKER_DLOG)  EndpickmarkerDialog(); 
 else if(ACTIVE_DLOG==MAKETEXTHELP_DLOG) EndAdvancedHelpDialog(NEXT_DLOG_PARAM);
 else if(ACTIVE_DLOG==MAKEKINEMAGE_DLOG) EndAdvancedHelpDialog(NEXT_DLOG_PARAM);
 else if(ACTIVE_DLOG==MAKEHTMLHELP_DLOG) EndAdvancedHelpDialog(NEXT_DLOG_PARAM);
 else if(ACTIVE_DLOG==MAKEDOCKOBJECT_DLOG) EndAdvancedHelpDialog(NEXT_DLOG_PARAM);
 else if(ACTIVE_DLOG==BACKGND_DLOG)  EndbackgroundDialog();
 else if(ACTIVE_DLOG==MICKEYMOUSE_DLOG) EndmickeymouseDialog();
 else if(ACTIVE_DLOG==IMPORT_DLOG) EndimportDialog();
 else if(ACTIVE_DLOG==FOOCAVITY_DLOG) EndfoocavityDialog();
 else if(ACTIVE_DLOG==DIMENSION_DLOG) EnddimensionDialog(); /*060121*/
 else if(ACTIVE_DLOG==SELECTING_DLOG) EndSELECTINGlistDialog();/*060121,060622*/
 else
 {/*bug: not defined, fudge and go on*/
    ACTIVE_DLOG = 0;
 }
 
              /*some dialogs call dialogs before dropping through here*/
              /*avoid extra calls to SetsinGrafWindow()*/
              /*which destroys and recreates buttons in button panel*/
 if(NEXT_DLOG==0 && CURRENT_DLOG!=POSTSCR_DLOG && CURRENT_DLOG!=RASTER3D_DLOG)
 { /*Need file written before redrawvec, or another dialog processed*/
     LneedSetsinGrafWindow = 1;  
     if(Ltablepresent && Ltablesearchcells) {redrawtable();}/*000324*/
     redrawvec();              /*MUXMDRAW*/
 }

 if(NEXT_DLOG > 0) 
 {
    DonextanotherDialog();  /*____DLOG.c*/
 }
}
/*___genericdlogrouter()_____________________________________________________*/

/****DonextanotherDialog()****************************************************/
void    DonextanotherDialog()
{
  XtAppAddTimeOut(app_context, 0, (XtTimerCallbackProc)nextanotherDialog, NULL);
}
/*___DonextanotherDialog()___________________________________________________*/

/****nextanotherDialog()******************************************************/
XtTimerCallbackProc nextanotherDialog(XtPointer data)
{
   data = data; /*dummy this, pass parameter as NEXT_DLOG_PARAM*/
   
   if(NEXT_DLOG == UPDATEPIPE_DLOG) 
   {
       NEXT_DLOG = 0;
       DoupdatePipeDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==UPDATEPROBE_DLOG)
   {
       NEXT_DLOG = 0;
       DoupdateProbeDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==UPDATEPREKIN_DLOG) 
   {
       NEXT_DLOG = 0;
       DoupdatePrekinDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==UPDATECONTOURS_DLOG) 
   {
       NEXT_DLOG = 0;
       DoupdateContoursDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==UPDATENOES_DLOG) 
   {
       NEXT_DLOG = 0;
       DoupdateNOEsDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==UPDATESOCKET_DLOG) 
   {
       NEXT_DLOG = 0;
       DoupdateSocketDialog(NEXT_DLOG_PARAM);
   }
   else if(NEXT_DLOG==MAKEDOCKOBJECT_DLOG) 
   {
       NEXT_DLOG = 0;
       DoAdvancedHelpDialog(NEXT_DLOG_PARAM);
   }
   return(NULL);
}
/*___nextanotherDialog()_____________________________________________________*/

