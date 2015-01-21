/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*        MAGEOUTR.c   writeout of rotated coords in PDB format*/

#include "MAGE.h"
#define   EXTERNOUTR
#include "MAGEOUTR.h"
#include "MAGESCRT.h"

/****writeOUTRotordered()*****************************************************/
void writeOUTRotordered()
{
    /*for each residue to be outputted in order: */
    /* scan through scratch buffer*/
    /* write out scratchline that matches, then loop to get next atom*/

    int   numresidue=0, numres=0, maxres=1; /*presume at least one residue*/ 
    int   nextatom=0, numatom=0, maxatom=1; /*and at least one atom*/
    char  numstr[6]; /*numatom 5places +endstr*/
    int   Lmatchthiscycle=0, futile=0;

    while(numresidue <= maxres)/*look at all numbered residues before quitting*/
    {/*while cycles productive loop over rotated residues to be outputted*/
        numresidue++;  /*starts at 1...*/
        nextatom = 0; /*set or reset for search in current residue*/ 
        /*test all atoms before trying next residue number*/
        while(nextatom < maxatom) /*the maxatom found in scratch buffer*/
        {/*while cycles productive loop through all scratch entries*/
            nextatom++; /*augment expected atom number*/
            /*look for this atom number in the entries that has this resnum*/
            rewindtextblock(&rotoutscratch);
            Lmatchthiscycle = 0; /*new cycle through scratch buffer*/
            temps[0] = ' '; /*to get started*/
            while(temps[0] != '\0')
            {/*scan through scratch block, each line is a PDB ATOM record*/
               getonetextblockline(&rotoutscratch,temps); /*MAGESCRT.c*/
               if(temps[0] != '\0')
               {/*entries still in scratch block*/
                  /*test*/
/*
0123456789012345678901234567890
ATOM      1  O3*   I     1       1.556  -0.388   0.000  1.0  33.33
      nnnnn aaaa rrr  nnnn
*/
                  numstr[0]=temps[ 6];numstr[1]=temps[ 7];numstr[2]=temps[ 8];
                  numstr[3]=temps[ 9];numstr[4]=temps[10];numstr[5]='\0';
                  numatom = atoi(numstr);
                  if(numatom > maxatom){maxatom = numatom;}
                  numstr[0]=temps[22];numstr[1]=temps[23];numstr[2]=temps[24];
                  numstr[3]=temps[25];numstr[4]='\0',numstr[5]='\0';
                  numres = atoi(numstr);
                  if(numres > maxres){maxres = numres;}
                  if(    numres == numresidue
                      && numatom == nextatom)
                  {/*atom matches next atom of the current residue*/
                     {
                       /*replace residue name and number*/
                       /*residue name is that of the alternative*/
                       /*residue number unchanged as of 071005 */
                       temps[17]=OUTRalternate[numresidue][0]; 
                       temps[18]=OUTRalternate[numresidue][1]; 
                       temps[19]=OUTRalternate[numresidue][2]; 
                       
                       fprintf(fpout,"%s",temps);
                     }
                     Lmatchthiscycle = 1;
                     break; /*not efficient, search whole list for each atom*/
                            /*avoid block on missing atom number */
                  }/*atom matches next atom of the current residue*/
               }/*entries still in scratch block*/
            }/*scan through scratch block*/
            if(Lmatchthiscycle) {futile = 0;}
            else {futile++;}
        }/*while cycles productive loop through all scratch entries*/
    }/*while cycles productive loop over rotated residues to be outputted*/
}
/*___writeOUTRotordered()____________________________________________________*/

