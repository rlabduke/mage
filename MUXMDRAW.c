/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUXMDRAW.c*/
#include "MAGE.h"   
#include "MAGELIST.h"
#include "MAGETABL.h"
#include "MAGESYNC.h" /*finger*/

    int ifontsize = 0; /*061117*/
    float testx = 0; /*061120*/
    float testy = 0; /*061120*/

/*Try to gather all OS specific calls to the top of this file */
/*view-in-mind of more sections of common code*/

/****offscreenclear()********************************************************/
void    offscreenclear() /*heavy platform specific stuff: */ 
{
    if(Lerase) 
    {
        if(Lwhitebkg) { XSetForeground(dpy,offgc,incolor[WHITEINDEX]); }
        else          { XSetForeground(dpy,offgc,incolor[BLACKINDEX]); }

        XFillRectangle(dpy,offscreen,offgc,0,0,GWIDTH,GHEIGHT);

        if(Lwhitebkg) { XSetForeground(dpy,offgc,incolor[BLACKINDEX]); }
        else          { XSetForeground(dpy,offgc,incolor[WHITEINDEX]); }
    }
}
/*___offscreenclear()_______________________________________________________*/

/****rectangleclear()********************************************************/
void    rectangleclear(int x1, int y1, int x2, int y2) /*platform specific */ 
{
        if(Lwhitebkg) { XSetForeground(dpy,offgc,incolor[WHITEINDEX]); }
        else          { XSetForeground(dpy,offgc,incolor[BLACKINDEX]); }

        XFillRectangle(dpy,offscreen,offgc,x1,y1,x2,y2);

        if(Lwhitebkg) { XSetForeground(dpy,offgc,incolor[BLACKINDEX]); }
        else          { XSetForeground(dpy,offgc,incolor[WHITEINDEX]); }
}
/*___offscreenclear()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****preparegrafwindow()*****************************************************/
void    preparegrafwindow() /*platform specific stuff: */ 
{
        if(Lwhitebkg) 
        {
            XSetForeground(dpy,offgc,incolor[BLACKINDEX]);
            XSetBackground(dpy,offgc,incolor[WHITEINDEX]);
        }
        else
        {
            XSetForeground(dpy,offgc,incolor[WHITEINDEX]);
            XSetBackground(dpy,offgc,incolor[BLACKINDEX]);
        }
}
/*___preparegrafwindow()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****setpencolor()***********************************************************/
void setpencolor(icolor)
{/*set foreground*/ /*color and defined color cell*/
   int Lset=0;

   if(Lcolor)
   {
      if(Lgrayscale && ingrayned[icolor])
         {XSetForeground(dpy,offgc,ingrayn[icolor]);  Lset=1; }
      else if(incolored[icolor])
         {XSetForeground(dpy,offgc,incolor[icolor]);  Lset=1; }
   }
   if(!Lset)
   {  /*051201 DEADBLACKINDEX, DEADWHITEINDEX */
      /*BLACKINDEX can be reset by MUXMDLOG, so use predefined DEADBLACKINDEX*/
      if(icolor==255)    {XSetForeground(dpy,offgc,incolor[DEADWHITEINDEX]);}
      else if(icolor==0) {XSetForeground(dpy,offgc,incolor[DEADBLACKINDEX]);}
      else if(Lwhitebkg) {XSetForeground(dpy,offgc,incolor[BLACKINDEX]);}
      else               {XSetForeground(dpy,offgc,incolor[WHITEINDEX]);}
   }
}
/*___setpencolor()__________________________________________________________*/

/****setpentoscreenforeground()************************************************/
void setpentoscreenforeground()
{
    if(Lwhitebkg) {XSetForeground(dpy,offgc,incolor[BLACKINDEX]);}
    else          {XSetForeground(dpy,offgc,incolor[WHITEINDEX]);}
}
/*___setpentoscreenforeground()_______________________________________________*/

/****setpentohighlight()*******************************************************/
void setpentohighlight()
{
   if(Lcolor)         {XSetForeground(dpy,offgc,incolor[WHITEINDEX]);}
   else if(Lwhitebkg) {XSetForeground(dpy,offgc,incolor[WHITEINDEX]);}
   else               {XSetForeground(dpy,offgc,incolor[BLACKINDEX]);}
}
/*___setpentohighlight()______________________________________________________*/

/****copytoscreen()**********************************************************/
  /* copy bitmap into front window to see what has just been drawn */
void    copytoscreen() 
{ 
    XCopyArea(dpy,offscreen,drawWindow,offgc,0,0,GWIDTH,GHEIGHT,0,0);
}
/*___copytoscreen()_________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****redrawgraphics()********************common name: MAC & PC***************/
void  redrawgraphics() {redrawvec();}  /* equivalence Mac vs PC MAGE*/
/*___redrawgraphics()_______________________________________________________*/




/****redrawvec()*****************common name: MAC & PC**********************/
void    redrawvec()
{
   Lredrawvector = 0; /*030329*/

   Lrecalculate = 0; /*need here since UNIX_PEX doesn't do it below */
   do 
   {
      predrawticks = (theclocktickcount() - icountticks);
      icountticks = theclocktickcount();

#ifndef UNIX_PEX
          if(Lplotonly)     Lerase = FALSE;
          else            Lerase = TRUE;
          if(Lfirstcall){ Lerase = TRUE; Lfirstcall = FALSE; }

          if(LGUI) offscreenclear();
          /*XClearArea(dpy,drawWindow,0,0,GWIDTH,GHEIGHT,False);*/
          Lrecalculate = 0;         /*need here to allow redrawing to flag it*/
          drawvec(); /*MAGEDRAW.C*/
          if(LGUI) preparegrafwindow();

          if(LGUI) copytoscreen();
#endif /*UNIX_PEX*/

      postcopyticks = (theclocktickcount() - icountticks);
      icountticks = theclocktickcount();
      if(Lrecalculate) rescalekinemage();    /*MAGEINPT.C*/
   }while(Lrecalculate);
   if(Lfingerin) fingerplacer(0); /*MUXMMAIN.c*/
}
/*___redrawvec()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****stringtoscreen()*********************common name: MAC & PC**************/
void    stringtoscreen(char string[256],int ix,int iy)
{
    int ifontsize = 0; /*010529*/
    
 if(LGUI)
 {/*LGUI*/

    /*if(infofontsize != oldfontsize)*/
    {
      if(Lscalefonts)
      {
         ifontsize = (2*infofontsize);
      }
      else
      {
         ifontsize = (infofontsize);
      }

        if(ifontsize <= littlefontsize && littlefont)
        {
            XSetFont(dpy,offgc,littlefont->fid);
        }
        else if(ifontsize <= mediumfontsize && mediumfont)
        {
            XSetFont(dpy,offgc,mediumfont->fid);
        }
        else if(ifontsize >= largefontsize && largefont)
        {
            XSetFont(dpy,offgc,largefont->fid);
        }
        else XSetFont(dpy,offgc,littlefont->fid); /*most likely exists*/
    }
    setpentoscreenforeground();
    XDrawString(dpy,offscreen,offgc,ix,iy,string,strlen(string));
    oldfontsize = infofontsize; 
    
 }/*LGUI*/

}
/*___stringtoscreen_________________________________________________________*/

/****labeltoscreen()**************************common name: MAC & PC**********/
void    labeltoscreen(char string[256],int ix,int iy,int icolorit)
{                                /* for labels */ /*PC has to set color */
    int ifontsize = 0; /*010529*/
    
    /*if(labelsfontsize != oldfontsize)*/
    {
      if(Lscalefonts)
      {
         ifontsize = (2*labelsfontsize);
      }
      else
      {
         ifontsize = (labelsfontsize);
      }

        if(ifontsize <= littlefontsize && littlefont)
        {
           XSetFont(dpy,offgc,littlefont->fid);
        }
        else if(ifontsize <= mediumfontsize && mediumfont)
        {
           XSetFont(dpy,offgc,mediumfont->fid);
        }
        else if(ifontsize >= largefontsize && largefont)
        {
           XSetFont(dpy,offgc,largefont->fid);
        }
        else XSetFont(dpy,offgc,littlefont->fid); /*most likely exists*/
    }
    setpencolor(icolorit);
    XDrawString(dpy,offscreen,offgc,ix,iy,string,strlen(string));
    oldfontsize = labelsfontsize;
}
/*___labeltoscreen__________________________________________________________*/

/****killzoneonscreen()********************************************************/
void killzoneonscreen(void) /*990211*/
{
        setdrawcolorpen(2,2,3);
        killradius = 20;
        XDrawArc(dpy,offscreen,offgc
                    ,mousexold-killradius 
                    ,mouseyold-killradius
                    ,2*killradius
                    ,2*killradius
                    ,0,360*64);
}
/*___killzoneonscreen()_______________________________________________________*/

/****SELECTINGzoneonscreen()**************************************************/
void SELECTINGzoneonscreen(void) /*060123*/
{
  /*scalenew = Scale*zoom; Scale = oldmaxwide/(fdiff); MAGEINPT/scalekinemage*/

        setdrawcolorpen(2,2,3);
        XDrawArc(dpy,offscreen,offgc
                    ,mousexold-(radiusSELECTING * scalenew)
                    ,mouseyold-(radiusSELECTING * scalenew)
                    ,2*(radiusSELECTING * scalenew)
                    ,2*(radiusSELECTING * scalenew)
                    ,0,360*64);
}
/*___SELECTINGzoneonscreen()_________________________________________________*/

/****balltoscreen()*****************************common name: MAC & PC*********/
void balltoscreen(int xplace,int yplace,int idisk,int intradius,int Lblackrim
                 ,int ipen,int icolor,int type, int LTBstereo)
{/*combined with  TBstereo_balltoscreen 040918*/
  int    iradius=0, inner=0, outer=0, expand=0, iok=1, ky=1;

  iradius = intradius; /*convert int to short for Mac*/    
  if(LTBstereo) {ky = 2;} /*divisor for y vs x dimension 040918*/  

  /*if(idisk == 5)*/ 
  if(idisk == 8 || idisk == 9) /*for 9 disks, highlight is on 7th 040918*/
  {/*keep ultimate capping disk from obscuring highlight on penutimate disk*/
      if(iradius == 2) iradius = 1; /*2: drop to 1*/
      else if(iradius <=1) iok = 0; /*0: don't draw*/
  }
  if(iok) /*030626*/
  {/*iok*/
    /*ipen==1 for thinline, 2: onewidth, 1---5 multiwide*/ /*981015*/
    /*X11 lines really stay within one pixel of centered on coord pixel*/
    /* so expand ball evenly around coord */
    if(ipen > 1) expand = ipen - 1; /*981015*/
    else expand = 0;
    /*limit sum of radius and expand to >= 1 so balls always visible 040918*/
    while(iradius+expand < 1) {iradius++;} /*040918*/

    if(iradius+expand > 0) /*030626*/
    {/*draw disk of finite radius*/
      if(Lblackrim && (idisk==0 || idisk==1)) /*040925*/
      {/*plain old ball==disk, but Jane wants black edges on my balls 040918*/
         setpencolor(0); /*black, draw disk 1 pixel bigger in radius*/
         XFillArc(dpy,offscreen,offgc
                          ,xplace-(iradius+expand+1)
                          ,yplace-(iradius+expand+1)/ky
                          ,2*(iradius+expand+1)
                          ,2*(iradius+expand+1)/ky
                          ,0,360*64);
         {setpencolor(icolor);} /*____DRAW.c restore working color*/
      }
      if(Lwhitebkg && idisk!=0) /*orig depth cueing kept for plain balls*/
      {/*use blk palatte 040918*/
         if(icolor<251 && icolor>25)
         {
            setpencolor(icolor-25);
         }
      }
      XFillArc(dpy,offscreen,offgc
                          ,xplace-(iradius+expand)    /*981015*/
                          ,yplace-(iradius+expand)/ky /*981015*/
                          ,2*(iradius+expand)         /*981015*/
                          ,2*(iradius+expand)/ky      /*981015*/
                          ,0,360*64);
      if(Lwhitebkg){setpencolor(icolor);} /*____DRAW.c restore working color*/
      /*if(type==1 &&(idisk == 0 || idisk == 4))*/ /*type 7 is NOHIGHLIGHT*/
      /*point type 7 is for NOHIGHLIGHT, type==1 for highlight on ball,sphere*/
      if(type==1 &&(idisk == 0 || idisk == 7)) /*040918 7th disk carries spot*/
      {/*highlight if only disk or the one that carries highlight 040918*/
        /* used to be on the penultimate annulus*/
        /*either pure disk */
        /*or near-to-top disk of fudged space-fill stacked disks*/ 
        /*adjust iradius for the highlight*/
        if(idisk == 0)
        {
          /*iradius = iradius/2;*/ /*pure single disk, old case*/
          outer = (9*iradius)/20; /*where outer==0 when iradius <= 2 */
          if(outer==0) outer = 1; /*030626*/
          inner = outer/2;
        }
        else if(idisk == 7) /* was 4, 040918*/
        {
          /*highlight on annulus made by penultimate disk or one below*/
          /*outer = (9*iradius)/12;*/ /*radius of base disk already reduced*/
          outer = iradius;/*radius of base disk already reduced, spot off edge*/
          if(outer==0) outer = 1; /*030626 where outer==0 when iradius <= 1*/
          inner = outer/2; 
        }
        if( (outer - inner) <= 1)
        {/*always have a highlight, makes a gleam even when ball very small */
          /*need to adjust to keep highlight visible*/
          if( (outer - inner) <= 0) outer = inner; /*030626*/
          inner = inner-1; /*effectively increase highlight size*/
        }
        setpentohighlight();

        XFillArc(dpy,offscreen,offgc
                    ,xplace-(outer+expand)      /*981015*/
                    ,yplace-(outer+expand)/ky   /*981015*/
                    ,(outer-inner)
                    ,(outer-inner)/ky
                    ,0
                    ,360*64);
               
        setpencolor(icolor); /*____DRAW.c*/ /*restore working color*/
      }/*highlight if only disk or the one that carries highlight 040918*/
    }/*draw disk of finite radius*/
  }/*iok*/
}
/*___balltoscreen()_________________________________________________________*/

/****wordstoscreen()************************common name: MAC & PC*************/
void    wordstoscreen(char commentstr[256],int ix,int iy,int icolorit)
{                                /* for labels */ /*PC has to set color */
  int   j,k,kk,workingfontsize,ixorig;
  /*int Thisfont;*/
  /*int Thisface;*/
  int Thissize;
  XFontStruct *thisfont;

  ixorig = ix;

  if(commentstr[3] != '\0')
  {/*string longer than zero length*/

      /*Thisfont = (int)commentstr[0];*/
      /*Thisface = (int)commentstr[1];*/
      Thissize = (int)commentstr[2];
      for(j=3;j<=255;j++)
      {
          commentstr[j-3] = commentstr[j];
          if(commentstr[j]=='\0') break;
      } 

/*printf("%s\n",commentstr);*/

      if(Thissize != 0) {workingfontsize = Thissize;}
      else {workingfontsize = wordsfontsize;}
      if(Lscalefonts) {workingfontsize = (2*workingfontsize);}

      /*if(workingfontsize != oldfontsize)*/
      {
         if(workingfontsize <= littlefontsize && littlefont)
         {
            thisfont = littlefont;
         }
         else if(workingfontsize <= mediumfontsize && mediumfont)
         {
            thisfont = mediumfont;
         }
         else if(workingfontsize >= largefontsize && largefont)
         {
            thisfont = largefont;
         }
         else
         {
            thisfont = littlefont;
         }
         XSetFont(dpy,offgc,thisfont->fid);
         XSetFont(dpy,gc,thisfont->fid);
      }
      setpencolor(icolorit);
      k=0;
      for(j=0;j<=255;j++)
      {/*loop over all char in string*/
          kk = j;
          word[j-k] = commentstr[j];
          if(commentstr[j]=='\\')
          {/*skip this \  and draw next character as a special case*/
              kk = j-1;
              word[j-k] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix = ix + XTextWidth(thisfont,word,strlen(word));



              /*if(restoreFont) XSetFont(dpy,offgc,thisfont->fid);*/
              j=j+1; 
              k=j+1;/*k is one more so when j loop augmented j-k==0*/
              word[0] = commentstr[j];
              word[1] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix = ix + XTextWidth(thisfont,word,strlen(word));
          }
          else if(commentstr[j]=='\015')
          {
              kk = j-1;
              word[j-k] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix = ix + XTextWidth(thisfont,word,strlen(word));
              /*if(restoreFont) XSetFont(dpy,offgc,thisfont->fid);*/
              j=j+1; /*j will be augmented at top of cycle*/
              k=j+2;
          }
          else if(commentstr[j]=='\015')
          {
              kk = j-1;
              word[j-k] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix = ixorig;
              iy = iy + wordsfontsize;
              k = j+1;
          }
          else if(commentstr[j]=='{')
          {
              kk = j-1;
              word[j-k] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix=ix+XTextWidth(thisfont,word,strlen(word));
              iy = iy + wordsfontsize/3;
              k = j+1;
          }
          else if(commentstr[j]=='}')
          {
              kk = j-1;
              word[j-k] = '\0';
              XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
              ix=ix+XTextWidth(thisfont,word,strlen(word));
              iy = iy - wordsfontsize/3;
              k = j+1;
          }
          else if(commentstr[j]==EOL) 
          {
               kk = j-1;
               word[j-k] = '\0';
    XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*XDrawString(dpy,offscreen,offgc,ix,iy,commentstr[k],strlen(commentstr[k]));*/
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
               iy=iy + workingfontsize;
               ix = ixorig;
               k = j+1  ;
          }
          else if(commentstr[j] == '\0') break;
      }/*loop over all char in string*/
      if(k<kk)
      {
         word[j-k] = '\0';
         XDrawString(dpy,offscreen,offgc,ix,iy,word,strlen(word));
/*printf("%s: x==%d, y==%d\n",word,ix,iy);*/
      }
    oldfontsize = workingfontsize;
  }/*string longer than zero length*/

}
/*___wordstoscreen__________________________________________________________*/

/****setdrawcolorpen()**************************common name: MAC & PC********/
void    setdrawcolorpen(int ix,int iy,int icolor)
{
  XSetLineAttributes(dpy,offgc,ix,LineSolid,CapRound,JoinRound); /*090626*/
  /*XSetLineAttributes(dpy,offgc,ix,LineSolid,CapButt,JoinMiter);*/ /*090626*/
  setpencolor(icolor); /*____DRAW.c*/
}
/*___setdrawcolorpen()______________________________________________________*/

/****setdrawcolorpensharp()**************************************************/
void    setdrawcolorpensharp(int ix,int iy,int icolor) /*030626*/
{
  XSetLineAttributes(dpy,offgc,ix,LineSolid,CapButt,JoinMiter);
  setpencolor(icolor); /*____DRAW.c*/
}
/*___setdrawcolorpensharp()_________________________________________________*/

/****triangletoscreen()******************************************************/
void triangletoscreen(int x1, int y1, int x2, int y2, int x3, int y3
                    , int icolor)
{
#ifdef WIERD
  typedef struct {
      short x, y;
  } XPoint;
#endif

  XPoint aPoints[3];

  aPoints[0].x = x1;
  aPoints[0].y = y1;
  aPoints[1].x = x2;
  aPoints[1].y = y2;
  aPoints[2].x = x3;
  aPoints[2].y = y3;

  if(Lwhitebkg && icolor<251 && icolor>25) {setpencolor(icolor-25);} /*040918*/
  else setpencolor(icolor); /*____DRAW.c*/

  XFillPolygon(dpy,offscreen,offgc
              ,aPoints
              ,sizeof(aPoints)/sizeof(aPoints[0])
              ,Convex
              ,CoordModeOrigin);
  if(Lwhitebkg) {setpencolor(icolor);} /*restore pen 040918*/
}
/*___triangletoscreen()______________________________________________________*/

/****linetoscreen()***********************************************************/
void linetoscreen(int x1, int x2, int y1, int y2)
{
   XDrawLine( dpy, offscreen, offgc ,x1,y1,x2,y2);
}
/*___linetoscreen()__________________________________________________________*/

/****ellipsoidtoscreen()******************************************************/
void ellipsoidtoscreen( int x1,int x2,int y1,int y2,int z1,int z2
   ,int ipen, int icolor,void* passedptr)
{              /*ellipsoidtoscreen(): 080406*/
/*fprintf(stderr,"ellipsoidtoscreen x,y: %d, %d, corner: %d, %d\n",x1,y1,x2,y2);*/
   /*for now, just draw a line from center to ellipsoid extreme corner*/
   XDrawLine( dpy, offscreen, offgc ,x1,y1,x2,y2);
}
/*___ellipsoidtoscreen()_____________________________________________________*/

/****marktoscreen()***********************************************************/
void marktoscreen(int x, int y, int r, int icolor, int origpenwidth)
{
   int w;

   if(origpenwidth > 0) w = origpenwidth;
   else w = 4;
   
   if(r ==0){r = 1;} /*030626*/
   
   setdrawcolorpensharp(w,w,icolor);
   XDrawRectangle(dpy,offscreen,offgc
                       , (x  - r)   
                       , (y  - r)   
                       , (2*r)
                       , (2*r) );
}
/*___marktoscreen()__________________________________________________________*/

/****ringtoscreen()***********************************************************/
void ringtoscreen(int x, int y, int r, int icolor, int origpenwidth)
{
   int w;
   if(origpenwidth > 0) w = origpenwidth;
   else w = 4;
   
   if(r ==0){r = 1;} /*030626*/
   
   setdrawcolorpen(w,w,icolor);
   XDrawArc(dpy,offscreen,offgc
                       , (x  - r)   
                       , (y  - r)   
                       , (2*r)
                       , (2*r) 
                       ,0,360*64);
}
/*___ringtoscreen()__________________________________________________________*/

/****diamondtoscreen()********************************************************/
void diamondtoscreen(int x, int y, int r, int icolor, int origpenwidth)
{
   int w;
   float tmps,fr;

   if(origpenwidth > 0) w = origpenwidth;
   else w = 4;

   if(r==0)
   {/*set screen adjusted radius*/
      /*fr = 5;*/ /*ref: MAGEINPT.c/addmarkers() factor of 10 ????*/
      fr = .5;
      if(Lscalemarkers) tmps = 2;
      else tmps = 1;
      r = (int)(fr*(tmps*scalenew)/zoom);
      w = 2;
   }

   setdrawcolorpen(w,w,icolor);

   /*draw a diamond*/
   XDrawLine( dpy, offscreen, offgc ,x-r  ,y    ,x    ,y-r  );
   XDrawLine( dpy, offscreen, offgc ,x    ,y-r  ,x+r  ,y    );
   XDrawLine( dpy, offscreen, offgc ,x+r  ,y    ,x    ,y+r  );
   XDrawLine( dpy, offscreen, offgc ,x    ,y+r  ,x-r  ,y    );
}
/*___diamondtoscreen()_______________________________________________________*/

/****squaretoscreen()*********************************************************/
void squaretoscreen(int x, int y, int r, int icolor, int origpenwidth)
{
   int w;
   float tmps,fr;

   if(origpenwidth > 0) w = origpenwidth;
   else w = 4;

   if(r==0)
   {/*set screen adjusted radius*/
      /*fr = 5;*/ /*ref: MAGEINPT.c/addmarkers() factor of 10 ????*/
      fr = .5;
      if(Lscalemarkers) tmps = 2;
      else tmps = 1;
      r = (int)(fr*(tmps*scalenew)/zoom);
      w = 2;
   }

   setdrawcolorpen(w,w,icolor);

   /*draw a square*/
   XDrawLine( dpy, offscreen, offgc ,x-r  ,y-r  ,x+r  ,y-r  );
   XDrawLine( dpy, offscreen, offgc ,x+r  ,y-r  ,x+r  ,y+r  );
   XDrawLine( dpy, offscreen, offgc ,x+r  ,y+r  ,x-r  ,y+r  );
   XDrawLine( dpy, offscreen, offgc ,x-r  ,y+r  ,x-r  ,y-r  );
}
/*___squaretoscreen()________________________________________________________*/

/****fantoscreen()************************************************************/
void fantoscreen(  int x, int y, int icolor, int coloroffset, int origpenwidth
                 , pointstruct* theptptr)
{
   int x2=0,y2=0,x3=0,y3=0, r=0, xo=0, yo=0;
   float co=0, si=0, mag=0, o=0;
   
   XPoint aPoints[4];
   int w;

   if(origpenwidth > 0) w = origpenwidth;
   else w = 2;

   icolor = icolor; /*PC needs icolor for brush to fill polygons*/
   
   /*NOTE: color and penwidth already have been set for this point*/
   /*virtual pen has already been moved to x1,y1*/
   if(theptptr->moreinfoptr != NULL)
   {/*there is cos,sin info for this radial line*/
      /*for planning, disect components despite computational inefficiency*/
      r = (int)(scalenew*tablefancore); /*fudged radius of inner circle*/
      co = (theptptr->moreinfoptr)->f1; /*stored values of cos, sin */
      si = (theptptr->moreinfoptr)->f2; /*computed when table loaded*/
      mag = scalenew*(theptptr->radius)*tablefanradius;
   

      x2 = (int)((r+mag)*co); /*spine end at outer extent*/
      y2 = (int)((r+mag)*si);
      x3 = (int)(r*co);  /*spine end at inner circle*/
      y3 = (int)(r*si);

      /*angle = (180/(MAXROW+1));*/
      /*angle between spines, half circum == h = pi * radius */
      /*max non-overlapped tangent length ~= t = h/(MAXROW+1) */
      /*max offset to either side at end of spine == o = t/2;*/
      o = 3.14*(r+mag)/(2*(MAXROW+1)); 
      xo = (int)(o * si); /*perpendicular to spine*/
      yo = (int)(o * co);
  
    

      aPoints[0].x = x+x3; /*inner end of radial spine*/
      aPoints[0].y = y-y3;
      aPoints[1].x = x+x2+xo; /*cross arm at outer end of spine*/
      aPoints[1].y = y-y2+yo;
      aPoints[2].x = x+x2-xo; /*cross arm at outer end of spine*/
      aPoints[2].y = y-y2-yo;
      aPoints[3].x = x+x3; /*inner end of radial spine*/
      aPoints[3].y = y-y3;

      XFillPolygon( dpy,offscreen,offgc
                   ,aPoints
                   ,sizeof(aPoints)/sizeof(aPoints[0])
                   ,Convex
                   ,CoordModeOrigin);

   }/*there is cos,sin info for this radial line*/
   setdrawcolorpen(w,w,7+coloroffset);

   XDrawArc( dpy,offscreen,offgc
            , (x  - r)   
            , (y  - r)   
            , (2*r)
            , (2*r) 
            ,0,360*64);
}
/*___fantoscreen()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****multipointparallelaxespre()**********************************************/
void multipointparallelaxespre(void) 
{/*061115 parallel axes for Ndimensional points*/
   /*pre calc items for this pass and draw axes to be behind data point lines*/

   /*draw within screen's native canvas, y==0 at TOP, x==0 at left */
   /*y increases going down!  Reverse y direction for sensible plot. */

   char label[MAXNAMECHAR+1];
   int offset = 20;
   int xplace = 20;
   int xdelta = 360/NDIM;
   int j=0,k=0,x1=0,y1=0,x2=0,y2=0,icolor=0,bright=0;
   float Tranx=0,Trany=0; /*061119*/
   float fudge=0,sized=0,factor=0,ratio=0;
   sized = (float)oldmaxwide/(float)400; /*MAGELIST/calcintegerscreenpoints()*/
   /*mage originally sized to a 400x400 screen*/

   factor = 1 + ((float)paraAXzoom)/400;
   fudge = sized * factor;
   /*fudge = sized * (1 + (float)paraAXzoom/400);*/
testx = (float)(200*sized-(paraAXtranx))/(sized*GWIDTH);
testy = (float)(200*sized-(paraAXtrany))/(sized*GHEIGHT);
   Tranx = testx*(fudge*GWIDTH - sized*GWIDTH);
   Trany = testy*(fudge*GHEIGHT - sized*GHEIGHT);

   if(paraAXnum == NULL) /*only do this once per kinemage run*/
   {/*alloc and initiate array for actual order of parallel axes*/
      paraAXnum = (int*)malloc(NDIM*sizeof(int));
      if(paraAXnum != NULL)
      {
         for(j=0; j<NDIM; j++){paraAXnum[j] = j;}
      }
   }

   /*for use this pass: recalculate ifontsize global to MUXMDRAW */
   if(Lscalefonts) { ifontsize = (2*labelsfontsize); }
   else { ifontsize = (labelsfontsize); }

   for(k=0; k<NDIM; k++) /*j==0 is 1st entry of the multi-point*/
   {/*draw axes before draw data point polylines, y reversed */
      if(paraAXnum != NULL){j=paraAXnum[k];}else{j=k;}
      y1 = (int)((400 - (offset) )*fudge) +paraAXtrany -(int)Trany;
      y2 = (int)((400 - (360 + offset) )*fudge) +paraAXtrany -(int)Trany;
      x1 = (int)((xplace + (k)*xdelta)*fudge) +paraAXtranx -(int)Tranx;
      x2 = (int)((xplace + (k)*xdelta)*fudge) +paraAXtranx -(int)Tranx; 
      setpentoscreenforeground();
      XDrawLine( dpy, offscreen, offgc ,x1,y1,x2,y2);

      sprintf(label,"%d",(int)dimensionptr[j].max); /*121202*/
      stringtoscreen(label,x2,y2-15);  /*121202*/ 
      sprintf(label,"%d",(int)dimensionptr[j].min); /*121202*/
      stringtoscreen(label,x1-15,y1+15);  /*121202*/ 

   }
   setpentoscreenforeground();
   y2 = (int)((400 - (350 + offset))*fudge) +paraAXtrany -(int)Trany;
   sprintf(label,"max");
   stringtoscreen(label,(x2+5),y2);
   y2 = (int)((400 - (175 + offset))*fudge) +paraAXtrany -(int)Trany;
   sprintf(label,"---");
   stringtoscreen(label,(x2+5),y2);
   y2 = (int)((400 - (-5 + offset))*fudge) +paraAXtrany -(int)Trany;
   sprintf(label,"min");
   stringtoscreen(label,(x2+5),(y2-15)); /*was (x2+5),y2 121202*/

}
/*___multipointparallelaxespre()_____________________________________________*/

/****multipointparallelaxespost()*********************************************/
void multipointparallelaxespost(void) 
{/*061115 parallel axes for Ndimensional points*/

   /*after drawing data point lines: draw labels and control boxes*/

   /*draw within screen's native canvas, y==0 at TOP, x==0 at left */
   /*y increases going down!  Reverse y direction for sensible plot. */

   static int LparaAXzoomtran=0, LparaAXresort=0; /*121108 int */
   static int pickedaxis = 0, pickedplace=0, Lpickedaxis=0; /*061118*/
   char label[MAXNAMECHAR+1];
   int offset = 20;
   int xplace = 20;
   int xdelta = 360/NDIM;
   int j=0,k=0,x1=0,y1=0,x2=0,y2=0,icolor=0,bright=0,yplace=0;
   int ifake=0;
   int moveaxisto=0, Lmoveaxis=0;
   float Tranx=0,Trany=0; /*061119*/
   float fudge=0,sized=0,factor=0;
   sized = (float)oldmaxwide/(float)400; /*MAGELIST/calcintegerscreenpoints()*/
   /*mage originally sized to a 400x400 screen*/

   if(Lwhitebkg) {bright = 225;}
   else {bright = 200;}

   /*draw pickedpoint on top of all other data point polylines 061119,20 */
   if(pickedpointptr != NULL && markergrupptr != NULL && markergrupptr->on)
   {
      ifake = 0;
      icolor = bright + ((pickedpointptr->colorwidth)&31);
      multipointparallels(pickedpointptr, icolor, 3, &ifake);
      setdrawcolorpen(2,2,icolor); /*restore 2 as default pen width*/
   }

   if(paraAXzoom != 0 ||paraAXtranx != 0 ||paraAXtrany != 0){LparaAXzoomtran=1;}
   factor = 1 + ((float)paraAXzoom)/400;
   fudge = sized * factor;
   /*fudge = sized * (1 + (float)paraAXzoom/400);*/

   Tranx = testx*(fudge*GWIDTH - sized*GWIDTH);
   Trany = testy*(fudge*GHEIGHT - sized*GHEIGHT);

   /*ifontsize global to MUXMDRAW, set in multipointparallelaxespre() */
   /*rectangleclear(0, 0, GWIDTH, ifontsize + 5); */ /*linux crowded at top*/
   /*clear space at top for reset buttons and axis labels... NOT DO 121202*/

   for(k=0; k<NDIM; k++) /*j==0 is 1st entry of the multi-point*/
   {/*label axes after point polylines: label for j at axis position k*/

      if(paraAXnum != NULL){j=paraAXnum[k];}else{j=k;}

      y1 = (int)((400 - (offset) )*fudge) +paraAXtrany -(int)Trany;  /* y reversed */
      y2 = (int)((400 - (360 + offset) )*fudge) +paraAXtrany -(int)Trany;
      x1 = (int)((xplace + (k)*xdelta)*fudge) +paraAXtranx -(int)Tranx;
      x2 = (int)((xplace + (k)*xdelta)*fudge) +paraAXtranx -(int)Tranx; 

      if(pickx >x2-5 && pickx < x2+5 && picky >y2-5 && picky < y2+5)
      {/*pick an axis*/
         pickedaxis = j;
         pickedplace = k;
         Lpickedaxis = 1;
      }

      sprintf(label,"%s",dimensionptr[j].name);
      if     (j == NX) {icolor =bright+numberofcolor("red");}
      else if(j == NY) {icolor =bright+numberofcolor("green");}
      else if(j == NZ) {icolor =bright+numberofcolor("blue");}
      else {icolor = bright+numberofcolor("white");}
      if(y2 < ifontsize+5){yplace = ifontsize+5;}
      else {yplace = y2;}
      labeltoscreen(label,x2,yplace,icolor);

   }/*label axes*/

   Lmoveaxis = 0;
   setpentoscreenforeground();
   y1 = (int)((400 - (offset) )*fudge) +paraAXtrany -(int)Trany;
   for(k=0; k<=NDIM; k++)
   {/*for k axes: draw (and pick from) move-axis positions between axes*/
      if(k==0) {x1 = (int)((xplace + (k)*xdelta -5)*fudge) +paraAXtranx -(int)Tranx;}
      else {x1 = (int)((xplace + (k)*xdelta -xdelta/2)*fudge) +paraAXtranx -(int)Tranx;}
      
      
      XFillArc(dpy,offscreen,offgc ,x1 ,y1 ,4 ,4 ,0,360*64);
      if(   Lpickedaxis 
          && pickx > x1-4 && pickx < x1+4 && picky > y1-4 && picky < y1+4)
      {/*pick an axis*/
         moveaxisto = k;
         Lmoveaxis = 1;
      }
   }/*for k axes...*/
   
   if(Lpickedaxis && Lmoveaxis && paraAXnum != NULL)
   {
      if(moveaxisto < pickedplace)
      {
         for(k=pickedplace; k>moveaxisto; k--)
            { paraAXnum[k] = paraAXnum[k-1]; }
         paraAXnum[moveaxisto] = pickedaxis; 
         Lredrawvector = 1;
         LparaAXresort = 1;
      }
      else if(moveaxisto > pickedplace)
      {
         for(k=pickedplace; k<(moveaxisto-1); k++)
            { paraAXnum[k] = paraAXnum[k+1]; }
         paraAXnum[moveaxisto-1] = pickedaxis; 
         Lredrawvector = 1;
         LparaAXresort = 1;
      }
      Lpickedaxis = 0; /*completed a cycle*/
   }
   Lmoveaxis = 0; /*always start this over again*/
   
   /*draw top-left reset zoom and translation box*/
   setpentoscreenforeground();
   XDrawLine( dpy, offscreen, offgc, 0, ifontsize+5, ifontsize, ifontsize+5);
   XDrawLine( dpy, offscreen, offgc, ifontsize, 0, ifontsize, ifontsize+5);
   if(   LparaAXzoomtran 
      && pickx > 0 && picky > 0
      && pickx < ifontsize && picky < ifontsize+5) 
   {/*re-zero tran and zoom 061117 */
      paraAXtranx = paraAXtrany = paraAXzoom = LNDpickedinfo = 0;
      LparaAXzoomtran = 0;
      Lredrawvector = 1;
   }
   /*draw top-right restore axes order box*/
   setpentoscreenforeground();
   XDrawLine(dpy,offscreen,offgc,GWIDTH-ifontsize,ifontsize+5,GWIDTH,ifontsize+5);
   XDrawLine(dpy,offscreen,offgc,GWIDTH-ifontsize,0,GWIDTH-ifontsize,ifontsize+5);
   if(   LparaAXresort 
      && pickx < GWIDTH && picky > 0 
      && pickx > GWIDTH-ifontsize && picky < ifontsize+5) 
   {/*reset axes order 061118*/
      for(k=0; k<NDIM; k++){paraAXnum[k] = k;}
      LparaAXresort = 0;
      Lredrawvector = 1;
   }
   
   
}
/*___multipointparallelaxespost()_____________________________________________*/

/****multipointparallels()****************************************************/
void multipointparallels(pointstruct* theptptr, int icolor, int w, int* LNDnewptr) 
{/*061114  first limited parallel axes drawing of Ndimensional points*/

   /*draw within screen's native canvas, y==0 at TOP, x==0 at left */
   /*y increases going down!  Reverse y direction for sensible plot. */
   /*eventually, multipoints would have STATUS == STATIC | SCREEN */
   /*for now, just plot them from the values in coordNptr NDIM array */
   /*for the point referred to by itpointptr[ncount] = thispointptr */
  
   static pointstruct* nearestptr;
   static float nearestsq = 0;
   float distsq = 0;
   float misssq = 16;
   float prevalue=0,value=0,scale=1;

   int offset = 20;
   int xplace = 20;
   int xdelta = 360/NDIM;
   int i=0,j=0,k=0,x1=0,y1=0,x2=0,y2=0;
   float parazoom = 1;
   float Tranx=0,Trany=0; /*061119*/
   float fudge=0,sized=0,factor=0;

  if(LNDpickedinfo &&((pointstruct*)pickedpointptr == (pointstruct*)theptptr))
  {/*not show picked point: thus can flash it on and off*/
     ;
  }
  else
  {/*visible ND paraAX point == polyline */

   sized = (float)oldmaxwide/(float)400; /*MAGELIST/calcintegerscreenpoints()*/
   /*mage originally sized to a 400x400 screen*/

   factor = 1 + ((float)paraAXzoom)/400;
   fudge = sized * factor;
   /*fudge = sized * (1 + (float)paraAXzoom/400);*/

   Tranx = testx*(fudge*GWIDTH - sized*GWIDTH);
   Trany = testy*(fudge*GHEIGHT - sized*GHEIGHT);

   if(*LNDnewptr) {nearestsq = misssq; *LNDnewptr = 0;}

   setdrawcolorpen(w,w,icolor);
   prevalue = 0;
   for(k=0; k<NDIM; k++) /*j==0 is 1st entry of the multi-point*/
   {/*loop over j axes in k order*/  /*y reversed*/
      if(paraAXnum != NULL){j=paraAXnum[k];}else{j=k;}
      /*scale = dimensionptr[j].scale;*/
      scale = 360/(dimensionptr[j].max - dimensionptr[j].min);
      if( dimensionptr[j].Lwrap && (theptptr->coordNptr[j]) < 0 )
           {value = scale*((dimensionptr[j].max) + (theptptr->coordNptr[j]));}
      else {value = scale*(theptptr->coordNptr[j]);}
      /*value is position on yaxis presuming 0 at min end  NB 121202*/
      /*adjust for actual range of 360-cyclic angle values NB 121202*/
      value = value - dimensionptr[j].min; /*121202*/
      /*value for j placed at the k position*/
      if(k>0) 
      {
         y1 = (int)((400 - (prevalue + offset))*fudge) +paraAXtrany -(int)Trany;
         y2 = (int)((400 - (value    + offset))*fudge) +paraAXtrany -(int)Trany;
         x1 = (int)((xplace + (k-1)*xdelta)*fudge) +paraAXtranx -(int)Tranx;
         x2 = (int)((xplace + (k  )*xdelta)*fudge) +paraAXtranx -(int)Tranx; 
         XDrawLine( dpy, offscreen, offgc ,x1,y1,x2,y2);
      }
      prevalue = value;
   
      if(ipick == 2)
      {/*check for pick: x2,y2 vs pickx,picky and do x1,y1 at leftmost axes */
         for(i=1 ; i<=2; i++) /*Mickey Mouse stuff to pick on leftmost axis*/
         {
            if(i==1 && k==1)
               { distsq = ((x1-pickx)*(x1-pickx) + (y1-picky)*(y1-picky)); }
            else
               { distsq = ((x2-pickx)*(x2-pickx) + (y2-picky)*(y2-picky)); }
            if( (distsq < misssq) && (distsq < nearestsq) )
            {
                nearestsq = distsq;
                temppickedpointptr = theptptr;
                temppickedlistptr = temppickedpointptr->listptr;
                temppickedsgrpptr = temppickedlistptr->sgrpptr;
                temppickedgrupptr = temppickedsgrpptr->grupptr;
                Lpick = 1;
            }
         }
      }/*check for pick: x2,y2 vs pickx,picky and do x1,y1 at leftmost axes */
   }/*loop over j axes in k order*/  /*y reversed*/
  }/*visible ND paraAX point == polyline */
}
/*___multipointparallels()___________________________________________________*/

