/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*****DoQuesDialog************************************************************/
void    DoQuesDialog()
{
  int ncommon;

 if(!ACTIVE_DLOG)
 {
/*overall DLOG box********************************/
  n = 0; 
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
  ncommon = n;
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  XtSetArg(args[n], XmNx, 10); n++;
  XtSetArg(args[n], XmNy, 10); n++;

  XtSetArg(args[n], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL); n++;
  XtSetArg(args[n], XmNtitle, "QUESTION_ANSWER"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING); n++; 
    /*ignore close commands from frame menu or X button*/

  questiondlog = XmCreateFormDialog(grafwindow," ",args,n);
  XtManageChild(questiondlog);

/*questionbox: row column container for everything*/
  n = ncommon; 
  XtSetArg(args[n], XmNwidth, 300); n++;
  XtSetArg(args[n], XmNheight, 200); n++;
  XtSetArg(args[n], XmNorientation, XmVERTICAL); n++;
  XtSetArg(args[n], XmNpacking, XmPACK_TIGHT); n++;
  if (textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  questionbox=XtCreateWidget(" ",xmRowColumnWidgetClass,questiondlog,args,n);
  XtManageChild(questionbox);

/*Subject******************/
  n = ncommon; 
  if(textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNwidth, 200); n++;
  XtSetArg(args[n], XmNrows, 3); n++;
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  questiontext = XtCreateWidget(" ", xmTextWidgetClass,questionbox,args,n);
  XtManageChild(questiontext); 

/* A row for Current Answer */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  questionrowA =
        XtCreateWidget(" ", xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionrowA);

  n = ncommon;
  if(textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, True); n++;
  questionanswer =
           XtCreateWidget(" ", xmTextWidgetClass,questionrowA,args,n);
  XtManageChild(questionanswer); /*manage after widget to Left managed*/            

  n = ncommon;
  if(textfont) { XtSetArg(args[n], XmNfontList, textfontList); n++; }
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg(args[n], XmNeditable, False); n++;
  questionheader =
           XtCreateWidget(" ", xmTextWidgetClass,questionrowA,args,n);
  XtManageChild(questionheader); /*manage after widget to Left managed*/            


/*control buttons: ACCEPT, Cancel, go to current graphics window */
  n = ncommon; 
  XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
  /*XtSetArg(args[n], XmNpacking, XmPACK_COLUMN); n++;*/
  questionbuttons = 
             XtCreateWidget("but", xmRowColumnWidgetClass,questionbox,args,n);
  XtManageChild(questionbuttons); 
  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  question_OK = 
      XmCreatePushButtonGadget(questionbuttons,"ACCEPT",args,n);
  XtAddCallback(question_OK,XmNactivateCallback
       ,(XtCallbackProc)question_OK_CB,NULL); 
  XtManageChild(question_OK);

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  question_CANCEL = 
      XmCreatePushButtonGadget(questionbuttons,"Cancel",args,n);
  XtAddCallback(question_CANCEL,XmNactivateCallback
       ,(XtCallbackProc)question_CANCEL_CB,NULL); 
  XtManageChild(question_CANCEL);

  n = ncommon; 
  if (buttonfontList) { XtSetArg(args[n], XmNfontList, buttonfontList); n++; }
  question_GOGRAPH = 
      XmCreatePushButtonGadget(questionbuttons,"go to current graphics window",args,n);
  XtAddCallback(question_GOGRAPH,XmNactivateCallback
       ,(XtCallbackProc)question_GOGRAPH_CB,NULL); 
  XtManageChild(question_GOGRAPH);

  ACTIVE_DLOG = QUESTION_DLOG;
  questionDialog();
 }/*not otherwise ACTIVE_DLOG*/
}
/*___DoQuesDialog____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****questionDialog()*********************************************************/
void questionDialog()
{
   XmString    xmstring;


   for(j=0;j<=iQstring;j++) word[j] = Qstring[j]; 
   XmTextSetString(questionanswer, word);  

   sprintf(word,"Current Answer: Question # %d",nQuestion);
   XmTextSetString(questionheader, word);  

}
/*___questionDialog()________________________________________________________*/

/****question_OK_CB()********************************************************/
XtCallbackProc question_OK_CB(Widget w,XtPointer client_data
                                      ,XtPointer call_data)
{
   char thestr[256];
   int j=0;

   thestr = (char *)XmTextGetString(questionanswer);
   for(j=0; thestr[j]!='\0'&&j<MAXQCHAR; j++)
         thisanswer[nQuestion][j] = thestr[j];                                        
   itextoffset=itextoffset+3; /* bracket,*,_ then 1st char of answer*/
   /*replace old answer (or ____...) with thisanswer in text field*/            

   LneedSetsinGrafWindow = 1; 
   redrawvec();              /*MUXMDRAW.c*/
   XtDestroyWidget(questiondlog); 
   ACTIVE_DLOG = 0;
   return(NULL);
}
/*___question_OK_CB_________________________________________________________*/

/****question_CANCEL_CB()****************************************************/
XtCallbackProc pickshow_CANCEL_CB(Widget w,XtPointer client_data
                                          ,XtPointer call_data)
{
    redrawvec();
    XtDestroyWidget(questiondlog); 
    ACTIVE_DLOG = 0;
    return(NULL);
}
/*___question_CANCEL_CB_____________________________________________________*/
