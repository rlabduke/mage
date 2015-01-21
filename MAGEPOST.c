/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****MAGEPOST.c********************************************************/
 
/*NEEDS FAN, RING, MARK drawing routines*/

/* Mike Word's smI88header.c + smI88output.c */
/* + dcr's fcheckzclip() + postscripter()*/
/* Name: smI88header.c,  Author: J. Michael Word,  DateCreated: 11/16/95 */
/* Purpose: Initialize and write header in PS format output */
/* Modified: JMW - 8/16/96 cut out Illustrator88 header to trim size*/
/* Name: smI88output.c,  Author: J. Michael Word,  DateCreated: 11/16/95 */
/* Purpose: write objects in PS format output */
/* Modified: JMW - 8/16/96 cut out Illustrator88 header to trim size*/
/*various special output objects by dcr, like ARROWs */
#include "MAGE.h" /*all PostScript options need this*/
#include "MAGELIST.h" /*data storage using pseude objects*/
#include "MAGEFLAG.h"
#include "MAGECOLR.h"
#include "MAGECMYK.h"
#include "MAGEPOST.h" /*#include "I88.h" renamed: */
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include <string.h>
#include <ctype.h>

/*prototypes*/
void I88mapbbox(   FILE *fp,
         double llx, double lly, double urx, double ury,
         int orientation);
void I88initalize(double llx, double lly, double urx, double ury);
int  I88getsPSmask(char c);
void I88drawtriangle(double fx, double fy, double tx, double ty
                                         , double mx, double my);
void I88drawline(double fx, double fy, double tx, double ty);
void I88drawarrow(float,float,float,float,float,float,struct pointstruct*);
void fballtoscreen(float,float,int,int,int,int,int,int);/*040925 Lblackrim*/
void fmarktoscreen(float, float, float, int);
void fringtoscreen(float, float, float, int);
void ffantoscreen(float,float,int,int,struct pointstruct*);
/*globals for this source file*/
I88globals I88 = {0};

float xx,yy,zz; /*used along with mage globals x2,y2,z2 to pass*/
                /*float zclip info*/

#define FABS(x)  (((x)<0.0)?-(x):(x))
#define MIN(x,y) (((x)<(y))? (x):(y))
#define MAX(x,y) (((x)>(y))? (x):(y))
#define EPSILON (0.000001)
#define INCH (72)

/* I88mapbbox() - map the input image size to the page bounding box */
void I88mapbbox(   FILE *fp,
         double llx, double lly, double urx, double ury,
         int orientation)
{
#define ox  ( 1.0*INCH)
#define oy  ( 1.0*INCH)
#define dx  ( 8.5*INCH - ox - ox)
#define dy  (11.0*INCH - oy - oy)
   double udx, udy, scale, ysign;

   I88.groupCount = 0;

   I88.fp = fp;

   /*llx==0, lly==GHEIGHT, urx==GWIDTH, ury==0*/
   ysign = (ury>lly)?1.0:-1.0;

   udx = FABS(urx-llx); /*GWIDTH - 0  i.e.  GWIDTH*/
   udy = FABS(ury-lly); /*0 - GHEIGHT i.e. GHEIGHT*/
   if (udx < EPSILON) udx = EPSILON;
   if (udy < EPSILON) udy = EPSILON;

   if (orientation == I88PORTRAIT) {
      scale = MIN(dx/udx, dy/udy);

      I88.orientation = I88PORTRAIT;

      I88.scaleA = scale;
      I88.scaleB = 0.0;
      I88.scaleC = 0.0;
      I88.scaleD = ysign*scale;
      I88.transX = ox - scale*llx;
      I88.transY = oy+ ((ysign>0.0)?(-scale*lly):(dy+scale*ury));

      I88.scaleLength = scale;
      I88.txtA = scale;
      I88.txtB = 0.0;
      I88.txtC = 0.0;
      I88.txtD = scale;

      I88.llx = (int)(I88.scaleA*llx+I88.scaleC*lly+I88.transX);
      I88.lly = (int)(I88.scaleB*llx+I88.scaleD*lly+I88.transY);
      I88.urx = (int)(I88.scaleA*urx+I88.scaleC*ury+I88.transX);
      I88.ury = (int)(I88.scaleB*urx+I88.scaleD*ury+I88.transY);
   }
   else {
      scale = MIN(dy/udx, dx/udy);

      I88.orientation = I88LANDSCAPE;

      I88.scaleA = 0.0;
      I88.scaleB = scale;
      I88.scaleC = -ysign*scale;
      I88.scaleD = 0.0;
      I88.transX = ox+ ((ysign>0.0)?(dx+scale*lly):(-scale*ury));
      I88.transY = oy - scale*llx;

      I88.scaleLength = scale;

      I88.txtA = 0.0;
      I88.txtB = scale;
      I88.txtC =-scale;
      I88.txtD = 0.0;

      /* in landscape mode ury and lly are flipped below */
      I88.llx = (int)(I88.scaleA*llx+I88.scaleC*ury+I88.transX);
      I88.lly = (int)(I88.scaleB*llx+I88.scaleD*ury+I88.transY);
      I88.urx = (int)(I88.scaleA*urx+I88.scaleC*lly+I88.transX);
      I88.ury = (int)(I88.scaleB*urx+I88.scaleD*lly+I88.transY);
   }
}


/* I88initalize() - initialize the graphic state and define clipping */
void I88initalize(double llx, double lly, double urx, double ury)
{
   /*llx==0, lly==GHEIGHT, urx==GWIDTH, ury==0*/
   double x1, y1;
   fprintf(I88.fp, "gsave newpath\n");
   I88transform(llx,lly, &x1,&y1);fprintf(I88.fp, "%.5g %.5g m ", x1, y1);
   I88transform(urx,lly, &x1,&y1);fprintf(I88.fp, "%.5g %.5g L\n", x1, y1);
   I88transform(urx,ury, &x1,&y1);fprintf(I88.fp, "%.5g %.5g L ", x1, y1);
   I88transform(llx,ury, &x1,&y1);fprintf(I88.fp, "%.5g %.5g L\n", x1, y1);

   fprintf(I88.fp,"closepath clip newpath\n");
   fprintf(I88.fp,"%.5g w 1 setlinejoin 1 setlinecap\n", I88.scaleLength);
   fprintf(I88.fp,"4 setmiterlimit [] 0 setdash\n");
}

/* I88header() - initialize mage Illustrator88 output and write header */
/*    I88header( fpout    output file                       */
/*              ,"MAGE"   Creator: cre?cre:"prog"           */
/*              ,"user"   For: usr?usr:"user"               */
/*              ,NameStr  Title: fn?fn:"filename"           */
/*              ,0        CreationDate: dt?dt:"whenever"    */
/*              ,0        lower left  x                     */
/*              ,GHEIGHT  lower left  y                     */
/*              ,GWIDTH   upper right x                     */
/*              ,0        upper right y                     */
/*              ,I88PORTRAIT);  orientation   jmw I88 source files*/
void I88header(FILE *fp, char* cre, char* usr, char* fn, char *dt,
         double llx, double lly, double urx, double ury, int orientation)
{
   /*llx==0, lly==GHEIGHT, urx==GWIDTH, ury==0*/
   I88mapbbox(fp, llx, lly, urx, ury, orientation);
   fprintf(fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");

   fprintf(fp, "%%%%Creator: %s\n", cre?cre:"prog");
   fprintf(fp, "%%%%For: (%s)\n", usr?usr:"user");
   fprintf(fp, "%%%%Title: (%s)\n", fn?fn:"filename");
   fprintf(fp, "%%%%CreationDate: (%s)\n", dt?dt:"whenever");
   fprintf(fp, "%%%%BoundingBox: %d %d %d %d\n",
      I88.llx, I88.lly, I88.urx, I88.ury);
   fprintf(fp, "%%%%EndComments\n");

   fprintf(fp, "\
%%%%BeginProcSet: MAGE_slimPS 0.9 0\n\
userdict /MAGE_slimPS 20 dict dup begin put\n\
/bdef {bind def} bind def\n");


   if(LspecialPS)
   {
      fprintf(fp, "\
/set1 { 2 copy 6 -2 roll 2 copy 10 2 roll } bind def\
/avg { 3 -1 roll add 2 div 3 1 roll add 2 div 2 -1 roll} bind def\
/set2 { 8 2 roll 4 -2 roll 2 copy 6 2 roll} bind def\
/setend { 6 2 roll 2 copy 6 2 roll moveto lineto curveto} bind def\
/swall { set1 avg set2 avg setend} bind def\n");
   }

   fprintf(fp, "\
/selectfont where { pop } {\n\
      /selectfont {\n\
    exch findfont exch\n\
    dup type /arraytype eq {makefont}{scalefont} ifelse setfont\n\
      } bdef\n\
   } ifelse\n");
   fprintf(fp, "\
/setcmykcolor where { pop } {\n\
      /setcmykcolor {\n\
    1 sub 4 1 roll\n\
    3 {\n\
      3 index add neg dup 0 lt { pop 0 } if\n\
      3 1 roll\n\
    } repeat\n\
    setrgbcolor pop\n\
      } bdef\n\
   } ifelse\n");
   fprintf(fp, "\
/m /moveto load def\n\
/L /lineto load def\n\
/S {lineto stroke} bdef\n\
/k /setcmykcolor load def\n\
/rgb /setrgbcolor load def\n\
/w /setlinewidth load def\n\
/f { closepath fill } bdef\n\
/circle {newpath 0 360 arc closepath fill} bdef\n\
/FD /selectfont load def\n\
/t { newpath moveto show } bdef\n\
");
   fprintf(fp, "\
currentdict readonly pop end\n\
%%%%EndProcSet\n");
   fprintf(fp, "\
%%%%EndProlog\n");

   fprintf(fp, "\
%%%%BeginSetup\n\
MAGE_slimPS begin\n\
%%%%EndSetup\n");

   I88initalize(llx, lly, urx, ury);
}/*I88header*/

 
/* I88transform() - convert screen x,y to printout x,y */
void I88transform(double x, double y, double *x2, double *y2)
{
   *x2 = (x * I88.scaleA) + (y * I88.scaleC) + I88.transX;
   *y2 = (x * I88.scaleB) + (y * I88.scaleD) + I88.transY;
}

/* I88rect() - draw a filled rectangle in the current color */
void I88rect(double x, double y, double w, double h)
{
   double x1, y1;

   I88transform(x, y, &x1, &y1); 
   fprintf(I88.fp,"newpath\n%.5g %.5g m ", x1, y1);

   I88transform(x+w, y  , &x1, &y1);fprintf(I88.fp,"%.5g %.5g L\n",x1,y1);
   I88transform(x+w, y+h, &x1, &y1);fprintf(I88.fp,"%.5g %.5g L ",x1,y1);
   I88transform(x  , y+h, &x1, &y1);fprintf(I88.fp,"%.5g %.5g L f\n",x1,y1);
}

/* I88circle() - draw a filled circle in the current color */
void I88circle(double x, double y, double r)
{
   double x1, y1;

   I88transform(x,   y,   &x1, &y1);
   fprintf(I88.fp,"%.5g %.5g %.5g circle\n", x1, y1, r*I88.scaleLength);
}

/* I88getsPSmask() - does this character need to be masked for PS text output? */
int I88getsPSmask(char c)
{
   return strchr("()\\", c) != NULL;
}

/* I88makePSstring() - mask special characters in output string and return length */
/* WARNING: caller must free(outputstring) if it does not point to input string. */
int I88makePSstring(char *str, char **outputstring)
{
   int len=0, pslen=0;
   char *pstr=0;

   len = pslen = strlen(str);
   for(pstr=str; *pstr; pstr++) {
      if (I88getsPSmask(*pstr)) pslen++;
   }

   pstr = str;

   if (len < pslen) {
      pstr = (char *) malloc(pslen+1);
      if (pstr) {
         char *p=pstr, *s=str;
         while(*s) {
            if (I88getsPSmask(*s)) {*p++ = '\\';}
            *p++ = isprint(*s) ? *s : ' ';
            s++;
         }
         *p = '\0';
      }
      else {
         /* ERROR: could not allocate (pslen) bytes */
      }
   }

   *outputstring = pstr;

   return ((pstr != NULL) ? pslen : 0);
}

/* I88text() - draw text in the current color */
void I88text(char *str, char *fnt, double x, double y, double sz)
{
   char *pstr=0;
   char *fname="Helvetica";
   int pslen = I88makePSstring(str, &pstr);

   if (pstr != NULL) {
      double x1, y1, sz1;
      sz1 = sz * I88.scaleLength * 2; /*991109 text was too small! ????*/
      I88transform(x, y, &x1, &y1);
      if(fnt) {        /* remove any leading underscore */
        fname = (fnt[0] == '_') ? (fnt+1) : fnt;
      }
      fprintf(I88.fp,"/%s ", fname);
      fprintf(I88.fp,"[ %.5g %.5g %.5g %.5g %.5g %.5g]FD\n",
         I88.txtA*sz1, I88.txtB*sz1, I88.txtC*sz1, I88.txtD*sz1,
         0.0, 0.0);
      fprintf(I88.fp,"(%s) %.5g %.5g t\n", pstr, x1, y1);

      if (pstr != str) { free(pstr); }
   }
   else {
      fprintf(I88.fp,"%%ERROR: could not malloc output string in I88getsPSmask()\n");
   }
}   

/* I88indexcolor() - set stroke and fill color from color index*/
void I88indexcolor(int idx) /*I88color inc grayscale 041001*/
{
   float c[4]; /*both 3 value RGB or 4 value CMYK*/
   if(LRGBPS)
   {   
      if (idx >= 0 && idx < MageRGBtableSIZE) 
      {  /*divide down by 256, multiply back up by 257*/
         /*but here dividing by maximum possible entry which is thus 1 */
         /*0---1 over full range of palette*/
         if(idx == 0)
         {/*dead black 041001*/
            c[0] = 0; c[1] = 0; c[2] = 0;
         }
         else if(idx == 255)
         {/*pure white 041001*/
            c[0] = 1; c[1] = 1; c[2] = 1;
         }
         else if(Lgrayscale) /*041001*/
         {
            c[0] = (float)mygrayscalepalette[idx][0]/255;
            c[1] = (float)mygrayscalepalette[idx][1]/255;
            c[2] = (float)mygrayscalepalette[idx][2]/255;
         }
         else
         {
            c[0] = (float)myfullpalette[idx][0]/255;
            c[1] = (float)myfullpalette[idx][1]/255;
            c[2] = (float)myfullpalette[idx][2]/255;
         }
         fprintf(I88.fp,"%.5g %.5g %.5g rgb\n", c[0], c[1], c[2]);
      }
      else 
      {
         fprintf(I88.fp,"%%ERROR: bad color index value %d (range 0..%d)\n"
         ,idx,MageRGBtableSIZE);
      }
   }
   else
   {/*CMYK not redone since Ian's colors, disabled as an option 041001*/
      if (idx >= 0 && idx < MageCMYKtableSIZE) 
      {
         c[0] = (float)MageCMYKtable[idx][0];
         c[1] = (float)MageCMYKtable[idx][1];
         c[2] = (float)MageCMYKtable[idx][2];
         c[3] = (float)MageCMYKtable[idx][3];
         fprintf(I88.fp,"%.5g %.5g %.5g %.5g k\n", c[0], c[1], c[2], c[3]);
      }
      else 
      {
         fprintf(I88.fp,"%%ERROR: bad color index value %d (range 0..%d)\n"
         ,idx,MageCMYKtableSIZE);
      }
   }

}

/* I88drawline() - write out a single line segment*/
void I88drawline(double fx, double fy, double tx, double ty)
{
   double x=0.0, y=0.0;

   I88transform(fx, fy, &x, &y);
   fprintf(I88.fp,"%.5g %.5g m ", x, y);
   I88transform(tx, ty, &x, &y);
   fprintf(I88.fp,"%.5g %.5g S\n", x, y);
}

/* I88drawtriangle() - write out a triangle*/
void I88drawtriangle(double fx, double fy, double tx, double ty
                                         , double mx, double my)
{
   double x=0.0, y=0.0;
   if(LspecialPS)
   {
      I88transform(fx, fy, &x, &y);
      fprintf(I88.fp,"%.5g %.5g \n", x, y);
      I88transform(tx, ty, &x, &y);
      fprintf(I88.fp,"%.5g %.5g \n", x, y);
      I88transform(mx, my, &x, &y);
      fprintf(I88.fp,"%.5g %.5g swall f\n", x, y);
   }
   else
   {
      I88transform(fx, fy, &x, &y);
      fprintf(I88.fp,"%.5g %.5g m\n", x, y);
      I88transform(tx, ty, &x, &y);
      fprintf(I88.fp,"%.5g %.5g L\n", x, y);
      I88transform(mx, my, &x, &y);
      fprintf(I88.fp,"%.5g %.5g L f\n", x, y);
   }
}

/* I88linewidth() - set line width*/
void I88linewidth(double wid)
{
   fprintf(I88.fp,"%.5g w\n", wid * I88.scaleLength);
}

/* I88beginGroup() - start of group specification - VOID */
void I88beginGroup(void)
{
   I88.groupCount++;
}   

/* I88endGroup() - termination of group specification - VOID */
void I88endGroup(void)
{
   I88.groupCount--;
}   

/* I88trailer() - complete the output file */
void I88trailer(void)
 {
   fprintf(I88.fp,"grestore\n"); /* to end clip group */
   if (I88.groupCount) {
      fprintf(I88.fp,
         "%%ERROR: %d unclosed groups\n",
         I88.groupCount);
   }
   fprintf(I88.fp,"\
%%%%PageTrailer\n\
showpage\n\
%%%%Trailer\n");
   fprintf(I88.fp,"\
end %%dictionary MAGE_slimPS\n");
   fprintf(I88.fp,"\
%%%%EOF\n");
}

/****fcheckzclip()************************************************************/
int fcheckzclip(float ix1,float iy1,float iz1,float ix2,float iy2,float iz2,
float *x1,float *y1,float *z1,float *x2,float *y2,float *z2,int type,int limitz)
/* passed altered coord values, local overrides global meaning,*/
/* return using adresses of x,y,z values*/
{/*used by both MAGEPOST/postscripter()  and MAGEREND/raster3Drender() */
  int            L1,L2,LOK;
  float            izmin,izmax,idelx,idely,ifudgx,ifudgy;
  float            idelmin1,idelmax1,ifudgmin1,ifudgmax1;
  float            idelmin2,idelmax2,ifudgmin2,ifudgmax2;
    
  z1 = z1; z2 = z2; /*980630 not used, avoid warning*/

  if(limitz)
  {
    izmin = (float)izctr - (float)izlimit;
    izmax = (float)izctr + (float)izlimit;
  }
  else
  {
    izmin = (float)izctr - (float)izclip;
    izmax = (float)izctr + (float)izclip;
  }
    L1 = 0;
    L2 = 0;
    LOK = 0; 
    if(iz1 >= (izmin) && iz1 <= (izmax) ) L1 = 1;
    if(iz2 >= (izmin) && iz2 <= (izmax) ) L2 = 1;
    
    if(L1 && L2) {LOK = 1; return(LOK);}
    
    if(iz1 < izmin && iz2 < izmin) {LOK = 0; return(LOK);}
    if(iz1 > izmax && iz2 > izmax) {LOK = 0; return(LOK);}

  /* Easy ones done, now have to do some work */
  /*don't do this for triangles, since clipping algorithm does not result*/
  /*in complete triangles, and writing outside of drawing area is tolerated*/
  if(!(type == 2 || type == 5 ) )
  {/*everything except triangle and ribbonlists*/

    
    /*so neither fully accepted, nor fully rejected, so got to do some work:*/
    idelx = ix2 - ix1;ifudgx = 1;
    while(idelx > 181 || idelx < -181){idelx = idelx/2; ifudgx = ifudgx*2;}
    idely = iy2 - iy1;ifudgy = 1;
    while(idely > 181 || idely < -181){idely = idely/2; ifudgy = ifudgy*2;}
    idelmin1 = izmin-iz1; ifudgmin1 = 1;
    while(idelmin1 > 181 || idelmin1 < -181)
    {idelmin1 = idelmin1/2; ifudgmin1 = ifudgmin1*2;}
    idelmax1 = izmax-iz1; ifudgmax1 = 1;
    while(idelmax1 > 181 || idelmax1 < -181)
    {idelmax1 = idelmax1/2; ifudgmax1 = ifudgmax1*2;}
    idelmin2 = izmin-iz2; ifudgmin2 = 1;
    while(idelmin2 > 181 || idelmin2 < -181)
    {idelmin2 = idelmin2/2; ifudgmin2 = ifudgmin2*2;}
    idelmax2 = izmax-iz2; ifudgmax2 = 1;
    while(idelmax2 > 181 || idelmax2 < -181)
    {idelmax2 = idelmax2/2; ifudgmax2 = ifudgmax2*2;}
    
    if(L1) /* 2 fault */
    {
      if(iz2 < izmin)  /* back plane */
      {
        *x2 = ix1 + ((idelx)*(idelmin1)/(iz2-iz1))*(ifudgx*ifudgmin1);
        *y2 = iy1 + ((idely)*(idelmin1)/(iz2-iz1))*(ifudgy*ifudgmin1);
        LOK = 1;
        return(LOK);
      }
      else /* front plane */
      {
        *x2 = ix1 + ((idelx)*(idelmax1)/(iz2-iz1))*(ifudgx*ifudgmax1);
        *y2 = iy1 + ((idely)*(idelmax1)/(iz2-iz1))*(ifudgy*ifudgmax1);
        LOK = 1;
        return(LOK);
      }
    }
    else if(L2) /* 1 fault */
    {
      if(iz1 < izmin)  /* back plane */
      {
        *x1 = ix2 + (( idelx)*(idelmin2)/(iz2-iz1))*(ifudgx*ifudgmin2);
        *y1 = iy2 + (( idely)*(idelmin2)/(iz2-iz1))*(ifudgy*ifudgmin2);
        LOK = 1;
        return(LOK);
      }
      else /* front plane */
       {
        *x1 = ix2 + (( idelx)*(idelmax2)/(iz2-iz1))*(ifudgx*ifudgmax2);
        *y1 = iy2 + (( idely)*(idelmax2)/(iz2-iz1))*(ifudgy*ifudgmax2);
        LOK = 1;
        return(LOK);
      }
    }
    else /* vector spans both planes, just leave the whole damn thing */
    {
      if(iz1 < izmin)  /* 1 at back plane, 2 at front plane */
      {
        *x1 = ix2 + ( ( idelx)*(idelmin2)/(iz2-iz1) )*(ifudgx*ifudgmin2);
        *y1 = iy2 + ( ( idely)*(idelmin2)/(iz2-iz1) )*(ifudgy*ifudgmin2);
        *x2 = ix1 + ( ( idelx)*(idelmax1)/(iz2-iz1) )*(ifudgx*ifudgmax1);
        *y2 = iy1 + ( ( idely)*(idelmax1)/(iz2-iz1) )*(ifudgy*ifudgmax1);

        LOK = 1;
        return(LOK);
      }
      else /* 1 at front plane, 2 at back plane */
      {
        *x1 = ix2 + ( ( idelx)*(idelmax2)/(iz2-iz1) )*(ifudgx*ifudgmax2);
        *y1 = iy2 + ( ( idely)*(idelmax2)/(iz2-iz1) )*(ifudgy*ifudgmax2);
        *x2 = ix1 + ( ( idelx)*(idelmin1)/(iz2-iz1) )*(ifudgx*ifudgmin1);
        *y2 = iy1 + ( ( idely)*(idelmin1)/(iz2-iz1) )*(ifudgy*ifudgmin1);

        LOK = 1;
        return(LOK);
      }
    }
  }/*everything except triangle and ribbonlists*/
  return(LOK); /*031201*/
}
/*___fcheckzclip()___________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****fchecksideclip()*********************************************************/
/*void fchecksideclip(int xyzflag)*/
int fchecksideclip(float jx1,float jy1,float jz1,float jx2,float jy2,float jz2,float *x1,float *y1,float *z1,float *x2,float *y2,float *z2,int type,float xmin,float xmax,float ymin,float ymax)

{
  int    L1,L2,LOK;
  float  idelx,ifudgx,idely,ifudgy,idel,ifudg;

  jz1=jz1;jz2=jz2;z1=z1;z2=z2; /*040609 now not use z, avoid no-use warning*/

  /*don't do this for triangles, since clipping algorithm does not result*/
  /*in complete triangles, and writing outside of drawing area is tolerated*/
  /*  if(    ((xyzflag & TRIANGLE) == TRIANGLE)        */
  /*   &&  !(Lstereo || Lcompareon)                  ) */
  if(type == 2 || type == 5 )
  {/*triangle component in MONO: accept all, let OS clip edges*/
    LOK = 1;
    return(LOK);
  }/*triangle component in MONO*/
  else
  {/*everything except triangle and ribbonlists in MONO*/

    LOK = 0;
    L1 = 0;
    L2 = 0;       

    if(jx1 >= xmin && jx1 <= xmax && jy1 >= ymin && jy1 <= ymax) L1 = 1;
    if(jx2 >= xmin && jx2 <= xmax && jy2 >= ymin && jy2 <= ymax) L2 = 1;
    
    if( L1 && L2) /* all inside, accept */
    {
       LOK = 1; 
       return(LOK);
    } 
    /* all outside, reject; LOK remains == 0; + = 031127 */
    if(jx1 <= xmin && jx2 <= xmin)
    { 
       return(LOK);
    }
    if(jy1 <= ymin && jy2 <= ymin)
    { 
       return(LOK);
    }
    if(jx1 >= xmax && jx2 >= xmax)
    {
       return(LOK);
    }
    if(jy1 >= ymax && jy2 >= ymax)
    {
       return(LOK);
    }

    /* Easy ones done, now have to do some work */
    /* cases: y1 < ymin,  y1 > ymax */
    /*        y2 < ymin,  y2 > ymax */
    /*        x1 < xmin,  x1 > xmax  */
    /*        x2 < xmin,  x2 > xmax  */
    /* first bring y in to y bounds , always work on adjusted coordinates */
    if(jy2 > ymax) /* ymax fault */
    {
            idel = ymax - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;
            jy2 = ymax;
            jx2 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    else if(jy2 < ymin) /* ymin fault */
    {
            idel = ymin - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;

            jy2 = ymin;
            jx2 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    if(jy1 > ymax) /* ymax fault */
    {
            idel = ymax - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;                    
            jy1 = ymax;
            jx1 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    else if(jy1 < ymin) /* ymin fault */
    {
            idel = ymin - jy1; ifudg = 1;
            while(idel > 181 || idel < -181)
            {idel = idel/2; ifudg = ifudg*2;}
            idelx = jx2 - jx1;ifudgx = 1;
            while(idelx > 181 || idelx < -181)
            {idelx = idelx/2; ifudgx = ifudgx*2;}
            idely = jy2-jy1;
            if(idely == 0) idely = 1;    
            jy1 = ymin;
            jx1 = jx1 + ((idel)*(idelx)/(idely))*(ifudgx*ifudg);
    }
    /* y coord adjusted on to y bounds, now adjust x coord */
    if(jx2 > xmax) /* xmax fault */
    {
      idel = xmax - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                    
      jx2 = xmax;
      jy2 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    else if(jx2 < xmin) /* xmin fault */
    {
      idel = xmin - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                        
      jx2 = xmin;
      jy2 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    if(jx1 > xmax) /* xmax fault */
    {
      idel = xmax - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                            
      jx1 = xmax;
      jy1 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    else if(jx1 < xmin) /* xmin fault */
    {
      idel = xmin - jx1; ifudg = 1;
      while(idel > 181 || idel < -181)
        {idel = idel/2; ifudg = ifudg*2;}
      idely = jy2 - jy1;ifudgy = 1;
      while(idely > 181 || idely < -181)
        {idely = idely/2; ifudgy = ifudgy*2;}
            idelx = jx2-jx1;
            if(idelx == 0) idelx = 1;                            
      jx1 = xmin;
      jy1 = jy1 + ((idel)*(idely)/(idelx))*(ifudgy*ifudg);
    }
    *x1 = jx1;
    *y1 = jy1;
    *x2 = jx2;
    *y2 = jy2;
    LOK = 1;
  }/*everything except triangle and ribbonlists*/
  return(LOK);
}
/*___fchecksideclip()________________________________________________________*/

/****postscripter*************************************************************/
void postscripter(
                   long i
                  ,int penwidth
                  ,int origpenwidth
                  ,int icolor
                  ,int ideepcolor
                  ,int idecue
                  ,double sinangle[3]
                  ,double cosangle[3]
                  ,int ixseting[3]
                  ,int iyset
                  ,int izset
                  ,int *postscriptcnt
                  ,int *inewpoint
                  ,float eyeposition
                  ,int xminleft,int xmaxleft,int xminright,int xmaxright
                  ,int ymin,int ymax  /*031127*/
                 )
{
   /*int    iradius,outer,inner,expand,idisk;*/ /*031201*/
   /*float  radius;*/ /*031201*/
   int    j,jmax,k,type,limitz;
   float  fthex[4],fthey[4],fthez[4]; /*931128PS*/ /*960220fthez*/
   float  delx,dely,delz,temp,fxyzx,fxyzy,fxyzz,delta;
   static int oldx=32000,oldy=32000 ;/*unlikely x,y values */
   static int ioldside=0;/*keep track of eye view*/
   pointstructptr targetpointptr;
   float  x1,y1,z1,x2,y2,z2;
   float fudge=0;
   int Lok = 1;    /* used for sideclipping, inc stereo middle*/
   int xmin=0, xmax=0; /*031127*/
   fudge = (float)oldmaxwide/(float)400;

   type = it(7,i);  /*080405*/
   /*type = it(0,i)>>11 & 15;*/ /*4 bits above lowest 11 */
   /*type = it(0,i)>>11 & 7;*/ /*3 bits above lowest 11 */
   I88indexcolor(icolor);
   I88linewidth(penwidth);

   {/*ALWAYS do floating point recalculation 990331*/ 
      if(ioldside != it(6,i)) /*keep track of which eye view*/
      {/*ilook index of L==1, R==2 stereo, or ==1 for mono*/
          word[0] = '%';
          (*postscriptcnt) = 0;
          (*inewpoint) = 1;
      }
      ioldside = it(6,i);

      if   (type == 2 || type == 5 ) jmax = 3; /*triangle or ribbon*/
      else              jmax = 2; /*usual moveto,drawto info*/
        
      for(j=1;j<=jmax;j++)
      {/*do the rotation matrix on original j=1,2 float coordinates*/
         /*i11=8192*(a11*cosangle[it(6,i)]+a13*sinangle[it(6,i)]);   */
         /*i12=8192*(a12);                                           */
         /*i13=8192*(a11*(-sinangle[it(6,i)])+a13*cosangle[it(6,i)]);*/
         /*i21=8192*(a21*cosangle[it(6,i)]+a23*sinangle[it(6,i)]);   */
         /*i22=8192*(a22);                                           */
         /*i23=8192*(a21*(-sinangle[it(6,i)])+a23*cosangle[it(6,i)]);*/
         /*i31=8192*(a31*cosangle[it(6,i)]+a33*sinangle[it(6,i)]);   */
         /*i32=8192*(a32);                                           */
         /*i33=8192*(a31*(-sinangle[it(6,i)])+a33*cosangle[it(6,i)]);*/

         if     (j==1)/* 1st preceeds the current point*/
         {/*it(1,i) contains offset of "MoveTo" back from current*/
                                    /* "DrawTo" point*/
            /*j==1 equivalent to jx2,jy2,jz2 in drawvec*/      
            targetpointptr = itpointptr[i];
            /*only VECTORS have a different MoveTo point*/
            if(it(1,i)>0)
            {/*MoveTo<DrawTo*/

               for(k=1;(k<=it(1,i) && k<=3);k++)
               {
                   targetpointptr = targetpointptr->previousptr;
               }
            }/*MoveTo<DrawTo*/
            fxyzx = targetpointptr->fx;
            fxyzy = targetpointptr->fy;
            fxyzz = targetpointptr->fz;
         }
         else if(j==2) /*2nd pnt is the DrawTo, the current point*/
         {
            fxyzx = itpointptr[i]->fx;
            fxyzy = itpointptr[i]->fy;
            fxyzz = itpointptr[i]->fz;
         }
         else if(j==3)/* 3rd is actually the zeroth pnt before the 1st*/
         {/*it(5,i) contains offset of 3rd point back from current one*/
            /*only get here for triangle or ribbon*/
            targetpointptr = itpointptr[i];
            if(it(5,i)>0)
              for(k=1;(k<=it(5,i) && k<=6);k++)
                targetpointptr = targetpointptr->previousptr;
                
            fxyzx = targetpointptr->fx;
            fxyzy = targetpointptr->fy;
            fxyzz = targetpointptr->fz;
         }

         if(  ((targetpointptr->STATUS)  & STATICFLAG)==0)
         {/*graphics object is usual 3D scaled sort*/            
            fthex[j]= (float)(
((( ((fxyzx)-fxcenternew)*(a11*cosangle[it(6,i)]+a13*sinangle[it(6,i)]) 
   +((fxyzy)-fycenternew)*(a21*cosangle[it(6,i)]+a23*sinangle[it(6,i)]) 
   +((fxyzz)-fzcenternew)*(a31*cosangle[it(6,i)]+a33*sinangle[it(6,i)]) )
                       *scalenew)+(float)ixseting[it(6,i)]));
            fthey[j]= (float)((( ((fxyzx)-fxcenternew)*a12 
                                +((fxyzy)-fycenternew)*a22 
                                +((fxyzz)-fzcenternew)*a32 )
                       *scalenew)+(float)iyset);
            fthez[j]= (float)(
((( ((fxyzx)-fxcenternew)*(a11*(-sinangle[it(6,i)])+a13*cosangle[it(6,i)])
   +((fxyzy)-fycenternew)*(a21*(-sinangle[it(6,i)])+a23*cosangle[it(6,i)])
   +((fxyzz)-fzcenternew)*(a31*(-sinangle[it(6,i)])+a33*cosangle[it(6,i)]) )
                       *scalenew)+(float)izset));
            
            if(Lperspec)
            {/*perspective on usual 3D graphics object*/
#ifdef OLDALGOR
                /*the ifudge mickey mouse stuff was to avoid*/
                /*exceeding the size of an integer*/
                delz = (float)(( (fthez[j]/2.0) - (float)izctr/2.0 ));
                      
                delx = fthex[j] - (float)ixseting[it(6,i)];
                temp = (float)(((delx*delz)/1000));
                fthex[j] = temp + fthex[j];
                     
                dely = fthey[j] - (float)iyset;
                temp = (float)(((dely*delz)/1000));
                fthey[j] = temp + fthey[j];
#endif
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
                /*old approximation was: extrax = delx*delz/V, same at delz==0*/                /* old one shows bent lines as scope in z increases*/
                /*i.e. xy-z to xy0 to xy+z not in line perspective projection*/
                /*delz = (float)jz2/2 - (float)izset/2; */
                      /*in effect, uses 1000 instead of 2000 for V*/

                delz = fthez[j] - (float)izset;
                if( delz > (eyeposition - 50))  delta = 50;
                else delta = eyeposition - delz;
                    /*big neg delz just makes less offset*/

                delx = fthex[j] - (float)ixseting[it(6,i)];
                temp = (delx*delz)/(delta);
                fthex[j] = temp + fthex[j];

                dely = fthey[j] - (float)iyset;
                temp = (dely*delz)/(delta);
                fthey[j] = temp + fthey[j];
            }/*perspective on usual 3D graphics object*/
         }/*graphics object is usual 3D scaled sort*/            
         else if( ((targetpointptr->STATUS)  & SCREENFLAG)==0)
         {/*see MAGEDRAW/drawvec() and MAGELIST/calcintegerfixedpoints()*/
            fthex[j] =  (fxyzx-fxcenter)*Scale + (float)ixseting[it(6,i)];
            fthey[j] = -(fxyzy-fycenter)*Scale + (float)iyset;
            fthez[j] =  (fxyzz-fzcenter)*Scale + (float)izset;
         }/*graphics object is NOT rotated*/            
         else
         {/*see MAGEDRAW/drawvec() and MAGELIST/calcintegerscreenpoints()*/
          /*ixseting[0]==ixctr, and presuming iyset==iyctr and izset==izctr*/
          /*SCREEN stuff done crt screen even in stereo: uses the whole screen*/
          /*and cannot be translated either*/
            fthex[j] =  (fxyzx)*fudge + (float)ixseting[0];
            fthey[j] = -(fxyzy)*fudge + (float)iyset;
            fthez[j] =  (fxyzz)*fudge + (float)izset;
         }/*graphics object is full SCREEN 2D graph*/            

      }/*do the rotation matrix on original j=1,2 float coordinates*/
      /*NOTE: x1,y1,z1,x2,y2,z2 are temporary floats */
      /*first, load the temporary floats:*/
      x1=fthex[1];
      y1=fthey[1];
      z1=fthez[1];
      x2=fthex[2];
      y2=fthey[2];
      z2=fthez[2];

      /*sideclip check so lines do not cross stereo center line 031127 */
      if(it(6,i)==1){xmin = xminleft; xmax = xmaxleft;}
      else        {xmin = xminright; xmax = xmaxright;}
      Lok = fchecksideclip(fthex[1],fthey[1],fthez[1],fthex[2],fthey[2],fthez[2]
                    ,&x1,&y1,&z1,&x2,&y2,&z2,type
                    ,(float)xmin,(float)xmax,(float)ymin,(float)ymax); /*.*/
      if(!Lok){return;}
      /*unpack the temporaries into the locals used below*/
      fthex[1]=x1;
      fthey[1]=y1;
      fthez[1]=z1;
      fthex[2]=x2;
      fthey[2]=y2;
      fthez[2]=z2; 

      /*then do the zclip check using temporary floats*/
      /*determine if this point in a list subject to zlimit clipping planes*/
      if( (itpointptr[i]->listptr)->STATUS & ZLIMITED) { limitz = 1; }
      else { limitz = 0; }
      if(Lzclipon && (((itpointptr[i]->listptr)->type & NOZCLIPFLAG)==0) )
      {/*Lzclipon 040409*/
         Lok = fcheckzclip(fthex[1],fthey[1],fthez[1],fthex[2],fthey[2],fthez[2]
                    ,&x1,&y1,&z1,&x2,&y2,&z2,type,limitz); /*.*/
      }
      if(!Lok){return;}
      /*then unpack the temporaries into the locals used below*/
      fthex[1]=x1;
      fthey[1]=y1;
      fthez[1]=z1;
      fthex[2]=x2;
      fthey[2]=y2;
      fthez[2]=z2; 

      if(it(1,i)!=oldx && it(2,i)!=oldy) (*inewpoint) = 1;
           /*check point number  THIS DOES NOT SEEM TO BE USED*/
   }/*ALWAYS do floating point recalculation 990331*/

   /*now have working x,y for current move or move-draw*/
   if( type  == 0 || type == 6 || type == 14) 
   {/*vector or dot or emphasis which only works on vectors 031127*/
       I88drawline(fthex[1],fthey[1],fthex[2],fthey[2]);
   }/*vector or dot*/
   else if(type == 11)
   {/*ARROW*/
       I88drawarrow(fthex[1],fthex[2],fthey[1],fthey[2],fthez[1],fthez[2]
               ,itpointptr[i]);
   }
   else if(type == 3)  /* label list */
   {/*label*/
              /* offset for giant character array */
              /*16*2048=32768 so if number > 2048, index must be long!*/
              /*even worse with (MAXPTIDCHAR+1) which started at 21 */
              /* use pointer arith to avoid using a long integer as an*/
              /* array index: see K&R A7.7 pg 205 2nd ed.: */        
              /* pointer + integer: integer promoted to make new */
              /*  pointer work for its type */
              getptIDstring(word, itpointptr[i]);

       I88text(word,"_Helvetica",fthex[2],fthey[2],labelsfontsize); 
                      /*951205*/
    }/*label*/
    else if( type == 1 || type == 7 ) /* ball or sphere list */
    {/*ball*/  /*no TBstereo involved for postscript output*/
       fballtoscreen(fthex[2],fthey[2],it(3,i),it(4,i),it(5,i),penwidth,icolor,type); /*040925*/
    }/*ball*/
    else if( type  == 4 ) /* word list */
    {/*words*/
              /*wordstoscreen( -it(5,i)  ,it(1,i),it(2,i), icolor );*/
              getcommentstring(commentstr, itpointptr[i]);/*MAGELIST gets wordcom[]*/             

              if(commentstr[2] != 0) k = commentstr[2]; /*951205*/
              else k = wordsfontsize; /*951205*/
              for(j=3;j<=255;j++)
              {/*strip off format info*/
                commentstr[j-3] = commentstr[j];
                if(commentstr[j]=='\0') break;
              }
              I88text(commentstr,"_Helvetica",fthex[2],fthey[2],k);
    }/*words*/
    else if( type  == 2 || type == 5 ) /* triangle, ribbon list */
    {
       if(Lwhitebkg && icolor<251 && icolor>25) /*041001*/
            {I88indexcolor(icolor-25);} /*041001*/
       else {I88indexcolor(icolor);}

       I88drawtriangle(fthex[1],fthey[1],fthex[2],fthey[2],fthex[3],fthey[3]);
    }
    else if(type ==  9 ) /*MARKTYPE and RINGSTYLE*/
    {/*ring: x,y,radius  postscripter already set color and penwidth */
       fringtoscreen(fthex[2],fthey[2],(float)it(4,i),origpenwidth);
    }
    else if(type ==  8 || type == 13) /*MARKTYPE & MARKTYPE+SQUARESTYLE*/
    {/*mark: x,y,radius  postscripter already set color and penwidth */
       fmarktoscreen(fthex[2],fthey[2],(float)it(4,i),origpenwidth);
    }
    else if(type == 10 ) /*MARKTYPE and FANSTYLE*/
    {/*fan at x,y,parent point: use point radius, (current color and pen)*/
       /*and send coloroffset for this depth and screen bkg*/
       ffantoscreen( (float)it(1,i),(float)it(2,i),origpenwidth
                    ,50*ideepcolor + idecue,itpointptr[i]);
    }
    return;
}
/*___postscripter()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writepostscript()*******************************************************/
int    writepostscript()
{
/*fprintf(stderr,"writepostscript header to fpout...\n");*/
    I88header( fpout    /*output file*/
              ,"MAGE"   /*Creator: cre?cre:"prog"*/
              ,"user"   /*For: usr?usr:"user"*/
              ,NameStr  /*Title: fn?fn:"filename"*/
              ,0        /*CreationDate: dt?dt:"whenever"*/
              ,0        /*lower left  x */
              ,GHEIGHT  /*lower left  y */
              ,GWIDTH   /*upper right x */
              ,0        /*upper right y */
              ,I88PORTRAIT);  /*orientation*/    /*jmw I88 source files*/
    I88indexcolor((Lwhitebkg||!Lcolor)?255:0);/*255 full white, 0 full black*/
    I88rect(0, 0, GWIDTH, GHEIGHT);     /* background*/
    Lspecialloop = 1;
    Lpostscript = 1;
    return(1); /*will contine writing to fpout in MAGEPOST.C/postscripter()*/
}
/*___writepostscript()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****finishpostscript()******************************************************/
void finishpostscript(int *postscriptcnt)
{/*finish anything left*/
    postscriptcnt = postscriptcnt;/*980630 not used, avoid warning*/
/*fprintf(stderr,"writepostscript trailer to fpout...\n");*/
    I88trailer();
    if(fpout != stdout) {fclose(fpout);} /*030725*/
    fpout = NULL;
     Lpostscript = 0;
     Lspecialloop = 0;
}
/*___finishpostscript()_____________________________________________________*/

/****loadpointpostscript()****************************************************/ 
void loadpointpostscript(long ncount,int xyzflag,int ilook
,pointstructptr jxyz0pointptr
,pointstructptr jxyz1pointptr
,pointstructptr jxyz2pointptr)
{/* used for both postscript and raster3Drender*/
    int    k;
    pointstructptr targetpointptr;
    
/*
printf("loading: %0.3f, %0.3f, %0.3f;   %0.3f, %0.3f, %0.3f;   %0.3f, %0.3f, %0.3f\n"
,jxyz0pointptr->fx,jxyz0pointptr->fy,jxyz0pointptr->fz
,jxyz1pointptr->fx,jxyz1pointptr->fy,jxyz1pointptr->fz
,jxyz2pointptr->fx,jxyz2pointptr->fy,jxyz2pointptr->fz);
*/
  /*it(1,ncount) used by vector for offset around shortened line points*/ /*980610*/
  /*it(2,ncount) used by ? for ?  */ 
  /*it(3,ncount) used by sphere for disk level*/
  /*it(4,ncount) used by sphere for radius*/
  /*it(5,ncount) used by labels and words for text index*/

    it(6,ncount) = ilook; /*index of L-R Stereo angle*/
    
    itpointptr[ncount] = jxyz2pointptr; /*ptr to jxyz2 */
    
    /*because of line shortening, extra points might be */
    /*between current point and the point which really is*/ 
    /*the beginning of a MoveTo-DrawTo couple (as well as*/
    /*extra points between that and third back point of a*/
    /* triangle list if balls were on intersections */
    /*To avoid allocating extra storage for the PostScript*/
    /*run for point-pointers, use it(1,ncount) and it(5,ncount) as */ 
    /* "Offsets" from current point of itpointptr[ncount]*/
    /*jxyz0pointptr,jxyz1pointptr,jxyz2pointptr;*/
    
    /*only vectors need the previous point*/
    /*if((xyzflag & VECTOR) && !(xyzflag & MOVETO_P))*/ /*030626*/
    if((xyzflag & VECTOR)) /*ribbons need all pts, revert:030910*/
    {/*VECTOR: at least 2 points needed, get offset*/
        targetpointptr = jxyz2pointptr; /*current effective point*/
        it(1,ncount) = 1; /*default, no intervening hidden points*/
        for(k=1; k<=3; k++)
        {
            targetpointptr = targetpointptr->previousptr;
            if(targetpointptr == jxyz1pointptr)
            {
                it(1,ncount) = k;
                break;
            }
        }
        if(xyzflag & VARIANT1) /*TRIANGLE OR RIBBON, need 3rd point*/
        {/*TRIANGLE, RIBBON, get offset of 3rd point*/
            targetpointptr = jxyz2pointptr; /*current effective point*/
            it(5,ncount) = 2; /*default, only jxyz1 intervenes */
            for(k=1; k<=6; k++)
            {
                targetpointptr = targetpointptr->previousptr;
                if(targetpointptr == jxyz0pointptr)
                {
                    it(5,ncount) = k;
                    break;
                }
            }
        }/*TRIANGLE, RIBBON, get offset of 3rd point*/
    }/*VECTOR: at least 2 points needed, get offset*/
    else
    {
        it(1,ncount) = 0;  /*MoveTo == DrawTo*/
        /*it(5,ncount) = 0;*/  /*not defined*/
        /*spheres use it(5,ncount) = Lblackrim, so do not reset it(5,) 041001*/
    }
}
/*___loadpointpostscript()___________________________________________________*/ 

/****I88drawarrow()************************************************************/
void I88drawarrow( float x1, float x2, float y1,float y2, float z1,float z2
                   ,pointstruct* itpointptr)
{              /*000804*/
   /*arrow head is drawn on second of two points at angle=angle and */
   /*spine length = radius  specified by that 2nd point (itpointptr) */
   float angle = (itpointptr->listptr)->angle;
   float radius = itpointptr->radius;
   float scale = 1.0;
   float fx0=0, fy0=0, fz0=0;
   float cmag=0,fx3=0,fy3=0,fz3=0,fx4=0,fy4=0,fz4=0;
   int   j=0;
   float fx1=(float)x1;
   float fy1=(float)y1;
   float fz1=(float)z1;
   float fx2=(float)x2;
   float fy2=(float)y2;
   float fz2=(float)z2;
   float dxs=0, dys=0, dzs=0;
     
   /*always output the arrow shaft vector, even if just a point*/
   I88drawline(x1,y1,x2,y2);
   if( (x2 == x1) && (y2 == y1) && (z2 == z1) ) return; /*can't do arrow*/
   
   /*static lists only get the original, place-on-screen, scaling*/
   if( ( ( (itpointptr->listptr)->STATUS & STATICFLAG) == STATICFLAG ) )
        scale = Scale;/*global*/ /*fixed list, i.e. "static" */
   else scale = scalenew;/*global*/
   if(radius < .001) 
   {
      if((itpointptr->listptr)->radius < .001) 
         radius = (float)(scale*0.2);
      else 
         radius = (float)(scale*(itpointptr->listptr)->radius);
   }
   else
   {
      radius = scale*radius;
   }
   /* 1-------------->2   */
   /*Need 3rd point to give a dihedral direction for the arrow spines*/
   /*Make off-axis point offset from the two points:*/
   /* use lesser of dimension change as the dimension to offset*/
   /*this stupid routine ignores sign change in any dimension*/
     
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   dxs = (fx2 - fx1)*(fx2 - fx1);
   dys = (fy2 - fy1)*(fy2 - fy1);
   dzs = (fz2 - fz1)*(fz2 - fz1);
   if( dxs < dys)
   {/*del x < del y*/
      if(dxs < dzs)
      {/*x smallest*/
         fx0 = fx1 + 10;
         fy0 = fy1;
         fz0 = fz1;
      }
      else
      {/*z smallest*/
         fx0 = fx1;
         fy0 = fy1;
         fz0 = fz1 + 10;
      }
   }
   else
   {/* del y < del x*/
      if(dys < dzs)
      {/*y smallest*/
         fx0 = fx1;
         fy0 = fy1 + 10;
         fz0 = fz1;
      }
      else
      {/*z smallest*/
         fx0 = fx1;
         fy0 = fy1;
         fz0 = fz1 + 10;
      }
   }
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */

   /*calculate a new line from 2 using points 0,1,2 */
   
   crossproduct( (double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1)
                ,(double)(fx1-fx0),(double)(fy1-fy0),(double)(fz1-fz0));
      /*return vector: dblx,dbly,dblz (globals)*/   
   cmag = (float)sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) );
   /*normalize and make c vector the desired length*/
   if(cmag < 0.000001) return; /*can't do arrow*/
   fx3 = (float)dblx*radius/cmag;
   fy3 = (float)dbly*radius/cmag;
   fz3 = (float)dblz*radius/cmag;
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   /*                 |   */
   /*                 3   , where 2-->3 perpendicular to 0--1--2 */

   /*place point 3 at correct distance from point #2 */
   /* as xproduct it is at a dihedral and angle of 90 to 1--->2*/
   fx3 = fx3 + fx2;
   fy3 = fy3 + fy2;
   fz3 = fz3 + fz2;
   
   /*find axis around which to rotate to put point at spine angle*/
   /* 0                 4 , where 2-->4 perpendicular to 1--2--3 */
   /* |                /  */

   /* 1-------------->2   */
   /*                 |   */
   /*                 3   , where 2-->3 perpendicular to 0--1--2 */
   
   /* cross:  (2->3)X(2->1) ==> (2->4), aXb ==> d at dihedral 90*/
   crossproduct( (double)(fx3-fx2),(double)(fy3-fy2),(double)(fz3-fz2)
                ,(double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1));
   cmag = (float)sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) );
   /*normalize, scale, and place crt point #2*/
   if(cmag < 0.000001) return; /*can't do arrow*/
   fx4 = (float)dblx*radius/cmag;
   fy4 = (float)dbly*radius/cmag;
   fz4 = (float)dblz*radius/cmag;

   fx4 = fx2 + fx4;
   fy4 = fy2 + fy4;
   fz4 = fz2 + fz4;

   /*rotate around 2->4 for correct angle*/
   doaxisrot( &fx3,&fy3,&fz3
             ,angle - 90
             ,(double)x2,(double)y2,(double)z2
             ,(double)(fx4),(double)(fy4),(double)(fz4));
   /*NOW 2-->c is one of the arrow spines at dihedral of 90 */
   /* 0                 4 , where 2-->4 perpendicular to 1--2--3 */
   /* |                /  */
   /* 1-------------->2   */
   /*                /    */
   /*               3     , where 2-->3 at arrow angle to 1-->2 */

   /* so can output this spine*/
   /*output the first arrow spine vector*/
   I88drawline(x2,y2,fx3,fy3);
   /*then calculate the 3 other spines*/
   for(j=1; j<=3; j++)
   {/*do 3 more arrow spines*/
      /*rotate around 1-->2 for correct dihedral*/
      doaxisrot( &fx3,&fy3,&fz3
                ,90
                ,(double)x1,(double)y1,(double)z1
                ,(double)x2,(double)y2,(double)z2);
      /*output the arrow spine vector*/
      I88drawline(x2,y2,fx3,fy3);
   }/*do 3 more arrow spines*/
}
/*___I88drawarrow()_________________________________________________________*/

/****fballtoscreen()**********************************************************/
void fballtoscreen(float x,float y,int idisk,int iradius,int Lblackrim
                  ,int ipen,int icolor, int type)
{/*fballtoscreen 030626*/
  /*ball*/  /*no TBstereo involved for postscript output*/
  /*balltoscreen(it(1,i),it(2,i),it(3,i),it(4,i),it(5,i),ipen,icolor);*/
  /*it(3,ncount) = idisk*/
  /*it(4,ncount) = intradius*/
  /*it(5,ncount) = Lblackrim 040925*/
  /*note: jx2==jx1 && jy2==jy1 for balllists*/ 
  /*penwidth may be depth cued width, vs origpenwidth */       
  /*ball at current color*/
  float radius=0,inner=0,outer=0;
  /*int expand=0,iok=1;*/
  float expand=0,pen=0; /*030823*/
  int   iok=1;

  /*if(idisk == 5)*/ /*030626*/
  if(idisk == 8 || idisk == 9) /*for 9 disks, highlight is on 7th 040918*/
  {/*keep ultimate capping disk from obscuring highlight on penutimate disk*/
     if(iradius == 2) iradius = 1; /*2: drop to 1*/
     else if(iradius <=1) iok = 0; /*0: don't draw*/ /*030626*/
  }
  if(iok) /*030626*/
  {/*iok*/
    /*postscript is floating pt, so ball should not vanish as r < 1 */
    /*but they do, so... floor on radius and expand for pen width GT 1*/
    /*if(2*iradius <= ipen || iradius < 1)*/ /* <= 030626*/
    /*     iradius = 1+ipen/2;*/ /*limit vanishing ball 981015*/
    radius = (float)(iradius); /*030823*/
    pen = (float)(ipen); /*030823*/
    /*ipen==1 for thinline, 2: onewidth, 1---5 multiwide*/ /*981015*/
    /*X11 lines really stay within one pixel of centered on coord pixel*/
    /* so expand ball evenly around coord */
    if(radius <= 0) {radius = .5;}
    if(pen > 1) {expand = (pen-1)/2;} /*030823*/
    else {expand = 0;}
  
    /*if(iradius >= 0)*/ /*030626*/
    if(radius+expand > 0) /*030626,030823*/
    {/*draw disk of finite radius*/
       /*I88indexcolor(icolor);*/ /*current color*/
       radius = (radius+expand); /*030626,030823*/
       /*060615 black disk behind color disk not centered accurately*/
       /*so move this section to after the color disk, and draw a line */
       /*if(Lblackrim && (idisk==0 || idisk==1))*/ /*040925*/
       /*{*//*plain old ball==disk, black edges on balls 040918*/
       /*   I88indexcolor(0);*/ /*black, draw disk 1 pixel bigger in radius*/
          /*but radius is float, so fudge radius, try 1/40  041001*/
       /*   I88circle((double)x,(double)y,(double)((radius)+(radius/40)));*/
       /*}*/
       if(Lwhitebkg && idisk != 0)  /*orig depth cueing kept for plain balls*/
       {
          if(icolor<251 && icolor>25) {I88indexcolor(icolor-25);}
       }
       else I88indexcolor(icolor); /*use current color*/
       I88circle((double)x,(double)y,(double)radius);
       /*if(Lwhitebkg) {I88indexcolor(icolor);}*/ /*restore current color*/

       /*if(type==1 &&(idisk == 0 || idisk == 4))*/ /*type 7 is NOHIGHLIGHT*/
       /*point type 7 is for NOHIGHLIGHT, type==1 for highlight on ball,sphere*/
       if(type==1 &&(idisk == 0 || idisk == 7)) /*040918 7th disk carries spot*/
       {/*highlight if only disk or the one that carries highlight 040918*/
         /* used to be on the penultimate annulus*/
         /*either pure disk */
         /*or near-to-top disk of fudged space-fill stacked disks*/
         /*adjust radius for the highlight*/
         if(idisk == 0)
         {
            outer = (9*radius)/20; 
            if(outer < 1) {outer = 1;} /*030626*/
            inner = outer/2;
         }
         else if(idisk == 7) /* was 4, 040918*/
         {
            /*highlight on annulus made by penultimate disk or one below*/
            /*outer = (9*radius)/12;*/ /*radius of base disk already reduced*/
            outer= radius;/*radius of base disk already reduced, spot off edge*/
            if(outer < 1) {outer = 1;} /*030626*/
            inner = outer/2;
         }
         if( (outer - inner) <= 1) 
         {/*need to adjust to keep highlight visible*/
            if( (outer - inner) <= 0) outer = inner;
            inner = inner-1; /*effectively increase highlight size*/
         }
         radius = (float)((outer-inner)/2.0);
         /*set highlight "color" */
         I88indexcolor(255); /*full white*/
         /*highlight spot is a white disk offset & overlaid on ball*/
         I88circle(x-((outer+inner)/2.0)
                  ,y-((outer+inner)/2.0)
                  ,radius );              
         /*I88indexcolor(icolor);*/ /*restore current color*/
       }/*highlight if only disk or the one that carries highlight 040918*/
       if(Lblackrim && (idisk==0 || idisk==1)) /*040925 as line 060615*/
       {/*plain old ball==disk, but Jane wants black edges on my balls 040918*/
          I88indexcolor(0); /*black, draw disk 1 pixel bigger in radius*/
          /*but radius is float, so fudge radius, try 1/40  041001*/
          /*I88circle((double)x,(double)y,(double)((radius)+(radius/40)));*/
          I88linewidth(1); /*would like a narrow border*/
          fringtoscreen(x,y,((radius)+(radius/40)),1); /*ipen of 1 */
       }
    }/*draw disk of finite radius*/
  }/*iok*/
}
/*___fballtoscreen()_________________________________________________________*/

/****fmarktoscreen()**********************************************************/
void fmarktoscreen(float x, float y, float r, int origpenwidth)
{
   float w; /*mark width despite current line width*/
   if(origpenwidth > 0) w = (float)origpenwidth;
   else w = (float)4;

   if(r ==0){r = 1;} /*030626 since r is just float of int iradius*/

   I88drawline(x-r,  y-r, x+r-w,y-r);
   I88drawline(x+r-w,y-r, x+r-w,y+r-w);
   I88drawline(x+r-w,y+r-w, x-r,  y+r-w);
   I88drawline(x-r,  y+r-w, x-r,  y-r);

}
/*___fmarktoscreen()_________________________________________________________*/

/****fringtoscreen()**********************************************************/
void fringtoscreen(float x, float y, float r, int origpenwidth)
{
   float a = (float)((float)r*12/13);
   float b = (float)((float)r*5/13);
   float c = (float)((float)r*9.3/13);
   float w; /*ring width despite current line width*/
   if(origpenwidth > 0) w = (float)origpenwidth;
   else w = (float)2;
   /*060615 w offset seems wrong-headed since always subtracted !? */
   w = 0; /*060615 current guess */

   if(r ==0){r = 1;} /*030626 since r is just float of int iradius*/

   I88drawline(x+r-w,y  -w, x+a-w,y+b-w); 
   I88drawline(x+a-w,y+b-w, x+c-w,y+c-w);
   I88drawline(x+c-w,y+c-w, x+b-w,y+a-w);
   I88drawline(x+b-w,y+a-w, x  -w,y+r-w);
   I88drawline(x  -w,y+r-w, x-b-w,y+a-w);
   I88drawline(x-b-w,y+a-w, x-c-w,y+c-w);
   I88drawline(x-c-w,y+c-w, x-a-w,y+b-w);
   I88drawline(x-a-w,y+b-w, x-r-w,y  -w);
   I88drawline(x-r-w,y  -w, x-a-w,y-b-w);
   I88drawline(x-a-w,y-b-w, x-c-w,y-c-w);
   I88drawline(x-c-w,y-c-w, x-b-w,y-a-w);
   I88drawline(x-b-w,y-a-w, x  -w,y-r-w);
   I88drawline(x  -w,y-r-w, x+b-w,y-a-w);
   I88drawline(x+b-w,y-a-w, x+c-w,y-c-w);
   I88drawline(x+c-w,y-c-w, x+a-w,y-b-w);
   I88drawline(x+a-w,y-b-w, x+r-w,y  -w);
}
/*___fringtoscreen()_________________________________________________________*/

/****ffantoscreen()***********************************************************/
void ffantoscreen( float x,float y,int coloroffset, int origpenwidth
                  ,pointstruct* theptptr)
{
   float x2=0,y2=0,x3=0,y3=0, r=0, xo=0, yo=0;
   float co=0, si=0, mag=0, o=0;

   /*NOTE: color and penwidth already have been set for this point*/
   /*virtual pen has already been moved to x1,y1*/
   if(theptptr->moreinfoptr != NULL)
   {/*there is cos,sin info for this radial line*/
      /*for planning, disect components despite computational inefficiency*/
      r   = (scalenew*tablefancore); /*fudged radius of inner circle*/
      co  = (theptptr->moreinfoptr)->f1; /*stored values of cos, sin */
      si  = (theptptr->moreinfoptr)->f2; /*computed when table loaded*/
      mag = scalenew*(theptptr->radius)*tablefanradius;
   

      x2 = ((r+mag)*co); /*spine end at outer extent*/
      y2 = ((r+mag)*si);
      x3 = (r*co);  /*spine end at inner circle*/
      y3 = (r*si);

      /*angle = (180/(MAXROW+1));*/
      /*angle between spines, half circum == h = pi * radius */
      /*max non-overlapped tangent length ~= t = h/(MAXROW+1) */
      /*max offset to either side at end of spine == o = t/2;*/
      o = (float)3.14*(r+mag)/(2*(MAXROW+1)); 
      xo = (o * si); /*perpendicular to spine*/
      yo = (o * co);
    
      /*draw the feather panel of fan == a triangle */
      I88drawtriangle(x+x3,y-y3,x+x2+xo, y-y2+yo,x+x2-xo, y-y2-yo);
      /*where*/
      /*(x+x3,y-y3)*/ /*inner end of radial spine*/
      /*(x+x2+xo, y-y2+yo)*/ /*cross arm at outer end of spine*/
      /*(x+x2-xo, y-y2-yo)*/ /*cross arm at outer end of spine*/
   }
   I88indexcolor(7+coloroffset); /*white/black circle at appropriate depth*/
   fringtoscreen(x, y, r, origpenwidth);
}
/*___ffantoscreen()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
