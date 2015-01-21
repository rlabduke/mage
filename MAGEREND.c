/* MAGEREND.c */
/*NEEDS FAN, RING, MARK drawing routines*/
 

#include "MAGE.h" 
#include "MAGELIST.h"
#include "MAGEFLAG.h"
#define EXTERNREND
#include "MAGEREND.h" /*stroked font*/
#include "MAGEANGL.h"
#include "MAGETABL.h"
#include "MAGECOLR.h"

static float maxz = 0;
static float cx,cy,cz,sc;

/*prototypes*/
void RENDERcylinder(float,float,float,float,float,float, float,int,char[256]);
void RENDERsphere(float,float,float, float, int, char[256]);
void RENDERtriangle( float,float,float
                    ,float,float,float
                    ,float,float,float
                    ,int
                    ,float,float,float
                    ,float,float,float
                    ,float,float,float
                    , int, char[256]);
void RENDERtext(float,float,float,int,int,char[256]);
void RENDERstrokedtext(float,float,float,float,int,int,char[256]);
void RENDERstrokedchar(float,float,float,float,int,int,char,float*,float*);
void RENDERcolorR3D(int, float*, float*, float*);
void RENDERarrow(float,float,float,float,float,float, float,int,char[256]
                 ,struct pointstruct*);
void Rfmarktoscreen(float,float,float,float,int,int,char[256]);
void Rfringtoscreen(float,float,float,float,int,int,char[256]);
void Rffantoscreen(float,float,float,int,int,char[256],int,struct pointstruct*);

/****RENDERcolorR3D()*********************************************************/
void RENDERcolorR3D(int icolor, float* redptr, float* greenptr, float* blueptr)
{
    float red=0, green=0, blue=0;
    
    if(!(icolor >= 0 && icolor <= 255))
    {
       icolor = 7;
    }
    if(icolor > 0 && icolor < 201) /*041001 don't mess with pure blk,wht 0,255*/
    {/*for POV-Ray render program, set color to max intensity 040430*/
       /*also for Raster3D*/
       while(icolor < 201) {icolor = icolor + 50;} /*040430*/
    }
    /*myGetEntryColor(icolor);*/ /*returns RGB in global structure rgbColor*/
    /*red   = (float)rgbColor.red/(float)65535;*/
    /*green = (float)rgbColor.green/(float)65535;*/
    /*blue  = (float)rgbColor.blue/(float)65535;*/
    /*041001 get RGB values from whatever was loaded in my own palette arrays*/
    if(Lgrayscale)
    {
       red   = (float)mygrayscalepalette[icolor][0]/255;
       green = (float)mygrayscalepalette[icolor][1]/255;
       blue  = (float)mygrayscalepalette[icolor][2]/255;
    }
    else
    {
       red   = (float)myfullpalette[icolor][0]/255;
       green = (float)myfullpalette[icolor][1]/255;
       blue  = (float)myfullpalette[icolor][2]/255;
    }

    /*Raster3D seems to need the square of the MAGE color values*/
    /*but rendered tiff images are too dark in Mac/Photoshop ...*/
    /*hence the Logical switch*/ 
    if(LR3Dsquaredcolor)
    {
       *redptr   = red*red;
       *greenptr = green*green;
       *blueptr  = blue*blue;
    }
    else
    {
       *redptr   = red;
       *greenptr = green;
       *blueptr  = blue;
    }
}
/*___RENDERcolorR3D()________________________________________________________*/

/****RENDERcylinder()*********************************************************/
void RENDERcylinder( float x1, float x2, float y1,float y2, float z1,float z2
                    , float radius, int icolor, char PTID[256])
{
    float red=0, green=0, blue=0;
    
    RENDERcolorR3D(icolor, &red, &green, &blue);
  if(Lraster3Drender == 1)
  {/*for Raster3D Render program*/
    fprintf(fpout,"3 %c",EOLOUT); /*Object type 3   round-ended cylinder */
    /* x1,y1,z1,R1,x2,y2,z2,R2,red,greeen,blue */
 fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f    %s%c"
            ,x1,y1,z1,radius/2 
            ,x2,y2,z2,radius/2
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
/*990825  lines had been too wide by a factor of about 2  */
  }/*for Raster3D Render program*/
  else if(Lraster3Drender == 2)
  {/*for POV-Ray render program*/
    if(x1==x2 && y1==y2 && z1==z2)
    {/*degenerate cylinder, artificially displace in z so POV-Ray will deal with it*/
       z2 = z2 + 1;
    }
    RENDERsphere(x1,y1,z1,radius/2,icolor,PTID);
    RENDERsphere(x2,y2,z2,radius/2,icolor,PTID);
    fprintf(fpout,"cylinder { %c",EOLOUT); /*cylinder */
 fprintf(fpout,"<%.3f, %.3f, %.3f> <%.3f, %.3f, %.3f> %.3f pigment{rgb<%.3f, %.3f, %.3f>}}//%s%c"
            ,x1,y1,z1
            ,x2,y2,z2,radius/2
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
  }/*for POV-Ray render program*/
}
/*___RENDERcylinder()________________________________________________________*/

/****RENDERsphere()***********************************************************/
void RENDERsphere(float x,float y,float z,float radius,int icolor,char PTID[256])
{
    float red=0, green=0, blue=0;
    
    RENDERcolorR3D(icolor, &red, &green, &blue);
  if(Lraster3Drender == 1)
  {/*for Raster3D Render program*/
    fprintf(fpout,"2 %c",EOLOUT); /*Object type 2   sphere x,y,z,R,G,B*/
    fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f    %s%c"
            ,x,y,z 
            ,radius
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
  }/*for Raster3D Render program*/
  else if(Lraster3Drender == 2)
  {/*for POV-Ray render program*/
    fprintf(fpout,"sphere { %c",EOLOUT); /*Object type 2   sphere x,y,z,R,G,B*/
    fprintf(fpout,"<%.3f, %.3f, %.3f> %.3f pigment {rgb< %.3f, %.3f, %.3f>}}//%s%c"
            ,x,y,z 
            ,radius
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
  }/*for POV-Ray render program*/
}
/*___RENDERsphere()__________________________________________________________*/

/****RENDERtriangle()*********************************************************/
void RENDERtriangle( float x1,float y1,float z1
                    ,float x2,float y2,float z2
                    ,float x3,float y3,float z3
                    ,int icolor
                    ,float fmx1,float fmy1,float fmz1
                    ,float fmx2,float fmy2,float fmz2
                    ,float fmx3,float fmy3,float fmz3
                    , int imore, char PTID[256])
{
   float red=0, green=0, blue=0;
    
   RENDERcolorR3D(icolor, &red, &green, &blue);
   if(Lraster3Drender == 1)
   {/*for Raster3D Render program*/
      fprintf(fpout,"1 %c",EOLOUT); /*Object type 1   triangle */
      /* x1,y1,z1,R1,x2,y2,z2,R2,red,greeen,blue */
      fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f    %s%c"
            ,x1,y1,z1
            ,x2,y2,z2
            ,x3,y3,z3
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
      if(imore)
      {
         /*normals for triangle front*/
         fprintf(fpout,"7 %c",EOLOUT); /*Object type 13   triangle normals*/
         /* normals for each of the 3 point of the triangle*/
         fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %c"
            ,fmx1,fmy1,fmz1 
            ,fmx2,fmy2,fmz2
            ,fmx3,fmy3,fmz3
            ,EOLOUT
           );
         /*triangle for backside*/
         fprintf(fpout,"1 %c",EOLOUT); /*Object type 1   triangle */
         /* x1,y1,z1,R1,x2,y2,z2,R2,red,greeen,blue */
         fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f    %s%c"
            ,x1,y1,z1
            ,x2,y2,z2
            ,x3,y3,z3
            ,red
            ,green
            ,blue
            ,PTID
            ,EOLOUT
           );
         /*normals for triangle back*/
         fprintf(fpout,"7 %c",EOLOUT); /*Object type 13   triangle normals*/
         /* normals for each of the 3 point of the triangle*/
         fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %c"
            ,-fmx1,-fmy1,-fmz1 
            ,-fmx2,-fmy2,-fmz2
            ,-fmx3,-fmy3,-fmz3
            ,EOLOUT
           );
      }
   }/*for Raster3D Render program*/
   else if(Lraster3Drender == 2)
   {/*for POV-Ray render program*/
      /*some MAGE starting triangles do not have well defined normals at first*/
      if(fmx1<.01 && fmy1<.01 &&fmz1<.01&&fmx1>-.01 && fmy1>-.01 &&fmz1>-.01)
      {fmx1=fmx3;fmy1=fmy3;fmz1=fmz3;}/*which may help*/
      if(fmx2<.01 && fmy2<.01 &&fmz2<.01&&fmx2>-.01 && fmy2>-.01 &&fmz2>-.01)
      {fmx2=fmx3;fmy2=fmy3;fmz2=fmz3;}/*which may help*/
      /*another POV-Ray problem is "degenerate" triangle from wimperwamp normals*/
      /*earlier averaging only makes normals == directly across ribbon*/
      /*the other point of a ribbon triangle can get set to incompatible value*/
      /*BUT setting odd one out leads to changing normals within a rectangle!!*/ 
      if(imore == 4)
      {/*last triangle of a rectangle, force last 2 normals == first normal*/
         fmx2=fmx1;fmy2=fmy1;fmz2=fmz1;
         fmx3=fmx1;fmy3=fmy1;fmz3=fmz1;
      }
      else if(imore == 3)
      {/*first triangle of a rectangle, force this last normal == first normal*/
         fmx3=fmx1;fmy3=fmy1;fmz3=fmz1;
      }
      
      fprintf(fpout,"smooth_triangle{  %c",EOLOUT); /*triangle with normals*/
      fprintf(fpout,"<%.3f, %.3f, %.3f>,< %.3f, %.3f, %.3f>, %c"
         ,x1,y1,z1,fmx1,fmy1,fmz1,EOLOUT);
      fprintf(fpout,"<%.3f, %.3f, %.3f>,< %.3f, %.3f, %.3f>, %c"
         ,x2,y2,z2,fmx2,fmy2,fmz2,EOLOUT);
      fprintf(fpout,"<%.3f, %.3f, %.3f>,< %.3f, %.3f, %.3f> %c"
         ,x3,y3,z3,fmx3,fmy3,fmz3,EOLOUT);
      fprintf(fpout,"pigment{rgb<%.3f, %.3f, %.3f>}}//%s%c"
         ,red,green,blue,PTID,EOLOUT);
   }/*for POV-Ray render program*/
}
/*___RENDERtriangle()________________________________________________________*/

/****RENDERtext()*************************************************************/
void RENDERtext(float x,float y,float z, int icolor,int isize,char txtstr[256])
{
    float red=0, green=0, blue=0;

    isize = isize;
    
    RENDERcolorR3D(icolor, &red, &green, &blue);
  if(Lraster3Drender == 1)
  {/*for Raster3D Render program*/
    fprintf(fpout,"11 %c",EOLOUT); /*Object type 11   Label text [XYZ] [RGB] */
    fprintf(fpout,"%.3f %.3f %.3f %.3f %.3f %.3f %c"  /*EOLOUT ????*/
            ,x,y,z 
            ,red
            ,green
            ,blue
            ,EOLOUT
           );
    fprintf(fpout,"%s%c",txtstr,EOLOUT);
  }/*for Raster3D Render program*/
  /*there is NO equivalent for POV-Ray*/
}
/*___RENDERtext()____________________________________________________________*/

/****RENDERstrokedtext()******************************************************/
void RENDERstrokedtext(float x,float y,float z
       ,float penwidth,int icolor,int isize,char txtstr[256])
{
   int j;
   static float spacex=0, spacey=0;
   char hold[256];
   
   if(Lscalefonts)
   {
      isize = 2*isize;
      penwidth = 2*penwidth;
   }

   for(j=0;j<256;j++)
   {/*establish local memory of text string, passed string gets lost*/
      hold[j] = txtstr[j];
      if(txtstr[j] == '\0') break;
   }
    j = 0;
    spacex = 0;
    spacey = 0;
    while(hold[j] != '\0')
    {/*print out char by char*/
        RENDERstrokedchar(x,y,z
                  ,penwidth,icolor,isize,hold[j],&spacex,&spacey);
        j++;
    }
}
/*___RENDERstrokedtext()_____________________________________________________*/

/****RENDERstrokedchar()******************************************************/
void RENDERstrokedchar( float x,float y,float z
                       ,float penwidth, int icolor,int isize,char c
                       ,float* spacex, float* spacey)
{
    
    float sized = (float)isize*(float)FONTSCALE; 
       /*arbitrary, needs to be adjusted for platform*/
    float x1,y1,z1,x2,y2,z2;
    int *thischar;
    int ix1,ix2,iy1,iy2;
    int k = 1; /*always looking at the second of a move-draw pair*/
    int j = 0; /*set j = k-1 for previous coord*/

  if(Lraster3Drender == 1)
  {/*usual line width is too fat for characters*/
    penwidth = penwidth/2; 
  }/*for Raster3D Render program*/

    thischar = MageStrokedChar[(int)c];
    
    /*ascii char number indexes array of pointers to array of stroke sequences*/
    while(*(thischar+3*k+0) < 2)
    {/*punitive move-draw pair*/
        if(*(thischar+3*k+0) == 1)
        {/*second is a move, just advance counter to get next move-draw pair*/
           k++;
           j = k-1;
        }
        else
        {/*previous point and current kth point make a move-draw pair*/
           /*coords are the scaled integer form*/
           ix1 = *(thischar+3*j+1);
           iy1 = *(thischar+3*j+2);
           ix2 = *(thischar+3*k+1);
           iy2 = *(thischar+3*k+2);
           x1 = x + sized*( +(float)ix1 + *spacex); /*scale by 1/zoom ????*/
           y1 = y + sized*( -(float)iy1 + *spacey); /*y decreases going up*/
           z1 = z;
           x2 = x + sized*( +(float)ix2 + *spacex);
           y2 = y + sized*( -(float)iy2 + *spacey);
           z2 = z;
           
           if(!Lraster3Drender)
           {/*gets here when called with MAGE ix,iy integer type coords*/
              setdrawcolorpen((int)penwidth, (int)penwidth, icolor);  /*____DRAW.c*/
              linetoscreen( (int)x1, (int)x2, (int)y1, (int)y2);
           }
           else
           {/*gets here through raster3Drender() with x,y as float coord*/
              word[0] = c;
              word[1] = '\0'; /*create identifier for this cylinder == the char*/
              RENDERcylinder( x1, x2, y1, y2, z1, z2,(float)penwidth, icolor, word);
           }
           k++; /*advance stroke counter*/
           j = k-1;
        }
    }/*punitive move-draw pair*/
    /*after while*/
    {/*end of character, get width, height offset for next char*/
       ix2 = *(thischar+3*k+1);
       iy2 = *(thischar+3*k+2);
       if(*(thischar+3*k+2) != 0)
       {/*end of line indicator, reset offsets to print on next line*/
          *spacex = 0;
          *spacey = *spacey + (float)iy2;
       }
       else
       {
          *spacex = *spacex + (float)ix2;
       }
    }
}
/*___RENDERstrokedchar()_____________________________________________________*/

/****raster3Dtextpreviewer()**************************************************/
void raster3Dtextpreviewer(float x,float y,float z
       ,float penwidth,int icolor,int isize,char txtstr[256])
{
    RENDERstrokedtext(x,y,z,penwidth,icolor,isize,txtstr);
}
/*___raster3Dtextpreviewer()_________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****RENDERarrow()************************************************************/
void RENDERarrow( float x1, float x2, float y1,float y2, float z1,float z2
                    , float cylradius, int icolor, char PTID[256]
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
   RENDERcylinder(x1,x2,y1,y2,z1,z2,cylradius,icolor,PTID);

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
   RENDERcylinder(x2,fx3,y2,fy3,z2,fz3,cylradius,icolor,PTID);
    
   /*then calculate the 3 other spines*/
   for(j=1; j<=3; j++)
   {/*do 3 more arrow spines*/
      /*rotate around 1-->2 for correct dihedral*/
      doaxisrot( &fx3,&fy3,&fz3
                ,90
                ,(double)x1,(double)y1,(double)z1
                ,(double)x2,(double)y2,(double)z2);
      /*output the arrow spine vector*/
      RENDERcylinder(x2,fx3,y2,fy3,z2,fz3,cylradius,icolor,PTID);
   }/*do 3 more arrow spines*/
}
/*___RENDERarrow()___________________________________________________________*/

/****Rfmarktoscreen()*********************************************************/
void Rfmarktoscreen( float x,float y,float z,float r,int icolor
                    ,int origpenwidth,char PTID[256])
{
   float w; /*mark width despite current line width*/
   if(origpenwidth > 0) w = (float)origpenwidth;
   else w = (float)4;


   /*RENDERcylinder(x,x,y,y,z,z,cylradius,icolor,PTID);*/
   RENDERcylinder(x-r,   x+r-w, y-r,   y-r,   z, z, w,icolor,PTID);
   RENDERcylinder(x+r-w, x+r-w, y-r,   y+r-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+r-w, x-r,   y+r-w, y+r-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-r,   x-r,   y+r-w, y-r,   z, z, w,icolor,PTID);
}
/*___Rfmarktoscreen()________________________________________________________*/

/****Rfringtoscreen()*********************************************************/
void Rfringtoscreen( float x,float y,float z,float r,int icolor
                    ,int origpenwidth,char PTID[256])
{
   float a = (float)((float)r*12/13);
   float b = (float)((float)r*5/13);
   float c = (float)((float)r*9.3/13);
   float w; /*ring width despite current line width*/
   if(origpenwidth > 0) w = (float)origpenwidth;
   else w = (float)2;

   /*RENDERcylinder(x,x,y,y,z,z,cylradius,icolor,PTID);*/
   RENDERcylinder(x+r-w, x+a-w, y  -w, y+b-w, z, z, w,icolor,PTID); 
   RENDERcylinder(x+a-w, x+c-w, y+b-w, y+c-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+c-w, x+b-w, y+c-w, y+a-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+b-w, x  -w, y+a-w, y+r-w, z, z, w,icolor,PTID);
   RENDERcylinder(x  -w, x-b-w, y+r-w, y+a-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-b-w, x-c-w, y+a-w, y+c-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-c-w, x-a-w, y+c-w, y+b-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-a-w, x-r-w, y+b-w, y  -w, z, z, w,icolor,PTID);
   RENDERcylinder(x-r-w, x-a-w, y  -w, y-b-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-a-w, x-c-w, y-b-w, y-c-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-c-w, x-b-w, y-c-w, y-a-w, z, z, w,icolor,PTID);
   RENDERcylinder(x-b-w, x  -w, y-a-w, y-r-w, z, z, w,icolor,PTID);
   RENDERcylinder(x  -w, x+b-w, y-r-w, y-a-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+b-w, x+c-w, y-a-w, y-c-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+c-w, x+a-w, y-c-w, y-b-w, z, z, w,icolor,PTID);
   RENDERcylinder(x+a-w, x+r-w, y-b-w, y  -w, z, z, w,icolor,PTID);
}
/*___Rfringtoscreen()________________________________________________________*/

/****Rffantoscreen()**********************************************************/
void Rffantoscreen( float x,float y,float z,int icolor,int origpenwidth
                   ,char PTID[256],int coloroffset,pointstruct* theptptr)
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
      RENDERtriangle( x+x3,y-y3,z
                     ,x+x2+xo, y-y2+yo,z
                     ,x+x2-xo, y-y2-yo,z
                     ,icolor
                     ,0,0,1 /*in-screen-plane triangle normals*/
                     ,0,0,1
                     ,0,0,1
                     ,1,PTID); /*imore==1*/

      /*where*/
      /*(x+x3,y-y3)*/ /*inner end of radial spine*/
      /*(x+x2+xo, y-y2+yo)*/ /*cross arm at outer end of spine*/
      /*(x+x2-xo, y-y2-yo)*/ /*cross arm at outer end of spine*/
   }
   /*white/black circle at appropriate depth*/
   Rfringtoscreen(x, y, z, r, 7+coloroffset,origpenwidth, PTID);
}
/*___Rffantoscreen()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****raster3Drender()*********************************************************/
void raster3Drender(
                   long i
                  ,int* oldcolordepth
                  ,int* oldpenwidth
                  ,int icolor
                  ,int ipen
                  ,int ideepcolor
                  ,int idecue
                  ,double sinangle[3]
                  ,double cosangle[3]
                  ,int ixseting[3]
                  ,int iyset
                  ,int izset
                  ,int *inewpoint
                  ,float eyeposition
                  ,int xminleft,int xmaxleft,int xminright,int xmaxright
                  ,int ymin,int ymax  /*031127*/
                 )
{
   int    j,jmax,k,penwidth,colordepth,type,iradius,idisk,Lmagecolordepth;
   float  fthex[4],fthey[4],fthez[4],radius,fmore[4][4];
   float  delta,delx,dely,delz,temp,fxyzx,fxyzy,fxyzz,fmorex,fmorey,fmorez;
   static int oldx=32000, oldy=32000;/*unlikely x,y values */
   static int ioldside=0;/*keep track of eye view*/
   pointstructptr targetpointptr;
   float  x1,y1,z1,x2,y2,z2;
   int imore,moreit;
   double b11,b12,b13,b21,b22,b23,b31,b32,b33;/*local matrix*/
   float ctrx,ctry,ctrz;
   static int icount=0;
   int origpenwidth;
   int itcolor,limitz;
   int Lexplicitcolordepth=0;
   int Ltriangles=0;
   float fudge=0;
   int Lok = 1;    /* used for sideclipping, inc stereo middle*/
   int xmin=0, xmax=0; /*031127*/
   fudge = (float)oldmaxwide/(float)400;

    
   if(Lraster3Drender == 1)
   {/*do not know how to do decent depth cueing in raster3Drender*/
      Lmagecolordepth = 1; /*use mage color, render adds lighting*/
   }
   else
   {/*maybe can do reasonable depth-cueing in POV-ray*/
      Lmagecolordepth = 0; /*use base color, let render do depth & lighting*/
   }
   /*individual pen width as well as color now possible*/
   /*so need to scan for change even if doing B&W*/

   /*color-depth-width-type compressed into it(0,i) */
   type = it(7,i);  /*080405*/
   /*type = it(0,i)>>11 & 15;*/ /*4 bits above lowest 11 */
   /*type = it(0,i)>>11 & 7;*/ /*3 bits above lowest 11 */
   colordepth = (it(0,i) & 255);/*lowest 5+3=8 bits*/
   origpenwidth = ((it(0,i)>>8) & 7);/*shift by 5+3, pick up only 3 bits*/
   itcolor = (it(0,i) & 31); /*mage color number*/
   if(((it(0,i)>>5) & 7) > 0 && ((it(0,i)>>5) & 7) <= 5) 
   {/*shift over by 5, pick up only 3 bits to test for explicit "depthcue"*/ 
      Lexplicitcolordepth = 1;
   }
   if( type  == 2 || type  == 5) /* triangle, ribbon list */
   {
      Ltriangles = 1;
   }

   penwidth = origpenwidth; /*for starters*/

      /* ipen is computed depth-que line width, while originally*/
      /*penwidth is line width specified by point width or list width*/
      /*where point width is dominant over list width setting*/
      /*BUT for many uses, penwidth=ipen as a default.  However, some */
      /*display objects need to use origpenwidth or default to a const*/

   if(Lcolor) /*LLcolor==1 true color, Lcolor==1 gray...*/
   {/*can adjust color and/or depth cue gray level*/
      /*if(colordepth != *oldcolordepth)*/ /*990314*/
      {/*change color*/

            if(Lexplicitcolordepth && !(Ltriangles && Lraster3Drender==1)) 
            {/*color code is effectively the depth-cue bin*/
              /*force palette shades rather than doing actual depth-cueing*/
              if(itcolor > 0 && itcolor < 26)
              {/*not deadblack nor deadwhite*/
                /*within range of indexed, depth-cued colors*/
                if(LLcolor)
                {/*true color*/
                    {icolor=itcolor+ 50*(((it(0,i)>>5) & 7)-1) +idecue;}
                }
                else
                {/*fudge gray level by explicit "depthcue"*/
                    {icolor= 13 + 50*(((it(0,i)>>5) & 7)-1) +idecue;}
                }
                /*depthcue 1-5 used as ideepcolor 0-4*/
              }/*not deadblack nor deadwhite*/
              else
              {
                 if(  itcolor == 0
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
                    {icolor = numberofcolor("default")
                                +50*(((it(0,i)>>5) & 7)-1) + idecue;}
                 }
              }
            } /*color code is effectively the depth-cue bin*/
            else if(LLcolor) /*can support real color*/
            {/*use depth-que bin calculated from actual depth*/
              if(itcolor > 0 && itcolor < 26)
              {/*not deadblack nor deadwhite*/
                if(Lmagecolordepth == 1)
                  {icolor = itcolor + 50*ideepcolor + idecue;}
                else
                  {icolor = itcolor +200 + idecue;}
              }/*not deadblack nor deadwhite*/
              else
              {
                 if(  itcolor == 0
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
                  if(Lmagecolordepth == 1)
                    {icolor = numberofcolor("default")
                                +50*ideepcolor + idecue;}
                  else
                    {icolor = numberofcolor("default")+200 + idecue;}
                 }
              }
            } 
        }/*change color*/
   }/*can adjust color and/or depth cue gray level*/  
   if(    (colordepth != *oldcolordepth)
        ||(penwidth   != *oldpenwidth  )
        ||(penwidth   == 0             ) )
   {/*change pen color and/or width*/
        if(penwidth == 0) penwidth = ipen;
        /*pen width used is the one calculated from the actual*/
        /* depth or other default*/
        /*pen width can be explicitly controlled*/
        *oldcolordepth = colordepth;
        *oldpenwidth = penwidth;
    }/*change pen color and/or width*/


    {/*ALWAYS do floating point recalculation 990331*/ 
        if(ioldside != it(6,i)) /*keep track of which eye view*/
        {
            word[0] = '%';
            (*inewpoint) = 1;
        }
        ioldside = it(6,i);
        if   (type == 2 || type == 5) jmax = 3; /*triangle or ribbon*/
        else              jmax = 2; /*usual moveto,drawto info*/
        
    b11=a11;b12=a12;b13=a13;b21=a21;b22=a22;b23=a23;b31=a31;b32=a32;b33=a33;/*local matrix*/
    ctrx = fxcenternew; ctry = fycenternew; ctrz = fzcenternew; /*local<--global*/

        for(j=1;j<=jmax;j++)
        {/*do the rotation matrix on original j=1,2 float coordinates*/
            /*cos,sin of stereo angle offset, which is a pure y rotation*/
            /*so has no y component*/
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
                if( type  == 2 || type  == 5)
                {/* triangle, ribbon list */
                   if(targetpointptr->moreinfoptr != NULL)
                   {
                       fmorex = targetpointptr->moreinfoptr->f1;
                       fmorey = targetpointptr->moreinfoptr->f2;
                       fmorez = targetpointptr->moreinfoptr->f3;
                   }
                }/* triangle, ribbon list */
            }
            else if(j==2) /*2nd pnt is the DrawTo, the current point*/
            {
                fxyzx = itpointptr[i]->fx;
                fxyzy = itpointptr[i]->fy;
                fxyzz = itpointptr[i]->fz;
                if( type  == 2 || type  == 5)
                {/* triangle, ribbon list */
                   if(itpointptr[i]->moreinfoptr != NULL)
                   {
                       fmorex = itpointptr[i]->moreinfoptr->f1;
                       fmorey = itpointptr[i]->moreinfoptr->f2;
                       fmorez = itpointptr[i]->moreinfoptr->f3;
                   }
                }/* triangle, ribbon list */
            }
            else if(j==3)/* 3rd is actually the zeroth pnt before the 1st*/
            {/*it(5,i) contains offset of 3rd point back from current one*/
                targetpointptr = itpointptr[i];
                if(it(5,i)>0)
                  for(k=1;(k<=it(5,i) && k<=6);k++)
                    targetpointptr = targetpointptr->previousptr;
                
                fxyzx = targetpointptr->fx;
                fxyzy = targetpointptr->fy;
                fxyzz = targetpointptr->fz;
                if( type  == 2 || type  == 5) 
                {/* triangle, ribbon list */
                   if(targetpointptr->moreinfoptr != NULL)
                   {
                       fmorex = targetpointptr->moreinfoptr->f1;
                       fmorey = targetpointptr->moreinfoptr->f2;
                       fmorez = targetpointptr->moreinfoptr->f3;
                       moreit = targetpointptr->moreinfoptr->it; 
                          /*flag for part of rectangle*/
                   }
                }/* triangle, ribbon list */
            }

      if(  ((targetpointptr->STATUS)  & STATICFLAG)==0)
      {/*graphics object is usual 3D scaled sort*/
            
            /*put every thing in terms of targetptr and gather common terms*/
            /*stereo angle is small, so cos terms dominant*/
            /*if stereo ==0, cos==1, sin==0 and get simple matrix multiplication*/
            fthex[j]= (float)(
((( ((fxyzx)-ctrx)*(b11*cosangle[it(6,i)]+b13*sinangle[it(6,i)]) 
   +((fxyzy)-ctry)*(b21*cosangle[it(6,i)]+b23*sinangle[it(6,i)]) 
   +((fxyzz)-ctrz)*(b31*cosangle[it(6,i)]+b33*sinangle[it(6,i)]) )
                       *scalenew)+(float)ixseting[it(6,i)]));
            fthey[j]= (float)((( ((fxyzx)-ctrx)*b12 
                                +((fxyzy)-ctry)*b22 
                                +((fxyzz)-ctrz)*b32 )
                       *scalenew)+(float)iyset);
            fthez[j]= (float)(
((( ((fxyzx)-ctrx)*(b11*(-sinangle[it(6,i)])+b13*cosangle[it(6,i)])
   +((fxyzy)-ctry)*(b21*(-sinangle[it(6,i)])+b23*cosangle[it(6,i)])
   +((fxyzz)-ctrz)*(b31*(-sinangle[it(6,i)])+b33*cosangle[it(6,i)]) )
                       *scalenew)+(float)izset));
            
            if(Lperspec)
            {/*perspective on usual 3D graphics object*/
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

if(maxz < fthez[j]) maxz = fthez[j]; /*needed for POV-Ray camera position*/


            if(( type==2 || type==5) && (itpointptr[i]->moreinfoptr != NULL))
            {/* triangle, ribbon list may have relevant extra info*/
              /*, e.g.triangle normals averaged for each point*/
              /*maybe point properties like radius should be stored this way?*/
              /*treat as normalized vectors at the origin, */
              /*  i.e. no trans, no scale*/
            
              fmore[1][j]= 
(float)((( ( ((fmorex))*(b11*cosangle[it(6,i)]+b13*sinangle[it(6,i)]) 
            +((fmorey))*(b21*cosangle[it(6,i)]+b23*sinangle[it(6,i)]) 
            +((fmorez))*(b31*cosangle[it(6,i)]+b33*sinangle[it(6,i)]) )
       )));
              fmore[2][j]=
(float)((( ( ((fmorex))*b12 
            +((fmorey))*b22 
            +((fmorez))*b32 )
       )));
              fmore[3][j]= 
(float)((( ( ((fmorex))*(b11*(-sinangle[it(6,i)])+b13*cosangle[it(6,i)])
            +((fmorey))*(b21*(-sinangle[it(6,i)])+b23*cosangle[it(6,i)])
            +((fmorez))*(b31*(-sinangle[it(6,i)])+b33*cosangle[it(6,i)]) )
       )));
            
/*991109 DO NOT CORRECT NORMALS FOR PERSPECTIVE, else wierd lighting !????*/
#ifdef NORMALSPERSPECTIVE
              if(Lperspec)
              {
                delz = fmore[3][j] - (float)izset;
                if( delz > (eyeposition - 50))  delta = 50;
                else delta = eyeposition - delz;
                    /*big neg delz just makes less offset*/

                delx = fmore[1][j] - (float)ixseting[it(6,i)];
                temp = (delx*delz)/(delta);
                fmore[1][j] = temp + fmore[1][j];

                dely = fmore[2][j] - (float)iyset;
                temp = (dely*delz)/(delta);
                fmore[2][j] = temp + fmore[2][j];
              }
#endif /* NORMALSPERSPECTIVE */

              if(   fmore[1][j] < .001 && fmore[1][j] > -.001
                 && fmore[2][j] < .001 && fmore[2][j] > -.001
                 && fmore[3][j] < .001 && fmore[3][j] > -.001)
              {
                 fmore[1][j] = (float).001;
                 fmore[2][j] = (float).001;
                 fmore[3][j] = (float).001;
              }
              if(moreit > 0)
                 imore = moreit; /*rect part flag: jmax one will persist*/ 
              else
                 imore = 1;
            }/* triangle, ribbon list may have relevant extra info*/
            else imore = 0;
            
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
                    ,&x1,&y1,&z1,&x2,&y2,&z2,type,limitz); 
      }
      if(!Lok){return;}
      /*then unpack the temporaries into the locals used below*/
      fthex[1]=x1;
      fthey[1]=y1;
      fthez[1]=z1;
      fthex[2]=x2;
      fthey[2]=y2;
      fthez[2]=z2; 

        if(it(1,i)!=oldx && it(2,i)!=oldy) (*inewpoint)=1;/*check point number*/
    }/*ALWAYS do floating point recalculation 990331*/

    /*now have working x,y for current move or move-draw*/
    if( type  == 0 || type == 6 || type == 11 || type == 14) 
    {/*vector or dot or ARROW or emphasis which only works for vectors 031127*/
        getptIDstring(word, itpointptr[i]);
        if( type  == 0 || type == 14)  /*emphasis 14 031127*/
        {/* vector ==> cylinder of radius penwidth */
           if(type == 14) {penwidth = 9;} /*fudge for emphasis 031127*/
           RENDERcylinder(fthex[1],fthex[2],fthey[1],fthey[2],fthez[1],fthez[2]
               ,(float)penwidth,icolor,word);
        }
        else if(type == 6)
        {/* dot ==> sphere of radius penwidth */
           RENDERsphere(fthex[2],fthey[2],fthez[2],(float)penwidth,icolor,word);
        }
        else if(type == 11)
        {/*ARROW*/
           RENDERarrow(fthex[1],fthex[2],fthey[1],fthey[2],fthez[1],fthez[2]
               ,(float)penwidth,icolor,word,itpointptr[i]);
        }
    }/*vector or dot*/
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
          if(!LR3Dstrokedtext)
          {  
             RENDERtext(fthex[2],fthey[2],fthez[2],icolor,labelsfontsize,word);
          }
          else
          {  
             RENDERstrokedtext(fthex[2],fthey[2],fthez[2],(float)penwidth
                             ,icolor,labelsfontsize,word);   
          } 
    }/*label*/
    else if( type == 1 || type == 7 ) /* ball or sphere list */
    {/*ball*/
        /*balltoscreen(it(1,i),it(2,i),it(3,i),it(4,i),it(5,i),ipen,icolor);*/
        /*it(3,ncount) = idisk*/
        /*it(4,ncount) = intradius*/
        /*it(5,ncount) = Lblackrim 040925 place black disk behind ball????*/
        /*note: jx2==jx1 && jy2==jy1 for balllists*/ 
               
        /*ball at current color*/

        idisk   = it(3,i);
        iradius = it(4,i);
#ifdef THISSEEMSUSELESS
        {/*keep ultimate capping disk from */
         /*obscuring highlight on penutimate disk*/
            if(iradius == 2) iradius = 1; /*2: drop to 1*/
            else if(iradius <=1) iradius = -1; /*-1: don't draw*/
        }
#endif
        if(iradius >= 0 && (idisk == 0 || idisk == 1) )
        {/*draw sphere of finite radius for only the base disk*/
            radius = (float)iradius;
            getptIDstring(word, itpointptr[i]);
            RENDERsphere(fthex[2],fthey[2],fthez[2],radius,icolor,word);
        }
    }/*ball*/
    else if( type  == 4 ) /* word list */
    {/*words*/
        /*wordstoscreen( -it(5,i)  ,it(1,i),it(2,i), icolor );*/
        getcommentstring(commentstr, itpointptr[i]);/*MAGELIST gets wordcom[]*/

        if(commentstr[2] != 0) k = commentstr[2];
        else k = wordsfontsize;
        for(j=3;j<=255;j++)
        {/*strip off format info*/
            commentstr[j-3] = commentstr[j];
            if(commentstr[j]=='\0') break;
        }
        if(!LR3Dstrokedtext)
        {
           RENDERtext(fthex[2],fthey[2],fthez[2],icolor,k,commentstr);
        }
        else
        {
           RENDERstrokedtext(fthex[2],fthey[2],fthez[2],(float)penwidth
                           ,icolor,k,commentstr);   
        } 
    }/*words*/
    else if( type  == 2 || type  == 5) /* triangle, ribbon list */
    {/* triangle, ribbon list */
       /*remember: 2 is current pt, 1 is preceeding, 3 is one before 1 !!!! */

       getptIDstring(word, itpointptr[i]);
       RENDERtriangle( fthex[3],fthey[3],fthez[3]
                      ,fthex[1],fthey[1],fthez[1]
                      ,fthex[2],fthey[2],fthez[2]
                      ,icolor
                      ,fmore[1][3],fmore[2][3],fmore[3][3]
                      ,fmore[1][1],fmore[2][1],fmore[3][1]
                      ,fmore[1][2],fmore[2][2],fmore[3][2]
                      ,imore,word);
icount++;
if(imore == 0) printf("MAGE fault: RENDERtriangle %d, {%s} has NO normal\n",icount,word);
/*#define NORMALSATPOINTS*/
#ifdef NORMALSATPOINTS      
       if(imore)
       {/*temp to show vectors*/
          if(fmore[1][3]< 0.001 && fmore[2][3]< 0.001 && fmore[3][3]< 0.001
          && fmore[1][3]>-0.001 && fmore[2][3]>-0.001 && fmore[3][3]>-0.001)
          {
             icolor = 201; /*red4*/
             fmore[1][3]=1.0;
             fmore[2][3]=1.0;
             fmore[3][3]=1.0;
          }
          RENDERcylinder( fthex[3], fthex[3]+100*fmore[1][3]
                         ,fthey[3], fthey[3]+100*fmore[2][3]
                         ,fthez[3], fthez[3]+100*fmore[3][3]
                         ,penwidth,icolor);
          if(fmore[1][1]< 0.001 && fmore[2][1]< 0.001 && fmore[3][1]< 0.001
          && fmore[1][1]>-0.001 && fmore[2][1]>-0.001 && fmore[3][1]>-0.001)
          {
             icolor = 201; /*red4*/
             fmore[1][1]=1.0;
             fmore[2][1]=1.0;
             fmore[3][1]=1.0;
          }
          RENDERcylinder( fthex[1], fthex[1]+100*fmore[1][1]
                         ,fthey[1], fthey[1]+100*fmore[2][1]
                         ,fthez[1], fthez[1]+100*fmore[3][1]
                         ,penwidth,icolor);
          if(fmore[1][2]< 0.001 && fmore[2][2]< 0.001 && fmore[3][2]< 0.001 
          && fmore[1][2]>-0.001 && fmore[2][2]>-0.001 && fmore[3][2]>-0.001)
          {
             icolor = 201; /*red4*/
             fmore[1][2]=1.0;
             fmore[2][2]=1.0;
             fmore[3][2]=1.0;
          }
          RENDERcylinder( fthex[2], fthex[2]+100*fmore[1][2]
                         ,fthey[2], fthey[2]+100*fmore[2][2]
                         ,fthez[2], fthez[2]+100*fmore[3][2]
                         ,penwidth,icolor);
       }
#endif
    }/* triangle, ribbon list */
    else if(type ==  9 ) /*MARKTYPE and RINGSTYLE*/
    {/*ring: x,y,radius  raster3Drender already set color and penwidth */
       getptIDstring(word, itpointptr[i]);
       Rfringtoscreen( fthex[2],fthey[2],fthez[2],(float)it(4,i),icolor
                      ,origpenwidth,word);
    }
    else if(type ==  8 ) /*MARKTYPE*/
    {/*mark: x,y,z,radius  raster3Drender has decoded icolor and penwidth */
       getptIDstring(word, itpointptr[i]);  /*MAGE global char[256] word*/
       Rfmarktoscreen( fthex[2],fthey[2],fthez[2],(float)it(4,i),icolor
                      ,origpenwidth,word);
    }
    else if(type == 10 ) /*MARKTYPE and FANSTYLE*/
    {/*fan at x,y,parent point: use point radius, (current color and pen)*/
       /*and send coloroffset for this depth and screen bkg*/
       getptIDstring(word, itpointptr[i]);
       Rffantoscreen( fthex[2],fthey[2],fthez[2],icolor,origpenwidth,word
                     ,50*ideepcolor + idecue,itpointptr[i]);
    }
}
/*___raster3Drender()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writeraster3Drender()****************************************************/
int    writeraster3Drender()
{
    cx = ((float)(GWIDTH)/2) + (float)ixtran;
    cy = ((float)(GHEIGHT)/2) + (float)iytran;
    cz = ((float)(GZDEEP)/2) + (float)iztran;
    sc = (float)oldmaxwide;
 if(!LR3Dappend)
 {/*new file, write header stuff*/    
  if(Lraster3Drender == 1)
  {/*for Raster3D Render program*/
  fprintf(fpout,"%s %c",NameStr,EOLOUT);
  /*scheme 3 produces (2/3)NTX*NPX by (2/3)NTY*NPY image */
  /*where NPX & NPY divisible by 3*/
  /*fprintf(fpout,"128 128    NTX,NTY  tiles in x,y %c",EOLOUT);*/
  /*fprintf(fpout,"12 12      NPX,NPY   pixels (x,y) per tile %c",EOLOUT);*/
  /*fprintf(fpout,"3          SCHEME anti-aliasing level %c",EOLOUT);*/
  /* scheme 3 is said to be obselete, use 4 instead*/
  /*scheme 4 produces NTX*NPX by NTY*NPY image */
  /*where NPX & NPY divisible by 2*/
  fprintf(fpout,"64 64      NTX,NTY  tiles in x,y %c",EOLOUT);
  fprintf(fpout,"16 16      NPX,NPY   pixels (x,y) per tile %c",EOLOUT); 
  fprintf(fpout,"4          SCHEME anti-aliasing level %c",EOLOUT); 
  if(Lwhitebkg)
     fprintf(fpout,"1 1 1      BKGND background, 1 1 1 for white (0 0 0 for black) %c",EOLOUT);
  else
     fprintf(fpout,"0 0 0      BKGND background, 0 0 0 for black (1 1 1 for white) %c",EOLOUT);
  if(LR3Dshadow)
     fprintf(fpout,"T       SHADOW  T with, F omit shadows %c",EOLOUT); 
  else
     fprintf(fpout,"F       SHADOW  T with, F omit shadows %c",EOLOUT); 
  fprintf(fpout,"25         IPHONG Phong power %c",EOLOUT);  
  fprintf(fpout,"0.25       STRAIT  secondary light percent contribution %c",EOLOUT);
   /*originally STRAIT 0.25*/
  fprintf(fpout,"0.05       AMBIEN  ambient light percent contribution %c",EOLOUT);
   /*originally was using AMBIEN 0.05, then tried 0.55*/
  fprintf(fpout,"0.25       SPECLR  specular reflection  percent contribution %c",EOLOUT);
   /*originally SPECLR 0.25*/
  if(Lperspec)
     fprintf(fpout,"4          EYPOS for perspective, 0 for orthographic %c",EOLOUT); 
  else
     fprintf(fpout,"0          EYPOS for perspective, 0 for orthographic %c",EOLOUT); 
  fprintf(fpout,"-1 1 1     SOURCE primary light position, 1 1 1 right shoulder %c",EOLOUT);
  fprintf(fpout,"1 0 0 0    TMAT  post-multipy a horizontal vector x y z 1 %c",EOLOUT);
  fprintf(fpout,"0 -1 0 0   TMAT %c",EOLOUT);
  fprintf(fpout,"0 0 1 0    TMAT %c",EOLOUT); 
  fprintf(fpout,"%.3f %.3f %.3f %.3f    TMAT %c",-cx,cy,-cz,sc,EOLOUT);
  fprintf(fpout,"3          INMODE input mode must be 3 for flagged type %c",EOLOUT);
  fprintf(fpout,"*          INFMTS free format for triangles and planes, type 1 (normals: 13) %c",EOLOUT);
  fprintf(fpout,"*          INFMTS free format for sphere descriptors, type 2 %c",EOLOUT);
  fprintf(fpout,"*          INFMTS free format for cylinder descriptors, type 3 %c",EOLOUT);
  }/*for Raster3D Render program*/
  else if(Lraster3Drender == 2)
  {/*for POV-Ray render program*/
   fprintf(fpout,"#include \"colors.inc\"%c",EOLOUT);  
   if(Lwhitebkg)
      fprintf(fpout,"background {color White}%c",EOLOUT);
   else
      fprintf(fpout,"background {color Black}%c",EOLOUT);
   fprintf(fpout,"//MAGE image was %d wide, %d high %c",GWIDTH,GHEIGHT,EOLOUT);
   fprintf(fpout,"//final camera and light positions at end of file %c",EOLOUT);
   /*start with camera at center*/
   /*move camera later to be clear of image: calculate maximum entent of image*/
   maxz = cz; /*accumulate maximum z beyond which to position camera at end*/


  }/*for POV-Ray render program*/
 }/*new file, write header stuff*/    
 /*overload Lraster3Drender == 1 for Raster3D, OR == 2 for POV-Ray*/
 Lspecialloop = 1; /*safety, should already be set*/
 return(1); /*will contine writing to fpout in MAGEREND/raster3Drender()*/
}
/*___writeraster3Drender()___________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****raster3Drenderfinish()***************************************************/
void raster3Drenderfinish()
{/*finish anything left*/
  int SCRNSIZE=0;

  if(GWIDTH > GHEIGHT) SCRNSIZE = GHEIGHT; /*mage image square in smaller dim*/
  else SCRNSIZE = GWIDTH;

  if(Lraster3Drender == 1)
  {/*for Raster3D Render program*/
    fprintf(fpout,"9 %c",EOLOUT);       /*end materials*/
    fprintf(fpout,"0 %c",EOLOUT);       /*needed ????*/
  }/*for Raster3D Render program*/
  else if(Lraster3Drender == 2)
  {/*for POV-Ray render program*/
   /*back camera off from center to be clear of all image elements*/
   /*put top at bottom to match our screen coords: 030121:NOT WORKING NOW*/
   /*right goes right, look from +z to -z, end up with R-handed system*/

   fprintf(fpout,"camera { %c",EOLOUT);
   fprintf(fpout,"   orthographic //use MAGE's perspective, if any %c",EOLOUT);
   fprintf(fpout,"   location <0,0, %.3f> //move camera back clear of image %c",maxz+100,EOLOUT); 
   fprintf(fpout,"   up <0,%d,0> //scale %c",SCRNSIZE,EOLOUT);
   fprintf(fpout,"   right <%d,0,0> //scale %c",SCRNSIZE,EOLOUT);
   fprintf(fpout,"   rotate <0,0,180> //coord system compensation %c",EOLOUT);
   fprintf(fpout,"   translate <%.3f, %.3f, %.3f> //move to center %c",cx,cy,cz,EOLOUT);
   fprintf(fpout,"   look_at <0,0,0> //camera looks in -z direction %c",EOLOUT);
   fprintf(fpout,"   } %c",EOLOUT);
      /*angle from camera not used for orthographic POV-Ray */
      /* with MAGE calculating any perspective shifts */
   
   /*put a light source well off over and behind viewer's left shoulder*/
   /*remember flip of coordinate system, so up is -y, left is -x, camera at +z*/
   fprintf(fpout,"light_source { <%.3f, %.3f, %.3f> color White shadowless} //1:over left shoulder %c"
      ,-cx,-cy,2*maxz,EOLOUT); 
      /*this position would give a MAGE-like highlight on a shiny sphere*/
   /*second over-the-shoulder light source to brighted the image*/
   fprintf(fpout,"light_source { <%.3f, %.3f, %.3f> color White shadowless} //2:over left shoulder %c"
      ,-cx,-cy,2*maxz,EOLOUT); 
      
   /*put a light source shining front-on from behind camera position*/
   fprintf(fpout,"light_source { <%.3f, %.3f, %.3f> color White shadowless} //front light %c"
      ,cx,cy,2*maxz,EOLOUT); 
      /*Unfortunately, shodowless lights give no highlights either. */
      /*Shadows can be very confusing in our virtual reality of a universe */
      /*where we explicitly see 3D relationships.  Like ancient Egyptians, */
      /*we want to see what is really there, however, relative geometry is */
      /*very important to us, so we work only with light and connections */
      /*to reveal relationships, rather than show "real-world-like" */
      /*2D surfaces from which we must infer 3D relationships*/
   if(Lwhitebkg)
      fprintf(fpout,"fog { distance %.3f color White} //depth cue %c",2*maxz,EOLOUT); 
   else
      fprintf(fpout,"fog { distance %.3f color Black} //depth cue %c",2*maxz,EOLOUT); 
      /*fog into the background matches MAGE's color change depth cueing*/
  }/*for POV-Ray render program*/

    fclose(fpout);
    fpout = NULL;
    Lraster3Drender = 0;
    Lspecialloop = 0;
}
/*___raster3Drenderfinish()_________________________________________________*/

