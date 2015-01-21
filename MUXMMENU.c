/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
                     /*MUX_MENU.C*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEMENU.h"  /*121108 now def setmeasuredsurveillance() */
#include "MAGEDLOG.h"
#include "MAGETABL.h"
#include "MAGEANGL.h" /*121108 for resetrot() setrotview() reinitialbondrots()*/
#include "MAGEBBOX.h" /*121108 for SetsinGrafWindow() */

/****mycheckitem()********************************************************/
void    mycheckitem(Widget themenu, Widget theitem, int thestate)
{                                  /*common name: MAC & PC & UNIX*/
    if(LGUI){XmToggleButtonSetState(theitem, thestate, 0);} /*030801*/
     /* 0: don't generate a callback*/
}
/*___mycheckitem()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****myenableitem()**********************************************************/
void myenableitem(Widget themenu, Widget theitem, int thestate)
{
     XtVaSetValues(theitem,XmNsensitive,thestate,NULL);
}
/*___myenableitem()_________________________________________________________*/

/****mymodifymenuitem()******************************************************/
void    mymodifymenuitem(Widget themenu, Widget theitem, char thestring[256])
{
     XmString    xmstring;

     xmstring = XmStringCreateSimple(thestring);
     XtVaSetValues(theitem,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
}
/*___mymodifymenuitem()_____________________________________________________*/

/****checkcurrentviewmenu()**************************************************/
void  checkcurrentviewmenu(int i)
{
    int icheck,imark;
    Widget theItem;
    
    /*awful code since Widgets individually named, not enumerated*/
    for(icheck = 1; icheck <= MAXRESETS; icheck++)
    {
        if(icheck== i) imark = 1;
        else imark = 0;
        if(icheck==1)      theItem = reset1Item;
        else if(icheck==2) theItem = reset2Item;
        else if(icheck==3) theItem = reset3Item;
        else if(icheck==4) theItem = reset4Item;
        else if(icheck==5) theItem = reset5Item;
        else if(icheck==6) theItem = reset6Item;
        else if(icheck==7) theItem = reset7Item;
        else if(icheck==8) theItem = reset8Item;
        else if(icheck==9) theItem = reset9Item;
        else if(icheck==10) theItem = reset10Item;
        else if(icheck==11) theItem = reset11Item;
        else if(icheck==12) theItem = reset12Item;
        else if(icheck==13) theItem = reset13Item;
        else if(icheck==14) theItem = reset14Item;
        else if(icheck==15) theItem = reset15Item;
        else if(icheck==16) theItem = reset16Item;
        else if(icheck==17) theItem = reset17Item;
        else if(icheck==18) theItem = reset18Item;
        else if(icheck==19) theItem = reset19Item;
        else if(icheck==20) theItem = reset20Item;
        else if(icheck==21) theItem = reset21Item;
        else if(icheck==22) theItem = reset22Item;
        else if(icheck==23) theItem = reset23Item;
        else if(icheck==24) theItem = reset24Item;
        else if(icheck==25) theItem = reset25Item;
        else if(icheck==26) theItem = reset26Item;
        else if(icheck==27) theItem = reset27Item;
        else if(icheck==28) theItem = reset28Item;
        else if(icheck==29) theItem = reset29Item;
        else if(icheck==30) theItem = reset30Item;
        else if(icheck==31) theItem = reset31Item;
        else if(icheck==32) theItem = reset32Item;
        else if(icheck==33) theItem = reset33Item;
        else if(icheck==34) theItem = reset34Item;
        else if(icheck==35) theItem = reset35Item;
        else if(icheck==36) theItem = reset36Item;
        else if(icheck==37) theItem = reset37Item;
        else if(icheck==38) theItem = reset38Item;
        else if(icheck==39) theItem = reset39Item;
        else if(icheck==40) theItem = reset40Item;
        else if(icheck==41) theItem = reset41Item;
        else if(icheck==42) theItem = reset42Item;
        else if(icheck==43) theItem = reset43Item;
        else if(icheck==44) theItem = reset44Item;
        else if(icheck==45) theItem = reset45Item;
        else if(icheck==46) theItem = reset46Item;
        else if(icheck==47) theItem = reset47Item;
        else if(icheck==48) theItem = reset48Item;
        else if(icheck==49) theItem = reset49Item;
        else if(icheck==50) theItem = reset50Item;
        else if(icheck==51) theItem = reset51Item;
        else if(icheck==52) theItem = reset52Item;
        else if(icheck==53) theItem = reset53Item;
        else if(icheck==54) theItem = reset54Item;
        else if(icheck==55) theItem = reset55Item;
        else if(icheck==56) theItem = reset56Item;
        else if(icheck==57) theItem = reset57Item;
        else if(icheck==58) theItem = reset58Item;
        else if(icheck==59) theItem = reset59Item;
        else if(icheck==60) theItem = reset60Item;
        else if(icheck==61) theItem = resetRItem;
        XmToggleButtonSetState(theItem, imark, 0);
    }
}
/*___checkcurrentviewmenu()_________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****fileok_CB****************************************************************/
XtCallbackProc fileok_CB(Widget w,XtPointer client_data,XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
  char *filename;
  int  j;

  /*See D.A.Young,1994,pg239*/
  
  
  XmFileSelectionBoxCallbackStruct *cbs =                           
                     (XmFileSelectionBoxCallbackStruct *)call_data; 

  /*destroy open file dialog: always create new one for latest file list*/
  XtDestroyWidget(openfiledlog); 
  
  /*retrieve the filename character string from the compound string format*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  if(Lgetmapfilename==1)
  {/*kinmapfilestr[256] is declared in MAGE.h*/
     for(j=0; j<256; j++)
     {
         kinmapfilestr[j] = filename[j];
         if(kinmapfilestr[j]=='\0') break;
     }
fprintf(stderr,"kinmapfilestr: %s\n",kinmapfilestr);
     Lgetmapfilename = 2; /*Now does have a map file name*/
     ACTIVE_DLOG = 0;
     NEXT_DLOG = UPDATECONTOURS_DLOG;
     DonextanotherDialog();  /*____DLOG.c*/
     return(NULL);
  }
  else if(LgetNOEfilename==1)
  {/*kinNOEfilestr[256] is declared in MAGE.h*/
     for(j=0; j<256; j++)
     {
         kinNOEfilestr[j] = filename[j];
         if(kinNOEfilestr[j]=='\0') break;
     }
fprintf(stderr,"kinNOEfilestr: %s\n",kinNOEfilestr);
     LgetNOEfilename = 2; /*Now does have a NOE file name*/
     LkinNOEfile = 1;
     ACTIVE_DLOG = 0;
     NEXT_DLOG = UPDATENOES_DLOG;
     DonextanotherDialog();  /*____DLOG.c*/
     return(NULL);
  }
  else if(Lgetpdbfilename==1) /*030930*/
  {/*kinpdbfilestr[256] is declared in MAGE.h*/
     for(j=0; j<256; j++)
     {
         kinpdbfilestr[j] = filename[j];
         if(kinpdbfilestr[j]=='\0') break;
     }
fprintf(stderr,"kinpdbfilestr: %s\n",kinpdbfilestr);
     Lgetpdbfilename = 2; /*Now does have a pdb file name*/
     Lkinpdbfile = 1;
     ACTIVE_DLOG = 0;
     NEXT_DLOG = RETURN_DLOG; /*since several dialogs might need a pdb file*/
     DonextanotherDialog();  /*____DLOG.c*/
     return(NULL);
  }
  else if(Lappend)
  {/*AppendNameStr[256] is declared in MAGE.h*/
     for(j=0; j<256; j++)
     {
         AppendNameStr[j] = filename[j];
         if(AppendNameStr[j]=='\0') break;
     }
     Lnewfile = 1; /*flag that new file name is available*/
     ACTIVE_DLOG = 0;
     newfile();  /*MUX_FILE.c, go ahead and process new file*/
     return(NULL);
  }
  else /*is to be the primary kinemage file*/
  {/*NameStr[256] is declared in MAGE.h*/
     for(j=0; j<256; j++)
     {
         NameStr[j] = filename[j];
         if(NameStr[j]=='\0') break;
     }
     Lnewfile = 1; /*flag that new file name is available*/
     ACTIVE_DLOG = 0;
     newfile();  /*MUX_FILE.c, go ahead and process new file*/
     return(NULL);
  }
  ACTIVE_DLOG = 0;
  return(NULL);
}
/*___fileok_CB_______________________________________________________________*/

/****filecancel_CB************************************************************/
XtCallbackProc filecancel_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  /*destroy open file dialog: always create new one for latest file list*/
  XtDestroyWidget(openfiledlog); 
  Lnewfile = 0; /*flag that new file name is NOT available*/
  ACTIVE_DLOG = 0;

  return(NULL);
}
/*___filecancel_CB___________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****open_CB******************************************************************/
XtCallbackProc open_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Lappend = 0;
    Lgetnewfile = 1; /*request to open-new-file */
    ireturnflag = 0;
    changecheckrouter(); /*MAGEINIT.c 020902*/
  return(NULL);
}
/*___open_CB_________________________________________________________________*/

/****append_CB****************************************************************/
XtCallbackProc append_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Lappend = 1; /*request to append stuff from new file onto current kin*/
    Lgetnewfile = 1; /*request to open-new-file */
    /*can go right to open-new-file stuff, since nothing will be lost*/
    OpenNewFile(); /*____DLOG.c*/
  return(NULL);
}
/*___append_CB_______________________________________________________________*/

/****import_CB****************************************************************/
XtCallbackProc import_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Limporton = 1; /*request to import data from remote program*/ /*020514*/
    DoimportDialog(); /*import_CB-->MAGEDLOG.c 020514*/
    return(NULL);
}
/*___import_CB_______________________________________________________________*/

/****quit_CB******************************************************************/
XtCallbackProc quit_CB(Widget w,XtPointer client_data,XtPointer call_data)
{         /* rewritten 971129 */
    if(((XmAnyCallbackStruct *)(call_data))->reason == XmCR_ACTIVATE)
    {
        trytoquitMAGE(); /*____MENU.c*/
    }
  return(NULL);
}
/*___quit_CB_________________________________________________________________*/

/****trytoquitMAGE()**********************************************************/
void trytoquitMAGE() /* 981013 */
{
     LquitMAGE = 1; /*common, to allow multi-step dialog handling*/
     changecheckrouter(); /*MAGEINIT.c 020902*/
}
/*___trytoquitMAGE()_________________________________________________________*/

/****quitMAGE()***************************************************************/
void quitMAGE() /* 971129 */
{
    LquitMAGE = 0; /*bookkeeping*/
#ifdef UNIX_PEX
    Cleanup(); /*MUXPHIG.c, Cleanup phigs structures, close workstation*/
#endif /*UNIX_PEX*/
    /*set flag so masterloop knows to end*/
    Lmagedone = 1;
    /*exit(0); */
}
/*___quitMAGE()______________________________________________________________*/

/****alert_CB()***************************************************************/
XtCallbackProc alert_CB(Widget alertButton,XtPointer client_data
                                          ,XtPointer call_data)
{
     setMageinitDLOGreport(); /*message*/  /*MAGEINIT.C*/
     DoMageDLOGreport(1); /*MUX_DLOG.c */
  return(NULL);
}
/*___alert_CB()______________________________________________________________*/

/****dummy_CB()***************************************************************/
XtCallbackProc dummy_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  return(NULL);
}
/*___dummy_CB()______________________________________________________________*/

/****writeabout_CB()**********************************************************/
XtCallbackProc writeabout_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     menuaboutwrite(0); /*MAGEMENU*/
  return(NULL);
}
/*___writeabout_CB()_________________________________________________________*/

/****screenout_CB()**********************************************************/
XtCallbackProc screenout_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     menuaboutscreenout(0); /*MAGEMENU*/
  return(NULL);
}
/*___screenout_CB()_________________________________________________________*/

/****writeview_CB()**********************************************************/
XtCallbackProc writeview_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    writeoutput(1); /*writes view, inc matrix*/ /*MUX_OUT.c*/
  return(NULL);
}
/*___writeview_CB()_________________________________________________________*/

/****writefocus_CB()**********************************************************/
XtCallbackProc writefocus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    writeoutput(2);/*writes just the focus pt triple*/ /*MUX_OUT.c*/
  return(NULL);
}
/*___writefocus_CB()_________________________________________________________*/

/****stderrfocus_CB()**********************************************************/
XtCallbackProc stderrfocus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   fprintf(stderr,"%s\n",focusxyzstr);
   return(NULL);
}
/*___stderrfocus_CB()_________________________________________________________*/

/****writedrawnew_CB()********************************************************/
XtCallbackProc writedrawnew_CB(Widget w,XtPointer client_data
                                       ,XtPointer call_data)
{
    writeoutput(3);/*MUX_OUT.C calls writedrawnew() MAGEOUT.C*/
  return(NULL);
}
/*___writedrawnew_CB()_______________________________________________________*/

/****writekin_CB()************************************************************/
XtCallbackProc writekin_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*writes entire, possibly modified, kinemage*/
   writeoutput(5); /*MUX_OUT.C*/
   /*mage is going to try to write out, then read back in the kinemage*/
   /*so restoring the various windows asynchonously with the output dialog*/
   /*is not only premature, but seems to cause timing problems*/
   /*SetsinGrafWindow();*/       /*MAGEBBOX.c*/
   /*if(Ltablepresent) {redrawtable();}*/ /*000324*/
   /*redrawvec();*/              /*MUXMDRAW*/
   return(NULL);
}
/*___writekin_CB()_________________________________________________________*/

/****writerotted_CB()*********************************************************/
XtCallbackProc writerotted_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
    DocoordformatDialog();  /*MAGEDLOG.c*/
    /*if(ireturnflag & nbondrot > 0) writeoutput(4);*/ /*writes rotated part*/
    /*taken care of in genericdialogrouter -- which is UNIX exclusive 990405*/
                        /*MACOUT.C*/ /*MPCOUT.C*/
  return(NULL);
}
/*___writerotted_CB()________________________________________________________*/

/****writePostScript_CB()*****************************************************/
XtCallbackProc writePostScript_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
    DoPostScriptDialog(); /*MAGEDLOG.C*/
    return(NULL);
}
/*___writePostScript_CB()____________________________________________________*/

/****writeQA_CB()************************************************************/
XtCallbackProc writeQA_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     writeoutput(7); /*MUX_OUT.C calls writequesanswers*/
  return(NULL);
}
/*___writeQA_CB()_________________________________________________________*/

/****writeR3D_CB()************************************************************/
XtCallbackProc writeR3D_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DoRaster3DDialog();
    redrawvec();  /*000509*/
    return(NULL);
}
/*___writeR3D_CB()_________________________________________________________*/

/****writeptID_CB()***********************************************************/
XtCallbackProc writeptID_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    writeoutput(10); /*000326*/
    return(NULL);
}
/*___writeptID_CB()_________________________________________________________*/

/****writepalette_CB()*******************************************************/
XtCallbackProc writepalette_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    writeoutput(11); /*010604*/
    return(NULL);
}
/*___writepalette_CB()______________________________________________________*/

/****writemeansigma_CB()******************************************************/
XtCallbackProc writemeansigma_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    writeoutput(18); /*MAGEOUT/routeoutput sets Laverageout, 060604*/
    return(NULL);
}
/*___writemeansigma_CB()_____________________________________________________*/

/****editabout_CB()**********************************************************/
XtCallbackProc editabout_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutedit(0); /*MAGEMENU*/
  return(NULL);
}
/*___editabout_CB()_________________________________________________________*/

/****editview_CB()************************************************************/
XtCallbackProc editview_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     DoViewsDialog(0); /*MAGEDLOG.c*/ /*int 0 for use next available view*/
  return(NULL);
}
/*___editview_CB()___________________________________________________________*/

/****drawline_CB()************************************************************/
XtCallbackProc drawline_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setdrawstuff(0); /*MAGEMENU.C*/ /*971129*/
    AdjustMenus();
  return(NULL);
}
/*___drawline_CB()___________________________________________________________*/

/****drawsetup_CB()***********************************************************/
XtCallbackProc drawsetup_CB(Widget w,XtPointer client_data,XtPointer call_data)
{  /*971129*/
    DodrawlineDialog(); /*MAGEDLOG.c*/
  return(NULL);
}
/*___drawsetup_CB()__________________________________________________________*/

/****superpick_CB()***********************************************************/
XtCallbackProc superpick_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setsuperpick(); /*MAGEMENU.C*/
  return(NULL);
}
/*___superpick_CB()__________________________________________________________*/

/****pickcolor_CB()***********************************************************/
XtCallbackProc pickcolor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setcolorshow(); /*MAGEMENU.C*/
  return(NULL);
}
/*___pickcolor_CB()__________________________________________________________*/

/****emphasis_CB()************************************************************/
XtCallbackProc emphasis_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ /*061012 emphasis taken off menu*/
    setemphasis(); /*MAGEMENU.C*/
  return(NULL);
}
/*___emphasis_CB()___________________________________________________________*/

/****pickshow_CB()***********************************************************/
XtCallbackProc pickshow_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setpickshow(); /*MAGEMENU.C*/
  return(NULL);
}
/*___pickshow_CB()__________________________________________________________*/

/****movepoints_CB()*********************************************************/
XtCallbackProc movepoints_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setmovepoints(); /*MAGEMENU.c  061126*/
   return(NULL);
}
/*___movepoints_CB()________________________________________________________*/

/****meansigmas_CB()*********************************************************/
XtCallbackProc meansigmas_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setmeansigmas(); /*MAGEMENU.c  061126*/
   return(NULL);
}
/*___meansigmas_CB()________________________________________________________*/

/****SELECTINGlist_CB()*******************************************************/
XtCallbackProc SELECTINGlist_CB(Widget w,XtPointer client_data,XtPointer call_data)
{  /*060121,060622 list*/
    DoSELECTINGlistDialog(); /*MAGEDLOG.c*/
  return(NULL);
}
/*___SELECTINGlist_CB()______________________________________________________*/

/****SELECTINGcolor_CB()******************************************************/
XtCallbackProc SELECTINGcolor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{  /*061030*/
    DoSELECTINGcolorDialog(); /*MAGEDLOG.c*/
  return(NULL);
}
/*___SELECTINGcolor_CB()_____________________________________________________*/

/****enableTE_CB()***********************************************************/
XtCallbackProc enableTE_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Ltexteditable = !Ltexteditable;
    AdjustMenus();
    resetmenuchecks();
    makecursoractive(); /*MUXMMAIN.c*/
  return(NULL);
}
/*___enableTE_CB()__________________________________________________________*/

/****tablecut_CB()***********************************************************/
XtCallbackProc tablecut_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    tablecelldelete(); /*MAGETABL.c*/
  return(NULL);
}
/*___tablecut_CB()__________________________________________________________*/

/****tablerestore_CB()*******************************************************/
XtCallbackProc tablerestore_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    restoredeletedpoint(); /*MAGETABL.c*/
  return(NULL);
}
/*___tablerestore_CB()______________________________________________________*/

/****tablepreinsert_CB()*****************************************************/
XtCallbackProc tablepreinsert_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    tablecellinsert(-1); /*MAGETABL.c*/
  return(NULL);
}
/*___tablepreinsert_CB()___________________________________________________*/

/****tablepostinsert_CB()***************************************************/
XtCallbackProc tablepostinsert_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    tablecellinsert(+1); /*MAGETABL.c*/
  return(NULL);
}
/*___tablepostinsert_CB()_________________________________________________*/

/****tablerebuild_CB()*****************************************************/
XtCallbackProc tablerebuild_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    tablerebuild(); /*MAGETABL.c*/
  return(NULL);
}
/*___tablerebuild_CB()____________________________________________________*/

/****tablecolor_CB()*******************************************************/
XtCallbackProc tablecolor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    tablecolor(); /*MAGETABL.c*/
  return(NULL);
}
/*___tablecolor_CBtablecolor_CB()________________________________________*/

/****prune_CB()***********************************************************/
XtCallbackProc prune_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setprune(); /*MAGEMENU.C*/
  return(NULL);
}
/*___prune_CB()__________________________________________________________*/

/****showtext_CB()***********************************************************/
XtCallbackProc showtext_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(toptext),XtWindow(toptext));
  return(NULL);
}
/*___showtext_CB()__________________________________________________________*/

/****showcaption_CB()********************************************************/
XtCallbackProc showcaption_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(topcapt),XtWindow(topcapt));
  return(NULL);
}
/*___showcaption_CB()_______________________________________________________*/

/****showgraphics_CB()********************************************************/
XtCallbackProc showgraphics_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(topgraf),XtWindow(topgraf));
  return(NULL);
}
/*___showgraphics_CB()_______________________________________________________*/

/****showbuttonwindow_CB()****************************************************/
XtCallbackProc showbuttonwindow_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(topbutt),XtWindow(topbutt));
  return(NULL);
}
/*___showbuttonwindow_CB()___________________________________________________*/

/****showtable_CB()***********************************************************/
XtCallbackProc showtable_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(toptable),XtWindow(toptable));
  return(NULL);
}
/*___showtable_CB()__________________________________________________________*/

/****showbondrots_CB()********************************************************/
XtCallbackProc showbondrots_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
    XRaiseWindow(XtDisplay(topbrot),XtWindow(topbrot));
  return(NULL);
}
/*___showbondrots_CB()_______________________________________________________*/

/****next_CB()****************************************************************/
XtCallbackProc next_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Lgetnextkinemage = 1; /*971129*/
    changecheckrouter(); /*MAGEINIT.c 020902*/
    return(NULL);
}
/*___next_CB()_______________________________________________________________*/

/****choose_CB()**************************************************************/
XtCallbackProc choose_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Lgetkinemagechoice = 1; /*971129*/
    changecheckrouter(); /*MAGEINIT.c 020902*/
    return(NULL);
}
/*___choose_CB()_____________________________________________________________*/

/****scoreNnext_CB()**********************************************************/
XtCallbackProc scoreNnext_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Ltesttimeend =1;/*for magetimercheck() timed-test branch of main loop*/
    return(NULL);
}
/*___scoreNnext_CB()_________________________________________________________*/

/****scoreNstay_CB()**********************************************************/
XtCallbackProc scoreNstay_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Scorepositive = 0;
    Scorenegative = 0;
    Scoretotal = 0;
    scoretest();  /*MAGESCOR.c*/
    rescalekinemage();  /*MAGEINPT.c*/
    LneedSetsinGrafWindow = 1; 
    redrawvec();
    return(NULL);
}
/*___scoreNstay_CB()_________________________________________________________*/

/****optionsabout_CB()********************************************************/
XtCallbackProc optionsabout_CB(Widget w,XtPointer client_data
                                       ,XtPointer call_data)
{
   menuaboutoptions(0); /*MAGEMENU*/
   return(NULL);
}
/*___optionsabout_CB()_______________________________________________________*/

/****multiwidth_CB()**********************************************************/
XtCallbackProc multiwidth_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setmultiwidth();  /*MAGEMENU.c*/
   return(NULL);
}
/*___multiwidth_CB()_________________________________________________________*/

/****width_CB()***************************************************************/
XtCallbackProc width_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setwidthtoggle();  /*MAGEMENU.c*/
   return(NULL);
}
/*___width_CB()______________________________________________________________*/

/****thin_CB()****************************************************************/
XtCallbackProc thin_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setthinline();   /*MAGEMENU.c*/
   return(NULL);
}
/*___thin_CB()_______________________________________________________________*/

/****depth5_CB()**************************************************************/
XtCallbackProc depth5_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
              L5depths = !L5depths;
              if(L5depths) {L3depths = 0;}
              else {L3depths = 1;}
              setNdepths();       /*MAGEMENU.C*/ /*981114*/
   return(NULL);
}
/*___depth5_CB()_____________________________________________________________*/

/****depth3_CB()**************************************************************/
XtCallbackProc depth3_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
              L3depths = !L3depths;
              if(L3depths) {L5depths = 0;}
              else {L5depths = 1;}
              setNdepths();       /*MAGEMENU.C*/ /*010927*/
   return(NULL);
}
/*___depth3_CB()_____________________________________________________________*/

/****perspec_CB()*************************************************************/
XtCallbackProc perspec_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setperspec();   /*MAGEMENU.c*/
   return(NULL);
}
/*___perspec_CB()____________________________________________________________*/

/****whtbkg_CB()**************************************************************/
XtCallbackProc whtbkg_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setwhtbkg(); /*MAGEMENU.C*/
   return(NULL);
}
/*___whtbkg_CB()_____________________________________________________________*/

/****B_W_CB()*****************************************************************/
XtCallbackProc B_W_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    /*if color possible, UNIX just fakes a B&W display*/
    Lcolor=!Lcolor;
    mycheckitem(optiMenu, B_WItem, !Lcolor);
    if(!Lcolor) Lwhitebkg = 1;
    mycheckitem(optiMenu, whtbkgItem, Lwhitebkg);
    redrawvec();    /*___DRAW.C*/
   return(NULL);
}
/*___B_W_CB()________________________________________________________________*/

/****gray_CB()*****************************************************************/
XtCallbackProc gray_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    /*Lcolor and LLcolor date back to the first Macs which were often gray*/
    /*but sometimes had real color... then the first UNIX systems had many*/
    /*ways of describing the capabilities of the Xt color possibilities*/
    /*040616 gray override is patched onto this history: */
    /*everything is treated as the gray color on a color displaydevice*/
    Lgrayscale=!Lgrayscale;
    mycheckitem(optiMenu, grayItem, Lgrayscale);
    redrawvec();    /*___DRAW.C*/
    return(NULL);
}
/*___gray_CB()________________________________________________________________*/

/****aspect#_CB()**************************************************************/
XtCallbackProc aspect0_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(0); return(NULL); }
XtCallbackProc aspect1_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(1); return(NULL); }
XtCallbackProc aspect2_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(2); return(NULL); }
XtCallbackProc aspect3_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(3); return(NULL); }
XtCallbackProc aspect4_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(4); return(NULL); }
XtCallbackProc aspect5_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(5); return(NULL); }
XtCallbackProc aspect6_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(6); return(NULL); }
XtCallbackProc aspect7_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(7); return(NULL); }
XtCallbackProc aspect8_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(8); return(NULL); }
XtCallbackProc aspect9_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(9); return(NULL); }
XtCallbackProc aspect10_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(10); return(NULL); }
XtCallbackProc aspect11_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(11); return(NULL); }
XtCallbackProc aspect12_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(12); return(NULL); }
XtCallbackProc aspect13_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(13); return(NULL); }
XtCallbackProc aspect14_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(14); return(NULL); }
XtCallbackProc aspect15_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(15); return(NULL); }
XtCallbackProc aspect16_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ setcurrentaspect(16); return(NULL); }
XtCallbackProc aspectscrn_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    Laspectonscreen = !Laspectonscreen;
    sprintf(temps,"on screen");
    setaspectonscreen(temps,Laspectonscreen);
    return(NULL); 
 }

/****view#_CB()***************************************************************/
XtCallbackProc view1_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(1); return(NULL); }
XtCallbackProc view2_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(2); return(NULL); }
XtCallbackProc view3_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(3); return(NULL); }
XtCallbackProc view4_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(4); return(NULL); }
XtCallbackProc view5_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(5); return(NULL); }
XtCallbackProc view6_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(6); return(NULL); }
XtCallbackProc view7_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(7); return(NULL); }
XtCallbackProc view8_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(8); return(NULL); }
XtCallbackProc view9_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(9); return(NULL); }
XtCallbackProc view10_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(10); return(NULL); }
XtCallbackProc view11_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(11); return(NULL); }
XtCallbackProc view12_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(12); return(NULL); }
XtCallbackProc view13_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(13); return(NULL); }
XtCallbackProc view14_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(14); return(NULL); }
XtCallbackProc view15_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(15); return(NULL); }
XtCallbackProc view16_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(16); return(NULL); }
XtCallbackProc view17_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(17); return(NULL); }
XtCallbackProc view18_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(18); return(NULL); }
XtCallbackProc view19_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(19); return(NULL); }
XtCallbackProc view20_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(20); return(NULL); }
XtCallbackProc view21_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(21); return(NULL); }
XtCallbackProc view22_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(22); return(NULL); }
XtCallbackProc view23_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(23); return(NULL); }
XtCallbackProc view24_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(24); return(NULL); }
XtCallbackProc view25_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(25); return(NULL); }
XtCallbackProc view26_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(26); return(NULL); }
XtCallbackProc view27_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(27); return(NULL); }
XtCallbackProc view28_CB(Widget w,XtPointer client_data,XtPointer call_data)
{ processView(28); return(NULL); }
XtCallbackProc view29_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(29); return(NULL); }
XtCallbackProc view30_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(30); return(NULL); }
XtCallbackProc view31_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(31); return(NULL); }
XtCallbackProc view32_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(32); return(NULL); }
XtCallbackProc view33_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(33); return(NULL); }
XtCallbackProc view34_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(34); return(NULL); }
XtCallbackProc view35_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(35); return(NULL); }
XtCallbackProc view36_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(36); return(NULL); }
XtCallbackProc view37_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(37); return(NULL); }
XtCallbackProc view38_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(38); return(NULL); }
XtCallbackProc view39_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(39); return(NULL); }
XtCallbackProc view40_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(40); return(NULL); }
XtCallbackProc view41_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(41); return(NULL); }
XtCallbackProc view42_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(42); return(NULL); }
XtCallbackProc view43_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(43); return(NULL); }
XtCallbackProc view44_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(44); return(NULL); }
XtCallbackProc view45_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(45); return(NULL); }
XtCallbackProc view46_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(46); return(NULL); }
XtCallbackProc view47_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(47); return(NULL); }
XtCallbackProc view48_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(48); return(NULL); }
XtCallbackProc view49_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(49); return(NULL); }
XtCallbackProc view50_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(50); return(NULL); }
XtCallbackProc view51_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(51); return(NULL); }
XtCallbackProc view52_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(52); return(NULL); }
XtCallbackProc view53_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(53); return(NULL); }
XtCallbackProc view54_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(54); return(NULL); }
XtCallbackProc view55_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(55); return(NULL); }
XtCallbackProc view56_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(56); return(NULL); }
XtCallbackProc view57_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(57); return(NULL); }
XtCallbackProc view58_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(58); return(NULL); }
XtCallbackProc view59_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(59); return(NULL); }
XtCallbackProc view60_CB(Widget w,XtPointer client_data,XtPointer call_data)
{      processView(60); return(NULL); }

/****viewR_CB()***************************************************************/
XtCallbackProc viewR_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   processView(MAXRESETS);
   return(NULL);
}
/*___viewR_CB()______________________________________________________________*/

/****processView()************************************************************/
void processView(int j)
{
   resetrot(j);
#ifdef UNIX_PEX
   resetalltransformations(); /*MUXPHIG.c*/
   knobmotionhandler();       /*MUXPHIG.c*/
#endif
   redrawvec(); 
}
/*___processView()___________________________________________________________*/

/****otherabout_CB()********************************************************/
XtCallbackProc otherabout_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutOther(0); /*MAGEMENU*/
   return(NULL);
}
/*___otherabout_CB()_______________________________________________________*/


/****stereo_CB()**************************************************************/
XtCallbackProc stereo_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     setstereo();  /*MAGEMENU.C*/
   return(NULL);
}
/*___stereo_CB_______________________________________________________________*/

/****compare_CB()*************************************************************/
XtCallbackProc compare_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     setcompare();  /*MAGEMENU.C*/
   return(NULL);
}
/*___compare_CB______________________________________________________________*/

/****listcolor_CB()***********************************************************/
XtCallbackProc listcolor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     setlistcolordominance();  /*MAGEMENU.C*/
   return(NULL);
}
/*___listcolor_CB____________________________________________________________*/
 
/****measure_CB()*************************************************************/
XtCallbackProc measure_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     setmeasures(); 

#ifdef UNIX_PEX
     setpexmeasures();
#endif /*UNIX_PEX*/
#ifdef FOOL
     LneedSetsinGrafWindow = 1; /*deal with measures toggle button*/
#endif
   return(NULL);
}
/*___measure_CB()____________________________________________________________*/

/****search_CB()*************************************************************/
XtCallbackProc search_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     DoSearchDialog();  /*___DLOG.c*/
     /*SetsinGrafWindow();*/   /*MAGEBBOX.C*/
     /*redrawvec();*/
   return(NULL);
}
/*___search_CB______________________________________________________________*/

/****findagain_CB()**********************************************************/
XtCallbackProc findagain_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
     DoSearchAgain();    /*MAGEUTIL.c*/ /*971204*/
     /*SetsinGrafWindow();*/   /*MAGEBBOX.C*/
     /*redrawvec();*/
   return(NULL);
}
/*___findagain_CB____________________________________________________________*/

/****readersview_CB()*********************************************************/
XtCallbackProc readersview_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
     sprintf(viewcom[MAXRESETS],"Reader's View");
     sprintf(temps,"%s",viewcom[MAXRESETS]);
     condition(temps);
     /*SetItem(viewMenu,MAXRESETS,(unsigned char *)temps);*/
     setviewidentifier(MAXRESETS,temps); /*980927*/ /*MAGEMENU.c*/
     checkcurrentviewmenu(MAXRESETS); /*.*/

     Lzoomer[MAXRESETS] = 1; /*store zoom as a scaling value 980930*/
     setrotview(MAXRESETS); /*MAXRESETS==reader's view*/ /*MAGEANGL.C*/
     AdjustMenus(); /*MAGEMENU.C*/
   return(NULL);
}
/*___readersview_CB__________________________________________________________*/

/****NDaxesPickedCtr_CB()*****************************************************/
XtCallbackProc NDaxesPickedCtr_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
   LNDaxesPickedCtr = !LNDaxesPickedCtr;
   mycheckitem(viewMenu, NDaxesPickedCtrItem, LNDaxesPickedCtr);
   return(NULL);
}
/*___NDaxesPickedCtr_CB______________________________________________________*/

/****undopickctr_CB()*********************************************************/
XtCallbackProc undopickctr_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
     doundopickcenter();  /*MAGEMENU.C*/
   return(NULL);
}
/*___undopickctr_CB__________________________________________________________*/

/****kludgesabout_CB()********************************************************/
XtCallbackProc kludgesabout_CB(Widget w,XtPointer client_data
                                       ,XtPointer call_data)
{
   menuaboutkluges(0); /*MAGEMENU.c*/
   return(NULL);
}
/*___kludgesabout_CB_________________________________________________________*/

/****GFont_CB()********************************************************/
XtCallbackProc GFont_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DoSetgraffontsDialog();  /*___DLOG.c*/
   return(NULL);
}
/*___GFont_CB_________________________________________________________*/

/****strokedfont_CB()********************************************************/
XtCallbackProc strokedfont_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setstrokedfont();  /*MAGEMENU.c*/
    return(NULL);
}
/*___strokedfont_CB_________________________________________________________*/

/****sterang_CB()********************************************************/
XtCallbackProc sterang_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DostereoDialog();  /*___DLOG.c*/
   return(NULL);
}
/*___sterang_CB_________________________________________________________*/

/****detail_CB()********************************************************/
XtCallbackProc detail_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DodetailDialog();  /*___DLOG.c*/
   return(NULL);
}
/*___detail_CB_________________________________________________________*/

/****lens_CB()********************************************************/
XtCallbackProc lens_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setLens(); /*MAGEMENU.C*/
   return(NULL);
}
/*___lens_CB_________________________________________________________*/

/****xyzpoint_CB()********************************************************/
XtCallbackProc xyzpoint_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setxyzpoint();       /*MAGEMENU.C*/
   return(NULL);
}
/*___xyzpoint_CB_________________________________________________________*/

/****gnomon_CB()********************************************************/
XtCallbackProc gnomon_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setgnomon();       /*MAGEMENU.C*/
   return(NULL);
}
/*___gnomon_CB_________________________________________________________*/

/****singlemarker_CB()********************************************************/
XtCallbackProc singlemarker_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setsinglemarker();       /*MAGEMENU.C*/
   return(NULL);
}
/*___singlemarker_CB_________________________________________________________*/

/****hidemeasures_CB()********************************************************/
XtCallbackProc hidemeasures_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    potentiateMeasures(); /*MAGEMENU.c*/
   return(NULL);
}
/*___hidemeasures_CB_________________________________________________________*/

/****brotangle_CB()********************************************************/
XtCallbackProc brotangle_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    reinitialbondrots();       /*MAGEANGL.C*/
   return(NULL);
}
/*___brotangle_CB_________________________________________________________*/

/****rot90plus_CB()********************************************************/
XtCallbackProc rot90plus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*y axis*/
    getrot(0,0, 2);  /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90plus_CB_________________________________________________________*/

/****rot90minus_CB()********************************************************/
XtCallbackProc rot90minus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*y axis*/
    getrot(0,0,-2); /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90minus_CB_________________________________________________________*/

/****rot90xplus_CB()********************************************************/
XtCallbackProc rot90xplus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*x axis*/
    getrot(0,0, 1);  /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90xplus_CB_________________________________________________________*/

/****rot90xminus_CB()********************************************************/
XtCallbackProc rot90xminus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*x axis*/
    getrot(0,0,-1); /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90xminus_CB_________________________________________________________*/

/****rot90zplus_CB()********************************************************/
XtCallbackProc rot90zplus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*z axis*/
    getrot(0,0, 3);  /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90zplus_CB_________________________________________________________*/

/****rot90zminus_CB()********************************************************/
XtCallbackProc rot90zminus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*z axis*/
    getrot(0,0,-3); /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___rot90zminus_CB_________________________________________________________*/

/****stereoplus_CB()********************************************************/
XtCallbackProc stereoplus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*y axis stereo angle*/
    getrot(0,0, 4);  /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___stereoplus_CB_________________________________________________________*/

/****stereominus_CB()********************************************************/
XtCallbackProc stereominus_CB(Widget w,XtPointer client_data,XtPointer call_data)
{/*y axis stereoangle*/
    getrot(0,0,-4); /*MAGEDRAW*/
    redrawvec();
   return(NULL);
}
/*___stereominus_CB_________________________________________________________*/

/****count_CB()********************************************************/
XtCallbackProc count_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    countredraws(); /* MUX_MAIN.c */ 
   return(NULL);
}
/*___count_CB_________________________________________________________*/

/****rate_CB()********************************************************/
XtCallbackProc rate_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setrate();       /*MAGEMENU.C*/
   return(NULL);
}
/*___rate_CB_________________________________________________________*/

/****clipmiddle_CB()********************************************************/
XtCallbackProc clipmiddle_CB(Widget w,XtPointer client_data,XtPointer call_data)
{     /*051214*/
   setclipmiddle();       /*MAGEMENU.C*/
   return(NULL);
}
/*___clipmiddle_CB_________________________________________________________*/

/****measperp_CB()********************************************************/
XtCallbackProc measperp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setmeasureperpendic();       /*MAGEMENU.C*/
   return(NULL);
}
/*___measperp_CB_________________________________________________________*/

/****measuredsurveil_CB()*****************************************************/
XtCallbackProc measuredsurveil_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   setmeasuredsurveillance();       /*MAGEMENU.C*/
   return(NULL);
}
/*___measuredsurveil_CB______________________________________________________*/

/****formattest_CB()********************************************************/
XtCallbackProc formattest_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setformattest();       /*MAGEMENU.C*/
   return(NULL);
}
/*___formattest_CB_________________________________________________________*/

/****test_CB()********************************************************/
XtCallbackProc test_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    settest();       /*MAGEMENU.C*/
   return(NULL);
}
/*___test_CB_________________________________________________________*/

/****secret_CB()********************************************************/
XtCallbackProc secret_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutsecrets(0);       /*MAGEMENU.C*/
   return(NULL);
}
/*___secret_CB_________________________________________________________*/

/****plotonly_CB()********************************************************/
XtCallbackProc plotonly_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setplotonly(); /*kalide*/      /*MAGEMENU.C*/
   return(NULL);
}
/*___plotonly_CB_________________________________________________________*/

/****fontcursor_CB()********************************************************/
XtCallbackProc fontcursor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setfontcursor(); /*font choice for Linux-UNIX*/      /*MUXMINIT.c*/
   return(NULL);
}
/*___fontcursor_CB_________________________________________________________*/
/****flat_CB()*************************************************************/
XtCallbackProc flat_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    setflat(); /*flatland*/ /*MAGEMENU.C*/
   return(NULL);
}
/*___flat_CB______________________________________________________________*/

/****horizon_CB()**********************************************************/
XtCallbackProc horizon_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    sethorizon(); /*maintain horizon, only Y rotation*/ /*MAGEMENU.C*/
   return(NULL);
}
/*___horizon_CB____________________________________________________________*/

/****dockingscope_CB()*****************************************************/
XtCallbackProc dockingscope_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    setdockingscope(); /*MAGEMENU.C*/  /*000413*/ 
    return(NULL);
}
/*___dockingscope_CB______________________________________________________*/

/****zlimitscope_CB()*****************************************************/
XtCallbackProc zlimitscope_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    setzlimitscope(); /*MAGEMENU.C*/  /*011201*/ 
    return(NULL);
}
/*___zlimitscope_CB______________________________________________________*/

/****finestep_CB()********************************************************/
XtCallbackProc finestep_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DofinestepDialog();  /*___DLOG.c*/
   return(NULL);
}
/*___finestep_CB_________________________________________________________*/

/****pickmarker_CB()*****************************************************/
XtCallbackProc pickmarker_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    DopickmarkerDialog(); /*MAGEDLOG.C*/  /*000611*/ 
    return(NULL);
}
/*___pickmarker_CB______________________________________________________*/

/****visibleout_CB()*****************************************************/
XtCallbackProc visibleout_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    setvisibleout(); /*MAGEMENU.c*/
    return(NULL);
}
/*___visibleout_CB______________________________________________________*/

/****pdbformatout_CB()*****************************************************/
XtCallbackProc pdbformatout_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    setpdbformat(); /*MAGEMENU.c*/
    return(NULL);
}
/*___pdbformatout_CB______________________________________________________*/

/****dimension_CB()***********************************************************/
XtCallbackProc dimension_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    DodimensionDialog(); /*MAGEDLOG.c*/
    return(NULL);
}
/*___dimension_CB____________________________________________________________*/

/****rescoring_CB()*****************************************************/
XtCallbackProc rescoring_CB(Widget w,XtPointer client_d,XtPointer call_d)
{/*only UNIX/Linux can report reliably to a console window*/
    if(scoretest() ) /*MAGESCOR.c menu-chosen scoring*/
    {
       fprintf(stderr
       ,"Kinemage %d test score:  %d correct, out of %d,   %d wrong\n"
       ,thiskinenum,Scorepositive,Scoretotal,Scorenegative);
    }
    else
    {
       fprintf(stderr,"Kinemage %d test scoring FAILED\n",thiskinenum);
    }
    return(NULL);
}
/*___rescoring_CB______________________________________________________*/

/****background_CB()********************************************************/
XtCallbackProc background_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DobackgroundDialog();  /*MAGEDLOG.c*/
    return(NULL);
}
/*___background_CB_________________________________________________________*/

/****buttonwindow_CB()********************************************************/
XtCallbackProc buttonwindow_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
 /*static int BWIDTHSAVE=0;*/

    Lbuttonseparate = !Lbuttonseparate;
    if(Lbuttonseparate)
    {
/*       BWIDTHSAVE = BWIDTH;*/ /*BWIDTH stuff was commented out < 060613 */
/*       BWIDTH = 0;         */ /* because just doesn't work! */
       createbuttonwindow();
    }
    else
    {
       destroybuttonwindow();
/*       BWIDTH = BWIDTHSAVE; */
    }
/*    resizegrafwindowgently();*/ /*actual resize routine 060613*/
    AdjustMenus();  /*060613 for showbuttwinItem*/
    SetsinGrafWindow();
    return(NULL);
}
/*___buttonwindow_CB_________________________________________________________*/

/****foocavity_CB()********************************************************/
XtCallbackProc foocavity_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   DofoocavityDialog();
   return(NULL);
}
/*___foocavity_CB_________________________________________________________*/

/****allbrslider_CB()********************************************************/
XtCallbackProc allbrslider_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
   Lallbondrotsliders = !Lallbondrotsliders; /*allbrslider_CB 070617*/
   if(!Lallbondrotsliders){killbondrotsliders(0);} /*0==just sliders 070617*/
   AdjustMenus(); 
   SetsinGrafWindow();
   return(NULL);
}
/*___allbrslider_CB_________________________________________________________*/

/****mickeymouse_CB()********************************************************/
XtCallbackProc mickeymouse_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    DomickeymouseDialog();  /*MAGEDLOG.c*/
    return(NULL);
}
/*___mickeymouse_CB_________________________________________________________*/

/****remoteupdate_CB()*****************************************************/
XtCallbackProc remoteupdate_CB(Widget w,XtPointer client_d,XtPointer call_d)
{
    setremoteupdate(); /*MAGEMENU.c*/
   return(NULL);
}
/*___remoteupdate_CB______________________________________________________*/

/****specialabout_CB()********************************************************/
XtCallbackProc specialabout_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutspecial(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___specialabout_CB_________________________________________________________*/

/****mousehelp_CB()********************************************************/
XtCallbackProc mousehelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutmouse(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___mousehelp_CB_________________________________________________________*/

/****dockinghelp_CB()********************************************************/
XtCallbackProc dockinghelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutdocking(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___dockinghelp_CB_________________________________________________________*/

/****resizehelp_CB()********************************************************/
XtCallbackProc resizehelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutresize(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___resizehelp_CB_________________________________________________________*/

/****tablehelp_CB()********************************************************/
XtCallbackProc tablehelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuabouttable(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___tablehelp_CB_________________________________________________________*/

/****bondshelp_CB()********************************************************/
XtCallbackProc bondshelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutbonds(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___bondshelp_CB_________________________________________________________*/

/****NDIMhelp_CB()*********************************************************/
XtCallbackProc NDIMhelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuaboutNDIM(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___NDIMhelp_CB___________________________________________________________*/

/****test3Dhelp_CB()********************************************************/
XtCallbackProc test3Dhelp_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuabouttest3D(0);    /*MAGEMENU.C*/
   return(NULL);
}
/*___test3Dhelp_CB_________________________________________________________*/

/****makehelptext_CB()********************************************************/
XtCallbackProc makehelptext_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuadvancedhelp(1);    /*MAGEMENU.C*/
   return(NULL);
}
/*___makehelptext_CB_________________________________________________________*/

/****makekinemage_CB()********************************************************/
XtCallbackProc makekinemage_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuadvancedhelp(2);    /*MAGEMENU.C*/
   return(NULL);
}
/*___makekinemage_CB_________________________________________________________*/

/****makehelphtml_CB()********************************************************/
XtCallbackProc makehelphtml_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
    menuadvancedhelp(3);    /*MAGEMENU.C*/
   return(NULL);
}
/*___makehelphtml_CB_________________________________________________________*/

/****drawArea_CB()************************************************************/
XtCallbackProc drawArea_CB(Widget drawArea,XtPointer client_data
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
/*printf("drawArea_CB count==%d\n",j);*/

      if(j==0)
      {
         if(!ACTIVE_DLOG && !Lresizeactive) /*040523 Lresizeactive*/
         {/*grafwindow maybe really exposed and needing to be redrawn*/
#ifdef UNDEFINED
            predraw_all_structs(WS, PFLAG_ALWAYS);
#endif
            /*SetsinGrafWindow(); */  /*MAGEBBOX.C*/
            redrawvec();
            /*redraws all structures on the workstation*/
         }/*grafwindow maybe really exposed and needing to be redrawn*/
      }
   }/*real expose event*/
   return(NULL);
}
/*___drawArea_CB_____________________________________________________________*/

/****resize_CB()**************************************************************/
XtCallbackProc resize_CB(Widget drawArea,XtPointer client_data
                                        ,XtPointer call_data)
{
   /*why bother to check to see if resize_CB was called by a resize cmd????*/
   if(((XmAnyCallbackStruct *) call_data)->reason == XmCR_RESIZE)
   {/*real resize event*/
      resizegrafwindowgently(); /*060613 also call when make Button Window*/
   }/*real resize event*/
 
   return(NULL);
}
/*___resize_CB()_____________________________________________________________*/

/****resizegrafwindowgently()*************************************************/
void resizegrafwindowgently() /*060613 also call when make Button Window*/
{
   int msecinterval=1000;
      /*initiate or reset timer*/
      /*if(Timerforresize != NULL)*/
      if(Timerforresize != 0)/*040605 XtIntervalID is some kind of integer????*/
      {
         XtRemoveTimeOut(Timerforresize);
      }
      Timerforresize = XtAppAddTimeOut(app_context, msecinterval, 
                       (XtTimerCallbackProc)magedelayresize, NULL);
      Lresizeactive = 1; /*1 for delay to also inhibit exposure callbacks*/
}
/*___resizegrafwindowgently()________________________________________________*/

/****resizegrafwindow()*******************************************************/
void resizegrafwindow(int wide, int high) /*041031*/
{
   Arg arg[10];
   int n=0;
   int left, top, right, bottom;

   XtSetArg(arg[n], XmNwidth, wide+BWIDTH+ZWIDTH); n++;
   XtSetArg(arg[n], XmNheight,high+MESSHEIGHT+MENUHEIGHT); n++;
/*   XtSetValues(grafwindow, arg, n);*/ /*060613  what level to size??*/
   XtSetValues(topgraf, arg, n); /*060613 must be topgraf */

/*   getgrafwinedges(&left,&top,&right,&bottom);*/ /*060613 why do this??*/
}
/*___resizegrafwindow()______________________________________________________*/

/****magedelayresize()********************************************************/
XtTimerCallbackProc magedelayresize(XtPointer data) /*040523*/
{
   data = data; /*dummy, not used*/
   Lresizeactive = 0; /*flag 1 for delay*/
   doresize();
   return(NULL);
}
/*___magedelayresize()_______________________________________________________*/

/****doresize()***************************************************************/
void doresize() /*split resize_CB to avoid hang doing refill during resize*/
{
   /*MacOSX and Gnome seem to have no way to do resize without continually*/
   /*showing content, this generates a series of resize events that totally*/
   /*clog Mage event que when Mage showing a very large kinemage*/
   /*Trick is to reset a Timer every time get a resize event, only do a */
   /*single resize when last Timer-interval expires after resizing ends*/
 
   int left, top, right, bottom;

   getgrafwinedges(&left,&top,&right,&bottom);
   /*sprintf(dstring,"width== %d, height== %d",right,bottom);*/
      /*added 041031, commented out 060613*/
   /*MUXMBAR.c sets L,T,R,B = 0,0,width,height */
   if(GWIDTH != right || GHEIGHT != bottom)
   {/*only reset size if size actually changed*/
     GWIDTH = right;
     GHEIGHT = bottom;
     XFreePixmap(dpy,offscreen);
     XFreeGC(dpy,gc);
     XFreeGC(dpy,offgc);
     offscreen = XCreatePixmap(dpy,RootWindow(dpy,DefaultScreen(dpy))
                               ,GWIDTH,GHEIGHT,magedepth);

     gc = XCreateGC(dpy,drawWindow,0L,NULL);
     XSetFont(dpy,gc,textfont->fid);
     XSetForeground(dpy,gc,WhitePixel(dpy,DefaultScreen(dpy)));
     XSetBackground(dpy,gc,BlackPixel(dpy,DefaultScreen(dpy)));

     offgc = XCreateGC(dpy,offscreen,0L,NULL);
     XSetFont(dpy,offgc,textfont->fid);
     XSetForeground(dpy,offgc,WhitePixel(dpy,DefaultScreen(dpy)));
     XSetBackground(dpy,offgc,BlackPixel(dpy,DefaultScreen(dpy)));

        if(GWIDTH<GHEIGHT) oldmaxwide = GWIDTH;
        else               oldmaxwide = GHEIGHT;
        GZDEEP = oldmaxwide;
        ieyewide = GWIDTH/2;
        HEIGHT = GHEIGHT; /*variables used in MAGEDRAW/drawvec*/
        WIDTH = GWIDTH;
        Scale = oldmaxwide/fdiff;
        scalenew = Scale*zoom;
        /*original settings GWIDTH==400,izclip == 200*/
        /*fzclip == fUDGE FACTOR FOR zclip*/
        fzclip = (float)oldmaxwide/400.0 ;/*recompute fudge factor*/
        izclip = fzclip*izclipold[0]; /*refudge izclip*/
            /*working zclip set from stored value and graphics size*/
        izlimit = fzclip*izlimit; /*refudge izlimit*/
        iztran = fzclip*iztranold[0]; /*refudge iztran*/
        Loriginatctr = 0;
        ixctr = GWIDTH/2;
        iyctr = GHEIGHT/2;
        izctr = GZDEEP/2;

        /*Lrecalculate = 1;*/
        rescalekinemage(); /*MAGEINPT.c*/
        SetsinGrafWindow();  /*MAGEBBOX.C, so why was this jumpered out?*/
        redrawvec();
   }/*only reset size if size actually changed*/
}
/*___doresize()______________________________________________________________*/

/****showthegrafwindow()******************************************************/
void showthegrafwindow()
{
    XRaiseWindow(XtDisplay(topgraf),XtWindow(topgraf));
}
/*___showthegrafwindow()_____________________________________________________*/
 
/****mylabelbutton()***MUX***************************************************/
void    mylabelbutton(ControlHandle button,char name[16]) /*common name*/
{
     XmString    xmstring;

     xmstring = XmStringCreateSimple(name);
     XtVaSetValues(button,XmNlabelString,xmstring,NULL);
     XmStringFree(xmstring);
}
/*___mylabelbutton()___MUX__________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUX_MENU.c*/
