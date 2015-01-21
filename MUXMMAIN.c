    /*MUX_MAIN.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#define EXTERN   /* as nothing, this file has the main() */
#include "MAGE.h"   
#include "MAGELIST.h"
#include "MAGEDLOG.h"
#include "MAGEMENU.h"
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include "MAGESYNC.h" /*sync and adjust: controls, sockets,...*/
#include "MAGEBBOX.h" /*121108 for adjustrock, etc... */

/****main()*******************************************************************/
int main(int argc, char** argv) /*121108 main is of type int !? */
{
  XEvent report;
int Limages=0,Nimage=0; /*030725*/

/*  setenv( "LANG", "C", 1 );*/ /* MGP: maybe needed for Redhat 8.0 ? */

/*printf("CurrentTime==%ld\n",CurrentTime);*/ /*keep so dcr remembers call*/
/*PEX stuff kept as guide for eventual openGL port*/

  Lmagedone = 0; /*master flag, set true exits program */
  mainsetup(&argc, argv); /*MUXMINIT.C*/

if(LGUI)
{/*LGUI*/
     
#ifdef _STDFONTCURSORS_
printf("defined: _STDFONTCURSORS_\n");
#endif

#ifdef UNIX_PEX
  OpenPhigs();   /*MUXPHIG.c*/
#endif /*UNIX_PEX*/

  while (!Lmagedone) /* until user wants to quit... */
  {/*master event loop: while(!Lmagedone)*/

    /*enter check-que only if not doing an auto update function*/
    /*OR something has triggered the XtAppPending flag*/
    /*like a keypress associated with drawing. */
    if(  (!Lmickeymouserun&&!Lmagetimerrunning)
       || Lpausemickeymouse
       || (XtAppPending(app_context)!=0)
      ) 
    {/*check event que*/

      XtAppNextEvent(app_context,&report); /* get the next event */
#ifdef UNIX_PEX
      if (report.type < LASTEvent)
#endif
      {
 
         if(report.type == KeyPress||report.type == KeyRelease ) 
         {/*key press or release: need both to track ShiftKey state*/
           /*if(!ACTIVE_DLOG && !Ltexteditable) */
           if(!ACTIVE_DLOG) 
           {
             if(   Ltexteditable
                && (  (Window)(report.xkey).window == XtWindow(textwindow)
                    ||(Window)(report.xkey).window == XtWindow(captwindow)))
             {/*Ltexteditable, catch key in text and caption windows*/
                XtDispatchEvent(&report);
             }/*Ltexteditable, catch key in text and caption windows*/
             else
             {
                fprintf(stderr," \010");/*print space+backspace to trick SGI*/
                keypresseventhandler(&report);/*MUX_MAIN.c*/
             }
           }
           else
           {/*ACTIVE_DLOG*/
               XtDispatchEvent(&report);
           }
         }/*key press or release*/
         else
         {
             XtDispatchEvent(&report);
         }
         /*NOTE:  mousedown event calls pick_CB as handler */
      }
#ifdef UNIX_PEX
      else
      {
          dospecialevent(&report);  /*MUXPHIG.c*/
      }
#endif /*UNIX_PEX*/
    }/*check event que*/
      
    if(Lpipeactive)
    {/*controls for input from an active pipe*/
         if(Lpipebufferloaded)
         {
            if(!Linhibiton)
            {
/*does NOT come through here doing simulation from tail of active file!!!!*/
               if(framenumber==0) Lappend = 0;
               else Lappend = 1;
               GetStuffFromFile(); /*MAGEINPT*/
            }
         }
         else
         {
            readpipeintobuffer(1); /*up to #char before returning,____PIPE*/
         }
    }/*controls for input from an active pipe*/

    if(Lsocketoutactive) /*more restrictive than Lsockets*/
    {
         syncsocketout(0); /* 0 for normal flow MAGESYNC.c*/
    }
    if(Lsocketoutextraactive) /*more restrictive than Lsockets*/
    {
         syncsocketoutextra(0); /* 0 for normal flow MAGESYNC.c*/
    }

    if(Lsockettrigger)
    {/*commandline request for socket setup*/
       setsockets(0); /*create socket for input*/
       Lsockettrigger = 0;
#ifdef ALLATONCE
       if(Lsocketsame == 1)
       {/*mages must be initiated in reverse order, so 1-->2 and 1-->3*/
          setsockets(Lsockets); /*-->2*/
          setsockets(Lsockets); /*-->3*/
          Lsocketreturn = 1; /*syncsocketout to request a return connection*/
          Lsocketoutactive = 1;
       }
#endif
    }

    if(Lmickeymouserun && !Lpausemickeymouse)
    {/*LINUX does NOT provide a stream of interrupts, so rocking is spastic*/
       /*however, checking for XtAppPending keeps things clicking*/
       /*animatesteptime and rocksteptime floating point seconds*/
       /*ticks are long integer 60ths of a second*/
       if(Lautorock && theclocktickcount() > nextrocktick)
       {
           nextrocktick = theclocktickcount() + (long)(rocksteptime*60);
           adjustrock(); /*MAGEBBOX.C*/
           /*redrawvec() called by adjustrock()*/
       }
       if(Lautoanimate&&(nanimate > 1)&&(theclocktickcount() > nextanimatetick))
       {
           nextanimatetick = theclocktickcount() + (long)(animatesteptime*60);
           adjustanimate(1); /*MAGEBBOX.C*/
           if(Lautorock) adjustrock();
           else redrawvec();
       }
    }
    if(Ltestmode) /*magetimeticklimit*/
    {
       magetimercheck(); /*MAGEINIT.c 020816*/
    }
    if(Lsearched) /*NON-MODAL FIND DIALOG left unfinished business...030121*/
    {
       DoSearchDialog(); /*MUXMDLOG.c*/
    }
    if(LneedSetsinGrafWindow)
    {
       SetsinGrafWindow(); /*MAGEBBOX.c  030320*/
       redrawvec(); /*040609 now not called by entry(), but needed here*/
    }
    if(Lreloadkinemage)
    {
       reloadkinemage();  /*060121*/
    }
    if(Lreplacemarkers)
    {
       replacemarkers();  /*061116*/
    }
    if(Lrescalekinemage)
    {
        rescalekinemage();
    }
    if(Lredrawvector)
    {
       redrawvec();
    }
    if(Lparamtriggeron) /*141006*/
    {
       Lparamtriggeron = 0;
       DoNucleicAcidParameters();
    }
  }/*master event loop: while(!Lmagedone)*/
  if(Lsockets)
  {
      syncsocketout(1); /*1 to close sockets*/
      syncsocketoutextra(1); /*1 to close sockets*/
  }
}/*LGUI*/
else
{/*NOT LGUI*/
  if(Lpostscript)
  {/*write eps file(s) of each animation*/
    if(nanimate > 0) {Limages = nanimate;} /*030725*/
    else {Limages = 1;} /*030725*/
    Nimage = 1;
    while(Limages > 0)  /*030725*/
    {
      /*fpout = stdout;*/ /*NOT LGUI used to write to stdout*/
      sprintf(OutStr,"%s.%d.eps",NameStr,Nimage);
      fpout = fopen(OutStr,"w"); /*MAGEPOST closes fpout at end of kinemage*/
      writepostscript(); /*writes postscript header stuff for NOT LGUI*/
      drawvec(); /*draws the kinemage and writes postscript line-by-line*/
      Limages--;  /*030725*/
      Nimage++;  /*030725*/
      if(Limages) {adjustanimate(1);} /*MAGEBBOX.C*/  /*030725*/
    } 
  }/*write eps file(s) of each animation*/
  else if(Lhelpstdout)    { writehelptostdout();    /*MAGEHELP.c*/ }
  else if(Lchangesstdout) { writechangestostdout(); /*MAGEINIT.c*/ }
  else if(Lreportstderr)  { writereporttostderr();  /*MAGEINIT.c 070901*/ }
}/*NOT LGUI*/

   exit(0);
  
}
/*___main()_________________________________________________________________*/

/****keypresseventhandler()***************************************************/
void keypresseventhandler(XEvent* event)
{/*keyboard events*/
  /*ref: Johnson&Reichard,2nd Ed,Motif,MIS Press,1993, pg 387*/
  KeySym         keysym;
  XComposeStatus compose_status;
  int            length, size=9;
  char           string[10];
  float          factor;

/*keysym:*/
#define SPACEBAR  32
#define LARROW 65361
#define RARROW 65363
#define DARROW 65364
#define UARROW 65362

#define LSHIFT 65505
#define RSHIFT 65506
#define LCONTROLKEY 65507 
#define RCONTROLKEY 65508 
#define LALTKEY 65513 
#define RALTKEY 65514 

  switch(event->type)
  {
    case KeyRelease: /*121108 disambiguate with () and clean code*/
      length = 
         XLookupString((XKeyEvent *)event,string,size,&keysym,&compose_status);
      if(keysym==SPACEBAR){ Lspacebar=0;}
      if((keysym==LSHIFT) || (keysym==RSHIFT)){ Lshiftkey=0;}
      if((keysym==LCONTROLKEY) || (keysym==RCONTROLKEY)){ Lctrlkey=0;}
      if((keysym==LALTKEY) || (keysym==RALTKEY)){ Laltkey=0;}
/*
fprintf(stderr,"key: %s, l== %d, e->state== %u, ->code== %u, sym== %u, Lspace==%d, Lshift==%d, Lctrl==%d, Lalt==%d\n"
,string,length,((XKeyEvent *)event)->state,((XKeyEvent *)event)->keycode,keysym,Lspacebar,Lshiftkey,Lctrlkey,Laltkey);
*/
    break;

    case KeyPress:
      length = 
         XLookupString((XKeyEvent *)event,string,size,&keysym,&compose_status);
      if(keysym==SPACEBAR){ Lspacebar=1;}
      if(keysym==LSHIFT || keysym==RSHIFT){ Lshiftkey=1;}
      if(keysym==LCONTROLKEY || keysym==RCONTROLKEY){ Lctrlkey=1;}
      if(keysym==LALTKEY || keysym==RALTKEY){ Laltkey=1;}
/*
fprintf(stderr,"key: %s, l== %d, e->state== %u, ->code== %u, sym== %u, Lspace==%d, Lshift==%d, Lctrl==%d, Lalt==%d\n"
,string,length,((XKeyEvent *)event)->state,((XKeyEvent *)event)->keycode,keysym,Lspacebar,Lshiftkey,Lctrlkey,Laltkey);
*/
      /*drop through from special key-down logical state settings*/
      if(length>0 && length<=9)
      {
        string[length] = '\0';
        
        /*020718 control key forces all groups animate: for simulation file*/
        if(  ((string[0] == 'a') || (string[0] == 'A'))
           &&((nanimate > 1)) ) 
        {/* 'a'  animation*/
             adjustanimate(1); /*MAGEBBOX.c*/
             redrawvec();      /*___DRAW.c*/
        }
        if(  ((string[0] == 'b') || (string[0] == 'B'))
           &&((nanimate > 1)) ) 
        {/* 'b' 2animation*/
             adjustanimate(2); /*MAGEBBOX.c*/
             redrawvec();      /*___DRAW.c*/
        }
        if(string[0]=='c')
        {/* c toggles +,- stereo angle */
            stereoangle = -stereoangle;
            redrawvec(); /*___DRAW.c*/
        }        
        if(string[0]=='d')
        {/* d toggles dragline pick-up/new */
            Ladddrag = !Ladddrag;
            redrawvec(); /*___DRAW.c*/
        }
        if(string[0]=='e')
        {/* e toggles between Lens on and Lens off */ /*971109*/
           setLens(); /*MAGEMENU.C*/
            redrawvec(); /*___DRAW.c*/
        }
        if(string[0]=='f')
        {/* f toggles scroll state, either xy rotations or xy tranlations */
            setflat();
        }        
        if(string[0]=='F' && Lkinfile)
        {/* F invokes Find == search dialog */
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            DoSearchDialog(); /*MUXMDLOG*/
            /*redrawvec();*/ /*___DRAW.c*/
        }
        if(string[0]=='g')
        {/* g toggles white back-g-round */
            setwhtbkg();
        }
        if(string[0]=='G' && Lkinfile)
        {/* G invokes Find Again == search again for same strings */
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            DoSearchAgain(); /*MAGEUTIL*/
            /*redrawvec();*/ /*___DRAW.c*/
        }        
        if(string[0]=='h')
        {/* h toggles maintain horizon state, only y rotation */
            sethorizon();
        }
        if(string[0]=='H')
        {/* Help : keyboard Shortcuts dialog*/
            menuaboutsecrets(0);
        }

        if(string[0]=='i' && (NDIM > 3) )
        {/* i toggles LNDpointsinfo 061115 */
           LNDpointsinfo = !LNDpointsinfo;  
           if(LNDpointsinfo) {adjustSELECTINGlist(7);} /*MAGEDLOG*/
           else              {adjustSELECTINGlist(3);}
           redrawvec(); /*___DRAW.c*/
        }

        if(string[0]=='I' && (NDIM > 3) )
        {/* I toggles LNDpickedinfo 061117 */
           LNDpickedinfo = !LNDpickedinfo;  
           redrawvec(); /*___DRAW.c*/
        }

        if(string[0]=='k')
        {/* k toggles kaleidoscope==plotonly mode */
            mycheckitem(kludgeMenu,plotonlyItem,Lplotonly=!Lplotonly);
                /*___MENU.c*/
            redrawvec(); /*___DRAW.c*/
        }        
        if( string[0] == 'K' )
        { /* 'K'  next Kinemage, re: meta-N on Mac */
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            Lgetnextkinemage = 1; /*971129*/
            changecheckrouter(); /*MAGEINIT.c 020902*/
        }
        if(string[0]=='l')
        {/* l toggles listcolor state, either color specified by list or point*/
            setlistcolordominance();
            redrawvec(); /*___DRAW.c*/
        }
        if(string[0]=='L')
        {/* L toggles listradius state, radius specified by list or point*/
            setlistradiusdominance(); /*040402*/
            redrawvec(); /*___DRAW.c*/
        }

        /* m blanks measures lines & dots, does not remove old values */
        /*does disable measures operation, i.e. Lmeasureson==0, button==off*/
        /*but potentiates a new measures sequence for a new Lmeasureson==1*/
        /*effectively NOP if hit when measures button is off*/
        /*971204*/
        if(string[0]=='m')
        {/* m blanks measures lines & dots*/
           /*new 140518: errases ANGLE report: construct5 AngleBetweenLines*/
           constructionstr[0] = '\0'; /*140518, tested in MAGEDRAW/redrawvec*/ 
           monitorparamstr[0] = '\0'; /*140915, tested in MAGEDRAW/redrawvec*/ 
           potentiateMeasures(); /*MAGEMENU.c note calls redrawvec 140518*/
        }
        if(string[0]=='n')
        {/* n for +90 ninty*/  /*971104*/
           getrot(0,0, 2); /*MAGEDRAW*/
           redrawvec();
        }
        if(string[0]=='N')
        {/* N for -90 Ninty*/  /*971104*/
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
           getrot(0,0,-2); /*MAGEDRAW*/
           redrawvec();
        }
        if( (string[0] == 'O')&&(!Ltestmode) ) /* 'O'  open*/
        {
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            Lappend = 0;
            Lgetnewfile = 1; /*request to open-new-file */
            ireturnflag = 0;
changecheckrouter(); /*MAGEINIT.c 020902*/
#ifdef OLDCODE
            if(Lpruneactive||Ldrawactive||Lnewviewactive||Lnewstuffactive)
            {/*check dialog sets ireturnflag*/
                 DoChangeCheckDialog();
            }
            else
            {/*can go right to open-new-file stuff*/
                OpenNewFile(); /*.*/
            }
#endif
        }
        if(string[0]=='p')
        {/* p toggles keepperspective */
            /*Lkeepperspective = !Lkeepperspective;*/
            /*if(Lkeepperspective) Lperspec = 1;*/
            /*else                 Lperspec = 0;*/
            Lperspec = !Lperspec; /*991130 like Mac 981114*/
            resetmenuchecks(); /*___PMENU.c calls AdjustMenus();*/
            redrawvec();       /*___DRAW.c*/
        }
        if( string[0] == 'P' ) /* 'P' */
        {/* P Puts PointID into commandline string */
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            commandfrompointID(); /*MAGEOUT.c*/
        }
        if(string[0]=='q')
        {/* q toggles double-sized markers, see 'w' */ /*000804*/
            Lscalemarkers = !Lscalemarkers;
            /*resetmenuchecks();no menu item yet*/ 
            /*____MENU.C calls AdjustMenus();*/
            rescalekinemage();  /*MAGEINPT*/
            redrawvec();       /*MACDRAW.C*/
        }
        if( (string[0] == 'Q')&&(!Ltestmode) ) /* 'Q'  quit*/
        {
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
             trytoquitMAGE();      /*____MENU.c*/
        }
        if( (string[0] == 'r') || (string[0] == ' '))/*i.e. Lspacebar*/
        {/* r toggles mickeymouse autoanimate and autorock performance 010609*/
         /* spacebar RUN/STOP once empowered by dialog makes sense*/
         /* difference between stop and pause is kludgy*/
         /*so use of r/R key is best compromise*/
          if(Lmickeymouse==1)
          {/*must be empowered by dialog before controled by keyboard*/
            Lautorock = !Lautorock;
            if(Lautorock || Lautoanimate) 
                 {Lmickeymouserun = 1; /*some auto running*/ }
            else {Lmickeymouserun = 0; /* no auto running*/ }
            if(!Lautorock)
            {/*reset current rotation matrix to current rocked state*/
               a11 = r11; a12 = r12; a13 = r13;
               a21 = r21; a22 = r22; a23 = r23;
               a31 = r31; a32 = r32; a33 = r33;
            }
            else
            {/*reset current rocked state to current rotation matrix*/
               r11 = a11; r12 = a12; r13 = a13;
               r21 = a21; r22 = a22; r23 = a23;
               r31 = a31; r32 = a32; r33 = a33;
            }
            Lnewrock = 1; /*autorock to restart smoothly from mouse setting*/
            redrawvec(); /*remove on-screen notification*/
          }/*must be empowered by dialog before controled by keyboard*/
          else
          {
             ;/*NOP*/
          }
        }
        /*subvert R so it no longer resets to view 1   010609*/
        if( string[0] == 'R' ) /* R for autoanimate */
        {
          if(Lmickeymouse==1)
          {/*must be empowered by dialog before controled by keyboard*/
            Lautoanimate = !Lautoanimate;
            if(Lautorock || Lautoanimate) 
                 {Lmickeymouserun = 1; /*some auto running*/ }
            else {Lmickeymouserun = 0; /* no auto running*/ }
            redrawvec(); /*possible reset of on-screen notification*/
          }
          else
          {
             ;/*NOP*/
          }
        }
        if(string[0]=='s' && !Lcompareon)
        {/* s toggles keepstereo */
            Lkeepstereo = !Lkeepstereo;
            if(Lkeepstereo) Lstereo = 1;
            else            Lstereo = 0;
            adjuststereo(); /*MAGEMENU*/
        }
        if( string[0] == 'S' ) /* 'S'  save whole kin as a kip*/
        {
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
            writeoutput(5);/*MUX_OUT.c*/ /*writes entire, possibly modified, kinemage*/
        }

        if(string[0]=='t')
        {/* t toggles keepthinline */
            Lkeepthinline=0; /*this keep stuff was a bad idea anyway*/
            if(Lthin){Lthin=0;Lonewidth=1;Lmultiwidth=0;}
            else if(Lonewidth){Lthin=0;Lonewidth=0;Lmultiwidth=1;}
            else if(Lmultiwidth){Lthin=1;Lonewidth=0;Lmultiwidth=0;}
            resetmenuchecks();/*___MENU.c calls AdjustMenus();*/
            redrawvec();      /*___DRAW.c*/
        }
        if(string[0]=='u' &&Lkinfile) 
        {/* u invokes general update Dialog */
            if(Lkinfile) DoupdateDialog(); /*MAGEDLOG*/
        }
        if( string[0] == 'V' ) /* 'V'  viewing parameters dialog*/
        {
            Lshiftkey=0; /*drop no-longer-needed shift flag*/
             DostereoDialog(); /*which is still does as well*/
        }
        if(string[0]=='w')
        {/* w toggles double-sized fonts, see 'q' */ /*010521*/
            Lscalefonts = !Lscalefonts;
            /*resetmenuchecks();no menu item yet*/ /*MACPMENU.C calls AdjustMenus();*/
            rescalekinemage();  /*MAGEINPT*/
            redrawvec();       /*MACDRAW.C*/
        }

        if(string[0]=='y')
        {/* y toggles 5 depths vs 3 depths */ /*010927*/
            L5depths = !L5depths;
            if(L5depths) {L3depths=0;}
            else {L3depths = 1;}
            resetmenuchecks(); /*MAGEMENU.C calls AdjustMenus();*/
            redrawvec();       /*____DRAW.C*/
        }

        if(Lztran)
        {
            factor = 0.0;
            if(string[0]=='x') factor = -1.0;
                  /*press 'x' to do small step translate in -z*/
            if(string[0]=='X') factor = -10.0;
                  /*press 'X' to do big step translate iz*/

            if(string[0]=='z') factor = +1.0;
                  /*press 'z' to do small step translate in +z*/
            if(string[0]=='Z') factor = +10.0;
                  /*press 'Z' to do big step translate in +z*/
            if(factor > 0.01 || factor < -0.01)
            {
                fxcenternew = fxcenternew + (float)(factor*finestep*a13);
                fycenternew = fycenternew + (float)(factor*finestep*a23);
                fzcenternew = fzcenternew + (float)(factor*finestep*a33);
                rescalekinemage();  /*MAGEINPT.c*/
                redrawvec();        /*___DRAW.c*/
            }
        }

        if(string[0]=='1') 
        {/* 1 zooms bigger by one unit*/
           zoom = zoom +0.01;
           if(zoom > 10.04) zoom = 10.04;/*maximum*/
           adjustzoom(); /*MAGESYNC.c*/
        }
        if(string[0]=='2') 
        {/* 2 zooms bigger by ten units*/
           zoom = zoom +0.1;
           if(zoom > 10.04) zoom = 10.04;/*maximum*/
           adjustzoom(); /*MAGESYNC.c*/
        }
        if(string[0]=='3') 
        {/* 3 zooms smaller by one unit*/
           zoom = zoom -0.01;
           if(zoom < .04) zoom = .04;/*minimum*/
           adjustzoom(); /*MAGESYNC.c*/
        }
        if(string[0]=='4') 
        {/* 4 zooms smaller by ten units*/
           zoom = zoom -0.1;
           if(zoom < .04) zoom = .04;/*minimum*/
           adjustzoom(); /*MAGESYNC.c*/
        }
        if(string[0]=='9') 
        {/* 9 augments TBstereo offset amount*/
           TBYoffset++;
           redrawvec();
        }
        if(string[0]=='0') 
        {/* 0 decrements TBstereo offset amount*/
           TBYoffset--;
           redrawvec();
        }
        if(string[0]==']') /* ] augments currentaspect*/
        {
           adjustcurrentaspect(+1);
        }
        if(string[0]=='[') /* [ decrements currentaspect*/
        {
           adjustcurrentaspect(-1);
        }
        if( ((string[0]=='+')||(string[0]=='=')) && (nanimate > 1) ) /*030111*/
        {/* += accumulates animates*/
           Laccumulate = 1;
           adjustanimate(1); /*MAGEBBOX.c*/
           Laccumulate = 0;
           redrawvec();      /*___DRAW.c*/
        }
        if(string[0]=='/' && (NDIM > 3) )
        {/* forward slash toggles LNDparallelaxes 061115 */
           LNDparallelaxes = !LNDparallelaxes;  
           redrawvec(); /*___DRAW.c*/
        }

      }
      else
      {
        if(nbondrot > 0 )
        {/*arrow keys*/
           /*<- one degree less of current bondrotation*/ /*970917*/
           if(keysym==LARROW) 
           {
              grafbondrotarrow(-1.0); /*MAGEANGL*/
           }
           /*-> one degree more of current bondrotation*/ /*970917*/
           if(keysym==RARROW)
           {
              grafbondrotarrow(+1.0); /*MAGEANGL*/
           }
           if(keysym==DARROW && LbondrotHplot && LbondrotVplot)
           {
              dobondrotplotter(); /*MAGEANGL*/
           }
           if(keysym==UARROW && LbondrotHplot && LbondrotVplot)
           {
              dobondrotgrapher(); /*MAGEANGL*/
              if(Lsocketout && Lsocketidsearch)
              {
                 Lsocketoutactive = 1;
                 Lsocketoutgrapher = 1;
              }
#ifdef TRYFORTHIRD
              if(Lsocketoutextra && Lsocketidsearch)
              {
                 Lsocketoutextraactive = 1;
                 Lsocketoutextragrapher = 1;
              }
#endif
           }
           if(keysym==UARROW && NDIM>0) /*121208*/
           {
              dobondrotgrapherNDIMsuitefit(); /*MAGEANGL 121208,130116*/
           }
        }
      }
    break;
  }
}
/*___keypresseventhandler()__________________________________________________*/

/****textcursor_CB()**********************************************************/
XtCallbackProc textcursor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  XmTextPosition   textplace;
  XmTextVerifyCallbackStruct  *cbs = 
                  (XmTextVerifyCallbackStruct  *)call_data;
    textplace = cbs->newInsert;
    /*this is the index into the text buffer, hypertext?? */

    DotextContentClick(textplace);
    return(NULL);
}
/*___textcursor_CB()_________________________________________________________*/

/****captcursor_CB()**********************************************************/
XtCallbackProc captcursor_CB(Widget w,XtPointer client_data,XtPointer call_data)
{
  XmTextPosition   captplace;
  XmTextVerifyCallbackStruct  *cbs = 
                  (XmTextVerifyCallbackStruct  *)call_data;
    captplace = cbs->newInsert;
    /*this is the index into the capt buffer, hypercapt?? */
    DocaptContentClick(captplace);
    return(NULL);
}
/*___captcursor_CB()_________________________________________________________*/

/****pick_CB()****************************************************************/
XtCallbackProc pick_CB(Widget drawArea,caddr_t client_data,
                       XmDrawingAreaCallbackStruct *call_data)
{
/*equivalent to MPCMAIN/WMLButtonDown(), MACMAIN/DografContentClick() */
/*static int oldx,oldy;*/
/*Window *root, *child;*/
/*int *root_x,*root_y,*child_x,child_y;*/
/*unsigned int keys_buttons;*/
/*XEvent *report;*/
/*int idelx,idely;*/

int Nblank = 0; /*000410*/

XButtonPressedEvent *bpress = (XButtonPressedEvent *) call_data->event;
/*XPointerMovedEvent *mousemove = (XPointerMovedEvent *) call_data->event;*/
    

  if (call_data->reason == XmCR_INPUT)
  {/*call_data->reason == XmCR_INPUT*/
    switch (call_data->event->type)
    {/*switch call_data->event->type*/
      case ButtonPress:
        if(Lmickeymouserun) 
        {
           Lpausemickeymouse=1;
            if(Lautorock)
            {/*reset current rotation matrix to current rocked state*/
               a11 = r11; a12 = r12; a13 = r13;
               a21 = r21; a22 = r22; a23 = r23;
               a31 = r31; a32 = r32; a33 = r33;
               Lnewrock = 1; /*autorock to restart smoothly from mouse setting*/
            }        
        }
        if (bpress->button == Button1)
        {/*Button1*/
          LmouseLdown = 1; /*so, for instance,  Levelofdetail could be invoked*/
          LmouseRdown = 0;
          
#ifdef UNIX_PEX
          doxpick(call_data->window,  bpress->x, bpress->y); /*MUXPHIG.c*/ 
#else /*NOT UNIX_PEX*/
              
          /*updateratelimit = 10.0; */
          /* updates per sec, default in MAGEINIT*/

          updaterate = updateratelimit;
          /*set rate so starts with a full update*/
          /*and thus can evaluate what the */
          /*full detail update rate really is */

          fullupdaterate = updaterate;
          ncountticks=theclocktickcount();/*___MAIN.C*/

          mousex = bpress->x;
          mousexold = bpress->x;
          mousey = bpress->y;
          mouseyold = bpress->y;

          if( mousey > (GHEIGHT/6) ) { irotxy = 1; }
          else { irotxy = 0; }

          if(Lflat)
          {/*flatland scrolling*/
             makecursorflatland();
             /*irotxy = 1;  030205 */
          }
          else
          {/*3D rotations*/
             if(irotxy==1) makecursorxyrot(); /*___MAIN.C*/ 
             else          makecursorzrot();  /*___MAIN.C*/ 
          }


          ipick = 2; /*flag for pick point recognition for button press*/
          pickx = mousex;
          picky = mousey;
          /*flag nothing picked yet */
          Lpick = 0; 
          Lpicknow = 0; 
          if(Ldraglineon || (Lnewlabelson&&!Ltestmode) /*991130*/
                         ||  (Lnewballson&&!Ltestmode)) /*031127*/
          {
              Lnewdrag = 1;
              Lnewmatrix = 1;
          }
          redrawvec();

          Lcarry = 1;
          totalrunticks = (theclocktickcount() - ncountticks);  /*960809*/

          if(totalrunticks != 0)
               updaterate = (float)60/(float)totalrunticks;
          else updaterate = 999;
          if(Ldetail==1) fullupdaterate = updaterate;
          while(Lpicknow) /*990211*/
          {/*while Lpicknow*/
             if(Ltablepickactive && Ltablegraphpicked) /*000327*/
             {/*find corresponding cell in table to graphics picked point*/
                getptIDstring(search1str, pickedpointptr);/*MAGELIST*/
                for(isearch1=0;isearch1<256;isearch1++)
                {
                   if(search1str[isearch1]==' ') Nblank++; /*000410*/
                   if(search1str[isearch1]=='\0') break;
                }/*isearch1 is size of pointID str*/  
                if(isearch1 == Nblank) isearch1 = 0; /*000410*/
                isearch2 = 0;
                if(isearch1 > 0)  /*000410*/
                {
                   searchtablecells(0); /*MAGETABL.c: 0 == match {ptID}*/
                   Ltablegraphpicked = 0;
                      /*avoid coordinate matching in drawtable()*/
                   redrawtable();/*000324*/
                   removetablemarkpoints();
                   markfromtablecellsID(1); 
                      /*MAGETABL.c flag==1 for ptID matching*/      
                }
             }/*find corresponding cell in table to graphics picked point*/
             Lpicknow = 0; 
             /* passes state between here and drawvec()*/
             if(Lsuperpunchon) ipick = 2; /*990211*/
             /*ipick==2 allows picking, so Lpicknow could become ==1 */
             if(Lpickcenteron && Lztran)/*Lmouse ztran w  Lpickcenteron 030709*/
             {/*pickcenter so rezero ztran*/
                 iztran = 0;
                 resetgrafztranBar(iztran);
             }
             redrawvec(); /*MACDRAW.C*/
                /*to move marker to picked atom if any near */
          }/*while Lpicknow*/

        /*if(Ltablepickactive && Ltablegraphpicked) redrawtable();*/ /*000324*/

          mouseticks=theclocktickcount();/*____MAIN.C*/
          /*set reference for timing mouse induced movements*/

#endif /*if---else NOT UNIX_PEX*/
          if(Lpick) 
          {/*a pick is currently active*/
             /*entered twice when last pick done for construction*/
             if(Lconstruct4on && Lpoint==4)
             {/*ACTIVE_DLOG filter protects dialog box creation*/
                DoconstructDialog(); /*MUX_DLOG.c*/
                /*this doesn't seem to need redrawvec() here ! */
                /*141126 needed it at end of construct_OK_CB for years! */
             }
             else if(Lconstruct5on && Lpoint==5)
             {
                DoConstructFifthDialog(); /*MAGEDLOG.c*/
             }
             else if(Lconstruct6on && Lpoint==7)
             {
                DoConstructSixthDialog(); /*140912 MAGEDLOG.c*/
             }
             if(Lpickcoloron) DocolorshowDialog(); /*___DLOG.C*/
             if(Lpickshowon)  DopickshowDialog();  /*___DLOG.C*/
             if(Lmovepointson)  DopickpointsDialog();  /*___DLOG.C 061129*/
             if(Lmeansigmason)  DopickpointsDialog();  /*___DLOG.C 061129*/
             if(Lsocketout)
             {
                Lsocketoutactive = 1;
                if(Lsocketidsearch)
                {
                   Lsocketoutidsearch = 1;
                }
                else if(Lpickcenteron) 
                {
                   Lsocketoutcenter = 1;
                }
                else
                {
                   Lsocketoutpick = 1;
                }
             }
          }/*a pick is currently active*/
          break;
        }/*Button1*/
        else if (bpress->button == Button3)
        {/*Button3 Right Button pickcenter*/
          mousex = bpress->x;    /*030405 any mouse button can reset these*/
          mousexold = bpress->x; /*030405 any mouse button can reset these*/
          mousey = bpress->y;    /*030405 any mouse button can reset these*/
          mouseyold = bpress->y; /*030405 any mouse button can reset these*/
          LmouseRdown = 1;
          LmouseLdown = 0;
          Lpickcenteron = 1; /*forced for Right Button 030205*/
          ipick = 2; /*flag for pick point recognition for button press*/
          pickx = bpress->x;
          picky = bpress->y;
          /*flag nothing picked yet */
          Lpick = 0; 
          Lpicknow = 0; 
          redrawvec();
          if(Lpicknow) /*did a pick! */
          {/*if Lpicknow*/
             Lpick = 1; /*consistent w PC, but is this needed????*/
             Lpicknow = 0; 
             if(Lztran && iztran != 0)  /*Rmouse==pickcenter so rezero ztran*/
             {
                iztran = 0;
                resetgrafztranBar(iztran);
                redrawvec(); 
             }
          }/*if Lpicknow*/
          if(Lpick) 
          {/*a pick is currently active*/
             if(Lsocketout && !Lsocketidsearch)
             {
                Lsocketoutactive = 1;
                Lsocketoutcenter = 1; /*since Lpickcenteron==1*/
             }
          }/*a pick is currently active*/
        }/*Button3 Right Button pickcenter*/
	     else
        {/*some other button pressed*/
           ;
#ifdef UNIX_PEX
           doxprepick(call_data->window,  bpress->x, bpress->y);/*MUXPHIG.c*/
#endif /*UNIX_PEX*/
        }
	break; /*from case ButtonPress */
	case ButtonRelease:
        if (bpress->button == Button1)
        {/*Button1*/
          makecursoractive();  /*___MAIN.C*/ 
          Lcarry = 0;
          if(LmouseLdown && Lmickeymouserun)
          {/*reset current rocked state to current rotation matrix*/
             r11 = a11; r12 = a12; r13 = a13;
             r21 = a21; r22 = a22; r23 = a23;
             r31 = a31; r32 = a32; r33 = a33;
             Lnewrock = 1; /*autorock to restart smoothly from mouse setting*/
          }
          LmouseLdown = 0; /*so Levelofdetail not be invoked*/
          if(Ldetail==0) 
          {
             redrawvec(); /*restore detail*/
          }
#ifdef UNIX_PEX
	       doxpickpredraw(); /*MUXPHIG.c*/ 
#endif /*UNIX_PEX*/
	       if(Lmickeymouserun) {Lpausemickeymouse=0;}
        }/*Button1*/
        if (bpress->button == Button3)
        {/*Button3*/
           LmouseRdown = 0;
        }/*Button3*/
	 break; /*from case ButtonRelease*/
      }/*switch call_data->event->type*/
   }/*call_data->reason == XmCR_INPUT*/
   return(NULL);
}
/*___pick_CB()_______________________________________________________________*/

/****fingerplacer()***********************************************************/
void fingerplacer(int irefresh)
{
   /*places finger in mage-screen where mouse cursor is in X-window*/
   /*put a finger on the local screen as ghost at remote cursor position*/
   /*first: copy the offscreen image to the computer screen*/
   /*if this is just a cursor motion without a redraw*/
   if(irefresh) {copytoscreen();} /*121108 MUXMDRAW.c def in MAGEhdr.h*/
   /*then: draw the finger image on top on it*/
   XDrawLine( dpy, drawWindow, gc ,mousex,mousey,mousex+20,mousey);
   XDrawLine( dpy, drawWindow, gc ,mousex,mousey,mousex+20,mousey+10);
   XDrawLine( dpy, drawWindow, gc ,mousex,mousey,mousex+20,mousey-10);
   Lfingerin = 0; /*fingerplacer called only when remote sends new fingerx,y*/
}
/*___fingerplacer()__________________________________________________________*/

/****motion_handler***********************************************************/
/*void motion_handler(Widget widget,caddr_t client_data,XEvent *event,Boolean *continue_to_dispatch)*/

void motion_handler(widget, client_data, event)
     Widget widget;                 
     caddr_t client_data; 
     register XPointerMovedEvent  *event;   /*Xlib.h*/  /*Vol 4, 218*/
  /* Boolean *continue_to_dispatch; */

/*equivalent to MPCMAIN/WMMouseMove(), MACDRAW/Drawgraf( internal loop ) */

{
int idelx,idely;
long temptimeticks=0;

  mousex = event->x; 
  mousey = event->y; 

  if(XtAppPending(app_context) == 0)
  {/*no pending events*/ /*so caught up to latest mouse movement*/
     if(!LmouseLdown)
     {/*e.g. ctrl-down blocks mousedown signel but allows mouseup signel*/
        mousexold = mousex;
        mouseyold = mousey;
        LmouseLdown = 1; /*should be only time when motion event possible*/
        LmouseRdown = 0;
       if( mousey > (GHEIGHT/6) ) { irotxy = 1; }
       else { irotxy = 0; }
     }
    idelx = (mousex - mousexold);
    idely = (mousey - mouseyold);
    if(idelx!=0 || idely!=0)
    {/*mouse has moved*/
      mousexold = mousex;
      mouseyold = mousey;
      pickx = mousex; /*dragline needs this updated*/
      picky = mousey;
      if(LNDparallelaxes) /*subvert mouse drag for paraAX space 061117*/
      {
         paraAXtranx = paraAXtranx + idelx;
         paraAXtrany = paraAXtrany + idely;
      }
      else
      {/*regular moves in 3D space*/
         /*labels can be dragged */  /*991130*/ /*but not in test mode*/
         /*balls can be dragged 031127 like labels*/
         if(Lpick && ( Ldraglineon || (Lnewlabelson&&!Ltestmode)
                                   ||  (Lnewballson&&!Ltestmode) ) )
         {
            Ldraglineactive=1;
         }
         /*if(Lpick&&Ldraglineon) Ldraglineactive = 1;*/ 
         if(!Ldraglineactive)
         {
             if(Lctrlkey) Lscroll = 1;
             else Lscroll = 0; /*Lscroll is independent of Lflat*/
             /*MAGEUTIL/getrot takes docking mode into account*/
             if(irotxy == 1 && Lshiftkey) /*==1 for initial decision*/
             {/*emphasize x or y : use >=1 for change emphasis while moving*/
                if(idely*idely < idelx*idelx)  irotxy = 2; 
                                 /*rotate around y axis*/
                else irotxy = 3; /*rotate around x axis*/
             }/*emphasize x or y */
             getrot(idelx,idely,0); /*MAGEUTIL.c*/
             Lnewmatrix = 1;
             if(Lsocketout && !Lsocketidsearch)
             {
                Lsocketoutactive = 1;
                if(Lflat || Lscroll) Lsocketouttran = 1;
                else Lsocketoutmatrix = 1;
             }
         }
         temptimeticks = (float)theclocktickcount();
         temptimeticks = temptimeticks - mouseticks;
         if(temptimeticks != 0)
         {
            mousetickrate =   (float)60/temptimeticks;
         }
         else
         {
            mousetickrate = 0;
         }
         mouseticks = theclocktickcount();  /*____MAIN.C*/ 

      }/*regular moves in 3D space*/
      redrawvec();
    }/*mouse has moved*/
  }/*no pending events*/
}
/*___motion_handler()________________________________________________________*/

/****Rmotion_handler**********************************************************/
/*void Rmotion_handler(Widget widget,caddr_t client_data,XEvent *event,Boolean *continue_to_dispatch)*/

void Rmotion_handler(widget, client_data, event)  /*030405*/
     Widget widget;                 
     caddr_t client_data; 
     register XPointerMovedEvent  *event;   /*Xlib.h*/  /*Vol 4, 218*/
  /* Boolean *continue_to_dispatch; */

/*equivalent to MPCMAIN/WMMouseMove(), MACDRAW/Drawgraf( internal loop ) */

{
int idelx=0,idely=0;

  mousex = event->x; 
  mousey = event->y; 

  Lpickcenteron = 0; /*forced off for Right Button */
  if(XtAppPending(app_context) == 0)
  {/*no pending events*/ /*so caught up to latest mouse movement*/
     if(!LmouseRdown)
     {/*e.g. ctrl-down blocks mousedown signel but allows mouseup signel*/
        mousexold = mousex;
        mouseyold = mousey;
        LmouseRdown = 1; /*should be only time when motion event possible*/
        LmouseLdown = 0;
     }
    idelx = (mousex - mousexold);
    idely = (mousey - mouseyold);
    if(LNDparallelaxes) /*subvert mouse drag for paraAX space 061117*/
    {
       paraAXzoom = paraAXzoom + idely;
       if(paraAXzoom < 0){paraAXzoom = 0;}
       redrawvec();
    }
    else
    {/*regular moves in 3D space*/
      if(idelx!=0 || idely!=0)
      {
         mousexold = mousex;
         mouseyold = mousey;
         if(LmouseRdown == 1)
         {/*new motion*/
            if(idely*idely < idelx*idelx)  
            {
               LmouseRdown = 2;
               resetzclip(idelx); /*MAGEANGL*/
            } 
            else 
            {
               LmouseRdown = 3;
               resetzoom(idely); /*MAGEANGL*/
            }  
         }
         else
         {/*only respond in a continuing direction*/
            if(LmouseRdown ==2){resetzclip(idelx);}
            else {resetzoom(idely);}
         }
         redrawvec();
      }
    }/*regular moves in 3D space*/

  }/*no pending events*/
}
/*___Rmotion_handler()_______________________________________________________*/

/****pointer_handler*********************************************************/
void pointer_handler(widget, client_data, event)
     Widget widget;                 
     caddr_t client_data; 
     register XMotionEvent  *event; /*Xlib.h*/  /*Vol 4, 218*/
{
  /*motion_handler may already have triggered at this mousex,y*/
  if((mousex != event->x) || (mousey != event->y))
  {
     mousex = event->x; 
     mousey = event->y; 
     if(Lsocketout && !Lsocketidsearch) 
     {
        Lsocketoutactive = 1;
        Lsocketoutfinger = 1;
     }
  }
}
/*___pointer_handler()______________________________________________________*/

/****condition()*************************************************************/
void    condition(char strg[256]){;}
/*___condition()____________________________________________________________*/
/****recondition()***********************************************************/
void    recondition(char strg[256]){;}
/*___recondition()__________________________________________________________*/

/****makecursoractive()******************************************************/
void    makecursoractive()
{
   if(Ltexteditable) 
   {
      if(Lflat)  magecursor = mageTE2Dcursor;
      else  magecursor = mageTE3Dcursor;
   }
   else 
   {
      if(Lflat) magecursor = mage2Dcursor;
       else magecursor = mage3Dcursor;
   }
   XDefineCursor(dpy,drawWindow,magecursor);
   Lcursoractive = 1;
}
/*___makecursoractive()_____________________________________________________*/

/****makecursorwait()********************************************************/
void    makecursorwait()
{
 XDefineCursor(dpy,drawWindow,waitcursor);
 Lcursoractive = 0;
/*XRaiseWindow(XtDisplay(topgraf),XtWindow(topgraf));*/
}
/*___makecursorwait()_______________________________________________________*/

/****makecursorxyrot()*******************************************************/
void    makecursorxyrot()
{
 XDefineCursor(dpy,drawWindow,magexy3Dcursor);
 Lcursoractive = 1;
/*XRaiseWindow(XtDisplay(topgraf),XtWindow(topgraf));*/
}
/*___makecursorxyrot()______________________________________________________*/

/****makecursorzrot()********************************************************/
void    makecursorzrot()
{
   XDefineCursor(dpy,drawWindow,magez3Dcursor);
   Lcursoractive = 1;
/*XRaiseWindow(XtDisplay(topgraf),XtWindow(topgraf));*/
}
/*___makecursorzrot()_______________________________________________________*/

/****makecursorflatland()****************************************************/
void    makecursorflatland()
{
   XDefineCursor(dpy,drawWindow,magexy2Dcursor);
   Lcursoractive = 1;
}
/*___makecursorflatland()___________________________________________________*/

/****theclocktickcount()********************950113*********/
long    theclocktickcount()
{
   int      iok;
   struct timeval timeofday;
   struct timezone zone; /*dummy in recent UNIXs*/
   long sec,usec,ticks;

   iok = gettimeofday(&timeofday,&zone);
   if(iok != -1)
   {
      sec = timeofday.tv_sec;
      usec = timeofday.tv_usec;
      /*avoid 2**31 when multiply seconds up to ticks (60th seconds) */
      /*seconds accummulate from Jan 1, 1970 */
      /*2**32 == 4,294,967,296  (unsigned) */
      /*2**31 == 2,147,483,648*/
      /*2,147,483,648/60 = 35,000,000 largest long able to be multiplied by 60*/
      /*least significant 1,000,000 safe to multiply by 60, */
      /* only 31 years, so this is an issue 020919*/

      if(sec > 35000000)
      {/*strip off most significant digits, leave the changing detail */
         sec = (sec - ((sec/1000000)*1000000));
      }
      sec = sec*60; /*integral seconds now in ticks*/
      /*convert microsecond part into ticks*/
      usec = ((usec*60)/1000000);
      ticks = sec + usec;
      return(ticks);
   }
   else
   {
       return(0L);
   }
}
/*___theclocktickcount()____________________________*/

/****countredraws()***********************************************************/
void    countredraws()
{
            int idelx,idely, Lnew;
            float    rate;
/*static        int xold,yold;*/

    Lnew = 1;
    Lcount = 0;
    ncountticks = theclocktickcount(); /*long*/
    icountticks = ncountticks; /*start with same number for both*/
        /*icountticks will be reset as get intermediates*/
    {
        /* not testing if mouse still down */
            if(Lnew)
            {/*establish mouse position at beginning of mouse down episode*/
                Lnew = 0;
                /*xold = 0;*/
                /*yold = 0;*/
            }
                /* simulate mouse has moved */
            {    /*begin-mouse has moved */
                /* get deltas and call for new rotation matrix */
                idelx = 0;
                idely = 0;
                getrot(idelx,idely,0); /*MAGEUTIL.C*/

                redrawvec(); /*MACDRAW.C*/

            }/*endof-mouse has moved */
    }
    postdrawticks = (theclocktickcount() - icountticks);
    icountticks = theclocktickcount();

    totalrunticks = (theclocktickcount() - ncountticks);
    /*rate = 1.0/( totalrunticks/60.0 ) ;*/
    if(totalrunticks > 0)
        rate = (float)60 / (float)totalrunticks  ;
    else
        rate = 999.;
        
         sprintf(alertstr,"timing in .01 sec *1,000,000: "
            CRLF"predraw = %ld, preloop = %ld"
            CRLF", firstloop = %ld, secondloop = %ld"
            CRLF", pastcopy = %ld, postdraw = %ld"
            ,predrawticks,preloopticks,firstloopticks,secondloopticks
            ,postcopyticks,postdrawticks
            );
         sprintf(alertstr2,
                " totalrun = %ld,                rate = %.3f per sec"
            CRLF"number of 2-D move-draws = %ld, out of %ld triples "
            CRLF" (text has %ld chars)"
            ,totalrunticks,rate
            ,nmovedraws,nxyz,ntotaltext
            );
         alertstr3[0]='\0';
         DoMageDLOGreport(1); /*alertstr,alertstr2,alertstr3*/ /*___DLOG.C*/

}
/*___countredraws()__________________________________________________________*/

/*
				Get_events

  This routine processes events requested by the program.  XtNextEvent
  obtains the next event from the input queue and places it in report.  The
  type of the event is then examined to determine if it is an extension
  event or a regular X event.  The constant LASTEvent (defined in X.h) is
  bigger than the event type of any X event and can be used to
  differentiate extension events from normal X events.

  Regular events are handled by the toolkit dispatch mechanism via
  XtDispatchEvent.  Extension events (DeviceMotion and picking events) are
  handled on a case-by-case basis.

  When a DeviceMotion event is encountered, all device motion events are
  removed from the event queue and accumulated into knob_totals, since the
  dials box reports relative changes.  Event explosion is a very real
  possibility since every device motion event requires 2 XEvent structures
  (only 6 axes' worth of data fit in a single XEvent) and the sample rate
  of the dials box is high.  Since this program is only concerned with
  cumulative changes in the dials values, it is safe to condense the device
  motion events via XCheckTypedEvent.  Since XCheckTypedEvent can remove
  events that are not at the head of the event queue, it may not be
  appropriate for situations where the semantics of a device motion event
  can be changed by another event (for instance, a key or button press).

  When a pick event is encountered, the picked structure element is identified
  and the corresponding list is searched for the nearest point to the picked
  coordinates.  If pick-centering is on, then the local MAGE center is updated
  and the display redrawn.  If markers are on, the markers are placed on the
  closest point to the pick-place.
*/

/****readsocketworkproc()*****************************************************/
Boolean readsocketworkproc(XtPointer client_data )
{
   syncsocketin(); /*MAGESYNC.c*/
   return(FALSE);
}
/*___readsocketworkproc()____________________________________________________*/

/****readsocketextraworkproc()************************************************/
Boolean readsocketextraworkproc(XtPointer client_data )
{
   syncsocketinextra(); /*MAGESYNC.c*/
   return(FALSE);
}
/*___readsocketextraworkproc()_______________________________________________*/

/****magetimerset()***********************************************************/
void    magetimerset(int isec)
{
  isec = isec;
}
/*____magetimerset()_________________________________________________________*/

