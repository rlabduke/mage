/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MACMAIN.C*/ /*MPCMAIN.C*/ /*MUX_MAIN.c*/
void    makecursorwait(void);
void    makecursoractive(void);
void    condition(char[256]);
void    recondition(char[256]);
long    theclocktickcount(void);
  
void    makecursorxyrot(void);  
void    makecursorzrot(void);   
void    makecursorflatland(void);   
void    countredraws(void);     
void    waste(long); /*for number of seconds as originally written*/
void    fingerplacer(int); /*ghost cursor from remote computer */
void    magetimerset(int); /*invokes 1 sec interrupts, only on PC 020830*/
/*MACMAIN.C*/ /*MPCMAIN.C*/ /*MUX_MAIN.c*/

#ifdef UNIX_X11        /*MUX_MAIN.c*/
void    keypresseventhandler(XEvent*);
#endif

#ifdef   MACINTOSH               /*MACMAIN.C*/
  void    main(void);
  void    HandleMouseDown(EventRecord *);
  void    HandleEvent(void);
  void    HandlekeyDown(EventRecord *);
  void    DografContentClick(EventRecord *);
  void    getgrafmousexy(void);
  Boolean mousestilldown(void);
  int     keyanimate();
  void    emptyeventque(void);
#endif /*MACINTOSH*/ /*MACMAIN.C*/


#ifdef   PCWINDOWS               /*MPCMAIN.C*/
  int MainLoop(void);
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
  LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
  LRESULT CALLBACK GraphWndProc(HWND, UINT, WPARAM, LPARAM);
  void WMLButtonDown(LPARAM lParam);
  void WMLButtonUp(void);
  void WMRButtonDown(LPARAM lParam);
  void WMRButtonUp(void);
  void WMMouseMove(LPARAM lParam);
  void ResizeMageGraphics(int width, int height);
  void WMSize(int width, int height);
#endif /*PCWINDOWS*/ /*MPCMAIN.C*/
/*--------------------------------------------------------------------------*/
/*general MAGEINIT.C routines*/
void    getmageversion(void);
void    mageinitialvalues(void);
void    setMageinitDLOGreport(void);
void    clearfocusxyzstr(void);
void    magetimercheck(void);
void    changecheckrouter(void); /*020902*/

void    allocdataarrays(int);
void    allocdisplayarrays(int);
void    allocwordcomarray(int);
void    alloclinkarrays(void);
void    alloccomarray(int);
void    cleanup(void);

#ifdef   MACINTOSH               /*MACINIT.C*/
  void    mainsetup(void);  
  void    Initcomputer(void);
  void    firstLtest(void);
  void    beginnings(void);
  void    SetUpWindowSize(void);  /*980918*/
  void    SetUptextWindow(void);
  void    SetUpcaptWindow(void);
  void    SetUpGrafWindow(void);
  int     SetUpGrafPort(void);     
  int     restoreGrafWindow(void);   
  int     resizeGrafWindow(void);  
  void    definecursors(void);      

#endif /*MACINTOSH*/ /*MACINIT.C*/

#ifdef   PCWINDOWS               /*MPCINIT.C*/
  void    definecursors(HINSTANCE);   /*MPCINIT.C*/
  BOOL    InitInstance(HINSTANCE, int);  /*establishes actual windows*/
#endif /*PCWINDOWS*/ /*MPCINIT.C*/

#ifdef UNIX_MOTIF                /*MUXMINIT.c*/
  void    mainsetup(int *, char **); /*HM950508*/
  void    beginnings(void);
  void    definecursors(void);    
#endif /*UNIX_MOTIF*/
  void changecursors(void); /*uses Lfontcursor*/

/*--------------------------------------------------------------------------*/
#ifdef   MACINTOSH               /*MACFILE.C*/
  void    newfile(void);
  int     getafile(void);
  int     ifilelaunched(void);
  void    getnamestroffile(void);

  /* Apple Event Handler Prototypes! */
  pascal OSErr HandleAEOpenDoc( AppleEvent *, AppleEvent *, long);
  pascal OSErr HandleAEQuitApp( AppleEvent *, AppleEvent *, long);
  pascal OSErr HandleAEIgnore(  AppleEvent *, AppleEvent *, long);
  
  static void ConvertFilename(FSSpec *fss); /*RasMol, Roger Sayle routine*/
  int    openfilebyname(char[256]);  /* MACFILE.C*/ 
   
#endif /*MACINTOSH*/ /*MACFILE.C*/

#ifdef UNIX_X11                 /* MUXMFILE.C*/ 
  int    openfilebyname(char[256]);  
#endif /*UNIX_X11*/ /*MUXMFILE.C*/

#ifdef   PCWINDOWS               /*MPCFILE.C*/
  long    filesize(LPSTR);
  int     findkin(void);
  void    openfilebyname(LPSTR);
#endif /*PCWINDOWS*/ /*MPCFILE.C*/

/*general MAGEFILE.C routines*/
void    GetTextFromFile(void);
void    getnextkinemage(void);
int     getkinemagenum(char,int);
int     kinesearch(void);
void    findkinemage(void);
void    Dotexthyperbutton(int); 
void    gographics(void);  
void    entry(void);
void    filerewind(FILE*);

/*general ____FILE.C routines*/
void    OpenNewFile(void);
void    FindFileName(void); /*030312*/
  
/*--------------------------------------------------------------------------*/
/*____WIND.c routines*/
  void    gettablewinedges(int*, int*, int*, int*);
#ifdef   MACINTOSH               /*MACWIND.C*/  
  void    showallwindows(void);
  void    choosefrontwindow(void);
  int     IsColor(void);
  void    getdeviceattributes(int *,int *);
  long    getfreememory(void);
  void   growawindow(WindowPtr,Point);
  pascal void    textScroller(ControlHandle,short);
  pascal void    captScroller(ControlHandle,short);
  void    DotextContentClick(Point);
  void    DocaptContentClick(Point); /*040306 UNIX new, MAC old & not updated*/

/*  void    writefirsttextword(void);         */   /*????*/
/*  void    writefirstcaptkinename(char[256]);*/   /*????*/
#endif /*MACINTOSH*/ /*MACWIND.C*/

#ifdef UNIX_X11
  void    DotextContentClick(long);
  void    DocaptContentClick(long); /*040306 UNIX new, MAC old & not updated*/
  long    Dohypertextfind(char[256]); /*040306 new for unix hyperhypertext*/
  void    showallwindows(void);    /*121107 compiler wants this*/
  void    choosefrontwindow(void); /*121107 compiler wants this*/

#endif /*UNIX_X11*/

void    cleartexts(void);        /*MACWIND.C*/  /*MPCWIND.C*/
void    inserttexts(char[256]);  /*MACWIND.C*/  /*MPCWIND.C*/
void    completetext(void);      /*MACWIND.C*/  /*MPCWIND.C*/
void    clearcapts(void);        /*MACWIND.C*/  /*MPCWIND.C*/
void    insertcapts(char[256]);  /*MACWIND.C*/  /*MPCWIND.C*/
void    docompletecapt();        /*MACWIND.C*/  /*MPCWIND.C*/ 
void    adjustwindowwidth(void); /*MACWIND.C*/  /*MPCWIND.C*/
void    destroybondrotwindow(void); /*MUXMWIND.c 031001*/
void    createbuttonwindow(void);   /*MUXMWIND.c 041108*/
void    destroybuttonwindow(void);  /*MUXMWIND.c 041108*/

#ifdef   PCWINDOWS               /*MPCWIND.C*/
  void   textbufclear(HWND);
  void   textbufinsert(HWND, char[256]);
  long   totalbufferlines(HWND, HDC, LPSTR);
  long   linefromoffset(HWND, unsigned long *, long);
  long   makelinestarts(HWND);
  static int   linesonascreen(HWND);
  void   resetscrollbarvalues(HWND);
  HWND   NewTextWindow(HWND,int,int,int,int, int, int);
  void   SetTextCaption(HWND, LPSTR);
  void   SetMageCaption(void);
  int    RePaint(HWND, HDC);
  int    nlinebreaks(LPSTR, LPSTR *);
  int    sizeoftextline(HDC, int, char *);
  LRESULT CALLBACK MAGETextWndProc(HWND, UINT, WPARAM, LPARAM);
  /*dummies for Mac routines*/
  void    showallwindows(void);
  void    choosefrontwindow(void);
                                   /*MPCTABL.c*/
  LRESULT CALLBACK MAGETableWndProc(HWND, UINT, WPARAM, LPARAM);

#endif /*PCWINDOWS*/ /*MPCWIND.C*/

/*--------------------------------------------------------------------------*/
/*MAGECOLR.C routines*/
int     numberofcolor(char[256]);
int     decodecolor(char[256],int);
void    interpretcolor(int);
char    colorcode(int);
void    loadmyfullpalette(int);
void    loadmygrayscalepalette(void); /*041001*/

  
#ifdef   MACINTOSH               /*MACCOLR.C*/
  PaletteHandle    makePalette(void);
  PaletteHandle    makegrayPalette(void);
  void    finddisplaytype(void); /*UNIX_X11 -> MUX_COLR.c*/
#endif /*MACINTOSH*/ /*MACCOLR.C*/

#ifdef   PCWINDOWS               /*MPCCOLR.C*/
  void    setpalette(HDC);
#endif /*PCWINDOWS*/ /*MPCCOLR.C*/ 

#ifdef UNIX_X11
void    finddisplaytype(void); /*UNIX_X11 -> MUX_COLR.c*/ /*MACINIT.C*/
void    makexcolors(void); /*MUX_COLR.c*/
int     makegrayxcolor(int); /*040926*/
#endif /*UNIX_X11*/

/*--------------------------------------------------------------------------*/  
/*MAGEBBOX.C , ____BBOX routines moved to MAGEBBOX.H*/

/*--------------------------------------------------------------------------*/
                       /*M___DLOG.C*/ 
void    DoMageDLOGreport(int);     
void    DoMageinitDLOGreport(void);
void    DoconstructDialog(void);
void    DoQuesDialog(void);     /*common name, all platforms*/
void    DopickshowDialog(void); 
void    DopickpointsDialog(void); /*movepoints,meansigmas  061129*/
void    DocolorshowDialog(void);
void    DopickcolorDialog(int,int);
void    DopalettenameDialog(void);
void    DogenericDialog(void);

#ifndef PCWINDOWS
void    genericDialog(void);
#endif /*!PCWINDOWS*/

#ifdef UNIX_X11
void    MageDLOGreport(void);
void    MageinitDLOGreport(void);
XtCallbackProc DLOG_PARAMA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_PARAMB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_PARAMC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_PARAMD_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_PARAME_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_PARAMF_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKD_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKE_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CHECKF_CB(Widget, XtPointer, XtPointer);

XtCallbackProc DLOG_RADIOA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOD_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOE_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOF_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOG_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOH_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOI_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOJ_CB(Widget, XtPointer, XtPointer);
void  clearradioA_J(void);

XtCallbackProc DLOG_RADIOK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOL_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOM_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADION_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOO_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOP_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOQ_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOR_CB(Widget, XtPointer, XtPointer);
void  clearradioK_R(void);

XtCallbackProc DLOG_RADIOS_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOT_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOU_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOV_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOW_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOX_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOY_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_RADIOZ_CB(Widget, XtPointer, XtPointer);
void  clearradioS_Z(void);

XtCallbackProc DLOG_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_OPTION_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_CANCEL_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_EXTRA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_EXTRA2_CB(Widget, XtPointer, XtPointer);
XtCallbackProc DLOG_INFO_CB(Widget, XtPointer, XtPointer);

XtCallbackProc listboxA_CB(Widget, XtPointer, XtPointer); /*060619*/
XtCallbackProc listboxB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc listboxC_CB(Widget, XtPointer, XtPointer);

void           DoOpenFileDialog(void);

void           genericdlogrouter(void);
void           constructDialog(void);   /*same name: MPCDLOG.C, MACDLOG.C*/
/*Linux Lesstif seems to get in trouble if more than one widget*/
/*uses the same Callback Procedure: radio, distance, angle, dihedral*/
XtCallbackProc construct_A_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructxA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructyA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructzA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc construct_B_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructxB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructyB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructzB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc construct_C_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructxC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructyC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc constructzC_CB(Widget, XtPointer, XtPointer);
XtCallbackProc construct_D_CB(Widget, XtPointer, XtPointer); /*971122*/
XtCallbackProc construct_E_CB(Widget, XtPointer, XtPointer); /*971122*/
XtCallbackProc construct_F_CB(Widget, XtPointer, XtPointer); /*971122*/
XtCallbackProc construct_G_CB(Widget, XtPointer, XtPointer); /*971126*/
XtCallbackProc construct_H_CB(Widget, XtPointer, XtPointer); /*011007*/
XtCallbackProc construct_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc construct_CANCEL_CB(Widget, XtPointer, XtPointer);
void constructsetstate(void);
void colorshowDialog(void); /*same name: MPCDLOG.C, MACDLOG.C*/
XtCallbackProc colorshow_CB(Widget, XtPointer, XtPointer);
XtCallbackProc colorshow_RADIOA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc colorshow_RADIOB_CB(Widget, XtPointer, XtPointer);
XtCallbackProc colorshow_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc colorshow_CANCEL_CB(Widget, XtPointer, XtPointer);
void DoSELECTINGcolorDialog(void); /*061030*/
void SELECTINGcolorDialog(void); /*MUXMDLOG.c 061030*/
XtCallbackProc SELECTINGcolorOK_CB(Widget, XtPointer, XtPointer); /*061030*/
XtCallbackProc SELECTINGcolor_CANCEL_CB(Widget, XtPointer, XtPointer);/*061030*/
void pickshowDialog(void); /*same name: MPCDLOG.C, MACDLOG.C*/
XtCallbackProc pickshow_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pickshow_CANCEL_CB(Widget, XtPointer, XtPointer);
void pickpointsDialog(int); /*061129*/
XtCallbackProc pickshow_SOURCE_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_TARGET_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_NEWTARGET_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_MOVE_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_MEANSIGMAS_CB(Widget, XtPointer, XtPointer); /*061126*/
XtCallbackProc pickshow_RADIOA_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_RADIOB_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_RADIOC_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_RADIOD_CB(Widget, XtPointer, XtPointer); /*061124*/
XtCallbackProc pickshow_EDITactive_CB(Widget, XtPointer, XtPointer); /*061126*/

void questionDialog(void);
XtCallbackProc question_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc question_CANCEL_CB(Widget, XtPointer, XtPointer);
XtCallbackProc question_GOGRAPH_CB(Widget, XtPointer, XtPointer);
#endif

#ifdef   MACINTOSH               /*MACDLOG.C*/
  void    MageDLOGreport(int);/*970411*/
  void    MageinitDLOGreport(void);
  void    DrawOKButton(DialogPtr);
  void    constructDialog(void);   /*same name: MPCDLOG.C*/
  void    MAXsizeDialog(void);     /*same name: MPCDLOG.C*/
  void    QuesDialog(void);        /*same name: MPCDLOG.C*/
  void    pickshowDialog(void);    /*same name: MPCDLOG.C*/
  void    colorshowDialog(void);   /*same name: MPCDLOG.C*/
  void    pickcolorDialog(int,int);
  void    palettenameDialog(void);
  void    genericDialog(void);     /*same name: MPCDLOG.C*/

  pascal Boolean    pickshowDFilter(DialogPtr,EventRecord *,int *);
  pascal Boolean    colorshowDFilter(DialogPtr,EventRecord *,int *);
  pascal Boolean    pickcolorDFilter(DialogPtr,EventRecord *,int *);
#endif /*MACINTOSH*/ /*MACDLOG.C*/  

#ifdef   PCWINDOWS               /*MPCDLOG.C*/
  BOOL CALLBACK constructDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK INITIALDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK AlertDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK QuesDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK colorshowDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK pickshowDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK pickpointsDialog(HWND, UINT, WPARAM, LPARAM);
  BOOL CALLBACK genericDialog(HWND, UINT, WPARAM, LPARAM);
void  clearradioA_J(HWND);
void  clearradioK_R(HWND);
void  clearradioS_Z(HWND);
#endif /*PCWINDOWS*/ /*MPCDLOG.C*/

void      resetmagepalette(void); /*010604*/
                                    
                          /* MAGEDLOG */
void    cleargenericflags(void);
void    dosinglealert(int);
void	colorshow_update(int, int);
void    DokineDialog(void);
void    kineDialog(void);
void    EndkineDialog(void);
void    DostereoDialog(void);
void    stereoDialog(void);
void    EndstereoDialog(void);
void    DodrawlineDialog(void);
void    drawlineDialog(void);
void    EnddrawlineDialog(void);

void    DoConstructFifthDialog(void);  /*971122*/
void    ConstructFifthDialog(void);    /*971122*/
void    EndConstructFifthDialog(void); /*971122*/

void    DoConstructSixthDialog(void);  /*140912*/
void    ConstructSixthDialog(void);    /*140912*/
void    EndConstructSixthDialog(void); /*140912*/

void    DoSearchDialog(void);
void    SearchDialog(void);
void    EndSearchDialog(void);
void    DoViewsDialog(int);
void    ViewsDialog(int);
void    EndViewsDialog(void);
void    DoChangeCheckDialog(void);
void    ChangeCheckDialog(void);
void    EndChangeCheckDialog(void);
void    DoSetgraffontsDialog(void);
void    SetgraffontsDialog(void);
void    EndSetgraffontsDialog(void);
void    DoKipOutputDialog(void);
void    KipOutputDialog(void);
void    EndKipOutputDialog(void);
void    DoPostScriptDialog(void);
void    PostScriptDialog(void);
void    EndPostScriptDialog(void);
void    DoRaster3DDialog(void);
void    Raster3DDialog(void);
void    EndRaster3DDialog(void);
void    DocoordformatDialog(void);
void    coordformatDialog(void);
void    EndcoordformatDialog(void);
void    DodetailDialog(void);
void    detailDialog(void);
void    EnddetailDialog(void);
void    DousernameDialog(void);
void    usernameDialog(void);
void    EndusernameDialog(void);
void    DofilenameDialog(void);
void    filenameDialog(void);
void    EndfilenameDialog(void);
void    DofinestepDialog(void);
void    finestepDialog(void);
void    EndfinestepDialog(void);
void    DoimportDialog(void);
void    importDialog(void);
void    EndimportDialog(void);
void    DoupdateDialog(void);
void    updateDialog(void);
void    EndupdateDialog(void);
void    DoupdateProbeDialog(int);
void    updateProbeDialog(int);
void    EndupdateProbeDialog(void);

int     getrotationfocus(float*,float*,float*,float*);

void    DoupdatePrekinDialog(int);
void    updatePrekinDialog(int);
void    EndupdatePrekinDialog(void);
void    DoupdatePipeDialog(int);
void    updatePipeDialog(int);
void    EndupdatePipeDialog(void);
void    DoupdateSocketDialog(int);
void    updateSocketDialog(int);
void    EndupdateSocketDialog(void);
void    DoupdateContoursDialog(int);
void    updateContoursDialog(int);
void    EndupdateContoursDialog(void);

void    DoupdateNOEsDialog(int);   /*030928*/
void    updateNOEsDialog(int);     /*030928*/
void    EndupdateNOEsDialog(void); /*030928*/

void    DopickmarkerDialog(void);   /*000611*/
void    pickmarkerDialog(void);     /*000611*/
void    EndpickmarkerDialog(void);  /*000611*/

void    DoAdvancedHelpDialog(int);   /*000606*/
void    AdvancedHelpDialog(int);     /*000606*/
void    EndAdvancedHelpDialog(int);  /*000606*/

void    DobackgroundDialog(void);   /*010408*/
void    backgroundDialog(void);     /*010408*/
void    EndbackgroundDialog(void);  /*010408*/

void    DomickeymouseDialog(void);   /*010609*/
void    mickeymouseDialog(void);     /*010609*/
void    EndmickeymouseDialog(void);  /*010609*/

void    DofoocavityDialog(void);   /*030331*/
void    foocavityDialog(void);     /*030331*/
void    EndfoocavityDialog(void);  /*030331*/

void    defaultdimnames(void);     /*060622*/
void    DodimensionDialog(void);   /*060121*/
void    dimensionDialog(void);     /*060121*/
void    EnddimensionDialog(void);  /*060121*/

void    DoSELECTINGlistDialog(void);   /*060121,060622list*/
void    SELECTINGlistDialog(void);     /*060121*/
void    EndSELECTINGlistDialog(void);  /*060121*/
void    adjustSELECTINGlist(int);      /*060121*/
void    actualSELECTINGtask(void);     /*061029*/

void    createinternalkinemage(int); /*020514*/

void    DonextanotherDialog(void);   /*030306*/

/*do not declare   nextanotherDialog(void) here,  ____DLOG.c unique 030306*/
/*--------------------------------------------------------------------------*/
/*___MENU.C  */
void    resetmenuchecks(void);
void    checkcurrentviewmenu(int);
void    quitMAGE(void);        /*971129*/
void    DoSearchAgain(void);   /*971204*/
void    doresize(void); /*040509*/

#ifndef PCWINDOWS
void    trytoquitMAGE(void);   /*981013*/
#endif

#ifndef UNIX_X11
void    mycheckitem(MenuHandle,int,int);
void    myenableitem(MenuHandle,int,int);
void    mymodifymenuitem(MenuHandle,int,char[256]);
void    mylabelbutton(int, char[16]);
#endif /*!UNIX_X11*/

#ifdef UNIX_X11
void    mycheckitem(MenuHandle,Widget,int);
void    myenableitem(MenuHandle,Widget,int);
void    mymodifymenuitem(MenuHandle,Widget,char[256]);
void    mylabelbutton(Widget, char[16]);
#endif /*UNIX_X11*/


#ifdef   MACINTOSH               /*MACMENU.C*/
  void    SetUpMenus(void);
  void    enable(MenuHandle,int,int);
  void    HandleMenu(long);
  void    setB_Wscrn(void);
#endif /*MACINTOSH*/ /*MACMENU.C*/

#ifdef   PCWINDOWS               /*MPCMENU.C*/
  void    checkamenuitem(int,int,int);
#endif /*PCWINDOWS*/ /*MPCMENU.C*/

#ifdef  UNIX_X11
  void showthegrafwindow(void); /*MUX_MENU.c*/
  void motion_handler();
  void Rmotion_handler(); /*030405*/
  void pointer_handler();
#endif /*UNIX_X11*/

#ifdef  UNIX_MOTIF
/* motif dependent routines in MUX_MENU.c */
XtCallbackProc fileok_CB(Widget, XtPointer, XtPointer);
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XtCallbackProc *call_data; */
XtCallbackProc reportOK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc filecancel_CB(Widget, XtPointer, XtPointer);
XtCallbackProc open_CB(Widget, XtPointer, XtPointer);
XtCallbackProc append_CB(Widget, XtPointer, XtPointer);
XtCallbackProc import_CB(Widget, XtPointer, XtPointer); /*020514*/
XtCallbackProc writeabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc screenout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writeview_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writefocus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc stderrfocus_CB(Widget, XtPointer, XtPointer); /*071201*/
XtCallbackProc writedrawnew_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writekin_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writerotted_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writePostScript_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writeQA_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writeR3D_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writeptID_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writepalette_CB(Widget, XtPointer, XtPointer);
XtCallbackProc writemeansigma_CB(Widget, XtPointer, XtPointer); /*060604*/
XtCallbackProc quit_CB(Widget, XtPointer, XtPointer);
XtCallbackProc OKalert_CB(Widget, XtPointer, XtPointer);
XtCallbackProc alert_CB(Widget, XtPointer, XtPointer);
XtCallbackProc option_CB(Widget, XtPointer, XtPointer);
XtCallbackProc dummy_CB(Widget, XtPointer, XtPointer);
XtCallbackProc editabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc editview_CB(Widget, XtPointer, XtPointer);
XtCallbackProc drawline_CB(Widget, XtPointer, XtPointer);
XtCallbackProc drawsetup_CB(Widget, XtPointer, XtPointer); /*971129*/
XtCallbackProc superpick_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pickcolor_CB(Widget, XtPointer, XtPointer);
XtCallbackProc emphasis_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pickshow_CB(Widget, XtPointer, XtPointer);
XtCallbackProc movepoints_CB(Widget, XtPointer, XtPointer); /*061126*/
XtCallbackProc meansigmas_CB(Widget, XtPointer, XtPointer); /*061126*/
XtCallbackProc SELECTINGlist_CB(Widget, XtPointer, XtPointer); /*060121,060622*/
XtCallbackProc SELECTINGcolor_CB(Widget, XtPointer, XtPointer); /*061030*/
XtCallbackProc enableTE_CB(Widget, XtPointer, XtPointer); /*981005*/
XtCallbackProc prune_CB(Widget, XtPointer, XtPointer);
XtCallbackProc showtext_CB(Widget, XtPointer, XtPointer);
XtCallbackProc showcaption_CB(Widget, XtPointer, XtPointer);
XtCallbackProc showgraphics_CB(Widget, XtPointer, XtPointer);
XtCallbackProc showbuttonwindow_CB(Widget, XtPointer, XtPointer); /*060613*/
XtCallbackProc showtable_CB(Widget, XtPointer, XtPointer);
XtCallbackProc showbondrots_CB(Widget, XtPointer, XtPointer);
XtCallbackProc next_CB(Widget, XtPointer, XtPointer);
XtCallbackProc choose_CB(Widget, XtPointer, XtPointer);
XtCallbackProc scoreNnext_CB(Widget, XtPointer, XtPointer);
XtCallbackProc scoreNstay_CB(Widget, XtPointer, XtPointer);
XtCallbackProc optionsabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc width_CB(Widget, XtPointer, XtPointer);
XtCallbackProc multiwidth_CB(Widget, XtPointer, XtPointer);
XtCallbackProc thin_CB(Widget, XtPointer, XtPointer);
XtCallbackProc depth5_CB(Widget, XtPointer, XtPointer);
XtCallbackProc depth3_CB(Widget, XtPointer, XtPointer);
XtCallbackProc perspec_CB(Widget, XtPointer, XtPointer);
XtCallbackProc whtbkg_CB(Widget, XtPointer, XtPointer);
XtCallbackProc B_W_CB(Widget, XtPointer, XtPointer);
XtCallbackProc gray_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect0_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect1_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect2_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect3_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect4_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect5_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect6_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect7_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect8_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect9_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect10_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect11_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect12_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect13_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect14_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect15_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspect16_CB(Widget, XtPointer, XtPointer);
XtCallbackProc aspectscrn_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view1_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view2_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view3_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view4_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view5_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view6_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view7_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view8_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view9_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view10_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view11_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view12_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view13_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view14_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view15_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view16_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view17_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view18_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view19_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view20_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view21_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view22_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view23_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view24_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view25_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view26_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view27_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view28_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view29_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view30_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view31_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view32_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view33_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view34_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view35_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view36_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view37_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view38_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view39_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view40_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view41_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view42_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view43_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view44_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view45_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view46_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view47_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view48_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view49_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view40_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view41_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view42_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view43_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view44_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view45_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view46_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view47_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view48_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view49_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view50_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view51_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view52_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view53_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view54_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view55_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view56_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view57_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view58_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view59_CB(Widget, XtPointer, XtPointer);
XtCallbackProc view60_CB(Widget, XtPointer, XtPointer);
XtCallbackProc viewR_CB(Widget, XtPointer, XtPointer);
void           processView(int);
XtCallbackProc otherabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc stereo_CB(Widget, XtPointer, XtPointer);
XtCallbackProc compare_CB(Widget, XtPointer, XtPointer);
XtCallbackProc listcolor_CB(Widget, XtPointer, XtPointer);
XtCallbackProc measure_CB(Widget, XtPointer, XtPointer);
XtCallbackProc search_CB(Widget, XtPointer, XtPointer);
XtCallbackProc findagain_CB(Widget, XtPointer, XtPointer);
XtCallbackProc readersview_CB(Widget, XtPointer, XtPointer);
XtCallbackProc NDaxesPickedCtr_CB(Widget, XtPointer, XtPointer); /*070315*/
XtCallbackProc undopickctr_CB(Widget, XtPointer, XtPointer);
XtCallbackProc kludgesabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc GFont_CB(Widget, XtPointer, XtPointer);
XtCallbackProc strokedfont_CB(Widget, XtPointer, XtPointer);
XtCallbackProc sterang_CB(Widget, XtPointer, XtPointer);
XtCallbackProc detail_CB(Widget, XtPointer, XtPointer);
XtCallbackProc lens_CB(Widget, XtPointer, XtPointer);
XtCallbackProc xyzpoint_CB(Widget, XtPointer, XtPointer);
XtCallbackProc gnomon_CB(Widget, XtPointer, XtPointer);
XtCallbackProc singlemarker_CB(Widget, XtPointer, XtPointer);
XtCallbackProc hidemeasures_CB(Widget, XtPointer, XtPointer);
XtCallbackProc brotangle_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90plus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90minus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90xplus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90xminus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90zplus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rot90zminus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc stereoplus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc stereominus_CB(Widget, XtPointer, XtPointer);
XtCallbackProc count_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rate_CB(Widget, XtPointer, XtPointer);
XtCallbackProc clipmiddle_CB(Widget, XtPointer, XtPointer); /*051214*/
XtCallbackProc measperp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc measuredsurveil_CB(Widget, XtPointer, XtPointer); /*091212*/
XtCallbackProc formattest_CB(Widget, XtPointer, XtPointer);
XtCallbackProc test_CB(Widget, XtPointer, XtPointer);
XtCallbackProc secret_CB(Widget, XtPointer, XtPointer);
XtCallbackProc plotonly_CB(Widget, XtPointer, XtPointer);
XtCallbackProc fontcursor_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablebutton_CB(Widget, XtPointer, XtPointer);
XtCallbackProc flat_CB(Widget, XtPointer, XtPointer);
XtCallbackProc horizon_CB(Widget, XtPointer, XtPointer);
XtCallbackProc dockingscope_CB(Widget, XtPointer, XtPointer);
XtCallbackProc zlimitscope_CB(Widget, XtPointer, XtPointer);
XtCallbackProc finestep_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pickmarker_CB(Widget, XtPointer, XtPointer);
XtCallbackProc visibleout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc rescoring_CB(Widget, XtPointer, XtPointer);
XtCallbackProc background_CB(Widget, XtPointer, XtPointer);
XtCallbackProc buttonwindow_CB(Widget, XtPointer, XtPointer); /*041108*/
XtCallbackProc foocavity_CB(Widget, XtPointer, XtPointer);
XtCallbackProc allbrslider_CB(Widget, XtPointer, XtPointer); /*070617*/
XtCallbackProc mickeymouse_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pdbformatout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc dimension_CB(Widget, XtPointer, XtPointer); /*060121*/
XtCallbackProc egamickdocking_CB(Widget, XtPointer, XtPointer); /*egam 030215*/
XtCallbackProc annealmobile_CB(Widget, XtPointer, XtPointer); /*egam 030215*/
XtCallbackProc remoteupdate_CB(Widget, XtPointer, XtPointer);
XtCallbackProc specialabout_CB(Widget, XtPointer, XtPointer);
XtCallbackProc mousehelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc dockinghelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc resizehelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablehelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc bondshelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc NDIMhelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc test3Dhelp_CB(Widget, XtPointer, XtPointer);
XtCallbackProc makehelptext_CB(Widget, XtPointer, XtPointer);
XtCallbackProc makekinemage_CB(Widget, XtPointer, XtPointer);
XtCallbackProc makehelphtml_CB(Widget, XtPointer, XtPointer);
XtCallbackProc drawArea_CB(Widget, XtPointer, XtPointer);
XtCallbackProc resize_CB(Widget, XtPointer, XtPointer);
XtCallbackProc pick_CB(Widget, caddr_t, XmDrawingAreaCallbackStruct *);
XtCallbackProc textcursor_CB(Widget, XtPointer, XtPointer);
XtCallbackProc captcursor_CB(Widget, XtPointer, XtPointer); /*040306*/
XtCallbackProc kinecancel_CB(Widget, XtPointer, XtPointer);
XtCallbackProc kinenext_CB(Widget, XtPointer, XtPointer);
XtCallbackProc kineOK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc kinenumber_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablecut_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablerestore_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablepreinsert_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablepostinsert_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablerebuild_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tablecolor_CB(Widget, XtPointer, XtPointer);
XtTimerCallbackProc magedelayresize(XtPointer); /*040523*/
void resizegrafwindowgently(void); /*060613*/
#endif  /*UNIX_MOTIF*/

/*--------------------------------------------------------------------------*/
/*MACBAR.C and MPCBAR.C and MUXMBAR.c*/
void    creategrafbondrotBar(int);
void    resetgrafbondrotBar(int);
void    writebondrotvalue(int);
void    erasebondrotvalue(int);
void    dobondrot(int,float);
void    resetgrafzoomBar(void);
void    resetgrafzclipBar(int);
void    resetgrafztranBar(int);
void    erasezoomvalue(void);
void    erasezclipvalue(void);
void    eraseztranvalue(void);   /*970905*/
void    recreatesliderBars(void);
void    makegrafbondrotbar(ControlHandle *,int,int,int,int,int,int);/*981013*/
/*970407*/

#ifdef UNEMPLOYED
  void    resetgrafzoomBar(ControlHandle *,int);
  void    resetgrafzclipBar(ControlHandle *,int);
  void    resetgrafztranBar(ControlHandle *,int);
#endif

#ifdef UNIX_MOTIF             /*MUXMBAR.c*/
XtCallbackProc zoom_CB(Widget, XtPointer, XtPointer);
XtCallbackProc zclip_CB(Widget, XtPointer, XtPointer);
XtCallbackProc ztran_CB(Widget, XtPointer, XtPointer);
XtCallbackProc bondrot_CB(Widget, XtPointer, XtPointer);
void    makegrafcntllabel(Widget *, Widget *);
void    makegrafcntltitle(Widget *,int,int,int,int, char*);
void    writebondrotbarvalue(Widget *);
void    writecntlbarvalue(Widget *);
void    makebondrotlabels(int);
void    makebondrotlabelwidget(Widget *, Widget *, int, int, int, int);
void    makebondrotscopewidget(Widget *, Widget *, int, int, int, int, int);
void    makebondrotdividewidget(Widget *, Widget *, int, int, int, int);
XtCallbackProc bondrotpattern_CB(Widget, XtPointer, XtPointer);
void DrawPattern(Display* , Window , GC , int , int , int);

#endif /*UNIX_MOTIF*/

#ifndef   PCWINDOWS               /*MACBAR.C*/ /*MUXMBAR.c*/
  void    creategrafzoomBar(void);
  void    writezoomvalue(void);
  void    creategrafzclipBar(void);
  void    writezclipvalue(int);
  void    creategrafztranBar(void);
  void    writeztranvalue(int); /*970905*/
  void    makegrafcntlbar(ControlHandle *,int,int,int,int,int,int);/*981013*/
  void    hilitegrafcntlbar(ControlHandle *);
  void    resetgrafcntlbar(ControlHandle *,int);
  void    resetgrafbondrotcntlbar(ControlHandle *,int);
  void    getgrafwinedges(int *,int *,int *,int *);
#endif

#ifdef   MACINTOSH               /*MACBAR.C*/
  void    writecntlbarvalue(int,int,int,int);
void    writebondrotbarvalue(int,int,int,int);
  pascal void    grafzoomer(ControlHandle,short);
  pascal void    grafzcliper(ControlHandle,short);
  pascal void    grafztraner(ControlHandle,short);
 int myscrolltarget(int, int, int, int,ControlHandle, short, int*); /*981010*/

  pascal void    grafbondrotter(ControlHandle,short); 

  void    settextScrollBar(void);
  void    setcaptScrollBar(void);

  /*MACWIND.c*/
  void    writespecialmark(int,int,int,int);

  void    gettextTextarea(void);
  void    getcaptTextarea(void);
  void    settextTextarea(void);
  void    setcaptTextarea(void);
  void    captupdate(void);
  void    textupdate(void);
#endif /*MACINTOSH*/   

#ifdef   PCWINDOWS               /*MPCBAR.C*/
  void	systemfontsize(void);
  typedef double (*sliderfunc)(double);
  static int AddSliderScrollBar(HWND,int,RECT *,int,int,int,int,int,sliderfunc);
  static int AddHScrollBar(HWND,int,RECT*,int,int,int,int);
  void NewSliderWindows(void);
  HWND NewScrollWindow(HWND parent, int id, int left, int top, int right,
	 int bottom, int low, int high, int start, int page, int titleid,
	 sliderfunc func);
  HWND NewHScrollWindow(HWND parent, int id, int left, int top, int right,
     int bottom, int low, int high, int start, int titleid, sliderfunc func);
  LRESULT CALLBACK ScrollWndProc(HWND, UINT, WPARAM, LPARAM);
  LRESULT CALLBACK HScrollWndProc(HWND, UINT, WPARAM, LPARAM);
#endif /*PCWINDOWS*/ /*MPCBAR.C*/

/*--------------------------------------------------------------------------*/
/*MAGEDRAW.C routines*/
void    drawvec(void);

/*MAGEUTIL.C*/
float   floatfromstr(char[256]);
int     intfromstr(char[256]);
void    doundoprune(void);
int     mysearchpointID(int);
int     matchstrings(char[256],char[256]); /*000411*/
void    SearchAgain(void);      /*971204*/
void    uctolcstr(char[256]);
void    consoleshowbits(unsigned); /*980804*/
void    checksideclip(int);
void    checkmiddleclip(int,int); /*051214*/
void    checkzclip(int,int,int,int,int,int,int,int);
void    getrot(int,int,int); /*delx,dley,mode 031127*/

/*MAGEPOST*/ /*defines own prototypes, and put in MAGELIST.h*/

/*____DRAW.C */
void    redrawvec(void); /*real MAC, dummy MPC */
void    myGetEntryColor(int);
void    stringtoscreen(char[256],int,int);
void    labeltoscreen(char[256],int,int,int);
void    killzoneonscreen(void); /*____DRAW.c*/
void    SELECTINGzoneonscreen(void); /*MUXMDRAW.c  060123*/
void    wordstoscreen(char[256],int,int,int);
void    balltoscreen(int,int,int,int,int,int,int,int,int);/*TBstereo_balltoscreen*/
void    setdrawcolorpen(int,int,int);
void    setdrawcolorpensharp(int,int,int); /*030626*/
void    triangletoscreen(int,int,int,int,int,int,int);
void    linetoscreen(int,int,int,int);
void    ellipsoidtoscreen(int,int,int,int,int,int,int,int,void*); /*080406*/
void    emphasistoscreen(int,int,int,int,int,int,int,int,void*); /*020112xrc*/
void    arrowtoscreen(int,int,int,int,int,int,void*);
void    marktoscreen(int,int,int,int,int);
void    ringtoscreen(int,int,int,int,int);
void    diamondtoscreen(int,int,int,int,int);  /*010916*/
void    squaretoscreen(int,int,int,int,int);  /*010916*/
/*void    fantoscreen(int,int,int,int,int,struct pointstruct*);*/ /*--> MAGELIST.h*/

#ifdef   UNIX_MOTIF
void    setpencolor(int);
void    setpentoscreenforeground(void); /*040926*/
void    setpentohighlight(void); /*040926*/
void    copytoscreen(void); /*121108 needed by MUXMMAIN.c as well as MUXMDRAW*/
#endif

#ifdef   MACINTOSH               /*MACDRAW.C*/
    void    Drawgraf(void);  
    void    offscreenclear(void);
    void    preparegrafwindow(void);
    void    copytoscreen(void);
#endif /*MACINTOSH*/ /*MACDRAW.C*/

#ifdef   PCWINDOWS               /*MPCDRAW.C*/
    void    paintaux(HDC);
    void    makenewDC(void);
    void    destroynewDC(void);
    void    setdrawcolorbrush(int,int,int);  /*PC:balltoscreen()*/
#endif /*PCWINDOWS*/ /*MPCDRAW.C*/
/*--------------------------------------------------------------------------*/

/*MAGEOUT.C routines*/
void commandfrompointID(void);
void   writematrix(void);
void   writefocus(void);
void   stderrfocus(void); /*071201*/
void   writerotated(void);
int    writepruned(void);
int    writepostscript(void);     /*MAGEPOST*/
int    writeraster3Drender(void); /*MAGEREND*/
int    writedrawnew(void);
void   writeoutput(int);         /*M___OUT.C*/  
int    writetext(void);          /*M___OUT.C*/
int    writecaption(void);       /*M___OUT.C*/
int    getcaptiontop(char*, int); /*M___OUT.C 020914*/
int    writequesanswers(void);   /*M___OUT.C*/
void   clearwriteconditions(void);
int    isahydrogen(char atmname[5]); /*070829 for writerotated hydrogen choice*/

#ifdef   PCWINDOWS			/*MPCOUT.C*/
void   writerotatedPC(HANDLE write);
void   writeindividuallistPC(HANDLE write);
#endif

#ifdef UNIX_MOTIF
XtCallbackProc fileout_OK_CB(Widget, XtPointer, XtPointer);
XtCallbackProc fileout_CANCEL_CB(Widget, XtPointer, XtPointer);
FILE** p2sys(char *shell_command); /*970521*/
int p2closeData(FILE **fp);
int p2done(FILE **fp);
void dooutputfiledialog(int);
void determinedirectory(void);
#endif

#ifdef PCWINDOWS
HANDLE* p2sys(int putout, char *command_line); /*980610*/
int p2closeData(HANDLE *fileptr);
int p2done(HANDLE *fileptr);
#endif

/*All have doupdate() so generic call is valid*/ /*980712*/
void doupdate(int, char[256] );
void doupdatefrompipe(int, char[256] ); /*991213*/
void launchnleave(int, char[256] ); /*020930 based on doupdate*/
int  kinescopeout(void);
void inventoutputname(int ,int , int);
int  routeoutput(int);
int  outputtoinput(int);
void getstufffrompipebuffer(void);

/*-------------------------------------------------------------------------*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*generic routines */
void setsockets(int);        /*MAGESYNC.c*/
/*MAGEANGL.C routines moved to MAGEANGL.H*/

/*MAGEINPT.C routines*/
/*some that are only accessed within MAGEINPT are moved to there*/
void    GetStuffFromFile(void);
void    scalekinemage(void);
void    rescalekinemage(void);
void    reloadkinemage(void); /*060121*/
void    resetkinemage(void);
void    replacemarkers(void);  /*061116*/

void    addmarkers(void);
void    killmarkers(void);
void    killpickedpointers(void);  /*050916*/
void    addmeasures(void);
void    killmeasures(void);
void    addmonitorparam(void); /*140915*/
void    killmonitorparam(void); /*140915*/
void    adddrawstuff(void);
void    killdrawstuff(void);
int     getinteger(int,int);
int     getfloat(int,int);
void    killbondrotsliders(int); /*070617*/
/*MAGEHELP*/
void    writehelptohtmlfile(void); /*020929*/
void    writehelptostdout(void); /*031011*/
void    dohelpintextwin(int);
char    getinternalkinemage(int);
/*MAGEINIT.c*/
void    writechangestostdout(void); /*031011*/
void    writereporttostderr(void); /*070901 quick non-GUI tests of whatever...*/
/*MAGESETS.C routines*/
void    getgroup(int,int);
void    interpretgroupkeyword(char[256],int);
void    getnewgroup(void);
int     getnewmaster(void);
unsigned long long longpower(int, int); /*081120 vs 1<<n */
int     getnewcolorset(char*,char*);
int     mastermaker(char);  /*991210, int 051114*/
void    masterfinder(unsigned long long*); /*081120 long long rather than int*/
int     masterconfirmer(char);  /*991210,030425*/
int     colorsetfinder(unsigned int*);
void    getsubgroup(int,int);
void    interpretsubgroupkeyword(char[256],int);
void    getnewsubgroup(void);
void    getlist(int,int);
void    getnewlist(void);
void    errorreport(void);
int     listfirstline(int,int);
void    interpretlistkeyword(void);
void    addanothervector(int); /*060121 Ndimension*/


/*MAGEMENU.c*/
void    AdjustMenus(void);  /*971001*/
void    setdrawstuff(int);  /*971129, 020830 int==2 for connect-the-dots*/
void    closedrawstuff(void);/*971129*/
void    setsuperpick(void);
void    setcolorshow(void);
void    setemphasis(void);
void    setpickshow(void);
void    setmovepoints(void); /*061126*/
void    setmeansigmas(void); /*061126*/
void    setprune(void);
void    getusername(void); /*020816*/
void    getkinemagechoice(void);
void    setmultiwidth(void);
void    setwidthtoggle(void);
void    setthinline(void);
void    forcethinline(void);
void    setNdepths(void);
void    setperspec(void);
void    forceperspective(void);
void    setwhtbkg(void);
void    setcurrentaspect(int);
void    adjustcurrentaspect(int);
void    setaspect(int, int);
void    adjuststereo(void);
void    setstereo(void);
void    forcestereo(void);
void    setcompare(void);
void    setlistcolordominance(void);
void    setlistradiusdominance(void);
void    setmeasures(void);
void    setmonitorparam(void); /*140915*/
void    setxyzpoint(void);
void    setgnomon(void);
void    setsinglemarker(void); /*031226*/
void    setrate(void);
void    setclipmiddle(void);
void    setmeasureperpendic(void); /*030930*/
void    setformattest(void);
void    setvisibleout(void);
void    settest(void);
void    setstrokedfont(void);     /*MAGE stroked font, al la R3D*/
void    setplotonly(void); /*kalide*/
void    setfontcursor(void); /*Lfontcursor choice for Linux-UNIX*/
void    setflat(void);     /*flatland*/
void    sethorizon(void);
void    setremoteupdate(void);/*remote update e.g. of dots by PROBE*/
void    doundopickcenter(void);   /*960809*/
int     checksurvival(void);

void    menuadvancedhelp(int);
int     menuaboutsecrets(int);
int     menuaboutwrite(int);
int     menuaboutscreenout(int);  /*020904*/
int     menuaboutedit(int);
int     menuaboutoptions(int);
int     menuaboutspecial(int);
int     menuaboutOther(int);
int     menuaboutkluges(int);
int     menuaboutmouse(int);
int     menuaboutdocking(int);
int     menuaboutresize(int);
int     menuabouttable(int); /*000506*/
int     menuabouttest3D(int); /*020830*/
int     menuaboutbonds(int); /*130119*/
int     menuaboutNDIM(int); /*130119*/

void setaspectidentifier(int,char*); /*010908*/
void setaspectonscreen(char*,int); /*010908*/
void setviewidentifier(int,char*); /*970705*/
void addmeasures(void);
void    setLens(void);  /*971109*/
void    potentiateMeasures(void);  /*971204*/

/*____PIPE*/
int putintopipebuffer(char);
void readpipeintobuffer(int);
void destroyallpipebuff(void);
char getfrompipebuffer(void);
void flushpipe(void);

/*MAGESCOR*/
int  scoretest(void);


