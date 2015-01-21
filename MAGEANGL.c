/************************MAGEANGL*********************************************/
#include "MAGE.h"
#include "MAGELIST.h"
 
#define EXTERNANGL
#include "MAGEANGL.h"
#include "MAGEFLAG.h"
#include "MAGEBBOX.h"
#include "MAGEMENU.h" /*121105*/

void doxrayplotter(pointstructptr, float, int*, int*); 
void rotatemobiledock(double[3],double,double[3]);
void translatemobiledock(double[3],double[3]);

/****domeasures()*************************************************************/
void    domeasures()
{
static  int       nmeasures = 0;
        double    angle,dihedral;
        float     sumx,sumy,sumz,fxyztemp[3];        
static  struct pointstruct* measurelinepointptr;/*keeps track of which of */ 
static  struct pointstruct* measuredotspointptr; /*the 1-to-4 points have  */
                                                /* been reached */
static  double    SUMX,SUMY,SUMZ; /*running sums for average of 4+ points*/
static  float  fcxyz[3][7]; /*030930*/
/* 080916:
fprintf(stderr,"\n enter domeasures()   mmmmMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM \n");
*/

        if(Lnewmeasures)
        {
            nmeasures = 0;
            Lnewmeasures = 0;
            anglemeasure = 0.0;
            dihedralmeasure = 0.0;
            measuregrupptr->on = 1; /* enable display of this measures group*/
            measurelinelistptr->on = 0; /*start with list of points off */
            measuredotslistptr->on = 0; /*start with list of average dots off*/
            measurelinepointptr = measurelinelistptr->firstpointptr;
            measuredotspointptr = measuredotslistptr->firstpointptr;
            SUMX=SUMY=SUMZ=0.0; /*971204*/
            measured1pointptr = NULL; /*091212 for surveillance*/
            measured2pointptr = NULL; /*091212 for surveillance*/
            measured3pointptr = NULL; /*091212 for surveillance*/
            measured4pointptr = NULL; /*091212 for surveillance*/
        }
        
            nmeasures++; /*up to four successive picked points */
            SUMX = SUMX + pickedpointptr->fx;
            SUMY = SUMY + pickedpointptr->fy;
            SUMZ = SUMZ + pickedpointptr->fz;
        if(nmeasures <= 5)
        {/*points for lines increase up to 4*/

            if(nmeasures == 1)
            {/*first of a new series*/ 
                measurelinepointptr = measurelinelistptr->firstpointptr;
            }
            else if(nmeasures <= 4)
            {/*continuing*/
                measurelinepointptr = measurelinepointptr->nextptr; 
            }
            if(nmeasures <= 4)
                measurelinelistptr->lastpointptr = measurelinepointptr; 
                /*added to show list*/
            if(nmeasures == 2)
            {
                measuredotspointptr = measuredotslistptr->firstpointptr;
                measuredotslistptr->lastpointptr = measuredotspointptr; 
                    /*added to show list*/
            }        
            else if(nmeasures >  2 && nmeasures <=  5)
            {
                measuredotspointptr = measuredotspointptr->nextptr;
                measuredotslistptr->lastpointptr = measuredotspointptr; 
                    /*added to show list*/
            }
            /* store vectors along the picked path as Move-Draw-Draw-Draw*/
            /* then up to 3 Dots for the average of 2, 3, 4 picked points*/
            /* these are in the different list structures */
        }/*points for lines increase up to 4*/
        /*store values temporarily in case pickvec is a measures point*/
        fxyztemp[0] = pickedpointptr->fx;
        fxyztemp[1] = pickedpointptr->fy;
        fxyztemp[2] = pickedpointptr->fz;
            
        measured4pointptr = measured3pointptr; /*091212 surveillance */
        measured3pointptr = measured2pointptr; 
        measured2pointptr = measured1pointptr; 
        measured1pointptr = pickedpointptr;

        if(nmeasures > 1) /* for all later picks after first */
        {
            /*NOTE measure lines are drawn from last picked back */
            /* to the earlier picked ones*/
            thispointptr = measurelinelistptr->lastpointptr;
            while( thispointptr != NULL )
            {
                thispointptr->fx = (thispointptr->previousptr)->fx;
                thispointptr->fy = (thispointptr->previousptr)->fy;
                thispointptr->fz = (thispointptr->previousptr)->fz;
                thispointptr->ix = (thispointptr->previousptr)->ix;
                thispointptr->iy = (thispointptr->previousptr)->iy;
                thispointptr->iz = (thispointptr->previousptr)->iz;
                thispointptr->colorwidth 
                    = (thispointptr->previousptr)->colorwidth;
                thispointptr->type = VECTOR; /*pickable DrawTo*/
                
                thispointptr = thispointptr->previousptr;
                if(thispointptr == measurelinelistptr->firstpointptr)
                    thispointptr = NULL;
            }
        }
        
        /* put last pick point into working list at first position*/
        (measurelinelistptr->firstpointptr)->fx = fxyztemp[0];
        (measurelinelistptr->firstpointptr)->fy = fxyztemp[1];
        (measurelinelistptr->firstpointptr)->fz = fxyztemp[2];
        calcintegerpoints(measurelinelistptr->firstpointptr);/*MAGELIST*/
        (measurelinelistptr->firstpointptr)->colorwidth = 0; /*use list's*/
        (measurelinelistptr->firstpointptr)->type = (VECTOR|MOVETO_P);
                                                   /*pickable MoveTo*/
        
        if(nmeasures >= 2)  
        {   
            /*NOTE measure dots are calc over each time */
            /*calculate average, put into first dot position*/
            (measuredotslistptr->firstpointptr)->fx
                = (  (measurelinelistptr->firstpointptr)->fx
                   +((measurelinelistptr->firstpointptr)->nextptr)->fx )/2;
            (measuredotslistptr->firstpointptr)->fy
                = (  (measurelinelistptr->firstpointptr)->fy
                   +((measurelinelistptr->firstpointptr)->nextptr)->fy )/2;
            (measuredotslistptr->firstpointptr)->fz
                = (  (measurelinelistptr->firstpointptr)->fz
                   +((measurelinelistptr->firstpointptr)->nextptr)->fz )/2;

            measurelinelistptr->on = 1;/*turn list of points on for first line*/
            measuredotslistptr->on = 1; /*turn list of average dots on */
            calcintegerpoints(measuredotslistptr->firstpointptr);/*MAGELIST*/
            (measuredotslistptr->firstpointptr)->colorwidth = 0;
        }
        
        if(nmeasures >= 3)  /*calculate angle*/
        {/*nmeasures >= 3*/
            /*going backwards, so first three give latest angle*/
/*.*/        angle = angle3pt(  
                (measurelinelistptr->firstpointptr)->fx
               ,(measurelinelistptr->firstpointptr)->fy
               ,(measurelinelistptr->firstpointptr)->fz
               ,((measurelinelistptr->firstpointptr)->nextptr)->fx
               ,((measurelinelistptr->firstpointptr)->nextptr)->fy
               ,((measurelinelistptr->firstpointptr)->nextptr)->fz
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz);
            anglemeasure = (float)angle; /* transfer by common storage */
            
            /*calculate average of 3, */
            /* put into dots position beyond average of 2 */
            ((measuredotslistptr->firstpointptr)->nextptr)->fx 
              =( (measurelinelistptr->firstpointptr)->fx
                +((measurelinelistptr->firstpointptr)->nextptr)->fx
                +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx
               )/3;
            ((measuredotslistptr->firstpointptr)->nextptr)->fy 
              =( (measurelinelistptr->firstpointptr)->fy
                +((measurelinelistptr->firstpointptr)->nextptr)->fy
                +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy
               )/3;
            ((measuredotslistptr->firstpointptr)->nextptr)->fz 
              =( (measurelinelistptr->firstpointptr)->fz
                +((measurelinelistptr->firstpointptr)->nextptr)->fz
                +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz
               )/3;
        
            calcintegerpoints((measuredotslistptr->firstpointptr)->nextptr);
                /*MAGELIST*/
            ((measuredotslistptr->firstpointptr)->nextptr)->colorwidth = 0;

          if(Lmeasureperpendics)
          {/*perpendicular distance of third point to line of first two*/
             /*measures index is push down stack, fcxyz is indexed in order*/
             fcxyz[0][3] = (measurelinelistptr->firstpointptr)->fx;
             fcxyz[1][3] = (measurelinelistptr->firstpointptr)->fy;
             fcxyz[2][3] = (measurelinelistptr->firstpointptr)->fz;
             fcxyz[0][2] = ((measurelinelistptr->firstpointptr)->nextptr)->fx;
             fcxyz[1][2] = ((measurelinelistptr->firstpointptr)->nextptr)->fy;
             fcxyz[2][2] = ((measurelinelistptr->firstpointptr)->nextptr)->fz;
             fcxyz[0][1] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx;
             fcxyz[1][1] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy;
             fcxyz[2][1] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz;
             DoPerpendicularToLine(fcxyz);
             /*fcxyz[012][0] has xyz of perpendicular pt on the line*/
             perpendictoline = (float)sqrt(
                      ((fcxyz[0][3] - fcxyz[0][0])*(fcxyz[0][3] - fcxyz[0][0]))
                    + ((fcxyz[1][3] - fcxyz[1][0])*(fcxyz[1][3] - fcxyz[1][0]))
                    + ((fcxyz[2][3] - fcxyz[2][0])*(fcxyz[2][3] - fcxyz[2][0]))
                   );
          }/*perpendicular distance of third point to line of first two*/

        }/*nmeasures >= 3*/
        
        if(nmeasures >= 4)  /*calculate dihedral*/  /*971204*/
        {/*nmeasures >= 4*/
/*.*/        dihedral = dihedral4pt(
                (measurelinelistptr->firstpointptr)->fx
               ,(measurelinelistptr->firstpointptr)->fy
               ,(measurelinelistptr->firstpointptr)->fz
               ,((measurelinelistptr->firstpointptr)->nextptr)->fx
               ,((measurelinelistptr->firstpointptr)->nextptr)->fy
               ,((measurelinelistptr->firstpointptr)->nextptr)->fz
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy
               ,(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz
               ,((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fx
               ,((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fy
               ,((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fz
              );
               /*030930: NOTE: there are only 4 points in measures' train,*/
               /*and the fourth point is at the lastpointptr!!!!*/
               /*indeed this fourth and the last ptr do have the same values!*/
               /*the confusion is that average points use a running x,y,z sum*/
               /*for more than 4 pts in a measures sequence.*/
               /*030930 mismatch: perpendicular to a plane is irreproducable*/
               /*,(measurelinelistptr->lastpointptr)->fx*/
               /*,(measurelinelistptr->lastpointptr)->fy*/
               /*,(measurelinelistptr->lastpointptr)->fz*/
                                
            dihedralmeasure = (float)dihedral; /* transfer by common storage */
            
            /*calculate average of 4, */
            /*put into dots position beyond average of 3 */
            sumx = 0.0;
            sumy = 0.0;
            sumz = 0.0;
            thispointptr = measurelinelistptr->firstpointptr;
            while(thispointptr != NULL)
            {
                sumx = sumx + thispointptr->fx;
                sumy = sumy + thispointptr->fy;
                sumz = sumz + thispointptr->fz;
                if(thispointptr == measurelinelistptr->lastpointptr)
                    thispointptr = NULL;
                else thispointptr = thispointptr->nextptr;
            }
         (((measuredotslistptr->firstpointptr)->nextptr)->nextptr)->fx = sumx/4;
         (((measuredotslistptr->firstpointptr)->nextptr)->nextptr)->fy = sumy/4;
         (((measuredotslistptr->firstpointptr)->nextptr)->nextptr)->fz = sumz/4;
            calcintegerpoints(
                ((measuredotslistptr->firstpointptr)->nextptr)->nextptr );
                /*MAGELIST*/
         (((measuredotslistptr->firstpointptr)->nextptr)->nextptr)->colorwidth
                 = 0;
           if(Lmeasureddihedralon)
           {/*label midpt of dihedral axis with dihedral value  020220*/ 
            mdihedx 
            =( ((measurelinelistptr->firstpointptr)->nextptr)->fx
              +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx)/2;
            mdihedy 
            =( ((measurelinelistptr->firstpointptr)->nextptr)->fy
              +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy)/2;
            mdihedz 
            =( ((measurelinelistptr->firstpointptr)->nextptr)->fz
              +(((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz)/2;
              dodrawline(); /* put dihedralmeasure at mdihedx,mdihedy,mdihedz*/
           }
          if(Lmeasureperpendics)
          {/*perpendicular distance of fourth point to plane of first three*/
             /*measures index is push down stack, fcxyz is indexed in order*/
             fcxyz[0][4] = (measurelinelistptr->firstpointptr)->fx;
             fcxyz[1][4] = (measurelinelistptr->firstpointptr)->fy;
             fcxyz[2][4] = (measurelinelistptr->firstpointptr)->fz;
             fcxyz[0][3] = ((measurelinelistptr->firstpointptr)->nextptr)->fx;
             fcxyz[1][3] = ((measurelinelistptr->firstpointptr)->nextptr)->fy;
             fcxyz[2][3] = ((measurelinelistptr->firstpointptr)->nextptr)->fz;
             fcxyz[0][2] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fx;
             fcxyz[1][2] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fy;
             fcxyz[2][2] = (((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->fz;
             fcxyz[0][1] = ((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fx;
             fcxyz[1][1] = ((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fy;
             fcxyz[2][1] = ((((measurelinelistptr->firstpointptr)->nextptr)->nextptr)->nextptr)->fz;
             /*fcxyz[0][1] = (measurelinelistptr->lastpointptr)->fx;*/
             /*fcxyz[1][1] = (measurelinelistptr->lastpointptr)->fy;*/
             /*fcxyz[2][1] = (measurelinelistptr->lastpointptr)->fz;*/
             DoPerpendicularToPlane(fcxyz);
             /*fcxyz[012][0] has xyz of perpendicular pt in the plane*/
             perpendictoplane = (float)sqrt(
                      ((fcxyz[0][4] - fcxyz[0][0])*(fcxyz[0][4] - fcxyz[0][0]))
                    + ((fcxyz[1][4] - fcxyz[1][0])*(fcxyz[1][4] - fcxyz[1][0]))
                    + ((fcxyz[2][4] - fcxyz[2][0])*(fcxyz[2][4] - fcxyz[2][0]))
                   );
          }/*perpendicular distance of fourth point to plane of first three*/
        }/*nmeasures >= 4*/  
        if(nmeasures >= 5)  /*971204*/
        {/*nmeasures >= 5*/  
            (measuredotslistptr->lastpointptr)->fx = (float)SUMX/nmeasures;
            (measuredotslistptr->lastpointptr)->fy = (float)SUMY/nmeasures;
            (measuredotslistptr->lastpointptr)->fz = (float)SUMZ/nmeasures;   
            calcintegerpoints(measuredotslistptr->lastpointptr);   
            (measuredotslistptr->lastpointptr)->colorwidth = 0;  
            sprintf(word,"avg of %d pts",nmeasures);/* Dot for average point*/
            storeptIDstring(word, measuredotslistptr->lastpointptr);/*MAGELIST*/
        }/*nmeasures >= 5*/ /*971204*/
}
/*___domeasures()____________________________________________________________*/

/****dosurveillancemeasured()************************************************/
void dosurveillancemeasured(void)
{ 
   double ang1=0,middist=0,ang2=0,dihedral=0;
   if(   measured1pointptr!=NULL 
      && measured2pointptr!=NULL
      && measured3pointptr!=NULL
      && measured4pointptr!=NULL)
   { 
      ang1= angle3pt
         (measured4pointptr->fx,measured4pointptr->fy,measured4pointptr->fz,
          measured3pointptr->fx,measured3pointptr->fy,measured3pointptr->fz,
          measured2pointptr->fx,measured2pointptr->fy,measured2pointptr->fz);
      middist= sqrt(
               (  ( measured3pointptr->fx - measured2pointptr->fx )
                * ( measured3pointptr->fx - measured2pointptr->fx ))
              +(  ( measured3pointptr->fy - measured2pointptr->fy )
                * ( measured3pointptr->fy - measured2pointptr->fy ))
              +(  ( measured3pointptr->fz - measured2pointptr->fz )
                * ( measured3pointptr->fz - measured2pointptr->fz ))
            );
      ang2= angle3pt
         (measured3pointptr->fx,measured3pointptr->fy,measured3pointptr->fz,
          measured2pointptr->fx,measured2pointptr->fy,measured2pointptr->fz,
          measured1pointptr->fx,measured1pointptr->fy,measured1pointptr->fz);

      dihedral= dihedral4pt
         (measured1pointptr->fx,measured1pointptr->fy,measured1pointptr->fz,
          measured2pointptr->fx,measured2pointptr->fy,measured2pointptr->fz,
          measured3pointptr->fx,measured3pointptr->fy,measured3pointptr->fz,
          measured4pointptr->fx,measured4pointptr->fy,measured4pointptr->fz);

      sprintf(surveillancestr
          ,"ang1: %.3f, middist: %.3f, ang2: %.3f, dihedral: %.3f"
          ,ang1,middist,ang2,dihedral); /*121105 sprintf: does the null term*/ 
   }
   else
   {
      surveillancestr[0] = '\0';
   }
}
/*___dosurveillancemeasured()________________________________________________*/

/****angle3pt()*************************************************************/
double    angle3pt(    double p1x,double p1y,double p1z,
                    double p2x,double p2y,double p2z,
                    double p3x,double p3y,double p3z )
{
        double    ax,ay,az,bx,by,bz;
        double    amag,bmag,dot;
        double    angle;
        
            /* angle defined between two vectors with a common base*/
            /* so subtract middle point from the two end points */
            ax = p1x - p2x;
            ay = p1y - p2y;
            az = p1z - p2z;
            bx = p3x - p2x;
            by = p3y - p2y;
            bz = p3z - p2z;
            
        /* these have to be normalized by dividing by their magnitudes, */
        /*this can be done either before taking the dot product or after*/
/*.*/    dot = (double)dotproduct(ax,ay,az,bx,by,bz);
/*c*/    amag = sqrt(ax*ax + ay*ay + az*az);
/*c*/    bmag = sqrt(bx*bx + by*by + bz*bz);
        if(amag*bmag <0.0001) 
        {
            angle = 0.0;
        }    
/*c*/    else angle = acos( dot/(amag*bmag) );
        angle = angle*360.0/(2*3.14159);
        return(angle);
}
/*___angle3pt()____________________________________________________________*/

/****crossproduct()***********************************************************/
void    crossproduct(    double ax,double ay,double az,
                        double bx,double by,double bz)
{
    double gx,gy,gz;
    gx = (ay)*(bz) - (az)*(by);
    gy = (az)*(bx) - (ax)*(bz);
    gz = (ax)*(by) - (ay)*(bx);
    /*(*rx) = gx;*/
    /*(*ry) = gy;*/
    /*(*rz) = gz;*/
    dblx = gx;
    dbly = gy;
    dblz = gz;
    if(Ltest>1) 
    { 
        sprintf(alertstr,"cross product:"
                         CRLF"%f = %f = %f x %f - %f x %f"
                         CRLF"%f = %f = %f x %f - %f x %f"
                         CRLF"%f = %f = %f x %f - %f x %f"
                        ,dblx,gx,ay,bz,az,by
                        ,dbly,gy,az,bx,ax,bz
                        ,dblz,gz,ax,by,ay,bx               );
        dosinglealert(3); /*____DLOG.C*/
    }
}
/*___crossproduct()__________________________________________________________*/

/****dotproduct()***********************************************************/
double        dotproduct(    double ax,double ay,double az,
                        double bx,double by,double bz )
{
    double answer;
    
    answer = ax*bx + ay*by + az*bz;
    return(answer);
}
/*___dotproduct()__________________________________________________________*/

/****dihedral4pt()************************************************************/
double    dihedral4pt(  double p1x,double p1y,double p1z,
                        double p2x,double p2y,double p2z,
                        double p3x,double p3y,double p3z,
                        double p4x,double p4y,double p4z )
{
        double    angle,angledhdrl,dot,dmag,emag,fmag;
        double    ax,ay,az,bx,by,bz,cx,cy,cz;
        double    dx,dy,dz,ex,ey,ez;
        double    fx,fy,fz;
        
        /* a,b,c 3 vectors between 4 points, */
        /* d,e   2 crossproduct vectors referenced as pointers*/
        /* f     a crossproduct vector  referenced as pointer */
        /* if d = a x b and e = b x c, then d orth a & b, e orth b & c */
        /* d & e both perpendicular to b */
        /* so d dot e is the dihedral, that is */
        /*   same angle as a to c if a & c were projected on a plane*/
        
        /* angle defined between two vectors with a common base*/
        /* so subtract middle point from the two end points */
        ax = p1x - p2x;
        ay = p1y - p2y;
        az = p1z - p2z;
        bx = p3x - p2x;
        by = p3y - p2y;
        bz = p3z - p2z;
            
        /* d = a x b */
/*.*/    crossproduct(ax,ay,az,bx,by,bz); /*return vector is dblx,dbly,dblz */
        dx = dblx;
        dy = dbly;
        dz = dblz;
        
        /* angle defined between two vectors with a common base*/
        /* so subtract middle point from the two end points, */
        /* redefining b for cross product around third point (b = -b)*/
        bx = p2x - p3x;   /* -b as far as d is concerned */
        by = p2y - p3y;
        bz = p2z - p3z;
        cx = p4x - p3x;
        cy = p4y - p3y;
        cz = p4z - p3z;
        
        /* e = b x c */
/*.*/    crossproduct(bx,by,bz,cx,cy,cz); /*return vector is dblx,dbly,dblz */
        ex = dblx;
        ey = dbly;
        ez = dblz;
        
        /*Now for d dot e  pass values to dotproduct()*/
/*.*/    dot = dotproduct(dx,dy,dz,ex,ey,ez);
        /*normalization factors*/
/*c*/    dmag = sqrt( (dx)*(dx) + (dy)*(dy) + (dz)*(dz) );
/*c*/    emag = sqrt( (ex)*(ex) + (ey)*(ey) + (ez)*(ez) );
        if(dmag*emag <0.0001) 
        {
            angle = 0.0;
        }    
/*c*/    else angle = acos( dot/(dmag*emag) );
        angledhdrl = angle*360.0/(2*3.14159);
        if(Ltest)
        {
           sprintf(alertstr,"angle = %f  radians\015angle = %f  degrees"
                        ,angle,angledhdrl);
           dosinglealert(3); /*MACDLOG.C*/ /*MPCDLOG.C*/
        }
        
        /* Now, need to establish correct handedness */
        /* d x -b is vector pointing in same sort of direction as orig a */
        /* f = d x -b = d x b as b was redefined for e calculation */
/*.*/    crossproduct(dx,dy,dz,bx,by,bz); /*return vector is dblx,dbly,dblz */
        fx = dblx;
        fy = dbly;
        fz = dblz;
        
        /*Now for f dot e  pass values to dotproduct()*/
/*.*/    dot = dotproduct(fx,fy,fz,ex,ey,ez);

        /*normalization factors*/
/*c*/    fmag = sqrt( (fx)*(fx) + (fy)*(fy) + (fz)*(fz) );

        if(fmag*emag <0.0001) 
        {
            angle = 0.0;
        }    
/*c*/    else angle = acos( dot/(fmag*emag) );
        if(Ltest) { sprintf(alertstr,    "angle = %f  radians",angle );
             dosinglealert(3); /*MACDLOG.C*/ /*MPCDLOG.C*/
        }

        angle = angle*360.0/(2*3.14159);
        if(angle > 90.0) angledhdrl = -angledhdrl;
        if(Ltest) { sprintf(alertstr,    "signed angle = %f  degrees"
                        ,angledhdrl);
             dosinglealert(3); /*MACDLOG.C*/ /*MPCDLOG.C*/
        }

        return(angledhdrl);

}
/*___dihedral4pt()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****dodrawline()************************************************************/
void    dodrawline()
{
   float    fvectorx,fvectory,fvectorz,veclength;
   float    fmiddlex,fmiddley,fmiddlez;/*971124SplitLine*/
   float    length,delx,dely,delz,startx,starty,startz;
   int      ndots,j;
   char     startID[256];
   
   if(Lnewdrawline)
   {
      numberline = 0;
      Lnewdrawline = 0;
      drawnewlistptr->on = 0; /*start with drawn lines off*/ 
      drawgrupptr->on = 1; /* enable display */
   }
   numberline++;
   if((thispointptr = allocpointstructure(drawnewlistptr)) == NULL)/*MAGELIST*/
   {/*allocation failed*/
      sprintf(alertstr,"failed to alloc a drawline point");
      alertstr2[0]='\0';alertstr3[0]='\0';
        DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
        numberline--;
        return;
   }/*allocation failed*/
   /*can construct another point */
   maxnxyz++;

   if(Lmeasureddihedralon)
   {/*make label of dihedralmeasure at mid pt of dideral axis*/
       thispointptr->fx = mdihedx;
       thispointptr->fy = mdihedy;
       thispointptr->fz = mdihedz;
   }
   else
   {
       thispointptr->fx = pickedpointptr->fx;
       thispointptr->fy = pickedpointptr->fy;
       thispointptr->fz = pickedpointptr->fz;
   }
   calcintegerpoints(thispointptr); /*MAGELIST*/
   thispointptr->colorwidth = 0; /*use list's default color*/
   
   thispointptr->type = (VECTOR|MOVETO_P);
       /*P== MoveTo, presume start of new line*/
       /*then if it isn't, type will be changed below*/
   /*STATUS of last picked point probably OK even for dihedral label*/
   if(   (((pickedpointptr->STATUS) & STATICFLAG) == STATICFLAG)
      || (((pickedpointptr->listptr)->STATUS) & STATICFLAG) == STATICFLAG)
   {
      thispointptr->STATUS = thispointptr->STATUS | STATICFLAG;
   }
   prelastdrawnpointptr = lastdrawnpointptr;
   lastdrawnpointptr = pickedpointptr; /*not so good for dihedral label*/
   
   if(Lmeasureddihedralon)
   {/*pointID is the dihedral angle value*/
      sprintf(word,"%.3f",dihedralmeasure);
      storeptIDstring( (char*)word,thispointptr);
      Lpoint = 1; /*needed just below here in code*/
   }
   else
   {
      /*copy pointID into new position(n) in com array from pickvec position*/
      /*stored pointID string NOT conditioned */
      /*copy char str from storage */
      getptIDstring(word, pickedpointptr);
      /*store directly in giant character array */
      storeptIDstring( (char*)word,thispointptr);
   }
   /*if a Point, the next entry will define a line and */
   /* constructing the actual, possibly shortened, drawn line */
   if(Lpoint > 1) /*Lnewlabels or Ldraglabel or Lnewballs never lets Lpoint>0*/
   {/*second of Point-Line sequence*/
      if(LSplitLine)    /*971124SplitLine*/
      {/*calculate the lines midpoint for later use*/
       fmiddlex=(float)((thispointptr->fx+(thispointptr->previousptr)->fx)/2.0);
       fmiddley=(float)((thispointptr->fy+(thispointptr->previousptr)->fy)/2.0);
       fmiddlez=(float)((thispointptr->fz+(thispointptr->previousptr)->fz)/2.0);      
      }/*971124SplitLine*/
      
      if(shortenline > 0.0001 || shortenline < -0.0001) 
      /* i.e. surely non-zero */
      {/*shorten line between last two entered triples and update*/
      
          fvectorx = thispointptr->fx - (thispointptr->previousptr)->fx;
          fvectory = thispointptr->fy - (thispointptr->previousptr)->fy;
          fvectorz = thispointptr->fz - (thispointptr->previousptr)->fz;
          veclength = (float)sqrt(fvectorx*fvectorx + 
                                  fvectory*fvectory + 
                                  fvectorz*fvectorz  );
          /*normalize: */
          if(veclength > 0.0001) /*050407*/
          {
             fvectorx = fvectorx/veclength;
             fvectory = fvectory/veclength;
             fvectorz = fvectorz/veclength;
          }
          /*add to n-1, subtract from n to make */
          /*shortened line end positions*/
      
          (thispointptr->previousptr)->fx = 
              (thispointptr->previousptr)->fx + shortenline*fvectorx;
          (thispointptr->previousptr)->fy = 
              (thispointptr->previousptr)->fy + shortenline*fvectory;
          (thispointptr->previousptr)->fz = 
              (thispointptr->previousptr)->fz + shortenline*fvectorz;
          calcintegerpoints(thispointptr->previousptr);
          (thispointptr->previousptr)->colorwidth = 0; 
          
          thispointptr->fx = thispointptr->fx - shortenline*fvectorx;
          thispointptr->fy = thispointptr->fy - shortenline*fvectory;
          thispointptr->fz = thispointptr->fz - shortenline*fvectorz;
          calcintegerpoints(thispointptr);
          thispointptr->colorwidth = 0; /*use list's default color*/
              /*use list's default color*/
        
      }/*shorten line between last two entered triples and update*/
       
      /* store vectors as (Move-Draw)s, 2 entries per drawline*/
      /* but done sequentially as they come in as Point, Line */
      /* if shortened usually don't allow picking, set Ldrawunpickable */
      if(Ldrawunpickable == 1 )
      {
          (thispointptr->previousptr)->type = (VECTOR|MOVETO_P|UNPICKABLE);
              /*Point==Move,Unpickable*/
          thispointptr->type = (VECTOR|UNPICKABLE);
              /*Line==Draw, Unpickable*/
      }
      else
      {
           (thispointptr->previousptr)->type = (VECTOR|MOVETO_P);
               /* Point == Move */
           thispointptr->type = (VECTOR);  /* Line == Draw */            
      }
      if(Larrowline)
      {/*ARROW is a list and point type, set for tail and head points*/
         thispointptr->type = (thispointptr->type | ARROW);
         (thispointptr->previousptr)->type 
             = ((thispointptr->previousptr)->type | ARROW);
      }
      drawnewlistptr->on = 1; /*insure: drawn lines are on*/ 
      drawmarker1listptr->on = 0; /*turn off point indicater*/   
      drawmarker2listptr->on = 0; /*turn off point indicater*/  
      drawmarker3listptr->on = 0; /*turn off point indicater*/  
      drawmarker4listptr->on = 0; /*turn off point indicater*/ 
      Lpoint = 0; 
      if(LSplitLine && !Ldottedline)    /*SplitLine*/
      {/*allocate another pt and reassign a P-L-L sequence*/
        numberline++;
        if((thispointptr=allocpointstructure(drawnewlistptr))==NULL)/*MAGELIST*/
        {/*allocation failed*/
          sprintf(alertstr,"failed to alloc a drawline point");
          alertstr2[0]='\0';alertstr3[0]='\0';
          DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
          numberline--;
          return;
        }/*allocation failed*/
        /*can construct another point */
        maxnxyz++;
        /*new pt has all properties of the previous last pt, except color*/
        thispointptr->fx = (thispointptr->previousptr)->fx;
        thispointptr->fy = (thispointptr->previousptr)->fy;
        thispointptr->fz = (thispointptr->previousptr)->fz;
        thispointptr->type = (thispointptr->previousptr)->type;
        thispointptr->colorwidth = numberofcolor("red"); /*MAGECOLR.c*/
        getptIDstring(word, (thispointptr->previousptr));
        storeptIDstring( (char*)word,thispointptr);
        thispointptr->STATUS = (thispointptr->previousptr)->STATUS;
        thispointptr->STYLE  = (thispointptr->previousptr)->STYLE;
        
        /*previous last pt is same except located at midpoint of line*/ 
        (thispointptr->previousptr)->fx = fmiddlex;
        (thispointptr->previousptr)->fy = fmiddley;
        (thispointptr->previousptr)->fz = fmiddlez;
        (thispointptr->previousptr)->colorwidth = numberofcolor("blue"); 
           /*MAGECOLR.c*/ /*DrawTo so blue start*/
      }/*971124SplitLine*/
      else if(Ldottedline)
      {/*making dotted line*/
         /*make the two end points of this drawline be dots*/
         if(Ldrawunpickable == 1 )
          thispointptr->type=(thispointptr->previousptr)->type=(DOT|UNPICKABLE);
         else
          thispointptr->type = (thispointptr->previousptr)->type = DOT;
         if(LSplitLine)

         {
            thispointptr->colorwidth = numberofcolor("red"); /*MAGECOLR.c*/ 
               /*red finish*/
            (thispointptr->previousptr)->colorwidth = numberofcolor("blue"); 
               /*MAGECOLR.c*/ /*blue start*/
         }
         /*compute number of dots to see if more needed inbetween ends*/
         delx = (thispointptr->fx - (thispointptr->previousptr)->fx);
         dely = (thispointptr->fy - (thispointptr->previousptr)->fy);
         delz = (thispointptr->fz - (thispointptr->previousptr)->fz);
         length = delx*delx + dely*dely + delz*delz;
         if(length > 0) length = (float)sqrt(length); /*length is a float*/
         ndots = (int)(length*4); /*dots per unit length*/
         if(ndots > 2)
         {/*create additional points*/
            /*remember starting point coords to compute intermediates*/
            startx = (thispointptr->previousptr)->fx;
            starty = (thispointptr->previousptr)->fy;
            startz = (thispointptr->previousptr)->fz;
            getptIDstring(startID, (thispointptr->previousptr));
            for(j=1;j<=(ndots-2);j++)
            {/*new intermediate points constructed from effective finish point*/
               numberline++;
               if((thispointptr=allocpointstructure(drawnewlistptr))==NULL)
               {/*allocation failed*/ /*MAGELIST*/
                 sprintf(alertstr,"failed to alloc a drawline point");
                 alertstr2[0]='\0';alertstr3[0]='\0';
                 DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
                 numberline--;
                 return;
               }/*allocation failed*/
               /*can construct another point, old thispoint now previous point*/
               maxnxyz++;
               /*new pt has all properties of the previous last pt because*/
               /* it becomes the old finish point moved ahead in the pt list*/
               thispointptr->fx = (thispointptr->previousptr)->fx;
               thispointptr->fy = (thispointptr->previousptr)->fy;
               thispointptr->fz = (thispointptr->previousptr)->fz;
               thispointptr->type = (thispointptr->previousptr)->type;
               getptIDstring(word, (thispointptr->previousptr));
               storeptIDstring( (char*)word,thispointptr);
               thispointptr->STATUS = (thispointptr->previousptr)->STATUS;
               thispointptr->STYLE  = (thispointptr->previousptr)->STYLE;
               thispointptr->colorwidth=(thispointptr->previousptr)->colorwidth;

               /*point now left one behind in the list needs new coord */
               /*(and color?)*/
               (thispointptr->previousptr)->fx = startx + j*(delx/(ndots-1));
               (thispointptr->previousptr)->fy = starty + j*(dely/(ndots-1));
               (thispointptr->previousptr)->fz = startz + j*(delz/(ndots-1));
               if(LSplitLine)
               {
                 if(j<(ndots/2))
                 {
                   storeptIDstring((char*)startID,(thispointptr->previousptr));
                   (thispointptr->previousptr)->colorwidth = numberofcolor("blue"); /*MAGECOLR.c*/
                 }
                 else (thispointptr->previousptr)->colorwidth = numberofcolor("red"); /*MAGECOLR.c*/
               }
            }/*each new intermediate point*/
         }/*create additional points*/
      }/*making dotted line*/
   }/*second of Point-Line sequence*/
   /*if(Lnewlabelson == 1 || Ldraglabelson==1)*/ 
   if(Lnewlabelson == 1 || Lmeasureddihedralon == 1) 
   {
      thispointptr->type = LABEL;  /* Label */ 
      drawnewlistptr->on = 1; /*insure: drawn lines are on*/ 
      drawmarker1listptr->on = 0; /*turn off point indicater*/   
      drawmarker2listptr->on = 0; /*turn off point indicater*/  
      drawmarker3listptr->on = 0; /*turn off point indicater*/
      drawmarker4listptr->on = 0; /*turn off point indicater*/ 
      Lpoint = 0;
   }
   if(Lnewballson == 1) 
   {
      thispointptr->type = BALL;  /* Ball */ 
      thispointptr->colorwidth =   ((thispointptr->colorwidth) & ~31) 
                                 | numberofcolor("magenta");
      drawnewlistptr->on = 1; /*insure: drawn lines are on*/ 
      drawmarker1listptr->on = 0; /*turn off point indicater*/   
      drawmarker2listptr->on = 0; /*turn off point indicater*/  
      drawmarker3listptr->on = 0; /*turn off point indicater*/
      drawmarker4listptr->on = 0; /*turn off point indicater*/ 
      Lpoint = 0;
   }
   rescalekinemage(); 
}
/*___dodrawline()____________________________________________________________*/

/****doeraseline()************************************************************/
void doeraseline()
{
   if(Ldock3on3on) /*020305*/
   {/*subvert doeraseline to back out of any picking dodock3on3 points*/
      if(Lpoint > 0)
      {
         if(Lpoint==6) drawmarker6listptr->on = 0; /*probably can't get here*/
         else if(Lpoint==5) drawmarker5listptr->on = 0;
         else if(Lpoint==4) drawmarker4listptr->on = 0;
         else if(Lpoint==3) drawmarker3listptr->on = 0;
         else if(Lpoint==2) drawmarker2listptr->on = 0;
         else if(Lpoint==1) drawmarker1listptr->on = 0;
         Lpoint = Lpoint - 1;
      }
   }
   else
   {/*erase last line or completely get out of drawing attempt*/
      /*n = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT + numberline;*/
      if(Lpoint > 0 && (Lconstruct4on || Lconstruct5on || Lconstruct6on) ) 
      {  /*constructions do no drawing until all points in, 140912 add 6th*/
         Lpoint = 0;
      }
      else if(  (Lpoint > 0 && numberline >= 1)
              ||(  (( drawnewlistptr->lastpointptr)!=NULL)   
                 &&(((drawnewlistptr->lastpointptr->type)&VECTOR)==0) ) 
             )
      {/*point start is one position, as is a label*/
         numberline = numberline - 1;
         maxnxyz--;
         Lpoint = 0;
         if(drawnewlistptr->lastpointptr != NULL)
         {/*there is at least one point in the drawnew list*/
            destroypointstructure(drawnewlistptr->lastpointptr); /*MAGELIST*/
            /*this routine takes care of reconnection and parent updating*/
            /* it also protects itself against NULL pointptrs! */
         }/*there is at least one point in the drawnew list*/
      }
      else if(numberline >= 2)
      {/*full point-line is 2 positions*/
         numberline = numberline - 2;
         maxnxyz--;
         maxnxyz--;
         Lpoint = 0;
         if(drawnewlistptr->lastpointptr != NULL)
         {/*there is at least one point in the drawnew list*/
            destroypointstructure(drawnewlistptr->lastpointptr); /*MAGELIST*/
            /*this routine takes care of reconnection and parent updating*/
         }/*there is at least one point in the drawnew list*/
         if(drawnewlistptr->lastpointptr != NULL)
         {/*there is at least one point in the drawnew list*/
            destroypointstructure(drawnewlistptr->lastpointptr); /*MAGELIST*/
            /*this routine takes care of reconnection and parent updating*/
         }/*there is at least one point in the drawnew list*/
      }
      if(numberline <  0) 
      {
         numberline = 0;
         drawnewlistptr->lastpointptr = NULL;  /*this should be redundant*/
         drawnewlistptr->firstpointptr = NULL; /*this should be redundant*/
             /*reset show list*/ 
               
               drawnewlistptr->on = 0; /*turn off list*/ /*960527*/ 
               maxnxyz = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT;

      }
      drawmarker1listptr->on = 0; /*turn off marker1*/
      drawmarker2listptr->on = 0; /*turn off marker2*/
      drawmarker3listptr->on = 0; /*turn off marker3*/
      drawmarker4listptr->on = 0; /*turn off marker4*/ /*971122*/
      drawmarker5listptr->on = 0; /*turn off marker5*/ /*020305*/
      drawmarker6listptr->on = 0; /*turn off marker6*/ /*020305*/
   }/*erase last line or completely get out of drawing attempt*/
}
/*___doeraseline()__________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****doconstructline()*******************************************************/
void    doconstructline() /*140912 overloaded with construct6 multi-monitor*/
{
           float  fvectorx,fvectory,fvectorz,veclength;
   static  float  fcxyz[3][7]; /*140912 ?? MAYBE NEED 7 to 8 for costruct6 ??*/
           double angledhdrl,cmag;
           double ax,ay,az,bx,by,bz,cx,cy,cz;
           float  costheta; /*for projection to tangent plane*/
   static  int    fixedstate;
           int    nth, nminus; 
   /*double A1X,A1Y,A1Z,A2X,A2Y,A2Z,ADX,ADY,ADZ,ART;*/ /*for perpendicular*/
   /*double B1X,B1Y,B1Z,B2X,B2Y,B2Z;*/ /*for perpendicular*/

   int       ntines,nrepeat;
   
   if     (Lconstruct4on) {nth = 4; nminus = 3;}
   else if(Lconstruct5on) {nth = 5; nminus = 4;}
   else if(Lconstruct6on) {nth = 7; nminus = 6;}
                                  /*140912 4 define glycosidic vectors*/
                                  /*140912 2 for 2 P of PO4 -1 and 1 */
                          //BUT NEED NTH > LPOINT TO STORE POINT
//BEWARE: LOGIC OF CONSTRUCT6 IS BACKWARDS FROM OTHERS
//Lpoint is incremented again in ConstructSixthDialog to get routed here

   if(Lpoint<nth)
   {/*accumulating n-1 points from which to construct nth point*/
        if(Lnewdrawline)
        {
            numberline = 0;
            Lnewdrawline = 0;
            drawnewlistptr->on = 0; /*start with drawn lines off*/ /*960527*/
            drawgrupptr->on = 1; /* enable display */
        }
        /* get this last picked point into coordinate list at new position */
        
        fcxyz[0][Lpoint] = pickedpointptr->fx;
        fcxyz[1][Lpoint] = pickedpointptr->fy;
        fcxyz[2][Lpoint] = pickedpointptr->fz;

        dockpointptr[Lpoint] = pickedpointptr; /*140912*/
        //MAGELIST.h/pointstructptr dockpointptr[7];
        //overload docking points and nucleic param points
               
        /*Wait for 3 or 4 entries to define a line before */
        /* constructing the actual, possibly shortened, drawn line */
        
        /*use point indicator to signel when enough points entered */
//LPOINT IS INCREMENTED WITHIN THE PICK CODE IN MAGEDRAW.c
//LPOINT NOT USED LOCALLY BELOW IN THIS FILE, BUT NEEDED BY PICK MOUSEDOWN.
//LPOINT IS A GLOBAL, AND CONTROLS AT WHICH PICK A CONSTRUCTION DIALOG IS CALLED
        if( Lconstruct4on && Lpoint==3 ) { Lpoint=4; }
        if( Lconstruct5on && Lpoint==4 ) { Lpoint=5; }
    
        if( Lconstruct6on && Lpoint==6 ) { Lpoint=7; }
        /*At fruition, this will be intercepted at a successful pick report in*/
        /* ____MAIN after Drawgraf() with Lpick==TRUE, then Dialog box called*/
   }/*accumulating n-1 points from which to construct nth point*/

/*---------------------------------------------------------------------------*/
   else  /*"middle" dividing line of doconstructline() */
/*---------------------------------------------------------------------------*/

   {/*construct nth point based on previous n-1 points*/ 
    /*Lpoint=nth, i.e. Lpoint==4||Lpoint==5||Lpoint==6||Lpoint==7 140912*/
    if(Lconstruct5on && LAngleBetweenLines) /*140518*/
    {/*Angle only 140518*/
       DoAngleBetweenLines(fcxyz); /*140518*/
    }

    /*usual case allocate 2 or multiples of 2 points for drawn lines 140518*/
    else if(  (Lconstructperpendicular)
            ||(distconstruct != 0 && angleconstruct != 0) )
    {/*distconstruct and angleconstruct both non-zero*/
       /*or doing Lconstructperpendicular*/    
     if(   (((pickedpointptr->STATUS) & STATICFLAG) == STATICFLAG)
        || (((pickedpointptr->listptr)->STATUS) & STATICFLAG) == STATICFLAG)
          fixedstate = 1;
     else fixedstate = 0;  
        /*constructed points fixed crt last point picked*/

     if(LconstructMultiple) /*971126ARROW*/
     {
        ntines = (int)(360/dihedralconstruct);
     }
     else
     {
        ntines = 1;
     }/*971126ARROW*/

     for(nrepeat = 1; nrepeat<=ntines; nrepeat++)/*971126ARROW*/
     {/*nrepeat of constructline, dihedral incremented*/ /*971126ARROW*/

      /*for any construction mode: common code to alloc two new points*/
      /*since always draw both ends of the new line*/ /*971122*/
           /* called 4th time from DoconstructDialog() with values for */
           /* distconstruct, angleconstruct, dihedralconstruct*/
           /*now have 3 entries to define a line, can */
           /* construct the actual, possibly shortened, drawn line */
           /* called 5th time from DoConstructFifthDialog()*/ /*971122*/
           /* called 6th time from DoConstructSixthDialog()*/ /*140912*/
           numberline = numberline + 2;
           /*n = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT + numberline;*/
           /*970410*/
        
        /*#1...of  making 2 new points: n-1 and n ...*/
        if((thispointptr = allocpointstructure(drawnewlistptr)) == NULL)
        {/*allocation failed*/ /*MAGELIST*/
            sprintf(alertstr,"failed to alloc a 1st constructline point");
             alertstr2[0]='\0';alertstr3[0]='\0';
             DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
             numberline--;
             return;
        }/*allocation failed*/
        /*can construct another point */
        
        if(fixedstate)
        {
           thispointptr->STATUS = thispointptr->STATUS | STATICFLAG;
        }
        thispointptr->type = (VECTOR|MOVETO_P);/*default simple Point == Move */
        if(Ldrawnewghost)
        {
           thispointptr->STATUS = thispointptr->STATUS|GHOST;
           thispointptr->colorwidth = numberofcolor("white");
        }
        else
        {
           thispointptr->colorwidth = 0;
        }

        /*#2...of  making 2 new points: n-1 and n ...*/
        if((thispointptr = allocpointstructure(drawnewlistptr)) == NULL)
        {/*allocation failed*/ /*MAGELIST*/
            sprintf(alertstr,"failed to alloc a 2nd constructline point");
             alertstr2[0]='\0';alertstr3[0]='\0';
             DoMageDLOGreport(2); /*forced message*/ /*___DLOG.C*/ 
             numberline--;
             return;
        }/*allocation failed*/
        /*can construct another point */
        
        if(fixedstate)
        {
           thispointptr->STATUS = thispointptr->STATUS | STATICFLAG;
        }
        thispointptr->type = (VECTOR); /*default simple Line == Draw */
        if(Ldrawnewghost)
        {
           thispointptr->STATUS = thispointptr->STATUS|GHOST;
           thispointptr->colorwidth = numberofcolor("white");
        }
        else
        {
           thispointptr->colorwidth = 0;
        }
        
        /*get this last picked point into coordinate list at new position*/
        ((drawnewlistptr->lastpointptr)->previousptr)->fx = pickedpointptr->fx;
        ((drawnewlistptr->lastpointptr)->previousptr)->fy = pickedpointptr->fy;
        ((drawnewlistptr->lastpointptr)->previousptr)->fz = pickedpointptr->fz;
           /*ptIDs... making 2 new points: n-1 and n ...*/
           /*copy pointID into new position(n-1)  from pickvec position*/
        /*copy pointID into new position(n) in com array from pickvec position*/
        /*stored pointID string NOT conditioned */
        /*copy char str from storage */
        getptIDstring(word, pickedpointptr);
        /*store directly in giant character array */
        storeptIDstring(word,(drawnewlistptr->lastpointptr)->previousptr);
        calcintegerpoints( (drawnewlistptr->lastpointptr)->previousptr );
        
        /*create a new ptID for the second new point: n */
        sprintf(word,"construc");
        storeptIDstring(word,drawnewlistptr->lastpointptr);


      if((Lconstruct4on) && !Lconstructperpendicular) 
      {/*construct at distance,angle,dihedral*/ 
           
           /*calculate the new line from points 1,2,3, */
           /* dist 3-4, angle 2-3-4, dihedral 1-2-3-4 */
           /* cross:  (2->3)X(2->1) ==> (2->5), aXb ==> c at dihedral 90*/
           ax = fcxyz[0][3] - fcxyz[0][2];
           ay = fcxyz[1][3] - fcxyz[1][2];
           az = fcxyz[2][3] - fcxyz[2][2];
           bx = fcxyz[0][1] - fcxyz[0][2];
           by = fcxyz[1][1] - fcxyz[1][2];
           bz = fcxyz[2][1] - fcxyz[2][2];
           /* c = a x b */
/*.*/      crossproduct(ax,ay,az,bx,by,bz);/*return vector is dblx,dbly,dblz*/
           cx = dblx;cy = dbly;cz = dblz;
           /*normalization factor*/
/*
           printf("cross product:"
                         CRLF"%f = %f x %f - %f x %f"
                         CRLF"%f = %f x %f - %f x %f"
                         CRLF"%f = %f x %f - %f x %f"
                        ,dblx,ay,bz,az,by
                        ,dbly,az,bx,ax,bz
                        ,dblz,ax,by,ay,bx               );
*/
           cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) );

           if(Lconstructprojection)
           {/*project line from sphere of radius dist to tangent plane*/
              costheta = (float)cos( (double)(angleconstruct)*3.14159*2/360);
              if(costheta > .00001)
              cmag = cmag*costheta;
           }

           /*place c at correct distance*/
           if(cmag > 0.000001)
           {
               cx = cx*distconstruct/cmag;
               cy = cy*distconstruct/cmag;
               cz = cz*distconstruct/cmag;
           }
           /*place c at correct distance from 3rd point,*/
           /* as xproduct it is at a dihedral of 90 to a vector*/
           cx = cx + fcxyz[0][3];
           cy = cy + fcxyz[1][3];
           cz = cz + fcxyz[2][3];
           /*load actual array with only-distance-set point*/
           thispointptr->fx = (float)cx;
           thispointptr->fy = (float)cy;
           thispointptr->fz = (float)cz;
           /*rotate around 2->3 for correct dihedral*/
           
           /*angledhdrl = dihedralconstruct - 90.0;*/
           angledhdrl = nrepeat*dihedralconstruct - 90.0; /*971126ARROW*/
           
           ax = fcxyz[0][2];
           ay = fcxyz[1][2];
           az = fcxyz[2][2];
           bx = fcxyz[0][3];
           by = fcxyz[1][3];
           bz = fcxyz[2][3];
           doaxisrot(&(thispointptr->fx),&(thispointptr->fy),&(thispointptr->fz)
                 ,(float)angledhdrl, ax, ay, az
                                   , bx, by, bz);
           /* cross:  (3->4)X(3->2) ==> (3->6), aXb ==> c at dihedral 90*/
           ax = thispointptr->fx - fcxyz[0][3];
           ay = thispointptr->fy - fcxyz[1][3];
           az = thispointptr->fz - fcxyz[2][3];
           bx = fcxyz[0][2] - fcxyz[0][3];
           by = fcxyz[1][2] - fcxyz[1][3];
           bz = fcxyz[2][2] - fcxyz[2][3];
           /* c = a x b */
/*.*/      crossproduct(ax,ay,az,bx,by,bz);/*return vector is dblx,dbly,dblz*/
           cx = dblx;cy = dbly;cz = dblz;
           /*normalization factor*/

/*c*/      cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) );
           if(cmag > 0.000001)
           {
               cx = cx/cmag;
               cy = cy/cmag;
               cz = cz/cmag;
           }

           /*rotate around 3->6 for correct angle*/
           angledhdrl = 90.0 - angleconstruct;
           ax = fcxyz[0][3];
           ay = fcxyz[1][3];
           az = fcxyz[2][3];
           cx = cx + fcxyz[0][3];
           cy = cy + fcxyz[1][3];
           cz = cz + fcxyz[2][3];
           bx = cx;
           by = cy;
           bz = cz;
           doaxisrot(&(thispointptr->fx),&(thispointptr->fy),&(thispointptr->fz)
                 ,(float)angledhdrl, ax, ay, az
                                   , bx, by, bz);
 
           /*stop for now and see if this is reasonable so far*/
           if((shortenline > 0.0001 || shortenline < 0.0001)) 
           /* i.e. surely non-zero */
           {    /*shorten line between last two entered triples and update*/
                
                fvectorx = thispointptr->fx - (thispointptr->previousptr)->fx;
                fvectory = thispointptr->fy - (thispointptr->previousptr)->fy;
                fvectorz = thispointptr->fz - (thispointptr->previousptr)->fz;

                veclength = (float)sqrt(   fvectorx*fvectorx +
                                    fvectory*fvectory + 
                                    fvectorz*fvectorz  );
                /*normalize: */
                if(veclength > 0.000001)
                {
                    fvectorx = fvectorx/veclength;
                    fvectory = fvectory/veclength;
                    fvectorz = fvectorz/veclength;
                }
                /*add to n-1, subtract from n to make */
                /*shortened line end positions*/
                
                (thispointptr->previousptr)->fx 
                    = (thispointptr->previousptr)->fx + shortenline*fvectorx;
                (thispointptr->previousptr)->fy 
                    = (thispointptr->previousptr)->fy + shortenline*fvectory;
                (thispointptr->previousptr)->fz
                    = (thispointptr->previousptr)->fz + shortenline*fvectorz;
                calcintegerpoints(thispointptr->previousptr);
                
                thispointptr->fx = thispointptr->fx - shortenline*fvectorx;
                thispointptr->fy = thispointptr->fy - shortenline*fvectory;
                thispointptr->fz = thispointptr->fz - shortenline*fvectorz;
              
                calcintegerpoints(thispointptr);
           }
           /* load integer display list */
            
           /*maxnxyz = n;*/
           maxnxyz = nxyz + NMARKERS + MAXMEASURES + 3*NDRAWPOINT + numberline;
           /*970410*/ 
           /* store vectors as (Move-Draw)s, 2 entries per drawline*/
           /* but done sequentially as they come in as Point, Line */
           /* originally, if shortened, don't allow picking */
           /*if(shortenline > 0.0001 )*/
           if(Ldrawunpickable == 1) /*explicitly control unpickable-ness*/
           {
                (thispointptr->previousptr)->type=(VECTOR|MOVETO_P|UNPICKABLE);
                    /*Point==Move,Unpickable*/
                thispointptr->type = (VECTOR|UNPICKABLE);  
                    /* Line == Draw, Unpickable */
           }
           else
           {
                (thispointptr->previousptr)->type = (VECTOR|MOVETO_P);  
                    /* Point == Move */
                thispointptr->type = (VECTOR);  /* Line == Draw */            
           }

           if(Lconstructdot) 
           {/*dot at end instead of line*/
              thispointptr->type = (DOT); /*dot*/
           }

           drawnewlistptr->on = 1; /*insure: drawn lines are on*/ /*960527*/
    
      }/*construct at distance,angle,dihedral*/
      else if(Lconstruct4on)
      {/*construct perpendicular from point to line*/    
         DoPerpendicularToLine(fcxyz);
         (thispointptr->previousptr)->type 
             = (thispointptr->previousptr)->type |VECTOR|MOVETO_P;

         thispointptr->fx = fcxyz[0][0]; /*030930 rtn coord in 0th array*/
         thispointptr->fy = fcxyz[1][0];
         thispointptr->fz = fcxyz[2][0];
         thispointptr->type = thispointptr->type |VECTOR;
         calcintegerpoints(thispointptr);
         
         drawnewlistptr->on = 1;

      }/*construct perpendicular from point to line*/    
      else if(Lconstruct5on && LPerpendicularToPlane)
      {
         DoPerpendicularToPlane(fcxyz); /*030930 rtn coord in 0th array*/
         thispointptr->fx = fcxyz[0][0];
         thispointptr->fy = fcxyz[1][0];
         thispointptr->fz = fcxyz[2][0];
         calcintegerpoints(thispointptr); 
      }
      else if(Lconstruct5on && LPerpendicularBetweenLines)
      {
         DoPerpendicularBetweenLines(fcxyz);
      }
      else if(Lconstruct5on && LShortestBetweenLinesegments)
      {
         DoShortestBetweenLinesegments(fcxyz);
      }
      drawnewlistptr->on = 1;
     }/*nrepeat of constructline, dihedral incremented*/ /*971126ARROW*/
    }/*distconstruct and angleconstruct both non-zero*/
       /*or doing Lconstructperpendicular*/    
    /*ALL ABOVE DRAW LINES THAT CAN BE MULTIPLE*/
    /*140912 construct6 does not fit above paradigm (yet!?)*/
    //else if(Lconstruct6on && LNucleicAcidParameters) /*140912*/
    else if(Lconstruct6on) /*140915*/
    {
       //refers to MAGELIST.h/pointstructptr dockpointptr[7];
       //DoNucleicAcidParameters(fcxyz); /*140912*/
       DoNucleicAcidParameters(); /*dockpointptr[7],141006*/
       drawnewlistptr->on = 1;
    }
    /*Lpoint=nth, i.e. Lpoint==4||Lpoint==5||Lpoint==6||Lpoint==7 140912*/
   }/*construct nth point based on previous n-1 points*/ 
}
/*___doconstructline()______________________________________________________*/

/****doaxisrot()*************************************************************/
void  doaxisrot(float* fxptr,float* fyptr,float* fzptr
,float theta,double ax,double ay,double az,double bx,double by,double bz)
{
   double xx,yy,zz,cosn1,cosn2,cosn3,costheta,sintheta;
   double fx1,fy1,fz1,fx2,fy2,fz2;
   double a11,a12,a13,a21,a22,a23,a31,a32,a33;
   int    LOK=1;
    
   xx = bx - ax;
   yy = by - ay;
   zz = bz - az;
   if(xx*xx + yy*yy + zz*zz > 0.000001)
   {
       cosn1 = (xx)/sqrt(xx*xx + yy*yy + zz*zz);
       cosn2 = (yy)/sqrt(xx*xx + yy*yy + zz*zz);
       cosn3 = (zz)/sqrt(xx*xx + yy*yy + zz*zz);
   }
   else
   {/*xx and yy and zz all must be very small, so*/
      /*cosn1,2,3 all very small and no way to normalize them*/ /*egam 030217*/
      /*and all off diagonal terms are very small*/
      /*also all diagonal terms use the square so even the sign not used*/
      cosn1 = xx;
      cosn2 = yy;
      cosn3 = zz;
      LOK = 0; /*not worth doing the matrix operation*/
   }
   while(theta >  360) theta = theta - 360;
   while(theta < -360) theta = theta + 360;
   if(theta >  180) theta = theta - 360;
   if(theta < -180) theta = theta + 360;
   
   if(theta > -.0001 && theta < 0.0001)
   {/*angle very small, off diagonal terms very small*/
      LOK = 0; /*not worth doing the matrix operation*/
   }

   if(LOK)
   {/*do matrix operation*/
      costheta = cos( (double)theta*3.14159*2/360);
      sintheta = sin( (double)theta*3.14159*2/360);

      a11=( cosn1*cosn1 + (1-cosn1*cosn1)*costheta);
      a12=( cosn1*cosn2*(1-costheta) + cosn3*sintheta );
      a13=( cosn1*cosn3*(1-costheta) - cosn2*sintheta );

      a21=( cosn1*cosn2*(1-costheta) - cosn3*sintheta );
      a22=( cosn2*cosn2 + (1-cosn2*cosn2)*costheta);
      a23=( cosn2*cosn3*(1-costheta) + cosn1*sintheta );

      a31=( cosn1*cosn3*(1-costheta) + cosn2*sintheta );
      a32=( cosn2*cosn3*(1-costheta) - cosn1*sintheta );
      a33=( cosn3*cosn3 + (1-cosn3*cosn3)*costheta);
      /*det=a11*(a22*a33-a23*a32)-a12*(a21*a33-a23*a31)+a13*(a21*a32-a22*a31);*/

      fx1 = ( *fxptr - bx );
      fy1 = ( *fyptr - by );
      fz1 = ( *fzptr - bz );

      fx2 = (   fx1*a11 + fy1*a21 + fz1*a31 );
      fy2 = (   fx1*a12 + fy1*a22 + fz1*a32 );
      fz2 = (   fx1*a13 + fy1*a23 + fz1*a33 );

      *fxptr = (float)(fx2 + bx); 
      *fyptr = (float)(fy2 + by); 
      *fzptr = (float)(fz2 + bz); 
   }/*do matrix operation*/
}
/*___doaxisrot()____________________________________________________________*/    
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****dobondrot()*************************************************************/
void    dobondrot(int i,float theta)
{
   double xx,yy,zz,cosn1,cosn2,cosn3,costheta,sintheta;
   int   n;
   double fx1,fy1,fz1,fx2,fy2,fz2;
   double a11,a12,a13,a21,a22,a23,a31,a32,a33;
   static int Lnew=1, Lold=0; /*981107*/
   float center; /*001106*/
   pointstruct* thepointptr;
   pointstruct* thelastpointptr;
   liststruct*  thelistptr;     /*970905*/

   if(parameter[9] != 0)
   {/*for doxrayplotter(): crystal-to-film distance*/
      center=parameter[9];
   }
   else
   {/*defaults to Ewald sphere radius: (min+max)/2*/
      center = (parameter[1] + parameter[2])/2;
   }
   Lnewstuffactive = 1; /*for warning about write-out before quiting, etc.*/

   if(Lcontrolon) Lnew = 0; /*981107*/
   else           Lnew = 1;
    
   xx = (bondrotptr[i]->headpointptr)->fx - (bondrotptr[i]->tailpointptr)->fx;
   yy = (bondrotptr[i]->headpointptr)->fy - (bondrotptr[i]->tailpointptr)->fy;
   zz = (bondrotptr[i]->headpointptr)->fz - (bondrotptr[i]->tailpointptr)->fz;
   cosn1 = (xx)/sqrt(xx*xx + yy*yy + zz*zz);
   cosn2 = (yy)/sqrt(xx*xx + yy*yy + zz*zz);
   cosn3 = (zz)/sqrt(xx*xx + yy*yy + zz*zz);
   while(theta >  360) theta = theta - 360;
   while(theta < -360) theta = theta + 360;
   if(theta >  180) theta = theta - 360;
   if(theta < -180) theta = theta + 360;
   costheta = cos( (double)theta*3.14159*2.0/360.0);
   sintheta = sin( (double)theta*3.14159*2.0/360.0);

   a11=( cosn1*cosn1 + (1-cosn1*cosn1)*costheta);
   a12=( cosn1*cosn2*(1-costheta) + cosn3*sintheta );
   a13=( cosn1*cosn3*(1-costheta) - cosn2*sintheta );

   a21=( cosn1*cosn2*(1-costheta) - cosn3*sintheta );
   a22=( cosn2*cosn2 + (1-cosn2*cosn2)*costheta);
   a23=( cosn2*cosn3*(1-costheta) + cosn1*sintheta );

   a31=( cosn1*cosn3*(1-costheta) + cosn2*sintheta );
   a32=( cosn2*cosn3*(1-costheta) - cosn1*sintheta );
   a33=( cosn3*cosn3 + (1-cosn3*cosn3)*costheta);

/*
   det=a11*(a22*a33-a23*a32)-a12*(a21*a33-a23*a31)+a13*(a21*a32-a22*a31);
*/

   /*scope is usually taken as starting from the 3rd point input */
   /* to the point preceeding the vectorlist that defines the */
   /* end of the scope */
   /*In order to combine rotations rather than stack them hierachically*/
   /* one can define that several rotations have exactly the same scope*/
   /* e.g. to simulate precession motion */
   /* thus several successive bondrot axes can all have the scope of the*/
   /* last one in the series.  scopeinstance is therefore always pointing*/
   /* to a bondrot definition that occurred later */
   if(bondrotptr[i]->scopeinstance != 0) 
        n = bondrotptr[i]->scopeinstance;
   else n = i; /*the current bondrotptr index */
   thepointptr = bondrotptr[n]->firstpointptr;
   thelastpointptr  = bondrotptr[n]->lastpointptr;

   if(thelastpointptr == NULL) thepointptr = NULL; /*010902*/
   /*A BONDROT SCOPE THAT GOES TO THE END OF THE KINEMAGE WILL HAVE*/
   /* lastpointptr set == last point in MAGEINPUT at finishthiskinemage:.*/
      /*wierd end, so abort: */
      /*a failed clone just previous to the end of scope will do this*/
      
   while(thepointptr != NULL)
   {/*loop over all points in scope*/
      /*new coord = (transto000,* matrix elements, transback)*/
      /*matrix components from Rogers & Adams pg. 55 */

      fx1 = ( thepointptr->fx - (bondrotptr[i]->headpointptr)->fx );
      fy1 = ( thepointptr->fy - (bondrotptr[i]->headpointptr)->fy );
      fz1 = ( thepointptr->fz - (bondrotptr[i]->headpointptr)->fz );

      fx2 =          (   fx1*a11
                       + fy1*a21
                       + fz1*a31
                     );

      fy2 =          (   fx1*a12
                       + fy1*a22
                       + fz1*a32
                     );

      fz2 =          (   fx1*a13
                       + fy1*a23
                       + fz1*a33
                     );

      thepointptr->fx = (float)(fx2 + (bondrotptr[i]->headpointptr)->fx);
      thepointptr->fy = (float)(fy2 + (bondrotptr[i]->headpointptr)->fy);
      thepointptr->fz = (float)(fz2 + (bondrotptr[i]->headpointptr)->fz);
/*----------------*/
      if(thepointptr == beginselectionpointptr)
      {
         Lbeginselection = 1;
      }      
      if(   bondrotptr[i]->option == 3 && Lbeginselection)
      {/*selection of some by parameters*/
        if(thepointptr == endselectionpointptr)
        {
           Lbeginselection = 0;/*981008*/
        }
        if(   thepointptr->listptr->on
           && thepointptr->listptr->sgrpptr->on
           && thepointptr->listptr->sgrpptr->grupptr->on/*test only ON objects*/
           &&(   !Lcontrolon   /*test all points */
              || ((thepointptr->type) & PRUNED) == PRUNED ) )/*only OFF points*/
        {/*show points within min ... max limits*/ /*981010*/
          if(parameter[2] > 0.00001)
          {/*distmax*/
             if(((sqrt(  (thepointptr->fx -parameter[6])
                      *(thepointptr->fx -parameter[6])
                     + (thepointptr->fy -parameter[7])
                      *(thepointptr->fy -parameter[7])
                     + (thepointptr->fz -parameter[8])
                      *(thepointptr->fz -parameter[8]) ) ) )
                > parameter[2])
             {/*out by distmax, inclusive OR on the PRUNED flag*/
                 thepointptr->type=(thepointptr->type) | PRUNED;
             }/*add PRUNED flag*/
             else
             {/*within distmax, AND everything but the PRUNED flag*/
                 thepointptr->type = (thepointptr->type) & (32767-PRUNED);
             }/*remove PRUNED flag*/
          }/*distmax*/
          if(parameter[1] > 0.00001)
          {/*distmin*/
             if(((sqrt(  (thepointptr->fx -parameter[3])
                      *(thepointptr->fx -parameter[3])
                     + (thepointptr->fy -parameter[4])
                      *(thepointptr->fy -parameter[4])
                     + (thepointptr->fz -parameter[5])
                      *(thepointptr->fz -parameter[5]) ) ) )
                < parameter[1])
             {/*out by distmin, inclusive OR on the PRUNED flag*/
                     thepointptr->type = (thepointptr->type) | PRUNED; 
             }/*add PRUNED flag*/
             else
             {/*passed: is NOT below minimum*/ /*981107*/
               if(parameter[2] > 0.00001)
               {/*already tested to see if NOT above maximum*/
                  ; /*NOP, let previous test stand for all NOT below minimum*/
               }
               else
               {/*Allow anything that is NOT below minimum*/
                 thepointptr->type = (thepointptr->type) & (32767-PRUNED);
                 /*allow all >distmin*/  /*remove PRUNED flag*/
                 /* AND everything but the PRUNED flag*/
               }/*Allow anything that is NOT below minimum*/

             }/*passed: is NOT below minimum*/ /*981107*/
          }/*distmin*/
          if(  ((thepointptr->type) & PRUNED) == 0) /*981107*/
          {/*passed tests, plot points projected like rays onto flat film*/
             doxrayplotter(thepointptr,center,&Lnew,&Lold);  /*981107*/ 
                /*dobondrot() sets,stores Lnew,Lold; */
                /*doxrayplotter() uses,changes them*/        
          }/*passed tests, plot points projected like rays onto flat film*/
            /*981107*/
        }/*show points within min ... max limits*/
      }/*selection of some by parameters*/
/*--------------*/
    
      if(thepointptr == thelastpointptr)
      {/*end of scope*/
         thepointptr = NULL;
      }
      else if(thepointptr == (thepointptr->listptr)->lastpointptr)
      {/*at last point in a list*/
         /*970903 MAGE keeps track of all lists as a linked list during */
         /* allocliststructure() so only need to go up to the list level*/
         
         if((thepointptr->listptr)->nextptr != NULL) 
         {/*there is a next list: get first point of next list*/ 
            thelistptr = (thepointptr->listptr)->nextptr;
            thepointptr = thelistptr->firstpointptr;
            if(thepointptr == NULL) /*list with NO points*/
            {
               while(   (thepointptr == NULL) /*current point*/
                     && (thelistptr->nextptr != NULL) ) /*next list*/
               {/*search for a non-NULL point in the scope*/
                 thelistptr = thelistptr->nextptr; /*next list*/
                 thepointptr = thelistptr->firstpointptr;
               }
            }
         }
         else
         {/*NO  more lists, no more points to rotate*/
            /*somehow thelastpointptr failed to be found*/
            thepointptr = NULL;
         }
      }/*at last point in a list*/
      else
      { 
         thepointptr = thepointptr->nextptr;
      }
   }/*loop over all points in scope*/
}
/*___dobondrot()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****reinitialbondrots()*************************************************/
void reinitialbondrots()
{
  int i;
  float theta,thetain,thetaout;
  
  if(nbondrot > 0)
  {
       for(i=1; i<=nbondrot; i++)
       {
              if(bondrotptr[i]->origangle<0.0) 
                  thetain = 360 + bondrotptr[i]->origangle;
              else thetain = bondrotptr[i]->origangle;
              if(bondrotptr[i]->angle<0.0) 
                  thetaout = 360 + bondrotptr[i]->angle;
              else thetaout = bondrotptr[i]->angle;
              theta = thetain - thetaout;
              if(theta >  180.0) theta =  theta - 360;
              if(theta < -180.0) theta =  theta + 360;
              dobondrot(i,theta);
              bondrotptr[i]->angle = bondrotptr[i]->origangle;
       }
       LneedSetsinGrafWindow = 1; 
       
       if(Lupdate && !Linhibiton && Lprobeactive)
       {/*update from remote program: e.g. dots by PROBE*/
          Nthisgroupkind = PROBEDOTSKKIND; /*bondrot change needs probe run*/
          Ldeleteprobedots = 1;
          doupdate(1,update_probe_command_line);   
          /* 1: sends coords to probe*/
          Nthisgroupkind = 0;
       }

       Lrecalculate = 1;
       rescalekinemage();    /*MAGEINPT.C*/
       redrawvec();          /*MUXMDRAW.C*/
  }
}
/*___reinitialbondrots()________________________________________________*/

/****restorebondrots()*************************************************/
void restorebondrots(int istart, int iend)
{
  int i;
  float theta,thetain,thetaout;
  
       for(i=istart; i<=iend; i++)
       {
              if(bondrotptr[i]->origangle<0.0) 
                  thetain = 360 + bondrotptr[i]->origangle;
              else thetain = bondrotptr[i]->origangle;
              if(bondrotptr[i]->angle<0.0) 
                  thetaout = 360 + bondrotptr[i]->angle;
              else thetaout = bondrotptr[i]->angle;
              theta = thetain - thetaout;
              if(theta >  180) theta =  theta - 360;
              if(theta < -180) theta =  theta + 360;
              dobondrot(i,theta);
              bondrotptr[i]->angle = bondrotptr[i]->origangle;
       }
}
/*___restorebondrots()________________________________________________*/

/****initrot()*****initialize stored rotation matrix*************************/
void    initrot()
{
    int        i;
    
    for(i=1; i<=MAXRESETS ; i++)
    {
        ma[i][1] = 1.0;  
        ma[i][2] = 0.0;
        ma[i][3] = 0.0;
        ma[i][4] = 0.0;
        ma[i][5] = -1.0;  /* handedness of screen coordinates, */
        ma[i][6] = 0.0;        /* y mapped down from upper left, we map y up*/
        ma[i][7] = 0.0;
        ma[i][8] = 0.0;
        ma[i][9] = 1.0;
     }
 }
/*___initrot()______________________________________________________________*/

/****resetrot()*****reset rotation matrix, etc ******************************/
void    resetrot(int i)
{
    int j;  /* resetting is tricky since matrix, centering, zcliping can all*/
            /*  be set separately.  So undefined ones for a particular i  */
            /*  are reset to the 1st resetting */
    
    if(i <= MAXRESETS)/*980630*/
    {/*legal view number*/
      if(LNDaxesPickedCtr)  /*070315*/
      {
         if(Laxischoice[i]) /* axis choice and center on picked point only*/
         {
            NX = axischoice[i][0]; 
            NY = axischoice[i][1];
            NZ = axischoice[i][2];
            if(LNdimnames && (dimensionptr != NULL) ) /*060622*/
            {/*then dimensionptr should have been allocated*/
               strcpy(NXstr,dimensionptr[NX].name);
               strcpy(NYstr,dimensionptr[NY].name);
               strcpy(NZstr,dimensionptr[NZ].name);
            }
            sprintf(onscreeninfostr,"X: %s,  Y: %s,  Z: %s",NXstr,NYstr,NZstr);
            Lonscreeninfo = 1;
            Lonscreeninfoon = 1;
            reloadkinemage(); /*before center on reloaded pickedpoint*/
            /* point last centered on i.e. right pick */
            if(ctrpickedpointptr != NULL)
            {
               fxcenternew = ctrpickedpointptr->fx;
               fycenternew = ctrpickedpointptr->fy;
               fzcenternew = ctrpickedpointptr->fz;
            }
            Lreloadkinemage = 1; /*needed to get markers reset on point*/
         }
      }
      else
      {/*full change of view*/
        iviewset = i;
    /*---------------------------------*/
        if(Lreset[i]) j = i;
        else          j = 1;

        a11 = ma[j][1];
        a12 = ma[j][2];
        a13 = ma[j][3];
        a21 = ma[j][4];
        a22 = ma[j][5];
        a23 = ma[j][6];
        a31 = ma[j][7];
        a32 = ma[j][8];
        a33 = ma[j][9];
    /*---------------------------------*/
        if(Lzoomer[i]!=0) j = i;
        else           j = 1;
        if(Lzoomer[j] < 0) 
        {/* span value, span of screen: calculate appropriate zoom*/
           zoom = oldmaxwide/(Scale * zoomold[j]) ;
        }
        else
        {
           zoom = zoomold[j]; /*update current stored zoom value*/
        }
        scalenew = Scale*zoom;
        /*Lzoom = Lzoomer[j];*/
        
/*Zx*/  if(LGUI) resetgrafzoomBar(); 
      /*izoomold[0] = izoomold[j];*/ /*update current stored zoom value */
        /*zoom itself holds this value  */
    /*---------------------------------*/
        if(Lcenter[i]) j = i;
        else           j = 1;
        fxcenternew = fxcenterold[j];
        fycenternew = fycenterold[j];
        fzcenternew = fzcenterold[j];
        fxcenterold[0] = fxcenterold[j]; /*update current stored center value*/
        fycenterold[0] = fycenterold[j]; 
        fzcenterold[0] = fzcenterold[j]; 
    /*----------------------------------*/
        if(Lzcliper[i]) j = i;
        else            j = 1;
        Lzclip = Lzcliper[j];
        izclip = (int)(izclipold[j]*fzclip); 
        if(LGUI) resetgrafzclipBar(izclipold[j]);  /*___BAR.C*/
        izclipold[0] = izclipold[j]; /*update current stored zclip value */
    /*----------------------------------*/
      if(!Lzlimitscope)
      {/*original use of ztran slider bar*/
        if(Lztraner[i]) j = i;
        else            j = 1;
        Lztran = Lztraner[j];
        iztran = (int)(iztranold[j]*fzclip); 
        if(LGUI) resetgrafztranBar(iztranold[j]);  /*___BAR.C*/ 
        iztranold[0] = iztranold[j]; /*update current stored ztran value*/
      }
    /*----------------------------------*/
      if(Laxischoice[i]) /*060613 only set axes if this view has them*/
      {
         NX = axischoice[i][0]; 
         NY = axischoice[i][1];
         NZ = axischoice[i][2];
         if(LNdimnames && (dimensionptr != NULL) ) /*060622*/
         {/*then dimensionptr should have been allocated*/
            strcpy(NXstr,dimensionptr[NX].name);
            strcpy(NYstr,dimensionptr[NY].name);
            strcpy(NZstr,dimensionptr[NZ].name);
         }
         sprintf(onscreeninfostr,"X: %s,  Y: %s,  Z: %s",NXstr,NYstr,NZstr);
         Lonscreeninfo = 1;
         Lonscreeninfoon = 1;
         Lreloadkinemage = 1;
      }
    /*----------------------------------*/

    /*flatland only changes ixtran, iytran reset to zero anyway*/
        ixtran = 0;
        iytran = 0;
    /*----------------------------------*/
#ifdef UNIX_X11
      if(Lgwidthheightview[i]) 
      {/*060613 at end, only set screen size if this view so specifies*/
         if(gwidthview[i] > 0 && gheightview[i] > 0)
         {
            resizegrafwindow(gwidthview[i],gheightview[i]);
         }
      }
#endif
    /*----------------------------------*/


      }/*full change of view*/
      if(LGUI) checkcurrentviewmenu(i);  /*___MENU.C*/

      rescalekinemage(); /*MAGEINPT.c*/

    }/*legal view number*/
}
/*___resetrot()_____________________________________________________________*/

/****resetzoom()**************************************************************/
void resetzoom(int imove)  /*030405*/
{
   zoommax = 10;
   zoom = zoom + (float)0.01*(float)imove*(float)oldmaxwide/400;
   if(zoom < 0) {zoom = 0;}
   if(zoom > zoommax) {zoom = zoommax;}
   scalenew = Scale*zoom;
   resetgrafzoomBar(); 
   rescalekinemage();
}
/*___resetzoom()_____________________________________________________________*/

/****resetzclip()*************************************************************/
void resetzclip(int imove) /*030405*/
{
   izclipmax = 800;
   izclip = izclip + (int)((float)imove*(float)oldmaxwide/400);
   if(izclip < 0) {izclip = 0;}
   if(izclip > izclipmax) {izclip = izclipmax;}
   izclipold[0] = izclip; /*remember value for current view   050407*/
   resetgrafzclipBar(izclip);  /*___BAR.C*/
}
/*___resetzclip()____________________________________________________________*/

/****setrotview()*****reset rotation matrix, etc ****************************/
void    setrotview(int i)
{    
    iviewset = i; /*current last-set view */
    Lreset[i] = 1;  /* there is ith matrix */
    Lview[i] = 1;   /* there is ith view */
    
    ma[i][1] = a11;
    ma[i][2] = a12;
    ma[i][3] = a13;
    ma[i][4] = a21;
    ma[i][5] = a22;
    ma[i][6] = a23;
    ma[i][7] = a31;
    ma[i][8] = a32;
    ma[i][9] = a33;

    /*Lzoomer[i] set by calling routine  981013 */
    if(Lzoomer[i] >0)
        zoomold[i] = zoom;
    else if(Lzoomer[i] < 0)
        /*zoom = oldmaxwide/(Scale * zoomold[j]) ;*/
        zoomold[i] = oldmaxwide/(Scale * zoom);
    else  zoomold[i] = 1.0;/*arbitrary default, Lzoomer should be +-1*/
        
    /*scaleold[i] = scalenew;*/

    Lcenter[i] = 1;

    fxcenterold[i] = fxcenternew;
    fycenterold[i] = fycenternew;
    fzcenterold[i] = fzcenternew;

    if(Lzclip)  Lzcliper[i] = 1;
    else        Lzcliper[i] = 0;
    Lzcliper[i] = 1; /*artifact: Lzclip always==1*/
    izclipold[i] = izclipold[0]; /*stored current unscaled value*/ 
    

  if(!Lzlimitscope)
  {/*original use of ztran slider bar*/
    if(Lztran) Lztraner[i] = 1;
    else       Lztraner[i] = 0;
    Lztraner[i] = 1; /*artifact: Lztran always==1*/
    iztranold[i] = iztranold[0]; /*stored current unscaled value*/ 
  }  
}
/*___setrotview()____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****dodocktran()*************************************************************/
void    dodocktran(int i,float trans)
{
   float factorx,factory,factorz;     
   pointstruct* thepointptr;
   pointstruct* thelastpointptr;
   liststruct*  thelistptr;
   grupstruct*  thegrupptr;

 if(beginselectionpointptr!=NULL && endselectionpointptr!=NULL)
 {/*protection against egamickdocking annealing  egam 030215*/  
   /*beware: trans == bondrot theta, so rolls over at +-180*/ /*991202*/
   if(trans < -358) trans = +1;
   else if(trans > +358) trans = -1;
   /*translations damped by 0.1 for all methods, */
   /*see MAGEUTIL/getrot() for additional /2 mouse motion damping*/
   /*BUT 011201 now can set global finestep, so arbitrary damping not needed*/
   if(i == 1)
   {/*screen oriented x motion*/
      factorx = (float)(finestep*trans*a11);
      factory = (float)(finestep*trans*a21);
      factorz = (float)(finestep*trans*a31);
   }/*screen oriented x motion*/
   if(i == 2)
   {/*screen oriented y motion*/
     factorx = (float)(finestep*trans*a12);
     factory = (float)(finestep*trans*a22);
     factorz = (float)(finestep*trans*a32);
   }/*screen oriented y motion*/
   if(i == 3)
   {/*screen oriented z motion*/
     factorx = (float)(finestep*trans*a13);
     factory = (float)(finestep*trans*a23);
     factorz = (float)(finestep*trans*a33);
   }/*screen oriented z motion*/
    
   /*scope set by @beginselect and @endselect */
    
   thepointptr = beginselectionpointptr;
   thelastpointptr  = endselectionpointptr;
    
   /*first, update the gnomon points*/  
   thegrupptr = ((thepointptr->listptr)->sgrpptr)->grupptr;
   /*while(thegrupptr->bondrot == DOCKSET)*/
   while((thegrupptr->bondrot|DOCKSET) == DOCKSET) /*040320,121105 inner()*/
   {
      thegrupptr->pointx[0] = thegrupptr->pointx[0] + factorx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] + factory;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] + factorz;

      thegrupptr->pointy[0] = thegrupptr->pointy[0] + factorx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] + factory;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] + factorz;

      thegrupptr->pointz[0] = thegrupptr->pointz[0] + factorx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] + factory;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] + factorz;

      thegrupptr->position[0] = thegrupptr->position[0] + factorx;
      thegrupptr->position[1] = thegrupptr->position[1] + factory;
      thegrupptr->position[2] = thegrupptr->position[2] + factorz;
      thegrupptr = thegrupptr->nextptr;
      if(thegrupptr == NULL) break;
   } 
   /*matrix elements are from the global current rotation matrix*/
   while(thepointptr != NULL)
   {/*loop over all points in scope*/
                thepointptr->fx = thepointptr->fx + factorx;
                thepointptr->fy = thepointptr->fy + factory;
                thepointptr->fz = thepointptr->fz + factorz;                        

      if(thepointptr == thelastpointptr)
      {/*end of scope*/
         thepointptr = NULL;
      }
      else if(thepointptr == (thepointptr->listptr)->lastpointptr)
      {/*at last point in a list*/
         /*970903 MAGE keeps track of all lists as a linked list during */
         /* allocliststructure() so only need to go up to the list level*/

         if((thepointptr->listptr)->nextptr != NULL)
         {/*there is a next list: get first point of next list*/
            thelistptr = (thepointptr->listptr)->nextptr;
            thepointptr = thelistptr->firstpointptr;
            if(thepointptr == NULL) /*list with NO points*/
            {
               while(   (thepointptr == NULL) /*current point*/
                     && (thelistptr->nextptr != NULL) ) /*next list*/
               {/*search for a non-NULL point in the scope*/
                 thelistptr = thelistptr->nextptr; /*next list*/
                 thepointptr = thelistptr->firstpointptr;
               }
            }
         }
         else
         {/*NO  more lists, no more points to rotate*/
            /*somehow thelastpointptr failed to be found*/
            thepointptr = NULL;
         }
      }/*at last point in a list*/
      else
      {
         thepointptr = thepointptr->nextptr;
      }
   }/*loop over all points in scope*/
 }/*protection against egamickdocking annealing  egam 030215*/  
}
/*___dodocktran()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****dodockrot()*************************************************************/
void    dodockrot(int i,float theta)
{
 
    pointstruct* thepointptr;
    pointstruct* thelastpointptr;
    liststruct*  thelistptr;
    grupstruct*  thegrupptr;
    
    float fx1,fy1,fz1,fx2,fy2,fz2,centerx,centery,centerz;

    double detmatrix;

 if(beginselectionpointptr!=NULL && endselectionpointptr!=NULL)
 {/*protection against egamickdocking annealing  egam 030215*/  

/*Docking rotations are screen oriented, original coordinates are actually */
/*in screen orientation.  Back rotate the chosen screen axes by the */
/*inverse of the global matrix then apply a general axis rotation to the */
/*original coords, offseting for the effective origin of the rotated set*/

    /* sin and cos of current delta angle of this axis */

      fx1 = 0.0;
      fy1 = 0.0;
      fz1 = 0.0;
      fx2 = 0.0;
      fy2 = 0.0;
      fz2 = 0.0;

    if     (i==1) fx1 = 1.0; /*screen x axis*/
    else if(i==2) fy1 = 1.0; /*screen y axis*/
    else if(i==3) fz1 = 1.0; /*screen z axis*/

    {/*calculate inverse rotation matrix, see MAGEDRAW/Lnewmatrix discussion*/
                detmatrix = a11*a22*a33+a12*a23*a31+a13*a21*a32
                           -a13*a22*a31-a11*a23*a32-a12*a21*a33;
                ma[0][1] = ((+a22*a33-a23*a32)/detmatrix);
                ma[0][4] = ((-a21*a33+a23*a31)/detmatrix);
                ma[0][7] = ((+a21*a32-a22*a31)/detmatrix);
                ma[0][2] = ((-a12*a33+a13*a32)/detmatrix);
                ma[0][5] = ((+a11*a33-a13*a31)/detmatrix);
                ma[0][8] = ((-a11*a32+a12*a31)/detmatrix);
                ma[0][3] = ((+a12*a23-a13*a22)/detmatrix);
                ma[0][6] = ((-a11*a23+a13*a21)/detmatrix);
                ma[0][9] = ((+a11*a22-a12*a21)/detmatrix);
             /*11=[1];12=[2];13=[3];21=[4];22=[5];23=[6];31=[7];32=[8];33=[9];*/
             /*use otherwise unused 0th matrix entries */
    }/*calculate inverse rotation matrix*/

/*back rotate the selected axis*/
              fx2 = (float)(fx1*ma[0][1] + fy1*ma[0][4] + fz1*ma[0][7]);
              fy2 = (float)(fx1*ma[0][2] + fy1*ma[0][5] + fz1*ma[0][8]);
              fz2 = (float)(fx1*ma[0][3] + fy1*ma[0][6] + fz1*ma[0][9]);



    /*scope set by @beginselect and @endselect */
    
    thepointptr = beginselectionpointptr;
    thelastpointptr  = endselectionpointptr;

    /*centerx = beginselectionpointptr->fx;*/
    /*centery = beginselectionpointptr->fy;*/
    /*centerz = beginselectionpointptr->fz;*/
    /*use current center of the screen as center of the rotation*/ /*991202*/
    centerx = fxcenternew;
    centery = fycenternew;
    centerz = fzcenternew;

   /*first, update the gnomon points*/  
   thegrupptr = ((thepointptr->listptr)->sgrpptr)->grupptr;
   /*while(thegrupptr->bondrot == DOCKSET)*/
   while((thegrupptr->bondrot|DOCKSET) == DOCKSET) /*040320, 121105 inner ()*/
   {
      thegrupptr->pointx[0] = thegrupptr->pointx[0] - centerx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] - centery;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] - centerz;
      doaxisrot(&(thegrupptr->pointx[0]),&(thegrupptr->pointx[1])
               ,&(thegrupptr->pointx[2])
                     ,theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointx[0] = thegrupptr->pointx[0] + centerx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] + centery;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] + centerz;
      
      thegrupptr->pointy[0] = thegrupptr->pointy[0] - centerx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] - centery;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] - centerz;
      doaxisrot(&(thegrupptr->pointy[0]),&(thegrupptr->pointy[1])
               ,&(thegrupptr->pointy[2])
                     ,theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointy[0] = thegrupptr->pointy[0] + centerx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] + centery;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] + centerz;
      
      thegrupptr->pointz[0] = thegrupptr->pointz[0] - centerx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] - centery;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] - centerz;
      doaxisrot(&(thegrupptr->pointz[0]),&(thegrupptr->pointz[1])
               ,&(thegrupptr->pointz[2])
                     ,theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointz[0] = thegrupptr->pointz[0] + centerx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] + centery;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] + centerz;
      
      thegrupptr->position[0] = thegrupptr->position[0] - centerx;
      thegrupptr->position[1] = thegrupptr->position[1] - centery;
      thegrupptr->position[2] = thegrupptr->position[2] - centerz;
      doaxisrot(&(thegrupptr->position[0]),&(thegrupptr->position[1])
               ,&(thegrupptr->position[2])
                     ,theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->position[0] = thegrupptr->position[0] + centerx;
      thegrupptr->position[1] = thegrupptr->position[1] + centery;
      thegrupptr->position[2] = thegrupptr->position[2] + centerz;

      thegrupptr = thegrupptr->nextptr;
      if(thegrupptr == NULL) break;
   } 
    
    while(thepointptr != NULL)
    {/*loop over all points in scope*/
      thepointptr->fx = thepointptr->fx - centerx;
      thepointptr->fy = thepointptr->fy - centery;
      thepointptr->fz = thepointptr->fz - centerz;


      doaxisrot(&(thepointptr->fx),&(thepointptr->fy),&(thepointptr->fz)
                     ,theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/

      thepointptr->fx = thepointptr->fx + centerx;
      thepointptr->fy = thepointptr->fy + centery;
      thepointptr->fz = thepointptr->fz + centerz;

      if(thepointptr == thelastpointptr)
      {/*end of scope*/
         thepointptr = NULL;
      }
      else if(thepointptr == (thepointptr->listptr)->lastpointptr)
      {/*at last point in a list*/
         /*970903 MAGE keeps track of all lists as a linked list during */
         /* allocliststructure() so only need to go up to the list level*/

         if((thepointptr->listptr)->nextptr != NULL)
         {/*there is a next list: get first point of next list*/
            thelistptr = (thepointptr->listptr)->nextptr;
            thepointptr = thelistptr->firstpointptr;
            if(thepointptr == NULL) /*list with NO points*/
            {
               while(   (thepointptr == NULL) /*current point*/
                     && (thelistptr->nextptr != NULL) ) /*next list*/
               {/*search for a non-NULL point in the scope*/
                 thelistptr = thelistptr->nextptr; /*next list*/
                 thepointptr = thelistptr->firstpointptr;
               }
            }
         }
         else
         {/*NO  more lists, no more points to rotate*/
            /*somehow thelastpointptr failed to be found*/
            thepointptr = NULL;
         }
      }/*at last point in a list*/
      else
      {
         thepointptr = thepointptr->nextptr;
      }

    }/*loop over all points in scope*/
 }/*protection against egamickdocking annealing  egam 030215*/  
}
/*___dodockrot()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****dogangrotscan()**********************************************************/
void    dogangrotscan(int ifirst,float theta)
{
  int i;
  
  /*slider control points to first of a set of bondrotations distinguished*/
  /*by identical names*/
  /*scan all bondrots, picking out those with this option flagged*/
  /*and that have a name identical to this one*/
  for(i=ifirst; i<=nbondrot; i++)
  {
     if(bondrotptr[i]->level == -2 || bondrotptr[i]->level == -1)
     {/*one of a ganged set of rotations*/
        if(strncmp(bondrotptr[i]->name,bondrotptr[ifirst]->name,MAXNAMECHAR)==0)
        {/*one of THIS set of rotations*/
           dobondrot(i, theta);
        }
     }
  }
}
/*___dogangrotscan()_________________________________________________________*/

/****bondrotoptioner()*******************************************************/
int bondrotoptioner(int i) /*981010*/
{
   float thetheta,theangle;
   int ireturn; /*set ireturn == 1 for dobondrot() also*/

   theangle = bondrotptr[i]->angle; /*970917*/
   
   if(bondrotptr[i]->option == 2 )
   {
      restorebondrots(i-1,i-1); /*MAGEANGL*/
      restorebondrots(i-2,i-2);
      /*bring 2 preceeding angles back to initial settings*/
      /*reverse order helps samescope rotations return to actual start*/
      /*set previous two angles as precession angle components*/

      thetheta =
       (float)(bondrotptr[i+1]->angle*sin( (double)theangle*3.14159*2.0/360.0));/*970917*/
      bondrotptr[i-2]->angle = thetheta;
      dobondrot(i-2,thetheta);    /*MAGEANGL*/
      thetheta =
       (float)(bondrotptr[i+1]->angle*cos( (double)theangle*3.14159*2.0/360.0));/*970917*/
      bondrotptr[i-1]->angle = thetheta;
      dobondrot(i-1,thetheta);
      writebondrotvalue(i-2); /*____BAR.C*/
      writebondrotvalue(i-1); /*____BAR.C*/
      ireturn = 0; /*dobondrot() already done*/
   }
   else if(bondrotptr[i]->option == 1)
   {/*do nothing more than just change value of angle, e.g. mu */
      ireturn = 0;
   }
   else if(bondrotptr[i]->option == 4) /*dodockrot(1,(float)theta)*/
   {/*x docking rotation*/
      ireturn = 4;
   }
   else if(bondrotptr[i]->option == 5) /*dodockrot(2,(float)theta)*/
   {/*y docking rotation*/
      ireturn = 5;
   }
   else if(bondrotptr[i]->option == 6) /*dodockrot(3,(float)theta)*/
   {/*z docking rotation*/
      ireturn = 6;
   }
   else if(bondrotptr[i]->option == 7) /*dodocktran(1,(float)theta)*/
   {/*x docking translation*/
      ireturn = 7;
   }
   else if(bondrotptr[i]->option == 8) /*dodocktran(2,(float)theta)*/
   {/*y docking translation*/
      ireturn = 8;
   }
   else if(bondrotptr[i]->option == 9) /*dodocktran(3,(float)theta)*/
   {/*z docking translation*/
      ireturn = 9;
   }
   else if(bondrotptr[i]->option == 10)
   {/*ganged rotations*/
      ireturn = 10;
   }

   else ireturn = 1; /*presume need yet to do dobondrot()*/
   return(ireturn);
}
/*____bondrotoptioner()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****bondrotbyoption()********************************************************/
void bondrotbyoption(int iflag, float theta) /*001005*/
{
   int i=0,it = 0;
   float angle = 0;
   
   for(i=1; i<=nbondrot; i++)
   {
      if(bondrotptr[i]->option == iflag)
      {
         it = i;
         break;
      }
   }
   if(it > 0 && iflag >= 4 && iflag <=9)
   {/*found it*/
      angle = bondrotptr[it]->angle; /*present displayed slider value*/
      angle = angle + theta; /*add in change to value*/
      /*adjust to be within -180 -- +180 range*/
      while(angle > 180 || angle < -180)
      {
         if(angle > 180) angle = angle - 360;
         else if(angle < -180) angle = angle + 360;
      }
      bondrotptr[it]->angle = angle; /*new value*/
      resetgrafbondrotBar(it);

      if     (iflag==4) dodockrot(1,(float)theta);
      else if(iflag==5) dodockrot(2,(float)theta);
      else if(iflag==6) dodockrot(3,(float)theta);
      else if(iflag==7) dodocktran(1,(float)theta);
      else if(iflag==8) dodocktran(2,(float)theta);
      else if(iflag==9) dodocktran(3,(float)theta);
   }
}
/*___bondrotbyoption()_______________________________________________________*/

/****dobondrotrouter()********************************************************/
void dobondrotrouter(int i, float theta)                  
{/*some bondrot options substitute, or do not require dobondrot()*/
   int iflag;
                  
   if(bondrotptr[i]->option > 0 ) 
       iflag = bondrotoptioner(i); /*MAGEANGL.c*/ /*981010*/
   else iflag = 1;
   /*do bond rotation matrix update and apply */
/*printf("dobondrotrouter: dodockrot(%d, %.3f)\n",iflag,theta);*/
   if(iflag==1) dobondrot(i,theta);  /*MAGEANGL.C*/
      /* rotation done on float array entries ! */                   
   else if(iflag==4) dodockrot(1,(float)theta);
   else if(iflag==5) dodockrot(2,(float)theta);
   else if(iflag==6) dodockrot(3,(float)theta);
   else if(iflag==7) dodocktran(1,(float)theta);
   else if(iflag==8) dodocktran(2,(float)theta);
   else if(iflag==9) dodocktran(3,(float)theta);
   else if(iflag==10) dogangrotscan(i,(float)theta);
   resetgraphedrotamer(i); /*MAGEANGL.c, checks for this graphing*/
   /*need this since can get here without invoking resetgrafbondrotBar()*/
}
/*___dobondrotrouter_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****grafbondrotarrow()*******************************************************/
void grafbondrotarrow(float theta)
{
   float anglemax=0,anglemin=0;

   if(L0to360) {anglemax = 360; anglemin = 0;} /*030819*/
   else {anglemax = 180; anglemin = -180;}
   bondrotptr[icurrentrotation]->angle 
      = bondrotptr[icurrentrotation]->angle + theta;
   if(bondrotptr[icurrentrotation]->angle >  anglemax)   /* was  180.0*/
      bondrotptr[icurrentrotation]->angle =  anglemin+1; /* was -179.0*/
   if(bondrotptr[icurrentrotation]->angle <  anglemin)   /* was -180.0*/
      bondrotptr[icurrentrotation]->angle =  anglemax-1; /* was  179.0*/
   resetgrafbondrotBar(icurrentrotation); /*____BAR.c*/
   dobondrotrouter(icurrentrotation,theta);/*MAGEANGL.c*/
   if(Lupdate && !Linhibiton && Lprobeactive)
   {/*update from remote program: e.g. dots by PROBE*/
       Nthisgroupkind = PROBEDOTSKKIND; /*bondrot change needs probe run*/
       Ldeleteprobedots = 1;
       doupdate(1,update_probe_command_line); 
       /* 1: sends coords to probe*/
       Nthisgroupkind = 0;
   }
   rescalekinemage(); /*MAGEINPT.C*/
                      /* rescale rebuilds from float arrary*/
   redrawvec(); /*MAGEDRAW.c*/
} 
/*___grafbondrotarrow()_____________________________________________________*/

/****dobondrotgrapher()******************************************************/
void dobondrotgrapher() /*angle parsing separated for remote socket feed*/
{
   int   iang[7]={0,0,0,0,0,0,0},Nangles=0;
   float fang[7]={0,0,0,0,0,0,0};

   if(pickedpointptr != NULL)
   {/*picked point exists*/
      if(  LbondrotZplot   /*030819 which accepts any picked point...*/
         ||(  (LbondrotHplot > 0 && LbondrotVplot > 0)
         &&((pickedpointptr->STATUS & SCREENFLAG) == SCREENFLAG) ) )
      {/*screen oriented and sized plot is present and was site of last pick*/
         if(LbondrotZplot > 0) {Nangles = 3;} /*030819*/
         else {Nangles = 2;}
         /*map pick x to angle plotted horizontally and y to vertical one*/
         /*and maybe z to zplot direction*/
         iang[1] = LbondrotHplot;
         fang[1] = pickedpointptr->fx;
         iang[2] = LbondrotVplot;
         fang[2] = pickedpointptr->fy;
         iang[3] = LbondrotZplot;
         fang[3] = pickedpointptr->fz;
         bondrotgrapher(Nangles,iang,fang);
      }/*screen oriented and sized plot is present and was site of last pick*/
   }/*picked point exists*/
}
/*___dobondrotgrapher()_____________________________________________________*/

/****dobondrotgrapherNDIMsuitefit()******************************************/
void dobondrotgrapherNDIMsuitefit() /*121208,121209,130115,130116*/
{
   int   iang[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},Nangles=0;
   float fang[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int i=0;
   int k=0;
   int n=0; /*121213 iterator for n<NDIMangmax angle names to screen*/
   float theta  = 0.0;
   float target = 0.0;
   int Lmayneeddoupdate=0; /*121209 call probe, re:MAGEFILE/Dotexthyperbutton*/
   int nth = 0;
   float nthtarget[4] = {0.0,0.0,0.0,0.0};
   char  nthname[4][32];
   int   nthangle = 0;
   int   inthname = 0;
   char NDIMangstr [17][32];  /*121211*/
   int  NDIMangn = 0; /*count number of angles (parameters) 121211*/
   int  NDIMangmax = 17; /*hardcode later to value expected 121211 */
   int  Llocaltest = 0; /*130309 just test this pesky angle reader...*/

   Lsuitefittedstr = 0; /*MAGEANGL.h 121211*/
   suitefittedstr[0] = '\0'; /*MAGEANGL.h 121211*/

   if(pickedpointptr != NULL)
   {/*picked point exists*/
    /*if((pickedpointptr->STATUS & SCREENFLAG) == SCREENFLAG) 130115 */
    /*130115 not restrict to screen oriented and sized plot*/
      if(pickedpointptr->listptr->NDIM) /*130115*/
      {/*NDIM parameter point was site of last pick  130115*/
         Nangles = NDIM; 
if(Ltest)
{sprintf(alertstr,"MAGEANGL/dobondrotgrapherNDIMsuitefit()/MAXNDIM %d\n",MAXNDIM);dosinglealert(3);}
/*MAGEDLOG.c/dosinglealert() requires Ltest||Lformattest */
if(Ltest)
{sprintf(alertstr,"pickedpointptr->listptr->NDIM== %d\n",pickedpointptr->listptr->NDIM);dosinglealert(3);}
      
 
        for(k=0; k<pickedpointptr->listptr->NDIM; k++)
        {/*loop over k dimensions*/
if(Ltest){sprintf(alertstr,"pickedpointptr->coordNptr[%d]== %.3f, dimensionptr[%d].name== %s\n",k,pickedpointptr->coordNptr[k],k,dimensionptr[k].name);dosinglealert(3);}
if(Llocaltest)
{fprintf(stderr,"\npickedpointptr->coordNptr[%d]== %.3f, dimensionptr[%d].name== %s\n",k,pickedpointptr->coordNptr[k],k,dimensionptr[k].name);}

           /*delta angle alone fails miserably to set actual pucker type*/
           /*121209 try to use generic 3' and 2' ribose puckers + other angles*/
           /*use angle naming and arrangement of suitefit42label.kin*/
           /*NB: this makes Mage absolutely dependent on suitefit42label.kin*/
           /*AND dependent on exact set of names for 7Dkin dimensions !!!! */
           /* delta-1, epsilon-1, zeta-1, alpha, beta, gamma, delta+ */ 
           /* including forcing a loop for delta to set its correlated angles*/
           /* SOOOOOO: names and number of parameters == angles is hard coded*/
           NDIMangmax = 7; /*121211 HARD CODED */
/*
ribose conformers: from suitefit42label.kin @text hypertext control
strip suitefit42label.kin ??

*{5' ribose rot={5' c4*--c3* delta} 84.5 rot={5' c4*--o4*} 19.6 rot={5' o--c3*--c2*--o} 39.4 rot={5' o4*--c1*} 4.4 }*
*{5' ribose rot={5' c4*--c3* delta} 147.0 rot={5' c4*--o4*} -2.25 rot={5' o--c3*--c2*--o} -38.4 rot={5' o4*--c1*} -23.7 }*
*{3' ribose rot={3' c4*--c3* delta} 84.3 rot={3' c4*-c3*-c2*-c1*} 36.9 rot={3' c3*-c2*-c1*-o4*} -26.2 }*
*{3' ribose rot={3' c4*--c3* delta} 147.0 rot={3' c4*-c3*-c2*-c1*} -40.5 rot={3' c3*-c2*-c1*-o4*} 39.5 }*
*/

           sprintf(word,"%s",dimensionptr[k].name);
           iword = strlen(word);
           target = pickedpointptr->coordNptr[k];
           if(  (iword == 7)  /*avoid plain delta or delta+ 130309*/
              &&(strncmp(word,"delta-1",iword) == 0))
           {/*setup loop for delta and related angles*/
              /*Swati's restrictive ranges 60-105 C3', 125-165 C2'  121210*/
              if(target < 115) /*delta < 115  3'pucker*/
              {
                  strcpy(nthname[0],"5' c4*--c3* delta");
                  nthtarget[0] = 84.5;
                  strcpy(nthname[1],"5' c4*--o4*");
                  nthtarget[1] = 19.6;
                  strcpy(nthname[2],"5' o--c3*--c2*--o");
                  nthtarget[2] = 39.4;
                  strcpy(nthname[3],"5' o4*--c1*");
                  nthtarget[3] = 4.4;
              }
              else /*delta >= 115  2'pucker*/
              {
                  strcpy(nthname[0],"5' c4*--c3* delta");
                  nthtarget[0] = 147.0;
                  strcpy(nthname[1],"5' c4*--o4*");
                  nthtarget[1] = -2.25;
                  strcpy(nthname[2],"5' o--c3*--c2*--o");
                  nthtarget[2] = -38.4;
                  strcpy(nthname[3],"5' o4*--c1*");
                  nthtarget[3] = -23.7;
              }
              nth = 4;
              sprintf(NDIMangstr[0],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[0], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(strncmp(word,"epsilon-1",iword) == 0)
           {
                  strcpy(nthname[0],"c3*--o3* epsilon");
                  nthtarget[0] = target;
                  nth = 1;
              sprintf(NDIMangstr[1],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[1], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(strncmp(word,"zeta-1",iword) == 0)
           {
                  strcpy(nthname[0],"o3*--p   zeta");
                  nthtarget[0] = target;
                  nth = 1;
              sprintf(NDIMangstr[2],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[2], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(strncmp(word,"alpha",iword) == 0)
           {
                  strcpy(nthname[0],"p  --o5* alpha");
                  nthtarget[0] = target;
                  nth = 1;
              sprintf(NDIMangstr[3],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[3], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(strncmp(word,"beta",iword) == 0)
           {
                  strcpy(nthname[0],"o5*--c5* beta");
                  nthtarget[0] = target;
                  nth = 1;
              sprintf(NDIMangstr[4],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[4], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(strncmp(word,"gamma",iword) == 0)
           {
                  strcpy(nthname[0],"c5*--c4* gamma");
                  nthtarget[0] = target;
                  nth = 1;
              sprintf(NDIMangstr[5],"%s  %.3f , ",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[5], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }
           else if(  (iword == 5 || iword == 6) /*delta or delta+ 130309*/
                   &&(strncmp(word,"delta",iword) == 0))
           {/*setup loop for delta and related angles*/
              if(target < 110) /*delta < 110  3'pucker*/
              {
                  strcpy(nthname[0],"3' c4*--c3* delta");
                  nthtarget[0] = 84.3;
                  strcpy(nthname[1],"3' c4*-c3*-c2*-c1*");
                  nthtarget[1] = 36.9;
                  strcpy(nthname[2],"3' c3*-c2*-c1*-o4*");
                  nthtarget[2] = -26.2;
              }
              else /*delta >= 110  2'pucker*/
              {
                  strcpy(nthname[0],"3' c4*--c3* delta");
                  nthtarget[0] = 147.0;
                  strcpy(nthname[1],"3' c4*-c3*-c2*-c1*");
                  nthtarget[1] = -40.5;
                  strcpy(nthname[2],"3' c3*-c2*-c1*-o4*");
                  nthtarget[2] = 39.5;
              }
              nth = 3;
              sprintf(NDIMangstr[6],"%s  %.3f",word,target); 
if(Llocaltest)
fprintf(stderr,"NDIMangstr[6], %s (%d char) %.3f , \n",word,iword,target);
              NDIMangn++;
           }

           for(nthangle = 0; nthangle < nth; nthangle++)
           {/*loop over entangled angles, 4 or 3 for delta, only 1 for others*/
            /*from MAGEFILE.c/Dotexthyperbutton/case bondrotID:*/
            /* INEFFICIENT updating, identifies updateable angles by name*/
            inthname = strlen(nthname[nthangle]);
            if(nbondrot > 0)
            {/*there are bondrotations */
             for(i=1; i<=nbondrot; i++)
             {/*loop over all existing rotations try to match name*/
                if(strncmp(nthname[nthangle],bondrotptr[i]->name,inthname) == 0)
                {/*name match*/
if(Llocaltest)
fprintf(stderr,"name match: nthname[nthangle %d]: %s\n",nthangle,nthname[nthangle]);
                   icurrentrotation = i;
                   theta = nthtarget[nthangle] - bondrotptr[i]->angle;
                   if(theta > 180) {theta = theta - 360;}
                   else if(theta < -180) {theta = 360 + theta;}

                   bondrotptr[i]->angle = nthtarget[nthangle];
                   resetgrafbondrotBar(i);/*____BAR.c*/
      
                   dobondrot(i, theta);                            
               
                   Lmayneeddoupdate = 1;
if(Llocaltest)
fprintf(stderr,"name match: nthname[nthangle %d]: %s\n",nthangle,nthname[nthangle]);
                   break; /*out of loop over all existing rotations*/
                }/*name match*/
             }/*loop over all existing rotations try to match name*/
            }/*there are bondrotations */
           }/*loop over entangled angles, 4 or 3 for delta, only 1 for others*/
        }/*loop over k dimensions*/
        if(NDIMangn == NDIMangmax)
        {/*expected number of parameter angles found and probably set*/
           for(n=0; n<NDIMangmax; n++) /*121213*/
           {
if(Llocaltest)
fprintf(stderr,"NDIMangstr[%d] == %s\n",n,NDIMangstr[n]);
              strcat(suitefittedstr,NDIMangstr[n]); /*...str MAGEANGL.h 121211*/
           }
           Lsuitefittedstr = 1; /*MAGEANGL.h*/
        }
        else
        {/*mis-match of expected vs actual number of parameter angles...*/
  
        }
        if(Lmayneeddoupdate) /*NOTE: no error checking !!!! */
        {
           Lmayneeddoupdate=0;
           if(Lupdate &&!Linhibiton && Lprobeactive)
           {/*update from remote program: e.g. dots by PROBE*/
              Nthisgroupkind = PROBEDOTSKKIND; /*needs probe run*/
              sprintf(word,"\""); /*ditto mark: use previously set command*/
              doupdate(1,word); /*____OUT.c or MUXMPIPE.c*/
              /* 1: sends coords to probe*/
              Nthisgroupkind = 0;
           }
           rescalekinemage(); /*MAGEINPT.c*/
             /* rescale rebuilds from float arrary*/
           redrawvec(); /*MAGEDRAW.c*/
           makecursoractive();  /*___MAIN.C*/
        } 
      }/*NDIM parameter point was site of last pick  130115*/
   }/*picked point exists*/
}
/*___dobondrotgrapherNDIMsuitefit()_________________________________________*/

/****bondrotgrapher()********************************************************/
void bondrotgrapher(int Nangles, int iang[7], float fang[7])
{/*dobondrotgrapher and bondrotgrapher split 031226*/
  /*allow 6 angles, some of which can be dummies, for remote echo model angles*/
   float theta=0,thetain=0,thetaout=0,angle=0;
   int   i=0, j=0;
   float anglemax=0,anglemin=0;

   for(j=1; j<=Nangles; j++)
   {/*do each angle in turn*/
      i = iang[j];
      angle = fang[j];
      if(i!=0) /* i==0 is a dummy angle, e.g. 3rd in a 2D case*/
      {
         if(angle<0.0) 
              thetain = 360 + angle;
         else thetain = angle;
         if(bondrotptr[i]->angle<0.0) 
              thetaout = 360 + bondrotptr[i]->angle;
         else thetaout = bondrotptr[i]->angle;
         theta = thetain - thetaout;
         if(L0to360) { anglemax=360; anglemin=0;}
         else { anglemax = 180; anglemin = -180;}
         if(theta >  anglemax) theta =  theta - 360;
         if(theta <  anglemin) theta =  theta + 360;
         
         if(bondrotptr[i]->option == 10 ) /*020310*/
         {
            dogangrotscan(i,(float)theta); /*020310*/
         }
         else
         {
            dobondrot(i,theta);
         }
         bondrotptr[i]->angle = angle;
         resetgrafbondrotBar(i); /*___BAR.C*/ /*001005*/
      }
   }/*do each angle in turn*/
   if(Lupdate && !Linhibiton && Lprobeactive)
   {/*update from remote program: e.g. dots by PROBE*/
      Nthisgroupkind = PROBEDOTSKKIND; /*bondrot change needs probe run*/
      Ldeleteprobedots = 1;
      doupdate(1,update_probe_command_line); 
      /* 1: sends coords to probe*/
      Nthisgroupkind = 0;
   }
   rescalekinemage(); /*MAGEINPT.c*/ /*rescale rebuilds from float array*/
   redrawvec(); /*MAGEDRAW.c*/
}
/*___bondrotgrapher()_______________________________________________________*/

/****resetgraphedrotamer()***************************************************/
void resetgraphedrotamer(int i)  /*001007*/
{/*update the plotted ROTAMER*/
  if(i == LbondrotHplot)
  {
    if(L0to360 && bondrotptr[LbondrotHplot]->angle < 0)
     {(drawnewlistptr->firstpointptr)->fx=bondrotptr[LbondrotHplot]->angle+360;}
    else
     {(drawnewlistptr->firstpointptr)->fx = bondrotptr[LbondrotHplot]->angle;}
  }
  if(i == LbondrotVplot)
  {
    if(L0to360 && bondrotptr[LbondrotVplot]->angle < 0)
     {(drawnewlistptr->firstpointptr)->fy=bondrotptr[LbondrotVplot]->angle+360;}
    else
     {(drawnewlistptr->firstpointptr)->fy = bondrotptr[LbondrotVplot]->angle;}
  }
  if(i == LbondrotZplot) /*030819*/
  {
    if(L0to360 && bondrotptr[LbondrotZplot]->angle < 0)
     {(drawnewlistptr->firstpointptr)->fz=bondrotptr[LbondrotZplot]->angle+360;}
    else
     {(drawnewlistptr->firstpointptr)->fz = bondrotptr[LbondrotZplot]->angle;}
  }
}
/*___resetgraphedrotamer()__________________________________________________*/

/****dobondrotplotter()******************************************************/
void dobondrotplotter()  /*971011*/
{
   float maxangle=180;

   if(L0to360) {maxangle = 360;} /*030819*/
   else {maxangle = 180;}
   allocpointstructure(drawnewlistptr);
   if((bondrotptr[LbondrotHplot]->angle) >= maxangle)
      thispointptr->fx = (bondrotptr[LbondrotHplot]->angle) -360 ;
   else
      thispointptr->fx = (bondrotptr[LbondrotHplot]->angle);
   if((bondrotptr[LbondrotVplot]->angle) >= maxangle)
      thispointptr->fy = (bondrotptr[LbondrotVplot]->angle) -360 ;
   else
      thispointptr->fy = (bondrotptr[LbondrotVplot]->angle);
   if(LbondrotZplot > 0)
   {
      if((bondrotptr[LbondrotZplot]->angle) >= maxangle)
         thispointptr->fz = (bondrotptr[LbondrotZplot]->angle) -360 ;
      else
         thispointptr->fz = (bondrotptr[LbondrotZplot]->angle);
   }
   else
   {
      thispointptr->fz = 0.0;
   }
   thispointptr->type = DOT;
   if(LbondrotZplot > 0)
   {
      sprintf(word,"%.2f,%.2f,%.2f"
            ,bondrotptr[LbondrotHplot]->angle
            ,bondrotptr[LbondrotVplot]->angle
            ,bondrotptr[LbondrotZplot]->angle);
   }
   else
   {
      thispointptr->STATUS = thispointptr->STATUS | SCREENFLAG | STATICFLAG;
      sprintf(word,"%.2f,%.2f"
            ,bondrotptr[LbondrotHplot]->angle
            ,bondrotptr[LbondrotVplot]->angle);
   }
   storeptIDstring(word, thispointptr); /*MAGELIST*/
   numberline++;
   maxnxyz++;
   rescalekinemage(); /*MAGEINPT.C*/
       /* rescale rebuilds from float arrary*/
   redrawvec(); /*MAGEDRAW.c*/
}
/*___dobondrotplotter()_____________________________________________________*/

/****doxrayplotter()******************************************************/
void doxrayplotter(pointstructptr ptptr, float ctr, int* Lnew, int* Lold)
{/*xray from XL at origin to film, Ewald ctr +z axis,*/
 /* crystal to film distance defaults to Ewald sphere radius*/
 /* if it is not specified as parameter[9] */
 /*reciprocal lattice point on Ewald sphere at x,y,z*/
   static struct grupstruct* spotgrupptr;
   static struct sgrpstruct* spotsgrpptr;
   static struct liststruct* spotlistptr;
   
   /*NEEDS better protections against missing/naming structures */
   
   /*first, need group in which to hold these projected points*/
   if(*Lold==0)
   {
      getnewgroup(); /*sets to itself the glabal: thisgrupptr */
         /* and becomes lastgrupptr so MAGEDRAW will find it*/
      spotgrupptr = thisgrupptr;
      sprintf(spotgrupptr->name," FILM SPOTS");/*note blank 1st char*/
      spotgrupptr->STATUS = spotgrupptr->STATUS | DOMINANT;
      spotgrupptr->on = 1;
      
      getnewsubgroup(); /*sets to itself the glabal: thissgrpptr */
      spotsgrpptr = thissgrpptr;
      spotsgrpptr->grupptr = spotgrupptr; /*acknowledge parent*/
      spotgrupptr->lastsgrpptr = spotsgrpptr;
         /* adjust pointer in parent group to include this subgroup */
      spotgrupptr->firstsgrpptr = spotsgrpptr;/*parent only has this subgroup*/
      spotsgrpptr->on = 1;
      
      getnewlist(); /*sets to itself the glabal: thislistptr */
      spotlistptr = thislistptr;
      spotlistptr->sgrpptr = spotsgrpptr; /*acknowledge parent*/
      spotlistptr->sgrpptr = spotsgrpptr; /*acknowledge parent*/
      spotsgrpptr->lastlistptr = spotlistptr;
         /* adjust pointer in parent subgroup to include this list */
      spotsgrpptr->firstlistptr = spotlistptr;/*parent only has this list */
      spotlistptr->on = 1;
      *Lnew = 0;
      *Lold = 1;

      LneedSetsinGrafWindow = 1;
   }
   else if(*Lnew==1)
   {
      destroyliststructure(spotlistptr);
      thissgrpptr = spotsgrpptr;
      getnewlist(); /*sets to itself the glabal: thislistptr */
      spotlistptr = thislistptr;
      spotlistptr->sgrpptr = spotsgrpptr; /*acknowledge parent*/
      spotsgrpptr->lastlistptr = spotlistptr;
         /* adjust pointer in parent subgroup to include this list */
      spotsgrpptr->firstlistptr = spotlistptr;/*parent only has this list */
      spotlistptr->on = 1;
      *Lnew = 0;     
   }
   if(Lswitchon)  /*090419 HelixBearHair xrays to film vs spots on film*/
   {
      allocpointstructure(spotlistptr);
      thispointptr->fx = 0;
      thispointptr->fy = 0;
      thispointptr->fz = 0;
      thispointptr->type = MOVETO_P;
      getptIDstring(word, ptptr);
      storeptIDstring(word, thispointptr); /*MAGELIST*/
      calcintegerpoints(thispointptr); /*MAGELIST*/
   }
   allocpointstructure(spotlistptr);
   thispointptr->fx = ptptr->fx + (ptptr->fx*ptptr->fz)/(ctr - ptptr->fz);
   thispointptr->fy = ptptr->fy + (ptptr->fy*ptptr->fz)/(ctr - ptptr->fz);
   thispointptr->fz = -ctr;
   if(Lswitchon) thispointptr->type = VECTOR; /*090419*/
   else  thispointptr->type = DOT; /* DOT for spot */
   getptIDstring(word, ptptr);
   storeptIDstring(word, thispointptr); /*MAGELIST*/
   calcintegerpoints(thispointptr); /*MAGELIST*/
/*
printf("on film: {%s}  %d, %d, %d pruned%d? %d\n"
  , word, thispointptr->ix,thispointptr->iy,thispointptr->iz
  ,PRUNED,thispointptr->type);
*/
}
/*___doxrayplotter()_____________________________________________________*/

#ifdef PROJECTIONGEOMETRY

      __center of Ewald sphere
\
|\
| \
|  \      __reciprocal spot hits Ewald sphere
|   | fz
_fx_|     __crystal
\
|\                      dx     fx
| \                     -- : ------     dx=fx*fz/(ctr-fz)
|  \                    fz   ctr-fz
_fx_\ ___ ctr-fz
|   |\
|___|_\__ ctr     film plane
     dx
#endif

/****dodock3on3()************************************************************/
void dodock3on3() /*020305*/
{
   static  float  fxyz[7][3]; 

   if(Lpoint<=6)
   { 
        if(Lnewdrawline)
        {
            numberline = 0;
            Lnewdrawline = 0;
            drawnewlistptr->on = 0; /*start with drawn lines off*/ /*960527*/
            drawgrupptr->on = 1; /* enable display */
        }
        /* get this last picked point into coordinate list at new position */
        fxyz[Lpoint][0] = pickedpointptr->fx;
        fxyz[Lpoint][1] = pickedpointptr->fy;
        fxyz[Lpoint][2] = pickedpointptr->fz;
      if(Legamickdocking) /*030225*/
      {
         dockpointptr[Lpoint] = pickedpointptr;
         LegamickrepeatOK = 0; /*030225*/
      } 
   }
   if(Lpoint==6)
   {/*Lpoint==6*/
      threepointdock(fxyz);
      Lpoint = 0;
      Ldock3on3on = 0; /*so  dock3on3 is single shot*/
      drawmarker1listptr->on = 0; /*turn off point indicater*/   
      drawmarker2listptr->on = 0;   
      drawmarker3listptr->on = 0;   
      drawmarker4listptr->on = 0;   
      drawmarker5listptr->on = 0;   
      drawmarker6listptr->on = 0;   
      LneedSetsinGrafWindow = 1;
      if(Legamickdocking) /*030217*/
      {
         Lneedannealmobile = 1;
         Lfreeegamickmobile = 0;
         LegamickrepeatOK = 1; /*030225*/
      }
   }/*Lpoint==6*/
}
/*___dodock3on3()___________________________________________________________*/

/*threepointdock()************************************************************/
void threepointdock(float f[7][3])
{
   /*dock mobile group(s) by their 3 points (m) onto reference 3 points (r)*/
   double r1[3],r2[3],r3[3],m1[3],m2[3],m3[3],dbl[3];
   double alignaxis[3],angle1,angle2;
   double ax,ay,az,bx,by,bz,cmag,dist,dot,dmag,emag;
   float  xyz[3];
   
   r1[0] = (double)f[1][0]; r1[1] = (double)f[1][1]; r1[2] = (double)f[1][2];
   r2[0] = (double)f[2][0]; r2[1] = (double)f[2][1]; r2[2] = (double)f[2][2];
   r3[0] = (double)f[3][0]; r3[1] = (double)f[3][1]; r3[2] = (double)f[3][2];
   m1[0] = (double)f[4][0]; m1[1] = (double)f[4][1]; m1[2] = (double)f[4][2];
   m2[0] = (double)f[5][0]; m2[1] = (double)f[5][1]; m2[2] = (double)f[5][2];
   m3[0] = (double)f[6][0]; m3[1] = (double)f[6][1]; m3[2] = (double)f[6][2];

      /*3 step dock of three points onto 3 points*/
      /*1) translate to ref pt 1, s.t mobile pt 1 (m1) is on r1 */
      /*rotations, etc. operate around origin at r1 moved to 0,0,0*/
      /*(m2-m1) X (r2-r1) ==> alignaxis */
      /*(m2-m1) dot (r2-r1)  ==> cos(angle1) */
      /*doaxisrot(mobilexyz-r1, angle1, r1-r1, alignaxis) i.e. origin 0,0,0*/
      /*==> newmobilexyz */

      /*dihedral4pt(newm3,r1,newm2,r3-r1) ==> angle2*/
      /*doaxisrot(newmobilexyz, angle2, origin, r2-r1) ==>newermobilexyz */

      /*translate all newermobilexyz points to origin at r1*/
      
      /*1) translate to ref pt 1, s.t mobile pt 1 (m1) is on r1 */
      translatemobiledock(r1,m1);
      /*also move the mobile reference points, first use orig m1*/
      m2[0] = m2[0] - m1[0] + r1[0];
      m2[1] = m2[1] - m1[1] + r1[1];
      m2[2] = m2[2] - m1[2] + r1[2];
      m3[0] = m3[0] - m1[0] + r1[0];
      m3[1] = m3[1] - m1[1] + r1[1];
      m3[2] = m3[2] - m1[2] + r1[2];
      /*then reset m1 = r1*/
      m1[0] = r1[0];
      m1[1] = r1[1];
      m1[2] = r1[2];

      /*(m2-m1) X (r2-r1) ==> alignaxis */
      ax = m2[0] - m1[0];
      ay = m2[1] - m1[1];
      az = m2[2] - m1[2];
      bx = r2[0] - r1[0];
      by = r2[1] - r1[1];
      bz = r2[2] - r1[2];

      /* c = a x b */
      crossproduct(ax,ay,az,bx,by,bz);/*return global vector dblx,dbly,dblz*/
      /*normalization factor*/
       cmag = sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) ); /*c*/
      dist = 1;
      /*get components of c normalized*/
      if(cmag > 0.000001)
      {
         dblx = dblx*dist/cmag;
         dbly = dbly*dist/cmag;
         dblz = dblz*dist/cmag;
      }
      alignaxis[0] = dblx;
      alignaxis[1] = dbly;
      alignaxis[2] = dblz;

      /*(m2-m1) dot (r2-r1)  ==> cos(angle1) */
      dot = (ax*bx + ay*by + az*bz);
      /*normalization factors*/
      dmag = sqrt( (ax)*(ax) + (ay)*(ay) + (az)*(az) );
      emag = sqrt( (bx)*(bx) + (by)*(by) + (bz)*(bz) );
      if(dmag*emag <0.0001)
      {
          angle1 = 0.0;
      }   
      else angle1 = acos( dot/(dmag*emag) );
      angle1 = angle1*360.0/(2*3.14159);

      /*doaxisrot(mobilexyz-r1, angle1, r1-r1, alignaxis) i.e. origin 0,0,0*/
      /*==> newmobilexyz */
      rotatemobiledock(r1,angle1,alignaxis);
         /*calls doaxisrot() for all points in mobile docking group(s) scope*/

      /*also act on the locally stored mobile point coords picked at start*/ 
      /*but m1 had already been placed at r1*/
      /*the just previously done step puts m2 onto the line toward r2*/
      /*so m3 is the only one really needed to be  moved*/
      m2[0] = r2[0]; m2[1] = r2[1]; m2[2] = r2[2];

      xyz[0] = (float)(m3[0]-r1[0]);
      xyz[1] = (float)(m3[1]-r1[1]);
      xyz[2] = (float)(m3[2]-r1[2]);

      doaxisrot(&xyz[0],&xyz[1],&xyz[2],(float)angle1,0,0,0
                ,alignaxis[0],alignaxis[1],alignaxis[2]);

      m3[0] = xyz[0]+r1[0]; m3[1] = xyz[1]+r1[1]; m3[2] = xyz[2]+r1[2];

      /*dihedral4pt(newm3,r1,r2,r3) ==> angle2*/
      angle2 = dihedral4pt( m3[0],m3[1],m3[2]
                           ,r1[0],r1[1],r1[2]
                           ,r2[0],r2[1],r2[2]
                           ,r3[0],r3[1],r3[2]);

      dbl[0] = r2[0] - r1[0];
      dbl[1] = r2[1] - r1[1];
      dbl[2] = r2[2] - r1[2];

      /*doaxisrot(newmobilexyz, angle2, origin, r2-r1) ==>newermobilexyz */
      /*translates all newermobilexyz points to origin at r1*/
      rotatemobiledock(r1,angle2,dbl);
         /*calls doaxisrot() for all points in mobile docking group(s) scope*/
}
/*___threepointdock()________________________________________________________*/

/****rotatemobiledock()*******************************************************/
void rotatemobiledock(double ctr[3],double theta,double axis[3])
{
   pointstruct* thepointptr;
   pointstruct* thelastpointptr;
   liststruct*  thelistptr;
   grupstruct*  thegrupptr;

   float centerx = (float)ctr[0];
   float centery = (float)ctr[1];
   float centerz = (float)ctr[2];
   double fx2 = (double)axis[0];
   double fy2 = (double)axis[1];
   double fz2 = (double)axis[2];

 if(beginselectionpointptr!=NULL && endselectionpointptr!=NULL)
 {/*protection against egamickdocking annealing  egam 030215*/  
   /*scope set by @beginselect and @endselect */
   thepointptr = beginselectionpointptr;
   thelastpointptr  = endselectionpointptr;

   /*first, update the gnomon points*/  
   thegrupptr = ((thepointptr->listptr)->sgrpptr)->grupptr;
   /*while(thegrupptr->bondrot == DOCKSET)*/
   while((thegrupptr->bondrot|DOCKSET) == DOCKSET) /*040320, 121105 inner ()*/
   {
      thegrupptr->pointx[0] = thegrupptr->pointx[0] - centerx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] - centery;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] - centerz;
/*
void  doaxisrot(float* fxptr,float* fyptr,float* fzptr
,float theta,double ax,double ay,double az,double bx,double by,double bz)
*/
      doaxisrot(&(thegrupptr->pointx[0]),&(thegrupptr->pointx[1])
               ,&(thegrupptr->pointx[2])
               ,(float)theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointx[0] = thegrupptr->pointx[0] + centerx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] + centery;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] + centerz;
      
      thegrupptr->pointy[0] = thegrupptr->pointy[0] - centerx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] - centery;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] - centerz;
      doaxisrot(&(thegrupptr->pointy[0]),&(thegrupptr->pointy[1])
               ,&(thegrupptr->pointy[2])
               ,(float)theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointy[0] = thegrupptr->pointy[0] + centerx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] + centery;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] + centerz;
      
      thegrupptr->pointz[0] = thegrupptr->pointz[0] - centerx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] - centery;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] - centerz;
      doaxisrot(&(thegrupptr->pointz[0]),&(thegrupptr->pointz[1])
               ,&(thegrupptr->pointz[2])
               ,(float)theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->pointz[0] = thegrupptr->pointz[0] + centerx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] + centery;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] + centerz;
      
      thegrupptr->position[0] = thegrupptr->position[0] - centerx;
      thegrupptr->position[1] = thegrupptr->position[1] - centery;
      thegrupptr->position[2] = thegrupptr->position[2] - centerz;
      doaxisrot(&(thegrupptr->position[0]),&(thegrupptr->position[1])
               ,&(thegrupptr->position[2])
               ,(float)theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/
      thegrupptr->position[0] = thegrupptr->position[0] + centerx;
      thegrupptr->position[1] = thegrupptr->position[1] + centery;
      thegrupptr->position[2] = thegrupptr->position[2] + centerz;

      thegrupptr = thegrupptr->nextptr;
      if(thegrupptr == NULL) break;
   } 
    
   while(thepointptr != NULL)
   {/*loop over all points in scope*/
      thepointptr->fx = thepointptr->fx - centerx;
      thepointptr->fy = thepointptr->fy - centery;
      thepointptr->fz = thepointptr->fz - centerz;


      doaxisrot(&(thepointptr->fx),&(thepointptr->fy),&(thepointptr->fz)
              ,(float)theta,(double)0.0,(double)0.0,(double)0.0
                           ,(double)fx2,(double)fy2,(double)fz2);/*MAGEANGL*/

      thepointptr->fx = thepointptr->fx + centerx;
      thepointptr->fy = thepointptr->fy + centery;
      thepointptr->fz = thepointptr->fz + centerz;

      if(thepointptr == thelastpointptr)
      {/*end of scope*/
         thepointptr = NULL;
      }
      else if(thepointptr == (thepointptr->listptr)->lastpointptr)
      {/*at last point in a list*/
         /*970903 MAGE keeps track of all lists as a linked list during */
         /* allocliststructure() so only need to go up to the list level*/

         if((thepointptr->listptr)->nextptr != NULL)
         {/*there is a next list: get first point of next list*/
            thelistptr = (thepointptr->listptr)->nextptr;
            thepointptr = thelistptr->firstpointptr;
            if(thepointptr == NULL) /*list with NO points*/
            {
               while(   (thepointptr == NULL) /*current point*/
                     && (thelistptr->nextptr != NULL) ) /*next list*/
               {/*search for a non-NULL point in the scope*/
                 thelistptr = thelistptr->nextptr; /*next list*/
                 thepointptr = thelistptr->firstpointptr;
               }
            }
         }
         else
         {/*NO  more lists, no more points to rotate*/
            /*somehow thelastpointptr failed to be found*/
            thepointptr = NULL;
         }
      }/*at last point in a list*/
      else
      {
         thepointptr = thepointptr->nextptr;
      }

   }/*loop over all points in scope*/
 }/*protection against egamickdocking annealing  egam 030215*/  
}
/*___rotatemobiledock()______________________________________________________*/

/****translatemobiledock()****************************************************/
void translatemobiledock(double ctr[3],double mobile[3])
{
   pointstruct* thepointptr;
   pointstruct* thelastpointptr;
   liststruct*  thelistptr;
   grupstruct*  thegrupptr;

   float centerx = (float)(mobile[0]-ctr[0]);
   float centery = (float)(mobile[1]-ctr[1]);
   float centerz = (float)(mobile[2]-ctr[2]);

 if(beginselectionpointptr!=NULL && endselectionpointptr!=NULL)
 {/*protection against egamickdocking annealing  egam 030215*/  
   /*scope set by @beginselect and @endselect */
   thepointptr = beginselectionpointptr;
   thelastpointptr  = endselectionpointptr;

   /*first, update the gnomon points*/  
   thegrupptr = ((thepointptr->listptr)->sgrpptr)->grupptr;
   /*while(thegrupptr->bondrot == DOCKSET)*/
   while((thegrupptr->bondrot|DOCKSET) == DOCKSET) /*040320, 121105 inner ()*/
   {
      thegrupptr->pointx[0] = thegrupptr->pointx[0] - centerx;
      thegrupptr->pointx[1] = thegrupptr->pointx[1] - centery;
      thegrupptr->pointx[2] = thegrupptr->pointx[2] - centerz;
      
      thegrupptr->pointy[0] = thegrupptr->pointy[0] - centerx;
      thegrupptr->pointy[1] = thegrupptr->pointy[1] - centery;
      thegrupptr->pointy[2] = thegrupptr->pointy[2] - centerz;
      
      thegrupptr->pointz[0] = thegrupptr->pointz[0] - centerx;
      thegrupptr->pointz[1] = thegrupptr->pointz[1] - centery;
      thegrupptr->pointz[2] = thegrupptr->pointz[2] - centerz;
      
      thegrupptr->position[0] = thegrupptr->position[0] - centerx;
      thegrupptr->position[1] = thegrupptr->position[1] - centery;
      thegrupptr->position[2] = thegrupptr->position[2] - centerz;

      thegrupptr = thegrupptr->nextptr;
      if(thegrupptr == NULL) break;
   } 
    
   while(thepointptr != NULL)
   {/*loop over all points in scope*/
      thepointptr->fx = thepointptr->fx - centerx;
      thepointptr->fy = thepointptr->fy - centery;
      thepointptr->fz = thepointptr->fz - centerz;

      if(thepointptr == thelastpointptr)
      {/*end of scope*/
         thepointptr = NULL;
      }
      else if(thepointptr == (thepointptr->listptr)->lastpointptr)
      {/*at last point in a list*/
         /*970903 MAGE keeps track of all lists as a linked list during */
         /* allocliststructure() so only need to go up to the list level*/

         if((thepointptr->listptr)->nextptr != NULL)
         {/*there is a next list: get first point of next list*/
            thelistptr = (thepointptr->listptr)->nextptr;
            thepointptr = thelistptr->firstpointptr;
            if(thepointptr == NULL) /*list with NO points*/
            {
               while(   (thepointptr == NULL) /*current point*/
                     && (thelistptr->nextptr != NULL) ) /*next list*/
               {/*search for a non-NULL point in the scope*/
                 thelistptr = thelistptr->nextptr; /*next list*/
                 thepointptr = thelistptr->firstpointptr;
               }
            }
         }
         else
         {/*NO  more lists, no more points to translate*/
            /*somehow thelastpointptr failed to be found*/
            thepointptr = NULL;
         }
      }/*at last point in a list*/
      else
      {
         thepointptr = thepointptr->nextptr;
      }

   }/*loop over all points in scope*/
 }/*protection against egamickdocking annealing  egam 030215*/  
}
/*___translatemobiledock()___________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****DoAngleBetweenLines()***************************************************/
void DoAngleBetweenLines(float fcxyz[3][7]) /*140518*/
{
double A1X,A1Y,A1Z,A2X,A2Y,A2Z,ADX,ADY,ADZ;
double AX,AY,AZ,AMAG;
double B1X,B1Y,B1Z,B2X,B2Y,B2Z,BDX,BDY,BDZ;
double BX,BY,BZ,BMAG;
double dot = 0;
double ANGLE = 0;
char   angleBtwStr[256];

    /*line A has ends A1X,Y,Z  A2X,Y,Z 
      line B has ends B1X,Y,Z  B2X,Y,Z
      A • B  gives cos simple angle between A and B 
    */
    A1X = fcxyz[0][1];
    A1Y = fcxyz[1][1];
    A1Z = fcxyz[2][1];
    A2X = fcxyz[0][2];
    A2Y = fcxyz[1][2];
    A2Z = fcxyz[2][2];
    B1X = fcxyz[0][3];
    B1Y = fcxyz[1][3];
    B1Z = fcxyz[2][3];
    B2X = fcxyz[0][4];
    B2Y = fcxyz[1][4];
    B2Z = fcxyz[2][4];
    /*vectoris as length and direction from origin*/
    AX = A2X - A1X;
    AY = A2Y - A1Y;
    AZ = A2Z - A1Z;
    BX = B2X - B1X;
    BY = B2Y - B1Y;
    BZ = B2Z - B1Z;

        /* these have to be normalized by dividing by their magnitudes, */
        /*this can be done either before taking the dot product or after*/
        dot = (double)dotproduct(AX,AY,AZ,BX,BY,BZ);
        AMAG = sqrt(AX*AX + AY*AY + AZ*AZ);
        BMAG = sqrt(BX*BX + BY*BY + BZ*BZ);
        if(AMAG*BMAG <0.0001) 
        {
            ANGLE = 0.0;
        }    
/*c*/   else ANGLE = acos( dot/(AMAG*BMAG) );
        ANGLE = ANGLE*360.0/(2*3.14159);
        /*BUT CANNOT return(ANGLE), DIALOG LONG GONE, PUT ANGLE ON SCREEN!*/
   
    /*140518 try writing directly to screen...*/
    sprintf(constructionstr,"ANGLE= %.3f",ANGLE); /*140518*/
    /*to screen above where measures dist,ang,dih placed 140518*/
}
/*___DoAngleBetweenLines()__________________________________________________*/

/****DoShortestBetweenLinesegments()*****************************************/
void DoShortestBetweenLinesegments(float fcxyz[3][7]) /*971122*/
{
   DoPerpendicularBetweenLines(fcxyz);
   /*which is fine to start with, but if mutual perpendicular is not in the*/
   /*scope of both of the line segments, then the Mickey Mouse stuff...*/
   /*which has not been written*/
}
/*___DoShortestBetweenLinesegments()________________________________________*/

/****DoPerpendicularBetweenLines()*******************************************/
void DoPerpendicularBetweenLines(float fcxyz[3][7]) /*971122*/
{

double A1X,A1Y,A1Z,A2X,A2Y,A2Z,ADX,ADY,ADZ,AMAG,ART;
double B1X,B1Y,B1Z,B2X,B2Y,B2Z,BDX,BDY,BDZ,BMAG,BRT;
double C1X,C1Y,C1Z,C2X,C2Y,C2Z,CDX,CDY,CDZ,CMAG,CRT;
double B[NMATRIX+1],D[NMATRIX+1];
int    jrow,icol;

    /*line A has ends A1X,Y,Z  A2X,Y,Z and direction ADX,Y,Z
      line B has ends B1X,Y,Z  B2X,Y,Z and direction BDX,Y,Z
      line C has ends C1X,Y,Z  C2X,Y,Z and direction CDX,Y,Z
      where line C is the putative perpendicular to A and B 
      Define point C1X,Y,Z to be on A and C2X,Y,Z to be on B
      A x B  gives CDX,Y,Z, solve for C1X,Y,Z and C2X,Y,Z 
    */
    A1X = fcxyz[0][1];
    A1Y = fcxyz[1][1];
    A1Z = fcxyz[2][1];
    A2X = fcxyz[0][2];
    A2Y = fcxyz[1][2];
    A2Z = fcxyz[2][2];
    B1X = fcxyz[0][3];
    B1Y = fcxyz[1][3];
    B1Z = fcxyz[2][3];
    B2X = fcxyz[0][4];
    B2Y = fcxyz[1][4];
    B2Z = fcxyz[2][4];
    ADX = A2X - A1X;
    ADY = A2Y - A1Y;
    ADZ = A2Z - A1Z;
    AMAG = sqrt( ADX*ADX + ADY*ADY + ADZ*ADZ);
    BDX = B2X - B1X;
    BDY = B2Y - B1Y;
    BDZ = B2Z - B1Z;    
    BMAG = sqrt( BDX*BDX + BDY*BDY + BDZ*BDZ);
    
    
    

    /*A x B  gives CDX,Y,Z  returned in dblx,dbly,dblz*/
    crossproduct( (ADX)/AMAG,(ADY)/AMAG,(ADZ)/AMAG
                      ,(BDX)/BMAG,(BDY)/BMAG,(BDZ)/BMAG ); /*.*/

    CDX = dblx;
    CDY = dbly;
    CDZ = dblz;
         
    CMAG = sqrt( CDX*CDX + CDY*CDY + CDZ*CDZ);
         
/*         
printf("CMAG == %f\n",CMAG);         
*/         
    if(CMAG == 0.0)
    {/*parallel lines*/

printf("lines are parallel, try construction to drop a perpendicular from point to line\n");         

    }
    else
    {/*lines not parallel*/

         CDX = CDX/CMAG;
         CDY = CDY/CMAG;
         CDZ = CDZ/CMAG;

    /*equation of a line: point C1 on line A  
      (C1X - A1X)/ADX = (C1Y - A1Y)/ADY = (C1Z - A1Z)/ADZ
      C1X - ADX*ART = A1X ...
      equation of a line: point C2 on line B  
      (C2X - B1X)/BDX = (C2Y - B1Y)/BDY = (C2Z - B1Z)/BDZ
      C2X - BDX*BRT = B1X ...
      equation of a line with points C1 and C2
      (C2X - C1X)/CDX = (C2Y - C1Y)/CDY = (C2Z - C1Z)/CDZ
      C2X - C1X - CDX*CRT = 0 ...
      Normal equation, terms in variables by column:
       1   2   3   4   5   6   7   8   9
      C1X C1Y C1Z C2X C2Y C2Z ART BRT CRT  =   B
       1   0   0   0   0   0 -ADX  0   0   =  A1X
       0   1   0   0   0   0 -ADY  0   0   =  A1Y
       0   0   1   0   0   0 -ADZ  0   0   =  A1Z
       0   0   0   1   0   0   0 -BDX  0   =  B1X
       0   0   0   0   1   0   0 -BDY  0   =  B1Y
       0   0   0   0   0   1   0 -BDZ  0   =  B1Z
      -1   0   0   1   0   0   0   0 -CDX  =   0
       0  -1   0   0   1   0   0   0 -CDY  =   0
       0   0  -1   0   0   1   0   0 -CDZ  =   0
    */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
    /*load matrix components into A, 9x9 matrix*/

A[1][1]= 1;A[1][2]= 0;A[1][3]= 0;A[1][4]=0;A[1][5]=0;A[1][6]=0;A[1][7]=-ADX;A[1][8]=0;A[1][9]=0;
A[2][1]= 0;A[2][2]= 1;A[2][3]= 0;A[2][4]=0;A[2][5]=0;A[2][6]=0;A[2][7]=-ADY;A[2][8]=0;A[2][9]=0;
A[3][1]= 0;A[3][2]= 0;A[3][3]= 1;A[3][4]=0;A[3][5]=0;A[3][6]=0;A[3][7]=-ADZ;A[3][8]=0;A[3][9]=0;

A[4][1]= 0;A[4][2]= 0;A[4][3]= 0;A[4][4]=1;A[4][5]=0;A[4][6]=0;A[4][7]=0;A[4][8]=-BDX;A[4][9]=0;
A[5][1]= 0;A[5][2]= 0;A[5][3]= 0;A[5][4]=0;A[5][5]=1;A[5][6]=0;A[5][7]=0;A[5][8]=-BDY;A[5][9]=0;
A[6][1]= 0;A[6][2]= 0;A[6][3]= 0;A[6][4]=0;A[6][5]=0;A[6][6]=1;A[6][7]=0;A[6][8]=-BDZ;A[6][9]=0;

A[7][1]=-1;A[7][2]= 0;A[7][3]= 0;A[7][4]=1;A[7][5]=0;A[7][6]=0;A[7][7]=0;A[7][8]=0;A[7][9]=-CDX;
A[8][1]= 0;A[8][2]=-1;A[8][3]= 0;A[8][4]=0;A[8][5]=1;A[8][6]=0;A[8][7]=0;A[8][8]=0;A[8][9]=-CDY;
A[9][1]= 0;A[9][2]= 0;A[9][3]=-1;A[9][4]=0;A[9][5]=0;A[9][6]=1;A[9][7]=0;A[9][8]=0;A[9][9]=-CDZ;

/*
for(j=1;j<=9;j++)
printf("A %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n"
,A[j][1],A[j][2],A[j][3],A[j][4],A[j][5],A[j][6],A[j][7],A[j][8],A[j][9]);
*/      
    if(matrixinvert(9))
    {
/*
for(j=1;j<=9;j++)
printf("C %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n"
,C[j][1],C[j][2],C[j][3],C[j][4],C[j][5],C[j][6],C[j][7],C[j][8],C[j][9]);
*/      
      /*load Barray*/
      B[1]=A1X;B[2]=A1Y;B[3]=A1Z;B[4]=B1X;B[5]=B1Y;B[6]=B1Z;B[7]=0;B[8]=0;B[9]=0;
/*
printf("B %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n"
,B[1],B[2],B[3],B[4],B[5],B[6],B[7],B[8],B[9]);
*/
      /*solve for line components*/
      for(jrow=1;jrow<=9;jrow++)
      {  
        D[jrow] = 0.0;
        for(icol=1; icol<=9;icol++)
        {
          D[jrow] = D[jrow] + C[jrow][icol]*B[icol];
          /*where for a given row, multiply C's by col times B by row==col*/
        }
      }
/*
printf("D %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n"
,D[1],D[2],D[3],D[4],D[5],D[6],D[7],D[8],D[9]);
*/
      C1X = D[1];
      C1Y = D[2];
      C1Z = D[3];
      C2X = D[4];
      C2Y = D[5];
      C2Z = D[6];
      ART = D[7];
      BRT = D[8];
      CRT = D[9];
    }
         (thispointptr->previousptr)->fx = (float)C1X;
         (thispointptr->previousptr)->fy = (float)C1Y;
         (thispointptr->previousptr)->fz = (float)C1Z;
         calcintegerpoints(thispointptr->previousptr);
         thispointptr->fx = (float)C2X;
         thispointptr->fy = (float)C2Y;
         thispointptr->fz = (float)C2Z;
         calcintegerpoints(thispointptr); /*971122*/         
if(Ltest)
{         
printf("AMAG %.3f, BMAG %.3f, CMAG %.3f\n",AMAG,BMAG,CMAG);
printf("ART %.3f, BRT %.3f, CRT %.3f\n",ART,BRT,CRT);
printf("CDX  %.3f, CDY  %.3f, CDZ  %.3f\n",CDX,CDY,CDZ);
printf("C1 %.3f,%.3f,%.3f, C2 %.3f,%.3f,%.3f\n",C1X,C1Y,C1Z,C2X,C2Y,C2Z);
}


    }/*lines not parallel*/
}
/*___DoPerpendicularBetweenLines()__________________________________________*/

/****DoNucleicAcidParameters()***********************************************/
//void DoNucleicAcidParameters(float fcxyz[3][7]) /*140912,140915*/
void DoNucleicAcidParameters() /*dockpointptr[7],141006*/
{
//called from doconstructline(), makes MonitorParam
//static  float  fcxyz[3][7]; holds atom xyz values needed for calc
//BUT need ptr to atom for monitoring change of parameters
//use MAGELIST.h/pointstructptr dockpointptr[7] pointers to atoms;
 

    // re MAGEDLOG.c ConstructSixthDialog() 
    //"140912, Nucleic acid backbone: pick n1, c1', c1', n1, P"
    //"base n, ribose c1' of i-1 residue, ribose c1', base n of next (==i),"
    //"then P of intervening PO4 (residue i-1) then P of PO4 residue i"
    //"returns backbone params defined 140912 vintage..."

static int Nwriteout=0;
char     theStringm[256], theStringi[256], theString[256];
int      i;
double   N1mx, N1my, N1mz, C1mx, C1my, C1mz, C1x, C1y, C1z, N1x, N1y, N1z;
double   POmx, POmy, POmz, POx, POy, POz;
double   baseNNdistance, dihedralNCCN, dihedralNCCP, angleNCC, angleCCN;
float    online[3], tript1[3], tript2[3], tript3[3];
float    onC1mC1[3], onN1mC1m[3], onN1C1[3];
float    PmCmCdistance, PmprojCmdist, CmCdist, PmNmCmdistance, PCNdistance;
/*       Pperp-CC,      Pperp i-1,      Pperp ith (maybe) */
float    PPdist; /*if no PO4 beyond last base, repeat the click on previous P*/
/* distances relating intersections of P-perp lines and extension lines*/
float C1mTOperpdist;
float portionalCCdist; /*aka portionaldist*/
float extdistN1mC1m;
float extdistN1C1;
float   NNmidptx, NNmidpty, NNmidptz;
float   CCmidptx, CCmidpty, CCmidptz;
float   PprojRatio; /*141031*/
int n,m;
char  PDBid[8];

 
/*resolve into understandable, this special case, xyz triples: */
/*which is why you gotta pick them in a defined order*/
/*...problematic to depend on perfectly defined residue atom names...*/ 

   N1mx= dockpointptr[1]->fx; /*i.e. N1 or N9 of residue i-1*/
   N1my= dockpointptr[1]->fy;
   N1mz= dockpointptr[1]->fz;
   C1mx= dockpointptr[2]->fx;
   C1my= dockpointptr[2]->fy;
   C1mz= dockpointptr[2]->fz;
   C1x= dockpointptr[3]->fx;
   C1y= dockpointptr[3]->fy;
   C1z= dockpointptr[3]->fz;
   N1x= dockpointptr[4]->fx; /*i.e. N1 or N9 of residue i*/
   N1y= dockpointptr[4]->fy;
   N1z= dockpointptr[4]->fz;
   POmx= dockpointptr[5]->fx;
   POmy= dockpointptr[5]->fy;
   POmz= dockpointptr[5]->fz;
   POx= dockpointptr[6]->fx;
   POy= dockpointptr[6]->fy;
   POz= dockpointptr[6]->fz;
/*resolve into xyz triples: */

//if(Lmonitorparam)
//{
   //fprintf(stderr,"about to killmonitorparam()\n");
//   killmonitorparam(); /*MAGEINPT.c*/
//}/*141126*/
if(!Lmonitorparam)
{
   //fprintf(stderr,"about to addmonitorparam()\n");
    addmonitorparam(); /*MAGEINPT.c*/
}
 //fprintf(stderr,"Lmonitorparam == %d\n",Lmonitorparam);

 {/*replace monitorline points with those just picked ------------------------------*/

 //fprintf(stderr,"about to find monitorlinepointptr\n");
        /*space allocated point-by-point MAGEINPT/addmonitorparam() */
        monitorlinepointptr = monitorlinelistptr->firstpointptr; /*#1 ------------*/
 //fprintf(stderr,"did find monitorlinepointptr\n");
 
        (monitorlinelistptr->firstpointptr)->fx = N1mx;
        (monitorlinelistptr->firstpointptr)->fy = N1my;
        (monitorlinelistptr->firstpointptr)->fz = N1mz;
        //calcintegerpoints(monitorlinelistptr->firstpointptr);/*MAGELIST*/
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

 //fprintf(stderr,"about to redefine monitorlinepointptr\n");
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#2 ------------*/
 //fprintf(stderr,"did redefine monitorlinepointptr\n");
        monitorlinepointptr->fx  = N1x;
        monitorlinepointptr->fy  = N1y;
        monitorlinepointptr->fz  = N1z;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
 //fprintf(stderr,"did assign fxfyfz to  monitorlinepointptr\n");

        monitorlinepointptr = monitorlinepointptr->nextptr; /*#3 ------------*/
        monitorlinepointptr->fx  = C1x;
        monitorlinepointptr->fy  = C1y;
        monitorlinepointptr->fz  = C1z;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /* ------------#4*/
        monitorlinepointptr->fx  = C1mx;
        monitorlinepointptr->fy  = C1my;
        monitorlinepointptr->fz  = C1mz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /* ------------#5*/
        monitorlinepointptr->fx  = N1mx;
        monitorlinepointptr->fy  = N1my;
        monitorlinepointptr->fz  = N1mz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

   baseNNdistance = sqrt(  (N1mx-N1x)*(N1mx-N1x)
                         + (N1my-N1y)*(N1my-N1y)
                         + (N1mz-N1z)*(N1mz-N1z) );

   dihedralNCCN = dihedral4pt(  N1mx, N1my, N1mz
                              , C1mx, C1my, C1mz
                              , C1x , C1y , C1z
                              , N1x , N1y , N1z );

   angleNCC = angle3pt(  N1mx, N1my, N1mz
                       , C1mx, C1my, C1mz
                       , C1x , C1y , C1z  );

   angleCCN = angle3pt(  C1mx, C1my, C1mz
                       , C1x , C1y , C1z
                       , N1x , N1y , N1z );

   dihedralNCCP = dihedral4pt(  N1mx, N1my, N1mz
                              , C1mx, C1my, C1mz
                              , C1x , C1y , C1z
                              , POmx, POmy, POmz );

 /*onC1mC1[3], onN1mC1m[3], onN1C1[3] */
 /*perpendicular- 0th point on line-from 3rd pt to line defined by pts 1 & 2 */
   tript1[0] = C1mx; tript1[1] = C1my; tript1[2] = C1mz; 
   tript2[0] = C1x;  tript2[1] = C1y;  tript2[2] = C1z; 
   tript3[0] = POmx; tript3[1] = POmy; tript3[2] = POmz; 
   PerpPtOnLine(onC1mC1, tript1, tript2, tript3);
   PmCmCdistance = sqrt(  (POmx-onC1mC1[0])*(POmx-onC1mC1[0])
                        + (POmy-onC1mC1[1])*(POmy-onC1mC1[1])
                        + (POmz-onC1mC1[2])*(POmz-onC1mC1[2]) );
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#6 ------------*/
        monitorlinepointptr->fx  = POmx;
        monitorlinepointptr->fy  = POmy;
        monitorlinepointptr->fz  = POmz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#7 ------------*/
        monitorlinepointptr->fx  = onC1mC1[0];
        monitorlinepointptr->fy  = onC1mC1[1];
        monitorlinepointptr->fz  = onC1mC1[2];
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

   PmprojCmdist =  sqrt(  (C1mx-onC1mC1[0])*(C1mx-onC1mC1[0])
                        + (C1my-onC1mC1[1])*(C1my-onC1mC1[1])
                        + (C1mz-onC1mC1[2])*(C1mz-onC1mC1[2]) );

   CmCdist =  sqrt(  (C1mx-C1x)*(C1mx-C1x)
                   + (C1my-C1y)*(C1my-C1y)
                   + (C1mz-C1z)*(C1mz-C1z) );

   portionalCCdist = PmprojCmdist/CmCdist; /*later recalc orig portionaldist*/

   /*CC length calc above, CCmidptx,y,z for display of torsion: dihedralNCCN*/
   CCmidptx = (C1mx + C1x)/2; 
   CCmidpty = (C1my + C1y)/2; 
   CCmidptz = (C1mz + C1z)/2; 

 /*perpendicular- 0th point on line-from 3rd pt to line defined by pts 1 & 2 */
   tript1[0] = N1mx; tript1[1] = N1my; tript1[2] = N1mz; 
   tript2[0] = C1mx; tript2[1] = C1my; tript2[2] = C1mz; 
   tript3[0] = POmx; tript3[1] = POmy; tript3[2] = POmz; 
   PerpPtOnLine(onN1mC1m, tript1, tript2, tript3);
   PmNmCmdistance = sqrt(  (POmx-onN1mC1m[0])*(POmx-onN1mC1m[0])
                         + (POmy-onN1mC1m[1])*(POmy-onN1mC1m[1])
                         + (POmz-onN1mC1m[2])*(POmz-onN1mC1m[2]) );
   /*PmPerp P to glycosidic  line, NmCm extent: Cm to intersection*/
   extdistN1mC1m = sqrt(  (C1mx-onN1mC1m[0])*(C1mx-onN1mC1m[0])
                        + (C1my-onN1mC1m[1])*(C1my-onN1mC1m[1])
                        + (C1mz-onN1mC1m[2])*(C1mz-onN1mC1m[2]) );
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#8 ------------*/
        monitorlinepointptr->fx  = POmx;
        monitorlinepointptr->fy  = POmy;
        monitorlinepointptr->fz  = POmz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#9 ------------*/
        monitorlinepointptr->fx  = onN1mC1m[0];
        monitorlinepointptr->fy  = onN1mC1m[1];
        monitorlinepointptr->fz  = onN1mC1m[2];
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

 /*perpendicular- 0th point on line-from 3rd pt to line defined by pts 1 & 2 */
   tript1[0] = C1x; tript1[1] = C1y; tript1[2] = C1z; 
   tript2[0] = N1x; tript2[1] = N1y; tript2[2] = N1z; 
   tript3[0] = POx; tript3[1] = POy; tript3[2] = POz; 
   /*look for repeat click on POm to indicate PO is missing, i.e. zero dist*/
   PPdist =   sqrt(  (POx-POmx)*(POx-POmx)
                   + (POy-POmy)*(POy-POmy)
                   + (POz-POmz)*(POz-POmz) );
 //if(PPdist < .1) 141106 zero makes a mess...
 //{/*repeat hit on earlier PO4 indicates missing terminal PO4*/
 //   PCNdistance = 0;/*141106 don't use this*/
 //}
 //else
   {
      PerpPtOnLine(onN1C1, tript1, tript2, tript3);
      PCNdistance = sqrt(  (POx-onN1C1[0])*(POx-onN1C1[0])
                         + (POy-onN1C1[1])*(POy-onN1C1[1])
                         + (POz-onN1C1[2])*(POz-onN1C1[2]) );
   }
   /*PiPerp P to glycosidic  line, NC extent: C1 to intersection*/
   extdistN1C1 = sqrt(  (C1x-onN1C1[0])*(C1x-onN1C1[0])
                      + (C1y-onN1C1[1])*(C1y-onN1C1[1])
                      + (C1z-onN1C1[2])*(C1z-onN1C1[2]) );
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#10 ------------*/
        monitorlinepointptr->fx  = POx;
        monitorlinepointptr->fy  = POy;
        monitorlinepointptr->fz  = POz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /*#11 ------------*/
        monitorlinepointptr->fx  = onN1C1[0];
        monitorlinepointptr->fy  = onN1C1[1];
        monitorlinepointptr->fz  = onN1C1[2];
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

   /*PPdistance as a parameter  141021*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /*12 ------------*/
        monitorlinepointptr->fx  = POmx;
        monitorlinepointptr->fy  = POmy;
        monitorlinepointptr->fz  = POmz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/
        monitorlinepointptr = monitorlinepointptr->nextptr; /*13 ------------*/
        monitorlinepointptr->fx  = POx;
        monitorlinepointptr->fy  = POy;
        monitorlinepointptr->fz  = POz;
        calcintegerpoints(monitorlinepointptr);/*MAGELIST*/

 }/*replace monitorline points with those just picked ------------------------------*/

    /*140912 try writing directly to screen...*/
    /*to screen above where measures dist,ang,dih placed 140912*/
    /*constructionstr written there eventually in MAGEDRAW.c*/

getptIDstring(theStringm, dockpointptr[1]);
for(i=5; i<15; i++){theStringm[i-5] = theStringm[i];} /*15<16<17 sans Bfactor 141031*/
{theStringm[i-5] = '\0';}
getptIDstring(theStringi, dockpointptr[4]);
for(i=5; i<15; i++){theStringi[i-5] = theStringi[i];}
{theStringi[i-5] = '\0';}

/*PDB code is expected to be the last 4 characters of the ptID*/
/*recapture a whole ptID*/
getptIDstring(theString, dockpointptr[4]);
m = strlen(theString);
n = 0;
for(i=m-4; i<m; i++) {PDBid[n++] = theString[i];} /*where n is now left augmented */
{PDBid[n] = '\0';} /*nth position had been augmented beyond last significant character*/

//fprintf(stderr,"\nPDBid = %s\n",PDBid);
//fprintf(stderr,"\nptID = %s\n",theString);

    sprintf(constructionstr,"Suite %s to %s ,N-N= %.3f",theStringm,theStringi,baseNNdistance); //140912

//sprintf(monitorparamstr,"%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", baseNNdistance, dihedralNCCN, angleNCC, angleCCN, dihedralNCCP, PmCmCdistance, PmprojCmdist, CmCdist, PmNmCmdistance, PCNdistance);

/*PmCmCdistance: P to C1'-C1'line, i.e. Pprojection distance*/ 
/*PmprojCmdist: C1' to where Pprojection hits C1'-C1' line*/
/* PprojRatio = PmprojCmdist/PmCmCdistance */
   PprojRatio = PmprojCmdist/PmCmCdistance; /*141031*/

    sprintf(monitorparamstr,"NN %.3f, NCCN %.3f, NCC %.3f, CCN %.3f, NCCP %.3f, PCC %.3f, pCC %.3f, CC %.3f, PNC %.3f, PCN %.3f, PP %.3f, Pratio %.3f, partCC %.3f", baseNNdistance, dihedralNCCN, angleNCC, angleCCN, dihedralNCCP, PmCmCdistance, PmprojCmdist, CmCdist, PmNmCmdistance, PCNdistance, PPdist, PprojRatio, portionalCCdist); /*PP 141021, % small distance of Pproj on CC line to CCdist 141106*/


if(Nwriteout <1) //ONE TIME HEADER
{/*ONE TIME HEADER*/
   fprintf(stdout,"\n DoNucleicAcidParameters for Suites in %s\n",PDBid); /*141031*/
   fprintf(stdout,"\n ptID, PDB, baseNNdistance, dihedralNCCN, angleNCC, angleCCN, dihedralNCCP, PmCmCdistance, PmprojCmdist, /CmCdist, PmNmCmdistance, extdistN1mC1m, PCNdistance, extdistN1C1, P-P ,Pratio \n"); /*141021*/
   fprintf(stdout,"{base ptID :base ptID ,PDB },  N-N , *NCCN* ,   NCC  ,   CCN  ,  NCCP  ,*PCCpj,*PCCpt,  C-C , Pp_m , ext_m,  Pp  , ext  ,  P-P ,Pratio \n"); /*PP distance  141021, Pratio and spacing 141031*/
}/*ONE TIME HEADER*/


if(!LParameterlinekinemage) /*LNucleicAcidParameters WRITE ITEM TO STDOUT*/
{/*!LParameterlinekinemage*/
                //{ s  s},  NN  , NCCN , NCC  , CCN  , NCC  , PCC  , pCC  , CC   , PNC  , ext  , PCN  , ext  , P-P , ratio 
   fprintf(stdout,"{%s:%s,%s}, %5.3f, %7.2f, %7.2f, %7.2f, %7.2f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f, %5.3f  \n",theStringm,theStringi,PDBid, baseNNdistance, dihedralNCCN, angleNCC, angleCCN, dihedralNCCP, PmCmCdistance, PmprojCmdist, CmCdist, PmNmCmdistance, extdistN1mC1m, PCNdistance, extdistN1C1, PPdist, PprojRatio, portionalCCdist);
}/*!LParameterlinekinemage*/




else /*LParameterlinekinemage*/
{/*LParameterlinekinemage*/
   if(Nwriteout <1)
   {
      fprintf(stdout,"@group {Params}\n");
   }
   fprintf(stdout,"\n@subgroup {%s:%s} dominant \n",theStringm,theStringi);
   fprintf(stdout,"@vectorlist {%s:%s} color= green width= 7\n",theStringm,theStringi);

   fprintf(stdout,"{%s N1m} P %.3f, %.3f, %.3f\n",theStringm,N1mx,N1my,N1mz);
   fprintf(stdout,"{%s N1} L %.3f, %.3f, %.3f\n",theStringi,N1x,N1y,N1z);

   /*NN length calc above, midpoint for display of length: */
   NNmidptx = (N1mx + N1x)/2; 
   NNmidpty = (N1my + N1y)/2; 
   NNmidptz = (N1mz + N1z)/2; 

   fprintf(stdout,"{%s N1m} P %.3f, %.3f, %.3f\n",theStringm,N1mx,N1my,N1mz);
   fprintf(stdout,"{%s C1m} L %.3f, %.3f, %.3f\n",theStringm,C1mx,C1my,C1mz);

   fprintf(stdout,"{%s C1m} P %.3f, %.3f, %.3f\n",theStringm,C1mx,C1my,C1mz);
   fprintf(stdout,"{%s C1} L %.3f, %.3f, %.3f\n",theStringi,C1x,C1y,C1z);

   fprintf(stdout,"{%s C1} P %.3f, %.3f, %.3f\n",theStringi,C1x,C1y,C1z);
   fprintf(stdout,"{%s N1} L %.3f, %.3f, %.3f\n",theStringi,N1x,N1y,N1z);

   fprintf(stdout,"{%s POm} P %.3f, %.3f, %.3f\n",theStringm,POmx,POmy,POmz);
   fprintf(stdout,"{%s onC1mC1} L %.3f, %.3f, %.3f\n",theStringm,onC1mC1[0],onC1mC1[1],onC1mC1[2]);

   fprintf(stdout,"{%s POm} P %.3f, %.3f, %.3f\n",theStringm,POmx,POmy,POmz);
   fprintf(stdout,"{%s onN1mC1m} L %.3f, %.3f, %.3f\n",theStringm,onN1mC1m[0],onN1mC1m[1],onN1mC1m[2]);

   fprintf(stdout,"{%s PO} P %.3f, %.3f, %.3f\n",theStringi,POx,POy,POz);
   fprintf(stdout,"{%s onN1C1} L %.3f, %.3f, %.3f\n",theStringi,onN1C1[0],onN1C1[1],onN1C1[2]);

   fprintf(stdout,"{%s POm} P %.3f, %.3f, %.3f\n",theStringm,POmx,POmy,POmz);
   fprintf(stdout,"{%s PO} L %.3f, %.3f, %.3f\n",theStringi,POx,POy,POz);
   /*PPdistance 141021*/

   /*BALLLIST incl calculations of distances of balls to&on extension lines*/
   fprintf(stdout,"@balllist {%s:%s} color= green radius= 0.3 \n",theStringm,theStringi);
   fprintf(stdout,"{%s onC1mC1} L %.3f, %.3f, %.3f\n",theStringm,onC1mC1[0],onC1mC1[1],onC1mC1[2]); /*POm to C1m---C1 line, PmprojCmdist */
   /*intersection on C1m---C1 line*/
   C1mTOperpdist = sqrt(  (C1mx-onC1mC1[0])*(C1mx-onC1mC1[0])
                        + (C1my-onC1mC1[1])*(C1my-onC1mC1[1])
                        + (C1mz-onC1mC1[2])*(C1mz-onC1mC1[2]) );
   portionalCCdist = C1mTOperpdist/CmCdist; /*was portionaldist*/

   fprintf(stdout,"{%s onN1mC1m} L %.3f, %.3f, %.3f\n",theStringm,onN1mC1m[0],onN1mC1m[1],onN1mC1m[2]); /*POm to NmCm line, PmNmCmdistance == PmPerp */
   /*PmPerp P to glycosidic  line, NmCm extent: Cm to intersection*/
   /*extdistN1mC1m calculated earlier*/
   //extdistN1mC1m = sqrt(  (C1mx-onN1mC1m[0])*(C1mx-onN1mC1m[0])
   //                     + (C1my-onN1mC1m[1])*(C1my-onN1mC1m[1])
   //                     + (C1mz-onN1mC1m[2])*(C1mz-onN1mC1m[2]) );

   fprintf(stdout,"{%s onN1C1} L %.3f, %.3f, %.3f\n",theStringi,onN1C1[0],onN1C1[1],onN1C1[2]); /*POm to NmCm line, PCNdistance == next PiPerp */
   /*PiPerp P to glycosidic  line, NC extent: C1 to intersection*/
   /*extdistN1C1 calculated earlier*/
   //extdistN1C1 = sqrt(  (C1x-onN1C1[0])*(C1x-onN1C1[0])
   //                   + (C1y-onN1C1[1])*(C1y-onN1C1[1])
   //                   + (C1z-onN1C1[2])*(C1z-onN1C1[2]) );

   /*labelLIST of calculated  distances of balls to&on extension lines*/
   fprintf(stdout,"@labellist {%s:%s} color= green master= {paramLabels} \n",theStringm,theStringi);

   fprintf(stdout,"{   NNlength %.3f } L %.3f, %.3f, %.3f\n",baseNNdistance,NNmidptx, NNmidpty, NNmidptz);

   fprintf(stdout,"{   fraction %.3f } L %.3f, %.3f, %.3f\n",portionalCCdist,onC1mC1[0],onC1mC1[1],onC1mC1[2]); /*POm to C1m---C1 line, PmprojCmdist */
   /*intersection on C1m---C1 line*/

   fprintf(stdout,"{   perp %.3f at %.3f } L %.3f, %.3f, %.3f\n",PmNmCmdistance,extdistN1mC1m,onN1mC1m[0],onN1mC1m[1],onN1mC1m[2]); /*POm to NmCm line, PmNmCmdistance == PmPerp */
   /*PmPerp P to glycosidic  line, NmCm extent: Cm to intersection*/

   fprintf(stdout,"{   perp %.3f at %.3f } L %.3f, %.3f, %.3f\n",PCNdistance,extdistN1C1,onN1C1[0],onN1C1[1],onN1C1[2]); /*PO to NC line, PCNdistance == PiPerp */
   /*PiPerp P to glycosidic  line, NC extent: C1 to intersection*/

   /*CC length calc above, CCmidptx,y,z for display of torsion: dihedralNCCN*/
   fprintf(stdout,"{   NCCN %.3f } L %.3f, %.3f, %.3f\n",dihedralNCCN,CCmidptx,CCmidpty,CCmidptz); /*PO to NC line, PCNdistance == PiPerp */
}/*LParameterlinekinemage*/

   Nwriteout++;
   Lrescalekinemage = 1; /*invokes rescalekinemage: fxfyfz into display form*/
   Lredrawvector = 1; /*141021*/

} 
/*___DoNucleicAcidParameters()______________________________________________*/

/****matrixinvert()**********************************************************/
int matrixinvert(int N)
{
   /*MATINV.SUB pg 427, Programming in Basic for Engineers, Kamal B. Rojiani*/
   /*1988 PWS-KENT (Wadsworth) Boston  ISBN 0-534-91899-9                   */
   /*double A[ROWA][COLA]; */ /* matrix  ROWA == COLA */
   /*double C[ROWA][COLA]; */ /* inverse of A */
   /*double DET;*/ /*determinant*/ /*980610*/ /*orig. not initialized before being used*/
   double DET = 1.0; /*determinant = product of PIVOTs */ /*980610*/
   double TEMP,PIVOT,MULTIPLIER;
   int    NPIVOT = 0; /*number of times rows are pivited*/
   int    ROWA,COLA,ROWC;

   int i,j,PASS,MAXROW,ROW,COL;

   if(N>NMATRIX)
   {
      return(0);
   }
   ROWA = N;
   COLA = N;
   ROWC = N;

   /*store identity matrix in C, this will be replaced by the inverse*/
   for(i=1; i<= ROWC; i++)
   {
     for(j=1; j<= ROWC; j++)
     {
        if(i==j) C[i][j] = 1;
        else     C[i][j] = 0;
     }
   }
   /*computation*/
   for(PASS=1; PASS<= COLA; PASS++)
   {/*PASS*/
     /*partial pivoting, For each pass find the largest element in the column*/
     /*containing the pivot element.  The pivot column is PASS.  MAXROW is */
     /*row containing the largest element in the pivot column.  */
     /*It is initially set equal to the pivot row.*/
     
     MAXROW = PASS;
     for(ROW = PASS; ROW<= ROWA; ROW++)
     {
        /*if(abs((int)A[ROW][PASS]) > abs((int)A[MAXROW][PASS])) MAXROW = ROW;*/
        /*080912 seems wrong to use abs(int) here... */
        if(fabs(A[ROW][PASS]) > fabs(A[MAXROW][PASS])) MAXROW = ROW; /*080912*/
     }
     if(MAXROW != PASS)
     {/*Interchange pivot row*/ 
       /*with row containing largest element in the */
       /*pivot column in both A and C */
       NPIVOT++;
       for(COL=1; COL<= COLA; COL++)
       {
          TEMP = A[PASS][COL];
          A[PASS][COL] = A[MAXROW][COL];
          A[MAXROW][COL] = TEMP;
          TEMP = C[PASS][COL];
          C[PASS][COL] = C[MAXROW][COL];
          C[MAXROW][COL] = TEMP;
       }
     }/*Interchange pivot row*/ 
     /*Compute determinant and check for singularity */
     /*Determinant is the product of the pivot elements*/
     /*Pivot element is A[PASS][PASS] */
     PIVOT = A[PASS][PASS];
     DET = DET*PIVOT;
     if(DET == 0.0)
     {/*matrix is singular*/
        fprintf(stderr,"matrix singular\n");
        return(0);
     }/*matrix is singular*/
     /*Normalization. Divide pivot row by the pivot element*/
     /* so pivot element is reduced to 1 */
     for(COL=1; COL <= COLA; COL++)
     {
        A[PASS][COL] = A[PASS][COL]/PIVOT;
        C[PASS][COL] = C[PASS][COL]/PIVOT;
     }
     /*Elimination. Reduce all elements in pivot column to zero*/
     for(ROW=1; ROW<= ROWA; ROW++)
     {/*ROW*/
       if(ROW != PASS)  /*what happens if ROW==PASS???? */
       {
         MULTIPLIER = A[ROW][PASS];
         for(COL=1; COL<= COLA; COL++)
         {/*COL*/
           A[ROW][COL] = A[ROW][COL] - (A[PASS][COL]*MULTIPLIER);
           C[ROW][COL] = C[ROW][COL] - (C[PASS][COL]*MULTIPLIER);
               /*MULTIPLIER is how information from A gets into C */
         }/*COL*/
       }
     }/*ROW*/
   }/*PASS*/  
   if(NPIVOT%2) DET = -DET; /*if NPIVOT odd ... */
   return(1); /*1 for success*/   
}
/*___matrixinvert()_________________________________________________________*/

/****InTriangle()************************************************************/
int  InTriangle(float fcxyz[3][7])  /*080912*/
{
   /*check that point in fcxyz[][0] is within triangle of fcxyz[][1,2,3] */
   /*Three crossproducts of the edges in order with the corresponding line */
   /*from the starting edge point to the putative contained point*/
   /*If the normals which these three describe are all on the same side, */
   /*then the point is within the triangle. */
   double ax=0,ay=0,az=0,bx=0,by=0,bz=0;
   double n1x=0,n1y=0,n1z=0,n2x=0,n2y=0,n2z=0,n3x=0,n3y=0,n3z=0;
   double d12=0,d23=0,d31=0;
   int    LIN = 0;

   ax = (double)(fcxyz[0][2]-fcxyz[0][1]);
   ay = (double)(fcxyz[1][2]-fcxyz[1][1]);
   az = (double)(fcxyz[2][2]-fcxyz[2][1]);
   bx = (double)(fcxyz[0][0]-fcxyz[0][1]);
   by = (double)(fcxyz[1][0]-fcxyz[1][1]);
   bz = (double)(fcxyz[2][0]-fcxyz[2][1]);
   crossproduct(ax,ay,az,bx,by,bz); /*return vector is dblx,dbly,dblz */
   n1x = dblx; n1y = dbly; n1z = dblz;
   ax = (double)(fcxyz[0][3]-fcxyz[0][2]);
   ay = (double)(fcxyz[1][3]-fcxyz[1][2]);
   az = (double)(fcxyz[2][3]-fcxyz[2][2]);
   bx = (double)(fcxyz[0][0]-fcxyz[0][2]);
   by = (double)(fcxyz[1][0]-fcxyz[1][2]);
   bz = (double)(fcxyz[2][0]-fcxyz[2][2]);
   crossproduct(ax,ay,az,bx,by,bz); /*return vector is dblx,dbly,dblz */
   n2x = dblx; n2y = dbly; n2z = dblz;
   ax = (double)(fcxyz[0][1]-fcxyz[0][3]);
   ay = (double)(fcxyz[1][1]-fcxyz[1][3]);
   az = (double)(fcxyz[2][1]-fcxyz[2][3]);
   bx = (double)(fcxyz[0][0]-fcxyz[0][3]);
   by = (double)(fcxyz[1][0]-fcxyz[1][3]);
   bz = (double)(fcxyz[2][0]-fcxyz[2][3]);
   crossproduct(ax,ay,az,bx,by,bz); /*return vector is dblx,dbly,dblz */
   n3x = dblx; n3y = dbly; n3z = dblz;
   d12 = (double)dotproduct(n1x,n1y,n1z,n2x,n2y,n2z);
   d23 = (double)dotproduct(n2x,n2y,n2z,n3x,n3y,n3z);
   d31 = (double)dotproduct(n3x,n3y,n3z,n1x,n1y,n1z);
   if     (d12 > 0 && d23 > 0 && d31 > 0) {LIN = +1;}
   else if(d12 < 0 && d23 < 0 && d31 < 0) {LIN = -1;}
   else {LIN = 0;}
/*
fprintf(stderr,"pt: %.4f %.4f %.4f, d12== %.3f, d23== %.3f, d31== %3f, LIN== %d\n"
,fcxyz[0][0],fcxyz[1][0],fcxyz[2][0]
,d12,d23,d31,LIN);
*/
   return(LIN);
}
/*___InTriangle()___________________________________________________________*/

/****InTriangle2()************************************************************/
int  InTriangle2(float punitivept[3], float edgemiddle[4][3], float edgenormal[4][3])  /*080916*/
{
   /*parameters used in: InTriange2()   080916*/  
   /*check that point in fcxyz[][0] is within triangle of fcxyz[][1,2,3] */
   /* where we pass punitive point as punitivept[]  and */
   /* edgenormal[][] is the reference normal as if at edgemiddle[][]  */
   /*drop perpendicular from each point of the triangle to opposite side*/
   /*perpendicular vector from putative interior point to a side should */
   /*be in same direction as corresponding vector to that side from opp pt*/
   /*any failure falsifies conjecture, all 3 true proves conjecture */
   /*thet the point is within the triangle. */

   double ax=0,ay=0,az=0;
   int    LIN = 1; /*In until falsified*/
   int    i=0;

   for(i=1; i<=3; i++)
   {
      ax = (double)(punitivept[0] - edgemiddle[i][0]);
      ay = (double)(punitivept[1] - edgemiddle[i][1]);
      az = (double)(punitivept[2] - edgemiddle[i][2]);
      if(dotproduct(ax,ay,az,edgenormal[i][0],edgenormal[i][1],edgenormal[i][2])
         < 0) {LIN=0;break;} /*point cannot be in triangle*/
   }
   return(LIN);
}
/*___InTriangle2()___________________________________________________________*/

/****TriangleParams()*********************************************************/
void TriangleParams(float trianglept[4][3], float edgemiddle[4][3], float edgenormal[4][3])  /*080916*/
{
   /*parameters used in: InTriange2()   080916*/  
   /*check that point in fcxyz[][0] is within triangle of fcxyz[][1,2,3] */
   /*drop perpendicular from each point of the triangle to opposite side*/
   /*perpendicular vector from putative interior point to a side should */
   /*be in same direction as corresponding vector to that side from opp pt*/
   /*any failure falsifies conjecture, all 3 true proves conjecture */
   /*thet the point is within the triangle. */
   float perponline[4][3];

   /*point on edge-line perpendicular to opposite triangle pt*/
   PerpPtOnLine(perponline[3],trianglept[1],trianglept[2],trianglept[3]); 
   edgenormal[3][0] =  trianglept[3][0] - perponline[3][0];
   edgenormal[3][1] =  trianglept[3][1] - perponline[3][1];
   edgenormal[3][2] =  trianglept[3][2] - perponline[3][2];
   edgemiddle[3][0] = (trianglept[1][0] + trianglept[2][0])/2;
   edgemiddle[3][1] = (trianglept[1][1] + trianglept[2][1])/2;
   edgemiddle[3][2] = (trianglept[1][2] + trianglept[2][2])/2;

   PerpPtOnLine(perponline[1],trianglept[2],trianglept[3],trianglept[1]); 
   edgenormal[1][0] =  trianglept[1][0] - perponline[1][0];
   edgenormal[1][1] =  trianglept[1][1] - perponline[1][1];
   edgenormal[1][2] =  trianglept[1][2] - perponline[1][2];
   edgemiddle[1][0] = (trianglept[2][0] + trianglept[3][0])/2;
   edgemiddle[1][1] = (trianglept[2][1] + trianglept[3][1])/2;
   edgemiddle[1][2] = (trianglept[2][2] + trianglept[3][2])/2;

   PerpPtOnLine(perponline[2],trianglept[3],trianglept[1],trianglept[2]); 
   edgenormal[2][0] =  trianglept[2][0] - perponline[2][0];
   edgenormal[2][1] =  trianglept[2][1] - perponline[2][1];
   edgenormal[2][2] =  trianglept[2][2] - perponline[2][2];
   edgemiddle[2][0] = (trianglept[3][0] + trianglept[1][0])/2;
   edgemiddle[2][1] = (trianglept[3][1] + trianglept[1][1])/2;
   edgemiddle[2][2] = (trianglept[3][2] + trianglept[1][2])/2;
}
/*___TriangleParams()________________________________________________________*/

/****EquationOfPlane()*******************************************************/
void EquationOfPlane(float fcxyz[3][7])  /*080912*/
{
     /*returns AP,BP,CP,DP: fcxyz[0][5],fcxyz[1][5],fcxyz[2][5],fcxyz[0][6] */
double A1X,A1Y,A1Z,A2X,A2Y,A2Z;
double B1X,B1Y,B1Z,B2X,B2Y,B2Z;
double C1X,C1Y,C1Z;
double T,AP,BP,CP,DP,PI;
double TN,TD; /*080912  parts of T*/
double B[4][4]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
double D[4][4]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
/*080912 B and D matrix to test inversion process */
    
    PI = 3.14159;/*971103*/ /*Matrix inverter can't handle point 0,0,0*/
    /*and is flaky if any coord == 0, sooooo add PI to all coord*/
    /*and hope that that doesn't inadventantly set some coord==0 */
    /*080912 but this seems to change parameters of plane's equation*/
    PI = 0.0; /*test 080912*/
    
    /*three points A1X,Y,Z   A2X,Y,Z   B1X,Y,Z define a plane*/
    /*fourth point  B2X,Y,Z is not on this plane, find perpendicular*/
    /*from this point to the plane at C1X,Y,Z */
    
    A1X = fcxyz[0][1]+PI;
    A1Y = fcxyz[1][1]+PI;
    A1Z = fcxyz[2][1]+PI;
    A2X = fcxyz[0][2]+PI;
    A2Y = fcxyz[1][2]+PI;
    A2Z = fcxyz[2][2]+PI;
    B1X = fcxyz[0][3]+PI;
    B1Y = fcxyz[1][3]+PI;
    B1Z = fcxyz[2][3]+PI;
    B2X = fcxyz[0][4]+PI; /*080912 4th point not used for eq of plane*/
    B2Y = fcxyz[1][4]+PI;
    B2Z = fcxyz[2][4]+PI;

    
    /*Equation of a plane from the earliest 3 points
      A1X*AP + A1Y*BP + A1Z*CP + DP = 0
      A2X*AP + A2Y*BP + A2Z*CP + DP = 0
      B1X*AP + B1Y*BP + B1Z*CP + DP = 0
      rearrange into normal equation form:
      A1X*AP + A1Y*BP + A1Z*CP = -DP
      A2X*AP + A2Y*BP + A2Z*CP = -DP
      B1X*AP + B1Y*BP + B1Z*CP = -DP
            [AP]   [-DP]             [-DP] = [AP]
      [   ]*[BP] = [-DP]   so  [ i ]*[-DP] = [BP]  
            [CP]   [-DP]             [-DP] = [CP]
      load 3x3 matrix  A
    */
    A[1][1] = B[1][1] = A1X; A[1][2] = B[1][2] = A1Y; A[1][3] = B[1][3] = A1Z;
    A[2][1] = B[2][1] = A2X; A[2][2] = B[2][2] = A2Y; A[2][3] = B[2][3] = A2Z;
    A[3][1] = B[3][1] = B1X; A[3][2] = B[3][2] = B1Y; A[3][3] = B[3][3] = B1Z;
    /*keeping a copy in B[][] to check inverse matrix, A gets set to I */
/*
fprintf(stderr,"matrix A\n");
fprintf(stderr,"A %.3f,%.3f,%.3f\n",A[1][1],A[1][2],A[1][3]);
fprintf(stderr,"A %.3f,%.3f,%.3f\n",A[2][1],A[2][2],A[2][3]);
fprintf(stderr,"A %.3f,%.3f,%.3f\n",A[3][1],A[3][2],A[3][3]);
*/
    matrixinvert(3); 
/*
fprintf(stderr,"matrix inverted C\n");
fprintf(stderr,"C %.3f,%.3f,%.3f\n",C[1][1],C[1][2],C[1][3]);
fprintf(stderr,"C %.3f,%.3f,%.3f\n",C[2][1],C[2][2],C[2][3]);
fprintf(stderr,"C %.3f,%.3f,%.3f\n",C[3][1],C[3][2],C[3][3]);
*/
    /*080912 test that origA*C = I i.e. test B*C==I ? */
/*
    D[1][1] = (B[1][1]*C[1][1] + B[1][2]*C[2][1] + B[1][3]*C[3][1]);
    D[1][2] = (B[1][1]*C[1][2] + B[1][2]*C[2][2] + B[1][3]*C[3][2]);
    D[1][3] = (B[1][1]*C[1][3] + B[1][2]*C[2][3] + B[1][3]*C[3][3]);

    D[2][1] = (B[2][1]*C[1][1] + B[2][2]*C[2][1] + B[2][3]*C[3][1]);
    D[2][2] = (B[2][1]*C[1][2] + B[2][2]*C[2][2] + B[2][3]*C[3][2]);
    D[2][3] = (B[2][1]*C[1][3] + B[2][2]*C[2][3] + B[2][3]*C[3][3]);

    D[3][1] = (B[3][1]*C[1][1] + B[3][2]*C[2][1] + B[3][3]*C[3][1]);
    D[3][2] = (B[3][1]*C[1][2] + B[3][2]*C[2][2] + B[3][3]*C[3][2]);
    D[3][3] = (B[3][1]*C[1][3] + B[3][2]*C[2][3] + B[3][3]*C[3][3]);

fprintf(stderr,"matrix D = A*C\n");
fprintf(stderr,"D %.3f,%.3f,%.3f\n",D[1][1],D[1][2],D[1][3]);
fprintf(stderr,"D %.3f,%.3f,%.3f\n",D[2][1],D[2][2],D[2][3]);
fprintf(stderr,"D %.3f,%.3f,%.3f\n",D[3][1],D[3][2],D[3][3]);
*/
    /*
      now C is inverse of A
      AP = C[1][1]*(-DP) + C[1][2]*(-DP) + C[1][3]*(-DP)
      BP = C[2][1]*(-DP) + C[2][2]*(-DP) + C[2][3]*(-DP)
      CP = C[3][1]*(-DP) + C[3][2]*(-DP) + C[3][3]*(-DP)
      and equation of plane is
      X*AP + Y*BP + Z*CP + DP = 0
        X*(-DP)*(C[1][1] + C[1][2] + C[1][3])
       +Y*(-DP)*(C[2][1] + C[2][2] + C[2][3])
       +Z*(-DP)*(C[3][1] + C[3][2] + C[3][3])
       = -DP
      multiply through by -DP
        X*(C[1][1] + C[1][2] + C[1][3])
       +Y*(C[2][1] + C[2][2] + C[2][3])
       +Z*(C[3][1] + C[3][2] + C[3][3])
       = 1
      direction components of normal to this plane:
    */
    AP =  (C[1][1] + C[1][2] + C[1][3]);
    BP =  (C[2][1] + C[2][2] + C[2][3]);
    CP =  (C[3][1] + C[3][2] + C[3][3]);
    /*for a given case
      A1X*AP + A1Y*BP + A1Z*CP = -DP
    */
    DP =  -A1X*AP - A1Y*BP - A1Z*CP;
    /*080912  equation of plane:*/
    /*  AP x  +  BP y  +  CP z  +  DP  =  0  */
    fcxyz[0][5] = (float)AP;
    fcxyz[1][5] = (float)BP;
    fcxyz[2][5] = (float)CP;
    fcxyz[0][6] = (float)DP;
}
/*___EquationOfPlane()______________________________________________________*/

/****DoPerpendicularToPlane()************************************************/
void DoPerpendicularToPlane(float fcxyz[3][7])
{
   double B2X,B2Y,B2Z;  /*original 4th point */    
   double C1X,C1Y,C1Z;  /*putative point in plane to be found*/
   double T,AP,BP,CP,DP; /*equation of plane components*/

   EquationOfPlane(fcxyz);

      /*returns AP,BP,CP,DP: fcxyz[0][5],fcxyz[1][5],fcxyz[2][5],fcxyz[0][6] */

      AP = (double)fcxyz[0][5];
      BP = (double)fcxyz[1][5];
      CP = (double)fcxyz[2][5];
      DP = (double)fcxyz[0][6];

   /*at this place: equation of plane:  AP x  +  BP y  +  CP z  =  DP  */
   /* for a given x,y,z e.g. B2X,B2Y,B2Z, find the point where a perp */
   /* intersects the plane */

      B2X = fcxyz[0][4];
      B2Y = fcxyz[1][4];
      B2Z = fcxyz[2][4];
   /*
      find line through point B2X,B2Y,B2Z that has these direction components
      and then find point on the plane where this line intersects the plane.
      C1X = B2X + AP*T
      C1Y = B2Y + BP*T
      C1Z = B2Z + CP*T
      C1X*AP + C1Y*BP + C1Z*CP + DP = 0;
      
      AP*(B2X + AP*T) + BP*(B2Y + BP*T) + CP*(B2Z + CP*T) + DP = 0;
      AP*B2X + AP*AP*T + BP*B2Y + BP*BP*T + CP*B2Z + CP*CP*T + DP = 0;
      T*(AP*AP + BP*BP + CP*CP) = -D -AP*B2X -BP*B2Y -CP*B2Z
      T = (-DP -AP*B2X -BP*B2Y -CP*B2Z)/(AP*AP + BP*BP + CP*CP)
      substituting results from above
    */
    T =   (-DP -(B2X*AP) -(B2Y*BP) -(B2Z*CP))/( (AP*AP) +(BP*BP) +(CP*CP) );
    C1X = B2X + AP*T;
    C1Y = B2Y + BP*T;
    C1Z = B2Z + CP*T;


fprintf(stderr,"MAGEANGL:equation of triangle plane: %fx + %fy + %fz + %f = 0,"
               "  T== %f \n" ,AP,BP,CP,DP,T);

/*
fprintf(stderr,"A1X,A1Y,A1Z,    A2X,A2Y,A2Z,   B1X,B1Y,B1Z\n");
fprintf(stderr," %.3f,%.3f,%.3f   ,%.3f,%.3f,%.3f   ,%.3f,%.3f,%.3f\n"
,A1X,A1Y,A1Z,A2X,A2Y,A2Z,B1X,B1Y,B1Z);
fprintf(stderr,"B2X,B2Y,B2Z: %.3f,%.3f,%.3f,  C1X,C1Y,C1Z: %.3f,%.3f,%.3f\n"
,B2X,B2Y,B2Z,C1X,C1Y,C1Z);
*/
         fcxyz[0][0] = (float)(C1X);
         fcxyz[1][0] = (float)(C1Y);
         fcxyz[2][0] = (float)(C1Z);
/*
fprintf(stderr,"\n\nMAGEANGL/DoPerpendicularToPlane() %f %f %f \n\n"
,fcxyz[0][0],fcxyz[1][0],fcxyz[2][0]);
*/

}
/*___DoPerpendicularToPlane()_______________________________________________*/

/****DoPerpendicularToLine(fcxyz)*********************************************/
void DoPerpendicularToLine(float fcxyz[3][7]) /*rtn in fcxyz[123][0] */
{
   double A1X,A1Y,A1Z,A2X,A2Y,A2Z,ADX,ADY,ADZ,ART; /*for perpendicular*/
   double B1X,B1Y,B1Z,B2X,B2Y,B2Z; /*for perpendicular*/

        /*perpendicular from 3rd point to line of points 1 and 2 */
        A1X = fcxyz[0][1];
        A1Y = fcxyz[1][1];
        A1Z = fcxyz[2][1];
        A2X = fcxyz[0][2];
        A2Y = fcxyz[1][2];
        A2Z = fcxyz[2][2];
        B1X = fcxyz[0][3];
        B1Y = fcxyz[1][3];
        B1Z = fcxyz[2][3];
        ADX = A2X - A1X;
        ADY = A2Y - A1Y;
        ADZ = A2Z - A1Z;
/*arbitrary point on line A 
   B2X = A1X + ADX*ART;B2Y = A1Y + ADY*ART;B2Z = A1Z + ADZ*ART;
      
   direction components of line B1 --- B2  
   BDX = A1X-B1X+ADX*ART; BDY = A1Y-B1Y+ADY*ART; BDZ = A1Z-B1Z+ADZ*ART;
   for this to be perpendicular to line A, sum of product of components ==0
   ADX*(A1X-B1X+ADX*ART)+ADY*(A1Y-B1Y+ADY*ART)+ADZ*(A1Z-B1Z+ADZ*ART)=0;
   rearrange
   ADX*A1X-ADX*B1X+ADX*ADX*ART
   +ADY*A1Y-ADY*B1Y+ADY*ADY*ART
   +ADZ*A1Z-ADZ*B1Z+ADZ*ADZ*ART=0;
   regroup
   ART*(ADX*ADX++ADY*ADY+ADZ*ADZ)
         =ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z;
*/
         ART =  (ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z)
               /(ADX*ADX+ADY*ADY+ADZ*ADZ);

         B2X = A1X + ADX*ART;
         B2Y = A1Y + ADY*ART;
         B2Z = A1Z + ADZ*ART;

         fcxyz[0][0] = (float)B2X;
         fcxyz[1][0] = (float)B2Y;
         fcxyz[2][0] = (float)B2Z;
}
/*___DoPerpendicularToLine(fcxyz)____________________________________________*/

/****PerpPtOnLine( )**********************************************************/
void PerpPtOnLine(float online[3],float tript1[3],float tript2[3],float tript3[3]) 
{
   double A1X,A1Y,A1Z,A2X,A2Y,A2Z,ADX,ADY,ADZ,ART; /*for perpendicular*/
   double B1X,B1Y,B1Z,B2X,B2Y,B2Z; /*for perpendicular*/

        /*perpendicular from 3rd point to line of points 1 and 2 */
        A1X = tript1[0];
        A1Y = tript1[1];
        A1Z = tript1[2];
        A2X = tript2[0];
        A2Y = tript2[1];
        A2Z = tript2[2];
        B1X = tript3[0];
        B1Y = tript3[1];
        B1Z = tript3[2];
        ADX = A2X - A1X;
        ADY = A2Y - A1Y;
        ADZ = A2Z - A1Z;
/*arbitrary point on line A 
   B2X = A1X + ADX*ART;B2Y = A1Y + ADY*ART;B2Z = A1Z + ADZ*ART;
      
   direction components of line B1 --- B2  
   BDX = A1X-B1X+ADX*ART; BDY = A1Y-B1Y+ADY*ART; BDZ = A1Z-B1Z+ADZ*ART;
   for this to be perpendicular to line A, sum of product of components ==0
   ADX*(A1X-B1X+ADX*ART)+ADY*(A1Y-B1Y+ADY*ART)+ADZ*(A1Z-B1Z+ADZ*ART)=0;
   rearrange
   ADX*A1X-ADX*B1X+ADX*ADX*ART
   +ADY*A1Y-ADY*B1Y+ADY*ADY*ART
   +ADZ*A1Z-ADZ*B1Z+ADZ*ADZ*ART=0;
   regroup
   ART*(ADX*ADX++ADY*ADY+ADZ*ADZ)
         =ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z;
*/
         ART =  (ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z)
               /(ADX*ADX+ADY*ADY+ADZ*ADZ);

         B2X = A1X + ADX*ART;
         B2Y = A1Y + ADY*ART;
         B2Z = A1Z + ADZ*ART;

         online[0] = (float)B2X;
         online[1] = (float)B2Y;
         online[2] = (float)B2Z;
}
/*___PerpPtOnLine()__________________________________________________________*/



