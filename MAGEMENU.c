/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 
                      /*MAGEMENU.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#define EXTERNMENU  
#include "MAGEMENU.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h" /*971122*/
#include "MAGEDLOG.h" /*971129*/
#include "MAGEANGL.h" /*971129*/
#include "MAGETABL.h"

int     Leavingflatland = 0; /*declared extern in  MAGEFLAG.h*/ /*991117*/

/****AdjustMenus()***********************************************************/
void    AdjustMenus()
{
  myenableitem(fileMenu, openItem, !Ltestmode);
     /*open for input file always enabled except after entered timed test mode*/
  myenableitem(fileMenu, appendItem, Lkinfile && !Ltestmode);
     /*open to append if kin file in*/
  myenableitem(fileMenu, importItem, LimportOK && !Ltestmode);
     /*remote data import*/
  myenableitem(fileMenu, writekinItem, Lkinfile && !Ltestmode);
/*myenableitem(fileMenu, writeItem, Lkinfile);*/ /*cannot disable sub_menu*/
  myenableitem(fileMenu, writePostScriptItem, Lkinfile && !Ltestmode);
  myenableitem(fileMenu, quitItem, !Ltestmode);/*test mode only go Next...*/
              /*only write if kin file in*/
  myenableitem(writeMenu, writedrawnewItem, Lkinfile&&Ldrawstuff && !Ltestmode);
  myenableitem(writeMenu, writerottedItem, Lkinfile && nbondrot && !Ltestmode);
  myenableitem(writeMenu, writeviewItem, Lkinfile && !Ltestmode);
  myenableitem(writeMenu, writefocusItem, Lkinfile && !Ltestmode);
  myenableitem(writeMenu, writeQAItem, Lquesanswers && Lkinfile && !Ltestmode);
  myenableitem(writeMenu, writeR3DItem, Lkinfile && !Ltestmode);
  myenableitem(writeMenu, writeptIDItem, Lkinfile && !Ltestmode);
  myenableitem(writeMenu, writepaletteItem, Lkinfile && !Ltestmode);
#ifdef UNIX_X11
    myenableitem(writeMenu, writemeansigmaItem, Lkinfile && !Ltestmode);
#endif

  myenableitem(editMenu, drawlineItem, (!LbondrotHplot && !LbondrotVplot) );
  myenableitem(editMenu, drawsetupItem,(!LbondrotHplot && !LbondrotVplot) );
  myenableitem(editMenu, editviewItem, 1);
  myenableitem(editMenu, colorshowItem, 1);
  myenableitem(editMenu, pruneItem, 1);
  myenableitem(editMenu, superpickItem, 1);
  myenableitem(editMenu, pickshowItem, 1); /*Edit Object Properties*/
  myenableitem(editMenu, movepointsItem, 1); /*Redefine Point Groupings*/
  myenableitem(editMenu, meansigmasItem, 1); /*Calc Mean & Sigmas  061126*/
  myenableitem(editMenu, enableTEItem, 1);
#ifndef UNIX_X11
    myenableitem(editMenu, cutItem,   Ltexteditable);
    myenableitem(editMenu, copyItem,  Ltexteditable);
    myenableitem(editMenu, pasteItem, Ltexteditable);
#endif

    myenableitem(editMenu, tablecutItem,   Ltablepresent);
    myenableitem(editMenu, tablerestoreItem,   Ltablepresent);
    myenableitem(editMenu, tablepreinsertItem,  Ltablepresent);
    myenableitem(editMenu, tablepostinsertItem, Ltablepresent);
    myenableitem(editMenu, tablerebuildItem, Ltablepresent);
    myenableitem(editMenu, tablecolorItem, Ltablepresent);

  
  myenableitem(kineMenu, nextItem, LnextOK&&(!Ltestmode));
  myenableitem(kineMenu, chooseItem, Lkinfile&&(!Ltestmode));
  myenableitem(kineMenu, scoreNnextItem, Lkinfile&&Ltestmode);
  myenableitem(kineMenu, scoreNstayItem, Lkinfile&&Lpracticemode);
   /*Ltestmode and Lpracticemode are mutually exclusive*/

  
  myenableitem(optiMenu, multiwidthItem, 1);
  myenableitem(optiMenu, widthItem, 1);
  myenableitem(optiMenu, thinItem, 1);
  myenableitem(optiMenu, depth5Item, 1);
  myenableitem(optiMenu, depth3Item, 1);
  myenableitem(optiMenu, perspecItem, 1);

  myenableitem(optiMenu, stereoItem, !Lcompareon);
  myenableitem(optiMenu, compareItem, Lcompare&&!Lstereo);  
  myenableitem(optiMenu, sterangItem, 1);

  myenableitem(optiMenu, listcolorItem, 1);
  myenableitem(optiMenu, lensItem, LLensflagset);
  myenableitem(optiMenu, detailItem, 1);
  
  myenableitem(optiMenu, whtbkgItem, LCscrn);
  myenableitem(optiMenu, B_WItem, LCscrn);
  myenableitem(optiMenu, grayItem, LCscrn); /*040616*/
  
  myenableitem(optiMenu, GFontItem, 1);
  myenableitem(optiMenu, strokedfontItem, 1);
  
  myenableitem(optiMenu, flatItem, 1);     /*flatland, 2-D kinemages*/
  myenableitem(optiMenu, horizItem, 1);
  myenableitem(optiMenu, plotonlyItem, 1); /*kalide*/
#ifdef UNIX_X11
  myenableitem(optiMenu, fontcursorItem, 1); /*cursor choice for Linux*/
#endif

  myenableitem(aspectMenu, aspect0Item, Laspect[1]);/*off enabled aspects>=1*/
  myenableitem(aspectMenu, aspect1Item, Laspect[1]);
  myenableitem(aspectMenu, aspect2Item, Laspect[2]);
  myenableitem(aspectMenu, aspect3Item, Laspect[3]);
  myenableitem(aspectMenu, aspect4Item, Laspect[4]);
  myenableitem(aspectMenu, aspect5Item, Laspect[5]);
  myenableitem(aspectMenu, aspect6Item, Laspect[6]);
  myenableitem(aspectMenu, aspect7Item, Laspect[7]);
  myenableitem(aspectMenu, aspect8Item, Laspect[8]);
  myenableitem(aspectMenu, aspect9Item, Laspect[9]);
  myenableitem(aspectMenu, aspect10Item, Laspect[10]);
  myenableitem(aspectMenu, aspect11Item, Laspect[11]);
  myenableitem(aspectMenu, aspect12Item, Laspect[12]);
  myenableitem(aspectMenu, aspect13Item, Laspect[13]);
  myenableitem(aspectMenu, aspect14Item, Laspect[14]);
  myenableitem(aspectMenu, aspect15Item, Laspect[15]);
  myenableitem(aspectMenu, aspect16Item, Laspect[16]);
  myenableitem(aspectMenu, aspectscrnItem, Laspect[1]);/*enabled if aspects>=1*/
  
#ifdef MACINTOSH   /*981004*/
#define EXTRAVIEW extraviewMenu
#else
#define EXTRAVIEW viewMenu /*UNIX, PC routines do not use Menu Handle*/
#endif 
  myenableitem(viewMenu, undopickctrItem, 1);
  myenableitem(viewMenu, reset1Item, Lkinfile); /*only valid if kin file in*/
  myenableitem(viewMenu, reset2Item, Lview[2]);
  myenableitem(viewMenu, reset3Item, Lview[3]);
  myenableitem(viewMenu, reset4Item, Lview[4]);
  myenableitem(viewMenu, reset5Item, Lview[5]);
  myenableitem(viewMenu, reset6Item, Lview[6]);
  myenableitem(viewMenu, reset7Item, Lview[7]);
  myenableitem(viewMenu, reset8Item, Lview[8]);
  myenableitem(viewMenu, reset9Item, Lview[9]);
  myenableitem(viewMenu, reset10Item, Lview[10]);
  myenableitem(viewMenu, reset11Item, Lview[11]); 
  myenableitem(viewMenu, reset12Item, Lview[12]);
  myenableitem(viewMenu, reset13Item, Lview[13]);
  myenableitem(viewMenu, reset14Item, Lview[14]);
  myenableitem(viewMenu, reset15Item, Lview[15]);
  myenableitem(viewMenu, reset16Item, Lview[16]);
  myenableitem(viewMenu, reset17Item, Lview[17]);
  myenableitem(viewMenu, reset18Item, Lview[18]);
  myenableitem(viewMenu, reset19Item, Lview[19]);
  myenableitem(viewMenu, reset20Item, Lview[20]);
  myenableitem(viewMenu, reset21Item, Lview[21]); 
  myenableitem(viewMenu, reset22Item, Lview[22]);
  myenableitem(viewMenu, reset23Item, Lview[23]);
  myenableitem(viewMenu, reset24Item, Lview[24]);
  myenableitem(viewMenu, reset25Item, Lview[25]);
  myenableitem(viewMenu, reset26Item, Lview[26]);
  myenableitem(viewMenu, reset27Item, Lview[27]);
  myenableitem(viewMenu, reset28Item, Lview[28]);
  myenableitem(viewMenu, reset29Item, Lview[29]);
  
  myenableitem(EXTRAVIEW, reset30Item, Lview[30]);
  myenableitem(EXTRAVIEW, reset31Item, Lview[31]); 
  myenableitem(EXTRAVIEW, reset32Item, Lview[32]);
  myenableitem(EXTRAVIEW, reset33Item, Lview[33]);
  myenableitem(EXTRAVIEW, reset34Item, Lview[34]);
  myenableitem(EXTRAVIEW, reset35Item, Lview[35]);
  myenableitem(EXTRAVIEW, reset36Item, Lview[36]);
  myenableitem(EXTRAVIEW, reset37Item, Lview[37]);
  myenableitem(EXTRAVIEW, reset38Item, Lview[38]);
  myenableitem(EXTRAVIEW, reset39Item, Lview[39]);
  myenableitem(EXTRAVIEW, reset40Item, Lview[40]);
  myenableitem(EXTRAVIEW, reset41Item, Lview[41]); 
  myenableitem(EXTRAVIEW, reset42Item, Lview[42]);
  myenableitem(EXTRAVIEW, reset43Item, Lview[43]);
  myenableitem(EXTRAVIEW, reset44Item, Lview[44]);
  myenableitem(EXTRAVIEW, reset45Item, Lview[45]);
  myenableitem(EXTRAVIEW, reset46Item, Lview[46]);
  myenableitem(EXTRAVIEW, reset47Item, Lview[47]);
  myenableitem(EXTRAVIEW, reset48Item, Lview[48]);
  myenableitem(EXTRAVIEW, reset49Item, Lview[49]);
  myenableitem(EXTRAVIEW, reset50Item, Lview[50]);
  myenableitem(EXTRAVIEW, reset51Item, Lview[51]); 
  myenableitem(EXTRAVIEW, reset52Item, Lview[52]);
  myenableitem(EXTRAVIEW, reset53Item, Lview[53]);
  myenableitem(EXTRAVIEW, reset54Item, Lview[54]);
  myenableitem(EXTRAVIEW, reset55Item, Lview[55]);
  myenableitem(EXTRAVIEW, reset56Item, Lview[56]);
  myenableitem(EXTRAVIEW, reset57Item, Lview[57]);
  myenableitem(EXTRAVIEW, reset58Item, Lview[58]);
  myenableitem(EXTRAVIEW, reset59Item, Lview[59]);
  myenableitem(EXTRAVIEW, reset60Item, Lview[60]);
  

 /* myenableitem(viewMenu, extraviewItem, 1);*/ /*sub_menu, stays enabled*/
  myenableitem(viewMenu, resetRItem,  Lview[61]);
  myenableitem(viewMenu, readersviewItem, 1);
  myenableitem(viewMenu, NDaxesPickedCtrItem, LNdimensions); /*070315*/
  
  /*otherMenu == Tools*/
  myenableitem(otherMenu, searchItem, Lkinfile);/*only "FIND" if kin file in*/
  myenableitem(otherMenu, findagainItem, Lkinfile);/*only "FIND" if kin file */
  
  myenableitem(otherMenu, measureItem, 1);
  myenableitem(otherMenu, xyzpointItem, 1);
#ifdef UNIX_X11
  myenableitem(otherMenu, dimensionItem, LNdimensions);/*060622 was L7d 060121*/
  myenableitem(otherMenu, SELECTINGlistItem, LSELECTINGlist); /*060121*/
#endif
  myenableitem(otherMenu, gnomonItem, 1);
  myenableitem(otherMenu, rateItem, 1);
  myenableitem(otherMenu, countItem, 1);
  myenableitem(otherMenu, dockingscopeItem, 1); /*000814*/
  myenableitem(otherMenu, zlimitscopeItem, 1); /*011201*/
  myenableitem(otherMenu, finestepItem, 1); /*011201*/
  

  myenableitem(otherMenu, remoteupdateItem, 1);/*invoke remote program*/

  /*keyempMenu == Empowerments*/
  myenableitem(keyempMenu, hidemeasuresItem, 1);
  myenableitem(keyempMenu, brotangleItem, !Ldockingscope);/*050407 dock NOT OK*/
  myenableitem(keyempMenu, rot90plusItem, 1);
  myenableitem(keyempMenu, rot90minusItem, 1);
  myenableitem(keyempMenu, rot90xplusItem, 1);
  myenableitem(keyempMenu, rot90xminusItem, 1);
  myenableitem(keyempMenu, rot90zplusItem, 1);
  myenableitem(keyempMenu, rot90zminusItem, 1);
 
  /*971126*/
  /*myenableitem(otherMenu, kludgeItem, 1);*/ /*sub_menu, stays enabled*/
  /*myenableitem(otherMenu, helpItem, 1);*/ /*sub_menu, stays enabled*/
  /*971126*/

  myenableitem(kludgeMenu, clipmiddleItem, 1); /*051214*/
  myenableitem(kludgeMenu, formattestItem, 0); /*990407 always disable*/
  myenableitem(kludgeMenu, testItem, 1);
  myenableitem(kludgeMenu, pickmarkerItem, 1); /*000413*/
  myenableitem(kludgeMenu, singlemarkerItem, 1); /*move here 060206*/
  myenableitem(kludgeMenu, visibleoutItem, 1); /*001118*/
  myenableitem(kludgeMenu, rescoringItem, Lrescoring); /*020830*/
#ifdef UNIX_X11
  myenableitem(kludgeMenu, backgroundItem, 1); /*010408*/
  myenableitem(kludgeMenu, buttonwindowItem, 1); /*041108*/
  myenableitem(kludgeMenu, allbrsliderItem, 1); /*070617*/
  myenableitem(kludgeMenu, foocavityItem, 1); /*030330*/
#endif
  myenableitem(kludgeMenu, mickeymouseItem, 1); /*010609*/
  myenableitem(kludgeMenu, pdbformatoutItem, 1); /*040208*/
  
  /*Macintosh: windMenu may be added to the end of the Macintosh's Help Menu:*/
  /* then all Item #s need to be increased by the # of items in Mac Help*/
  /* and windMenu set == mac's Help Menu*/
  /*NOTE 991114 use of dummylasthelpItem in MACMENU/HandleMenu():case windID:*/
#ifdef MACINTOSH /*obselete artifact of adding MAGE-HELP to Mac Help menu*/
/*020904 MACINTOSH defines NHelpItems==0, leave until change proved robust*/
  myenableitem(windMenu, showtextItem + NHelpItems, 1);
  myenableitem(windMenu, showcaptionItem + NHelpItems, 1);
  myenableitem(windMenu, showgraphicsItem + NHelpItems, 1);
  myenableitem(windMenu, showtableItem + NHelpItems, Ltablepresent);
  myenableitem(windMenu, showbondrotsItem + NHelpItems, nbondrot);
  myenableitem(windMenu, aboutItem + NHelpItems, 1);/*MAGE info */
  /*myenableitem(windMenu, writeaboutItem + NHelpItems, 1);020904 About Write*/
  /*myenableitem(windMenu, iohelpItem + NHelpItems, 1);*//*020904-->IO submenu*/
  myenableitem(windMenu, editaboutItem + NHelpItems, 1);  /*About Edits*/
  myenableitem(windMenu, optionsaboutItem + NHelpItems, 1);
  myenableitem(windMenu, otheraboutItem + NHelpItems, 1);
  myenableitem(windMenu, kludgesaboutItem + NHelpItems, 1);
  myenableitem(windMenu, specialaboutItem + NHelpItems, 1);
  myenableitem(windMenu, secretItem + NHelpItems, 1);
  myenableitem(windMenu, mousehelpItem + NHelpItems, 1);
  myenableitem(windMenu, dockinghelpItem + NHelpItems, 1);
  myenableitem(windMenu, resizehelpItem + NHelpItems, 1);
  myenableitem(windMenu, tablehelpItem + NHelpItems, 1);
  myenableitem(windMenu, bondshelpItem + NHelpItems, 1);
  myenableitem(windMenu, test3DhelpItem + NHelpItems, 1);
  myenableitem(windMenu, NDIMhelpItem + NHelpItems, 1);
  myenableitem(windMenu, makehelptextItem + NHelpItems, 1);
  myenableitem(windMenu, makekinemageItem + NHelpItems, 1);
#else
  myenableitem(windMenu, showtextItem, 1);
  myenableitem(windMenu, showcaptionItem, 1);
  myenableitem(windMenu, showgraphicsItem, 1);
#ifdef UNIX_X11
  myenableitem(windMenu, showbuttwinItem, Lbuttonseparate);/*060613*/
#endif
  myenableitem(windMenu, showtableItem, Ltablepresent);
  myenableitem(windMenu, showbondrotsItem, nbondrot);
  myenableitem(windMenu, aboutItem, 1);/*MAGE info */
  /*myenableitem(windMenu, writeaboutItem, 1);020904 About Write*/
  /*myenableitem(windMenu, iohelpItem, 1);*/ /*020904 --> IO submenu*/
  myenableitem(windMenu, editaboutItem, 1);  /*About Edits*/
  myenableitem(windMenu, optionsaboutItem, 1);
  myenableitem(windMenu, otheraboutItem, 1);
  myenableitem(windMenu, kludgesaboutItem, 1);
  myenableitem(windMenu, specialaboutItem, 1);
  myenableitem(windMenu, secretItem, 1);
  myenableitem(windMenu, mousehelpItem, 1);
  myenableitem(windMenu, dockinghelpItem, 1);
  myenableitem(windMenu, resizehelpItem, 1);
  myenableitem(windMenu, test3DhelpItem, 1);
  myenableitem(windMenu, makehelptextItem, 1);
  myenableitem(windMenu, makekinemageItem, 1);
#endif

  myenableitem(iohelpMenu, writeaboutItem, 1); /*020904*/
  myenableitem(iohelpMenu, screenoutItem, 1);  /*020904*/

}
/*___AdjustMenus()__________________________________________________________*/

/****resetmenuchecks()*******************************************************/
void    resetmenuchecks()  /*971010*/
{    
    /*only works on ToggleButtons in the UNIX system 980928 */
    mycheckitem(editMenu, drawlineItem, Ldrawstuff); /*____MENU*/
    mycheckitem(editMenu, superpickItem, Lsuperpick);
    mycheckitem(editMenu, colorshowItem, Lpickcolor);
    mycheckitem(editMenu, pickshowItem, Lpickshow);
    mycheckitem(editMenu, movepointsItem, Lmovepoints); /*061126*/
    mycheckitem(editMenu, meansigmasItem, Lmeansigmas); /*061126*/
    mycheckitem(editMenu, pruneItem, Lprune);

    mycheckitem(editMenu, enableTEItem, Ltexteditable);

    mycheckitem(optiMenu, multiwidthItem, Lmultiwidth);
    mycheckitem(optiMenu, widthItem, Lonewidth);
    mycheckitem(optiMenu, thinItem, Lthin);
    mycheckitem(optiMenu, depth5Item, L5depths);
    mycheckitem(optiMenu, depth3Item, L3depths);
    mycheckitem(optiMenu, perspecItem, Lperspec);
    mycheckitem(optiMenu, B_WItem, !Lcolor);
    mycheckitem(optiMenu, whtbkgItem, Lwhitebkg);
    mycheckitem(optiMenu, grayItem, Lgrayscale);
    mycheckitem(optiMenu, stereoItem, Lstereo);

    mycheckitem(optiMenu, compareItem, Lcompareon);
    mycheckitem(optiMenu, listcolorItem, Listcolordominant);
    mycheckitem(optiMenu, lensItem, LLenson);
    mycheckitem(optiMenu, flatItem, Lflat);
    mycheckitem(optiMenu, horizItem, Lflat);
    mycheckitem(optiMenu, plotonlyItem, Lplotonly);
#ifdef UNIX_X11
    mycheckitem(optiMenu, fontcursorItem, Lfontcursor);
#endif

 /*Tools*/    
    mycheckitem(otherMenu, measureItem, Lmeasures);
    mycheckitem(otherMenu, xyzpointItem, Lxyzpoint);
    mycheckitem(otherMenu, gnomonItem, Lgnomon);
    mycheckitem(otherMenu, rateItem, Lrate);
    mycheckitem(otherMenu, remoteupdateItem, Lupdate);
    mycheckitem(otherMenu, dockingscopeItem, Ldockingscope);
    mycheckitem(otherMenu, zlimitscopeItem, Lzlimitscope);

    mycheckitem(kludgeMenu, clipmiddleItem, Lclipmiddle);
    mycheckitem(kludgeMenu, formattestItem, Lformattest);
    mycheckitem(kludgeMenu, testItem, Ltest);
    mycheckitem(kludgeMenu, pickmarkerItem, 0);
    mycheckitem(kludgeMenu, singlemarkerItem, Lsinglemarker);
    mycheckitem(kludgeMenu, visibleoutItem, Lvisibleout);
    mycheckitem(kludgeMenu, mickeymouseItem, Lmickeymouse);
    mycheckitem(kludgeMenu, pdbformatoutItem, LPDBformat);
#ifdef UNIX_X11
    mycheckitem(kludgeMenu, buttonwindowItem, Lbuttonseparate);
    mycheckitem(kludgeMenu, allbrsliderItem, Lallbondrotsliders); /*070617*/
#endif
    AdjustMenus();  /*MAGEMENU*/
}        
/*___resetmenuchecks()______________________________________________________*/

/****setdrawstuff()***********************************************************/
void    setdrawstuff(int mode) /*020830 mode==0 default ON/OFF*/
{
    Ldrawstuff=!Ldrawstuff;
    
    if(!Ldrawstuff) /*turn off draw new*/
    {
      if(LbondrotHplot || LbondrotVplot) /*030127*/
      {
         sprintf(alertstr,
         "DrawNew invoked to plot 3D to 2D, cannot be turned off gracefully."
         );
         alertstr2[0]='\0';alertstr3[0]='\0';
         DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
         Ldrawstuff = 1;
      }
      else if(numberline>0)
      {
          Lclosedrawstuff = 1;
          changecheckrouter(); /*MAGEINIT.c 020902*/
      }
      else closedrawstuff(); /*.*/
    }
    else
    {/*a new invocation of drawlines*/
      adddrawstuff();  /*tries to establish drawline storage*/ /*MAGEINPT.c*/
      if(Ldrawstuff)  /* returns as TRUE if can set up drawline */
      {/*can do drawline*/    
         /*Ldrawstuff == 1 generic control for this whole set*/
                               /*draw empowered*/
         Lnewdrawline = 1;   /*starts new counting*/
         numberline = 0;     /*the counter*/
         Lnewlines  = 0;     /* lines */  /*020904*/
         Lnewlabels = 0;     /* labels*/
         Lnewballs  = 0;     /* balls */
         Ldragline  = 0;     /* drag  */
         Lconstructline = 0; /* construction*/
         Lnewlineson  = 0; /*initially on , turn off by control panel button*/
         Lnewlabelson = 0; /*initially off, turn on  by control panel button*/
         Lnewballson = 0;  /*initially off, turn on  by control panel button*/
         Lconstruct4on= 0; /*initially off, turn on  by control panel button*/
         Lconstruct5on= 0; /*initially off, turn on  by control panel button*/
         Lconstruct6on= 0; /*initially off, turn on  by control panel button*/
         Ldraglineon  = 0; /*initially off, turn on  by control panel button*/
         /*Presume default or setup values for */  /*980919*/
         /*shortenline,Ldrawunpickable,Lmonitor,Lmonitoron,LSplitLine re:lines*/

         if(mode&32) {Lfoocavity = 1;} /*030331*/
         if(mode&16) {Ldock3on3  = 1; Ldock3on3on  = 1;} /*egam 030216*/
         if(mode&1) {Lnewlines  = 1; Lnewlineson  = 1;} 
         if(mode&2) {Lnewlabels = 1; Lnewlabelson = 1;}
         if(mode&4) {Lnewballs  = 1; Lnewballson  = 1;}
         if(mode&8 || mode==0)
         {/*enable all drawing functions*/
           Lnewlines  = 1;     /*empowers lines */
           Lnewlabels = 1;     /*empowers labels*/
           Lnewballs = 1;      /*empowers balls*/
           Ldragline = 1;      /*empowers drag*/
           Lconstructline = 1; /*empowers construction*/
           Lnewlineson = 1; /*default start with lines active*/
         }
         mycheckitem(editMenu, drawlineItem, Ldrawstuff);/*___MENU.C*/
         AdjustMenus();  /*MAGEMENU*/
         SetsinGrafWindow(); /*040612 make button boxes now so logic will work*/
         if(Lnewlineson) /*dominant*/
            {turnoffallbutoneboxLogical((long)drawlinebboxptr);} /*MAGEBBOX*/
         else if(Lnewlabelson)
            {turnoffallbutoneboxLogical((long)drawlabelsbboxptr);} /*MAGEBBOX*/
         else if(Lnewballson)
            {turnoffallbutoneboxLogical((long)drawballsbboxptr);} /*MAGEBBOX*/
         redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/

      }/*can do drawline*/ 
    }/*a new invocation of drawlines*/
}
/*___setdrawstuff()__________________________________________________________*/

/****closedrawstuff()*********************************************************/
void    closedrawstuff()
{
   /*during input of a new kinemage, any old drawline stuff is killed*/
   /*reading back in a just written .kip does this BEFORE the output*/
   /*routines have had a chance to call closedrawstuff() which in turn*/
   /*calls killdrawstuff.  So unset Lclosedrawstuff so output routine*/
   /*can avoid calling closedrawstuff(). */
   /*Also no need for closedrawstuff() to do drawgrupptr->on = 0 anyway. */

   Lclosedrawstuff = 0;
   Ldrawstuff = 0;
   Lconstructline = 0;
   Lconstruct4on = 0; /*971122*/
   Lconstruct5on = 0; /*971122*/
   Lconstruct6on = 0; /*140912*/
   Ldragline = 0;
   Ldraglineon = 0;
   Lnewlines = 0;
   Lnewlineson = 0;
   Lnewlabels = 0;
   Lnewlabelson = 0;
   Lnewballs = 0;
   Lnewballson = 0;
   Lmonitor = 0; /*last line length*/
   Lmonitoron = 0;  /*last line length*/
   /*drawgrupptr->on = 0;*/ /* disable display */ /*020211*/
   if(!Lmeasures) 
   {
      if(Lmarkers) maxnxyz = nxyz + NMARKERS;
      else maxnxyz = nxyz;
   }
   else
   {
      maxnxyz = nxyz + NMARKERS + MAXMEASURES;
   }
   killdrawstuff(); /*MAGEINPT*/

   mycheckitem(editMenu, drawlineItem, Ldrawstuff);/*___MENU.C*/
   LneedSetsinGrafWindow = 1; 
   redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___closedrawstuff()________________________________________________________*/

/****setsuperpick()**********************************************************/
void    setsuperpick()   
{
    mycheckitem(editMenu, superpickItem, Lsuperpick=!Lsuperpick);/*___MENU.C*/
}
/*___setsuperpick()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setpickshow()***********************************************************/
void    setpickshow()   
{
    mycheckitem(editMenu, pickshowItem, Lpickshow=!Lpickshow);
             /*MACMENU.C*/ /*MPCMENU.C*/
    if(Lpickshow)
    {    Lpickshowon = 1; turnoffallbutoneboxLogical((long)pickshowbboxptr);}
    else Lpickshowon = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setpickshow()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setmovepoints()*********************************************************/
void    setmovepoints()   
{
    mycheckitem(editMenu, movepointsItem, Lmovepoints=!Lmovepoints);
             /*MACMENU.C*/ /*MPCMENU.C*/
    if(Lmovepoints)
    {  Lmovepointson = 1; turnoffallbutoneboxLogical((long)movepointsbboxptr);}
    else Lmovepointson = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setmovepoints()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setmeansigmas()*********************************************************/
void    setmeansigmas()   
{
    mycheckitem(editMenu, meansigmasItem, Lmeansigmas=!Lmeansigmas);
             /*MACMENU.C*/ /*MPCMENU.C*/
    if(Lmeansigmas)
    {  Lmeansigmason = 1; turnoffallbutoneboxLogical((long)meansigmasbboxptr);}
    else Lmeansigmason = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setmeansigmas()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setcolorshow()***********************************************************/
void    setcolorshow()   
{
    mycheckitem(editMenu, colorshowItem, Lpickcolor=!Lpickcolor);/*___MENU.C*/ 
    if(Lpickcolor)
    {    Lpickcoloron = 1;turnoffallbutoneboxLogical((long)pickcolorbboxptr);}
    else Lpickcoloron = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setcolorshow()__________________________________________________________*/

/****setemphasis()************************************************************/
void    setemphasis()   /*061012 emphasis taken off menu*/
{
    mycheckitem(editMenu, emphasisItem, Lemphasis=!Lemphasis);/*___MENU.C*/ 
    if(Lemphasis)
    {    Lemphasison = 1;turnoffallbutoneboxLogical((long)emphasisbboxptr);}
    else Lemphasison = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setemphasis()___________________________________________________________*/

/****setprune()**************************************************************/
void    setprune()   
{
    mycheckitem(editMenu, pruneItem, Lprune=!Lprune); /*___MENU.C*/
    if(Lprune){Lpunchon = 1;turnoffallbutoneboxLogical((long)punchbboxptr);}
    else       Lpunchon = 0;
    Lpruneon = 0;
    Lsuperpunchon = 0; /*990212*/
    Lundop = 0;
    nundop = 0;
    LneedSetsinGrafWindow = 1; 
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setprune()_____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getusername()************************************************************/
void    getusername(void)
{
    Lcancel = 1; /*not allowed to cancel*/
       /* or not give at least one alphabetic character for user name*/
    DousernameDialog();  /*MAGEDLOG.C*/ /*020816*/
    /*return of Lcancel==1 will have calling routine invalidate username*/
    /* so force a repeat of user name dialog*/
    /*see MAGEINIT/magetimercheck()*/
    /*UNIX must return to master waiting loop, so cannot use a while(Lcancel)*/
}
/*___getusername()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getkinemagechoice()*****************************************************/
void    getkinemagechoice(void)
{
    DokineDialog();  /*MAGEDLOG.C*/ /*971128*/
}
/*___getkinemagechoice()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setmultiwidth()********************************************************/
void    setmultiwidth() /*971130*/ /*981114*/
{
    mycheckitem(optiMenu, multiwidthItem, Lmultiwidth=1);/*___MENU.C*/
    Lonewidth = 0;
    Lthin = 0;
    mycheckitem(optiMenu, widthItem, Lonewidth);
    mycheckitem(optiMenu, thinItem, Lthin);
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setmultiwidth()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****setwidthtoggle()********************************************************/
void    setwidthtoggle() /*971130*/ /*981114  not a toggle*/
{
    mycheckitem(optiMenu, widthItem, Lonewidth=1);/*___MENU.C*/
    Lmultiwidth = 0;
    Lthin = 0;
    mycheckitem(optiMenu, multiwidthItem, Lmultiwidth);
    mycheckitem(optiMenu, thinItem, Lthin);
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setwidthtoggle()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setthinline()***********************************************************/
void    setthinline() /*971130*/  /*981114*/
{
    mycheckitem(optiMenu, thinItem, Lthin=1); /*___MENU.C*/
    Lmultiwidth = 0;
    Lonewidth = 0;
    mycheckitem(optiMenu, multiwidthItem, Lmultiwidth);
    mycheckitem(optiMenu, widthItem, Lonewidth);
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setthinline()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****forcethinline()*********************************************************/
void    forcethinline()
{
    mycheckitem(optiMenu, thinItem, Lthin=1); /*___MENU.C*/
    if(Lmenu) redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___forcethinline()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setNdepths()*************************************************************/
void    setNdepths() /*010927*/
{
    mycheckitem(optiMenu, depth5Item, L5depths); /*___MENU.C*/
    mycheckitem(optiMenu, depth3Item, L3depths); /*___MENU.C*/
    redrawvec();       /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setNdepths()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setperspec()************************************************************/
void    setperspec()
{
    mycheckitem(optiMenu, perspecItem, Lperspec=!Lperspec); /*___MENU.C*/
    if(Lmenu) redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setperspec()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****forceperspective()******************************************************/
void    forceperspective()   /*common name: MAC & PC */
{
    mycheckitem(optiMenu, perspecItem, Lperspec=1); /*___MENU.C*/
    if(Lmenu) redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___forceperspective()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setwhtbkg()*************************************************************/
void    setwhtbkg()
{
#ifdef MACINTOSH    
        mycheckitem(optiMenu, whtbkgItem, Lwhitebkg=!Lwhitebkg);/*MACMENU.C*/
#else
        mycheckitem(0, whtbkgItem, Lwhitebkg=!Lwhitebkg);/*___MENU.C*/
        /*PC just fakes a B&W display 140912, so whtbkg and B&W related */
        /* if UNIX can do color, it just fakes a B&W screen*/
        if(!Lwhitebkg) Lcolor=1;
        mycheckitem(0, B_WItem, !Lcolor);
#endif
    redrawvec();    /*___DRAW.C*/
}
/*___setwhtbkg()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/


/****setcurrentaspect()******************************************************/
void    setcurrentaspect(int ith)
{
   int j=0,ionoff=0;
   currentaspect = ith;
   for(j=0; j<=MAXPOINTASPECTS; j++)
   {
      if(j==ith) {ionoff = 1;}
      else       {ionoff = 0;}
      setaspect(j,ionoff);
   }
   if(!Linputactive){redrawvec();} /*040605*/   /*___DRAW.C*/
   if(Ltableready) {redrawtable();}
}
/*___setcurrentaspect()_____________________________________________________*/

/****adjustcurrentaspect()***************************************************/
void adjustcurrentaspect(int iup)
{/*cycles forward or backwards through the aspects*/
   /*NOTE: currentaspect set to param in subroutine setcurrentaspect() */
   if(naspectsthiskinemage > 0)
   {
      if(iup > 0)
      {
         /*if(currentaspect < naspectsthiskinemage) */
         if(currentaspect < maxaspectsperpoint) /*030605*/
         {
            setcurrentaspect(currentaspect+1);
         }
         else 
         {
            setcurrentaspect(1);
         }
      }
      else
      {
         if(currentaspect > 1) 
         {
            setcurrentaspect(currentaspect-1);
         }
         else 
         {
            setcurrentaspect(maxaspectsperpoint); /*030605*/
            /*setcurrentaspect(naspectsthiskinemage);*/
         }
      }
      redrawvec();    /*___DRAW.C*/
      if(Ltableready) {redrawtable();}
   }
}
/*___adjustcurrentaspect()__________________________________________________*/

/****setaspect()*************************************************************/
void    setaspect(int ith, int ionoff)
{
#ifdef UNIX_X11
   Widget item;
#else
   int item;
#endif

   if     (ith == 0) item = aspect0Item;
   else if(ith == 1) item = aspect1Item;
   else if(ith == 2) item = aspect2Item;
   else if(ith == 3) item = aspect3Item;
   else if(ith == 4) item = aspect4Item;
   else if(ith == 5) item = aspect5Item;
   else if(ith == 6) item = aspect6Item;
   else if(ith == 7) item = aspect7Item;
   else if(ith == 8) item = aspect8Item;
   else if(ith == 9) item = aspect9Item;
   else if(ith == 10) item = aspect10Item;
   else if(ith == 11) item = aspect11Item;
   else if(ith == 12) item = aspect12Item;
   else if(ith == 13) item = aspect13Item;
   else if(ith == 14) item = aspect14Item;
   else if(ith == 15) item = aspect15Item;
   else if(ith == 16) item = aspect16Item;
#ifdef MACINTOSH    
        mycheckitem(aspectMenu, item, ionoff);/*MACMENU.C*/
#else
        mycheckitem(0, item, ionoff);/*___MENU.C*/
#endif
    if(!Linputactive) {redrawvec();} /*040605*/    /*___DRAW.C*/
}
/*___setaspect()____________________________________________________________*/


/****setaspectidentifier()****************************************************/
void setaspectidentifier(int i, char* temps)
{
                if(i==0)  mymodifymenuitem(aspectMenu, aspect0Item, temps);
                if(i==1)  mymodifymenuitem(aspectMenu, aspect1Item, temps);
                if(i==2)  mymodifymenuitem(aspectMenu, aspect2Item, temps);
                if(i==3)  mymodifymenuitem(aspectMenu, aspect3Item, temps);
                if(i==4)  mymodifymenuitem(aspectMenu, aspect4Item, temps);
                if(i==5)  mymodifymenuitem(aspectMenu, aspect5Item, temps);
                if(i==6)  mymodifymenuitem(aspectMenu, aspect6Item, temps);
                if(i==7)  mymodifymenuitem(aspectMenu, aspect7Item, temps);
                if(i==8)  mymodifymenuitem(aspectMenu, aspect8Item, temps);
                if(i==9)  mymodifymenuitem(aspectMenu, aspect9Item, temps);
                if(i==10) mymodifymenuitem(aspectMenu, aspect10Item, temps);
                if(i==11) mymodifymenuitem(aspectMenu, aspect11Item, temps);
                if(i==12) mymodifymenuitem(aspectMenu, aspect12Item, temps);
                if(i==13) mymodifymenuitem(aspectMenu, aspect13Item, temps);
                if(i==14) mymodifymenuitem(aspectMenu, aspect14Item, temps);
                if(i==15) mymodifymenuitem(aspectMenu, aspect15Item, temps);
                if(i==16) mymodifymenuitem(aspectMenu, aspect16Item, temps);
}
/*___setaspectidentifier()___________________________________________________*/

/****setaspectonscreen()******************************************************/
void setaspectonscreen(char* temps, int ionoff)
{
   mymodifymenuitem(aspectMenu, aspectscrnItem, temps);
#ifdef MACINTOSH    
        mycheckitem(aspectMenu, aspectscrnItem, ionoff);/*MACMENU.C*/
#else
        mycheckitem(0, aspectscrnItem, ionoff);/*___MENU.C*/
#endif
   if(!Linputactive) {redrawvec();} /*040605*/    /*___DRAW.C*/
}
/*___setaspectonscreen()_____________________________________________________*/

/****setviewidentifier()******************************************************/
void setviewidentifier(int i, char* temps)
{

#ifdef MACINTOSH   /*981004*/
#define EXTRAVIEW extraviewMenu
#else
#define EXTRAVIEW viewMenu /*UNIX, PC routines do not use Menu Handle*/
#endif 
                if(i==1)  mymodifymenuitem(viewMenu, reset1Item, temps);
                if(i==2)  mymodifymenuitem(viewMenu, reset2Item, temps);
                if(i==3)  mymodifymenuitem(viewMenu, reset3Item, temps);
                if(i==4)  mymodifymenuitem(viewMenu, reset4Item, temps);
                if(i==5)  mymodifymenuitem(viewMenu, reset5Item, temps);
                if(i==6)  mymodifymenuitem(viewMenu, reset6Item, temps);
                if(i==7)  mymodifymenuitem(viewMenu, reset7Item, temps);
                if(i==8)  mymodifymenuitem(viewMenu, reset8Item, temps);
                if(i==9)  mymodifymenuitem(viewMenu, reset9Item, temps);
                if(i==10) mymodifymenuitem(viewMenu, reset10Item, temps);
                if(i==11) mymodifymenuitem(viewMenu, reset11Item, temps);
                if(i==12) mymodifymenuitem(viewMenu, reset12Item, temps);
                if(i==13) mymodifymenuitem(viewMenu, reset13Item, temps);
                if(i==14) mymodifymenuitem(viewMenu, reset14Item, temps);
                if(i==15) mymodifymenuitem(viewMenu, reset15Item, temps);
                if(i==16) mymodifymenuitem(viewMenu, reset16Item, temps);
                if(i==17) mymodifymenuitem(viewMenu, reset17Item, temps);
                if(i==18) mymodifymenuitem(viewMenu, reset18Item, temps);
                if(i==19) mymodifymenuitem(viewMenu, reset19Item, temps);
                if(i==20) mymodifymenuitem(viewMenu, reset20Item, temps);
                if(i==21) mymodifymenuitem(viewMenu, reset21Item, temps);
                if(i==22) mymodifymenuitem(viewMenu, reset22Item, temps);
                if(i==23) mymodifymenuitem(viewMenu, reset23Item, temps);
                if(i==24) mymodifymenuitem(viewMenu, reset24Item, temps);
                if(i==25) mymodifymenuitem(viewMenu, reset25Item, temps);
                if(i==26) mymodifymenuitem(viewMenu, reset26Item, temps);
                if(i==27) mymodifymenuitem(viewMenu, reset27Item, temps);
                if(i==28) mymodifymenuitem(viewMenu, reset28Item, temps);
                if(i==29) mymodifymenuitem(viewMenu, reset29Item, temps);
                
                if(i==30) mymodifymenuitem(EXTRAVIEW, reset30Item, temps);
                if(i==31) mymodifymenuitem(EXTRAVIEW, reset31Item, temps);
                if(i==32) mymodifymenuitem(EXTRAVIEW, reset32Item, temps);
                if(i==33) mymodifymenuitem(EXTRAVIEW, reset33Item, temps);
                if(i==34) mymodifymenuitem(EXTRAVIEW, reset34Item, temps);
                if(i==35) mymodifymenuitem(EXTRAVIEW, reset35Item, temps);
                if(i==36) mymodifymenuitem(EXTRAVIEW, reset36Item, temps);
                if(i==37) mymodifymenuitem(EXTRAVIEW, reset37Item, temps);
                if(i==38) mymodifymenuitem(EXTRAVIEW, reset38Item, temps);
                if(i==39) mymodifymenuitem(EXTRAVIEW, reset39Item, temps);
                if(i==40) mymodifymenuitem(EXTRAVIEW, reset40Item, temps);
                if(i==41) mymodifymenuitem(EXTRAVIEW, reset41Item, temps);
                if(i==42) mymodifymenuitem(EXTRAVIEW, reset42Item, temps);
                if(i==43) mymodifymenuitem(EXTRAVIEW, reset43Item, temps);
                if(i==44) mymodifymenuitem(EXTRAVIEW, reset44Item, temps);
                if(i==45) mymodifymenuitem(EXTRAVIEW, reset45Item, temps);
                if(i==46) mymodifymenuitem(EXTRAVIEW, reset46Item, temps);
                if(i==47) mymodifymenuitem(EXTRAVIEW, reset47Item, temps);
                if(i==48) mymodifymenuitem(EXTRAVIEW, reset48Item, temps);
                if(i==49) mymodifymenuitem(EXTRAVIEW, reset49Item, temps);
                if(i==50) mymodifymenuitem(EXTRAVIEW, reset50Item, temps);
                if(i==51) mymodifymenuitem(EXTRAVIEW, reset51Item, temps);
                if(i==52) mymodifymenuitem(EXTRAVIEW, reset52Item, temps);
                if(i==53) mymodifymenuitem(EXTRAVIEW, reset53Item, temps);
                if(i==54) mymodifymenuitem(EXTRAVIEW, reset54Item, temps);
                if(i==55) mymodifymenuitem(EXTRAVIEW, reset55Item, temps);
                if(i==56) mymodifymenuitem(EXTRAVIEW, reset56Item, temps);
                if(i==57) mymodifymenuitem(EXTRAVIEW, reset57Item, temps);
                if(i==58) mymodifymenuitem(EXTRAVIEW, reset58Item, temps);
                if(i==59) mymodifymenuitem(EXTRAVIEW, reset59Item, temps);
                if(i==60) mymodifymenuitem(EXTRAVIEW, reset60Item, temps);
                
                if(i==61) mymodifymenuitem(viewMenu, resetRItem, temps);
   /*remember that moview # in MAGESETS/getgroup() has maximum view limit*/
}
/*___setviewidentifier()_____________________________________________________*/


#ifdef SGI_GL
#include <gl/gl.h>
#include <gl/device.h>
#include <gl/get.h>
#endif

/****adjuststereo()**********************************************************/
void    adjuststereo()
{
  static int     old_displaydevice = 0; /*140912 was old_, now_, monitor*/
#ifdef SGI_GL                     /*970712*/
  int            now_displaydevice;
#endif
   mycheckitem(optiMenu, stereoItem, Lstereo); /*___MENU.C*/
   myenableitem(optiMenu, compareItem, Lcompare&&!Lstereo);
#ifdef OLDCODE
   if(Lstereo)
      myenableitem(otherMenu,compareItem, 0);/*disable compare*/
   else
      myenableitem(otherMenu,compareItem, 1);/*enable compare*/
#endif
   if(LTBstereo)
   {
#ifdef MACINTOSH
 InvalRect(&grafWindow->portRect); /*m*/ /*970408*/
     /*which invokes SetsinGrafWindow*/
#endif 
#ifdef SGI_GL
      LneedSetsinGrafWindow = 1;
      recreatesliderBars();/*970604 clears space zoom,zslab,ztran*/ 
          /*MUXMBAR.c*/
      if(Lstereo)
      {
         if (getgdesc(GD_STEREO))
         {
            old_displaydevice = getmonitor(); /*140912 is C library call*/
            setmonitor(STR_RECT); /*140912 is C library call*/
         }
      }
      else
      {
         if (getgdesc(GD_STEREO) && old_displaydevice != 0) /*140912*/
         {
            setmonitor(old_displaydevice); /*140912 is C library call*/
         }
      }
#endif
   }
#ifdef SGI_GL
   else
   {/*NOT in LTBstereo*/
      LneedSetsinGrafWindow = 1;
      recreatesliderBars();/*970604 clears space zoom,zslab,ztran*//*MUXMBAR.c*/
      if(Lstereo) /*maybe was switched from TB stereo*/
      {/*see if need to restore displaydevice state 140912 */
         if (getgdesc(GD_STEREO) && old_displaydevice != 0) /*140912*/
         {
            now_displaydevice = getmonitor(); /*140912 is C library call*/
            if(now_displaydevice != old_displaydevice) /*140912*/
               setmonitor(old_displaydevice); /*140912 is C library call*/
         }
      }
   }
#endif
         
   /*resetmenuchecks();*/ /*___MENU.c calls AdjustMenus();*/ /*NOT NEEDED????*/


   redrawvec();         /*___DRAW.C*/ /*MPCDRAW ->()*/
}
/*___adjuststereo()_________________________________________________________*/

/****setstereo()*************************************************************/
void    setstereo()
{
    Lstereo=!Lstereo; 
    adjuststereo(); /*MAGEMENU*/
}
/*___setstereo()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****forcestereo()***********************************************************/
void    forcestereo()
{
    mycheckitem(optiMenu, stereoItem, Lstereo=1); /*___MENU.C*/
    if(Lmenu) redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___forcestereo()__________________________________________________________*/

/****setLens()***************************************************************/
void    setLens()  /*971122*/
{
    LLenson = !LLenson;
    mycheckitem(optiMenu, lensItem, LLenson ); /*___MENU.C*/
    /*if(Lmenu)*/ 
      redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setLens()______________________________________________________________*/

/****potentiateMeasures()******************************************************/
void    potentiateMeasures()  /*971204*/
{
     Lnewmeasures = 1; /* so will start a new sequence next time activated*/
     Lmeasureson = 0;  /*so is deactivated*/
     if (measurelinelistptr != NULL) measurelinelistptr->on = 0; /*981214*/
		/*start with list of points off */
     if (measuredotslistptr != NULL) measuredotslistptr->on = 0; /*981214*/
		/*start with list of average dots off */
     adjustcontrolbuttonboxchecks(); /*for control button boxes*/
     redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___potentiateMeasures()_____________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setcompare()************************************************************/
void    setcompare()
{
    mycheckitem(otherMenu, compareItem, Lcompareon=!Lcompareon); /*___MENU.C*/
    if(Lcompareon)
    {
        myenableitem(0,stereoItem, 0);/*disable stereo*/
        adjustanimate(0);
        adjustanimate(1);
    }
    else
    {
        myenableitem(0,stereoItem, 1); /*enable stereo*/
    }
    LneedSetsinGrafWindow = 1;
    redrawvec();         /*___DRAW.C*/ /*MPCDRAW ->()*/
/*010609 obscure as to whether this should be done here*/
/*    if(Lautorock) adjustrock();*/
/*    else redrawvec();*/

}
/*___setcompare()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setlistcolordominance()**************************************************/
void    setlistcolordominance()
{
    mycheckitem(   optiMenu, listcolorItem
                 , Listcolordominant=!Listcolordominant); /*___MENU.C*/
    redrawvec();
}
/*___setlistcolordominance()_________________________________________________*/

/****setlistradiusdominance()*************************************************/
void    setlistradiusdominance() /*040402 not yet a menu item, just L key*/
{
    /*mycheckitem(   optiMenu, listcolorItem
                 , Listcolordominant=!Listcolordominant);*/ /*___MENU.C*/
    Listradiusdominant=!Listradiusdominant; /*040402*/
    redrawvec();
}
/*___setlistradiusdominance()________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setmeasures()***********************************************************/
void    setmeasures()
{
  if(!Lmeasures)  /*have to make sure there is room to add lines to display*/
  {
    addmeasures(); /*tries to establish measures storage*/
    if(Lmeasures)  /*returns as TRUE if can set up measures*/
    {
        Lnewmeasures = 1;
        Lmeasureson = 1;
        turnoffallbutoneboxLogical((long)measuregrupptr);
    }
  }
  else
  {
        Lmeasures = 0;
        Lmeasureson = 0;
        measuregrupptr->on = 0; /* disable display*/
        if(!Ldrawstuff)
        {
            if(Lmarkers) maxnxyz = nxyz + NMARKERS;
            else         maxnxyz = nxyz;
        }
        killmeasures(); /*MAGEINPT.c*/
  }
  mycheckitem(otherMenu, measureItem, Lmeasures); /*___MENU.C*/
  if(Lmenu)
  {
         LneedSetsinGrafWindow = 1;
         redrawvec();         /*___DRAW.C*/ /*MPCDRAW ->()*/
  }
}
/*___setmeasures()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setxyzpoint()***********************************************************/
void    setxyzpoint()
{
    mycheckitem(otherMenu, xyzpointItem, Lxyzpoint=!Lxyzpoint); /*___MENU.C*/
    if(Lmenu) redrawvec();    /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
}
/*___setxyzpoint()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setgnomon()*************************************************************/
void    setgnomon()
{
  if (Lkinemagecount>0)
  {
    mycheckitem(otherMenu, gnomonItem, Lgnomon=!Lgnomon); /*___MENU.C*/

    if(Lgnomon)
    {
        markergnom1listptr->on = 1; /*gnomon list vectors display on*/
        markergnom2listptr->on = 1; /*gnomon list labels display on*/  /*960529*/
        if(Lmarkers)
        {
            markergrupptr->on = 1;
                /*markers group display on*/
#ifdef MACINTOSH
            SetControlValue(markergrupptr->button, 1);/*980623*/
#endif
#ifdef PCWINDOWS
            SendMessage(markergrupptr->button,BM_SETCHECK,1,0l);
#endif            
#ifdef UNIX_PEX
            updatevisibility();
#endif /*UNIX_PEX*/
#ifdef UNIX_X11
            buttonboxcheck(markergrupptr->button,1); /*981015*/
#endif
        }
    }
    else
    {
        markergnom1listptr->on = 0; /*gnomon list vectors display off*/
        markergnom2listptr->on = 0; /*gnomon list labels display off*/ /*960529*/
    }
    if(Lmenu)
    {
         rescalekinemage();   /*MAGEINPT.C*/
         redrawvec();         /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
    }
  }
}
/*___setgnomon()____________________________________________________________*/

/****setsinglemarker()*******************************************************/
void    setsinglemarker() /*031226*/
{
  if (Lkinemagecount>0)
  {
    mycheckitem(otherMenu, singlemarkerItem, Lsinglemarker=!Lsinglemarker);

    if(Lsinglemarker)
    {
        marker2listptr->on = 0; /*second marker list display off*/ 
    }
    else
    {/*the usual case of showing both first and second markers*/
        marker2listptr->on = 1; /*second marker list display on*/ 
    }
    if(Lmenu)
    {
         rescalekinemage();   /*MAGEINPT.C*/
         redrawvec();         /*___DRAW.C*/ /*MPCDRAW.C -> ()*/
    }
  }
}
/*___setsinglemarker()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setrate()***************************************************************/
void    setrate()
{
    mycheckitem(otherMenu, rateItem, Lrate=!Lrate); /*___MENU.C*/
}
/*___setrate()______________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setclipmiddle()**********************************************************/
void    setclipmiddle() /*051214*/
{
  mycheckitem(kludgeMenu, clipmiddleItem, Lclipmiddle=!Lclipmiddle);
}
/*___setclipmiddle()_________________________________________________________*/

/****setmeasureperpendic()****************************************************/
void    setmeasureperpendic()
{
  mycheckitem(kludgeMenu, measperpItem, Lmeasureperpendics=!Lmeasureperpendics);
}
/*___setmeasureperpendic()___________________________________________________*/

/****setmeasuredsurveillance()************************************************/
void    setmeasuredsurveillance()  /*091212*/
{
  mycheckitem(kludgeMenu, measuredsurveilItem
               , Lmeasuredsurveillance=!Lmeasuredsurveillance);
}
/*___setmeasuredsurveillance()_______________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****settest()***************************************************************/
void    settest()
{
    mycheckitem(kludgeMenu, testItem, Ltest=!Ltest); /*___MENU.C*/
}
/*___settest()______________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setformattest()*********************************************************/
void    setformattest()
{
    mycheckitem(kludgeMenu, formattestItem, Lformattest=!Lformattest);
        /*___MENU.C*/
}
/*___setformattest()________________________________________________________*/

/****setvisibleout()***********************************************************/
void    setvisibleout()  /*001118*/
{
   mycheckitem(kludgeMenu,visibleoutItem,Lvisibleout=!Lvisibleout);/*___MENU.C*/
}
/*___setvisibleout()__________________________________________________________*/

/****setpdbformat()***********************************************************/
void    setpdbformat()  /*040208*/
{
   mycheckitem(kludgeMenu,pdbformatoutItem,LPDBformat=!LPDBformat);/*___MENU.C*/
}
/*___setpdbformat()__________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setstrokedfont()*********************************************************/
void    setstrokedfont(void)
{/*MAGE stroked font, al la R3D*/
    mycheckitem(optiMenu,strokedfontItem,LMAGEstrokedfont=!LMAGEstrokedfont);
    redrawvec();
}
/*___setstrokedfont()________________________________________________________*/

/****setplotonly()***********************************************************/
void    setplotonly()
{
    mycheckitem(optiMenu,plotonlyItem,Lplotonly=!Lplotonly); /*___MENU.C*/
    /*this is the kaleidoscope control*/
}
/*___setplotonly()__________________________________________________________*/

/****setfontcursor()*********************************************************/
void    setfontcursor()
{
    mycheckitem(optiMenu,fontcursorItem,Lfontcursor=!Lfontcursor); /*___MENU.C*/
    /*this controls type of cursor for Linux vs UNIX systems*/
    changecursors(); /*MUXMINIT.c dummied in MAC, PC*/
    redrawvec();
}
/*___setfontcursor()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setflat()***************************************************************/
void    setflat()
{
    mycheckitem(optiMenu,flatItem,Lflat=!Lflat); /*___MENU.C*/
    if(!Lflat)
    {
        Leavingflatland = 1;
    }
    makecursoractive(); /*allows different active cursors*/
    redrawvec();
}
/*___setflat()______________________________________________________________*/

/****sethorizon()*************************************************************/
void    sethorizon()
{
    mycheckitem(optiMenu,horizItem,Lhorizon=!Lhorizon); /*___MENU.C*/
    makecursoractive(); /*allows different active cursors*/
    redrawvec();
}
/*___sethorizon()____________________________________________________________*/

/****setremoteupdate()********************************************************/
void    setremoteupdate()   /*970522*/
{
    DoupdateDialog(); /*invokes remote program choice*/ /*MAGEDLOG.C*/
}
/*___setremoteupdate()_______________________________________________________*/

/****setdockingscope()********************************************************/
void setdockingscope()
{
   int j = 0;
   
   if(Ldockingscope==0)
   {/*define and start docking*/
      /*take as scope of docking first set of contiguous groups that are ON*/
      Lbeginselection = 0;
      Ndockingbondrot = 0;
      thisgrupptr = firstgrupptr;
      while(thisgrupptr != NULL)
      {/*loop over groups*/
         if(  (  ((thisgrupptr->STATUS & NOBUTTON) == 0) 
               || (thisgrupptr->STATUS & RECESSIVEON)     )
            &&(      thisgrupptr != markergrupptr               /*040320*/
                  && thisgrupptr != tablemarkergrupptr
                  && thisgrupptr != measuregrupptr
                  && thisgrupptr != drawgrupptr   ) )
         {/*has user available group box*/
            if(thisgrupptr->on && !Lbeginselection)
            {/*first group encountered that is ON*/
               Lbeginselection = 1;
               if(   (thisgrupptr->firstsgrpptr) != NULL
                 && ((thisgrupptr->firstsgrpptr)->firstlistptr) != NULL
                && (((thisgrupptr->firstsgrpptr)->firstlistptr)->firstpointptr)
                      != NULL  )
               {/*if graphics shows anything, first point should be OK 061022*/
                  beginselectionpointptr 
                   = ((thisgrupptr->firstsgrpptr)->firstlistptr)->firstpointptr;
               }
               if(   (thisgrupptr->lastsgrpptr) != NULL
                 && ((thisgrupptr->lastsgrpptr)->lastlistptr) != NULL
                && (((thisgrupptr->lastsgrpptr)->lastlistptr)->lastpointptr)
                      != NULL  )
               {/*usually OK, but MolProbity can output listless subgroups*/
                  endselectionpointptr 
                   = ((thisgrupptr->lastsgrpptr)->lastlistptr)->lastpointptr;
               }
               else
               {/*search this group's subgroups for a viable list 061022*/
                  thissgrpptr = (thisgrupptr->firstsgrpptr);
                  while(thissgrpptr != NULL)
                  {
                     if(    thissgrpptr->lastlistptr != NULL
                        && (thissgrpptr->lastlistptr)->lastpointptr != NULL)
                     {/*keep resetting this... at least first subgroup OK!?*/
                        endselectionpointptr
                         = (thissgrpptr->lastlistptr)->lastpointptr;
                     }
                     thissgrpptr = thissgrpptr->nextptr;
                  }/*last viable point should now be == endselectionpointptr*/
               }
               /*thisgrupptr->bondrot = DOCKSET;*/
               thisgrupptr->bondrot = thisgrupptr->bondrot | DOCKSET; /*040320*/
               mobilegrupptr = thisgrupptr; /*egam 030215 last one so far*/
            }
            else if(thisgrupptr->on && Lbeginselection)
            {/*another group encountered that is ON*/
               /*endselectionpointptr    061022 protect against empty subgroup*/
               /*   = ((thisgrupptr->lastsgrpptr)->lastlistptr)->lastpointptr;*/
               if(   (thisgrupptr->lastsgrpptr) != NULL
                 && ((thisgrupptr->lastsgrpptr)->lastlistptr) != NULL
                && (((thisgrupptr->lastsgrpptr)->lastlistptr)->lastpointptr)
                      != NULL  )
               {/*usually OK, but MolProbity can output listless subgroups*/
                  endselectionpointptr
                   = ((thisgrupptr->lastsgrpptr)->lastlistptr)->lastpointptr;
               }
               else
               {/*search this group's subgroups for a viable list 061022*/
                  thissgrpptr = (thisgrupptr->firstsgrpptr);
                  while(thissgrpptr != NULL)
                  {
                     if(    thissgrpptr->lastlistptr != NULL
                        && (thissgrpptr->lastlistptr)->lastpointptr != NULL)
                     {/*keep resetting this... at least first subgroup OK!?*/
                        endselectionpointptr
                         = (thissgrpptr->lastlistptr)->lastpointptr;
                     }
                     thissgrpptr = thissgrpptr->nextptr;
                  }/*last viable point should now be == endselectionpointptr*/
               }
               /*thisgrupptr->bondrot = DOCKSET;*/
               thisgrupptr->bondrot = thisgrupptr->bondrot | DOCKSET; /*040320*/
               mobilegrupptr = thisgrupptr; /*egam 030215 last one so far*/
            }
            else if(!thisgrupptr->on && Lbeginselection)
            {/*OFF group breaks run of ON groups*/
               if(referencegrupptr==NULL) /*first reference group egam 030215*/
                  {referencegrupptr=thisgrupptr;}/*egam 030215*/
               break; /*loop over groups*/
            }
            else
            {/*still waiting for an ON group*/
               if(referencegrupptr==NULL) /*first reference group egam 030215*/
                  {referencegrupptr=thisgrupptr;}/*egam 030215*/
               ;
            }
         }/*has user available group box*/
         thisgrupptr = thisgrupptr->nextptr;              
      }/*loop over groups*/
      if(Lbeginselection)
      {/*some ON groups were found, set them to be docked*/
          if((nbondrot + 6) < MAXBONDROT)
          {/*enough bondrot ControlHandle typedef--> HWND(PC), -->Widget(MUX)*/ 
            Ndockingbondrot = nbondrot;
            Ldockingscope = 1;
            Ldock3on3 = 1; /*allow [Edit/Draw New] dock3on3 button */
            if(!Legamickdocking)
            {/*egamickdocking only allows dock3on3  egam 030215 */
              for(j=1; j<=6; j++)
              {/*setup XYZ rotations and translations*/
                if((thisbondrotlistptr = allocbondrotliststructure()) == NULL)
                {/*allocation failed*/ /*MAGELIST*/
                   sprintf(alertstr,"failed to alloc %dth bondrotlist"
                       ,nbondrot+1);
                   alertstr2[0]='\0';alertstr3[0]='\0';
                   DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
                }/*allocation failed*/
                else
                {/*can construct another bondrotlist */
                   
                   nbondrot = nbondrot+1;
                   bondrotptr[nbondrot] = thisbondrotlistptr;
                   thisbondrotlistptr->origangle  = 0;
                   thisbondrotlistptr->angle  = 0;
                   if(nbondrot == (Ndockingbondrot+1))
                   {
                      sprintf(thisbondrotlistptr->name,"X rotation");
                      thisbondrotlistptr->level = 1;
                      thisbondrotlistptr->option = 4;
                   }
                   else if(nbondrot == (Ndockingbondrot+2))
                   {
                      sprintf(thisbondrotlistptr->name,"Y rotation");
                      thisbondrotlistptr->level = 2;
                      thisbondrotlistptr->option = 5;
                   }
                   else if(nbondrot == (Ndockingbondrot+3))
                   {
                      sprintf(thisbondrotlistptr->name,"Z rotation");
                      thisbondrotlistptr->level = 3;
                      thisbondrotlistptr->option = 6;
                   }
                   else if(nbondrot == (Ndockingbondrot+4))
                   {
                      sprintf(thisbondrotlistptr->name,"X translation");
                      thisbondrotlistptr->level = 4;
                      thisbondrotlistptr->option = 7;
                   }
                   else if(nbondrot == (Ndockingbondrot+5))
                   {
                      sprintf(thisbondrotlistptr->name,"Y translation");
                      thisbondrotlistptr->level = 5;
                      thisbondrotlistptr->option = 8;
                   }
                   else if(nbondrot == (Ndockingbondrot+6))
                   {
                      sprintf(thisbondrotlistptr->name,"Z translation");
                      thisbondrotlistptr->level = 6;
                      thisbondrotlistptr->option = 9;
                   }
                   thisbondrotlistptr->firstpointptr = NULL;  
                   thisbondrotlistptr->lastpointptr = NULL; 
                   thisbondrotlistptr->tailpointptr = NULL;
                   thisbondrotlistptr->headpointptr = NULL;
                   thisbondrotlistptr->scopeinstance = 0;

                   /*NOTE: This structure holds information about two sets of*/
                   /*points: those that define the axis, and those that define*/
                   /*the scope.  BUT it does not create those points, and it */
                   /*does NOT own those points: i.e. is NOT a parent */
 /*------------------*/               
                }/*can construct another bondrotlist */
              }/*setup XYZ rotations and translations*/
            }/*egamickdocking only allows dock3on3  egam 030215 */
          }/*enough bondrot ControlHandle typedef--> HWND(PC), -->Widget(MUX)*/ 
      }/*some ON groups were found, set them to be docked*/
      Lbeginselection = 0; /*must clear, used as global MAGEINPT & MAGESETS*/
   }/*define and start docking*/
   else
   {/*stop docking*/
      if(!Legamickdocking){ nbondrot = nbondrot - 6;}

      thisgrupptr = firstgrupptr;
      while(thisgrupptr != NULL)
      {/*loop over groups*/
         /*if(thisgrupptr->bondrot == DOCKSET)*/
         if((thisgrupptr->bondrot|DOCKSET) == DOCKSET) /*121108 (logic) */
         {/*clear any group that was specified as a Docking group*/
            /*thisgrupptr->bondrot = 0;*/
            thisgrupptr->bondrot = thisgrupptr->bondrot & ~DOCKSET;/*040320*/
         }
         thisgrupptr = thisgrupptr->nextptr;              
      }/*loop over groups*/
      Ldockingscope = 0;
      Ldock3on3 = 0; /*disallow [Edit/Draw New] dock3on3 button */

      referencegrupptr = NULL; /*egam 030215*/
      
      beginselectionpointptr = NULL;
      endselectionpointptr = NULL;
   }/*stop docking*/
/*   isavewidth = RWIDTH;*/
   if(nbondrot > 0) 
   {
      RWIDTH = ROTSWIDTH; /*100 Mac, fraction of screen PC */
      isavewidth = 0; /*reset to force recreation of bondrot window*/
   }
   else
   {
      RWIDTH =   0;
      icurrentrotation = 0;
   }
/*   if(RWIDTH != isavewidth) */
   {/*force recreation of sliders for any bondrots that are left over*/
      adjustwindowwidth(); /*MACWIND.C,MPCWIND.C,MUXMWIND.c*/
      if(RWIDTH == ROTSWIDTH) /*bondrots just now started */
          icurrentrotation = 1; /*970917*/ /*reset to first rotation*/
               /*so will always be defined either initially or by append*/
   }
   resetmenuchecks(); /*which calls Adjustmenus()*/
   LneedSetsinGrafWindow = 1; 
   redrawvec();         /*___DRAW.C*/ /*MPCDRAW ->()*/
}
/*___setdockingscope()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****setzlimitscope()********************************************************/
void setzlimitscope()
{
  if(Lzlimitscope==0)
  {/*define and start extra z-limit planes for selected lists*/
    /*take as scope of extra limit planes the lists that are ON*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*begin-loop-over-groups*/
      if(thisgrupptr->on)
      {/*begin-visible-group*/

        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*begin-loop-over-subgroups*/
          if(thissgrpptr->on > 0)
          {/*begin-visible-subgroup*/
            thislistptr = thissgrpptr->firstlistptr;

            while(thislistptr != NULL)
            {/*begin-loop-over-lists*/
              if(thislistptr->on > 0)
              {/*begin-visible-list*/

                thislistptr->STATUS = thislistptr->STATUS | ZLIMITED;

              }/*endof-visible-list*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*endof-loop-over-lists*/

          }/*endof-visible-subgroup*/
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr=NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*endof-loop-over-subgroups*/

      }/*endof-visible-group*/
      thisgrupptr = thisgrupptr->nextptr;/*==NULL if last group*/
    }/*endof-loop-over-groups*/

    Lzlimitscope = 1;
    izlimit = izclip; /*starting value*/

  }/*define and start extra z-limit planes for selected lists*/
  else /*Lzlimitscope == 1*/
  {/*remove flag for z-limit plane clipping from all lists*/

    /*take as scope of extra limit planes the lists that are ON*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*begin-loop-over-groups*/

        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*begin-loop-over-subgroups*/
            thislistptr = thissgrpptr->firstlistptr;

            while(thislistptr != NULL)
            {/*begin-loop-over-lists*/

                thislistptr->STATUS = thislistptr->STATUS & ~ZLIMITED;
                   /*unset ZLIMITED part of STATUS flag*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*endof-loop-over-lists*/

          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr=NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*endof-loop-over-subgroups*/

      thisgrupptr = thisgrupptr->nextptr;/*==NULL if last group*/
    }/*endof-loop-over-groups*/

    Lzlimitscope = 0;

  }/*remove flag for z-limit plane clipping from all lists*/

  recreatesliderBars(); /*____BAR.c*/
  resetmenuchecks(); /*which calls Adjustmenus()*/
  LneedSetsinGrafWindow = 1; 
  redrawvec();         /*___DRAW.C*/ /*MPCDRAW ->()*/
}
/*___setzlimitscope()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****menuadvancedhelp()*******************************************************/
void    menuadvancedhelp(int ichoice)  /* Help */
{
    DoAdvancedHelpDialog(ichoice);
}
/*___menuadvancedhelp()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/* incomplete, noticed 061116
a,A: animate
b,B: 2animate
c: cross<->wall eyed
C
d: dragline move/new
D
e: detail-at-ctr
E
f: flat scroll -vs- xy rotation with mouse drag
F: Find  UNIX uses Caps, Mac uses Command, PC uses Alt
g: wht bkg
G: Find Again  UNIX uses Caps, Mac uses Command, PC uses Alt
h: only rotate around Y axis (horizontal)
H
i
I
j
J
k: kaleidoscope
K: Next Kinemage  UNIX uses Caps, Mac uses Command, PC uses Alt
l: list/point color dominant
L
m: clears measure's lines from screen
M
n,N: ninty degree Y-axis rotation
o
O: Open  UNIX uses Caps, Mac uses Command, PC uses Alt
p: perspective
P: PointID into commandline string then --> simout.kin
q: bigger markers
Q: Quit  UNIX uses Caps, Mac uses Command, PC uses Alt
r,R: autoanimate and autorock
s: stereo
S: Save  UNIX uses Caps, Mac uses Command, PC uses Alt
t: cycles line width
T
u: remote update dialog
U
v
V: View params dlog  UNIX uses Caps, Mac uses Command, PC uses Alt
w: bigger text
W
x,X: + true recentering recalculation along screen Z axis
y: cycles color depths
Y
z,Z: - true recentering recalculation along screen Z axis
1,2,3,4: zoom +.01,+.1,-.01,-.1
5,6,7,8
9,0 : TBstereo offset
[,]: cycles Aspects
+,= accumulates animates
LARROW: decrement last angle
RARROW: increment last angle
DARROW: plot conformation position
UARROW: position graphics conformation
shiftkey down: keep detail, move dragline, reverse animate,restrict rotation...
*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
a,A: animate, b,B: 2animate, [,]: cycles Aspects"z,Z,x,X: true recentering recalculation along screen Z axis" +1*, +10*, -1*, -10* motion (ztran just quick offset)" n,N: ninty degree Y-axis rotation"1,2,3,4: zoom +.01,+.1,-.01,-.1"9,0: TBstereo offset

TOGGLES: s: stereo;  c: cross<->wall eyed"k: kaleidoscope;   p: perspective"f: flat scroll -vs- xy rotation with mouse drag"h: only rotate around Y axis (horizontal)"l: list/point color dominant"e: detail-at-ctr;  d: dragline move/new;  g: wht bkg

t: cycles line width, y: cycles color depths"u: remote update dialog."shiftkey down: keep detail, move dragline, reverse animate,"     restrict rotation to starting axis."m: clears measure's lines from screen."q: bigger markers, w: bigger text.

H: Help, +,=: accumulates animates"r,R: autoanimate, autorock (Tools/Kludges/MickeyMouse)"P: PointID -->simout.kin"MENU items (UNIX capital, Mac command, PC Alt):"F: Find, G: Find-again, O: Open, Q: Quit, S: Save"K: next Kinemage, V: View params dlog
*********PUSH256*/
/****menuaboutsecrets()******************************************************/
int    menuaboutsecrets(int it)  /*Keyboard Shortcuts*/
{
    int irtn=0;

    sprintf(alertstr,
        "a,A: animate, b,B: 2animate, [,]: cycles Aspects"
        CRLF"z,Z,x,X: true recentering recalculation along screen Z axis"
        CRLF" +1*, +10*, -1*, -10* motion (ztran just quick offset)"
        CRLF" n,N: ninty degree Y-axis rotation"
        CRLF"1,2,3,4: zoom +.01,+.1,-.01,-.1"
        CRLF"9,0: TBstereo offset"
        );
    sprintf(alertstr2,
            "TOGGLES: s: stereo;  c: cross<->wall eyed"
        CRLF"k: kaleidoscope;   p: perspective"
        CRLF"f: flat scroll -vs- xy rotation with mouse drag"
        CRLF"h: only rotate around Y axis (horizontal)"
        CRLF"l: list/point color dominant"
        CRLF"e: detail-at-ctr;  d: dragline move/new;  g: wht bkg"
        );
    sprintf(alertstr3,
            "t: cycles line width, y: cycles color depths"
        CRLF"u: remote update dialog."
        CRLF"shiftkey down: keep detail, move dragline, reverse animate,"
        CRLF"     restrict rotation to starting axis."
        CRLF"m: clears measure's lines from screen."
        CRLF"q: bigger markers, w: bigger text."
        );
    sprintf(alertstr4,
            "H: Help, +,=: accumulates animates"
        CRLF"r,R: autoanimate, autorock (Tools/Kludges/MickeyMouse)"
        CRLF"P: PointID -->simout.kin"
        CRLF"MENU items (UNIX capital, Mac command, PC Alt):"
        CRLF"F: Find, G: Find-again, O: Open, Q: Quit, S: Save"
        CRLF"K: next Kinemage, V: View params dlog"
        );
/*all recognize capital F and G, see  menuaboutOther(void)*/ /*Tools Help*/
/*keyboard menu invocations idiosyncratic to platform: */
/*F: Find, G: Find-again, K: next Kinemage, O: Open, Q: Quit, S: Save, */
/*V: View params dlog   UNIX uses Caps, Mac uses Command, PC uses Alt*/
/*LARROW: decrement last angle, RARROW: increment last angle,*/
/*DARROW: plot conformation position, UARROW: position graphics conformation*/
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4);   /*alertstr,alertstr2,alertstr3,alertstr4*/
       }
    }
    else
    {
       sprintf(word,"about keyboard short-cuts");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutsecrets()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Open New File...: (? save if kinemage edited)"Append File...: to current kinemage, inc. text"Save As...: regular output writes a file with" modified kin after pruning, new draws, views, etc." special option to write only currently Visible Parts

Save Special: writes a file with just the" New parts added with Draw_New functions" Rotatable parts, of bond-rotations (can append to an old file)." Current View with  zoom,zslab,center,matrix,viewID." Current XYZ point picked (e.g. for PREKIN focus).

   Questions and Answers from TEXT window."   file for 2D rendering of image by Raster3D or POV-Ray."   pointID re: @nowfind."   palette re: @fullrgbpalette."Write PostScript File: 2D image as currently displayed"Quit: suggests write if kinemage changed.
*********PUSH256*/
/****menuaboutwrite()*********Output Help*************************************/
int    menuaboutwrite(int it) /*Input Output Help*/
{
    int irtn=0;

    sprintf(alertstr,
            "Open New File...: (? save if kinemage edited)"
        CRLF"Append File...: to current kinemage, inc. text"
        CRLF"Save As...: regular output writes a file with"
        CRLF" modified kin after pruning, new draws, views, etc."
        CRLF" special option to write only currently Visible Parts"
    );
    sprintf(alertstr2,
            "Save Special: writes a file with just the"
        CRLF" New parts added with Draw_New functions"
        CRLF" Rotatable parts, of bond-rotations (can append to an old file)."
        CRLF" Current View with  zoom,zslab,center,matrix,viewID."
        CRLF" Current XYZ point picked (e.g. for PREKIN focus)."
    );
    sprintf(alertstr3,
            "   Questions and Answers from TEXT window."
        CRLF"   file for 2D rendering of image by Raster3D or POV-Ray."
        CRLF"   pointID re: @nowfind."
        CRLF"   palette re: @fullrgbpalette."
        CRLF"Write PostScript File: 2D image as currently displayed"
        CRLF"Quit: suggests write if kinemage changed."

    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about input and output");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutwrite()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
MACINTOSH: screen capture key push (s and 4 keys pushed together)"shift command s 4 "hear a shutter noise; \"Picture 1\" (2 ...) saved as pict file" view in SimpleText or some other plain viewer."put picture into clipboard: ctrl shift apple s 4

UNIX Commandline parameter: -postscript"kinemage dumped directly to invented output file, e.g.:"mage 1abc.kin -postscript"1abc.kin.1.eps , 1abc.kin.2.eps , ..."No GUI, Mage never on-screen!, one eps for each animation view

Postscript file of 2D image as currently displayed"send .eps file to a printer, or open in Gimp"or open in Illustrator, export to Photoshop"file for 2D rendering of image by Raster3D or POV-Ray."Raster3D: render -png <xxx.r3d >xxx.png
*********PUSH256*/
/****menuaboutscreenout()*****Output Help*************************************/
int    menuaboutscreenout(int it) /*Screen Capture Help*/ /*020904*/
{
    int irtn=0;

    sprintf(alertstr,
            "MACINTOSH: screen capture key push (s and 4 keys pushed together)"
        CRLF"shift command s 4 "
        CRLF"hear a shutter noise; \"Picture 1\" (2 ...) saved as pict file"
        CRLF" view in SimpleText or some other plain viewer."
        CRLF"put picture into clipboard: ctrl shift apple s 4"
    );
    sprintf(alertstr2,
            "UNIX Commandline parameter: -postscript"
        CRLF"kinemage dumped directly to invented output file, e.g.:"
        CRLF"mage 1abc.kin -postscript"
        CRLF"1abc.kin.1.eps , 1abc.kin.2.eps , ..."
        CRLF"No GUI, Mage never on-screen!, one eps for each animation view"
    );
    sprintf(alertstr3,
            "UNIX Commandline parameter: -help"
        CRLF"meager, use output to text window, or to html file!"
        CRLF"UNIX Commandline parameter: -changes"
        CRLF"chronological brief list of changes"
        CRLF"No GUI, Mage never on-screen!"
    );
    sprintf(alertstr4,
            "Postscript file of 2D image as currently displayed"
        CRLF"send .eps file to a printer, or open in Gimp"
        CRLF"or open in Illustrator, export to Photoshop"
        CRLF"file for 2D rendering of image by Raster3D or POV-Ray."
        CRLF"Raster3D: render -png <xxx.r3d >xxx.png"
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4); /*alertstr,alertstr2,alertstr3,alertstr4*/ 
       }
    }
    else
    {
       sprintf(word,"about screen capture, commandlines, foo in cavity");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutscreenout()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Edit image: side-panel-button selections"Draw New: lines, labels, constructions, dragged-out lines"Draw New Setup...: shortened lines, arrows, etc."Construct 4th line after 3 new picks based on 4-pt 'measures'"Arrows, multiple angles, perpendiculars

Keep Current View...: adds item to VIEWS menu"Change Color: of point or list of picked point"Prune: deletes PLL... points, 'Punch' only one, 'Auger' many"Superpick: also picks 'U'npickable points"Show Object Properties: edit data hierarchy of picked point

Text Editable: text and caption Cut, Copy, Paste."(can edit and use hypertext during a session)"Table Cell: edits whole cells, not cell contents." (table cells relate to graphics points by {pointID} )" (group parameter 'table' invokes a table window)
*********PUSH256*/
/****menuaboutedit()*************Edit Help*******************************/
int    menuaboutedit(int it)  /*Edit Help*/
{
    int irtn=0;

    sprintf(alertstr,
           "Edit image: side-panel-button selections"
       CRLF"Draw New: lines, labels, constructions, dragged-out lines"
       CRLF"Draw New Setup...: shortened lines, arrows, etc."
       CRLF"Construct 4th line after 3 new picks based on 4-pt 'measures'"
       CRLF"Arrows, multiple angles, perpendiculars"
    );
    sprintf(alertstr2,
           "Keep Current View...: adds item to VIEWS menu"
       CRLF"Change Color: of point or list of picked point"
       CRLF"Prune: deletes PLL... points, 'Punch' only one, 'Auger' many"
       CRLF"Superpick: also picks 'U'npickable points"
       CRLF"Show Object Properties: edit data hierarchy of picked point"
    );
    sprintf(alertstr3,
           "Text Editable: text and caption Cut, Copy, Paste."
       CRLF"(can edit and use hypertext during a session)"
       CRLF"Table Cell: edits whole cells, not cell contents."
       CRLF" (table cells relate to graphics points by {pointID} )"
       CRLF" (group parameter 'table' invokes a table window)"
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about editing kinemages");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutedit()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
KINEMAGE next kinemage or choose numbered kinemage in open file."Display options"Multi-width: line for depth cuing," or One Width: lines two pixels wide, or Thin Line: one pixel. 't' "5 depths of colors & line widths or just 3.  'y'

Perspective: Front larger, rear smaller (vs orthographic) 'p'"Stereo On: side-by-side images  's'" (cross-eye vs. wall-eye  'c')"Compare On: side-by-side pair of animated groups"Viewing Params...: stereo: angle, separation, mode...

List Color: made dominant over point color 'l'"Lens at ctr: list points only show in center region 'e'"Detail Level...: as a function of update rate"White Bkg: Colors on white background"B&W: pure Black on pure White;  grayscale: gray on B or W
*********PUSH256*/
/****menuaboutoptions()******************************************************/
int    menuaboutoptions(int it)  /*Display Help*/
{
    int irtn=0;

    sprintf(alertstr,
           "KINEMAGE next kinemage or choose numbered kinemage in open file."
       CRLF"Display options"
       CRLF"Multi-width: line for depth cuing,"
       CRLF" or One Width: lines two pixels wide, or Thin Line: one pixel. 't' "
       CRLF"5 depths of colors & line widths or just 3.  'y'"
    );
    sprintf(alertstr2,
           "Perspective: Front larger, rear smaller (vs orthographic) 'p'"
       CRLF"Stereo On: side-by-side images  's'"
       CRLF" (cross-eye vs. wall-eye  'c')"
       CRLF"Compare On: side-by-side pair of animated groups"
       CRLF"Viewing Params...: stereo: angle, separation, mode..."
    );
    sprintf(alertstr3,
           "List Color: made dominant over point color 'l'"
       CRLF"Lens at ctr: list points only show in center region 'e'"
       CRLF"Detail Level...: as a function of update rate"
       CRLF"White Bkg: Colors on white background"
       CRLF"B&W: pure Black on pure White;  grayscale: gray on B or W"
    );

    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about display options");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutoptions()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Font Sizes: label & wordlist font sizes, e.g. for lectures."stroked font: mimics 2D rendered font output."XY Scroll: (flatland) mouse motion->xy translations 'f'"Y rotation: only, horizontal world view 'h'"Kaleidoscope: no bkg clear: smears image 'k'

VIEWS menu"Undo Pickcenter: back out of bad pick."View#: go to that numbered, (named) view."Set Reader's View: temporarily save current appearance."Reader's View: go to appearance as Set.

Cursor cues for Display modes, pick point at left arrow tip."Cursor with I-beam on nose: Edit Menu: text editable!"Cursor crossed arrows: 2D xy scrolling, flatland 'f'"Cursor with upper & lower bars: horizontal rotations (y axis) 'h'

COMMANDLINE options for menu,dialog,text font sizes:"-fontinfo   dumps font possibilities to console"-large    largest font for menus and dialog boxes"-medium   medium  font"-macular  medium or large font based on screen size
*********PUSH256*/
/****menuaboutspecial()*******************************************************/
int    menuaboutspecial(int it) /*Special Display Help: fonts, scrolls, views*/
{
    int irtn=0;

    sprintf(alertstr,
            "Font Sizes: label & wordlist font sizes, e.g. for lectures."
        CRLF"stroked font: mimics 2D rendered font output."
        CRLF"XY Scroll: (flatland) mouse motion->xy translations 'f'"
        CRLF"Y rotation: only, horizontal world view 'h'"
        CRLF"Kaleidoscope: no bkg clear: smears image 'k'"
    );
    sprintf(alertstr2,
            "VIEWS menu"
        CRLF"Undo Pickcenter: back out of bad pick."
        CRLF"View#: go to that numbered, (named) view."
        CRLF"Set Reader's View: temporarily save current appearance."
        CRLF"Reader's View: go to appearance as Set."
    );
    sprintf(alertstr3,
            "Cursor cues for Display modes, pick point at left arrow tip."
        CRLF"Cursor with I-beam on nose: Edit Menu: text editable!"
        CRLF"Cursor crossed arrows: 2D xy scrolling, flatland 'f'"
        CRLF"Cursor with upper & lower bars: horizontal rotations (y axis) 'h'"
    );
    sprintf(alertstr4,
            "COMMANDLINE options for menu,dialog,text font sizes:"
        CRLF"-fontinfo   dumps font possibilities to console"
        CRLF"-large    largest font for menus and dialog boxes"
        CRLF"-medium   medium  font"
        CRLF"-macular  medium or large font based on screen size"
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4); /*alertstr,alertstr2,alertstr3,alertstr4*/ 
       }
    }
    else
    {
       sprintf(word,"about fonts, views, and cursors");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutspecial()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
measures: distance between last 2 points picked"  simple & dihedral angles from last 4 points"  highlighted lines connect up to last 4 points"  dots for average of last 2,3,4, &(5 or more) points shown"  measured dihedrals labeled is Draw New option

  Also: perpendicular distance from last point picked"  to the line defined by the previous two picked points,"  and perpendicular distance from last point picked"  to the plane defined by the previous 3 points."  (see menu item: Kludges/measures+perp d)

F: Find point by name... ; G: Find Again"Empowerments: submenu of useful quickies"  Hide Measures: clear lines, dots from screen 'm'"  Restore Bond Rotations: to initial angles."  Rotate 90 deg +: 'n', -: 'N' y axis"  Rotate 90 deg +, -, other axes

Remote Update...: from prekin, probe, etc. 'u'"XYZ point: coord of last picked point exhibited"gnomon: at center. Single_Marker option"Rate: updates/sec exhibited"Count: #lines on, total# points, time"Z limit extra for ON lists.  Fine step amount choice
*********PUSH256*/
/****menuaboutOther()********************************************************/
int    menuaboutOther(int it) /*Tools Help*/
{
    int irtn=0;

    sprintf(alertstr,
           "measures: distance between last 2 points picked"
       CRLF"  simple & dihedral angles from last 4 points"
       CRLF"  highlighted lines connect up to last 4 points"
       CRLF"  dots for average of last 2,3,4, &(5 or more) points shown"
       CRLF"  measured dihedrals labeled is Draw New option"
    );
    sprintf(alertstr2,
           "  Also: perpendicular distance from last point picked"
       CRLF"  to the line defined by the previous two picked points,"
       CRLF"  and perpendicular distance from last point picked"
       CRLF"  to the plane defined by the previous 3 points."
       CRLF"  (see menu item: Kludges/measures+perp d)"
    );
    sprintf(alertstr3,
           "F: Find point by name... ; G: Find Again"
       CRLF"Empowerments: submenu of useful quickies"
       CRLF"  Hide Measures: clear lines, dots from screen 'm'"
       CRLF"  Restore Bond Rotations: to initial angles."
       CRLF"  Rotate 90 deg +: 'n', -: 'N' y axis"
       CRLF"  Rotate 90 deg +, -, other axes"
    );
    sprintf(alertstr4,
           "Remote Update...: from prekin, probe, etc. 'u'"
       CRLF"XYZ point: coord of last picked point exhibited"
       CRLF"gnomon: at center. Single_Marker option"
       CRLF"Rate: updates/sec exhibited"
       CRLF"Count: #lines on, total# points, time"
       CRLF"Z limit extra for ON lists.  Fine step amount choice" 
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4); /*alertstr,alertstr2,alertstr3,alertstr4*/ 
       }
    }
    else
    {
       sprintf(word,"about measures, rotations, and updates");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutOther()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
clip middle if ends off-scrn, measures+perp-dist. to line&plane"Pick Marker: axial extentions for graph correlation"Mickey Mouse: r: autorock; R: autoanimate"Test Kin Format: strict kin format during input"Cursor Choice: co-laboratory remote"

Hypertext control showing alternate rotamers:"*{rot={name 1} #.#, rot={name 2} #.# ...}*  abr: 'rot=','rot ','r '"where 'name 1' is the bondrot identifier string on the slider bar"and #.# is the new value for that rotation angle

If Update by PROBE active, hypertext 'rot' triggers an update."PREKIN after version 5.74 makes hypertext commands" for rot/mutated residues"'Append File' will append (hyper)text to text window" Show Object Properties: can delete last rot group.

Capture the foo: menu:Tools/Kludges/foo in cavity"Pick point for initial foo, grow in cavity, surface the foo."Foo will coat a surface, and not puff out into the void."Show Object Properties: group ignorable by foo"VdW size: prekin -atomradii ...
*********PUSH256*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****menuaboutkluges()*******************************************************/
int    menuaboutkluges(int it)  /*Kludges,kludges: More Tools*/
{
    int irtn=0;

    sprintf(alertstr,
           "clip middle if ends off-scrn"
       CRLF"measures+perp-dist. to line&plane"
       CRLF"Pick Marker: axial extentions for graph correlation"
       CRLF"Mickey Mouse: r: autorock; R: autoanimate"
       CRLF"Test Kin Format: strict kin format during input"
       CRLF"Cursor Choice: co-laboratory remote"
       /*CRLF"Debug Test printf"*/
    );
    sprintf(alertstr2,
           "Hypertext control showing alternate rotamers:"
       CRLF"*{rot={name 1} #.#, rot={name 2} #.# ...}*  abr: 'rot=','rot ','r '"
       CRLF"where 'name 1' is the bondrot identifier string on the slider bar"
       CRLF"and #.# is the new value for that rotation angle"
    );
    sprintf(alertstr3,
           "If Update by PROBE active, hypertext 'rot' triggers an update."
       CRLF"PREKIN after version 5.74 makes hypertext commands"
       CRLF" for rot/mutated residues"
       CRLF"'Append File' will append (hyper)text to text window"
       CRLF" Show Object Properties: can delete last rot group."
    );
    sprintf(alertstr4,
           "Capture the foo: menu:Tools/Kludges/foo in cavity"
       CRLF"Pick point for initial foo, grow in cavity, surface the foo."
       CRLF"Foo will coat a surface, and not puff out into the void."
       CRLF"Show Object Properties: group ignorable by foo"
       CRLF"VdW size: prekin -atomradii ..."
    );

    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF); 
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4);/*alertstr,alertstr2,alertstr3,alertstr4*/
             /*___DLOG*/
       }
    }
    else
    {
       sprintf(word,"about kludges and hypertext");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutkluges()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
MAGE cursor points active tip to left, shape indicates mode."Clicking the mouse button while the arrow cursor tip touches" a point, writes that point's {point ID} to the screen, and" distance to previous picked point."(use single or left mouse button)

Dragging:"Moving the mouse while holding the button down rotates image"Starting cursor in lower 5/6 screen rotates around x,y axes,"Starting in upper 1/6 of the screen, rotates around z axis,"Shift--Drag intiates motion only around initial starting axis

Rotation of the image continues to be coupled to the hand motion"exactly as it started no matter where the cursor moves on the screen."Thus the reader (lecturer) can place the cursor where convenient"and can ignore it thereafter."Option 2D scrolling 'f'
*********PUSH256*/
/****menuaboutmouse()********************************************************/
int    menuaboutmouse(int it)  /*Mouse Operations*/
{
    int irtn=0;

    sprintf(alertstr,
          "MAGE cursor points active tip to left, shape indicates mode."
      CRLF"Clicking the mouse button while the arrow cursor tip touches"
      CRLF" a point, writes that point's {point ID} to the screen, and"
      CRLF" distance to previous picked point."
      CRLF"(use single or left mouse button)"
      );
    sprintf(alertstr2,
          "Dragging:"
      CRLF"Moving the mouse while holding the button down rotates image"
      CRLF"Starting cursor in lower 5/6 screen rotates around x,y axes,"
      CRLF"Starting in upper 1/6 of the screen, rotates around z axis,"
      CRLF"Shift--Drag intiates motion only around initial starting axis"
      );

    sprintf(alertstr3,
         "Rotation of the image continues to be coupled to the hand motion"
     CRLF"exactly as it started no matter where the cursor moves on the screen."
      CRLF"Thus the reader (lecturer) can place the cursor where convenient"
      CRLF"and can ignore it thereafter."
      CRLF"Option 2D scrolling 'f'"
      );

    sprintf(alertstr4,  /*050916*/
          "Use Left mouse button for the above operations."
      CRLF" "
      CRLF"Right mouse button:  click on point does pick-center"
      CRLF"   drag up and down to adjust zoom,"
      CRLF"   drag left and right to adjust zclip"
      );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4);/*alertstr,alertstr2,alertstr3,alertstr4*/
                              /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about using the mouse");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutmouse()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Docking Scope: contiguous groups all ON made mobile"6 sliders: x,y,z screen axis rotations, translations, always active"Shift-mouse-drag rotates mobile group(s)"Mouse scrolls while ctrl (PC:alt) key held down"Shift-ctrl-mouse-drag translates mobile groups

Rotations are around screen center: re Pickcenter"except rotation center moves with Shift-ctrl-mouse-drag."Note: 2D scrolling mode by 'f' key toggle:" release resets rotation ctr to screen ctr."Toggle 'f' to reset center after mouse-drag operations.

(use single or left mouse button)"Docking mode pre-empts Shift-mouse-drag," so not restricted to initial direction (see mouse help)."Docking scope activates [Edit/Draw New] dock3on3 button" pick 3pts in ref, 3 in mobile: m1-->r1, m2-->r2, m3-->r3

The game EGAM (Docking practice) use only 3on3 docking."EGAM immediately anneals in the moved object and"regenerates another free mobile object."The multi-egamick-docking anneals only when another object"is selected from the button panel.
*********PUSH256*/
/****menuaboutdocking()******************************************************/
int    menuaboutdocking(int it)  /*docking and more mouse Operations*/
{
    int irtn=0;

    sprintf(alertstr,
           "Docking Scope: contiguous groups all ON made mobile"
       CRLF"6 sliders: x,y,z screen axis rotations, translations, always active"
       CRLF"Shift-mouse-drag rotates mobile group(s)"
       CRLF"Mouse scrolls while ctrl (PC:alt) key held down"
       CRLF"Shift-ctrl-mouse-drag translates mobile groups"
      );
    sprintf(alertstr2,
          "Rotations are around screen center: re Pickcenter"
      CRLF"except rotation center moves with Shift-ctrl-mouse-drag."
      CRLF"Note: 2D scrolling mode by 'f' key toggle:"
      CRLF" release resets rotation ctr to screen ctr."
      CRLF"Toggle 'f' to reset center after mouse-drag operations."
      );

    sprintf(alertstr3,
          "(use single or left mouse button)"
      CRLF"Docking mode pre-empts Shift-mouse-drag,"
      CRLF" so not restricted to initial direction (see mouse help)."
      CRLF"Docking scope activates [Edit/Draw New] dock3on3 button" /*020305*/
      CRLF" pick 3pts in ref, 3 in mobile: m1-->r1, m2-->r2, m3-->r3"
      );

    sprintf(alertstr4,
          "The game EGAM (Docking practice) use only 3on3 docking."
      CRLF"EGAM immediately anneals in the moved object and"
      CRLF"regenerates another free mobile object."
      CRLF"The multi-egamick-docking anneals only when another object"
      CRLF"is selected from the button panel."
      );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4);/*alertstr,alertstr2,alertstr3,alertstr4*/
             /*___DLOG*/
       }
    }
    else
    {
       sprintf(word,"about docking");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutdocking()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
The windows can be resized and repositioned."If a window gets hidden behind others " it can be brought to the front " by the Show commands under the Help pull-down-menu

A combination of moving the window by " dragging the window's top bar and resizing using the" hot point at the lower right corner of the graphics window" allows control of the size and placement of the image.

The Mac graphics window expansion box is at the far right of its"title bar.  Large displaydevices (140912) may need greater than default memory size."Before running MAGE, Get-Info on the Macintosh File"pull-down-menu allows increasing the Minimum and Preferred Size.

The PC graphics and kinemage control buttons are in separate windows."Blindly expanding the graphics can cover essential controls.

The X-Windows graphics, text, caption, (and rotations bars) windows "are arbitrarily placed (and sized) by the current X client process. "Drag and resize them for your convenience.
*********PUSH256*/
/****menuaboutresize()********************************************************/
int    menuaboutresize(int it)  /*Window Resizing*/
{
    int irtn=0;

    char alertstr4[MAXALERTSTR],alertstr5[MAXALERTSTR];
    sprintf(alertstr,
        "The windows can be resized and repositioned."
    CRLF"If a window gets hidden behind others "
    CRLF" it can be brought to the front "
    CRLF" by the Show commands under the Help pull-down-menu"    
        );
    sprintf(alertstr2,
        "A combination of moving the window by "
    CRLF" dragging the window's top bar and resizing using the"
    CRLF" hot point at the lower right corner of the graphics window"
    CRLF" allows control of the size and placement of the image."   
        );
    sprintf(alertstr3,
        "The Mac graphics window expansion box is at the far right of its"
    CRLF"title bar.  Large displays may need greater than default memory size."
    CRLF"Before running MAGE, Get-Info on the Macintosh File"    
    CRLF"pull-down-menu allows increasing the Minimum and Preferred Size."
        );
    sprintf(alertstr4,
        "The PC graphics and kinemage control buttons are in separate windows."
    CRLF"Blindly expanding the graphics can cover essential controls." 
        );
    sprintf(alertstr5,
        "The X-Windows graphics, text, caption, (and rotations bars) windows "
    CRLF"are arbitrarily placed (and sized) by the current X client process. "
    CRLF"Drag and resize them for your convenience." 
        );

    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4);
          inserttexts(CRLF);
          inserttexts(alertstr5);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
#ifdef PCWINDOWS
          sprintf(alertstr3,"%s",alertstr4);
#endif /*PCWINDOWS*/
#ifdef UNIX_X11
          sprintf(alertstr3,"%s",alertstr5);
#endif /*UNIX_X11*/

          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about resizing windows");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else if(it==5) {sprintf(temps,"%s",alertstr5); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutresize()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Table: specify by @group {name} table"naturally use wordlist of points with {pointID} and <comment>"Table <--> Graphics picking by common {point ID} strings"Table --> Graphics picking always active"Graphics --> Table picking toggle button in right panel

Table cells contain the point's <comment> field"Menu FIND can look for string in Table cells' <contents>" check box choice in FIND dialog."Point's coordinates 'x, y, z' not used,"can be replaced with 'tablecell' or 'noxyz'

*********PUSH256*/
/****menuabouttable()*********************************************************/
int    menuabouttable(int it)  /*Table Help*/
{
    int irtn=0;

    sprintf(alertstr,
            "Table: specify by @group {name} table" 
        CRLF"naturally use wordlist of points with {pointID} and <comment>"
        CRLF"Table <--> Graphics picking by common {point ID} strings"
        CRLF"Table --> Graphics picking always active"
        CRLF"Graphics --> Table picking toggle button in right panel"
    );
    sprintf(alertstr2,
            "Table cells contain the point's <comment> field"
        CRLF"Menu FIND can look for string in Table cells' <contents>"
        CRLF" check box choice in FIND dialog."
        CRLF"Point's coordinates 'x, y, z' not used,"
        CRLF"can be replaced with 'tablecell' or 'noxyz'"
    );
    sprintf(alertstr3,
           ""
       CRLF""
       CRLF""
       CRLF""
       CRLF""
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about tables");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuabouttable()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
Bonds:"vectorlist {...} ... #bondrot #.# specifies rotations around"list's first vector.  Nested larger prefix #, listed value == #.#"scope continues until #bondrot of == or lower #, or end of file"Update: PREKIN can do mutations and set up CHI rotations.
*********PUSH256*/
/****menuaboutbonds()*********************************************************/
int    menuaboutbonds(int it)  /*Bonds Help, inc bondrots  130119*/
{
    int irtn=0;

    sprintf(alertstr,
           "Bonds:"
       CRLF"vectorlist {...} ... #bondrot #.# specifies rotations around"
       CRLF"list's first vector.  Nested larger prefix #, listed value == #.#"
       CRLF"scope continues until #bondrot of == or lower #, or end of file"
       CRLF"Update: PREKIN can do mutations and set up CHI rotations."
    );
    sprintf(alertstr2,
            "ganged sets: bondrot (no #) same names use same angle settings"
        CRLF""
       CRLF"use dummy noslider to end numbered bondrots sets within ganged sets"
        CRLF"e.g. for ile in middle of regular phi,psi polyala 12-mer"
        CRLF""
    );
    sprintf(alertstr3,
            "" 
        CRLF""
        CRLF""
        CRLF""
        CRLF""
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about bonds");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutbonds()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256
3D Literacy timed test invoked only from input kinemage:"@minutes 5.5"@drawline"for kinemage timed for 5 minutes and 30 sec."At beginning a Dialog box requests a few character for a"unique identifier (or short name) for the test taker.

At end: results of each test automatically written"to a uniquely named file with answer as \"new group\"."   Auto-scoring can be set up."Score value is at beginning of kinemage part,"and color-coded \"score\" group at kinemage end."see results in mage.

Test mode disables Quit,"  force progression with KINEMAGE/Score & Next."Between timed kinemages, a Dialog box shows previous score,"and brief insructions about next test"(taken from first 255 characters of next caption).

     Untimed practice: scoring set up but no @minutes"Check result with KINEMAGE/Score & Stay."Add @drawline,@drawlabel,@drawball,"But only one type of answer can be scored.
*********PUSH256*/
/****menuabouttest3D()********************************************************/
int    menuabouttest3D(int it)  /*3D Literacy test Help*/
{
    int irtn=0;

    sprintf(alertstr,
            "3D Literacy timed test invoked only from input kinemage:" 
        CRLF"@minutes 5.5"
        CRLF"@drawline"
        CRLF"for kinemage timed for 5 minutes and 30 sec."
        CRLF"At beginning a Dialog box requests a few character for a"
        CRLF"unique identifier (or short name) for the test taker."
    );
    sprintf(alertstr2,
            "At end: results of each test automatically written"
        CRLF"to a uniquely named file with answer as \"new group\"."
        CRLF"   Auto-scoring can be set up."
        CRLF"Score value is at beginning of kinemage part,"
        CRLF"and color-coded \"score\" group at kinemage end."
        CRLF"see results in mage."
    );
    sprintf(alertstr3,
           "Test mode disables Quit,"
       CRLF"  force progression with KINEMAGE/Score & Next."
       CRLF"Between timed kinemages, a Dialog box shows previous score,"
       CRLF"and brief insructions about next test"
       CRLF"(taken from first 255 characters of next caption)."
    );
    sprintf(alertstr4,  /*020904  additional field*/
           "     Untimed practice: scoring set up but no @minutes"
       CRLF"Check result with KINEMAGE/Score & Stay."
       CRLF"Add @drawline,@drawlabel,@drawball,"
       CRLF"But only one type of answer can be scored."
    );

    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);
          inserttexts(alertstr4); /*020904 now additional field*/
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(4);/*alertstr,alertstr2,alertstr3,alertstr4*/
             /*___DLOG*/
       }
    }
    else
    {
       sprintf(word,"about 3D Literacy test");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else if(it==4) {sprintf(temps,"%s",alertstr4); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuabouttest3D()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/******PUSHES256

*********PUSH256*/
/****menuaboutNDIM()*********************************************************/
int    menuaboutNDIM(int it)  /*N DIMension Help 130119*/
{
    int irtn=0;

    sprintf(alertstr,
            "Mobile 3D at a time NDIM, select color, specify" 
        CRLF"@dimension {names} {names} ... e.g. 7 "
        CRLF"@dimminmax 0.0 360.0 0.0 360.0 etc or -180 +180 ..."
        CRLF"Parellel coords toggle with '/'"
        CRLF"@group {___} ...  dimension=7  select"
    );
    sprintf(alertstr2,
            "screen fixed NDIM with mobile 3D suitfit:"
        CRLF"screen fixed NDIM7 parameter balllist radius= 0.0051,"
        CRLF"ringlist radius= 0.75"
        CRLF"works well with @scale 60.0 and suitefit42NDIM7.kin with @zoom 1.0"
        CRLF"note: list inherits screen from group."
    );
    sprintf(alertstr3,
           "Mobile 3D at a time NDIM and mobile 3D suitefit also works"
       CRLF"suitefit has to be scaled to match NDIM parameters"
       CRLF"and centered at 0,0,0"
       CRLF""
       CRLF""
    );
    if(it==0)
    {
       if(Linternal)
       {/*write alertstrs to text window, need intervening CRLFs*/
          inserttexts(CRLF); /*extra line between alertstr sets*/
          inserttexts(alertstr);
          inserttexts(CRLF);
          inserttexts(alertstr2);
          inserttexts(CRLF);
          inserttexts(alertstr3);
          inserttexts(CRLF);  /*always end with a CRLF*/
       }
       else
       {
          DoMageDLOGreport(1);   /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/
       }
    }
    else
    {
       sprintf(word,"about N DIMensions");
       if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
       else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
       else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
       else           {sprintf(temps," ");            irtn=0;}
    }
    return(irtn);
}
/*___menuaboutNDIM()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

