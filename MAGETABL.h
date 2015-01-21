/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 
#ifdef  MAGETABL
#undef  MAGETABL
#define MAGETABL
#else
#define MAGETABL extern
#endif


MAGETABL int  MAXROW;
MAGETABL int  MAXCOL;
MAGETABL int  tableheaderMAXCOL;
MAGETABL int  Ltableblackbkg;

 /* MAGE.H for UNIX_MOTIF: */
 /* typedef Widget  WindowPtr; */
 /* typedef Widget  ControlHandle; */

 /* MAGE.H for PCWINDOWS: */
 /* typedef HWND  WindowPtr; */
 /* typedef HWND  ControlHandle; */

MAGETABL WindowPtr  tableWindow;
MAGETABL ControlHandle  vtableScroll,htableScroll;

#ifdef UNIX_X11
MAGETABL Widget toptable,tabledrawArea;
MAGETABL Window tabledrawWindow;
MAGETABL GC tablegc;
MAGETABL Display* tabledpy; 
#endif

MAGETABL int  Ltablepresent,Ltableready,Ltablesearchcells,Ltablesearchgraphics;
MAGETABL int  tablewrapcol; /*number of columns desired across the table*/
MAGETABL float  tablecolscale; /*factor to scale col size*/
MAGETABL float  tablemarkradius;
MAGETABL float  tablefanradius;
MAGETABL float  tablefancore;
MAGETABL int  Ltablepicked;
MAGETABL int  Ltablegraphpicked;
MAGETABL int  Ltablegroupgraphed;
MAGETABL int  Ltablepickactive;
MAGETABL int  Ltablecellispicked,Ltableextrapass;
MAGETABL int  Ltablecellgetwidth;
MAGETABL int  tablepickx;
MAGETABL int  tablepicky;
MAGETABL int  tablewide, tablewindowwide, tablezerowide, tablecolwidth;
MAGETABL int  tablehigh, tablewindowhigh, tablezerohigh, tablerowheight;
MAGETABL int  tablelasty, tablerowstack, tablewordwidth;
MAGETABL int  Ltablecellnewlypicked;
MAGETABL int  Ltablewrapdecades; /*010916*/
/*prototypes*/
void drawtable(int);   /*MAGETABL.c*/

void setuptable(void);  /*MAGETABL.c*/
void loadtable(int);   /*MAGETABL.c*/
void createtable(void); /*MAGETABL.c*/
void constructtablecells(void); /*MAGETABL.c*/
void tablecelldelete(void);  /*MAGETABL.c*/
void restoredeletedpoint(void);  /*MAGETABL.c*/
void tablecellinsert(int);  /*MAGETABL.c*/
void tablerebuild(void); /*MAGETABL.c*/
void tablecolor(void); /*MAGETABL.c*/
void alloctablearray(void); /*MAGETABL.c*/
void cleanuptable(void); /*MAGETABL.c*/
void redrawtable(void); /*____DRAW.c*/
void dodrawtable(int); /*____DRAW.c*/
void stringtotable(char[256],int,int,int); /*____DRAW.c*/
int  wordstotable(char[256],int,int,int,int,int); /*____DRAW.c*/
void rectangletoscreen(int,int,int,int, int); /*____DRAW.c*/
void SetUptableWindow(void); /*____INIT.c*/
void settableScrollBars(void); /*____WIND.c*/
void createtablescrollbars(void); /*MACINIT.c, MUXMTABL.c*/
int  searchtablecells(int); /*MAGEDLOG.c*/ /*000324*/
void addtablemarker(pointstruct*);
void killtablemarkers(void);
void removetablemarkpoints(void);
int  markfromtablecellsID(int);

#ifdef   MACINTOSH 
void DotableContentClick(Point); /*____WIND.c*/
pascal void tableScroller(ControlHandle,short); /*____WIND.c*/
#endif /*MACINTOSH*/

#ifdef  UNIX_MOTIF
XtCallbackProc tablepick_CB(Widget, caddr_t, XmDrawingAreaCallbackStruct *);
XtCallbackProc tabledrawArea_CB(Widget, XtPointer, XtPointer);
XtCallbackProc tableresize_CB(Widget, XtPointer, XtPointer);
XtCallbackProc htablescroll_CB(Widget, XtPointer, XtPointer);
XtCallbackProc vtablescroll_CB(Widget, XtPointer, XtPointer);
void resettablescrollbar(ControlHandle *,int);
void maketablescrollbar(Widget *,int,int,int,int,int,int,char);
#endif  /*UNIX_MOTIF*/

