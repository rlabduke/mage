 
/*let MAGELIST.c be the only routine to define MAGELIST*/
/*then  MAGELIST will mean extern for all other routines */
#ifdef  MAGELIST
#undef  MAGELIST
#define MAGELIST
#else
#define MAGELIST extern
#endif

#define MAXBONDPATT    37 /*# char in pattern describing rotation restrictions*/

/*#define MAXNAMECHAR    36*/ /*# char excluding terminator of group,etc names*/
/*because of master by name, group, subgroup, and list names must have*/
/*same maximum number of characters, was 14 < 060613 */
/* moved to Mage.h  060619*/

MAGELIST int maxlists,maxalloclists;
MAGELIST int maxsgrps,maxallocsgrps;
MAGELIST int maxgrups,maxallocgrups;
MAGELIST unsigned int Nthisgroupkind;
MAGELIST int actualpickx,actualpicky,actualpickz; /*moved here 061115*/
MAGELIST int pickedpointcolor; /*061124*/
MAGELIST int sourcepointcolor; /*061124*/

 /*----------- multi, variable value, storage for dimensions, etc. --060622--*/
MAGELIST int NX,NY,NZ; /*indices to dimn[]  060121,061028 */
/*MAGELIST char NXstr[MAXNDIM],NYstr[MAXNDIM],NZstr[MAXNDIM];*/ /*MAXNDIM 061028*/
MAGELIST char NXstr[MAXNAMECHAR+1],NYstr[MAXNAMECHAR+1],NZstr[MAXNAMECHAR+1]; /*MAXNAMECHAR+1 061111*/

MAGELIST int paraAXtranx,paraAXtrany,paraAXzoom; /*parallel AXes 061117*/

 typedef struct dimensionstruct { /*dynamic alloc dimension info individually061112*/
    char name[MAXNAMECHAR+1];
    int    Lwrap;
    float  wrap;
    float  min;
    float  max;
    float  scale;
 }dimensionstruct;
MAGELIST struct dimensionstruct* dimensionptr;
    
 typedef struct clusterstruct { /*dynamic alloc cluster info individually061112*/
    char name[MAXNAMECHAR+1];
    unsigned long number;
    double sigma;
    double means;
 }clusterstruct;
MAGELIST struct clusterstruct* clusterptr;
    
MAGELIST int* paraAXnum;

#ifdef OLDCODE
 typedef struct dimnamestruct {   /*060622*/
    char  dimname[MAXNAMECHAR+1];
 }dimnamestruct;
MAGELIST struct dimnamestruct* dimnameNptr;
#endif

 typedef struct calcNstruct {   /*060604 holds sums,means,deviations,sigmas*/
    double dimn[MAXNDIM];
    unsigned long    number;
 }calcNstruct;
MAGELIST struct calcNstruct* sigmaNptr;
MAGELIST struct calcNstruct* meansNptr;

 typedef struct moreinfostruct {
	 struct moreinfostruct* moreinfoptr;
     int   it;
     float f1;
     float f2;
     float f3;
 }moreinfostruct;

 typedef struct pointstruct {
     struct moreinfostruct* moreinfoptr;
     struct pointstruct* previousptr;
     struct pointstruct* nextptr;
     struct liststruct*  listptr;
     long  nameoffset;   /*use offset so name char array can be*/
                         /*reallocated to hold more ptIDnames*/
                         /*use subroutines to store and retrieve ptIDs*/
                         /*so mechanism hidden under, e.g. "thispointptr"*/
                         /*storeptIDstring(), getptIDstring() */
     long  wordoffset;
     unsigned long number;  /*pointstruct: egamick 030225,030403*/
     unsigned int type; 
     unsigned int STATUS;
     unsigned int STYLE;
     unsigned long long master; /*081120 long long rather than just int*/
     int   colorwidth; 
     int   ix;
     int   iy;
     int   iz;
     float fx;
     float fy;
     float fz;
     float radius;
     char aspects[MAXPOINTASPECTS+1];
     float* coordNptr; /*instead of farrayptr 061028, which replaced angle7ptr*/
  }pointstruct;

 typedef pointstruct* pointstructptr;
 typedef pointstructptr* pointptrarray;
 MAGELIST pointptrarray itpointptr;
 MAGELIST pointstructptr prunepoint1ptr[11];
 MAGELIST pointstructptr prunepoint2ptr[11];
 MAGELIST int prunepointflag[11]; 
 MAGELIST pointstructptr dockpointptr[7]; /*pointstruct: egamick 030225*/

 MAGELIST struct pointstruct* firstpointptr;
 MAGELIST struct pointstruct* thispointptr;
 MAGELIST struct pointstruct* lastpointptr;
 MAGELIST struct pointstruct* pickedpointptr;
 MAGELIST struct pointstruct *pickedpointoldptr; /*050916 from MAGEDRAW.c*/
 MAGELIST struct pointstruct* temppickedpointptr;
 MAGELIST struct pointstruct* ctrpickedpointptr; /*070315*/
 MAGELIST struct pointstruct* lastdrawnpointptr;
 MAGELIST struct pointstruct* prelastdrawnpointptr;
 MAGELIST struct pointstruct* measured1pointptr; /*091212 for surveillance*/
 MAGELIST struct pointstruct* measured2pointptr; /*091212*/
 MAGELIST struct pointstruct* measured3pointptr; /*091212*/
 MAGELIST struct pointstruct* measured4pointptr; /*091212*/
 MAGELIST struct pointstruct* lastscaledpointptr;
 MAGELIST struct pointstruct* origpointptr;
 MAGELIST struct pointstruct* searchpointptr;
 MAGELIST struct pointstruct* NEWpointptr; /*020830*/
 MAGELIST struct pointstruct* scorepointptr;/*020830 accompany NEWpointptr*/
 MAGELIST struct pointstruct* answerkeypointptr; /*020901*/
 MAGELIST struct pointstruct* monitorlinepointptr; /*140915 for monitorparam*/
  
  MAGELIST struct pointstruct* beginselectionpointptr;
  MAGELIST struct pointstruct* endselectionpointptr;
  MAGELIST int Lbeginselection;
  MAGELIST int Ldockingscope;
  MAGELIST int Ndockingbondrot;
  MAGELIST int Lzlimitscope;
  MAGELIST int LNDaxesPickedCtr;

 typedef struct liststruct {
     ControlHandle button;
          /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
     struct pointstruct* firstpointptr;
     struct pointstruct* lastpointptr;
     struct sgrpstruct* sgrpptr; 
     struct liststruct* previousptr;
     struct liststruct* nextptr;
     struct liststruct* instanceptr;
     unsigned int type; 
     unsigned int STATUS;
     unsigned int STYLE;
     unsigned int KKIND; /*060121 DIMENSIONKKIND */
     float fwrap; /*060525 re WRAPPINGKKIND used with DIMENSIONKKIND */
     int   NDIM; /*060622 list re DIMENSIONKKIND */
     int   on; 
     int   color; 
     int   width;
     int   owncolor;
     int   bondrot;
     int   resnumb; /*071005 for alternate bases on suitefit beware hybrid36*/
     char  resname[5]; /*071005 for alternate bases on suitefit*/
     
     unsigned long long master; /*081120 long long rather than just int*/
     unsigned int colorset;
     float radius;
     float angle; /*000804*/
     char  wordsfont;
     char  wordsface;
     char  wordssize;
     char  name[MAXNAMECHAR+1];
 }liststruct;


 MAGELIST struct liststruct* firstlistptr;
 MAGELIST struct liststruct* thislistptr;
 MAGELIST struct liststruct* lastlistptr;
 MAGELIST struct liststruct* sourcelistptr; /*061124*/
 MAGELIST struct liststruct* targetlistptr; /*061124*/
 MAGELIST struct liststruct* meansigmaslistptr; /*061126*/
 MAGELIST struct liststruct* pickedlistptr;
 MAGELIST struct liststruct* pickedlistoldptr; /*050916 from MAGEDRAW.c*/
 MAGELIST struct liststruct* temppickedlistptr;
 MAGELIST struct liststruct* marker1listptr;
 MAGELIST struct liststruct* marker2listptr;
 MAGELIST struct liststruct* markergnom1listptr; /*gnomon list vectors*/
 MAGELIST struct liststruct* markergnom2listptr; /*gnomon list labels*/
 MAGELIST struct liststruct* marker1origlistptr;
 MAGELIST struct liststruct* marker2origlistptr;
 MAGELIST struct liststruct* markergnom1origlistptr;/*gnomon list vectors*/
 MAGELIST struct liststruct* markergnom2origlistptr;/*gnomon list labels*/
 MAGELIST struct liststruct* thismarkerlistptr;     /*only used in MAGEDRAW ??*/
 MAGELIST struct liststruct* thismarkeroriglistptr; /*only used in MAGEDRAW ??*/
 MAGELIST struct liststruct* tablemarkerlistptr;

 MAGELIST struct liststruct* measurelinelistptr;
 MAGELIST struct liststruct* measuredotslistptr;
 MAGELIST struct liststruct* monitorlinelistptr;
 MAGELIST struct liststruct* monitordotslistptr;
 MAGELIST struct liststruct* drawmarkeroriglistptr;
 MAGELIST struct liststruct* drawmarker1listptr;
 MAGELIST struct liststruct* drawmarker2listptr;
 MAGELIST struct liststruct* drawmarker3listptr;
 MAGELIST struct liststruct* drawmarker4listptr; /*971122*/
 MAGELIST struct liststruct* drawmarker5listptr; /*020305*/
 MAGELIST struct liststruct* drawmarker6listptr; /*020305*/
 MAGELIST struct liststruct* drawnewlistptr;
 MAGELIST struct liststruct* searchlistptr;
 MAGELIST struct liststruct* origlistptr;
 MAGELIST struct liststruct* answerlistptr;/*020830 for connect-the-dots*/
 MAGELIST struct liststruct* resultlistptr;/*020830 for connect-the-dots*/
 MAGELIST struct liststruct* NEWlistptr;/*020830 color-coded scored lines*/
 MAGELIST struct liststruct* scorelistptr;  /*020830 to hold scorepointptr*/
 MAGELIST struct liststruct* answerkeylistptr;/*020901 for connect-the-dots*/
 MAGELIST struct liststruct* firstdotlistptr;/*100406 for make dot balls*/

 typedef struct sgrpstruct {
     ControlHandle button;
          /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
     struct grupstruct* grupptr;
     struct liststruct* firstlistptr;
     struct liststruct* lastlistptr;
     struct sgrpstruct* previousptr;
     struct sgrpstruct* nextptr;
     struct sgrpstruct* instanceptr;
     unsigned int STATUS;
     unsigned int type;
     unsigned int KKIND; /*060121 DIMENSIONKKIND */
     float fwrap; /*060525 re WRAPPINGKKIND used with DIMENSIONKKIND */
     int   NDIM; /*060622 subgroup re DIMENSIONKKIND */
     int   on; 
     int   balllists; 
     int   bondrot;
     unsigned long long master; /*081120 long long rather than just int*/
     char  name[MAXNAMECHAR+1];
 }sgrpstruct;


 MAGELIST struct sgrpstruct* firstsgrpptr;
 MAGELIST struct sgrpstruct* thissgrpptr;
 MAGELIST struct sgrpstruct* lastsgrpptr;
 MAGELIST struct sgrpstruct* sourcesgrpptr; /*061124*/
 MAGELIST struct sgrpstruct* targetsgrpptr; /*061124*/
 MAGELIST struct sgrpstruct* meansigmassgrpptr; /*061126*/
 MAGELIST struct sgrpstruct* pickedsgrpptr;
 MAGELIST struct sgrpstruct* temppickedsgrpptr;
 MAGELIST struct sgrpstruct* markersgrpptr;
 MAGELIST struct sgrpstruct* markerorigsgrpptr;
 MAGELIST struct sgrpstruct* measuresgrpptr;
 MAGELIST struct sgrpstruct* monitorsgrpptr;
 MAGELIST struct sgrpstruct* drawmarkersgrpptr;
 MAGELIST struct sgrpstruct* drawnewsgrpptr;
 MAGELIST struct sgrpstruct* searchsgrpptr;
 MAGELIST struct sgrpstruct* tablemarkersgrpptr;
 MAGELIST struct sgrpstruct* NEWsgrpptr;
 MAGELIST struct sgrpstruct* answerkeysgrpptr;
 

 typedef struct grupstruct {
     ControlHandle button;
          /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
     struct sgrpstruct* firstsgrpptr;
     struct sgrpstruct* lastsgrpptr;
     struct grupstruct* previousptr;
     struct grupstruct* nextptr;
     struct grupstruct* instanceptr;
     float pointx[3];/*pt on each axis*/
     float pointy[3];/*pt on each axis*/
     float pointz[3];/*pt on each axis*/
     float position[3];/*, and at origin*/
     unsigned int STATUS;
     unsigned int type;
     unsigned int KKIND; /*060121 DIMENSIONKKIND */
     float fwrap; /*060525 re WRAPPINGKKIND used with DIMENSIONKKIND */
     float scaled; /*130309 distance reported = scaled * dist(picked pts)*/
     int   NDIM; /*060121 group re DIMENSIONKKIND */
     int   on; 
     int   balllists; 
     int   bondrot;
     int   bondrotmin;
     int   animate;
     int   moview;
     int   info;
     unsigned long long master; /*081120 long long rather than just int*/
     char  name[MAXNAMECHAR+2];/*allow animate flag & \0 ending*/
     char  pdbfilestr[256]; /*groups can refer to a pdb file*/
 }grupstruct;


 MAGELIST struct grupstruct* firstgrupptr;
 MAGELIST struct grupstruct* thisgrupptr;
 MAGELIST struct grupstruct* lastgrupptr;
 MAGELIST struct grupstruct* sourcegrupptr; /*061124*/
 MAGELIST struct grupstruct* targetgrupptr; /*061124*/
 MAGELIST struct grupstruct* meansigmasgrupptr; /*061126*/
 MAGELIST struct grupstruct* pickedgrupptr;
 MAGELIST struct grupstruct* temppickedgrupptr;
 MAGELIST struct grupstruct* markergrupptr;
 MAGELIST struct grupstruct* measuregrupptr;
 MAGELIST struct grupstruct* monitorgrupptr; /*140915*/
 MAGELIST struct grupstruct* drawgrupptr;
 MAGELIST struct grupstruct* searchgrupptr;
 MAGELIST struct grupstruct* nextanimategrupptr;

 MAGELIST struct grupstruct* tablemarkergrupptr;
 MAGELIST struct grupstruct* NEWgrupptr;
 MAGELIST struct grupstruct* answerkeygrupptr;
 MAGELIST struct grupstruct* referencegrupptr; /*egam 030215*/
 MAGELIST struct grupstruct* mobilegrupptr; /*egam 030215*/
 MAGELIST struct grupstruct* foogrupptr; /*egam 030223*/
 MAGELIST struct grupstruct* dotballgrupptr; /*egam 030323*/


 typedef struct masterstruct {
     ControlHandle button;
          /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
     struct masterstruct* previousptr;
     struct masterstruct* nextptr;
     unsigned int STYLE; /*060212, earlier STATUS was unused*/
     int   on; 
     int   indent;
     int   space;
     unsigned long long  mask; /*081120 long long rather than just int*/
     char  name[MAXNAMECHAR+1];/*allow \0 ending*/
     char  masterchar; /*single char pointmaster code */ /*991210*/
 }masterstruct;

 MAGELIST struct masterstruct* firstmasterptr;
 MAGELIST struct masterstruct* lastmasterptr;
 MAGELIST struct masterstruct* thismasterptr;
 
 MAGELIST  unsigned long long  nmaster,Lmaster; /*081120 long long*/

 typedef struct colorsetstruct {
     struct colorsetstruct* previousptr;
     struct colorsetstruct* nextptr;
     int   color; 
     unsigned int   number;
     char  name[MAXNAMECHAR+1];/*allow \0 ending at end of MAXNAMECHAR chars*/
 }colorsetstruct;
 MAGELIST struct colorsetstruct* firstcolorsetptr;
 MAGELIST struct colorsetstruct* lastcolorsetptr;
 MAGELIST struct colorsetstruct* thiscolorsetptr;
 MAGELIST  int    ncolorset,Lcolorset; 

#define MAXBONDROT 200  /*number of bond rotations provided, 080315 was 100*/
MAGELIST ControlHandle  hgrafbondrot[MAXBONDROT+1];
MAGELIST    int     nbondrot,Lbondrotscope;
MAGELIST    int     Lbrotoption,Lsamescope;

#ifdef PCWINDOWS
#define MAGEBONDROTID 100     /* up to MAXBONDROT ( = 10 ) 10 --- 19 */
  MAGELIST HWND bondrotWindow[MAXBONDROT+1];
#endif /*PCWINDOWS*/

#ifdef UNIX_X11
/*slider Widgets == ControlHandle  hgrafbondrot[MAXBONDROT+1];*/
MAGELIST  Widget bondrotvalue[MAXBONDROT+1], bondrotstart[MAXBONDROT+1];
MAGELIST  Widget bondrotname[MAXBONDROT+1]; 
MAGELIST  Widget bondrotscope[MAXBONDROT+1]; 
MAGELIST  Widget bondrotdivide[MAXBONDROT+1]; 
MAGELIST  GC bondrotpatterngc[MAXBONDROT+1]; 
#endif /*UNIX_X11*/
MAGELIST  char bondrotpattern[MAXBONDROT+1][MAXBONDPATT+1]; 
MAGELIST  int  bondrotgoodcolor[MAXBONDROT+1]; 
MAGELIST  int  bondrotbadcolor[MAXBONDROT+1]; 
MAGELIST  int  bondrotblahcolor[MAXBONDROT+1]; 
MAGELIST  int  bondrotgoodmax[MAXBONDROT+1]; 
MAGELIST  int  bondrotbadmin[MAXBONDROT+1]; 
MAGELIST  int  Lpattern; /*input flag*/

 typedef struct bondrotliststruct {
     struct pointstruct* firstpointptr; /*scope of rotation*/
     struct pointstruct* lastpointptr;
     struct pointstruct* tailpointptr;/*"bond"axis of rotation*/
     struct pointstruct* headpointptr;
     struct sgrpstruct* sgrpptr; 
     struct bondrotliststruct* previousptr;
     struct bondrotliststruct* nextptr;
     int    level;
     int    option;
     int    scopeinstance;
     int    slider; /*draw/NOTdraw slider bar 070617*/
     int    select; /*selected for write-out if slider also visible*/
     int    changed; /*when slider callback invoked, for suitefitroc 071215*/
     float  origangle;
     float  angle;
     char   name[MAXNAMECHAR+1]; /*==list name so can't be longer 040320*/
 }bondrotliststruct;

 MAGELIST struct bondrotliststruct* firstbondrotlistptr;
 MAGELIST struct bondrotliststruct* thisbondrotlistptr;
 MAGELIST struct bondrotliststruct* lastbondrotlistptr;
 MAGELIST struct bondrotliststruct* activebondrotlistptr;

 typedef bondrotliststruct* bondrotliststructptr;
 MAGELIST bondrotliststructptr bondrotptr[MAXBONDROT+1];

/*prototypes*/

struct dimensionstruct*  allocdimensionstructure(void);     /*121107*/
void destroydimensionstructure(dimensionstruct* victimptr); /*121107*/

struct calcNstruct*  alloccalcNstructure(void);     /*060604,061028*/
void destroycalcNstructure(calcNstruct* victimptr);

struct moreinfostruct*  allocmoreinfostructure(void);
void destroymoreinfostructure(moreinfostruct* victimptr);

struct pointstruct*   allocnewpointstructure(liststruct*);
struct pointstruct*   allocpointstructure(liststruct*);
struct pointstruct*   alloclonelypointstructure();
int    copypointstructure(pointstruct*, pointstruct*);
struct pointstruct*  insertpointstructure(pointstruct*,int);
void destroypointstructure(pointstruct*);

int allocspecialintpoint(liststruct*, int, int, int, int);

int  allocinsertedliststructure(liststruct*); /*061216*/
int  allocliststructure(void);
void destroyliststructure(liststruct* victimptr);

int  allocsgrpstructure(void);
void destroysgrpstructure(sgrpstruct* victimptr);

void suppressthisgroup(grupstruct*); /*030220*/
void appendscreentext(void); /*030312*/
void replacecaptiontext(void); /*030315*/

int  allocgrupstructure(void);
void destroygrupstructure(grupstruct* victimptr);

void killkinemage(void); /*030320*/
void killgroups(void);
void killkindofgroups(unsigned int);

int  allocmasterstructure(void);
void destroymasterstructure(masterstruct* victimptr);
void killmasters(void);

int  alloccolorsetstructure(void);
void destroycolorsetstructure(colorsetstruct* victimptr);
void killcolorsets(void);
void    adjustcolorset(unsigned int, int);
void    resetcolorset(int);


struct bondrotliststruct*  allocbondrotliststructure(void);
void destroybondrotliststructure(bondrotliststruct* victimptr);

void storeptIDstring(char* thestring, pointstruct* thepointptr);
void getptIDstring(char* thestring, pointstruct* thepointptr);
void recoverptIDstring(char* thestring, pointstruct* thepointptr); /*970920*/
void storecommentstring(char* thestring, pointstruct* thepointptr);
void getcommentstring(char* thestring, pointstruct* thepointptr);
void calcintegerpoints(pointstruct* thepointptr);
void calcintegerpointscentered(pointstruct* thepointptr,float,float,float);/*130115*/
void calcintegerfixedpoints(pointstruct* thepointptr);
void calcintegerscreenpoints(pointstruct* thepointptr); /*971011*/

/*prototypes that would be expected to be in MAGEhdr.h, BUT...*/
void    fantoscreen(int,int,int,int,int,pointstruct*); 
void    multipointparallels(pointstruct*,int,int,int*);  /*paraAX 061114,15*/ 
   /*needs MAGELIST.h: struct pointstruct*  */
void    multipointparallelaxespre(void);               /*paraAX 061115*/
void    multipointparallelaxespost(void);               /*paraAX 061117*/
void    stackeddiskssphere(int,int,long,int);
void    surfacetriangle(int,int,int,int
         ,int,int,int,int,int,int,int
         ,pointstructptr,pointstructptr,pointstructptr);
void    movepointsbycolor(liststruct*,liststruct*,int);       /*061124*/
void    calcmeansigmas(liststruct*, liststruct*); /*061126*/

/*prototypes moved from MAGEPOST.C on 970705*/
void postscripter(
  long i, int penwidth, int origpenwidth, int icolor
 ,int ideepcolor, int idecue
 ,double sinangle[3],double cosangle[3]
 ,int ixseting[3],int iyset,int izset, int *postscriptcnt, int *inewpoint
 ,float eyeposition 
 ,int xminleft,int xmaxleft,int xminright,int xmaxright,int ymin, int ymax); 
/*+minmax:031127*/
int fcheckzclip(float,float,float,float,float,float,float*,float*,
  float*,float*,float*,float*,int,int); /*for postscript and raster3Drender*/
int fchecksideclip(float jx1,float jy1,float jz1,float jx2,float jy2,float jz2
  ,float *x1,float *y1,float *z1,float *x2,float *y2,float *z2,int type
  ,float xmin, float xmax, float ymin, float ymax); /*031127*/
void finishpostscript(int*);
void loadpointpostscript(long,int,int,pointstructptr
    ,pointstructptr,pointstructptr); /*used for postscript and raster3Drender*/

void raster3Drender(
long i,int* oldcolordepth,int* oldpenwidth,int icolor
 ,int ipen,int ideepcolor,int idecue,double sinangle[3],double cosangle[3]
 ,int ixseting[3],int iyset,int izset, int *inewpoint, float eyeposition
 ,int xminleft,int xmaxleft,int xminright,int xmaxright,int ymin, int ymax); 
/*+minmax:031127*/
void raster3Drenderfinish(void);
void raster3Dtextpreviewer(float,float,float,float,int,int,char[256]);

/*prototypes MUXMDLOG.c calls for MAGEDLOG.c routines needing struct defs 121108*/
int  maketargetgrup(grupstruct* srcgrupptr, char* name); /*121108*/
int  maketargetsgrp(grupstruct* trggrupptr, sgrpstruct* srcsgrpptr, char* name);/*121108*/
int  maketargetlist(sgrpstruct* trgsgrpptr, liststruct* srclistptr, char* name);/*121108*/


/*prototype MUXMINIT.c call for MUXMFILE.c routine 121108*/
void    newfile();

