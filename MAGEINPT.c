/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
 
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"  /*MAGEINPT.c is its parent*/
#include "MAGEANGL.h" /* for icurrentrotation */
                      /* for LbondrotHplot,LbondrotVplot */
#include "MAGETABL.h" /* for table window parameters*/
#include "MAGECOLR.h" /*for palette entries*/
#include "MAGEFOO.h"
#ifdef PCWINDOWS
#include "MPCPIPE.h"
#endif
/*#include "MAGE_GLS.H"*/
#include <ctype.h> /*for isdigit()*/
#include <string.h>

#define PDB 1
#define MAP 2

int  Listtype; /*declared extern in MAGEFLAG.H*/ 
int  Liststyle; /*declared extern in MAGEFLAG.H*/
int  LLensflagset; /*declared extern in MAGEFLAG.H*/ 
int  LLenson; /*declared  extern in MAGEFLAG.H*/ 
char masterchar='\0'; /*declared  extern in MAGEFLAG.H*/ 

char reportstr[256];
int  ireport=0;

/*prototypes only referenced within this source file*/
int getnewmatrix(char,int,int);
int getneweuler(char,int,int); /*081120*/
int getviewid(char,int,int); /*view identifier */
int getaspect(char,int,int); /*aspect identifier */ /*010603*/
int getmasterid(char,int*); 
int getsinglequotedchar(char c,int* Lnew);
int getcommandline(char c,int Lnew);
int gettitlestring(char c,int Lnew);
int getcommentinputstring(char c,int Lnew);
int getcopyrightstring(char c,int Lnew);
int getreferencefilestring(char,int,int); 
int getbracketstring(char,int*);
int getinputstring(char,int*);
int getpalette(char,int);
void squarearoundkinemage();

/*keywords, BEWARE: some have NOT been used in published work*/
/* and will NOT be included in the future!*/
/*--->maintain list in MAGEHELP.c*/

/****GetStuffFromFile()******************************************************/
void    GetStuffFromFile()
{
    char     wkeys[256],capts[256],texts[256],buffs[256];
    int     c,ikeyw,icapt,itext,ibuff,n,i,j,k,icounter,icnt,iflag;
    int        flowID, storID, Lfile;  /* switch controls for input */
    static int ncacceptable=0;
    int icacceptable;
    static char previouschar;
    static char rawpreviouschar;
    static char rawcurrentchar;
    int* integerplaceptr;
    float* floatplaceptr;

    enum
    {
         neutID = 1
        ,keywID
        ,grupID
        ,kineID
        ,captID
        ,textID
        ,listID   /*this is overloaded, used for list stuff & point stuff*/
        ,matrID
        ,eulerID /*081120 3 euler angles, then create localmatrix*/
        ,viewID
        ,setsID
        ,mbinsID
        ,zoomID
        ,centerID
        ,sizeID   /*041031*/
        ,axischoiceID /*060613*/
        ,dimensionsID /*060622*/
        ,dimminmaxID /*061121*/
        ,orientID /*000514*/
        ,zclipID
        ,ztranID
        ,stereoID
        ,scaleID
        ,isizeID
        ,paramID
        ,mageID
        ,masterID
        ,pointmasterID  /*991210*/
        ,masterparamID  /*991210*/
        ,lensID    /*970905*/
        ,commandID /*970912*/
        ,spanID    /*980929*/
        ,titleID   /*990129*/
        ,copyrightID   /*990129*/
        ,pdbfileID   /*990622*/
        ,mapfileID   /*030312*/
        ,integerinID /*991012*/
        ,floatinID
        ,colorsetID
        ,nowfindID /*000326*/
        ,aspectID /*010603*/
        ,paletteID /*010604*/
        ,timerID /*010822*/
        ,commentID /*020917*/
        ,nowsetrotID    /*071113*/
        ,nowwriterotID    /*071113*/
    };     
    int Lcmnt,Lcapt,Ltext,Llist,Lpass; /* Logicals for input flow */
    int Lnew,Lprekey,Lcaptstate,Ltextstate,Lknow,Lfirst,Lnext,Lnextnew,itch;
    int Leadingspace = 0;
    int maxviews = 0; /*temp used both for orig and Lappend kins*/
    int maxaspects = 0; /*temp used both for orig and Lappend kins*/ /*010603*/
/*static int  Lstart=0;*/ /*prototype hangs waiting for input from pipe*/  /*991213*/
    int Nblank = 0; /*000410*/
    int Lsetdrawtest = 0; /*010822*/
    int nbondrotold = 0; /*031011*/   
    int IOK = 0; /*051114*/
    float minlocal=0,maxlocal=0; /*061121*/
    char bondstr[256]; /*for nowsetrot 071113*/
    float theta; /*for nowsetrot 071113*/

    /*executable statements:*/

   Linputactive = 1; /*040605*/

if(Ltest)
{
sprintf(alertstr,"MAGEINPT/entered GetStuffFromFile() with Lappend== %d\n"
,Lappend);
dosinglealert(3);
}
    Lidn = 0; /*set within a bracketed identifier to accept all char, incl<>*/
/*    Lstart = 0;*/ /*prototype hangs waiting for input from pipe*/  /*991213*/
    Lmenu = 0; /*Mac sets this on a menu call, but UNIX callbacks need it set */
      /*INPT info from kin file, not from menu, reset Lmenu=1 after file input*/  
    Lnew=1;          /*initialize first encounter of an object */
    Lprekey=0;       /* help skip white spaces */
    Lcaptstate=0;    /*allow @ to be a character inside of a caption*/
    Ltextstate=0;    /*allow @ to be a character inside of appended text*/
    Lcapt = 0;
    Ltext = 0;
    Lknow=0;         /*to stop searching when interpret a key word*/
    flowID = neutID;
    Lpass = 1; /*has to start by accepting any char */
    Lcmnt = 0;
    Lnewframenumber = 0; /*always check for newly inputed frame number*/
    Lnewpalette = 0; /*010604*/
    Lemphasison = 0; /*020112xrc @beginemphasis,@endemphasis*/
    DOscale = TRUE; /* whether any scaling to be done, orthogonal to append,*/
    /* set false if NOSCALE before any vectors */
    Lscale = TRUE;/* which vectors get scaled,*/
    /* set false by NOSCALE control card */
    nbondrotold = nbondrot; /*031011 adjust bond rot window*/
  if(Lappend == 0 || (Lpipeactive && framenumber==-1) )/*effects new kinemage*/
  {/*reset kinemage parameters*/

    killkinemage(); /*MAGELIST.c destroys hierarchies of all data structures*/

    /*MAGEINIT should set framenumber==-1, active pipe augments it*/
    /*framenumber here is that of the previous frame! */
    framenumber = 0; /*default first pass initialized value*/
    Lpreviousscalables = 0;
    Lpresentscalables  = 0;
    Lpreviousbondrotplot = 0;
    L0to360 = 0; /*AXE:bondrotangles shown 0 - 360 , special for UNIX */
    update_command_line[0] = '\0'; /*981125*/
    input_command_line[0] = '\0'; /*990404*/
    update_res_num = 0; /*990622*/
    update_res_probe = 0; /*990622*/
    update_res_num_index = 0; /*990622*/
    Lkinemagecount++; /*increment each kinemage in this file*/
    LLensflagset = 0; /*set if LENSFLAG set for a list, etc. */
                      /*or if @lens factor read in separately*/ /*970917*/
    LLenson = 0; /*but if a lens is set, then expect to show its effects*/
    Ltexteditable = 0; /*980920*/
    Ltablecolheaders = 0; /*000324*/
    Ltablemarkers = 0; /*000327*/
    Lkintitle = 0; /*990129*/
    Lkincopyright = 0;
    Lkinpdbfile = 0;
     
    if(LGUI) clearcapts();   /*____WIND.c*/

    /*write new kinemage number to caption window */
    if(LGUI) insertcapts(KineName);   /*___WIND.c*/
            /*so captWindow will be updated and readable*/

    thiskinenum = kinemagenum;
    /* enters with file positioned within a particular kinemage */
    /* so next @kinemage keyword or EOF ends this section */

    /*Initiate kinemage: reset logicals and counters */
    Lexcessivepoints = 0; /*only warn once per kinemage*/
    LpointIDtoolong = 0;  /*only warn once per kinemage*/
    Lplotonly = FALSE;
    Lflat = FALSE;/*xy rotations instead of translation by mouse*/
    Lscroll = 0; /*xy rotations instead of translation by mouse*/
    Lhorizon = 0; /*ON: mouse only rotates around vertical y axis*/
    Lvisibleout = 0;
    Lfirstcall = TRUE;
    Lplotpoint = FALSE;
    LnextOK = 1;
    Lgnomon = 0;
    Newscale = TRUE; /* set false on first vector set */
    /*max value of 32 bit float is about 10^38 with 7 significant digits*/
    /*9999 is too small to scale some recent odd-ball plots*/
    fxmin =  (float)1E37; /* scaling limits initialized wimperwhump*/
    fxmax = (float)-1E37;
    fymin =  (float)1E37;
    fymax = (float)-1E37;
    fzmin =  (float)1E37;
    fzmax = (float)-1E37;
    x2 = (float)1E37; y2 = (float)1E37; z2 = (float)1E37;

    Scale = 0;   /*set by scalekinemage() by actual data or set to be 1.0 */
    /*sprintf(dstring,"\0");*/ /*121108 compiler warns not good string */
    dstring[0] = '\0'; /*121108 clear distance report on graph screen */
    condition(dstring);
    sprintf(comstr,"%c",' ');   /*clear label report on graph screen*/
    condition(comstr);
    Lset = 0;
    nanimate = 0;
    nnanimate = 0;
    nmaster = 0;
    ncolorset = 0;

    nxyz = 1;    /* counter for triplets of numbers */
    Lmarkers = FALSE;
    Lonewidth = FALSE;
    Lnewstuffactive = 0;
    Lprobeactive = 0;
    LNOEdisplayactive = 0;
    Lwhitebkg = FALSE;
    Ldragline = 0;
    Lnewdrag = 0;
    Lnewmatrix = 0;
    Ldraglineon = 0;
    Ldraglineactive = 0;
    Lnewlabelson = 0; /*980610*/
    Lnewballson = 0; /*020113xrc*/
    Ladddrag = 0; /*dragline will pick up old last point and change line*/
        Lzoom = 1;  /* zoom, zclip, ztran sliders are now constitutive*/
        Lpickcenter = TRUE;
        Lpickcenteron = FALSE;
        Lcenterpoint = FALSE;
        Lzclip = TRUE;
        Lzclipon = TRUE;
        Lztran = TRUE;
        Lmultibins = TRUE;
        multibins = 20;
        
        Lprune = FALSE;
        Lpruneactive = FALSE;
        Lpruneon = FALSE;
        Lpunchon = FALSE;
        Lsuperpunchon = FALSE; /*990211*/
        nundop = 0;
        Lsuperpick = FALSE;
        Lpickshowon = FALSE;
        Lmovepointson = FALSE; /*061126*/
        Lmeansigmason = FALSE; /*061126*/
        Lpickcoloron = FALSE;
        Lpickshow = FALSE;
        Lmovepoints = FALSE; /*061126*/
        Lmeansigmas = FALSE; /*061126*/
        Lpickcolor = FALSE;
        Listcolordominant = FALSE;
        Lmeasures = FALSE;
        Lmonitorparam = FALSE; /*140915 TRUE when DoNucleicAcidParameters...*/
        Lmeasureson = FALSE;
        Lmeasureperpendics = 0; /*030930*/
        Ldrawactive = 0;
        Lnewviewactive = 0;
        Ldrawstuff = 0;
		Lmonitor = 0; /*last line length*/
		Lmonitoron = 0; /*last line length*/
        Lconstruct4on = FALSE; /*971122*/
        Lconstruct5on = FALSE; /*971122*/
        Lconstruct6on = FALSE; /*140912*/
        Ldock3on3 = 0; /*020305*/
        Ldock3on3on = 0; /*020305*/
        Larrowline = FALSE;
        Lpoint = 0;
        shortenline = 0;
        Lcmnt = 0;
        ibuff = 0;
        flowID = neutID;
        Lcapt = 0;
        Ltext = 0;
  
        if(LGUI) erasezoomvalue();
        if(LGUI) erasezclipvalue();
        if(LGUI) eraseztranvalue();
  
        zoom = 1;
        izclip = 200;/*fzclip may not be ==1.0 if screen box resized! */
        iztran = 0;
        ixtran = 0;
        iytran = 0;
        iviewset = 1;
        Llist = 0; /*so an EOF on coord line will still pass last point*/
        
        naspectsthiskinemage = 0; /*global = 0 for new original kin*/ /*010603*/
        maxaspectsperpoint = 0; /*030605*/
        nviewsthiskinemage = 0; /*global = 0 for new original kin*/
        for(i=0; i<= MAXRESETS ; i++) /*970915*/
        {
            Lreset[i] = 0;  /* [1] = 1, see below */
            Lview[i] = 0;   /* [1] = 1, see below */
            Lzoomer[i] = 0; /* [1] = 1, see below */
            zoomold[i] = 1;
            /*scaleold[i] = 1.0;*/
            Lcenter[i] = 0;
            fxcenterold[i] = 0;
            fycenterold[i] = 0;
            fzcenterold[i] = 0;
            Lzcliper[i] = 0; /* [1] = 1, see below */
            izclipold[i] = 200;
            Lztraner[i] = 0; /* [1] = 1, see below */
            iztranold[i] = 0;
            viewcom[i][0] = '\0'; /*comment associated with ith view*/
            sprintf(temps,"View%d",i);
            if(i==MAXRESETS) sprintf(temps,"Reader's View");
            condition(temps);
  
            Lgwidthheightview[i] = 0; /*060613*/
            gwidthview[i]  = 0; /*041031*/
            gheightview[i] = 0; /*041031*/

            if(LGUI) setviewidentifier(i, temps); /*MAGEMENU.C*/
 
            aspect[i][0] = '\0'; /*010603*/
            Laxischoice[i] = 0; /*060613*/
            axischoice[i][0] = 0; /*060613*/
            axischoice[i][1] = 0; /*060613*/
            axischoice[i][2] = 0; /*060613*/
        }
        Lreset[1] = 1;
        Lview[1] = 1;
        Lzoomer[1] = 1;
        Lzcliper[1] = 1;
        Lztraner[1] = 1;
        /*050816 defaults for cold start with, e.g., the foo*/
        ma[1][1] =  1;
        ma[1][2] =  0;
        ma[1][3] =  0;
        ma[1][4] =  0;
        ma[1][5] = -1; /*handedness correction*/
        ma[1][6] =  0;
        ma[1][7] =  0;
        ma[1][8] =  0;
        ma[1][9] =  1;
        ma[0][1] =  1;
        ma[0][2] =  0;
        ma[0][3] =  0;
        ma[0][4] =  0;
        ma[0][5] = -1; /*handedness correction*/
        ma[0][6] =  0;
        ma[0][7] =  0;
        ma[0][8] =  0;
        ma[0][9] =  1;

        currentaspect = 0;
        Laspectonscreen = 1;
        for(i=0; i<=MAXPOINTASPECTS; i++)
        {
            if(i==0) {sprintf(temps,"off");}
            else {sprintf(temps,"%d",i);}
            condition(temps);
  
            if(LGUI) setaspectidentifier(i, temps); /*MAGEMENU.C*/
  
        }
        sprintf(temps,"on screen");
        condition(temps);
  
        if(LGUI) setaspectonscreen(temps,Laspectonscreen); /*MAGEMENU.C*/
 
        if(nbondrot > 0)
        {/*nbondrot>0*/
           killbondrotsliders(1); /*1==kill sliders & structures 070617*/
           nbondrotold = 0; /*031012*/
        }/*nbondrot>0*/
        LbondrotHplot = 0; /*970918*/
        LbondrotVplot = 0; /*970918*/
        LbondrotZplot = 0; /*030819*/
        Lbondrotscope = 0;
        nbondrot = 0;
        Lcontrol = 0;    /*981010*/
        Lcontrolon = 0;  /*981010*/
        Lswitchon = 0;  /*090419*/
        Lparamtrigger = 0; /*141006*/
        Lparamtriggeron = 0; /*141006*/
        
        if(Lkeepstereo) Lstereo = TRUE;
        else            Lstereo = FALSE; /*981114 kinemage default*/
        /*LTBstereo set only in MAGEINIT and by Kluges MENU pulldown*/
        if(Lkeepthinline) /*981015*/
        {
            Lthin = 1;
            Lmultiwidth = 0; 
        }
        else
        {
            Lthin = 0;
            Lmultiwidth = 1; /*981114 kinemage default*/
        }
        Lonewidth = 0; /*981015*/

        if(Lkeepperspective) Lperspec = TRUE;
        else                 Lperspec = FALSE; /*981114 kinemage default*/

        Lcompare = 0;/*981114 kinemage default*/
        Lcompareon = 0;
        Lcomparestart = 0;
       /* nextanimategrup = 0;*/ /*980707*/
        
        Llocalrotation = 0;
        Llocalprecenter = 0;
        Llocalpostcenter = 0;
        Lnewlabels = 0;
        Lnewballs = 0;
        Lsearchatend = 1;
        
        LspecialPS = 0; /* PS presume regular triangles*/
        LRGBPS = 1; /* PS output in terms of Mage RGB palette */
        LR3Dappend = 0; /*set to expect to append to existing R3D file*/
        LR3Dshadow = 0; /*set to have Raster3D shadows for display objects*/
        LR3Dstrokedtext = 1; /*set for stroked text font in R3D output*/
        LMAGEstrokedfont = 0; /*optional stroked text font used on MAGE screen*/
        Lconstructprojection = 0; /*constructline extends by 1/cos(angle)*/
        Lconstructdot = 0;        /*constructline puts dot instead of line*/
        beginselectionpointptr = NULL;
        endselectionpointptr = NULL;
        Lbeginselection = 0;

        memset(parameter,0,MAXPARAMETER);   /* optimized array initialization */
        /*for(iparameter=0;iparameter<=MAXPARAMETER;iparameter++)
            parameter[iparameter] = 0.0;*/
        iparameter = 0; /*981010*/

        ncom = 1; /*reset entry point of ptID strings in com array */
        nwordcom = 1; /*reset entry point of word strings in wordcom array */


        Levelofdetail = 0;/*sphere,triangle,ribbon list will turn this on*/
            /*allows updaterate < updateratelimit to NOT render solids*/
            /*while doing mousedown rotations */
        updateratelimit = updateratelimitdefault; /*set in MAGEINIT.c*/
        
        if(LGUI) resetmenuchecks();/*MACMENU calls AdjustMenus() sets enables*/
        
        /*reset Limit to reporting allocation failures*/
        Limitmaxnum = 0;
        Limitmaxdraws = 0;
        Limitmaxcom = 0;
        Limitmaxwordcom = 0;
        magetimeticklimit = 0; /*long tickcounts == 1/60 second*/ /*010822*/
        magetimeremaining = 0; /*float time remaining decimal minutes 020816*/
        magetesttime = 0; /*float test time decimal minutes 020816*/
        Lmagetimerrunning = 0;
        Lmagetimerset = 0; /*when initiated*/
        Lmagetimer = 0; /*flag for a timed kinemage*/
        Ltestmode = 0; /*flag for test-mode where certain user options limited*/
        Ltestcontinue = 0; /*flags continue status of current test*/
        Lpracticemode = 0; /*practice-mode: overrides testmode*/
        Lpickerase = 0; /*option erase picked point*/
        Lpickeraseon = 0; /*can only pick in new group*/
        answerlistptr = NULL; /*each kinemage can have answer key list*/
        answerkeylistptr = NULL; /*practice mode can reuse validated key copy*/
        resultlistptr = NULL;/*drawnew list result from earlier mage run*/
        Lnosavealert = 0;
        Lscalefonts = 0; /*usually screen labels, info, etc. in smaller font*/ 
        Lrescoring = 0;
        Scoretotal = 0;
        Scorepositive = 0;
        Scorenegative = 0;
        Ldrawlineinput = 0; /*input ---> output*/ /*020917*/
        Ldrawlabelinput = 0;
        Ldrawballinput = 0;
        Ldrawnewinput = 0;
        Lbigfontsinput = 0;
        Lnosavealertinput = 0;
        Lshortlineinput = 0;
        Lcommentinput = 0;
        fpin = fp; 
        Ltablewrapdecades = 0; /*010916*/

        pickedpointcolor = 0; /*061125*/
        sourcepointcolor = 0; /*061125*/

        firstdotlistptr = NULL; /*100407 for make dot balls*/

  }/*reset kinemage parameters*/
  else /*Lappend!=0, starts as ==1, set==2 if hit valid kin contents*/
  {/*appending to this kinemage*/
     /*will later develop an active group pointer*/
     if(Legamickdocking && mobilegrupptr != NULL) /*egamickdocking egam 030216*/
     {
        thisgrupptr = mobilegrupptr; /*egamickdocking  egam 030216*/
        thissgrpptr = mobilegrupptr->lastsgrpptr; /*egamickdocking egam 030216*/
     }
     else
     {
        thisgrupptr = NULL;
     }
     fpin = fpappend; /*reset at end of this routine*/
  }/*appending to this kinemage*/

  previouschar = EOL;/*in strict usage, keyword @ must be first char of a line*/
  rawpreviouschar = EOL;

  Lfile = 1; /*local*/
  while (Lfile)  /* flow controls and logicals together direct the flow */
  {/*begin-loop-while-file-not-at-EOF*/        

#ifdef UNIX_X11
    if(Lpipeactive)
    {/*get characters out of Mage local pipebuffer*/
       c = getfrompipebuffer();
    }
    else
#endif /*UNIX_X11*/
       
#ifdef PCWINDOWS
    if (Lupdate)
    {
       c = winHandleGetc(readpipe);
    }
    else 
#endif 
    {/*regular input routines*/
        if(Linternal)  /*000506*/
        {
           c = getinternalkinemage(Ninternal); /*MAGEHELP.c*/
        }
        else
        {/*regular flow from external file*/
           c = getnextchar(); /*NOTE: c is an int !! */
           /* getnextchar() defined as getc(fpin), fpin == fp or fpappend*/
        }
    }/*regular input routines*/

    rawcurrentchar = c; /*030308*/
    
if(Ltest && Lappend) 
{
   reportstr[ireport++] = (char)c;
   if(c==EOL)
   {
      reportstr[ireport++] = '\0';
      sprintf(alertstr,"%s\n",reportstr); /*which has an end of line anyway*/
      ireport = 0;
      dosinglealert(3);
   }
}
   if(Lpipeactive && Lupdate)
   {/*different behavior with EOF when reading from an active pipe*/
      if(c == EOF) {c = 999;} /*an unacceptable value, char just ignored*/
      /*since characters still have to drop through regular parser flow*/
      /*actually, should be no EOFs when coming in from Mage pipebuffer*/
   }/*different behavior with EOF when reading from an active pipe*/
   else
   {/*usual input stream EOF handling*/
      if(c == EOF)
      {
          Lfile = 0;  /* will terminate loop at next cycle */
          c = EOL; /*whatever int value this is*/
          /* this will allow any active line to finish */
      }
   }/*usual input stream EOF handling*/


      if((int)c < 127 || (int)c == EOL ) 
      {/*lower half of ascii table, not inc del at 127*/
                if((int)c > 31) {icacceptable = 1;} /*above the control set*/
                else if(c=='\t'||c=='\n'||c=='\r'||c==EOL) /*tab,LF,CR,EOL*/
                     {icacceptable=1;}
                else {icacceptable=0;}
      }
      else {icacceptable=0;}
      if(!icacceptable)
      {
          ncacceptable++;
          if(ncacceptable==1)
          {
              sprintf(alertstr,"non flat ascii character (%d) in kinemage"
                CRLF"no further reports of this for this kinemage"
                CRLF"Make sure file is saved as text file",(int)c);
              dosinglealert(0);
          }
      }
      if(icacceptable)
      {/*char acceptable*/ 
        if(Lcmnt)  /* ignore characters until hit a > unless passing to vect*/
        {    /*begin-a-<comment>*/
            if( c == '>' && previouschar != '\\')
            {
                Lcmnt = 0; /* next character will not be ignored */
                /* but Lpass == 1 will let this '>' get to vect */
                flowID = storID;  
                /* note: flowID not used until next char comes in */
                /* if Lpass == 0, but since Lcmnt now 0, Lpass will */
                /* be set to 1 after the if(Lpass){} routine */
            }
        }    /*endof-a-<comment>*/
        if(Lpass)
        {    /*begin-not-a-<comment> or pass anyway*/
            if(c==' ')
            {/*a space character*/
               if(previouschar==EOL || Leadingspace) 
               {/*space at beginning of a line*/
                  Leadingspace = 1;
               }/*allows indenting of keywords*/
            }
            else
            {/*not a space character*/
               if(c!='@')
               {/*drop space flag*/
                  Leadingspace = 0; 
               }
               /*@ may be start of a keyword, allow spaceflag to stay set*/
            }
            if (c == '<' && previouschar != '\\')
            {
                        
                if(   flowID == listID  /*allow comment chars to get to lists */
                   || flowID == setsID  /*allow comment chars to get to subgrp*/
                   || flowID == grupID  /*allow comment chars to get to group*/
                   || flowID == captID  /*allow < char to get to caption*/
                   || flowID == textID  /*allow < char to get to appended text*/
                   || Lidn) /*030626*/  /*allow < char to be in bracketed str*/
                {
                    Lpass = 1;
                }
                else
                { 
                    Lcmnt = 1; /* establish a comment */
                    storID = flowID;  /* comments can be within anything */
                    flowID = neutID;  /* this keeps < char out of flow */
                    Lpass = 0; /*this stops anything later */
                }
            }
            if (c == '\n') c = EOL;/* whatever EOL is, */
            if (c == '\r') c = EOL;/* whatever EOL is, */
                                   /*newline needs to be CR in caption for Mac*/
            if (c == '@' && (previouschar==EOL || Leadingspace)) 
            {/*keyword must be at beginning of line, indenting spaces allowed*/
                storID = flowID;  /*for keywords that can be embedded*/
                flowID = keywID; /* need to get begin's key word */
                ikeyw = 0;  /* set keyw character counter */
                Lprekey = 1; /* Logical to skip blank spaces before key*/
                if(Lcapt)
                {
                    Lcaptstate = 1; 
                    /* allows @ to be in caption if not a keyword */
                }
                else if(Ltext)
                {
                   Ltextstate=1; /*allows @ to be in appended text*/
                }
            }
            switch(flowID)
            {/*general flowID switch loop among cases*/

                case neutID: 
                    Llist = 0; /*980610*/ 
                    /* neutral: no flow designated, discard characters */
                break;                    
                    
                case keywID: 
                    Llist = 0; /*980610*/
                    /* try to interpret next set of characters as a key word*/
                    if((Lcaptstate||Ltextstate) && ibuff<255)
                    {/* continue to accummulate in case this */
                        /* is not a key word */
                        buffs[ibuff] = (char)c;
                        ibuff++;
                    }

                    /* skip blank spaces, or tabs, in the beginning */
                    if((Lprekey && (c == ' ')) || (Lprekey && (c == '\t')))  
                    /* Lprekey set = 1 where keywID set */ /*extra () 121108*/
                    {
                        ;  /* skip white space */
                    }
                    else if(c == '@')
                    {
                        ;  /* @ discarded */
                    }
                    else
                    {
                        Lprekey = 0;  /* hit a non-blank character */
                        ikeyw++; /* ikeyw set =0 when flowID set = keywID */
                        
                        wkeys[ikeyw] = (char)c;
                        
/*c*/                    wkeys[ikeyw] = tolower(wkeys[ikeyw]);  
                                        /*case independent */
                        
                        if( (c == ' ')||(c == '\n')||(c == ',')||(c == ';')
                              ||(c == '\r')||(c == '\t')||(ikeyw >= 21) )
                        {/*stop looking when hit white space or 20 char max*/
                            /* pad ends so can treat wkeys as a real string */
                            wkeys[ikeyw] = '\0';
                            wkeys[0] = ' ';
                            /* try to interpret as a keyword string */
                            
                            if(Ltest)
                            {
/*c*/                          sprintf(alertstr,"Putative keyword: %s",wkeys);
                               dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                            }

                          if(!Lknow)
                          {/*991213*/
                            /* search for finish as key word*/
                                               /* note leading space */
/*.*/                       if(strncmp(wkeys," finish",7) == 0) Lknow=1;
                            if(Lknow && (Lpipeactive||Linternal)) /*020718*/
                            {/*only ends input during loading from active pipe*/
                             /*so simulation output file will serve as as a*/
                             /*multi-frame regular kinemage*/
                             /*or ends from an internal kinemage*/ /*020816*/
                                Lfile = 0;/*will terminate loop at next cycle */
                                c = EOL; /*whatever int value this is*/
                                /* this will allow any active line to finish */
                                /*but this should not be relevent here*/
                                Lpipebufferloaded = 0; /*finished unloading it*/
                                flowID = neutID; 
                                if(!Lkinfile){ Lkinfile = 1;} /*020514,020830*/
                                /*internal 3, from pipe 1 for external kin*/
if(Ltest) printf("found @finish\n");
                            }
                          }

                          if(!Lknow)
                          {
                            /* search for ignore as key word*/
                                               /* note leading space */
/*.*/                       if(strncmp(wkeys," ignore",7) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = neutID; 
                            }
                          }
                          if(!Lknow)
                          {
                            /* search for caption as key word*/
                            Lcapt = 0;            /* note leading space */
/*.*/                       if(strncmp(wkeys," caption",8) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lcapt = 1;
                                flowID = captID; 
                                icapt = 0;
                                Lnew = 1;
                                ibuff = 0;
                            }
                          }
                                                
                          if(!Lknow)
                          {
                            /* search for text as key word*/
                            Ltext = 0;            /* note leading space */
/*.*/                       if(strncmp(wkeys," text",8) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Ltext = 1;
                                flowID = textID; 
                                itext = 0;
                                Lnew = 1;
                                ibuff = 0;
                            }
                          }
                          if(!Lknow)/*030522*/
                          {
/*.*/                       if(  strncmp(wkeys," mapfile",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for filename string*/
                                flowID = mapfileID; /*030522*/
                            }
                          }
                          if(!Lknow)/*990622*/ 
                          {/*this test must preceed @group in parser*/
/*.*/                       if(  strncmp(wkeys," pdbfile",8) == 0
                               ||strncmp(wkeys," grouppdbfile",13)==0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for filename string*/
                                flowID = pdbfileID;
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," group",6) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                flowID = grupID; 
                                Lset = 0; 
                                /* need new set definition for this group*/
                                Lnew = 1;  
                                /*Lnew passed to casegroup: initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                        if(strncmp(wkeys," set",4) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                flowID = setsID; 
                                Lset = 1; /*new set definition here */
                                Lnew = 1;  
                                /*Lnew passed to casegroup: initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," subgroup",9) == 0) Lknow=1;
                            if(Lknow)
                            {
                                flowID = setsID; 
                                Lset = 1; /*new set definition here */
                                Lnew = 1;  
                                /*Lnew passed to casegroup: initialize list*/
                            }
                          }
/*-------------------------_______________Listtype___________________________*/
/**960729 MAGEINPT.c/GetStuffFromFile() in the "switch(flowID)/case keywID:"*/
/*need to recognize "sphere" as a keyword for @spherelist and avoid */
/*confusing with other kinds of vectorlists */
/*960808*/ /*define a "Listtype" that gets set to different values for the*/
/*different types of lists.  Note that Move vs Draw is addition info for */
/*points within some types of lists*/
/*000401*/ /*define STYLE attributes of list and point types*/
/*MAGEFLAG.H*/

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," vector",7) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = VECTOR;  /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," label",6) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = LABEL; /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," dot",4) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = DOT;  /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          /*990209 de-blot*/
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," ball",5) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = BALL;  /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)  /*960729*/
                          {
/*.*/                       if(strncmp(wkeys," sphere",7) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = (SPHERE);/*SPHERE*/
                                                          /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          } /*960729*/
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," triangle",9) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = (TRIANGLE);/*TRIANGLE*/
                                                        /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," ribbon",7) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = (RIBBON); 
                                    /*RIBBON*/ /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," arrow",6) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = (ARROW); /*000804*/
                                /*ARROW == VECTOR | VARIANT3 */ /*MAGEFLAG.H*/
                                  /*has to be VECTOR type */
                                  /*to be shortened when needed*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," word",5) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = WORDS;  /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," mark",5) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = MARKTYPE; /*MAGEFLAG.H*/
                                Liststyle = 0;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," ring",5) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = MARKTYPE;   /*MAGEFLAG.H*/
                                Liststyle = RINGSTYLE;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," fan",4) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = MARKTYPE;   /*MAGEFLAG.H*/
                                Liststyle = FANSTYLE;
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," ellipsoid",9) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = VECTOR;  /*MAGEFLAG.H*/
                                Liststyle = ELLIPSOID; /*080406 @ellipsoidlist*/
                                 /*P center, L major axes radii, multiD 080406*/
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," pixelated",9) == 0) Lknow = 1;
                            if(Lknow) 
                            {
                                Listtype = TRIANGLE;  /*MAGEFLAG.H*/
                                Liststyle = PIXELATED; /*080912 @pixelatedlist*/
                                 /*area filled with z-buffered dots 080912*/
                                flowID = listID; 
                                Lnew = 1;  
                                /*Lnew passed to casevect to initialize list*/
                            }
                          }

/*-------------------------^^^^^^^^^^^^^Listtype^^^^^^^^^^^^^^^^^^^^^^^980907*/
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," kinemage",9) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = kineID; 
                                Lnew = 1;  
                                /* flag start of scan for kinemage number */
                            }
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "aspect"*/
                             if(ikeyw >= 7 && ikeyw <= 9)
                             {/*keyword of a size to hold "aspect"*/
                                for(k=ikeyw,j=6; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                                if(strncmp(word,"aspect",6 ) == 0) /*.*/
                                {/*"aspect": find aspect number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                   }
                                   else
                                   {
                                      word[0]='1'; /*default 1 aspect*/
                                      word[1] = '\0';
                                   }
                                   icounter = intfromstr(word); /*MAGEUTIL*/
                                   /*since kinemages can append each other...*/
                                 icounter = icounter + naspectsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                    flowID = aspectID;
                                       /* flag start of scan for aspect name */ 
                                    if(maxaspects < icounter) maxaspects = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                                }/*"aspect": find aspect number*/
                             }/*keyword of a size to hold "aspect"*/
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," localrotat",11)==0
                               ||strncmp(wkeys," localmatrix",12)==0
                              ) Lknow=1;
                            if(Lknow) 
                            {
                                icounter=0;
                                Llocalrotation = 1;
                                flowID = matrID; 
                                Lnew = 1;  
                                /* flag start of scan for matrix numbers */
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," localeulerradians",15)==0
                              ) Lknow=1;
                            if(Lknow) 
                            {
                                icounter=0;
                                Llocalrotation = 1;
                                flowID = eulerID; 
                                Lnew = 1;  
                                /* flag start of scan for euler angles */
                            }
                          }


                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," fullrgbpalette",15)==0
                              ) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = paletteID; 
                                Lnew = 1;  
                                /* flag start of scan for palette entries */
                                Lnewpalette = 1; /*010604*/
                            
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," endlocalrot",12)==0
                               ||strncmp(wkeys," endlocalmatrix",15)==0
                              ) Lknow=1;
                            if(Lknow) 
                            {
                                Llocalrotation = 0;
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," onewidth",9) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lthin = 0; /*981114*/
                                Lonewidth = 1; /*981114*/
                                Lmultiwidth = 0; /*981114*/
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," compare",8) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lcomparestart = 1;  /* set flag */
                                Lstereo = 0; /*overrides previous keepstereo */
                                  /*everything else will be done in MAGEBBOX.C*/
                                  /*if more than one animate group*/
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          
                          
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," noscale",8) == 0) Lknow =1;
                            if(Lknow) 
                            {
                                DOscale = FALSE;  /* set flag */
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," thinline",9) == 0) Lknow=1;
                            if(Lknow)  /* flag set */
                            {
                                Lthin = 1; /*981114*/
                                Lonewidth = 0; /*981114*/
                                Lmultiwidth = 0; /*981114*/
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                      if(strncmp(wkeys," perspective",12)==0)Lknow=1;
                            if(Lknow)  /* flag set */
                            {
                                Lperspec = 1; 
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  (strncmp(wkeys," flat",5) == 0) 
                               ||(strncmp(wkeys," xytranslation",14) == 0)) 
                               Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lflat = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," plotonly",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lfirstcall = TRUE;
                                Lplotonly = 1;
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," multibin",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = neutID;  /*multibins dummied*/
                            }
                          }

                          /**** VIEWS SECTION ********************************/

                          if(!Lknow)
                          {
                             /*look for trailing char str "matrix"*/
                             if(ikeyw >= 7 && ikeyw <= 9)
                             {/*keyword of a size to hold "matrix"*/
                               /*wkeys[ikeyw]=='\0'*/
                               /*wkeys[0]==' '*/
                               for(k=ikeyw,j=6; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(strncmp(word,"matrix",6 ) == 0) /*.*/
                               {/*"matrix": find view number*/
                                 if(isdigit(wkeys[1])) 
                                 {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                    flowID = matrID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"matrix": find view number*/
                             }/*keyword of a size to hold "matrix"*/
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "viewid"*/
                             if(ikeyw >= 7 && ikeyw <= 9)
                             {/*keyword of a size to hold "viewid"*/
                                /*wkeys[ikeyw]=='\0'*/
                                /*wkeys[0]==' '*/
                                for(k=ikeyw,j=6; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                                if(strncmp(word,"viewid",6 ) == 0) /*.*/
                                {/*"viewid": find view number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                   }
                                   else
                                   {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                   }
                                   icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                    flowID = viewID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                                }/*"viewid": find view number*/
                             }/*keyword of a size to hold "viewid"*/
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "zoom"*/
                             if(ikeyw >= 5 && ikeyw <= 7)
                             {/*keyword of a size to hold "zoom"*/
                                /*wkeys[ikeyw]=='\0'*/
                                /*wkeys[0]==' '*/
                                for(k=ikeyw,j=4; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                                if(strncmp(word,"zoom",4 ) == 0) /*.*/
                                {/*"zoom": find view number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                   }
                                   else
                                   {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                   }
                                   icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = zoomID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                                }/*"zoom": find view number*/
                             }/*keyword of a size to hold "zoom"*/
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "axischoice"*/
                             if(ikeyw >= 11 && ikeyw <= 13)
                             {/*keyword of a size to hold "axischoice"*/
                                /*wkeys[ikeyw]=='\0'*/
                                /*wkeys[0]==' '*/
                                for(k=ikeyw,j=10; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                                if(strncmp(word,"axischoice",10 ) == 0) /*.*/
                                {/*"axischoice": find view number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                   }
                                   else
                                   {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                   }
                                   icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = axischoiceID;
                                       /* flag start of scan for numbers */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                                }/*"axischoice": find view number*/
                             }/*keyword of a size to hold "axischoice"*/
                          }

                          if(!Lknow) /*980929*/
                          {
                             /*look for trailing char str "span"*/
                             if(ikeyw >= 5 && ikeyw <= 7)
                             {/*keyword of a size to hold "span"*/
                               /*wkeys[ikeyw]=='\0'*/
                               /*wkeys[0]==' '*/
                               for(k=ikeyw,j=4; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(strncmp(word,"span",4 ) == 0) /*.*/
                               {/*"span": find view number*/
                                 if(isdigit(wkeys[1])) 
                                 {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = spanID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"span": find view number*/
                             }/*keyword of a size to hold "span"*/
                          }/*980929*/
                          if(!Lknow)
                          {
                             /*look for trailing char str "center"*/
                             if(ikeyw >= 7 && ikeyw <= 9)
                             {/*keyword of a size to hold "center"*/
                               /*wkeys[ikeyw]=='\0'*/
                               /*wkeys[0]==' '*/
                               for(k=ikeyw,j=6; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(strncmp(word,"center",6 ) == 0) /*.*/
                               {/*"center": find view number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = centerID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"center": find view number*/
                             }/*keyword of a size to hold "center"*/
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "size"*/
                             if(ikeyw >= 5 && ikeyw <= 7)
                             {/*keyword of a size to hold "size"*/
                               for(k=ikeyw,j=4; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(strncmp(word,"size",4 ) == 0) /*.*/
                               {/*"size": find view number*/
                                   if(isdigit(wkeys[1])) 
                                   {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = sizeID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"size": find view number*/
                             }/*keyword of a size to hold "size"*/
                          }

                          /*___VIEWS____SECTION______________________________*/

                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," localcenter",12)==0
/*.*/                          ||strncmp(wkeys," localprecenter",15)==0
                              )Lknow=1;

                            if(Lknow) /* flag set */
                            {
                                Llocalprecenter = 1;
                                icounter=0;/*store before usual views*/
                                Lnew = 1;  
                                /* flag start of scan for center setting */                   

                                flowID = centerID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," localpostcenter",16)== 0)
                              Lknow=1;
                                                                  
                            if(Lknow) /* flag set */
                            {
                                Llocalpostcenter = 1;
                                icounter=MAXRESETS+1;/*store beyond usual views*/
                                Lnew = 1;  
                                /* flag start of scan for center setting */                   
                                flowID = centerID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  strncmp(wkeys," endlocalcen",12)==0
                               ||strncmp(wkeys," endlocalprecen",15)==0
                               ||strncmp(wkeys," endlocalpostcen",16)==0
                              ) Lknow=1;
                                                                  
                            if(Lknow) /* flag set */
                            {
                                Llocalprecenter = 0;
                                Llocalpostcenter = 0;
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," gnomon",7)== 0)
                              Lknow=1;
                                                                  
                            if(Lknow && thisgrupptr != NULL) /* flag set */
                            {
                                Lnew = 1;  
                                flowID = orientID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," pickcenter",11)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lpickcenter = TRUE;
                                Lpickcenteron = TRUE;
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," zoomonly",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = neutID;  /*this one does nothing*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," sideclip",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = neutID;  /*this one does nothing*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," zclipoff",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lzclipon = 0;
                                flowID = neutID;  /*this one does something! 980906*/
                            }
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "zslab"*/
                             if(ikeyw >= 6 && ikeyw <= 8)
                             {/*keyword of a size to hold "zslab"*/
                               /*wkeys[ikeyw]=='\0'*/
                               /*wkeys[0]==' '*/
                               for(k=ikeyw,j=5; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(  strncmp(word,"zslab",5 ) == 0
                                   ||strncmp(word,"zclip",5 ) == 0) /*.*/
                               {/*"zslab": find view number*/
                                 if(isdigit(wkeys[1])) 
                                 {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = zclipID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"zslab": find view number*/
                             }/*keyword of a size to hold "zslab"*/
                          }

                          if(!Lknow)
                          {
                             /*look for trailing char str "ztran"*/
                             if(ikeyw >= 6 && ikeyw <= 8)
                             {/*keyword of a size to hold "ztran"*/
                               /*wkeys[ikeyw]=='\0'*/
                               /*wkeys[0]==' '*/
                               for(k=ikeyw,j=5; j>=0; k--,j--) 
                                   word[j] = wkeys[k];
                               if(  strncmp(word,"ztran",5 ) == 0) /*.*/
                               {/*"ztran": find view number*/
                                 if(isdigit(wkeys[1])) 
                                 {
                                      word[0] = wkeys[1];
                                      if(isdigit(wkeys[2])) 
                                      {
                                         word[1] = wkeys[2];
                                         word[2] = '\0';
                                      }
                                      else word[1] = '\0';
                                 }
                                 else
                                 {
                                      word[0]='1'; /*default 1 view*/
                                      word[1] = '\0';
                                 }
                                 icounter = intfromstr(word); /*MAGEUTIL*/
                                 /*nviewsthiskinemage ==0 for a new kinemage*/
                                 icounter = icounter + nviewsthiskinemage;
                                 if(icounter > MAXRESETS) icounter=0;
                                 if(icounter > 0)
                                 {
                                   flowID = ztranID;
                                       /* flag start of scan for number */ 
                                    if(maxviews < icounter) maxviews = icounter;
                                 }
                                 else flowID = neutID; /*ignore*/
                                 Lknow = 1; /*processed keyword*/
                                 Lnew = 1;  
                               }/*"ztran": find view number*/
                             }/*keyword of a size to hold "ztran"*/
                          }

                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," whitebkg",9) == 0  || 
                             strncmp(wkeys," whiteback",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                setwhtbkg(); /*MAGEMENU.C*/
                                /*invokes Pulldown Menu routines */                        
                                flowID = neutID;  /*that's all this one does*/
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," keepstereo",11) == 0) 
                                                                      Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lkeepstereo = 1;
/*Px*/                          forcestereo();
                                /*invokes simulated-Pulldown-Menu routine */                       

                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," keepthinline",13) == 0) 
                                                                      Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lkeepthinline = 1;
/*Px*/                          forcethinline();
                                /*invokes simulated-Pulldown-Menu routine */                       

                                flowID = neutID;  /*that's all this one does*/
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," keepperspec",12) == 0) 
                                                                      Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lkeepperspective = 1;
/*Px*/                          forceperspective();
                                /*invokes simulated-Pulldown-Menu routine */                       

                                flowID = neutID;  /*that's all this one does*/
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," stereoangle",12) == 0) 
                                                                      Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*flag start of scan for stereoangle setting*/                   

                                flowID = stereoID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," scale",6) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*flag start of scan for scale value*/
                                DOscale = FALSE; /*no auto-scaling*/                    
                                flowID = scaleID;
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                      if(strncmp(wkeys," fontsizeinfo",13)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;
                                iflag = 1;    
                                flowID = isizeID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                      if(strncmp(wkeys," fontsizelabel",14)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;
                                iflag = 2;    
                                flowID = isizeID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," fontsizeword",13)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;
                                iflag = 3;    
                                flowID = isizeID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," tablefontsize",14)==0 ||
                               strncmp(wkeys," fontsizetable",14)==0   )Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;
                                iflag = 4;    
                                flowID = isizeID;
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," specialps",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                LspecialPS = 1; /*931128PS*/
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," parameter",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;
                                flowID = paramID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," projection",11) == 0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lconstructprojection = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," constructdot",13)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lconstructdot = 1;
                                flowID = neutID;
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," beginselect",12)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lbeginselection = 1; 
                                /*so know to set beginselectionpointptr*/
                                /*when next point is allocated*/
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," endselect",10)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                endselectionpointptr = thispointptr;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow) /*020112xrc beginemphasis*/
                          {
/*.*/                       if(strncmp(wkeys," beginemphasis",14)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lemphasison = 1; 
                                /*so know to set pointptr type*/
                                /*when next point is allocated*/
                                flowID = storID;
                            }
                          }
                          if(!Lknow) /*020112xrc endemphasis*/
                          {
/*.*/                       if(strncmp(wkeys," endemphasis",10)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lemphasison = 0;
                                flowID = storID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," listcolordom",13)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Listcolordominant = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  (strncmp(wkeys," mage",5) == 0)
                               ||(strncmp(wkeys," prekin",6)==0) ) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for mage version of this kinemage*/
                                flowID = mageID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," master",7) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                masterchar = '\0'; /*reset 030425*/
                                /*flag start of scan for master declaration*/
                                flowID = masterID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," pointmaster",12) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                masterchar = '\0'; /*reset 030425*/
                                /*flag start of scan for master declaration*/
                                flowID = pointmasterID; /*later: sets masterID*/
                            }
                          }

                          if(!Lknow)     /*970905*/
                          {
/*.*/                       if( strncmp(wkeys," lens",5) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for lens radius parameter*/
                                flowID = lensID;
                            }
                          }
                          if(!Lknow)     /*970912*/
                          {
/*.*/                       if( strncmp(wkeys," command",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for command string*/
                                flowID = commandID;
                            }
                          }
                          if(!Lknow)  /*981010*/
                          {
/*.*/                       if(strncmp(wkeys," control",8)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                              Lcontrol = 1; /*buttonbox generic control button*/
                              flowID = neutID;
                            }
                          }/*981010*/
                         if(!Lknow)     /*990129*/
                          {
/*.*/                       if( strncmp(wkeys," title",6) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for title string*/
                                flowID = titleID;
                            }
                          }
                          if(!Lknow)     /*990129*/
                          {
/*.*/                       if( strncmp(wkeys," copyright",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for copyright string*/
                                flowID = copyrightID;
                            }
                          }
                          if(!Lknow)     /*990819*/
                          {
/*.*/                       if( strncmp(wkeys," float",6) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lfloat = 1; /*floating point in inner loop*/
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)     /*990819*/
                          {
/*.*/                       if( strncmp(wkeys," integer",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lfloat = 0; /*NOT floating point in inner loop*/
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)     /*991012*/
                          {
/*.*/                       if( strncmp(wkeys," tablewrapdecade",16) == 0) 
                              {Lknow=1;}
                            if(Lknow) /* flag set */
                            {
                                Ltablewrapdecades = 1; /*MAGETABL..h"*/
                                flowID = neutID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)     /*991012*/
                          {
/*.*/                       if( strncmp(wkeys," tablewrap",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                integerplaceptr = &tablewrapcol;/*MAGETABL..h"*/
                                flowID = integerinID;
                                Lnew = 1;
                            }
                          }

                          if(!Lknow)     /*991012*/
                          {
/*.*/                       if(strncmp(wkeys," tablecolscale",14) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                floatplaceptr = &tablecolscale; /*MAGETABL..h"*/
                                flowID = floatinID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," tablemarkradius",15)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                               floatplaceptr = &tablemarkradius;/*MAGETABL..h"*/
                               flowID = floatinID;
                               Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," tablefancore",12)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                floatplaceptr = &tablefancore; /*MAGETABL..h"*/
                                flowID = floatinID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," tablefanradius",14)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                floatplaceptr = &tablefanradius;/*MAGETABL..h"*/
                                flowID = floatinID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(  (strncmp(wkeys," tableblackbkg",13) == 0) 
                               ||(strncmp(wkeys," tableblackback",14)==0) ) 
                              {Lknow=1;}
                            if(Lknow) /* flag set */
                            {
                                Ltableblackbkg = 1;  
                                flowID = neutID;
                            }
                          }

                          if(!Lknow)     /*991103*/
                          {
/*.*/                       if( strncmp(wkeys," colorset",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = colorsetID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," frame",6) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                integerplaceptr = &framenumber;
                                flowID = integerinID;
                                /*start scan for frame number*/
                                Lnewframenumber = 1;
                            }
                          }
                          if(!Lknow)     /*071113*/
                          {
/*.*/                       if( strncmp(wkeys," nowsetrot",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = nowsetrotID;
                                Lnew = 1; Lfirst=1; Lnext=0,Lnextnew=0;
                            }
                          }
                          if(!Lknow)     /*071113*/
                          {
/*.*/                       if( strncmp(wkeys," nowwriterot",11) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                               Lkinformat = 0;
                               LPDBformat = 1;
                               Lhydrogensout = 1;
                               LhydrogensSep = 0;
                               fphold = fpout; /*store*/
                               fpout = stdout;
                               itch++;
                               fprintf(fpout,"MODEL     %4d\n",itch);
                               writerotated();
                               fprintf(fpout,"ENDMDL\n");
                               fpout = fphold; /*restore*/
                               flowID = neutID;
                            }
                          }
                          if(!Lknow)     /*000326*/
                          {
/*.*/                       if( strncmp(wkeys," nowfind",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = nowfindID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)     /*010822*/
                          {
/*.*/                       if( strncmp(wkeys," minutes",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                flowID = timerID;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," drawline",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {/*setdrawstuff after kin in*/
                                Lsetdrawtest = Lsetdrawtest | 1;
                                Ldrawlineinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," drawlabel",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {/*setdrawstuff after kin in*/
                                Lsetdrawtest = Lsetdrawtest | 2;
                                Ldrawlabelinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," drawball",9) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {/*setdrawstuff after kin in*/
                                Lsetdrawtest = Lsetdrawtest | 4;
                                Ldrawballinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," drawnew",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {/*setdrawstuff after kin in*/
                                Lsetdrawtest = Lsetdrawtest | 8;
                                Ldrawnewinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," bigfont",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {/*setdrawstuff after kin in*/
                                Lscalefonts = 1;
                                Lbigfontsinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if( strncmp(wkeys," nosavealert",12) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnosavealert = 1; /*NOT warn user about new */
                                  /*drawings before doing next kinemage*/
                                Lnosavealertinput = 1;
                                flowID = neutID;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys," shortline",10)==0)Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                floatplaceptr = &shortenline;
                                flowID = floatinID;
                                Ldrawunpickable = 0;
                                Lshortlineinput = 1;
                                Lnew = 1;
                            }
                          }
                          if(!Lknow) 
                          {
/*.*/                       if( strncmp(wkeys," comment",8) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for comment string*/
                                /* expect to set Lcommentinput = 1;*/
                                flowID = commentID;
                            }
                          }

                          if(!Lknow) 
                          {
/*.*/                       if( strncmp(wkeys," dimension",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for dimension name strings*/
                                flowID = dimensionsID;
                                if(dimensionptr == NULL)
                                {  /*alloc array of dimension structures*/
                                  dimensionptr 
                                  = (dimensionstruct*)allocdimensionstructure();
                                }
                                icounter=0;
                            }
                          }

                          if(!Lknow) 
                          {
/*.*/                       if( strncmp(wkeys," dimminmax",10) == 0) Lknow=1;
                            if(Lknow) /* flag set */
                            {
                                Lnew = 1;  
                                /*start scan for dimminmax value pairs*/
                                flowID = dimminmaxID;
                                if(dimensionptr == NULL)
                                {  /*alloc array of dimension structures*/
                                  dimensionptr 
                                  = (dimensionstruct*)allocdimensionstructure();
                                }
                                icounter=0;
                            }
                          }

                          /*______________________________________________*/
                          /* end try to interpret as control string */
                          if(Lknow && (Lcaptstate || Ltextstate)) 
                          {/* real keyword found*/
                              if(Lcaptstate)
                              {
                                 Lcaptstate = 0;
                                 docompletecapt();
                              }
                              else
                              {
                                 Ltextstate = 0;
                                 completetext();
                              }
                          }
                          if(!Lknow && (Lcaptstate || Ltextstate))
                          {/*write key word buffer to capt*/
                              for(n=0;n<=ibuff;n++)
                              {
                                  if(Lcaptstate)
                                 {
                                     capts[icapt] = buffs[n];
                                     if (icapt == 254 || c == '\n' || c == '\r')
                                     {
                                         capts[icapt+1] = '\0';
                                         insertcapts(capts);  /*____WIND*/
                                         icapt = 0;
                                     }
                                     else
                                     {
                                         icapt++;
                                     }
                                  }
                                  else
                                  {
                                     texts[itext] = buffs[n];
                                     if (itext == 254 || c == '\n' || c == '\r')
                                     {
                                         texts[itext+1] = '\0';
                                         inserttexts(texts);  /*____WIND*/
                                         itext = 0;
                                     }
                                     else
                                     {
                                         itext++;
                                     }
                                  }

                              }
                              if(Lcaptstate)
                              {
                                 flowID = captID; /* restore control */
                                 Lcapt = 1;
                                 ibuff = 0;
                                 Lcaptstate = 0;  
                                    /* only in effect during putative keyword*/
                              }
                              else
                              {
                                 flowID = textID; /* restore control */
                                 Ltext = 1;
                                 ibuff = 0;
                                 Ltextstate = 0;  
                                    /* only in effect during putative keyword*/
                              }
                          }
                          else if(!Lknow)/*putative keyword not recognized*/
                          {
                              wkeys[0] = '@';
                              /* restore @ for warning message */
                              if(strncmp(wkeys,"@text",5) == 0)
                              {
                                   sprintf(alertstr,
                                        "%s not expected in this part"
                                        " of kinemage file.",wkeys);
                                   dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
                              }
                              else
                              {
                                   sprintf(alertstr,

                                        "%s not recognized as keyword"
                                        " by this version of MAGE.",wkeys);
                                   dosinglealert(2); /*MACDLOG.C,MPCDLOG.C*/
                              }                                
                              flowID = neutID;  
                              /* skip along until another keyword */
                          }
                          /* reset search flags: */
                          Lknow = 0;
                          ikeyw = 0;
                            
                        }/*stop looking when hit white space or 20 char max*/
                         /* endof-loop at white space after possible keyword*/
                    }/* endof-passed blank spaces before key word */
                break; /* case = keywID */


                case captID: 
                    Llist = 0; /*980610*/
                    if(Lcapt)/*caption expected, send characters to captText*/
                    {
                      if(!Lignorecaption)
                      {/*OK to accummulate caption text*/
                        if(Lnew)
                        {
                            /*clearcapts();*/ /*done in entry()*/
                            Lnew = 0;
                        }
#ifdef UNIX_X11
                        if(c == '\r') c='\n';
#endif
                        capts[icapt] = (char)c;
                        if (icapt >= 253 || c == '\r' || c == '\n')
                        { 
                            capts[icapt+1] = '\0';/*make array of characters*/
                                             /*into properly ended C string*/
                            if(Ltest)
                            {
                                  sprintf(alertstr,"%s",capts);
                                  dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                            }
                            insertcapts(capts); /*MACWIND.C*/ /*MPCWIND.C*/
                            icapt = 0;
                        }
                        else
                        {
                            icapt++;
                        }
                      }/*OK to accummulate caption text*/
                    }
                break;
                case textID: 
                    Llist = 0;
                    if(Ltext)/*text expected, send characters to textText*/
                    {
                      /*always allow appended text*/
                      {/*OK to accummulate appended text*/
                        if(Lnew)
                        {
                            Lnew = 0;
                        }
#ifdef UNIX_X11
                        if(c == '\r') c='\n'; /*this should be superfluous now*/
#endif
                        if(rawcurrentchar=='\n'&&rawpreviouschar=='\r')
                        {
                           ; /*skip last of CR LF sequence*/
                        }
                        else
                        {/*keep character to put into text window*/
                          texts[itext] = c;
                          if (itext >= 253 || c == '\r' || c == '\n')
                          { 
                             texts[itext+1] = '\0';/*make array of characters*/
                                              /*into properly ended C string*/
                             if(Ltest)
                             {
                                  sprintf(alertstr,"%s",texts);
                                  dosinglealert(3); /*____DLOG*/
                             }
                             inserttexts(texts); /*____WIND*/
                             itext = 0;
                          }
                          else
                          {
                             itext++;
                          }
                        }/*keep character to put into text window*/
                      }/*OK to accummulate appended text*/
                    }
                break;
                
                case grupID:
                    Llist = 0; /*980610*/
                    {
                        getgroup(c,Lnew); /*MAGESETS*/
                        Lnew = 0; /* reset for later calls */
                    }
                break;

                case listID:
                    Llist = 1; /*entered a list, expect list stuff*/ /*980610*/
                    {         
                       if(Lappend==1) Lappend = 2;/*presume real kin stuff*/
                       /*overloaded: used for all list stuff & point stuff*/
                       getlist(c,Lnew); /*MAGESETS*/
                       Lnew = 0; /* reset for later calls */
                    }
                break;

                case kineID: 
                    Llist = 0; /*980610*/
                    /* marks legitimate end to previous kinemage */
                    /*when done, getkinemagenum returns as 1  */
                    if( getkinemagenum((char)c,Lnew) ) /*MAGEFILE*/
                    {
                        /* either ends or begins a kinemage*/
                        /*New kinemage request has Lappend==0*/
                        /*any kind of list sets Lappend==2*/
                        if(Lappend!=1)
                           goto finishthiskinemage;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case paletteID: 
                    Llist = 0; /*980610*/
                   /*when 255 sets of 3 numbers read, getpalette returns as 1 */
/*.*/               if( getpalette((char)c,Lnew) ) 
                    {
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;

                
                case matrID: 
                    Llist = 0; /*980610*/
                    /*when 9 numbers read, getnewmatrix returns as 1 */
/*.*/                if( getnewmatrix((char)c,Lnew,icounter) ) 
                    {
                        flowID = neutID;
                        Lview[icounter] = 1;  /* menu choice to access this */
                        Lreset[icounter] = 1; /* matrix defined for this view*/
                    }
                    Lnew = 0;
                break;
                case eulerID:  /*081120*/
                    /*when 3 numbers read, getneweuler returns as 1 */
                    /*after calculating and loading a new localmatrix */
/*.*/               if( getneweuler((char)c,Lnew,0) ) 
                    {
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case viewID:
                    Llist = 0; /*980610*/
                    /*when viewid string read, getviewid returns as 1 */
/*.*/                if( getviewid((char)c,Lnew,icounter) )
                    {
                        flowID = neutID;
                        Lview[icounter] = 1; /* menu choice to access this */
                    }
                    Lnew = 0;
                break;
                case aspectID:  /*010603*/
                    Llist = 0; /*980610*/
                    /*when aspect string read, getaspect returns as 1 */
/*.*/               if( getaspect((char)c,Lnew,icounter) )
                    {
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;

                case masterID:
                    Llist = 0; /*980610*/
                    /*when masterid string read, getmasterid returns as 1 */
/*.*/               if( getmasterid((char)c,&Lnew) )
                    {
                       /*mastermaker() moved out of getmasterid() 051114*/
                       IOK = mastermaker(masterchar); /*name== global word[]*/
                       /* loads global: thismasterptr */
                       /*maybe single masterchar from pointmasterID assignment*/
                       if(IOK)
                       {
                          flowID = masterparamID;
                          Lnew = 3; /*for plain string field*/
                       }
                       else
                       {
                          flowID = neutID;
                          Lnew = 0;
                       }
                    }
                   /*DO NOT set Lnew = 0, set in getmasterid()*/
                break;

                case pointmasterID:
                    Llist = 0; /*991210*/
                    /*when masterchar is in, getsinglequotedchar returns as 1*/
/*.*/               if( getsinglequotedchar((char)c,&Lnew) )
                    {
                        flowID = masterID; /*now needs name for this master*/
                        Lnew = 1; /*for second field*/
                    }
                    /*DO NOT set Lnew = 0, set in getsinglequotedchar()*/
                break;
                case masterparamID:
                    Llist = 0; /*980610*/
                    /*when done, getinputstring returns as 1, loads temps */
/*.*/               if( getinputstring(c,&Lnew) )
                    {
                        /* strncmp returns as 0 if matched*/
                        if(strncmp(temps,"indent",6) == 0)
                        {
                          if((thismasterptr->STYLE & SPACED) != SPACED)
                             {(thismasterptr->indent)++;}
                        }
                        else if(strncmp(temps,"space",5) == 0)
                        {
                          if((thismasterptr->STYLE & SPACED) != SPACED)
                             {(thismasterptr->space)++;}
                        }
                        else if(strncmp(temps,"on",3) == 0) /*030425*/
                        {
                             (thismasterptr->on) = 1;
                        }
                        else if(strncmp(temps,"off",3) == 0) /*030425*/
                        {
                             (thismasterptr->on) = 0;
                        }
                        Lnew = 3;
                        if(c == EOL)
                        {/*keep looking until reach end of the line*/
                            thismasterptr->STYLE |= SPACED; /*060212*/
                              /*space,indent only at 1st encounter of master*/
                              /*so no space,indent is taken as a choice*/
                            flowID = neutID;
                            Lnew = 0;
                        }
                    }
                    /*DO NOT set Lnew = 0 for a string field*/
                break;
                                
                case setsID:
                    Llist = 0; /*980610*/
                    {                
/*S*/                   getsubgroup(c,Lnew);
                        Lnew = 0; /* reset for later calls */
                    }
                break;
                case mbinsID: /*obselete, retained for backwards compatibility*/
                    Llist = 0; /*980610*/
                    /* needs integer for multibins */
                    /*when done, getinteger returns as 1 */
                   /*multibins function is now done automatically in drawvec()*/
/*.*/               if( getinteger(c,Lnew) ) 
                    {
                        flowID = neutID;
                        iflag = 0;
                    }
                    Lnew = 0;
                break;
                
                case zoomID:
                    Llist = 0; /*980610*/
                    /* needs float for zoom value */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        zoomold[icounter] = floatpass; 
                        /* floatpass global to pass value back */
                        if(zoomold[icounter] == 0.0) zoomold[icounter] = 1.0;
                        
                        if(icounter == 1)
                        {/*update current stored zoom value 980906*/
                            zoom = zoomold[icounter];
                        }
                        flowID = neutID;
                        Lzoom = 1; /*set true if any zooms invoked */
                        Lzoomer[icounter] = 1;
                        Lview[icounter] = 1; /* menu choice to access this */
                        if(Ltest)
                        {
                            sprintf(alertstr,"zoomold[%d]= %f",icounter
                                ,zoomold[icounter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }

                    }
                    Lnew = 0;
                break;
                case spanID:
                    Llist = 0; /*980610*/
                    /* needs float for span value */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        zoomold[icounter] = floatpass; 
                        /* floatpass global to pass value back */
                        if(zoomold[icounter] == 0.0) zoomold[icounter] = 1.0;
                        
                        flowID = neutID;
                        Lzoom = 1; /*set true if any zooms invoked */
                        Lzoomer[icounter] = -1; /*for span values*/
                        Lview[icounter] = 1; /* menu choice to access this */
                        if(Ltest)
                        {
                            sprintf(alertstr,"span at zoomold[%d]= %f",icounter
                                ,zoomold[icounter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }

                    }
                    Lnew = 0;
                break;
                case centerID:
                    Llist = 0; /*980610*/
                    /* needs 3 floats for center values */
                    /*when done, getfloat returns as 1 */
                    if(Lnew) 
                    {
                        icnt = 0;
                        iflag = 1;/* used as Lnew by getfloat() */
                        Lnew = 0;
                    }
                    if(getfloat(c,iflag) )
                    {        /* floatpass global pass value back */
                        icnt++;
                        if(icnt == 1) 
                        { /*postcenter xyz is carried in MAXRESETS+1*/
                            if(icounter <= MAXRESETS+1)
                              fxcenterold[icounter] = floatpass;   
                            iflag = 1;
                        }
                        else if(icnt == 2) 
                        {
                            if(icounter <= MAXRESETS+1)
                              fycenterold[icounter] = floatpass;
                            iflag = 1;
                        }
                        else if(icnt == 3) 
                        {
                            if(icounter <= MAXRESETS+1)
                              fzcenterold[icounter] = floatpass;
                        }
                    }
                    else
                    {
                        iflag = 0;
                    }
                    
                    if(icnt == 3)
                    {
                        flowID = neutID;
                        if(icounter <= MAXRESETS)
                        {/*could be an actual stored center xyz*/
                          Lcenterpoint = 1; /* set for any centering */
                          Lcenter[icounter] = 1;
                          Lview[icounter] = 1; /* menu choice to access this */
                        } /*only postcenter xyz is carried in MAXRESETS+1*/
                    
                        if(Ltest)
                        {
                            sprintf(alertstr,"i-xyz-centerold[%d]= %f %f %f"
                                ,icounter
                                ,fxcenterold[icounter]
                                ,fycenterold[icounter]
                                ,fzcenterold[icounter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                    }
                    if(c=='\015' && icnt < 3)
                    {
                        sprintf(alertstr,
                            "@...center failed to get 3 numbers:?! %f %f %f",
                            fxcenterold[icounter],
                            fycenterold[icounter],
                            fzcenterold[icounter]);
                        dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
                        if(icounter == 0) Llocalprecenter = 0;
                        if(icounter == MAXRESETS+1) Llocalpostcenter = 0;
                        flowID = neutID;
                    }
                break;
                
                case sizeID: /*041031*/
                    Llist = 0; /*980610*/
                    /* needs 2 integers for width, height values */
                    /*when done, getinteger returns as 1 */
                    if(Lnew) 
                    {
                        icnt = 0;
                        iflag = 1;/* used as Lnew by getinteger() */
                        Lnew = 0;
                    }
                    if(getinteger(c,iflag) )
                    {        /* intpass global pass value back */
                        icnt++;
                        if(icnt == 1) 
                        {
                            if(icounter <= MAXRESETS)
                              gwidthview[icounter] = intpass;   
                            iflag = 1;
                        }
                        else if(icnt == 2) 
                        {
                            if(icounter <= MAXRESETS)
                              gheightview[icounter] = intpass;
                            iflag = 1;
                        }
                    }
                    else
                    {
                        iflag = 0;
                    }
                    
                    if(icnt == 2)
                    {
                        flowID = neutID;
                        if(icounter <= MAXRESETS)
                           Lgwidthheightview[icounter] = 1; /*060613*/
                    
                        if(Ltest)
                        {
                            sprintf(alertstr,"view:gwidth,height[%d]= %d %d"
                                ,icounter
                                ,gwidthview[icounter]
                                ,gheightview[icounter]
                                );
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                    }
                    if(c=='\015' && icnt < 2)
                    {
                        sprintf(alertstr,
                            "@...size failed to get 2 numbers:?! %d %d",
                            gwidthview[icounter],
                            gheightview[icounter]);
                        dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
                        flowID = neutID;
                    }
                break;
                
                case axischoiceID: /*060613*/
                    Llist = 0;
                    /* needs 3 integers for axischoice values */
                    /*when done, getinteger returns as 1 */
                    if(Lnew) 
                    {
                        icnt = 0;
                        iflag = 1;/* used as Lnew by getinteger() */
                        Lnew = 0;
                    }
                    if(getinteger(c,iflag) )
                    {        /* intpass global pass value back */
                       /*axes count from 1, array indexed from 0 */
                       iflag = 1; /*so getinteger() will start over*/
                       if(intpass >= 1 && intpass <= MAXNDIM) /*061113 was 7*/
                       {/*in acceptable range*/
                          icnt++; 
                          if(icnt == 1) 
                          {
                              if(icounter <= MAXRESETS)
                                axischoice[icounter][0] = intpass -1;   
                          }
                          else if(icnt == 2) 
                          {
                              if(icounter <= MAXRESETS)
                                axischoice[icounter][1] = intpass -1;
                          }
                          else if(icnt == 3) 
                          {
                              if(icounter <= MAXRESETS)
                                axischoice[icounter][2] = intpass -1;
                          }
                       }
                    }
                    else
                    {
                        iflag = 0;
                    }
                    
                    if(icnt == 3)
                    {
                        flowID = neutID;
                        if(icounter <= MAXRESETS)
                        {/*could be actual axischoice numbers*/
                          /* set for axischoice this view */
                          Laxischoice[icounter] = 1;
                          Lview[icounter] = 1; /* menu choice to access this */
                        } 
                    
                        if(Ltest)
                        {
                            sprintf(alertstr,"axischoice[%d]= %d %d %d"
                                ,icounter
                                ,axischoice[icounter][0]
                                ,axischoice[icounter][1]
                                ,axischoice[icounter][2]
                                );
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                    }
                    if(c=='\015' && icnt < 3)
                    {
                        sprintf(alertstr,
                           "@%daxischoice failed to get 3 numbers:?!",icounter);
                        dosinglealert(0);
                        flowID = neutID;
                    }
                break;

                case orientID:
                    Llist = 0; /*980610*/
                    /* needs 12 floats for point on each axis and center*/
                    /*when done, getfloat returns as 1 */
                    if(Lnew) 
                    {
                        icnt = 0;
                        iflag = 1;/* used as Lnew by getfloat() */
                        Lnew = 0;
                    }
                    if(getfloat(c,iflag) )
                    {        /* floatpass global pass value back */
                       if(icnt < 3)
                          thisgrupptr->pointx[icnt] = floatpass; 
                       else if(icnt < 6)
                         thisgrupptr->pointy[icnt-3] = floatpass; 
                       else if(icnt < 9)
                         thisgrupptr->pointz[icnt-6] = floatpass; 
                       else if(icnt < 12)
                         thisgrupptr->position[icnt-9] = floatpass; 
                       iflag = 1;
                       icnt++;
                    }
                    else
                    {
                        iflag = 0;
                    }
                    
                    if(icnt == 12)
                    {
                        flowID = neutID;

                    }
                break;
                
                case zclipID:
                    Llist = 0; /*980610*/
                    /* needs integer for zclip value */
                    /*when done, getinteger returns as 1 */
/*.*/                if( getinteger(c,Lnew) ) 
                    {
                        izclipold[icounter] = intpass; 
                        /*intpass global to pass int back */
                        Lzcliper[icounter] = 1;
                        if(icounter == 1)
                        {/*update current stored zclip value 980906*/
                            izclipold[0] = izclipold[icounter];/*970915*/
                            if(izclipold[0]==0) Lzclipon = 0;/*970915*/
                        }
                        Lzclip = 1; /*need things set up*/
                        flowID = neutID;
                        if(Ltest)
                        {
                            sprintf(alertstr,"izclip[%d]= %d",icounter
                                ,izclipold[icounter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                        Lview[icounter] = 1; /* menu choice to access this */
                    }
                    Lnew = 0;
                break;

                case ztranID:
                    Llist = 0; /*980610*/
                    /* needs integer for ztran value */
                    /*when done, getinteger returns as 1 */
/*.*/                if( getinteger(c,Lnew) ) 
                    {
                        iztranold[icounter] = intpass; 
                        /*intpass global to pass int back */
                        Lztraner[icounter] = 1;
                        if(icounter == 1)
                        {/*update current stored iztran value 980906*/
                            iztranold[0] = iztranold[icounter];
                        }
                        Lztran = 1; /*need things set up*/
                        flowID = neutID;
                        if(Ltest)
                        {
                            sprintf(alertstr,"iztran[%d]= %d",icounter
                                ,iztranold[icounter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                        Lview[icounter] = 1; /* menu choice to access this */
                    }
                    Lnew = 0;
                break;


                case stereoID:
                    Llist = 0; /*980610*/
                    /* needs float for stereo value */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        stereoangle = floatpass; 
                        /* floatpass global to pass value back */
                        flowID = neutID;
                        if(Ltest)
                        {
                            sprintf(alertstr,"stereoangle= %f"
                                ,stereoangle);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }

                    }
                    Lnew = 0;
                break;

                case scaleID:
                    Llist = 0; /*980610*/
                    /* needs float for scale value */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        Scale = floatpass; 
                        /* floatpass global to pass value back */
                        flowID = neutID;
                        if(Ltest)
                        {
                            sprintf(alertstr,"Scale= %f"
                                ,Scale);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }

                    }
                    Lnew = 0;
                break;

                case isizeID: 
                    Llist = 0; /*980610*/
                    /* needs integer for font size value */
                    /*when done, getinteger returns as 1 */
/*.*/                if( getinteger(c,Lnew) ) 
                    {
                        if(iflag==1)
                        {
                           infofontsize = intpass;
                           Linfofontsize = 1;
                        }
                        if(iflag==2)
                        {
                           labelsfontsize = intpass;
                           Llabelsfontsize = 1;
                        }
                        if(iflag==3)
                        {
                           wordsfontsize = intpass; 

                           Lwordsfontsize = 1;
                        }
                        if(iflag==4)
                        {
                           tablefontsize = intpass;
                           Ltablefontsize = 1;
                        }
                        flowID = neutID;
                        iflag = 0;
                    }
                    Lnew = 0;
                break;
               
               case paramID:
                   Llist = 0; /*980610*/
                    /* arbitrary float for programmer to play with */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        if(iparameter+1<=MAXPARAMETER)
                        {
                            iparameter = iparameter +1;
                            parameter[iparameter] = floatpass; 
                        }
                        else
                        {
                            sprintf(alertstr,"parameter= %f, "
                                "exceeds maximum number of parameters (%d)"
                                ,floatpass,MAXPARAMETER);
                            dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
                        }
                        /* floatpass global to pass value back */
                        flowID = neutID;
                        if(Ltest)
                        {
                            sprintf(alertstr,"parameter[%d]= %f"
                                ,iparameter,parameter[iparameter]);
                            dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
                        }
                    }
                    Lnew = 0;
                break;
                case mageID:
                    Llist = 0; /*980610*/
                    /* needs float for mage version */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        KINVERSION = floatpass; 
                        /* floatpass global to pass value back */
                        flowID = neutID;
                        if( (KINVERSION > MAGEVERSION && Lformattest)||(Ltest) )
                        {
                            sprintf(alertstr
                              ,"This kinemage flagged to be made for MAGE %.3f "
                              CRLF"but this is only MAGE %.3f"
                            ,KINVERSION,MAGEVERSION);
                            dosinglealert(3);   
                        }                     
                    }
                    Lnew = 0;
                break;
                case lensID:   /*970905*/
                    Llist = 0; /*980610*/
                    /* needs float for lens radius parameter for "lens" lists */
                    /*when done, getfloat returns as 1 */
                    if(getfloat(c,Lnew) )
                    {
                        lensFactor = floatpass; 
                        /* floatpass global to pass value back */
                        LLensflagset = 1; /*has lens value, expects lens lists*/
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case commandID:   /*970912*/
                    Llist = 0; /*980610*/
                    /* needs command character string for, e.g. remote update*/
                    /*when done, getcommandline returns as 1 */
                    /*stores in global input_command_line[256] 990404*/
                    if(getcommandline((char)c,Lnew) )
                    {
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case titleID:   /*990129*/
                    Llist = 0; 
                    /*when done, gettitlestring returns as 1 */
                    /*stores in global kintitlestr[256] */
                    if(gettitlestring((char)c,Lnew) )
                    {
                        Lkintitle = 1; 
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case commentID:   /*020917*/
                    Llist = 0; 
                    /*when done, getcommentinputstring returns as 1 */
                    /*stores in global commentinputstr[256] */
                    if(getcommentinputstring((char)c,Lnew) )
                    {
                        Lcommentinput = 1; 
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case copyrightID:   /*990129*/
                    Llist = 0; 
                    /*when done, getcopyrightstring returns as 1 */
                    /*stores in global kincopyrightstr[256] */
                    if(getcopyrightstring((char)c,Lnew) )
                    {
                        Lkincopyright = 1;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case pdbfileID:   /*990622*/
                    Llist = 0; 
                    /*when done, getreferencefilestring returns as 1 */
                    /*stores in global kinpdbfilestr[256] */
                    if(getreferencefilestring((char)c,Lnew,PDB) )
                    {
                        Lkinpdbfile = 1;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case mapfileID:   /*030312*/
                    Llist = 0; 
                    /*when done, getreferencefilestring returns as 1 */
                    /*stores in global kinmapfilestr[256] */
                    if(getreferencefilestring((char)c,Lnew,MAP) )
                    {
                        Lkinmapfile = 1;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case integerinID:   /*991012*/
                    Llist = 0; 
                    /* needs integer value */
                    /*when done, getinteger returns as 1 */
/*.*/               if( getinteger(c,Lnew) ) 
                    {
                        *integerplaceptr = intpass;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case floatinID:   /*991012*/
                    Llist = 0; 
                    /* needs float value */
                    /*when done, getfloat returns as 1 */
/*.*/               if( getfloat(c,Lnew) ) 
                    {
                        *floatplaceptr = floatpass;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;
                case colorsetID:   /*991103*/
                    Llist = 0; 
                    /* needs two strings, first in {}, second naked */
                    /*when done, getbracketstring returns as 1, loads numstr */
                    /*when done, getinputstring returns as 1, loads temps */
/*.*/               if(Lnew < 2)
                    {/*first part*/
/*.*/                  if(getbracketstring(c,&Lnew) ) /*Lnew 1 to 0*/
                       {
                          Lnew = 3; 
                       }
                    }
                    else
                    {/*second part*/
                       if(getinputstring(c,&Lnew) ) /*Lnew 3 to 2*/
                       {
                          j = getnewcolorset(numstr,temps);
                          /*returns int== colorindex, but not tested here*/
                          flowID = neutID;
                       }
                    }
                    /*Lnew reset by pointer in subroutines*/
                break;
                case nowfindID:   /*000326*/
/*.*/              if(getbracketstring(c,&Lnew) ) /*Lnew 1 to 0*/
                   {
                       for(j=0;j<256;j++)
                       {
                          search1str[j] = numstr[j];
                          if(search1str[j] == ' ') Nblank++;/*000410*/
                          if(search1str[j] == '\0') break;
                       } 
                       isearch1 = j;
                       if(isearch1 == Nblank) isearch1 = 0;/*000410*/
                       isearch2 = 0;
                       Lpickcenteron = 1;
                       Lsearchbegin = 1;
                       if(isearch1 > 0)/*000410*/
                       {
                          mysearchpointID(1);/*MAGEUTIL.c for justone hit*/
                       }
                       Lpickcenteron = 0; /*011208 singleshot*/
                   }
                   /*Lnew reset by pointer in subroutines*/
                break;
                case nowsetrotID:   /*0711113*/
/*.*/              if(Lfirst && getbracketstring(c,&Lnew) ) /*Lnew 1 to 0*/
                   {
                       for(j=0;j<256;j++)
                       {
                          bondstr[j] = numstr[j];
                          if(bondstr[j] == '\0') break;
                       } 
                       Lfirst = 0;
                       Lnext = 1;
                       Lnextnew = 1;
                       Lnew = 1; /*initiate getfloat*/
                       floatpass = 0.0;
                   }
                   else if(Lnext && getfloat(c,Lnew) )
                   {  /*expect bond value in floatpass*/
                       if(nbondrot > 0)
                       {/*there are bondrotations */
                          for(i=1; i<=nbondrot; i++)
                          {/*loop over all existing bondrots to match name*/
                             if(strncmp(bondstr,bondrotptr[i]->name,j) == 0)
                             {/*name match*/
                                icurrentrotation = i;
                                theta = floatpass - bondrotptr[i]->angle;
                                if(theta > 180)
                                   theta = theta - 360; /*001017*/
                                else if(theta < -180)
                                   theta = 360 + theta;

                                bondrotptr[i]->angle = floatpass;
                                resetgrafbondrotBar(i);/*____BAR.c */
                                dobondrotrouter(i,theta); /*MAGEANGL.c*/ 
                                rescalekinemage(); /*MAGEINPT.c*/
                                /* rescale rebuilds from float arrary*/
                                break;/*out of loop over all existing bondrots*/
                             }/*name match*/
                          }/*loop over all existing bondrots to match name*/
                       }/*there are bondrotations */
                       flowID = neutID;
                   }
                   /*Lnew reset by pointer in some subroutines*/
                   if(Lnextnew)
                   {
                      Lnew=1;
                      Lnextnew = 0;
                   }
                   else
                   {
                      Lnew = 0; /*getfloat() does not reset Lnew*/
                         /* ? this is part of logic of multiple float inputs?*/
                   }
                break;
                case timerID:   /*010822*/
                    Llist = 0; 
                    /* needs float value of minutes*/
                    /*when done, getfloat returns as 1 */
/*.*/               if( getfloat(c,Lnew) ) 
                    {
                        magetesttime = floatpass;
                        magetimeremaining = magetesttime;
                        flowID = neutID;
                    }
                    Lnew = 0;
                break;

                case dimensionsID:   /*060622, 061112*/
/*.*/              if(getbracketstring(c,&Lnew) ) /*Lnew 1 to 0*/
                   {  /*returns char string in global numstr */
                     if(icounter < MAXNDIM && dimensionptr != NULL)
                     {
                       for(j=0;j<MAXNAMECHAR;j++)
                       {
                          dimensionptr[icounter].name[j] = numstr[j];
                          if(numstr[j] == '\0') {break;}
                       }
                       NDIM = icounter + 1; /*the global NDIM */
                       LNdimnames = 1; /*have at least one dimension's name*/
                       icounter++; /*actual index of putative next name*/
                       Lnew = 1; /*anticipate another dimension name*/ 
                     }
                     else
                     {  /*exceeds max number of dimensions, ignore name*/
                        /*eventually: realloc dimensionptr and continue*/
                        /*adding dimensions */
                        /*Also can realloc paraAXnum[] array */
                        /*As of 061118 require dimensions statement?? */
                        flowID = neutID;
                     }
                   }
                   /*Lnew reset by pointer in subroutines*/
                break;

                case dimminmaxID:   /*061121*/
                   /*needs 2 float values per dimension*/
                   if(Lnew)
                   {
                      icnt = 0;
                      iflag = 1; /* used as Lnew by getfloat() */
                      Lnew = 0;
                   }
                   if(getfloat(c,iflag) )
                   {        /* floatpass global pass value back */
                      icnt++;
                      if(icnt == 1) 
                      {
                         minlocal = floatpass;
                         iflag = 1;
                      }
                      else if(icnt == 2)
                      {
                         maxlocal = floatpass;
                      }
                   }
                   else
                   {
                      iflag = 0;
                   }

                   if(icnt == 2) /*store this min max pair*/
                   {
                     if(icounter < MAXNDIM && dimensionptr != NULL)
                     {
                       dimensionptr[icounter].min = minlocal;
                       dimensionptr[icounter].max = maxlocal;
                       icounter++;
                       Lnew = 1; /*anticipate another min max pair*/
                     }
                     else
                     {/*exceeds max number of dimensions, ignore rest of line*/
                        /*eventually: realloc dimensionptr and continue*/
                        /*adding dimensions */
                        flowID = neutID;
                     }
                   }
                   if(c=='\015' ) /*reached end of this input line*/
                   {
                      flowID = neutID;
                   }
                   /*Lnew reset by pointer in subroutines*/
                break;
            }/*general flowID switch loop among cases*/
        }    /*endof-not-a-<comment> or pass anyway*/
        if(Lcmnt == 0) Lpass = 1; /* only comments are blocked*/
        previouschar = c;
        rawpreviouschar = rawcurrentchar; /*030308*/
      }/*char acceptable*/  
  }/*endof-loop-while-file-not-at-EOF*/
/*if(Ltest)*/
{
sprintf(alertstr,"just got to endof-loop-while-file-not-at-EOF");
dosinglealert(3);
}

    if(Lcapt)  /* finish out anything in caption buffer */
    {
      if(!Lignorecaption)
      {/*OK to accummulate caption text*/
        capts[icapt+1] = '\0';
        if ( icapt >  0 )    
        {
            insertcapts(capts);
			docompletecapt(); /*MPCWIND does whole caption*/
        }
      }/*OK to accummulate caption text*/
    }
    if(Ltext)  /* finish out anything in text buffer */
    {
      /*always can accummulate appended text*/
      {/*OK to accummulate appended text*/
        texts[itext+1] = '\0';
        if ( itext >  0 )    
        {
            inserttexts(texts);
			completetext(); /*MPCWIND does whole text*/
        }
      }/*OK to accummulate appended text*/
    }

    if(Llist)  /* finish out anything still pending in vector list */
    {
        c = '\015';  
        /*CR will end any number, the most likely useful thing maybe pending*/
        getlist(c,Lnew);
    }
        atEOF = 1;
        LnextOK = 0;

        if(Ltest)
        {
             sprintf(alertstr,"Reached EOF in input file");
             dosinglealert(3); /*MACDLOG.C,MPCDLOG.C*/
        }
    
    finishthiskinemage:
if(Ltest)
{
sprintf(alertstr,"just got to finishthiskinemage\n");
dosinglealert(3);
}
        if(Lbondrotscope) /*current bond rotation active*/
        {    /*end all current bond rotation stuff*/
            for(j=1; j<=nbondrot; j++)  /*010902*/
            {/*scan over all bondrots from 1 to nbondrot*/
              if(bondrotptr[j]->lastpointptr == NULL)
              {
                bondrotptr[j]->lastpointptr = thispointptr;
              }
            }
            Lbondrotscope = 0;
        }
        if(beginselectionpointptr != NULL && endselectionpointptr == NULL)
        {
            endselectionpointptr = thispointptr;
        }
        for(i=1; i<= MAXPOINTASPECTS ; i++)
        {/*set aspect names*/
            if(aspect[i][0] != '\0')
            {
                if(i==0) {sprintf(temps,"off");}
                else {sprintf(temps,"%d: %s",i,aspect[i]);}
                condition(temps);
                if(LGUI) setaspectidentifier(i, temps);
                Laspect[i] = 1;
            }
        }
        for(i=1; i< MAXRESETS ; i++)
        {
            if(viewcom[i][0] != '\0')
            {
                sprintf(temps,"View%d %s",i,viewcom[i]);
                condition(temps);
                if(LGUI) setviewidentifier(i, temps);
            }
        }

		if (LLensflagset)
		{
			LLenson = 1;
		}
        maxnxyz = nxyz;    /*just coords so far*/

        if(   fxmax != -9999 || fxmin != 9999
           || fymax != -9999 || fymin != 9999
           || fzmax != -9999 || fzmin != 9999 )
        {
           Lpresentscalables = 1;
           if(Lappend==0) Lpreviousscalables = 1; /*set for later append*/
        }
        if(Lappend==0 || (Lpreviousscalables==0 && Lpresentscalables==1) )
        { 
           scalekinemage();  /*.*/
           if(Lpresentscalables) Lpreviousscalables = 1; /*set for next time*/
           if(!Lmarkers)
              addmarkers();  /*.*/ /*will reset maxnxyz if successful*/
        }
        /*move setupbondrotplot stuff   030821*/
        if(nbondrot > 1 && LbondrotHplot > 0 && LbondrotHplot <= nbondrot
                        && LbondrotVplot > 0 && LbondrotVplot <= nbondrot)
        {/*plotting bondrots */
           if(!Lpreviousbondrotplot)
           {/*setup for plotting bondrots */
              squarearoundkinemage();/*970918*/
              Lpreviousbondrotplot = 1;
           }
        }
        else
        {/*make sure will not try to plot bondrots*/
           LbondrotHplot = 0; LbondrotVplot = 0; LbondrotZplot = 0;
        }

        rescalekinemage();/*.*/ /*apply zooming, centering, etc.*/
        /*RWIDTH sets window width if bondrot sliders changed*/
        isavewidth = RWIDTH;
        if(nbondrot > 0) 
        {
            RWIDTH = ROTSWIDTH; /*100 Mac, fraction of screen PC */
        }
        else
        {
            RWIDTH =   0;
            icurrentrotation = 0;
        }
        if(  (RWIDTH != isavewidth)
#ifdef UNIX_X11
           ||(nbondrot != nbondrotold) /*031011*/
#endif
          )
        {/*UNIX_X11: changing bondrot window can destroy the window*/ 
            adjustwindowwidth(); /*MACWIND.C,MPCWIND.C,MUXMWIND.c*/
              /*where topbrot == bondrot window is created if needed, UNIX_X11*/
            if(RWIDTH == ROTSWIDTH) /*bondrots just now started */
               icurrentrotation = 1; /*970917*/ /*reset to first rotation*/
               /*so will always be defined either initially or by append*/
        }

        if(Lappend==0) 
        {
          if(Lpickcenter)/*always true since very early MAGE*/
          {/*update current stored center value 980906*/
            fxcenterold[0] = fxcenterold[1]; /*initialize to 1st view*/
            fycenterold[0] = fycenterold[1];
            fzcenterold[0] = fzcenterold[1];
          }
        }
        if(Lzclip)/*always true since very early MAGE*/
        {
            ;    
        }

        if(Lappend==0) 
        {
           izclip = (int)(izclipold[0]*fzclip); /*970915*/
           /*fzclip scales zclip & ztran to resized screen box*/
           iztran = (int)(iztranold[0]*fzclip); /*970915*/
           /*971014*/
        }
        if(Lsetdrawtest > 0 && !Ldrawstuff)
        {/*@drawline can only turn drawline ON for connect-the-dots vectors*/
           /*similarly for labels only and balls only */
           setdrawstuff(Lsetdrawtest); /*MAGEMENU.c*/ /*calls adddrawstuff();*/ 
           /*with just lines or labels or balls specified, then also set:*/
           Lpickerase = 1; /*option to punch in new group only*/
        }

        if(Lappend==0 && magetesttime > 0.001) 
        {/*last step in bringing in a new kinemage*/
           /*timer min * 60sec/min * 60ticks/sec */
           magetimeticklimit = (long)(magetesttime*60*60) + theclocktickcount();
           /*this can be recomputed at end of user name Dialog*/
           Lmagetimerrunning = 1; /*this can be stopped by user name Dialog*/
           Lmagetimer = 1; /*timed kinemage*/
           Ltestmode = 1;  /*disables certain IO operations*/
           Lpickerase = 1; /*button controls erase in picked new group*/
           Ltestcontinue = 1; /*continue test until timed end or user end*/
        }
        else if(answerlistptr!=NULL && resultlistptr!=NULL)
        {/*allow scoring of result list points crt reference list points*/
           Lrescoring = 1;
        }
        else if(answerlistptr!=NULL)
        {
           Lpracticemode = 1; /*enables menu Item KINEMAGE/scoreNstay*/
           Ltestmode = 0;
           Lpickerase = 1; /*button controls erase in picked new group*/
        }
        Lemphasison = 0; /*020112xrc @beginemphasis,@endemphasis I/O safety*/
        Lemphasis = 0; /*020112xrc menu safety*/

        if(LGUI) resetmenuchecks(); /*MAGEMENU checks/unchecks by logicals*/
        
        if(LGUI) AdjustMenus();     /*MAGEMENU enables/disables*/

        if(Lnewpalette)
        { /*010604*/   
           resetmagepalette(); /*----DLOG*/
           Lnewpalette = 0;
        }
        
         Lprocessing = 0; /*finished bringing in this kinemage */
         Lmenu = 1; /*any changes will now be from menu choices*/

        fpin = fp; /*always reset from Lappend case to be sure */    
        
        if(maxviews > nviewsthiskinemage)
        {/*set global in the case when append more views*/
           nviewsthiskinemage = maxviews;
        } 
        if(maxaspects > naspectsthiskinemage)
        {/*set global in the case when append more aspects*/
          naspectsthiskinemage = maxaspects;
        }
        if(naspectsthiskinemage > 0 && currentaspect==0)
        {
           currentaspect = 1; /*start with first aspect*/
        }

        if(LGUI) setcurrentaspect(currentaspect);

        if(!LGUI)
        {/*postscript output needs finite font sizes 030823*/
           if(labelsfontsize <= 0) {labelsfontsize = 14;}
           if(wordsfontsize <= 0)  {wordsfontsize = 14;}
        }
#ifdef UNIX_X11
        /*later can append more text beyond the 32k nominal Mage limit*/
        if(maxtextchar < 65534L)  /*UNIX_X11 030910*/
        { 
           Limittext = 0; 
           maxtextcharcurrent = 0;
        } 
#endif

if(Ltest)
{
sprintf(alertstr,"about to return from GetStuffFromFile()\n");
dosinglealert(3);
}
}
/*___GetStuffFromFile()_____________________________________________________*/

/****squarearoundkinemage()**************************************************/
void squarearoundkinemage()
{

#define Nbox2D 13
#define Nbox3D 18
float xytype[Nbox2D][3] =
{-180.0,-180.0,(float)(VECTOR|MOVETO_P),
+180.0,-180.0,(float)(VECTOR),
+180.0,+180.0,(float)(VECTOR),
-180.0,+180.0,(float)(VECTOR),
-180.0,-180.0,(float)(VECTOR),
-180.0,   0.0,(float)(VECTOR|MOVETO_P),
-175.0,   0.0,(float)(VECTOR),
+180.0,   0.0,(float)(VECTOR|MOVETO_P),
+175.0,   0.0,(float)(VECTOR),
0.0,-180.0,(float)(VECTOR|MOVETO_P),
0.0,-175.0,(float)(VECTOR),
0.0,+180.0,(float)(VECTOR|MOVETO_P),
0.0,+175.0,(float)(VECTOR),
};
float xyztype[Nbox3D][4] =
{   0.0,   0.0,   0.0,(float)(VECTOR|MOVETO_P),
360.0,   0.0,   0.0,(float)(VECTOR),
360.0, 360.0,   0.0,(float)(VECTOR),
0.0, 360.0,   0.0,(float)(VECTOR),
0.0,   0.0,   0.0,(float)(VECTOR),
0.0,   0.0, 360.0,(float)(VECTOR|MOVETO_P),
360.0,   0.0, 360.0,(float)(VECTOR),
360.0, 360.0, 360.0,(float)(VECTOR),
0.0, 360.0, 360.0,(float)(VECTOR),
0.0,   0.0, 360.0,(float)(VECTOR),
0.0,   0.0,   0.0,(float)(VECTOR|MOVETO_P),
0.0,   0.0, 360.0,(float)(VECTOR),
360.0,   0.0,   0.0,(float)(VECTOR|MOVETO_P),
360.0,   0.0, 360.0,(float)(VECTOR),
360.0, 360.0,   0.0,(float)(VECTOR|MOVETO_P),
360.0, 360.0, 360.0,(float)(VECTOR),
0.0, 360.0,   0.0,(float)(VECTOR|MOVETO_P),
0.0, 360.0, 360.0,(float)(VECTOR),
};
int i;

if(LbondrotZplot) {L0to360 = 1;} /*030819 3D RNA angles plotted 0--360*/ 
else {L0to360 = 0;} /*bondrotangles NOT shown 0 - 360 , special for UNIX */
if(!Ldrawstuff)
{ /*tries to establish drawline storage*/ /*MAGEINPT.c*/
adddrawstuff(); 
}
if(Ldrawstuff)  /* returns as TRUE if can set up drawline */
{/*Ldrawstuff == 1 generic control for this whole set*/
			   /*draw empowered*/
   Lnewdrawline = 0;  /*start counting here, not by call to dodrawline*/
   numberline = 0;    /*the counter*/
   Lnewlines  = 1;    /*empowers lines */
   Lnewlabels = 1;    /*empowers labels*/
   Lnewballs  = 1;    /*empowers balls*/
   Ldragline  = 1;    /*empowers drag*/
   Lconstructline = 1;/*empowers construction*/
   Lnewlineson   = 0; /*initially off, turn on by control panel button*/
   Lnewlabelson  = 0; /*initially off, turn on by control panel button*/
   Lnewballson  = 0; /*initially off, turn on by control panel button*/
   Lconstruct4on = 0; /*initially off, turn on by control panel button*/
   Lconstruct5on = 0; /*initially off, turn on by control panel button 971122*/
   Lconstruct6on = 0; /*initially off, turn on by control panel button 140912*/
   Ldraglineon   = 0; /*initially off, turn on by control panel button*/
	
   shortenline = 0.0; 
   Ldrawunpickable = 0;
   Lmonitor = 0; /*last line length*/
   if(Lmonitor) Lmonitoron = 1; /*last line length*/
   else         Lmonitoron = 0; /*last line length*/
   Lconstructprojection = 0;
   Lconstructdot = 0;
   turnoffallbutoneboxLogical((long)drawlinebboxptr); /*MAGEBBOX*/

drawnewlistptr->on = 1;
      sprintf(drawgrupptr->name," angle plot");
                    /*note leading space in group name*/
     if(!LbondrotZplot) /*030819*/
     {
      drawnewlistptr->STATUS = drawnewlistptr->STATUS | SCREENFLAG | STATICFLAG;
     }
      drawnewlistptr->STATUS = drawnewlistptr->STATUS | NOHILITEFLAG; 
      /*no highlight on ball showing plot pt */
      /*first point is designated for the plotted hplot,vplot, (& zplot) point*/
      /*this will be updated as that point is changed*/
      
         allocpointstructure(drawnewlistptr);
         thispointptr->fx = bondrotptr[LbondrotHplot]->angle;
         thispointptr->fy = bondrotptr[LbondrotVplot]->angle;
         if(LbondrotZplot > 0)
         { thispointptr->fz = bondrotptr[LbondrotZplot]->angle;}
         else
         { thispointptr->fz = -10.0;} /*so will be behind plotted screen stuff*/
         thispointptr->type = thispointptr->type | BALL;
         thispointptr->colorwidth = numberofcolor("brown"); /*MAGECOLR.c*/
         /*simulate exclaimation point that forces point color to be dominant*/
         thispointptr->STYLE = thispointptr->STYLE | FORCEPTCOLOR;
 /*thispointptr->radius = (float)0.2;*/ /*disk, plotted green point on top*/
         thispointptr->radius = (25*(float)0.2)/Scale; /*020311*/
           /*fudge disk size to go with initial coordinate size,*/
           /* The plotted green points will go on top of this disk marker*/
         sprintf(word,"PLOTTED");
         storeptIDstring(word,thispointptr);
         numberline++;
         maxnxyz++;

     if(LbondrotZplot) /*030819*/
     {
      for(i=0; i< Nbox3D; i++)
      {
         allocpointstructure(drawnewlistptr);
         thispointptr->fx = xyztype[i][0];
         thispointptr->fy = xyztype[i][1];
         thispointptr->fz = xyztype[i][2];
         thispointptr->type = thispointptr->type | (int)xyztype[i][3];
         thispointptr->colorwidth = numberofcolor("blue"); /*MAGECOLR.c*/
       sprintf(word,"%.2f,%.2f,%.2f",xyztype[i][0],xyztype[i][1],xyztype[i][2]);
         storeptIDstring(word,thispointptr);
         numberline++;
         maxnxyz++;
      }
     }
     else
     {
      for(i=0; i< Nbox2D; i++)
      {
         allocpointstructure(drawnewlistptr);
         thispointptr->fx = xytype[i][0];  /*971011*/
         thispointptr->fy = xytype[i][1];
         thispointptr->fz = 0.0;
         thispointptr->type = thispointptr->type | (int)xytype[i][2];
         thispointptr->colorwidth = numberofcolor("blue"); /*MAGECOLR.c*/
         sprintf(word,"%.2f,%.2f",xytype[i][0],xytype[i][1]);
         storeptIDstring(word,thispointptr);
         numberline++;
         maxnxyz++;
      }
     }
     
   }/*Ldrawstuff == 1 generic control for this whole set*/
   
}
/*___squarearoundkinemage()_______________________________________*/

/****scalekinemage()*************************************/
void    scalekinemage()
{
#define FUDGED 1.237
  if(Scale == 0.0) Scale = 1.0;  /* Scale has to be set somewhere */
  /*  data can come in prescaled, e.g. to a coord system */
  if(DOscale)  
  {/*DOscale*/
    fdiff = fxmax-fxmin;  /* find maximum difference across coordinates */
    if(fdiff < (fymax-fymin) ) fdiff = fymax-fymin;
    if(fdiff < (fzmax-fzmin) ) fdiff = fzmax-fzmin;        
    fdiff = (float)(fdiff*FUDGED);  
    /* fudge so corners almost fit along coord direction  1.5? */

    if(fdiff < 2.0) fdiff = 2.0; /*971011*/
        
    Scale = oldmaxwide/(fdiff); /* define Scale ,oldmaxwide==GWIDTH*/

    fxcenter = (fxmax + fxmin)/2;
    fycenter = (fymax + fymin)/2;
    fzcenter = (fzmax + fzmin)/2;
    /*move setupbondrotplot stuff   030821*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*loop-over-groups*/
      thissgrpptr = thisgrupptr->firstsgrpptr;
      while(thissgrpptr != NULL)
      {/*loop-over-sets*/

        thislistptr = thissgrpptr->firstlistptr;
        while(thislistptr != NULL)
        {/*loop-over-lists*/  
          thispointptr = thislistptr->firstpointptr;
          while(thispointptr != NULL)
          {/*loop-over-points*/
            /*load scaled, centered integers */
            if(  thispointptr->fx >= 0.0)
                 thispointptr->ix =(int)(Scale*(thispointptr->fx-fxcenter) +.5);
            else thispointptr->ix =(int)(Scale*(thispointptr->fx-fxcenter) -.5);

            if(  thispointptr->fy >= 0.0)
                 thispointptr->iy =(int)(Scale*(thispointptr->fy-fycenter) +.5);
            else thispointptr->iy =(int)(Scale*(thispointptr->fy-fycenter) -.5);

            if(  thispointptr->fz >= 0.0)
                 thispointptr->iz =(int)(Scale*(thispointptr->fz-fzcenter) +.5);
            else thispointptr->iz =(int)(Scale*(thispointptr->fz-fzcenter) -.5);

            if(thispointptr == lastscaledpointptr) thispointptr = NULL;
            else if(thispointptr == thislistptr->lastpointptr)
                    thispointptr=NULL;
            else thispointptr = thispointptr->nextptr;
          }/*loop-over-points*/
          if(thispointptr == lastscaledpointptr) thislistptr = NULL;
          else if(thislistptr == thissgrpptr->lastlistptr)
                  thislistptr=NULL;
          else thislistptr = thislistptr->nextptr;
        }/*loop-over-lists*/  
        if(thispointptr == lastscaledpointptr) thissgrpptr = NULL;
        else if(thissgrpptr == thisgrupptr->lastsgrpptr) 
                thissgrpptr = NULL;
        else thissgrpptr = thissgrpptr->nextptr;
      }/*loop-over-sets*/
      if(thispointptr == lastscaledpointptr) thisgrupptr = NULL;
      else thisgrupptr = thisgrupptr->nextptr;
    }/*loop-over-groups*/

    fxcenternew = fxcenter; /*...new factors used for resizing image */
    fycenternew = fycenter;
    fzcenternew = fzcenter;
    scalenew = Scale;
    if(!Lcenter[1])
    {
        fxcenterold[1] = fxcenter;
        fycenterold[1] = fycenter;
        fzcenterold[1] = fzcenter;
    }
    
  }/*DOscale*/
  else
  {
      fdiff = oldmaxwide/(Scale); /* define fdiff */ /*931018*/
  }

  if(Lcenterpoint) /*some centering info was in input file*/
  {
     if(Lcenter[1]) /*centering for first view*/
     {
         fxcenternew = fxcenterold[1]; /*from input file*/
         fycenternew = fycenterold[1];
         fzcenternew = fzcenterold[1];
     }
  }
  if(Lzoom)
  {
     if(Lzoomer[1]!=0) /*zoom set for first view in input*/ /*980929*/
     {
         scalenew = Scale*zoom;
     }
  }
}
/*___scalekinemage()_____________________________________*/

/****reloadkinemage()** L7dimensions LNdimensions 7D nD ...********************/
void reloadkinemage() /*060121 reloads fx,fy,fz with ANG[NX],ANG[NY],ANG[NZ] */
{  /*reloads fx,fy,fz from coordNptr[NX,NY,NZ] 060622,061028*/
   /*list, subgroup, group can be declared dimension= n, sets DIMENSIONKKIND*/
   /*list can inherit from subgroup, subgroup can inherit from group*/
   /*thus only need to check at list */
   /*also sets general kinemage attribute NDIM = n, potential conflict 121224*/ 

    Lreloadkinemage = 0;
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*loop-over-groups*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*loop-over-sets*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*loop-over-lists*/  
              if(thislistptr->NDIM > 3) /*060622*/
              {/*list is multi-dimensional*/
                thispointptr = thislistptr->firstpointptr;
                while(thispointptr != NULL)
                {/*loop-over-points*/
                   if(thispointptr->coordNptr != NULL)
                   {
                     if(   dimensionptr[NX].Lwrap       
                        && (thispointptr->coordNptr[NX]) < 0 ) /*061121*/
                     {
                        /*thispointptr->fx = dimensionptr[NX].scale*/ 
                        thispointptr->fx = (360/( dimensionptr[NX].max 
                                                 -dimensionptr[NX].min))
                                           * (  (dimensionptr[NX].max) 
                                              + (thispointptr->coordNptr[NX]));
                     }
                     else 
                     {
                        /*thispointptr->fx = dimensionptr[NX].scale */
                        thispointptr->fx = (360/( dimensionptr[NX].max 
                                                 -dimensionptr[NX].min))
                                           * (thispointptr->coordNptr[NX]);
                     }
                     if(   dimensionptr[NY].Lwrap 
                        && (thispointptr->coordNptr[NY]) < 0 )
                     {
                        /*thispointptr->fy = dimensionptr[NY].scale */
                        thispointptr->fy = (360/( dimensionptr[NY].max 
                                                 -dimensionptr[NY].min))
                                           * (  (dimensionptr[NY].max) 
                                              + (thispointptr->coordNptr[NY]));
                     }
                     else 
                     {
                        /*thispointptr->fy = dimensionptr[NY].scale */
                        thispointptr->fy = (360/( dimensionptr[NY].max 
                                                 -dimensionptr[NY].min))
                                           * (thispointptr->coordNptr[NY]);
                     }
                     if(   dimensionptr[NZ].Lwrap 
                        && (thispointptr->coordNptr[NZ]) < 0 )
                     {
                        /*thispointptr->fz = dimensionptr[NZ].scale*/ 
                        thispointptr->fz = (360/( dimensionptr[NZ].max 
                                                 -dimensionptr[NZ].min))
                                           * (  (dimensionptr[NZ].max) 
                                              + (thispointptr->coordNptr[NZ]));
                     }
                     else 
                     {
                        /*thispointptr->fz = dimensionptr[NZ].scale */
                        thispointptr->fz = (360/( dimensionptr[NZ].max 
                                                 -dimensionptr[NZ].min))
                                           * (thispointptr->coordNptr[NZ]);
                     }
                   }
                   if(thispointptr == thislistptr->lastpointptr)
                       thispointptr=NULL;
                   else thispointptr = thispointptr->nextptr;
                }/*loop-over-points*/
              }/*list is multi-dimensional*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*loop-over-lists*/  
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
        }/*loop-over-sets*/
      thisgrupptr = thisgrupptr->nextptr;
    }/*loop-over-groups*/
    Lreplacemarkers = 1;
    Lrescalekinemage = 1;
    Lredrawvector = 1;
}
/*___reloadkinemage()________________________________________________________*/

/****replacemarkers()*********************************************************/
void replacemarkers(void)   /*061116*/
{/*marker placing, called when markers to be redrawn at putative new point*/
 /*Not adequate to show markers reliably in front of screen fixed NDIM balls*/
 /*so calling rescalekinemage() is a dirty way to compensate for my */
 /*lack of understanding how markers can be resized and shown then. dcr 121211*/
  int   Lneedrescalekinemage = 0; /*121211*/
  int   marker=0;
  float fudge=0;
  float tmps=0;
  struct pointstruct *thispickedpointptr = NULL;

   Lreplacemarkers = 0; /*unset flag*/
  
   if(Lmarkers && pickedpointptr != NULL && pickedlistptr != NULL)
   {/*there are markers and there is a picked point*/

          for(marker=2; marker>=1; marker--)
          {/*another level: load both markers crt last two picked points*/
             
            if(marker == 2) 
            {
              if(pickedlistoldptr == NULL || pickedpointoldptr == NULL) 
              {
                   continue; /*on to next marker value*/
              }
              else /* else not strictly needed since continue hops up to loop*/
              {
                thismarkerlistptr = marker2listptr;
                thismarkeroriglistptr = marker2origlistptr;
                /*set marker list to static of picklist*/
                marker2listptr->type = pickedlistoldptr->type;
                marker2listptr->STATUS = pickedlistoldptr->STATUS; /*020503*/
                mark2deep = mark1deep; /*presume old point had marker1 on it*/
                thispickedpointptr = pickedpointoldptr;
              }
            }
            else 
            {
                thismarkerlistptr = marker1listptr; 
                thismarkeroriglistptr = marker1origlistptr; 
                /*set marker list to static of picklist*/
                marker1listptr->type = pickedlistptr->type;
                marker1listptr->STATUS = pickedlistptr->STATUS; /*020503*/
                mark1deep = markdeep;
                thispickedpointptr = pickedpointptr;
            }
            thispointptr = thismarkerlistptr->firstpointptr;
            origpointptr = thismarkeroriglistptr->firstpointptr;
            while(thispointptr != NULL)
            {/*loop places all points of this marker crt this picked point*/
              thispointptr->type = origpointptr->type;
              thispointptr->colorwidth = origpointptr->colorwidth;
              if(Lscalemarkers) tmps = 2; /*in replacemarkers()*/
              else tmps = 1;

              if((pickedlistptr->STATUS & STATICFLAG)==0) /*non-static*/
              {/*NOT "STATIC", NOT "STATIC & SCREEN"*/
                thispointptr->fx 
                   = (tmps*origpointptr->fx)/zoom + thispickedpointptr->fx;
                thispointptr->fy 
                   = (tmps*origpointptr->fy)/zoom + thispickedpointptr->fy;
                thispointptr->fz 
                   = (tmps*origpointptr->fz)/zoom + thispickedpointptr->fz;
                
                thispointptr->ix=(int)(scalenew*(thispointptr->fx-fxcenternew));
                thispointptr->iy=(int)(scalenew*(thispointptr->fy-fycenternew));
                thispointptr->iz=(int)(scalenew*(thispointptr->fz-fzcenternew));
              }
              else  /*marker on a static list*/
              {/*"STATIC" either "STATIC alone" OR "STATIC & SCREEN"*/
                  thispointptr->fx = origpointptr->fx + thispickedpointptr->fx;
                  thispointptr->fy = origpointptr->fy + thispickedpointptr->fy;
                  thispointptr->fz = origpointptr->fz + thispickedpointptr->fz;

                if((pickedlistptr->STATUS & SCREENFLAG)==0) /*non-screen*/
                {/*STATIC alone is Scaled and centered*/
                  thispointptr->ix = (int)(Scale*(thispointptr->fx - fxcenter));
                  thispointptr->iy = (int)(Scale*(thispointptr->fy - fycenter));
                  thispointptr->iz = (int)(Scale*(thispointptr->fz - fzcenter));
                }
                else
                {/*STATIC & SCREEN is NOT Scaled and NOT centered*/
                  fudge = (float)oldmaxwide/(float)400;
/*121210 fudge fudge to get big enough markers on NDIM screen param points*/
/*121211 fudge does not seem to have any noticeable effect on marker size!!*/
/*121210 tmps only does the second anyway...*/
                  /*if(NDIM) general kinemage attribute*/ /*121211*/
                  /*if(pickedlistptr->KKIND & DIMENSIONKKIND) picked pt 121224*/
                  if(pickedlistptr->NDIM > 3) /*specific picked pt 121224*/
                  {
                    thispointptr->ix = (int)((thispointptr->fx)*fudge);
                    thispointptr->iy = (int)((thispointptr->fy)*fudge);
                    thispointptr->iz = (int)((thispointptr->fz)*fudge);
                    /*fudge has NO effect on screenfixed markers 121211*/
                    Lneedrescalekinemage = 1; /*121211*/
                  }  /*NDIM 121210,  ->NDIM 121224, or DIMENSIONKKIND 121224*/
                  else
                  {  /*usual 3D projection*/
                  thispointptr->ix = (int)((thispointptr->fx)*fudge);
                  thispointptr->iy = (int)((thispointptr->fy)*fudge);
                  thispointptr->iz = (int)((thispointptr->fz)*fudge);
                  }
                }
              }/*"STATIC" either "STATIC alone" OR "STATIC & SCREEN"*/
              
              if(   thispointptr == thismarkerlistptr->lastpointptr
                 || origpointptr == thismarkeroriglistptr->lastpointptr )
              {    
                 thispointptr = NULL;
              }
              else
              {
                  thispointptr = thispointptr->nextptr;
                  origpointptr = origpointptr->nextptr;
              }
            }/*loop places all points of this marker crt this picked point*/

            if(Lpickmarkcriteria >0) /*see pickmarkerDialog()*/
            {/*NOTE (thismarkerlistptr->lastpointptr)->previousptr should*/
             /*be a zero point placed exactly on the picked point this keeps*/
             /*extended lines honest and provides a ref pt for scaling markers*/
             /*This makes choice of lines extending out from 3D marker...*/
               thispointptr = ((thismarkerlistptr->lastpointptr)->previousptr)->previousptr;
               if((Lpickmarkcriteria &  1) ==   1) thispointptr->fx = fxmin;
               if((Lpickmarkcriteria &  2) ==   2) thispointptr->fy = fymin;
               if((Lpickmarkcriteria &  4) ==   4) thispointptr->fz = fzmin;
               if((Lpickmarkcriteria &  8) ==   8) thispointptr->fx = fxmax;
               if((Lpickmarkcriteria & 16) ==  16) thispointptr->fy = fymax;
               if((Lpickmarkcriteria & 32) ==  32) thispointptr->fz = fzmax;
               calcintegerpoints(thispointptr); /*MAGELIST*/
               thispointptr = thismarkerlistptr->lastpointptr;
               if((Lpickmarkcriteria &  256) ==   256) thispointptr->fx = fxmin;
               if((Lpickmarkcriteria &  512) ==   512) thispointptr->fy = fymin;
               if((Lpickmarkcriteria & 1024) ==  1024) thispointptr->fz = fzmin;
               if((Lpickmarkcriteria & 2048) ==  2048) thispointptr->fx = fxmax;
               if((Lpickmarkcriteria & 4096) ==  4096) thispointptr->fy = fymax;
               if((Lpickmarkcriteria & 8192) ==  8192) thispointptr->fz = fzmax;
               calcintegerpoints(thispointptr); /*MAGELIST*/
               if((Lpickmarkcriteria & 64) ==  64) thismarkerlistptr->width = 1;
               else thismarkerlistptr->width = 0; 
            }
            else
            {
               thismarkerlistptr->width = 0; 
            }
          }/*another level: load both markers crt last two picked points*/
   }/*there is a picked point*/
   if(Lneedrescalekinemage) rescalekinemage(); /*121211*/
   Lneedrescalekinemage = 0; /*121211*/
}/*marker placing*/
/*___replacemarkers()________________________________________________________*/

/****rescalekinemage()*******************************************************/
void    rescalekinemage()
{/*called when image or part of image, even just markers, is to be rescaled*/
  struct pointstruct* origpointptr;
  float tmps=0; /*000804*/
  float dozoom,doscale,fxctr,fyctr,fzctr;
  int Lfirstpoint=0; /*130115 centered group firstpoint centered*/
  int Lcenteredgroup=0; /*130115 centered group*/
  float xgctr,ygctr,zgctr; /*130115 first point of centered group*/

  Linvert360yz = 0; /*121212*/

    /*scan over all points, scale and center */
    /*historically, just: for(n=1;n<=maxnxyz;n++)  but now 940903+ with */
    /*static lists, need to scan over full hierarchy.  MAGE_3.1.2 */
    Lrescalekinemage = 0; /*030329*/
    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*loop-over-groups*/
        if(Lcentered && (thisgrupptr->KKIND & CENTEREDKKIND)) /*130115*/
        {/*group to be kept centered based on first point*/
           Lfirstpoint = 1;
           Lcenteredgroup = 1; /*Beware: expected to be nonstatic...130115*/ 
        }
        else
        {
           Lfirstpoint = 0;
           Lcenteredgroup = 0; 
        }
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*loop-over-sets*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*loop-over-lists*/  
                thispointptr = thislistptr->firstpointptr;
                while(thispointptr != NULL)
                {/*loop-over-points*/
                    /*screen more rigorous than plain static*/
                    if     (  ((thislistptr->STATUS)  & SCREENFLAG)==SCREENFLAG
                            ||((thispointptr->STATUS) & SCREENFLAG)==SCREENFLAG)
                    {/*screen, fits into window width*/
                        calcintegerscreenpoints(thispointptr); /*MAGELIST*/
                    }/*screen*/
                    else if(  ((thislistptr->STATUS)  & STATICFLAG)==STATICFLAG
                            ||((thispointptr->STATUS) & STATICFLAG)==STATICFLAG)
                    {/*static, Scale has window width builtin*/
                        calcintegerfixedpoints(thispointptr); /*MAGELIST*/
                    }/*static*/
                    else if(Lcenteredgroup)
                    {/*nonstatic and kept centered on first point*/ /*130115*/
                        if(Lfirstpoint)
                        {
                           /*xgctr = fxcenternew - thispointptr->fx;*/
                           /*ygctr = fycenternew - thispointptr->fy;*/
                           /*zgctr = fzcenternew - thispointptr->fz;*/
                           xgctr = thispointptr->fx;
                           ygctr = thispointptr->fy;
                           zgctr = thispointptr->fz;
                           Lfirstpoint = 0;
                        }
                        calcintegerpointscentered(thispointptr, xgctr, ygctr, zgctr); /*MAGELIST*/
                    }/*nonstatic and kept centered*/
                    else
                    {/*nonstatic*/ /*971011*/
                        calcintegerpoints(thispointptr); /*MAGELIST*/
                    }/*nonstatic*/
                    if(thispointptr == thislistptr->lastpointptr)
                        thispointptr=NULL;
                    else thispointptr = thispointptr->nextptr;
                }/*loop-over-points*/
                
                if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
                else thislistptr = thislistptr->nextptr;
            }/*loop-over-lists*/  
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
            else thissgrpptr = thissgrpptr->nextptr;
        }/*loop-over-sets*/
        thisgrupptr = thisgrupptr->nextptr;
    }/*loop-over-groups*/
    if(Lmarkers)
    {
        if(Lscalemarkers) tmps = 2; /* in rescalekinemage() */
        else  tmps = 1;

        /* put gnomon on actual center of view, recreating coords from*/
        /* the original ones defined by MAGE */
        thispointptr = markergnom1listptr->firstpointptr;
        origpointptr = markergnom1origlistptr->firstpointptr;
        while(thispointptr != NULL)
        {/*rescale gnomon 1*/
           thispointptr->type = origpointptr->type;
           thispointptr->fx = origpointptr->fx + fxcenternew;
           thispointptr->fy = origpointptr->fy + fycenternew;
           thispointptr->fz = origpointptr->fz + fzcenternew;
           thispointptr->ix=(int)(tmps*scalenew*(thispointptr->fx-fxcenternew));
           thispointptr->iy=(int)(tmps*scalenew*(thispointptr->fy-fycenternew));
           thispointptr->iz=(int)(tmps*scalenew*(thispointptr->fz-fzcenternew));

           if(thispointptr==markergnom1listptr->lastpointptr) thispointptr=NULL;
           else
           {
               thispointptr = thispointptr->nextptr;
               origpointptr = origpointptr->nextptr;
           }
        }/*rescale gnomon 1*/

        thispointptr = markergnom2listptr->firstpointptr;
        origpointptr = markergnom2origlistptr->firstpointptr;
        while(thispointptr != NULL)
        {/*rescale gnomon 2*/
           thispointptr->type = origpointptr->type;
           thispointptr->fx = origpointptr->fx + fxcenternew;
           thispointptr->fy = origpointptr->fy + fycenternew;
           thispointptr->fz = origpointptr->fz + fzcenternew;
           thispointptr->ix=(int)(tmps*scalenew*(thispointptr->fx-fxcenternew));
           thispointptr->iy=(int)(tmps*scalenew*(thispointptr->fy-fycenternew));
           thispointptr->iz=(int)(tmps*scalenew*(thispointptr->fz-fzcenternew));
           if(thispointptr==markergnom2listptr->lastpointptr) thispointptr=NULL;
           else
           {
               thispointptr = thispointptr->nextptr;
               origpointptr = origpointptr->nextptr;
           }
        }/*rescale gnomon 2*/
        /*---marker 1 --------*/        
        Linvert360yz = 0;
        thislistptr = marker1listptr;
        if(  ((thislistptr->STATUS)  & SCREENFLAG)==SCREENFLAG)
        {/*STATIC and SCREEN is not Scaled and not centered*/
           dozoom = 1;
           doscale = (float)oldmaxwide/(float)400;/*adjusts screen position*/
           fxctr = 0;
           fyctr = 0;
           fzctr = 0;
                  /*if(NDIM) general kinemage attribute*/ /*121211*/
                  /*if(pickedlistptr->KKIND & DIMENSIONKKIND) picked pt 121224*/
           if(pickedlistptr->NDIM > 3) /*specific picked pt 121224*/
           {
              dozoom = 0.1; /*121211*/
              if(Linvert360yzon) { Linvert360yz = 1;} /*121212*/
           }
        }
        else if(  ((thislistptr->STATUS)  & STATICFLAG)==STATICFLAG)
        {/*STATIC alone is Scaled and centered*/
           dozoom = 1;
           doscale = Scale;
           fxctr = fxcenternew;
           fyctr = fycenternew;
           fzctr = fzcenternew;
        }
        else
        {/*regular is zoomed as well as newscaled and centered*/
           dozoom = zoom;
           doscale = scalenew;
           fxctr = fxcenternew;
           fyctr = fycenternew;
           fzctr = fzcenternew;
        }
        thispointptr = marker1listptr->firstpointptr;
        origpointptr = marker1origlistptr->firstpointptr;
        while(thispointptr != NULL)
        {/*loop for marker 1 points*/
            thispointptr->type = origpointptr->type;
            thispointptr->fx = (tmps*origpointptr->fx)/dozoom 
                           + ((marker1listptr->lastpointptr)->previousptr)->fx;
            thispointptr->fy = (tmps*origpointptr->fy)/dozoom
                           + ((marker1listptr->lastpointptr)->previousptr)->fy;
            thispointptr->fz = (tmps*origpointptr->fz)/dozoom
                           + ((marker1listptr->lastpointptr)->previousptr)->fz;
            thispointptr->ix = (int)(doscale*(thispointptr->fx - fxctr));
         if(Linvert360yz)
         {
            thispointptr->iy = (int)(doscale*((360-thispointptr->fy) - fyctr));
            thispointptr->iz = (int)(doscale*((360-thispointptr->fz) - fzctr));
         }
         else
         {
            thispointptr->iy = (int)(doscale*(thispointptr->fy - fyctr));
            thispointptr->iz = (int)(doscale*(thispointptr->fz - fzctr));
         }
            if(thispointptr == marker1listptr->lastpointptr)
                thispointptr = NULL;
            else
            {
                thispointptr = thispointptr->nextptr;
                origpointptr = origpointptr->nextptr;
            }
        }/*loop for marker 1 points*/

        /*---marker 2 --------*/       
        Linvert360yz = 0; /*121212*/ 
        thislistptr = marker2listptr;
        if(  ((thislistptr->STATUS)  & SCREENFLAG)==SCREENFLAG)
        {/*STATIC and SCREEN is not Scaled and not centered*/
           dozoom = 1; 
           doscale = (float)oldmaxwide/(float)400; /*adjusts screen position*/
           fxctr = 0;
           fyctr = 0;
           fzctr = 0;
                  /*if(NDIM) general kinemage attribute*/ /*121211*/
                  /*if(pickedlistptr->KKIND & DIMENSIONKKIND) picked pt 121224*/
           if(pickedlistptr->NDIM > 3) /*specific picked pt 121224*/
           {
              dozoom = 0.1; /*121211*/
              if(Linvert360yzon) { Linvert360yz = 1;} /*121212*/
           }
        }
        else if(  ((thislistptr->STATUS)  & STATICFLAG)==STATICFLAG)
        {/*STATIC alone is Scaled and centered*/
           dozoom = 1;
           doscale = Scale;
           fxctr = fxcenternew;
           fyctr = fycenternew;
           fzctr = fzcenternew;
        }
        else
        {/*regular is zoomed as well as newscaled and centered*/
           dozoom = zoom;
           doscale = scalenew;
           fxctr = fxcenternew;
           fyctr = fycenternew;
           fzctr = fzcenternew;
        }
        thispointptr = marker2listptr->firstpointptr;
        origpointptr = marker2origlistptr->firstpointptr;
        while(thispointptr != NULL)
        {/*loop for marker 2 points*/
            thispointptr->type = origpointptr->type;
            thispointptr->fx = (tmps*origpointptr->fx)/dozoom 
                           + ((marker2listptr->lastpointptr)->previousptr)->fx;
            thispointptr->fy = (tmps*origpointptr->fy)/dozoom
                           + ((marker2listptr->lastpointptr)->previousptr)->fy;
            thispointptr->fz = (tmps*origpointptr->fz)/dozoom
                           + ((marker2listptr->lastpointptr)->previousptr)->fz;
            thispointptr->ix = (int)(doscale*(thispointptr->fx - fxctr));
          if(Linvert360yz)
          {
            thispointptr->iy = (int)(doscale*((360-thispointptr->fy) - fyctr));
            thispointptr->iz = (int)(doscale*((360-thispointptr->fz) - fzctr));
          }
          else
          {
            thispointptr->iy = (int)(doscale*(thispointptr->fy - fyctr));
            thispointptr->iz = (int)(doscale*(thispointptr->fz - fzctr));
          }
            if(thispointptr == marker2listptr->lastpointptr)
                thispointptr = NULL;
            else
            {
                thispointptr = thispointptr->nextptr;
                origpointptr = origpointptr->nextptr;
            }
        }/*loop for marker 2 points*/
        Linvert360yz = 0;/*121212*/
        /*----  -----*/
    }
    if(Lpickcenteron) /*030121*/
    {
        Lpickcenteron = 0; /*so any induced pickcenter is single shot*/
        if(!LmouseRdown)
        {/*avoid SetsinGrafWindow when recentering with mouseRdown 030713*/
           LneedSetsinGrafWindow = 1;
        }
    }
    if(Lneedannealmobile && !Legamickchoice) /*egamick 030217*/
    {
       annealmobilelist(); /*MAGEUTIL.c*/  /*egamick 030217*/
       getmobilelist();    /*MAGEUTIL.c*/ /*egamick 030217*/
    }
}
/*___rescalekinemage()______________________________________________________*/

/****addmarkers()************************************************************/
void    addmarkers()
{
   int  p=7;  /* octahedron points +- p*/

      /* set up the changing-position marker structures */
      
      Nthisgroupkind = 0;
      if( !allocgrupstructure() )
      {
          sprintf(alertstr,"failed to alloc group for markers");
          goto allocerr;
      }
      else
      {        
          thisgrupptr->on = 0; /*group display off*/
          thisgrupptr->type = 0; /*nothing special*/
          thisgrupptr->firstsgrpptr = NULL;  
          thisgrupptr->lastsgrpptr = NULL;
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT; 
              /* markers group is by definition dominant, no buttons below */
          thisgrupptr->STATUS = thisgrupptr->STATUS | NOBUTTON; /*020718*/
          /*MAGEBBOX will create special button so no button STATUS here*/
          thisgrupptr->master = 0; /*no master*/
          sprintf(thisgrupptr->name,"markers");
          markergrupptr = thisgrupptr;
      }

      if( !allocsgrpstructure() )
      {
          sprintf(alertstr,"failed to alloc subgroup for markers");
          goto allocerr;
      }
      else
      {        
          thissgrpptr->on = 1; /*subgroup display on*/
          thissgrpptr->firstlistptr = NULL;  
          thissgrpptr->lastlistptr = NULL;
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
          /* no button to be made for invented markers subgroup */
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->firstsgrpptr = thissgrpptr;  
          thisgrupptr->lastsgrpptr = thissgrpptr;
          sprintf(thissgrpptr->name,"markers");
          markersgrpptr = thissgrpptr;
      }

      /*need 4 visible, changing lists: 2 markers,1 center,1 labels*/
      if( !allocliststructure() )
      {
          sprintf(alertstr,"failed to alloc lists for markers");
          goto allocerr;
      }
      else
      {       
        thislistptr->on = 1; /*list display on*/
        thislistptr->firstpointptr = NULL;
        thislistptr->lastpointptr = NULL;
        thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
        thislistptr->sgrpptr = thissgrpptr;
        thissgrpptr->firstlistptr = thislistptr;
        marker1listptr = thislistptr;
        sprintf(thislistptr->name,"marker1");
        
        sprintf(word,"marker");
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  p,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p, -p, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            , -p,  p,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p, -p, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p, -p,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  p, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  p, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p, -p,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
      }
     
      if( !allocliststructure() )      
      {
          sprintf(alertstr,"failed to alloc lists for markers");
          goto allocerr;
      }
      else
      {       
       thislistptr->on = 1; /*list display on*/
        thislistptr->firstpointptr = NULL;
        thislistptr->lastpointptr = NULL;
        thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
        thislistptr->sgrpptr = thissgrpptr;
        marker2listptr = thislistptr;
        sprintf(thislistptr->name,"marker2");
        
        sprintf(word,"marker");
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
      } 
      
      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc lists for markers");
          goto allocerr;
      }
      else
      {       
        thislistptr->on = 0; /*list display off*/
        thislistptr->firstpointptr = NULL;  /*gnomon axes*/
        thislistptr->lastpointptr = NULL;
        thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
        thislistptr->sgrpptr = thissgrpptr;
        markergnom1listptr = thislistptr;
        sprintf(thislistptr->name,"gnomon");
        sprintf(word,"gnomon");
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            , -9,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  9,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            ,  0, -9,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  9,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            ,  0,  0, -9); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  0,  0); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  0,  9); /*MAGELIST*/
            storeptIDstring(word, thispointptr);            /*MAGELIST*/
       
        /*put gnomon on actual center of first view*/
        
        thispointptr = thislistptr->firstpointptr;
        while(thispointptr != NULL)
        {
           thispointptr->fx =  thispointptr->fx + fxcenternew;
           thispointptr->fy =  thispointptr->fy + fxcenternew;
           thispointptr->fz =  thispointptr->fz + fxcenternew; 
           calcintegerpoints(thispointptr); /*MAGELIST*/
                    
           if(thispointptr == thislistptr->lastpointptr) thispointptr = NULL;
           else thispointptr = thispointptr->nextptr;
        }
      }
      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc lists for markers");
          goto allocerr;
      }
      else
      {               
        thislistptr->on = 0; /*list display off*/
        thislistptr->firstpointptr = NULL;  /*gnomon labels*/
        thislistptr->lastpointptr = NULL;
        thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
        thislistptr->sgrpptr = thissgrpptr;
        markergnom2listptr = thislistptr;
        thissgrpptr->lastlistptr = thislistptr;
        sprintf(thislistptr->name,"gnomonXYZ");
        sprintf(word,"X");
        allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  9,  0,  0);
        storeptIDstring(word, thispointptr);            /*MAGELIST*/
        sprintf(word,"Y");
        allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  0,  9,  0);
        storeptIDstring(word, thispointptr);            /*MAGELIST*/
        sprintf(word,"Z");
        allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  0,  0,  9);
        storeptIDstring(word, thispointptr);            /*MAGELIST*/
      
        /*build pseudo-original float markers back from the integer form*/
        /* and put gnomon on actual center of first view*/
        thispointptr = thislistptr->firstpointptr;
        while(thispointptr != NULL)
        {
           thispointptr->fx =  thispointptr->fx + fxcenternew;
           thispointptr->fy =  thispointptr->fy + fxcenternew;
           thispointptr->fz =  thispointptr->fz + fxcenternew; 
           calcintegerpoints(thispointptr); /*MAGELIST*/
           
           if(thispointptr == thislistptr->lastpointptr) thispointptr = NULL;
           else thispointptr = thispointptr->nextptr;
        }
        Lmarkers = TRUE;
        if(Lsinglemarker)
        {/*040106 commandline singlemarker*/
           markergrupptr->on = 1;
           marker2listptr->on = 0; /*second marker list display off*/
        }
        maxnxyz = nxyz + NMARKERS;
      }

    /*marker original objects*/
      if( !allocsgrpstructure() )
      {
          sprintf(alertstr,"failed to alloc subgroup for orig markers");
          goto allocerr;
      }
      else
      {        
          thissgrpptr->on = 0; /*subgroup display OFF*/
          thissgrpptr->firstlistptr = NULL;  
          thissgrpptr->lastlistptr = NULL;
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
          /* no button to be made for hidden invented markers subgroup */
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->lastsgrpptr = thissgrpptr;/*first is visible sgrp*/
          sprintf(thissgrpptr->name,"orig markers");
          markerorigsgrpptr = thissgrpptr;
      }
      /*need 4 hidden, reference lists: 2 markers,1 center,1 labels*/
        if( !allocliststructure() )   /*MARKER #1 */
        {
            sprintf(alertstr,"alloclist failed for original marker objects(1)");
            goto allocerr;
        }
        else  
        {/*Do VECTOR points with Unpickable MOVE, Unpickable DRAW */  
            thislistptr->on = 0; /*list display OFF*/
            thislistptr->firstpointptr = NULL;
            thislistptr->lastpointptr = NULL;
            thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
            thislistptr->sgrpptr = thissgrpptr;
            thissgrpptr->firstlistptr = thislistptr;
            sprintf(thislistptr->name,"orig mark1");
            marker1origlistptr = thislistptr;  
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  p,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p, -p, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            , -p,  p,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p, -p, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p, -p,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  p, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  p, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p, -p,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
        }
        if( !allocliststructure() )   /*MARKER #2 */
        {
            sprintf(alertstr,"alloclist failed for original marker objects(2)");
            goto allocerr;
        }
        else  
        {/*Do VECTOR points with Unpickable MOVE, Unpickable DRAW */  
            thislistptr->on = 0; /*list display OFF*/
            thislistptr->firstpointptr = NULL;
            thislistptr->lastpointptr = NULL;
            thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
            thislistptr->sgrpptr = thissgrpptr;
            sprintf(thislistptr->name,"orig mark2");
            marker2origlistptr = thislistptr;  
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0, -p,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0,  p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            , -p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, -p); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  p,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | UNPICKABLE)
            ,  0,  0, 0); /*MAGELIST*/
        }
        if( !allocliststructure() )   /*GNOMON*/
        {
            sprintf(alertstr,"alloclist failed for original marker objects(3)");
            goto allocerr;
        }
        else  
        {/*Do points with pickable MOVE ,  pickable DRAW  */ 
            /*gnomon, ends and middle pickable for Jane */ 
            thislistptr->on = 0; /*list display OFF*/
            thislistptr->firstpointptr = NULL;
            thislistptr->lastpointptr = NULL;
            thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
            thislistptr->sgrpptr = thissgrpptr;
            sprintf(thislistptr->name,"orig gnomon");
            markergnom1origlistptr = thislistptr;  
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            , -9,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  9,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            ,  0, -9,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  9,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR | MOVETO_P)
            ,  0,  0, -9); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  0,  0); /*MAGELIST*/
            allocspecialintpoint(thislistptr, (VECTOR)
            ,  0,  0,  9); /*MAGELIST*/
        }
        if( !allocliststructure() )  /*GNOMON LABELS*/
        {
            sprintf(alertstr,"alloclist failed for original marker objects(4)");
            goto allocerr;
        }
        else  
        {/*Do points with UNpickable MOVE ,  UNpickable DRAW */ 
            /* gnomon:  LABEL for label MAGEFLAG.H*/ /*960810*/
            thislistptr->on = 0; /*list display OFF*/
            thislistptr->firstpointptr = NULL;
            thislistptr->lastpointptr = NULL;
            thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
            thislistptr->sgrpptr = thissgrpptr;
            thissgrpptr->lastlistptr = thislistptr;
            sprintf(thislistptr->name,"orig labels");
            markergnom2origlistptr = thislistptr;  
            allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  9,  0,  0);
            allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  0,  9,  0);
            allocspecialintpoint(thislistptr,(LABEL | UNPICKABLE),  0,  0,  9); 
        }
    /*marker original objects*/



 return;
 allocerr:
 { 
        dosinglealert(0); /*shows alertstr[] */ /*____DLOG.C*/
        Lmarkers = FALSE;
        return;
 }


}
/*___addmarkers()___________________________________________________________*/

/****killmarkers()***********************************************************/
void killmarkers()
{
    if(markergrupptr != NULL)
    {/*take care of all the marker things that are ever visible*/
        destroygrupstructure(markergrupptr);
        /*this NULLs all members and reconnects nonmember linkages*/
        /*but does NOT set the specific alias pointers to NULL !! */
        markergrupptr = NULL;
        markersgrpptr = NULL;
        marker1listptr = NULL; 
        marker2listptr = NULL;
        markergnom1listptr = NULL;
        markergnom2listptr = NULL;
        markerorigsgrpptr = NULL;
        marker1origlistptr = NULL;
        marker2origlistptr = NULL;
        markergnom1origlistptr = NULL;
        markergnom2origlistptr = NULL;
    }
    Lmarkers = 0;
}
/*___killmarkers()__________________________________________________________*/

/****killpickedpointers()****************************************************/
void killpickedpointers() /*050916 avoid PC crash, kill old picked pointer*/
{
   pickedlistoldptr  = NULL;  /*always allocated in MAGELIST.h*/
   pickedpointoldptr = NULL;
   if(sourcelistptr != NULL) {sourcelistptr = NULL;} /*061125*/
   if(sourcesgrpptr != NULL) {sourcesgrpptr = NULL;}
   if(sourcegrupptr != NULL) {sourcegrupptr = NULL;}
   if(targetlistptr != NULL) {targetlistptr = NULL;} /*061125*/
   if(targetsgrpptr != NULL) {targetsgrpptr = NULL;}
   if(targetgrupptr != NULL) {targetgrupptr = NULL;}
}
/*___killpickedpointers()___________________________________________________*/

/****addmeasures()***********************************************************/
void    addmeasures()  /* only if Lmarkers and room for measures as well */
{
    
   Lmeasures = FALSE;
                    /*MAGE.H MAXMEASURES defined 7 */
   if(Lmarkers)
   {/*Lmarkers TRUE*/

      Nthisgroupkind = 0;
      if( !allocgrupstructure() ) 
      {
          sprintf(alertstr,"failed to alloc group for measures");
          goto allocerr;
      }
      else
      {             
          thisgrupptr->on = 0; /* start out turned off for no display */
          thisgrupptr->type = 0; /*nothing special*/
          thisgrupptr->firstsgrpptr = NULL; 
          thisgrupptr->lastsgrpptr = NULL; 
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT; 
              /*measures group is by definition dominant, no buttons below*/
          thisgrupptr->master = 0; /*no master*/
          measuregrupptr = thisgrupptr;
          sprintf(thisgrupptr->name,"Measures");
      }
      if( !allocsgrpstructure() ) 
      {
          sprintf(alertstr,"failed to alloc subgroup for measures");
          goto allocerr;
      }
      else
      {       
          thissgrpptr->on = 1;
          thissgrpptr->firstlistptr = NULL;
          thissgrpptr->lastlistptr  = NULL;
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
              /* no button to be made for invented measures subgroup */
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->firstsgrpptr = thissgrpptr; 
          thisgrupptr->lastsgrpptr  = thissgrpptr;
      }                
      /*need 2 more lists beyond 4 for markers*/
      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc lists for measures");
          goto allocerr;
      }
      else
      {       
          thislistptr->on = 1; /*list display on*/
          thislistptr->type = 0; /*NOT a static list,NOT a detail list*/
          thislistptr->firstpointptr = NULL;
              /* points for angle measures */
          thislistptr->lastpointptr = NULL; 
              /* this will be set by picked points*/
          thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
          thislistptr->width = 5; /*990224 wide as widest feature*/
          thislistptr->sgrpptr = thissgrpptr;
          thissgrpptr->firstlistptr = thislistptr;
          measurelinelistptr = thislistptr;
          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0); 
          sprintf(word,"#1 point");   /* Point == Move */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0); 
          sprintf(word,"#2 point");   /* Line == Draw */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#3 point");   /* Line == Draw */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0); 
          sprintf(word,"#4 point");   /* Line == Draw */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
      } 
      
      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc lists for measures");
          goto allocerr;
      }
      else
      {       
          thislistptr->on = 1; /*list display on*/
          thislistptr->type = 0; /*NOT a static list, NOT a detail list*/
          thislistptr->firstpointptr = NULL; 
              /* average dots */
          thislistptr->lastpointptr = NULL; 
              /* this will be set by picked points*/
          thislistptr->color = numberofcolor("hotpink"); /*MAGECOLR.c*/
          thislistptr->sgrpptr = thissgrpptr;
          thissgrpptr->lastlistptr = thislistptr;
          measuredotslistptr = thislistptr;
          
          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 2 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          
          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 3 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          
          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 4 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
       
      /*971204*/    
          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
      /*971204*/ 

      } 
      
      /* give word a blank,*/ 
      /* \0 ends C word or declares P word of zero length */
      word[0] = '\0';
      

      Lmeasures = TRUE;
      if(!Ldrawstuff) maxnxyz = nxyz + NMARKERS + MAXMEASURES;
        /*reset maxnxyz, unless already set further*/
   }/*Lmarkers TRUE*/
   else
   {
       sprintf(alertstr,"angle measures can't be done"
                        " since markers not done");
       dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
   }
   return;
   allocerr:
   {
       dosinglealert(0); /*shows alertstr*/ /*____DLOG.C*/
       Lmeasures = FALSE;
       return;
   }
}
/*___addmeasures()__________________________________________________________*/

/****addmonitorparam()*******************************************************/
void    addmonitorparam()  /* only if Lmarkers and room for measures as well*/
{

   Lmonitorparam = FALSE;
                    /*MAGE.H MAXMONITOR defined 17 on spec 140915*/
   if(Lmarkers)
   {/*Lmarkers TRUE*/

      Nthisgroupkind = 0;
      if( !allocgrupstructure() )
      {
          sprintf(alertstr,"failed to alloc group for monitorparam");
          goto allocerr;
      }
      else
      {
          thisgrupptr->on = 1; /* NOT start out turned off for no display */
          thisgrupptr->type = 0; /*nothing special*/
          thisgrupptr->firstsgrpptr = NULL;
          thisgrupptr->lastsgrpptr = NULL;
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT;
              /*monitor group is by definition dominant, no buttons below*/
          thisgrupptr->master = 0; /*no master*/
          monitorgrupptr = thisgrupptr;
          sprintf(thisgrupptr->name,"MonitorParam");
      }
      if( !allocsgrpstructure() )
      {
          sprintf(alertstr,"failed to alloc subgroup for monitorparam");
          goto allocerr;
      }
      else
      {
          thissgrpptr->on = 1;
          thissgrpptr->firstlistptr = NULL;
          thissgrpptr->lastlistptr  = NULL;
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
              /* no button to be made for invented monitor subgroup */
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->firstsgrpptr = thissgrpptr;
          thisgrupptr->lastsgrpptr  = thissgrpptr;
      }
      /*need 2 more lists beyond 4 for markers*/
      if( !allocliststructure() )
      {
          sprintf(alertstr,"failed to alloc lists for monitorparam");
          goto allocerr;
      }
      else /* filled point-by-point in MAGEANGL/DoNucleicAcidParameters() */
      {
          thislistptr->on = 1; /*list display on*/
          thislistptr->type = 0; /*NOT a static list,NOT a detail list*/
          thislistptr->firstpointptr = NULL;
              /* points for monitorlines */
          thislistptr->lastpointptr = NULL;
              /* this will be set by points picked ...*/
          thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
          thislistptr->width = 5; /*990224 wide as widest feature*/
          thislistptr->sgrpptr = thissgrpptr;
          thissgrpptr->firstlistptr = thislistptr;
          monitorlinelistptr = thislistptr;
          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0);
          sprintf(word,"#1 point");   /* Point == Move to N1 of a base*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#2 point");   /* Line == Draw to C1' of ribose*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#3 point");   /* Line == Draw to C1' of next ribose*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#4 point");   /* Line == Draw to N1 of that next base*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#5 point");/* Line == Draw back to N1 of previous base*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0);
          sprintf(word,"#6 point");   /* Point == Move to P of i-1 PO4*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#7 point");   /* Line == Draw perp to C1m---C1*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0);
          sprintf(word,"#8 point");   /* Point == Move to P of i-1 PO4*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#9 point");   /* Line == Draw Pperp to C1m---N1m*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0);
          sprintf(word,"#10 point");   /* Point == Move to P of i PO4*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#11 point");   /* Line == Draw Pperp to C1---N1*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P), 0, 0, 0);
          sprintf(word,"#12 point");   /* Point == Move to P of i PO4*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
          allocspecialintpoint(thislistptr,(VECTOR),  0,  0,  0);
          sprintf(word,"#13 point");   /* Line == Draw Pperp to C1---N1*/
          storeptIDstring(word, thispointptr);            /*MAGELIST*/
      }

      if( !allocliststructure() )
      {
          sprintf(alertstr,"failed to alloc lists for monitorparam");
          goto allocerr;
      }
      else
      {
          thislistptr->on = 1; /*list display on*/
          thislistptr->type = 0; /*NOT a static list, NOT a detail list*/
          thislistptr->firstpointptr = NULL;
              /* dots for monitor not yet defined 140915*/
          thislistptr->lastpointptr = NULL;
              /* this will be set by picked points*/
          thislistptr->color = numberofcolor("hotpink"); /*MAGECOLR.c*/
          thislistptr->sgrpptr = thissgrpptr;
          thissgrpptr->lastlistptr = thislistptr;
          monitordotslistptr = thislistptr;

          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 2 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 3 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
          sprintf(word,"avg of 4 pts");/* Dot for average points */
          storeptIDstring(word, thispointptr);            /*MAGELIST*/

      /*971204*/
          allocspecialintpoint(thislistptr,(DOT), 0, 0, 0);/*MAGELIST*/
      /*971204*/

      }

      /* give word a blank,*/
      /* \0 ends C word or declares P word of zero length */
      word[0] = '\0';


      Lmonitorparam = TRUE;
      if(!Ldrawstuff) maxnxyz = nxyz + NMARKERS + MAXMEASURES + MAXMONITOR;
        /*reset maxnxyz, unless already set further*/
   }/*Lmarkers TRUE*/
   else
   {
       sprintf(alertstr,"monitorparam can't be done"
                        " since markers not done");
       dosinglealert(0); /*MACDLOG.C,MPCDLOG.C*/
   }
   return;
   allocerr:
   {
       dosinglealert(0); /*shows alertstr*/ /*____DLOG.C*/
       Lmonitorparam = FALSE;
       return;
   }
}
/*___addmonitorparam()______________________________________________________*/

/****killmeasures()**********************************************************/
void killmeasures()
{
    if(measuregrupptr != NULL)
    {
        destroygrupstructure(measuregrupptr);
        /*this NULLs all members and reconnects nonmember linkages*/
        /*but does NOT set the specific alias pointers to NULL !! */
        measuregrupptr = NULL;
        measuresgrpptr = NULL;
        measurelinelistptr = NULL;
        measuredotslistptr = NULL;
    }
    Lmeasures = 0;
}
/*___killmeasures()_________________________________________________________*/

/****killmonitorparam()******************************************************/
void killmonitorparam()
{
    if(monitorgrupptr != NULL)
    {
        destroygrupstructure(monitorgrupptr);
        /*this NULLs all members and reconnects nonmember linkages*/
        /*but does NOT set the specific alias pointers to NULL !! */
        monitorgrupptr = NULL;
        monitorsgrpptr = NULL;
        monitorlinelistptr = NULL;
        monitordotslistptr = NULL;
    }
    Lmonitorparam = 0;
}
/*___killmonitorparam()_____________________________________________________*/


/****adddrawstuff()***********************************************************/
void    adddrawstuff()  
    /* only if Lmarkers and room for measures as well as room for*/
{        /* more lines to be drawn, construct line takes 3 marked points */
   int        n,nn;
    
   Ldrawstuff = FALSE;
                       /*MAGE.H NDRAWPOINT  defined 7 */
                       /*MAGE.H MAXMEASURES defined 7 */
   /*must define this since drawlines could be invoked before measures*/
   {/*room for drawline*/
      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc list for original drawpptr");
          goto allocerr;
      }
      else  
      {/*Do points with Unpickable MOVE, Unpickable DRAW */  
        /*this is a marker that appears on the first picked point of*/
        /*a pair between which a line is being drawn*/
        /*This sets up the original figure which is copied later */
        /* to the specific point where the line is being drawn*/
        drawmarkeroriglistptr = thislistptr;  
        /*020305 size was all 2...*/
        allocspecialintpoint(thislistptr,(VECTOR | MOVETO_P | UNPICKABLE)
        , -5,  0,  0); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        ,  5,  0,  0); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        ,  0, -5,  0); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        ,  0,  5,  0); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        ,  0,  0, -5); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        ,  0,  0,  5); /*MAGELIST*/
        allocspecialintpoint(thislistptr,(VECTOR | UNPICKABLE)
        , -5,  0,  0); /*MAGELIST*/
      }
      /*need another group*/
      Nthisgroupkind = 0;
      if( !allocgrupstructure() ) 
      {
          sprintf(alertstr,"failed to alloc group for drawnew");
          goto allocerr;
      }
      else
      {       
          thisgrupptr->on = 1; /* start out turned on */ 
          thisgrupptr->type = 0; /*nothing special*/
          thisgrupptr->firstsgrpptr = NULL; 
          thisgrupptr->lastsgrpptr  = NULL;
          thisgrupptr->animate = 0; /*not animated to start*/
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT;  
              /*drawline group starts dominant, no buttons below*/
          thisgrupptr->master = 0; /*start with no master*/
          sprintf(thisgrupptr->name," new group");
              /*note leading space in group name*/
          drawgrupptr = thisgrupptr;
      } 
      /*need two subgroup, list combinations*/
      if( !allocsgrpstructure() ) 
      {
          sprintf(alertstr
              ,"failed to alloc subgroup for drawmarkersgrpptr");
          goto allocerr;
      }
      else
      {       
          /*this subgroup holds the this list for */
          /* a marker that appears on the first picked point of*/
          /*a pair between which a line is being drawn*/
          thissgrpptr->on = 1; /*subgroup on*/
          thissgrpptr->firstlistptr = NULL; 
          thissgrpptr->lastlistptr  = NULL; 
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON; 
             /*no button for invented subgroup*/ 
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->firstsgrpptr = thissgrpptr;
          drawmarkersgrpptr = thissgrpptr;
      }

      for(n=1; n<=6; n++) /*971122,020305*/
      {/*set up 6 drawmarker lists*/ /*971122,020305*/
        if( !allocliststructure() ) 
        {
          sprintf(alertstr,"failed to alloc list for drawmarkerlistptr");
          goto allocerr;
        }
        else
        {/*allocated*/     
          /*this list holds */
          /* a marker that appears on a picked point when*/
          /*a line is being drawn from that point to another*/
          thislistptr->on = 0;/*point indicater off to start*/
          thislistptr->firstpointptr = NULL;
          thislistptr->lastpointptr = NULL;
              /*draw pointer, set up for one point marker*/ 
          thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
          thislistptr->type = 0; /*not a static list, not a detail list*/
          thislistptr->sgrpptr = thissgrpptr;
          if(thissgrpptr->firstlistptr==NULL)
              thissgrpptr->firstlistptr = thislistptr;
          thissgrpptr->lastlistptr  = thislistptr;
          if(n==1)
          {
              thislistptr->color = numberofcolor("red"); /*MAGECOLR.c*/
              drawmarker1listptr = thislistptr;
          }
          if(n==2)
          {
              thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
              drawmarker2listptr = thislistptr;
          }
          if(n==3)
          {
              thislistptr->color = numberofcolor("blue"); /*MAGECOLR.c*/
              drawmarker3listptr = thislistptr;
          }
          if(n==4)/*971122*/
          {
              thislistptr->color = numberofcolor("magenta");/*MAGECOLR.c*/
              drawmarker4listptr = thislistptr;
          }
          if(n==5)/*020305*/
          {
              thislistptr->color = numberofcolor("yellow");/*MAGECOLR.c*/
              drawmarker5listptr = thislistptr;
          }
          if(n==6)/*020305*/
          {
              thislistptr->color = numberofcolor("cyan"); /*MAGECOLR.c*/
              drawmarker6listptr = thislistptr;
          }

          /*alloc points to hold the marker(s) when needed*/
          /*these markers are seven point pseudo jacks*/
          for(nn=1; nn<=7; nn++)
          {
              if(nn==1) /*type Unpickable MoveTo*/
                  allocspecialintpoint(thislistptr
                      ,(VECTOR | MOVETO_P | UNPICKABLE)
                      ,  0,  0,  0); /*MAGELIST*/
              else      /*type Unpickable DrawTo*/
                  allocspecialintpoint(thislistptr
                      ,(VECTOR | UNPICKABLE)
                      ,  0,  0,  0); /*MAGELIST*/
          }
        }/*allocated*/
      }/*set up 6 drawmarker lists*/ /*971122,020305*/
      if( !allocsgrpstructure() ) 
      {
          sprintf(alertstr,"failed to alloc subgroup for drawnew lines");
          goto allocerr;
      }
      else
      {       
          thissgrpptr->on = 1; /*subgroup on*/
          thissgrpptr->firstlistptr = NULL; 
          thissgrpptr->lastlistptr  = NULL; 
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON; 
             /*no button for invented subgroup*/ 
          thissgrpptr->grupptr = thisgrupptr;
          thisgrupptr->lastsgrpptr = thissgrpptr;
          drawnewsgrpptr = thissgrpptr;
      }

      if( !allocliststructure() ) 
      {
          sprintf(alertstr,"failed to alloc lists for drawnew lines");
          goto allocerr;
      }
      else
      {         
          thislistptr->on = 0; /* the new list starts off */
          thislistptr->firstpointptr = NULL;
                  /* set to be just beyond the possible 3 point markers*/
          thislistptr->lastpointptr = NULL; 
                  /* this will be set by picked points*/
          thislistptr->color = numberofcolor("green"); /*MAGECOLR.c*/
          thislistptr->type = 0; /*not a static list, not a detail list*/
          thislistptr->sgrpptr = thissgrpptr;
          thissgrpptr->firstlistptr = thislistptr;
          thissgrpptr->lastlistptr  = thislistptr;
          drawnewlistptr = thislistptr;
          sprintf(thislistptr->name,"Drawn");
      }
      if(!Lmeasures) 
          if(measuregrupptr != NULL) /*if there is a measures group */
              measuregrupptr->on = 0; /*set measures group off*/

      Ldrawstuff = TRUE;
      maxnxyz = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT; 
                                 /*at least...*/
      Lnewdrawline = 1; /*030402*/
   }/*room for drawline*/
   return;
   allocerr:
   {
        dosinglealert(0); /*shows alertstr */ /*____DLOG.C*/
        Ldrawstuff = FALSE;
        return;
   }
}
/*___adddrawstuff()__________________________________________________________*/

/****killdrawstuff()**********************************************************/
void killdrawstuff()
{

    if(drawgrupptr != NULL)
    {/*take care of all the drawline things that are ever visible*/
        destroygrupstructure(drawgrupptr);
        /*this NULLs all members and reconnects nonmember linkages*/
        /*but does NOT set the specific alias pointers to NULL !! */
        drawgrupptr = NULL;
        drawnewsgrpptr = NULL;
        drawnewlistptr = NULL;
        drawmarker1listptr = NULL;
        drawmarker2listptr = NULL;
        drawmarker3listptr = NULL;
        drawmarker4listptr = NULL; /*971122*/
        drawmarker5listptr = NULL; /*020305*/
        drawmarker6listptr = NULL; /*020305*/
        
    }
    /*then deal with the lists of drawline marker shapes which are copied*/
    if(drawmarkeroriglistptr != NULL) 
    {
        destroyliststructure(drawmarkeroriglistptr);
        drawmarkeroriglistptr = NULL;
    }
    Ldrawstuff = 0;
    Lclosedrawstuff = 0; /*020211*/
      /*during input of a new kinemage, any old drawline stuff is killed*/
      /*reading back in a just written .kip does this BEFORE the output*/
      /*routines have had a chance to call closedrawstuff() which in turn*/
      /*calls killdrawstuff.  So unset Lclosedrawstuff so output routine*/
      /*can avoid calling closedrawstuff(). */
      /*Also no need for closedrawstuff() to do drawgrupptr->on = 0 anyway. */
}
/*___killdrawstuff()_________________________________________________________*/

/****getinteger(c,Lnew)**************************************************/  
int        getinteger(int c,int Lnew)  
                /* sucessful acquisition of number sets IOerr = 0 */
{
    int        ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0; /*global*/
        /* Lnew local, must reset in calling routine */
		memset(numstr,' ',256);   /* optimized */
        /*for(j=0 ; j<256 ; j++) numstr[j] = ' '; */ /*global*/
    }

    if(c==' ' && inumstr==0)
    {
        ;  /* skip beginning spaces */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9'||c=='-'||c=='+')
    {
        numstr[inumstr] = c;
        inumstr++;
    }
    else  /* anything besides numerals ends a number*/
    {
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr == 0)
        {
            intpass = 0;
        }
        else
        {
            sscanf(numstr,"%d",&intpass); /* need a pointer */
        }
        ireturn = 1;  /* some number was retrieved or invented*/
        IOerr = 0; 
    }
    return(ireturn);
}
/*___getinteger()_________________________________________________________*/

/****getfloat(c,Lnew)****************************************************/  
int        getfloat(int c,int Lnew)  
                    /* sucessful acquisition of number sets IOerr = 0 */
{
    int        ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0; /*global*/
        /* Lnew local, must reset in calling routine */
		memset(numstr,'\0',256);  /* optimized */
        /* for(j=0 ; j<256 ; j++) numstr[j] = '\0'; */ /*global*/
    }
    
    if((inumstr==0&&c==' ')||(inumstr==0&&c=='\n')||(inumstr==0&&c=='\r'))
    {
        ;  /* skip beginning spaces, or CRLF between numbers */
    }
    else if(  c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'
            ||c=='7'||c=='8'||c=='9'||c=='.'
            ||(c=='-'&&inumstr==0)||(c=='+'&&inumstr==0) )
    {
        numstr[inumstr] = c;
        inumstr++;        /* only valid numeric character increases inumstr */
    }
    else  /* anything besides numerals ends a number*/
    {
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr == 0)
        {
            floatpass = 0.0; /* complete blank sets default = 0.0 */ 
        }
        else
        {
            sscanf(numstr,"%f",&floatpass); /*needs a pointer */
        }
        ireturn = 1;  /* some number was retrieved or invented*/
        IOerr = 0; 
    }
    return(ireturn);
}
/*___getfloat()_____________________________________________________________*/

/****getnewmatrix(c,Lnew,imatrix)********************************************/  
                    /* sucessful acquisition of 9 numbers sets IOerr = 0 */
int    getnewmatrix(char c,int Lnew,int imatrix)
{
static    int     n;      /* n remembers which of nine numbers accummulated*/
static    float   matrix[10]; /*matrix accummulates the numbers*/
        int       k, ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0;
        n = 1;
        Lidn = 0;
    }
    /*931126 matrix, view identifier */
    if(c == '{')  /* start an identifier */
    {
        Lidn = 1; 
        iword = 0;
    }
    else if(c == '}')  /* end an identifier */
    {

    for(k=iword ; k<(MAXVIEWIDCHAR) ; k++) word[k] = '\0';
          /*revision930207 pad out identifier to } with '\0' rather than ' '*/
        word[(MAXVIEWIDCHAR)] = '\0';  /* pad & end the string in c manner */ 
        {/*should hold identifier of this subgroup */
            /* view name: copy it to viewcom_ment */
            for(k=0; k<=(MAXVIEWIDCHAR) ; k++)         
            {
                viewcom[imatrix][k] = word[k]; 
            }
        }
        Lidn = 0;
    }
    else if(Lidn == 1)    /*note: no protection from line > 256 char !! */ 
    {
        word[iword++] = c;
    }
    else if( (c==' ' && inumstr==0)||(c==EOL && inumstr==0) ) /*950113*/
    {
        ;  /* skip beginning spaces */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9'||c=='-'||c=='.')
    {
        numstr[inumstr] = c;
        inumstr++;
    }
    else  /* anything besides numerals ends a number*/
    {
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr == 0)
        {
            ;  /* error */
        }
        else
        {
/*c*/        sscanf(numstr,"%f",&matrix[n]); /* get the number */
            inumstr = 0;                     /* reset char counter */
            n++;                            /* increase number counter */
            if(n >= 10)            /* see if nine numbers for matrix in yet */
            {    /* matrix was retrieved */
                /* put into reset matrix storage */
                ma[imatrix][1] =  matrix[1];
                ma[imatrix][2] = -matrix[2];  /*  1  2  3        1   0   0 */
                ma[imatrix][3] =  matrix[3];  /*  4  5  6   X    0  -1   0 */    
                ma[imatrix][4] =  matrix[4];  /*  7  8  9        0   0   1 */
                ma[imatrix][5] = -matrix[5];
                ma[imatrix][6] =  matrix[6];  /*screen handedness correction*/
                ma[imatrix][7] =  matrix[7];
                ma[imatrix][8] = -matrix[8];
                ma[imatrix][9] =  matrix[9];
                ireturn = 1;  
                IOerr = 0;

                if(imatrix == 0)
                {/*local rotation performed as coord input*/
                    ma[0][2] = -ma[0][2]; /*restore original handedness */
                    ma[0][5] = -ma[0][5];
                    ma[0][8] = -ma[0][8];
                }
            } 
        }
    }
   return(ireturn);
}
/*___getnewmatrix(c,Lnew,imatrix)___________________________________________*/

/****getneweuler(c,Lnew,imatrix)********************************************/  
     /*sucessful acquisition of 3 numbers and calc of matrix sets IOerr = 0*/
int  getneweuler(char c,int Lnew,int imatrix)
{
static    int     n;      /* n remembers which of three numbers accummulated*/
static    float   euler[4],phi=0,psi=0,tht=0; /* accummulate the numbers*/
          int     k, ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0;
        n = 1;
        Lidn = 0;
    }
    if( (c==' ' && inumstr==0)||(c==EOL && inumstr==0) ) 
    {
        ;  /* skip beginning spaces */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9'||c=='-'||c=='.')
    {
        numstr[inumstr] = c;
        inumstr++;
    }
    else  /* anything besides numerals ends a number*/
    {
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr == 0)
        {
            ;  /* error */
        }
        else
        {
/*c*/        sscanf(numstr,"%f",&euler[n]); /* get the number */
            inumstr = 0;                     /* reset char counter */
            n++;                            /* increase number counter */
            if(n >= 4) /*have 3 euler angles so can calculate new localmatrix*/           
            {   /* matrix can be calculated */
            /*081120 @localeulerradians phi,psi,theta As phi,theta,psi: z,x,z */
            /*       ref Goldstein 1st pag 109 for Havranek-Baker paper 200811*/
                phi = euler[1];
                psi = euler[2];
                tht = euler[3];
                /* put into input local matrix storage */
                /*  1  2  3  */
                /*  4  5  6  */    
                /*  7  8  9  */
                ma[0][1] =  cos(psi)*cos(phi)-cos(tht)*sin(phi)*sin(psi);
                ma[0][2] =  cos(psi)*sin(phi)+cos(tht)*cos(phi)*sin(psi);
                ma[0][3] =  sin(psi)*sin(tht);
                ma[0][4] = -sin(psi)*cos(phi)-cos(tht)*sin(phi)*cos(psi);
                ma[0][5] = -sin(psi)*sin(phi)+cos(tht)*cos(phi)*cos(psi);
                ma[0][6] =  cos(psi)*sin(tht); 
                ma[0][7] =  sin(tht)*sin(phi);
                ma[0][8] = -sin(tht)*cos(phi);
                ma[0][9] =  cos(tht);
                ireturn = 1;  
                IOerr = 0;
            } 
        }
    }
   return(ireturn);
}
/*___getneweuler(c,Lnew,imatrix)___________________________________________*/

/****getviewid(c,Lnew,imatrix)***********************************************/

int    getviewid(char c,int Lnew,int imatrix) /*931126 view identifier */
{
        int       k, ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    /*931126 matrix, view identifier */
    if(c == '{')  /* start an identifier */
    {
        Lidn = 1; 
        iword = 0;
    }
    else if(c == '}')  /* end an identifier */
    {
        for(k=iword ; k<(MAXVIEWIDCHAR) ; k++) word[k] = '\0';  /*950113*/
          /*revision930207 pad out identifier to } with '\0' rather than ' '*/
        word[(MAXVIEWIDCHAR)] = '\0';  /* pad & end the string in c manner */
        for(k=0; k<=(MAXVIEWIDCHAR) ; k++)  /*950113*/
        {
            viewcom[imatrix][k] = word[k];
        }
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        word[iword++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getviewid(c,Lnew,matrix)_______________________________________________*/

/****getaspect(c,Lnew,ith)***********************************************/

int    getaspect(char c,int Lnew,int ith) /*010603 aspect identifier */
{
        int       k, ireturn;
    
    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start an identifier */
    {
        Lidn = 1; 
        iword = 0;
    }
    else if(c == '}')  /* end an identifier */
    {
        for(k=iword ; k<(MAXVIEWIDCHAR) ; k++) word[k] = '\0'; 
          /*revision930207 pad out identifier to } with '\0' rather than ' '*/
        word[(MAXVIEWIDCHAR)] = '\0';  /* pad & end the string in c manner */
        for(k=0; k<=(MAXVIEWIDCHAR) ; k++)  
        {
            aspect[ith][k] = word[k];
        }
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        word[iword++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getaspect(c,Lnew,ith)___________________________________________________*/

/****getpalette(c,Lnew)*******************************************************/  
int    getpalette(char c,int Lnew)
{
static    int     n=0;  /* n remembers which of 256 numbers accummulated*/
static    int     m=0;  /* m remembers which of 3 rgb numbers accummulated*/
static    int     irgb[3] = {0,0,0}; /*matrix accummulates the numbers*/
static    int     index=0,inumstr=0;
static   char     numstr[256];
    int  ireturn=0;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0;
        n = 0;
        m = 0;
        Lidn = 0;
    }

    if(c == '{')  /* start an identifier */
    {
        Lidn = 1; 
        inumstr = 0; /*global*/
    }
    else if(c == '}')  /* end an identifier */
    {
       Lidn = 0;
       if(m == 0)
       {/*identifier is interpreted as index of palette entry*/
          numstr[inumstr] = '\0';  /* ends a number */
          if(inumstr == 0)
          {
             index = 0;
          }
          else
          {
             sscanf(numstr,"%d",&index); /* need a pointer here for sscanf*/
          }

       }
       else
       {
           m = 0; /*reset to count next triplet*/
           /*otherwise ignore trailing {...}*/
       }
       inumstr = 0;
    }
    else if(inumstr==0 &&(  (c==' ')
                          ||(c==',') 
                          ||(c=='\t')
                          ||(c==EOL) 
                         )
           )
    {
        ;  /* skip beginning white space */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||
            c=='6'||c=='7'||c=='8'||c=='9')
    {
        numstr[inumstr] = c;
        inumstr++;
    }
    else if(!Lidn) 
    {/* anything besides digits ends a number*/
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr > 0 && m < 3)
        {/*at least one character of a number*/
           /*and not gotten 3 numbers yet for this rgb color*/
           sscanf(numstr,"%d",&irgb[m]); /* get the number */ 
           inumstr = 0;                /* reset char counter */
           if(irgb[m] >= 0 && irgb[m] <= 255)
           {/*valid size for a palette entry*/
              if(m >= 2)            /* see if 0,1,2 rgb numbers are in yet */
              {/* rgb triplet was retrieved */
                 /*divide down by 256, multiply back up by 257*/
                 /* put into full rgb palette */
                 if(index >= 0 && index <= 255)
                 {/*possible index*/
                    myfullpalette[index][0] =  irgb[0];
                    myfullpalette[index][1] =  irgb[1];
                    myfullpalette[index][2] =  irgb[2];
                    n++; /*increase entry counter, even at end of palette*/
                 }
              }
              m++; /* increase rgb number counter */
           }/*valid size for a palette entry*/
        }/*at least one character of a number*/
    }/* anything besides digits ends a number*/
    if(n >= 256)
    {/*reached end of all possible palette entries*/
       ireturn = 1;
    }
    else
    {
       ireturn = 0;
    }
    return(ireturn);
}
/*___getpalette(c,Lnew,imatrix)______________________________________________*/

/****getmasterid(c,Lnew)******************************************************/
int    getmasterid(char c,int* Lnew) 
{
          int     k, ireturn;
    
    ireturn = 0;
    if(*Lnew)
    {
        Lidn = 0;
        *Lnew = 0;
    }
    if(c == '{')  /* start an identifier */
    {
        Lidn = 1; 
        iword = 0;
    }
    else if(c == '}')  /* end an identifier */
    {

    for(k=iword ; k<(MAXNAMECHAR) ; k++) word[k] = '\0';
        word[(MAXNAMECHAR)] = '\0';  /* pad & end the string in c manner */
        /*master name has now been accummulated in string word*/
        Lidn = 0;
        ireturn = 1;
        /*mastermaker() call moved to where getmasterid() called 051114*/
    }
    else if(Lidn == 1)
    {
        word[iword++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getmasterid(c,Lnew)____________________________________________________*/

/****getsinglequotedchar(c,Lnew)*********************************************/
int    getsinglequotedchar(char c,int* Lnew) 
{
          int     ireturn;
    
    ireturn = 0;
    if(*Lnew)
    {
        Lidn = 0;
    }
    if(Lidn==0 && c=='\'')  /* start an identifier */
    {
        Lidn = 1; 
        iword = 0;
        *Lnew = 0;
    }
    else if(Lidn==1 && c=='\'')  /* end an identifier */
    {/*master char has now been accummulated in string word*/
        if(iword == 1)
        {/*single character field*/
           masterchar = word[0];
        }
        else
        {
           masterchar = '\0';
        }
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        word[iword++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getsinglequotedchar(c,Lnew)____________________________________________*/

/****getcommandline(c,Lnew)**************************************************/
int    getcommandline(char c,int Lnew) 
{
static    int     ith; /*remembers in scope of a commandline*/
          int     ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start a command line */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the command line  */
    {
        /*command line has now been accummulated in string */
        /*end the string in a "C" manner*/
        input_command_line[ith] = '\0'; /*990404 input, not update*/
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        input_command_line[ith++] = c; /*990404 input, not update*/
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getcommandline(c,Lnew)_________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****gettitlestring(c,Lnew)**************************************************/
int    gettitlestring(char c,int Lnew)  /*990129*/
{
static    int     ith; /*remembers in scope of a titlestring*/
          int     ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start a titlestring */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the titlestring  */
    {
        /*titlestring has now been accummulated in string */
        /*end the string in a "C" manner*/
        kintitlestr[ith] = '\0';
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        kintitlestr[ith++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___gettitlestring(c,Lnew)_________________________________________________*/

/****getcommentinputstring(c,Lnew)********************************************/
int    getcommentinputstring(char c,int Lnew)  /*020917*/
{
static    int     ith; /*remembers in scope of a commentstring*/
          int     ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start a commentstring */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the commentstring  */
    {
        /*commentstring has now been accummulated in string */
        /*end the string in a "C" manner*/
        commentinputstr[ith] = '\0';
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        commentinputstr[ith++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getcommentinputstring(c,Lnew)___________________________________________*/

/****getcopyrightstring(c,Lnew)**********************************************/
int    getcopyrightstring(char c,int Lnew) /*990129*/
{
static    int     ith; /*remembers in scope of a copyrightstring*/
          int     ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start a copyrightstr */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the copyrightstr  */
    {
        /*copyrightstr  has now been accummulated in string */
        /*end the string in a "C" manner*/
        kincopyrightstr[ith] = '\0';
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        kincopyrightstr[ith++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getcopyrightstring(c,Lnew)_____________________________________________*/

/****getreferencefilestring(c,Lnew,Lkind)*************************************/
int    getreferencefilestring(char c,int Lnew,int Nkind) /*990622,030312*/
{
static    int     ith; /*remembers in scope of a pdbfilestring*/
          int     ireturn;

    ireturn = 0;
    if(Lnew)
    {
        Lidn = 0;
    }
    if(c == '{')  /* start a pdbfilestr */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the pdbfilestr  */
    {
        /*reference file name has now been accummulated in string */
        /*end the string in a "C" manner*/
       if(Nkind==PDB)
       {
          if(thisgrupptr == NULL)
          {
             kinpdbfilestr[ith++] = '\0';
          }
          else
          {
             thisgrupptr->pdbfilestr[ith++] = '\0';
          }
       }
       else if(Nkind==MAP)
       {
           kinmapfilestr[ith++] = '\0';
       }
       /*typical of Mage: what it doesn't understand, it ignores*/
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
       if(Nkind==PDB)
       {
          if(thisgrupptr == NULL)
          {
             kinpdbfilestr[ith++] = c;
          }
          else
          {
             thisgrupptr->pdbfilestr[ith++] = c;
          }
       }
       else if(Nkind==MAP)
       {
          kinmapfilestr[ith++] = c;
       }
          
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getreferencefilestring(c,Lnew,Ntype)____________________________________*/

/****getbracketstring(c,Lnew)************************************************/
int    getbracketstring(char c,int* Lnew)
{
static    int     ith; /*remembers in scope of a string*/
          int     ireturn;

    ireturn = 0;
    if(*Lnew > 0)
    {
        Lidn = 0;
        /* Lnew local, must reset as a pointer in the calling routine */
        *Lnew = 0;
    }
    if(c == '{')  /* start a string */
    {
        Lidn = 1; 
        ith = 0;
    }
    else if(c == '}')  /* end the string  */
    {
        /*end the string in a "C" manner*/
        numstr[ith] = '\0';
        Lidn = 0;
        ireturn = 1;
    }
    else if(Lidn == 1)
    {
        numstr[ith++] = c;
    }
    else 
    {
        ;  /* skip anything not in brackets */
    }
    return(ireturn);
}
/*___getbracketstring(c,Lnew)_____________________________________________*/

/****getinputstring(c,Lnew)**************************************************/
int  getinputstring(char c,int* Lnew)  
{
    int ireturn = 0;
    int j=0;
    
    if(*Lnew > 2)
    {
        itemps = 0; /*global*/
        for(j=0; j<256; j++) temps[j]='\0'; /*global char[256]*/
        /* Lnew local, must reset as a pointer in the calling routine */
        *Lnew = 2;
    }

    if(itemps==0 && (c==' ' || c==',') )
    {
        ;  /* skip beginning spaces */
    }
    else if(c==' ' || c=='\0' || c==EOL ) 
    {/* ends the string*/
        temps[itemps] = '\0'; 
        ireturn = 1;  /* some string was retrieved, could be NULL */
    }
    else 
    {
        temps[itemps] = c;
        itemps++;
    }
    return(ireturn);
}
/*___getinputstring()________________________________________________________*/

/****killbondrotsliders()*****************************************************/
void killbondrotsliders(int kill)  /*separate routine 070617*/
{
   /*kill==1: kill both sliders and underlying bondrot structures*/
   /*kill==0: kill only sliders NOT underlying bondrot structures*/

   int i=0;
           for(i=1; i<=nbondrot; i++)
           {/*loop over bondrots*/
              if(bondrotptr[i] != NULL)
              {                  
                 if(bondrotptr[i]->level > 0 || bondrotptr[i]->level== -1)
                 {/*independent rotation or first of a ganged set of rotations*/
                    /*only these have sliders*/
                    erasebondrotvalue(i);
                    /*removes any previous kinemage bond rotation text*/
#ifdef UNIX_MOTIF
                    XtDestroyWidget(hgrafbondrot[i]);
                    hgrafbondrot[i] = NULL; /*031011*/
                    XtDestroyWidget(bondrotvalue[i]);
                    bondrotvalue[i] = NULL; /*031011*/
                    XtDestroyWidget(bondrotscope[i]);
                    bondrotscope[i] = NULL; /*031011*/
                    XtDestroyWidget(bondrotdivide[i]);
                    bondrotdivide[i] = NULL; /*031011*/
#endif
                 }/*independent rotation or first of a ganged set of rotations*/
                 if(kill){destroybondrotliststructure(bondrotptr[i]);}
              }
              if(kill){bondrotptr[i] = NULL;}
           }/*loop over bondrots*/
#ifdef UNIX_MOTIF
           if(topbrot != NULL && kill)
           {
              destroybondrotwindow(); /*MUXMWIND.c 031011*/
           }
#endif
}
/*___killbondrotsliders()____________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
