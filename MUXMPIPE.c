/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUX_OUT.c */

#include "MAGE.h"
#include "MAGEDLOG.h"
#include "MAGELIST.h"
#include "MAGEBBOX.h"
#include "MAGEFLAG.h"


#define MAXPIPEBUFF 80

typedef struct pipebuffstruct {
   struct pipebuffstruct* nextptr;
   char buff[MAXPIPEBUFF+1];
   int putth;
   int getth;
   int max;
}pipebuffstruct;

struct  pipebuffstruct* firstpipebuffptr;
struct  pipebuffstruct* lastpipebuffptr;
struct  pipebuffstruct* frompipebuffptr;
struct  pipebuffstruct* intopipebuffptr;

XtWorkProcId theworkprocID = (int)NULL;

/*prototypes*/
pipebuffstruct* allocpipebuffstruct(void);
int putintopipebuffer(char);
void readpipeintobuffer(int);
Boolean readpipeworkproc(XtPointer);

void update(int putout, char command[256]);

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****p2sys()******************************************************************/
/* name: p2sys.c                                      */
/* author: J. Michael Word      date written: 5/20/97 */
/* purpose: spawn off a command and return pipes for  */
/*          communication with the process            */

/* WARNING: You must call p2done() to stop the child  */
/*          process before stopping the parent.       */
/*          Otherwise the child can become a Zombie!  */

/* WARNING: If you overflow the data buffer when      */
/*          writing to the child process, you will    */
/*          hang. This is mostly a problem if the     */
/*          child produces a large amount of output   */
/*          before it has finished reading its input. */
/*          The buf max = PIPE_BUF defined in limits.h*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/file.h>

#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif 
/*121110 OPEN_MAX had been 20, 256 does not change behavior */

/* record of file pointers and process ids */
struct child_io_rec {
   pid_t id;
   FILE *fileptr[2];
} P2SYS_KidInfo[OPEN_MAX];

/*limits.h:   (/usr/include/linux/limits.h)*/
/*#define OPEN_MAX 256*/ /* max files a process can have open */
/*DEC alpha does not define OPEN_MAX, probably safe just to define ?*/
/* better to find equivalent header and name ! */
/* since IRIX6.5 defines it as just 20 */

static int icountupdates=0; /*121110 debug cycle counter*/

static int P2SYS_Kid = 0;

#define AFD 0
#define BFD 1

/* p2sys() - executes a shell command and returns a vector with     */
/*           two file pointers. The first is for writing data to    */
/*           the command and the second is for reading the results. */
FILE** p2sys(char *shell_command) 
{
   int fifo1[2], fifo2[2]; /*file descriptors are int, see man pipe */

   fifo1[0] = fifo1[1] = fifo2[0] = fifo2[1] = -1;
   /*create two two-way pipes*/
   if (pipe(fifo1) || pipe(fifo2)) 
   { 
      perror("pipe"); 
      return NULL; 
   }
   switch(P2SYS_KidInfo[P2SYS_Kid].id = fork()) 
   {/* make child */
      case -1: 
         perror("fork"); 
         return NULL;
      case  0: /*child gets told that its pid_t id == 0 */
         close(fifo1[BFD]); /* we are now in the child process*/
         close(fifo2[AFD]);

         dup2(fifo1[AFD],  STDIN_FILENO);
         dup2(fifo2[BFD], STDOUT_FILENO);

         close(fifo1[AFD]);
         close(fifo2[BFD]);

         execl("/bin/sh", "sh", "-c", shell_command, (char *)0);
         perror("execl"); /* if execl works we can't ever get here */
         _exit(EXIT_FAILURE);
   }
   /*meanwhile, parent reaches here with finite pid_t id value*/
   close(fifo1[AFD]); /* this code runs in the parent process */
   close(fifo2[BFD]);

   if (Lpipeactive) {  /* this is how we make long-active pipes non-blocking */
      fcntl(fifo2[AFD], F_SETFL, FNDELAY);
      /* fcntl(fifo2[AFD], F_SETFL, FNONBLK); */
   }

   P2SYS_KidInfo[P2SYS_Kid].fileptr[0] = fdopen(fifo1[BFD], "w");
   P2SYS_KidInfo[P2SYS_Kid].fileptr[1] = fdopen(fifo2[AFD], "r");
   return P2SYS_KidInfo[P2SYS_Kid++].fileptr;
}
/*___p2sys()_________________________________________________________________*/

/****p2closeData()***********************************************************/
/* p2closeData() - closes the data pipe (but not the results pipe) */
int p2closeData(FILE **fptr) 
{
/*121110 pipe files not being properly closed,*/
/*121110  update fails for too many files open at about 248*/ 
/*121110 fcntl() test seems to be returning as error status*/

   int i=0, rc = -1;
   int TESTINT=0;
   
   for(i=0; i < P2SYS_Kid; i++) 
   {
      if (P2SYS_KidInfo[i].fileptr == fptr) 
      { 
         break; 
      }
   }
   if (i < P2SYS_Kid) 
   {
      /*linux may have closed the file without setting file descriptor = 0*/
      if (fptr[0]) 
      { 
         /*int fcntl(int fd, int cmd);*/
         /*use dummy read of descriptor's flags to check if really open*/
         TESTINT = fcntl((int)fptr[0], F_GETFD);
         if(TESTINT == EBADF || TESTINT == -1)
         {/*fptr[0] is not an open file descriptor*/
            fptr[0] = NULL;
         }
         else
         {
            fclose(fptr[0]); 
            fptr[0] = NULL; 
         }
      }
      rc = 0;
   }
   return rc;
}
/*___p2closeData()___________________________________________________________*/

/****p2done()*****************************************************************/
/* p2done() - closes both pipes to the command  */
int p2done(FILE **fptr) 
{ 
/*121110 pipe files not being properly closed,*/
/*121110  update fails for too many files open at about 248*/ 
/*121110 fcntl() test seems to be returning as error status*/

   int i=0, status=0, rc = -1, j=0;
   int TESTINT=0;
   
   for(i=0; i < P2SYS_Kid; i++) 
   {
      if (P2SYS_KidInfo[i].fileptr == fptr) 
      {
         break; 
      }
   }
   if (i < P2SYS_Kid) 
   {
      for(j=0; j<=1; j++)
      {/*close both pipes*/
         if(fptr[j]) 
         {/*maybe still open*/ 
            /*int fcntl(int fd, int cmd);*/
            /*use dummy read of descriptor's flags to check if really open*/
            TESTINT = fcntl((int)fptr[j], F_GETFD);
            if(TESTINT == EBADF || TESTINT == -1)
            {/*fptr[j] is not an open file descriptor*/
               fptr[j] = NULL;
            }
            else
            {
               fclose(fptr[j]); 
               fptr[j] = NULL; 
            }
         }
      }/*close both pipes*/

      kill(P2SYS_KidInfo[i].id, SIGKILL); /* deliberate infantcide */

      if (waitpid(P2SYS_KidInfo[i].id, &status, 0) == P2SYS_KidInfo[i].id
                                           || WIFEXITED(status)) {
	 rc = WEXITSTATUS(status);
      }
      for(i++; i < P2SYS_Kid; i++) { P2SYS_KidInfo[i-1] = P2SYS_KidInfo[i]; }
      P2SYS_Kid--;
   }
   return rc;
}
/*___p2done()________________________________________________________________*/
 
/****doupdate()*********remote update*****************************************/
void doupdate(int putout, char command[256])
{
   static char commandstr[256];
   static unsigned int Previousgroupkind=0;
   int  Lredobutton=0;

   if(command[0]=='"')
   {/*as of 030315 only probe called with ditto*/
      if(Nthisgroupkind==PROBEDOTSKKIND)
         {strcpy(commandstr,update_probe_command_line);}
      else if(Nthisgroupkind==MAPCONTOURSKKIND)
         {strcpy(commandstr,update_contour_command_line);}
      else if(Nthisgroupkind==PREKINAPPENDKKIND)
         {strcpy(commandstr,update_prekin_command_line);}
      else if(Nthisgroupkind==PIPEINPUTKKIND)
         {strcpy(commandstr,update_command_line);}
      else if(Nthisgroupkind==NOEDISTANCESKKIND)
         {strcpy(commandstr,update_NOE_command_line);}
   }
   else
   {/*NOT ditto an earlier commandstr*/
      strcpy(commandstr, command); /*commandstr stores previous commandline*/
   }

   if(Nthisgroupkind==PROBEDOTSKKIND)
   {
      if(Ldeleteprobedots) { killkindofgroups(PROBEDOTSKKIND); Lredobutton=1;}
   }
   else if(Nthisgroupkind==MAPCONTOURSKKIND)
   {
      if(Ldeletemapcontours) {killkindofgroups(MAPCONTOURSKKIND);Lredobutton=1;}
   }
   else if(Nthisgroupkind==PREKINAPPENDKKIND)
   {
     if(Ldeleteprekinappend){killkindofgroups(PREKINAPPENDKKIND);Lredobutton=1;}
   }
   else if(Nthisgroupkind==PIPEINPUTKKIND)
   {
      if(Ldeletepipeinput) { killkindofgroups(PIPEINPUTKKIND); Lredobutton=1;}
   }
   else if(Nthisgroupkind==NOEDISTANCESKKIND)
   {
      if(LdeleteNOEinput) { killkindofgroups(NOEDISTANCESKKIND); Lredobutton=1;}
   }
   Lreplacecaptiontexton = 0;
   update(putout, commandstr); /*first pass: run remote program*/
   if(Lredobutton){UpdateAppendedSets(Nthisgroupkind);}
    
   if(Nthisgroupkind==PROBEDOTSKKIND && Lreplacecaptiontext)
   {/*second run to get score*/
      /*sprintf(word,"%s -c -u",commandstr);*/
      sprintf(word,"%s -c ",commandstr);
      Lreplacecaptiontexton = 1;
      update(1, word);  /*second pass: run remote program*/
      Lreplacecaptiontexton = 0;
   }
          
   if(Nthisgroupkind != Previousgroupkind)
   {
      LneedSetsinGrafWindow = 1;
   }
   Previousgroupkind = Nthisgroupkind;
}
/*___doupdate()______________________________________________________________*/

/****update()*********remote update*******************************************/
void update(int putout, char command[256])
{ /*121110 CRASHES AFTER 248 calls: "Too many open files":NOT closing them ??*/
   FILE** p2sys(char *shell_command);
   int p2closeData(FILE **fptr);
   int p2done(FILE **fptr);
   int i=0,j=0;
   int TESTINT=0;

#define BUFSZ 1000

   FILE **fpv = NULL;
   FILE *fptest = NULL;
   Lnewfile = 0; /*reset new file name available flag*/
   /*make sure mage input and output files are closed*/
   /*can't trust that finite value in FILE pointers means file is open*/
   /*and closing a not-open file will crash in linux*/
      for(j=0; j<=1; j++)
      {/*close both input and output files*/
         if(j==0) fptest = fpappend;
         else     fptest = fpout;
         if(fptest) 
         {/*maybe still open*/ 
            /*int fcntl(int fd, int cmd);*/
            /*use dummy read of descriptor's flags to check if really open*/
            TESTINT = fcntl((int)fptest, F_GETFD);
            if(TESTINT == EBADF || TESTINT == -1)
            {/*fptest is not an open file descriptor*/
               fptest = NULL;
            }
            else
            {
               fclose(fptest); 
               fptest = NULL; 
            }
         }
      }/*close both input and output files*/
   /*if(fpappend != NULL) fclose(fpappend);*/ /*close a previous append file*/
   /*if(fpout != NULL) fclose(fpout); fpout=NULL;*/
            /* if a previous output file is open, close it */

   /*request p2sys() for pipe pseudo files to write to: fpv[0], */
   /*and read from: fpv[1]*/
   if ((fpv = p2sys(command)) == NULL) 
   {
      perror("p2sys");
      exit(1);
   }
   if(putout)
   {/*write coords of rotation group, to pipe, e.g. to PROBE */
      fpout = fpv[0];
      Lkinformat = 0; /*NOT kin format, so will be PDB format*/
   
      writerotated(); /*MAGEOUT.c*/
   }/*write coords of rotation group, to pipe, e.g. to PROBE */

   /*close the pseudo file pipe line that when out to the remote program*/
   p2closeData(fpv); /*this will close fpv[0] == fpout*/
   /*ensure that fpout looks closed*/
   fpout = NULL;


   /*now read in the stuff generated by the remote program*/
   /*using MAGE parser routines*/
   fpappend = fpv[1];
   Lappend = 1;
   /*GetStuffFromFile() will set fpin = fpappend*/
   if(Lreplacecaptiontexton) 
   {
      replacecaptiontext();
   }
   else
   {
      GetStuffFromFile(); /*MAGEINPT.c*/
   }
   if(nmaster > 0) 
   {
      for(i=1; i<=nmaster; i++) resetmaster(i);/*MAGEBBOX*/ /*030425*/
      /*each master has a mask position based on "nmaster"*/
   }
   Lappend = 0;
   /*close the pseudo file pipe line that came in from the remote program*/
   p2done(fpv); /*trys to close both fpv[0] and fpv[1]*/
   fpappend = NULL; /*ensure that fpappend looks closed*/
}
/*___update()________________________________________________________________*/

/****launchnleave()*********remote launch*************************************/
void launchnleave(int putout, char command[256])
{ /*121110 launchnleave() does not seem to be ever called*/
   FILE** p2sys(char *shell_command);
   int p2closeData(FILE **fptr);
   int p2done(FILE **fptr);
   int TESTINT=0;

#define BUFSZ 1000

   FILE **fpv = NULL;
   FILE *fptest = NULL;

   static char commandstr[256];

   if(command[0]!='"')
   {/*NOT ditto an earlier commandstr*/
      strcpy(commandstr, command);
   }
   Lnewfile = 0; /*reset new file name available flag*/
   /*make sure mage input and output files are closed*/
   /*can't trust that finite value in FILE pointers means file is open*/
   /*and closing a not-open file will crash in linux*/
      {/*close output file*/
         fptest = fpout;
         if(fptest) 
         {/*maybe still open*/ 
            /*int fcntl(int fd, int cmd);*/
            /*use dummy read of descriptor's flags to check if really open*/
            TESTINT = fcntl((int)fptest, F_GETFD);
            if(TESTINT == EBADF || TESTINT == -1)
            {/*fptest is not an open file descriptor*/
               fptest = NULL;
            }
            else
            {
               fclose(fptest); 
               fptest = NULL; 
            }
         }
      }/*close output files*/

   /*request p2sys() for pipe pseudo files to write to: fpv[0], */
   /*and read from: fpv[1]*/
   if ((fpv = p2sys(commandstr)) == NULL) 
   {
      perror("p2sys");
      exit(1);
   }
   if(putout)
   {/*write coords of html help to pipe, e.g. to netscape */
      fpout = fpv[0];
      writehelptohtmlfile(); /*writes help in html format to fpout*/ /*020929*/
   }/*write coords of html help to pipe, e.g. to netscape */
   /*close the pseudo file pipe line that when out to the remote program*/
   p2closeData(fpv); /*this will close fpv[0] == fpout*/
   /*ensure that fpout looks closed*/
   fpout = NULL;
   /*This leaves the child process alive, but we are ignoring the pipe from it*/
}
/*___launchnleave()__________________________________________________________*/

/****doupdatefrompipe()*********remote update*********************************/
void doupdatefrompipe(int putout, char command[256]) /*991213*/
{
   FILE** p2sys(char *shell_command);
   int p2closeData(FILE **fptr);
   int p2done(FILE **fptr);

#define BUFSZ 1000

   FILE **fpv = NULL;

   static char commandstr[256];
 if(putout == -1)
 {/*initial call, open pipes, send command */
   if(command[0]!='"')
   {/*NOT ditto an earlier commandstr*/
      strcpy(commandstr, command);
   }
   Lnewfile = 0; /*reset new file name available flag*/
   if(fpappend != NULL) fclose(fpappend);   /*close a previous append file*/

   if(fpout != NULL) 
   {/* if a previous output file is open, close it */
      fclose(fpout);    
      fpout = NULL;
   }
   Lpipeactive = 1;
   /*request p2sys() for pipe pseudo files to write to: fpv[0], */
   /*and read from: fpv[1]*/
   if ((fpv = p2sys(commandstr)) == NULL) 
   {
      perror("p2sys");
      exit(1);
   }
   
   /*close the pseudo file pipe line that when out to the remote program*/
   p2closeData(fpv); /*this will close fpv[0] == fpout*/

   /*register an XtWorkProc() */
   theworkprocID = XtAppAddWorkProc(app_context, readpipeworkproc, NULL);
 }/*initial call, open pipes, send command */



 if(putout < 100)
 {/*pipe is to be still active*/
   /*option to delete group(s) previously appended to this kinemage*/
   if(Ldeletepipeinput)
   {
      killkindofgroups(PIPEINPUTKKIND);
   }

   /*now get set to read in the stuff generated by the remote program*/
   /*using MAGE parser routines*/
   fpappend = fpv[1];
   Lappend = 1;
   /*GetStuffFromFile() will set fpin = fpappend*/
   /*XtWorkProc will eventually call GetStuffFromFile(); MAGEINPT.c*/
   /*Boolean readpipeworkproc(XtPointer client_data )  MUXMPIPE.c */
 }/*pipe is to be still active*/
 else
 {/*KILL pipe*/
   /*close the pseudo file pipe line that came in from the remote program*/
   p2done(fpv);
   Lpipeactive = 0;
   XtRemoveWorkProc(theworkprocID); 
 }/*KILL pipe*/

}
/*___doupdatefrompipe()______________________________________________________*/

#ifdef ORIGFROMJMW

#include <stdio.h>

FILE** p2sys(char *shell_command);
int p2closeData(FILE **fp);
int p2done(FILE **fp);

#define BUFSZ 1000

void main(int argc, char **argv) {
   FILE *pdbf = NULL;
   FILE **fpv = NULL;
   char buf[BUFSZ];
   int i, j;

   if (argc != 3) {
      fprintf(stderr, "syntax: %s shell_command PDBfile\n", argv[0]);
      exit(1);
   }
   printf("*command: '%s'\n", argv[1]);
   printf("*PDBfile: '%s'\n", argv[2]);

   if ((pdbf = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
   }
   if ((fpv = p2sys(argv[1])) == NULL) {
      perror("p2sys");
      exit(1);
   }
   while(fgets(buf, BUFSZ, pdbf)) {
      fputs(buf, fpv[0]);
   }
   fclose(pdbf);
   p2closeData(fpv);
   while(fgets(buf, BUFSZ, fpv[1])) {
      printf(">>%s", buf);
   }
   p2done(fpv);
   printf("*done.\n");
}
#endif /*ORIGFROMJMW*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****allocpipebuffstruct()****************************************************/
pipebuffstruct* allocpipebuffstruct()
{
   pipebuffstruct* trialpipebuffptr;
   trialpipebuffptr=(pipebuffstruct*)malloc(sizeof(struct pipebuffstruct));
   if(trialpipebuffptr==NULL)
   {/*failed*/
      ; /*error message, if any, goes here*/
   }
   else
   {/*initialize*/
      trialpipebuffptr->buff[0] = '\0';
      trialpipebuffptr->buff[MAXPIPEBUFF] = '\0'; /*i.e. MAXPIPEBUFF+1 position*/
         /*so buffer lines can be written out as printed char strings*/
      trialpipebuffptr->putth = 0;
      trialpipebuffptr->getth = 0;
      trialpipebuffptr->max = MAXPIPEBUFF; /*redundant??*/

      trialpipebuffptr->nextptr = NULL;
      if(lastpipebuffptr != NULL) 
      {/*link old last one to this new one*/
         lastpipebuffptr->nextptr = trialpipebuffptr;
      }
      /*rename this new one to be the designated last one*/
      lastpipebuffptr = trialpipebuffptr;
      /*Now last one properly has nextptr==NULL*/
      if(firstpipebuffptr == NULL)
      {/*special case of first allocation of pipebuffer space*/
         firstpipebuffptr = trialpipebuffptr;
         frompipebuffptr = trialpipebuffptr;
      }
   }
   return(trialpipebuffptr);
}
/*___allocpipebuffstruct()___________________________________________________*/

/****destroyallpipebuff()*****************************************************/
void destroyallpipebuff()
{
   pipebuffstruct* victimptr;
   
   while(firstpipebuffptr != NULL)
   {
      victimptr = firstpipebuffptr;
      firstpipebuffptr = firstpipebuffptr->nextptr;
      free(victimptr);
   }
}
/*___destroyallpipebuff()____________________________________________________*/

/****putintopipebuffer()******************************************************/
int  putintopipebuffer(char c)
{
   int ireturn = 1;
   int Lneedtoput = 1;
   
   while(Lneedtoput)
   {/*Lneedtoput*/
      if(intopipebuffptr == NULL)
      {/*need another buffer structure*/
         if((intopipebuffptr = allocpipebuffstruct()) == NULL)
         {/*trouble*/
            ireturn = 0;
            Lneedtoput = 0; /*no place to put anything anyway*/
         }
         else
         {
            ireturn = 1;
         }
      }/*need another buffer structure*/
      if(ireturn)
      {/*buffer structure OK*/
         if(intopipebuffptr->putth < intopipebuffptr->max)
         {/*still room in this structure*/
            intopipebuffptr->buff[intopipebuffptr->putth++] = c;
            Lneedtoput = 0; /*since just did it*/
            ireturn = 1;
         }/*still room in this structure*/
         else
         {/*ran out of room in this structure*/
            intopipebuffptr = intopipebuffptr->nextptr;
         }/*ran out of room in this structure*/
      }/*buffer structure OK*/
   }/*Lneedtoput*/
   return(ireturn);
}
/*___putintopipebuffer()_____________________________________________________*/

/****getfrompipebuffer()******************************************************/
char getfrompipebuffer()
{
   char c = '\0';
   int  Lneedtoget = 1;
   while(Lneedtoget)
   {/*Lneedtoget*/
      if(frompipebuffptr == NULL)
      {/*frompipebuffptr does not exist yet, we've caught up with put process*/
            c = '\0'; /*end of data so far*/
            Lneedtoget = 0; /*since there is no more to get right now*/
      }/*frompipebuffptr does not exist yet*/
      else
      {/*frompipebuffptr OK*/
         if(frompipebuffptr->getth < frompipebuffptr->putth)
         {/*still characters to get from this structure*/
            c = frompipebuffptr->buff[frompipebuffptr->getth++];
            Lneedtoget = 0; /*since we really did get a char this time*/
         }/*still characters to get from this structure*/
         else if(frompipebuffptr->getth < frompipebuffptr->max)
         {/*caught up with put process*/
            c = '\0'; /*end of data so far*/
            Lneedtoget = 0; /*since there is no more to get right now*/
         }/*caught up with put process*/
         else
         {/*move on to next buffer structure*/
            frompipebuffptr = frompipebuffptr->nextptr;
         }/*move on to next buffer structure*/
      }/*frompipebuffptr OK*/
   }/*Lneedtoget*/
   return(c);
}
/*___getfrompipebuffer()_____________________________________________________*/

/****readpipeintobuffer()*****************************************************/
void readpipeintobuffer(int nchar)
{     
   /*nchar: # characters time-spent tolerance*/
   /* 0 for everything in pipe or everything to @finish*/
 static int       Lgotat = 0;
 static int       keycnt = 0;
 static char keystr[256];

   int  icount = 0;
   int  LOK = 1;
   char c = '\0';
   int  Lfinishflag = 0;

static char reportstr[256];
static int  ireport=0;

   fpin = fpappend; /*reset at end of this routine*/
   while(LOK)
   {/*LOK*/
      /* getnextchar() defined as getc(fpin), fpin == fp or fpappend*/
#ifdef PCWINDOWS
      c = winHandleGetc(readpipe);
#else
      c = getnextchar();
#endif 
if(Ltest && Lappend)
{
   reportstr[ireport++] = c;
   if(c==EOL)
   {
      reportstr[ireport++] = '\0';
      printf("%s\n",reportstr); /*which has an end of line anyway*/
      ireport = 0;
   }
}

      /*note:  EOF is usually -1, force both to be (unsigned int) */
      /* to avoid compiler warning, at least*/
      /*if((unsigned int)c == 255)*/
      if((unsigned int)c == (unsigned int)EOF)
      {/*reached end of file, wait for more to be poured in other end of pipe*/
         LOK = 0;
         break;
      }/*reached end of file*/
      
      /*test incoming stream for certain keywords*/
      if(c == '@') /*kinemage format prescribes @ of a keyword to be first */ 
      {            /* character on a newline.  Check for this here ?? */
         Lgotat = 1;
         keycnt = 0;
         keystr[0] = '\0';
      }
      if(Lgotat)
      {/*in putative keyword*/  /*no space allowed between @ and keyword*/
         if(c==' '||c=='\t'||c=='\n'||c=='\r') /*i.e. @string is the keyword! */
         {/*white space may delineate end of a keyword*/
            if(keycnt >= 5 && keycnt <= 7)
            {/*only looking for 4 char done, 5 char start, or 6 char finish*/
               if(strncmp(keystr,"@start",6)==0)
               {/*start of a kinemage data stream*/
                  Lstartinpipe = 1;
               }/*start of a kinemage data stream*/
               else if((strncmp(keystr,"@finish",7)==0) && Lstartinpipe)
               {/*finish after start*/
                  Lpipebufferloaded = 1;
                  Lcontrolon = 1;
                  if(controlbboxptr != NULL)
                     {buttonboxcheck(controlbboxptr->button, Lcontrolon);} 
                  Lstartinpipe = 0; /*clear for next kinemage set*/
                  LOK = 0; /*stop looking at stream*/
                  Lfinishflag = 1; /*so last char of finish gets into buffer*/
                  c = EOL; /*for a white space after @finish for Mage parser*/
               }/*finish after start*/
               else if(strncmp(keystr,"@done",5)==0)
               {/*remote program has signeled that it is done with making data*/
                  Ldonewithpipe = 1;
               }/*remote program has signeled that it is done with making data*/
            }/*only looking for 4 char done, 5 char start, or 6 char finish*/
            Lgotat = 0; /*lurk for another keyword*/
         }/*white space may delineate end of a keyword*/
         else
         {/*is character of a putative keyword*/
            keystr[keycnt++] = c;
         }/*is character of a putative keyword*/
      }/*in putative keyword*/
      
      /*store character in local buffer*/
      if(Lstartinpipe || Lfinishflag) 
      {/*only store characters between @start and @finish*/
         /*note: this will store inclusive from the first white space after */
         /*@start to the h on the end of the word @finish*/

         if(putintopipebuffer(c) == 0)
         {
            LOK = 0; /*trouble: stop trying to store*/
         }
      }/*only store characters between @start and @finish*/
      icount++;
      if(nchar > 0 && icount >= nchar) 
      {
         LOK = 0; /*time-spent tolerance reached*/
      }
      /*if nchar == 0 gets everything in pipe or everything to @finish*/
   }/*LOK*/
   fpin = fp; /*always reset from Lappend case to be sure */    
}
/*___readpipeintobuffer()____________________________________________________*/

/****flushpipe()**************************************************************/
void flushpipe()
{
   char c = ' ';
   
   destroyallpipebuff();
   firstpipebuffptr = NULL;
   lastpipebuffptr = NULL;
   frompipebuffptr = NULL;
   intopipebuffptr = NULL;

   Lpipebufferloaded = 0;
   Lstartinpipe = 0; /*clear for next kinemage set*/

   fpin = fpappend; /*reset at end of this routine*/
   /*fruitlessly read pipe until EOF*/
   /*note:  EOF is usually -1, force both to be (unsigned int) */
   /* to avoid compiler warning, at least*/
   /*while((unsigned int)c != (unsigned int)255)*/
   while((unsigned int)c != (unsigned int)EOF)
   {
#ifdef PCWINDOWS
      c = winHandleGetc(readpipe);
#else
      c = getnextchar();
#endif 
   }/*this will churn for as long as it takes to catch up to end of file*/
   fpin = fp; /*always reset from Lappend case to be sure */    
}
/*___flushpipe()_____________________________________________________________*/

/****readpipeworkproc()*******************************************************/
Boolean readpipeworkproc(XtPointer client_data )
{
      /*controls for input from an active pipe*/
      if(Lpipeactive)
      {
/*
printf("Lpipeactive and Lpipebufferloaded== %d, Lstartinpipe== %d\n"
,Lpipebufferloaded,Lstartinpipe);
*/
         if(Lpipebufferloaded)
         {
            if(!Linhibiton)
            {
               getstufffrompipebuffer();
            }
         }
         else
         {
            readpipeintobuffer(1); /*up to #char before returning,____PIPE*/
         }
      }
   return(FALSE);
}
/*___readpipeworkproc()______________________________________________________*/

/****getstufffrompipebuffer()*************************************************/
void getstufffrompipebuffer(void)
{
   if(Ldeletepipeinput)
   {
      killkindofgroups(PIPEINPUTKKIND);
   }
   Nthisgroupkind = PIPEINPUTKKIND;
   GetStuffFromFile(); /*MAGEINPT*/
   if(Lpipebufferloaded==0) 
   {
      if(Lnewframenumber) /*use newly set number*/
         sprintf(framenumberstr,"FRAME: %d",framenumber);
      else /*just augment last number*/
         sprintf(framenumberstr,"FRAME: %d",++framenumber);
      LneedSetsinGrafWindow=1;
      redrawvec();
   }
   Nthisgroupkind = 0;
}
/*___getstufffrompipebuffer()________________________________________________*/

/* 
man pipe:  
       #include <unistd.h>
extern int pipe __P ((int __pipedes[2]));  listed in unistd.h
       int pipe(int filedes[2]);
DESCRIPTION
       pipe  creates  a  pair  of file descriptors, pointing to a
       pipe inode, and places them in the  array  pointed  to  by
       filedes.   filedes[0]  is  for  reading, filedes[1] is for
       writing.
RETURN VALUE
       On success, zero is returned.  On error, -1  is  returned,
       and errno is set appropriately.                                          

unistd.h   
   Execute PATH with all arguments after PATH until
   a NULL pointer and environment from `environ'.  
extern int execl __P ((__const char *__path, __const char *__arg, ...));        
   Terminate program execution with the low-order 8 bits of STATUS.  
extern void _exit __PMT ((int __status)) __attribute__ ((__noreturn__));        
*/

