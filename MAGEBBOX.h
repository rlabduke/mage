#ifdef EXTERNBBOX
#undef  EXTERNBBOX
#define EXTERNBBOX
#else
#define EXTERNBBOX extern
#endif

 
 typedef struct bboxstruct {
     ControlHandle button;
          /*ControlHandle typedef--> HWND for PC, -->Widget for MUX */
     struct bboxstruct* previousptr;
     struct bboxstruct* nextptr;
     int   on; 
     char  name[MAXNAMECHAR+2];/*allow \0 ending*/
 }bboxstruct;

 EXTERNBBOX struct bboxstruct* firstbboxptr;
 EXTERNBBOX struct bboxstruct* lastbboxptr;
 EXTERNBBOX struct bboxstruct* thisbboxptr;


 EXTERNBBOX struct bboxstruct* animbboxptr;
 EXTERNBBOX struct bboxstruct* anim2bboxptr;
 EXTERNBBOX struct bboxstruct* pickbboxptr;
 EXTERNBBOX struct bboxstruct* picktablebboxptr;
 EXTERNBBOX struct bboxstruct* pickshowbboxptr;
 EXTERNBBOX struct bboxstruct* movepointsbboxptr; /*061126*/
 EXTERNBBOX struct bboxstruct* meansigmasbboxptr; /*061126*/
 EXTERNBBOX struct bboxstruct* pickcolorbboxptr;
 EXTERNBBOX struct bboxstruct* emphasisbboxptr;
 EXTERNBBOX struct bboxstruct* zclipbboxptr;
 EXTERNBBOX struct bboxstruct* onscreeninfobboxptr; /*060122*/
 EXTERNBBOX struct bboxstruct* drawlabelsbboxptr;
 EXTERNBBOX struct bboxstruct* drawdihedralbboxptr;
 EXTERNBBOX struct bboxstruct* drawballsbboxptr;
 EXTERNBBOX struct bboxstruct* drawlinebboxptr;
 EXTERNBBOX struct bboxstruct* construct4bboxptr;/*971122*/
 EXTERNBBOX struct bboxstruct* construct5bboxptr;/*971122*/
 EXTERNBBOX struct bboxstruct* construct6bboxptr;/*140912*/
 EXTERNBBOX struct bboxstruct* dock3on3bboxptr;/*020305*/
 EXTERNBBOX struct bboxstruct* tetrabboxptr;/*030217*/
 EXTERNBBOX struct bboxstruct* cubebboxptr;/*030217*/
 EXTERNBBOX struct bboxstruct* octabboxptr;/*030217*/
 EXTERNBBOX struct bboxstruct* widgetbboxptr;/*030217*/
 EXTERNBBOX struct bboxstruct* waterbboxptr;/*091127*/
 EXTERNBBOX struct bboxstruct* lionbboxptr;/*030329*/
 EXTERNBBOX struct bboxstruct* knightbboxptr;/*030329*/
 EXTERNBBOX struct bboxstruct* knavebboxptr;/*030329*/
 EXTERNBBOX struct bboxstruct* caltropbboxptr;/*030329*/
 EXTERNBBOX struct bboxstruct* wallbboxptr;/*040217*/
 EXTERNBBOX struct bboxstruct* extrabboxptr;/*030425*/
 EXTERNBBOX struct bboxstruct* rejectbboxptr;/*030217*/
 EXTERNBBOX struct bboxstruct* repeatbboxptr;/*030223*/
 EXTERNBBOX struct bboxstruct* foofillbboxptr;/*030223*/
 EXTERNBBOX struct bboxstruct* foosurfacebboxptr;/*030329*/
 EXTERNBBOX struct bboxstruct* foosealerbboxptr;/*030411*/
 EXTERNBBOX struct bboxstruct* draglinebboxptr;
 EXTERNBBOX struct bboxstruct* monitorbboxptr;
 EXTERNBBOX struct bboxstruct* paramtriggerbboxptr; /*141006*/
 EXTERNBBOX struct bboxstruct* eraselinebboxptr;
 EXTERNBBOX struct bboxstruct* pickerasebboxptr;
 EXTERNBBOX struct bboxstruct* prunebboxptr;
 EXTERNBBOX struct bboxstruct* punchbboxptr;
 EXTERNBBOX struct bboxstruct* augerbboxptr; /*990211*/
 EXTERNBBOX struct bboxstruct* undopbboxptr;
 EXTERNBBOX struct bboxstruct* SELECTINGlistbboxptr; /*060121,060622list*/
 EXTERNBBOX struct bboxstruct* UnSELECTINGbboxptr; /*060206*/
 EXTERNBBOX struct bboxstruct* oneSELECTINGbboxptr; /*060207*/
 EXTERNBBOX struct bboxstruct* switchbboxptr;  /*090419*/
 EXTERNBBOX struct bboxstruct* controlbboxptr;  /*981010*/
 EXTERNBBOX struct bboxstruct* inhibitbboxptr;
/*MAGEBBOX.C routines*/
void    SetsinGrafWindow(void);
void    UpdateAppendedSets(unsigned int ); /* 030319*/
void    adjustanimate(int);
void    adjustrock(void);
void    adjustmaster(unsigned long long, int); /*081120 long long, not int*/
void    resetmaster(int);
void    buttonboxscan(ControlHandle, int);
void    turnoffallbutoneboxLogical(long);
void    adjustcontrolbuttonboxchecks(void);
void    killgrafcontrols(void);
void    destroyallbboxstructures(void); /*061128*/

int  allocbboxstructure(void);
void destroybboxstructure(bboxstruct* victimptr);

/*MACBBOX routines and MPCBBOX.C; 121105 note also MUXMBBOX.c routines*/
void    destroybuttons(ControlHandle); /*121105 MUXMBBOX.c needs prototype*/ 
void    makebuttonbox(ControlHandle *,int,int,int,int
                            ,char[MAXNAMECHAR+2],int,int,int,int);
void    clearbuttonbox(ControlHandle *); /*061129*/
void    mybuttonspacer(int,int,char[MAXNAMECHAR+2]);
void    getbuttonplace(void);
void    placewindowresizemark(void);
void    buttonboxcheck(ControlHandle,int);
int     testbutton(ControlHandle, ControlHandle);

#ifdef   MACINTOSH               /*MACBBOX.C*/
  void    destroybuttons(ControlHandle); /*980610 this form is Mac specific*/
  /*void    killgrafcontrols(void);*/ /*980610 NOT Mac specific*/
#endif /*MACINTOSH*/ /*MACBBOX.C*/

#ifdef   PCWINDOWS               /*MPCBBOX.C*/
  void    createbuttons(void);
  void    destroybuttons(HWND);
  void    destroyallbuttons(void);
#endif /*PCWINDOWS*/ /*MPCBBOX.C*/ 


