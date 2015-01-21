/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*              MAGEOUT.C           MAGE general calls mostly doing writeouts */
 
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEPOST.h"
#include "MAGEFLAG.h"
#include "MAGEANGL.h"
#include "MAGEBBOX.h"
#include "MAGETABL.h"
#include "MAGECOLR.h"
#include "MAGESCRT.h"
#include "MAGEOUTR.h"

#define MAXFILESTR 128

void writeindividuallist(void);
void writeindividualpoint(int);
void writepointID(void);
void writefullrgbpalette(void);
void accumsums(void);
void getmeans(void);
void accumdevi(void);
void getsigmas(void);
void meansigmasout(void);     

static int  Lbeginselectionout=0; /*beginselectionpointptr */
static int  ipass=0, npass=0;
static int  LemphasisOUT=0;
static int  LNOditto=1; /*writerotated needs all full pointIDs 051120*/

/****commandfrompointID()*****************************************************/
void commandfrompointID(void)
{
   char command[256];

   getptIDstring(word, pickedpointptr);
   sprintf(command,"echo \"@start\" >>simout.kin\n"); 
   system(command);
   sprintf(command,"echo \"@nowfind {%s}\" >>simout.kin\n",word); 
   system(command);
   sprintf(command,"echo \"@finish\" >>simout.kin\n"); 
   system(command);
}
/*___commandfrompointID()____________________________________________________*/

/****writefullrgbpalette()****************************************************/
void writefullrgbpalette()
{
   int j=0, k=0, level=0;
   char bkg = ' ';

   fprintf(fpout,"@fullrgbpalette\n");
   for(j=0; j<MageRGBtableSIZE; j++)
   {
      k = j;
      level = 0;
      while(k > 50) /*change to 50 when black-->deadblack*/
      {
         k = k - 50; 
         level++;
      }
      
      if(k > 25) /*change to 25 when black-->deadblack*/
      {/*white background*/
         bkg = 'W';
         k = k - 25;       
      }
      else
      {/*black background*/
         bkg = 'B';
      }
      /*now k is the index of the color*/
      interpretcolor(k); /*which puts color name into global word*/
      if(level >= 5)
      {/*ran off end of actual used palette cells*/
         if(j == 255)
         {
            sprintf(word,"deadwhite index==");
            level = 255;
         }
         else
         {
            sprintf(word,"unused index==");
         }
         level = j;
         bkg = ' ';
      }
      else if(j == 0)
      {
         sprintf(word,"deadblack index==");
         bkg = ' ';
      }
      if(Lshiftkey)
      {
         fprintf(fpout,"{%3d} %5d, %5d, %5d, {%-10s %d %c}\n"
            ,j
            ,(257*(myfullpalette[j][0]))
            ,(257*(myfullpalette[j][1]))
            ,(257*(myfullpalette[j][2]))
            ,word,level,bkg);        
      }
      else
      {  /*divide down by 256, multiply back up by 257*/
         fprintf(fpout,"{%3d} %3d, %3d, %3d, {%-10s %d %c}\n"
            ,j
            ,((myfullpalette[j][0]))
            ,((myfullpalette[j][1]))
            ,((myfullpalette[j][2]))
            ,word,level,bkg);        
      }
      /* and the {...} can be edited to be comments*/
   }
   fclose(fpout);
   fpout = NULL;
}
/*___writefullrgbpalette()___________________________________________________*/

/****writepointID()***********************************************************/
void    writepointID()
{
   fprintf(fpout,"@start\n"); /*c*/
   getptIDstring(word, pickedpointptr);
   fprintf(fpout,"@nowfind {%s}\n",word); /*c*/
   fprintf(fpout,"@finish\n"); /*c*/
   fclose(fpout);
   fpout = NULL;
}
/*___writepointID()__________________________________________________________*/

/****writematrix()************************************************************/
void    writematrix() /*write current view*/
{
    int   iout=0,i=0;
    float span=0;
        
    fprintf(fpout,"@%dviewid {%s}\n",iviewset,viewcom[iviewset]);
    if(Lspanfromzoom)
    {
       span = oldmaxwide/(Scale * zoom);
       fprintf(fpout,"@%dspan %.2f \n",iviewset,span);
    }
    else
    {
       fprintf(fpout,"@%dzoom %.2f \n",iviewset,zoom);
    }
    iout = izclipold[0];/*970915*/
    fprintf(fpout,"@%dzslab %d\n",iviewset,iout);
    iout = iztranold[0];/*970915*/
    fprintf(fpout,"@%dztran %d\n",iviewset,iout); /*970909*/
                           
    fprintf(fpout,"@%dcenter %.3f %.3f %.3f\n"
                          ,iviewset,fxcenternew,fycenternew,fzcenternew);
    if(gwidthview[iviewset] > 0 && gheightview[iviewset] > 0) /*041031*/
    {
       fprintf(fpout,"@%dsize %d %d\n"
                     ,iviewset,gwidthview[iviewset],gheightview[iviewset]);
    }
    if(Laxischoice[iviewset])  /*060613 save as, 061123 here, iviewset 061125*/
    {  /*axes array indexed from 0, count axes from 1 */
       fprintf(fpout,"@%daxischoice %d %d %d\n"
        ,iviewset,1+axischoice[iviewset][0],1+axischoice[iviewset][1]
        ,1+axischoice[iviewset][2]);
    }
    fprintf(fpout,"@%dmatrix\n",iviewset);
    fprintf(fpout,"%8.5f ",a11);
    fprintf(fpout,"%8.5f ",-a12);      /*  1  2  3            1   0   0   */
    fprintf(fpout,"%8.5f ",a13);       /*  7  8  9            0   0   1   */
    fprintf(fpout,"%8.5f ",a21);       /*  4  5  6       X    0  -1   0   */
    fprintf(fpout,"%8.5f ",-a22);
    fprintf(fpout,"%8.5f ",a23);       /* screen handedness correction: */
    fprintf(fpout,"%8.5f ",a31);       /* back correct so written matrix*/
    fprintf(fpout,"%8.5f ",-a32);      /* is correct for input to this, */
    fprintf(fpout,"%8.5f\n",a33);      /* or any program                */
    fclose(fpout);
    fpout = NULL;
}
/*___writematrix()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writefocus()*************************************************************/
void    writefocus()
{
            recondition(focusxyzstr);
            fprintf(fpout,"%s\n",focusxyzstr); /*c*/
            condition(focusxyzstr);
            fclose(fpout);
            fpout = NULL;
}
/*___writefocus()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writerotated()***********************************************************/
void      writerotated()
{/*this only works if the rotated groups/sections are still active*/

   if(beginselectionpointptr != NULL)
   {/*expect selection from begin and end selection*/ 
      Lbeginselectionout = 1; /*flag to write @beginselect*/
   }
   Lnewstuffactive = 0;
   if(Lkinformat)
   {   
       fprintf(fpout,"@text rotated stuff\n"
                     "@kinemage 1\n"
                     "@caption MAGE rotated and outputted\n");
   }
   else if(Lrotangleout) /*070706,071215*/
   {
      fprintf(fpout,"@text rotation angles \n"); /*hypertext*/
      if(conformerstr[0] != '\0') fprintf(fpout,"*{%s\n",conformerstr); /* } */
      else fprintf(fpout,"*{\n"); /*balance} hypertext start*/
   }
   else /*!Lrotangleout and NOT Lkinformat*/  /*071215*/
   {/*PDB format: */
      if(conformerstr[0] != '\0') /*071215*/
      {
         fprintf(fpout,"REMARK *{%s}*\n",conformerstr);
      }
      if(ribose3hyperstr[0] != '\0')
      {
         fprintf(fpout,"REMARK *{%s}*\n",ribose3hyperstr);
      }
      if(ribose5hyperstr[0] != '\0')
      {
         fprintf(fpout,"REMARK *{%s}*\n",ribose5hyperstr);
      }
   }
   thisgrupptr = firstgrupptr;
   while(thisgrupptr != NULL)
   {/*loop-over-groups*/
     if(thisgrupptr->bondrot != 0) /*inc thisgrupptr->bondrot==DOCKSET */
     {/*rotated-group*/
      if(  (Lprobeactive && !thisgrupptr->on)  /*NO Probe if NOT ON 041109*/
         ||(!Lkinformat && !thisgrupptr->on && Lvisibleout) ) /*070702*/
      {;} /*NOP*/
      else
      {/*group outputable*/
       if(Lkinformat)
       {/*Lkinformat*/
            fprintf(fpout,"@group {%s}\n",thisgrupptr->name);
            if(thisgrupptr->pdbfilestr[0] != '\0')
            {
               fprintf(fpout,"@grouppdbfile {%s}\n",thisgrupptr->pdbfilestr);
            }
            if(  (  thisgrupptr->pointx[0] >  (float)1.0001 
                  ||thisgrupptr->pointx[0] <  (float)0.9999)
               ||(  thisgrupptr->pointx[1] >  (float)0.0001 
                  ||thisgrupptr->pointx[1] < -(float)0.0001)
               ||(  thisgrupptr->pointx[2] >  (float)0.0001 
                  ||thisgrupptr->pointx[2] < -(float)0.0001)
                  
               ||(  thisgrupptr->pointy[0] >  (float)0.0001 
                  ||thisgrupptr->pointy[0] < -(float)0.0001)
               ||(  thisgrupptr->pointy[1] >  (float)1.0001 
                  ||thisgrupptr->pointy[1] <  (float)0.9999)
               ||(  thisgrupptr->pointy[2] >  (float)0.0001 
                  ||thisgrupptr->pointy[2] < -(float)0.0001)
                  
               ||(  thisgrupptr->pointz[0] >  (float)0.0001 
                  ||thisgrupptr->pointz[0] < -(float)0.0001)
               ||(  thisgrupptr->pointz[1] >  (float)0.0001 
                  ||thisgrupptr->pointz[1] < -(float)0.0001)
               ||(  thisgrupptr->pointz[2] >  (float)1.0001 
                  ||thisgrupptr->pointz[2] <  (float)0.9999)

               ||(  thisgrupptr->position[0] >  (float)0.0001 
                  ||thisgrupptr->position[0] < -(float)0.0001)
               ||(  thisgrupptr->position[1] >  (float)0.0001 
                  ||thisgrupptr->position[1] < -(float)0.0001)
               ||(  thisgrupptr->position[2] >  (float)0.0001 
                  ||thisgrupptr->position[2] < -(float)0.0001) )
            {/*initial values: 1,0,0  0,1,0  0,0,1  0,0,0 */
               fprintf(fpout,"@gnomon\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  ,thisgrupptr->pointx[0]
                  ,thisgrupptr->pointx[1]
                  ,thisgrupptr->pointx[2]
                  ,thisgrupptr->pointy[0]
                  ,thisgrupptr->pointy[1]
                  ,thisgrupptr->pointy[2]
                  ,thisgrupptr->pointz[0]
                  ,thisgrupptr->pointz[1]
                  ,thisgrupptr->pointz[2]
                  ,thisgrupptr->position[0]
                  ,thisgrupptr->position[1]
                  ,thisgrupptr->position[2]
                  );
            }
            npass = 1;
       }/*Lkinformat*/
       else if(!Lrotangleout) /*and NOT Lkinformat*/  /*070705*/
       {/*PDB format: 2 passes in group: 1st heavy atoms: 2nd hydrogens*/
          if(Lhydrogensout && LhydrogensSep) /*070829*/
          {/*double pass to separate hydrogens only works for 1 residue/group*/
             npass = 2;
          }
          else {npass = 1;}
          if(update_res_model >= 0)
          {/*model record needed by probe if model is specified*/
             fprintf(fpout,"MODEL      %3d\n",update_res_model); /*050121*/
             /*ignore this possibility for LrotoutSORTED as of 070901*/
          }
       }
       else {npass = 1;}  /*070705*/
       for(ipass = 1; ipass<=npass; ipass++)
       {/*passes over each group*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL)
        {/*loop-over-subgroups*/
         if(  (Lprobeactive && !thissgrpptr->on)  /*NO Probe if NOT ON 041109*/
            ||(!Lkinformat && !thissgrpptr->on && Lvisibleout) ) /*070702*/
         {;} /*NOP*/
         else
         {/*subgroup outputable*/
          if(  (thissgrpptr->bondrot != 0)
             ||( ((thisgrupptr->bondrot)&DOCKSET)==DOCKSET) ) /*050123*/
          {/*rotated-subgroup*/ /*inc thisgrupptr->bondrot==DOCKSET*/
            if(Lkinformat)
            {
                fprintf(fpout,"@subgroup {%s}\n",thissgrpptr->name);
            }
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*loop-over-lists*/              
             if(  (Lprobeactive && !thislistptr->on)/*NO Probe if OFF 041109*/
                ||(!Lkinformat && !thislistptr->on && Lvisibleout) ) /*070702*/
             {;} /*NOP*/
             else
             {/*list outputable*/
              if(  (thislistptr->bondrot != 0)
                 ||( ((thisgrupptr->bondrot)&DOCKSET)==DOCKSET) ) /*050123*/
              {/*rotated-list*/ /*Sheep from Goats criteria*/
                  /*inc thisgrupptr->bondrot==DOCKSET*/ 
                 if(Lrotangleout)  /*070705*/
                 {
                   if(  (  (thislistptr->bondrot >0 && thislistptr->bondrot<100)
                         &&(bondrotptr[thislistptr->bondrot])->slider  != 0 )
                      &&(  (bondrotptr[thislistptr->bondrot])->select  != 0 
                         ||(bondrotptr[thislistptr->bondrot])->changed != 0 ) )
                   {
                      fprintf(fpout,"rot={%s} %.3f\n"
                         ,thislistptr->name
                         ,bondrotptr[thislistptr->bondrot]->angle);
                   }
                 }
                 else
                 {
                    writeindividuallist(); /*MAGEOUT.c*/
                 }
              }/*rotated-list*/
             }/*list outputable*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
              else thislistptr = thislistptr->nextptr;
            }/*loop-over-lists*/              
          }/*rotated-subgroup*/
         }/*subgroup outputable*/
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*loop-over-subgroups*/
       }/*passes over each group*/
      }/*group outputable*/
     }/*rotated-group*/
     thisgrupptr = thisgrupptr->nextptr;
   }/*loop-over-groups*/
   if(Lrotangleout) /*070706*/
   {
      /*{balance*/ fprintf(fpout,"}*"); /* hypertext end*/
   }
   if(LrotoutSORTED)
   {
      writeOUTRotordered();
   }
   Lhydrogensout = 1; /*reset so probe will get hydrogens 070829*/
   LhydrogensSep = 0;
   LrotoutSORTED = 0; /*reset so probe will get its usual atoms 070901*/
   fprintf(fpout,"\n");
   if(fpout != stdout && fpout != stderr)
   {
      fclose(fpout);
      fpout = NULL;
   }
}
/*___writerotated()__________________________________________________________*/
/*name[n],&ncnt[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,i5,1x,a5,a3,1x,a1,i4,a1,3x,3&8.3)*/
/*  these next lines are 84 char long, they contain 80 char templates */
/*0123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789*/
/*123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_*/
/*atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89*/
/*atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90*/
/*  --name*6 record designator: atom or hetatom recognized  */
/*  --ncnt:i5 atom number: moved blindly to output file  */
/*  --atom*5 includes atomname*4 and alt-conformation-indicator*1,  */ 
/*  --res*3 residue name  */
/*  --sub*1 chain ID: subunit designator  */
/*  --num:i4 residue number  */
/*  --rins*1 inserted residue indicator  */
/*  --x,y,z coord of the atom  */

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writedrawnew()**********************************************************/
int        writedrawnew()
{
   int     ireturn;
   char    dname[26],hold[256],colorstr[32],buttonstr[32],masterstr[256];
   int     hasvectorlist,hasdotlist,haslabellist,hasarrowlist,haswierdlist;
   int     hasballlist;
   unsigned long long mask; /*081120 long long*/
   char    hiliteness[32],staticness[26],onoffstr[16],tempstr[32]; 
    
   /*in any case, this attempt to write a file with the drawnew */
   /*stuff relieves MAGE of worrying about saving drawnew stuff*/
   Ldrawactive = 0;  /*worry flag*/
   hasvectorlist = 0;
   hasdotlist    = 0;
   haslabellist  = 0;
   hasarrowlist  = 0;
   hasballlist    = 0;
   haswierdlist  = 0;
   /*scan to see what type of lists are present */
   thispointptr = drawnewlistptr->firstpointptr;
   while(thispointptr != NULL)
   {/*scan all points in drawnew list*/   
     if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
        &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
     {/*survived pruning*/
        if(       (  (thispointptr->type) & VECTOR  ) 
                &&(  (thispointptr->type) & VARIANT1) == 0 
                &&(  (thispointptr->type) & VARIANT2) == 0 
                &&(  (thispointptr->type) & VARIANT3) == 0 )
            /*plain vector*/
            hasvectorlist = 1;
       else if(   (  (thispointptr->type) & DOT     ) ) /*dot*/
            hasdotlist = 1;  
       else if(   (  (thispointptr->type) & LABEL   ) ) /*label*/
            haslabellist = 1;
       else if(   (  (thispointptr->type) & ARROW   ) ) /*arrow*/
            hasarrowlist = 1;
       else if(   (  (thispointptr->type) & BALL    ) ) /*ball*/
            hasballlist = 1;
       else if(   (  (thispointptr->type) & STORED  ) )
            haswierdlist = 1;
       else haswierdlist = 1;
     }/*survived pruning*/
     /*NOTE: ARROW == (VECTOR | VARIANT3) see MAGEFLAG.h*/
     if(thispointptr == drawnewlistptr->lastpointptr)  
           thispointptr = NULL;
     else thispointptr = thispointptr->nextptr;

   }/*scan all points in drawnew list*/
          
   if(haswierdlist)
   {
       sprintf(alertstr,"something besides vectors, labels, dots, balls"
                        " in draw new stuff!");
       dosinglealert(0); /*____DLOG.C*/
   }
   if(hasvectorlist||haslabellist||hasdotlist||hasarrowlist||hasballlist)
   {/*has vectors or labels or dots or arrows or balls*/
       thislistptr = drawnewsgrpptr->firstlistptr; 
       /*NOTE: only one list of draw stuff*/
       interpretcolor(thislistptr->color);  
          /*color name string put into global "word[]"*/ 
       sprintf(temps," color= %s",word);
       sprintf(colorstr,"%s",temps);

       if(thislistptr->STATUS & NOBUTTON) sprintf(buttonstr," nobutton");
	        else buttonstr[0]='\0';

       masterstr[0]='\0'; /*clear str to hold place for masters*/ 

       if(thislistptr->master > 0) /*master button for this one*/
       {
           thismasterptr = firstmasterptr;
           while(thismasterptr != NULL)
           {
               mask = thismasterptr->mask;
               /*mask = 1<<(i-1);*/ /*see ajustmaster() in MAGEBBOX.c*/
               if((thislistptr->master)&mask)
               {
                  sprintf(hold,"%s master= {%s}"
                      ,masterstr,thismasterptr->name);
                  sprintf(masterstr,"%s",hold); /*combined->masterstr*/
               }
               thismasterptr = thismasterptr->nextptr;
           }
       }
       if(thislistptr->on == 0) sprintf(onoffstr," off ");
       else onoffstr[0]='\0';
       
       if(thislistptr->radius > 0.001) 
       /*only allow positive values now*/
       {
           sprintf(dname," radius= %.3f ",thislistptr->radius); 
       }
       else dname[0]='\0';

       if( (thislistptr->STATUS & STATICFLAG) == STATICFLAG)
       {
          sprintf(staticness," static");
       }
       if( (thislistptr->STATUS & SCREENFLAG) == SCREENFLAG)
       {
          sprintf(staticness," screen");
       }
       else staticness[0]='\0';
       /*Thus screen is a more rigorous form of static */
       /*but in any case, they are incompatible types */

       hiliteness[0] = '\0'; /*clear*/
       if( (thislistptr->STATUS & NOHILITEFLAG) == NOHILITEFLAG)
       {
           sprintf(tempstr," nohilite");
       }
       else tempstr[0]='\0';
       if( (thislistptr->STATUS & NOBLKRIMFLAG) == NOBLKRIMFLAG) /*040925*/
       {
           strcat(tempstr," noblkrim");
       }
       sprintf(hiliteness,"%s",tempstr);
       
       if(hasballlist)
       {/*ball output*/
           fprintf(fpout,"@balllist {balls}%s%s%s%s%s%s%s\n"
             ,colorstr,buttonstr,masterstr,onoffstr
             ,dname,hiliteness,staticness); /*invent list name*/
           thispointptr = thislistptr->firstpointptr;
           while(thispointptr != NULL)
           {
             if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
                &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
             {/*survived pruning*/
               if( (thispointptr->type) & BALL ) /*ball*/
               {/*write ball point*/
                  writeindividualpoint(0); /*iPointFlag==0*/ /*writedrawnew()*/
               }/*write ball point*/
             }/*survived pruning*/
             if(thispointptr == thislistptr->lastpointptr)
                   thispointptr = NULL;
             else thispointptr = thispointptr->nextptr;
           }
       }/*ball output*/

       if(hasvectorlist) 
       {/*vector output*/
           fprintf(fpout,"@vectorlist {%s}%s%s%s%s%s%s%s\n"
             ,thislistptr->name,colorstr,buttonstr,masterstr,onoffstr
             ,dname,hiliteness,staticness); /*using the one list name is*/
           thispointptr = thislistptr->firstpointptr;
           while(thispointptr != NULL)
           {
             if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
                &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
             {/*survived pruning*/
               if(  (  (thispointptr->type) & VECTOR  ) 
                  &&(  (thispointptr->type) & VARIANT1) == 0 
                  &&(  (thispointptr->type) & VARIANT2) == 0 
                  &&(  (thispointptr->type) & VARIANT3) == 0 )
               {/*write plain vector point*/
                   writeindividualpoint(0); /*iPointFlag==0*/ /*writedrawnew()*/
               }/*write plain vector point*/
             }/*survived pruning*/
             if(thispointptr == thislistptr->lastpointptr)
                   thispointptr = NULL;
             else thispointptr = thispointptr->nextptr;
           }
       }/*vector output*/

       if(haslabellist)
       {/*label output*/
           fprintf(fpout,"@labellist {labels}%s%s%s%s%s%s%s\n"
             ,colorstr,buttonstr,masterstr,onoffstr
             ,dname,hiliteness,staticness); /*invent list name*/
           thispointptr = thislistptr->firstpointptr;
           while(thispointptr != NULL)
           {
             if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
                &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
             {/*survived pruning*/
               if( (thispointptr->type) & LABEL ) /*label*/
               {/*write label point*/
                  writeindividualpoint(0); /*iPointFlag==0*/ /*writedrawnew()*/
               }/*write label point*/
             }/*survived pruning*/
             if(thispointptr == thislistptr->lastpointptr)
                   thispointptr = NULL;
             else thispointptr = thispointptr->nextptr;
           }
       }/*label output*/
       if(hasdotlist)
       {/*dot output*/
           fprintf(fpout,"@dotlist {dots}%s%s%s%s%s%s%s\n"
             ,colorstr,buttonstr,masterstr,onoffstr
             ,dname,hiliteness,staticness); /*invent list name*/
           thispointptr = thislistptr->firstpointptr;
           while(thispointptr != NULL)
           {
             if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
                &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
             {/*survived pruning*/
               if( (thispointptr->type) & DOT ) /*dot*/
               {/*write dot point*/
                  writeindividualpoint(0); /*iPointFlag==0*/ /*writedrawnew()*/
               }/*write dot point*/
             }/*survived pruning*/
             if(thispointptr == thislistptr->lastpointptr)
                   thispointptr = NULL;
             else thispointptr = thispointptr->nextptr;
           }
       }/*dot output*/
       if(hasarrowlist)
       {/*arrow output*/
         sprintf(dname," radius= %.3f angle= %.3f ",distarrow,anglearrow); 
         /*globals: length of tine, angle of tine,*/
         /*implicit: dihedralarrow==90: 4 tines*/
           fprintf(fpout,"@arrowlist {arrows}%s%s%s%s%s%s%s\n"
             ,colorstr,buttonstr,masterstr,onoffstr
             ,dname,hiliteness,staticness); /*invent list name*/
           thispointptr = thislistptr->firstpointptr;
           while(thispointptr != NULL)
           {
             if(  !((thispointptr->type)  & PRUNED)   /*survived pruning*/
                &&!((thispointptr->STATUS) & GHOST) ) /*not a ghost*/
             {/*survived pruning*/
               if( (thispointptr->type) & ARROW ) /*arrow*/
               {/*write dot point*/
                  writeindividualpoint(0); /*iPointFlag==0*/ /*writedrawnew()*/
               }/*write dot point*/
             }/*survived pruning*/
             if(thispointptr == thislistptr->lastpointptr)
                   thispointptr = NULL;
             else thispointptr = thispointptr->nextptr;
           }
       }/*arrow output*/
       fprintf(fpout,"\n");
       ireturn = 1;
   }/*has vectors or labels or dots or arrows or balls*/
   else
   {
        ireturn = 0;
   }
   /*fclose(fpout);fpout = NULL;*/ 
   /*Don't close here, writedrawnew can be part of output*/
   return(ireturn);
}
/*___writedrawnew()_________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writepruned()***********************************************************/
int    writepruned()    /*960316 viewout stuff*/
{/*output (modified) current kinemage*/
   /*kinescopeout() calls this as part of writing all kins in its file*/

   int       ireturn,i,j,isurvived;
   char      aname[MAXNAMECHAR+1],bname[MAXNAMECHAR+1];
   char      cname[MAXNAMECHAR+1],extraname[MAXNAMECHAR+1];
   char      dname[256]; /*060525 for dimension=7 etc...*/
   char      dname1[16],dname2[16],dname3[16],dname4[16],dname5[16]; 
   char      dNnames[256]; /*060525 for dimension=7 etc...*/
   char      onename[256],newname[256]; /*060726 for dimension=7 etc...*/
   char      hold[256];
   unsigned long long mask; /*081120 long long*/
   float     mx[10];    
   int Lfirsttablegroup = 1; /*until encounter a first table group*/
   int       IONlimit; /*060604 for Lononlyout*/
   int       Lononlyout=0; /*made local to writepruned() as of 061123*/

   if(LPDBformat) /*040208,rev 070829*/
   {
      Lkinformat = 0;
   }
   else
   {
      Lkinformat = 1; /*kinemage rather than PDB format*/
   }

   if(Lvisibleout || Laverageout) /*060604*/
   {
      Lononlyout = 1;
      IONlimit = 0; /*survive for output only if really visibly ON */
   }
   else /*revert Lononlyout 061124*/
   {
      IONlimit = -50;/*survived pickshow deletion, either ON or OFF*/ /*060604*/
      Lononlyout = 0;
   }

   Lpruneactive = 0; /* worry flags*/
   Ldrawactive = 0;
   Lnewviewactive = 0;
   Lnewstuffactive = 0;
   LemphasisOUT = 0; /*031127*/

  if(Lkinformat && !Laverageout) /*040208, 060604*/
  {/*output header stuff*/
   if(beginselectionpointptr != NULL)
   {/*expect selection from begin and end selection*/ 
      Lbeginselectionout = 1; /*flag to write @beginselect*/
   }

   fprintf(fpout,"@kinemage %d\n",thiskinenum);

   writecaption();  /*MACOUT.C  MPCOUT.C*/
   /*fprintf(fpout,"\n");*/ /*ensure next keyword on a new line*/  
   /*990124 remove, writetext and writecaption should ensure final EOL char*/
   if(Lkintitle)   {fprintf(fpout,"@title {%s}\n",kintitlestr);} /*030605*/
   if(Lkinpdbfile) {fprintf(fpout,"@pdbfile {%s}\n",kinpdbfilestr);} /*011010*/
   if(Lkinmapfile) {fprintf(fpout,"@mapfile {%s}\n",kinmapfilestr);} /*030312*/

   /*if(Lspanofview) fprintf(fpout,"@xxxx %.2f\n",spanofview);*/ /*980920*/
   
   if(LLensflagset){fprintf(fpout,"@lens %.2f\n",lensFactor);} /*970917*/
   if(Listcolordominant) {fprintf(fpout,"@listcolordominant\n");} /*970917*/
   if(update_command_line[0] != '\0') 
   {
      fprintf(fpout,"@command {%s}\n",update_command_line);
   }
   else if(input_command_line[0] != '\0') 
   {
      fprintf(fpout,"@command {%s}\n",input_command_line);
   }   
   if(Lcommentinput) /*020917*/
   {
      fprintf(fpout,"@comment {%s}\n",commentinputstr);
   }   
   if(Ldrawnewinput)     {fprintf(fpout,"@drawnew\n");}
   if(Ldrawlineinput)    {fprintf(fpout,"@drawline\n");}
   if(Ldrawlabelinput)   {fprintf(fpout,"@drawlabel\n");}
   if(Ldrawballinput)    {fprintf(fpout,"@drawball\n");}
   if(Lbigfontsinput)    {fprintf(fpout,"@bigfonts\n");}
   if(Lnosavealertinput) {fprintf(fpout,"@nosavealert\n");}
   if(Lshortlineinput)   {fprintf(fpout,"@shortline\n");}
           
   if(Lonewidth) {fprintf(fpout,"@onewidth\n");}
   if(Lthin) {fprintf(fpout,"@thinline\n");}
   if(Lperspec)  {fprintf(fpout,"@perspective\n");}
   if(Lwhitebkg) {fprintf(fpout,"@whitebackground\n");} /*990125*/
   
   if(LNdimnames) /*060726*/
   {  /*construct keyword and its set of dimensions*/
      /*names*/
      dNnames[0] = '\0'; /*trouble with first field 061124,26,...*/
      sprintf(dNnames,"@dimension"); /*to be continued...*/
      for(j=0; j<NDIM; j++)
      {
         sprintf(onename," {%s}",dimensionptr[j].name);
         /*if(sizeof(onename) + sizeof(dNnames) < 255) both 256 */
         {/*character string still in bounds  one hopes since sizeof not work*/
            sprintf(newname,"%s%s",dNnames,onename);
            strcpy(dNnames,newname); 
         }
      }
      sprintf(newname,"%s\n",dNnames); /*tack on LF*/
      strcpy(dNnames,newname); /*bounds above allow LF character*/
      fprintf(fpout,"%s",dNnames);  /*actual output line 061113*/

      /*min max pairs*/
      dNnames[0] = '\0'; /*trouble with first field 061124,26,...*/
      sprintf(dNnames,"@dimension"); /*to be continued...*/
      for(j=0; j<NDIM; j++)
      {
         sprintf(onename," {%s}",dimensionptr[j].name);
         /*if(sizeof(onename) + sizeof(dNnames) < 255) both 256 */
         {/*character string still in bounds  one hopes since sizeof not work*/
            sprintf(newname,"%s%s",dNnames,onename);
            strcpy(dNnames,newname); 
         }
      }
      sprintf(dNnames,"@dimminmax"); /*to be continued...*/
      for(j=0; j<NDIM; j++)
      {
         sprintf(onename," %.3f %.3f",dimensionptr[j].min,dimensionptr[j].max);
         /*if(sizeof(onename) + sizeof(dNnames) < 255) both 256 */
         {/*character string still in bounds  one hopes since sizeof not work*/
            sprintf(newname,"%s%s",dNnames,onename);
            strcpy(dNnames,newname); 
         }
      }
      sprintf(newname,"%s\n",dNnames); /*tack on LF*/
      strcpy(dNnames,newname); /*bounds above allow LF character*/
      fprintf(fpout,"%s",dNnames);  /*actual output line*/
   }

   for(i=1; i< MAXRESETS ;i++)
   {
      if(Lview[i])
      {/*Lview*/
               /*---------------------------------*/
               if(viewcom[i][0] != '\0') /*not null string*/
               {
                  if(i==1) fprintf(fpout,"@viewid {%s}\n",viewcom[i]);
                  else if(i<10) fprintf(fpout,"@%1dviewid {%s}\n",i,viewcom[i]);
                  else fprintf(fpout,"@%2dviewid {%s}\n",i,viewcom[i]);

               }
               if(Lzoomer[i]!=0)/*980929*/
               {
                 if(Lzoomer[i]>0)/*980929*/
                 {
                   if(i==1) fprintf(fpout,"@zoom %.2f\n",zoomold[i]);
                   else if(i<10) fprintf(fpout,"@%1dzoom %.2f\n",i,zoomold[i]);
                   else fprintf(fpout,"@%2dzoom %.2f\n",i,zoomold[i]);
                 }
                 else
                 {/* span of screen from which to calc zoom*/
                   if(i==1) fprintf(fpout,"@span %.2f\n",zoomold[i]);
                   else if(i<10) fprintf(fpout,"@%1dspan %.2f\n",i,zoomold[i]);
                   else fprintf(fpout,"@%2dspan %.2f\n",i,zoomold[i]);
                 }/*980929*/
               }
               /*---------------------------------*/
               if(Lzcliper[i])
               {
                   if(i==1) fprintf(fpout,"@zslab %d\n",izclipold[i]);
                   else if(i<10) fprintf(fpout,"@%1dzslab %d\n"
                                             ,i,izclipold[i]);
                   else fprintf(fpout,"@%2dzslab %d\n",i,izclipold[i]);
               }
               /*----------------------------------*/
                if(Lztraner[i])  /*970909*/
               {
                   if(i==1) fprintf(fpout,"@ztran %d\n",iztranold[i]);
                   else if(i<10) fprintf(fpout,"@%1dztran %d\n"
                                             ,i,iztranold[i]);
                   else fprintf(fpout,"@%2dztran %d\n",i,iztranold[i]);
              }
               /*----------------------------------*/
               if(Lcenter[i])
               {
                   if(i==1) fprintf(fpout,"@center %.3f %.3f %.3f\n"
                              ,fxcenterold[i],fycenterold[i],fzcenterold[i]);
                   else if(i<10) fprintf(fpout,"@%1dcenter %.3f %.3f %.3f\n"
                              ,i,fxcenterold[i],fycenterold[i],fzcenterold[i]);
                   else fprintf(fpout,"@%2dcenter %.3f %.3f %.3f\n"
                              ,i,fxcenterold[i],fycenterold[i],fzcenterold[i]);
               }
               /*----------------------------------*/
               if(    Lgwidthheightview[i] >0                 /*060613*/
                   && gwidthview[i] >0 && gheightview[i] >0)  /*041031*/
               {/*041031 060613*/
                   if(i==1) fprintf(fpout,"@size %d %d\n"
                       ,gwidthview[i],gheightview[i]);
                   else if(i<10) fprintf(fpout,"@%1dsize %d %d\n"
                     ,i,gwidthview[i],gheightview[i]);
                   else fprintf(fpout,"@%2dsize %d %d\n"
                     ,i,gwidthview[i],gheightview[i]);
               }
               /*----------------------------------*/
               if(Laxischoice[i])  /*060613*/
               {  /*axes array indexed from 0, count axes from 1 */
                  if(i==1)
                  {
                     fprintf(fpout,"@axischoice %d %d %d\n"
                     ,1+axischoice[i][0],1+axischoice[i][1],1+axischoice[i][2]);
                  }
                  else if(i<10)
                  { 
                     fprintf(fpout,"@%1daxischoice %d %d %d\n"
                   ,i,1+axischoice[i][0],1+axischoice[i][1],1+axischoice[i][2]);
                  }
                  else
                  {
                     fprintf(fpout,"@%2daxischoice %d %d %d\n"
                   ,i,1+axischoice[i][0],1+axischoice[i][1],1+axischoice[i][2]);
                  }
               }
               /*----------------------------------*/
               if(Lreset[i])
               {
                   j = i;
                   mx[1] =  (float)ma[j][1];
                   mx[2] = -(float)ma[j][2]; /*  1  2  3          1   0   0   */
                   mx[3] =  (float)ma[j][3]; /*  4  5  6     X    0  -1   0   */
                   mx[4] =  (float)ma[j][4]; /*  7  8  9          0   0   1   */
                   mx[5] = -(float)ma[j][5];
                   mx[6] =  (float)ma[j][6]; /* Mac handedness correction :   */
                   mx[7] =  (float)ma[j][7]; /* back correct so written matrix*/
                   mx[8] = -(float)ma[j][8]; /* is correct for input to this, */
                   mx[9] =  (float)ma[j][9]; /* or any program                */

                   if(i==1) fprintf(fpout,"@matrix\n");
                   else if(i<10)fprintf(fpout,"@%1dmatrix\n",i);
                   else         fprintf(fpout,"@%2dmatrix\n",i);
                   fprintf(fpout,"%f %f %f %f %f %f %f %f %f\n",
                      mx[1],mx[2],mx[3],mx[4],mx[5],mx[6],mx[7],mx[8],mx[9]);
               }
               /*---------------------------------*/
      }/*Lview*/
   }
   if(firstmasterptr != NULL) /*there are masters*/ /*970513*/
   {
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         fprintf(fpout,"@master {%s}\n",thismasterptr->name);          
         thismasterptr = thismasterptr->nextptr;
      }
      /*scan through again looking for pointmasters*/  /*991210*/
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         if(thismasterptr->masterchar != '\0')
         {
            if(thismasterptr->on) {sprintf(word," on");} /*030425*/
            else                  {sprintf(word," off");} /*030425*/
            fprintf(fpout,"@pointmaster '%c' {%s}%s\n"
               ,thismasterptr->masterchar,thismasterptr->name,word);
         }          
         thismasterptr = thismasterptr->nextptr;
      }

   }
   if(firstcolorsetptr != NULL) /*there are colorsets*/ /*991104*/
   {
      thiscolorsetptr = firstcolorsetptr;
      while(thiscolorsetptr != NULL)
      {
         interpretcolor(thiscolorsetptr->color); /*returns in word*/
         fprintf(fpout,"@colorset {%s} %s\n",thiscolorsetptr->name,word);          
         thiscolorsetptr = thiscolorsetptr->nextptr;
      }
   }

   if(naspectsthiskinemage > 0) /*there are aspects*/ /*031227*/
   {
      for(i=1; i<naspectsthiskinemage+1; i++)
      {
         fprintf(fpout,"@%daspect {%s}\n",i,aspect[i]);          
      }
   }

  }/*output header stuff*/
     thisgrupptr = firstgrupptr;
     while(thisgrupptr != NULL)
     {/*BIG loop-over-groups*/
      isurvived = 0; /*set to NOT survive, must be persuaded to live*/

      /*check for survival of self and at least one child*/
      if(thisgrupptr->on > IONlimit) /*0606040*/
      {/*for this group: group survived pickshow*/
        thissgrpptr = thisgrupptr->firstsgrpptr;
        while(thissgrpptr != NULL) 
        {/*for this group: test for subgroup survival*/
          if(thissgrpptr->on > IONlimit) /*060604*/
          {/*for this group: subgroup survived pickshow*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*for this group: test for list survival*/
              if(thislistptr->on > IONlimit) /*060604*/
              {/*for this group: list survived pickshow*/
                   thispointptr = thislistptr->firstpointptr;
                   while(thispointptr != NULL)
                   {/*for this group: test for point survival*/
                      /*since a surviving point is the ultimate and only true*/
                      /*survival test, check for bond rotation lists here*/
                      /*some of them have no points but all must be kept*/
                      if(  !((thispointptr->type) & PRUNED) /*survived pruning*/
                         || (thislistptr->bondrot != 0)    )/*bondrot list pt*/
                      {/*for this group: point survived pruning, etc.*/
                        if(   !Lononlyout 
                           || (Lononlyout && thispointptr->STATUS & ON) )
                        {
                          isurvived = 1;
                        }
                      }/*for this group: point survived pruning*/
                      if(thispointptr == thislistptr->lastpointptr)
                           thispointptr = NULL;
                      else thispointptr = thispointptr->nextptr;
                   }/*for this group: test for point survival*/
              }/*for this group: list survived pickshow*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
              else thislistptr = thislistptr->nextptr;
            }/*for this group: test for list survival*/
          }/*for this group: subgroup survived pickshow*/
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
          else thissgrpptr = thissgrpptr->nextptr;
        }/*for this group: test for subgroup survival*/

       /*for this group: check for groups not to be included in a kinemage*/
       if(thisgrupptr==markergrupptr ||thisgrupptr==measuregrupptr)isurvived =0;
       if(Lononlyout && thisgrupptr->on == 0) isurvived = 0; 
          /*off: don't output*/
      }/*for this group: group survived pickshow*/

      if(isurvived)
      {/*BIG this group survived*/
       if(Lkinformat && !Laverageout) /*040208,060604*/
       {/*this group: output first line*/

        /*actually, before the first line of the first table group*/
        if( (thisgrupptr->type & TABLEFLAG) == TABLEFLAG)
        {/*table group*/
           if(Lfirsttablegroup)
           {
              fprintf(fpout,"@tablewrap %d\n",tablewrapcol);   
              fprintf(fpout,"@tablecolscale %.2f\n",tablecolscale);   
              fprintf(fpout,"@tablefontsize %d\n",tablefontsize);        
              fprintf(fpout,"@tablemarkradius %.2f\n",tablemarkradius); 
              fprintf(fpout,"@tablefanradius %.2f\n",tablefanradius);
              fprintf(fpout,"@tablefancore %.2f\n",tablefancore);       
              if(Ltableblackbkg)
              {
                fprintf(fpout,"@tableblackbkg\n");   
              }
              Lfirsttablegroup = 0;
              if( (thisgrupptr->type & TABLECOLHEAD) == TABLECOLHEAD)
              {
                 sprintf(extraname," table header");
              }
              else
              {
                 sprintf(extraname," table");
              }
           }
           else
           {
              sprintf(extraname," table");
           }
        }/*table group*/
        else extraname[0] = '\0';
        
        /*dominance and button stuff for a group*/
        temps[0]='\0';
        if(thisgrupptr->STATUS & RECESSIVEON)
        {/*has to override dominant and nobutton anyway*/
           sprintf(temps," recessiveon");
        }
        else if(thisgrupptr->STATUS & NOBUTTON)
        {
           if(thisgrupptr->STATUS & DOMINANT)
           {/*both*/
               sprintf(temps," nobutton dominant");
           }
           else
           {/*just nobutton*/
              sprintf(temps," nobutton");
           }
        }
        else if(thisgrupptr->STATUS & DOMINANT)
        {/*just dominant*/
           sprintf(temps," dominant");
        }
        
        if(thisgrupptr->instanceptr != NULL)/*this instances an earlier group*/
        {/*this group: instances an earlier group*/
             for(j=0;j<(MAXNAMECHAR);j++)   
                 cname[j]=(thisgrupptr->instanceptr)->name[j+1];
             cname[(MAXNAMECHAR)]='\0';/*force C string end*/ 
             sprintf(bname," instance= {%s}",cname);
        }/*this group: instances an earlier group*/
        else bname[0]='\0';
      /*else sprintf(bname," ");*/

      /*sprintf(numstr," ");*/ /*clear numstr to hold place for masters*/
        numstr[0]='\0'; /*clear numstr to hold place for masters*/
        if(thisgrupptr->master > 0) /*master button for this one*/
        {/*this group: has a master button*/
            thismasterptr = firstmasterptr;
            while(thismasterptr != NULL)
            {
                mask = thismasterptr->mask;
                /*mask = 1<<(i-1);*/ /*see ajustmaster() in MAGEBBOX.c*/
                if((thisgrupptr->master)&mask)
                {
                    sprintf(hold,"%s master= {%s}",numstr,thismasterptr->name);
                    sprintf(numstr,"%s",hold); /* combined -> numstr*/
                }
                thismasterptr = thismasterptr->nextptr;
            }
        }/*this group: has a master button*/

        if     (thisgrupptr->animate == 2) sprintf(word," 2animate");
        else if(thisgrupptr->animate == 1) sprintf(word," animate");
        else if(thisgrupptr->on == 0) sprintf(word," off");
        else word[0]='\0';
      /*else                          sprintf(word," ");*/
            /*animate group can't be arbitrarily off */
        
        if(thisgrupptr->moview > 0) 
            sprintf(cname," moview= %d",thisgrupptr->moview);
        else cname[0]='\0';
      /*else  sprintf(cname," ");*/

        /*animated groups are on/off controlled by MAGE input */
        /*groupnames are given a leading space on input*/
        /*shift groupname left one space*/
        /*this also removes asterisk at beginning of animated group name*/
        for(j=0;j<(MAXNAMECHAR);j++)   {aname[j]=thisgrupptr->name[j+1];}
        aname[(MAXNAMECHAR)]='\0';/*force C string end*/    

        dname[0]='\0'; dname1[0]='\0';
        dname2[0]='\0';dname3[0]='\0';dname4[0]='\0';dname5[0]='\0';
        if(thisgrupptr->KKIND & DIMENSIONKKIND) /*060525*/
           {  sprintf(dname1," dimension=%d ",thisgrupptr->NDIM); }
        if(thisgrupptr->KKIND & WRAPPINGKKIND) /*060525*/
           {  sprintf(dname2," wrap=%.0f ",thisgrupptr->fwrap); }
        if(thisgrupptr->KKIND & SELECTINGKKIND) /*060525*/
           {  sprintf(dname3," select"); }
        if(thisgrupptr->KKIND & AVERAGEKKIND) /*061129*/
           {  sprintf(dname4," average"); }
        if(thisgrupptr->KKIND & SIGMASKKIND) /*061129*/
           {  sprintf(dname5," sigma"); }
        sprintf(dname," %s%s%s%s%s",dname1,dname2,dname3,dname4,dname5);

        fprintf(fpout,"@group {%s}%s%s%s%s%s%s%s\n"
           ,aname,extraname,temps,numstr,bname,word,cname,dname); 
           /*990407 was bname,numstr*/ /*060525 dname */
           /*MAGE input problem if instance (bname) before master (numstr)*/

            /*always check these, docking can be eposodic during a run*/
            if(thisgrupptr->pdbfilestr[0] != '\0')
            {
               fprintf(fpout,"@grouppdbfile {%s}\n",thisgrupptr->pdbfilestr);
            }
            if(  (  thisgrupptr->pointx[0] >  (float)1.0001 
                  ||thisgrupptr->pointx[0] <  (float)0.9999)
               ||(  thisgrupptr->pointx[1] >  (float)0.0001 
                  ||thisgrupptr->pointx[1] < -(float)0.0001)
               ||(  thisgrupptr->pointx[2] >  (float)0.0001 
                  ||thisgrupptr->pointx[2] < -(float)0.0001)
                  
               ||(  thisgrupptr->pointy[0] >  (float)0.0001 
                  ||thisgrupptr->pointy[0] < -(float)0.0001)
               ||(  thisgrupptr->pointy[1] >  (float)1.0001 
                  ||thisgrupptr->pointy[1] <  (float)0.9999)
               ||(  thisgrupptr->pointy[2] >  (float)0.0001 
                  ||thisgrupptr->pointy[2] < -(float)0.0001)
                  
               ||(  thisgrupptr->pointz[0] >  (float)0.0001 
                  ||thisgrupptr->pointz[0] < -(float)0.0001)
               ||(  thisgrupptr->pointz[1] >  (float)0.0001 
                  ||thisgrupptr->pointz[1] < -(float)0.0001)
               ||(  thisgrupptr->pointz[2] >  (float)1.0001 
                  ||thisgrupptr->pointz[2] <  (float)0.9999)

               ||(  thisgrupptr->position[0]  >  (float)0.0001 
                  ||thisgrupptr->position[0]  < -(float)0.0001)
               ||(  thisgrupptr->position[1] >  (float)0.0001 
                  ||thisgrupptr->position[1] < -(float)0.0001)
               ||(  thisgrupptr->position[2] >  (float)0.0001 
                  ||thisgrupptr->position[2] < -(float)0.0001) )
            {
               fprintf(fpout,"@gnomon\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  " %.3f %.3f %.3f\n"
                  ,thisgrupptr->pointx[0]
                  ,thisgrupptr->pointx[1]
                  ,thisgrupptr->pointx[2]
                  ,thisgrupptr->pointy[0]
                  ,thisgrupptr->pointy[1]
                  ,thisgrupptr->pointy[2]
                  ,thisgrupptr->pointz[0]
                  ,thisgrupptr->pointz[1]
                  ,thisgrupptr->pointz[2]
                  ,thisgrupptr->position[0]
                  ,thisgrupptr->position[1]
                  ,thisgrupptr->position[2]
                  );
            }

       }/*this group: output first line*/
        
        if(thisgrupptr->instanceptr == NULL) 
        {/*BIG this group: not instancing earlier group*/
         /*so can have children of its own*/
         thissgrpptr = thisgrupptr->firstsgrpptr;
         while(thissgrpptr != NULL)
         {/*BIG loop-over-subgroups*/
          isurvived = 0; /*set to NOT survive, must be persuaded to live*/

          /*this subgroup: check for self and at least one child survival*/
          if(thissgrpptr->on > IONlimit) /*survived pickshow deletion*/
          {/*this subgroup: survived pickshow deletion*/
           thislistptr = thissgrpptr->firstlistptr;
           while(thislistptr != NULL)
           {/*for this subgroup: test for list survival*/
              if(thislistptr->on > IONlimit) /*survived pickshow deletion*/
              {/*for this subgroup: list survived pickshow*/
                   thispointptr = thislistptr->firstpointptr;
                   while(thispointptr != NULL)
                   {/*for this subgroup: test for point survival*/
                       /*since a surviving point is the ultimate and only true*/
                      /*survival test, check for bond rotation lists here*/
                      /*some of them have no points but all must be kept*/
                      if(  !((thispointptr->type) & PRUNED) /*survived pruning*/
                         || (thislistptr->bondrot != 0)    )/*bondrot list pt*/
                       {/*for this subgroup: point survived pruning*/
                        if(   !Lononlyout 
                           || (Lononlyout && thispointptr->STATUS & ON) )
                        {
                          isurvived = 1;
                        }
                       }/*for this subgroup: point survived pruning*/  
                       if(thispointptr == thislistptr->lastpointptr)
                           thispointptr = NULL;
                       else thispointptr = thispointptr->nextptr;
                   }/*for this subgroup: test for point survival*/
              }/*for this subgroup: list survived pickshow*/
              if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
              else thislistptr = thislistptr->nextptr;
           }/*for this subgroup: test for list survival*/
           /*for this subgroup: check for subgroups not to be in a kinemage*/
           if(thisgrupptr == drawgrupptr)
              if(thissgrpptr == drawmarkersgrpptr) isurvived=0;
                  /*this subgroup is just the little draw markers*/
           if(Lononlyout && thissgrpptr->on == 0)isurvived=0;/*off: not output*/
          }/*this subgroup: survived pickshow deletion*/
           
           
           
          if(isurvived)
          {/*BIG this subgroup survived*/

          if(Lkinformat && !Laverageout) /*040208,060604*/
          {/*this subgroup: output first line*/
            if((thissgrpptr->STATUS & NEVERSHOW) == 0) 
            {/*explicitly name subgroup in kinemage display list*/
              
              /*dominance and button stuff for a subgroup*/
              temps[0]='\0';
              if(thissgrpptr->STATUS & RECESSIVEON)
              {/*has to override dominant and nobutton anyway*/
                 sprintf(temps," recessiveon");
              }
              else if(thissgrpptr->STATUS & NOBUTTON)
              {
                 if(thissgrpptr->STATUS & DOMINANT)
                 {/*both*/
                     sprintf(temps," nobutton dominant");
                 }
                 else
                 {/*just nobutton*/
                    sprintf(temps," nobutton");

                 }
              }
              else if(thissgrpptr->STATUS & DOMINANT)
              {/*just dominant*/
                 sprintf(temps," dominant");
              }
              
              if(thissgrpptr->instanceptr!= NULL) /*this instances earlier set*/
              {
                 sprintf(bname," instance= {%s}"
                     ,(thissgrpptr->instanceptr)->name);
              }
              else bname[0]='\0';
            /*else sprintf(bname," ");*/

            /*sprintf(numstr," ");*/ /*clear numstr to hold place for masters*/
              numstr[0]='\0'; /*clear numstr to hold place for masters*/
              if(thissgrpptr->master > 0) /*master button for this one*/
              {/*this subgroup has a master button*/
                  thismasterptr = firstmasterptr;
                  while(thismasterptr != NULL)
                  {
                      mask= thismasterptr->mask;
                      /*mask = 1<<(i-1);*/ /*see ajustmaster() in MAGEBBOX.c*/
                      if((thissgrpptr->master)&mask)
                      {
                         sprintf(hold,"%s master= {%s}"
                             ,numstr,thismasterptr->name);
                         sprintf(numstr,"%s",hold); /* combined -> numstr*/
                      }
                      thismasterptr = thismasterptr->nextptr;
                  }
              }/*this subgroup has a master button*/
              
            /*if(thissgrpptr->on > 0) sprintf(KineName," ");*/
              if(thissgrpptr->on > 0) KineName[0]='\0';
              else                    sprintf(KineName," off");

              fprintf(fpout,"@subgroup {%s}%s%s%s%s\n"
                ,thissgrpptr->name,temps,numstr,bname,KineName); 
                /*990407 was bname,numstr*/

            }/*explicitly name subgroup in kinemage display list*/
            
          }/*this subgroup: output first line*/
            
          if(thissgrpptr->instanceptr == NULL)            
          {/*BIG this subgroup: not instancing earlier subgroup*/
            /*so can have children of its own*/

            if(thisgrupptr != drawgrupptr)
            {/*MOD this subgroup: not in the draw-new group*/
             thislistptr = thissgrpptr->firstlistptr;
             while(thislistptr != NULL)
             {/*MOD for this subgroup: loop-over-lists*/
              isurvived = 0;
              
                         
              /*check for survival of self and at least one child*/
              if(thislistptr->on > IONlimit) 
              {/*for this list: does at least one point survive?*/
                   if(thislistptr->firstpointptr==NULL
                      && thislistptr->bondrot == -1)
                   {/*dummy list to end bondrot scope*/
                       isurvived = 1;
                   }/*dummy list to end bondrot scope*/

                   thispointptr = thislistptr->firstpointptr;
                   while(thispointptr != NULL)
                   {/*for this list: check point survival*/
                       /*since a surviving point is the ultimate and only true*/
                      /*survival test, check for bond rotation lists here*/
                      /*some of them have no points but all must be kept*/
                      if(  !((thispointptr->type) & PRUNED) /*survived pruning*/
                         || (thislistptr->bondrot != 0)    )/*bondrot list pt*/
                      {/*for this list: a point survived pruning*/ 
                        if(   !Lononlyout 
                           || (Lononlyout && thispointptr->STATUS & ON) )
                        {
                          isurvived = 1;
                        }
                      }/*for this list: a point survived pruning*/   
                      if(thispointptr == thislistptr->lastpointptr)
                         thispointptr = NULL;
                      else thispointptr = thispointptr->nextptr;
                   }/*for this list: check point survival*/
                   /*for this list: check if not to be in a kinemage*/
                   if(Lononlyout && thislistptr->on == 0) isurvived = 0; 
              }/*for this list: does at least one point survive?*/

              if(isurvived)
              {/*MOD this list survived*/
                  /*Lkinformat control moved to beginning, 040208*/
                  writeindividuallist(); /*MAGEOUT.c*/
              }/*MOD this list survived*/

              if(thislistptr == thissgrpptr->lastlistptr) thislistptr = NULL;
              else thislistptr = thislistptr->nextptr;
             }/*MOD for this subgroup: loop-over-lists*/

            }/*MOD this subgroup: not in the draw-new group*/
            else
            {/*MOD this subgroup: in draw-new group, process list differently*/
              if( writedrawnew() ==1)
              {
                ;
              }
              else
              {
                sprintf(alertstr,"failed writing draw_new group to modified kinemage\n");
                dosinglealert(0);
              }
            }/*MOD this subgroup: in draw-new group, process list differently*/
            
           }/*BIG this subgroup: not instancing earlier subgroup*/
          }/*BIG this subgroup survived*/
          if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr = NULL;
          else thissgrpptr = thissgrpptr->nextptr;
         }/*BIG loop-over-subgroups*/
        }/*BIG this group: not instancing earlier group*/
      }/*BIG this group survived*/
      thisgrupptr = thisgrupptr->nextptr;
    }/*BIG loop-over-groups*/
    ireturn = 1;
    Lpruneactive = 0;/*so know that modified kinemage saved*/
    /* fprintf(fpout,"\n"); */
    /* fclose(fpout); fpout = NULL;*/
    LemphasisOUT = 0; /*031127*/
    return(ireturn);
}
/*___writepruned()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****writeindividuallist()***************************************************/
void writeindividuallist(void)
{
  int    igoout,k;                   
  long   i11,i12,i13,i21,i22,i23,i31,i32,i33;
  int    ixset,iyset,izset;
  int    ifudgx,ifudgy,ifudgz,idelx,idely,idelz,itemp;
  int    iPointFlag; /*signels new start point for truncated polyline*/
  static  int    nscope=0,newnscope=0,nscopecount=0; 
     /*for conveying samescope%d information*/
  char      bname[26],dname[32],hiliteness[32],tempstr[32];
  char      ename[26],fname[26],gname[26],staticness[26],detailness[26];
  char      widthamt[26]; /*000412*/
  char      lensness[12],nozclipness[12],plotness[12],scopeness[12];
  char      sliderness[12]; /*070702*/
  char      ptIDstr[256];
  int       ipad=0; /*for padding ptIDstr  040208*/
  char      hold[256];
  
  char      keyword[21]; /*MAGEINPT limits keywords to 20 char max*/ /*990124*/
  unsigned long long mask; /*081120 long long*/

  char atmname[5]; /*070829 use to test for hydrogens in rotated output*/

  if(Lvisibleout) /*specific to Lvisibleout*/
  {/*recalculates current view, only outputs what is in sight*/
     /* establish integer array,  8192* for reasonable percision 2**13 */
     i11=(long)(8192*a11); i12=(long)(8192*a12); i13=(long)(8192*a13);
     i21=(long)(8192*a21); i22=(long)(8192*a22); i23=(long)(8192*a23);
     i31=(long)(8192*a31); i32=(long)(8192*a32); i33=(long)(8192*a33);

     ixset = ixctr; 
     iyset = iyctr;
     izset = izctr;
  
     if(Lztran) izset = (izctr + iztran);
     iyset = iyset + iytran;
     iPointFlag = 0;
  }/*recalculates current view, only outputs what is in sight*/
  
  /*kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk*/
  if(Lkinformat && !Laverageout)             
  {/*Lkinformat: this list: output first line*/
     
     interpretcolor(thislistptr->color);
         /* returns color in string word */
         /*but don't put out a color=  unless this instance really */
         /*had its own color= originally!*/
         /*So, rewrite string word appropriately */
     if(thislistptr->owncolor != 0)/*960528*/
     {
        if(thislistptr->colorset)
        {/*list color is determined by a named colorset*/
           thiscolorsetptr = firstcolorsetptr;
           while(thiscolorsetptr!=NULL)
           {
              if(thiscolorsetptr->number == thislistptr->colorset)
              {/*found its colorset*/
                 /*this list defines color as name of colorset*/
                 sprintf(temps," color= {%s}",thiscolorsetptr->name);
                 sprintf(word,"%s",temps);
                 break; /*only get one color designation per list*/
              }
              thiscolorsetptr = thiscolorsetptr->nextptr;
           }
        }
        else
        {/*list color is actually set explicitly*/
           sprintf(temps," color= %s",word);
           sprintf(word,"%s",temps);
        }
     }
     else word[0]='\0';

     if( (thislistptr->STATUS & STATICFLAG) == STATICFLAG)
     {
         sprintf(staticness," static");
         if( (thislistptr->STATUS & SCREENFLAG) == SCREENFLAG)
         {/*variant of staticness: */  /*0020208*/
            sprintf(staticness," static screen");
         }
     }
     else staticness[0]='\0';
     /*Thus screen is a more rigorous form of static */
     /*but in any case, they are incompatible types */

     hiliteness[0] = '\0'; /*clear*/
     if( (thislistptr->STATUS & NOHILITEFLAG) == NOHILITEFLAG)
     {
         sprintf(tempstr," nohilite");
     }
     else tempstr[0]='\0';
     if( (thislistptr->STATUS & NOBLKRIMFLAG) == NOBLKRIMFLAG) /*040925*/
     {
         strcat(tempstr," noblkrim");
     }
     sprintf(hiliteness,"%s",tempstr);

     if( (thislistptr->type & DETAILFLAG) == DETAILFLAG)
     {
         sprintf(detailness," detail");
     }
     else detailness[0]='\0';
 
     if( (thislistptr->type & LENSFLAG) == LENSFLAG)
     {
         sprintf(lensness," lens");
     }
     else lensness[0]='\0';
     
     if( (thislistptr->type & NOZCLIPFLAG) == NOZCLIPFLAG)
     {
         sprintf(nozclipness," nozclip");
     }
     else nozclipness[0]='\0';
     
     if(thislistptr->STATUS & NOBUTTON) sprintf(temps," nobutton");
     else temps[0]='\0';
     
     if(thislistptr->instanceptr != NULL) 
     {/*this instances earlier set*/
          /*recondition(listname[ilist[nlist][7]]);*/
                               
          sprintf(bname," instance= {%s}"
                        ,thislistptr->instanceptr->name);
      }
     else bname[0]='\0';
     
     numstr[0]='\0'; /*clear numstr to hold place for masters*/
     if(thislistptr->master > 0) 
     {/*this list has a master button*/
         thismasterptr = firstmasterptr;
         while(thismasterptr != NULL)
         {
             mask = thismasterptr->mask;
             /*mask = 1<<(i-1);*/ /*see ajustmaster() in MAGEBBOX.c*/
             if((thislistptr->master)&mask)
             {
                sprintf(hold,"%s master= {%s}"
                    ,numstr,thismasterptr->name);
                sprintf(numstr,"%s",hold); /* combined -> numstr*/
             }
             thismasterptr = thismasterptr->nextptr;
         }
     }/*this list has a master button*/
     
     if(thislistptr->on > 0) KineName[0]='\0';
     else                    sprintf(KineName," off");

     /*radiuslist[nlist]  float property associated with this list*/

     /* angle    also a   float property associated with this list*/
     if(   thislistptr->radius > 0.001 
        &&(thislistptr->angle > 0.001 || thislistptr->angle < -0.001) )
     {/*only allow positive values of radius*/
         sprintf(dname," radius= %.3f angle= %.3f"
                ,thislistptr->radius,thislistptr->angle);
     }
     else if(thislistptr->radius > 0.001)
     {/*only allow positive values of radius*/
         sprintf(dname," radius= %.3f",thislistptr->radius);
     }
     else if(thislistptr->angle > 0.001 || thislistptr->angle < -0.001 )
     {
         sprintf(dname," angle= %.3f",thislistptr->angle);
     }
     else dname[0]='\0';
     
     if(thislistptr->width > 0 )
     {/*only allow positive values now*/
         sprintf(widthamt," width= %d",thislistptr->width);
     }
     else widthamt[0]='\0';
     
     /*output of the @----list first line*/
     /*if the scope of a rotation selection starts with the first point */
     /* of a vectorlist, the @beginselection should be entered before*/
     /*the vectors of the list*/
     
     if(Lbeginselectionout && !Ldockingscope) /*030312 NOT do docking scope*/
     {
         if(thislistptr->firstpointptr == beginselectionpointptr)
         {
             fprintf(fpout,"@beginselect\n");
             Lbeginselectionout = 0; /*only do this once*/
         }
     }

     /*Now set up First Line for each type of list*/     /*990124*/
     if(    (   ((thislistptr->type) & VECTOR  )
             &&!((thislistptr->type) & VARIANT1)
             &&!((thislistptr->type) & VARIANT2)
             &&!((thislistptr->type) & VARIANT3) )
        ||  (   ((thislistptr->type) & DOT     )
             &&!((thislistptr->type) & VARIANT1) )
       )
     {/*actual plain VECTOR list, or a plain DOT list*/ /*990124*/
       if((thislistptr->type) & VECTOR) strcpy(keyword,"@vectorlist ");
       else  strcpy(keyword,"@dotlist ");

       /* only a vectorlist or a dotlist can specify a rotation*/
       if(   (thislistptr->bondrot <= nbondrot) 
          && (thislistptr->bondrot >  0       ) )
       {/*this one does specify a rotation*/
         if( (bondrotptr[thislistptr->bondrot])->option > 0 )
         {
             if     ( (bondrotptr[thislistptr->bondrot])->option == 1 )
                sprintf(gname,"parameter"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 2 )
                sprintf(gname,"precession"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 3 )
                sprintf(gname,"selection"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 4 )
                sprintf(gname,"xrotation"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 5 )
                sprintf(gname,"yrotation"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 6 )
                sprintf(gname,"zrotation"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 7 )
                sprintf(gname,"xtranslation"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 8 )
                sprintf(gname,"ytranslation"); 
             else if( (bondrotptr[thislistptr->bondrot])->option == 9 )
                sprintf(gname,"ztranslation"); 
             else
                gname[0]='\0';
         }
         else
         {
              gname[0]='\0';
         }
         
         if     (thislistptr->bondrot == LbondrotHplot) 
              {sprintf(plotness,"hplot");}
         else if(thislistptr->bondrot == LbondrotVplot) 
              {sprintf(plotness,"vplot");}
         else if(thislistptr->bondrot == LbondrotZplot) 
              {sprintf(plotness,"zplot");}
         else {plotness[0]='\0';}

         if((bondrotptr[thislistptr->bondrot])->slider == 0)  /*070702*/
              {sprintf(sliderness," noslider");} /*070702*/
         else {sliderness[0] = '\0';}

         if(nscope == thislistptr->bondrot)
         {/*previous bondrots use this ones scope, this one gets marked*/
             sprintf(scopeness,"samescope%d",nscopecount);
             nscopecount = 0; /*reset*/
             nscope = 0;
         }
         else scopeness[0] = '\0';
            
         if((newnscope = (bondrotptr[thislistptr->bondrot]->scopeinstance)) > 0)
         {/*refers to later bondrot for scope of rotation*/
             if(newnscope == nscope) ; /*first of a set determines nscopecount*/
             else if(nscope ==0)
             {/*set up for later bondrot to declare samescope*/
                 nscopecount = newnscope - thislistptr->bondrot + 1;
                 nscope = newnscope;
             }
             /*((nscope > 0 && newnscope != nscope)) PROBABLY AN ERROR*/
         }
         if(bondrotptr[thislistptr->bondrot]->level < 0) /*020310*/
         {/*ganged rotation linked by list name, NOTE naked bondrot*/
          fprintf(fpout,"%s{%s}%s%s%s%s%s%s%s%s%s%s%s%s bondrot %.3f %s%s%s%s\n"
           ,keyword,thislistptr->name,word,temps,numstr,bname,KineName
           ,dname,widthamt,hiliteness,staticness,detailness,lensness,nozclipness
           ,bondrotptr[thislistptr->bondrot]->angle,gname,plotness,scopeness
           ,sliderness);  /*070702*/
         }
         else
         {
        fprintf(fpout,"%s{%s}%s%s%s%s%s%s%s%s%s%s%s%s %dbondrot %.3f %s%s%s%s\n"
           ,keyword,thislistptr->name,word,temps,numstr,bname,KineName
           ,dname,widthamt,hiliteness,staticness,detailness,lensness,nozclipness
           ,bondrotptr[thislistptr->bondrot]->level
           ,bondrotptr[thislistptr->bondrot]->angle,gname,plotness,scopeness
           ,sliderness);  /*070702*/
         }

       }/*this one does specify a rotation*/
       else if(thislistptr->bondrot <  0) /*-1 for 0bondrot*/
       /*else if(thislistptr->bondrot -42)*/ /*-42 for 0bondrot*/ /*020310*/
       {
        fprintf(fpout,"%s{%s}%s%s%s%s%s%s%s%s%s%s%s%s nobutton 0bondrot 0.0\n"
           ,keyword,thislistptr->name,word,temps,numstr,bname,KineName,dname
           ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);  
       }
       else 
       {
        fprintf(fpout,"%s{%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
           ,keyword,thislistptr->name,word,temps,numstr,bname,KineName,dname
           ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
       }
     }/*actual plain VECTOR list, or a plain DOT list*/
     else if(   ((thislistptr->type) & VECTOR  )
             && ((thislistptr->type) & VARIANT1)
             &&!((thislistptr->type) & VARIANT2)
            )
     {/*actual TRIANGLE list*/ 
        fprintf(fpout,"@trianglelist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
           ,thislistptr->name,word,temps,numstr,bname,KineName,dname
           ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual TRIANGLE list*/ 
     else if(   ((thislistptr->type) & VECTOR  )
             && ((thislistptr->type) & VARIANT1)
             && ((thislistptr->type) & VARIANT2)
            )
     {/*actual RIBBON list*/ 
        fprintf(fpout,"@ribbonlist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
           ,thislistptr->name,word,temps,numstr,bname,KineName,dname
           ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual RIBBON list*/ 
     else if(   ((thislistptr->type) & VECTOR  )
             && ((thislistptr->type) & VARIANT3)
            )
     {/*actual ARROW list*/  /*000804*/
        fprintf(fpout,"@arrowlist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
           ,thislistptr->name,word,temps,numstr,bname,KineName,dname
           ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual ARROW list*/ 

     /*990209 de-blot*/
     else if(   ((thislistptr->type) & BALL    )
             &&!((thislistptr->type) & VARIANT1)
            )  /* balllist */
     {/*actual BALL list*/ 
       fprintf(fpout,"@balllist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual BALL list*/
     else if(   ((thislistptr->type) & BALL    )
             && ((thislistptr->type) & VARIANT1)
            )  /* spherelist */
     {/*actual SPHERE list*/
       fprintf(fpout,"@spherelist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual SPHERE list*/
     else if(   ((thislistptr->type) & LABEL   )
            )  /* labellist  if radius given, radius output*/
     {/*actual LABEL list*/
       fprintf(fpout,"@labellist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
     }/*actual LABEL list*/
     else if(   ((thislistptr->type) & MARKTYPE   )
            )  /* marklist  if radius given, radius output*/
     {/*one of the MARKTYPE list*/
        if(   ((thislistptr->STYLE) & RINGSTYLE   )
            )  /* ringlist  if radius given, radius output*/
        {/*actual RINGSTYLE list*/
           fprintf(fpout,"@ringlist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
        }/*actual RINGSTYLE list*/
        else if(   ((thislistptr->STYLE) & FANSTYLE   )
            )  /* ringlist  if radius given, radius output*/
        {/*actual FANSTYLE list*/
           fprintf(fpout,"@fanlist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
        }/*actual FANSTYLE list*/
        else
        {/*actual plain MARKTYPE list*/
           fprintf(fpout,"@marklist {%s}%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName,dname
            ,widthamt,hiliteness,staticness,detailness,lensness,nozclipness);
        }/*actual plain MARKTYPE list*/
     }/*one of the MARKTYPE list*/
     else if(   ((thislistptr->type) & WORDS    )
            )  
     {/*actual WORDS list*/
       
       if(thislistptr->wordsfont == 0) ename[0]='\0';
       else sprintf(ename," font= %d",thislistptr->wordsfont);
       if(thislistptr->wordsface == 0) fname[0]='\0';
       else sprintf(fname," face= %d",thislistptr->wordsface);
       if(thislistptr->wordssize == 0) gname[0]='\0';
       else sprintf(gname," size= %d",thislistptr->wordssize);
       
       fprintf(fpout,"@wordlist {%s}%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n"
            ,thislistptr->name,word,temps,numstr,bname,KineName
            ,dname,widthamt,ename,fname,gname,hiliteness,staticness
            ,detailness,lensness,nozclipness);   /*970917*/
     }/*actual WORDS list*/
  }/*Lkinformat: this list: output first line*/
  /*kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk*/
     
  if( thislistptr->instanceptr == 0 )/*list not an instance */
  {/*this list has points to output*/
      if(LrotoutSORTED && thislistptr->KKIND & ALTERNATEKKIND)
      {
        strcpy((char*)OUTRalternate[thislistptr->resnumb],thislistptr->resname);

      }
      thispointptr = thislistptr->firstpointptr;
      while(thispointptr != NULL)
      {/*loop over point triples*/ 
         igoout = 0; /*look for exceptions*/
         if( !((thispointptr->type) & PRUNED) ) /*survived pruning*/
         {
             igoout = 1;
             if( ( (thispointptr->type) & INVENTED ) )
               igoout=0;
                /*extra, shortened vector internal to MAGE*/
         }
         if(   (Lvisibleout || Laverageout) /*060207,061123 replace Lononlyout*/
            && !(thispointptr->STATUS & ON) )
         {
            igoout = 0;
         }
         /*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
         if(Lvisibleout && igoout) /*specific to Lvisibleout*/
         {/*Lvisibleout*/

             igoout = 0; /*presume reject, accept if in view*/
if( (thislistptr->STATUS & STATICFLAG)==0)
{
 jx2 = (  (thispointptr->ix*i11 +thispointptr->iy*i21 +thispointptr->iz*i31)
       >>13)+ixset;
 jy2 = (  (thispointptr->ix*i12 +thispointptr->iy*i22 +thispointptr->iz*i32)
       >>13)+iyset;
 jz2 = (  (thispointptr->ix*i13 +thispointptr->iy*i23 +thispointptr->iz*i33)
       >>13)+izset;
         /*shift by 13 bits rather than divide by 8192 */
}
else
{
    jx2 =  thispointptr->ix + ixset;
    jy2 = -thispointptr->iy + iyset;
    jz2 =  thispointptr->iz + izset;
}
         if(Lperspec)
         {
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
         }
         LOK = 0; /*flag for passing clip checks */
         /*checksideclip();*/ /*always do this, it is relatively cheap*/
         /*if(jx2 >= xmin && jx2 <= xmax && jy2 >= ymin && jy2 <= ymax)*/
         /*060525 xmin,xmax changed by stereo, but visible only is mono */
         /*060525 so revert back to orig, pre-stereo, 0 -- WIDTH */
         if(jx2 >= 0 && jx2 <= WIDTH && jy2 >= 0 && jy2 <= HEIGHT) /*060525*/
         {
            LOK = 1;
         }
         if(Lzclipon && LOK == 1) 
         {  /* pass altered coord values*/
             /*checkzclip(jx1,jy1,jz1,jx2,jy2,jz2); */
             LOK = 0;
             if(jz2 >= (izctr - izclip) && jz2 <= (izctr + izclip) ) LOK = 1;
         }
         if(LOK == 1)
         {/*survived clipping*/
             igoout = 1; 
         }   
         else 
         {/*clipped point*/
             igoout = 0;
             iPointFlag = 1; /*next point must restart a polyline*/
         }
         }/*Lvisibleout*/
         /*vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/         
         
         if(igoout)
         {/*survived exceptions, write point to output*/
          /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

          if(Lkinformat && !Laverageout)  /*060604*/
          {/*Lkinformat with Mickey Mouse stuff to get most compressed output*/
             writeindividualpoint(iPointFlag); /*writeindividuallist()*/

             iPointFlag = 0; /*allow next point to be continuing polyline*/

             if(endselectionpointptr != NULL && !Ldockingscope) /*030312 NOT do docking scope*/
             {
                if(thispointptr == endselectionpointptr)
                {/*write keyword after the point in the kinemage file*/
                   fprintf(fpout,"@endselect\n");
                   endselectionpointptr = NULL; /*only do this once*/
                }
             }
          }/*Lkinformat with Mickey Mouse stuff to get most compressed output*/

          /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
          
          /*PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUTPUT*/
          /*  in writeindividuallist()  */
          /*LPDBformat is for general PDB format instead of kinemage format*/
          /* NOT for PBD format version of write rotated atoms only, */
          /*  writerotated just does atoms uniquely drawn-to in vectorlists*/

          else if(  (LPDBformat && (  ((thispointptr->type)&DOT)   /*040208*/
                                    ||((thispointptr->type)&BALL)
                                   )
                    ) 
                 || (     ((thispointptr->type)&VECTOR  ) 
                      && !((thispointptr->type)&MOVETO_P)
                      && !((thispointptr->type)&INVENTED)
                      && !((thispointptr->STATUS)&GHOST)
                    )
                 )
          {/*BRK PDB format:fish out unique and complete set of DrawTo Vectors*/
             ptIDstr[0] = '\0'; /*use ptIDstr[], initialize*/
             getptIDstring(ptIDstr, thispointptr);

             /*ptID in ptIDstr supposed to be in PDB format */
             /*for(k=0;ptIDstr[k]!='\0';k++) ptIDstr[k]=toupper(ptIDstr[k]);*/
             ipad = 0;
             for(k=0;k<15;k++)
             {
                if(ptIDstr[k]=='\0'){ipad = 1;}
                if(ipad == 1)
                {
                   ptIDstr[k]=' ';
                }
                else
                {
                   ptIDstr[k]=toupper(ptIDstr[k]);
                }
             }
             for(k=0;k<4;k++)
             {
                atmname[k] = ptIDstr[k];
             }
             atmname[4] = '\0';

             /*ptIDstr should start with a5,a3,1x,a1,i4,a1 == 15 characters*/
             /*the preceding 1x and the next 3x is part of fprintf format*/
             ptIDstr[15] = '\0'; /*force truncation to just PDB format info*/
             /*this allows ptID to carry extra info after that for kinemage*/
             /* B==33.33 default since Mage doesn't track this info  050407*/
             /*occupancy f6.2, Bvalue f6.2                  occupaBvalue...*/
             /*cols:        12345678901...                  567890123456...*/
#ifdef OLDCODE
            if(LPDBformat || (  (ipass==1 && ptIDstr[1]!='H')    /*040208*/
                              ||(ipass==2 && ptIDstr[1]=='H') ) )
#else       /* isahydrogen(atmname) 070829*/

            if(   (LPDBformat) /*the general case PDB instead of kin format*/
                  /* OR called by writerotated() */
               || (   npass==2 /*only if Lhydrogensout && LhydrogensSep*/
                   &&(   (ipass==1 && !isahydrogen(atmname) ) 
                      || (ipass==2 &&  isahydrogen(atmname) ) 
                     ) 
                  )
               || ( npass==1 && Lhydrogensout && isahydrogen(atmname) )
               || ( npass==1 && !isahydrogen(atmname) )
              )
#endif
              /*NOTE: We are in writeindividuallist() and */
              /* Lhydrogensout && LhydrogensSep come from writerotated() */
              /*at end reset Lhydrogensout && LhydrogensSep for probe output*/
            {
               if(LrotoutSORTED) /*070901, 071005 ATOM  number*/
               { 
                /*sprintf(temps,"ATOM      1 %s   %8.3f%8.3f%8.3f  1.0  33.33*/
                  sprintf(temps,"ATOM  %5lu %s   %8.3f%8.3f%8.3f  1.0  33.33\n"
                          ,thispointptr->number /*121108 unsigned long NOT int*/
                          ,ptIDstr
                          ,thispointptr->fx,thispointptr->fy,thispointptr->fz);
                  putonetextblockline(&rotoutscratch,temps);
               }
               else
               {/*usual flow*/
                fprintf(fpout,"ATOM      1 %s   %8.3f%8.3f%8.3f  1.0  33.33\n"
                          ,ptIDstr
                          ,thispointptr->fx,thispointptr->fy,thispointptr->fz);
/*name[n],&ncnt[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,i5,1x,a5,a3,1x,a1,i4,a1,3x,3f8.3)*/
/*
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89
*/ 
               }
               if(Ldumpprobe)
               {
                 fprintf(stderr,"ATOM      1 %s   %8.3f%8.3f%8.3f  1.0  33.33\n"
                          ,ptIDstr
                          ,thispointptr->fx,thispointptr->fy,thispointptr->fz);
               }
            }
          }/*BRK PDB format:fish out unique and complete set of DrawTo Vectors*/
          /*PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUT_PDB_OUTPUT*/

          else if(  (Laverageout && (  ((thispointptr->type)&DOT)   /*060604*/
                                     ||((thispointptr->type)&BALL)
                                   )
                    )
                 || (     ((thispointptr->type)&VECTOR  )
                      && !((thispointptr->type)&MOVETO_P)
                      && !((thispointptr->type)&INVENTED)
                      && !((thispointptr->STATUS)&GHOST)
                    )
                 )
          {
             if(Npassout < 2)
             {/*first pass: accummulate sums for averages*/
                accumsums();
             }
             else
             {/*second pass: accummulate deviations from averages*/
                accumdevi();
             }
          }
         }/*survived exceptions, write point to output*/

         if(thispointptr == thislistptr->lastpointptr)
             thispointptr = NULL;
         else thispointptr = thispointptr->nextptr;

      }/*loop over point triples*/
  }/*this list has points to output*/
}
/*___writeindividuallist()___________________________________________________*/

/****writeindividualpoint()****************************************************/
void writeindividualpoint(int iPointFlag)
{
  char ptIDstr[256];
  char hold[256];
  char char1,char2,char3,char4,charset[32];
  char aname[MAXNAMECHAR+1];
  char bname[MAXNAMECHAR+1]; 
  /*bname alternates with aname to get clean accummulation  051120*/
  char noxyzstr[]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  int  i,k,n;
  int  Ltablepoint = 0, Lnoxyz = 0;
  char asp[MAXPOINTASPECTS+5]; /*aspect string inc (,),trailing space,\0*/
  char pointstuff[256]; /*060207 separate point's x,y,z from other-point-stuff*/
   
  for(i=0; i<(MAXNAMECHAR+1); i++) {aname[i]='\0'; bname[i]='\0';}
           /*enforce aname, bname clearance since text is accummulated in it*/

   /*EMPHASIS emphasis Lemphasis scope carried in point STYLE */
   if((thispointptr->STYLE) & EMPHASIS) /*031127 output*/
   {
      if(LemphasisOUT == 0)
      {/*entering emphasis region*/
         LemphasisOUT = 1;
         fprintf(fpout,"@beginemphasis\n");                         
      }
   }
   else
   {
      if(LemphasisOUT)
      {/*leaving emphasis region*/
         LemphasisOUT = 0;
         fprintf(fpout,"@endemphasis\n");
      }
   }



   if( thispointptr->listptr != NULL)
   {
      if((thispointptr->listptr)->sgrpptr != NULL)
      {
         if(((thispointptr->listptr)->sgrpptr)->grupptr != NULL)
         {
            if(((((thispointptr->listptr)->sgrpptr)->grupptr)->type & TABLEFLAG)
               == TABLEFLAG)
            {
               Ltablepoint = 1;
            }
         }
      }
   }
  
   /*ditto " for efficient ptID in kinemage file*/ 
   /*this gets pretty hairy for shortened vectors with balllists*/
   /*so can't just use thispointptr->previousptr, but have to */
   /* remember the last valid output point's pointID */
   ptIDstr[0] = '\0'; /*use ptIDstr[], initialize*/ 
   getptIDstring(ptIDstr, thispointptr);

   if(!LNOditto) /*051120*/ /*060207 always force LNOditto==1*/
   {/*ditto identical pointIDs*/
      if(thispointptr == (thispointptr->listptr)->firstpointptr )
      {/*first point in a list, make this pointID a reference*/
         strcpy(hold,ptIDstr); /*copy:   2 <- 1 */
      }/*first point in a list, make this pointID a reference*/
      else
      {/*not the first point in a list*/
         /*compare with previously outputed pointID held in hold */ 
         if( (strcmp(ptIDstr,hold)==0) && (hold[0] != '\0')) /*971001*/
         {/*non-NULL pointID same as previous pointID*/ /*971001*/
            ptIDstr[0] = '\"';
            ptIDstr[1] = '\0';
         }
         else
         {/*NOT the same, remember for next point*/
            strcpy(hold,ptIDstr); /*copy:   2 <- 1 */
         }
      }
   }/*ditto identical pointIDs*/

   if(   (thispointptr->type) & MOVETO_P) 
        {char1 = 'P';} /* valid MOVE: P Point: needed! */
   else {char1 = ' ';} /*L Line or elsething*/
   if(Lvisibleout && iPointFlag) {char1 ='P';} /*after clipped pt,Lvisibleout*/
   if(  (thispointptr->type) & UNPICKABLE) 
        {char2 = 'U';} /*U Unpickable*/
   else {char2 = ' ';}  

   /*Ball, Sphere, Arrow, Ring, Square(Mark) mutually exclusive*/
   /*set the explicit flag on the point if different than listtype*/
   /*NOTE: pointtype inherits listtype then | point explicit flags*/
   /*multiple byte flags need completely specific tests*/

   if(     (thispointptr->type) & BALLHERE) /*BALLHERE just point type*/
     /*(|| (thispointptr->type) & BALL)*/ 
     /*BALL point type 051120, set from list type, NOT B marked point*/
       {char3 = 'B';} /*B Ball explicit*/
   else if(  (((thispointptr->type)&SPHERE)==SPHERE)
           &&(((thislistptr->type )&SPHERE)!=SPHERE) ) 
       {char3 = 'S';} /*S Sphere */
   else if(  (((thispointptr->type)&ARROW)==ARROW)
           &&(((thislistptr->type )&ARROW)!=ARROW) ) 
       {char3 = 'A';} /*A Arrow */
   else if(  (((thispointptr->type)&MARKTYPE)==MARKTYPE)
           &&(((thislistptr->type )&MARKTYPE)!=MARKTYPE) )
   {
          if((thispointptr->STYLE) & RINGSTYLE)
             {char3 = 'R';} /*R Ring */
          else if((thispointptr->STYLE) & FANSTYLE)
             {char3 = ' ';} /*Fan is only a list type */
          else
             {char3 = 'Q';} /*Q sQuare Mark*/
   }
   else  {char3 = ' ';}

   if(((thispointptr->type)&TRIANGLE)==TRIANGLE) 
         {char4 = 'T';} /*point is part of a triangle*/
   else  {char4 = ' ';} 

   /*avoid space between bracket and P: don't k++ before charset[k]= */
   charset[0] = '\0'; /*don't need space between bracket and number*/
   for(k=1;k<32;k++) {charset[k] = '\0';} /*force all new entries*/
   k=0;

   if(char1 == 'P') 
      {charset[k++] =char1;charset[k++]=' ';} /*do need space after each */
   if(char2 == 'U') 
      {charset[k++] =char2;charset[k++]=' ';} /*single letter flag*/
   if(char3 != ' ') /*B,S,A,R,Q*/ 
      {charset[k++] =char3;charset[k++]=' ';}
   if(char4 == 'T') 
      {charset[k++] =char4;charset[k++]=' ';}

   if( (thispointptr->master) > 0 ) /*030528*/
   {/*this point has pointmaster*/
      charset[k++]='\'';
      /*scan through masters looking for pointmasters*/
      thismasterptr = firstmasterptr;
      while(thismasterptr != NULL)
      {
         if( (thispointptr->master)&(thismasterptr->mask) )
         {/*retrieve pointmaster character*/
            charset[k++] =thismasterptr->masterchar;
         }/*retrieve pointmaster character*/
         thismasterptr = thismasterptr->nextptr;
      }
      charset[k++]='\'';
      charset[k++]=' ';
   }/*this point has pointmaster*/

   if(thispointptr->aspects[0] != '\0')
   {/*point has aspects*/
      asp[0] = '(';
      for(i=1; i<MAXPOINTASPECTS+1; i++)
      {
         if(thispointptr->aspects[i-1] == ' ') break;
         asp[i] = thispointptr->aspects[i-1]; 
      }
      asp[i] = ')'; 
      asp[i+1] = ' '; /*trailing space*/
      asp[i+2] = '\0';
   }
   else
   {
      asp[0] = '\0';
   }

   /*960810*/
   /*coded in thispointptr->colorwidth*/
   /* 2 byte integer: "colordepthwidthtype"
   extra types:     8,     9,    10,    11,    12,    13,    14,    15
                16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720

   - 32768
   - 16384  8 point type code (shift >>11, mask 15) 4 bit new usage: 000804
   -  8192  4 point type code (shift >>11, mask  7) old 3 bit codes:
   -  4096  2 "    1,     2,     3,     4,     5,     6,     7
   -  2048  1 "  2048,  4096,  6144,  8192, 10240, 12288, 14336
   -  1024  4 pen width code (shift >>8, mask 7)
   -   512  2 "
   -   256  1 "
   -   128  4 depth cue code (shift >>5, mask 7)
   -    64  2 "    1    2    3    4    5    6    7
   -    32  1 "   32,  64,  96, 128, 160, 192, 224
   -    16 color code (mask 31)
   -    8 "
   -    4 "
   -    2 "
   -    1 "
   */
           word[0] = '\0';/*initialize*/ /*960812*/
           aname[0] = '\0';/*initialize*/ /*960812*/
           bname[0] = '\0';/*initialize*/ /*051120this should not be necessary*/
           if(thispointptr->colorwidth != 0) /*960810*/
           {/*decode color_depth_width_type */
      if( (thispointptr->colorwidth & 31)!= 0)
      {/*explicite color for this point*/
          interpretcolor((thispointptr->colorwidth & 31));
              /*MAGECOLR.c*/
          /*color name string put into global "word[]"*/ 
      }/*explicite color for this point*/

      
      if( (thispointptr->colorwidth>>8 & 7)!= 0)
      {/*explicit pen width for this point*/
          k = (thispointptr->colorwidth>>8) & 7;/*971014 debug*/
          if      (k==1) sprintf(aname,"width1");
          else if (k==2) sprintf(aname,"width2");
          else if (k==3) sprintf(aname,"width3");
          else if (k==4) sprintf(aname,"width4");
          else if (k==5) sprintf(aname,"width5");
          else if (k==6) sprintf(aname,"width6");
          else if (k==7) sprintf(aname,"width7");
      }/*explicit pen width for this point*/

      
      /*960810 eplicit depth_cue and explicit type*/
      /*NOT coded at input, not allowed for in output*/
      /*Note, however, that write draw_new_parts does*/
      /*separate out constructed dots!  971014*/
      
   }/*decode color_depth_width_type */

   if(thispointptr->radius > 0.001)  /*980817*/
   {/*point radius*/
      sprintf(bname,"%s r=%-.2f",aname,thispointptr->radius);
      strcpy(aname,bname); /*051120*/
   }/*point radius*/ /*980817*/

   if( (thispointptr->STATUS & DUMMY) == DUMMY)
   {/*dummy point*/  /*991203*/
      if(Ltablepoint)
      {/*means has no functional x,y,z and doesn't need "dummy" param*/
         Lnoxyz = 1;
         sprintf(noxyzstr," tablecell");
      }
      else
      {/*explicit "dummy" parameter*/
        sprintf(bname," dummy %s",aname);
        strcpy(aname,bname); /*051120*/
      }
   }/*dummy point*/
           
   if( (thispointptr->STATUS & NOXYZ) == NOXYZ)
   {/*no functional x,y,z coord for this point*/  
      Lnoxyz = 1;
      if(Ltablepoint)
      {
         sprintf(noxyzstr," tablecell");
      }
      else
      {
         sprintf(noxyzstr," noxyz");
      }
   }/*no functional x,y,z coord for this point*/  

   if( (thispointptr->STATUS & GHOST) == GHOST)
   {/*seen but not real for some purposes (like PDB output)*/  
      sprintf(bname," ghost %s",aname);
      strcpy(aname,bname); /*051120*/
   }/*seen but not real for some purposes (like PDB output)*/  
           
   /*"ptIDstr[]" has ptID*/
   /*"temps[]" gets <words comment> */
   /*"charset[]" has P/L, U, B codes*/
   /*"word[]" has color name if given explicitly for point*/
   /*"aname[]" has pen width if given explicitly for point*/

   if(thispointptr->wordoffset != 0)
   {/*has wordstring, get it*/
      getcommentstring(commentstr, thispointptr);
      for(n=3;commentstr[n]!='\0';n++)
          temps[n-3]=commentstr[n];
      /*temps[n-2]='\0'; but n-3 is not yet defined 960228*/
      temps[n-3]='\0';
   }/*has wordstring, get it*/
   else
   {
       temps[0] = '\0';
   }
   if(   temps[0] != '\0' 
      || Ltablepoint     /*table cells always get {} <>...*/
      || Lnoxyz )        /*point with no xyz should have something*/
   {/*write with <word>, presume ptID as well*/
      if(word[0]!='\0'||aname[0]!='\0')
      {/*has some explicit point properties*/
         sprintf(pointstuff,"{%s} <%s> %s%s%s %s"
          ,ptIDstr,temps,charset,asp,aname,word);
      }
      else
      {
         sprintf(pointstuff,"{%s} <%s> %s%s"
          ,ptIDstr,temps,charset,asp);
      }
   }/*write with <word>, presume ptID as well*/
   else if(ptIDstr[0] != '\0')
   {/*write point with ptID*/
      if(word[0]!='\0'||aname[0]!='\0')
      {/*has some explicit point properties*/
        sprintf(pointstuff,"{%s}%s%s%s %s"
         ,ptIDstr,charset,asp,aname,word);
      }/*has some explicit point properties*/
      else
      {
        sprintf(pointstuff,"{%s}%s%s" 
         ,ptIDstr,charset,asp);
      }
   }/*write point with ptID*/
   else
   {/*write point with no ptID, minimal designation*/
      if(word[0]!='\0'||aname[0]!='\0')
      {/*has some explicit point properties*/
        sprintf(pointstuff,"%s%s%s %s"
         ,charset,asp,aname,word);
      }/*has some explicit point properties*/
      else
      {
        sprintf(pointstuff,"%s%s"
         ,charset,asp);
      }
   }/*write point with no ptID, minimal designation*/
          
   /*now actually write the point with appropriate coordinates to fpout*/
   if(Lnoxyz)
   {
      fprintf(fpout,"%s %s\n",pointstuff,noxyzstr);
   }
   else if(  ((thispointptr->listptr)->NDIM > 3)  /*060622*/
           &&(thispointptr->coordNptr != NULL)  ) /*061028*/
   {/*output all multi-dimension coordinates, not just current 3*/
      fprintf(fpout,"%s",pointstuff);

      for(i=0; i<(thispointptr->listptr)->NDIM; i++)
      {
         fprintf(fpout," %.3f",thispointptr->coordNptr[i]); /*061028*/
      }

      fprintf(fpout,"\n"); /*end the point's line*/
   }
   else
   {
      fprintf(fpout,"%s, %.3f, %.3f, %.3f\n"  /*101225 magekinjas*/
        ,pointstuff
        ,thispointptr->fx,thispointptr->fy,thispointptr->fz);
   }
}
/*___writeindividualpoint()__________________________________________________*/

/****inventoutputname()*******************************************************/
void inventoutputname( int kind, int k, int Lnameflag)
{
   /* construct output file name from input file name */
   /* add 2,3,or 4 char: .new to input file name */
   /* or add or increment .#.kin */
   /*or replace last .___ with .new*/
   /* no matter what that name is like. new==vw,ksp,kqa,kin,...*/
   int i=0,j=0,n=0,kk=0,m=0;
   int maxuniquelength=0; /*never more than MAXUSERCHAR, */
       /*<= MAXQCHAR if 1st answer used to hold user name*/
   int stamplength = 0; /*length for time stamp */
   int namelength = 0; /*length of name that goes along with time stamp */
   int maxuniquefield = 28;
#ifdef MACINTOSH
   maxuniquefield = 16;
#endif

   if(kind == 14)
   {/*HTML help file*/
      sprintf(TestStr,"magehelp.html");
      k = 8; /*index of last . in TestStr*/
   }
   for(i=0;i<256;i++)
   {/*copy test file name to make start of output file name*/
      OutStr[i] = TestStr[i];
      if(OutStr[i] == '\0') break;
   }
   /*modify output file name*/
   kk = 4; /*expect 3 char + \0 , trailing \0 put on later !!!! */
   if     (kind==1){sprintf(word,"save current view as:");
      OutStr[k+1]='v';OutStr[k+2]='w';OutStr[k+3]='\0';}
   else if(kind==2){sprintf(word,"save current picked point as:");
      OutStr[k+1]='f';OutStr[k+2]='p';OutStr[k+3]='t';}
   else if(kind==3){sprintf(word,"save newly drawn things as:");
      OutStr[k+1]='n';OutStr[k+2]='e';OutStr[k+3]='w';}
   else if(kind==4){sprintf(word,"save rotated stuff as:");
      OutStr[k+1]='r';OutStr[k+2]='o';OutStr[k+3]='t';}
   else if(kind==5 || kind==18) /*060604 18: means and sigmas kin*/
   {/*modified kin*/
      /*usual save whole kinemage as*/
      if(kind==18){sprintf(word,"write means & sigmas to:");}
      else {sprintf(word,"write modified kinemage to:");}
      /*k indexes either last . in orig name or position just past last char*/
      OutStr[k]='.'; /*in all cases a . goes here*/
      OutStr[k+1]='k';OutStr[k+2]='i';OutStr[k+3]='n'; /*030310 not kip*/
      if(   (TestStr[k  ] == OutStr[k  ])
         && (TestStr[k+1] == OutStr[k+1])
         && (TestStr[k+2] == OutStr[k+2])
         && (TestStr[k+3] == OutStr[k+3]) )
      {/*possible duplicate names, they must be different*/
         /*work back into name, try to add, or add to, .#.kin*/
         if(k >= 3 && (TestStr[k-2] == '.'))
         {/*presumptive xxx.#.kin from earlier kin name construction*/
            /*simplest case:  K.X.kin with k==3 */  
            /*k-2 and k both .  Try to increment intervening character*/
            if(TestStr[k-1] < '1')
            {/*something wierd, substitute 1 */
               OutStr[k-1] = '1';
               kk = 4; /*char count of to-be-added \0 (unchanged) */
            }
            else if(TestStr[k-1] >= '1' && TestStr[k-1] < '9')
            {/*safely between 1 and 9, just increment the digit*/
               OutStr[k-1] = OutStr[k-1] + 1;
               kk = 4; /*char count of to-be-added \0 (unchanged) */
            }
            else if(TestStr[k-1] == '9')
            {/*run out of digits, go alphabetic*/
               OutStr[k-1] = 'a';
               kk = 4; /*char count of to-be-added \0 (unchanged) */
            }
            else if(TestStr[k-1] >= 'a' && TestStr[k-1] < 'z') 
            {/*safely within alphabet, just increment the character*/
               OutStr[k-1] = OutStr[k-1] + 1;
               kk = 4; /*char count of to-be-added \0 (unchanged) */
            }
            else
            {/*either reached z or some wierd .?. sequence, add onto end*/
               OutStr[k] = '.';OutStr[k+1]='1';OutStr[k+2]='.';
               OutStr[k+3]='k';OutStr[k+4]='i';OutStr[k+5]='n';  
               kk = 6; /*char count of to-be-added \0 (2 more) */
            }
         }
         else
         {/*orig name ended in .kin but had no preceeding version number*/
            /*add version number, simplest case: K.kin becomes K.1.kin */
            /*pathological case .K.kin becomes continued pathological .K.1.kin*/
            /*pathological case ..kin  becomes continued pathological ..1.kin*/
            /*k should always be >= 1 from calling routine */
            OutStr[k] = '.';OutStr[k+1]='1';OutStr[k+2]='.';
            OutStr[k+3]='k';OutStr[k+4]='i';OutStr[k+5]='n';
            kk = 6; /*char count of to-be-added \0 (2 more) */
         }
       
      }/*possible duplicate names, they must be different*/
   }/*modified kin*/
   else if(kind==6){sprintf(word,"current view as postscript file:");
       OutStr[k+1]='e';OutStr[k+2]='p';OutStr[k+3]='s';}
   else if(kind==7){sprintf(word,"current Answers to file:");
       OutStr[k+1]='k';OutStr[k+2]='q';OutStr[k+3]='a';}
   else if(kind==8){sprintf(word,"current view as raster3Drender file:");
       OutStr[k+1]='r';OutStr[k+2]='3';OutStr[k+3]='d';}
   else if(kind==9){sprintf(word,"current view as POV-Ray render file:");
       OutStr[k+1]='p';OutStr[k+2]='o';OutStr[k+3]='v';}
   else if(kind==10){sprintf(word,"current pointID to file:");
       OutStr[k+1]='p';OutStr[k+2]='i';OutStr[k+3]='d';}
   else if(kind==11){sprintf(word,"current palette to file:");
       OutStr[k+1]='p';OutStr[k+2]='l';OutStr[k+3]='t';}
   else if(kind==14)
   {
                     sprintf(word,"help to html file:");
       OutStr[k+1]='h';OutStr[k+2]='t';OutStr[k+3]='m';
#ifndef PCWINDOWS /*MS Windows really likes its old 3-char extents*/
       OutStr[k+4]='l'; kk = 6; /*make room for 4 char extent*/
#endif
   }
   else if(kind==15||kind==17)
   {/*timed-test results with possible student name and unique identifier*/
      /*Mac restricts file name to 31 characters, so accepting this limitation*/
       /*use up to 10 characters of the original file name*/
       /*a spacer, then up to 12 characters of the user name or identifier*/
       /*a spacer, then k, then up to 2 characters for the kinemage number*/
       /*a spacer, then .kin*/
       /*Linux originally used MAXFILESTR as an upper limit*/
       sprintf(word,"write test answer kinemage to:");
       if(k > 10) {k=10;} /*only take 0-9 char of orig file name, */
       /*k==one past end, normally expected to contain a "." */
       if(usernamestr[0] != '\0') /*use of usernamestr limited to MAXUSERCHAR*/
       {/*presume user name is stored in usernamestr*/
       }
       else if(thisanswer[1][0] != '\0') /*thisanswer limited to MAXQCHAR*/
       {/*presume user name is stored in answer to question #1*/
          /*copy into usernamestr for output info*/
          for(m=0;m<MAXQCHAR-1;m++)
          {
             usernamestr[m] = thisanswer[1][m];
             if(thisanswer[1][m]=='\0') break;
          }
       }

       i = 0; /*no characters for separate user name*/
       j = 0; /*no chars for a kinemage number: maybe multiple kins in file*/
       n = 0; /*number of characters in partial-name plus time-stamp */
        
       /*even in Mac case, partial name along with timestamp should be unique*/
       OutStr[k+i+j] = '_'; /*separator before time stamp*/
       j++;
       sprintf(numstr,"%ld",theclocktickcount());
       stamplength = strlen(numstr);
       namelength = strlen(usernamestr); /*length all available char of name*/
       /*Use: name + stamp <= maxuniquefield   */
       if(stamplength < maxuniquefield)
       {/*room for at least one char of usernamestr*/
          if(namelength < maxuniquefield - stamplength)
          {/*user name is shorter than max available space*/
             /*room for a spacer between user name and time stamp*/
             /*so add white space to user name: (char[128] so should be room)*/
             usernamestr[namelength] = ' ';
             namelength++;
             usernamestr[namelength] = '\0';
             maxuniquelength = namelength + stamplength;
          }
          else
          {/*user name longer than, or an exact fit to, available space*/
            namelength = maxuniquefield - stamplength; /*only use first part*/
            maxuniquelength = maxuniquefield;
          }
       }
       else
       {/*time stamp characters use all available space*/
          namelength = 0;
          maxuniquelength = maxuniquefield;
       }
       m = stamplength-1; /*orig index of last char of number*/
          /*index 1 less than actual length*/
       /*in unexpected case where stamplength > maxuniquelength: m > n  */
       for(n=maxuniquelength-1; n>=0&&m>=0; n--,m--)
       {
          OutStr[k+i+j+n] = numstr[m];
       }
       if(namelength > 0)
       {/*insert first part of user name, decrementing n*/
          for(n=namelength-1;n>=0;n--)
          {
             if(isalpha(usernamestr[n]) || isdigit(usernamestr[n]) )
                  {OutStr[k+i+j+n] = usernamestr[n];}
             else {OutStr[k+i+j+n] = '_';}
          }
       }
       /*adjust n such that now index the \0 character at string end*/
       n = maxuniquelength; /*redefine n*/
       kk = i+j+n; /*redefine kk for all this extra stuff*/
       /*time stamp*/
       
       /*put on a .kin extent*/
       OutStr[k+i+j+n]='.';
       OutStr[k+i+j+n+1]='k';OutStr[k+i+j+n+2]='i';OutStr[k+i+j+n+3]='n';
       OutStr[k+i+j+n+4]='\0';
       kk = i+j+n+4; /*redefine kk for all this extra stuff plus .kin*/
       
   }/*timed-test results with possible student name and unique identifier*/
   else 
   { 
      sprintf(word,"no known kind of output %d",kind);
      OutStr[k+1]='k';OutStr[k+2]='x';OutStr[k+3]='x';
   }

   OutStr[k+kk] = '\0'; /*computed end of output file name string*/

   if(Lnameflag)
   {/*trial output name maybe too long for modification, should edit the name*/
       /*copy OutStr in what ever form it now has back into TestStr*/
       strcpy(TestStr,OutStr);/*later filter against idenity*/
   }
}
/*___inventoutputname()______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****kinescopeout()***********************************************************/
int     kinescopeout()
{
  /*scope out the entire kinemage file, except text and modified current kin*/
  /*then closes input file and redefines output file to be the current input*/

  int     c,flowID;  /* switch controls for input */
  int  Lkine, Lnew; /* Logicals for input flow */ 
  int  Lscopeout = 0; /*presume false until @kinemage # appropriate*/
  char  buff[256];
  int   ibuff;
  enum{ neutID = 1, keywID, kineID };     
  int   ireturn = 1; /*presume successful*/
  int  Lwrotecurrentkinemage = 0; /*020814*/

  if(!Lrotformat) /*040208*/
  {
     writetext();  /*to fpout, the current output file */ /*____OUT.c*/
  }

 /*origkinenum = -99 default for keeping track of kins in a file*/ 
 /*kinemagenum = 0 default for kinemage about to be read in*/ 
 /*Lorigkinfile tracks changes to input file */ 
 if(!Lorigkinfile || Lrotformat) /*040208*/ 
 {/*nolonger have original file open so can only write this one*/
   if(Lkinfile==1 || Lkinfile==3)
   {
      /*valid inputted kinemage sets Lkinfile==1*/
      /*internal kinemage sets Lkinfile==3*/
      writepruned(); /*output current kinemage*/
      Lwrotecurrentkinemage = 1;
      Lscopeout = 0; /*chug through file image of current kin*/
   }
   else
   {/*Lkinfile==2*/
     ;/*now just get text as output if Lkinfile == 2*/
   }
 } 
 /*if(Lorigkinfile && !(origkinenum==thiskinenum && atEOF) )*/
 /*more than current kinemage to write out*/
 else   /*Lorigkinfile*/
 {/*original kin file is available to write out, even if only 1 kinemage*/ 
   fpin = fp; /*scan primary kinemage file for the other kins*/
   filerewind(fpin); 
   atEOF = 0;

   Lnextkine = 0;
   LnextOK = 1;
   word[0] = '\0';
   iword = 0;
   flowID = neutID;
   while (!atEOF)
   {/*loop while file not at EOF */
     /* #define getnextchar()  getc(fpin), fpin == fp here*/
     /* flow controls and logicals together direct the flow */
     c = getnextchar();
     if(c==EOF) atEOF = 1;
            
     if(Lscopeout) buff[ibuff++] = c; /*buffer for output*/
 
     if (c == '\n' || c == '\r' || (Lscopeout && ibuff >= 254) )
     {/*char is end of line or pushing buffer limit*/
        if(Lscopeout && (ibuff > 0))
        {
           if(buff[0]=='@'&&ibuff >= 8 &&(strncmp(buff,"@kinemage",9)==0)) 
           {/*keyword starts with @: char after \n or \r, check for kinemage*/
              ; /*NOP: don't pass this to output*/
           }
           else
           {/*output this string*/
              buff[ibuff] = '\0';
              fprintf(fpout,"%s",buff);
              buff[0] = '\0';
              ibuff = 0;
           }
        }
        /*c = '\015';*/
        /* whatever newline is, it needs to be CR in text */
     }/*char is end of line or pushing buffer limit*/
     if (c == '@') 
     {
        flowID = keywID; /* need to get begin's key word */
        iword = 0;  /* set keyw character counter */
     }
     switch(flowID)
     {/*switch on flowID*/
         case neutID:  
          /* neutral: no flow designated, no further work on characters */
         break;
                    
         case keywID: 
          /*try to interpret next set of characters as a key wor*/
          /*@keyword must be contiguous characters as a single word*/
          if(c == '@')
          {
             ;  /* @ discarded */
          }
          else
          {/* this routine only looks for kinemage to end */
             iword++;
             word[iword] = c;
             word[iword] = tolower(word[iword]);  
                           /*case independent*/
             if( (c == ' ')||(c == '\015')||(c == ',' )||
                 (c == ';')||(iword >=  8)||(c == '\n')  )
             {/* stop looking when hit white space or 8 char max */
                /* pad ends so can treat word as a real string */
                word[iword+1] = '\0';
                word[0] = ' ';
                           
                /* try to interpret as a keyword string */
                /* search for kinemage as key word */
                Lkine = 0;
                if(strncmp(word," kinemage",9) == 0) Lkine =1; /*MAGEINPT*/
                if(Lkine) 
                {
                   flowID = kineID; 
                   Lnew = 1; /* flag start of scan for kinemage number */
                }
                else
                {
                   flowID = neutID; /* clear flowID */
                }
                iword = 0;
             }/* stop looking when hit white space or 8 char max */
          }/* this routine only looks for kinemage to end */
         break; /* case = keywID */
                
         case kineID: 
           /*when done, getkinemagenum returns as 1 */
           if( getkinemagenum((char)c,Lnew) ) 
           {
              if(kinemagenum == thiskinenum)
              {
                 writepruned(); /*output current kinemage*/
                 Lwrotecurrentkinemage = 1;
                   /* found desired number */
                 Lscopeout = 0; /*chug through file image of current kin*/
              }
              else if(   kinemagenum >= nextkinenum
                      || kinemagenum < thiskinenum  )
              {/*before or next past current kin,  scope out from file*/  
                 Lscopeout = 1;
                 fprintf(fpout,"@kinemage %d\n",kinemagenum);
                 buff[0] = '\0';
                 ibuff = 0;
              }
              flowID = neutID;
           }
           Lnew = 0;
         break;
     }/*switch on flowID*/
   }/*loop while file not at EOF */
   if(!Lwrotecurrentkinemage) /*020814*/
   {/*NOT yet output the current kinemage, might as well try to*/
      writepruned(); /*output current kinemage*/
      Lwrotecurrentkinemage = 1;
   }
 }/*original kin file is available to write out, even if only 1 kinemage*/
 makecursoractive();
 IOerr = 0;
 atEOF = 1;
 fprintf(fpout,"\n"); /*Keep end clean*/
 fclose(fpout);
 fpout = NULL;
 return(ireturn);
}
/*___kinescopeout()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****routeoutput()************************************************************/
int routeoutput(int kind)
{
   /*NameStr only used when replacing full kinemage file*/
   int ireturn = 1; /*presume successful*/
   int i = 0; /*counter 060604*/
   
   LNOditto = 1; /* 1 always write full pointIDs, no ditto for repeats 051120*/
     /*060207 always force ==1, economy not now worth editing difficulties*/

   if(kind == 1) 
   {
      writematrix(); /*MAGEOUT.C*/ /*this now writes all view params*/
      Lnewviewactive = 0;  /*971129*/
   }
   else if(kind == 2) writefocus();  /*MAGEOUT.C*/
   else if(kind == 3)
   {
      if( writedrawnew() !=1)  /*MAGEOUT.C*/
      {
         sprintf(alertstr,"nothing recognized in draw new stuff!");
         dosinglealert(0); /*MACDLOG.C*/ /*MPCDLOG.C*/
      }
      fclose(fpout); /* close here, writedrawnew can be part of output*/
      fpout = NULL;
      Ldrawactive = 0;  /*971129*/
   }
   else if(kind == 4) {LNOditto=1; writerotated();} /*MAGEOUT.C*/
   else if(kind == 5) 
   {/*modified kinemage*/
      /*writepruned() called as part of kinescopeout()i, scope is whole file*/ 
      /*strobe out the entire kinemage file, inc text and modified current kin*/
      if(kinescopeout()) /*returns with fpout closed! */
      {/*successfully written new output file*/
         Lpruneactive = 0;
         Ldrawactive = 0;
         Lnewviewactive = 0;
         Lnewstuffactive = 0;
#ifdef OLDCODE /*070702 DO NOT read in output file, keep going as is*/
         /*call dialog to decide just what one wants as current file*/    
         if(Lkinformat && Lorigkinfile) /*040208*/
         {/*read in output file to be current input file*/
            Lupdate = 0; /*must reinvoke activity with, e.g., probe*/
              /*effective appended group is lost on re-input of kinemage*/
              /*and there may be other reasons that cause crashes*/
            if(!LquitMAGE && !Lvisibleout && !Laverageout)/*Laverageout 060604*/
            {outputtoinput(kind);}
         }
#endif
      }
      else
      {/*failed to even write to output file */
         sprintf(alertstr,"BEWARE: failure in writing: %s"
                  ,OutStr);
         sprintf(alertstr2,"Current active kinemage file remains: %s"
                  ,NameStr);
         sprintf(alertstr3,"Modified kin file may be defective.");
         DoMageDLOGreport(3); /*mode 2 for abort, 3 for toggle debug*/
         ireturn = 0; /*failure*/
      }

   }/*modified kinemage*/
   else if(kind == 6)
   {
        writepostscript(); /*writes header, sets flags to continue*/
        LneedSetsinGrafWindow = 1;
        redrawvec();              /*____DRAW*/
   }
   else if(kind == 7) writequesanswers(); /*____OUT.C*/
   else if(kind == 8 || kind == 9)
   {/*overload Lraster3Drender == 1 for Raster3D, OR == 2 for POV-Ray*/
        writeraster3Drender(); /*writes header, sets flags to continue*/
        LneedSetsinGrafWindow = 1;
        redrawvec();              /*____DRAW*/
   }
   else if(kind == 10)
   {
        writepointID(); /*writes pointID to file*/   /*000326*/
        LneedSetsinGrafWindow = 1; 
        redrawvec();              /*____DRAW*/
   }
   else if(kind == 11)
   {
        writefullrgbpalette(); /*writes full rgb palette to file*/   /*010604*/
        LneedSetsinGrafWindow = 1; 
        redrawvec();              /*____DRAW*/
   }
   else if(kind == 14)
   {
        writehelptohtmlfile(); /*writes help to html file*/ /*020929*/
        LneedSetsinGrafWindow = 1;
        redrawvec();              /*____DRAW*/
   }
   else if(kind == 15 || kind == 17 ) 
   {/*user test results as current kinemage*/
      fprintf(fpout,"@text\n"); /*user test results to current output*/
      fprintf(fpout," Kinemage %d, Name: %s\n"
         ,thiskinenum,usernamestr);
      if(answerlistptr != NULL)
      {/*should be scored test results*/
         if(Ltestscoreavailable) /*the return flag from scoretest()*/
         {
          fprintf(fpout
              ," Kinemage %d test Score: %d correct out of %d,   %d wrong\n"
              ,thiskinenum
              ,Scorepositive,Scoretotal,Scorenegative);
           fprintf(fpout," Time allowed %.2f, time used %.2f\n"
              ,magetesttime,magetesttime-magetimeremaining);
           fprintf(fpout,"\n");
         }
         else
         {
           fprintf(fpout," (score vs answer failed)\n");
           fprintf(fpout,"\n");
         }
      }
      else
      {
        fprintf(fpout," (score answer not found)\n");
        fprintf(fpout,"\n");
      }
      if(Ltestoutfile==1) {writetext();} /*start of new test output file*/ 
      writepruned(); /*to current output file fpout just the current kinemage*/
      /*writepruned does not close output file, sometimes just part of output*/
      fprintf(fpout,"\n");
      fclose(fpout); /*probably good to close file as insurance against crash*/
      fpout = NULL;
   }/*user test results as current kinemage*/
   else if(kind == 18) /*060604*/
   {/*Laverageout  e.g. mean and sigma of 7D distribution, can do 3D as well*/
      Laverageout = 1; 
      Npassout = 0;
      writepruned(); /*which accum sums for means, but does not write to file*/
      getmeans();
      if(Npassout == 2 && NDIMout > 0)
      {
         writepruned(); /*which accum sigmas, but does not write to file*/
         if(sigmaNptr != NULL) {getsigmas();}
      }
      if(meansNptr!= NULL && sigmaNptr!=NULL && sigmaNptr->number > 1)
      {
         meansigmasout(); /*which writes means and sigmas to file*/
      }
      fclose(fpout); /*probably good to close file as insurance against crash*/
      fpout = NULL;
      destroycalcNstructure(meansNptr); /*housekeeping*/
      destroycalcNstructure(sigmaNptr); 
      NDIMout = 0;
      Npassout = 0;
      Laverageout = 0;
   }

   return(ireturn);
}
/*___routeoutput()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****outputtoinput()**********************************************************/
int outputtoinput(int kind)
{
   int ireturn = 1; /*presume successful*/

   kind = kind; /*avoids not-used warning*/  /*981212*/
   if ((fpnew = fopen(OutStr,"r")) == NULL)
   {/*failed to open output file for reading*/
       sprintf(alertstr,"BEWARE: failed to open file for reading: %s"
                    ,OutStr);
       sprintf(alertstr2,"Current active kinemage file remains: %s"
                    ,NameStr);
       sprintf(alertstr3,"Modified kin file may be defective.");
       DoMageDLOGreport(3); /*mode 2 for abort, 3 for toggle debug*/
       ireturn = 0; /*treat like canceling file open procedure*/
   }
   else
   {
       /*close input file, redefine output file to be the current input*/
       fclose(fp);
       fp = NULL;
       fp = fpnew;
       Lorigkinfile = 1; /*by definition*/  /*981207*/
       Lappend = 0; /*by definition, modified file is a primary file*/
       filerewind(fp);/*now the current primary kinemage file*/
       atEOF = 0;
       /*copy name, so can help get a different name if do this again*/
       strcpy(NameStr, OutStr);
#ifdef UNIX_X11
       printf(" MAGE opened modified file: %s\n",OutStr);
#endif

#ifdef PCWINDOWS
		SetMageCaption();
#endif
       Lnextkine = 0;
       LnextOK = 0;
       nextkinenum = thiskinenum; /*get back to current kinemage*/
       thiskinenum = 0;
       findkinemage(); /*kinesearch();*/
       ireturn = 1;/*981212*/
       /*000410 so kinemage itself is reread from file BUT text is NOT */
   }
   return(ireturn);
}
/*___outputtoinput()_________________________________________________________*/

/****clearwriteconditions()***************************************************/
void clearwriteconditions()
{
     Lgetnewfile=0;
     LquitMAGE=0;
     Lclosedrawstuff=0;
     Lgetnextkinemage=0;
     Lgetkinemagechoice=0;
     /*and restore the various active window parts*/
     LneedSetsinGrafWindow = 1; 
     if(Ltablepresent) {redrawtable();} /*000324*/
     redrawvec();              /*____DRAW*/
     makecursoractive();
}
/*___clearwriteconditions()__________________________________________________*/

/****isahydrogen()************************************************************/
int  isahydrogen(char atmname[5])/*070829 cribbed from prekin*/
{  /*toupper just before this called*/
   /*presumably the element name is within the first two non-blank characters*/
   /* avoid false positive at the potential expense of some false negatives*/
   /* for some HETATM record atoms */

    if(  ((atmname[0] == ' ')||(isdigit(atmname[0]) ) )         /* HN2,1HH1*/
       &&(atmname[1]=='H' ||atmname[1]=='D' ||atmname[1]=='Q') )
         {return(TRUE);}

    else if(  ((isdigit(atmname[2]) )&&(isdigit(atmname[3]))) /*old HG12,HH11*/
            &&(atmname[0]=='H' || atmname[0]=='D' || atmname[0]=='Q') )
         {return(TRUE);} /*this is an old (v. 2.3)  safe condition*/

    else if(        /*H 1st char of atom record name, new HOP2*/
            (atmname[0]=='H' ||atmname[0]=='D' ||atmname[0]=='Q') )
         {return(TRUE);}  /*general case of previous condition */
                          /*safe, one hopes, for ATOM records anyway*/

    else {return(FALSE);} /*maybe fails for some HETATM record hydrogens ??  */
}
/*___isahydrogen()___________________________________________________________*/

/****accumsums()**************************************************************/
void accumsums(void) /*Laverageout stuff*/
{
   int NDIMcurrent = 0;
   int i = 0;

   if(   ((thispointptr->listptr)->KKIND & DIMENSIONKKIND) 
       &&((thispointptr->listptr)->NDIM != 0) ) 
        { NDIMcurrent = (thispointptr->listptr)->NDIM; }
   else { NDIMcurrent = 3; }  /*first encountered group rules */

   if(NDIMout == 0) /*initialize*/
   {
      NDIMout = NDIMcurrent;  /*first encountered group sets the dimension */
      Npassout = 1;
      meansNptr = alloccalcNstructure();
      if(meansNptr == NULL)
      {
         NDIMout = -1;
fprintf(stderr,"failed to allocate storage for averages\n");
      }
   }
   else if(NDIMcurrent != NDIMout)
   {
      NDIMout = -1; /*inconsistent dimensions error*/ 
   }
   if(NDIMout == 3)
   {
      meansNptr->dimn[0] = meansNptr->dimn[0] + thispointptr->fx;
      meansNptr->dimn[1] = meansNptr->dimn[1] + thispointptr->fy;
      meansNptr->dimn[2] = meansNptr->dimn[2] + thispointptr->fz;
   }
   else if(NDIMout > 0 && (thispointptr->coordNptr != NULL) ) 
   {
      for(i=0; i<NDIMout; i++)
      {
        meansNptr->dimn[i] = meansNptr->dimn[i] + thispointptr->coordNptr[i];
      }
   }
   meansNptr->number++;
}
/*___accumsums()_____________________________________________________________*/

/****getmeans()***************************************************************/
void getmeans(void) /*Laverageout stuff*/
{
   int i=0;

   for(i=0; i<NDIMout; i++)
   {
      meansNptr->dimn[i] = meansNptr->dimn[i] / meansNptr->number;
   }
   Npassout = 2; /*first pass is now finished*/
}
/*___getmeans()______________________________________________________________*/

/****accumdevi()**************************************************************/
void accumdevi(void) /*Laverageout stuff*/
{
   int i = 0;
   double del=0;

   if(Npassout == 2) /*initialize*/
   {
      sigmaNptr = alloccalcNstructure();
      if(sigmaNptr == NULL)
      {
         NDIMout = -1;
fprintf(stderr,"failed to allocate storage for sigmas\n");
      }
      else
      {
         Npassout = 3; /*now past initialization for 2nd pass*/
      }
   }
   if(NDIMout == 3)
   {
      del = (thispointptr->fx) - meansNptr->dimn[0];
      sigmaNptr->dimn[0] = sigmaNptr->dimn[0] + del*del;
      del = (thispointptr->fy) - meansNptr->dimn[1];
      sigmaNptr->dimn[1] = sigmaNptr->dimn[1] + del*del;
      del = (thispointptr->fz) - meansNptr->dimn[2];
      sigmaNptr->dimn[2] = sigmaNptr->dimn[2] + del*del;

      sigmaNptr->number++;
   }
   else if(NDIMout > 0 && (thispointptr->coordNptr != NULL) ) 
   {
      for(i=0; i<NDIMout; i++)
      {
         del = (thispointptr->coordNptr[i]) - meansNptr->dimn[i];
         sigmaNptr->dimn[i] = sigmaNptr->dimn[i] + del*del;
      }
      sigmaNptr->number++;
   }
}
/*___accumdevi()_____________________________________________________________*/

/****getsigmas()**************************************************************/
void getsigmas(void) /*Laverageout stuff*/
{
   int i=0;
  if(sigmaNptr->number > 1)
  {
   for(i=0; i<NDIMout; i++)
   {/*where sigmaNptr->dimn[] holds the accum sum of differences squared*/
      sigmaNptr->dimn[i] = sqrt( (sigmaNptr->dimn[i])/((sigmaNptr->number)-1) );
   }
  }
}
/*___getsigmas()_____________________________________________________________*/

/****meansigmasout()**********************************************************/
void meansigmasout(void) /*Laverageout stuff*/
{
   int i = 0, j = 0;
   char dimstr[32];
   double avsigma = 0; /*averagesigma 060611*/

   if(NDIMout > 3) {sprintf(dimstr," dimension=%d wrap=360",NDIMout);}
   else {sprintf(dimstr," wrap=360");}

   fprintf(fpout,"@kinemage 1\n");
   fprintf(fpout,"@group {mean-sigma}%s dominant\n",dimstr);

   avsigma = 0;
   for(j=0; j<NDIMout; j++){avsigma += sigmaNptr->dimn[j];}
   avsigma = avsigma/NDIMout;
 
   fprintf(fpout,"@ringlist {mean} color= gray radius= %.3f width= 1 master= {mean}\n",avsigma);
   fprintf(fpout,"{mean of %lu} ",meansNptr->number); /*start ring point*/ /*121108 unsigned long NOT int*/
   fprintf(fpout,"<"); /*start comment of sigma values*/
   for(i=0; i<NDIMout; i++) {fprintf(fpout," %.3f",sigmaNptr->dimn[i]);}
   fprintf(fpout,">"); /*end comment of sigma values*/
   for(i=0; i<NDIMout; i++) {fprintf(fpout," %.3f",meansNptr->dimn[i]);}
   fprintf(fpout,"\n"); /*end of ring point*/

   /*060611 one line from - to + sigma rather than 2 both from middle*/
   fprintf(fpout,"@vectorlist {sigma} color= gray width= 1 master= {sigma}\n"); 
   for(j=0; j<NDIMout; j++) 
   {/*sigmas in each dimension extend only in one direction, so need N lines*/
      /*line - to + sigma in each dimension*/

      fprintf(fpout,"{-%.3f 1sig in %s}P ",sigmaNptr->dimn[j],dimensionptr[j].name); /*start P */
      for(i=0; i<NDIMout; i++) 
      { 
         if(i==j) /*line is drawn in this direction*/
           {fprintf(fpout," %.3f",(meansNptr->dimn[i] - sigmaNptr->dimn[i]) );}
         else /*coordinates stay the same for the L point*/
           {fprintf(fpout," %.3f",(meansNptr->dimn[i]) );}
      }
      fprintf(fpout,"\n"); /*end P - point*/

      fprintf(fpout,"{+%.3f 1sig in %s} ",sigmaNptr->dimn[j],dimensionptr[j].name); /*start L */
      for(i=0; i<NDIMout; i++) 
      { 
         if(i==j) /*line is drawn in this direction*/
           {fprintf(fpout," %.3f",(meansNptr->dimn[i] + sigmaNptr->dimn[i]) );}
         else /*coordinates stay the same for the L point*/
           {fprintf(fpout," %.3f",(meansNptr->dimn[i]) );}
      }
      fprintf(fpout,"\n"); /*end L + point*/
   }
}
/*___meansigmasout()_________________________________________________________*/

