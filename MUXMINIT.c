/*MUXMINIT.c*/  /* MOTIF widgets */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*
  mainsetup
  The mainsetup routine creates the widget hierarchy for the program.
  The widget hierarchy used here is:

  (class topLevelShell)
     |
     +-- form (class Form)
           |
	   +--- drawArea (class DrawingArea)
	   |

  The Form widget is used for organizing its child widgets.
  The drawing area widget is used for X11 (or other) draws. 
  The MAGE toggle buttons are used to control the parts of the image displayed.
  The menu items control other functions like file select and quit operations.

  Any necessary resources for the widgets are specified here in the
  program, which override any user defaults or command-line options.  Note
  that this is not very friendly to the user who may want to change the
  font of the push buttons, but MAGE traditionally has had to control its
  own appearance, and it has its own requirments for real estate usage.  

  The display connection (dpy) and window ID (drawWindow) of the drawing
  area widget are available after the widget hierarchy has been realized.
*/
/* 
      Once upon a time this file started with code bundled with the
  ESV, though it has been hacked much beyond recognition, some of the
  motif calls and setups here owe recognition to:
       		Rich Thomson
                Design Systems Division
		Evans & Sutherland Computer Corporation
         Date:  Thursday, June 12th, 1990
       Modified:  J. Buckmiller   Mar 1991.   Approved C binding
  Dave Richardson, June 1994
*/
  
#include "MAGE.h"       
#include "MAGEMENU.h"
#include "MAGELIST.h"
#include "MAGETABL.h"
#include "MAGESYNC.h"

#ifdef UNIX_PEX
 /* Historical code for the ESV, archive as example of using external dials*/
 /* these event handlers take care of blanking and restoring the dial labels */
 extern void enter_handler(Widget, caddr_t, XEvent *,Boolean *); /*MUXPHIG.c*/
 extern void leave_handler(Widget, caddr_t, XEvent *,Boolean *); /*MUXPHIG.c*/
 /*defined for drawArea*/
#endif /*UNIX_PEX*/

#ifdef UNIX_X11
#include <X11/cursorfont.h>
#include <Xm/Protocols.h>
#endif /*UNIX_X11*/

static CursHandle    magecursorB;   /*Bitmap cursors*/
static CursHandle    mage3DcursorB;
static CursHandle    mage2DcursorB;
static CursHandle    mageTE2DcursorB;
static CursHandle    mageTE3DcursorB;
static CursHandle    magexy3DcursorB;
static CursHandle    magez3DcursorB;
static CursHandle    magexy2DcursorB;

static CursHandle    magecursorF;   /*Standard Font Cursors  for LINUX*/
static CursHandle    mage3DcursorF;
static CursHandle    mage2DcursorF;
static CursHandle    mageTE2DcursorF;
static CursHandle    mageTE3DcursorF;
static CursHandle    magexy3DcursorF;
static CursHandle    magez3DcursorF;
static CursHandle    magexy2DcursorF;
int allocextrafilenames(void); /*021211*/

/****allocextrafilenames()***************************************************/
int allocextrafilenames() /*021211*/
{
   int iOK=1,n=0;
   if(Nextrafile <= MAXEXTRA)
   {
      for(n=0; n<Nextrafile; n++)
      {
         ExtraStr[n] = (char *)malloc(sizeof(char)*256);
         if(ExtraStr[n]==NULL) {iOK = 0; break;}
         else {ExtraStr[n][0] = '\0';}
      }
   }
   return(iOK);
}
/*___allocextrafilenames()__________________________________________________*/

/****CompArgStr()***********jmw/utility***************************************/
int CompArgStr(char *str, char *arg, int min) 
{
   int i, max;
   char s, a;

   if (!str || !arg) return 0;

   max = strlen(arg);

   for(i=0; i<max; i++) 
   {
      s = toupper(str[i]);
      a = toupper(arg[i]);

      if (i >= min && (   s == '\0' || s == ',' || s == '(' || s == '.'
                       || s == '+'  || s == '-' || isdigit(s))) 
      {
          break; /* good ending point */
      }
      else if (s != a) 
      {
          i = 0; /* failed to match */
          break;
      }
   }
   return i;
}
/*___CompArgStr()____________________________________________________________*/

/****mainsetup()*************************************************************/
void    mainsetup(int *argc, char** argv)
{
  int     copy_of_argc, argv_size;
  char**  copy_of_argv;

  int    ncommon = 0;
  int scrHeight = 0;
  int scrWidth = 0;
  int scrsize = 0;
  
  char *p;  /*030505*/
  static int  i=0, nname=0; /*030505*/

  Lfontinfo = 0; /*normally ==0 NOT print info about font sets 031025*/
  Lmacularfont = 0; /*031025 menu,text,... font size can be set by commandline*/
  Lforcedfont = 0; /*040225 menu font size can be set by commandline*/
  LGUI = 1; /*usual to have GUI  030505*/
  LGUINO = 0; /*GUI-less operation number*/
  Lpostscript = 0; /*possible GUI-less controls*/
  Lhelpstdout = 0; /*031011*/
  Lchangesstdout = 0; /*031011*/
  Lsocketsame = 0; /*001222*/
  Lsockettrigger = 0; /*040106*/
  Lsocketreturn = 0; /*040106*/
  Lsinglemarker = 0; /*040106*/
  Lgwidthset = 0; /*040218*/
  
  Lcarry = 0;
  Lnewfile = 0; /*no file in mind*/
  Lstartfile = 0; /*mage not launched by a file*/
  Nextrafile = 0;
  Lextrafile = 0;

  getmageversion(); /*common version number: MAGEINIT.c */


    IMAC = 0; /*flow control for Macintosh computers */
    IPC  = 0;  /*flow control for PC computers with windows */
    IESV = 0;  /*flow control for E&S ESV Unix with  Xwindows */
    IUXM = 1;  /*flow control for  Unix with Xwindows and Motif*/
    IUX  = 1;  /*flow control for  Unix with Xwindows*/
    IUXP = 0;  /*flow control for  Unix with Xwindows and PEX*/
  mageinitialvalues(); /*mageinit.c*/ /*in case need MAGE parameters*/

  fprintf(stderr,"%s\n",version);

  /*Initialize X toolkit the long way so can mess with color palette */
  XtToolkitInitialize();
  app_context = XtCreateApplicationContext();
  /*copy commandline parameters*/
  copy_of_argc = *argc;
  argv_size = copy_of_argc * (sizeof(char*));
  copy_of_argv = (char**)XtMalloc(argv_size);
  memcpy(copy_of_argv, argv, argv_size);


  /****parsecommandline()   this part is nearest thing to this function ****/
  /*this comes after initialvalues so commandline can over-ride... 051114*/

/*121222*/
/*
  fprintf(stderr,"MUXMINIT/mainsetup *argc== %d\n",*argc);
  for(i=0; i<*argc; i++)
  {
     fprintf(stderr,"argv[%d] == %s\n",i,argv[i]);
  }
*/


  if(*argc>1) /*parsecommandline*/
  {/*commandline probably has file name, or something... use parser*/
     for(i=1; i<*argc; i++)
     {/*loop over arguments*/
        p = argv[i];
        if(p[0] == '-')
        {/*flag*/
           if(p[1] == '\0')
           {/*naked flag ignored*/
              ;
           }/*naked flag ignored*/
           else
           {/*interpret flag*/
              if(CompArgStr(p+1,"postscript", 10))
              {
                 LGUI = 0; /* 030505*/
                 LGUINO = Lpostscript = 1; /*  030505*/
                 Lraster3Drender=0; Lcolor=1; LLcolor=1; LCscrn=1;/*LGUI030505*/
              }
              else if(CompArgStr(p+1,"error", 3)) /*051114*/
              {
                 Lallocerror = 1; /*report allocation error 051114*/
              }
              else if(CompArgStr(p+1,"help", 1))
              {
                 LGUI = 0; 
                 LGUINO = Lhelpstdout = 1; /*031011*/
              }
              else if(CompArgStr(p+1,"changes", 7))
              {
                 LGUI = 0; 
                 LGUINO = Lchangesstdout = 1; /*031011*/
              }
              else if(CompArgStr(p+1,"report", 6))
              {
                 LGUI = 0; 
                 LGUINO = Lreportstderr = 1; /*070901*/
              }
              else if(CompArgStr(p+1,"jeremy", 6))
              {
                 Lmacularfont = 1;/*large menu giant text font crt screen size*/
              }
              else if(CompArgStr(p+1,"macular", 7))
              {
                 Lmacularfont = 1;/*large menu giant text font crt screen size*/
              }
              else if(CompArgStr(p+1,"show", 4))
              {
                 Lforcedfont = 5; /*show fonts for 1024x768*/
              }
              else if(CompArgStr(p+1,"giant", 5))
              {
                 Lforcedfont = 4; /*giant fonts*/
              }
              else if(CompArgStr(p+1,"large", 5))
              {
                 Lforcedfont = 3; /*large fonts*/
              }
              else if(CompArgStr(p+1,"medium", 6))
              {
                 Lforcedfont = 2; /*medium fonts*/
              }
              else if(CompArgStr(p+1,"small", 5))
              {
                 Lforcedfont = 1; /*small (old default) fonts*/
              }
              else if(CompArgStr(p+1,"fontinfo", 8))
              {
                 Lfontinfo = 1;
              }
              else if(CompArgStr(p+1,"IDsearch", 8))
              {
                 Lsocketidsearch = 1;
                 Lsinglemarker = 1;
              }
              else if(CompArgStr(p+1,"socket1", 7))
              {
                 Lsocketsame = 1;
                 Lsockettrigger = 1;
              }
              else if(CompArgStr(p+1,"socket2", 7))
              {
                 Lsocketsame = 2;
                 Lsockettrigger = 1;
              }
              else if(CompArgStr(p+1,"socket3", 7))
              {
                 Lsocketsame = 3;
                 Lsockettrigger = 1;
              }
              else if(CompArgStr(p+1,"gwidth", 6))
              {
                 i = i+1; /*number is in next character string*/
                 p = argv[i];
                 sscanf(p,"%d",&gwidthset);
                 Lgwidthset = 1;
              }
              else if(CompArgStr(p+1,"dumpprobe", 9)) /*061222 probe atoms*/
              {
                 Ldumpprobe = 1;  /*061222*/
              }

           }/*interpret flag*/
        }/*flag*/
        else
        {/*presume an input file name*/
           nname++;
           if(nname == 1)
           {/*THE kinemage file name*/
              strcpy(NameStr,argv[i]); /*copy name into MAGE input file Name*/
              Lnewfile=1;   /*file is present*/
              Lstartfile=1; /*file launched mage, use it*/
           }
           else if(nname > 1)
           {/*extra file names*/
              if(nname == 2)
              {/*first of the expected extra files*/
                 Nextrafile = *argc-2; /*max number that could be expected*/
                 if( allocextrafilenames() ) 
                 {/*space allocated for extra file names*/
                    Lextrafile = 1;
                 }
                 else
                 {/*can't handle extra file names*/
                    Nextrafile = 0;
                    Lextrafile = 0;
                 }
              }/*first of the expected extra files*/
              if(Lextrafile)
              {
                 if( (nname-2) < Nextrafile)
                 {/*extra files start at nname==2 which is stored as zeroth*/
                   strcpy(ExtraStr[nname-2],argv[i]);/*rack up extra file name*/
                 }
              }
           }/*extra file names*/
        }/*presume an input file name*/
     }/*loop over arguments*/
     Nextrafile = nname - 1; /*readjust so scan for append files OK*/
  }/*commandline probably has file name, or something...*/

if(LGUI)
{/*LGUI*/
     
  /*open display connection*/
  dpy = XtOpenDisplay(app_context,
        NULL,
        NULL,
        "MAGE",
        (XrmOptionDescList) NULL,
        0,
        argc,
        argv);

  /*Now can set visual stuff before opening top shell */
  scrHeight = DisplayHeight( dpy, DefaultScreen(dpy));
  scrWidth  = DisplayWidth ( dpy, DefaultScreen(dpy));
  if(scrHeight < scrWidth) {scrsize = scrHeight;}
  else {scrsize = scrWidth;}
  /*this duplicate some of the static initializations in MAGEINIT.c*/
  if(scrsize > 875)
  {
      GWIDTH  = 800;  /* UNIX effective graphics area window width */
      GHEIGHT = 800;  /* graphics window height*/
  }
  else if(scrsize > 675)
  {
      GWIDTH  = 600;  /* UNIX effective graphics area window width */
      GHEIGHT = 600;  /* graphics window height*/
  }
  else
  {
      GWIDTH  = 400;  /* original Mac effective graphics area window width */
      GHEIGHT = 400;  /* graphics window height*/
  }
  if(Lforcedfont == 5) /*040225*/
  {/*optimize for show on 1024x768 projector*/
      GWIDTH  = 700;
      GHEIGHT = 700;
  }
  if(Lgwidthset) /*040218*/
  {/*commandline over-ride*/
      GWIDTH  = gwidthset;
      GHEIGHT = gwidthset;
  }
  WIDTH =  GWIDTH; /*GWIDTH & GHEIGHT adjusted when make grafWindow*/
  HEIGHT = GHEIGHT;/* WIDTH & HEIGHT reset then also */
  if(GWIDTH<GHEIGHT) oldmaxwide = GWIDTH;
  else               oldmaxwide = GHEIGHT;
  
  GZDEEP = oldmaxwide;
  ieyewide = GWIDTH/2;
  /*Scaling based on 400 for 1990's small-screen (640x480) computers*/
  /*std screen box 400x400x400, 0 at ctr, zclip 200 from ctr*/
  /*fzclip = 1.0;*/ /*factor==1.0 for std box, scale if screen box resized */
  fzclip = (float)GZDEEP/(float)400;/*std izclip defined in std screen box*/
  /*fzclip mediates between internal izclip value adjusted for box expansion*/
  /* and external izclip which pretends to be in a 400x400x400 box */
  izclipold[0] = 200;
  izclip = fzclip*izclipold[0]; /*refudge izclip*/
  iztranold[0] = 0;
  iztran = fzclip*iztranold[0]; /*refudge iztran*/
  Loriginatctr = 0;
  ixctr = GWIDTH/2;
  iyctr = GHEIGHT/2;
  izctr = GZDEEP/2;

/*font designation used here used to be:*/
/*"-*-Helvetica-Bold-R-Normal--12*" */
/*"-*-Helvetica-Bold-R-Normal--14*" */
/*"-*-Helvetica-Bold-R-Normal--18*" */
/*"-*-Helvetica-Bold-R-Normal--24*" */
/*see MUXMBBOX.c discussion of font designations, like:*/
/*"*helvetica-bold-r-*-100*" */
   tinyfont = NULL;
   littlefont = NULL;
   mediumfont = NULL;
   largefont = NULL;
   giantfont = NULL;
   buttonfont = NULL;
   menufont = NULL;
   textfont = NULL;
   tinyfontList = NULL;
   littlefontList = NULL;
   mediumfontList = NULL;
   largefontList = NULL;
   giantfontList = NULL;
   buttonfontList = NULL;
   menufontList = NULL;
   textfontList = NULL;
/*Define various size fonts*/ /*121108 extra () to avoid = vs == warning*/
/*121108 using if with assignment to test existance of fonts*/ 
if((tinyfont = XLoadQueryFont(dpy,"*helvetica-medium-r-*-80*")))
{
   tinyfontList = XmFontListCreate(tinyfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo) {fprintf(stderr,"tiny font *helvetica-medium-r-*-80*, %ld\n",(long)tinyfont);}
}

/*define little font*/

if((littlefont = XLoadQueryFont(dpy, "*helvetica-bold-r-*-120*") ))
{
    littlefontList = XmFontListCreate(littlefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"little font *helvetica-bold-r-*-120*, %ld\n",(long)littlefont);}
}
else if((littlefont = XLoadQueryFont(dpy, "*helvetica-bold-r-*-140*") ))
{
    littlefontList = XmFontListCreate(littlefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"little font *helvetica-bold-r-*-140*, %ld\n",(long)littlefont);}
}
else if((littlefont = XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--15*") ))
{
    littlefontList = XmFontListCreate(littlefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"little font fixed-bold-r 15, %ld\n",(long)littlefont);}
}
else if((littlefont = XLoadQueryFont(dpy, "-*-fixed--r-normal--15*") ))
{
    littlefontList = XmFontListCreate(littlefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"little font fixed--r 15, %ld\n",(long)littlefont);}
}

/*find medium font*/

if((mediumfont = XLoadQueryFont(dpy, "*helvetica-bold-r-*-140*") ))
{
    mediumfontList = XmFontListCreate(mediumfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"medium font *helvetica-bold-r-*-140*, %ld\n",(long)mediumfont);}
}
else if(( mediumfont =
       XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--14*") ))
{
    mediumfontList = XmFontListCreate(mediumfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"medium font fixed-bold-r-normal--14, %ld\n",(long)mediumfont);}
}
else if(( mediumfont =
       XLoadQueryFont(dpy, "-*-fixed--r-normal--14*") ))
{
    mediumfontList = XmFontListCreate(mediumfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"medium font fixed--r-normal--14, %ld\n",(long)mediumfont);}
}

/*find large font*/

if((largefont = XLoadQueryFont(dpy, "*helvetica-bold-r-*-180*") ))
{
    largefontList = XmFontListCreate(largefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"large font *helvetica-bold-r-*-180*, %ld\n",(long)largefont);}
}
else if(( largefont =
       XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--18*") ))
{
    largefontList = XmFontListCreate(largefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"large font fixed-bold-r-normal--18, %ld\n",(long)largefont);}
}
else if(( largefont =
       XLoadQueryFont(dpy, "-*-fixed--r-normal--18*") ))
{
    largefontList = XmFontListCreate(largefont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"large font fixed--r-normal--18, %ld\n",(long)largefont);}
}

/*find giant font*/

if ((giantfont = XLoadQueryFont(dpy, "*helvetica-bold-r-*-240*") ))
{
    giantfontList = XmFontListCreate(giantfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"giant font *helvetica-bold-r-*-240*, %ld\n",(long)giantfont);}
}
else if(( giantfont = XLoadQueryFont(dpy, "-*-fixed-bold-r-normal--24*") ))
{
    giantfontList = XmFontListCreate(giantfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
    {fprintf(stderr,"giant font fixed-bold-r-normal--24, %ld\n",(long)giantfont);}
}
else if(( giantfont = XLoadQueryFont(dpy, "-*-fixed--r-normal--24*") ))
{
    giantfontList = XmFontListCreate(giantfont, XmSTRING_DEFAULT_CHARSET);
   if(Lfontinfo)
   {fprintf(stderr,"giant font fixed--r-normal--24, %ld\n",(long)giantfont);}
}

/*test logic incomplete: presumes littlefont is most likely to be defined*/
if(tinyfont == NULL)
{
   if(littlefont != NULL)
   {
      tinyfont = littlefont;
      tinyfontList = littlefontList;
      if(Lfontinfo) {fprintf(stderr,"tiny font == little font\n");}
   }
}
if(littlefont == NULL)
{
   if(Lfontinfo) {fprintf(stderr,"little font not defined\n");}
}
if(mediumfont == NULL)
{
   if(littlefont != NULL)
   {
      mediumfont = littlefont;
      mediumfontList = littlefontList;
      if(Lfontinfo) {fprintf(stderr,"medium font == little font\n");}
   }
}
if(largefont == NULL) 
{
   if(mediumfont != NULL)
   {
      largefont = mediumfont;
      largefontList = mediumfontList;
      if(Lfontinfo) {fprintf(stderr,"large font == medium font\n");}
   }
   else if(littlefont != NULL)
   {
      largefont = littlefont;
      largefontList = littlefontList;
      if(Lfontinfo) {fprintf(stderr,"large font == little font\n");}
   }
}
if(giantfont == NULL) 
{
   if(largefont != NULL)
   {
      giantfont = largefont;
      giantfontList = largefontList;
      if(Lfontinfo) {fprintf(stderr,"giant font == large font\n");}
   }
   if(mediumfont != NULL)
   {
      giantfont = mediumfont;
      giantfontList = mediumfontList;
      if(Lfontinfo) {fprintf(stderr,"giant font == medium font\n");}
   }
   else if(littlefont != NULL)
   {
      giantfont = littlefont;
      giantfontList = littlefontList;
      if(Lfontinfo) {fprintf(stderr,"giant font == little font\n");}
   }
}
if(littlefont)
{/*some font defined, adjust size used in various places*/
   /*define title,menu,button,text in terms of tiny,little,medium,large,giant*/
   /*Determine what size is to be used:*/
   /*GWIDTH determines size unless over-ridden on commandline*/
   /*(gwidth can also be set on the commandline*/
   /*macular allows a choice based on GWIDTH*/

   if(Lmacularfont || Lforcedfont)
   {/*Lmacularfont*/
      /* BWIDTH = 125; */
      /* ZWIDTH =  45; */
      /* ALERTHEIGHT = 150 of each alertstr panel*/
      /* FONTSIZE = 20; */
      if(Lforcedfont == 4 || (Lmacularfont==1 && GWIDTH > 875) )
      {
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 150; /*space for group...list buttons*/
         BUTTWINWIDTH = 150; /*space for group...list buttons*/
         ZWIDTH = 70; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 195; /* of each alertstr panel*/
         FONTSIZE = 31;
         titlefont = giantfont;
         titlefontList = giantfontList;
         menufont = giantfont;
         menufontList = giantfontList;
         buttonfont = giantfont;
         buttonfontList = giantfontList;
         textfont = giantfont;
         textfontList = giantfontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using giant font for everything\n");
         }
      }
      else if(Lforcedfont == 3)
      {
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 150; /*space for group...list buttons*/
         BUTTWINWIDTH = 150; /*space for group...list buttons*/
         ZWIDTH = 60; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 170; /* of each alertstr panel*/
         FONTSIZE = 25;
         titlefont = largefont;
         titlefontList = largefontList;
         menufont = largefont;
         menufontList = largefontList;
         buttonfont = largefont;
         buttonfontList = largefontList;
         textfont = largefont;
         textfontList = largefontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using large font for everything\n");
         }
      }
      else if(Lforcedfont == 2)
      {/*use medium fonts and spacing*/
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 125; /*space for group...list buttons*/
         BUTTWINWIDTH = 125; /*space for group...list buttons*/
         ZWIDTH = 45; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 150; /* of each alertstr panel*/
         FONTSIZE = 20;
         titlefont = mediumfont;
         titlefontList = mediumfontList;
         menufont = mediumfont;
         menufontList = mediumfontList;
         buttonfont = mediumfont;
         buttonfontList = mediumfontList;
         textfont = mediumfont;
         textfontList = mediumfontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using medium font for everything\n");
         }
      }
      else if(Lforcedfont == 1)
      {/*use little fonts and spacing*/
         ROTWINDWIDTH = 225; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 125; /*space for group...list buttons*/
         BUTTWINWIDTH = 125; /*space for group...list buttons*/
         ZWIDTH = 35; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 150; /* of each alertstr panel*/
         FONTSIZE = 20;
         titlefont = littlefont;
         titlefontList = littlefontList;
         menufont = littlefont;
         menufontList = littlefontList;
         buttonfont = littlefont;
         buttonfontList = littlefontList;
         textfont = littlefont;
         textfontList = littlefontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using little font for everything\n");
         }
      }
      else if(Lforcedfont == 5)
      {/*show==projector 1024x768 fonts and spacing*/
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 125; /*space for group...list buttons*/
         BUTTWINWIDTH = 125; /*space for group...list buttons*/
         ZWIDTH = 60; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 170; /* of each alertstr panel*/
         FONTSIZE = 25;
         titlefont = largefont;
         titlefontList = largefontList;
         menufont = largefont;
         menufontList = largefontList;
         buttonfont = largefont;
         buttonfontList = largefontList;
         textfont = largefont;
         textfontList = largefontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using show fonts\n");
         }
      } 
      else
      {/*largish compromise used as default 031127*/
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 200; /*space for group...list buttons*/
         BUTTWINWIDTH = 200; /*space for group...list buttons*/
         ZWIDTH = 65; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 170; /* of each alertstr panel*/
         FONTSIZE = 25;
         titlefont = largefont;
         titlefontList = largefontList;
         menufont = largefont;
         menufontList = largefontList;
         buttonfont = giantfont;
         buttonfontList = giantfontList;
         textfont = giantfont;
         textfontList = giantfontList;
         if(Lfontinfo)
         {
            fprintf(stderr,"using large font for menus, giant for text\n");
         }
      }
   }/*Lmacularfont*/
   else
   {/*normal vision version*/
      if(GWIDTH <= 400)
      {/*use smallest fonts and spacing*/
         ROTWINDWIDTH = 200; /*200  bondrotsliders in separate UNIX window*/
         BWIDTH = 90; /*space for group...list buttons*/
         BUTTWINWIDTH = 90; /*space for group...list buttons*/
         ZWIDTH = 30; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 100; /* of each alertstr panel*/
         FONTSIZE = 20;
         titlefont = tinyfont;
         titlefontList = tinyfontList;
         menufont = tinyfont;
         menufontList = tinyfontList;
         buttonfont = littlefont;
         buttonfontList = littlefontList;
         textfont = littlefont;
         textfontList = littlefontList;
      }
      else if(GWIDTH <= 600)
      {/*use smaller fonts and spacing*/
         ROTWINDWIDTH = 225; /*200  bondrotsliders in separate UNIX window*/
         /*BWIDTH = 150;*/ /*space for group...list buttons*/
         /*BWIDTH = 135;*/ /*space for group...list buttons*/
         BWIDTH = 180; /*space for group...list buttons, need 14char 070831*/
         BUTTWINWIDTH = 135; /*space for group...list buttons*/
         ZWIDTH = 40; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 150; /* of each alertstr panel*/
         FONTSIZE = 20;
         titlefont = mediumfont;
         titlefontList = mediumfontList;
         menufont = mediumfont;
         menufontList = mediumfontList;
         buttonfont = mediumfont;
         buttonfontList = mediumfontList;
         textfont = mediumfont;
         textfontList = mediumfontList;
      }
      else if(GWIDTH <= 800)
      {/*use littlish fonts and spacing*/
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         /*BWIDTH = 135;*/ /*space for group...list buttons*/
         BWIDTH = 180; /*space for group...list buttons, need 14char 070831*/
         BUTTWINWIDTH = 135; /*space for group...list buttons*/
         ZWIDTH = 45; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 150; /* of each alertstr panel*/
         FONTSIZE = 20;
         titlefont = mediumfont;
         titlefontList = mediumfontList;
         menufont = largefont;
         menufontList = largefontList;
         buttonfont = mediumfont;
         buttonfontList = mediumfontList;
         textfont = mediumfont;
         textfontList = mediumfontList;
      }
      else
      {/*could use largest fonts and spacing, but don't*/
         ROTWINDWIDTH = 250; /*200  bondrotsliders in separate UNIX window*/
         /*BWIDTH = 150;*/ /*space for group...list buttons*/
         BWIDTH = 180; /*space for group...list buttons, need 14char 070831*/
         BUTTWINWIDTH = 150; /*space for group...list buttons*/
         ZWIDTH = 65; /*space for zoom,clip... bars*/
         ALERTHEIGHT = 170; /* of each alertstr panel*/
         FONTSIZE = 25;
         titlefont = mediumfont;
         titlefontList = mediumfontList;
         menufont = largefont;
         menufontList = largefontList;
         buttonfont = largefont;
         buttonfontList = largefontList;
         textfont = largefont;
         textfontList = largefontList;
      } 
   }/*normal vision version*/
}/*some font defined, adjust size used in various places*/
   if(Lfontinfo)
   {
      fprintf(stderr,"screen Height== %d, Width== %d\n",scrHeight,scrWidth);
      fprintf(stderr,"actual GWIDTH %d, BWIDTH %d, ZWIDTH %d\n",GWIDTH,BWIDTH,ZWIDTH);
      fprintf(stderr,"title font: %ld\n",(long)titlefont);
      fprintf(stderr,"menu font: %ld\n",(long)menufont);
      fprintf(stderr,"text font: %ld\n",(long)textfont);
      fprintf(stderr,"button font: %ld\n",(long)buttonfont);
   }

labelsfontsize = 14;
wordsfontsize = 14;
infofontsize = 14;
oldfontsize = 14;
littlefontsize = 14;
mediumfontsize = 18;
largefontsize = 24;



/*discover Visual attributes of this computer */
#ifdef UNIX_PEX
  magevisual = DefaultVisual(dpy,DefaultScreen(dpy));
  magedepth = DefaultDepth(dpy,DefaultScreen(dpy));
  colormap = DefaultColormap(dpy,DefaultScreen(dpy));
      windowbackground = WhitePixel(dpy,DefaultScreen(dpy));
      windowborder = BlackPixel(dpy,DefaultScreen(dpy));
#else /*ordinary X11, not PEX*/

  finddisplaytype(); /*calls makexcolors(), renamed 140912*/ /*MUX_COLR.c*/

  /*full white*/
  /*yellowtint4*/
  /*yellow0*/
  /*brown4*/
  /*brown0w*/
#ifdef OLDPINKBROWNSURROUND
  if(incolored[numberofcolor("brown")+25]) /*brown0W*/
  {
      windowbackground = incolor[numberofcolor("brown")+25];
  }
#endif
  if(incolored[numberofcolor("white")+50*2])
  {
      windowbackground = incolor[numberofcolor("white")+50*2];
  }
  else
  {
      windowbackground = WhitePixel(dpy,DefaultScreen(dpy));
  }
  if(incolored[0]) /*incolor for black can be == 0 !! */
  {
      windowborder = incolor[0];
      windowforeground = incolor[0];
  }
  else
  {
      windowborder = BlackPixel(dpy,DefaultScreen(dpy));
      windowforeground = BlackPixel(dpy,DefaultScreen(dpy));
  }
if(Ltest)
{
printf("incolor[255]==%ld, incolor[0]==%ld\n"
,incolor[255],incolor[0]);
printf("WhitePixel==%ld, BlackPixel==%ld\n"
,WhitePixel(dpy,DefaultScreen(dpy)),BlackPixel(dpy,DefaultScreen(dpy)) );
printf("windowbackground==%ld, windowforeground==%ld\n" /*121108 pixel %ld */
,windowbackground,windowforeground);
}
  highlightcolor = incolor[numberofcolor("green")+50*2]; /*green2*/
  topshadowcolor = incolor[numberofcolor("gray")]; /*gray0*/
  bottomshadowcolor = incolor[numberofcolor("gray")]; /*gray0*/
  selectcolor = incolor[numberofcolor("green")]; /*green0*/
  troughcolor = incolor[numberofcolor("gray")+50*2];/*gray2*/
  armedfillcolor = incolor[numberofcolor("green")+50*1]; /*green1*/

#endif /*ordinary X11, not PEX*/

  /* create Application Shell for mage named topLevel */
  /* set in commandline parameters */
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

  if (menufont){ XtSetArg(args[n], XmNfontList, menufontList); n++;}

  XtSetArg(args[n], XmNargc, copy_of_argc); n++;
  XtSetArg(args[n], XmNargv, copy_of_argv); n++;
  XtSetArg(args[n], XmNallowResize, True); n++;
  XtSetArg(args[n], XmNmappedWhenManaged, False); n++;
  XtSetArg(args[n], XmNtitle, "MAGE"); n++;
  XtSetArg(args[n], XmNx, 250); n++;
  XtSetArg(args[n], XmNy, 0); n++;
#ifdef SIZEOFWINDOW
  XtSetArg(args[n], XmNwidth, GWIDTH+BWIDTH+ZWIDTH); n++; 
                         /*orig==800, 100,   30,  UNIX not use RWIDTH here */
  XtSetArg(args[n], XmNheight,GHEIGHT+MESSHEIGHT+MENUHEIGHT); n++; 
                                     /*orig==860,GHEIGHT==800*/
#endif
  topLevel = XtAppCreateShell(NULL,
                             "MAGE",
                             applicationShellWidgetClass,
                             dpy,
                             args,n);

/*printf("topLevel created\n");*/

  /*create topgraf as a popup shell under topLevel*/
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

  if (menufont){ XtSetArg(args[n], XmNfontList, menufontList); n++;}
  ncommon = n;
  XtSetArg(args[n], XmNtitle, "GRAPHICS: MAGE"); n++;
  XtSetArg(args[n], XmNx, 250); n++;
  XtSetArg(args[n], XmNy, 0); n++;
#ifdef SIZEOFWINDOW
  XtSetArg(args[n], XmNwidth, GWIDTH+BWIDTH+ZWIDTH); n++; 
  XtSetArg(args[n], XmNheight,GHEIGHT+MESSHEIGHT+MENUHEIGHT); n++;
                               /*orig==860,GHEIGHT==800*/
#endif
  XtSetArg(args[n], XmNallowResize, True); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; /*use quit_CB*/
  /*XtSetArg(args[n], XmNmappedWhenManaged, False); n++;*/
  topgraf = XtCreatePopupShell("graf",
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);
  /*setup a callback so choosing close, or exit from the window bar*/
  /*actually quits the whole MAGE program, not just closes that window*/
  /*Though it is supposed to be bad programing practice to just disable*/
  /*the close and exit functions, I can't make the suggested code work to */
  /*call my own quit_CB.  Disable is worse than disappear and/or crash*/
/* WM_DELETE_WINDOW = XmInternAtom(dpy,"WM_DELETE_WINDOW",False);*/
/* XmAddWMProtocolCallback(topLevel,WM_DELETE_WINDOW,(XtCallbackProc)quit_CB,NULL);*/

  /*create grafwindow as a form widget to hold drawArea, menus, buttons, etc.*/
  n = ncommon;  /* window title is a regular string */
  XtSetArg(args[n], XmNtitle, "GRAPHICS form"); n++;
  XtSetArg(args[n], XmNx, 250); n++;
  XtSetArg(args[n], XmNy, 0); n++;
  XtSetArg(args[n], XmNwidth, GWIDTH+BWIDTH+ZWIDTH); n++;
  XtSetArg(args[n], XmNheight,GHEIGHT+MESSHEIGHT+MENUHEIGHT); n++; 
                                  /*orig==860,GHEIGHT==800*/

  grafwindow =				/* create the grafwindow for layout */
    XtCreateManagedWidget("graff",xmFormWidgetClass,topgraf, args, n);
    /*XmCreateForm(topgraf,"grafwindow", args, n);*/
    /*XtCreateWidget("grafwindow", xmFormWidgetClass, topgraf, NULL, 0);*/
  /*XtManageChild(grafwindow);*/
#ifdef UNIX_PEX
  XtAddEventHandler(grafwindow, Button1MotionMask, False, motion_handler, NULL);
  /*motion_handler and pick_CB in MUX_MAIN.c for consistency of ___MAIN stuff*/
  /*motion by dials even when move cursor out of drawArea to hit buttons*/
  /*these event handlers take care of blanking and restoring the dial labels */
  XtAddEventHandler(grafwindow, EnterWindowMask, False, enter_handler, NULL);
  XtAddEventHandler(grafwindow, LeaveWindowMask, False, leave_handler, NULL);
#endif
/***********************MENU************************************************/
/*create menuBar*/
  n = ncommon;
  XtSetArg(args[n], XmNrowColumnType, XmMENU_BAR); n++;
  XtSetArg(args[n], XmNheight, MENUHEIGHT); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  /*XtSetArg(args[n], XmNrightOffset, BWIDTH+ZWIDTH); n++;*/
  XtSetArg(args[n], XmNrightOffset, ZWIDTH); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  menuBar = XmCreateMenuBar(grafwindow, "menuBar", args, n);
  XtManageChild(menuBar);

/*FILEMENU**********************************************************/
/*create a Pulldown Menu Pane */
  n = ncommon;
  fileMenu = XmCreatePulldownMenu(menuBar, "File", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, fileMenu); n++;
  fileButton = XmCreateCascadeButton(menuBar, "File", args, n);
  XtManageChild(fileButton);

/*create buttons on this Pulldown Menu Pane*/
/*this works for created colormap, fails for default colormap*/
  n = ncommon;
  openItem = XmCreatePushButton(fileMenu,"Open New Kin File...",args,n);
  XtAddCallback(openItem, XmNactivateCallback,(XtCallbackProc)open_CB, NULL); 
  XtManageChild(openItem);

  n = ncommon;
  appendItem = XmCreatePushButton(fileMenu,"Append Kin File...",args,n);
  XtAddCallback(appendItem, XmNactivateCallback,(XtCallbackProc)append_CB,NULL);
  XtManageChild(appendItem);

  n = ncommon;
  importItem = XmCreatePushButton(fileMenu,"Import Data...",args,n);
  XtAddCallback(importItem, XmNactivateCallback,(XtCallbackProc)import_CB,NULL);
  XtManageChild(importItem);

  n = ncommon;
  writekinItem = 
             XmCreatePushButtonGadget(fileMenu,"Save As...",args,n);
  XtAddCallback(writekinItem,XmNactivateCallback
                                ,(XtCallbackProc)writekin_CB, NULL);
  XtManageChild(writekinItem);

/*NOTE: USING DISTINCTION BETWEEN PushButton and ToggleButton !!!! */
 
/*    FileMenu sub: WriteMenu  **********/

/*create Pulldown MenuPane for Write Menu under File Menu*/
  n = ncommon;
  writeMenu = XmCreatePulldownMenu(fileMenu, "Write Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/

/*create menus CascadeButton for each MenuPane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, writeMenu); n++;
  writeButton = XmCreateCascadeButton(fileMenu, "Save Special", args, n);
  XtManageChild(writeButton);

  n = ncommon;
  writedrawnewItem =XmCreatePushButtonGadget(writeMenu,"DrawNew Parts",args,n);
  XtAddCallback(writedrawnewItem,XmNactivateCallback
                ,(XtCallbackProc)writedrawnew_CB, NULL);
  XtManageChild(writedrawnewItem);

  n = ncommon;
  writerottedItem = XmCreatePushButtonGadget(writeMenu,"Rotated Parts",args,n);
  XtAddCallback(writerottedItem,XmNactivateCallback
                 ,(XtCallbackProc)writerotted_CB, NULL);
  XtManageChild(writerottedItem);
 
  n = ncommon;
  writeviewItem = XmCreatePushButtonGadget(writeMenu,"Current View",args,n);
  XtAddCallback(writeviewItem,XmNactivateCallback
                                 ,(XtCallbackProc)writeview_CB, NULL);
  XtManageChild(writeviewItem);
  n = ncommon;
  writefocusItem = XmCreatePushButtonGadget(writeMenu,"Picked Point to file",args,n);
  XtAddCallback(writefocusItem,XmNactivateCallback
                                  ,(XtCallbackProc)writefocus_CB,NULL);
  XtManageChild(writefocusItem);
  n = ncommon;
  stderrfocusItem = XmCreatePushButtonGadget(writeMenu,"Picked Point to stderr",args,n);
  XtAddCallback(stderrfocusItem,XmNactivateCallback
                                  ,(XtCallbackProc)stderrfocus_CB,NULL);
  XtManageChild(stderrfocusItem);


  n = ncommon;
  writeQAItem = XmCreatePushButtonGadget(writeMenu,"Questions & Ans",args,n);
  XtAddCallback(writeQAItem,XmNactivateCallback
                               ,(XtCallbackProc)writeQA_CB,NULL);
  XtManageChild(writeQAItem);

  n = ncommon;
  writeR3DItem = XmCreatePushButtonGadget(writeMenu,"2D rendering file",args,n);
  XtAddCallback(writeR3DItem,XmNactivateCallback
                               ,(XtCallbackProc)writeR3D_CB,NULL);
  XtManageChild(writeR3DItem);

  n = ncommon;
  writeptIDItem = XmCreatePushButtonGadget(writeMenu,"pointID to file",args,n);
  XtAddCallback(writeptIDItem,XmNactivateCallback
                               ,(XtCallbackProc)writeptID_CB,NULL);
  XtManageChild(writeptIDItem);

  n = ncommon;
  writepaletteItem = XmCreatePushButtonGadget(writeMenu,"palette to file",args,n);
  XtAddCallback(writepaletteItem,XmNactivateCallback
                               ,(XtCallbackProc)writepalette_CB,NULL);
  XtManageChild(writepaletteItem);

  n = ncommon;
  writemeansigmaItem = XmCreatePushButtonGadget(writeMenu,"coord averages to file",args,n); /*060604 Laverageout*/
  XtAddCallback(writemeansigmaItem,XmNactivateCallback
                               ,(XtCallbackProc)writemeansigma_CB,NULL);
  XtManageChild(writemeansigmaItem);

/*    FileMenu sub: WriteMenu  _______________*/

  n = ncommon;
  writePostScriptItem =
     XmCreatePushButtonGadget(fileMenu,"Write PostScript File...",args,n);
  XtAddCallback(writePostScriptItem,XmNactivateCallback
                ,(XtCallbackProc)writePostScript_CB,NULL);
  XtManageChild(writePostScriptItem);

  n = ncommon;
  quitItem = XmCreatePushButtonGadget(fileMenu,"Quit",args,n);
  XtAddCallback(quitItem,XmNactivateCallback,(XtCallbackProc)quit_CB, NULL);
  XtManageChild(quitItem);
/*FILEMENU______________________________________________________*/

/*EDITMENU*******************************************************/
/*create a Pulldown Menu Pane  */
  n = ncommon;
  editMenu = XmCreatePulldownMenu(menuBar, "Edit", args, n);
/*create CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, editMenu); n++;
  editButton = XmCreateCascadeButton(menuBar, "Edit", args, n);
  XtManageChild(editButton);
  
  n = ncommon;  /*Toggle buttons all have same args */
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  drawlineItem = XmCreateToggleButtonGadget(editMenu,"Draw New",args,n);
  XtAddCallback(drawlineItem,XmNvalueChangedCallback
                                     ,(XtCallbackProc)drawline_CB,NULL);
  XtManageChild(drawlineItem);

  n = ncommon;  /*971129*/
  drawsetupItem = XmCreatePushButtonGadget(editMenu,"Draw New Setup...",args,n);
  XtAddCallback(drawsetupItem,XmNactivateCallback
                               ,(XtCallbackProc)drawsetup_CB, NULL);
  XtManageChild(drawsetupItem);

  n = ncommon;
  editviewItem=XmCreatePushButtonGadget(editMenu,"Keep Current View...",args,n);
  XtAddCallback(editviewItem,XmNactivateCallback
                               ,(XtCallbackProc)editview_CB, NULL);
  XtManageChild(editviewItem);

  colorshowItem = XmCreateToggleButtonGadget(editMenu,"Change Color",args,n);
  XtAddCallback(colorshowItem,XmNvalueChangedCallback
                             ,(XtCallbackProc)pickcolor_CB,NULL);
  XtManageChild(colorshowItem);

  emphasisItem = XmCreateToggleButtonGadget(editMenu,"Emphasis",args,n);
  XtAddCallback(emphasisItem,XmNvalueChangedCallback
                             ,(XtCallbackProc)emphasis_CB,NULL);
  XtManageChild(emphasisItem);

  pruneItem = XmCreateToggleButtonGadget(editMenu,"Prune",args,n);
  XtAddCallback(pruneItem,XmNvalueChangedCallback 
                                          ,(XtCallbackProc)prune_CB, NULL);
  XtManageChild(pruneItem);

  superpickItem = XmCreateToggleButtonGadget(editMenu,"Superpick",args,n);
  XtAddCallback(superpickItem,XmNvalueChangedCallback
                             ,(XtCallbackProc)superpick_CB, NULL);
  XtManageChild(superpickItem);
  
  pickshowItem = XmCreateToggleButtonGadget(editMenu,"Edit Object Properties",args,n);
  XtAddCallback(pickshowItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)pickshow_CB, NULL);
  XtManageChild(pickshowItem);

  movepointsItem = XmCreateToggleButtonGadget(editMenu,"Redefine Point Groupings",args,n);
  XtAddCallback(movepointsItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)movepoints_CB, NULL);
  XtManageChild(movepointsItem); /*061126*/

  meansigmasItem = XmCreateToggleButtonGadget(editMenu,"Calc Mean & Sigmas",args,n);
  XtAddCallback(meansigmasItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)meansigmas_CB, NULL);
  XtManageChild(meansigmasItem); /*061126*/

  enableTEItem = XmCreateToggleButtonGadget(editMenu,"Text Editable",args,n);
  XtAddCallback(enableTEItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)enableTE_CB, NULL);
  XtManageChild(enableTEItem);

  tablecutItem=XmCreateToggleButtonGadget(editMenu,"Table Cell: Delete",args,n);
  XtAddCallback(tablecutItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)tablecut_CB, NULL);
  XtManageChild(tablecutItem);

  tablerestoreItem = XmCreateToggleButtonGadget(editMenu,"   Restore",args,n);
  XtAddCallback(tablerestoreItem,XmNvalueChangedCallback
                                   ,(XtCallbackProc)tablerestore_CB, NULL);
  XtManageChild(tablerestoreItem);

  tablepreinsertItem=XmCreateToggleButtonGadget(editMenu," Insert before",args,n);
  XtAddCallback(tablepreinsertItem,XmNvalueChangedCallback
                                 ,(XtCallbackProc)tablepreinsert_CB, NULL);
  XtManageChild(tablepreinsertItem);

  tablepostinsertItem=XmCreateToggleButtonGadget(editMenu," Insert after",args,n);
  XtAddCallback(tablepostinsertItem,XmNvalueChangedCallback
                                ,(XtCallbackProc)tablepostinsert_CB, NULL);
  XtManageChild(tablepostinsertItem);

  tablerebuildItem = XmCreateToggleButtonGadget(editMenu,"Table rebuild",args,n);
  XtAddCallback(tablerebuildItem,XmNvalueChangedCallback
                                   ,(XtCallbackProc)tablerebuild_CB, NULL);
  XtManageChild(tablerebuildItem);

  tablecolorItem=XmCreateToggleButtonGadget(editMenu,"Table change color",args,n);
  XtAddCallback(tablecolorItem,XmNvalueChangedCallback
                                    ,(XtCallbackProc)tablecolor_CB, NULL);
  XtManageChild(tablecolorItem);

/*EDITMENU_______________________________________________________*/

/*KINEMENU***********************************************************/
/*create a Pulldown Menu Pane  */
  n = ncommon;
  kineMenu = XmCreatePulldownMenu(menuBar, "KINEMAGE", args, n);
/*create CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, kineMenu); n++;
  kineButton = XmCreateCascadeButton(menuBar, "KINEMAGE", args, n);
  XtManageChild(kineButton);
  n = ncommon;
  nextItem = XmCreatePushButtonGadget(kineMenu,"Next",args,n);
  XtAddCallback(nextItem,XmNactivateCallback,(XtCallbackProc)next_CB, NULL); 
  XtManageChild(nextItem);
  n = ncommon;
  chooseItem = XmCreatePushButtonGadget(kineMenu,"Choose...",args,n);
  XtAddCallback(chooseItem,XmNactivateCallback,(XtCallbackProc)choose_CB, NULL);
  XtManageChild(chooseItem);
  n = ncommon;
  scoreNnextItem = XmCreatePushButtonGadget(kineMenu,"Score & Next",args,n);
  XtAddCallback(scoreNnextItem,XmNactivateCallback,(XtCallbackProc)scoreNnext_CB, NULL);
  XtManageChild(scoreNnextItem);
  n = ncommon;
  scoreNstayItem = XmCreatePushButtonGadget(kineMenu,"Score & Stay",args,n);
  XtAddCallback(scoreNstayItem,XmNactivateCallback,(XtCallbackProc)scoreNstay_CB, NULL);
  XtManageChild(scoreNstayItem);
/*KINEMENU__________________________________________________________*/

/*OPTIMENU******Display**********************************************/ 
/*create a Pulldown Menu Pane */
  n = ncommon;
  optiMenu = XmCreatePulldownMenu(menuBar, "Display", args, n);
/*create a CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, optiMenu); n++;
  optiButton = XmCreateCascadeButton(menuBar, "Display", args, n);
  XtManageChild(optiButton);


/*  ASPECTSMENU  *****************************************************/
/*create Pulldown Menu Pane for Aspects Menu under Opti Menu*/
  n = ncommon;
  aspectMenu = XmCreatePulldownMenu(optiMenu, "aspects Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, aspectMenu); n++;
  aspectButton = XmCreateCascadeButton(optiMenu,"Aspects",args,n);
  XtManageChild(aspectButton);

  

  n = ncommon; /*aspect Toggle buttons: all have same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  
  aspect0Item = XmCreateToggleButtonGadget(aspectMenu,"off",args,n);
     XtAddCallback(aspect0Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect0_CB, NULL); 
     XtManageChild(aspect0Item);
  aspect1Item = XmCreateToggleButtonGadget(aspectMenu,"1:",args,n);
     XtAddCallback(aspect1Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect1_CB, NULL); 
     XtManageChild(aspect1Item);
  
  aspect2Item = XmCreateToggleButtonGadget(aspectMenu,"2:",args,n);
     XtAddCallback(aspect2Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect2_CB, NULL); 
     XtManageChild(aspect2Item);
  aspect3Item = XmCreateToggleButtonGadget(aspectMenu,"3:",args,n);
     XtAddCallback(aspect3Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect3_CB, NULL); 
     XtManageChild(aspect3Item);
  aspect4Item = XmCreateToggleButtonGadget(aspectMenu,"4:",args,n);
     XtAddCallback(aspect4Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect4_CB, NULL); 
     XtManageChild(aspect4Item);
  aspect5Item = XmCreateToggleButtonGadget(aspectMenu,"5:",args,n);
     XtAddCallback(aspect5Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect5_CB, NULL); 
     XtManageChild(aspect5Item);
  aspect6Item = XmCreateToggleButtonGadget(aspectMenu,"6:",args,n);
     XtAddCallback(aspect6Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect6_CB, NULL); 
     XtManageChild(aspect6Item);
  aspect7Item = XmCreateToggleButtonGadget(aspectMenu,"7:",args,n);
     XtAddCallback(aspect7Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect7_CB, NULL); 
     XtManageChild(aspect7Item);
  aspect8Item = XmCreateToggleButtonGadget(aspectMenu,"8:",args,n);
     XtAddCallback(aspect8Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect8_CB, NULL); 
     XtManageChild(aspect8Item);
  aspect9Item = XmCreateToggleButtonGadget(aspectMenu,"9:",args,n);
     XtAddCallback(aspect9Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect9_CB, NULL); 
     XtManageChild(aspect9Item);
  aspect10Item = XmCreateToggleButtonGadget(aspectMenu,"10:",args,n);
     XtAddCallback(aspect10Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect10_CB, NULL); 
     XtManageChild(aspect10Item);
  aspect11Item = XmCreateToggleButtonGadget(aspectMenu,"11:",args,n);
     XtAddCallback(aspect11Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect11_CB, NULL); 
     XtManageChild(aspect11Item);
  aspect12Item = XmCreateToggleButtonGadget(aspectMenu,"12:",args,n);
     XtAddCallback(aspect12Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect12_CB, NULL); 
     XtManageChild(aspect12Item);
  aspect13Item = XmCreateToggleButtonGadget(aspectMenu,"13:",args,n);
     XtAddCallback(aspect13Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect13_CB, NULL); 
     XtManageChild(aspect13Item);
  aspect14Item = XmCreateToggleButtonGadget(aspectMenu,"14:",args,n);
     XtAddCallback(aspect14Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect14_CB, NULL); 
     XtManageChild(aspect14Item);
  aspect15Item = XmCreateToggleButtonGadget(aspectMenu,"15:",args,n);
     XtAddCallback(aspect15Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect15_CB, NULL); 
     XtManageChild(aspect15Item);
  aspect16Item = XmCreateToggleButtonGadget(aspectMenu,"16:",args,n);
     XtAddCallback(aspect16Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspect16_CB, NULL); 
     XtManageChild(aspect16Item);
  aspectscrnItem = XmCreateToggleButtonGadget(aspectMenu,"on screen",args,n);
     XtAddCallback(aspectscrnItem, XmNvalueChangedCallback
                                    ,(XtCallbackProc)aspectscrn_CB, NULL); 
     XtManageChild(aspectscrnItem);


  n = ncommon;  /*Toggle buttons all have same args */
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  
  multiwidthItem = XmCreateToggleButtonGadget(optiMenu,"Multi-Width",args,n);
  XtAddCallback(multiwidthItem,XmNvalueChangedCallback
                                  ,(XtCallbackProc)multiwidth_CB, NULL); 
  XtManageChild(multiwidthItem);

  widthItem = XmCreateToggleButtonGadget(optiMenu,"One Width",args,n);
  XtAddCallback(widthItem,XmNvalueChangedCallback
                                  ,(XtCallbackProc)width_CB, NULL); 
  XtManageChild(widthItem);

  thinItem = XmCreateToggleButtonGadget(optiMenu,"Thin Line 't'",args,n);
  XtAddCallback(thinItem,XmNvalueChangedCallback,(XtCallbackProc)thin_CB, NULL);
  XtManageChild(thinItem);

  depth5Item = XmCreateToggleButtonGadget(optiMenu,"5 depths 'y'",args,n);
  XtAddCallback(depth5Item,XmNvalueChangedCallback,(XtCallbackProc)depth5_CB, NULL);
  XtManageChild(depth5Item);

  depth3Item = XmCreateToggleButtonGadget(optiMenu,"3 depths 'y'",args,n);
  XtAddCallback(depth3Item,XmNvalueChangedCallback,(XtCallbackProc)depth3_CB, NULL);
  XtManageChild(depth3Item);
    
  perspecItem = XmCreateToggleButtonGadget(optiMenu,"Perspective 'p'",args,n);
  XtAddCallback(perspecItem,XmNvalueChangedCallback
                                    ,(XtCallbackProc)perspec_CB, NULL); 
  XtManageChild(perspecItem);

  stereoItem = XmCreateToggleButtonGadget(optiMenu,"Stereo On 's','c'",args,n);
  XtAddCallback(stereoItem, XmNvalueChangedCallback
                                    ,(XtCallbackProc)stereo_CB, NULL); 
  XtManageChild(stereoItem);
  compareItem = XmCreateToggleButtonGadget(optiMenu,"Compare On",args,n);
  XtAddCallback(compareItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)compare_CB, NULL); 
  XtManageChild(compareItem);

  n = ncommon;
  sterangItem = XmCreatePushButtonGadget(optiMenu,"Viewing Params...",args,n);
  XtAddCallback(sterangItem, XmNactivateCallback
                                    ,(XtCallbackProc)sterang_CB, NULL);
  XtManageChild(sterangItem);

  n = ncommon;  /*Toggle buttons all have same args */
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;

  listcolorItem = 
    XmCreateToggleButtonGadget(optiMenu,"List Color Dominant 'l'",args,n);
  XtAddCallback(listcolorItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)listcolor_CB, NULL); 
  XtManageChild(listcolorItem);
  
  lensItem = 
    XmCreateToggleButtonGadget(optiMenu,"Lens at Center 'e'",args,n);
  XtAddCallback(lensItem, XmNvalueChangedCallback
                                    ,(XtCallbackProc)lens_CB, NULL);
  XtManageChild(lensItem);

  n = ncommon;
  detailItem = XmCreatePushButtonGadget(optiMenu,"Detail Level...",args,n);
  XtAddCallback(detailItem, XmNactivateCallback
                                    ,(XtCallbackProc)detail_CB, NULL);
  XtManageChild(detailItem);
  
  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  whtbkgItem = XmCreateToggleButtonGadget(optiMenu,"White Background",args,n);
  XtAddCallback(whtbkgItem,XmNvalueChangedCallback
                                    ,(XtCallbackProc)whtbkg_CB, NULL); 
  XtManageChild(whtbkgItem);

  B_WItem = XmCreateToggleButtonGadget(optiMenu,"Black&White",args,n);
  XtAddCallback(B_WItem,XmNvalueChangedCallback,(XtCallbackProc)B_W_CB, NULL); 
  XtManageChild(B_WItem);

  grayItem = XmCreateToggleButtonGadget(optiMenu,"Gray Scale",args,n);
  XtAddCallback(grayItem,XmNvalueChangedCallback,(XtCallbackProc)gray_CB, NULL);
  XtManageChild(grayItem);

  n = ncommon;
  GFontItem = XmCreatePushButtonGadget(optiMenu,"Font Sizes...",args,n);
  XtAddCallback(GFontItem, XmNactivateCallback,(XtCallbackProc)GFont_CB, NULL);
  XtManageChild(GFontItem);
 
  n = ncommon;
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  strokedfontItem = XmCreateToggleButtonGadget(optiMenu,"stroked font",args,n);
  XtAddCallback(strokedfontItem, XmNvalueChangedCallback
                       ,(XtCallbackProc)strokedfont_CB, NULL);
  XtManageChild(strokedfontItem);

  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  flatItem=XmCreateToggleButtonGadget(optiMenu,"Flatland XY Scroll 'f'",args,n);
  XtAddCallback(flatItem,XmNvalueChangedCallback
                                   ,(XtCallbackProc)flat_CB,NULL);
  XtManageChild(flatItem);
 
  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  horizItem=XmCreateToggleButtonGadget(optiMenu,"horizontal around Y rotation 'h'",args,n);
  XtAddCallback(horizItem,XmNvalueChangedCallback
                                   ,(XtCallbackProc)horizon_CB,NULL);
  XtManageChild(horizItem);
  
  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  plotonlyItem = XmCreateToggleButtonGadget(optiMenu,"Kaleidoscope 'k'",args,n);
  XtAddCallback(plotonlyItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)plotonly_CB,NULL);
  XtManageChild(plotonlyItem);

  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  fontcursorItem = XmCreateToggleButtonGadget(optiMenu,"Font Cursor",args,n);
  XtAddCallback(fontcursorItem,XmNvalueChangedCallback
                                       ,(XtCallbackProc)fontcursor_CB,NULL);
  XtManageChild(fontcursorItem);
/*OPTIMENU_____Display_______________________________________________*/

/*VIEWMENU************************************************************/

/*create a Pulldown Menu Pane */
  n = ncommon;
  viewMenu = XmCreatePulldownMenu(menuBar, "VIEWS", args, n);
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, viewMenu); n++;
  viewButton = XmCreateCascadeButton(menuBar, "VIEWS", args, n);
  XtManageChild(viewButton);
  
  undopickctrItem = XmCreateToggleButtonGadget(viewMenu,"Undo Pickcenter"
                                               ,args,n);
  XtAddCallback(undopickctrItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)undopickctr_CB, NULL); 
  XtManageChild(undopickctrItem);


  n = ncommon; /*many view Toggle buttons: all have same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  
  reset1Item = XmCreateToggleButtonGadget(viewMenu,"View1",args,n);
  XtAddCallback(reset1Item,XmNvalueChangedCallback
                                 ,(XtCallbackProc)view1_CB, NULL); 
  XtManageChild(reset1Item);
  
  reset2Item = XmCreateToggleButtonGadget(viewMenu,"View2",args,n);
  XtAddCallback(reset2Item, XmNvalueChangedCallback
                                  ,(XtCallbackProc)view2_CB, NULL); 
  XtManageChild(reset2Item);
  reset3Item = XmCreateToggleButtonGadget(viewMenu,"View3",args,n);
  XtAddCallback(reset3Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view3_CB, NULL); 
  XtManageChild(reset3Item);
  reset4Item = XmCreateToggleButtonGadget(viewMenu,"View4",args,n);
  XtAddCallback(reset4Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view4_CB, NULL); 
  XtManageChild(reset4Item);
  reset5Item = XmCreateToggleButtonGadget(viewMenu,"View5",args,n);
  XtAddCallback(reset5Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view5_CB, NULL); 
  XtManageChild(reset5Item);
  reset6Item = XmCreateToggleButtonGadget(viewMenu,"View6",args,n);
  XtAddCallback(reset6Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view6_CB, NULL); 
  XtManageChild(reset6Item);
  reset7Item = XmCreateToggleButtonGadget(viewMenu,"View7",args,n);
  XtAddCallback(reset7Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view7_CB, NULL); 
  XtManageChild(reset7Item);
  reset8Item = XmCreateToggleButtonGadget(viewMenu,"View8",args,n);
  XtAddCallback(reset8Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view8_CB, NULL); 
  XtManageChild(reset8Item);
  reset9Item = XmCreateToggleButtonGadget(viewMenu,"View9",args,n);
  XtAddCallback(reset9Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view9_CB, NULL); 
  XtManageChild(reset9Item);
  reset10Item = XmCreateToggleButtonGadget(viewMenu,"View10",args,n);
  XtAddCallback(reset10Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view10_CB, NULL); 
  XtManageChild(reset10Item);
  reset11Item = XmCreateToggleButtonGadget(viewMenu,"View11",args,n);
  XtAddCallback(reset11Item,XmNvalueChangedCallback
                                 ,(XtCallbackProc)view11_CB, NULL); 
  XtManageChild(reset11Item);
  reset12Item = XmCreateToggleButtonGadget(viewMenu,"View12",args,n);
  XtAddCallback(reset12Item, XmNvalueChangedCallback
                                  ,(XtCallbackProc)view12_CB, NULL); 
  XtManageChild(reset12Item);
  reset13Item = XmCreateToggleButtonGadget(viewMenu,"View13",args,n);
  XtAddCallback(reset13Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view13_CB, NULL); 
  XtManageChild(reset13Item);
  reset14Item = XmCreateToggleButtonGadget(viewMenu,"View14",args,n);
  XtAddCallback(reset14Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view14_CB, NULL); 
  XtManageChild(reset14Item);
  reset15Item = XmCreateToggleButtonGadget(viewMenu,"View15",args,n);
  XtAddCallback(reset15Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view15_CB, NULL); 
  XtManageChild(reset15Item);

  /*    VIEWSMENU: sub  EXTRAMENU***extra views*****/
/*create Pulldown Menu Pane for ExtraViews Menu under Views Menu*/
  n = ncommon;
  extraviewMenu = XmCreatePulldownMenu(viewMenu, "extra Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, extraviewMenu); n++;
  extraviewsButton = XmCreateCascadeButton(viewMenu,"more views",args,n);
  XtManageChild(extraviewsButton);
  n = ncommon; /*many view Toggle buttons: all have same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;

  reset16Item = XmCreateToggleButtonGadget(extraviewMenu,"View16",args,n);
  XtAddCallback(reset16Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view16_CB, NULL); 
  XtManageChild(reset16Item);
  reset17Item = XmCreateToggleButtonGadget(extraviewMenu,"View17",args,n);
  XtAddCallback(reset17Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view17_CB, NULL); 
  XtManageChild(reset17Item);
  reset18Item = XmCreateToggleButtonGadget(extraviewMenu,"View18",args,n);
  XtAddCallback(reset18Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view18_CB, NULL); 
  XtManageChild(reset18Item);
  reset19Item = XmCreateToggleButtonGadget(extraviewMenu,"View19",args,n);
  XtAddCallback(reset19Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view19_CB, NULL); 
  XtManageChild(reset19Item);
  reset20Item = XmCreateToggleButtonGadget(extraviewMenu,"View20",args,n);
  XtAddCallback(reset20Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view20_CB, NULL); 
  XtManageChild(reset20Item);
  reset21Item = XmCreateToggleButtonGadget(extraviewMenu,"View21",args,n);
  XtAddCallback(reset21Item,XmNvalueChangedCallback
                                 ,(XtCallbackProc)view21_CB, NULL); 
  XtManageChild(reset21Item);
  reset22Item = XmCreateToggleButtonGadget(extraviewMenu,"View22",args,n);
  XtAddCallback(reset22Item, XmNvalueChangedCallback
                                  ,(XtCallbackProc)view22_CB, NULL); 
  XtManageChild(reset22Item);
  reset23Item = XmCreateToggleButtonGadget(extraviewMenu,"View23",args,n);
  XtAddCallback(reset23Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view23_CB, NULL); 
  XtManageChild(reset23Item);
  reset24Item = XmCreateToggleButtonGadget(extraviewMenu,"View24",args,n);
  XtAddCallback(reset24Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view24_CB, NULL); 
  XtManageChild(reset24Item);
  reset25Item = XmCreateToggleButtonGadget(extraviewMenu,"View25",args,n);
  XtAddCallback(reset25Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view25_CB, NULL); 
  XtManageChild(reset25Item);
  reset26Item = XmCreateToggleButtonGadget(extraviewMenu,"View26",args,n);
  XtAddCallback(reset26Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view26_CB, NULL); 
  XtManageChild(reset26Item);
  reset27Item = XmCreateToggleButtonGadget(extraviewMenu,"View27",args,n);
  XtAddCallback(reset27Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view27_CB, NULL); 
  XtManageChild(reset27Item);
  reset28Item = XmCreateToggleButtonGadget(extraviewMenu,"View28",args,n);
  XtAddCallback(reset28Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view28_CB, NULL); 
  XtManageChild(reset28Item);
  reset29Item = XmCreateToggleButtonGadget(extraviewMenu,"View29",args,n);
  XtAddCallback(reset29Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)view29_CB, NULL); 
  XtManageChild(reset29Item);
  reset30Item = XmCreateToggleButtonGadget(extraviewMenu,"View30",args,n);
  XtAddCallback(reset30Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view30_CB, NULL); 
  XtManageChild(reset30Item);
  reset31Item = XmCreateToggleButtonGadget(extraviewMenu,"View31",args,n);
  XtAddCallback(reset31Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)view31_CB, NULL); 
  XtManageChild(reset31Item);
  
  reset32Item = XmCreateToggleButtonGadget(extraviewMenu,"View32",args,n);
  XtAddCallback(reset32Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view32_CB, NULL); 
  XtManageChild(reset32Item);
  reset33Item = XmCreateToggleButtonGadget(extraviewMenu,"View33",args,n);
  XtAddCallback(reset33Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view33_CB, NULL); 
  XtManageChild(reset33Item);
  reset34Item = XmCreateToggleButtonGadget(extraviewMenu,"View34",args,n);
  XtAddCallback(reset34Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view34_CB, NULL); 
  XtManageChild(reset34Item);
  reset35Item = XmCreateToggleButtonGadget(extraviewMenu,"View35",args,n);
  XtAddCallback(reset35Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view35_CB, NULL); 
  XtManageChild(reset35Item);
  reset36Item = XmCreateToggleButtonGadget(extraviewMenu,"View36",args,n);
  XtAddCallback(reset36Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view36_CB, NULL); 
  XtManageChild(reset36Item);
  reset37Item = XmCreateToggleButtonGadget(extraviewMenu,"View37",args,n);
  XtAddCallback(reset37Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view37_CB, NULL); 
  XtManageChild(reset37Item);


  /*    VIEWSMENU: sub  EXTRAMENU***more views*****/
/*create Pulldown Menu Pane for ExtraViews Menu under Views Menu*/
  n = ncommon;
  moreviewMenu = XmCreatePulldownMenu(extraviewMenu, "extra Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, moreviewMenu); n++;
  moreviewsButton = XmCreateCascadeButton(extraviewMenu,"more views",args,n);
  XtManageChild(moreviewsButton);
  n = ncommon; /*many view Toggle buttons: all have same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;


  reset38Item = XmCreateToggleButtonGadget(moreviewMenu,"View38",args,n);
  XtAddCallback(reset38Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view38_CB, NULL); 
  XtManageChild(reset38Item);
  reset39Item = XmCreateToggleButtonGadget(moreviewMenu,"View39",args,n);
  XtAddCallback(reset39Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view39_CB, NULL); 
  XtManageChild(reset39Item);
  reset40Item = XmCreateToggleButtonGadget(moreviewMenu,"View40",args,n);
  XtAddCallback(reset40Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view40_CB, NULL); 
  XtManageChild(reset40Item);
  reset41Item = XmCreateToggleButtonGadget(moreviewMenu,"View41",args,n);
  XtAddCallback(reset41Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)view41_CB, NULL); 
  XtManageChild(reset41Item);
  reset42Item = XmCreateToggleButtonGadget(moreviewMenu,"View42",args,n);
  XtAddCallback(reset42Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view42_CB, NULL); 
  XtManageChild(reset42Item);
  reset43Item = XmCreateToggleButtonGadget(moreviewMenu,"View43",args,n);
  XtAddCallback(reset43Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view43_CB, NULL); 
  XtManageChild(reset43Item);
  reset44Item = XmCreateToggleButtonGadget(moreviewMenu,"View44",args,n);
  XtAddCallback(reset44Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view44_CB, NULL); 
  XtManageChild(reset44Item);
  reset45Item = XmCreateToggleButtonGadget(moreviewMenu,"View45",args,n);
  XtAddCallback(reset45Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view45_CB, NULL); 
  XtManageChild(reset45Item);
  reset46Item = XmCreateToggleButtonGadget(moreviewMenu,"View46",args,n);
  XtAddCallback(reset46Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view46_CB, NULL); 
  XtManageChild(reset46Item);
  reset47Item = XmCreateToggleButtonGadget(moreviewMenu,"View47",args,n);
  XtAddCallback(reset47Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view47_CB, NULL); 
  XtManageChild(reset47Item);
  reset48Item = XmCreateToggleButtonGadget(moreviewMenu,"View48",args,n);
  XtAddCallback(reset48Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view48_CB, NULL); 
  XtManageChild(reset48Item);
  reset49Item = XmCreateToggleButtonGadget(moreviewMenu,"View49",args,n);
  XtAddCallback(reset49Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view49_CB, NULL); 
  XtManageChild(reset49Item);
  reset50Item = XmCreateToggleButtonGadget(moreviewMenu,"View50",args,n);
  XtAddCallback(reset50Item, XmNvalueChangedCallback
                                   ,(XtCallbackProc)view50_CB, NULL); 
  XtManageChild(reset50Item);
  reset51Item = XmCreateToggleButtonGadget(moreviewMenu,"View51",args,n);
  XtAddCallback(reset51Item,XmNvalueChangedCallback
                                    ,(XtCallbackProc)view51_CB, NULL); 
  XtManageChild(reset51Item);
  reset52Item = XmCreateToggleButtonGadget(moreviewMenu,"View52",args,n);
  XtAddCallback(reset52Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view52_CB, NULL); 
  XtManageChild(reset52Item);
  reset53Item = XmCreateToggleButtonGadget(moreviewMenu,"View53",args,n);
  XtAddCallback(reset53Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view53_CB, NULL); 
  XtManageChild(reset53Item);
  reset54Item = XmCreateToggleButtonGadget(moreviewMenu,"View54",args,n);
  XtAddCallback(reset54Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view54_CB, NULL); 
  XtManageChild(reset54Item);
  reset55Item = XmCreateToggleButtonGadget(moreviewMenu,"View55",args,n);
  XtAddCallback(reset55Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view55_CB, NULL); 
  XtManageChild(reset55Item);
  reset56Item = XmCreateToggleButtonGadget(moreviewMenu,"View56",args,n);
  XtAddCallback(reset56Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view56_CB, NULL); 
  XtManageChild(reset56Item);
  reset57Item = XmCreateToggleButtonGadget(moreviewMenu,"View57",args,n);
  XtAddCallback(reset57Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view57_CB, NULL); 
  XtManageChild(reset57Item);
  reset58Item = XmCreateToggleButtonGadget(moreviewMenu,"View58",args,n);
  XtAddCallback(reset58Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view58_CB, NULL); 
  XtManageChild(reset58Item);
  reset59Item = XmCreateToggleButtonGadget(moreviewMenu,"View59",args,n);
  XtAddCallback(reset59Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view59_CB, NULL); 
  XtManageChild(reset59Item);
  reset60Item = XmCreateToggleButtonGadget(moreviewMenu,"View60",args,n);
  XtAddCallback(reset60Item, XmNvalueChangedCallback
                                    ,(XtCallbackProc)view60_CB, NULL); 
  XtManageChild(reset60Item);



  resetRItem = XmCreateToggleButtonGadget(viewMenu,"Reader's View",args,n);
  XtAddCallback(resetRItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)viewR_CB, NULL); 
  XtManageChild(resetRItem);

  n = ncommon;
  readersviewItem=
           XmCreatePushButtonGadget(viewMenu,"Set Reader's View",args,n);
  XtAddCallback(readersviewItem,XmNactivateCallback
                 ,(XtCallbackProc)readersview_CB,NULL); 
  XtManageChild(readersviewItem);

  n = ncommon;
  NDaxesPickedCtrItem=
           XmCreateToggleButtonGadget(viewMenu,"NDaxesPickedCtr",args,n);
  XtAddCallback(NDaxesPickedCtrItem,  XmNvalueChangedCallback
                 ,(XtCallbackProc)NDaxesPickedCtr_CB,NULL); 
  XtManageChild(NDaxesPickedCtrItem);  /*070315*/

/*VIEWMENU________________________________________________________*/


/*OTHERMENU*********Tools*******************************************/
/*create a Pulldown Menu Pane */
  n = ncommon;
  otherMenu = XmCreatePulldownMenu(menuBar, "Other", args, n);
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, otherMenu); n++;
  otherButton = XmCreateCascadeButton(menuBar, "Tools", args, n);
  XtManageChild(otherButton);

  n = ncommon;
  searchItem = XmCreatePushButtonGadget(otherMenu,"Find ...",args,n);
  XtAddCallback(searchItem, XmNactivateCallback,(XtCallbackProc)search_CB,NULL);
  XtManageChild(searchItem);

  n = ncommon;
  findagainItem = XmCreatePushButtonGadget(otherMenu,"Find Next",args,n);
  XtAddCallback(findagainItem, XmNactivateCallback,(XtCallbackProc)findagain_CB,NULL);
  XtManageChild(findagainItem);
  
  n = ncommon; /* set of toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  measureItem = XmCreateToggleButtonGadget(otherMenu,"measures",args,n);
  XtAddCallback(measureItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)measure_CB, NULL); 
  XtManageChild(measureItem);
  
  /*    OTHERMENU(Tools): sub  EMPOWERMENU***Empowerments*****/

/*create Pulldown Menu Pane for Empowerments Menu under Other Menu*/
  n = ncommon;
  empowerMenu = XmCreatePulldownMenu(otherMenu, "Empower Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, empowerMenu); n++;
  helpButton = XmCreateCascadeButton(otherMenu, "Empowerments", args, n);
  XtManageChild(helpButton);

  n = ncommon;
  hidemeasuresItem = XmCreatePushButtonGadget(empowerMenu,"Hide Measures  'm'",args,n);
  XtAddCallback(hidemeasuresItem, XmNactivateCallback,(XtCallbackProc)hidemeasures_CB,NULL);
  XtManageChild(hidemeasuresItem);

  n = ncommon;
  brotangleItem = XmCreatePushButtonGadget(empowerMenu,"Restore Rotations",args,n);
  XtAddCallback(brotangleItem, XmNactivateCallback,(XtCallbackProc)brotangle_CB,NULL);
  XtManageChild(brotangleItem);

  n = ncommon;
  rot90plusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90+  'n'",args,n);
  XtAddCallback(rot90plusItem, XmNactivateCallback,(XtCallbackProc)rot90plus_CB,NULL);
  XtManageChild(rot90plusItem);

  n = ncommon;
  rot90minusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90-  'N'",args,n);
  XtAddCallback(rot90minusItem, XmNactivateCallback,(XtCallbackProc)rot90minus_CB,NULL);
  XtManageChild(rot90minusItem);

  n = ncommon;
  rot90xplusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90+ horiz",args,n);
  XtAddCallback(rot90xplusItem, XmNactivateCallback,(XtCallbackProc)rot90xplus_CB,NULL);
  XtManageChild(rot90xplusItem);

  n = ncommon;
  rot90xminusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90- horiz",args,n);
  XtAddCallback(rot90xminusItem, XmNactivateCallback,(XtCallbackProc)rot90xminus_CB,NULL);
  XtManageChild(rot90xminusItem);

  n = ncommon;
  rot90zplusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90+ screen",args,n);
  XtAddCallback(rot90zplusItem, XmNactivateCallback,(XtCallbackProc)rot90zplus_CB,NULL);
  XtManageChild(rot90zplusItem);

  n = ncommon;
  rot90zminusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate 90- screen",args,n);
  XtAddCallback(rot90zminusItem, XmNactivateCallback,(XtCallbackProc)rot90zminus_CB,NULL);
  XtManageChild(rot90zminusItem);

  n = ncommon;
  stereoplusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate stereo +",args,n);
  XtAddCallback(stereoplusItem, XmNactivateCallback,(XtCallbackProc)stereoplus_CB,NULL);
  XtManageChild(stereoplusItem);

  n = ncommon;
  stereominusItem = XmCreatePushButtonGadget(empowerMenu,"Rotate stereo -",args,n);
  XtAddCallback(stereominusItem, XmNactivateCallback,(XtCallbackProc)stereominus_CB,NULL);
  XtManageChild(stereominusItem);

/*    OTHERMENU(Tools): sub  EMPOWERMENU___Empowerments_____*/

  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  remoteupdateItem = XmCreateToggleButtonGadget(otherMenu
                                                ,"Remote Update... 'u'",args,n);
  XtAddCallback(remoteupdateItem,XmNvalueChangedCallback
                                   ,(XtCallbackProc)remoteupdate_CB,NULL);
  XtManageChild(remoteupdateItem);

  n = ncommon; /* set of toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  xyzpointItem = XmCreateToggleButtonGadget(otherMenu,"XYZ point",args,n);
  XtAddCallback(xyzpointItem,XmNvalueChangedCallback
                                     ,(XtCallbackProc)xyzpoint_CB,NULL);
  
  XtManageChild(xyzpointItem);

  gnomonItem = XmCreateToggleButtonGadget(otherMenu,"Gnomon",args,n);
  XtAddCallback(gnomonItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)gnomon_CB, NULL);
  XtManageChild(gnomonItem);

  n = ncommon; /* set of toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  rateItem = XmCreateToggleButtonGadget(otherMenu,"Rate",args,n);
  XtAddCallback(rateItem, XmNvalueChangedCallback,(XtCallbackProc)rate_CB,NULL);
  XtManageChild(rateItem);
 
  n = ncommon;
  countItem = XmCreatePushButtonGadget(otherMenu,"Count",args,n);
  XtAddCallback(countItem, XmNactivateCallback,(XtCallbackProc)count_CB, NULL);
  XtManageChild(countItem);
  
  n = ncommon;
  dockingscopeItem=XmCreateToggleButtonGadget(otherMenu,"Docking Scope",args,n);
  XtAddCallback(dockingscopeItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)dockingscope_CB,NULL);
  XtManageChild(dockingscopeItem);
 
  n = ncommon;
  zlimitscopeItem=XmCreateToggleButtonGadget(otherMenu,"Z limit Scope",args,n);
  XtAddCallback(zlimitscopeItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)zlimitscope_CB,NULL);
  XtManageChild(zlimitscopeItem);
 
  n = ncommon;
  finestepItem = XmCreatePushButtonGadget(otherMenu,"fine step...",args,n);
  XtAddCallback(finestepItem, XmNactivateCallback
                                    ,(XtCallbackProc)finestep_CB, NULL);
  XtManageChild(finestepItem);
  
  n = ncommon; /* push buttons have fewer args */
  dimensionItem = XmCreatePushButtonGadget(otherMenu,"choose nD axes...",args,n);
  XtAddCallback(dimensionItem, XmNactivateCallback
                                   ,(XtCallbackProc)dimension_CB,NULL);
  XtManageChild(dimensionItem); /*L7dimension*/
  
  n = ncommon; /* push buttons have fewer args */
  SELECTINGlistItem = XmCreatePushButtonGadget(otherMenu,"nD point select...",args,n);
  XtAddCallback(SELECTINGlistItem, XmNactivateCallback
                                       ,(XtCallbackProc)SELECTINGlist_CB, NULL);
  XtManageChild(SELECTINGlistItem); /*LNdimension, was L7 060622*/
  SELECTINGcolorItem = XmCreatePushButtonGadget(otherMenu,"nD color select...",args,n);
  XtAddCallback(SELECTINGcolorItem, XmNactivateCallback
                                      ,(XtCallbackProc)SELECTINGcolor_CB, NULL);
  XtManageChild(SELECTINGcolorItem); /*color list box 061030 LNdimension, L7*/

/*    OTHERMENU(Tools): sub  KLUGESMENU**************/
/*create Pulldown Menu Pane for Kluges Menu under Other Menu*/
  n = ncommon;
  kludgeMenu = XmCreatePulldownMenu(otherMenu, "Kluges Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, kludgeMenu); n++;
  kludgeButton = XmCreateCascadeButton(otherMenu, "Kludges", args, n);
  XtManageChild(kludgeButton);

  n = ncommon; /* set of toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;

  clipmiddleItem = 
     XmCreateToggleButtonGadget(kludgeMenu,"clip middle",args,n);
  XtAddCallback(clipmiddleItem, XmNvalueChangedCallback
                              ,(XtCallbackProc)clipmiddle_CB, NULL);
  XtManageChild(clipmiddleItem);  /*051214*/

  measperpItem = 
     XmCreateToggleButtonGadget(kludgeMenu,"measures+perp d",args,n);
  XtAddCallback(measperpItem, XmNvalueChangedCallback
                              ,(XtCallbackProc)measperp_CB, NULL);
  XtManageChild(measperpItem);

  measuredsurveilItem =                              /*091212*/ 
     XmCreateToggleButtonGadget(kludgeMenu,"measured surveillance",args,n);
  XtAddCallback(measuredsurveilItem, XmNvalueChangedCallback
                              ,(XtCallbackProc)measuredsurveil_CB, NULL);
  XtManageChild(measuredsurveilItem);

  formattestItem = 
     XmCreateToggleButtonGadget(kludgeMenu,"Test Kin Format",args,n);
  XtAddCallback(formattestItem, XmNvalueChangedCallback
                              ,(XtCallbackProc)formattest_CB, NULL);
  XtManageChild(formattestItem);

  testItem = XmCreateToggleButtonGadget(kludgeMenu,"Debug test",args,n);
  XtAddCallback(testItem, XmNvalueChangedCallback,(XtCallbackProc)test_CB,NULL);
  XtManageChild(testItem);

  pickmarkerItem = XmCreateToggleButtonGadget(kludgeMenu,"Pick Marker",args,n);
  XtAddCallback(pickmarkerItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)pickmarker_CB,NULL);
  XtManageChild(pickmarkerItem);

  singlemarkerItem = XmCreateToggleButtonGadget(kludgeMenu,"Single_Marker",args,n);
  XtAddCallback(singlemarkerItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)singlemarker_CB, NULL);
  XtManageChild(singlemarkerItem);

  visibleoutItem = XmCreateToggleButtonGadget(kludgeMenu,"output only visible",args,n);
  XtAddCallback(visibleoutItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)visibleout_CB,NULL);
  XtManageChild(visibleoutItem);

  rescoringItem = XmCreateToggleButtonGadget(kludgeMenu,"score 3D test",args,n);
  XtAddCallback(rescoringItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)rescoring_CB,NULL);
  XtManageChild(rescoringItem);

  n = ncommon;
  backgroundItem = XmCreatePushButtonGadget(kludgeMenu,"BackGround...",args,n);
  XtAddCallback(backgroundItem, XmNactivateCallback,(XtCallbackProc)background_CB, NULL);
  XtManageChild(backgroundItem);

  n = ncommon;
  buttonwindowItem =XmCreateToggleButtonGadget(kludgeMenu,"Button Window",args,n);
  XtAddCallback(buttonwindowItem, XmNvalueChangedCallback
                                     ,(XtCallbackProc)buttonwindow_CB, NULL);
  XtManageChild(buttonwindowItem);

  n = ncommon;
  allbrsliderItem =XmCreateToggleButtonGadget(kludgeMenu,"show all sliders",args,n);
  XtAddCallback(allbrsliderItem, XmNvalueChangedCallback,(XtCallbackProc)allbrslider_CB, NULL);
  XtManageChild(allbrsliderItem);

  n = ncommon;
  foocavityItem =XmCreatePushButtonGadget(kludgeMenu,"foo in cavity...",args,n);
  XtAddCallback(foocavityItem, XmNactivateCallback,(XtCallbackProc)foocavity_CB, NULL);
  XtManageChild(foocavityItem);

  n = ncommon;
  mickeymouseItem = XmCreateToggleButtonGadget(kludgeMenu
                                 ,"Mickey Mouse...",args,n);
  XtAddCallback(mickeymouseItem, XmNvalueChangedCallback
                                 ,(XtCallbackProc)mickeymouse_CB, NULL);
  XtManageChild(mickeymouseItem);
  pdbformatoutItem = XmCreateToggleButtonGadget(kludgeMenu,"output pdb format",args,n);
  XtAddCallback(pdbformatoutItem, XmNvalueChangedCallback
                                   ,(XtCallbackProc)pdbformatout_CB,NULL);
  XtManageChild(pdbformatoutItem);

/*    OTHERMENU: sub  KLUGESMENU_____________*/
/*OTHERMENU____________________________________________________________*/

/*WINDMENU******************Help*********************************/
/*create a Pulldown Menu Pane  */
  n = ncommon;
  windMenu = XmCreatePulldownMenu(menuBar, "Windows", args, n);
/*create CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, windMenu); n++;
  windButton = XmCreateCascadeButton(menuBar, "MAGE-HELP", args, n);
  XtManageChild(windButton);
  
  n = ncommon;
  secretItem = XmCreatePushButtonGadget(windMenu,"Keyboard Shortcuts",args,n);
  XtAddCallback(secretItem, XmNactivateCallback,(XtCallbackProc)secret_CB,NULL);
  XtManageChild(secretItem);
  
  
  n = ncommon;
  showtextItem = XmCreatePushButtonGadget(windMenu,"Show text",args,n);
  XtAddCallback(showtextItem,XmNactivateCallback
                                   ,(XtCallbackProc)showtext_CB, NULL);
  XtManageChild(showtextItem);
  n = ncommon;
  showcaptionItem = XmCreatePushButtonGadget(windMenu,"Show caption",args,n);
  XtAddCallback(showcaptionItem,XmNactivateCallback
                               ,(XtCallbackProc)showcaption_CB, NULL);
  XtManageChild(showcaptionItem);
  n = ncommon;
  showgraphicsItem = XmCreatePushButtonGadget(windMenu,"Show graphics",args,n);
  XtAddCallback(showgraphicsItem,XmNactivateCallback
                      ,(XtCallbackProc)showgraphics_CB, NULL);
  XtManageChild(showgraphicsItem);
  n = ncommon;
  showbuttwinItem = XmCreatePushButtonGadget(windMenu,"Show buttons",args,n);
  XtAddCallback(showbuttwinItem,XmNactivateCallback
                      ,(XtCallbackProc)showbuttonwindow_CB, NULL);
  XtManageChild(showbuttwinItem); /*060613*/
   n = ncommon;
  showtableItem = XmCreatePushButtonGadget(windMenu,"Show table",args,n);
  XtAddCallback(showtableItem,XmNactivateCallback
                      ,(XtCallbackProc)showtable_CB, NULL);
  XtManageChild(showtableItem);
   n = ncommon;
  showbondrotsItem = XmCreatePushButtonGadget(windMenu,"Show rotations",args,n);
  XtAddCallback(showbondrotsItem,XmNactivateCallback
                      ,(XtCallbackProc)showbondrots_CB, NULL);
  XtManageChild(showbondrotsItem);
  n = ncommon;
  aboutItem = XmCreatePushButtonGadget(windMenu,"About MAGE",args,n);
  XtAddCallback(aboutItem, XmNactivateCallback,(XtCallbackProc)alert_CB, NULL); 
  XtManageChild(aboutItem);
 
/*    WINDMENU(HELP): sub  IOHELPMENU**************/
/*create Pulldown Menu Pane for iohelp Menu under Wind Menu*/
  n = ncommon;
  iohelpMenu = XmCreatePulldownMenu(windMenu, "IOhelp Menu dummy", args, n);
  /*do not managechild here, popup level is not complete*/
/*create menu CascadeButton for this Menu Pane*/
  n = ncommon;
  XtSetArg(args[n], XmNsubMenuId, iohelpMenu); n++;
  iohelpButton = XmCreateCascadeButton(windMenu, "IO-Help", args, n);
  XtManageChild(iohelpButton);

  n = ncommon;
  writeaboutItem =XmCreatePushButtonGadget(iohelpMenu,"Input/Output Help",args,n);
  XtAddCallback(writeaboutItem,XmNactivateCallback
                                   ,(XtCallbackProc)writeabout_CB,NULL);
  XtManageChild(writeaboutItem);
  n = ncommon;
  screenoutItem =XmCreatePushButtonGadget(iohelpMenu,"Screen Capture",args,n);
  XtAddCallback(screenoutItem,XmNactivateCallback
                                   ,(XtCallbackProc)screenout_CB,NULL);
  XtManageChild(screenoutItem);

/*_____WINDMENU(HELP): sub  IOHELPMENU____*/

  n = ncommon; /* toggle buttons all have the same args*/
  XtSetArg(args[n], XmNindicatorOn, True); n++;
  XtSetArg(args[n], XmNfillOnSelect, True); n++;
  XtSetArg(args[n], XmNselectColor, (Pixel)1); n++;
  editaboutItem = XmCreatePushButtonGadget(windMenu,"Edit Help",args,n);
  XtAddCallback(editaboutItem,XmNactivateCallback
                                ,(XtCallbackProc)editabout_CB, NULL);
  XtManageChild(editaboutItem);

  n = ncommon;
  optionsaboutItem = XmCreatePushButtonGadget(windMenu,"Display Help",args,n);
  XtAddCallback(optionsaboutItem,XmNactivateCallback
                ,(XtCallbackProc)optionsabout_CB,NULL); 
  XtManageChild(optionsaboutItem);

  n = ncommon;
  specialaboutItem = XmCreatePushButtonGadget(windMenu,"fonts,,,views Help",args,n);
  XtAddCallback(specialaboutItem,XmNactivateCallback
                                    ,(XtCallbackProc)specialabout_CB,NULL);
  XtManageChild(specialaboutItem);

  otheraboutItem = XmCreatePushButtonGadget(windMenu,"Tools Help",args,n);
  XtAddCallback(otheraboutItem,XmNactivateCallback
                                      ,(XtCallbackProc)otherabout_CB,NULL); 
  XtManageChild(otheraboutItem);
 
  n = ncommon;
  kludgesaboutItem = XmCreatePushButtonGadget(windMenu,"Kludges Help",args,n);
  XtAddCallback(kludgesaboutItem,XmNactivateCallback
                 ,(XtCallbackProc)kludgesabout_CB,NULL);
  XtManageChild(kludgesaboutItem);

  mousehelpItem = XmCreatePushButtonGadget(windMenu,"Mouse Help",args,n);
  XtAddCallback(mousehelpItem,XmNactivateCallback
                                    ,(XtCallbackProc)mousehelp_CB, NULL);
  XtManageChild(mousehelpItem);
  dockinghelpItem = XmCreatePushButtonGadget(windMenu,"Docking Help",args,n);
  XtAddCallback(dockinghelpItem,XmNactivateCallback
                                    ,(XtCallbackProc)dockinghelp_CB, NULL);
  XtManageChild(dockinghelpItem);

  resizehelpItem = XmCreatePushButtonGadget(windMenu,"Window sizing Help",args,n);
  XtAddCallback(resizehelpItem,XmNactivateCallback
                                     ,(XtCallbackProc)resizehelp_CB,NULL);
  XtManageChild(resizehelpItem);
  tablehelpItem = XmCreatePushButtonGadget(windMenu,"Table Help",args,n);
  XtAddCallback(tablehelpItem,XmNactivateCallback
                                     ,(XtCallbackProc)tablehelp_CB,NULL);
  XtManageChild(tablehelpItem);
  bondshelpItem = XmCreatePushButtonGadget(windMenu,"Bonds Help",args,n);
  XtAddCallback(bondshelpItem,XmNactivateCallback
                                     ,(XtCallbackProc)bondshelp_CB,NULL);
  XtManageChild(bondshelpItem);
  test3DhelpItem = XmCreatePushButtonGadget(windMenu,"3D test Help",args,n);
  XtAddCallback(test3DhelpItem,XmNactivateCallback
                                     ,(XtCallbackProc)test3Dhelp_CB,NULL);
  XtManageChild(test3DhelpItem);
  NDIMhelpItem = XmCreatePushButtonGadget(windMenu,"N DIMensions Help",args,n);
  XtAddCallback(NDIMhelpItem,XmNactivateCallback
                                     ,(XtCallbackProc)NDIMhelp_CB,NULL);
  XtManageChild(NDIMhelpItem);

  n = ncommon;
  makehelptextItem = XmCreatePushButtonGadget(windMenu,"Help->text...",args,n);
  XtAddCallback(makehelptextItem,XmNactivateCallback
                                    ,(XtCallbackProc)makehelptext_CB,NULL);
  XtManageChild(makehelptextItem);

  n = ncommon;
  makekinemageItem = XmCreatePushButtonGadget(windMenu,"Make kinemage...",args,n);
  XtAddCallback(makekinemageItem,XmNactivateCallback
                                    ,(XtCallbackProc)makekinemage_CB,NULL);
  XtManageChild(makekinemageItem);

  n = ncommon;
  makehelphtmlItem = XmCreatePushButtonGadget(windMenu,"Help->HTML...",args,n);
  XtAddCallback(makehelphtmlItem,XmNactivateCallback
                                    ,(XtCallbackProc)makehelphtml_CB,NULL);
  XtManageChild(makehelphtmlItem);

/*WINDMENU__________________________________________________________*/



/*___________________________MENU__________________________________________*/

#ifdef UNIX_PEX
/*create pointID and distance message place, for PEX use only*/
  n = ncommon;/* create the message place for pointID, dist, etc. */
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNheight, MESSHEIGHT); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset, BWIDTH+ZWIDTH); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNmarginHeight, 0); n++;
  XtSetArg(args[n], XmNmarginWidth, 5); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  XtSetArg(args[n], XmNautoShowCursorPosition, False); n++;
  XtSetArg(args[n], XmNcursorPositionVisible, False); n++;
  if (textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  message = XtCreateWidget("message", xmTextWidgetClass, grafwindow, args, n);
  XtManageChild(message);
#endif
/*create drawArea directly in grafwindow as parent*/
  n = ncommon;/* create the drawing area for PEX or X11 */
  XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNtopOffset, MENUHEIGHT); n++;
  XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNrightOffset, BWIDTH+ZWIDTH); n++;
  XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
/*  XtSetArg(args[n], XmNbottomOffset, MESSHEIGHT); n++;*/
  drawArea = XtCreateWidget("drawArea", xmDrawingAreaWidgetClass,
				   grafwindow, args, n);
/* add exposure and input callbacks */
  XtAddCallback(drawArea,XmNinputCallback,(XtCallbackProc)pick_CB,NULL); 
                                                        /*MUX_MAIN.c*/
  XtAddCallback(drawArea,XmNexposeCallback,(XtCallbackProc)drawArea_CB,NULL);
  XtAddCallback(drawArea, XmNresizeCallback,(XtCallbackProc)resize_CB, NULL);
                                                            /*MUX_MENU.c*/
  XtAddEventHandler(drawArea, Button1MotionMask, False, motion_handler, NULL);
  /*motion_handler and pick_CB in MUX_MAIN.c for consistency of ___MAIN stuff*/
  XtAddEventHandler(drawArea, PointerMotionMask, False, pointer_handler, NULL);
  /*enter and leave not useful for finger: no update when mouse out of window*/
  XtAddEventHandler(drawArea, Button3MotionMask, False,Rmotion_handler, NULL);/*030405*/
#ifdef UNIX_PEX
/* these event handlers take care of blanking and restoring the dial labels */
  XtAddEventHandler(drawArea, EnterWindowMask, False, enter_handler, NULL);
  XtAddEventHandler(drawArea, LeaveWindowMask, False, leave_handler, NULL);
#endif /*UNIX_PEX*/

  XtManageChild(drawArea);

/*printf("drawArea created\n");*/

/*create captwindow*/
 /*create topcapt as a topLevel shell with mage topLevel as parent*/
 /*topcapt = XtCreateApplicationShell("CAPT",topLevelShellWidgetClass,NULL,0);*/
  n = ncommon;  /* window title is a regular string */
  XtSetArg(args[n], XmNtitle, "CAPTION: MAGE"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; /*use quit_CB*/
  topcapt = XtCreatePopupShell(NULL,
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);

  n = ncommon;  /* window title is a regular string */
  XtSetArg(args[n], XmNtitle, "CAPTION text"); n++;
  XtSetArg(args[n], XmNx, 0); n++;
  XtSetArg(args[n], XmNy, 600); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
/*  XtSetArg(args[n], XmNscrollHorizontal, True); n++; */ /*010906*/
/*  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++; */
XtSetArg(args[n], XmNscrollHorizontal, False); n++; /*020512*/
  XtSetArg(args[n], XmNeditable, True); n++; /*981005*/
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNautoShowCursorPosition, True); n++; /*981005*/
  XtSetArg(args[n], XmNcursorPositionVisible, True); n++; /*981005*/
  XtSetArg(args[n], XmNwordWrap, True); n++;
  if (textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  /* create the captwindow for MAGE CAPTION  */
  captwindow =
    XmCreateScrolledText(topcapt,"captwindow", args, n);
  XtAddCallback(captwindow,XmNmotionVerifyCallback,(XtCallbackProc)captcursor_CB,NULL); 
      /*for hypertext*/
  XtManageChild(captwindow);
  XtPopup(topcapt,XtGrabNone);
/*printf("captwindow created\n");*/

/*create textwindow*/
 /*create toptext as a topLevel shell with mage topLevel as parent*/
 /*toptext = XtCreateApplicationShell("TEXT",topLevelShellWidgetClass,NULL,0);*/
  n = ncommon;  /* window title is a regular string */
  XtSetArg(args[n], XmNtitle, "TEXT: MAGE"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; /*use quit_CB*/
  toptext = XtCreatePopupShell(NULL,
                               topLevelShellWidgetClass,
                               topLevel,
                               args, n);

  n = ncommon;  /* window title is a regular string */
  XtSetArg(args[n], XmNtitle, "TEXT text"); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 100); n++;
  XtSetArg(args[n], XmNwidth, 600); n++;
  XtSetArg(args[n], XmNheight, 400); n++;
/*  XtSetArg(args[n], XmNscrollHorizontal, True); n++; */ /*010906*/
/*  XtSetArg(args[n], XmNscrollingPolicy, XmAUTOMATIC); n++;*/
XtSetArg(args[n], XmNscrollHorizontal, False); n++;
  XtSetArg(args[n], XmNeditable, True); n++; /*981005*/
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNautoShowCursorPosition, True); n++; /*981005*/
  XtSetArg(args[n], XmNcursorPositionVisible, True); n++; /*981005*/
  XtSetArg(args[n], XmNwordWrap, True); n++;
  if (textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  /* create the textwindow for MAGE TEXT  */
  textwindow =
    XmCreateScrolledText(toptext,"textwindow", args, n);
  XtAddCallback(textwindow,XmNmotionVerifyCallback,(XtCallbackProc)textcursor_CB,NULL); 
      /*for hypertext*/
  XtManageChild(textwindow);
  XtPopup(toptext,XtGrabNone);  /*RH7.2,7.3 need these done earlier*/
/*printf("textwindow created\n");*/

/*  XtPopup(topcapt,XtGrabNone); */
/*  XtPopup(toptext,XtGrabNone); */ /*RH7.2,7.3 need these done earlier*/
  XtPopup(topgraf,XtGrabNone); /*can do this here after the others*/
/*realize widget hierarchy*/
#ifdef REALIZEEACH  /*not needed*/
  XtRealizeWidget(topcapt);
  XtRealizeWidget(toptext);
  XtRealizeWidget(topgraf); /*realize last: overlays others*/
#endif
/*get the drawing area widget's window ID*/
  drawWindow = XtWindow(drawArea);

#ifndef UNIX_PEX  /*GENERIC UNIX uses offscreen buffer space */
/*create offscreenpixmap*/
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

/*see MUXMDRAW.c where XSetLineAttributes is used frequently  090626*/
/*XSetLineAttributes(dpy,gc,0,LineDoubleDash,CapButt,JoinMiter);*/    /*090626*/
/*XSetLineAttributes(dpy,offgc,0,LineDoubleDash,CapButt,JoinMiter);*/ /*090626*/

if(Ltest)
{
   printf("offscreen created\n");
}
/*Generic Mage: zoom, zslab, and ztrans  are done with on-screen sliders */
   creategrafzoomBar();  /*___BAR.c*/
   creategrafzclipBar(); /*___BAR.c*/
   creategrafztranBar(); /*___BAR.c*/
if(Ltest)
{
   printf("Zbars created\n");
}
#endif /*of NOT UNIX_PEX*/

if(Ltest)
{
   printf("call definecursors()\n");
}
   definecursors();  /*MUXMINIT.C*/
if(Ltest)
{
   printf("rtn  definecursors()\n");
}
   makecursorwait(); /*MUX_MAIN.C*/

   resetmenuchecks(); /*MAGEMENU.c which calls AdjustMenus()*/
   /*AdjustMenus();*/ /*MAGEMENU.c*/ /*deactivate writes until file comes in*/
/*printf("just did AdjustMenus()\n");*/

}/*LGUI*/

if(Ltest)
fprintf(stderr,"ROTWINDWIDTH %d, BWIDTH %d, BUTTWINWIDTH %d, ZWIDTH %d, ALERTHEIGHT %d, FONTSIZE %d\n",ROTWINDWIDTH,BWIDTH,BUTTWINWIDTH,ZWIDTH,ALERTHEIGHT,FONTSIZE); /*121213*/

/*finished mainsetup, set flag so mage will do beginnings() */
   Lbegin = 1;
if(Ltest)
{
   printf("call beginnings()\n");
}
   beginnings(); /*MUXMINIT.C*/
if(Ltest)
{
   printf("leaving mainsetup()\n");
}

}
/*___mainsetup()____________________________________________________________*/

/****beginnings()*************************************************************/
void    beginnings()
{
   int  launched,nth;
    
   launched = 0;
   Lbegin = 0;
   fp = NULL;
   IOerr = 1;    /* Begins asumming no file */ 
       /* and various flags set for no legitimate file */

   launched = Lstartfile; /*gotten from ARGC in MAIN */
 if(LGUI)
 {/*LGUI*/
   makecursoractive(); /*MACMAIN*/
   setMageinitDLOGreport(); /*message*/  /*MAGEINIT.C*/
   /*initial dialog box here for always initial box*/
   /*DoMageinitDLOGreport();*/ 
   
      /*  XRaiseWindow(dpy,textwindow);*/ /* next thing is a message*/
        /* to be shown in the textWindow.  Avoids flipping to graphics */
        /* and messing with the graphics cursor choices */
   
   if( launched != 0) /* invoked by a file */
   {
       makecursorwait();  /*MUX_MAIN.C*/
       IOerr = 0; /* putative .kin file available */
       Lnewfile = 1;
       Lstartfile = 1;
       sprintf(word,"%s\n A request for a kinemage "      /*c*/
                    "file has launched MAGE.",version);
       cleartexts();      /*MUXMWIND.C*/
       inserttexts(word); /*MUXMWIND.C*/
       showthegrafwindow(); /*MUX_MENU.c*/  /*970422*/
   }
   else
   {
       makecursoractive();  /*MACMAIN.C*/
      /*initial dialog box here for no box on file launch*/
      DoMageinitDLOGreport(); 

      sprintf(word,"%s\nUse File pull-down menu above"    /*c*/ 
                   " to Open a kinemage File.",version);

       cleartexts();         /*MUXMWIND.C*/
       inserttexts(word);    /*MUXMWIND.C*/
   }
   completetext();  /*MUXMWIND.c*/ /*011023*/
 }/*LGUI*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
       /*MAGEINIT.C  alloc storage for big arrays*/ /*do this before open file*/
   alloclinkarrays(); /*MAGEINIT.c*/
     /*thelink[0][1000] first indexed display point in this bin*/
     /*thelink[1][1000] last indexed display point in this bin*/
   allocdisplayarrays(0); /*it[8][MAXDRAWS], itpointptr[MAXDRAWS] 080405 was 7*/
   allocwordcomarray(0);
     /* char  *wordcom;  pointer to array, for wordcom[MAXWORDCOM], */
     /*size = MAXWORDCOM*/ /* the comment or wordlist character array  */
   alloccomarray(0); /*ptIDs*/

   if(Lnewfile)
   {
      newfile();  /*MUXMFILE.c*/
      if(Nextrafile > 0) /*021211*/
      {
         for(nth=0; nth<Nextrafile; nth++)
         {
            strcpy(AppendNameStr,ExtraStr[nth]); /*append extra files*/
            Lappend = 1;
            if(nth==Nextrafile-1)
            {/*on last appended file, no more pending*/
               Lextrafile = 0; /*allow kinemage to complete*/
            }
            newfile();  /*MUXMFILE.c*/
         }
      }
   }
}
/*___beginnings()____________________________________________________________*/

/****definecursors()**********************************************************/
void    definecursors()  /*CURSORS*/
{
 XColor  body, back;
 Pixmap mycursor,mymask;
#define xh 0 /*hotspot*/
#define yh 8
static unsigned char curs3D_bits[] = {
0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x0e,0x80,0x03,0xe0,0x79,0xf8,0x0f,0xfe,0x7f,
0xfe,0x7f,0xf8,0x0f,0xe0,0x79,0x80,0x03,0x00,0x0e,0x00,0x18,0x00,0x00,0x00,0x00};
static unsigned char mask3D_bits[] = {
0xff,0xff,0xff,0xc3,0xff,0xc0,0x3f,0xe0,0x1f,0x00,0x07,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x07,0x00,0x1f,0x00,0x3f,0xe0,0xff,0xc0,0xff,0xc3,0xff,0xff};
static unsigned char curs2D_bits[] = {
0xff,0xff,0x00,0x00,0x00,0x18,0x00,0x0e,0x80,0x03,0xe0,0x79,0xf8,0x0f,0xfe,0x7f,
0xfe,0x7f,0xf8,0x0f,0xe0,0x79,0x80,0x03,0x00,0x0e,0x00,0x18,0x00,0x00,0xff,0xff};
static unsigned char mask2D_bits[] = {
0x00,0x00,0x00,0x00,0xff,0xc0,0x3f,0xe0,0x1f,0x00,0x07,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x07,0x00,0x1f,0x00,0x3f,0xe0,0xff,0xc0,0x00,0x00,0x00,0x00};
static unsigned char cursTE3D_bits[] = {
0x09,0x00,0x06,0x00,0x06,0x18,0x06,0x0e,0x86,0x03,0xe2,0x79,0xf8,0x0f,0xfe,0x7f,
0xfe,0x7f,0xf8,0x0f,0xe2,0x79,0x86,0x03,0x06,0x0e,0x06,0x18,0x06,0x00,0x09,0x00};
static unsigned char maskTE3D_bits[] = {
0xf0,0xff,0xf0,0xc3,0xf0,0xc0,0x30,0xe0,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0xe0,0xf0,0xc0,0xf0,0xc3,0xf0,0xff};
static unsigned char cursTE2D_bits[] = {
0xe9,0xff,0x06,0x00,0x06,0x18,0x06,0x0e,0x86,0x03,0xe2,0x79,0xf8,0x0f,0xfe,0x7f,
0xfe,0x7f,0xf8,0x0f,0xe2,0x79,0x86,0x03,0x06,0x0e,0x06,0x18,0x06,0x00,0xe9,0xff};
static unsigned char maskTE2D_bits[] = {
0x00,0x00,0x00,0x00,0xf0,0xc0,0x30,0xe0,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x30,0xe0,0xf0,0xc0,0x00,0x00,0x00,0x00};
static unsigned char cursxy3D_bits[] = {
0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x0e,0x80,0x03,0xe0,0x19,0xf8,0x19,0xfe,0x7f,
0xfe,0x7f,0xf8,0x19,0xe0,0x19,0x80,0x03,0x00,0x0e,0x00,0x18,0x00,0x00,0x00,0x00};
static unsigned char maskxy3D_bits[] = {
0xff,0xff,0xff,0xc3,0xff,0xc0,0x3f,0xe0,0x1f,0xc0,0x07,0xc0,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x07,0xc0,0x1f,0xc0,0x3f,0xe0,0xff,0xc0,0xff,0xc3,0xff,0xff};
static unsigned char cursxy2D_bits[] = {
0xff,0xff,0x00,0x00,0x00,0x18,0x00,0x0e,0x80,0x03,0xe0,0x19,0xf8,0x19,0xfe,0x7f,
0xfe,0x7f,0xf8,0x19,0xe0,0x19,0x80,0x03,0x00,0x0e,0x00,0x18,0x00,0x00,0xff,0xff};
static unsigned char maskxy2D_bits[] = {
0x00,0x00,0x00,0x00,0xff,0xc0,0x3f,0xe0,0x1f,0xc0,0x07,0xc0,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x07,0xc0,0x1f,0xc0,0x3f,0xe0,0xff,0xc0,0x00,0x00,0x00,0x00};
static unsigned char cursz3D_bits[] = {
0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x0e,0x80,0x03,0xe0,0x3d,0xf8,0x42,0xfe,0x5a,
0xfe,0x5a,0xf8,0x42,0xe0,0x3d,0x80,0x03,0x00,0x0e,0x00,0x18,0x00,0x00,0x00,0x00};
static unsigned char maskz3D_bits[] = {
0xff,0xff,0xff,0xc3,0xff,0xc0,0x3f,0xc0,0x1f,0x80,0x07,0x00,0x01,0x3c,0x00,0x24,
0x00,0x24,0x01,0x3c,0x07,0x00,0x1f,0x80,0x3f,0xc0,0xff,0xc0,0xff,0xc3,0xff,0xff};
#define OLDCOLRS
#ifdef OLDCOLRS /*black with white outline*/
    body.red = 0;     body.green = 0;     body.blue = 0;
    back.red = 65535; back.green = 65535; back.blue = 65535;
#else   /*white*/
    body.red = 65535; body.green = 65535; body.blue = 65535;
    back.red = 0;     back.green = 0;     back.blue = 0;
#endif

    waitcursor = XCreateFontCursor(dpy,XC_watch);
/*#ifdef _STDFONTCURSORS_  */
    magecursorF = XCreateFontCursor(dpy,XC_top_left_arrow);
    mage3DcursorF = XCreateFontCursor(dpy,XC_top_left_arrow);
    mage2DcursorF = XCreateFontCursor(dpy,XC_top_left_corner);
    mageTE2DcursorF = XCreateFontCursor(dpy,XC_top_right_corner);
    mageTE3DcursorF = XCreateFontCursor(dpy,XC_left_side);
    magexy3DcursorF = XCreateFontCursor(dpy,XC_fleur);
    magez3DcursorF = XCreateFontCursor(dpy,XC_exchange);
    magexy2DcursorF = XCreateFontCursor(dpy,XC_cross_reverse);
/*    XRecolorCursor(dpy,magecursorF,&body,&back);*/
/*    XRecolorCursor(dpy,mage3DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,mage2DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,mageTE3DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,mageTE2DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,waitcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,magexy3DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,magez3DcursorF,&body,&back);*/
/*    XRecolorCursor(dpy,magexy2DcursorF,&body,&back);*/
/*#else*/
/*BITMAPS*/
mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)curs3D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)mask3D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
mage3DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)curs2D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)mask2D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
mage2DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)cursTE3D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)maskTE3D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
mageTE3DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)cursTE2D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)maskTE2D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
mageTE2DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)cursxy3D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)maskxy3D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
magexy3DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)cursxy2D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)maskxy2D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
magexy2DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

mycursor = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)cursz3D_bits,16,16,incolor[WHITEINDEX],incolor[BLACKINDEX],1);
mymask   = XCreatePixmapFromBitmapData(dpy,drawWindow
,(char*)maskz3D_bits,16,16,incolor[BLACKINDEX],incolor[WHITEINDEX],1);
magez3DcursorB= XCreatePixmapCursor(dpy,mycursor,mymask,&body,&back,xh,yh);

/*#endif*/
changecursors();

} 
/*___definecursors()_________________________________________________________*/

/****changecursors()***********************************************************/
void changecursors()
{
   if(Lfontcursor)
   {
      magecursor     =  magecursorF;   /*Std Font cursors*/
      mage3Dcursor   =  mage3DcursorF;
      mage2Dcursor   =  mage2DcursorF;
      mageTE2Dcursor =  mageTE2DcursorF;
      mageTE3Dcursor =  mageTE3DcursorF;
      magexy3Dcursor =  magexy3DcursorF;
      magez3Dcursor  =  magez3DcursorF;
      magexy2Dcursor =  magexy2DcursorF;
   }
   else
   {
      magecursor     =  magecursorB;   /*Bitmap cursors*/
      mage3Dcursor   =  mage3DcursorB;
      mage2Dcursor   =  mage2DcursorB;
      mageTE2Dcursor =  mageTE2DcursorB;
      mageTE3Dcursor =  mageTE3DcursorB;
      magexy3Dcursor =  magexy3DcursorB;
      magez3Dcursor  =  magez3DcursorB;
      magexy2Dcursor =  magexy2DcursorB;
   }
}
/*___changecursors()_________________________________________________________*/

