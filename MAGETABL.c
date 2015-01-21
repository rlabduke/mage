/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGETABL.c*/
#include "MAGE.h"
#include "MAGEFLAG.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
 
#define MAGETABL
#include "MAGETABL.h"
#undef MAGETABL

#define tablepick(i,j) tablepick[i*(MAXCOL+1)+j]  /*pointer arithmetic*/
#define tablepointptr(i,j) tablepointptr[i*(MAXCOL+1)+j]
#define tablecolwide(j)  tablecolwide[j]
#define tablerowhigh(j)  tablerowhigh[j]

static pointstructptr* tablepointptr;
static int*  tablerowhigh; /*will get alloc as an array of MAXROW items*/
static int*  tablecolwide; /*will get alloc as an array of MAXCOL items*/
static int*  tablepick;
static pointstruct* savedvictimptr = NULL; /*last deleted point */
/*beware, if try to destroy this point after alloc, parent list NULL or wierd*/
int  scantableforpick(pointstructptr*); /*MAGETABL.c*/
void freetablearrays(void);
static pointstruct* tablesearchrefpointptr = NULL;


/*Like everything else in mage and the kinemage format, there is great */
/*flexibility despite arbitrary limitations based on original intended use, */
/* and no effective protection against the kinemage author/reader from making */
/*totally obscure and meaningless kinemages.  However, for good or ill, */
/*MAGE remains ignorant of any particular subject of science, like molecules.  */
/*Good luck! */
/****drawtable()***************************************************************/
void drawtable(int ipass)
{/*can only get here called from ____DRAW.c/redrawtable(): SetPort(tableWindow) */
/*EXCEPT on first setup called from createtable to get actual row and col sizes*/
/*Ltablepicked flags table initiated pick, set here, used in MAGEDRAW.c/drawvec()*/
/*Ltablegraphpicked flags graph initiated pick, set by graphic, used here*/
/*both of these flags are used for both discovery and final show passes */
/*Ltablepicked managed by DotableContentClick()*/
/*Ltablegraphpicked managed by redrawtable()*/

   int  rowstack = 0;
   int  lasty = 0;
   int  row = 0;
   int  col = 0;
   int  colleft = 0;
   int  colx = 0;
   int  wraps = 0;
   int  rowy = 0;
   int  cellx = 0;
   int  celly = 0;
   int  color = 0;
   int  icolor = 0;
   int  bcolor = 0;
   int  idecue = 0;
   int  ideepcolor = 0;
   pointstructptr currentpointptr = NULL;
   int  Lcellpicked = 0;
   int  Lthiscellpicked = 0;
   int  i,j;
   int  trialrowhigh = 0;
   int maxx = 0;
   int maxy = 0;
   int Nrowpicked = -1;
   int Ncolpicked = -1;
   int Lneedcolornumber = 0, numcolor = 0;
   
   
   /*calc the height of the set of rows to be shown*/
   /*rowlast, collast are the max row,col numbers read in for this kinemage*/
   for(row=0; row<= MAXROW; row++)
   {
      if( (row==0 && (tablewrapcol >= tableheaderMAXCOL) )  )
      {
         ;/*don't include row0 in rowstack*/
      }
      else
      {
         /*ipass INITIAL determines height of rows*/
         rowstack = rowstack + tablerowhigh[row];
      }
   }
   rowy = tablezerohigh; /*starting position in absolute coordinates*/
   /*keep setting actual table sizes as they are determined*/

   /*loop over all table items*/
   for(row = 0; row <= MAXROW; row++)
   {/*loop over rows, the y, vertical, direction*/
      colx = tablezerowide; /*scroll bar sets effective zero of coordinates*/
      colleft = 0;
      wraps = 0; /*start of new row, is in first set of a stack of rows*/

      tablerowheight = tablerowhigh[row]; /*global value of latest row height*/
      /*set y position after the row since use upper left of cell for point*/
      
      
      for(col = 0; col <= MAXCOL; col++)
      {/*loop over columns, the x, horizontal, direction*/
        if(tablewrapcol && ((col) > (colleft + tablewrapcol )) ) 
        {/*more columns than desired horizontally, wrap them*/
            colleft = colleft + tablewrapcol; 
            wraps++;           
            /*wraped lines start under col(1)*/
            colx = tablezerowide + tablecolwide(0);
        }
        /*colx is augmented by the column width after cell is written*/
        cellx = colx; /*index point at upper left of cell*/
        celly = rowy + wraps * rowstack;

        tablecolwidth = tablecolwide[col]; /*global value of latest col width*/
        
        /*retrieve the kinemage data point that is associated with this cell*/
        currentpointptr = tablepointptr(row,col);

        if(currentpointptr != NULL)
        {/*cell has designated contents to be put in the table*/
     
           if( (currentpointptr->type & UNPICKABLE)==0 || Lsuperpick||col==0||row==0)/*000324*/
           {/*pickable cell*/
              if(Ltablepicked == 0 && Ltablegraphpicked==3)
              {/*3: graphics call from displayed table object: match pickedpointptr*/ 
                 if(pickedpointptr==currentpointptr)
                 {/*graphics copy of this point has been picked*/
                    /*only expect one such pick per pass, reset flag now*/
                    Ltablegraphpicked=1; /*1 for another pass to show all current picks*/
                    Lthiscellpicked = 1;
                 }
              }
              else if(Ltablepicked == 0 && Ltablegraphpicked==2)
              {/*2: arbitrary graphics point has been picked, */
               /*this can flag more than one table cell*/
                 if(   pickedpointptr->fx == currentpointptr->fx
                    && pickedpointptr->fy == currentpointptr->fy
                    && pickedpointptr->fz == currentpointptr->fz )
                 {
                    /*could be more than one such pick per pass, DO NOT reset flag now*/
                    /*Ltablegraphpicked left ==2, */
                    /*later: set 1 for another pass to show all current picks*/
                    Lthiscellpicked = 1;
                 }
              }
              else if(   Ltablepicked == 2
                   && tablepickx >= cellx && tablepickx <= cellx + tablecolwide(col)
                   && tablepicky >= celly && tablepicky <= celly + tablerowhigh(row))
              {/*table cell has been picked*/
                if(col==0 && row==0) /*000324*/
                {
                   ;
                }
                else if(col==0)  /*000324*/
                {
                   /*zeroth cell picked when OFF, turns row ON*/
                   if( tablepick(row,col) == 0) Nrowpicked = row; /*010916*/
                   /*zeroth cell picked when ON, turns row OFF*/
                   else Nrowpicked = -1; /*010916*/
                   Lthiscellpicked = 1; /*010916*/
                   Ltablecellnewlypicked = 1;
                   Ltablepicked = 1; /* 1 for another pass to show all current picks*/
                }
                else if(row==0) /*000324*/
                {
                   /*zeroth cell picked when OFF, turns column ON*/
                   if( tablepick(row,col) == 0) Ncolpicked = col; /*010916*/
                   /*zeroth cell picked when ON, turns column OFF*/
                   else Ncolpicked = -1; /*010916*/
                   Lthiscellpicked = 1; /*010916*/
                   Ltablecellnewlypicked = 1;
                   Ltablepicked = 1; /* 1 for another pass to show all current picks*/
                }
                else
                {
                   if( tablepick(row,col) == 0)
                   {/*pick of previously NOT-picked cell (else will toggle it to not-picked)*/
                      Ltablecellnewlypicked = 1;
                      pickedpointptr = currentpointptr;
                      pickedlistptr  = currentpointptr->listptr;
                      pickedsgrpptr  = pickedlistptr->sgrpptr; /*for safety of completeness*/
                      pickedgrupptr  = pickedsgrpptr->grupptr; /*for safety of completeness*/
                   }
                   /*only allow one such positional pick per pass, reset flag now*/
                   Ltablepicked = 1; /* 1 for another pass to show all current picks*/
                   Lthiscellpicked = 1;
                } /*000324*/
              }
              if(col>0 && Nrowpicked == row)/*000324*/
              {
                 Lthiscellpicked = 1;
              }
              else if(row>0 && Ncolpicked == col)/*000324*/
              {
                 Lthiscellpicked = 1;
              }

              if(Lthiscellpicked > 0)
              {/*this cell is picked*/
                 Lthiscellpicked = 0; /*just for this cell*/
                 Lcellpicked++;
                 /*toggle pick state of this cell*/
                 if(tablepick(row,col) == 0)
                 {
                    tablepick(row,col) = 1;
                 }
                 else if(Nrowpicked != row && Ncolpicked != col)/*000324*/
                 {
                    tablepick(row,col) = 0;
                 }
                 /*problem Lshiftkey: */
                 /*shifted state has special, wierd meaning for graphics picking*/
                 if(row!=0 && col!=0 && !Lshiftkey && Lcellpicked==1 && tablepick(row,col) == 1)
                 {/*first ordinary cell picked this pass, wipe out all previous pickstates*/
                    for(i=0; i<= MAXROW; i++) /*i.e. MAXROW+1 entries including 0*/
                       for(j=0; j<= MAXCOL; j++) /*i.e. MAXCOL+1 entries including 0*/
                          if(i==row && j==col) ;
                          else tablepick(i,j) = 0;
                 }
                 if(row==0 && col!=0 && (Lcellpicked==1 && tablepick(row,col) == 0))
                 {/*zeroth row toggled OFF turns OFF whole column*/
                    for(i=0; i<= MAXROW; i++) /*i.e. MAXROW+1 entries including 0*/
                       for(j=0; j<= MAXCOL; j++) /*i.e. MAXCOL+1 entries including 0*/
                          if(j==col) tablepick(i,j) = 0;                 
                 }
                 if(row!=0 && col==0 && (Lcellpicked==1 && tablepick(row,col) == 0))
                 {/*zeroth col toggled OFF turns OFF whole row*/
                    for(i=0; i<= MAXROW; i++) /*i.e. MAXROW+1 entries including 0*/
                       for(j=0; j<= MAXCOL; j++) /*i.e. MAXCOL+1 entries including 0*/
                          if(i==row) tablepick(i,j) = 0;                 
                 }

              }/*this cell is picked*/
           }/*pickable cell*/

           /*getptIDstring(word, currentpointptr);*/
           /*word char str stored in pointstruct pointed to by tablepointptr*/
           getcommentstring(word, currentpointptr);/*MAGELIST gets wordcom[]*/
               /* note: 3 byte format info on front of string*/
           /*as of 991008 only use color, not type, depthcue, penwidth of a point*/
           /*table entries are all of one type: label: where pointID --> cell*/
           /*penwidth is not used for the font*/
           /*depthcue modification of color is used for highlighting*/
           /*(later might use background color for highlighting also)*/
           /*However, there is the option of list color and point color*/

           Lneedcolornumber = 1;
           if(    naspectsthiskinemage > 0 /*there are aspects possible*/
               && currentaspect > 0 /*an aspect is selected*/
               && currentaspect <= MAXPOINTASPECTS /*point storage limited*/
             )
           {/*aspect color dominant*/
             numcolor = decodecolor(currentpointptr->aspects,currentaspect);
             if(numcolor > 0 && numcolor < 32)
             {/*renderer can translate colornumber to palette index*/
                color=((numcolor&31) | ((currentpointptr->colorwidth) & ~31));/*121108 (logic)*/
                       /*~31: one's complement of 31, i.e. bits 1->0,0->1*/
                       /*i.e. include everything except color code*/
                Lneedcolornumber = 0;
             }
           }/*aspect color dominant*/
           if(Lneedcolornumber)
           {/*NOT aspect color, try for point color, then try list color*/
           if( ((currentpointptr->colorwidth) & 31) > 0)
           {/*point has its own color*/
              if(Listcolordominant)
              {/*do not use this point's color*/
                 color = ((currentpointptr->listptr)->color & 31);
              }
              else
              {color = ((currentpointptr->colorwidth) & 31);}
           }/*point has its own color*/
           else {color = ((currentpointptr->listptr)->color & 31);}
           }/*NOT aspect color, try for point color, then try list color*/


           /*idecue = 0; for black bkg palette*/
           /*idecue =25; for white bkg palette*/
           /*ideepcolor = 0; for furthest back to background*/
           /*ideepcolor = 4; for nearest-to-front color hue and saturation*/
           /*depthcue 1-5 used as ideepcolor 0-4*/
         
           if(tablepick(row,col) == 1)
           {/*make colored background in cell*/
              /*bcolor = numberofcolor("gray");*/ /*MAGECOLR.c*/
              bcolor = numberofcolor("white"); /*MAGECOLR.c*/
              /*use gray or less saturated of same color ????*/
              /*ideepcolor = 4;*/ /*0 far from viewer, to 4 close to viewer*/
              ideepcolor = 2; /*0 far from viewer, to 4 close to viewer*/
              if(Ltableblackbkg)
              {
                 idecue =  0; /*for black bkg palette*/
              }
              else
              {
                 idecue = 25; /*for white bkg palette*/
              }
              if(LLcolor) /*can support real color*/
              {/*use depthque bin selected for highlight property*/
                 bcolor = (bcolor & 31) + 50*ideepcolor + idecue;
              }/*the "& 31" is a final sanity filter limit for valid color values*/
              else
              {/*fudge gray level by explicit "depthcue"*/
                 bcolor= numberofcolor("gray") + 50*ideepcolor +idecue;
              }
              /*
                 rectangletoscreen(cellx, celly
                    , cellx + tablecolwide(col), celly + tablerowhigh(row), bcolor);
              */
              Ltablecellispicked = 1;
              currentpointptr->STATUS = currentpointptr->STATUS | ON;/*ON*/
              /*000324  pick state of cell sets ON/OFF of putative graphics point*/
           }
           else  
           {/*000324  pick state of cell sets ON/OFF of putative graphics point*/
              Ltablecellispicked = 0;
              currentpointptr->STATUS = currentpointptr->STATUS & ~ON; /*OFF*/
           }
           /*Now write cell characters to the table*/
           /*color >= 0 && color <= 255 is pretty robust, don't bother to check*/
           if(color == 31 && ipass > 0)
           {/*invisible and not checking col 1 of row 1 for actual drawn width*/
              icolor = -1;
           }
           else
           {/*visible color value*/
              ideepcolor = 4;
              if(Ltableblackbkg)
              {
                 idecue =  0; /*for black bkg palette*/
              }
              else
              {
                 idecue = 25; /*for white bkg palette*/
              }
              if(LLcolor) /*can support real color*/
              {/*use depthque bin selected for foreground*/
                 icolor = (color & 31) + 50*ideepcolor + idecue;
              }/*the "& 31" is a final sanity filter limit for valid color values*/
              else
              {/*fudge gray level by explicit "depthcue"*/
                 icolor= numberofcolor("gray") + 50*ideepcolor +idecue;
              }
           }/*visible color value*/
           if( (ipass == 0) && (row==1) && (col==0||col==1) )
         /*if( (ipass == 0) && (row==0) && (col==0||col==1) )*/ /*000324*/
           {
              Ltablecellgetwidth = 1;
           }
           else 
           {
              Ltablecellgetwidth = 0;
           }
           if(ipass == 0 || ipass == 1 || (ipass == 2 && tablepick(row,col) == 1) )
           {/*actual call to draw contents in cell*/
              /*only use bcolor when a pick is current, so bcolor is defined correctly*/
              tablewordwidth = 0; /*so return with word witdth of this word itself*/
                  /*010916 linehigh == tablefontsize + 4*/
              lasty = wordstotable(word,cellx,celly + tablefontsize
                                   , icolor, bcolor, ipass ); 
                          /*____DRAW.C*/
              /*lasty is max y of a character placed in the table*/
              /*tablewordwidth == max string width on any one line of the word str*/           
           }

           if(ipass==0 && row==0)
           {/*pass==0 special processing of row 0*/
              tableheaderMAXCOL = col;
           }

        }/*cell has designated contents*/
        

        if(ipass==0 && row==1) /*see if can improve on default col widths, & row height*/
        {/*pass==0 special processing of row 1, col 0 and 1 */
           if(col==0 || col==1)
           {
              if(tablecolscale < 0.1 || tablecolscale > 100.1 ) { tablecolscale = 1.0;}
              if(tablewordwidth > 0)
              {
                 tablecolwide(col) = (int)(tablewordwidth * tablecolscale);
              }
           }
           if(col==0)
           {
              if( (lasty - celly)>0) trialrowhigh = lasty - celly;
              else trialrowhigh = 0;
           }
           else if(col==1)
           {
              if( (lasty - celly)>0 && (lasty - celly) >trialrowhigh)
                 tablerowhigh(1) = lasty - celly;
              else if(trialrowhigh > 0)
                 tablerowhigh(1) = trialrowhigh;
           }
        }/*pass==0 special processing of row 1, col 0 and 1 */
        
        /*set x position for the next cell*/
        colx = colx + tablecolwide(col);  
        if(maxx < colx) maxx = colx;
      }/*loop over columns, the x, horizontal, direction*/
      /*now increase y for the next row of cells*/
      rowy = rowy + tablerowhigh(row); 
      if(maxy < celly+tablerowhigh(row)) maxy = celly+tablerowhigh(row);
   }/*loop over rows, the y, vertical, direction*/
  
   if(Ltablepicked > 0 && Lcellpicked > 0) 
        Ltablepicked = 1; /*update pick state of graphics*/
/*Ltablepicked managed by DotableContentClick(), will be set to 0 when done*/

   if(Ltablegraphpicked > 0 && Lcellpicked > 0) 
        Ltablegraphpicked = 1; /*update pick state of table*/
/*Ltablegraphpicked managed by redrawtable(), will be set to 0 when done*/
   
   Lcellpicked = 0;/*redundant, local for new picks of current pass*/

/*table actually written between absolute coords tablezerowide to maxx*/
/* and tablezerohigh to maxy, where tablezeros can be negative*/
/* In absolute terms: */
   tablehigh = maxy - tablezerohigh;
   tablewide = maxx - tablezerowide;
   if(ipass==0) {Ltableready = 1;}
}
/*___drawtable()______________________________________________________________*/

/****scantableforpick()********************************************************/
int scantableforpick(pointstructptr* apointptrptr)
{
   int row = 0;
   int col = 0;
   int npicks = 0;
   
   /*apointptr = NULL;*/
   for(row = 0; row <= MAXROW; row++)
   {/*loop over rows, the y, vertical, direction*/
      for(col = 0; col <= MAXCOL; col++)
      {/*loop over columns, the x, horizontal, direction*/
         if(tablepick(row,col) == 1)
         {/*cell is in picked state*/
            *apointptrptr = tablepointptr(row,col);
            if(*apointptrptr != NULL)
            {/*picked cell is occupied*/
               npicks = npicks + 1;
            }/*picked cell is occupied*/
         }/*cell is in picked state*/
      }/*loop over columns, the x, horizontal, direction*/
   }/*loop over rows, the y, vertical, direction*/
   return(npicks); /*apointptr points to last picked point*/
}
/*___scantableforpick()_______________________________________________________*/

/****setuptable()**************************************************************/
void setuptable()
{
   int j=0;
   int left=0, top=0, right=0, bottom=0;
   
   /*initial values, fit table into window.  Can be changed by input or in run*/
   gettablewinedges(&left, &top, &right, &bottom);
   tablewindowhigh = bottom - top - 22;
   tablewindowwide = right - left - 22;

   tablezerohigh = 0; /*keeps track of scrolled effective top  window place*/
   tablezerowide = 0; /*keeps track of scrolled effective left window place*/
   tablewordwidth = 0; /*keeps track of maximum width of table cells*/
   
   tablehigh = 0;
 /*if(tablerowhigh(0) == 0) tablerowhigh(0) = tablefontsize/2;*/
   if(tablerowhigh(0) == 0) tablerowhigh(0) = tablefontsize;/*000324*/
   tablehigh = tablehigh + tablerowhigh(0);
   if(tablerowhigh(1) == 0) tablerowhigh(1) = tablefontsize;
   tablehigh = tablehigh + tablerowhigh(1);
   for(j=2; j<= MAXROW; j++) /*i.e. MAXROW+1 entries including 0*/
   {
      tablerowhigh(j) = tablerowhigh(1);
      tablehigh = tablehigh + tablerowhigh(j);
   }
if(Ltest) 
printf("setuptable set values for tablerowhigh(0) %d\n",tablerowhigh(0));
if(Ltest) 
printf("setuptable set values for tablerowhigh(1) %d\n",tablerowhigh(1));  

   tablewide = 0;
   if(tablecolwide(0) == 0) tablecolwide(0) = 16;
   tablewide = tablewide + tablecolwide(0);
   if(tablecolwide(1) == 0) tablecolwide(1) = 32;
   tablewide = tablewide + tablecolwide(1);
   for(j=2; j<= MAXCOL; j++) /*i.e. MAXCOL+1 entries including 0*/
   {
      tablecolwide(j) = tablecolwide(1);
      tablewide = tablewide + tablecolwide(j);
   }
if(Ltest) 
printf("setuptable set values for tablecolwide(0) %d\n",tablecolwide(0));
if(Ltest) 
printf("setuptable set values for tablecolwide(1) %d\n",tablecolwide(1));

   /*if(tablewrapcol ==0) table will not wrap, else wraps at the # columns*/
}
/*___setuptable()_____________________________________________________________*/

/****loadtable()***************************************************************/
void loadtable(int ipass)
{
   int row = 0;
   int col = 0;
   int rowmax = 0;
   int colmax = 0;
   float angle = 0;
   
   /*only called if Ltablepresent flag set for this kinemage*/
   /*scan over all groups, etc.: future plans for embedded table info anywhere*/
   row = 0;
   thisgrupptr = firstgrupptr;
   while(thisgrupptr != NULL)
   {/*begin-loop-over-groups*/
      if((thisgrupptr->type & TABLEFLAG) == TABLEFLAG) /*991009 only condition*/
      {/*table group*/
         if(row==0 && (thisgrupptr->type & TABLECOLHEAD) == TABLECOLHEAD)
         {
            row = 0; /*000324*/
            Ltablecolheaders = 1;  /*000324*/
         }
         else
         {
            row++;
         }
            /*get row,col assignment or set*/
            if(row>rowmax) rowmax = row;
            
            col = 0; /*each row starts with a zeroth col*/
         thissgrpptr = thisgrupptr->firstsgrpptr;
         while(thissgrpptr != NULL)
         {/*begin-loop-over-sets*/
            thislistptr = thissgrpptr->firstlistptr;
            while(thislistptr != NULL)
            {/*begin-loop-over-lists*/  
               /*if((thislistptr->type & WORDS) == WORDS)*/
               {/*only can handle wordlists as of 991009*/
               /*000324 treats anything like a wordlist crt setting table cells*/
                  thispointptr = thislistptr->firstpointptr;
                  while(thispointptr != NULL)
                  {/*begin-loop-of-points*/ 
                     if(ipass != 0) /*pass 0 just counts rows and columns*/
                     {/*regular flow, load the cell point pointer*/
                        /*turn all points OFF as far as graphics is concerned*/
                        thispointptr->STATUS = thispointptr->STATUS & ~ON; /*i.e. OFF*/
                        tablepointptr(row,col) = thispointptr;
                        /*establish any special characteristics of this point as a function*/
                        /*of position in the table*/
                        if((thispointptr->type & MARKTYPE)==MARKTYPE)
                        {/*special type of marks presented in graphics when cell picked*/
                           if((thispointptr->STYLE & FANSTYLE)==FANSTYLE)
                           {/*radial magnitude at row indexed postion in a fan shape*/
                              /*for now, just count rows, presume all have fan spines*/
                              angle = (float)(row*(180/(MAXROW+1)));/*line at angle degrees from 0*/
                              if(((thispointptr->moreinfoptr) = allocmoreinfostructure()) != NULL)
                              {
                                 (thispointptr->moreinfoptr)->f1 
                                    = (float)cos((double)((2*3.14159)*angle/360));
                                 (thispointptr->moreinfoptr)->f2 
                                    = (float)sin((double)((2*3.14159)*angle/360));
                              }
                           }/*radial magnitude at row indexed postion in a fan shape*/
                        }/*special type of marks presented in graphics when cell picked*/
                     }/*regular flow, load the cell point pointer*/
                     
                     col++; /*augment for next entry*/
                     if(col>colmax) colmax = col;

                     if(thispointptr == thislistptr->lastpointptr) 
                        thispointptr=NULL;
                     else thispointptr = thispointptr->nextptr;                 
                  }/*endof-draw-loop-of-points*/

               }/*only can handle wordlists as of 991009*/
               /*000324 treats anything like a wordlist crt setting table cells*/
               if(thislistptr == thissgrpptr->lastlistptr) thislistptr=NULL;
               else thislistptr = thislistptr->nextptr;
            }/*endof-loop-over-lists*/                
            if(thissgrpptr == thisgrupptr->lastsgrpptr) thissgrpptr=NULL;
            else thissgrpptr = thissgrpptr->nextptr;
         }/*endof-loop-over-subgroups*/
      }/*table group*/

      thisgrupptr = thisgrupptr->nextptr;/*==NULL if last group*/
   }/*endof-loop-over-groups*/

   if(ipass==0)
   {
      MAXROW = rowmax;
      MAXCOL = colmax;
if(Ltest)
printf("pass: %d: loadtable gets MAXROW %d, MAXCOL %d\n",ipass,MAXROW,MAXCOL);
   }
}
/*____loadtable()_____________________________________________________________*/



/****createtable()*************************************************************/
void createtable()
{/*called when Ltablepresent > 1 when return from GetStuffFromFile()*/

   Ltablepresent = 1; /*keep true, but don't construct more table windows*/
   Ltablepicked = 0; /*initialize*/
   Ltablepickactive = 0;
   Ltablegraphpicked = 0;
   Ltablegroupgraphed = 0; 
   
   /*compromise size, since font size NOT really correlated to window size*/
   SetUptableWindow(); /*____INIT.c*/
   constructtablecells(); 
   createtablescrollbars(); /*separately done after window and cells defined*/
}
/*___createtable()____________________________________________________________*/

/****constructtablecells()*****************************************************/
void constructtablecells()
{
   tableheaderMAXCOL = 0;
   loadtable(0); /*first pass gets MAXROW, MAXCOL, nothing to load yet*/
   alloctablearray(); /*using MAXROW, MAXCOL, so arrays for loading will exist*/  
   setuptable(); /*default sizes for table of MAXROW,MAXCOL, to display cells*/
   loadtable(1); /*now can associate data points with table cells*/
   dodrawtable(0); /*now can get actual row and col sizes from content*/
   setuptable(); /*default sizes for table of MAXROW,MAXCOL, to display cells*/
   dodrawtable(1); /*now can get actual row and col sizes from content*/                 
}
/*___constructtablecells()___________________________________________________*/

/****tablecelldelete()********************************************************/
void tablecelldelete(void)
{
   pointstruct* victimptr = NULL; /*point picked*/
   int npicks = 0; /*number of picks found*/

   npicks = scantableforpick(&victimptr);
   if(npicks == 1)
   {/*found unique picked point*/
      /*first set up to be able to undo destruction*/
      if(savedvictimptr == NULL) 
      {
         savedvictimptr = alloclonelypointstructure();
      }
      if(savedvictimptr != NULL)
      {
         copypointstructure(savedvictimptr, victimptr);
         savedvictimptr->listptr = victimptr->listptr; /*020830 re MAGESCOR*/
           /*so stored saved point knows who its original list is */
      }

      destroypointstructure(victimptr);
      
      /*now rebuild table*/
      freetablearrays(); /*first destroy the old table arrays, leaving window*/
      /*then reconstruct the table as it is now defined by the kinemage groups*/
      constructtablecells();
      
   }/*found unique picked point*/
   else
   {
       sprintf(alertstr,"Deletion NOT done, will only work if just one cell picked");
       sprintf(alertstr2," Scan of table found %d cells flagged as picked",npicks);
       sprintf(alertstr3," ");
       DoMageDLOGreport(1); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
   }
}
/*___tablecelldelete()_______________________________________________________*/

/****restoredeletedpoint()****************************************************/
void restoredeletedpoint()
{
   pointstruct* apointptr = NULL; /*point created*/


   if(savedvictimptr != NULL)
   {/*saved point exists*/
      if(savedvictimptr->previousptr != NULL)
      {/*can do insertion after a neighbor point*/
        apointptr = insertpointstructure(savedvictimptr->previousptr, 1);
      }
      else if(savedvictimptr->nextptr != NULL)
      {/*can do insertion before a neighbor point*/
        apointptr = insertpointstructure(savedvictimptr->nextptr,-1);
      }
      else if(savedvictimptr->listptr != NULL)
      {/*must have been last member of otherwise empty list*/
        apointptr = allocpointstructure(savedvictimptr->listptr);
      }
      if(apointptr != NULL)
      {/*restore point to table*/
         copypointstructure(apointptr, savedvictimptr);
         apointptr->listptr = savedvictimptr->listptr;  /*020830 re MAGESCOR*/
           /*so new recovered point knows its parent list*/
         /*now rebuild table*/
         freetablearrays(); /*first destroy the old table arrays, leaving window*/
         /*then reconstruct the table as it is now defined by the kinemage groups*/
         constructtablecells();
      }   
   }/*saved point exists*/
   else
   {
       sprintf(alertstr,"Restoration NOT done, did not find a saved point");
       sprintf(alertstr2," ");
       sprintf(alertstr3," ");
       DoMageDLOGreport(1); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
   }

}
/*___restoredeletedpoint()___________________________________________________*/

/****tablecellinsert()********************************************************/
void tablecellinsert(int where)
{
   pointstruct* victimptr = NULL; /*point picked*/
   pointstruct* apointptr = NULL; /*point created*/
   int npicks = 0; /*number of picks found*/

   /*where > 0 for placing after picked cell, <= 0 for placing before*/
   npicks = scantableforpick(&victimptr);
   if(npicks == 1)
   {/*found unique picked point*/
      apointptr = insertpointstructure(victimptr, where);
      if(apointptr != NULL)
      {/*now rebuild table*/
         freetablearrays(); /*first destroy the old table arrays, leaving window*/
         /*then reconstruct the table as it is now defined by the kinemage groups*/
         constructtablecells();
      }   
   }/*found unique picked point*/
   else
   {
       sprintf(alertstr,"Insertion NOT done, will only work if just one cell picked");
       sprintf(alertstr2," Scan of table found %d cells flagged as picked",npicks);
       sprintf(alertstr3," ");
       DoMageDLOGreport(1); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
   }

}
/*___tablecellinsert()_______________________________________________________*/

/****tablerebuild()************************************************************/
void tablerebuild()
{
         freetablearrays(); /*first destroy the old table arrays, leaving window*/
         /*then reconstruct the table as it is now defined by the kinemage groups*/
         constructtablecells();
}
/*___tablerebuild()___________________________________________________________*/

/****tablecolor()**************************************************************/
void tablecolor()
{
   pointstruct* victimptr = NULL; /*point picked*/
   int npicks = 0; /*number of picks found*/

   /*where > 0 for placing after picked cell, <= 0 for placing before*/
   npicks = scantableforpick(&victimptr);
   if(npicks == 1)
   {/*found unique picked point*/
      pickedpointptr = victimptr;
      pickedlistptr  = victimptr->listptr;
      pickedsgrpptr  = pickedlistptr->sgrpptr; /*for safety of completeness*/
      pickedgrupptr  = pickedsgrpptr->grupptr; /*for safety of completeness*/

      DocolorshowDialog(); /*operates on global pickedpointptr*/
   }/*found unique picked point*/
}
/*tablecolor()______________________________________________________________*/

/****alloctablearray()*********************************************************/
void    alloctablearray()
{/*one shot version of allocating pointptr array for this kinemage's table*/
    int  ifail;
   int row,col;

   ifail = 0;
   if( (MAXROW*MAXCOL) < 0) ifail = 1; /*large number rolls to negative*/
   
   if(ifail==0)
   {/*still OK */
      tablepointptr = (pointstructptr *)malloc(
                                         sizeof(pointstructptr)*(MAXCOL+1)*(MAXROW+1));
      if(tablepointptr==NULL) ifail = 1;
      else
      {
         for(row=0; row<=MAXROW; row++) /*i.e. MAXROW+1 entries including 0*/
         {
            for(col = 0; col <= MAXCOL; col++)
            {/*loop over columns, the x, horizontal, direction*/
               
               tablepointptr(row,col) = NULL;
            }/*loop over columns, the x, horizontal, direction*/
         }
      }
   }
   
   if(ifail==0)
   {/*still OK */
      tablepick = (int *)malloc(sizeof(int)*(MAXCOL+1)*(MAXROW+1));
      if(tablepick==NULL) ifail = 1;
      else
      {
         for(row=0; row<=MAXROW; row++)
         {
            for(col = 0; col <= MAXCOL; col++)
            {/*loop over columns, the x, horizontal, direction*/
               tablepick(row,col) = 0;
            }/*loop over columns, the x, horizontal, direction*/
         }
      }
   }

   if(ifail==0)
   {/*still OK */
      tablerowhigh = (int *)malloc(sizeof(int)*(MAXROW+1));
      if(tablerowhigh==NULL) ifail = 1;
      else
      {
         for(row = 0; row <= MAXROW; row++)
         {/*loop over rows, the y, vertical, direction*/
            tablerowhigh(row) = 0;
         }
      }
   }


   if(ifail==0)
   {/*still OK */
      tablecolwide = (int *)malloc(sizeof(int)*(MAXCOL+1));
      if(tablecolwide==NULL) ifail = 1;
      else
      {
         for(col = 0; col <= MAXCOL; col++)
         {/*loop over columns, the x, horizontal, direction*/
            tablecolwide(col) = 0;
         }
      }
   }



   if(ifail)
   {/*failure at first load, report real trouble*/
      printf("failure allocating table array\n");
      sprintf(alertstr,"allocation of table space failed");
      sprintf(alertstr2,"table cells will not be referred to graphics objects");
#ifdef MACINTOSH
      sprintf(alertstr3
                   ,"Perhaps preferred size too small: Before launching MAGE:"
                        CRLF"one click on MAGE, choose Info under File Menu: "
                        CRLF"change preferred size to, e.g., 10000 ");
#else
      sprintf(alertstr3," ");
#endif
      DoMageDLOGreport(2); /*___DLOG.C*/ /*alertstr,alertstr2,alertstr3*/
        /*cleanup();*/
        /*exit(EXIT_FAILURE);*/
   }/*failure at first load, report real trouble*/
}
/*___alloctablearray()_______________________________________________________*/

/****freetablearrays()********************************************************/
void freetablearrays()
{
	if (tablepointptr != NULL)
	{
	   free(tablepointptr);
	   tablepointptr = NULL;
	}
	if (tablepick != NULL)
	{
	   free(tablepick);
	   tablepick  = NULL;
	}
	if (tablerowhigh != NULL)
	{
	   free(tablerowhigh);
	   tablerowhigh  = NULL;
	}   
	if (tablecolwide != NULL)
	{
	   free(tablecolwide);
	   tablecolwide  = NULL;
	}   
}
/*___freetablearrays()_______________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****cleanuptable()***********************************************************/
void cleanuptable(void)  
{	
   Ltablepresent = 0; /*called when new kinemage comes in */
   Ltableready = 0;
     /* before GetStuffFromFile has a chance to find a new table group */
   freetablearrays();
   killtablemarkers();
#ifdef UNIX_X11
   XtDestroyWidget(toptable);
#endif   
#ifdef MACINTOSH
   DisposeControl(htableScroll);
   DisposeControl(vtableScroll);
   DisposeWindow(tableWindow);
#endif   
#ifdef PCWINDOWS  
   ReleaseDC(tableWindow,tableDC);
   tableDC = NULL;
   DestroyWindow(tableWindow);
#endif   

}
/*___cleanuptable()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*****searchtablecells()******************************************************/
int    searchtablecells(int how) /* <- MAGEDLOG/SearchDialog() */
{
   int    kmax=0,ifoundpart=0,ifoundcell=0,ireturn=0;
   int    col = 0, row = 0;
#ifdef OLDCODE
   int    k=0,kk=0,j = 0;
#endif
   pointstructptr currentpointptr;
   
   if(isearch1>0) uctolcstr(search1str);
   if(isearch2>0) uctolcstr(search2str);
   /*loop over all table items*/
   for(row = 0; row <= MAXROW; row++)
   {/*loop over rows, the y, vertical, direction*/
      for(col = 0; col <= MAXCOL; col++)
      {/*loop over columns, the x, horizontal, direction*/
        /*retrieve the kinemage data point that is associated with this cell*/
        currentpointptr = tablepointptr(row,col);

        if(currentpointptr != NULL)
        {/*cell has designated contents in the table*/

           /*copy char str from storage */
           if(how == 1)
           {/*search for match in cell contents themselves, i.e. the <comment>*/
              getcommentstring(trystr, currentpointptr);/*MAGELIST gets wordcom[]*/
              /*strip off 3 byte format info on front of string*/
              for(kmax=0;kmax<256;kmax++)
              {
                 trystr[kmax] = trystr[kmax+3];
                 if(trystr[kmax] == '\n' || trystr[kmax] == '\r') trystr[kmax] = ' ';
                 if(trystr[kmax]=='\0') break;
              }/*kmax is size of plain str comment*/         
           }
           else
           {/*search for match in cell's point ID, i.e. the {ptID} */
              getptIDstring(trystr, currentpointptr);/*MAGELIST*/
              for(kmax=0;kmax<256;kmax++)
              {
                 if(trystr[kmax]=='\0') break;
              }/*kmax is size of pointID str*/         
           }
           if(kmax > 0)
           {/*trial str not null*/ /*000410*/
             uctolcstr(trystr); /*ignore case */ 
             if(isearch1 > 0)
             {/*compare with first search string*/
              ifoundpart = matchstrings(trystr,search1str); /*MAGEUTIL.c*/
             }/*compare with first search string*/
             if(ifoundpart && isearch2>0)
             {/*compare with second search string*/
              ifoundpart = matchstrings(trystr,search2str); /*MAGEUTIL.c*/
             }/*compare with second search string*/

             if(ifoundpart)
             {
                tablepick(row,col) = 1; /*protect or amplify row 0, col 0 ???? */
                ifoundcell = 1;
             }
           }/*trial str not null*/ /*000410*/

        }/*cell has designated contents in the table*/

      }/*loop over columns, the x, horizontal, direction*/

   }/*loop over rows, the y, vertical, direction*/

   if(ifoundcell)
   {
       Lsearchatend = 1;
       Lpick = 1; /* flag for successful pick */
       ipick = 0; /*so will not try to find a pickpoint by cursor x,y*/
       ireturn = 1;
   }
   return(ireturn);
}
/*____searchtablecells()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****addtablemarker()*********************************************************/
void    addtablemarker(struct pointstruct* subjectpointptr)
{
   if(tablemarkergrupptr==NULL)
   {
      if( !allocgrupstructure() )
      {
          sprintf(alertstr,"failed to alloc group for table markers");
          goto allocerr;
      }
      else
      {        
          thisgrupptr->on = 1; /*group display off*/
          thisgrupptr->type = 0; /*nothing special*/
          thisgrupptr->firstsgrpptr = NULL;  
          thisgrupptr->lastsgrpptr = NULL;
          thisgrupptr->STATUS = thisgrupptr->STATUS | DOMINANT; 
              /* markers group is by definition dominant, no buttons below */
          thisgrupptr->master = 0; /*no master*/
          sprintf(thisgrupptr->name,"tablemarks");
          tablemarkergrupptr = thisgrupptr;
          /*ignoring items like NDIM, not relevant to tables? 060622*/
      }
   }
   if(tablemarkergrupptr!=NULL && tablemarkersgrpptr==NULL)
   {
      if( !allocsgrpstructure() )
      {
          sprintf(alertstr,"failed to alloc subgroup for table markers");
          goto allocerr;
      }
      else
      {        
          thissgrpptr->on = 1; /*subgroup display on*/
          thissgrpptr->firstlistptr = NULL;  
          thissgrpptr->lastlistptr = NULL;
          thissgrpptr->STATUS = thissgrpptr->STATUS | NOBUTTON;
          /* no button to be made for invented markers subgroup */
          thissgrpptr->grupptr = tablemarkergrupptr;
          tablemarkergrupptr->firstsgrpptr = thissgrpptr;  
          tablemarkergrupptr->lastsgrpptr  = thissgrpptr;
          sprintf(thissgrpptr->name,"tablemarks");
          tablemarkersgrpptr = thissgrpptr;
          /*ignoring items like NDIM, not relevant to tables? 060622*/
      }
   }
   if(tablemarkergrupptr!=NULL && tablemarkersgrpptr!=NULL && tablemarkerlistptr==NULL)
   {
      if( !allocliststructure() )
      {
          sprintf(alertstr,"failed to alloc lists for table markers");
          goto allocerr;
      }
      else
      {       
        thislistptr->on = 1; /*list display on*/
        thislistptr->firstpointptr = NULL;
        thislistptr->lastpointptr = NULL;
        thislistptr->color = numberofcolor("white"); /*MAGECOLR.c*/
        thislistptr->sgrpptr = tablemarkersgrpptr;
        tablemarkersgrpptr->firstlistptr = thislistptr;
        tablemarkersgrpptr->lastlistptr  = thislistptr;
        sprintf(thislistptr->name,"tablemarks");
        tablemarkerlistptr = thislistptr;
          /*ignoring items like NDIM, not relevant to tables? 060622*/
      }
   }
   if(tablemarkerlistptr!=NULL)
   {
      thispointptr = allocpointstructure(tablemarkerlistptr);
      if(thispointptr == NULL)
      {
         sprintf(alertstr,"failed to alloc point for a table marker");
         goto allocerr;
      }
      else
      {/*allocation routine hooks point up into tablemarkerlist */
          /*ignoring items like NDIM, coordNptr not relevant to tables? 060622*/
         thispointptr->type = thispointptr->type|MARKTYPE;
         thispointptr->fx = subjectpointptr->fx;
         thispointptr->fy = subjectpointptr->fy;
         thispointptr->fz = subjectpointptr->fz;
         thispointptr->ix = subjectpointptr->ix;
         thispointptr->iy = subjectpointptr->iy;
         thispointptr->iz = subjectpointptr->iz;
         thispointptr->STYLE = tablesearchrefpointptr->STYLE;
         thispointptr->radius = tablesearchrefpointptr->radius;
         if(tablesearchrefpointptr->colorwidth != 0)
         {
            thispointptr->colorwidth = tablesearchrefpointptr->colorwidth;
         }
         else
         {
            thispointptr->colorwidth = (tablesearchrefpointptr->listptr)->color;
         }
         if(thispointptr->STYLE == 0 || thispointptr->radius < 0.001)
         {/*regular plain marks get a set radius, all marks need some radius*/
            thispointptr->STYLE = SQUARESTYLE; /*010916, not DIAMONDSTYLE*/
            if(tablemarkradius > 0.001)/*needs to be an inputed variable*/
               thispointptr->radius = tablemarkradius; 
            else
               thispointptr->radius = 0; /*invokes screen adjusted marker*/ /*010916*/ 
         }/*regular plain marks get a set radius, all marks need some radius*/
         else if((thispointptr->STYLE & FANSTYLE) == FANSTYLE)
         {/*should have more info about angle of the fan spine*/
            if(((thispointptr->moreinfoptr) = allocmoreinfostructure()) != NULL)
            {

               if( (tablesearchrefpointptr->moreinfoptr) != NULL)
               {/*there is more info about fan spine angle available*/

                  (thispointptr->moreinfoptr)->f1 
                     = (tablesearchrefpointptr->moreinfoptr)->f1;
                  (thispointptr->moreinfoptr)->f2 
                     = (tablesearchrefpointptr->moreinfoptr)->f2;
               }
               else
               {
 sprintf(alertstr,"was NOT moreinfoptr for picked cell identified graphics marker\n");
               }
            }
            else
            {
 sprintf(alertstr,"failed to alloc moreinfoptr for picked cell identified graphics marker\n");
            }   
         }
         if(thispointptr->STYLE == 0)
         {/*regular plain marks get a plain vanilla color*/
            thispointptr->colorwidth = numberofcolor("white"); /*MAGECOLR.c*/
         }/*regular plain marks get a plain vanilla color*/

      }
      if(Ltablemarkers == 0)
      {
         Ltablemarkers = 1; 
         SetsinGrafWindow();
      }
   }
   return;
   allocerr:
   { 
        dosinglealert(0); /*shows alertstr[] */ /*____DLOG.C*/
        Ltablemarkers = 0;
        return;
   }

}
/*___addtablemarker()_______________________________________________________*/

/****killtablemarkers()******************************************************/
void killtablemarkers()
{
    if(tablemarkergrupptr != NULL)
    {/*take care of all the marker things that are ever visible*/
        destroygrupstructure(tablemarkergrupptr);
        /*this NULLs all members and reconnects nonmember linkages*/
        /*but does NOT set the specific alias pointers to NULL !! */
        tablemarkergrupptr = NULL;
        tablemarkersgrpptr = NULL;
        tablemarkerlistptr = NULL; 
    }
    Ltablemarkers = 0;
}
/*___killtablemarkers()__________________________________________________________*/

/****removetablemarkpoints()******************************************************/
void removetablemarkpoints()
{
  
  if(tablemarkerlistptr!=NULL)
  {
    /*destroy member points*/    
    if (tablemarkerlistptr->firstpointptr != NULL)
    {/*first member exists*/
       do   /*test after loop so "==" case will also be destroyed*/
       {
          destroypointstructure(tablemarkerlistptr->firstpointptr);
          /*this redefines the list's first and last points*/
        }while(    tablemarkerlistptr->firstpointptr != NULL );
     }/*first member exists*/ 
  }
}
/*___removetablemarkpoints()_____________________________________________________*/

/*****markfromtablecellsID()******************************************************/
int    markfromtablecellsID(int how) 
{
   int    ireturn=0;
   int    col = 0, row = 0;
   int    Nblank = 0;  /*000410*/
   
   pointstructptr currentpointptr;

   search1str[0] = '\0';
   isearch1 = 0;
   
   /*loop over all table items*/
   for(row = 0; row <= MAXROW; row++)
   {/*loop over rows, the y, vertical, direction*/
      for(col = 0; col <= MAXCOL; col++)
      {/*loop over columns, the x, horizontal, direction*/
         if(tablepick(row,col) == 1)
         {/*cell is in picked state*/
            /*retrieve the kinemage data point that is associated with this cell*/
            currentpointptr = tablepointptr(row,col);

            if(currentpointptr != NULL)
            {/*cell has designated contents in the table*/

               /*copy char str from storage */
               if(how == 1)
               {/*get search str from cell's point ID, i.e. the {ptID} */
                  getptIDstring(search1str, currentpointptr);/*MAGELIST*/
                  Nblank = 0; /*reset blank counter 010102*/
                  for(isearch1=0;isearch1<256;isearch1++)
                  {
                     if(search1str[isearch1] == ' ') Nblank++;/*000410*/
                     if(search1str[isearch1]=='\0') break;
                  }/*isearch1 is size of pointID str*/         
               }
               if(isearch1 == Nblank) isearch1 = 0; /*str all blanks 000410*/
               if(isearch1>0)
               {/*invoke regular graphics FIND in multiple hit mode*/
                  search2str[0] = '\0';
                  isearch2 = 0;
                  Lsearchbegin = 1;
                  tablesearchrefpointptr = currentpointptr;
                  Ltablesearchgraphics = 1; /*puts marker on graphics point*/
                  mysearchpointID(0);/*MAGEUTIL.c not justone hit*/
                  Ltablesearchgraphics = 0;
               }/*invoke regular graphics FIND in multiple hit mode*/
            }/*cell has designated contents in the table*/
         }/*cell is in picked state*/

      }/*loop over columns, the x, horizontal, direction*/

   }/*loop over rows, the y, vertical, direction*/
   ireturn = 1;
   return(ireturn);
}
/*____markfromtablecellsID()_____________________________________________________*/

