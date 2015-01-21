/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 
/*#define UNIX_PEX */     /* unix PEX (phigs) */
/*#define UNIX_TBLZ*/     /* unix TBLZ (Trailblazer) */

#define UNIX_X11      /* unix X11 */
#define UNIX_MOTIF    /* unix motif */
/*#define UNIX_GL  */     /* unix GL (SGI) */
/*#define UNIX_OGL */     /* unix OpenGL (SGI+others) */
/*#define UNIX_ESV */     /* unix ESV (E&S special extensions, mainly to PEX)*/

/*MACINTOSH and PC undef the above, so can leave them in convenient state*/

/*MAGE.H decalrations for all mage files: */
/*   invokes include MAGE.HDR with subroutine and function prototypes*/
/*  main defines EXTERN as nothing ( "" ), so really does the declarations */
/* if EXTERN not defined, then it is defined as "extern"  so only referenced*/
/*   thus declarations are idempotent */
/*The specific compiler for each computer defines its own name control */
/*e.g. for Mac: THINK_C defined; for PC __BORLANDC__ defined */
/*  Beware of this for different compilers, different platforms*/
/* The Weiss Borland Workshop resource compiler chokes on*/
/* function declarations; thus all function declarations are kept in */
/* MAGE.HDR (invoked at end of this file) */
/*           (this is a useful distinction anyway) */
/* Furthermore, the Richardson Borland Workshop can't find standard */
/* includes; so all variables needed for menus and dialogs are kept */
/* in magemenu.h, PCWmage.rc just #includes MAGEMENU.H */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/                                

/*#include "MAGE.HDR" */ /*these includes are at the end of this mage.h file*/

/*merge of MAGE and EGAM:  force a definition here, and all files */
/* will be recompiled! */

/*#define EGAM*/     /*comment out this definition for MAGE! */

#ifdef __sgi            /*__sgi compiler specific*/
#define SGI_GL
#endif

/*#ifdef macintosh   */         /*__MWERKS__ compiler specific*/ /*980610*/
#ifdef __MWERKS__            /*__MWERKS__ compiler specific*/
#ifndef _WIN32
#define MACINTOSH
#define POWERMAC     /*MetroWerks defines __POWERPC__  __CFM68K__  */

#if defined(powerc) 
#undef OLD68KMAC
#else
#define OLD68KMAC
#endif

#undef PCWINDOWS
#undef UNIXMPESV
#undef UNIX_X11   /* unix X11 */
#undef UNIX_MOTIF /* unix motif */
#undef UNIX_PEX   /* unix PEX (phigs) */
#undef UNIX_GL    /* unix GL (SGI) */
#undef UNIX_OGL   /* unix OpenGL (SGI+others) */
#undef UNIX_ESV   /* unix ESV (E&S special extensions, mainly to PEX) */
#undef UNIX_TBLZ  /* unix TBLZ (Trailblazer) */
#endif  /*_WIN32*/
#endif  /*__MWERKS__*/


#ifdef THINK_C              /*Symantec THINK C compiler specific*/
#define MACINTOSH
#undef PCWINDOWS
#undef UNIXMPESV
#undef UNIX_X11   /* unix X11 */
#undef UNIX_MOTIF /* unix motif */
#undef UNIX_PEX   /* unix PEX (phigs) */
#undef UNIX_GL    /* unix GL (SGI) */
#undef UNIX_OGL   /* unix OpenGL (SGI+others) */
#undef UNIX_ESV   /* unix ESV (E&S special extensions, mainly to PEX) */
#undef UNIX_TBLZ  /* unix TBLZ (Trailblazer) */
#endif

#ifdef _WIN32
#define PCWINDOWS
#undef MACINTOSH
#undef UNIXMPESV
#undef UNIX_X11   /* unix X11 */
#undef UNIX_MOTIF /* unix motif */
#undef UNIX_PEX   /* unix PEX (phigs) */
#undef UNIX_GL    /* unix GL (SGI) */
#undef UNIX_OGL   /* unix OpenGL (SGI+others) */
#undef UNIX_ESV   /* unix ESV (E&S special extensions, mainly to PEX) */
#undef UNIX_TBLZ  /* unix TBLZ (Trailblazer) */
#endif

#ifdef EXTERN
#undef EXTERN
#define EXTERN
#define MAGE_INIT
#else
#define EXTERN extern
#endif

#define getnextchar() getc(fpin)

#ifdef THINK_C              /*compiler specific*/
#include <MacHeaders>
#include <GestaltEqu.h>
#include <AppleEvents.h>
#include <Palettes.h>     /*s 960221*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#endif /*MACINTOSH*/

/*#ifdef macintosh   */      /*__MWERKS__ compiler specific*/ /*980610*/
#ifdef MACINTOSH         /*__MWERKS__ compiler specific*/

/*  #if defined(powerc)*/  /*980630*/
     /*nothing special for power Mac*/
/*  #else*/
/*     SetA5(oldA5);*/ /*set A5 world for 68K */
/*  #endif*/
#define screenBits qd.screenBits

#include <stdio.h>

#include <math.h>
#include <Palettes.h>     /*s 960221*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Balloons.h>   /*980619*/

/*failure to avoid error despite entry present*/
#include <Dialogs.h> /*GetDItem,DisposDialog,SetIText,SelIText,GetIText*/
#include <Controls.h>   /*SetCtlValue,GetCtlValue,inUpButton,inPageUp*/
/*,inDownButton,inPageDown,SetCTitle,inCheckBox,inThumb,GetCRefCon*/
#include <MacMemory.h>   /*DisposPtr, */
#include <Menus.h>  /*AddResMenu,GetItem,SetItem*/
#include <MacTypes.h> 

#include <Quickdraw.h>  /*lists LocalToGlobal, which fails in MAGE 5.80 010509*/
#include <MacWindows.h> /*010509*/
/*010509 Metrowerks CodeWarrior/MacOS Support/Universal/Interfaces/CIncludes */
#endif /*__MWERKS__*/

#ifdef OLD__MWERKS__         /*__MWERKS__ compiler specific*/
/*#include <MacHeaders>*/
#include <stdio.h>

#include <Gestalt.h>
#include <AppleEvents.h>
#include <Quickdraw.h>
#include <Memory.h>
#include <Fonts.h>
#include <Events.h>

/*#include <Windows.h>*/ /*renamed MacWindows.h to avoid collision with MicroShit Windows*/
#include <MacWindows.h> /*010509*/
#include <OSUtils.h>
#include <ToolUtils.h>
#include <SegLoad.h>


#include <Dialogs.h>
#include <Types.h>
#include <TextEdit.h>
#endif /*OLD__MWERKS__*/

#ifdef PCWINDOWS
#ifndef __MWERKS__
#define STRICT
#endif
#include <afxwin.h>
#include <afxext.h>
#include <afxcmn.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#ifndef _MAX_PATH
#define _MAX_PATH 80
#endif
#endif /*PCWINDOWS*/

#ifdef UNIX_X11
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <sys/time.h>
#include <ctype.h>   /*20121107 for isalpha, isdigit*/
#include <unistd.h> /*121108 close() MAGESYNC.c */
#endif  /*UNIX_X11*/

#ifdef UNIX_MOTIF
#include <Xm/Xm.h> /*grafwindow*/
#include <Xm/Form.h> /*grafwindow*/
#include <Xm/DrawingA.h> /*actual drawing region*/
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/RowColumn.h> /*menuBar widget*/
#include <Xm/CascadeB.h> /*for individual menus */
#include <Xm/MessageB.h>
#include <Xm/ToggleB.h> /*for button boxes on screen */
#include <Xm/ToggleBG.h> /*for button boxes on screen */
#include <Xm/Text.h>
#include <Xm/FileSB.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#endif /*UNIX_MOTIF*/

#include <time.h>

#ifdef UNIX_X11
#define TRUE    1
#define FALSE   0
typedef Widget  MenuHandle;
typedef Widget  ControlHandle;
typedef Widget  WindowPtr;
#define XTR     dpy,offscreen,offgc,it(1,i),it(2,i),
#define LineTo  XDrawLine
EXTERN   struct brect { int left; int top; int right; int bottom;} brect ;
#define DEADBLACKINDEX BLACKINDEX   /*051201*/
#define DEADWHITEINDEX WHITEINDEX   /*051201*/
#endif /*UNIX_X11*/

#ifdef UNIX_X11      /*MAGE_4.0 great redo of all DLOG reports*/
#define CRLF "\012" /* \n, use CRLF outside of "..." in sprintf */
#define EOL    '\n'  /*octal 012, hex A, use as a character: if(c == EOL)...*/
#define EOLOUT '\n'
#endif

#ifdef MACINTOSH
#define CRLF "\015" /* \r, use CRLF outside of "..." in sprintf */
#define EOL    '\r'  /*octal 015, hex D, decimal 13 */
#define EOLOUT '\n'
#endif

#ifdef PCWINDOWS
#define CRLF "\015" /* \r, use CRLF outside of "..." in sprintf */
#define EOL    '\r'  /*octal 015, hex D, decimal 13 */
#define EOLOUT '\n'
#endif

/*'\n'  octal 012, hex A, decimal 10, Hex 0A */
/*'\r'  octal 015, hex D, decimal 13, Hex 0D */

#define MAXALERTSTR 256 /*alertstr limited to 256 by Mac Pascal str length*/

   /* array sizes */
/*#define MAXNUM    3500*/ /* total number of move triplets & draw triplets */
/*#define MAXDRAWS  3500*/ /* maximum number of 2-D Move-Draws displayable */
/*MAXNUM MAXDRAWS not used now in compilation, size can be dynamically alloc*/

#define MAXG         1        /* -1 = number of groups, index 1,...,MAXG */
#define MAXS         1        /* -1 = number of subgroups, index 1,...,MAXS */
#define MAXL         1  /*960603*/ /* -1 = number of lists, index 1,...,MAXL */
                /*beware! if this >255 get in trouble with wordsfont, etc.*/
#define MAXM         1 /*32?, # master button boxes,old 51*/
#define MAXXG        1  /*number of extra groups: markers,measures,drawgroup,+*/
                        /*inc functionality control buttonboxes  */
#define MAXBUTTONS  MAXG+MAXXG+MAXS+MAXL+MAXM /*allows up to MAXM masters*/

#define NMARKERS    26
#define MAXMEASURES  7
#define MAXMONITOR  17
#define NDRAWPOINT   7

#define MAXRESETS    61 /*number of resets (matrix,zoom,centering) views */
                        /*up to and including readers' view after the lastth */
                        /*allowance made so count is indeed from 1...MAXRESETS*/
                        /*count from 1 ... 60, with reader's view extra*/

#define MM_BUTTONINDEX  10000  /*PC index to define buttons as menu commands*/
#define MAXQUESTIONS    41 /*number of questions for hypertext mode*/
#define MAXQCHAR        25 /*number of characters in an answer */
   /*this can effect, e.g., text char counter in MAGETEXT.C*/
/*#define MAXPTIDCHAR    20*/ /*# char excluding terminator of point ID */
                  /*orig 15, change to 20 9412*/ /*obselete long before 980707*/
                          /*wordcom now allows indefinite length pointID*/
                          /*only FIND strings using this as of 980707*/
#define MAXVIEWIDCHAR  15 /*# char excluding terminator of view ID */
#define MAXPARAMETER   10 /*# arbitrary parameters for programmer to play with*/
#define MAXPOINTASPECTS 16 /*color codes for point aspects*/
#define MAXUSERCHAR     32 /*short name or personal identifier for timed tests*/
#define MAXNAMECHAR    36 /*# char excluding terminator of group,etc names*/
   /*because of master by name, group, subgroup, and list names must have*/
   /*same maximum number of characters, was 14 < 060613 */
   /* moved from MAGELIST.h 060619*/
#define MAXNDIM        17 /*# dimensions 060622 */
#define MAXHYPER     2048 /*Qstring for hypertext expression 070622,070709*/
#define MAXTEXT   1048574 /*1048576==2^20, for suitefit....kin 070622*/
 /*---------------allocated big storage arrays---MAC and UNIX----------------*/
  /* these arrays may be defined differently in different machines*/
                            
 EXTERN  char   *com;           /* pointer to start of array of ptID char */
                                /* limited to MAXCOM */
 EXTERN   char   *wordcom;      /* pointer to start of array of text char */
                                /* limited to MAXWORDCOM */
 EXTERN  long ncom;     /*offset for start of ptID string in com*/
 EXTERN  long nwordcom; /*offset for start of a word string in wordcom*/

 #define MAXEXTRA  64      /*number of extra strings, i.e. of file names*/
 EXTERN  char *ExtraStr[MAXEXTRA];
 EXTERN  int Nextrafile,Lextrafile;

 EXTERN int *it[8]; /*080405 was int, 7: array of pointers, it[8][MAXDRAWS]*/
     /* internal 2-D display list, regenerated by display loop: MAGEDRAW.C*/
 EXTERN    long     *linkit;     /*linklist entries per depth bin for each run*/
 EXTERN    long     *thelink[2]; /*first and last linklist entries in each bin*/
 /*   pointstructptr* itpointptr;  declared in MAGELIST.H*/
            /*relates display point back to orig coord point pointer structure*/
 /*__________________________________________________________________________*/

/*debug print statements to track input looping 040605*/
EXTERN char mess1[256],mess2[256],mess3[256],mess4[256],mess5[256];/*040605*/
EXTERN long nmess1,nmess2,nmess3,nmess4,nmess5;/*040605*/

#ifndef PCWINDOWS
#define   it(i,j)   it[i][j]
#define      pcom1(i) *(com+i)
#define   pwordcom(i) *(wordcom+i)
#define   plinkit(i) *(linkit+i)
#define   thelink(i,j) thelink[i][j]
EXTERN long ncount; /*current index into display arrays it( ,ncount) */

#endif

#ifdef PCWINDOWS
#define   it(i,j) *(it[i]   +j)
#define   pcom1(i) *(com+i)
#define   pwordcom(i) *(wordcom+i)
#define   plinkit(i) *(linkit+i)
#define   thelink(i,j) *(thelink[i]   +j)
EXTERN int  ncount; /*current index into display arrays it( ,ncount) */

 /*#define pcom1(i) com[i] */
 /*works identically to #define pcom1(i) *(com+i)*/
 /*#define pcom1(i) *(com+i)*/
 /*actaully works but gets error flag for >64k addr*/
 /*#define pcom1(i) com[i] */
 /*works identically to #define pcom1(i) *(com+i)*/

     /*using normalized huge pointer of form HHHH:000h */
     /*where the 20 bit address is actually: 000H:HHHh */
     /*com is of form hhhh:0000 since it is allocated to even 16 bits*/
     /*i is of form   000h:hhhh since it can't exceed 20 bits */
     /* proper normalized huge pointer aritmetic then is just:*/
/*#define pcom1(i) (char huge *)(com + ((i&0XFFFF0)<<12) + (i&0XF) ) */
/*but compiler claims is nonportable pointer conversion*/
/*so see if system can be pursuaded to do the pointer arithmetic*/
/*#define pcom1(i) (char huge *)(com+i)*/
/*but compiler claims is nonportable pointer conversion*/
/*#define pcom1(i) *((char huge *)com+i)*/
/*works but (char huge *) may not be necessary*/

#endif

#ifdef MACINTOSH
  /*Mac specific definitions*/
#define XTR
#define ARRAYSDONE
#endif /*MACINTOSH*/

#ifdef PCWINDOWS
/* offsets and sizes of text window variables in window extra bytes: */
/*   0  4   : bytes in text buffer */
/*   4  4   : total lines (after formatting) in buffer */
/*   8  4   : current line at top of screen */
/*  12  2   : global handle of text buffer */
/*  16  2   : global handle of line start array buffer */

#define TEXTLENGTH 0
#define TEXTLINESTOTAL 4
#define TEXTLINECURRENT 8
#define TEXTHANDLE 12
#define TEXTLINESTARTHANDLE 16

  EXTERN HANDLE readpipe; /* handle to read pipe for Probe/Prekin consoles */

#define ARRAYSDONE
#define XTR cDC,   /*the comma is part of the definition*/
         /* extra field needed in PC MoveTo, DrawTo commands */
#endif /*PCWINDOWS*/

EXTERN int WHITEINDEX, BLACKINDEX; /*961013*/
EXTERN int Lnew,icount,c; /*for all platforms, 030403*/

#ifdef UNIX_X11
EXTERN Display *dpy;
EXTERN  Arg args[100];
EXTERN  int n;
EXTERN  Widget topLevel, topgraf, toptext, topcapt, topbrot, topbutt;
EXTERN  Widget brotwinholder, buttwinholder; /*butt stuff 041108*/
EXTERN  Widget grafwindow, textwindow, captwindow, brotwindow, buttwindow;
EXTERN  Widget quitButton, resetButton, drawArea;
EXTERN  Widget msgbox,optionItem,message;
EXTERN  Widget alertButton;
EXTERN  Widget kinebox,kinerow,kinemsg,kinenum,kinenow;
EXTERN  Widget kinebut,kinebutok,kinebutnext,kinebutcan;
EXTERN  Widget reportbox,reports,report1,report2,report3,report4;
EXTERN  Widget reportbuts,reportbutOK,reportbutabort;
/*slider Widgets == ControlHandle vgrafzoom,vgrafzclip,vgrafztran;*/
EXTERN  XtIntervalId Timerforresize; /*040523*/
/*value label widgets for sliders: */
EXTERN  Widget zoomvalue, zclipvalue, ztranvalue;/*the 3 in the grafwindow*/
EXTERN  Widget zoomtitle, zcliptitle, ztrantitle;/*the 3 in the grafwindow*/
EXTERN  XtPointer Callbackvalue;
EXTERN  XFontStruct *titlefont; /*040223*/
EXTERN  XFontStruct *textfont; /*040223*/
EXTERN  XFontStruct *menufont; /*040223*/
EXTERN  XFontStruct *buttonfont;
EXTERN  XFontStruct *tinyfont;
EXTERN  XFontStruct *littlefont;
EXTERN  XFontStruct *mediumfont;
EXTERN  XFontStruct *largefont;
EXTERN  XFontStruct *giantfont;
EXTERN  XmFontList titlefontList,textfontList,menufontList,buttonfontList;
EXTERN  XmFontList tinyfontList;
EXTERN  XmFontList littlefontList;
EXTERN  XmFontList mediumfontList;
EXTERN  XmFontList largefontList;
EXTERN  XmFontList giantfontList;
EXTERN  Window  drawWindow;                     /* drawing window */
/*mage stuff below here */
EXTERN Boolean Lmagedone;
EXTERN float rotx, roty, rotz;
EXTERN GC gc,offgc,ggc;
EXTERN Pixmap offscreen;
EXTERN XtAppContext app_context;
EXTERN Visual* magevisual;
EXTERN int magedepth;
EXTERN Pixel windowbackground,windowborder,windowforeground;
EXTERN Pixel highlightcolor,topshadowcolor,bottomshadowcolor;
EXTERN Pixel selectcolor,troughcolor,armedfillcolor;

EXTERN Colormap colormap;
EXTERN unsigned long  incolor[256]; /*index to colormap */
EXTERN int incolored[256]; /*flag for incolor[] defined*/

EXTERN Colormap grayscalemap; /*040926*/
EXTERN unsigned long  ingrayn[256]; /*index to grayscalemap 040926*/
EXTERN int ingrayned[256]; /*flag for ingrayn[] defined     040926*/

typedef Cursor CursHandle;

#endif /*UNIX_X11*/

/*050121 moved to general so can be referred to anywhere, only used UNIX_X11*/
EXTERN int Lgrayscalemap; /*flag for created grayscalemap   040926*/
EXTERN int Lgrayscale; /*flag for using grayscalemap   040926*/

#ifdef PCWINDOWS
  typedef HWND WindowPtr;
  typedef HWND ControlHandle;
  typedef RECT Rect;
  typedef HWND TEHandle;
  typedef HANDLE PaletteHandle;
  typedef HANDLE DialogPtr;
  typedef HANDLE GrafPort;
  typedef HANDLE CGrafPort;
  typedef HANDLE BitMap;
  typedef HANDLE PixMap;
  typedef HANDLE MenuHandle;
  typedef HANDLE CursHandle;
  typedef int EventRecord;
#endif /*PCWINDOWS*/

#ifdef MACINTOSH
#define pascalvoid pascal void
#define pascalBOOL pascal Boolean
#endif

   /* array sizes */
EXTERN  long  MAXNUM; /*3500*/ /*total number of move triplets & draw triplets*/
   /*MAXNUM superceeded by individual point allocation in MAGE5 */ /*970408*/
EXTERN  long  MAXDRAWS;/*3500*/ /*maximum number of 2-D Move-Draws displayable*/
EXTERN  long  MAXWORDCOM; /*total number of characters of comment or textlist */
EXTERN  long  MAXCOM; /*total number of characters of all ptIDs */
/*MAXNUM MAXDRAWS MAXWORDCOM not used in compilation, run time dynamic alloc*/
EXTERN  int  Limitmaxnum, Limitmaxdraws, Limitmaxwordcom, Limitmaxcom;/*970410*/
  /*Logicals reset each kinemage so can know when hit Limit of allocation*/
  /*and thus report such only once*/
EXTERN  int  IMAC,IPC,IUX,IUXM,IUXP,IESV; /*flow control during execution */

/*kludge to get results of crossproduct calculation, avoids passing pointers*/
EXTERN    double        dblx,dbly,dblz;
/*group of variables with Mac specific declarations that must be PC,PEX valid*/
/*UNIX: typedef Widget  WindowPtr; PC: typedef HWND WindowPtr;*/
EXTERN WindowPtr  textWindow,captWindow,grafWindow;
EXTERN WindowPtr  genericdialogBox,MageDLOGreportBox,MageinitDLOGreportBox;
EXTERN WindowPtr  QuesdialogBox,colorshowdialogBox,pickshowdialogBox;
EXTERN WindowPtr  constructdialogBox,pickcolordialogBox,palettenamedialogBox;
EXTERN char currentcolorname[16];
EXTERN int  currentcolornumber,Lcurrentbkgwhite;

     /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
EXTERN ControlHandle  vtextScroll,vcaptScroll,vgrafzoom,vgrafzclip,vgrafztran;
/* group of variables with Mac specific declarations that must be PC valid*/

#ifndef UNIX_X11
EXTERN Rect           dragRect, windowBounds, boxBounds;
EXTERN TEHandle       theText, textText, captText;
EXTERN Rect   destRect, textdRect, captdRect, viewRect, textvRect, captvRect;
EXTERN PaletteHandle    makePalette(), MagePalette, makegrayPalette();
EXTERN GrafPort      offScreenPort;
EXTERN CGrafPort     CoffScreenPort;
EXTERN BitMap        offScreenBits;
EXTERN PixMap        CoffScreenBits;
EXTERN Rect brect;  /*920619 pass window rect for PC MAGE button placement*/
#endif  /*!UNIX_X11*/
EXTERN CursHandle waitcursor,magecursor;
EXTERN CursHandle mage3Dcursor,magexy3Dcursor,magez3Dcursor;
EXTERN CursHandle mage2Dcursor,magexy2Dcursor;
EXTERN CursHandle mageTE3Dcursor,mageTE2Dcursor;

EXTERN  int        grafwinportRectleft, grafwinportRecttop;
EXTERN  int        grafwinportRectright, grafwinportRectbottom;
EXTERN  int        grafwinportBitsleft, grafwinportBitstop;
EXTERN  int        grafwinportBitsright, grafwinportBitsbottom;
EXTERN  int        Gorigtop,Gorigleft,Gorigbottom,Gorigright;
/* variables with generic c declarations */
/*computer display (140912) characteristics as set for this mage run */
EXTERN    int     Lcolor,LCscrn,LLcolor,Lread_writecolors;
EXTERN    int     GWIDTH, GHEIGHT, CHEIGHT, BWIDTH,ZWIDTH, ibigfont,FONTSIZE;
EXTERN    int     BUTTWINWIDTH; /*separate UNIX button window width 041108*/
EXTERN    int     Lbuttonseparate; /*flag 041108 */
EXTERN    int     GZDEEP, MENUHEIGHT, MESSHEIGHT,Lfontinfo,Lmacularfont; 
EXTERN    int     gwidthset,Lgwidthset,Lforcedfont; /*040218*/
EXTERN    int     gwidthview[MAXRESETS+1],gheightview[MAXRESETS+1]; /*041031*/
EXTERN    int     Lgwidthheightview[MAXRESETS+1]; /*060613*/
EXTERN    int     ALERTHEIGHT; /*of individual alertstr dialog panels UNIX*/
EXTERN    int     ROTSWIDTH;    /* allowance for bondrot scroll bars */
EXTERN    int     RWIDTH; /*variable for bondrotsliders in MAC, PC graf window*/
EXTERN    int     ROTWINDWIDTH;/*constant, bondrotsliders separate UNIX window*/
EXTERN    int     ROTSLIDERWIDTH;/*constant, bondrotsliders bar width UNIX */
EXTERN    int     ZSLIDERWIDTH;/*constant, bondrotsliders bar width UNIX */
EXTERN    int     Lallbondrotsliders; /*trumps noslider bondrot list 070617*/
EXTERN    int     isavewidth,igrafscrntall;
EXTERN    int     TOPBAR,MIDBAR,BOTTOMBAR,MARGIN,TWIDTH,BORDER;
EXTERN    int     Liveoffscreenbitmap; /*flag for off-screen bitmap */    
EXTERN    int            npixelbits; /*screen depth*/
EXTERN    long    maxtextchar,maxtextcharcurrent; /*030910*/
EXTERN    int     Limittext; /*981010*/
EXTERN    short   lineHeight,textHeight,captHeight,widMax; /*981010*/
EXTERN    int     nLines, textLines, captLines, scrolLines; /*981010*/
/*other general variables */
EXTERN    float   lensFactor; /*970905*/
EXTERN    int     Ldock; /*970503*/
EXTERN    int     Ldumpprobe; /*061222 probe atoms to stderr*/
EXTERN    float   finestep,finestepdefault; /*011201*/
EXTERN    float   DEGSTEP,degreestepdefault; /*070617*/
EXTERN    float   MAGEVERSION,KINVERSION; /*970503*/
EXTERN    char    NameStr[256],KineName[256],AppendNameStr[256];
                      /*file names, char # limited*/
EXTERN    char    OutStr[256],DirStr[256],TestStr[256];
                     /*020209 need to be referenced by several routines*/
EXTERN    char    version[256],modification[256];/*about this MAGE version*/
EXTERN    char    features[256];
EXTERN    int     width,ipick,pickvec,Lpick,Lprepick,Lpicknow;
EXTERN    short   pickx,picky;
EXTERN    int     Lsuperpick,imodify,Lpickwhen,Lpickerase,Lpickeraseon;
EXTERN    int     irotxy;
EXTERN    int     Lcancel,Lerase,Lprocessing,Lkinformat,Lrotformat;
EXTERN    int     Lrotangleout;  /*070705*/
EXTERN    int     Ladddrag,Lspacebar,Laltkey,Lctrlkey,Lshiftkey,Lscroll;
EXTERN    int     Laccumulate;  /*030111*/
EXTERN    int     mousex, mousey, mousexold, mouseyold;
EXTERN    int     Llocalrotation,Llocalprecenter,Llocalpostcenter;/*input*/
EXTERN    long    icom;
EXTERN    float   x[8],x2,y2,z2; /*060121 was x[4] */
EXTERN    int     IOerr,atEOF,LOK,Lkinemagecount,Lkinfile;
EXTERN    int     LneedSetsinGrafWindow; /*030320*/
EXTERN    int     kinemagenum,nextkinenum,Lnextkine,thiskinenum,LnextOK;
EXTERN    int     Ltestmode,Lpracticemode,Ltestnext,Ltestoutfile;/*020904,14*/
EXTERN    int     Ltestscorepending,Ltestreportpending,Ltestscoreavailable;
EXTERN    int     Ltesttimeend,Lnosavealert,Ltestcontinue;
EXTERN    int     Lresizeactive; /*040509*/
EXTERN    int     Linputactive; /*040605*/
EXTERN    int     origkinenum,Lorigkinfile; /*981207*/
EXTERN    int     Ltexteditable;  /*980920*/
EXTERN    int     Ltablecolheaders,Ltablemarkers; /*000324*/
EXTERN    long     nxyz; /*point currently open*/ /*long 970410*/
EXTERN    long     maxnxyz;
/*EXTERN    int     npoint;*/
/*EXTERN    int     maxpoint;*/
/*EXTERN    int     nlist, maxlist;*/
EXTERN    int     nanimate,nnanimate;
EXTERN    int     Lcontrol,Lcontrolon;
EXTERN    int     Lparamtrigger,Lparamtriggeron; /*141006*/
EXTERN    int     Lswitch,Lswitchon; /*090419 for HelixBearHair.kin xrays*/
EXTERN    int     Linhibitbutton,Linhibiton;
EXTERN    char    usernamestr[128]; /*file name part < MAXUSERCHAR*/
EXTERN    char    thisanswer[MAXQUESTIONS][MAXQCHAR];
EXTERN    char    grafselection[2][MAXQCHAR];
EXTERN    int     mQuestion,nQuestion,Lgographics,Lisgraphics,Lredohyperbutton;
EXTERN    int     Lqueskin,Nqueskin[MAXQUESTIONS],oldNqueskin,Lquesanswers;
EXTERN    int     Lquesview,Nquesview[MAXQUESTIONS],oldNquesview;
EXTERN    int     Lquesfind,itextoffset,Lquesalloff;              /*980922c*/
EXTERN unsigned long long Lquesmastermask,Nquesmasteronoff; /*971012, 081120 long long*/
EXTERN   char Qstring[MAXHYPER]; /*hypertext expression size, suitefit 070622*/
            /*Qstring: question size increase 031002 for rotamers*/
EXTERN   int  iQstring; /*question*/
EXTERN   char conformerstr[256];/*071215*/
EXTERN   char ribose3hyperstr[256],ribose5hyperstr[256];/*071215*/

EXTERN    float   parameter[MAXPARAMETER+1];
EXTERN    int     iparameter;

EXTERN    int     markdeep,mark1deep,mark2deep; /*from MAGEDRAW.c 061116*/
EXTERN    int     nn,Lpickshow,Lpickshowon,Lpickcolor,Lpickcoloron,Lpointcolor;
EXTERN    int     Lmovepoints,Lmovepointson,Lmeansigmas,Lmeansigmason;/*061126*/
EXTERN    int     Ltargeteditactivity; /*061126*/
EXTERN    int     markergrp,markerset,marker1list,marker2list;
EXTERN    int     markergnom1list,markergnom2list;
EXTERN    int     measuregrp,measureset,measurelinelist,measuredotslist;
EXTERN    int     drawgrp,drawptrset,drawnewset,drawptrlist,drawnewlist;
EXTERN    int     marker[NMARKERS+1][4],drawpt[NDRAWPOINT+1][4];
EXTERN    float   fmarker[NMARKERS+1][3],fdrawpt[NDRAWPOINT+1][3];
EXTERN    float   fxcenternew,fycenternew,fzcenternew,scalenew;
EXTERN    float   fxcenterold[MAXRESETS+2];
EXTERN    float   fycenterold[MAXRESETS+2];
EXTERN    float   fzcenterold[MAXRESETS+2]; 
                   /*[0] used for localprecenter on input*/
                   /*and [0] reused to hold past pickcenter for UNDO */
EXTERN    int     imarker;
EXTERN    FILE    *fp;
EXTERN    FILE    *fpappend;
EXTERN    FILE    *fpin;
EXTERN    FILE    *fpout;
EXTERN    FILE    *fpnew; 
EXTERN    FILE    *fphold; /*071113*/ 
EXTERN    double   a11,a12,a13,a21,a22,a23,a31,a32,a33;
EXTERN    double   r11,r12,r13,r21,r22,r23,r31,r32,r33; /*010609*/
EXTERN    double   ma[MAXRESETS+1][10];
EXTERN    int     ixctr,iyctr,izctr,WIDTH,HEIGHT,ISOSCALE;
EXTERN    int    Lscale,DOscale,Newscale;
                 /*input control flags*/
EXTERN    int    Lgrp,Lset,Lidn; /*Lidn made global 030626*/
EXTERN    int    Lbegin,Lnewfile,Lreentry,Lcount,Lrecalculate;/*flow controls*/
EXTERN    int    Lgetnewfile,Lappend,Lupdate,Lignorecaption;
EXTERN    int    Lappendout; /*020303*/
EXTERN    int    LPDBformat,Lhydrogensout,LhydrogensSep; /*070829*/
EXTERN    int    LrotoutSORTED; /*070901*/
EXTERN    int    Ldeletemapcontours,Ldeleteprobedots,Lgetmapfilename; /*030312*/
EXTERN    int    Ldeleteprekinappend,Ldeletepipeinput,LdeleteNOEinput; 
EXTERN    int    Lappendscreentext, Lappendscreentexton; /*030311*/
EXTERN    int    Lreplacecaptiontext, Lreplacecaptiontexton; /*030315*/
EXTERN    int    LNEW; /*030315 global convenience flag, must set locally*/
EXTERN    int    LimportOK,Limporton; /*020514*/
EXTERN    int    LquitMAGE,Lgetnextkinemage,Lgetkinemagechoice;
EXTERN    int    Lclosedrawstuff;
EXTERN    int    Linternal,Ninternal; /*000506 for internally stored kinemage==help*/

EXTERN    int   update_res_num,update_res_num_index,update_res_probe;
EXTERN    int   update_res_model; /*050121*/
EXTERN    char  update_file_name[64],update_command_line[256];
EXTERN    char  update_probe_command_line[256],update_contour_command_line[256];
EXTERN    char  update_prekin_command_line[256],update_NOE_command_line[256];
           /*030312 probe and contour_map can be updated during same Mage run*/ 
           /*030928 noe distances can be updated...*/
EXTERN    char  input_command_line[256],update_res_num_str[64]; 
EXTERN    char  remote_computer_name[64]; 
EXTERN    char  import_command_line[256];

EXTERN    char   kintitlestr[256],kincopyrightstr[256];
EXTERN    char   kinpdbfilestr[256], kinmapfilestr[256]; /*030312*/
EXTERN    char   kinNOEfilestr[256]; /*030928*/
EXTERN    int    LgetNOEfilename,LkinNOEfile,Lgetpdbfilename; /*030928 030930*/
EXTERN    int    Lkintitle,Lkincopyright,Lkinpdbfile,Lkinmapfile; /*030312*/
EXTERN    int    Lsuperpunchon, killradius;

EXTERN    int    LAELaunched,Lappleevents; /*Mac AppleEvent flags*/
EXTERN    int    Lexcessivepoints,LpointIDtoolong; 
                 /*only first instance warning*/
EXTERN    int    Lvisibleout;
EXTERN    int    Laverageout, NDIMout, Npassout;/*060604 7D means*/
EXTERN    int    Lrate,Ltest,Lcursoractive,Lflat,Lformattest;
EXTERN    int    Lallocerror; /*errorreport an allocation error 051114*/
EXTERN    int    Lhorizon,Lpickmarkcriteria;
EXTERN    int    Lspecialloop,Lpostscript,LspecialPS,LRGBPS;
EXTERN    int    LGUI,LGUINO,Lhelpstdout,Lchangesstdout; /*030505,031011*/
EXTERN    int    Lreportstderr; /*070901 quick non-GUI tests of whatever...*/
EXTERN    int    LR3Dshadow,LR3Dappend,LR3Dpreview,LR3Dstrokedtext;
EXTERN    int    LR3Dsquaredcolor,Lraster3Drender;
EXTERN    int    LMAGEstrokedfont,Lstrokedfont,Lfloat;
EXTERN    int    Lcarry,Lcarry1,Lcarry2; /*MUX_MAIN ?? */
EXTERN    int    Lonewidth,Lthin,Lperspec,Lwhitebkg,Lxyzpoint,Lmarkers;
EXTERN    int    Lscalemarkers;
EXTERN    int    Lmultiwidth,Listcolordominant,Lscalefonts;
EXTERN    int    Listradiusdominant; /*040402*/
EXTERN    int    Lnewmeasures,Lmeasureson,Lmeasures,Lgnomon,Lsinglemarker;
EXTERN    int    Lmonitorparamon,Lmonitorparam; /*140915 Nucleic params*/
EXTERN    int    Lmeasureperpendics,LNOEdisplayactive; /*030930*/
EXTERN    int    Lmeasuredsurveillance; /*091212*/
EXTERN    int    Lprune,Lpruneon,Lpunchon,Lundop,nundop,Lpruneactive;
EXTERN    int    Ldrawactive,Lnewviewactive,Lnewstuffactive,Lprobeactive;
EXTERN    int    Lpipeactive,Lstartinpipe,Lpipebufferloaded,Ldonewithpipe;
EXTERN    int    Lnewframenumber,framenumber;
EXTERN    char   framenumberstr[128];           /*991213, 000115*/
/*EXTERN    int    nprune[11][3];*/
EXTERN    char   commentinputstr[256];  /*020917*/
EXTERN    int    Lcommentinput;  /*020917*/
EXTERN    int    Ldrawnewinput,Ldrawlineinput,Ldrawlabelinput,Ldrawballinput;
EXTERN    int    Lbigfontsinput,Lnosavealertinput,Lshortlineinput;
EXTERN    int    Lnewlabels,Lnewlabelson,Ldraglabelson;
EXTERN    int    Lnewballs,Lnewballson;
EXTERN    int    Lnewlines,Lnewlineson;
EXTERN    int    Lnewdrawline,Ldrawstuff,Lpoint,Loriginatctr;
EXTERN    int    Lconstructline,Ldrawunpickable,Lconstructdot;/*971122*/
EXTERN    int    Lconstruct4on,Lconstruct5on,Larrowline;/*000209*/
EXTERN    int    Lconstruct6on;/*140912*/
EXTERN    int    Ldock3on3,Ldock3on3on; /*020305*/
EXTERN    int    Lmeasureddihedral,Lmeasureddihedralon; /*020220*/
EXTERN    float  mdihedx,mdihedy,mdihedz;
EXTERN    int    Lconstructprojection,beginselection,endselection;
EXTERN    int    Lemphasis,Lemphasison; /*020112xrc beginemphasis,endemphasis*/
EXTERN    int    Ldragline,Lnewdrag,Lnewmatrix,Ldraglineactive,Ldraglineon;
EXTERN    int    Lmonitor,Lmonitoron,nlast,nprelast; /*140912 e.g. line length*/
EXTERN    int    Lonscreeninfo,Lonscreeninfoon; /*060122*/
EXTERN    char   onscreeninfostr[256]; /*060122*/
EXTERN    int    LSELECTINGlist,LSELECTINGliston; /*060121, general set 060622*/
EXTERN    int    LUnSELECTING,LUnSELECTINGon; /*060206*/
EXTERN    int    LoneSELECTING,LoneSELECTINGon; /*060207*/
EXTERN    int    ncolorSELECTING,ncolornotSELECTING; /*060121*/
EXTERN    float  radiusSELECTING; /*060121*/
EXTERN    float     distbetween;
EXTERN    float     perpendictoline,perpendictoplane; /*030930*/
EXTERN    float     distmeasure,anglemeasure,dihedralmeasure;
EXTERN    float     distconstruct,angleconstruct,dihedralconstruct;
EXTERN    float     distuser,angleuser,dihedraluser;
EXTERN    float     diststored,anglestored,dihedralstored;
EXTERN    float     distarrow,anglearrow,dihedralarrow;
EXTERN    float     fxcenter,fycenter,fzcenter,Scale,oldxyz[4],zclip;
EXTERN    float     zoom,zoomold[MAXRESETS+1],zoommax; /*030405*/
EXTERN    long      numberline; /*number drawnew points: 030331 --> long */
EXTERN    int       nscale,oldmaxwide;
EXTERN    float     fdiff,fxmax,fxmin,fymax,fymin,fzmax,fzmin;
EXTERN    int       Lpreviousscalables,Lpresentscalables,Lpreviousbondrotplot;
EXTERN char alertstr[MAXALERTSTR],alertstr2[MAXALERTSTR],alertstr3[MAXALERTSTR];
EXTERN char alertstr4[MAXALERTSTR];
EXTERN char  comstr[256],dstring[256],commentstr[256],focusxyzstr[256];
EXTERN char  surveilstr[256];  /*091212 for measuredsurveillance report to screen*/
EXTERN char  word[256],temps[256],numstr[256]; /*general strings*/
EXTERN int   iword, itemps, inumstr;
EXTERN int   Lmenu, Lmustkeeppoint, pointcolor;
EXTERN long  totalrunticks,firstloopticks,secondloopticks,nmovedraws;
EXTERN long  predrawticks, preloopticks, postcopyticks, postdrawticks;
EXTERN long  magetimeticklimit; /*timing in ticks == 1/60 seconds*/ /*010822*/
EXTERN int   Lmagetimerrunning, Lmagetimerupdate, Lmagetimerset, Lmagetimer; 
EXTERN float magetesttime,magetimeremaining; /*in decimal minutes*/ /*020816*/
EXTERN int   magetimermin,magetimersec; /*020830*/
EXTERN int   Scorepositive,Scorenegative,Scoretotal;  /*020907*/
EXTERN int   Scorepositiveold,Scorenegativeold,Scoretotalold,kinold; /*020914*/
EXTERN int   Lrescoring; /*020830 for rescoring result vs reference*/
EXTERN float mousetickrate,updaterate,updateratelimit,updateratelimitdefault;
EXTERN float fullupdaterate;
EXTERN int  LmouseLdown,LmouseMdown,LmouseRdown,Levelofdetail,Ldetail;/*030205*/
EXTERN long  icountticks,ncountticks,mouseticks,anysize;

EXTERN long  nextanimatetick, nextrocktick; /*010609*/
EXTERN int   Lmickeymouserun,Lmickeymouse,Lpausemickeymouse; /*010609,011023*/
EXTERN int   Lautoanimate,Lautorock,Lnewrock,Lmouseactive; /*010609*/
EXTERN float animatesteptime,rocksteptime; /*010609*/
EXTERN float rockmin,rockmax,rockcycletime,rocknumsteps; /*010609*/
EXTERN int   Legamickdocking,Lneedannealmobile,Modeegamickdocking;/*egam030217*/
EXTERN int   Negamickshape,Legamickchoice,Lfreeegamickmobile; /*egam030217*/
EXTERN int   Legamickfoo,Legamickfooon; /*egam030223*/
EXTERN int   Legamickrepeat,LegamickrepeatOK; /*egam030225*/
EXTERN int   Negamickfoocount,Negamickbarcount,Negamickvalue,Negamickpass; 
EXTERN int   Lrescalekinemage,Lredrawvector,Ltetramers; /*030329*/
EXTERN int   Lreloadkinemage,Lreplacemarkers; /*060121,061116*/
EXTERN int   Lfoocavity,Lfoosealer,Lfoosealeron,Lwildfoo; 
EXTERN float boundxmin,boundxmax,boundymin,boundymax,boundzmin,boundzmax;
EXTERN float fooradius,foogridstep,foodotdensity; /*030330*/
EXTERN    int       Linstance,Lplotonly,Lfirstcall,Lplotpoint;
EXTERN    int       Lstereo,LLRstereo,LTBstereo,LALTstereo;
EXTERN    int       Lclone,Lrotateye,Lcenterinfo,Lfontcursor;
EXTERN    int       TBYoffset; /*970603*/
EXTERN    int        Lnewpickcenter,Lzoom,Lcompareon,Lcompare,Lcomparestart;
/*EXTERN    int        nextanimategroup;*/ /*980707*/
EXTERN    int       Lzoomer[MAXRESETS+1],Lcenter[MAXRESETS+1];
EXTERN    int       Lzcliper[MAXRESETS+1],Lztraner[MAXRESETS+1];
EXTERN    int        Lzclip,Lzclipon,Lztran,Lspanfromzoom;
EXTERN    int        Lpickcenter,Lpickcenteron,Lcenterpoint;
EXTERN    float     fzclip; /*fudges zclip, zlimit, ztran to screen box size*/
EXTERN    int       izclip,izclipold[MAXRESETS+1],izlimit,izclipmax; /*030405*/
EXTERN    int       iztran,iztranold[MAXRESETS+1];
EXTERN    int       ixtran,iytran;
EXTERN    int       jx1,jy1,jz1,jx2,jy2,jz2;
EXTERN    int   Lclipmiddle; /*051214 clip middle line if both ends out of box*/
EXTERN    int   L7dimensions,L7dimnames; /*060121,060129*/
EXTERN    int   LNdimensions,LNdimnames,NDIM; /*060622*/
EXTERN    int   Laxischoice[MAXRESETS+1],axischoice[MAXRESETS+1][3];/*060613*/
EXTERN    int   LNDparallelaxes; /*plot NDIM ball as parallel axes 061114*/
EXTERN    int   LNDpointsinfo; /*toggle info: show only one color 061115*/
EXTERN    int   LNDpickedinfo; /*toggle info: on-off picked point 061117*/
EXTERN    int   Lcentered; /*existence of a group to keep centered 130115*/

EXTERN    int   Linvert360yzon; /*for NDIM screen fixed with suitefit 121212*/
EXTERN    int   Linvert360yz; /*for NDIM screen fixed with suitefit 121212*/
EXTERN    int   LSCREENpoints; /*existence of NDIM screen fixed points 121212*/

EXTERN    int     nviewsthiskinemage;
EXTERN    int     naspectsthiskinemage,maxaspectsperpoint; /*030605*/
EXTERN    int     currentaspect, Laspectonscreen;
EXTERN    char    viewcom[MAXRESETS+1][MAXVIEWIDCHAR+1];
EXTERN    char    aspect[MAXRESETS+1][MAXVIEWIDCHAR+1];
EXTERN    int     Lreset[MAXRESETS+1],Lview[MAXRESETS+1],iviewset;
EXTERN    int     Laspect[MAXPOINTASPECTS+2];
 /*aspects numbered  0 OFF,1---MAXPOINTASPECTS, +on_screen_flag*/

EXTERN    int        Lnewpalette;
EXTERN    int        Lmultibins,multibins;
EXTERN    int        L5depths,L3depths,Nbins,Ndepths;
EXTERN    int        Lstartfile;
EXTERN    int        Lnewlist;
EXTERN    float          stereoangle,defaultangle;
EXTERN    int            ieyewide, ieyedefault;
EXTERN    int            ieyeposition, ieyeposdefault;
EXTERN    int        Lmiddle;
EXTERN    int        xmin,xmax,ymin,ymax;
EXTERN    int        Lcaptsize,Ltextsize;
EXTERN    int        Lunpickable;
EXTERN    float      shortenline;
EXTERN    float      fcenterx,fcentery,fcenterz;
EXTERN    int        Lkeepstereo,Lkeepthinline,Lkeepperspective;
EXTERN    int        Lsearchbegin,Lsearched,Lsearchatend,Lsearchstatus;
EXTERN    int        ngrpsearch,nsetsearch,nlistsearch;
EXTERN    int        isearch1,isearch2;
EXTERN    char trystr[256],search1str[256],search2str[256],searchstatusstr[256];
EXTERN    int        Lfindwholeword; /*051004*/

EXTERN long ntotaltext; /*920720 dcr total number of chars in text window*/
EXTERN int ireturnflag; /*940125 lets dialog return a T/F value */
EXTERN    float     floatpass; /*global to pass a float*/
EXTERN    int       intpass, int2pass, int3pass; /*globals to save/pass ints */

EXTERN    int    origlistcolor,origpointcolor,trialcolor;

#ifdef PCWINDOWS
  EXTERN LPSTR filearg
#ifdef  MAGE_INIT
    = NULL
#endif
  ;
  EXTERN  char szDirName[256];   /*current directory */
  EXTERN HINSTANCE hInst;          /* current instance   */
  EXTERN char *textbuf;
  
  EXTERN HWND MageWindow;
  EXTERN HACCEL MageAccel;
  EXTERN int DisplaySizeX;
  EXTERN int DisplaySizeY;
  EXTERN int fontheight;
  EXTERN int fontwidth;
  EXTERN int fontmaxwidth;
  /*EXTERN int fontverticalspace;*/ /*need this for MAGEDRAW.C*/
  EXTERN char ofilename[FILENAME_MAX];
  EXTERN char startpath[_MAX_PATH];
  
  EXTERN HDC cDC, DC, newDC;
  EXTERN HDC tableDC;
  
#define MAGEGRAPHID  1  /*child windows of MageWindow*/
#define MAGETEXTID   2  /*must be unique at parent-child level*/
#define MAGESCROLLID 3  /* 3 scroll bars: zoom, zslab, ztran */
#define MAGETABLEID  4  /*Table Window*/

  EXTERN HWND gWindow;
  EXTERN HMENU gMenu;
  EXTERN HWND tWindow;
  EXTERN HMENU tMenu;

  EXTERN HWND captionWindow;
  
  EXTERN HWND sWindow;
  EXTERN HWND slabWindow;
  EXTERN HWND ztranWindow;
  EXTERN HWND zoommvWindow,zslabmvWindow,ztranmvWindow; /* to set values*/
  /*so input value of zoom, izclip, iztran will position the slider bars */
  EXTERN HWND zoomedWindow,zslabedWindow,ztranedWindow; /* to set values*/
  /*so input of zoom,izclip,iztran will be set text value of the slider bars*/

  EXTERN LOGPALETTE *lpPalette
#ifdef  MAGE_INIT
   = NULL
#endif
  ;

  EXTERN  int xmiddle,ymiddle;
  EXTERN  int polynum;

  EXTERN  int hasnewDC;
  EXTERN  HBITMAP defBitmap, newBitmap;
  EXTERN  HANDLE hBckGrndBrush;
  EXTERN  int tracking;
  EXTERN  int xold, yold;
  EXTERN  HPEN oldPen,newPen;
  EXTERN  HBRUSH oldBrush,newBrush;

#endif   /* PC stuff */

EXTERN int Initialized;
EXTERN int SliderWidth;
EXTERN int fontverticalspace;
EXTERN short  oldfontsize,labelsfontsize,infofontsize,wordsfontsize;
EXTERN short  Loldfontsize,Llabelsfontsize,Linfofontsize,Lwordsfontsize;
EXTERN short  tablefontsize,Ltablefontsize;
EXTERN int littlefontsize,mediumfontsize,largefontsize;
#ifdef MACINTOSH
  EXTERN    RGBColor rgbColor;
  EXTERN    int   hexappleOS;
#else
  EXTERN  struct { unsigned red; unsigned green; unsigned blue; } rgbColor;
#endif

#ifdef MACINTOSH
EXTERN WindowRecord grafwindowrecord,textwindowrecord,captwindowrecord;
#endif

#include "MAGEhdr.h"

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

