/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*MUX_FILE.C*/

#include "MAGE.h"  

/****newfile()****************************************************************/
void    newfile()    
{        /*enters here with Lnewfile=1 for request to open a new file */
    
    Lstartfile = 0; /*doesn't matter how gets here in UNIX, */
    if(Lappend)
    {
       openfilebyname(AppendNameStr); /*MUXMFILE.C*/ /*just opens the file*/
    }
    else
    {
       openfilebyname(NameStr); /*MUXMFILE.C*/ /*just opens the file*/
    }
    if( IOerr == 0 && Lappend == 0)
    {
        /*then trys to get kinemage text, or at least find @kinemage */
        /* if that successfull then IOerr = 0 */
        GetTextFromFile();/*MAGEFILE.c*/
    }
    if(IOerr == 0) 
    {
        Lreentry = 1; /*Got text, entry() will get rest */
        entry(); /*MAGEFILE.c*/
#ifdef UNIX_PEX
        resetpex();       /*MUXPHIG.c*/
#endif /*UNIX_PEX*/
    }
}
/*___newfile()_______________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getafile()**************************************************************/
int        getafile()  /*dummied for UNIX*/
{
    int   ireturn;

    Lnewfile = 1;
    ireturn = 1;
    IOerr = 0;
    return(ireturn);
}
/*___getafile()_____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/*****openfilebyname()********************************************************/
int openfilebyname(char filename[256]) /* <- newfile() MUXMFILE.C*/
{
   FILE *fptry = NULL;

   /*close a previous file*/
   if(Lappend)
   {
      if(fpappend != NULL) fclose(fpappend);
   }
   else if(fp != NULL)
   {
      fclose(fp);
   }

   fptry = fopen(filename,"r");
   if(fptry == NULL)
   {
      IOerr = 1;
      fprintf(stderr," failed to open file: %s\n",filename);
   }
   else /* file opened, get characters from it */
   { /*valid input file*/
      IOerr = 0;
      if(Lappend)
      {
         fprintf(stderr," MAGE opened append file: %s\n",filename);
         fpappend = fptry;
      }
      else
      {
         fprintf(stderr," MAGE opened new file: %s\n",filename);
         fp = fptry;
         Lnewfile = 0; /*reset new file name available flag*/
         Lorigkinfile = 0; /*fp might NOT be original input file*/ 
      }
   } /*valid input file*/
      return(IOerr);
}
/*___openfilebyname()________________________________________________________*/

