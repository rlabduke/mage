/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#ifdef EXTERNDLOG 
#undef  EXTERNDLOG
#define EXTERNDLOG
#else
#define EXTERNDLOG extern
#endif

int  getcharstring(char,int*);

#ifdef PCWINDOWS
/* Dialog box items IDC_ is used by PCwindows, DO NOT USE IDC_ */
#define MAGE_OK      901  /*OK, Yes, accept, proceed*/
#define MAGE_CANCEL  902  /*CANCEL, no action*/
#define MAGE_NO      903  /*NO, abort, kill */
#define MAGE_TEXT    904  /*TEXT, information, message to reader*/
#define MAGE_OPTION  905  /*OPTION, change, write, do something else*/
#define MAGE_OPT1    906  /*OPTION, change, write, do something else*/
#define MAGE_OPT2    907  /*OPTION, change, write, do something else*/
#define MAGE_OPT3    908  /*OPTION, change, write, do something else*/
#define MAGE_OPT4    909  /*OPTION, change, write, do something else*/
#define MAGE_TESTOFF 910  /*report dialog: turns off Ltest 040612*/
#define MAGE_EDIT1   911  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT2   912  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT3   913  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT4   914  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT5   915  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT6   916  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT7   917  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT8   918  /*TEXT, or any changeable stuff*/
#define MAGE_EDIT9   919  /*TEXT, or any changeable stuff*/

#define MAGE_RADIO1  921  /*RADIO BUTTON*/
#define MAGE_RADIO2  922  /*RADIO BUTTON*/
#define MAGE_RADIO3  923  /*RADIO BUTTON*/
#define MAGE_RADIO4  924  /*RADIO BUTTON*/
#define MAGE_CHECK1  927  /*CHECK BUTTON*/
#define MAGE_CHECK2  928  /*CHECK BUTTON*/
#define MAGE_CHECK3  929  /*CHECK BUTTON*/ 
#define MAGE_CHECK4  930  /*CHECK BUTTON*/ 
#define MAGE_ICON    933  /*unique id for icon */

#define MAGE_DEFAULT     110
#define MAGE_ANGLE       111
#define MAGE_SEPARATION  112
#define MAGE_DEFAULTANGLE 113
#define MAGE_CURRENTANGLE 114
#define MAGE_DEFAULTSEPARATION 115
#define MAGE_CURRENTSEPARATION 116
#define MAGE_DRAWLINESON       130
#define MAGE_DRAWLINESOFF      131
#define MAGE_DRAWLINESWRITE    132
#define MAGE_DRAWLINESSHORTEN  133
#define MAGE_DRAWLINESUNPICK   134
#define MAGE_DRAWLINESMONITOR  135

#define MAGE_LISTBOXA        201  /*list box 060619*/
#define MAGE_LISTBOXB        202  /*list box 060619*/
#define MAGE_LISTBOXC        203  /*list box 060619*/

#endif /*PCWINDOWS*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
EXTERNDLOG  float mapsigma,mapfx,mapfy,mapfz,maphalfwidth;
EXTERNDLOG  char  mapcolorstr[16],mapfocusxyzstr[32];
EXTERNDLOG  int   Ldlog_OKHIT;
EXTERNDLOG  int   Ldlog_option;
EXTERNDLOG  int   Ldlog_optionHIT;
EXTERNDLOG  int   Ldlog_cancel;
EXTERNDLOG  int   Ldlog_cancelHIT;
EXTERNDLOG  int   Ldlog_extra;    /*030312*/
EXTERNDLOG  int   Ldlog_extraHIT;
EXTERNDLOG  int   Ldlog_extra2;    /*030312*/
EXTERNDLOG  int   Ldlog_extra2HIT;
EXTERNDLOG  int   Ldlog_subject;
EXTERNDLOG  int   Ldlog_paramA;
EXTERNDLOG  int   Ldlog_textA;
EXTERNDLOG  int   Ldlog_paramB;
EXTERNDLOG  int   Ldlog_textB;
EXTERNDLOG  int   Ldlog_paramC;
EXTERNDLOG  int   Ldlog_textC;
EXTERNDLOG  int   Ldlog_paramD;
EXTERNDLOG  int   Ldlog_textD;
EXTERNDLOG  int   Ldlog_paramE;
EXTERNDLOG  int   Ldlog_textE;
EXTERNDLOG  int   Ldlog_paramF;
EXTERNDLOG  int   Ldlog_textF;
 
EXTERNDLOG  int   Ldlog_listboxA,Ldlog_listboxB,Ldlog_listboxC;  /* 060619*/
EXTERNDLOG  int   NlistboxAitems,NlistboxBitems,NlistboxCitems; /* 060619*/
EXTERNDLOG  int   listboxAitem,listboxBitem,listboxCitem; /* 060619*/
/*EXTERNDLOG  char listboxAitemName[MAXNDIM][MAXNAMECHAR+1];*/ /*060619,061028*/
/*EXTERNDLOG  char  listboxBitemName[MAXNDIM][MAXNAMECHAR+1];*/ /*060619*/ 
/*EXTERNDLOG  char  listboxCitemName[MAXNDIM][MAXNAMECHAR+1];*/ /*060619*/ 
EXTERNDLOG  char** listboxAitemName; /*061111*/
EXTERNDLOG  char** listboxBitemName; /*061111*/
EXTERNDLOG  char** listboxCitemName; /*061111*/

EXTERNDLOG  int   Ldlog_info;
EXTERNDLOG  int   Ldlog_checkA,Ldlog_checkB,Ldlog_checkC,Ldlog_checkD;
EXTERNDLOG  int   Ldlog_checkE,Ldlog_checkF;
EXTERNDLOG  int   Ldlog_radioA,Ldlog_radioB,Ldlog_radioC,Ldlog_radioD;
EXTERNDLOG  int   Ldlog_radioE,Ldlog_radioF,Ldlog_radioG,Ldlog_radioH;
EXTERNDLOG  int   Ldlog_radioI,Ldlog_radioJ;
EXTERNDLOG  int   Ldlog_radioK,Ldlog_radioL,Ldlog_radioM,Ldlog_radioN;
EXTERNDLOG  int   Ldlog_radioO,Ldlog_radioP,Ldlog_radioQ,Ldlog_radioR;
EXTERNDLOG  int   Ldlog_radioS,Ldlog_radioT,Ldlog_radioU,Ldlog_radioV;
EXTERNDLOG  int   Ldlog_radioW,Ldlog_radioX,Ldlog_radioY,Ldlog_radioZ;
EXTERNDLOG  char  dlog_OKstr[128]; /*050922 for box width control 3Dlit-test*/
EXTERNDLOG  char  dlog_cancelstr[32],dlog_optionstr[64];
EXTERNDLOG  char  dlog_extrastr[64],dlog_extra2str[64]; /*030312,030930*/
EXTERNDLOG  char  dlog_paramAstr[32],dlog_paramBstr[32],dlog_paramCstr[32];
EXTERNDLOG  char  dlog_paramDstr[32],dlog_paramEstr[32],dlog_paramFstr[32]; 
EXTERNDLOG  char  dlog_textAstr[128],dlog_textBstr[128],dlog_textCstr[128];
EXTERNDLOG  char  dlog_textDstr[128],dlog_textEstr[128],dlog_textFstr[128]; 
EXTERNDLOG  char  dlog_subjectstr[256],  dlog_infostr[256];
EXTERNDLOG  char  dlog_checkAstr[64],dlog_checkBstr[64],dlog_checkCstr[64];
EXTERNDLOG  char  dlog_checkDstr[64],dlog_checkEstr[64],dlog_checkFstr[64]; 
EXTERNDLOG  char  dlog_radioAstr[64],dlog_radioBstr[64],dlog_radioCstr[64];
EXTERNDLOG  char  dlog_radioDstr[64],dlog_radioEstr[64],dlog_radioFstr[64]; 
EXTERNDLOG  char  dlog_radioGstr[64],dlog_radioHstr[64]; 
EXTERNDLOG  char  dlog_radioIstr[64],dlog_radioJstr[64]; 
EXTERNDLOG  char  dlog_radioKstr[64],dlog_radioLstr[64],dlog_radioMstr[64];
EXTERNDLOG  char  dlog_radioNstr[64],dlog_radioOstr[64],dlog_radioPstr[64]; 
EXTERNDLOG  char  dlog_radioQstr[64],dlog_radioRstr[64]; 
EXTERNDLOG  char  dlog_radioSstr[64],dlog_radioTstr[64],dlog_radioUstr[64];
EXTERNDLOG  char  dlog_radioVstr[64],dlog_radioWstr[64],dlog_radioXstr[64]; 
EXTERNDLOG  char  dlog_radioYstr[64],dlog_radioZstr[64]; 
EXTERNDLOG  int   dlog_checkAint,dlog_checkBint,dlog_checkCint,dlog_checkDint; 
EXTERNDLOG  int   dlog_checkEint,dlog_checkFint;
EXTERNDLOG  int   dlog_radioAint,dlog_radioBint,dlog_radioCint,dlog_radioDint; 
EXTERNDLOG  int   dlog_radioEint,dlog_radioFint,dlog_radioGint,dlog_radioHint;
EXTERNDLOG  int   dlog_radioIint,dlog_radioJint;
EXTERNDLOG  int   dlog_radioKint,dlog_radioLint,dlog_radioMint,dlog_radioNint; 
EXTERNDLOG  int   dlog_radioOint,dlog_radioPint,dlog_radioQint,dlog_radioRint;
EXTERNDLOG  int   dlog_radioSint,dlog_radioTint,dlog_radioUint,dlog_radioVint; 
EXTERNDLOG  int   dlog_radioWint,dlog_radioXint,dlog_radioYint,dlog_radioZint;
EXTERNDLOG  int   ACTIVE_DLOG,NEXT_DLOG,NEXT_DLOG_PARAM,RETURN_DLOG; /*030930*/
EXTERNDLOG  int   LChangeCheckCode; /*971129*/
EXTERNDLOG  char  user1str[32],user2str[32]; /*FIND original strings 051005*/
#define KINE_DLOG       1
#define STEREO_DLOG     2
#define DRAWNEW_DLOG    3
#define FIND_DLOG       4
#define VIEW_DLOG       5
#define CHANGE_DLOG     6
#define FONT_DLOG       7
#define POSTSCR_DLOG    8
#define RASTER3D_DLOG   9
#define CONSTRUCT_DLOG 10
#define COLORSHOW_DLOG 11
#define PICKSHOW_DLOG  12
#define DETAIL_DLOG    13 
#define ROTFORMAT_DLOG 14
#define UPDATE_DLOG    15
#define UPDATEPROBE_DLOG    16
#define UPDATEPREKIN_DLOG   17
#define UPDATEPIPE_DLOG     18
#define UPDATESOCKET_DLOG   19
#define CONSTRUCTFIFTH_DLOG 20
#define CURSOR_DLOG         21
#define QUESTION_DLOG       22
#define MAKETEXTHELP_DLOG   23
#define MAKEKINEMAGE_DLOG   24
#define MAKEHTMLHELP_DLOG   25
#define KIPOUTPUT_DLOG      26
#define PICKMARKER_DLOG     27
#define FILE_DLOG           28
#define BACKGND_DLOG        29
#define MICKEYMOUSE_DLOG    30
#define FINESTEP_DLOG       31
#define IMPORT_DLOG         32
#define USERNAME_DLOG       33
#define FILENAME_DLOG       34
#define MAKEDOCKOBJECT_DLOG 35
#define UPDATECONTOURS_DLOG 36
#define FOOCAVITY_DLOG      37
#define UPDATENOES_DLOG     38
#define DIMENSION_DLOG      39
#define SELECTING_DLOG      40
#define SELECTINGCOLOR_DLOG 41
#define PICKPOINTS_DLOG     42 /*061129*/
#define CONSTRUCTSIXTH_DLOG 43 /*140912*/

#ifndef UNIX_X11 

#define DLOG_OK       1
#define DLOG_CANCEL   2
#define DLOG_OPTION   3
#define DLOG_PARAMA   4
#define DLOG_PARAMB   5
#define DLOG_PARAMC   6
#define DLOG_TEXTA    7
#define DLOG_TEXTB    8
#define DLOG_TEXTC    9
#define DLOG_SUBJECT 10
#define DLOG_INFO    11
#define DLOG_CHECKA  12
#define DLOG_CHECKB  13
#define DLOG_CHECKC  14
#define DLOG_CHECKD  15


#define DLOG_RADIOA  16
#define DLOG_RADIOB  17
#define DLOG_RADIOC  18
#define DLOG_RADIOD  19
#define DLOG_RADIOE  20

#define DLOG_PARAMD  21  /*991015 used for tablefontsize in FONT dialog*/
#define DLOG_TEXTD   22
#define DLOG_CHECKE  23  /*beware, Mac rsrc has hardwired numbers*/
#define DLOG_PARAME  24  /*991015 used for tablefontsize in FONT dialog*/
#define DLOG_TEXTE   25
#define DLOG_CHECKF  26  /*020220*/

#define DLOG_RADIOF  31
#define DLOG_RADIOG  32
#define DLOG_RADIOH  33
#define DLOG_RADIOI  34

#define DLOG_EXTRA   35
#define DLOG_EXTRA2  36 /*this was 35 as of 041031*/

#define DLOG_PARAMF  37  /*041031*/
#define DLOG_TEXTF   38

#define DLOG_RADIOJ  40
#define DLOG_RADIOK  41
#define DLOG_RADIOL  42
#define DLOG_RADIOM  43
#define DLOG_RADION  44
#define DLOG_RADIOO  45
#define DLOG_RADIOP  46
#define DLOG_RADIOQ  47
#define DLOG_RADIOR  48
#define DLOG_RADIOS  49
#define DLOG_RADIOT  50
#define DLOG_RADIOU  51
#define DLOG_RADIOV  52
#define DLOG_RADIOW  53
#define DLOG_RADIOX  54
#define DLOG_RADIOY  55
#define DLOG_RADIOZ  56

#else

EXTERNDLOG Widget fileoutdialog;
EXTERNDLOG Widget openfiledlog;
EXTERNDLOG Widget DLOG_OK, DLOG_CANCEL, DLOG_OPTION, DLOG_EXTRA,DLOG_EXTRA2;
EXTERNDLOG Widget DLOG_PARAMA,DLOG_PARAMB,DLOG_PARAMC,DLOG_PARAMD,DLOG_PARAME;
EXTERNDLOG Widget DLOG_PARAMF;
EXTERNDLOG Widget DLOG_TEXTA,DLOG_TEXTB,DLOG_TEXTC,DLOG_TEXTD,DLOG_TEXTE;
EXTERNDLOG Widget DLOG_TEXTF;
EXTERNDLOG Widget DLOG_SUBJECT, DLOG_INFO;
EXTERNDLOG Widget DLOG_CHECKA,DLOG_CHECKB,DLOG_CHECKC,DLOG_CHECKD,DLOG_CHECKE;
EXTERNDLOG Widget DLOG_CHECKF;
EXTERNDLOG Widget DLOG_RADIOA,DLOG_RADIOB,DLOG_RADIOC,DLOG_RADIOD,DLOG_RADIOE;
EXTERNDLOG Widget DLOG_RADIOF,DLOG_RADIOG,DLOG_RADIOH,DLOG_RADIOI,DLOG_RADIOJ;
EXTERNDLOG Widget DLOG_RADIOK,DLOG_RADIOL,DLOG_RADIOM,DLOG_RADION,DLOG_RADIOO;
EXTERNDLOG Widget DLOG_RADIOP,DLOG_RADIOQ,DLOG_RADIOR;
EXTERNDLOG Widget DLOG_RADIOS,DLOG_RADIOT,DLOG_RADIOU,DLOG_RADIOV,DLOG_RADIOW;
EXTERNDLOG Widget DLOG_RADIOX,DLOG_RADIOY,DLOG_RADIOZ;

EXTERNDLOG Widget DLOG_LISTA,DLOG_LISTB,DLOG_LISTC; /*list box 060619*/

EXTERNDLOG Widget genericdlog,genericbox,genericcolradios,genericrow2radios;
EXTERNDLOG Widget genericrow3radios;
EXTERNDLOG Widget genericrowA,genericrowB,genericrowC,genericrowD,genericrowE;
EXTERNDLOG Widget genericrowF;
EXTERNDLOG Widget genericrowchecks, genericrowradios, genericbuttons;
EXTERNDLOG Widget genericrowchecks2;
EXTERNDLOG Widget genericrowlistbox; /*060619*/
EXTERNDLOG Widget genericlistboxA,genericlistboxB,genericlistboxC; /*060619*/

EXTERNDLOG Widget constructdlog;
EXTERNDLOG Widget constructbox, constructrowA, constructrowB, constructrowC;
EXTERNDLOG Widget constructrowD,constructrowE,constructrowF;
EXTERNDLOG Widget constructtext,constructbuttons,construct_OK,construct_CANCEL;
EXTERNDLOG Widget constructRADIOA, constructRADIOB, constructRADIOC;
EXTERNDLOG Widget constructRADIOD,constructCHECKE,constructCHECKF;
EXTERNDLOG Widget constructrowG,constructrowH;
EXTERNDLOG Widget constructCHECKG,constructCHECKH;
EXTERNDLOG Widget constructdistA, constructdistB, constructdistC;
EXTERNDLOG Widget constructangleA, constructangleB, constructangleC;
EXTERNDLOG Widget constructdihedralA,constructdihedralB,constructdihedralC;

EXTERNDLOG Widget colorshowbox,colorshowtext,colorshowlist,colorshowbuttons;
EXTERNDLOG Widget colorshowtextrow; /*050816*/
EXTERNDLOG Widget colorshowcaveatrow,colorshowcaveat; /*050816*/
EXTERNDLOG Widget colorshowrowradios,colorshow_RADIOA,colorshow_RADIOB;
EXTERNDLOG Widget colorshowDLOG,colorshow_OK,colorshow_CANCEL;
EXTERNDLOG  int   colorshow_radioAint,colorshow_radioBint; 

EXTERNDLOG Widget SELECTINGcolorbox,SELECTINGcolortext; /*061030*/
EXTERNDLOG Widget SELECTINGcolorlist,SELECTINGcolorbuttons;
EXTERNDLOG Widget SELECTINGcolortextrow;
EXTERNDLOG Widget SELECTINGcolorDLOG,SELECTINGcolor_OK,SELECTINGcolor_CANCEL;

EXTERNDLOG Widget pickshowDLOG,pickshowBIGbox;
EXTERNDLOG Widget pickshowtext; /*subject*/
EXTERNDLOG Widget pickshowrowA; /*holds colB, colC, colD, colE */
EXTERNDLOG Widget pickshowcolB; /*group*/
EXTERNDLOG Widget pickshow_grptitle;
EXTERNDLOG Widget pickshow_grpname,pickshow_grpdom,pickshow_grpnobut;
EXTERNDLOG Widget pickshow_grpnoshow,pickshow_grpselect,pickshow_grpdelete;
EXTERNDLOG Widget pickshow_1animate,pickshow_2animate,pickshow_ignorable;
EXTERNDLOG Widget pickshow_foobarrier; /*090912 foo barrier*/
EXTERNDLOG Widget pickshowcolC; /*subgroup*/
EXTERNDLOG Widget pickshow_sgrptitle;
EXTERNDLOG Widget pickshow_sgrpname,pickshow_sgrpdom,pickshow_sgrpnobut;
EXTERNDLOG Widget pickshow_sgrpnoshow,pickshow_sgrpselect,pickshow_sgrpdelete;
EXTERNDLOG Widget pickshowcolD; /*list*/
EXTERNDLOG Widget pickshow_listtitle;
EXTERNDLOG Widget pickshow_listname,pickshow_listdom,pickshow_listnobut;
EXTERNDLOG Widget pickshow_listnoshow,pickshow_listselect,pickshow_listdelete;
EXTERNDLOG Widget pickshowcolE; /*point*/
EXTERNDLOG Widget pickshow_ptIDtitle;
EXTERNDLOG Widget pickshow_pointID,pickshow_ptIDdom,pickshow_ptunpick;
EXTERNDLOG Widget pickshow_pointcolor;

EXTERNDLOG Widget pickshowbuttons,pickshow_OK,pickshow_CANCEL;

EXTERNDLOG Widget pickshowrowM,pickshow_srcgrupname;
EXTERNDLOG Widget pickshow_srcsgrpname,pickshow_srclistname,pickshow_srctitle;
EXTERNDLOG Widget pickshow_srctrgnotes,pickshow_getTARGET;
EXTERNDLOG Widget pickshowrowN,pickshow_trggrupname;
EXTERNDLOG Widget pickshow_trgsgrpname,pickshow_trglistname,pickshow_trgtitle;
EXTERNDLOG Widget pickshowrowradios,pickshow_RADIOA,pickshow_RADIOB;
EXTERNDLOG Widget pickshow_RADIOC,pickshow_RADIOD;
EXTERNDLOG  int   pickshow_radioAint,pickshow_radioBint; 
EXTERNDLOG  int   pickshow_radioCint,pickshow_radioDint; 

EXTERNDLOG Widget pickshow_srctrgbuttons;
EXTERNDLOG Widget pickshow_SOURCE,pickshow_TARGET,pickshow_NEWTARGET;
EXTERNDLOG Widget pickshow_srctrgMOVE,pickshow_srctrgCALC,pickshow_CONTINUE;


EXTERNDLOG Widget questiondlog,questionbox,questiontext;
EXTERNDLOG Widget questionrowA,questionanswer,questionheader;
EXTERNDLOG Widget questionrowB,questionnewpt,questionnewptH;
EXTERNDLOG Widget questionrowC,questionoldpt,questionoldptH;
EXTERNDLOG Widget questionrowD,questiondist,questiondistH;
EXTERNDLOG Widget questionrowE,questionmdist,questionmdistH;
EXTERNDLOG Widget questionrowF,questionmangl,questionmanglH;
EXTERNDLOG Widget questionrowG,questionmdihd,questionmdihdH;
EXTERNDLOG Widget questionbuttons,question_OK,question_CANCEL,question_GOGRAPH;

#endif
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
