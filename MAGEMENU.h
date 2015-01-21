#ifdef EXTERNMENU
#undef  EXTERNMENU
#define EXTERNMENU
#else
#define EXTERNMENU extern
#endif
 

/*MAC && PC defined things, many are embedded in common code */
/* strings */
#define ST_ZOOM_TITLE       901
#define ST_ZSLAB_TITLE      902
#define ST_ZTRAN_TITLE      903
#define ST_ZLIMIT_TITLE     904

#ifdef PCWINDOWS
/* menu items */
#define openItem            101
#define appendItem          102
#define importItem          103
#define writekinItem        104
#define writePostScriptItem 105
#define quitItem            106
 
#define writedrawnewItem    111
#define writerottedItem     112
#define writeviewItem       113
#define writefocusItem      114
#define writeQAItem         115
#define writeR3DItem        116
#define writeptIDItem       117
#define writepaletteItem    118
#define writemeansigmaItem  119 /*060604*/

#define drawlineItem	    201
#define drawsetupItem       202
#define editviewItem        203
#define colorshowItem       204
#define emphasisItem        205
#define pruneItem           206
#define superpickItem       207
#define pickshowItem        208
#define movepointsItem      209 /*061126*/
#define meansigmasItem      210 /*061126*/
#define SELECTINGlistItem   211 /*060622list*/


#define enableTEItem        212

#define cutItem             213
#define copyItem            214
#define pasteItem           215



#define tablecutItem        221
#define tablerestoreItem    222
#define tablepreinsertItem  223
#define tablepostinsertItem 224
#define tablerebuildItem    225
#define tablecolorItem      226

#define nextItem            301
#define chooseItem          302
#define scoreNnextItem      303
#define scoreNstayItem      304

#define aspectsItem         400
#define multiwidthItem      401
#define widthItem           402
#define thinItem            403
#define depth5Item          404
#define depth3Item          405
#define perspecItem         406
#define stereoItem          407
#define compareItem         408
#define sterangItem         409
#define listcolorItem       410
#define lensItem            411
#define detailItem          412
#define whtbkgItem          413
#define B_WItem             414
#define grayItem            415
#define GFontItem           416
#define strokedfontItem     417
#define flatItem            418
#define horizItem           419
#define plotonlyItem        420
#define fontcursorItem      421

#define aspect0Item         450
#define aspect1Item         451
#define aspect2Item         452
#define aspect3Item         453
#define aspect4Item         454
#define aspect5Item         455
#define aspect6Item         456
#define aspect7Item         457
#define aspect8Item         458
#define aspect9Item         459
#define aspect10Item        460
#define aspect11Item        461
#define aspect12Item        462
#define aspect13Item        463
#define aspect14Item        464
#define aspect15Item        465
#define aspect16Item        466
#define aspectscrnItem      467

#define undopickctrItem    500
#define reset1Item			501
#define reset2Item			502
#define reset3Item			503
#define reset4Item			504
#define reset5Item			505
#define reset6Item			506
#define reset7Item			507
#define reset8Item			508
#define reset9Item			509
#define reset10Item			510
#define reset11Item			511
#define reset12Item			512
#define reset13Item			513
#define reset14Item			514
#define reset15Item			515
#define reset16Item			516
#define reset17Item			517
#define reset18Item			518
#define reset19Item			519
#define reset20Item			520
#define reset21Item			521
#define reset22Item			522
#define reset23Item			523
#define reset24Item			524
#define reset25Item			525
#define reset26Item			526
#define reset27Item			527
#define reset28Item			528
#define reset29Item			529
#define reset30Item			530
#define reset31Item			531
#define reset32Item			532
#define reset33Item			533
#define reset34Item			534
#define reset35Item			535
#define reset36Item			536
#define reset37Item			537
#define reset38Item			538
#define reset39Item			539
#define reset40Item			540
#define reset41Item			541
#define reset42Item			542
#define reset43Item			543
#define reset44Item			544
#define reset45Item			545
#define reset46Item			546
#define reset47Item			547
#define reset48Item			548
#define reset49Item			549
#define reset50Item			550
#define reset51Item			551
#define reset52Item			552
#define reset53Item			553
#define reset54Item			554
#define reset55Item			555
#define reset56Item			556
#define reset57Item			557
#define reset58Item			558
#define reset59Item			559
#define reset60Item			560
#define resetRItem			561
#define readersviewItem                 562
#define NDaxesPickedCtrItem             563  /*070315*/

#define searchItem          601
#define findagainItem       602
#define measureItem         603
#define hidemeasuresItem    604
#define brotangleItem       605
#define rot90plusItem       606
#define rot90minusItem      607
#define rot90xplusItem      608
#define rot90xminusItem     609
#define rot90zplusItem      610
#define rot90zminusItem     611
#define stereoplusItem      612
#define stereominusItem     613
#define remoteupdateItem    614
#define xyzpointItem        615
#define gnomonItem          616 /*021326*/
#define singlemarkerItem    617
#define rateItem            618
#define countItem           619
#define dockingscopeItem    620 /*000814*/
#define zlimitscopeItem     621 /*011201*/
#define finestepItem        622 /*011201*/
#define formattestItem      623
#define testItem            624
#define pickmarkerItem      625 /*000611*/
#define visibleoutItem      626 /*001118*/
#define rescoringItem       627 /*020830*/
#define backgroundItem      628 /*010408*/
#define mickeymouseItem     629 /*010609*/
#define pdbformatoutItem    630 /*040208*/
#ifdef EGAMICK
#define egamickdockingItem  631 /*030215*/
#define annealmobileItem    632 /*030215*/
#endif
#define foocavityItem       633 /*030330*/
#define measperpItem        634 /*030930*/
#define measuredsurveilItem 635 /*091212*/
#define dimensionItem       636 /*060121*/
#define allbrsliderItem     637 /*070617*/

#define clipmiddleItem      640 /*051214*/

#define secretItem          701
#define showtextItem	    702
#define showcaptionItem	    703
#define showgraphicsItem    704
#define showtableItem       705
#define showbondrotsItem    706
#define aboutItem           707
#define writeaboutItem      708
#define editaboutItem	    709
#define optionsaboutItem    710
#define specialaboutItem    711
#define kludgesaboutItem    712
#define kludgeItem          713
#define helpItem            714
#define mousehelpItem       715
#define dockinghelpItem     716
#define resizehelpItem      717
#define otheraboutItem      718
#define tablehelpItem       719
#define bondshelpItem       720
#define test3DhelpItem      721
#define NDIMhelpItem        722
#define makehelptextItem    723
#define makekinemageItem    724
#define makehelphtmlItem    725
#define iohelpItem          733
#define screenoutItem       734
#endif /*PCWINDOWS*/

EXTERNMENU int NHelpItems; /*Macintosh # of items already in Mac's Help menu*/

/* these menu enum items are embedded in the code, must be defined*/
/* PC  does a typedef to HANDLE for MenuHandle */
/* UNIX_X11 does a typedef to Widget for MenuHandle */
EXTERNMENU MenuHandle  fileMenu,writeMenu,editMenu,windMenu,kineMenu;
EXTERNMENU MenuHandle  optiMenu,viewMenu,otherMenu,keyempMenu,kludgeMenu;
EXTERNMENU MenuHandle  extraviewMenu,aspectMenu;
EXTERNMENU MenuHandle  appleMenu,mastersMenu,colorMenu,empowerMenu,iohelpMenu;
EXTERNMENU MenuHandle  moreviewMenu; /*UNIX divides into 3 for large fonts*/

/* Mac does the menu items enumerated, PC does them as arbitrary numbers */
/* but X-Windows does all buttons as Widgets, so these are redefined below*/
#ifdef MACINTOSH
  enum    
  {
      appleID = 1
     ,fileID
     ,writeID
     ,editID
     ,kineID
     ,optiID
     ,aspectID  /*submenu*/
     ,viewID
     ,extraviewID /*submenu*/ 
     ,otherID
     ,keyempID  /*submenu*/
     ,kludgeID  /*submenu*/
     ,windID
     ,colorID
     ,mastersID
     ,iohelpID  /*submenu 020904*/
     /*,helpID*/
  };

  enum    /*File*/
  { 
      openItem = 1
     ,appendItem     
     ,importItem     
     ,F1Item
     ,writekinItem    
     ,writeItem
     ,F2Item
     ,writePostScriptItem
     ,F3Item
     ,quitItem 
  };

  enum    /*Write*/
  {
      writedrawnewItem = 1
     ,writerottedItem
     ,writeviewItem   /* write matrix,center,zoom,slab of current view */
     ,writefocusItem  /* focusItem: write focus point choice */
     ,writeQAItem
     ,writeR3DItem
     ,writeptIDItem
     ,writepaletteItem
  };

  enum    /*Edit*/
  {
       drawlineItem = 1
      ,drawsetupItem
      ,E1Item
      ,editviewItem       /*old matrixItem*/
      ,E2Item
      ,colorshowItem
      ,emphasisItem
      ,pruneItem  
      ,superpickItem 
      ,pickshowItem    /*061126 old Mac NOT movepointsItem, meansigmasItem */
      ,E3Item
      ,enableTEItem
      ,cutItem
      ,copyItem
      ,pasteItem     
      ,E4Item
      ,tablecutItem
      ,tablerestoreItem
      ,tablepreinsertItem
      ,tablepostinsertItem  
      ,tablerebuildItem 
      ,tablecolorItem        
  };
  enum    /*Kinemage*/
  {  
       nextItem = 1
      ,chooseItem 
      ,scoreNnextItem 
      ,scoreNstayItem 
  };

  enum    /*Display*/
  {
      aspectItem = 1
     ,multiwidthItem
     ,widthItem
     ,thinItem
     ,depth5Item
     ,depth3Item
     ,D1Item
     ,perspecItem
     ,stereoItem
     ,compareItem
     ,sterangItem
     ,D2Item
     ,listcolorItem
     ,lensItem
     ,detailItem    
     ,D3Item
     ,whtbkgItem
     ,B_WItem
     ,grayItem
     ,D4Item
     ,GFontItem
     ,strokedfontItem
     ,D5Item
     ,flatItem     /*flatland*/
     ,horizItem
     ,plotonlyItem /*kalide*/
     ,fontcursorItem
  };

  enum    /*ASPECTS*/
  { 
     aspect0Item=1,aspect1Item,aspect2Item,aspect3Item,aspect4Item
     ,aspect5Item,aspect6Item,aspect7Item,aspect8Item,aspect9Item
     ,aspect10Item,aspect11Item,aspect12Item,aspect13Item,aspect14Item
     ,aspect15Item,aspect16Item,aspectscrnItem
  };

  enum    /*VIEWS*/
  { 
      undopickctrItem = 1
     ,reset1Item,reset2Item,reset3Item,reset4Item
     ,reset5Item,reset6Item,reset7Item,reset8Item,reset9Item
     ,reset10Item,reset11Item,reset12Item,reset13Item,reset14Item
     ,reset15Item,reset16Item,reset17Item,reset18Item,reset19Item
     ,reset20Item,reset21Item,reset22Item,reset23Item,reset24Item
     ,reset25Item,reset26Item,reset27Item,reset28Item,reset29Item
     ,resetRItem
     ,extraviewItem   
     ,readersviewItem 
     ,moreviewItem   
  };
  
  enum    /*MOREVIEWS*/
  { 
      reset30Item = 1
     ,reset31Item,reset32Item,reset33Item,reset34Item,reset35Item
     ,reset36Item,reset37Item,reset38Item,reset39Item,reset40Item
     ,reset41Item,reset42Item,reset43Item,reset44Item,reset45Item
     ,reset46Item,reset47Item,reset48Item,reset49Item,reset50Item
     ,reset51Item,reset52Item,reset53Item,reset54Item,reset55Item
     ,reset56Item,reset57Item,reset58Item,reset59Item,reset60Item
  };
  

  enum    /*Tools*/
  {
      searchItem = 1
     ,findagainItem
     ,measureItem
     ,T1Item
     ,keyempItem
     ,T2Item
     ,remoteupdateItem  /*invoke update by remote program*/
     ,T3Item
     ,xyzpointItem
     ,gnomonItem
     ,singlemarkerItem
     ,rateItem
     ,countItem
     ,dockingscopeItem  /*000814*/
     ,zlimitscopeItem  /*011201*/
     ,finestepItem  /*011201*/
     ,T4Item
     ,kludgeItem
  };      
  enum    /*Empowerments*/
  {
      hidemeasuresItem = 1
     ,brotangleItem
     ,rot90plusItem
     ,rot90minusItem
     ,rot90xplusItem
     ,rot90xminusItem
     ,rot90zplusItem
     ,rot90zminusItem
     ,stereoplusItem
     ,stereominusItem
  };
  enum    /*Kludges*/
  {
      clipmiddleItem = 1  /*051214*/
     ,measperpItem
     ,measuredsurveilItem
     ,formattestItem
     ,testItem
     ,cursorchoiceItem
     ,pickmarkerItem    /*000611*/
     ,visibleoutItem    /*001118*/
     ,rescoringItem     /*020830*/
     ,foocavityItem     /*030331*/
     ,backgroundItem    /*dummy except for UNIX*/
     ,mickeymouseItem   /*010609*/
     ,pdbformatoutItem  /*040208*/
     ,plaidonblkItem    /*010530x*/
     ,plaidonwhtItem    /*010530x*/
     ,plaidchoiceItem   /*010530x*/
  };
  enum   /*IO help menu 020904*/
  {
      writeaboutItem = 1
     ,screenoutItem = 2
  };

  enum    
  {
       secretItem = 1
      ,H1Item
      ,showtextItem
      ,showcaptionItem
      ,showgraphicsItem
      ,showtableItem
      ,showbondrotsItem
      ,H2Item
      ,aboutItem
      ,iohelpItem    /*,writeaboutItem 020904*/   /*"Output Help;"*/
      ,editaboutItem     /*"Edit Help;"*/
      ,optionsaboutItem  /*"Display Help;"*/
      ,specialaboutItem     /*"Special Display Help;"*/
      ,otheraboutItem    /*"Tools Help;"*/
      ,kludgesaboutItem  /*"Kludges Help;"*/
      ,mousehelpItem
      ,dockinghelpItem
      ,resizehelpItem 
      ,tablehelpItem
      ,bondshelpItem
      ,test3DhelpItem
      ,NDIMhelpItem
      ,H3Item
      ,makehelptextItem     /*" Help;"*/
      ,makekinemageItem     /*" Help;"*/
      ,makehelphtmlItem     /*" Help;"*/
      ,dummylasthelpItem /*980712 dummy entry to calc MAGE additions to MAC Help Menu*/
  };


#endif /* MACINTOSH*/
#ifndef UNIX_X11
EXTERNMENU  int theitem,theotheritem;
#endif

#ifdef UNIX_X11
EXTERNMENU  Widget theitem,theotheritem;
EXTERNMENU  Widget openItem,aboutItem,appendItem,importItem,writeItem,quitItem;
EXTERNMENU  Widget editaboutItem,optionsaboutItem,otheraboutItem;
EXTERNMENU  Widget iohelpItem,writeaboutItem,screenoutItem;
EXTERNMENU  Widget writeviewItem,writefocusItem,writedrawnewItem;
EXTERNMENU  Widget stderrfocusItem; /*071201*/
EXTERNMENU  Widget writekinItem,writeptIDItem,writepaletteItem;
EXTERNMENU  Widget writemeansigmaItem; /*060604*/
EXTERNMENU  Widget writerottedItem,writePostScriptItem,writeQAItem,writeR3DItem;
EXTERNMENU  Widget colorshowItem,emphasisItem;
EXTERNMENU  Widget editviewItem,drawlineItem,superpickItem,pickshowItem;
EXTERNMENU  Widget movepointsItem,meansigmasItem; /*061126*/
EXTERNMENU  Widget SELECTINGlistItem; /*060121,060622list*/
EXTERNMENU  Widget SELECTINGcolorItem; /*061030*/
EXTERNMENU  Widget drawsetupItem; /*971129*/
EXTERNMENU  Widget enableTEItem; /*981013*/
EXTERNMENU  Widget pruneItem,showtextItem,showcaptionItem,showgraphicsItem;
EXTERNMENU  Widget showtableItem,showbondrotsItem,showbuttwinItem; /*060613*/
EXTERNMENU  Widget nextItem,chooseItem,scoreNnextItem,scoreNstayItem;
EXTERNMENU  Widget aspectsItem,multiwidthItem;
EXTERNMENU  Widget widthItem,thinItem,depth5Item,depth3Item;
EXTERNMENU  Widget perspecItem,whtbkgItem,B_WItem,grayItem;
EXTERNMENU  Widget aspect0Item,aspect1Item,aspect2Item,aspect3Item,aspect4Item;
EXTERNMENU  Widget aspect5Item,aspect6Item,aspect7Item,aspect8Item,aspect9Item;
EXTERNMENU  Widget aspect10Item,aspect11Item,aspect12Item,aspect13Item;
EXTERNMENU  Widget aspect14Item,aspect15Item,aspect16Item,aspectscrnItem;
EXTERNMENU  Widget reset1Item,reset2Item,reset3Item,reset4Item,reset5Item;
EXTERNMENU  Widget reset6Item,reset7Item,reset8Item,reset9Item,reset10Item;
EXTERNMENU  Widget reset11Item,reset12Item,reset13Item,reset14Item,reset15Item;
EXTERNMENU  Widget reset16Item,reset17Item,reset18Item,reset19Item,reset20Item;
EXTERNMENU  Widget reset21Item,reset22Item,reset23Item,reset24Item,reset25Item;
EXTERNMENU  Widget reset26Item,reset27Item,reset28Item,resetRItem;
EXTERNMENU  Widget reset29Item,reset30Item;
EXTERNMENU  Widget reset31Item,reset32Item,reset33Item,reset34Item,reset35Item;
EXTERNMENU  Widget reset36Item,reset37Item,reset38Item,reset39Item,reset40Item;
EXTERNMENU  Widget reset41Item,reset42Item,reset43Item,reset44Item,reset45Item;
EXTERNMENU  Widget reset46Item,reset47Item,reset48Item,reset49Item,reset50Item;
EXTERNMENU  Widget reset51Item,reset52Item,reset53Item,reset54Item,reset55Item;
EXTERNMENU  Widget reset56Item,reset57Item,reset58Item,reset59Item,reset60Item;
EXTERNMENU  Widget stereoItem,compareItem,listcolorItem,measureItem;
EXTERNMENU  Widget searchItem,readersviewItem,undopickctrItem,kludgeItem;
EXTERNMENU  Widget NDaxesPickedCtrItem; /*070315*/
EXTERNMENU  Widget helpItem,findagainItem; /*971130*/
EXTERNMENU  Widget GFontItem,strokedfontItem;
EXTERNMENU  Widget clipmiddleItem; /*051214*/
EXTERNMENU  Widget measperpItem,formattestItem,sterangItem,detailItem;
EXTERNMENU  Widget measuredsurveilItem;  /*091212*/
EXTERNMENU  Widget lensItem;
EXTERNMENU  Widget xyzpointItem,gnomonItem,singlemarkerItem,fontcursorItem;
EXTERNMENU  Widget brotangleItem,countItem,rateItem,testItem,plotonlyItem;
EXTERNMENU  Widget flatItem,horizItem,dockingscopeItem;
EXTERNMENU  Widget zlimitscopeItem,finestepItem;
EXTERNMENU  Widget pickmarkerItem,rendererItem,renderer2Item,secretItem;
EXTERNMENU  Widget visibleoutItem,rescoringItem,backgroundItem,mickeymouseItem;
EXTERNMENU  Widget buttonwindowItem; /*041108*/
EXTERNMENU  Widget pdbformatoutItem,foocavityItem; /*040208,030330*/
EXTERNMENU  Widget dimensionItem; /*060121*/
EXTERNMENU  Widget allbrsliderItem; /*070617*/
#ifdef EGAMICK
EXTERNMENU  Widget egamickdockingItem,annealmobileItem; /*egam 030215*/
#endif
EXTERNMENU  Widget remoteupdateItem,test3DhelpItem;
EXTERNMENU  Widget mousehelpItem,dockinghelpItem,resizehelpItem,tablehelpItem;
EXTERNMENU  Widget bondshelpItem,NDIMhelpItem; /*130119*/
EXTERNMENU  Widget menuBar,fileButton,writeButton,editButton,windButton;
EXTERNMENU  Widget kineButton;
EXTERNMENU  Widget optiButton,viewButton,otherButton,kludgeButton,iohelpButton;
EXTERNMENU  Widget aspectButton,extraviewsButton,helpButton,moreviewsButton;
EXTERNMENU  Widget kludgesaboutItem,specialaboutItem;
EXTERNMENU  Widget makehelptextItem,makekinemageItem,makehelphtmlItem;
EXTERNMENU  Widget hidemeasuresItem,rot90plusItem,rot90minusItem;  /*971213*/
EXTERNMENU  Widget rot90xplusItem,rot90xminusItem; 
EXTERNMENU  Widget rot90zplusItem,rot90zminusItem;
EXTERNMENU  Widget stereoplusItem,stereominusItem;
EXTERNMENU  Widget tablecutItem,tablerestoreItem,tablepreinsertItem;
EXTERNMENU  Widget tablepostinsertItem,tablerebuildItem,tablecolorItem;

/*UNIX_X11 only prototypes*/
void resizegrafwindow(int , int); /*041031*/
void    setmeasuredsurveillance();  /*091212 MAGEMENU.c, 121108 MAGEMENU.h*/ 

#endif /*UNIX_X11*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*general prototypes*/
void setdockingscope(void);
void setzlimitscope(void);
void copycutline(int Lcut);
void pasteline(void);
void setpdbformat(void);

