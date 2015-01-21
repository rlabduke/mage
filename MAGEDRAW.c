/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*mageDraw.c*/ /*121107 deleted \0, unneeded at end of C string sprintf()*/
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEFLAG.h"
#include "MAGEPOST.h"
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include "MAGEDLOG.h"
#include "MAGEFOO.h"
 
void processpick(void);
   /*int   actualpickx,actualpicky,actualpickz; moved to MAGELIST.h 061115*/
 static int   pickz;
 static int    Lwordstr = 0; /*must keep between calls to drawvec()*/
 static char   wordstr[256];
   int   iytop, iybottom, iyfontsize, /*iyhigh,*/ ideepcueold;
   int   ixmax,ixmin;
/*   int   izmin,izmax;           moved to MAGEANGL.h 080915 for pixelated*/
/*   int   izminlimit,izmaxlimit; moved to MAGEANGL.h 080915 for pixelated*/
#ifdef MOVEDTOMAGEH061116
   int   markdeep;
 static int    mark1deep=0,mark2deep=0;
#endif /*MOVEDTOMAGEH061116*/
   int   ixset,iyset,izset;
   float snapplaces;
   float distnew = 0; /*001014 EGAM*/ 
   int   Lpickinhibitedgroup=0,Lpickinhibitedlist=0; /*020908*/

/*pickedlistoldptr,pickedpointoldptr to MAGELIST.h : set NULL between kins*/
         
/****drawvec()***************************************************************/
void    drawvec()
{
  int    LLens, LforceMOVETO_P; /*970905*/ /*970912*/
  int    j,k,ideep,iwide,idecue,ioffset,igraystart,neyes,ilook,ideepcue;
  int    ideepcolor;
/*  int    maxdeep;   moved to MAGEANGL.h 080912 for pixelated triangles*/
/*  int    izadjust;  moved to MAGEANGL.h 080912 for pixelated triangles*/
/*  long   izbinsize; moved to MAGEANGL.h 080912 for pixelated triangles*/
  long   i11,i12,i13,i21,i22,i23,i31,i32,i33;
  double dist;    
  long   i; /* indexes == ncount, which is a long*/

  int    icolor,ithick,ipen,itcolor,itdepth;
  int    ifudgx,ifudgy,ifudgz,idelx,idely,idelz,itemp;
  int    misspick,itrypick,iradius; 
  long   pickhit, pickerr;
  double detmatrix; /*040430*/

  double sinang,cosang,fvalue;
  int    iedge,imiddle,xminleft,xmaxleft,xminright,xmaxright;
  static    int        Lexcessvec=0,Lsetexcess=0;
  struct pointstruct *jxyz0pointptr, *jxyz1pointptr, *jxyz2pointptr;
  int     postscriptcnt; 
  int     inewpoint; /*THIS DOES NOT SEEM TO BE USED*/
  int     xyzflag,noballs,ivalid; 
  int     newx,newy,newz; 

/*int  jx1orig=0, jy1orig=0, jz1orig=0;*/
  int  jx2orig=0, jy2orig=0, jz2orig=0;
  int  ixseting[3];
       /* j NOT used as an index into data structure, pointptr used instead*/
  
  double sinangle[3],cosangle[3];
  long   nowticks;

#ifdef MACINTOSH
  KeyMap   currentkeymap;
  char     *ptr;
#endif
 
  float  ballscale,fradius;
  int    jradius;
  int    pointcounter,jx0,jy0,jz0, Lpointsarein;
  int    type, penwidth, colordepth, oldpenwidth, oldcolordepth; 
  int    origpenwidth=0;
  int    refpickz;
  int    /*refiradius,*/pickle,refpickle;
  int    localfontsize;
  float  radiusofpick=0;
  float  delta,delx,dely,delz,tempx,tempy,radiusinflation=1;
  float  x2,y2,eyeposition;
  int    idelta;
#ifdef REMOVED030310
  int    ididpick = 0; /*EGAM 001014*/
#endif
  char   runstr[32],rstr[4],Rstr[4];
  int    numcolor=0;
  int    Lneedcolornumber=1,Laspectpass=0;
  static int ideepprevious; /*010913*/
/*  int    limitz=0;  moved to MAGEANGL.h 0800915 pixelated zlimit clip*/
  int    LNDnew = 1; /*for LNDparallelaxes 061115 */

  int ixoffset=0; /*121208 adjust center for screen-fixed NDIM param*/
  int iyoffset=0; /*121208 e.g. screen fixed angle param with*/
  int izoffset=0; /*121208 embedded 3D-coord adjustable angles suitefit model*/
  int ixyzoffset=0; /*121213 use same offset, x & y, from screen middle */
  int nbinout = 1; /*121211 offset for marks on balls, NDIM screen needs more*/
  int ymini = 5; /*121213 offset from screen top for parameter report*/

/*begin executable********/

/*printf("drawvec enter\n");*/

eyeposition = (float)ieyeposition; 
   /*needed by postscript or raster3D perspective*/
   /*needed by new algorithm perspective*/
if(Lsuperpunchon) killzoneonscreen();
if(LSELECTINGliston && !LoneSELECTINGon) SELECTINGzoneonscreen();
     /*060123,060207*/
if(LNDparallelaxes) {multipointparallelaxespre();} /*061115 paraAX screen rear*/
LLens = (int)(scalenew*lensFactor); /*Lens around center shows more detail*/
  /*LENSFLAG is a list attribute, lensFactor is a global parameter */
  /*@lens LensFactor is a keyword and parameter*/
LforceMOVETO_P = 0;
  
igraystart = numberofcolor("gray"); /* protection for stereo gray ctr line value*/
   /* not otherwise set if NO groups are on */
if(Levelofdetail) /*set local Ldetail to control this pass */
{                    
    if( (fullupdaterate < updateratelimit) && LmouseLdown) Ldetail = 0;
    else Ldetail = 1;
}
else Ldetail = 1;  /*always show detail*/

            snapplaces = 0.0;/*for no limit to precision of draglines*/
#ifdef MACINTOSH
            GetKeys(currentkeymap);       /*960213*/
            ptr = (char *)currentkeymap;
            if(1&ptr[7]) Lshiftkey = 1;
            else  Lshiftkey = 0;
            
            if(      4&ptr[2]) snapplaces = 1.0;   /* 1 */
            else if( 8&ptr[2]) snapplaces = 10.0;  /* 2 */
            else if(16&ptr[2]) snapplaces = 100.0; /* 3 */
            else if(32&ptr[2]) snapplaces = 1000.0;/* 4 */
#endif
#ifdef PCWINDOWS
      if(GetKeyState(VK_SHIFT) < 0) Lshiftkey = 1;
      else Lshiftkey = 0;
#endif

#ifdef REMOVEUSELESSFEATURE /*EGAM 001014*/
  if(Lshiftkey && !Ltablepicked)
  {   
     Ldetail = 1;  /*over-ride: always show detail*/
     if(ipick==2)
     {/*mouse induced scan for a pick*/
         /*shiftkey also allows screen x,y to be selected by a pseudo pick*/
         actualpickx = pickx; /*mouse position*/
         actualpicky = picky; /*mouse position*/
         actualpickz = 0;   /*arbitrary*/
         pickedpointptr = NULL; 
     }/*mouse induced scan for a pick*/
  }
#endif

  inewpoint = 0;/*check point number, helps avoid duplicates in PS output*/

  /* perspective option in integer arithmetic requires a bit of fudging: */
  /*without zooming scaling allows numbers to be at most +-280 around center*/
  /* but 280*280 = 56000 > 32768, the limit of signed regular integers */
  /* so work with halves in z:  280*140 = 28000, so dynamic halving done! */
  if(Lscalefonts) 
  {
     iyfontsize = 2*infofontsize;
     iytop =  2*infofontsize;/*mac sets text at left-bottom of character*/  
  }
  else
  {
     iyfontsize = infofontsize;
     iytop =  + infofontsize;/*mac sets text at left-bottom of character*/  
  }
  iybottom = 10 ;    
         /*offset from bottom of window*/
       /*Mac uses same metric for bitmap as for grafwindow*/
       /* infofontsize initially from MACWINX.C SetUpGrafPort() */

  /*depth queing by writing in bin batches, rear to front */   
  Nbins = 1000; /*make 1000 z buffer bins*/
  if(L5depths) {Ndepths = 5;}
  else {Ndepths = 3;} /*L3depths*/
  for(k=0;k<Nbins;k++)
  {
      thelink(0,k) = 0;
  }
  maxdeep = ((Nbins)-1);
  izadjust = izclip - izctr;
  izbinsize = (1000L*(2L*(long)izclip))/((long)Nbins);
  if(izbinsize<=0) izbinsize = 1;
  markdeep = 0; 
  izmin = izctr - izclip;
  izmax = izctr + izclip;
  izminlimit = izctr - izlimit;
  izmaxlimit = izctr + izlimit;

  /* establish integer array,  8192* for reasonable percision 2**13 */
  if(Lmickeymouserun && Lautorock && !Lpausemickeymouse && !Lmouseactive)
  {/*020208*/
    i11=(long)(8192*r11); i12=(long)(8192*r12); i13=(long)(8192*r13);
    i21=(long)(8192*r21); i22=(long)(8192*r22); i23=(long)(8192*r23);
    i31=(long)(8192*r31); i32=(long)(8192*r32); i33=(long)(8192*r33);
  }
  else
  {
    i11=(long)(8192*a11); i12=(long)(8192*a12); i13=(long)(8192*a13);
    i21=(long)(8192*a21); i22=(long)(8192*a22); i23=(long)(8192*a23);
    i31=(long)(8192*a31); i32=(long)(8192*a32); i33=(long)(8192*a33);
  }

  ncount = 0;  /* clear vector counter */
  LOK = 1;    /* used for sideclipping if in that mode */
  if(!Lzoom) zoom = 1.0;

  /*center offset for screen fixed parameter plots, choice introduced 121208 */ 
  /*different for -180 to 180  and 0 to 360 */ 
  /*re-entrant data: 0,0 at screen center vs at lower left */
  /*screen dimensions originally -200 to +200: -180 to 180 nested conveniently*/
  /*recently, scaled to actual screen width and height (z set to min of x, y */
  /*where coord are scaled 360/400 i.e. .9 width of smallest screen dimension*/
  /*oldmaxwide is smallest of GWIDTH, GHEIGHT */
  /*Usual 0,0,0 at screen center: GWIDTH/2 , GHEIGHT/2 , oldmaxwide/2 */

  /*THIS IS THE PLACE where NDIM screen-fixed coordinate 0,0,0 set in screen*/ 
  /*restricted to either -180,+180 or to 0,360 case !!!! 121212, 121213*/
  
  if(NDIM > 0 && dimensionptr && (dimensionptr[0].min > -179))    /*121208*/
  {  /*choice of any 2 or 3 of multi-dimensions sensible if all isotropic*/
     /* Note if min == -180 then reverts to previous ixctr,iyctr,izctr 121208*/
     /* for 0,360 offset equally from ctr by smallest dimension: */
     /* fuss engendered by 1990's Mage speed by using integer arithemtic*/

     ixyzoffset = (int)(((float)oldmaxwide/2.0)*(180.0/200.0)); /*121213*/
     ixoffset = ixctr - ixyzoffset;
     iyoffset = iyctr + ixyzoffset; /*y = -y*/
     /*izoffset = izctr - ixyzoffset;*/
     izoffset = izctr; /*force z=0 to mid-depth field, need zclip off 121210*/
  }
  else
  {/*NOT NDIM defaults to previous use of ixctr,iyctr,izctr  121208*/
     ixoffset = ixctr; 
     iyoffset = iyctr;
     izoffset = izctr;
  }

  ixset = ixctr; 
  iyset = iyctr;
  izset = izctr;

  ixseting[0] = ixctr; /*030121 x ctr for 2D screen objects*/ 
  ixseting[1] = ixset; /*931128 x offset by eye for postscript calc*/ 
  sinangle[1] = 0; /*931128 stereo offset by eye for postscript calc*/
  cosangle[1] = 1; /*931128 stereo offset by eye for postscript calc*/
  
  if(Lztran) {izset = (izctr + iztran);}
  
  iyset = iyset + iytran;
  /*x-coord adjustment (inc. ixtran) for left and right eye done later*/
  if(Lonewidth) ithick = 2;
  if(Lthin) ithick = 1;

  Lpickwhen = 1; /*always==1, check all points, take front most*/
      /*and will be readjusted when point picked */
      /*if(!Ldraglineon) Lpickwhen = !Lpick;*/
      /*(ipick==2 && Lpickwhen==!Lpick)      first, enters with Lpick==0*/  
      /*(ipick==2 && Lpickwhen==TRUE always)  last*/ 
 
  refpickz = -32000; /*dummy very far away */
  refpickle = 0;
     
  nowticks = theclocktickcount();    /*____MAIN.C*/
  preloopticks = (nowticks - icountticks);  
  icountticks = nowticks;  
  
  ixmin = 0; 
  ixmax = WIDTH;
  ymin = 0;
  ymax = HEIGHT;
  misspick = 100; /*was 25, 140915 set for the radial nerve challenged*/ 

  xmin = ixmin;
  xmax = ixmax;
  
  xminleft  = ixmin; /*defaults to pass to postscripter 031127*/
  xmaxleft  = ixmax;
  xminright = ixmin;
  xmaxright = ixmax;

  neyes = 1;
  if(Lstereo || Lcompareon) 
  {
      neyes = 2;
      iedge = 0;
      imiddle = 0;
      if(ieyewide < WIDTH/2) iedge = WIDTH/2 - ieyewide;
      else if(ieyewide > WIDTH/2) imiddle = ieyewide - WIDTH/2;
      if(LTBstereo)
      {
          ;
      }
      else
      {
        xminleft  = ixmin + iedge;  /*clipping inside visable window*/
        xmaxleft  = ( (ixmax+ixmin)/2 ) - imiddle;
        xminright = ( (ixmax+ixmin)/2 ) + imiddle;
        xmaxright = ixmax - iedge;
      }
  }

  if(Lmeasuredsurveillance) /*091212 */
  {/*surveillance*/
    dosurveillancemeasured(); /*091212*/ 
    if(surveillancestr[0] != '\0') /*distance,angle,dihedral*/
    {/*report surveillance to graphics screen*/
       if(Lcenterinfo)
       {   stringtoscreen(surveillancestr,(2*ixctr/3)
           ,(ymax-iybottom-(3*iyfontsize)));
       }
       else
       {
          if( iyfontsize <= 14)
          {  
             stringtoscreen(surveillancestr,(ixctr+5),(ymax-(3*iybottom)));
          }
          else    
          {
              stringtoscreen(surveillancestr,(ixmin+5)
              ,(ymax-iybottom-(3*iyfontsize)));
          }
       }
    }/*report surveillance to graphics screen*/
  }

  if(constructionstr[0] != '\0') /*e.g. AngleBetweenLines 140518*/
  {/*report constructionstr to graphics screen 140518*/
       if(Lcenterinfo)
       {   stringtoscreen(constructionstr,(2*ixctr/3)
           ,(ymax-(2*iybottom)-(6*iyfontsize)));
       }
       else
       {
          if( iyfontsize <= 14)
          {  
             stringtoscreen(constructionstr,(ixctr+5),(ymax-(6*iybottom)));
          }
          else    
          {
              stringtoscreen(constructionstr,(ixmin+5)
              ,(ymax-(2*iybottom)-(6*iyfontsize)));
          }
       }
  }/*report constructionstr to graphics screen 140518*/

  if(monitorparamstr[0] != '\0') /*140915 NucleicAcidParameters*/
  {/*report monitorparamstr to graphics screen 140915*/
       if(Lcenterinfo)
       {   
           //stringtoscreen(monitorparamstr,(2*ixctr/3)
           //,(ymax-(iybottom)-(3*iyfontsize)));
           stringtoscreen(monitorparamstr,(ixmin+5)
           ,(ymax-(iybottom)-(3*iyfontsize)));
       }
       else
       {
          if( iyfontsize <= 14)
          {  
             //stringtoscreen(monitorparamstr,(ixctr+5),(ymax-(3*iybottom)));
             stringtoscreen(monitorparamstr,(ixmin+5),(ymax-(3*iybottom)));
          }
          else    
          {
              stringtoscreen(monitorparamstr,(ixmin+5)
              ,(ymax-(iybottom)-(3*iyfontsize)));
          }
       }
  }/*report monitorparamstr to graphics screen 140915*/

  if(Lmonitoron && numberline>1 ) /*so there is a line in newgroup */
  {/*monitor last draw-new line length, overrides surveillance distance*/
    /*n = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT + numberline;*/
    /* i.e. n is the most recently drawn point */
    /*lastdrawnpointptr and prelastdrawnpointptr set in ANGLE.C*/
    /*drawnewlistptr->lastpointptr is most recent drawn point*/
    (drawnewlistptr->lastpointptr)->ix = lastdrawnpointptr->ix;
    (drawnewlistptr->lastpointptr)->iy = lastdrawnpointptr->iy;
    (drawnewlistptr->lastpointptr)->iz = lastdrawnpointptr->iz;
    (drawnewlistptr->lastpointptr)->colorwidth = 
                 lastdrawnpointptr->colorwidth;  /*alternate color*/
    (drawnewlistptr->lastpointptr)->fx = lastdrawnpointptr->fx;
    (drawnewlistptr->lastpointptr)->fy = lastdrawnpointptr->fy;
    (drawnewlistptr->lastpointptr)->fz = lastdrawnpointptr->fz;
        
    ((drawnewlistptr->lastpointptr)->previousptr)->ix = 
                                prelastdrawnpointptr->ix;
    ((drawnewlistptr->lastpointptr)->previousptr)->iy = 
                                prelastdrawnpointptr->iy;
    ((drawnewlistptr->lastpointptr)->previousptr)->iz = 
                                prelastdrawnpointptr->iz;
    ((drawnewlistptr->lastpointptr)->previousptr)->colorwidth = 
                                prelastdrawnpointptr->colorwidth;
    ((drawnewlistptr->lastpointptr)->previousptr)->fx = 
                                prelastdrawnpointptr->fx;
    ((drawnewlistptr->lastpointptr)->previousptr)->fy = 
                                prelastdrawnpointptr->fy;
    ((drawnewlistptr->lastpointptr)->previousptr)->fz = 
                                prelastdrawnpointptr->fz;
    dist= sqrt(  
               (  ( (drawnewlistptr->lastpointptr)->fx -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fx )
                * ( (drawnewlistptr->lastpointptr)->fx -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fx ))
              +(  ( (drawnewlistptr->lastpointptr)->fy -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fy )
                * ( (drawnewlistptr->lastpointptr)->fy -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fy ))
              +(  ( (drawnewlistptr->lastpointptr)->fz -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fz )
                * ( (drawnewlistptr->lastpointptr)->fz -
                   ((drawnewlistptr->lastpointptr)->previousptr)->fz ))
              );
    /*dist= sqrt((fxyz(0,n)-fxyz(0,n-1))*(fxyz(0,n)-fxyz(0,n-1)) +  
                 (fxyz(1,n)-fxyz(1,n-1))*(fxyz(1,n)-fxyz(1,n-1)) +
                 (fxyz(2,n)-fxyz(2,n-1))*(fxyz(2,n)-fxyz(2,n-1))  );
    */
    if(Lmeasureson)
    {
            if(Lmeasureperpendics) /*030930*/
            {
               sprintf(dstring,"%.3lf, ang=%.1f, dhr= %.1f"
                               ", line=%.3lf, plane=%.3lf"
                  ,distmeasure,anglemeasure,dihedralmeasure
                  ,perpendictoline,perpendictoplane);
            }
            else
            {
               sprintf(dstring,"%.3lf, ang=%.1f, dhr= %.1f",
               distmeasure,anglemeasure,dihedralmeasure);
            }
    }
    else    sprintf(dstring,"%.3lf",dist);
    /*sprintf(dstring,"%.3lf",dist);*/
    condition(dstring);
    /* Mx for Mac make it a Pascal string, PC EXTRA dummy*/
    if(Lcenterinfo)
    {   stringtoscreen(dstring,2*ixctr/3
       ,(ymax-iybottom-iyfontsize-iyfontsize/2));
    }
    else
    {
       if( iyfontsize <= 14)
       {    stringtoscreen(dstring,ixctr+5,(ymax-iybottom));/*Mx*/
       }
       else    
       {   /* stringtoscreen(dstring,ixmin+5,(ymax-iybottom-iyfontsize));*/
       stringtoscreen(dstring,ixmin+5
       ,(ymax-iybottom-iyfontsize-iyfontsize/2));
       }
    }
  }/*monitor last draw-new line length*/
  if(Lonscreeninfoon && !LNDparallelaxes)  /*061115 not show with parallels*/
  {/*060122 angle names for X,Y,Z  see L7dimensions*/
    stringtoscreen(onscreeninfostr,ixmin+5,ymin+5+iyfontsize);/*tight top left*/
  }

/*printf("------------------------------------------\n");*/
  /* DOUBLE PASS MOVE-DRAW, PRIMATIVE Z-BUFFERING */
  /*FIRST OUTER LOOP--__------___-------------------------------------------*/
  for(ilook = 1; ilook <= neyes; ilook++)
  {/*ilook pass*/
    if(ilook == 1 && neyes == 2)
    {/*ilook == 1 && neyes == 2*/
      if(LTBstereo)
      {/*Top/Bottom stereo*/
          /*TBYoffset moves image up in top field, top clipped by window*/
          /*clip part going down into transition field between images*/ 
          iyset = -TBYoffset + iyctr - HEIGHT/4; /*LEFT on Top*/
          ymin =  0;
          ymax = (HEIGHT/2) - 2*TBYoffset;
          if(Lstereo && LTBstereo)
          {/*scale by 1/2 in vertical dimension*/
              i12 = i12/2;
              i22 = i22/2;
              i32 = i32/2;
          }
      }/*Top/Bottom stereo*/
      else
      {/*Left/Right stereo or compare*/
        ixset = (ixctr - ieyewide/2);
        if(Lmiddle) 
        {
          xmin = xminleft; 
          xmax = xmaxleft;
        }
      }/*Left/Right stereo or compare*/
      ixseting[1] = ixset; /* x offset by eye for postscript calc*/ 
    }/*ilook == 1 && neyes == 2*/
    if(ilook == 2 )
    {/*ilook == 2*/
      if(!Lcompareon)
      {
         fvalue = (-stereoangle/360.0)*6.283;
         sinang = sin(fvalue); /*c*/ 
         cosang = cos(fvalue); /*c*/ 
         /* update: aij-matrix X y-matrix:              */
         /*    a11 a12 a13     cosang    0   -sinang    */
         /*    a21 a22 a23  X    0       1      0       */
         /*    a31 a32 a33     sinang    0    cosang    */
         if(Lautorock && Lmickeymouserun && !Lmouseactive) /*010609*/
         {
         i11 = (long)(8192*(r11*cosang + r13*sinang));
         i12 = (long)(8192*(r12));
         i13 = (long)(8192*(r11*(-sinang) + r13*cosang));
         i21 = (long)(8192*(r21*cosang + r23*sinang));
         i22 = (long)(8192*(r22));
         i23 = (long)(8192*(r21*(-sinang) + r23*cosang));
         i31 = (long)(8192*(r31*cosang + r33*sinang));
         i32 = (long)(8192*(r32));
         i33 = (long)(8192*(r31*(-sinang) + r33*cosang));
         }
         else
         {
         i11 = (long)(8192*(a11*cosang + a13*sinang));
         i12 = (long)(8192*(a12));
         i13 = (long)(8192*(a11*(-sinang) + a13*cosang));
         i21 = (long)(8192*(a21*cosang + a23*sinang));
         i22 = (long)(8192*(a22));
         i23 = (long)(8192*(a21*(-sinang) + a23*cosang));
         i31 = (long)(8192*(a31*cosang + a33*sinang));
         i32 = (long)(8192*(a32));
         i33 = (long)(8192*(a31*(-sinang) + a33*cosang));
         }
      }
      else
      {/*Lcompareon:021216*/
         sinang = 0; /* no stereo offset by eye for postscript calc*/
         cosang = 1; /* no stereo offset by eye for postscript calc*/
      }
      if(LTBstereo)
      {/*Top/Bottom stereo*/
         /*TBYoffset moves image down in lower field, bottom clipped at window*/
         /*clip part overlapping menubar in transition field between images*/ 

          iyset = +TBYoffset + iyctr + HEIGHT/4; /*RIGHT on Bottom*/
          ymin =  (HEIGHT/2) + 2*TBYoffset;
          ymax = HEIGHT;
          if(Lstereo && LTBstereo)
          {/*scale by 1/2 in vertical dimension*/
              i12 = i12/2;
              i22 = i22/2;
              i32 = i32/2;
          }
      }/*Top/Bottom stereo*/
      else
      {/*Left/Right stereo or compare*/
        ixset = (ixctr + ieyewide/2);
        if(Lmiddle) 
        {
          xmin = xminright; 
          xmax = xmaxright;
        }
      }/*Left/Right stereo or compare*/
      ixseting[2] = ixset; /*931128 x offset by eye for postscript calc*/ 
      sinangle[2] = sinang; /*931128 stereo offset by eye for postscript calc*/
      cosangle[2] = cosang; /*931128 stereo offset by eye for postscript calc*/
    }/*ilook == 2*/

    /*just x-coord adjustment for left and right eye */
    ixset = ixset + ixtran;

    thisgrupptr = firstgrupptr;
    while(thisgrupptr != NULL)
    {/*begin-loop-over-groups*/
      if(  ((thisgrupptr->type & TABLEFLAG) == 0)
      &&(   (thisgrupptr->on > 0 && !Lcompareon)
         || (Lcompareon && ilook==1 && thisgrupptr->on>0  )
            /*first pass put left everything that is ON */
            
         || (Lcompareon && ilook==2 && thisgrupptr->on>0  
                        && thisgrupptr->animate==0)
            /*second pass put right, all non-animate groups that are ON */
         || (Lcompareon && ilook==2 && thisgrupptr->on==0 
                        && thisgrupptr == nextanimategrupptr) ) )
            /* and the OFF animate group that is flagged as being next */
      {/*begin-visible-group*/
        Lpickinhibitedgroup = 0; /*allow picking in this group*/
        if(Lpickeraseon && (thisgrupptr != drawgrupptr) )Lpickinhibitedgroup=1;
        noballs = 1; /*presume true, scope is just a group*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*begin-loop-over-sets*/

          if(thissgrpptr->on > 0)
          {/*begin-visible-set*/
            thislistptr = thissgrpptr->firstlistptr;
            
            if(!Ldetail && (((thislistptr->type) & DETAILFLAG) ==1) ) 
              thislistptr = NULL; /*don't do "detail" list*/
            while(thislistptr != NULL)
            {/*begin-loop-over-lists*/  

              if(thislistptr->on > 0)
              {/*begin-visible-list*/ 
                Lpickinhibitedlist = 0; /*allow picking in this list*/
                thispointptr = thislistptr->firstpointptr;
                if(thislistptr->STATUS & ZLIMITED) {limitz = 1;}
                else {limitz = 0;}
                if( (  Lmeasureson 
                     &&((thislistptr->STATUS)&STATICFLAG)==STATICFLAG) )
                   {Lpickinhibitedlist = 1;}
                pointcounter = 0;/*start fresh for each list*/
                while(thispointptr != NULL)
                {/*begin-draw-loop-of-points*/ 
  /****FIRST INNER LOOP*****************************************************/
  ivalid = 0;
  xyzflag =  thispointptr->type;

  if( (xyzflag & PRUNED) == 0)
  {/*NOT PRUNED*/
     if( (thispointptr->STATUS & DUMMY) != DUMMY)
     {/*NOT DUMMY*/
        if(thispointptr->STATUS & ON)
        {/*point is ON*/  /*991210*/
           ivalid = 1;
           if(xyzflag & BALL)  /*ball flag on point is dominant*/ /*980804*/
                     ivalid = 1;       /*980804*/
           /*if(noballs)*/ /*no previous balls in this group displayed*/ /*980804*/
           /*but presence of a point ball kills noballs flag !! */
           else if(noballs) /*no previous balls in this group displayed*/  /*980804*/
           {           /*don't shorten vectors unless forced*/
             /*check if point forces shortened vector*/
             /* e.g. if preparing for a later ball or sphere*/
             if(   xyzflag & BALLHERE )
             {
               if( xyzflag & STORED ) 
               {
                     ivalid = 0;/*Not display*/
               }
             }
             else /*NOT BALLHERE*/
             {   
               if( xyzflag & INVENTED)
               {
                   ivalid = 0;/*Not display*/ 
               }
             }
           }
           else /*The effective default case*/
           {/*balls detected earlier in this group*/
               /*use shortened vectors if they have been defined*/
               if(xyzflag & STORED) ivalid = 0;/*Not display*/ 
               /*Avoid the stored, unshorted-vector points*/
           }
        }/*point is ON*/  /*991210*/
        else
        {
           ;
        }
     }/*NOT DUMMY*/
  }/*NOT PRUNED*/
         
  if(ivalid && LLenson && (thislistptr->type & LENSFLAG) != 0)/*971122*/
  {
    if(   thispointptr->ix < -LLens || thispointptr->ix > +LLens
       || thispointptr->iy < -LLens || thispointptr->iy > +LLens
       || thispointptr->iz < -LLens || thispointptr->iz > +LLens )
    {/*outside lens show region*/
      ivalid = 0; 
      LforceMOVETO_P = 1; /*970912*/ /*next point must be new start*/
    }
    else /*within lens region*/
    {
      if(LforceMOVETO_P) xyzflag = xyzflag | MOVETO_P;
      LforceMOVETO_P = 0; /*970912*/
    }
  }
         
  if(ivalid)
  {/*point currently valid*/
    if(!Lsetexcess)
    {/*sufficient display room for this point*/
      /*ctr of rot moved to 0,0,0 by subtracting xctr,yctr,zctr*/
      /* before storing coordinates.  Restore ctr of rot by */
      /* adding xctr,yctr,zctr adjusted for scaling*/
      /* and any translations after multiply step*/
      /*For efficiency: color sets, not individual vectors */
      /*Matrix components integers 8192 larger than aij matrix*/
      /*            i11 i12 i13 */
      /*            i21 i22 i23 */
      /*            i31 i32 i33 */
      if(  ((thislistptr->STATUS)  & STATICFLAG)==0
         &&((thispointptr->STATUS) & STATICFLAG)==0)
      {
                      jx2 = ( (thispointptr->ix*i11 + thispointptr->iy*i21 
                             + thispointptr->iz*i31)>>13) +ixset;
                      jy2 = ( (thispointptr->ix*i12 + thispointptr->iy*i22 
                             + thispointptr->iz*i32)>>13) +iyset;
                      jz2 = ( (thispointptr->ix*i13 + thispointptr->iy*i23 
                             + thispointptr->iz*i33)>>13) +izset;
                      /*shift by 13 bits rather than divide by 8192 */
                      /*ixset has scrolling offset and screen indexing built */
                      /* in. thispointptr->ix,iy,iz  holds centered, scaled */
                      /* x,y,z coords.  This saves 3 additions for centering */
                      /* and 3 multiplies for scaling , but centering */
                      /* and scaling then require scanning through objects */
      }
      else if(  ((thislistptr->STATUS)  & SCREENFLAG)==0
              &&((thispointptr->STATUS) & SCREENFLAG)==0)
      {
                      jx2 =  thispointptr->ix + ixset;
                      jy2 = -thispointptr->iy + iyset;
                      jz2 =  thispointptr->iz + izset;
      }
      else
      {/*SCREEN stuff done crt screen even in stereo: uses the whole screen*/
       /*and cannot be translated either*/
                    /*jx2 =  thispointptr->ix + ixctr;*/
                    /*jy2 = -thispointptr->iy + iyctr;*/ /*beware TBstereo*/
                    /*jz2 =  thispointptr->iz + izctr;*/
                  jx2 =  thispointptr->ix + ixoffset; /*121208*/
                  jy2 = -thispointptr->iy + iyoffset; /*121208 beware TBstereo*/
                  jz2 =  thispointptr->iz + izoffset; /*121208*/
      }
      jxyz2pointptr = thispointptr;
      /*jxyz2 = j;*/ /*PS associate index with these coords*/
      /*980610 should be able to delete jxyz2 = j;*/
      if(Lperspec)
      {/*perspective*/
         if(Lfloat != 0)
         {/*different algorithm calculation*/
            if(Lfloat == 1)
            {/*floating point perspective calculation*/
#ifdef OLDALGOR
                /*the ifudge mickey mouse stuff was to avoid*/
                /*exceeding the size of an integer*/
                delz = (float)jz2/2 - (float)izset/2;
                delx = (float)jx2   - (float)ixset;
                tempx = (delx*delz)/(1000);
                x2 = (tempx + (float)jx2);
                dely = (float)jy2   - (float)iyset;
                tempy = (dely*delz)/(1000);
                y2 = (tempy + (float)jy2);
#endif
                /*the ifudge mickey mouse stuff was to avoid*/
                /*exceeding the size of an integer*/
                
                /*delx,dely,delz: displacements from center, */
                /*ctr plane == screen plane*/
                /*V: View distance from center, */
                /* so V is the eye point of the perspective projection*/
                /*similar triangles: delx/(V-delz) = extrax/delz*/
                /* extrax = delx*delz/(V-delz)*/
                /* 0----.--x :delx + extrax */
                /*        /   */
                /* z    x/   :delx at delz from ctr plane, (V-delz) from V*/
                /*      /     */
                /*     /      */
                /*    /       */
                /*   /        */
                /*  /         */
                /* V         :View point of perspective projection*/
                /* this gets real big as scope of z approaches V */
                /*which crashes size of integer */
                /* when that approaches size of graphics area, e.g. big zoom*/
                /*V=2000 approximates old projection at z near 0, */
                /*but wider at larger z */
                /* Near lines at high mag approach V and get >32767 problems*/
                /*V=3000: compromise value with .LT. old perspective at low z*/
                /*though still .GT. old perspective at high z*/
                /*old approximation was: extrax = delx*delz/V, same at delz==0*/
                /* old one shows bent lines as scope in z increases*/
                /*i.e. xy-z to xy0 to xy+z not in line perspective projection*/
                /*delz = (float)jz2/2 - (float)izset/2; */
                      /*in effect, uses 1000 instead of 2000 for V*/
                
                delz = (float)jz2 - (float)izset;
                if( delz > (eyeposition - 50))  delta = 50;
                else delta = eyeposition - delz; 
                    /*big neg delz just makes less offset*/
                
                delx = (float)jx2   - (float)ixset;
                tempx = (delx*delz)/(delta);
                /*if(tempx-jx2 > 32767  || tempx-jx2 < -32767) */
                      /*tempx = 32768 - jx2;*/
                x2 = (tempx + (float)jx2);
                
                dely = (float)jy2   - (float)iyset;
                tempy = (dely*delz)/(delta);
                /*if(tempy-jy2 > 32767  || tempy-jy2 < -32767) */
                       /*tempy = 32768 - jy2;*/
                y2 = (tempy + (float)jy2);

/*printf("x,y,z: %.0f,%.0f,%.0f",x2,y2,delz  );*/
                if(x2 > 32766) x2 = 32766;
                else if(x2 < -32766) x2 = -32766;

                if(y2 > 32766) y2 = 32766;
                else if(y2 < -32766) y2 = -32766;
                
                jx2=(int)x2;
                jy2=(int)y2;
                
                if( xyzflag & BALL ) /*balllist or spherelist*/
                {/*first cut, simple increase to ball radius*/
                   radiusinflation = delz/delta;
                }
            }/*floating point perspective calculation*/
            else
            {/*integer calculation with original algorithim*/
                      ifudgz = 1;
                      idelz = ( (jz2/2) - izctr/2 );
                      
                      while(idelz > 181 || idelz < -181)
                      {
                        idelz = idelz/2;
                        ifudgz = ifudgz*2;
                      }
                      ifudgx = 1;
                      idelx = jx2 - ixset;
                      while(idelx > 181 || idelx < -181)
                      {
                        idelx = idelx/2;
                        ifudgx = ifudgx*2;
                      }
                      itemp = ((idelx*idelz)/1000);
                      jx2 = itemp*ifudgx*ifudgz + jx2;
                      ifudgy = 1;
                      idely = jy2 - iyset;
                      while(idely > 181 || idely < -181)
                      {
                        idely = idely/2;
                        ifudgy = ifudgy*2;
                      }
                      itemp = ((idely*idelz)/1000);
                      jy2 = itemp*ifudgy*ifudgz + jy2;
            }/*integer calculation with original algorithim*/
         }/*different algorithm calculation*/
         else
         {/*integer with specified eye position, NEW algorithm*/
                      ifudgz = 1;
                      /*idelz = ( (jz2/2) - izctr/2 );*/
                      idelz = ( (jz2) - izctr );
                      if(idelz > (ieyeposition - 50)) idelta = 50;
                      else idelta = ieyeposition - idelz;
                      
                      while(idelz > 181 || idelz < -181)
                      {
                        idelz = idelz/2;
                        ifudgz = ifudgz*2;
                      }
                      ifudgx = 1;
                      idelx = jx2 - ixset;
                      while(idelx > 181 || idelx < -181)
                      {
                        idelx = idelx/2;
                        ifudgx = ifudgx*2;
                      }
                      /*itemp = ((idelx*idelz)/1000);*/
                      itemp = ((idelx*idelz)/idelta);
                      jx2 = itemp*ifudgx*ifudgz + jx2;
                      ifudgy = 1;
                      idely = jy2 - iyset;
                      while(idely > 181 || idely < -181)
                      {
                        idely = idely/2;
                        ifudgy = ifudgy*2;
                      }
                      /*itemp = ((idely*idelz)/1000);*/
                      itemp = ((idely*idelz)/idelta);
                      jy2 = itemp*ifudgy*ifudgz + jy2;
         }/*integer with specified eye position, NEW algorithm*/
      }/*perspective*/
      jx2orig = jx2; /* save to set up for next draw since */
      jy2orig = jy2; /* there may be no intervening move */
      jz2orig = jz2; /* and jx2,jy2,jz2 could get clipped */

      /*(ipick==2 && Lpickwhen==!Lpick)      first*/  
      /*(ipick==2 && Lpickwhen==TRUE always)  last*/  
      /*if(ipick==2 && Lpickwhen && !Lshiftkey)*/
      /*shift for screen position pseudo picking */
      /*imbed tests to minimize tests for continued rotation*/
      if(ipick==2 && ACTIVE_DLOG==0 && !LNDparallelaxes) /*011110,061115*/
      {/*pick requested*/
        /*if(Lpickwhen && !Lshiftkey )*/
        if(Lpickwhen ) /*Lpickwhen never set false????*/
        {/*still looking*/
          if(!Lpickinhibitedgroup && !Lpickinhibitedlist)
          {/*NOT Linhibitpicking for group,subgroup,list,...*/
            if( xyzflag & BALL )
            {/*balllist or spherelist*/
              /*if(thispointptr->radius < .001)*/
              if(Listradiusdominant || thispointptr->radius < .001) /*040402*/
              {
                if(thislistptr->radius  < .001)
                   {radiusofpick = scalenew*(float)0.2;}
                else {radiusofpick = (scalenew*(thislistptr->radius));}
              }
              else
              {/*has point radius*/
                 radiusofpick = (scalenew*(thispointptr->radius));
              }/*has point radius*/
              pickerr = (long)(radiusofpick*radiusofpick);
              iradius = (int)radiusofpick;
              /*so pickerr can get very big for a sphere*/
              /*since it is a square to avoid taking sqrt */
              /*since calculation is the square of the distance*/
              if(pickerr < misspick) {pickerr=misspick;}
              /*so retain viable circle of confusion*/
            }/*balllist or spherelist*/
            else
            {
               radiusofpick = 0;
               iradius = 0;
               pickerr = misspick;
            }
            if(Lsuperpunchon) /*990211  auger uses killradius*/
            {
               pickerr = killradius*killradius;
               pickx = mousexold;
               picky = mouseyold;
            }
            if(   (long)(jx2-pickx)*(long)(jx2-pickx)
                + (long)(jy2-picky)*(long)(jy2-picky)
                < pickerr)
            {/*close enough to a point*/
              itrypick = 1;
              if(!Lsuperpick)
              {
                 if( xyzflag & UNPICKABLE ) {itrypick =  0;}
                 /*inc stored atoms of shorted lines UNPICKABLE !?*/
              }
              if(Lzclipon)
              {/*do not allow a pick on a point that will be clipped out*/
                 if(thislistptr->STATUS & ZLIMITED )
                 {
                    if(jz2 <izminlimit||jz2 >izmaxlimit) {itrypick=0;} 
                 }
                 else
                 {
                    if(jz2 <izmin||jz2 >izmax) {itrypick=0;}
                 }
              }
              if(Lstereo  &&(jx2 > xmax||jx2 < xmin)) {itrypick=0;}

              if(itrypick)
              {/*declare a pick*/
                Lpick = 1;
                Lpicknow = 1;
                /*if(!Ldraglineon) Lpickwhen = !Lpick;*/ /*960809*/
                /*if(jz2 >= refpickz) */
                pickhit =   (long)(jx2-pickx)*(long)(jx2-pickx)
                          + (long)(jy2-picky)*(long)(jy2-picky);
                if(pickhit>0) {pickhit = (long)sqrt((float)pickhit);}
                else {pickhit = 0;}
                /*linear rather than spherical pickhit correction*/
                if(pickhit < iradius) {pickle = iradius-pickhit;}
                else {pickle = 0;}
                if(   (jz2+pickle > refpickz+refpickle       )
                   || (  (jz2+pickle == refpickz+refpickle)
                       &&(thisgrupptr->type & TABLEFLAG   )  )  )
                /*970101 always take what would be the front one*/
                /*but take first of those at same z (refpickz */
                /*fudged for spheres), since check all, should*/
                /*get dragline end if it is in front even though*/
                /*it is the last drawn thing.*/

                {/*closer to front, replace with newer pick*/
                  temppickedgrupptr  = thisgrupptr;
                  temppickedsgrpptr  = thissgrpptr;
                  temppickedlistptr  = thislistptr;
                  temppickedpointptr = thispointptr;
                     
                  /*reset pickx,y after final pick*/
                  /*later make exact so can use as picked coord*/
                  /*pickx = jx2;*/
                  /*picky = jy2;*/
                  /*the final transformed x,y,z will be:*/
                  /*used after 2nd loop when doing pick stuff*/

                  actualpickx = jx2;
                  actualpicky = jy2;
                  actualpickz = jz2;
                  if((Lpickmarkcriteria & 128) == 128)
                  {/*behind everything else at marker's screen depth*/
                     markdeep = -1; /*000615*/
                  }
                  else
                  {/*markers show in front of everything at this depth*/
                     if((temppickedlistptr->STATUS & SCREENFLAG) && NDIM)
                         { nbinout = 3;} /*121211 NDIM balls need more????*/ 
                     else{ nbinout = 1;}
                     markdeep = ((1000L*(long)iradius)/izbinsize) +nbinout;
                          /*970920 and 121211*/
/*
printf("markdeep==%d== ((1000L*(long)iradius) %ld / izbinsize %ld)  +1\n"
,markdeep,(1000L*(long)iradius),izbinsize);
*/
                     /*iradius already has scalenew built into it*/
                     /*markdeep = ((1000*(scalenew*iradius))/izbinsize) +1;*/
                     /*so marker drawn above sphere surface at any zoom*/
                     /*izbinsize is 1000* to avoid integer problems*/
                     /*970101 move marker out one bin further*/
                  }
                  refpickz   = jz2;
                  /*refiradius = iradius;*/
                  /*refpickhit = pickhit;*/
                  refpickle  = pickle;
                  /*offset for markers on balls, if any*/
                }/*closer to front, replace with newer pick*/
              }/*declare a pick*/
            }/*close enough to a point*/
          }/*NOT Linhibitpicking for group,subgroup,list,...*/
        }/*still looking*/
      }/*pick requested*/                      
      if(!Ldetail)
      {/*change xyzflag to make simpler representation*/
                      xyzflag = (xyzflag & ANTIVARIANT);
                /*both TRIANGLE and RIBBON revert to VECTOR*/  
                  /*SPHERE -> BALL*/
      }
      /*if( (xyzflag & VECTOR) == 0 )*/
      /*if( (xyzflag & VECTOR) == 0 || (xyzflag & BALL) )*/ /*980804 special ball*/
      if(    (xyzflag & VECTOR) == 0 
          || (xyzflag & BALL) 
          || (xyzflag & MARKTYPE) 
        )
      {/*DOT,BLOT,BALL,SPHERE,LABEL,WORDS*/
              /*I.E. EVERYTHING that comes to fruition on one pt*/
            /*one point DOT,LABEL: flesh out with dummy first point*/
            jx1 = jx2;
            jy1 = jy2;
            jz1 = jz2;

            /*jxyz1 = jxyz2;*/ /*931128PS index for first point*/
            /*980610 should be able to delete jxyz1 = jxyz2;*/
            jxyz1pointptr = jxyz2pointptr;
            Lpointsarein = 1;
      }/*DOT,BLOT,BALL,SPHERE,LABEL,WORDS*/
      else /*(xyzflag & VECTOR)*/ 
      {/*only those thus flagged have more than one point*/
         /*so need to know if sufficient points are in*/
                      pointcounter++;
                      /*STARTFLAG: beginnings of strips of triangles, ribbons*/
                      /*MOVETO_P only used if levelofdetail reverts to vectors*/
                      if(thispointptr->STATUS & STARTFLAG)
                      {/*only first point so: */
                          pointcounter = 1;
                          Lpointsarein = 0;
                      }
                      else if( (xyzflag & VARIANT1) )
                      {/*TRIANGLE or RIBBON*/
                          if(pointcounter>=3)
                          {/*three or more points of a triangle list*/
                             Lpointsarein = 1;
                          }
                          else if(pointcounter==2)
                          {/*special case of first two point in triangle list*/
                            Lpointsarein = 0;  /*only two*/
                            checksideclip(xyzflag);
                            if(   Lzclipon 
                               && LOK == 1 
                               && (thislistptr->type & NOZCLIPFLAG)==0  )
                            {/* pass altered coord values*/
                             checkzclip(jx1,jy1,jz1,jx2,jy2,jz2,xyzflag,limitz);
                            }
                            if(!LOK) pointcounter = 0; /*reject point*/
                                /*in effect, reject this edge of the triangle*/
                          }
                          else
                          {/*only one point in*/
                             Lpointsarein = 0;
                          }
                      }
                      else if( (xyzflag & MOVETO_P) )
                      {/*just a simple vector, check for "P" first point*/

                          Lpointsarein = 0;
                      }
                      else 
                      {/*just a simple vector, must have two points already*/
                          Lpointsarein = 1;
                      }
      }/*VECTOR or derived variant*/
      if(Lpointsarein)
      {/*Lpointsarein:DISPLAY: draw-vector dot ball label word*/
        /* depth cueing: initially 400 deep (jz1=0-400), */
        /*scaled so diagonals  will fit,     */
        /*divide into exactly 5 bins or Nbins/5 */
        /*from above: izadjust = izclip - izctr;*/
        /*from above: izbinsize = 2*izclip/(Nbins);*/
        /*use average so balls==disks at correct relative depth*/

        /*if( xyzflag & VECTOR )*/
        /*if( (xyzflag & VECTOR) && !(xyzflag & BALL))*/ /*980804 special ball*/
        if(    (xyzflag & VECTOR) 
           && !(xyzflag & BALL)
           && !(xyzflag & MARKTYPE)
          )
        {
           if( xyzflag & VARIANT1) /*average last 3 points*/
           {
               ideep=(1000L*(long)( ((jz0+jz1+jz2)/3)+izadjust))/izbinsize;
           }
           else /*plain vector of two points*/
/*fprintf(stderr,"thislistptr->STYLE == %d\n",thislistptr->STYLE);*/
           if(thislistptr->STYLE & REARSTYLE) /*090703 for black outline*/
           {/* take rear most point as ideep*/
              if(jz2 > jz1) 
{ ideep=(1000L*(long)(jz1 + izadjust))/izbinsize;
/*fprintf(stderr,"rear point at 1 %d\n",jz1);*/
}
              else          
{ ideep=(1000L*(long)(jz2 + izadjust))/izbinsize;
/*fprintf(stderr,"rear point at 2 %d\n",jz2);*/
}
              /*and push it even further rearward...*/
              ideep = ideep -2;  /*ideep bounds protected below*/
           }
           else if(thislistptr->STYLE & FORESTYLE) /*090703 infront of black*/
           {/* take rear most point as ideep*/
              if(jz2 > jz1) 
{ ideep=(1000L*(long)(jz2 + izadjust))/izbinsize;
/*fprintf(stderr,"fore point at 2 %d\n",jz2);*/
}
              else          
{ ideep=(1000L*(long)(jz1 + izadjust))/izbinsize;
/*fprintf(stderr,"fore point at 1 %d\n",jz1);*/
}
           }
           else
           {  /*classically, average the points, i.e. bin at midpoint*/
              ideep=(1000L*(long)( ((jz1+jz2)/2) + izadjust))/izbinsize;
           }
           
            /*note for node-point: optimize line-into-sphere effect 060622*/
            /*if(jz1<jz2){ideep= 1000L*(long)(jz1+radius+izadjust)/izbinsize;}*/
            /*else       {ideep= 1000L*(long)(jz2+radius+izadjust)/izbinsize;}*/
           
        }
        else
        {/*only point "2" is real*/
           ideep=(1000L*(long)( (jz2) + izadjust))/izbinsize;
        }
        /*izbinsize is 1000* to avoid integer problems*/
                      
        if     (thislistptr==marker1listptr)
        {
           ideep=ideep+mark1deep;
        }
        else if(thislistptr==marker2listptr)
        {
           ideep=ideep+mark2deep;
        }
        if( (xyzflag & LABEL)&&(xyzflag & VARIANT1) )
        {/*Label list on a sphere, needs radius to be visible*/
             /*this is a special type of labellist with a radius!!!!*/
             /*NOTE: when previewing raster3d stroked font*/
             /* ideep not honored in raster3d output, so is misleading*/
             if(thislistptr->radius  < .001)
             {
                iradius = (int)(scalenew*.2);
             }
             else
             {
                iradius = (int)(scalenew*(thislistptr->radius));
             }
             ideep = ideep+((1000*(long)iradius)/izbinsize);
        }/*Label list on a sphere, needs radius to be visible*/

        /*Then make sure ideep is in bounds !!!! */
        if(ideep <= 0) ideep = 0;
        /*from above: maxdeep = ((Nbins)-1); */
        if(ideep >  maxdeep ) ideep = maxdeep;

        LOK = 1; /*flag for passing clip checks */
        /*jx1,jy1,jx2,jy2 are changed at edges to truncate line*/
        /*before doing this clipping save originals that might*/
        /* be used to make a vector with a later point */
        jx2orig = jx2; /* save to set up for next draw since */
        jy2orig = jy2; /* there may be no intervening move */
        jz2orig = jz2;
        /*991118 decided to do sideclipping only when doing stereo where */
        /*the center line must be a clean cut,  otherwise, let the OS do */
        /*the clipping at the pixel writing stage - the cleaner way to do it*/
        /*providing the OS does not crash if the drawing operation is */
        /*outside the draw Area.  This used to crash the Mac in the early 90s*/
        /*but seems to be robust on all platforms these days. */
        /*So the saved integer points are the clipped ones which might confuse*/
        /*the depth cueing but otherwise needed to stay within bounds*/
        /*NOTE do clipping at stage where have 2 points even for triangles*/
        /* done as special case above where checking for Lpointsarein */
        
        if(Lclipmiddle){ checkmiddleclip(xyzflag,limitz);} /*051214*/
           /*sets global LOK */
        
        if(LOK){checksideclip(xyzflag);} /*always do this: relatively cheap*/
      
        if(Lzclipon && LOK == 1 && (thislistptr->type & NOZCLIPFLAG) == 0)
        {/* pass altered coord values*/
          checkzclip(jx1,jy1,jz1,jx2,jy2,jz2,xyzflag,limitz); 
        }
        
        /*check for invisible color of list and point before */
        /* this last display decision*/
          
        if(Listcolordominant || (((thispointptr->colorwidth) & 31) == 0) )
        {/*don't have, or can't use this point's color,*/
              /* see if list is colored invisible*/
              if(  (thislistptr->color) == 31) {LOK = 0;}
                  /*invisible so NOT OK to display*/ 
        }
        else
        {/*check this point's color for invisible color*/
              if( ((thispointptr->colorwidth)&31) == 31) {LOK = 0;}
        }
        Lneedcolornumber = 1;
        if(    naspectsthiskinemage > 0 /*there are aspects possible*/
            && currentaspect > 0 /*an aspect is selected*/
            && currentaspect <= MAXPOINTASPECTS /*point storage limited*/
          )
        {/*aspect color dominant*/
           numcolor = decodecolor(thispointptr->aspects,currentaspect);
           if(numcolor == 31) { LOK = 0; } /*invisible*/
           else if(numcolor >= 1 && numcolor <= 25)
           {/*renderer can translate colornumber to palette index*/
              Lneedcolornumber = 0;
              Laspectpass = 1;
           }
        }/*aspect color dominant*/
          
        if(LOK == 1)
        {/*-enter item in 2D display list */
          /*check state of the 2D display list, increase counter*/
          if(Lplotpoint)
          {
                           ncount = 1;
                          /*for plotonly plotting and kaleidoscope effect */
                          /*redrawvec() called individually for each input pnt*/
          }
          else if(Lsetexcess)
          {/*ran out of room earlier this cycle*/
            ;/*do not change ncount, this point will not be processed*/
          }
          /*else if(ncount >= MAXDRAWS-6)*/ /*allows 5 disks per sphere*/
          else if(ncount >= MAXDRAWS-10)/*allows 9 disks per sphere 040918*/
          {
            /*make sure display array allocation*/
            /* will accommodate stacked disk spheres*/
            if(!Lsetexcess) /*NOT failed yet THIS cycle*/
            {/*try to reallocate storage*/
                           allocdisplayarrays(5000); /*try 1000 more*/
            }
            if(ncount >= MAXDRAWS-10) 
            {/*protects if reallocation fails*/
                  Lexcessvec = 1;/*too many vectors in recent cycle*/
                  Lsetexcess = 1;/*too many vectors this drawing cycle*/
            }
            else
            {
               ncount++;    /*allocation OK: increase counter*/
            }
          }
          else
          {
             ncount++; /*normal increase counter*/
          }
          itpointptr[ncount] = thispointptr;

          /*allow for width to be spcified as well as color*/
          /*color is in first 5 bits, */
          /*depthcue in next 3, then width in next 3 */

          /*4 cases, of availability of point color and point width, */ 
          /*with option of List color Dominant */
          /*with added complecation of dominant point aspect color*/
          /*NOTE that point can also code for depth que color palette index */
          /*bits 5,4,3,2,1  (5 bits) are color code */
          /*bits 8,7,6      (3 bits) are depth que bin index (1-5 of 0-7 */
          /*bits 11,10,9    (3 bits) are line width in pixels */
          
          if(Lneedcolornumber) /*point NOT aspect colored, which is dominant*/
          {/*pointcolor..listcolor...choice*/
/*1*/       if(   ((thispointptr->colorwidth)    & 31) > 0 
                    && ((thispointptr->colorwidth)>>8 &  7) > 0 ) 
            {/*point has individually specified color and pen width*/
/*1 HAS point color and point width*/
               if(   Listcolordominant 
                  && ( ((thispointptr->STYLE)&FORCEPTCOLOR) != FORCEPTCOLOR) )
               {/*do not use this point's color, but do use its width*/
                /*might have point specified depth cue*/
/*1 USE list color and point width: List Color Dominant*/
                 it(0,ncount)=(   (thislistptr->color) 
                                | ((thispointptr->colorwidth) & ~31));
                           /*~31: one's complement of 31, i.e. bits 1->0,0->1*/
                           /*i.e. include everything except color code*/
               }
               else
               {/*use this point's color and width specifications*/
                /*might have point specified depth cue*/
/*1 USE point color and point width*/
                 it(0,ncount) = thispointptr->colorwidth; 
               }
            }/*point has individually specified color and pen width*/

/*2*/       else if(   ((thispointptr->colorwidth)    & 31) != 0 
                    && ((thispointptr->colorwidth)>>8 &  7) == 0 ) 
                         /*i.e. width 3 bits are clear*/
            {/*point has individually specified color but not pen width*/
/*2 HAS point color but NOT point width*/
               if(   Listcolordominant 
                  && ( ((thispointptr->STYLE)&FORCEPTCOLOR) != FORCEPTCOLOR) )
               {/*do not use this point's color and use list width*/
                /*might have point specified depth cue*/
/*2 USE list color and list width: List Color Dominant*/
                  it(0,ncount)=(   (thislistptr->color)  /*list color*/
                                |((thispointptr->colorwidth)&224) /*6,7,8 bits*/
                                |(((thislistptr->width) & 7)<<8) /*just 3 bits*/
                               );
               }
               else
               {/*use this point's color but use list width specification*/
                /*might have point specified depth cue*/
/*2 USE point color and list width*/
                  it(0,ncount)=( ((thispointptr->colorwidth)&31) /*point color*/
                                |((thispointptr->colorwidth)&224) /*6,7,8 bits*/
                                |(((thislistptr->width) & 7)<<8)
                               ); /* 3 bits*/
               }
            }

/*3*/       else if(   ((thispointptr->colorwidth)    & 31) == 0 /*NOT color*/
                    && ((thispointptr->colorwidth)>>8 &  7) != 0 ) /*width*/
            {/*point has individually specified pen width but not color*/
/*3 HAS NOT point color but has point width*/
               {/*use this point's width specification*/
/*3 USE list color and point width*/
                  it(0,ncount)=(  thispointptr->colorwidth /*3bits+000+00000*/
                                | ((thislistptr->color) & 31)); /*5bits color*/
               }
            }

/*4*/       else 
            {/*point does NOT have its own color, nor width,*/
             /*might have point specified depth cue*/
/*4 HAS NOT point color and NOT point width*/
/*4 USE list color and list width*/
               it(0,ncount) = (  (thislistptr->color) /*1,2,3,4,5 bits*/
                                |(((thislistptr->width) & 7)<<8) /*pack 3bits*/
                                |((thispointptr->colorwidth)&224) /*6,7,8 bits*/
                              );
            }
          }/*pointcolor..listcolor...choice*/
          else
          {/*aspect point color dominant*/
             numcolor = decodecolor(thispointptr->aspects,currentaspect);
             if(numcolor > 0 && numcolor < 32)
             {/*renderer can translate colornumber to palette index*/
                it(0,ncount)=((numcolor&31) | ((thispointptr->colorwidth) & ~31)); /*121107*/
                       /*~31: one's complement of 31, i.e. bits 1->0,0->1*/
                       /*i.e. include everything except color code*/
                Lneedcolornumber = 0;
                Laspectpass = 1;
             }
          }/*aspect point color dominant*/

          if(LOK)
          {/*passed LOK check point*/
            /*Now set up link list links for the display arrarys*/
            /*link list way of scoping out the z buffer bins*/
/*010913*/
if( (xyzflag & VECTOR) && (xyzflag & VARIANT1) && (xyzflag & VARIANT2) 
   && fmod(pointcounter,2)==0.0 && pointcounter>3)
{
   ideep = ideepprevious;
}
ideepprevious = ideep;
            if(thelink(0,ideep)==0)
            {
              thelink(0,ideep) = ncount;
              /*establish the first point at this ideep*/
              /*ideep is current z-buffer bin*/
            }
            else
            {
              k = thelink(1,ideep);
              plinkit((long)k) = ncount;
              /*inform previous point at this ideep about*/
              /* its next point*/
            }
            thelink(1,ideep) = ncount;
                /*record current point as the*/
                /*effective last point at this ideep*/
             plinkit((long)ncount) = 0; 
                /*flag current point that it is, at*/
                /*least for now, the last point at ideep z level*/

            /*type : too many for even 4 bits, now separate array 080405 */
            /* it(7,ncount):type 080405 */
            /*Set the parameters of the display arrays, it(0,ncount):type */
            /*type expanded, 3 to 4 bits, to include more varients 000804*/
            /*- 16384  8 point type code (shift >>11, mask 15) 4 bit new */
            /*-  8192  4 point type code (shift >>11, mask 7)  3 bit old */
            /*-  4096  2 "     1,     2,     3,     4,     5,     6,     7 */
            /*-  2048  1 "  2048,  4096,  6144,  8192, 10240, 12288, 14336 */
            /*vector 0,ball,sph 1,tri 2,label 3,word 4,ribb 5 ,dot 6,nohilte 7*/
            
            /*extra types of newer, 4 bit, designation */ /*020112xrc*/
            /*         8,     9,    10,    11,    12,    13,    14,    15 */
            /*     16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720 */
            /*  MARKTYPE, RING ,  FAN , ARROW,DIAMOND,SQUARE,EMPHASIS,MULTIpt*/
            /*it(0,ncount) has colorwidth info from above*/
            it(7,ncount) = 0;  /*for plain VECTOR,  080405*/ 
            it(1,ncount) = jx1;                /* xmove */
            it(2,ncount) = jy1;                /* ymove */
            it(3,ncount) = jx2;                /* xdraw */
            it(4,ncount) = jy2;                /* ydraw */

            /*if( (xyzflag & VECTOR) && (xyzflag & VARIANT1) ) */
            if(   (xyzflag & VECTOR) 
               && (xyzflag & VARIANT1) 
               &&!(xyzflag & BALL)
               &&!(xyzflag & MARKTYPE)
              )
            {/*triangle list*/
              if(xyzflag&VARIANT2) it(7,ncount)=it(7,ncount)| 5; /*RIBBON 5*/
              else                 it(7,ncount)=it(7,ncount)| 2; /*TRIANGLE 2*/
              it(5,ncount) = jx0;                /* xmove */
              it(6,ncount) = jy0;                /* ymove */
/*
 fprintf(stderr,"MAGEDRAW found a triangle list, it(7,%d)==%d\n",ncount,it(7,ncount));             
*/
              /*Gets here only if Lpointsarein set by pointcounter >= 3*/        
              if(thispointptr->STYLE & PIXELATED) /*080912  z-buffered dots*/
              {/*080912 fill area with z-buffered dots*/
                  it(7,ncount) =it(7,ncount)| 32; /*32: PIXELATED type 080912*/
                  /*heavy lifting must be done to fill display list with dots*/
                  /*reach here when Lpointsarein==1 for a triangle*/
                  /*last 3 points define transformed triangle, to be pixelated*/
                  /* jx0,jy0,jz0,jx1,jy1,jz1,jx2,jy2,jz2  */
                  /*OR jxyz0pointptr,jxyz1pointptr,jxyz2pointptr */
/*
 fprintf(stderr,"MAGEDRAW found a pixelated list, it(7,%d)==%d\n",ncount,it(7,ncount));             
*/
              }
              surfacetriangle( xyzflag,jx0,jy0,jz0
                              ,jx1,jy1,jz1,jx2,jy2,jz2
                              ,pointcounter
                              ,jxyz0pointptr,jxyz1pointptr,jxyz2pointptr
                             );  /*MAGEUTIL*/
              /*sets LOK, so watch out for LOK for ncount thelink stuff !!!!*/
            }/*triangle list*/
            else if     ( (xyzflag & WORDS ) ) 
            {/*words     4*/  
              it(7,ncount)=it(7,ncount) | 4; /*WORD 4*/
            }
            else if( (xyzflag & LABEL) ) 
            {/*labels    3*/ 
              it(7,ncount)=it(7,ncount) | 3; /*LABEL 3*/
            }
            else if((xyzflag & BALL ))/*balls,spheres 1 & 7, do before VECTOR*/
            {/*balls, spheres  1, 7, 15    type used in call to balltoscreen*/
              /*special section: multi-points-to-parallel-axes 061114*/
              /*Also invoked for multi-dimwnsioned dotlist  121202*/
              /*kludge prototype presumes NDIM wrapped angle coords */
              if( LNDparallelaxes && (thislistptr->KKIND & DIMENSIONKKIND) )
              {  /* 2nd loop calls multipointparallels()*/
                 /* NDIM balllist to be plotted on screen as parallel axes*/
                 {it(7,ncount)=it(7,ncount) | 15;} /*MULTIpt parallel 15*/
              }

              else if( (thislistptr->STATUS & NOHILITEFLAG) == NOHILITEFLAG)
                 {it(7,ncount)=it(7,ncount) | 7;} /*NOHILITE 7*/
              else 
                 {it(7,ncount)=it(7,ncount) | 1;}  /*BALL 1*/
              noballs = 0; /*only place to set this*/
              /*warns that later vectors may need shortening*/
              /*static lists only get the original, place-on-screen, scaling*/
              /*ballscale is local*/
              if( ( ( (itpointptr[ncount]->listptr)->STATUS 
                 & STATICFLAG) == STATICFLAG ) )
                 ballscale = Scale;/*global*/
                           /*fixed list, i.e. "static" */
              else ballscale = scalenew;/*global*/

              /*if(thispointptr->radius < .001)*/ /*980804*/
              if(Listradiusdominant || thispointptr->radius < .001) /*040402*/
              {/*980804*/
                if(thislistptr->radius < .001) 
                  fradius = (float)(ballscale*0.2);
                else 
                  fradius = (float)(ballscale*(thislistptr->radius));
              }
              else
              {
                 fradius = (float)(ballscale*(thispointptr->radius));
              }
              if(Lperspec && Lfloat)
              {
                  jradius = (int)(radiusinflation*fradius + fradius);
              }
              else
              {
                  jradius = (int)fradius;
              }
              
              it(4,ncount) = jradius;/*pass as integer*/
              it(3,ncount) = 0; /*indicates single disk == ball*/
                /*spherelist will override this*/
              /*it(5,ncount) =  32767;*/ /*????????????????????????*/
              if((thislistptr->STATUS & NOBLKRIMFLAG) == NOBLKRIMFLAG)/*040925*/
                  {it(5,ncount) = 0;}  /*Lblackrim==0 NO BLACK RIM  040925*/
              else{it(5,ncount) = 1;}  /*Lblackrim==1   BLACK RIM  040925*/
              /*balltoscreen does each disk in turn by depth*/
              /*sphere of stacked disks: creates additional points*/
              /* copies it(1,),it(2,),,it(4,),it(5,), uses it(3,) for disk# */ 
              /* defer until finsihed with this one point in case */
              /* postscript or raster3D set pointers rather than x,y*/
              /*see special section for sphere case below*/
            }/*balls, spheres  1, 7, 15 (below)multi-points-parallel-axes-plot*/
            else if(xyzflag & MARKTYPE)
            {/*MARKTYPE*/
               /*marks:  (rings, fans)  have their own type, not a DOT type*/
               /*if(thispointptr->radius < .001 && thislistptr->radius >.001)*/
               /*040402 Listradiusdominant vs point and list radii messed up*/
               if(Listradiusdominant) /*040831*/
               {/*use finite list radius (worst is zero point radius) */ 
                 if(thislistptr->radius >.001){fradius = thislistptr->radius;}
                 else                         {fradius = thispointptr->radius;}
               }
               else 
               {/*use finite point radius - or use list radius, at worst==0*/
                 if(thispointptr->radius >.001){fradius= thispointptr->radius;}
                 else                          {fradius = thislistptr->radius;}
               }
/*121210 marktype radii (e.g. ringlist for NDIM param placement) re ball radii*/
               if((thispointptr->STATUS & STATICFLAG) == STATICFLAG )
               {/*screen type point is a static point for scaling...121210*/
                  it(4,ncount) = (int)(fradius*Scale); /*pass as integer*/
               }
               else
               {/*radius scaled by zoom...*/
                  it(4,ncount) = (int)(fradius*scalenew); /*pass as integer*/
               }
               if((thispointptr->STYLE & RINGSTYLE) == RINGSTYLE)
               {/*ring, not just a special mark*/
                  it(7,ncount)=it(7,ncount) | 9; /*RING 9*/
               }
               if((thispointptr->STYLE & DIAMONDSTYLE) == DIAMONDSTYLE)
               {/*screen-sized diamond, not just a special mark*/
                  it(7,ncount)=it(7,ncount) | 12; /*DIAMOND 12*/ /*010916*/
               }
               if((thispointptr->STYLE & SQUARESTYLE) == SQUARESTYLE)
               {/*screen-sized square, not just a special mark*/
                  it(7,ncount)=it(7,ncount) | 13; /*SQUARE 13*/ /*010916*/
               }

               else if((thispointptr->STYLE & FANSTYLE) == FANSTYLE)
               {/*one spine of a fan*/
                  it(7,ncount)=it(7,ncount) | 10; /*FAN 10*/
               }
               else
               {/*plain mark*/
                  it(7,ncount)=it(7,ncount) | 8; /*MARK 8*/
               }
            }/*MARKTYPE*/
            else if( (xyzflag & VECTOR) || (xyzflag & DOT) )
            {/*VECTOR||DOT */
               if( (xyzflag & DOT) )
               {/*dot  6 */
                 it(7,ncount)=it(7,ncount) | 6;   /*DOT 6*/
                 /*it(5,ncount) =  0;*/ /*pure DOT*/
                    /*special section: multi-points-to-parallel-axes 061114*/
                    /*Here invoked for multi-dimwnsioned dotlist  121202*/
                    /*kludge prototype presumes NDIM wrapped angle coords */
                  if( LNDparallelaxes && (thislistptr->KKIND & DIMENSIONKKIND) )
                  {  /* 2nd loop calls multipointparallels()*/
                     /* NDIM balllist to be plotted on screen as parallel axes*/
                     {it(7,ncount)=it(7,ncount) | 15;} /*MULTIpt parallel 15*/
                  } 
               }
               else if(xyzflag & VARIANT3) /*ARROW*/
               {/*NOTE: level-of-detail will drop this to simple line since */
                /*arrowheads are expensive to construct on the fly*/
                  it(7,ncount)=it(7,ncount) | 11;   /*11: ARROW*/
                  it(5,ncount) = jz1; /*ARROW is inherently 3D*/
                  it(6,ncount) = jz2;
               }
               else if(thispointptr->STYLE & EMPHASIS) /*020113xrc*/
               {
                  it(7,ncount)=it(7,ncount) | 14;   /*14: EMPHASIS*/
                  it(5,ncount) = jz1; /*EMPHASIS is implicitly 3D*/
                  it(6,ncount) = jz2;
               }
               else if(thispointptr->STYLE & ELLIPSOID) /*080406*/
               {
                  /*it(7,ncount)=it(7,ncount) | 16;*/   /*16: ELLIPSOID*/
                  it(7,ncount) = 16;   /*16: ELLIPSOID forced type 080406*/
               }
               else /*presumably just ( (xyzflag & VECTOR) ) */
               {/*Vector*/
                   ;/*it(7,ncount) unchanged*/ /*vectors   0 */
               }
               if(  IPC &&(jx1==jx2 && jy1==jy2) )
               {/*PC can't draw zero length vector*/
                  /*make a line of finite length*/
                     it(3,ncount) = jx1+1;
                     it(4,ncount) = jy1+1;
                     /*PC won't show single pixel dot*/
               }/*PC can't draw zero length vector*/
            }/*VECTOR||DOT*/

            if(Lpostscript || Lraster3Drender)
            {/*PostScript and Raster3D overwrite arrays of regular flow*/
                 loadpointpostscript(ncount,xyzflag,ilook   
                   ,jxyz0pointptr,jxyz1pointptr,jxyz2pointptr);
                 it(2,ncount) = ideep;
                 /*vectors and triangles use offsets, spheres do not*/
                 /*spheres use it(5,ncount) Lblackrim==1 BLACK RIM 040925*/
            }/*PostScript and Raster3D*/

            /*special section for sphere case, must follow postscript*/
            /*which needs current ncount before it is incremented for*/
            /*the various disks of the sphere emulation*/
            if( (xyzflag & BALL) && (xyzflag & VARIANT1) ) 
            {/*sphere of stacked disks: creates additional points*/
                stackeddiskssphere(jradius,izadjust,izbinsize,maxdeep);
            }/*sphere of stacked disks*/ 


          }/*passed LOK check point*/
          if(!LOK)
          {/*flunked one of the LOK tests*/
            ncount--; /* didn't do vector, decrement counter*/
            pointcounter--;
          }
        }/*-enter item in 2D display list */

      }/*Lpointsarein:DISPLAY: draw-vector dot ball label word*/
      /*Storage of previous points needed for continued drawing*/
      /*This shift does no harm if current point was NOT valid*/
      /*jx0 = jx1orig;*/ /*save one further back for triangles*/
      /*jy0 = jy1orig;*/ /*made as continuous strip */
      /*jz0 = jz1orig;*/

      jx0 = jx1; /*save one further back for triangles*/
      jy0 = jy1; /*made as continuous strip */
      jz0 = jz1; /*used clipped position for triangles and ribbons*/

      if((xyzflag & TRIANGLE) == TRIANGLE)
      {/*Triangle or Ribbon, store clipped values for previous points*/
         jx1 = jx2; /*previous point with clipped current*/
         jy1 = jy2; /*for continuous polyline */
         jz1 = jz2;
      }
      else
      {
         jx1 = jx2orig;  /*previous point with unclipped current*/
         jy1 = jy2orig;  /*for continuous polyline */
         jz1 = jz2orig; 
      }
      /*jx1orig = jx2orig;*/ /*store unclipped for next draw */
      /*jy1orig = jy2orig;*/ /* there may be no intervening move */
      /*jz1orig = jz2orig;*/ 
      jxyz0pointptr = jxyz1pointptr;
      jxyz1pointptr = jxyz2pointptr;
         
    }/*sufficient display room for this point*/       
  }/*point currently valid*/  

  if(thispointptr == thislistptr->lastpointptr) 
     thispointptr=NULL;
  else thispointptr = thispointptr->nextptr;                 
                
  /*FIRST INNER LOOP-----^^-------^^------------*/
                }/*endof-draw-loop-of-points*/
              
              }/*endof-visible-list*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
              else thislistptr = thislistptr->nextptr;
            }/*endof-loop-over-lists*/                
          }/*endof-visible-set*/
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr=NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*endof-loop-over-subgroups*/
      }/*endof-visible-group*/
      thisgrupptr = thisgrupptr->nextptr;/*==NULL if last group*/
    }/*endof-loop-over-groups*/
  }/*ilook pass*/
/*printf("\n");*/

 /*FIRST OUTER LOOP--^^---------^^-----------------------------------------*/
 nowticks = theclocktickcount();    /*____MAIN.C*/
 firstloopticks = (nowticks - icountticks);  
 icountticks = nowticks;  

 /*SECOND OUTER LOOP--__---------__--------__----------------------------*/

 postscriptcnt = 0;
 inewpoint = 1;/*check point number, helps avoid duplicates in PS output*/
 ideepcueold = -1; /*so will get initialization of ideepcue==0 level*/
 if(!Lraster3Drender)
 {/*NORMAL FLOW, strobe out points back to front*/

  for(ideep = 0; ideep <= maxdeep ; ideep++)
  {/*SECOND OUTER LOOP--over bins */
    if(thelink(0,ideep) > 0)
    {/*this bin has entries*/
      /*ideep is the bin, ideepcue is the 0-4 depth cue bin */
      ideepcue = (int)((ideep*Ndepths)/Nbins);/*int arith truncations*/
      /*ideep range: 0-999, 5 depths: 0--4 intervals*/
      /*3 depths: 0-2: 2,3,4 colordepths, 1,2,3 line widths*/
      if(ideepcue != ideepcueold)
      {/*ideepcue has changed*/
         ideepcueold = ideepcue;
         oldcolordepth = 32000; /*impossible, so forces icolor recalc */
         oldpenwidth = 32000; /*impossible, so forces penwidth recalc */
         iwide = ideepcue + 1; /*1,2,3,4,5 or 1,2,3 */
         if(L5depths) {ideepcolor = ideepcue;} /*L5depths: 0,1,2,3,4*/
         else {ideepcolor = ideepcue+2;} /*L3depths: 2,3,4*/
         icolor = 0; /*black*/
            /*stays 0 for black unless color or gray palette*/
         rgbColor.red = 0;
         rgbColor.green = 0;
         rgbColor.blue = 0;
         idecue = 0;
         if(Lwhitebkg) idecue = 25;

         ioffset = 50;    /* so can get 5 depths within 250 color choices */
         igraystart = numberofcolor("gray");
         if(npixelbits == 4 && Lcolor) /*Lcolor TRUE if gray, FALSE if B&W */
         {
            ioffset = 1; /* only have 16 numbers, all gray packed together */
            igraystart = 1;
            if(Lwhitebkg) idecue =  5;  /* gray: 1-5: W on B, 6-10: B on W */
            icolor = igraystart + ioffset*ideepcolor + idecue;
            /*At least the Mac defines such a limited color-planes state*/
         }
         if(!LLcolor && Lcolor)  /* Gray scale monitor but color QuickDraw */
         {/*do gray tones*/    /* B&W set Lcolor FALSE */
           icolor = igraystart + ioffset*ideepcolor + idecue;
         }   /*real color gets set in inner loop */

         if(Lonewidth || Lthin)    ipen = ithick;
         else                      ipen = iwide;
      }/*ideepcue has changed*/

    /*SECOND INNER LOOP--__-touches only vectors at this depth once */
    i = thelink(0,ideep);
    while(i != 0)  /* "while" loop for clarity*/
    {/*loop on i while i != 0, z buffer bin==ideep*/

      /*080405 type split from color-depth-width, now in it(7,i) */
      /*color-depth-width-type compressed into it(0,i) */
      /*type = it(0,i)>>11 & 7;*/ /*3 bits above lowest 11 */
      /*type = it(0,i)>>11 & 15;*/ /*4 bits above lowest 11 */ /*000804*/
      type = it(7,i);  /*080405*/
      colordepth = (it(0,i) & 255);/*lowest 5+3=8 bits*/
      origpenwidth = ((it(0,i)>>8) & 7);/*shift by 5+3, pick up only 3 bits*/
      itdepth =  ((it(0,i)>>5) & 7);
      itcolor = (it(0,i) & 31); /*color code: translate to color index*/
      penwidth = origpenwidth; /*for starters*/

      /* ipen is computed depth-que line width, while originally*/
      /*penwidth is line width specified by point width or list width*/
      /*where point width is dominant over list width setting*/
      /*BUT for many uses, penwidth=ipen as a default.  However, some */
      /*display objects need to use origpenwidth or default to a const*/

      if(Lcolor) /*LLcolor==1 true color, Lcolor==1 gray...*/
      {/*can adjust color and/or depth cue gray level*/
       {/*change color*/
        /*it(0,i) 5bits color index,3bits color code,3bits line width*/
        /*remains: 4 bits for type, and 1 bit  unused 000804*/
        /*explicit codes substitute for old color index and  */
        /*fixed depth-que */
        /*shift over by 5, pick up only 3 bits to test for explicit */
        /*"depthcue", which can only have values of 1,2,3,4,5 */
        if(itdepth > 0 && itdepth <= 5)
        {/*force palette shades rather than doing actual depth-cueing*/
          /*color code is effectively the depth-cue bin*/
          if(LLcolor) /*true color*/
          {/*forcing palette shades rather than doing actual depth-cueing*/
            if(itcolor > 0 && itcolor < 26)
            {/*within range of indexed, depth-cued colors*/
               icolor=itcolor+ 50*(itdepth-1) +idecue;
            }
            else if(  itcolor == 0 
                    ||itcolor == numberofcolor("deadblack")) 
            {/*deadblack probably colornumber 30*/
               icolor = 0; /*std color palette entry place*/
            }
            else if(itcolor == numberofcolor("deadwhite")) 
            {/*deadwhite probably colornumber 29*/
               icolor = 255; /*std color palette entry place*/
            }
            else
            {/*set to default color so at least will draw something*/
               numberofcolor("default");
               icolor = numberofcolor("default")+50*(itdepth-1) +idecue;
            }
            /*26,27,28 probably are not defined*/
          }
          else
          {/*fudge gray level by explicit "depthcue"*/
             icolor= 24 + 50*(itdepth-1) +idecue;
          }
          /*depthcue 1-5 used as ideepcolor 0-4*/
        }/*force palette shades rather than doing actual depth-cueing*/
        else if(LLcolor) /*can support real color*/
        {/*use depth-que bin calculated from actual depth*/
            if(itcolor > 0 && itcolor < 26)
            {/*within range of indexed, depth-cued colors*/
               icolor = itcolor + 50*ideepcolor + idecue;
            }
            else if(  itcolor == 0 
                    ||itcolor == numberofcolor("deadblack")) 
            {/*deadblack probably colornumber 30*/
               icolor = 0; /*std color palette entry place*/
            }
            else if(itcolor == numberofcolor("deadwhite")) 
            {/*deadwhite probably colornumber 29*/
               icolor = 255; /*std color palette entry place*/
            }
            else
            {/*set to default color so at least will draw something*/
               icolor = numberofcolor("default")+50*ideepcolor + idecue;
            }
            /*26,27,28 probably are not defined*/
        }/*use depth-que bin calculated from actual depth*/
       }/*change color*/
      }/*can adjust color and/or depth cue gray level*/
      else
      {/*B&W*/                 /*051201 */
         if(itcolor == numberofcolor("deadwhite")) 
         {/*deadwhite probably colornumber 29*/
            icolor = 255; /*std color palette entry place for dead white*/
         }
         else
         {
            icolor = 0; /*std color palette entry place for deadblack*/ 
         }
      }
      /*now for some Mickey Mouse stuff that was designed to avoid*/
      /*more time consuming always reseting the pen color and width*/
      if(   (colordepth != oldcolordepth)
          ||(penwidth   != oldpenwidth  )
          ||(penwidth   == 0            ) )
      {/*change pen color and/or width*/
        if(penwidth == 0) penwidth = ipen;
          /*pen width used is the one calculated from the actual*/
          /* depth or other default*/
          /*pen width can be explicitly controlled*/
        if(LGUI) setdrawcolorpen(penwidth,penwidth,icolor);/*____DRAW.C*/
        /*PC is the only one that lumps pen width and color */
        /*but this is the way MAGE treats them anyway*/
        oldcolordepth = colordepth;
        oldpenwidth = penwidth;
      }/*change pen color and/or width*/

      if(Lpostscript) /*only gets here if !Lraster3Drender*/
      {/*postscript renderer to a file*/
          if(type == 14) /*NOTE also explicitly fudged in MAGEREND.c 031127*/
          {/*emphasis merely over-rides pen width for postscript 031127*/   
             penwidth = 9; /*compromise for radius-determined emphasis 031127*/
          }
          /*Now postscripter does NOT need to recalculate color stuff*/
          postscripter( i
                   ,penwidth
                   ,origpenwidth
                   ,icolor
                   ,ideepcolor
                   ,idecue
                   ,sinangle
                   ,cosangle
                   ,ixseting
                   ,iyset
                   ,izset
                   ,&postscriptcnt
                   ,&inewpoint
                   ,eyeposition
                   ,xminleft,xmaxleft,xminright,xmaxright,ymin,ymax /*031127*/
                  );/*MAGEPOST*/
      }/*postscript renderer to a file*/

      else if(LNDparallelaxes) /*NDIM ball multi-point-parallel-axes 061115*/
      {
         if( type == 15)  /*subverted balllist*/
         {  /*LNDparallelaxes: multipointparallels()  061114*/
             multipointparallels(itpointptr[i], icolor, ipen, &LNDnew);
         }
      }
      else /*only gets here if !Lraster3Drender*/
      {/*MAGE renderer to the screen*/
            /*LineTo same command Mac & PC */
            /* but PC has extra field to define where to move,draw*/
            /* mac defines XTR = ' '  PC defines XTR = 'cDC,'  */
            /*UNIX doesn't do a separate move step*/
#ifdef MACINTOSH /*971001*/
              MoveTo(XTR it(1,i),it(2,i)); /*THE MOVE, MAC command */  
#endif
#ifdef PCWINDOWS
              MoveToEx(XTR it(1,i),it(2,i),NULL); /*THE MOVE, PC command*/
#endif
            if( type  == 0 || type == 6 )/*simplist, most common, make fastest*/
            {/*vector or dot, VECTOR DOT*/
/*#define FOODOT*/
#ifdef  FOODOT
if((abs(it(3,i) - it(1,i)) < .001) && (abs(it(4,i) - it(2,i)) < .001))/*090626*/
{XSetLineAttributes(dpy,offgc,origpenwidth,LineSolid,CapRound,JoinRound);}
else
{XSetLineAttributes(dpy,offgc,origpenwidth,LineSolid,CapButt,JoinMiter);}
/*{XSetLineAttributes(dpy,offgc,origpenwidth,LineSolid,CapRound,JoinRound);}*/
#endif



               LineTo(XTR it(3,i),it(4,i)); 
                   /*THE DRAW, MAC & PC & UNIX_X11 command */
             
               /*UNIX_X11 defines:  (MAGE.H) */
               /*     LineTo =   'XDrawLine' */
               /*     XTR    =   'dpy,offscreen,offgc,it(1,i),it(2,i),'  */
               /*XDrawLine( dpy, offscreen, offgc
                           ,it(1,i),it(2,i),it(3,i),it(4,i));  */
            }/*vector or dot, VECTOR DOT*/
#ifdef UNIX_X11
            else if( type == 16 ) /*080406*/
            {/*ELLIPSOID*/
               ellipsoidtoscreen(it(1,i),it(3,i),it(2,i),it(4,i),it(5,i),it(6,i)
                             ,ipen,icolor,itpointptr[i]); /*MUXMDRAW for now*/
               /*NOTE order: jx1,jx2,jy1,jy2,jz1,jz2,icolor,itpointptr) */
            }/*ELLIPSOID*/
#endif
            else if( type == 14 ) /*020112xrc*/
            {/*EMPHASIS*/
               emphasistoscreen( it(1,i),it(3,i),it(2,i),it(4,i),it(5,i),it(6,i)
                             ,ipen,icolor,itpointptr[i]);
               /*NOTE order: jx1,jx2,jy1,jy2,jz1,jz2,icolor,itpointptr) */
               /*020112xrc BUT icolor IS NOT PASSED !!!! ????*/
               /*020112xrc ipen and icolor alread set !!!! ????*/
            }/*EMPHASIS*/
            else if( type == 11 )
            {/*ARROW*/
               arrowtoscreen( it(1,i),it(3,i),it(2,i),it(4,i),it(5,i),it(6,i)
                             ,itpointptr[i]);
               /*NOTE order: jx1,jx2,jy1,jy2,jz1,jz2,icolor,itpointptr) */
               /*020112xrc BUT icolor IS NOT PASSED !!!! ????*/
               /*020112xrc ipen and icolor alread set !!!! ????*/
            }/*ARROW*/

            else if(type ==  8 ) /*MARKTYPE alone*/
            {/*mark: x,y,radius,color*/
               marktoscreen(it(1,i),it(2,i),it(4,i),icolor,origpenwidth);
            }
            else if(type ==  9 ) /*MARKTYPE and RINGSTYLE*/
            {/*ring: x,y,radius,color*/
               ringtoscreen(it(1,i),it(2,i),it(4,i),icolor,origpenwidth);
            }
            else if(type == 10 ) /*MARKTYPE and FANSTYLE*/
            {/*fan at x,y,parent point: use point radius,*/
               /* (current color and pen)*/
               /*and send coloroffset for this depth and screen bkg*/
               fantoscreen( it(1,i),it(2,i),icolor,50*ideepcolor+idecue
                           ,origpenwidth,itpointptr[i]);
            }
            else if(type == 12 ) /*MARKTYPE and DIAMONDSTYLE*/ /*010916*/
            {/*ring: x,y,radius,color*/
               diamondtoscreen(it(1,i),it(2,i),it(4,i),icolor,origpenwidth);
            }
            else if(type == 13 ) /*MARKTYPE and SQUARESTYLE*/ /*010916*/
            {/*ring: x,y,radius,color*/
               squaretoscreen(it(1,i),it(2,i),it(4,i),icolor,origpenwidth);
            }

            else if(type == 3)  /* label list */
            {/*label*/ 
              {/*copy char str from storage */  
                /* ptID stored in array char* com starting at offset*/
                /* stored in pointstruct pointed to by itpointptr[i]*/
                /* variable length, so look for "\0"*/           
                 getptIDstring(word, itpointptr[i]);
              }
              if(!LMAGEstrokedfont)
              {
                 labeltoscreen( word  ,it(1,i),it(2,i), icolor ); /*____DRAW.C*/
              }
              else
              {/*label at point using stroked font*/
                  localfontsize = labelsfontsize;
                  raster3Dtextpreviewer( (float)it(1,i)
                                        ,(float)it(2,i)
                                        ,0.0
                                        ,(float)penwidth
                                        ,icolor
                                        ,localfontsize
                                        ,word);
              }
            }/*label*/
            else if( type == 1 || type == 7) /* ball or sphere list */
            {/*ball*/  /*1 default, with hilite, 7 override: nohilite*/
              /*it(3,i) carries disk #, 0 for ball/disk, 1-5 for "sphere"*/
              /*it(4,ncount) = int radius;  */
              /*it(5,ncount) = int blackrim flag;  040925*/
              if(LTBstereo && Lstereo) /*970603*/
              {/*TBstereo_balltoscreen funct flagged*/
                 balltoscreen(it(1,i),it(2,i),it(3,i),it(4,i),it(5,i) /*040925*/
                                       ,ipen,icolor,type,1);
              }
              else
              {
                 balltoscreen(it(1,i),it(2,i),it(3,i),it(4,i),it(5,i) /*040925*/
                                          ,ipen,icolor,type,0);
                    /*____DRAW.C*/
              }
            }/*ball*/
            else if( type  == 4 ) /* word list */
            {/*word char str stored in pointstruct pointed to by itpointptr[i]*/
               getcommentstring(word, itpointptr[i]);/*MAGELIST gets wordcom[]*/
               /* note: 3 byte format info on front of string*/
               if( (word[0] != 64) && !LMAGEstrokedfont)
               {
                  wordstoscreen(word,it(1,i),it(2,i), icolor );/*____DRAW.C*/
               }
               else
               {/*use raster3Dtextpreviewer to call RENDERstrokedtext */
                  if(word[2] != 0) localfontsize = word[2];
                  else localfontsize = wordsfontsize;
                  for(j=3;j<=255;j++)
                  {/*strip off format info*/
                      word[j-3] = word[j];
                      if(word[j]=='\0') break;
                  }
                  raster3Dtextpreviewer( (float)it(1,i)
                                     ,(float)it(2,i)
                                     ,0.0
                                     ,(float)penwidth
                                     ,icolor
                                     ,localfontsize
                                     ,word);
               }
            }/*word char str */
            else if(   type  == 2  || type  == 5 )  /* triangle, ribbon list */
            /*        || type  == 34 || type  == 37)*/ /* pixelated list 080912*/
            {
                triangletoscreen(it(5,i),it(6,i)
                                ,it(1,i),it(2,i)
                                ,it(3,i),it(4,i),icolor);/*____DRAW.C*/
            }
      }/*MAGE renderer to the screen*/

      i = linkit[i];/*this ith point has the link to the next point*/

    }/*loop on i while i != 0, z buffer bin==ideep*/
    /*SECOND INNER LOOP-----^^--special cases----*/
    }/*this bin has entries*/
  }/*SECOND OUTER LOOP--over bins */
 }/*NORMAL FLOW, strobe out points back to front*/
 else /*Lraster3Drender*/
 {/*SPECIAL LOOP FOR RASTER3D, POINTS IN ORIGINAL ORDER*/

  for(i = 1; i <= ncount ; i++)
  {/*SECOND OUTER LOOP--over points in original order */
      ideep = it(2,i); /*special use of this array position*/
      /*ideep is the bin, ideepcue is the 0-4 depth cue bin */
      /*ideepcolor is the actual color depth applied*/
      ideepcue = (int)((ideep*Ndepths)/Nbins);/*int arith truncations*/
      /*ideep range: 0-999, 5 depths: 0--4 intervals*/
      /*3 depths: 0-2: 2,3,4 colordepths, 1,2,3 line widths*/
      if(ideepcue != ideepcueold)
      {/*ideepcue has changed*/
      ideepcueold = ideepcue;
      oldcolordepth = 32000; /*impossible, so forces icolor recalc */
      oldpenwidth = 32000; /*impossible, so forces penwidth recalc */
      iwide = ideepcue + 1; /*1,2,3,4,5 or 1,2,3 */
      if(L5depths) {ideepcolor = ideepcue;} /*L5depths: 0,1,2,3,4*/
      else {ideepcolor = ideepcue+2;} /*L3depths: 2,3,4*/
      icolor = 0; /*black*/
       /*stays 0 for black unless color or gray palette*/
      rgbColor.red = 0;
      rgbColor.green = 0;
      rgbColor.blue = 0;
      idecue = 0;
      if(Lwhitebkg) idecue = 25;

      ioffset = 50;    /* so can get 5 depths within 250 color choices */
      igraystart = numberofcolor("gray");
      if(npixelbits == 4 && Lcolor) /*Lcolor TRUE if gray, FALSE if B&W */
      {
        ioffset = 1; /* only have 16 numbers, all gray packed together */
        igraystart = 1;
        if(Lwhitebkg) idecue =  5;  /* gray: 1-5: W on B, 6-10: B on W */
        icolor = igraystart + ioffset*ideepcolor + idecue;
        /*At least the Mac defines such a limited color-planes state*/
      }
      if(!LLcolor && Lcolor)  /* Gray scale monitor but color QuickDraw */
      {/*do gray tones*/    /* B&W set Lcolor FALSE */
        icolor = igraystart + ioffset*ideepcolor + idecue;
      }   /*real color gets set in inner loop */

      if(Lonewidth || Lthin)    ipen = ithick;
      else                      ipen = iwide;
      }/*ideepcue has changed*/
      /*type = it(0,i)>>11 & 15;*/ /*4 bits above lowest 11 */ /*000804,031201*/
      type = it(7,i);  /*080405*/
      if(type == 14)
      {/*emphasis merely over-rides pen width for postscript 031127*/   
         ipen = 9; /*compromise for radius-determined emphasis 031127*/
      }

      /*no SECOND INNER loop for raster3Drender output ... Lraster3Drender*/
          raster3Drender( i
                   ,&oldcolordepth
                   ,&oldpenwidth
                   ,icolor
                   ,ipen
                   ,ideepcolor
                   ,idecue
                   ,sinangle
                   ,cosangle
                   ,ixseting
                   ,iyset
                   ,izset
                   ,&inewpoint
                   ,eyeposition
                   ,xminleft,xmaxleft,xminright,xmaxright,ymin,ymax /*031127*/
                  );/*MAGEREND*/

      /*no SECOND INNER loop for raster3Drender output ... Lraster3Drender*/
  }/*SECOND OUTER LOOP--over points in original order */
 }/*SPECIAL LOOP FOR POSTSCRIPT AND RASTER3D, POINTS IN ORIGINAL ORDER*/

 /*SECOND OUTER LOOP----^^--------^^----------^^---------------------*/
  if(Lpostscript && fpout!=NULL) finishpostscript(&postscriptcnt); /*MAGEPOST*/
     /*sets  Lpostscript = 0; Lspecialloop = 0; */
   /* -postscript command can get here before kin opened and fpout set 030801*/ 
  if(Lraster3Drender)
  {
     raster3Drenderfinish(); /*MAGEREND*/
     Lraster3Drender = 0;
     Lspecialloop = 0;
  }
  
  if( !LNDparallelaxes && (Lstereo || Lcompareon))/*not LNDparallelaxes 061123*/
  {
        if(Lonewidth || Lthin) ipen =  2;
        else                   ipen =  4;

        setdrawcolorpen(ipen,ipen,igraystart); /*pen x,y size and color code*/
               /* (igraystart set above)*/ /*____DRAW.C*/
        /*MoveTo,LineTo common commands for Mac and PC */
        /* but PC has extra field to define place */
        /* mac defines XTR = ' '  PC defines XTR = 'cDC,'  */
        if(LTBstereo)
        {
#ifdef MACINTOSH
            MoveTo(XTR ixmin,HEIGHT/2 );   /*THE MOVE, MAC command */
            LineTo(XTR ixmax,HEIGHT/2 );   /*THE DRAW, MAC command */ 
#endif
#ifdef PCWINDOWS
            MoveToEx(XTR ixmin,0,NULL );   /*THE MOVE, PC command */
            LineTo(XTR ixmax,0 );   /*THE DRAW, PC command */ 
#endif
#ifdef UNIX_X11
            XDrawLine(dpy,offscreen,offgc,ixmin,HEIGHT/2,ixmax,HEIGHT/2);
#endif
        }
        else
        {
#ifdef MACINTOSH
            MoveTo(XTR (ixmax+ixmin)/2,ymin);   /*THE MOVE, MAC command */
            LineTo(XTR (ixmax+ixmin)/2,ymax);   /*THE DRAW, MAC command */ 
#endif
#ifdef PCWINDOWS
            MoveToEx(XTR (ixmax+ixmin)/2,ymin,NULL);   /*THE MOVE, PC command */
            LineTo(XTR (ixmax+ixmin)/2,ymax);   /*THE DRAW, PC command */ 
#endif
#ifdef UNIX_X11
            XDrawLine(dpy,offscreen,offgc,(ixmax+ixmin)/2,ymin
                                         ,(ixmax+ixmin)/2,ymax);
#endif
        }
  }
  /* note about performance of dotlist vs vectorlist: */
  /*  182 dots as dotlist: 1st loop: 3, 2nd: 8 for: 14 total ticks */
  /*  182 dots as zerolength vectors as vectorlist: 5,9  17 total ticks */
    
  nowticks = theclocktickcount();  /*___MAIN.C*/ 
  secondloopticks = (nowticks - icountticks); 
  icountticks = nowticks; 
  nmovedraws = ncount; 

  if(Lmickeymouserun)
  {/*auto animate and/or auto rock running*/
     if(Lautorock) {sprintf(rstr,"run");} else {rstr[0]='\0';}
     if(Lautoanimate) {sprintf(Rstr,"Run");} else {Rstr[0]='\0';}
     sprintf(runstr,"--> %s%s -->",rstr,Rstr);
     condition(runstr);  /*____MAIN.C*/
     stringtoscreen(runstr,ixctr+5
                     ,(ymax-iybottom-iyfontsize-iyfontsize/2));
  }/*put to bottom of screen just above distance, overlays Lpipeactive frames*/

  if(Lexcessvec)
  {
    if(Lsetexcess) /* really set too many vectors this drawing cycle*/
    {
         sprintf(comstr," too many vectors");
         condition(comstr);  /*____MAIN.C*/
         Lsetexcess = 0;
    }
    else /* no longer has too many vectors, clear comment */
    {    
         /*sprintf(comstr,"\0"); warning 121107*/
         comstr[0] = '\0';  /*121107*/
         condition(comstr);  /*____MAIN.C*/
         Lexcessvec = 0;
    }
  }

  if(ipick==2||Lprepick)
  {/*pick request, ipick==2*/
    /*if(Lshiftkey) Lpick = 1;*/ /*screen pseudo pick conditions*/
    if(Lpick)
    {
       processpick();
    }/*process pick, Lpick==1*/
    ipick = 1;
  }/*pick request, ipick==2||Lprepick*/
  else
  {/*ipick!=2 process further motion activity*/
        if(Lpick&&Ldraglineactive)
        {/*Ldraglineactive: place free end at mouse position*/
            if(Lpoint==1 && !Lnewballson   /*031127 balldrag*/
                         && !Lnewlabelson) /*991130 draglabel uses just one pt*/
            {/*need 2nd point for this dragline*/
                numberline++;
                if((thispointptr = allocpointstructure(drawnewlistptr)) == NULL)
                {/*allocation failed*/          /*MAGELIST*/
                    sprintf(alertstr,"failed to alloc a drawline point");
                    alertstr2[0]='\0';alertstr3[0]='\0';
                    DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
                    numberline--;
                    return;
                }/*allocation failed*/
                /*can construct another point */
                thispointptr->STATUS = (thispointptr->previousptr)->STATUS;
                Lpoint = 0;
            }/*need 2nd point for this dragline*/
            jx1 = pickx-ixset;
            jy1 = picky-iyset;
            jz1 = pickz-izset;
            /*11=[1];12=[2];13=[3];21=[4];22=[5];23=[6];31=[7];32=[8];33=[9];*/
            thispointptr = drawnewlistptr->lastpointptr;
            if( (thispointptr->STATUS & STATICFLAG) == STATICFLAG)
            {
              thispointptr->ix = jx1;
              thispointptr->iy = jy1;
              thispointptr->iz = jz1;
              thispointptr->fx = (thispointptr->ix/Scale)+fxcenter; 
              thispointptr->fy = (thispointptr->iy/Scale)+fycenter; 
              thispointptr->fz = (thispointptr->iz/Scale)+fzcenter;
            }
            else
            {
              thispointptr->ix=(int)((jx1*ma[0][1]+jy1*ma[0][4]+jz1*ma[0][7]));
              thispointptr->iy=(int)((jx1*ma[0][2]+jy1*ma[0][5]+jz1*ma[0][8]));
              thispointptr->iz=(int)((jx1*ma[0][3]+jy1*ma[0][6]+jz1*ma[0][9]));
              thispointptr->fx=(thispointptr->ix/scalenew)+fxcenternew; 
              thispointptr->fy=(thispointptr->iy/scalenew)+fycenternew; 
              thispointptr->fz=(thispointptr->iz/scalenew)+fzcenternew;
            }
        if(snapplaces >= 0.01)
        {/*snapplaces*/
          thispointptr->fx =((int)(thispointptr->fx *(snapplaces)))/snapplaces;
          thispointptr->fy =((int)(thispointptr->fy *(snapplaces)))/snapplaces;
          thispointptr->fz =((int)(thispointptr->fz *(snapplaces)))/snapplaces;
                          
          /*back calculate integer coords*/  
          if(thispointptr->fx>=0.0) 
             thispointptr->ix=(int)(scalenew*(thispointptr->fx-fxcenternew)+.5);
          else 
             thispointptr->ix=(int)(scalenew*(thispointptr->fx-fxcenternew)-.5);
          if(thispointptr->fy>=0.0)
             thispointptr->iy=(int)(scalenew*(thispointptr->fy-fycenternew)+.5);
          else 
             thispointptr->iy=(int)(scalenew*(thispointptr->fy-fycenternew)-.5);
          if(thispointptr->fz>=0.0)
             thispointptr->iz=(int)(scalenew*(thispointptr->fz-fzcenternew)+.5);
          else 
             thispointptr->iz=(int)(scalenew*(thispointptr->fz-fzcenternew)-.5);
        }/*snapplaces*/

            /*drawnewlistptr->lastpoint = n; presumed????*/ 
                /*add this line to show list*/
            drawgrupptr->on = 1; /* turn on display */
            /*maxnxyz = n;*/ /*970410*/

          /*if(!Ldraglabelson)*/ /*991130*/
            if(!Lnewlabelson && !Lnewballson) /*991130,031127*/
            {/*adjust properties of the pair of points of this dragline*/
               (thispointptr->previousptr)->type = (VECTOR|MOVETO_P);  
                 /* Point == Move */
               thispointptr->type = (VECTOR);  /* Line == Draw of dragline*/ 
               sprintf(temps,"dragline");         
               storeptIDstring(temps, thispointptr); /*MAGELIST.c*/
            }        
            thispointptr->colorwidth = 0;  /*color default list color*/  
            drawnewlistptr->on = 1; /*insure: drawn lines are on*/  
            Ldraglineactive = 0; 
            Lpoint = 0; /*line fully formed*/       
            /*create pointID into this new position in com array */
            /*store directly in giant character array */
          /*labels 1 point: dist to last draw pt meaningless even when defined*/
          if(!Lnewlabelson && !Lnewballson)
          {/*update reported length of dragline*/
            dist= sqrt(
                   (   thispointptr->fx -(thispointptr->previousptr)->fx )
                    *( thispointptr->fx -(thispointptr->previousptr)->fx )
                  +(   thispointptr->fy -(thispointptr->previousptr)->fy )
                    *( thispointptr->fy -(thispointptr->previousptr)->fy )
                  +(   thispointptr->fz -(thispointptr->previousptr)->fz )
                    *( thispointptr->fz -(thispointptr->previousptr)->fz )  );

            sprintf(dstring,"%.3lf",dist);
            condition(dstring); 
          }/*update reported length of dragline*/

            sprintf(focusxyzstr,"  %.3f   %.3f   %.3f"
                , (float)thispointptr->fx
                , (float)thispointptr->fy
                , (float)thispointptr->fz);
            condition(focusxyzstr); /*____MAIN.C*/
            
          /*if(Lshiftkey && !Ltablepicked &&!Ldraglabelson)*/ /*991130*/
            if(Lshiftkey && !Ltablepicked && !Lnewballson /*031127*/
                         && !Lnewlabelson) /*991130*/
            {/*move first point to mouse position also, so can fine tune*/
                /* the start point of this new dragline */
                /*shift pick means something else for table picks */
                /*drag label only has one point*/
                (thispointptr->previousptr)->fx = thispointptr->fx;
                (thispointptr->previousptr)->fy = thispointptr->fy;
                (thispointptr->previousptr)->fz = thispointptr->fz;
                (thispointptr->previousptr)->ix = thispointptr->ix;
                (thispointptr->previousptr)->iy = thispointptr->iy;
                (thispointptr->previousptr)->iz = thispointptr->iz;
            }
        }/*Ldraglineactive: place free end at mouse position*/
        if(Lcenterinfo)
        {
           stringtoscreen(comstr,(2*ixctr/3),(ymax - iybottom)); /*____DRAW.c*/
           stringtoscreen(dstring,2*ixctr/3
                      ,(ymax-iybottom-iyfontsize-iyfontsize/2));
        }
        else
        {        
           stringtoscreen(comstr,(ixmin+5),(ymax - iybottom)); /*____DRAW.c*/
          if( iyfontsize <= 14)
          {  stringtoscreen(dstring,ixctr+5,(ymax-iybottom));
          }
          else    
          {  /*stringtoscreen(dstring,ixmin+5,(ymax-iybottom-iyfontsize));*/
            stringtoscreen(dstring,ixmin+5
                      ,(ymax-iybottom-iyfontsize-iyfontsize/2));
          }
        }

        if(magetimeticklimit > 0)
        {
            sprintf(word,"time remaining: %d : %d",magetimermin,magetimersec);
            condition(word); /*____MAIN*/
            stringtoscreen(word,ixmin+5,ymin+6*infofontsize);/*____DRAW*/
        }
        else if(Lrate)
        {
            sprintf(word,"%.3f",mousetickrate); 
            condition(word); /*____MAIN*/
            stringtoscreen(word,ixmin+5,ymin+5+iytop+iyfontsize);/*____DRAW*/
        }
        else if(Lwordstr)
        {/*if present: <comments> to screen, as well as {ptID}*/
            stringtoscreen(wordstr,ixmin+5,ymin+5+iytop+iyfontsize);/*____DRAW*/
        }/*goes into upper left, otherwise used for rate of image update*/
        else if(Legamickdocking && Legamickfooon) /*030323*/
        {
            sprintf(word,"foo: %d, bar: %d, ratio: %f, pass: %d"
                  ,Negamickfoocount,Negamickbarcount
                  ,(float)Negamickfoocount/(float)Negamickbarcount
                  ,Negamickpass);
            condition(word); /*____MAIN*/
            stringtoscreen(word,ixmin+5,ymin+5+iytop+iyfontsize);
        }
        else if(Lfoocavity) /*030331*/
        {
            sprintf(word,"foo: %d",Negamickfoocount);
            condition(word); /*____MAIN*/
            stringtoscreen(word,ixmin+5,ymin+5+iytop+iyfontsize);
        }
        if(currentaspect > 0 && Laspectonscreen && Laspectpass)
        {
            sprintf(word,"%s",aspect[currentaspect]); 
            condition(word); /*____MAIN*/
            stringtoscreen(word,ixmin+5,ymin+iytop+2*iyfontsize);/*____DRAW*/
        }

        ymini = 5; /*121213*/        
        if(Lsuitefittedstr) /*MAGEANGL.c.h  suitefit NDIM param 121211*/
        {
           stringtoscreen(suitefittedstr,xmin+5,ymin+5+iytop+iyfontsize);
           ymini = 20; /*121213*/
        }
        if(Lxyzpoint)
        {            
           stringtoscreen(focusxyzstr,ixctr+5,ymin+ymini+iytop+iyfontsize);
        }
        ymini = 5; /*121213*/        

        if(Lpipeactive)
        {/*appended stuff expected coming in from a pipe, print frame number*/
           stringtoscreen(framenumberstr,ixctr+5
                         ,(ymax-iybottom-iyfontsize-iyfontsize/2));
        }/*put to bottom of screen just above distance*/

  }/*ipick!=2 process further motion activity*/

    if(Leavingflatland) /*991117*/
    {/*reset integer translations into floating point center*/
            /*calculate inverse rotation matrix, see Lnewmatrix for discussion*/
                detmatrix = a11*a22*a33+a12*a23*a31+a13*a21*a32
                           -a13*a22*a31-a11*a23*a32-a12*a21*a33;
                ma[0][1] = (+a22*a33-a23*a32)/detmatrix;
                ma[0][4] = (-a21*a33+a23*a31)/detmatrix;
                ma[0][7] = (+a21*a32-a22*a31)/detmatrix;
                ma[0][2] = (-a12*a33+a13*a32)/detmatrix;
                ma[0][5] = (+a11*a33-a13*a31)/detmatrix;
                ma[0][8] = (-a11*a32+a12*a31)/detmatrix;
                ma[0][3] = (+a12*a23-a13*a22)/detmatrix;
                ma[0][6] = (-a11*a23+a13*a21)/detmatrix;
                ma[0][9] = (+a11*a22-a12*a21)/detmatrix;
             /*11=[1];12=[2];13=[3];21=[4];22=[5];23=[6];31=[7];32=[8];33=[9];*/
             /*use otherwise unused 0th matrix entries */
            fxcenterold[0] = fxcenternew; /*setup for undopickcenter*/
            fycenterold[0] = fycenternew;
            fzcenterold[0] = fzcenternew;
              newx = (int)((-ixtran*ma[0][1]-iytran*ma[0][4]-iztran*ma[0][7]));
              newy = (int)((-ixtran*ma[0][2]-iytran*ma[0][5]-iztran*ma[0][8]));
              newz = (int)((-ixtran*ma[0][3]-iytran*ma[0][6]-iztran*ma[0][9]));
              fxcenternew = (newx/scalenew)+fxcenternew; 
              fycenternew = (newy/scalenew)+fycenternew; 
              fzcenternew = (newz/scalenew)+fzcenternew;
if(Ltest)
{
  printf("old ctr %.3f,%.3f,%.3f intxyz %d, %d, %d  new ctr: %.3f, %.3f, %.3f\n"
  ,fxcenterold[0],fycenterold[0],fzcenterold[0],ixtran,iytran,iztran
  ,fxcenternew,fycenternew,fzcenternew);
}
              ixtran = 0;
              iytran = 0;
              iztran = 0;
              iztranold[0] = 0; 
            if(!Lzlimitscope) { resetgrafztranBar(iztranold[0]); /*___BAR.C*/ }
            Lrecalculate = 1; /*apply new fxyzcenter */
            Leavingflatland = 0;
    }/*reset integer translations into floating point center*/

if(LNDparallelaxes) {multipointparallelaxespost();}/*06111 paraAX screen front*/
/*printf("drawvec return\n");*/
}
/*___drawvec()______________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****processpick()************************************************************/
void processpick()
{
   int k=0,n=0,j=0;
   double dist;    
     static    float scaleitold,scaleitnew;
  static    int   pickxold=0,pickyold=0,pickzold=0;
  double  detmatrix;
  int     newx,newy,newz; 
/*  float tmps=0;  variables moved to placemarkers 061116 */
/*  float fudge;*/ /*020503*/
/*  int   marker=0;*/
/*  struct pointstruct *thispickedpointptr = NULL;*/ 

/*process pick, Lpick==1*/
if(Ltest){sprintf(alertstr,"MAGEDRAW/processpick()/Lpick\n");dosinglealert(3);}

    if(    Lpickcenter && Lpickcenteron 
        && (temppickedlistptr->STATUS & STATICFLAG)==0) 
    {/*recentering operation*/ /*non-static*/
       /*960809*/
       fxcenterold[0] = fxcenternew; /*setup for undopickcenter*/
       fycenterold[0] = fycenternew;
       fzcenterold[0] = fzcenternew;

       ctrpickedpointptr = temppickedpointptr; /*070315*/

       fxcenternew = temppickedpointptr->fx;
       fycenternew = temppickedpointptr->fy;
       fzcenternew = temppickedpointptr->fz;
       Lrecalculate = 1;
         ixtran = 0;             /*960219*/
         iytran = 0;
         if(!Lflat)
         {
             iztranold[0] = 0;  /*970915*/
             iztran = 0; /*970915*/
            if(!Lzlimitscope){resetgrafztranBar(iztranold[0]);/*___BAR.C*/}
         }
    }/*recentering operation*/ /*non-static*/
    if(!LmouseRdown)
    {/*not just Right mouse button orthogonal recentering*/
      /*NOW save the previous pickedpointptr to clear the way for this new one*/
        pickedlistoldptr = pickedlistptr; 
        pickedpointoldptr = pickedpointptr; 
        pickxold = pickx;
        pickyold = picky;
        pickzold = pickz;
      pickedgrupptr  = temppickedgrupptr;
      pickedsgrpptr  = temppickedsgrpptr;
      pickedlistptr  = temppickedlistptr;
      pickedpointptr = temppickedpointptr;
      /*reset pickx,y after final pick*/ /*960809*/
      /*make exact so can use as picked coord*/
      pickx = actualpickx;
      picky = actualpicky;
      pickz = actualpickz;

      if( !Ldraglineactive )
      {/*regular pick stuff*/
        if(pickedpointptr!=NULL)
        {/*pickedpointptr!=NULL*/
           
          if(Lfoosealer && Lfoosealeron)  /*030411*/
          {
             foosealer(pickedpointptr);
          }
           
          if(Ltablepickactive && !Ltablecellnewlypicked)
          {/*avoid repicking in table if pick was initiated in table*/
             if((pickedgrupptr->type & TABLEFLAG) == TABLEFLAG)
             {/*graphics copy of a table point has been picked*/
                Ltablegraphpicked = 3;
             }
             else
             {/*arbitrary graphics point has been picked*/
                Ltablegraphpicked = 2;
             }
          }
          /*stored pointID string NOT conditioned, i.e. has \0 */
          /*getptIDstring(comstr, pickedpointptr);*/
          recoverptIDstring(comstr, pickedpointptr); /*MAGELIST*/
        
          for(k=0;k<=MAXQCHAR-1;k++) /*MAXQCHAR #char in an answer*/
          {
            grafselection[0][k]=grafselection[1][k]; /*shift to previous*/
          }
          for(k=0;k<=MAXQCHAR-1;k++) /*MAXQCHAR #char in an answer*/
          {   /*use smallest of this constr and room in grafselection*/
            grafselection[1][k]=comstr[k]; /*load this pick point ID*/
            if(comstr[k]=='\0') break;
          }
          grafselection[1][MAXQCHAR-1]='\0'; /*insurance*/
          condition(grafselection[1]);/*dummy except for Mac*/  
          condition(comstr); /*dummy except for Mac*/  
          
          if(((pickedpointptr->type)&LABEL)==LABEL)  
          {/*labellist, ptID is redundant, see if has a <comment> */
            if(pickedpointptr->wordoffset > 0)
            {/*there is a comment, replace comstr*/
                getcommentstring(comstr, pickedpointptr);
                /*need to strip off the first three char*/
                k = -1;
                do
                {
                    k++;
                    comstr[k] = comstr[k+3];
                }while(comstr[k] != '\0');
                condition(comstr);
                Lwordstr = 0; /*no special wordlist screen output*/
                wordstr[0] = '\0';
            }/*there is a comment, replace comstr*/
          }/*labellist, ptID is redundant, see if has a <comment> */ 
          else if(    (pickedpointptr->wordoffset > 0)
                   &&((pickedgrupptr->type & TABLEFLAG) == 0) )
          {/*there is a comment, make extra wordstr, unless is a TABLE*/
                getcommentstring(wordstr, pickedpointptr);
                /*need to strip off the first three char*/
                k = -1;
                do
                {
                    k++;
                    wordstr[k] = wordstr[k+3];
                }while(wordstr[k] != '\0');
                condition(wordstr);
                Lwordstr = 1;
          }/*there is a comment, make extra wordstr*/
          else 
          {
             Lwordstr = 0; /*no special wordlist screen output*/
             wordstr[0] = '\0';
          }

/*20121208 Ltest*/

if(Ltest){sprintf(alertstr,"MAGEDRAW/processpick()/MAXNDIM %d\n",MAXNDIM);dosinglealert(3);}
if(Ltest){sprintf(alertstr,"MAGEDRAW/processpick()/pickedpointptr->listptr->NDIM== %d\n",pickedpointptr->listptr->NDIM);dosinglealert(3);}
if(Ltest) for(k=0; k<pickedpointptr->listptr->NDIM; k++)
{sprintf(alertstr,"MAGEDRAW/processpick()/pickedpointptr->coordNptr[%d]== %.3f, dimensionptr[%d].name== %s\n",k,pickedpointptr->coordNptr[k],k,dimensionptr[k].name);dosinglealert(3);}

        }/*pickedpointptr!=NULL*/
        else comstr[0]='\0';
        
        if(Lcenterinfo)
           {stringtoscreen(comstr,(2*ixctr/3),(ymax-iybottom) );} /*___DRAW.C */
        else 
           {stringtoscreen(comstr,(ixmin+5),(ymax-iybottom) );} /*___DRAW.C */
        if(Lwordstr)
        {
            stringtoscreen(wordstr,ixmin+5,ymin+5+iytop+iyfontsize);/*____DRAW*/
        }
        
        if(Lmarkers)
        {/*marker placing*/
           replacemarkers();  /*061116*/
        }/*marker placing*/
          
if(Ltest)
{sprintf(alertstr,"MAGEDRAW/processpick()/calc dist\n");dosinglealert(3);}
        /* markers now defined in original coord, but are unpickable */
        /* use original coord for distance calculation*/
        /* unless successive picks are between static and non-static lists*/
        if(pickedlistoldptr==NULL)
        {
            dist = 0.0;
        }
        else if(    ((pickedlistptr->STATUS)    & STATICFLAG) 
                 == ((pickedlistoldptr->STATUS) & STATICFLAG) ) 
        {
            dist=sqrt(  
              (oldxyz[1]-pickedpointptr->fx)*(oldxyz[1]-pickedpointptr->fx) +  
              (oldxyz[2]-pickedpointptr->fy)*(oldxyz[2]-pickedpointptr->fy) +
              (oldxyz[3]-pickedpointptr->fz)*(oldxyz[3]-pickedpointptr->fz)  );
        }
        else
        {
            if(((pickedlistptr->STATUS) & STATICFLAG) == STATICFLAG) 
                scaleitnew = Scale; 
            else scaleitnew = scalenew;
            if(((pickedlistoldptr->STATUS) & STATICFLAG) == STATICFLAG) 
                scaleitold = Scale; 
            else scaleitold = scalenew;
             dist=sqrt(  
                      ((float)pickxold/scaleitold-(float)pickx/scaleitnew)*
                      ((float)pickxold/scaleitold-(float)pickx/scaleitnew) +
                      ((float)pickyold/scaleitold-(float)picky/scaleitnew)*
                      ((float)pickyold/scaleitold-(float)picky/scaleitnew) +
                      ((float)pickzold/scaleitold-(float)pickz/scaleitnew)*
                      ((float)pickzold/scaleitold-(float)pickz/scaleitnew) 
                     );
        }
        if(pickedgrupptr->KKIND & SCALEDKKIND)  /*130309*/
        {/*only works if previous picked point is in same scaled group 130309*/
           dist = dist/pickedgrupptr->scaled; /*130309*/
        }
        
        /*this is the picked-pt-distance that is reported to the screen*/

        /*130303 NOTE: if pickedpoint is part of measures lines or dots */
        /*130303 then when domeasures permutes its lines and dots       */
        /*130303 pickedpointptr will point to a different point         */
        /*130303 SO oldxyz[1,2,3] must be reset before domeasures()     */
        oldxyz[1] = pickedpointptr->fx; /* use original coord */
        oldxyz[2] = pickedpointptr->fy;
        oldxyz[3] = pickedpointptr->fz;

        if(Lmeasureson) 
        {
            distmeasure = (float)dist; /*measures uses picked-pt-distance*/
                /*constructline needs global distmeasure from measures*/
            domeasures();  /*MAGEANGL.C*/
                /*puts anglemeasure,dihedralmeasure into common storage*/
            if(Lmeasureperpendics) /*030930*/
            {
               sprintf(dstring,"%.3lf, ang=%.3f, dhr= %.3f" /* .3 070514*/
                               ", line=%.3lf, plane=%.3lf"
                  ,distmeasure,anglemeasure,dihedralmeasure
                  ,perpendictoline,perpendictoplane);
            }
            else
            {
               sprintf(dstring,"%.3lf, ang=%.3f, dhr= %.3f", /* .3 070514*/
               distmeasure,anglemeasure,dihedralmeasure);
            }
        }
        else    sprintf(dstring,"%.3lf",dist);
if(Ltest){sprintf(alertstr,"MAGEDRAW/processpick()/dist->scrn\n");dosinglealert(3);}

        /*remember distance between last 2 picked points in global*/
        distbetween = (float)dist;

        condition(dstring);  /* for Mac make it a Pascal string */
           /*MACDRAW.C & MPCDRAW.C*/
        if(Lcenterinfo)
        { stringtoscreen(dstring,2*ixctr/3
            ,(ymax-iybottom-iyfontsize-iyfontsize/2));
        }
        else
        {
           if( iyfontsize <= 14)
           { stringtoscreen(dstring,ixctr+5,(ymax-iybottom));
           }
           else    
           { stringtoscreen(dstring,ixmin+5
            ,(ymax-iybottom-iyfontsize-iyfontsize/2));
                 /*MACDRAW.C & MPCDRAW.C*/
           }
        }


#ifdef OLDRECENTERINGPLACE
        if(Lpickcenter && Lpickcenteron && (pickedlistptr->STATUS & STATICFLAG)==0) 
        {/*recentering operation*/ /*non-static*/
          /*960809*/
          fxcenterold[0] = fxcenternew; /*setup for undopickcenter*/
          fycenterold[0] = fycenternew;
          fzcenterold[0] = fzcenternew;

          fxcenternew = pickedpointptr->fx;
          fycenternew = pickedpointptr->fy;
          fzcenternew = pickedpointptr->fz;
          Lrecalculate = 1;
            ixtran = 0;             /*960219*/
            iytran = 0;
            if(!Lflat)
            {
                iztranold[0] = 0;  /*970915*/
                iztran = 0; /*970915*/
               if(!Lzlimitscope){resetgrafztranBar(iztranold[0]);/*___BAR.C*/}
            }
        }
#endif
        if(  Ldrawstuff
           &&(  Lnewlineson||Lconstruct4on||Lconstruct5on
              ||Lconstruct6on            /*140912*/
              ||(Lnewballson&&Ltestmode)
              ||Ldock3on3on ||(Lnewlabelson&&Ltestmode)))
        {/*line stuff: Ldrawstuff TRUE, including labels stuff*/  /*020904*/
            /*labels cannot be dragged when in testmode, scoring is by orig coords*/
          Lpoint++; /*increased flag, also needed for end of pointers*/

          /*Lpoint is now set for current state:*/
          Ldrawactive = 1; /*at least this one thing actually happened*/
          if(Lnewlineson) n = 2; 
          /*if(Lnewlabelson) n = 2;*/
          if(Lconstruct4on) n = 4; /*971122 */
          if(Lconstruct5on) n = 5; /*971122 */
             /*constructs either 4th or 5th pt*/
          if(Lconstruct6on) n = 7; /*140912, 7 141126 */
             /*constructs measures-like dihedral plus perp line to axis 140912*/
          if(Ldock3on3on) n = 6; /*020305*/

          if(Lpoint <= n) 
          {/*put drawmarker on picked point*/
              /* Lpoint counts 1,2,3 for points picked to make a line*/
              /* up to 4 for construction of fifth point or 5--6 line*/
              /* up to 6 for dock3on3*/  /*020305*/
              /*drawline has 1 such point, constructline has 3 */
              /*put successive drawmarkers on each newly picked point*/
              /* as Lpoint indicates */
              /*k = n + nxyz + NMARKERS + MAXMEASURES + Lpoint*NDRAWPOINT;*/
              if(Lpoint==1)
              {
                  thislistptr = drawmarker1listptr;
              }
              if(Lpoint==2)
              {
                  thislistptr = drawmarker2listptr;
              }
              if(Lpoint==3)
              {
                  thislistptr = drawmarker3listptr;
              }
              if(Lpoint==4)/*971122*/
              {
                  thislistptr = drawmarker4listptr;
              }
              if(Lpoint==5)/*020305*/
              {
                  thislistptr = drawmarker5listptr;
              }
              if(Lpoint==6)/*020305*/
              {
                  thislistptr = drawmarker6listptr;
              }

              thislistptr->STATUS = (pickedlistptr->STATUS & STATICFLAG);
                  /*just copy the STATICFLAG part*/
              origlistptr = drawmarkeroriglistptr;
              origpointptr = origlistptr->firstpointptr;
              thispointptr = thislistptr->firstpointptr;

              while(thispointptr != NULL)
              {
                thispointptr->type = origpointptr->type;
                thispointptr->colorwidth = origpointptr->colorwidth;
                /*recreate pseudo-original float markers */
                /*at new position*/
                thispointptr->fx = origpointptr->fx + pickedpointptr->fx;
                thispointptr->fy = origpointptr->fy + pickedpointptr->fy;
                thispointptr->fz = origpointptr->fz + pickedpointptr->fz;
                /*reset the integer-scaled,centered coords*/
                if((pickedlistptr->STATUS & STATICFLAG) == 0) /*non-static*/
                {
                  calcintegerpoints(thispointptr);
                }
                else  /*hit a static list*/
                {
                  calcintegerfixedpoints(thispointptr);
                }
                if(  thispointptr == thislistptr->lastpointptr 
                   ||origpointptr == origlistptr->lastpointptr )
                {
                     thispointptr = NULL;
                }
                else
                {
                     origpointptr = origpointptr->nextptr;
                     thispointptr = thispointptr->nextptr;
                }
              }
              thislistptr->on = 1; /*turn on point indicater*/ 
          }/*put drawmarker on picked point*/
          if(Lnewlineson ||(Lnewballson&&Ltestmode)||(Lnewlabelson&&Ltestmode))
          {
              dodrawline(); /*MAGEANGL.C*/
          }
          if(Lconstruct4on || Lconstruct5on || Lconstruct6on) /*6:140912*/
          {
              doconstructline();    /*MAGEANGL.C*/
                 /*stores points for 3 or 4 calls,971122, up to 5 140912*/
                 /*called here to load point info, called later to construct*/
                 /*line from construct4 & construct5 dialog, construct6 140912*/
          }
          if(Ldock3on3on) /*020305*/
          {
              dodock3on3(); /*MAGEANGL*/  /*020305*/
              Lrecalculate = 1; /*apply new docking */
          }
        }/*line stuff: Ldrawstuff TRUE, including Labels stuff*/
        if(Lpickeraseon ||(Lprune && (Lpruneon || Lpunchon || Lsuperpunchon)) )
        {/*prune and stuff*/
          /*discriminate points that are in groups that are not pruneable*/
          if(      pickedpointptr->listptr != NULL 
              &&  (pickedpointptr->listptr)->sgrpptr != NULL
              && ((pickedpointptr->listptr)->sgrpptr)->grupptr != NULL
              &&(((pickedpointptr->listptr)->sgrpptr)->grupptr)!=measuregrupptr
              &&(((pickedpointptr->listptr)->sgrpptr)->grupptr)!=markergrupptr
              &&(((pickedpointptr->listptr)->sgrpptr)->grupptr)!=tablemarkergrupptr
            )
          {/*OK group within which to prune points*/
            Lpruneactive = 1; /*at least this one thing actually happened*/
            nundop++; /*set undo information */
            if(nundop > 10)
            {
                for(j=1;j<=9;j++)
                {
                    prunepoint1ptr[j] = prunepoint1ptr[j+1];
                    prunepoint2ptr[j] = prunepoint2ptr[j+1];
                    prunepointflag[j] = prunepointflag[j+1];
                }
                nundop = 10;
            }
            if(Lpruneon)
            {
                prunepoint1ptr[nundop] = pickedpointptr;/*at least this one*/
                prunepoint2ptr[nundop] = pickedpointptr;
                prunepointflag[nundop] = 0;
                thispointptr = pickedpointptr;
                if(   ((thispointptr->type)&VECTOR  )
                          &&!((thispointptr->type)&MOVETO_P) )
                /*a DRAW operation,(NOTE the NOT==! flag!!!)*/
                {/*prune back to initiating P == MOVE operation*/
                  
                    thispointptr = pickedpointptr->previousptr;
                    while(   thispointptr!=NULL
                          && thispointptr->listptr==pickedpointptr->listptr
                          && ((thispointptr->type)&VECTOR  )
                          &&!((thispointptr->type)&MOVETO_P) )
                    {/*a DRAW operation*/
                        thispointptr->type = ((thispointptr->type)|PRUNED);
                        prunepoint1ptr[nundop] = thispointptr;
                        
                        thispointptr = thispointptr->previousptr;
                    }
                    /*see if there is an initiating move to prune*/
                    /*there will always be one if a DRAW was picked */
                    /* MAGE always assigns a MOVETO_P to first point of a*/
                    /* vectorlist, so can not run back into previous list*/
                    /*But check for list identity anyway...*/ /*970609*/
                    /*as well as existance*/
                    if(    thispointptr!=NULL
                        && thispointptr->listptr==pickedpointptr->listptr
                        && ((thispointptr->type)&VECTOR  )
                        && ((thispointptr->type)&MOVETO_P) )
                    {/*now isolated vector move, prune it*/                    
                        thispointptr->type = ((thispointptr->type)|PRUNED);
                        prunepoint1ptr[nundop] = thispointptr;
                    }
                }
                pickedpointptr->type = ((pickedpointptr->type)|PRUNED);
                    /*prune this one*/
                /*prune forward*/
                thispointptr = pickedpointptr->nextptr;
                j = pickvec + 1;
                while(   thispointptr!=NULL
                      && thispointptr->listptr==pickedpointptr->listptr
                      && ((thispointptr->type)&VECTOR  )
                      &&!((thispointptr->type)&MOVETO_P) )
                           /*a DRAW operation*/
                {
                    thispointptr->type = ((thispointptr->type)|PRUNED);
                    prunepoint2ptr[nundop] = thispointptr;

                    thispointptr = thispointptr->nextptr;
                    /* MAGE always assigns a MOVETO_P to first point of any*/
                    /* list, so can not run forward into next list*/
                    /*But check for list identity anyway...*/ /*970609*/
                    /*as well as existance*/
                }
                prunepointflag[nundop] = 0; /* nop on next move*/
            }
            else if(Lpickeraseon || Lpunchon || Lsuperpunchon)
            {/*punch operation*/
              pickedpointptr->type = ((pickedpointptr->type)|PRUNED);
              prunepoint1ptr[nundop] = pickedpointptr;
              prunepoint2ptr[nundop] = pickedpointptr;/*just one punched out*/
              prunepointflag[nundop] = 0; /*initially nop on next vector*/
              thispointptr = pickedpointptr;
              /*look forward*/
              if( (thispointptr->type)&VECTOR  )
              {/*look forward for DRAW operation*/
               /*this is only a valid thing to do within a list*/
               /* MAGE robustly flag first points in a list as MOVETO_P */
               /*But check for list identity anyway...*/
               if(  pickedpointptr->nextptr!=NULL  /*970609*/
                  &&pickedpointptr->listptr==(pickedpointptr->nextptr)->listptr)
               {/*next is in the same list, see if it is a continuing Line */
                /*this has two considerations, first is the next point*/
                /*a DrawTo one, if so, then is there a DrawTo beyond that*/
                /*to justify redefining the first one as a MoveTo*/
                /*If not, then that next DrawTo is isolated and should*/
                /*be pruned away */
                if(     (((pickedpointptr->nextptr)->type)&VECTOR  )
                    && !(((pickedpointptr->nextptr)->type)&MOVETO_P))
                {/*next is a DrawTo, must look ahead to see if connected*/
                 if(   (pickedpointptr->nextptr)->nextptr != NULL /*970604*/
                     &&(pickedpointptr->listptr == 
                         ((pickedpointptr->nextptr)->nextptr)->listptr) )
                 {/*one-beyond-next exists in same list, maybe next NOT orphan*/
                  if(   ((((pickedpointptr->nextptr)->nextptr)->type)&VECTOR  )
                     &&!((((pickedpointptr->nextptr)->nextptr)->type)&MOVETO_P))
                  {/*DRAW operation that continues next--- one-beyond-next*/
                   /*change next to be a MOVETO_P*/
                        (pickedpointptr->nextptr)->type 
                            = (((pickedpointptr->nextptr)->type)|MOVETO_P); 
                        prunepointflag[nundop] = 1;
                        /*flag that the next-beyond pruned was changed*/
                  }/*DRAW operation that continues next--- one-beyond-next*/
                  else
                  {/*DRAW operation isolate, so prune it*/
                        (pickedpointptr->nextptr)->type 
                            = (((pickedpointptr->nextptr)->type)|PRUNED);
                        prunepoint2ptr[nundop] = pickedpointptr->nextptr;
                  }/*DRAW operation isolate, so prune it*/
                 }/*one-beyond-next exists in same list, maybe next NOT orphan*/
                 else
                 {/*DRAW operation isolate, so prune it*/
                        (pickedpointptr->nextptr)->type 
                            = (((pickedpointptr->nextptr)->type)|PRUNED);
                        prunepoint2ptr[nundop] = pickedpointptr->nextptr;
                 }/*DRAW operation isolate, so prune it*/
                }/*next is a DrawTo, must look ahead to see if connected*/
               }/*next is in the same list, see if it is a continuing Line */
              }/*look forward for DRAW operation*/
               
              /*look back*/
              if(     pickedpointptr->previousptr != NULL  /*970604*/
                  && (pickedpointptr->listptr == 
                           (pickedpointptr->previousptr)->listptr) )
              {/*previous is in the same list, see if it is a Line start*/
                if(   (((pickedpointptr->previousptr)->type)&VECTOR  ) 
                   && (((pickedpointptr->previousptr)->type)&MOVETO_P))
                {/*now isolated vector move, prune it*/
                    (pickedpointptr->previousptr)->type 
                        = (((pickedpointptr->previousptr)->type)|PRUNED);
                    prunepoint1ptr[nundop] = pickedpointptr->previousptr;
                } 
              }/*previous is in the same list, see if it is a Line start*/
            }/*punch operation*/
          }/*OK group within which to prune points*/
          else if(Lsuperpunchon)
          {/*endless auger: should not be able to get here as of 040320*/
fprintf(stderr,"Endlessly trying to auger non-deletable points... \n");
          }
        }/*prune and stuff*/

        if(  (LSELECTINGliston)
           &&(pickedpointptr->listptr->KKIND & SELECTINGKKIND)
          )
        {
           actualSELECTINGtask(); /*061029 selects one or all within a radius*/
        }

        if(Lemphasison)
        {/*emphasis stuff*/
           if(    (pickedpointptr->type & VECTOR)
              && !(pickedpointptr->type & VARIANT1)
              && !(pickedpointptr->type & VARIANT2)
              && !(pickedpointptr->type & VARIANT3)
             )
           {/*for now, only can emphasis plain vectors*/
              if(pickedpointptr->STYLE & EMPHASIS)
              {/*remove emphasis by 2's complement of EMPHASIS*/
                 pickedpointptr->STYLE = pickedpointptr->STYLE & ~EMPHASIS;
              }
              else
              {
                pickedpointptr->STYLE = pickedpointptr->STYLE | EMPHASIS;
              }
           }
        }/*emphasis stuff*/

        /*can't reset Lpick yet since may be needed to redraw markers */
        /* but ipick can be reset to avoid re-entry to here */
        /* if DopickshowDialog() does a redraw of the vectors */
        ipick = 1;
        Lprepick = 0; /*clear flag that might have been set by FIND dialog*/
        /*if(Lpickcoloron) DocolorshowDialog();*/ /*MACDLOG.C*/ /*MPCDLOG.C*/
        /*if(Lpickshowon)  DopickshowDialog(); */ /*MACDLOG.C*/ /*MPCDLOG.C*/
        /*moved to ___MAIN so Do___Dialogs work non-modal e.g. UNIX */
        /*MUXMMAIN/pick_CB() invoked by mousedown in graphics drawArea*/
      }/*regular pick stuff*/
        if(Ldraglineon||(Lnewlabelson&&!Ltestmode)||(Lnewballson&&!Ltestmode))
        /*991130 labels can be dragged*/
        /*020904 dragged labels cannot be scored*/
        /*but a newly made dragline cannot be labeled!!!! 000130 */
        {/*Ldraglineon: set a point from which to drag a (force) line */
            Ldrawactive = 1; /*at least this one thing actually happened*/
            if(Lnewmatrix)
            {/*calculate inverse rotation matrix*/
                detmatrix = a11*a22*a33+a12*a23*a31+a13*a21*a32
                           -a13*a22*a31-a11*a23*a32-a12*a21*a33;
                ma[0][1] = (+a22*a33-a23*a32)/detmatrix;
                ma[0][4] = (-a21*a33+a23*a31)/detmatrix;
                ma[0][7] = (+a21*a32-a22*a31)/detmatrix;
                ma[0][2] = (-a12*a33+a13*a32)/detmatrix;
                ma[0][5] = (+a11*a33-a13*a31)/detmatrix;
                ma[0][8] = (-a11*a32+a12*a31)/detmatrix;
                ma[0][3] = (+a12*a23-a13*a22)/detmatrix;
                ma[0][6] = (-a11*a23+a13*a21)/detmatrix;
                ma[0][9] = (+a11*a22-a12*a21)/detmatrix;
             /*11=[1];12=[2];13=[3];21=[4];22=[5];23=[6];31=[7];32=[8];33=[9];*/
             /*use otherwise unused 0th matrix entries */
                Lnewmatrix = 0;

             /*040430 matrix elements now (double) */
             /*this just exchanges elements across the diagonal*/
             /*FOLEY pg 1102 coord matrix is orthonormal basis*/
             /*pg 1106: if columns of matrix are orthonormal basis, then*/
             /*transpose of matrix is the inverse matrix*/
             /*Note matrixinvert(N); NxN A inverted into C, A C globals*/

            }/*calculate inverse rotation matrix*/

            if(Lnewdrag)
            {/*see if new drag or pick up of previous drag*/
              Lnewdrag = 0; /*clear this flag*/
              
              if(  (pickedpointptr!=(drawnewlistptr->lastpointptr))
                 ||(Ladddrag) )   
              {/*new drag: make a new end on the picked point*/
                /*copy char str from storage */
                /*getptIDstring(word, pickedpointptr);*/
                recoverptIDstring(comstr, pickedpointptr); /*MAGELIST*/
                if(Lshiftkey && !Ltablepicked)
                {/*places both ends at mouse position*/
 
                    /*pickz = izset;*/ /*point in screen plane*/
                    /*jx1 = pickx-ixset;*/
                    /*jy1 = picky-iyset;*/
                    /*jz1 = pickz-izset;*/
                    jx1 = actualpickx-ixset; /*040503 put pts at actual pick*/
                    jy1 = actualpicky-iyset;
                    jz1 = actualpickz-izset; /*040503 keep pts at pick depth*/
             /*11=[1];12=[2];13=[3];21=[4];22=[5];23=[6];31=[7];32=[8];33=[9];*/
                    newx = (int)((jx1*ma[0][1] + jy1*ma[0][4] + jz1*ma[0][7]));
                    newy = (int)((jx1*ma[0][2] + jy1*ma[0][5] + jz1*ma[0][8]));
                    newz = (int)((jx1*ma[0][3] + jy1*ma[0][6] + jz1*ma[0][9]));
                    x2 = (newx/scalenew)+fxcenternew; /*x2,y2,z2 globals*/
                    y2 = (newy/scalenew)+fycenternew; 
                    z2 = (newz/scalenew)+fzcenternew;
                    if(snapplaces >= 0.01)
                    {
                            x2 = ((int)(x2*(snapplaces)))/snapplaces;
                            y2 = ((int)(y2*(snapplaces)))/snapplaces;
                            z2 = ((int)(z2*(snapplaces)))/snapplaces;
                    }                  
                }/*places both ends at mouse position*/
                /*now without the shiftkey, can drag out free end from there*/
                
                Lpoint = 0;
                drawmarker1listptr->on = 0; /*turn off point indicater*/ 
                drawmarker2listptr->on = 0; /*turn off point indicater*/ 
                drawmarker3listptr->on = 0; /*turn off point indicater*/ 
                drawmarker4listptr->on = 0; /*turn off point indicater*/ 
                drawmarker5listptr->on = 0; /*turn off point indicater 020305*/ 
                drawmarker6listptr->on = 0; /*turn off point indicater 020305*/ 
                dodrawline(); /*create base point*/
                Lpoint = 1; /*one point of a line*/

                if(Lnewlabelson||Lnewballson) Lpoint = 0; 
                           /*NOT one point of a line 000130*/

                /*numberline++; do this on actual drag*/ 
                    /*augment so ready for second, the dragged, point*/
              }/*new drag: make a new end on the picked point*/
              
            }/*see if new drag or pick up of previous drag*/
            
        }/*Ldraglineon: set a point from which to drag a (force) line */
        
        if(Lshiftkey && !Ltablepicked)
        {
            sprintf(focusxyzstr,"  %.3f   %.3f   %.3f"
                , x2
                , y2
                , z2);
        }
        else   
        {
          /*130303 domeasures() permutes its lines and dots, */
          /*130303 if pickedpoint is one of these, then must use oldxyz */
          /*130303 which is reset before calling domeasures() */
            sprintf(focusxyzstr,"  %.3f   %.3f   %.3f"
                , oldxyz[1],oldxyz[2],oldxyz[3]); /*130303*/
              /*  , (float)pickedpointptr->fx   */
              /*  , (float)pickedpointptr->fy   */
              /*  , (float)pickedpointptr->fz); */
        }
        condition(focusxyzstr); /*____MAIN.C*/
 
        if(Lxyzpoint)
        {
             stringtoscreen(focusxyzstr,ixctr+5,ymin+5+iytop+iyfontsize);
        }        
        if(Lpipeactive)
        {/*appended stuff expected coming in from a pipe, print frame number*/
           stringtoscreen(framenumberstr,ixctr+5
                         ,(ymax-iybottom-iyfontsize-iyfontsize/2));
        }/*put to bottom of screen just above distance*/
if(Ltest){sprintf(alertstr,"MAGEDRAW/processpick()/Lpick end\n");dosinglealert(3);}
    }/*not just Right mouse button orthogonal recentering*/
}
/*___processpick()___________________________________________________________*/


