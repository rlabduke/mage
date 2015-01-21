/*    MAGESYNC.c    */
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "MAGE.h"   
 
#define EXTERNSYNC
#include "MAGESYNC.h"
#undef  EXTERNSYNC

#include "MAGEANGL.h"
#include "MAGELIST.h"
#include "MAGEFLAG.h"

#ifdef UNIX_X11
#include <sys/socket.h>
#include <netinet/in.h> 
#include <fcntl.h>
#include <netdb.h> 
   struct sockaddr_in socketinname;
   struct sockaddr_in socketinextraname;
   struct hostent *socketouthost;
   struct sockaddr_in socketoutname;
   struct hostent *socketoutextrahost; /*031228*/
   struct sockaddr_in socketoutextraname; /*031228*/
#endif /*UNIX_X11*/

   char socketinbuf[1024];
/*   int  socketin, newsocketin, socketinlen;*/
/* 050916 TIGER compile re:
 warning: pointer targets in passing argument 3 of 'accept' differ in signedness
*/
   int  socketin, newsocketin;
   unsigned int  socketinlen;
static   int Lsocketin, Lsocketinactive;

   char socketinextrabuf[1024];
/*   int  socketinextra, newsocketinextra, socketinextralen;*/
/* 050916 TIGER compile re:
 warning: pointer targets in passing argument 3 of 'accept' differ in signedness
*/
   int  socketinextra, newsocketinextra;
   unsigned int  socketinextralen;

static   int Lsocketinextra, Lsocketinextraactive;

   char socketoutbuf[1024];
   char socketouthostname[64];
   char socketoutextrabuf[1024];
   char socketoutextrahostname[64]; /*031228*/
   int socketout, newsocketout, socketoutlen;
   int socketoutextra, newsocketoutextra, socketoutextralen;
   int fingerx,fingery;
#define PORT1NUMBER 12341
#define PORT2NUMBER 12342
#define PORT3NUMBER 12343
#define PORT4NUMBER 12344
#ifdef UNIX_X11
XtWorkProcId socketworkprocID = (int)NULL;
XtWorkProcId socketextraworkprocID = (int)NULL;
#endif
/****adjustzoom()*************************************************************/
void adjustzoom()
{
   scalenew = Scale*zoom;
   resetgrafzoomBar(); /*___BAR.C, calc setting from zoom, writezoomvalue*/
   rescalekinemage(); /*MAGEINPT.C*/
   redrawvec(); /*___DRAW.C*/
   if(Lsockets && !Lsocketiteminput && !Lsocketidsearch)
   {
      Lsocketoutactive = 1;
      Lsocketoutzoom = 1;
   }
}
/*___adjustzoom()____________________________________________________________*/

/****adjusttran()*************************************************************/
void adjusttran(int shiftPlace)
{
   resetgrafztranBar(shiftPlace); 
      /*___BAR.C, calc ztran setting from shift, writetranzvalue*/
   redrawvec(); /*___DRAW.C, applies ixtran,iytran,iztran*/
   if(Lsockets && !Lsocketiteminput && !Lsocketidsearch)
   {
      Lsocketoutactive = 1;
      Lsocketouttran = 1;
   }
   /*BUT ztranBar does not seem to be remote socketed */
   /* in contrast to the mouse translations which do seem to be sent out*/
}
/*___adjusttran()____________________________________________________________*/


/****adjustview()*************************************************************/
void adjustview()
{
   resetrot(iviewset); /*MAGEANGL.c*/
   rescalekinemage(); /*MAGEINPT.c*/
   redrawvec(); /*___DRAW.c*/
   if(Lsockets && !Lsocketiteminput && !Lsocketidsearch)
   {
      Lsocketoutactive = 1;
      Lsocketoutview = 1;
   }
}
/*___adjustview()____________________________________________________________*/

/****syncsocketout()**********************************************************/
void syncsocketout(int iend)
{
   int   iang[7]={0,0,0,0,0,0,0},na=0;
   float fang[7]={0,0,0,0,0,0,0};
 iend = iend; /*avoid non-UNIX_X11 error message*/
#ifdef UNIX_X11
   if(iend)
   {/*quit out of sockets*/
      /*inform remote mage to quit out of socket communication*/
      if(Lsocketsame != 3)
      {
         sprintf(socketoutbuf,"@quit\n");
         if(send(socketout, socketoutbuf, sizeof(socketoutbuf), 0) < 0)
         {/*need protection here from broken socket connection*/
            fprintf(stderr,"socketout send failed\n");
         }
      }
      /*then close local sockets*/
      if(newsocketin != 0) close(newsocketin);
      if(socketin != 0) close(socketin);
      if(Lsocketin) XtRemoveWorkProc(socketworkprocID);
      if(socketout != 0) close(socketout);
      if(newsocketinextra != 0) close(newsocketinextra);
      if(socketinextra != 0) close(socketinextra);
      if(Lsocketinextra) XtRemoveWorkProc(socketextraworkprocID);
      if(socketoutextra != 0) close(socketoutextra);
   }/*quit out of sockets*/
   else
   {/*try send to remote computer*/
      if(Lsocketout)
      {/*remote computer has been identified*/
        if(Lsocketoutactive)
        {/*socketoutactive*/
          if(Lsocketoutmatrix)
          {/*usual update of view*/
             sprintf(socketoutbuf,"@matrix %f %f %f %f %f %f %f %f %f %d %d\n"
                ,a11,a12,a13,a21,a22,a23,a31,a32,a33
                ,(mousex - GWIDTH/2)*400/oldmaxwide
                ,(mousey - GHEIGHT/2)*400/oldmaxwide);
          }
          else if(Lsocketouttran)
          {/*translations, send new trans offsets*/
             sprintf(socketoutbuf,"@tran %d %d %d %d %d\n"
                ,ixtran,iytran,iztran
                ,(mousex - GWIDTH/2)*400/oldmaxwide
                ,(mousey - GHEIGHT/2)*400/oldmaxwide);
          }
          else if(Lsocketoutcenter)
          {/*pickcenter, just do centering operation*/
             sprintf(socketoutbuf,"@center %f %f %f %d %d\n"
                ,fxcenternew,fycenternew,fzcenternew
                ,(mousex - GWIDTH/2)*400/oldmaxwide
                ,(mousey - GHEIGHT/2)*400/oldmaxwide);
          }
          else if(Lsocketoutpick)
          {/*general pick, invoke full pick cycle*/
             sprintf(socketoutbuf,"@pick %d %d\n"
                ,(mousex - GWIDTH/2)*400/oldmaxwide
                ,(mousey - GHEIGHT/2)*400/oldmaxwide);
          }
          else if(Lsocketoutview)
          {/*change view*/
             sprintf(socketoutbuf,"@view %d\n",iviewset);
          }
          else if(Lsocketoutzoom)
          {/*change zoom*/
             sprintf(socketoutbuf,"@zoom %f\n",zoom);
          }
          else if(Lsocketoutfinger) /*just update finger==cursor position*/
          {/*fingerx,y: mage screen coords, nominal 400 wide*/
           /*mousex,y, GW,H,oldmaxwide in pixels*/
             sprintf(socketoutbuf,"@finger %d %d\n"
                ,(mousex - GWIDTH/2)*400/oldmaxwide
                ,(mousey - GHEIGHT/2)*400/oldmaxwide);
          }
          else if(Lsocketoutidsearch && pickedpointptr!=NULL)
          {/*search for pointID*/
             recoverptIDstring(comstr, pickedpointptr); /*MAGELIST*/
             sprintf(socketoutbuf,"@idsearch %s",comstr);
          }
          else if(Lsocketoutgrapher && pickedpointptr!=NULL)
          {/*Up-Arrow trigger of picked-point angles to bond-rotatable model*/
             if(pickedpointptr != NULL)
             {/*picked point exists*/
                if(  LbondrotZplot   /* which accepts any picked point...*/
                   ||(  (LbondrotHplot > 0 && LbondrotVplot > 0)
                   &&((pickedpointptr->STATUS & SCREENFLAG) == SCREENFLAG) ) )
                {/*screen oriented and sized plot is present and was last pick*/
                   if(LbondrotZplot > 0) {na = 3;} /*030819 3rd dimension*/
                   else {na = 2;}
                   iang[1] = LbondrotHplot;
                   fang[1] = pickedpointptr->fx;
                   iang[2] = LbondrotVplot;
                   fang[2] = pickedpointptr->fy;
                   iang[3] = LbondrotZplot; /*==0 for 2D case*/
                   fang[3] = pickedpointptr->fz;

                   sprintf(socketoutbuf,"@grapher %d %d %f %d %f %d %f"
                   ,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]);
                   /*triggers remote mage to do its bondrot-grapher operation*/
                   /*Also bondrot#s and angles sent to remote mage*/
                }/*screen oriented and sized plot is present and was last pick*/
             }/*picked point exists*/
          }
          else if(Lsocketreturn)
          {/*request a return connection*/
             sprintf(socketoutbuf,"@return");
          }
          if(send(socketout, socketoutbuf, sizeof(socketoutbuf), 0) < 0)
          {/*need protection here from broken socket connection*/
             fprintf(stderr,"socketout send failed\n");
          }
          Lsocketoutactive = 0;
          Lsocketoutmatrix = 0;
          Lsocketoutfinger = 0;
          Lsocketoutcenter = 0;
          Lsocketouttran = 0;
          Lsocketoutpick = 0;
          Lsocketoutview = 0;
          Lsocketoutzoom = 0;
          Lsocketoutidsearch = 0;
          Lsocketoutgrapher = 0;
          Lsocketoutecho = 0;
          Lsocketreturn = 0;
        }/*socketoutactive*/
      }/*remote computer has been identified*/
   }/*try send to remote computer*/
#endif /*UNIX_X11*/
}
/*___syncsocketout()_________________________________________________________*/

/****syncsocketoutextra()*****************************************************/
void syncsocketoutextra(int iend)
{
   int   iang[7]={0,0,0,0,0,0,0},na=0;
   float fang[7]={0,0,0,0,0,0,0};
 iend = iend; /*avoid non-UNIX_X11 error message*/
#ifdef UNIX_X11
   if(iend)
   {/*send quit to extra remote*/
      /*inform remote mage to quit out of socket communication*/
      sprintf(socketoutextrabuf,"@quit\n");
      if(send(socketoutextra, socketoutextrabuf, sizeof(socketoutextrabuf), 0) < 0)
      {/*need protection here from broken socket connection*/
         fprintf(stderr,"socketoutextra send failed\n");
      }
      /*local sockets closed by old syncsocketout routine*/
   }/*send quit to extra remote*/
   else
   {/*try send to remote computer*/
      if(Lsocketoutextra)
      {/*remote computer has been identified*/
        if(Lsocketoutextraactive)
        {/*socketoutactive*/
          if(Lsocketoutextragrapher && pickedpointptr!=NULL)
          {/*Up-Arrow trigger of picked-point angles to bond-rotatable model*/
             if(pickedpointptr != NULL)
             {/*picked point exists*/
                if(  LbondrotZplot   /* which accepts any picked point...*/
                   ||(  (LbondrotHplot > 0 && LbondrotVplot > 0)
                   &&((pickedpointptr->STATUS & SCREENFLAG) == SCREENFLAG) ) )
                {/*screen oriented and sized plot is present and was last pick*/
                   if(LbondrotZplot > 0) {na = 3;} /*030819 3rd dimension*/
                   else {na = 2;}
                   iang[1] = LbondrotHplot;
                   fang[1] = pickedpointptr->fx;
                   iang[2] = LbondrotVplot;
                   fang[2] = pickedpointptr->fy;
                   iang[3] = LbondrotZplot; /*==0 for 2D case*/
                   fang[3] = pickedpointptr->fz;

                   sprintf(socketoutbuf,"@grapher %d %d %f %d %f %d %f"
                   ,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]);
                   /*triggers remote mage to do its bondrot-grapher operation*/
                   /*Also bondrot#s and angles sent to remote mage*/
                }/*screen oriented and sized plot is present and was last pick*/
             }/*picked point exists*/
          }
          if(send(socketoutextra, socketoutextrabuf, sizeof(socketoutextrabuf), 0) < 0)
          {/*need protection here from broken socket connection*/
             fprintf(stderr,"socketoutextra send failed\n");
          }
          Lsocketoutextraactive = 0;
          Lsocketoutextragrapher = 0;
        }/*socketoutextraactive*/
      }/*remote computer has been identified*/
   }/*try send to remote computer*/
#endif /*UNIX_X11*/
}
/*___syncsocketoutextra()____________________________________________________*/

/****syncsocketin()***********************************************************/
void syncsocketin()
{
#ifdef UNIX_X11
   int Lmatrixonsocketin = 0;
   char headersocketin[64];
   char socstring[1024];
   int n=0;
   int Lfound = 0, Nblank = 0;
   int   iang[7]={0,0,0,0,0,0,0},na=0;
   float fang[7]={0,0,0,0,0,0,0};

   if(Lsocketin)
   {/*socketin*/
         
      if(!Lsocketinactive)
      {
         /*Listen for connection*/
         /*fprintf(stderr,"try to listen for connection to socketin\n");*/
         if(listen(socketin, 5) < 0)
         {
            fprintf(stderr,"socketin not listened to correctly \n");
         }
         /*Accept a connection*/
         if((newsocketin = 
            accept(socketin,(struct sockaddr *) &socketinname,&socketinlen))<0)
         {
            /*fprintf(stderr,"socketin not accepted a connection\n");*/
         } 
         else
         {
            /*fcntl(newsocketin, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
            fcntl(newsocketin, F_SETFL, O_NDELAY);/*make it non-blocking*/
            Lsocketinactive = 1;
         }
      }
      if(Lsocketinactive)
      {/*socketin is active*/
       Lmatrixonsocketin = 1; /*well, maybe*/
       while(Lmatrixonsocketin)
       {/*while loop: use only last matrix queued on socketin*/
         n = recv(newsocketin, socketinbuf, sizeof(socketinbuf), 0);
         if(n > 0)
         {/*something on socketin*/
            if(socketinbuf[0]=='@' && socketinbuf[1]=='m')
            {/*matrix: on socketin, retain only most recent one*/
               /*PEEK at but do not remove next entry on socketin*/
               n = recv(newsocketin,socstring,sizeof(socstring),MSG_PEEK );
               if(n > 0)
               {/*still something on socketin*/
                  if(socstring[0]=='@' && socstring[1]=='m')
                  {/*still another matrix on socketin*/
                     /*allow recycle, really recv entry and process*/
                     Lmatrixonsocketin = 1;
                  }/*still another matrix on socketin*/
                  else
                  {/*NOT another matrix on socketin*/
                     Lmatrixonsocketin = 0;
                  }/*NOT another matrix on socketin*/
               }/*still something on socketin*/
               else
               {/*must be at last matrix on socketin*/
                  Lmatrixonsocketin = 0;
               }
               if(!Lmatrixonsocketin)
               {/*last matrix on socketin*/
                  sscanf(socketinbuf,"%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%d%d"
                  ,headersocketin
                  ,&a11,&a12,&a13,&a21,&a22,&a23,&a31,&a32,&a33
                  ,&fingerx,&fingery);
                  mousex = (fingerx*oldmaxwide/400) + GWIDTH/2;
                  mousey = (fingery*oldmaxwide/400) + GHEIGHT/2;
                  Lfingerin = 1; /*forces finger into drawArea*/
                  redrawvec(); /*which calls fingerplacer(0)*/
               }/*last matrix on socketin*/
            }/*matrix: on socketin, retain only most recent one*/
            else
            {/*process other-than-matrix entry, take any and all*/
               Lmatrixonsocketin = 0;/*NOT waiting for a matrix*/
               if(socketinbuf[0]=='@' && socketinbuf[1]=='c')
               {/*center: operation from remote pickcenter*/
                  sscanf(socketinbuf,"%s%f%f%f%d%d"
                  ,headersocketin
                  ,&fxcenternew,&fycenternew,&fzcenternew
                  ,&fingerx,&fingery);
                  mousex = (fingerx*oldmaxwide/400) + GWIDTH/2;
                  mousey = (fingery*oldmaxwide/400) + GHEIGHT/2;
                  Lfingerin = 1; /*forces finger into drawArea*/
                  rescalekinemage();
                  redrawvec();
               }/*center: operation from remote pickcenter*/
               if(socketinbuf[0]=='@' && socketinbuf[1]=='t')
               {/*tran: translations from remote*/
                  sscanf(socketinbuf,"%s%d%d%d%d%d"
                  ,headersocketin
                  ,&ixtran,&iytran,&iztran
                  ,&fingerx,&fingery);
                  mousex = (fingerx*oldmaxwide/400) + GWIDTH/2;
                  mousey = (fingery*oldmaxwide/400) + GHEIGHT/2;
                  Lfingerin = 1; /*forces finger into drawArea*/
                  /*is a call to adjusttran needed here????*/
                  /*BUT this seems to be mouse translations*/
                  /*while adjusttran is ztran sliderbar which is not socketed*/
                  redrawvec();
               }/*tran: translations from remote*/
               if(socketinbuf[0]=='@' && socketinbuf[1]=='p')
               {/*pick: invoke pick operation with new mousex,y*/
                  sscanf(socketinbuf,"%s%d%d"
                  ,headersocketin
                  ,&fingerx,&fingery);
                  mousex = (fingerx*oldmaxwide/400) + GWIDTH/2;
                  mousey = (fingery*oldmaxwide/400) + GHEIGHT/2;
                  Lfingerin = 1; /*forces finger into drawArea*/
                  ipick = 2; /*flag for pick point recognition at mousex,y*/
                  pickx = mousex;
                  picky = mousey;
                  /*flag nothing picked yet */
                  Lpick = 0;
                  Lpicknow = 0;
                  if(Ldraglineon || Lnewlabelson) 
                  {
                     Lnewdrag = 1;
                     Lnewmatrix = 1;
                  }
                  redrawvec();
                  while(Lpicknow)
                  {/*apply results, if any, of the requested pick*/
                     Lpicknow = 0;/*reset*/
                     redrawvec();
                  }
               }/*pick: invoke pick operation with new mousex,y*/
               if(socketinbuf[0]=='@' && socketinbuf[1]=='f')
               {/*finger: the remote cursor*/
                  sscanf(socketinbuf,"%s%d%d"
                  ,headersocketin
                  ,&fingerx,&fingery);
                  mousex = (fingerx*oldmaxwide/400) + GWIDTH/2;
                  mousey = (fingery*oldmaxwide/400) + GHEIGHT/2;
                  fingerplacer(1); /*refresh, then finger at mousex,y*/
                  /*fingerx,y: mage screen coords, nominal 400 wide*/
                  /*mousex,y, GW,H,oldmaxwide in pixels*/
                  /*fingerx,y is relative to mage draw area center*/
                  /*reverse calc back to computer window x,y*/
                  /*with this mage's GWIDTH, GHEIGHT, oldmaxwide*/
               }/*finger: the remote cursor*/
               if(socketinbuf[0]=='@' && socketinbuf[1]=='i')
               {/*idsearch: invoke search for pointID*/
                  /*sscanf(socketinbuf,"%s%s" ,headersocketin ,search1str);*/
                  for(isearch1=0; socketinbuf[isearch1+10]!='\0';isearch1++)
                  {
                     search1str[isearch1] = socketinbuf[isearch1+10]; 
                     if(search1str[isearch1]==' ') Nblank++; 
                  }
                  search1str[isearch1] = '\0';
                  /*count char*/
                  for(isearch1=0; search1str[isearch1]!='\0';isearch1++)
                  {
                     if(search1str[isearch1]==' ') Nblank++; 
                  }
                  /*Do NOT search for field of blank(s)*/
                  if(Nblank == isearch1) isearch1=0;
                  if(isearch1 > 0)
                  {/*DO SEARCH ON POINT IDs of GRAPHICS ITEMS*/
                     Lsearchbegin = 1; /*search all groups/.../points*/
                     isearch2 = 0; /*doing exact match on just one string*/
                     Lfound = mysearchpointID(1);/*MAGEUTIL.c for justone hit*/
                  }
                  if(Lfound)
                  {
                     /*Lpick,ipick,Lprepick set in mysearchpointID()*/
                     /* as is pickedpointptr, etc. */
                     /*flag that a point has been picked */
                     Lpicknow = 1;
                     redrawvec();
                     while(Lpicknow)
                     {/*apply results, if any, of the requested pick*/
                        Lpicknow = 0;/*reset*/
                        redrawvec();
                     }
                  }
               }/*idsearch: invoke search for pointID*/

               if(socketinbuf[0]=='@' && socketinbuf[1]=='g')
               {/*grapher: trigger picked-point angles to bond-rotatable model*/
                  sscanf(socketinbuf,"%s%d%d%f%d%f%d%f"
                  ,headersocketin
                  ,&na,&iang[1],&fang[1],&iang[2],&fang[2],&iang[3],&fang[3]);
                  /* simulated Up-Arrow for mutually picked point*/
/*
fprintf(stderr,"Lsocketsame== %d, %d, %d, %.2f, %d, %.2f, %d, %.2f\n"
,Lsocketsame,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]);
*/
                  if(Lsocketsame != 3)
                  {/*extra (#3) does not have its own data, only the model*/
                     dobondrotgrapher(); /*MAGEANGL*/
                  }
                  /*then bondrot#s and angles from sending mage*/
                  bondrotgrapher(na,iang,fang);

                  /*echo back to sender*/
                  iang[4] = LbondrotHplot;
                  fang[4] = pickedpointptr->fx;
                  iang[5] = LbondrotVplot;
                  fang[5] = pickedpointptr->fy;
                  iang[6] = LbondrotZplot; /*==0 for 2D case*/
                  fang[6] = pickedpointptr->fz;
                  na = 6; /*general case*/
                  sprintf(socketoutbuf,"@echo %d %d %f %d %f %d %f %d %f %d %f %d %f"
                    ,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]
                       ,iang[4],fang[4],iang[5],fang[5],iang[6],fang[6]);

                  /*all bondrot#s and angles sent to remote mage*/
                  if(send(socketout, socketoutbuf
                             , sizeof(socketoutbuf), 0) < 0)
                  {/*need protection here from broken socket connection*/
                     fprintf(stderr,"mage %d socketout send failed\n"
                                   ,Lsocketsame);
                  }

                  if(Lsocketsame==1 && Lsocketoutextra)
                  {/*Also all bondrot#s and angles sent to 3rd mage instance*/
                   sprintf(socketoutextrabuf,"@echo %d %d %f %d %f %d %f %d %f %d %f %d %f"
                      ,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]
                         ,iang[4],fang[4],iang[5],fang[5],iang[6],fang[6]);
                     if(send(socketoutextra, socketoutextrabuf
                             , sizeof(socketoutextrabuf), 0) < 0)
                     {/*need protection here from broken socket connection*/
                        fprintf(stderr,"mage 1 socketoutextra send failed\n");
                     }
                  }
               }/*grapher: trigger picked-point angles to bond-rotatable model*/

               if(socketinbuf[0]=='@' && socketinbuf[1]=='e')
               {/*echo: all plotted angles to bond-rotatable model*/
                  sscanf(socketinbuf,"%s%d%d%f%d%f%d%f%d%f%d%f%d%f"
                  ,headersocketin
                  ,&na,&iang[1],&fang[1],&iang[2],&fang[2],&iang[3],&fang[3]
                      ,&iang[4],&fang[4],&iang[5],&fang[5],&iang[6],&fang[6]);
/*
fprintf(stderr,"Lsocketsame== %d, %d, %d, %.2f, %d, %.2f, %d, %.2f, %d, %.2f, %d, %.2f, %d, %.2f\n"
,Lsocketsame,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3],iang[4],fang[4],iang[5],fang[5],iang[6],fang[6]);
*/
                  /*all bondrot#s and angles sent to bond rotations of model*/
                  bondrotgrapher(na,iang,fang);

                  if(Lsocketsame==1 && Lsocketoutextra)
                  {/*Also all bondrot#s and angles sent to 3rd mage instance*/
                   sprintf(socketoutextrabuf,"@echo %d %d %f %d %f %d %f %d %f %d %f %d %f"
                      ,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]
                         ,iang[4],fang[4],iang[5],fang[5],iang[6],fang[6]);
                     if(send(socketoutextra, socketoutextrabuf
                             , sizeof(socketoutextrabuf), 0) < 0)
                     {/*need protection here from broken socket connection*/
                        fprintf(stderr,"mage 1 socketoutextra send failed\n");
                     }
                  }
               }/*echo: all plotted angles to bond-rotatable model*/

               if(socketinbuf[0]=='@' && socketinbuf[1]=='v')
               {/*view: change view*/
                  sscanf(socketinbuf,"%s%d"
                  ,headersocketin
                  ,&iviewset);
                  Lsocketiteminput = 1;
                  adjustview(); /*MUXMMAIN.c */
                  Lsocketiteminput = 0;
               }
               if(socketinbuf[0]=='@' && socketinbuf[1]=='z')
               {/*zoom: change zoom*/
                  sscanf(socketinbuf,"%s%f"
                  ,headersocketin
                  ,&zoom);
                  Lsocketiteminput = 1;
                  adjustzoom(); /*MUXMMAIN.c */
                  Lsocketiteminput = 0;
               }
               if(socketinbuf[0]=='@' && socketinbuf[1]=='r')
               {/*return socket connection requested*/
                  setsockets(Lsockets); /*2-->1 is the only valid case*/
               }
               if(socketinbuf[0]=='@' && socketinbuf[1]=='q')
               {/*quit: remote mage has quit socket communication: quit here*/
                  setsockets(3); /*3 terminates socket stuff*/
               }
            }/*process other-than-matrix entry, take any and all*/
         }/*something on socketin*/
         else
         {/*nothing on socketin, clear while loop*/
            Lmatrixonsocketin = 0;
         }
       }/*while loop: use only last matrix queued on socketin*/
      }/*socketin is active*/
   }/*socketin*/
#endif /*UNIX_X11*/
}
/*___syncsocketin()__________________________________________________________*/

/****syncsocketinextra()******************************************************/
void syncsocketinextra()
{
#ifdef UNIX_X11
   char headersocketinextra[64];
   int n=0;
   int   iang[7]={0,0,0,0,0,0,0},na=0;
   float fang[7]={0,0,0,0,0,0,0};

   if(Lsocketinextra)
   {/*socketinextra*/
         
      if(!Lsocketinextraactive)
      {
         /*Listen for connection*/
         /*fprintf(stderr,"try to listen for connection to socketinextra\n");*/
         if(listen(socketinextra, 5) < 0)
         {
            fprintf(stderr,"socketinextra not listened to correctly \n");
         }
         /*Accept a connection*/
         if((newsocketinextra = 
            accept(socketinextra,(struct sockaddr *) &socketinextraname,&socketinextralen))<0)
         {
            /*fprintf(stderr,"socketinextra not accepted a connection\n");*/
         } 
         else
         {
            /*fcntl(newsocketinextra, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
            fcntl(newsocketinextra, F_SETFL, O_NDELAY);/*make it non-blocking*/
            Lsocketinextraactive = 1;
         }
      }
      if(Lsocketinextraactive)
      {/*socketinextra is active*/

         n = recv(newsocketinextra, socketinextrabuf, sizeof(socketinextrabuf), 0);
         if(n > 0)
         {/*something on socketinextra*/

               if(socketinextrabuf[0]=='@' && socketinextrabuf[1]=='g')
               {/*grapher: trigger picked-point angles to bond-rotatable model*/
                  sscanf(socketinextrabuf,"%s%d%d%f%d%f%d%f"
                  ,headersocketinextra
                  ,&na,&iang[1],&fang[1],&iang[2],&fang[2],&iang[3],&fang[3]);
                 /* simulated Up-Arrow for mutually picked point*/
/*
fprintf(stderr,"Lsocketsame== %d, %d, %d, %.2f, %d, %.2f, %d, %.2f\n"
,Lsocketsame,na,iang[1],fang[1],iang[2],fang[2],iang[3],fang[3]);
*/
                 if(Lsocketsame != 3)
                 {/*extra (#3) does not have its own data, only the model*/
                    dobondrotgrapher(); /*MAGEANGL*/
                 }
                 /*then bondrot#s and angles from sending mage*/
                 bondrotgrapher(na,iang,fang);
               }

         }/*something on socketinextra*/
      }/*socketinextra is active*/
   }/*socketinextra*/
#endif /*UNIX_X11*/
}
/*___syncsocketinextra()_____________________________________________________*/


/****setsockets()*************************************************************/
void setsockets(int ion) /*called from MAGEDLOG*/
{
/*see /usr/include/sys/socket.h */
 int LOK = 1; /*logical to deal with both input ports of 3rd mage instance*/
 ion = ion; /*avoid non-UNIX_X11 error message*/

#define TESTEXTRA
#undef  TESTEXTRA
#define TESTUSUAL
#ifdef UNIX_X11
 if(ion==0)
 {/*first thing is to initiate server function*/
   /*Create the socket for input*/
   /*   fprintf(stderr,"try to create socketin \n");*/
#ifdef TESTUSUAL
   if((socketin = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr,"socketin not created\n");
   }
   else
   {
      fprintf(stderr,"socketin %d created\n",socketin);
   }
   /*Create the address of the server*/
   memset(&socketinname, 0, sizeof(struct sockaddr_in));
   socketinname.sin_family = AF_INET;
#endif
#ifdef TESTEXTRA
   if(Lsocketsame == 3)
   {/*3rd instance of mage has 2 input ports*/
      if((socketinextra = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
         fprintf(stderr,"socketinextra not created\n");
      }
      else
      {
         fprintf(stderr,"socketinextra %d created\n",socketinextra);
      }
      /*Create the address of the server*/
      memset(&socketinextraname, 0, sizeof(struct sockaddr_in));
      socketinextraname.sin_family = AF_INET;
   }/*3rd instance of mage has 2 input ports*/
#endif
   /*attach name of input sockets to port numbers*/
   if(Lsocketsame == 3) /*listening as mage number 3*/
   {
#ifdef TESTUSUAL
      socketinname.sin_port = htons(PORT3NUMBER);
      fprintf(stderr,"mage %d,socketin port %d\n",Lsocketsame,PORT3NUMBER);
#endif
#ifdef TESTEXTRA
      socketinextraname.sin_port = htons(PORT4NUMBER);
      fprintf(stderr,"mage %d,socketinextra port %d\n",Lsocketsame,PORT4NUMBER);
#endif
   }
   else if(Lsocketsame == 2) /*listening as mage number 2*/
   {
      socketinname.sin_port = htons(PORT2NUMBER);
      fprintf(stderr,"mage %d, socketin port %d\n",Lsocketsame,PORT2NUMBER);
   }
   else
   {
      socketinname.sin_port = htons(PORT1NUMBER);
      fprintf(stderr,"mage %d, socketin port %d\n",Lsocketsame,PORT1NUMBER);
   }
   socketinlen = sizeof(struct sockaddr_in);
   /*Use the wildcard address*/
   n = INADDR_ANY;
#ifdef TESTUSUAL
   memcpy(&socketinname.sin_addr, &n, sizeof(long));
   /*Bind the socket to the address*/
   /*   fprintf(stderr,"try to bind socketin \n");*/
   if(bind(socketin, (struct sockaddr *) &socketinname, socketinlen) < 0)
   {
      fprintf(stderr,"socketin not bound \n");
      LOK = 0; /*only takes one failed condition*/
   }            
   else
   {
      fprintf(stderr,"socketin %d bound to %p\n",socketin,&socketinname);/*121108 %p for address*/
   }
#endif                                                             
#ifdef TESTEXTRA
   if(Lsocketsame == 3)
   {/*3rd instance of mage has 2 input ports*/
      memcpy(&socketinextraname.sin_addr, &n, sizeof(long));
      /*Bind the socket to the address*/
      /*   fprintf(stderr,"try to bind socketinextra \n");*/
      if(bind(socketinextra, (struct sockaddr *) &socketinextraname, socketinlen) < 0)
      {
         fprintf(stderr,"socketinextra not bound \n");
         LOK = 0; /*only takes one failed condition*/
      }                               
      else
      {
         fprintf(stderr,"socketinextra %d bound to %ld\n",socketinextra,&socketinextraname);
      }
   }/*3rd instance of mage has 2 input ports*/
#endif
   if(LOK)
   {/*OK, got a socket to receive anybody's mage messages*/
      Lsockets = 1; /*level 1: mage will act as server to any remote mage*/
#ifdef TESTUSUAL
      Lsocketin = 1; 
      Lsocketinactive = 0;
      /*fcntl(socketin, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
      fcntl(socketin, F_SETFL, O_NDELAY);/*make it non-blocking*/
      /*register an XtWorkProc() */
      socketworkprocID= XtAppAddWorkProc(app_context,readsocketworkproc,NULL);
#endif
#ifdef TESTEXTRA
      if(Lsocketsame == 3)
      {/*3rd instance of mage has 2 input ports*/
         Lsocketinextra = 1; 
         Lsocketinextraactive = 0;
         /*fcntl(socketinextra, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
         fcntl(socketinextra, F_SETFL, O_NDELAY);/*make it non-blocking*/
         /*register an XtWorkProc() */
         socketextraworkprocID= XtAppAddWorkProc(app_context,readsocketextraworkproc,NULL);
      }/*3rd instance of mage has 2 input ports*/
#endif
      /*prepare a tentative name for a remote computer to send to*/
      /*best one can do is use this computer itself, later can edit name*/
      /*if(gethostname(remote_computer_name, sizeof(remote_computer_name))<0)*/
      /*{*/ /*failed to even get our local computer's name*/
         /*remote_computer_name[0] = '\0';*/
      /*}*/
      /*040106 local computer name is not always viable, use localhost*/
      sprintf(remote_computer_name,"localhost");
   }

 }/*first thing is to initiate server function*/
 else if(ion==1)
 {/*next level is to initiate client function to a named remote computer*/
   Lsocketout = 0;
   Lsocketoutactive = 0;
   Lsocketoutmatrix = 0;
   Lsocketouttran = 0;
   Lsocketoutfinger = 0;
   Lsocketoutcenter = 0;
   Lsocketoutpick = 0;
   Lsocketoutview = 0;
   Lsocketoutzoom = 0;
   Lsocketoutidsearch = 0;
   Lsocketoutgrapher = 0;
   Lsocketoutecho = 0;
   /*get host name that come from the client dialog box*/
   sprintf(socketouthostname,"%s",remote_computer_name);
   /*fprintf(stderr,"socketouthostname: %s\n",socketouthostname);*/
   /*Look up our socketout  host's network address*/
   if((socketouthost= gethostbyname(socketouthostname)) == NULL)
   {
      fprintf(stderr,"unknown host: %s.\n",socketouthostname);
      Lsocketout = 0;
      Lsocketoutactive = 0;
   }
   else
   {/*known host*/
     fprintf(stderr,"socketouthost->h_name: %s\n",socketouthost->h_name);
     /*Create a socket in the INET domain*/
     if((socketout = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
        fprintf(stderr,"failed to create socketout\n");
        Lsocketout = 0;
        Lsocketoutactive = 0;
     }
     else
     {/*socketout created*/
       /*fcntl(socketout, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
       /*Create the address of the server*/
       memset(&socketoutname, 0, sizeof(struct sockaddr_in));
       socketoutname.sin_family = AF_INET;
       if(Lsocketsame == 1) /*sending to mage number 2*/
       {
          socketoutname.sin_port = htons(PORT2NUMBER);
          fprintf(stderr,"mage %d, socketout port %d\n",Lsocketsame,PORT2NUMBER);
       }
       else
       {
          socketoutname.sin_port = htons(PORT1NUMBER);
          fprintf(stderr,"mage %d, socketout port %d\n",Lsocketsame,PORT1NUMBER);
       }
       memcpy(&socketoutname.sin_addr, socketouthost->h_addr_list[0], socketouthost->h_length);
       socketoutlen = sizeof(struct sockaddr_in);
       /*Connect to the server*/
       if(connect(socketout,(struct sockaddr *) &socketoutname,socketoutlen)<0)
       {
         fprintf(stderr,"failed to connect socketout to socketouthost\n");
         Lsocketout = 0;
         Lsocketoutactive = 0;
       } 
       else
       {/*connected socketout to socketouthost*/ 
          Lsockets = 2; /*second level*/
          Lsocketout = 1;
          Lsocketoutactive = 0; /*set when mouse moves*/
       }/*connected socketout to socketouthost*/ 
     }/*socketout created*/
   }/*known host*/
 }/*next level is to initiate client function to a named remote computer*/
 else if(ion==2)
 {/*extra level is to initiate client function to a named remote computer*/
   Lsocketoutextra = 0;
   Lsocketoutextraactive = 0;
   Lsocketoutextragrapher = 0;
   /*get host name that come from the client dialog box*/
   sprintf(socketoutextrahostname,"%s",remote_computer_name);
   /*fprintf(stderr,"socketoutextrahostname: %s\n",socketoutextrahostname);*/
   /*Look up our socketout  host's network address*/
   if((socketoutextrahost= gethostbyname(socketoutextrahostname)) == NULL)
   {
      fprintf(stderr,"unknown host: %s.\n",socketoutextrahostname);
      Lsocketoutextra = 0;
      Lsocketoutextraactive = 0;
   }
   else
   {/*known host*/
     fprintf(stderr,"socketoutextrahost->h_name: %s\n",socketoutextrahost->h_name);
     /*Create a socket in the INET domain*/
     if((socketoutextra = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
        fprintf(stderr,"failed to create socketoutextra\n");
        Lsocketoutextra = 0;
        Lsocketoutextraactive = 0;
     }
     else
     {/*socketoutextra created*/
       /*fcntl(socketoutextra, F_SETFL, FNDELAY);*/ /*make it non-blocking*/
       /*Create the address of the server*/
       memset(&socketoutextraname, 0, sizeof(struct sockaddr_in));
       socketoutextraname.sin_family = AF_INET;

       if(Lsocketsame == 2)
       {/*sending to mage number 3 on port number 4*/
         socketoutextraname.sin_port = htons(PORT4NUMBER);
         fprintf(stderr,"mage %d, socketout port %d\n",Lsocketsame,PORT4NUMBER);
       }
       else
       {/*sending to mage number 3 on port number 3*/
         socketoutextraname.sin_port = htons(PORT3NUMBER);
         fprintf(stderr,"mage %d, socketout port %d\n",Lsocketsame,PORT3NUMBER);
       }

       memcpy(&socketoutextraname.sin_addr, socketoutextrahost->h_addr_list[0], socketoutextrahost->h_length);
       socketoutextralen = sizeof(struct sockaddr_in);
       /*Connect to the server*/
       if(connect(socketoutextra,(struct sockaddr *) &socketoutextraname,socketoutextralen)<0)
       {
         fprintf(stderr,"failed to connect socketoutextra to socketoutextrahost\n");
         Lsocketoutextra = 0;
         Lsocketoutextraactive = 0;
       } 
       else
       {/*connected socketoutextra to socketoutextrahost*/ 
          Lsockets = 3; /*third level*/
          Lsocketoutextra = 1;
          Lsocketoutextraactive = 0; /*set when mouse moves*/
       }/*connected socketoutextra to socketoutextrahost*/ 
     }/*socketoutextra created*/
   }/*known host*/
 }/*extra level is to initiate client function to a named remote computer*/
 else /*Lsockets==3, or special request to kill whatever sockets are there*/
 {/*terminate sockets, both server and client*/
    /*first inform remote to declare a quit*/
    syncsocketout(1); /*tell remote to quit, then close local sockets*/
    socketin = 0;
    newsocketin = 0;
    socketout = 0;
    Lsocketin = 0;
    Lsocketinactive = 0;
    Lsocketout = 0;
    Lsocketoutactive = 0;
    Lsocketoutmatrix = 0;
    Lsocketoutfinger = 0;
    Lsockets = 0;
 }/*terminate sockets, both server and client*/
#endif /*UNIX_X11*/
}                                                                                  
/*___setsockets()____________________________________________________________*/

