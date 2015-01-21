/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUX_OUT.c */

#include "MAGE.h"
#include "MAGEDLOG.h"
#include "MAGELIST.h"
#include "MAGETABL.h"

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writeoutput()************************************************************/
void writeoutput(int kind)
{            /*specific output controlled by kind */
    int    h,i,j,n,ivalid,Lnameflag;
 static char  FullOutStr[256];

 makecursorwait();
 /*presume will be able to invent a valid output name*/
 Lnameflag = 0; /*no problem anticipated*/

 /*first check that MAGE has valid material for this output*/
 ivalid = 1; /*presume true, then check for a few exceptions*/
 if(kind==3 && Ldrawstuff==0) ivalid = 0;

 if(ivalid)
 {/*valid output*/   
  if(!Ltestoutfile || FullOutStr[0]=='\0') /*safety if string fails*/
  {/*usual flow or first of test kinemages to be written*/
   for(j=0; j<256; j++)
   {/*clean out Strings*/
      TestStr[j] = '\0';
      OutStr[j] = '\0';
      FullOutStr[j] = '\0';
   }
   n = 0; /*use full name in UNIX*/
   if(kind==15||kind==17)
   {/*from MPCOUT 020816 with change of backslash to forward slash*/
     n = -1; /*keeps track of last / directory boundary in full path name*/
     for(i=0;i<256;i++)
     {/*isolate local, simple name from Full Path Name*/
        if(NameStr[i] == '/') n=i+1; /*char just past the slash*/
        if(NameStr[i] == '\0') break;
     }/*derive output file names from a full kinemage, not an appended file*/
     if(n==-1){n = 0;} /*there is no directory part in the file name*/
   }
   h = 0; /*index for working version of input name*/
   j = 0; /*flag position for a .kin type suffix to be replaced*/
   /*n is either==0, or set to first char of simple name in the Full Path name*/
   for(i=n;i<256;i++) 
   {/*make copy of working part of input file name, flagged for last . */
      TestStr[h] = NameStr[i]; /*this has exact name of current input file*/
      if(TestStr[h] == '.') j = h; /*j indexes last . in TestStr*/
      if(TestStr[h] == '\0') break;
      h++;
   }
   TestStr[255]='\0';/*safety, name should be a regular C string at this point*/
   if(j >= 251 || (j==0 && i>=251) ) 
   {                        /* j      */
      Lnameflag = 1;        /* .kin\0 */
      TestStr[251] = '\0';  /* 12345  */
      j = 251;
   }
   else if(j==0)
   {/*no . in name, presumptive new . position is just after last char*/ 
      j = h; /*starting index of .new name suffix */
   }
   /*copy just the directory part of the name to use as a file filter*/
   /*and include the permissive mask "*" so all files will be shown*/
   determinedirectory();
   inventoutputname(kind,j,Lnameflag);/*MAGEOUT.c*/
      /* construct output file name from input file name */
      /* add 2,3,or 4 char: .new to input file name */
      /* or add or increment .#.kin */
      /*or replace last .___ with .new*/
      /* no matter what that name is like. new==vw,ksp,kqa,kin,eps,...*/
   if(n>0) {for(i=0;i<n;i++) {FullOutStr[i] = NameStr[i];}}
   for(i=n,j=0;i<255;i++,j++)
      { FullOutStr[i] = OutStr[j]; if(OutStr[j]=='\0')break;}
  }/*usual flow or first of test kinemages to be written*/

   if(kind != 17 && kind != 13)
   {/*usual flow*/
      dooutputfiledialog(kind);
   }
   else
   {/*timed test output*/
      if(Ltestoutfile) /*is fpout actually open from earlier write ?????*/
      {/*append to already started test output file*/
         if(fpout == NULL)
         {/*previously openned file got closed somewhere*/
            fpout = fopen(FullOutStr,"a");
         }/*else presume file sitting open from earlier time*/
      }
      else
      {
         fpout = fopen(FullOutStr,"w");
      }
      if(fpout == NULL)
      {/*timed run, try again for a unique, valid name*/
          printf("FAILED to write file: %s\n",FullOutStr);
          writeoutput(15);/*which invokes std file choice dialog*/
      }
      else
      {/*succesful file open*/
        if(kind==17) {Ltestoutfile++;} /*maybe several test kins to one file*/
        if(!routeoutput(kind)) /*MAGEOUT.c*/
        {
           clearwriteconditions(); /*treat like canceling file open procedure*/
           return; /*not needed unless doing other things in outer loops*/
        }
      }/*succesful file open*/
   }/*timed test output*/
 }/*valid output*/
}
/*___writeoutput()___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****determinedirectory()*****************************************************/
void determinedirectory()
{
   /*copy just the directory part of the name to use as a file filter*/
   /*and include the permissive mask "*" so all files will be shown*/

   int i=0,s=-1;

   for(i=0; i<253; i++) /*see below*/
   {
      if(NameStr[i] == '/') {s = i;} /*so s==0 for top directory*/
   }
   if(s > 0)
   {/*finite length directory path*/
      for(i=0; i<253; i++) /*disaster protection: need room for 3 more char*/
      {
         DirStr[i] = NameStr[i];
         if(i == s) {break;} /*and i not incremented*/
         if(NameStr[i] == '\0') {break;}
      }
      DirStr[i++] = '/'; /*redundant?: puts a slash in position of last slash*/
      DirStr[i++] = '*';
      DirStr[i] = '\0';
   }/*finite length directory path*/
   else if(s==0)
   {/*last slash is char#0, presume in top directory*/
      DirStr[0] = '/';
      DirStr[1] = '*';
      DirStr[2] = '\0';
   }/*last slash is char#0, presume in top directory*/
   else
   {/*no directory path, presume in current directory*/
      DirStr[0] = '.';
      DirStr[1] = '/';
      DirStr[2] = '*';
      DirStr[3] = '\0';
   }/*no directory path, presume in current directory*/
}
/*___determinedirectory()____________________________________________________*/

/*callbacks need to be defined before they are invoked below*/
/****fileout_OK_CB************************************************************/
XtCallbackProc fileout_OK_CB(Widget w,XtPointer client_data,XtPointer call_data)
/*     Widget w;                       */
/*     caddr_t client_data;            */
/*     XmAnyCallbackStruct *call_data; */
{
  char *filename;
  int  j,kind;

  /*See D.A.Young,1994,pg239*/
  
  XmFileSelectionBoxCallbackStruct *cbs =                           
                     (XmFileSelectionBoxCallbackStruct *)call_data; 
  ACTIVE_DLOG = 0;
  XtUnmanageChild(fileoutdialog); 
  XtDestroyWidget(fileoutdialog); 
    /*So next time will create new fileselectiondialogbox including new files*/
  kind = intpass; /*retrieve type of output from global storage*/

  /*retrieve the filename character string from the compound string format*/
  XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename);
  for(j=0; j<256; j++)
  {
      OutStr[j] = filename[j];
      if(OutStr[j]=='\0') break;
  }
  if(strcmp(NameStr,OutStr)==0)
  {/*OutStr == current input name, this is pathelogical*/
     sprintf(word,"Output==Input: CHANGE OUTPUT NAME");
     determinedirectory();
     dooutputfiledialog(kind);
     return(NULL); /*from middle of things*/
  }/*or had to be edited to be valid, so pathelogical until changed*/
  else
  {/*filename OK*/
    makecursorwait(); 
    if(fpout != NULL)
    {/* if a previous file is open, close it */
       fclose(fpout);    
       fpout = NULL;
    }
    if(Lappendout) /*020303*/
    {
       fpout = fopen(OutStr,"a");
       Lappendout = 0; /*single shot, used for write rotated*/
    }
    else
    {
       fpout = fopen(OutStr,"w");
    } 
    /*if ((fpout = fopen(OutStr,"w") == NULL)*/  /*c*/ 
    if(fpout == NULL)  
    {
      if(kind==15) /*17 not use user choice file dialog, nor its call back*/
      {/*timed run, try again for a unique, valid name*/
        printf("FAILED to write file: %s\n",OutStr);
        writeoutput(kind);
        return(NULL); /*from middle of things*/
      }
      else
      {
         sprintf(alertstr,"BEWARE: failed to open: %s"
                  ,OutStr);
         sprintf(alertstr2,"Current active kinemage file remains: %s"
                  ,NameStr);
         sprintf(alertstr3,"Output file may not be there.");
         DoMageDLOGreport(3); /* 2 for abort, 3 for toggle debug*/
         clearwriteconditions(); /*treat like canceling file open procedure*/
         return(NULL);/*unsuccessful*/
      }
    }
    else
    {/*succesful file open*/
      /*NameStr only used when replacing full kinemage file*/
      if(!routeoutput(kind)) /*MAGEOUT.c*/
      {
         clearwriteconditions(); /*treat like canceling file open procedure*/
         return(NULL);/*unsuccessful*/
      }
    }/*succesful file open*/
    makecursoractive();
    if(LquitMAGE) quitMAGE(); /*MACMENU,MPCMAIN,MUXMMENU*/
    else if(Lgetnewfile) OpenNewFile(); /*____DLOG.c*/
    else if(Lclosedrawstuff) closedrawstuff(); /*MAGEMENU*/
    else if(Lgetnextkinemage) getnextkinemage(); /*MAGEFILE.C*/
    else if(Lgetkinemagechoice) getkinemagechoice(); /*MAGEUTIL*/

    return(NULL);/*successful*/
  }/*filename OK*/
}
/*___fileout_OK_CB___________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****fileout_CANCEL_CB********************************************************/
XtCallbackProc fileout_CANCEL_CB(Widget w,XtPointer client_data
                                         ,XtPointer call_data)
{
   ACTIVE_DLOG = 0;
   XtUnmanageChild(fileoutdialog); 
   XtDestroyWidget(fileoutdialog); 
     /*So will create new fileselectiondialogbox to include new files*/
   /*clearwriteconditions:*/
     LquitMAGE=0;
     Lgetnewfile=0; 
     Lclosedrawstuff=0;
     Lgetnextkinemage=0;
     Lgetkinemagechoice=0;
      Lspecialloop = 0;
      Lraster3Drender = 0;

   makecursoractive();
   return(NULL);
}
/*___fileout_CANCEL_CB_______________________________________________________*/

/****dooutputfiledialog()*****************************************************/ 
void dooutputfiledialog(int kind)
{
    XmString    xmstring1=NULL,xmstring2=NULL,xmstring3=NULL; 
    int n,j,k;

    xmstring1 = XmStringCreateSimple(word);
    xmstring2 = XmStringCreateSimple(OutStr);
    xmstring3 = XmStringCreateSimple(DirStr);
    /* invoke the standard dialog for finding a file */
    /* "word" holds message, "OutStr" has suggested file name */
    intpass = kind; /*global holds type of output to be done if Callback OK */
    /*always create so will get latest list of files available*/
    /*so have to call XtDestroyWidget() in callbacks*/
    n = 0;
#ifdef MAGECOLORMAP
    XtSetArg(args[n], XmNvisual, magevisual); n++;
    XtSetArg(args[n], XmNdepth, magedepth); n++;
    XtSetArg(args[n], XmNcolormap, colormap); n++;
    XtSetArg(args[n], XmNbackground, windowbackground); n++;
    XtSetArg(args[n], XmNborderColor, windowborder); n++;
    XtSetArg(args[n], XmNforeground, windowforeground); n++;
    XtSetArg(args[n], XmNhighlightColor, highlightcolor); n++;
    XtSetArg(args[n], XmNtopShadowColor, topshadowcolor); n++;
    XtSetArg(args[n], XmNbottomShadowColor, bottomshadowcolor); n++;
    XtSetArg(args[n], XmNselectColor, selectcolor); n++;
    XtSetArg(args[n], XmNtroughColor, troughcolor); n++;
    XtSetArg(args[n], XmNarmColor, armedfillcolor); n++;
#endif /*MAGECOLORMAP*/
    XtSetArg(args[n], XmNvisual, DefaultVisual(dpy,DefaultScreen(dpy))); n++;
    XtSetArg(args[n], XmNdepth,  DefaultDepth(dpy,DefaultScreen(dpy))); n++;
    XtSetArg(args[n], XmNcolormap,DefaultColormap(dpy,DefaultScreen(dpy))); n++;
    XtSetArg(args[n], XmNeditable, True); n++;

    XtSetArg(args[n], XmNselectionLabelString,xmstring1); n++; /*user message*/
    XtSetArg(args[n], XmNtextString,xmstring2); n++; /*OutStr*/


    /*creation step of the file selection dialog box*/
    fileoutdialog = 
              XmCreateFileSelectionDialog(grafwindow,"openFileDialog",args,n);
    XtManageChild(fileoutdialog); /*puts the file dialog widget on screen */
     /*Things seem to work better if XtManageChild is done right away. 020209*/

    /*force the filter Directory and file mask as specified for this task*/
    XmFileSelectionDoSearch(fileoutdialog,xmstring3);

    /*add call-back proceedures for OK and Cancel*/
    XtAddCallback(fileoutdialog,XmNokCallback
                      ,(XtCallbackProc)fileout_OK_CB,NULL);
    XtAddCallback(fileoutdialog,XmNcancelCallback
                      ,(XtCallbackProc)fileout_CANCEL_CB,NULL);
  if(kind==15) /*17 does not use this user choice file dialog*/
  {
    printf("input file: %s, try output file: %s\n",NameStr,OutStr);
  }
    /*set the invented file name as a starting suggestion*/
    /*this has to be done after the XmFileSelectionDoSearch() functions*/
    /*but motif,Xt,X11, whatever, does NOT always get that done first!*/
    for(j=0;j<32000;j++) for(k=0;k<2000;k++); /*delay helps here ?? */
    /*k==10000 sure but too obvious a delay*/
    XtVaSetValues(fileoutdialog
                  ,XmNtextString,xmstring2
                  ,NULL);

#ifdef DONOTDO 
/*setting Edit/DrawNew munges something that fouls XmStringFree*/
/*but later always segmentation fault with XmStringFree*/
/*otherwise, seems like a good idea to free storage*/
    XmStringFree(xmstring3);
    XmStringFree(xmstring1);
    XmStringFree(xmstring2);
#endif

/*#define PRINT*/
#ifdef PRINT
printf("dooutputfiledialog, kind: %d\n",kind);
printf("dooutputfiledialog, NameStr: %s\n",NameStr);
printf("dooutputfiledialog, DirStr:  %s\n",DirStr);
printf("dooutputfiledialog, OutStr:  %s\n",OutStr);
#endif

    ACTIVE_DLOG = 1; /*Avoids my key capture for graphics control*/
}
/*___dooutputfiledialog()____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getcaptiontop()**********************************************************/
int  getcaptiontop(char* tmpstr, int num)
{
    long         icharhit=0,icharend=0,icharbegin=0;
    int          j=0,Ldone=0;
    char *       mychar; 
    
    mychar = XmTextGetString(captwindow);

    icharhit = 0;
    icharbegin = 0;
    icharend = XmTextGetLastPosition(captwindow)-1;
    /*window always seems to get an extra line at the bottom */
    /*as if an extra linefeed is on end of text file, so back off one char*/
    mychar[icharend] = '\0'; /*cribbed from writecaption*/

    if(strlen(mychar) > 0)
    {/*there is caption*/
       /*space in over the "Kinemage  %d - " characters*/
       /*Mage always puts this on, so always strip it off beginning*/
       while(mychar[icharbegin]!='-') {icharbegin++;}
       icharhit = icharbegin = icharbegin+2;

       while(icharhit<icharend)
       {
          for(j=0; j<(num-1); j++)
          {
             icharhit = icharbegin+j;
             if(icharhit<icharend) /*so to enforce \0 as last character*/
             {
                tmpstr[j] = mychar[icharhit];
                /*presume input has filtered to appropriate EOL*/
             }
             else
             {/*last character*/
                 tmpstr[j] = '\0';
                 Ldone = 1;
                 break;
             }
          }
          if(Ldone) {break;}
          icharbegin = icharhit+1;
       }
       if(tmpstr[j] != '\0')
       {
          if(j < (num-1)) {j++;}
          tmpstr[j]='\0';
       }
    }/*there is caption*/
    XtFree(mychar); /*990209*/
    return(j);
}
/*___getcaptiontop()_________________________________________________________*/

/****writecaption()***********************************************************/
int    writecaption()
{
    char *   mychar; 
    int  k;
    
    /*begin write caption*/
    mychar = XmTextGetString(captwindow);
    if(strlen(mychar) > 0)
    {/*there is caption*/
       k=0;
       while(mychar[k]!='-') k++;
       k = k+2;

       /*window always seems to get an extra line at the bottom */
       /*as if an extra linefeed is on end of text file, so back off one char*/
       mychar[XmTextGetLastPosition(captwindow)-1] = '\0';/*990209*/

       fprintf(fpout,"@caption\n");
       fprintf(fpout,"%s\n",&mychar[k]);
       XtFree(mychar); /*990209*/
    }/*there is caption*/
    return(0);
}
/*___writecaption()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****writetext()**************************************************************/
int    writetext() 
{
    char *   mychar;

    /*begin write text*/
    mychar = XmTextGetString(textwindow);
    /*window always seems to get an extra line at the bottom */
    /*as if an extra linefeed is on end of text file, so back off one char*/
    mychar[XmTextGetLastPosition(textwindow)-1] = '\0';/*990209*/
    fprintf(fpout,"@text\n");
    fprintf(fpout,"%s\n",mychar);
    XtFree(mychar); /*990209*/
    return(0);
}
/*___writetext()_____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****writequesanswers()*******************************************************/
int    writequesanswers() /*931113*/
{
    int         j,ireturn,iend,Lquestionhere,flowID,Lnew;
    /*char        filenameStr[256];*/
    char *   mychar; 
    enum
    {
         skipID = 1
        ,numID = 2
        ,keepID = 3
    };

            /*begin scanning text*/
            mychar = XmTextGetString(textwindow);
            Lquestionhere = 0;
            iQstring = 0;
            iend = XmTextGetMaxLength(textwindow);
            flowID = skipID;
            fprintf(fpout,"Questions from file: %s\n",NameStr);
            for(j=0;j<iend;j++)
            {/*scan over all char in text buffer*/
                if(   (char)(*(mychar+j-4)) == '*'
                   && (char)(*(mychar+j-3)) == '{'
                   && (char)(*(mychar+j-2)) == 'Q'
                   && (char)(*(mychar+j-1)) == ':'
                   && (char)(*(mychar+j  )) == ' ' )
                {/*find the number*/
                    flowID = numID;
                    Lquestionhere = 1;
                    Lnew = 1;
                }
                if(   (char)(*(mychar+j-1)) == '}'
                   && (char)(*(mychar+j  )) == '*' )
                {/*found all the question*/
                    flowID = skipID;
                    if(Lquestionhere)
                    {/*really was a question*/
                         Qstring[iQstring-1] = '\0'; /*lop off bracket*/
                         fprintf(fpout,"Q:%d: %s\n"
                            ,nQuestion,Qstring);
                         fprintf(fpout," A:%d: %s\n"
                            ,nQuestion,thisanswer[nQuestion]);

                    }
                    Lquestionhere = 0;
                    iQstring = 0; /*reset char counter*/
                }

                switch(flowID)
                {/*switch*/
                    case skipID:
                        /* neutral: no flow designated, discard characters */
                    break;
                    case numID:
                        /*when done, getinteger returns as 1  */
                        if( getinteger((char)(*(mychar+j)),Lnew))/*MAGEINPT.C*/
                        {
                            nQuestion = intpass;
                            /*intpass global to pass int back */
                            flowID=keepID; /* accummulate characters */
                        }
                        Lnew = 0;
                    break;
                    case keepID:
                        Qstring[iQstring++] = (char)(*(mychar+j  ));
                    break;

                }
            }/*scan over all char in text buffer*/
            ireturn = 1;
            fclose(fpout);
            fpout = NULL;
    return(ireturn);
}
/*___writequesanswers()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

