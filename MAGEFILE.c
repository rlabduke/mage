/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MAGEFILE.c*****************************************************************/
 
#include "MAGE.h"
#include "MAGELIST.h"
#include "MAGEANGL.h"
#include "MAGEBBOX.h"
#include "MAGETABL.h"
#include "MAGEFLAG.h"

 
/****GetTextFromFile()*******************************************************/
void    GetTextFromFile()
{
   char  texts[256],buffs[256];
   int   itext=0,ibuff=0;
   int   c=0,i=0;
   int   cm0=0,cm1=0,cm2=0,cm3=0;
   int   hyperflag=0,quesflag=0;
   int   n=0,LCRLF=0,ido=0,Lastchar=0;
   int   flowID=0;  /* switch controls for input */
   enum
   {
       neutID = 1,
       keywID,
       textID,
       kineID,
       mageID    /*mage #.#  mage version number of kin file*/
   };
   int   Ltext=0, Lkine=0, Lmage=0, Lnew=1;  /*970503*/
           /* Logicals for input flow */
   int   Ltextstate=0; 
   int   Ltextfound=0; 
   int   Lholdstate=0;
           /* Logical to help allow @ to be a character inside of text */
   int ncacceptable=0; /*counter to report non flat ascii once*/
   int icacceptable=0;  /*for flat ascii acceptable characters*/
     
 if(IOerr == 0) /*so why should GetTextFromFile() be called unless valid file?*/
 { /*valid input file*/
   origkinenum = -99; /*default for keeping track of kins in a file*/ 
   kinemagenum = 0; /*default for kinemage about to be read in*/
   Lorigkinfile = 1; /*track changes of input file */
   atEOF = 0;
   flowID = neutID;
   /* flow controls and logicals together direct the flow */
   LCRLF = 0; /*flag so can make CRLF -> CR */
   Lastchar = 0; /*flag_char so can tell if @ starts a line*/
   ido = 1; /*flag that allows a character to be considered*/
   cleartexts();         /* ____WIND.c */ /*need this!?*/
   Lquesanswers = 0; /*flag for questions in text part of kinemage*/ /*980923b*/

if(Ltest)
{sprintf(alertstr,"GetTextFromFile()"
"\015 Looking for kinemages in file: %s \015",NameStr);
dosinglealert(3);
}
   fpin = fp; /*only get text from a primary kinemage file*/
   while ((c = getnextchar() ) != EOF)
   /* #define getnextchar()  getc(fpin) in MAGE.H, fpin==fp here */
   {/*individual char input loop while file not at EOF */
     
     ido = 1; /*expect a valid character to be processed*/
     if((int)c < 127) /*lower half of ascii table, not inc del at 127*/
     {
        if((int)c > 31) {icacceptable = 1;} /*above the control set*/
        else if((int)c==9||(int)c==10||(int)c==13||(int)c==EOL) 
                        {icacceptable=1;} /*EOF-> EOL 980821*/
                  /*tab,LF,CR,or EOL if something else*/ /*EOF-> EOL 980821*/
                 /* maybe should be: \t,\n,\r ... ???? */ /*980821*/
        else            {icacceptable=0;}
     }
     else {icacceptable=0;}
     if(!icacceptable)
     {
        ncacceptable++;
        if(ncacceptable==1)
        {
            sprintf(alertstr,"non flat ascii character (%d) in text"
              CRLF"no further reports of this for text section"
              CRLF"Make sure file is saved as text file",(int)c);
            dosinglealert(2);
        }
     }
     if(icacceptable)
     {/*char acceptable*/    

       if (c == '\n' || c == '\r') /*some mixture of LF:\012 and CR:\015*/
       {/*CRLF character type*/
          /*whatever newline is, it needs to be just single CR in text */
          /*this section flattens \012\015 pairs, but respects singles*/
          if(LCRLF>0)
          {/*previous character was CRLF type*/
             if(c=='\012' && LCRLF=='\015')
             {/*already got a CR, ignore this LF*/
                ido = 0;
                LCRLF = 0; /*clear flag*/
             }
             else if(c=='\015' && LCRLF=='\012')
             {/*already got a LF, ignore this CR*/
                ido = 0;
                LCRLF = 0; /*clear flag*/
             }
             else
             {/*previous character the same, so serious new line again!*/
                c = EOL;
                LCRLF = 0; /*clear flag*/
                /*note if \n || \r are not either \012 || \015 */
                /*  this routine treats them as singles and makes a */
                /*  new line for each! */
             }
          }/*previous character was CRLF type*/
          else
          {/*a new occurance of CRLF character type*/
             if(c=='\012') LCRLF = '\012';  /*set flag*/
             else if(c=='\015') LCRLF = '\015';  /*set flag*/
             c = EOL;/*accept, and make standard new line indicator*/
          }
       }/*CRLF character type*/
       else LCRLF = 0; /*not CRLF: make sure flag is clear*/

       if (   ((c == '@') && (Lastchar == 0)) /*true if @ is 1st char of a file! */ /*980821*/
           || ((c == '@') && (Lastchar == '\r'))
           || ((c == '@') && (Lastchar == '\n'))
           || ((c == '@') && ((int)Lastchar==EOL))) /*121107 2more () levels to disambiguate*/
       {/*keywords @text and @kinemage (and @mage) must begin a new line*/

          flowID = keywID; /* need to get begin's key word */
          iword = 0;       /* set keyword character counter */
          if(Ltext)
          {
             Ltextstate = 1;
             /*set flag so if @... part of text it can be recovered*/
             ibuff = 0;  /* set counter to track this text segment */
          }
          if(Ltest)
          {
             sprintf(alertstr,"found @");dosinglealert(3);
          }
       }
       if(ido)
       {/*ido*/
        switch(flowID)
        {
          case neutID:
               /* neutral: no flow designated, discard characters */
          break;

          case textID:
            if(Lnew)
            {/*Lnew:initialize*/
                  
               Lappend = 0;/*presume whole new kinemage*/
               /*Can't do this earlier, since don't want to kill */
               /*previous text when doing an append - but useless here*/
               n = 0;
               i = 0;
               ibuff = 0;
               itext = 0;
               ntotaltext = 0;

               /*clean up Answer storage for putative Questions in text*/
               for(i=0;i<MAXQUESTIONS;i++) thisanswer[i][0] = '\0';
               nQuestion = 0; /*Question counter*/
               hyperflag = 0; /*signels hypertext control region*/
               quesflag = 0; /*signels question is active*/
               oldNqueskin = 0;
               oldNquesview = 0;
                
               Lkinemagecount = 0;/*as far as one knows at this stage*/
             
            }/*Lnew:initialize*/
            Lnew = 0;  /*only set ==1 when keyword is recognized*/
            /*accummulate 4 successive characters,allows special input*/
            cm3=cm2;cm2=cm1;cm1=cm0;cm0=c;
            /* text expected, send characters to textText */
            texts[itext] = c;
            if(   cm1=='*'&&cm0=='{'   /*balance}*/)  
            {
               hyperflag = 1;
               iQstring = 0;
            }
            if(/*balance{*/ cm1=='}'&&cm0=='*'  ) 
            {
               hyperflag = 0;
               Qstring[iQstring-1] = '\0';/*overwrite closing bracket*/
               Dotexthyperbutton(0);/* 0 for setup */ /*MAGEFILE.c*/
               if(quesflag)
               {
                  for(i=0;i<MAXQCHAR+2;i++) texts[++itext] = '_';
                      /* underscores: extra on each end*/
                  quesflag = 0;
               }
            }
            if(hyperflag>0)
            {
               hyperflag++;
               if(hyperflag>2) 
               {
                  if(iQstring >= MAXHYPER-2)
                  {
                     sprintf(alertstr,"*{hypertext field}* about to exceed %d,"
                        " no more characters will be accepted",MAXHYPER);
                     dosinglealert(2);
#ifdef UNIX_X11
        fprintf(stderr,"%s\n",alertstr); /*UNIX has functional console window*/
                                     /*which doesn't overlap graphics*/
                                     /*and allows program to be ^c killed*/
#endif

                  }
                  else
                  {
                     Qstring[iQstring++] = c;
                  }
               }
            }
            if(cm3=='*'&&cm2=='{'&&cm1=='Q'&&cm0==':'  /*balance}*/) 
            {/*Question, insert a question number */
               Lquesanswers = 1;/*flag that there are questions*/
               nQuestion++;
               if(nQuestion < MAXQUESTIONS)
               {
                  sprintf(word," %d ",nQuestion);
                  for(i=0;word[i]!='\0';i++) texts[++itext]=word[i];
                  for(i=0;word[i]!='\0';i++) Qstring[iQstring++]=word[i];
                  quesflag = 1;
               }
               else
               {
                  sprintf(alertstr,"*{Q: questions exceed %d, " /*balance}*/
                      "no more can be assigned numbers"
                      ,MAXQUESTIONS-1);  
                  dosinglealert(2);
               }
            }
            if (itext >= 200 || c == EOL) /*allows for insertions */ /*990209*/
            {
               texts[itext+1] = '\0'; /*make array of characters*/
                                   /* into properly ended C string*/
               inserttexts(texts); /*____WIND.c */
               itext = 0;
            }
            else
            {
               itext++;
               ntotaltext++;
            }
          break;
/*KEYWORD******case*******/          
          case keywID:
            /*try to interpret next set of characters as a key word*/
            if(Ltextstate && ibuff<255)
            {    /*in case this is not a key word*/
               buffs[ibuff] = c; /*continue to accummulate text */
               ibuff++;        /*in case this is not a key word*/
               if(hyperflag) Qstring[iQstring++] = c; 
                   /*continue to accumulate hypertext string */
            }
            /*@keyword must be contiguous characters as a single word*/

            if(c == '@')
            {
                ;  /* @ discarded */
            }
            else
            {/*looking only for text to start, kinemage to end, or mage #.#*/
               /*970503  @mage #.# : defines kinemage version level*/
               iword = iword + 1;
                   /* iword set = 0 when flowID set = keywID */
               word[iword] = c;
               word[iword] = tolower(word[iword]); /*c*/
                           /*case independent */
                  /*beware: ctype.h  redefines tolower wrong*/
               /*if( (c == ' ')||(c == '\015')||(c == ',')||*/ /*980821*/
               /*    (c == ';')||(iword > 8)||(c == '\n')     )*/ /*980821*/
               if(  c == ' '||c == ','||c == ';'||c == '\t'
                  ||iword > 8||c == EOL )  /*generic EOL established above*/ /*980821*/
               {   /*stop looking when hit white space or 8 char*/
                   /* pad ends so can treat word as a real string */
                  word[iword] = '\0';
                  word[0] = ' ';

                  /* try to interpret as a keyword string */
                  if(Ltest)            /*980816*/
                  {
                     sprintf(alertstr,"TEXT parser finds: %s",word);
                     dosinglealert(3);   
                  }                    /*980816*/   
                  
/*KEYWORD TEXT-->*/ /*search for text as key word, note leading space*/
                  if(!Ltext)
                  {/*NOT found text as a keyword yet*/
                     if(strncmp(word," text",5) == 0) 
                     {
                        Ltext = 1;
                        Ltextfound = 1; /*so @kinemage flag will not overwrite text*/
                     }
                     if(Ltext)
                     {/*CONSIDER THIS A VALID FILE WITH AT LEAST TEXT*/
                        /*clear text window! */
                        cleartexts();     /*____WIND.c */
                        flowID = textID;
                        Lnew = 1;/*for initiallizations*/
                        itext = 0;
                        Lkine = 0;
                        Lmage = 0;
                        /* text and kinemage figure are exclusive */
                        if(Ltest)
                        {
                           sprintf(alertstr,"Found @text");
                           dosinglealert(3);    
                        }
                     }
                  }/*NOT found text as a keyword yet*/

/*KEYWORD KINEMAGE-->*/ /* search for kinemage as key word,*/
                  Lkine = 0;            /* note leading space*/
                  if(strncmp(word," kinemage",9) == 0)
                  {
                     Lkine =1;
                  }
                  if(Lkine)
                  {
                     /* finish out anything in text buffer */
                     /*since @kinemage preceeded by an EOL, then */
                     /*the last block of text should already be in*/
                     /*(Blocks limit by size || EOL) and itext==0 */
                     /*texts[itext+1] = '\0';*/
                     texts[itext] = '\0';
                     /*completetext();????*/
                     if ( itext >  0 )
                     {
                        inserttexts(texts); /*____WIND.c*/
                        itext = 0; /*990209*/
                     }
                     completetext();/*MPCFILE.c, MACFILE.c(dummied)*/ /*980917 */
                     if(c == EOL) /*980823*/
                     {/*@kinemage has NO characters for a kinemage number*/
                        /*treat this like similar pathology of blank for number*/
                        if(Ltest||Lformattest)
                        {
                          sprintf(alertstr,"Format fault: this kinemage has no number");
                          dosinglealert(3); 
                        }
                        sprintf(KineName,"Kinemage  %d - ",kinemagenum);/*980823*/
                        origkinenum = kinemagenum; /*981207*/  
                           /*kinemagenum defaults to 0 */
                        Lappend = 0;/*presume whole new kinemage*/
                        Lkinfile = 1;
                        if(LGUI) AdjustMenus();/*MAGEMENU affect kinemage menu*/
                        goto finishedtext;/*JUMP OUT OF THIS PLACE*/
                     }/*980823*/
                     flowID = kineID;
                     Ltext = 0;  /* text and figure are exclusive*/
                     Lnew = 1; /*flag start of scan for kinemage number*/
                  }

/*KEYWORD MAGE-->*/ /*search for mage as key word, note leading space*/
                  if(strncmp(word," mage",5) == 0)
                  {/*@mage*/
                     Lholdstate = Ltextstate;
                     Ltextstate = 0; 
                        /*clear flag so mage # not added to text window*/

                     flowID = mageID;
                     Lmage = 1;
                     Lkine = 0;
                     Ltext = 0;
                     Lnew = 1;
                     if(Ltest)
                     {
                        sprintf(alertstr,"Found @mage");
                        dosinglealert(3);    
                     }
                  }

                  iword = 0;
                       
                  /* end try to interpret as control string */
/*KEYWORD real? */if(Lkine && Ltextstate) {Ltextstate = 0;}
                     /*clear flag so mage # not added to text window*/
                  if(Ltextstate && !Lkine && !Lmage) 
                  {/*not a keyword: write local key word buffer to text*/
                     for(n=0;n<ibuff;n++)
                     {
                        texts[itext] = buffs[n];
                        if (itext == 254 || c == '\015')
                        {
                           texts[itext+1] = '\0';     
                           inserttexts(texts);/*____WIND.c */
                           itext = 0;
                        }
                        else
                        {
                           itext++;
                           ntotaltext++;
                        }
                     }
                     flowID = textID; /* restore control */
                     Lnew = 0;/*for safety*/
                     Ltext = 1; /*restore control*/
                     ibuff = 0;
                  }/*not a keyword: write local key word buffer to text*/

               }/* stop looking when hit white space or 8 char max */
            }/*looking only for text to start, kinemage to end, or mage #.#*/
          break; /* case = keywID */
/*KEYWORD____case____*/           
          case mageID: 
            /* marks the only legitimate interruption of text section */
            /*when done, getfloat returns as 1, */
            if( getfloat(c,Lnew) ) /*MAGEINPT.c*/
            {
               KINVERSION = floatpass;
               if(Ltest)
               {
                  sprintf(alertstr,"Found @mage %.3f ",KINVERSION);
                  dosinglealert(3);   
               } 
               if(Lholdstate)
               { 
                  flowID = textID; /*restore flow state to text*/ 
                  Ltext = 1; /*restore flag*/
                  Lnew = 0;/*for safety*/
               }
               else 
               {
                  flowID = neutID;
               }
               Lmage = 0;  /*mage # is in, so clear this flag*/
                   
               if(KINVERSION > MAGEVERSION && (Ltest||Lformattest) )
               {
                  sprintf(alertstr,"This kinemage flagged to be made for MAGE %.3f "
                     CRLF"but this is only MAGE %.3f"
                    ,KINVERSION,MAGEVERSION);
                  dosinglealert(3);   
               }                     
            }
            Lnew = 0; /*only set ==1 when keyword is recognized*/
          break; /*case mageID*/
           
          case kineID: 
            /* marks the only legitimate end to the text section */
            /*when done, getkinemagenum returns as 1, */
            /*this ends text section */
            if( getkinemagenum((char)c,Lnew) ) /*MAGEFILE.c*/
            {
               Lappend = 0;/*presume whole new kinemage*/
               /*text buffer flushed above when @kinemage recognized as a keyword*/
               if(Ltextfound==0)
               {/*no text was found*/
                  sprintf(texts,CRLF" Looking for kinemages in file: %s"
                          CRLF,NameStr);
                  cleartexts();         /* ____WIND.c */
                  inserttexts(texts);   /* ____WIND.c */
                  completetext();   /* ____WIND.c try this 050927*/
               }
               
               Lkinfile = 1;
               if(LGUI) AdjustMenus();  /*MACMENU.c, MPCMENU.c*/
                    /*affects kinemage menu*/
               if(Ltest)
               {
                  sprintf(alertstr,"Found @kinemage %d "
                  ,kinemagenum);
                  dosinglealert(3);   
               }                        
               origkinenum = kinemagenum; /*981207*/  
               goto finishedtext;/*JUMP OUT OF THIS PLACE*/
            }
            Lnew = 0; /*only set ==1 when keyword is recognized*/

          break; /*case kileID*/

        }/* loop switch cases */
       }/*ido*/
       Lastchar = c; /*however c is set, allows testing for new line*/
     }/*char acceptable*/    
   }/*individual char input loop while file not at EOF */
   /*EOF section******************/
   if(Ltext || Ltextstate)  /* finish out anything in text buffer */
   {  

      /*texts[itext+1] = '\0';*/ /*990209*/
      texts[itext] = '\0';
      if ( itext >  0 )
      {
         inserttexts(texts);  /*____WIND.c */
         itext = 0;  /*990209*/
      }
      completetext();/*____WIND.c */
      /* Can only get here if hit EOF while reading the text section */
      /* Do NOT Warn the reader: */
      /*  this may be a legitimate table of contents, etc. */
      IOerr = 0; 
                 
   }
   else
   {   /*section reworked 050927*/
      sprintf(alertstr,"EOF reached without finding"CRLF
      "@text or @kinemage in this file.");
      dosinglealert(0);
      IOerr = 1;
      /*dosinglealert() only invokes a dialog box if Ltest or Lformattest*/
      /*to alert the reader, write a message to the text window*/
      cleartexts();         /* ____WIND.c */

      sprintf(texts,CRLF" Looked for kinemages in file:");
      inserttexts(texts);   /* ____WIND.c */

      sprintf(texts,CRLF" %s"CRLF,NameStr);
      inserttexts(texts);   /* ____WIND.c */

      /*MPCDLOG will have cleared alertstr, so reprint it here on spec*/
      sprintf(alertstr,"EOF reached without finding"CRLF
      "@text or @kinemage in this file.");
      inserttexts(alertstr);  /*____WIND.c */

      sprintf(texts,CRLF"[Note: You can use PREKIN to "
      CRLF"make a kinemage file (*.kin)"
      CRLF"from a PDB coordinate file.]"
      CRLF);
      inserttexts(texts);   /* ____WIND.c */
      completetext();   /* ____WIND.c needed by MPC */
   }
   atEOF = 1;
   makecursoractive(); /*MACMAIN.c*/ /*reader needs control of cursor*/ 
   /*EOF section___________________*/
   
 } /* loop with valid input file */
    
 finishedtext: ;    
 return;
}
/*___GetTextFromFile()______________________________________________________*/

/****getkinemagenum(c,Lnew)**************************************************/  
int        getkinemagenum(char c,int Lnew)  
        /*sucessful acquisition of number sets IOerr = 0 */
{
    int  ireturn=0;
    
    ireturn = 0;
    if(Lnew)
    {
        inumstr = 0;
    }
    if(c==' ' && inumstr==0)
    {
        ;  /* skip beginning spaces */
    }
    else if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||
            c=='5'||c=='6'||c=='7'||c=='8'||c=='9'  )
    {
        numstr[inumstr] = c;
        inumstr++;
    }
    else  /* anything besides numerals ends a number*/
    {
        numstr[inumstr] = '\0';  /* ends a number */
        if(inumstr == 0)
        {
            kinemagenum = 0;
            if(Lformattest)  /*980823*/
            {
                sprintf(alertstr,"Format fault: this kinemage has no number");
                dosinglealert(3); 
            }
        }
        else
        {
            sscanf(numstr,"%d",&kinemagenum);
        }
        sprintf(KineName,"Kinemage  %d - ",kinemagenum);/*980823*/

        ireturn = 1;  /* some number was retrieved or invented*/
        IOerr = 0; 
    }
    return(ireturn);
}
/*___getkinemagenum()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****kinesearch()************************************************************/
int     kinesearch()
{
   int   c=0,flowID=0;  /* switch controls for input */
   int   Lkine=0, Lnew=0; /* Logicals for input flow */
   enum{ neutID = 1, keywID, kineID };     

   flowID = neutID;
   fpin = fp; /*only search for multiple kins in a primary kin file*/
   while ((c = getnextchar() ) != EOF)
   /* #define getnextchar()  getc(fpin)  fpin==fp here*/

   /* flow controls and logicals together direct the flow */
   {
       if (c == '\n') c = '\015';
          /* whatever newline is, it needs to be CR in text */
       if (c == '@') 
       {
           flowID = keywID; /* need to get begin's key word */
           iword = 0;  /* set keyw character counter */
       }
       switch(flowID)
       {
           case neutID: /* neutral: no flow designated, discard characters */
           break;
          
           case keywID: 
              /*try to interpret next set of characters as a key word*/
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
                  if( (c == ' ')||(c == '\015')||(c == ',')||
                      (c == ';')||(iword >= 8)||(c == '\n') )
                  {/* stop looking when hit white space or 8 char max */
                      /* pad ends so can treat word as a real string */
                      word[iword+1] = '\0';
                      word[0] = ' ';
             
                      /* try to interpret as a keyword string */
             
                      /* search for kinemage as key word */
                      Lkine = 0;
                      if(strncmp(word," kinemage",9) == 0) Lkine =1;
                      if(Lkine) 
                      {
                          flowID = kineID; 
                          Lnew = 1;  
                          /* flag start of scan for kinemage number */
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
              {/*test number and directly return if successful*/
                  if(kinemagenum == nextkinenum) return(1);
                     /* found desired number */
                  if(nextkinenum == 0) return(1); 
                     /* accept first encountered kinemage */
                  flowID = neutID;
                  /*wrong number, keep looking for another kinemage*/
              }
              Lnew = 0;
           break;
       }/*loop switch cases */
        
   }/*loop while file not at EOF */
   /*Warn the reader: make sure they have the mouse to click the box*/
   makecursoractive();
   sprintf(alertstr,"reached EOF while searching, "
                    "target kinemage not recognized!");
   dosinglealert(0);  /*MAGEDLOG.c*/
   IOerr = 0;
   atEOF = 1;
   return(0);
}
/*___kinesearch()___________________________________________________________*/

/****findkinemage()**********************************************************/
void    findkinemage()
{
    int Lsuccessful=0;
    makecursorwait(); /*____MAIN.c*/  
    /* now interpret in terms of which kinemage to go to next */
    if(Lnextkine) /*then nextkinenum == kinemagenum, which is poised for input*/
    {/*Lnextkine*/
        if(atEOF)
        {
            LnextOK = 0;
            Lreentry = 0; /*don't try to read in a kinemage*/
            Lprocessing = 0; /*finished trying to bring in a kinemage */
        }
        else
        {
            LnextOK = 1;
            Lreentry = 1; /*in effect, kinesearch() has been successful*/
        }
    }/*Lnextkine*/
    else if(Lcancel) 
    {/*cancel this dialog*/
        Lreentry = 0; /*don't try to read in a kinemage*/
        Lprocessing = 0; /*finished trying to bring in a kinemage */
    }/*cancel this dialog*/
    else if(nextkinenum != thiskinenum) /*must setup and do kinesearch()*/
    {/*nextkinenum != thiskinenum*/ 
        /* note: kinemagenum holds kinemage flag of next one */
        /*since that's what stopped this one */
        /*Beware that kinesearch() resets kinemagenum*/
        /*by kinemage format: kin numbers should be monotonic increasing*/
        /* and always positive,  MAGE allows anything, but may not be able*/
        /*to find a kinemage with a duplicate number*/
        if(nextkinenum < kinemagenum)    
        {/*nextkinenum < kinemagenum*/
            LnextOK = 1;
            filerewind(fp); /*primary kinemage, fpin==fp in GetStuffFromFile*/
            atEOF = 0;
        }/*nextkinenum < kinemagenum*/
        Lsuccessful = kinesearch(); /*search for desired kinemage MAGEFILE*/
        if( Lsuccessful )
        {
            Lreentry = 1;/*kinesearch() has been successful*/
        }
        else /* not successful, try something else */
        {/*first try not successful, try something else */
            if(nextkinenum > thiskinenum) 
            {/*possible out-of-order kinemage numbers, rewind try again*/
                LnextOK = 1;
                filerewind(fp); /*primary kinemage, fpin==fp:GetStuffFromFile*/
                atEOF = 0;
                Lsuccessful=kinesearch();/*search for desired kinemage*/
            }/*possible out-of-order kinemage numbers, rewind try again*/
            if( Lsuccessful )
            {
                Lreentry = 1;/*kinesearch() has been successful*/
            }
            else /* still not successful, reset to present kinemage */
            {/* still not successful, try to reset to present kinemage */
                nextkinenum = thiskinenum; /*reset to present kinemage*/
                filerewind(fp); /*primary kinemage, fpin==fp, GetStuffFromFile*/                     
                atEOF = 0;
                LnextOK = 1;
                Lsuccessful = kinesearch();/*search for current kinemage*/
                if( Lsuccessful )
                {/*read in kin again so file will be at known place*/
                    Lreentry = 1;/*kinesearch() has been successful*/
                }
                else
                {/*hopelessly muddled, leave well enough alone*/
                    Lreentry = 0;
                    
                }
            }/* still not successful, try to reset to present kinemage */
        }/*first try not successful, try something else */
    }/*nextkinenum != thiskinenum*/
    else /*nextkinenum == thiskinenum*/
    {/*nextkinenum == thiskinenum*/ /*already here, don't mess with the kin*/
        Lreentry = 0;
    }/*nextkinenum == thiskinenum*/
        
    if(Lreentry) 
    {
        entry();      /*MAGEFILE.c*/
        /*old kinemage cleared and next one read in */
    }
    AdjustMenus();   /*MAGEMENU.c*/
    makecursoractive(); /*____MAIN.c*/  
    Lgetnextkinemage   = 0;
    Lgetkinemagechoice = 0;
    return;    
}
/*___findkinemage()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****Dotexthyperbutton********************************************************/
void    Dotexthyperbutton(int isetup) /*931105,931204*/ 
{/*isetup==0 at input, 1 for run time*/

/* Note for Authors: moved to MAGEHELP.c*/
/*Feb 2004: proposed but not coded: double pass hypertext where pick text: */
/* *{signel= 5 ...explain...}*     and hypertext control passed to */
/* *{action= 5 ...actual hypertext control sequences elsewhere in text...*/
/*e.g. allow brief text to choose among for very complicated rot controls*/

    char  wkeys[256];
    int   i=0,j=0,n=0,c=0,ikeyw=0,Lprekey=0,flowID=0,Lknow=0;
    int   Lnew=0,Lthisisaquestion=0,Lsetmaster=0,Lquote=0,Lpass=0;
    unsigned long long Nthismastermask=0; /*971012 int, 081120 long long*/
    int   Lsetsearch=0,Lsetbondrot=0,Lmayneeddoupdate=0;
    int   Nonoff=0;
    float theta=0,target=0;
    
    enum
    {
         neutID = 1
        ,idenID = 2
        ,keywID = 3
        ,kineID = 4
        ,quesID = 5
        ,viewID = 6
        ,mastID = 7
        ,bondrotID = 8
        ,onID   = 9
        ,offID  = 10
    };     
    Nonoff=Nonoff; /*only used in commented out print statements*/
/*
printf("Dotexthyperbutton(%d), Lkinfile== %d\n",isetup,Lkinfile);
*/
  Lredohyperbutton=1; /*flag so can set kinemage, then do master selection*/
  Lpass = 0;
  /*Lquesalloff = 0;*/ 
  /*global flag set true to turn off all masters*/ /*980922c*/
  /*nQuestion = 0;*/ 
  /*global, identifies control as a question # or not one if 0 */ /*980922c*/
  /*Nqueskin[nQuestion]=thiskinenum;*/ 
  /*if kinemage # not specified, make it this kin # */ /*980922c*/
  /*Nquesview[nQuestion] = 1;*/ /*if view not specified, make it View 1 */ 
  
  while(Lredohyperbutton)
  {/*while Lredohyperbutton==1 */
/*
printf("Lpass == %d\n",Lpass);
*/
    Lnew=1;        /*initialize first encounter of an object */
    Lknow = 0;
    
    flowID = keywID; /* need to get a key word */ /*971012*/
    ikeyw = -1;  /* set keyw character counter */ /*971012*/
    Lprekey = 1; /* Logical to skip blank spaces before key*/
    /*wkeys[0]=' '; */ 
    /*blank inserted to make keyword logic here same as at input*/ /*971012*/
    
    Lthisisaquestion = 0; /*flag to do question dialog */
    Nthismastermask = 0;
    /*for(j=0;j<=MAXM;j++){Lquesmaster[j]=0;Nquesmaster[j]=0;}*/ 
       /*clearmasterflags*/
    Lquesmastermask  = 0;/*clearmasterflags mask bits*/ 
    Nquesmasteronoff = 0;/*clearmasterflags on-off indicator per mask bit*/ 
    Lquesfind = 0;/*clear search specified flag*/ 
    Nonoff = 1; /*default to start is true*/
    
    Lgographics = 0; /*flag for really go to gographics*/
    Lsetmaster = 0;  /*flag to look for a master button name*/
    Lsetsearch = 0; /*flag to do a FIND operation for a given string*/
    Lquote = 0; /*flag to signel quoted string, like a button name*/
    Lquesalloff = 0;/*global flag set true to turn off all masters*/ /*980922c*/
    
    if(isetup==1)                                                   /*980923b*/
    {/*called by click in text window*/ /* only do this at run time*/
         nQuestion = 0; /*if hypertext is a question: number in field*/
         /* NOT a question: need default index for control storage*/
         Nqueskin[nQuestion] = thiskinenum;
         Nquesview[nQuestion] = 1;
    }/*called by click in text window*/ /* only do this at run time*/
    else
    {/*initialization as text read in*/
        /*set default parameters for this hypertext control region*/
        Nqueskin[nQuestion]=oldNqueskin;/* 0 : go to first kinemage*/
        Nquesview[nQuestion] = oldNquesview; /* 0 should set view1 */ 
        /*only kinemage # and view # are carried to next hypertext*/
    }/*initialization as text read in*/                             /*980923b*/
    
    Lqueskin = 1; /* kinemage set flag, all questions have associated kinemage*/
                  /* if not specified, is presumed the current kinemage*/
    Lquesview = 0;/* view set flag, all questions have associated view*/
        /*BUT not all hypertext should have view changed, e.g. rotamer angles*/
    Lisgraphics = 1; /*flag to tell ques-dialog something to gographics for*/
    
    Lpass++;  
    Lredohyperbutton=0; /*one pass unless needed for master selection*/
/*
printf("iQstring: %d,|%s|\n",Qstring);
*/
    for(j=0; j<=iQstring+1; j++) /*971012*/
    {/*scan all characters, need one more pass if final keyword right adjusted*/

        if(j==iQstring+1) c = '\0'; /*and go with last set flowID*/ /*971012*/
        else              c = Qstring[j];

        if(c == '{' && Lquote==0  /*}*/)  /*commented brace for balance */
        {/* start a button identifier*/
            Lidn = 0; /* Lidn only set = 1 after next identifier char read */
            Lquote = 1;
            iword = 0;
            flowID = idenID;
        }
        else if( /*{*/   c == '}' && Lquote==1)/*commented brace for balance */
        {/*end a button identifier, require both "L&&R curly braces" */ 
            Lquote = 0;
            for(n=iword ; n<MAXNAMECHAR ; n++) word[n] = '\0'; /*971012*/
            /*revision930207 pad out identifier to " with '\0' rather than ' '*/
            word[MAXNAMECHAR] = '\0';/*pad & end the string in c manner*/ 
            /*word AND iword both used for a succession of tests*/

            /*071212 {name} previously had to be either master, find, bondrot*/
            /*not done as if,elseif --so set default flowID = keywID */
            /*before tests to allow isolated {name} to be ignored */
            /*default if no previous flag recognized 071212*/

            flowID = keywID; /* reset to look for next key word */

            if(Lsetmaster && isetup>0 && Lkinfile) /* only do this at run time*/
            {/*should hold identifier of a master button for this kinemage */
              if(Lqueskin && thiskinenum!=Nqueskin[nQuestion])
              {/*cannot do master button until new kinemage read in*/
                  Lredohyperbutton=1;
              }
              else
              {/*at kinemage for which master button name applies*/
                /* master named,*/
                /*condition(word);*/ /*make into a Pascal string if on Mac*/
            
                if(nmaster > 0) /*previous master button requested */ 
                {/*see if this one matches previous a master name */ 
                   thismasterptr = firstmasterptr;
                   while(thismasterptr != NULL)      
                   {
                                          /*971012*/
                      if(strncmp(word,thismasterptr->name,MAXNAMECHAR) == 0) 
                      {
                        Lquesmastermask = Lquesmastermask | thismasterptr->mask;
                              /*set mask bit for this masterbutton*/
                        Nthismastermask = thismasterptr->mask; 
                              /*current master specified*/
                              /*the ON or OFF is yet to be read in */
                              /*to set bit corresponding to thismasterptr->mask */
                        Lisgraphics = 1; /*something to gographics for*/
/*
printf("master %u, onoff== %u : new mask = %u, new onoff = %u, thismask = %u, ~: %u\n"
,thismasterptr->mask,Nonoff,Lquesmastermask,Nquesmasteronoff,Nthismastermask,(~Nthismastermask));
*/
                      }
                        thismasterptr = thismasterptr->nextptr; /*971012*/
                   }
                }
                Lsetmaster = 0;
              }/*at kinemage for which master button name applies*/
                flowID = keywID; /*can look for another keyword*/
            }/*should hold identifier of a master button for this kinemage */
            
            if(Lsetsearch && isetup>0 && (Lkinfile==1 || Lkinfile==3)) 
            /* only do this at run time*/
            {/*should hold identifier of a search pointID for this kinemage */
                if(Lqueskin && thiskinenum!=Nqueskin[nQuestion])
                {/*cannot do search until new kinemage read in*/
                    Lredohyperbutton=1;
                }
                else

                {/*at kinemage for which search is valid*/
                     /* pointID named,*/
                     /* Do NOT condition(word), search expects C-str*/
                     for(n=0;n<16;n++) search1str[n]=word[n];
                     isearch1 = iword;
                     isearch2 = 0;
                     Lquesfind = 1;/*search specified*/ 
                     Lisgraphics = 1; /*something to gographics for*/
                  /*  
                     sprintf(alertstr,"search string: %s",search1str);
                     dosinglealert(3);
                  */
                     Lsetsearch = 0;
                }/*at kinemage for which search is valid*/
                flowID = keywID; /*can look for another keyword*/
            }/*should hold identifier of a search pointID for this kinemage */
                /*980922c*/

            if(Lsetbondrot) /* still have bondrot value to read*/
            {/*should hold identifier of a bondrotation for this kinemage */
                Lsetbondrot = 0;
                /* bondrot name stored in word character string*/
                /*condition(word);*/ /*make into a Pascal string if on Mac*/
                /*keep iword until get bondrot value for this name*/
                flowID = bondrotID; /*000605*/
                Lnew = 1;  /* flag start of scan for number */
            }/*should hold identifier of a bondrotation for this kinemage */

            Lidn = 0;            
            ikeyw = -1;
            Lprekey = 1;
            c = ' '; 
          /*turn R curly brace into an ignored leading blank for next field*/
        }/*end a button identifier, require both L&&R curly braces*/ /*971012*/

            switch(flowID)
            {/*switch*/
                case neutID:  
                    /* neutral: no flow designated, discard characters */
                break;                    
                case idenID:
                    if(Lidn)
                    {
                        if(iword > 255) 
                        {
                             sprintf(alertstr,
                              "quoted string getting excess char: %s, %c\n",
                                  word,c);
                             dosinglealert(2);
                             iword = iword - 1;
                        }
                        word[iword] = c;
                        iword = iword + 1;
                    }
                    else
                    {
                        Lidn = 1; /*get ready for the actual quoted characters*/                               

                    }
                break;
                    
                case keywID: 
                    /* skip blank spaces, or tabs, in the beginning */
                    /* Lprekey set = 1 where keywID set */
                    if((Lprekey && (c == ' ')) || (Lprekey && (c == '\t')))  
                    {
                        ;  /* skip white space */
                    }
                    else
                    {/*non-blank character*/
                        Lprekey = 0;  /* hit a non-blank character */
                        ikeyw++; /* ikeyw set = -1 when flowID set = keywID */
                        
                        wkeys[ikeyw] = c;
/*c*/                   wkeys[ikeyw] = tolower(wkeys[ikeyw]); /*case independent */
                        
                        if(  (c == ' ')||(c =='\015')||(c == ',')||(c == ';')
                           ||(c =='\t')||(c =='\n')||(c == '?')||(ikeyw >= 21)
                           ||(c =='\0')||(c =='=') ) /*971012*/ /*980922c*/
                        {/*stop looking when hit white space or 20 char max*/
                            /* pad ends so can treat wkeys as a real string */
                            wkeys[ikeyw] = '\0';
                            /*wkeys[0] = ' ';*/ /*971012*/
                            /* try to interpret as a keyword string */
/*
printf("putative keyword %s\n",wkeys);
*/
                            if(Ltest)
                            {
/*c*/                        sprintf(alertstr,"Putative hyperbutton: %s",wkeys);
                             dosinglealert(3);
                            }
                          if(!Lknow) 
                          {
/*.*/                       if(strncmp(wkeys,"q:",2) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = quesID; 
                                Lnew = 1;  /* flag start of scan for number */
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(   strncmp(wkeys,"kinemage",8) == 0 
                                ||strncmp(wkeys,     "kin",3) == 0 
                                ||strncmp(wkeys,       "k",1) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = kineID; 
                                Lnew = 1;  /* flag start of scan for number */
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(   strncmp(wkeys, "view",4) == 0 
                                ||strncmp(wkeys,    "v",1) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = viewID; 
                                Lnew = 1;  /* flag start of scan for number */
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(   strncmp(wkeys, "master",7) == 0 
                                ||strncmp(wkeys,      "m",1) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lsetmaster = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(   strncmp(wkeys, "find",4) == 0
                                ||strncmp(wkeys,    "f",1) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lsetsearch = 1;
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(   strncmp(wkeys, "rot",3) == 0
                                ||strncmp(wkeys,   "r",1) == 0) Lknow=1;
                            if(Lknow) 
                            {

                                Lsetbondrot = 1;
                            }
                          }

                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys,"on",2) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = onID; 
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys,"off",3) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                flowID = offID; 
                            }
                          }
                          if(!Lknow)
                          {
/*.*/                       if(strncmp(wkeys,"alloff",6) == 0) Lknow=1;
                            if(Lknow) 
                            {
                                Lquesalloff = 1;
                            }
                          }

                          /* reset search flags: */
                          Lknow = 0;
                          
                          ikeyw = -1;
                          wkeys[0]=' '; 
                          Lprekey=1; 
                            
                        }/*stop looking when hit white space or 20 char max*/
                    }/*non-blank character*/
                break;
                case quesID: 
                    /*when done, getinteger returns as 1  */
                    if( getinteger(c,Lnew) ) /*MAGEINPT.c*/
                    {
                        nQuestion = intpass;  
                        /*intpass global to pass int back */
                        Lthisisaquestion = 1;
                        flowID = keywID; /* reset for next key word */
                        ikeyw = -1;
                        Lprekey = 1;
                        /*set default parameters for this question*/
                        if(isetup==0) Nqueskin[nQuestion] = oldNqueskin; 
                        if(isetup==0) Nquesview[nQuestion] = oldNquesview;   
                     /*only kinemage # and view # are carried to next question*/
                    }
                    Lnew = 0;
                break;
                
                case kineID: 
                    /*when done, getinteger returns as 1  */
                    if( getinteger(c,Lnew) ) /*MAGEINPT.c*/
                    {
                        Nqueskin[nQuestion] = intpass;  
                            /*intpass global to pass int back */
                        if(isetup==0) oldNqueskin=intpass; /*for next question*/
                        Lqueskin = 1;
                        flowID = keywID; /* reset for next key word */
                        ikeyw = -1;
                        Lprekey = 1;
                        Lisgraphics = 1; /*something to gographics for*/
                    }
                    Lnew = 0;
                break;
                
                case viewID: 
                    /*when done, getinteger returns as 1  */
                    if( getinteger(c,Lnew) ) /*MAGEINPT.c*/
                    {

                        if(intpass > 0 && intpass <= MAXRESETS)
                        {/*intpass global to pass int back */
                            Nquesview[nQuestion] = intpass;  
                            if(isetup==0) oldNquesview = intpass; 
                              /*for next question*/
                            Lquesview = 1;
                            Lisgraphics = 1; /*something to gographics for*/
                        }
                        flowID = keywID; /* reset for next key word */
                        ikeyw = -1;
                        Lprekey = 1;
                    }
                    Lnew = 0;
                break;

                case bondrotID: 
                    /*when done, getfloat returns as 1  */
                    if( getfloat(c,Lnew) ) /*MAGEINPT.c*/
                    {
                       if(Lqueskin && thiskinenum!=Nqueskin[nQuestion])
                       {/*cannot do bondrotation until new kinemage read in*/
                          Lredohyperbutton=1;
                       }
                       else
                       {/*at kinemage for which bondrotation name applies*/

                          /*floatpass global to pass int back */
                          target = floatpass;  

                          if(nbondrot > 0)  
                          {/*there are bondrotations */
                             for(i=1; i<=nbondrot; i++)
                             {/*loop over all existing rotations try to match name*/
                                if(strncmp(word,bondrotptr[i]->name,iword) == 0)
                                {/*name match*/
                                   icurrentrotation = i;
                                   theta = target - bondrotptr[i]->angle;
                                   if(theta > 180)
                                      theta = theta - 360; /*001017*/
                                   else if(theta < -180)
                                      theta = 360 + theta;
                                      
                                   bondrotptr[i]->angle = target;
                                   resetgrafbondrotBar(i);/*____BAR.c 001022*/
                                   /*writebondrotvalue(i);*/ /*MACBAR.c*/
                                   dobondrotrouter(i,theta); /*MAGEANGL.c*/ /*970917*/
                                   rescalekinemage(); /*MAGEINPT.c*/
                                   /* rescale rebuilds from float arrary*/
                                   Lmayneeddoupdate = 1; 
                                   break; /*out of loop over all existing rotations*/ 
                                }/*name match*/
                             }/*loop over all existing rotations try to match name*/                    
                          }/*there are bondrotations */
                          Lisgraphics = 1; /*something to gographics for*/
                       }/*at kinemage for which bondrotation name applies*/
                        flowID = keywID; /* reset for next key word */
                        ikeyw = -1;
                        Lprekey = 1;
                    }
                    Lnew = 0;
                break;

            }/*switch*/ 
       /*on and off do not have any further information needed before setting*/
            if(flowID == onID)
            {
                    Nonoff = 1;
                    if(Nthismastermask > 0) 
                        Nquesmasteronoff = Nquesmasteronoff | Nthismastermask;
                                                     /*current on-off flag*/
/*
printf("onoff== %u : master masks = %u, new onoffs = %u, thismask = %u, ~: %u\n"
,Nonoff,Lquesmastermask,Nquesmasteronoff,Nthismastermask,(~Nthismastermask));
*/
                    Nthismastermask = 0;/*clear so can't hit same master twice*/
                    flowID = keywID; /* reset for next key word */
                    ikeyw = -1;
                    Lprekey = 1;
            }
            else if(flowID == offID)
            {
                  Nonoff = 0;
                  if(Nthismastermask > 0) 
                     Nquesmasteronoff = Nquesmasteronoff & (~Nthismastermask);
                     /* bitwise "and" with the unary bitwise complement (~) */
                     /*will copy all bits except the mask bit will be set == 0*/
/*
printf("onoff== %u : master masks = %u, new onoffs = %u, thismask = %u, ~: %u\n"
,Nonoff,Lquesmastermask,Nquesmasteronoff,Nthismastermask,(~Nthismastermask));
*/
                    Nthismastermask = 0;/*clear so can't hit same master twice*/
                    flowID = keywID; /* reset for next key word */
                    ikeyw = -1;
                    Lprekey = 1;
            }               
     }/*scan all characters*/ 
     
     if( isetup>0 && Lthisisaquestion==1 && Lpass==1 && (Lkinfile==1 || Lkinfile==3)) 
     {
         DoQuesDialog(); 
         /*can return Lgographics as 2: go to graphics window*/
         if(Lgographics==0) Lredohyperbutton=0; /*no redo if no graphics*/
     }
     else if( isetup>0 && Lisgraphics) Lgographics = 1;

     if(Lmayneeddoupdate)
     {
        Lmayneeddoupdate=0;
        if(Lupdate &&!Linhibiton && Lprobeactive)
        {/*update from remote program: e.g. dots by PROBE*/
           Nthisgroupkind = PROBEDOTSKKIND; /*hypertext presume need probe run*/
           sprintf(word,"\""); /*ditto mark*/
           doupdate(1,word); /*____OUT.c or MUXMPIPE.c*/  
           /* 1: sends coords to probe*/
           Nthisgroupkind = 0;
        }
     }

     if( isetup>0 && Lgographics==1 && (Lkinfile==1 || Lkinfile==3)) gographics(); /*MAGEFILE.c*/
     if( isetup>0 && Lgographics==2 && (Lkinfile==1 || Lkinfile==3))
     {

#ifdef MACINTOSH
         ShowWindow(grafWindow);
         SelectWindow(grafWindow);
#endif
#ifdef PCWINDOWS
         ShowWindow(grafWindow, SW_SHOW);
         BringWindowToTop(gWindow);
#endif
#ifdef UNIX_X11
         showthegrafwindow(); /*MUX_MENU.c*/
#endif
     }
     if( isetup==0)  Lredohyperbutton=0;
  }/*while Lredohyperbutton==1 */             
}
/*____Dotexthyperbutton______________________________________________________*/

/****gographics()*************************************************************/
void    gographics()    /*931113*/
{
  int   j=0, ion=0, nbuttons=0;  /*971012*/
    
  unsigned int mask=0;  /*980922   */
    
  Lgographics = 0;
  if((Lkinfile==1 || Lkinfile==3))
  {/*there is a kinemage with graphics to go to*/
    if(Lqueskin)
    {/*971128*/
       /*setkinemage(Nqueskin[nQuestion]);*/
       nextkinenum = Nqueskin[nQuestion];
       
       if(nextkinenum == kinemagenum) Lnextkine = 1; /*980922b*/
       else Lnextkine = 0;  /*980922b*/
       
       Lappend = 0;
       findkinemage();
    }/*971128*/
    if(Lquesview) 
    {
        resetrot(Nquesview[nQuestion]); /*MAGEROTN.c*/
    }
    if(Lquesalloff)
    {/*turn off all master buttons*/
         /*identify this masterptr by its mask */ /*master button toggle*/
         thismasterptr = firstmasterptr;
         while(thismasterptr != NULL)
         {
             buttonboxcheck(thismasterptr->button,0);   /*____BBOX.c*/
             thismasterptr->on = 0; 
             adjustmaster(thismasterptr->mask, 0 );
             thismasterptr = thismasterptr->nextptr;
         }/*980922*/
         Lquesalloff = 0; /*unset flag */
    }/*turn off all master buttons*/
    if(Lquesmastermask!=0) /* has value if any mask bits are set*/ /*971012*/
    {/*hyper control of master buttons*/ 
        if(nmaster > 0)   
        {/*scan over Lquesmastermask and set appropriate master by bit*/
/*
printf("gographics master masks = %u,  onoffs = %u\n"
,Lquesmastermask,Nquesmasteronoff);
*/
            for(j=0; j<(8*((long)sizeof(long long))); j++) /*081120long*/
            {
                if(Lquesmastermask & 1<<j)
                {
                   if(((Lquesmastermask & 1<<j) & (Nquesmasteronoff & 1<<j))!=0)
                        ion = 1;
                   else ion = 0;

                   /*980922*/
                   /*identify this masterptr by its mask */ /*980922 master button toggle*/
                   mask = (Lquesmastermask & 1<<j);
                   thismasterptr = firstmasterptr;
                   while(thismasterptr != NULL)
                   {
                       if(thismasterptr->mask == mask) break;
                       thismasterptr = thismasterptr->nextptr;
                   }
                   if(thismasterptr != NULL)
                   {
                       buttonboxcheck(thismasterptr->button,ion);   /*____BBOX.c*/
                       thismasterptr->on = ion; 
                   }/*980922*/
                   adjustmaster( (Lquesmastermask & 1<<j), ion );

                     /*MAGEBBOX.c*/

/*
printf("j==%d, adjustmaster %u,  onoff  = %u\n"
,j,(Lquesmastermask & 1<<j),(Lquesmastermask & 1<<j) & (Nquesmasteronoff & 1<<j));
*/
                }
            }
            nbuttons = 1;  /*master buttons have been changed*/ /*971012*/
        }/*scan over Lquesmastermask and set appropriate master by bit*/                        
    }/*hyper control of master buttons*/  /*971012*/
    if(Lquesfind)
    {
        Lsearchbegin = 1; /*search whole kinemage*/
        if( Lmarkers) /*markers are in maxgrp+1 if there was room */
        {       
            /*if there are markers available and nothing is on that*/
            /*will flag that a search was successful, turn markers on*/
            if(   !Lnewlabelson 
               && !Lnewballson 
               && !Lpickcenteron 
               && !markergrupptr->on)
            {
                 markergrupptr->on=1;/*force on*/
            }
            nbuttons = 1;  /*marker button may have been changed*/ /*971012*/
        }
        if( mysearchpointID(1) ) ; /*MAGEUTIL.c for justone hit*/
        else 
        {
             sprintf(alertstr,"%s\015 NOT FOUND",search1str);
             dosinglealert(0);
             Lsearchatend = 1;
        }
    }
    if(nbuttons) /*971012*/
    {
        LneedSetsinGrafWindow = 1; /*DrawControls(grafWindow) called in MAC*/ 
    }/*971012*/
    
    redrawvec();
 
#ifdef MACINTOSH
         ShowWindow(grafWindow);
         SelectWindow(grafWindow);
#endif
#ifdef PCWINDOWS
         ShowWindow(grafWindow, SW_SHOW);
         BringWindowToTop(gWindow);
#endif
#ifdef UNIX_X11
         showthegrafwindow(); /*MUX_MENU.c*/
#endif
  }/*there is a kinemage with graphics to go to*/
}
/*___gographics()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****entry()*****************************************************************/
void    entry()
{
        int  j=0;

        sprintf(KineName,"Kinemage  %d - ",kinemagenum);

        Lreentry = 0;

        
        if(LGUI) makecursorwait(); /*MACMAIN.c*/ /*MPCMAIN.c*/
        /*maxgrp = 0; */                   
        /*igrp[maxgrp][0] = 0;*/ /* turn off all display*/
        
        /*Lmarkers = FALSE;*/ /*now could call redrawvec() to blank graphics*/
        if(LGUI) showallwindows(); /*make visible capt, text, graf windows*/
                          /*MACWIND.c*/ /*MPCWIND.c*/ /*MUXMWIND.c 121107*/

        if(Lappend==0)
        {
           if(Ltablepresent > 0) cleanuptable(); /*old table to be destroyed*/
           initrot(); /*MAGEANGL.c*/
        }
        /*usual flow: this is the only place to call GetStuffFromFile()*/
        GetStuffFromFile(); /* should end on new kinemage number, or EOF */
                            /*MAGEINPT.c*/
        if(Ltablepresent > 1) 
        {
           createtable(); /*new table to be invented*/
        }

        /*if(Lappend==0)*/ /*981210 always get the master buttons adjusted*/
        if(nmaster > 0)
            for(j=1 ; j<=nmaster; j++) {resetmaster(j);} /*MAGEBBOX.c*/
        if(Lappend==0)
        {
           resetrot(1);         /*MAGEANGL.c*/
           
           if(LGUI) choosefrontwindow(); /*MACWIND.c*/ /*MPCWIND.c*/ /*MUXMWIND.c 121107*/
           
        }

        if(Lappend==0) {clearfocusxyzstr();}  /*MAGEINIT.c*/

        if(Lextrafile==0) /*021229*/
        {/*no more input pending, so go ahead with showing the kinemage...*/
           Lnewfile = 0; 
           LneedSetsinGrafWindow = 1;
           /*if(LGUI) redrawvec(); NOT NEEDED 040605 */ /*____DRAW.c */
            
           if(LGUI) makecursoractive();    /*MACMAIN.c*/ /*MPCMAIN.c*/
           
        }
}
/*___entry()________________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getnextkinemage()*******************************************************/
void    getnextkinemage(void)
{
    ireturnflag = 1;
#ifdef UNNECESSARYNOWMAYBE
    if(Lpruneactive||Ldrawactive||Lnewstuffactive||Lnewviewactive) 
    {
        DoChangeCheckDialog(); 
        /*MACDLOG.c,MPCDLOG.c sets ireturnflag, ireturnflag==1 OK to proceed*/
        /*This probably won't work on UNIX non-modal dialog system*/
    }
#endif

    if(ireturnflag && !atEOF && !Lprocessing) 
    {   /*not at EOF and not in the middle of previous nextItem */
        Lprocessing = 1; /*flag bringing in this kinemage */
        Lnextkine = 1;
        findkinemage(); /*MAGEFILE.c, set Lreentry=1 if next kinemage OK*/
                        /*if(Lreentry==1) -> entry() MAGEFILE.c*/
    }
}
/*___getnextkinemage()______________________________________________________*/

/****filerewind()*************************************************************/
void filerewind(FILE *fpvictem)
{
    rewind(fpvictem);
}
/*___filerewind()____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
