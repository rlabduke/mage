/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGECOLR.c*****************************************************************/
/*MAGECOLR.c*/
#include "MAGE.h"
#include "MAGELIST.h"
#define EXTERNCOLR
#include "MAGECOLR.h"
 
/****decodecolor()***********************************************************/
int decodecolor(char b[256], int i)
{  
   /*specified on point's line, e.g.:  {pointID} L (AFJ)  0.0 0.0 0.0 */
   /*would make whatever drawn to that point be red,green,blue for 3 aspects*/
      /*i is the actual position, starting at 1, to be decoded*/
      int ireturn = 0;
      if(i > 0) /*i==0 should never get here, but check to be safe.*/
      {
        i = i - 1;
        /* now i is an index, starting at 0, the aspects character string*/
      }
      if     (b[i]=='A'){ireturn= 1;} /*"red"*/
      else if(b[i]=='B'){ireturn= 2;} /*"orange","rust"*/
      else if(b[i]=='C'){ireturn= 3;} /*"gold"*/
      else if(b[i]=='D'){ireturn= 4;} /*"yellow"*/
      else if(b[i]=='E'){ireturn= 5;} /*"lime"*/
      else if(b[i]=='F'){ireturn= 6;} /*"green"*/
      else if(b[i]=='G'){ireturn= 7;} /*"sea","seagreen"*/
      else if(b[i]=='H'){ireturn= 8;} /*"cyan"*/
      else if(b[i]=='I'){ireturn= 9;} /*"sky","skyblue"*/
      else if(b[i]=='J'){ireturn=10;} /*"blue"*/
      else if(b[i]=='K'){ireturn=11;} /*"purple"*/
      else if(b[i]=='L'){ireturn=12;} /*"magenta"*/
      else if(b[i]=='M'){ireturn=13;} /*"hotpink"*/
     /*121105 deleted extraneous parentheses of equality comparison ((.==.))*/ 
      else if(b[i]=='N'){ireturn=14;} /*"pink"*/
      else if(b[i]=='O'){ireturn=15;} /*"lilac"*/
      else if(b[i]=='P'){ireturn=16;} /*"peach"*/
      
      else if(b[i]=='Q'){ireturn=17;} /*"peachtint"*/
      else if(b[i]=='R'){ireturn=18;} /*"yellowtint","paleyellow"*/
      else if(b[i]=='S'){ireturn=19;} /*"greentint"*/
      else if(b[i]=='T'){ireturn=20;} /*"bluetint"*/
      else if(b[i]=='U'){ireturn=21;} /*"lilactint"*/
      else if(b[i]=='V'){ireturn=22;} /*"pinktint"*/
      
      else if(b[i]=='W'){ireturn=23;} /*"white"*/
      else if(b[i]=='X'){ireturn=24;} /*"gray","grey"*/
      else if(b[i]=='Y'){ireturn=25;} /*"brown"*/
      else if(b[i]=='Z'){ireturn=31;} /*"invisible"*/
      else { ireturn = 0;} /*no valid index recognized*/
      return ireturn; /*actual color index: 1---25 + invisible==31*/
      /*number 0 for no entry, 1-31 for Mage Colors coded by number*/
      /*actual depth-cued color numbers: 1---25*/
      /*translated into palette index in sets of 25*/
      /*5 sets for black background, 5 for white*/
      /*palette index: deadblack 0, deadwhite 255*/
}
/*___decodecolor()__________________________________________________________*/

/****interpretcolor()********************************************************/
void    interpretcolor(int i)  
{ 
    
  /*if     (i== 0) sprintf(word,"black"     );*/
  /*color integer should always be > 0 when this routine called*/
    if     (i== 1) sprintf(word,"red"       );
    else if(i== 2) sprintf(word,"orange"    );
    else if(i== 3) sprintf(word,"gold"      );
    else if(i== 4) sprintf(word,"yellow"    );
    else if(i== 5) sprintf(word,"lime"      );
    else if(i== 6) sprintf(word,"green"     );
    else if(i== 7) sprintf(word,"sea"       );
    else if(i== 8) sprintf(word,"cyan"      );
    else if(i== 9) sprintf(word,"sky"       );
    else if(i==10) sprintf(word,"blue"      );
    else if(i==11) sprintf(word,"purple"    );
    else if(i==12) sprintf(word,"magenta"   );
    else if(i==13) sprintf(word,"hotpink"   );

    else if(i==14) sprintf(word,"pink"      );
    else if(i==15) sprintf(word,"lilac"     );
    else if(i==16) sprintf(word,"peach"     );
 
    else if(i==17) sprintf(word,"peachtint" );
    else if(i==18) sprintf(word,"yellowtint");
    else if(i==19) sprintf(word,"greentint" );
    else if(i==20) sprintf(word,"bluetint"  );
    else if(i==21) sprintf(word,"lilactint" );
    else if(i==22) sprintf(word,"pinktint"  );
   
    else if(i==23) sprintf(word,"white"     );
    else if(i==24) sprintf(word,"gray"      );
    else if(i==25) sprintf(word,"brown"     );
    
    else if(i==29) sprintf(word,"deadwhite" );
    else if(i==30) sprintf(word,"deadblack" );
    else if(i==31) sprintf(word,"invisible" );
    
    else           sprintf(word,"white"     ); /*or color not specified*/
}
/*___interpretcolor()_______________________________________________________*/

/****colorcode()********************************************************/
char    colorcode(int i)  
{ 
   char creturn;
   switch(i)
   {
  /*case  0: creturn = ' '; break;*/ /*black*/
    case  1: creturn = 'A'; break; /*red*/
    case  2: creturn = 'B'; break; /*orange*/
    case  3: creturn = 'C'; break; /*gold*/
    case  4: creturn = 'D'; break; /*yellow*/
    case  5: creturn = 'E'; break; /*lime*/
    case  6: creturn = 'F'; break; /*green*/
    case  7: creturn = 'G'; break; /*sea*/
    case  8: creturn = 'H'; break; /*cyan*/
    case  9: creturn = 'I'; break; /*sky*/
    case 10: creturn = 'J'; break; /*blue*/
    case 11: creturn = 'K'; break; /*purple*/
    case 12: creturn = 'L'; break; /*magenta*/
    case 13: creturn = 'M'; break; /*hotpink*/

    case 14: creturn = 'N'; break; /*pink*/
    case 15: creturn = 'O'; break; /*lilac*/
    case 16: creturn = 'P'; break; /*peach*/

    case 17: creturn = 'Q'; break; /*peachtint*/
    case 18: creturn = 'R'; break; /*yellowtint*/
    case 19: creturn = 'S'; break; /*greentint*/
    case 20: creturn = 'T'; break; /*bluetint*/
    case 21: creturn = 'U'; break; /*lilactint*/
    case 22: creturn = 'V'; break; /*pinktint*/
    
    case 23: creturn = 'W'; break; /*white*/
    case 24: creturn = 'X'; break; /*gray*/
    case 25: creturn = 'Y'; break; /*brown*/

  /*case 26: creturn = ' '; break;*/ /* */
  /*case 27: creturn = ' '; break;*/ /* */
  /*case 28: creturn = ' '; break;*/ /* */
  /*case 29: creturn = ' '; break;*/ /*deadwhite*/
  /*case 30: creturn = ' '; break;*/ /*deadblack*/

    case 31: creturn = 'Z'; break; /*invisible*/
    
    default: creturn = 'W'; break; /*white*/
   }
   return(creturn);
}
/*___colorcode()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****numberofcolor()**********************************************************/
int numberofcolor(char cstr[256]) /*large enough to match generic word[256]*/
{
   int  i,ireturn;
   ireturn = 0; /*set for NO MATCH FOUND */
   i = strlen(cstr); /*char count in color name, excluding the \0 */
   /*default color == white on black or black on white*/
   /* strncmp returns as 0 if matched color, and so sets ireturn */
   /*return is color index number, except 0 for no match, so */
   /*default of white/black for no match done in calling program so */
   /*use this routine to distinguish point color names from other parameters*/
   /*but "default" in now a valid color name for white so */
   /*color name vs number bookkeeping is done here*/

   if     (!strncmp(cstr,"red"       ,i) ) { ireturn= 1;}
   else if(!strncmp(cstr,"orange"    ,i) ) { ireturn= 2;}
   else if(!strncmp(cstr,"rust"      ,i) ) { ireturn= 2;}
   else if(!strncmp(cstr,"gold"      ,i) ) { ireturn= 3;}
   else if(!strncmp(cstr,"yellow"    ,i) ) { ireturn= 4;}
   else if(!strncmp(cstr,"lime"      ,i) ) { ireturn= 5;}
   else if(!strncmp(cstr,"green"     ,i) ) { ireturn= 6;}
   else if(!strncmp(cstr,"sea"       ,i) ) { ireturn= 7;}
   else if(!strncmp(cstr,"seagreen"  ,i) ) { ireturn= 7;}
   else if(!strncmp(cstr,"cyan"      ,i) ) { ireturn= 8;}
   else if(!strncmp(cstr,"sky"       ,i) ) { ireturn= 9;}
   else if(!strncmp(cstr,"skyblue"   ,i) ) { ireturn= 9;}
   else if(!strncmp(cstr,"blue"      ,i) ) { ireturn=10;}
   else if(!strncmp(cstr,"purple"    ,i) ) { ireturn=11;}
   else if(!strncmp(cstr,"magenta"   ,i) ) { ireturn=12;}
   else if(!strncmp(cstr,"hotpink"   ,i) ) { ireturn=13;}
   
   else if(!strncmp(cstr,"pink"      ,i) ) { ireturn=14;}
   else if(!strncmp(cstr,"lilac"     ,i) ) { ireturn=15;}
   else if(!strncmp(cstr,"peach"     ,i) ) { ireturn=16;}
    /*short names first, nametint after, with char count*/
   else if(!strncmp(cstr,"peachtint" ,i) ) { ireturn=17;}
   else if(!strncmp(cstr,"paleyellow",i) ) { ireturn=18;}
   else if(!strncmp(cstr,"yellowtint",i) ) { ireturn=18;}
   else if(!strncmp(cstr,"greentint" ,i) ) { ireturn=19;}
   else if(!strncmp(cstr,"bluetint"  ,i) ) { ireturn=20;}
   else if(!strncmp(cstr,"lilactint" ,i) ) { ireturn=21;}
   else if(!strncmp(cstr,"pinktint"  ,i) ) { ireturn=22;}
   
   else if(!strncmp(cstr,"white"     ,i) ) { ireturn=23;} 
   else if(!strncmp(cstr,"default"   ,i) ) { ireturn=23;}
   else if(!strncmp(cstr,"gray"      ,i) ) { ireturn=24;}
   else if(!strncmp(cstr,"grey"      ,i) ) { ireturn=24;}
   else if(!strncmp(cstr,"brown"     ,i) ) { ireturn=25;}
   
   else if(!strncmp(cstr,"invisible" ,i) ) { ireturn=31;}
   else if(!strncmp(cstr,"deadwhite" ,i) ) { ireturn=29;}
   else if(!strncmp(cstr,"black"     ,i) ) { ireturn=30;}
   else if(!strncmp(cstr,"deadblack" ,i) ) { ireturn=30;}
   /*deadblack will eventually index into palette 0th entry*/
   else { ireturn= 0;}
   /*not recognized, return 0, let calling program call again for default*/
   return(ireturn);
}
/*___numberofcolor()_________________________________________________________*/
/****loadmyfullpalette()******************************************************/
void loadmyfullpalette(int itype)
{
   int j=0, i=0;

   if(itype==1)
   {
      for(j=0; j<MageRGBtableSIZE; j++)
      {
         for(i=0; i<3; i++)
         {
            myfullpalette[j][i] = mageorigpalette[j][i];
         }
      }
   }
   else
   {                 /* name    ,  Hue   Blk sat, Wht sat, Blk val, Wht val */
      RGBsetFromOneHSV("default",   0,      0,      0,      100,    0);
      RGBsetFromOneHSV("red",       0,      100,    100,    100,    80); 
      RGBsetFromOneHSV("orange",    20,     100,    100,    100,    90);
      RGBsetFromOneHSV("rust",      20,     100,    100,    100,    90);
      RGBsetFromOneHSV("gold",      40,     100,    100,    100,    90);
      RGBsetFromOneHSV("yellow",    60,     100,    100,    100,    90);
      RGBsetFromOneHSV("lime",      80,     100,    100,    100,    85);
      RGBsetFromOneHSV("green",     120,    80,     90,     100,    75);
      RGBsetFromOneHSV("sea",       150,    100,    100,    100,    85);
      RGBsetFromOneHSV("seagreen",  150,    100,    100,    100,    85);
      RGBsetFromOneHSV("cyan",      180,    100,    85,     85,     80);
      RGBsetFromOneHSV("sky",       210,    75,     80,     95,     90);
      RGBsetFromOneHSV("skyblue",   210,    75,     80,     95,     90);
      RGBsetFromOneHSV("blue",      240,    70,     80,     100,    100);
      RGBsetFromOneHSV("purple",    275,    75,     100,    100,    85);
      RGBsetFromOneHSV("magenta",   300,    95,     100,    100,    90);
      RGBsetFromOneHSV("hotpink",   335,    100,    100,    100,    90);
      RGBsetFromOneHSV("pink",      350,    55,     75,     100,    90);
      RGBsetFromOneHSV("peach",     25,     75,     75,     100,    90);
      RGBsetFromOneHSV("lilac",     275,    55,     75,     100,    80);
      RGBsetFromOneHSV("pinktint",  340,    30,     100,    100,    55);
      RGBsetFromOneHSV("peachtint", 25,     50,     100,    100,    60);
      RGBsetFromOneHSV("yellowtint",60,     50,     100,    100,    75);
      RGBsetFromOneHSV("paleyellow",60,     50,     100,    100,    75);
      RGBsetFromOneHSV("greentint", 135,    40,     100,    100,    35);
      RGBsetFromOneHSV("bluetint",  220,    40,     100,    100,    50);
      RGBsetFromOneHSV("lilactint", 275,    35,     100,    100,    45);
      RGBsetFromOneHSV("white",     0,      0,      0,      100,    0);
      RGBsetFromOneHSV("gray",      0,      0,      0,      50,     40);
      RGBsetFromOneHSV("grey",      0,      0,      0,      50,     40);
      RGBsetFromOneHSV("brown",     20,     45,     45,     75,     55);
   }
}
/*___loadmyfullpalette()_____________________________________________________*/

/****RGBsetFromOneHSV()*******************************************************/
void RGBsetFromOneHSV(char* name, float hue, float blackSat, float whiteSat,
      float blackVal, float whiteVal)
{
    float whitevalue=0;
    int   num=0;
 static int  bcolors[5][3],wcolors[5][3];
    
    hue = hue/360;
    blackSat = blackSat/100;
    whiteSat = whiteSat/100;
    blackVal = blackVal/100;
    whiteVal = whiteVal/100;
     
    /* value decreases going back */
    getRGB(bcolors[0], hue, blackSat, (float)0.36*blackVal); /*back*/
    getRGB(bcolors[1], hue, blackSat, (float)0.52*blackVal);
    getRGB(bcolors[2], hue, blackSat, (float)0.68*blackVal);
    getRGB(bcolors[3], hue, blackSat, (float)0.84*blackVal);
    getRGB(bcolors[4], hue, blackSat, (float)1.00*blackVal); /*front*/
     
    /* value increases, saturation decreases going back */
    whitevalue= (float)0.60 +whiteVal*(float)0.40; 
    if(whitevalue > 1) {whitevalue = 1;}
    getRGB(wcolors[0], hue, (float)0.36*whiteSat, whitevalue); /*back*/
    whitevalue= (float)0.45 +whiteVal*(float)0.55; 
    if(whitevalue > 1) {whitevalue = 1;}
    getRGB(wcolors[1], hue, (float)0.52*whiteSat, whitevalue);
    whitevalue= (float)0.30 +whiteVal*(float)0.70; 
    if(whitevalue > 1) {whitevalue = 1;}
    getRGB(wcolors[2], hue, (float)0.68*whiteSat, whitevalue);
    whitevalue= (float)0.15 +whiteVal*(float)0.85; 
    if(whitevalue > 1) {whitevalue = 1;}
    getRGB(wcolors[3], hue, (float)0.84*whiteSat, whitevalue);
    whitevalue= (float)0.00 +whiteVal*(float)1.00; 
    if(whitevalue > 1) {whitevalue = 1;}
    getRGB(wcolors[4], hue, (float)1.00*whiteSat, whitevalue); /*front*/
    
    num = numberofcolor(name);
    if(num >= 1 && num <= 25)
    {/*name resolves to valid palette number: load as entry in palette*/
       myfullpalette[num][0] = bcolors[0][0];
       myfullpalette[num][1] = bcolors[0][1];
       myfullpalette[num][2] = bcolors[0][2];
       myfullpalette[num+25][0] = wcolors[0][0];
       myfullpalette[num+25][1] = wcolors[0][1];
       myfullpalette[num+25][2] = wcolors[0][2];

       myfullpalette[num+50][0] = bcolors[1][0];
       myfullpalette[num+50][1] = bcolors[1][1];
       myfullpalette[num+50][2] = bcolors[1][2];
       myfullpalette[num+50+25][0] = wcolors[1][0];
       myfullpalette[num+50+25][1] = wcolors[1][1];
       myfullpalette[num+50+25][2] = wcolors[1][2];

       myfullpalette[num+100][0] = bcolors[2][0];
       myfullpalette[num+100][1] = bcolors[2][1];
       myfullpalette[num+100][2] = bcolors[2][2];
       myfullpalette[num+100+25][0] = wcolors[2][0];
       myfullpalette[num+100+25][1] = wcolors[2][1];
       myfullpalette[num+100+25][2] = wcolors[2][2];

       myfullpalette[num+150][0] = bcolors[3][0];
       myfullpalette[num+150][1] = bcolors[3][1];
       myfullpalette[num+150][2] = bcolors[3][2];
       myfullpalette[num+150+25][0] = wcolors[3][0];
       myfullpalette[num+150+25][1] = wcolors[3][1];
       myfullpalette[num+150+25][2] = wcolors[3][2];

       myfullpalette[num+200][0] = bcolors[4][0];
       myfullpalette[num+200][1] = bcolors[4][1];
       myfullpalette[num+200][2] = bcolors[4][2];
       myfullpalette[num+200+25][0] = wcolors[4][0];
       myfullpalette[num+200+25][1] = wcolors[4][1];
       myfullpalette[num+200+25][2] = wcolors[4][2];
   }
}
/*___RGBsetFromOneHSV()______________________________________________________*/

/****getRGB()*****************************************************************/
void getRGB(int rgb[3], float hue, float saturation, float brightness)
{/*get RGB as 0---255  from HSV as 0.0 --- 1.0 */
   float huesixth=0, huefudge=0, pfudge=0, qfudge=0, tfudge=0;
   int   huebin = 0;

   /*Divide Hue wheel into 6 regions*/
   /*each region has an idiosyncratic way of computing RGB from HSV*/
   huesixth = hue*6; /*huesixth range is 0.0 --- 6.0 */
   huebin = (int)huesixth; /*truncate to integer == floor*/
   if(huebin > 5){huebin = 5;} /*avoid top end artifact*/
   if(huebin < 0){huebin = 0;} 
   huefudge = huesixth - (int)huesixth;/*fraction within a sixth of the wheel*/
   pfudge = brightness * ((float)1.0 - (saturation));
   qfudge = brightness * ((float)1.0 - (saturation * huefudge));
   tfudge = brightness * ((float)1.0 - (saturation * ((float)1.0 - huefudge)));

   switch(huebin)
   {
      case 0:
         rgb[0] = (int)(brightness * 255 + 0.5);
         rgb[1] = (int)(tfudge * 255 + 0.5);
         rgb[2] = (int)(pfudge * 255 + 0.5);
      break;
      case 1:
         rgb[0] = (int)(qfudge * 255 + 0.5);
         rgb[1] = (int)(brightness * 255 + 0.5);
         rgb[2] = (int)(pfudge * 255 + 0.5);
      break;
      case 2:
         rgb[0] = (int)(pfudge * 255 + 0.5);
         rgb[1] = (int)(brightness * 255 + 0.5);
         rgb[2] = (int)(tfudge * 255 + 0.5);
      break;
      case 3:
         rgb[0] = (int)(pfudge * 255 + 0.5);
         rgb[1] = (int)(qfudge * 255 + 0.5);
         rgb[2] = (int)(brightness * 255 + 0.5);
      break;
      case 4:
         rgb[0] = (int)(tfudge * 255 + 0.5);
         rgb[1] = (int)(pfudge * 255 + 0.5);
         rgb[2] = (int)(brightness * 255 + 0.5);
      break;
      case 5:
         rgb[0] = (int)(brightness * 255 + 0.5);
         rgb[1] = (int)(pfudge * 255 + 0.5);
         rgb[2] = (int)(qfudge * 255 + 0.5);
      break;
   }  
   if(rgb[0] > 255) {rgb[0] = 255;}
   if(rgb[1] > 255) {rgb[1] = 255;}
   if(rgb[2] > 255) {rgb[2] = 255;}
   if(rgb[0] < 0) {rgb[0] = 0;}
   if(rgb[1] < 0) {rgb[1] = 0;}
   if(rgb[2] < 0) {rgb[2] = 0;}
}
/*___getRGB()________________________________________________________________*/

/****loadmygrayscalepalette()*************************************************/
void loadmygrayscalepalette(void)
{
   int i=0,j=0;
   float fgrayvalue=0;

   for(j=0; j<MageRGBtableSIZE; j++)
   {
      /*Ian's magic formula, supposedly stolen from POV-ray to get a gray tone*/
      fgrayvalue =   (float)0.297*(float)myfullpalette[j][0]
                   + (float)0.589*(float)myfullpalette[j][1]
                   + (float)0.114*(float)myfullpalette[j][2] ;
      if(fgrayvalue < 0 ) fgrayvalue = 0;
      if(fgrayvalue > 255) fgrayvalue = 255;

      for(i=0; i<3; i++)
      {
         mygrayscalepalette[j][i] = (int)fgrayvalue;
      }
   }
}
/*___loadmygrayscalepalette()________________________________________________*/

